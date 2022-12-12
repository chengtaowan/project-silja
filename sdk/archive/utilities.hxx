#pragma once

namespace sdk {
   std::int8_t* lookup_process_by_name(
      kernel* ctx,
      const wchar_t* process_name
   ) {
      for ( std::size_t i{}; i < 0xfffffff; i++ ) {
         auto pe{ ctx->ps_lookup_process_by_process_id( i ) };
         if ( !pe || !ctx->ps_get_process_exit_status( pe ) )
            continue;

         unicode_string image_name{};
         ctx->se_locate_process_image_name( pe, &image_name );
         if ( !image_name.m_buffer
           || !image_name.m_length )
            continue;

         if ( wcsstr( image_name.m_buffer, process_name ) )
            return pe;
      }
      return {};
   }

   [[ nodiscard ]]
   section_header* first_section(
      const nt_headers* nt_hdrs
   ) {
      return reinterpret_cast< section_header* >( reinterpret_cast
         < std::uintptr_t >( nt_hdrs ) + nt_hdrs->m_size_of_optional_header + 0x18 );
   }

   [[ nodiscard ]]
   std::int8_t* rva_va(
      const std::ptrdiff_t rva,
      const dos_header* dos_hdr,
      const nt_headers* nt_hdrs
   ) {
      auto local_image = (void*)dos_hdr;
      auto p_first_sect = first_section(nt_hdrs);
      for (auto p_section{ p_first_sect }; p_section < p_first_sect + nt_hdrs->m_number_of_sections; p_section++ )
	      if (rva >= p_section->m_virtual_address && rva < p_section->m_virtual_address+p_section->m_virtual_size)
		      return (std::int8_t*)local_image + p_section->m_pointer_to_raw_data+(rva-p_section->m_virtual_address);
      return {};
   }

   const std::int8_t fix_relocs(
      std::int8_t* p_remote_img,
      std::int8_t* p_local_img
   ) {
      auto dos_hdr{ reinterpret_cast< dos_header* >( p_local_img ) };
      auto nt_hdrs{ reinterpret_cast< nt_headers* >( p_local_img + dos_hdr->m_lfanew ) };
      if ( !dos_hdr->is_valid( )
        || !nt_hdrs->is_valid( ) )
         return 0;

      struct reloc_entry {
         std::uint32_t m_to_rva;
         std::uint32_t m_size;
         struct {
            std::int16_t m_offset : 0xc;
            std::int16_t m_type : 0x4;
         } m_item[0x1];
         std::int8_t m_pad0[0x2];
      };
      
      auto delta_offset{ ( std::uintptr_t )p_remote_img - nt_hdrs->m_image_base };
      if ( !delta_offset )
         return 1;

      auto reloc_ent{ reinterpret_cast< reloc_entry* >( rva_va( nt_hdrs->m_base_relocation_table.m_addr, dos_hdr, nt_hdrs ) ) };
      auto reloc_end{ reinterpret_cast< std::uintptr_t >( reloc_ent ) + nt_hdrs->m_base_relocation_table.m_size };
      if ( !reloc_ent )
         return 1;

      while ( reinterpret_cast< std::uintptr_t >( reloc_ent ) < reloc_end && reloc_ent->m_size ) {
         auto records_count{ ( reloc_ent->m_size - sizeof( std::int8_t* ) ) >> 0x1 };
         for ( std::size_t i{}; i < records_count; i++ ) {
            auto fix_type{ reloc_ent->m_item[ i ].m_type };
            auto shift_delta{ reloc_ent->m_item[ i ].m_offset % 0x1000 };
            
            if ( fix_type == 0x3 || fix_type == 0xa ) {
               auto fix_va{ rva_va( reloc_ent->m_to_rva, dos_hdr, nt_hdrs ) };
               if ( !fix_va )
                  fix_va = reinterpret_cast< std::int8_t* >( dos_hdr );
               *reinterpret_cast< std::uintptr_t* >( fix_va + shift_delta ) += delta_offset;
            }
         }
         reloc_ent = (reloc_entry*)((std::uint8_t*)reloc_ent + reloc_ent->m_size);
      }
      return 1;
   }

   const std::int8_t inject_library(
      kernel* ctx,
      const wchar_t* library_name,
      const wchar_t* process_name
   ) {
      unicode_string lib_path{};
      object_attributes obj_attr{};

      ctx->rtl_init_unicode_string( &lib_path, library_name );
      ctx->rtl_init_object_attributes( &obj_attr, &lib_path );

      io_status_block io_status{};
      auto handle{ ctx->zw_open_file< access_t::read, option_t::sync_non_dir >( &obj_attr, &io_status ) };
      if ( !handle )
         return 0;

      file_standard_info file_info{};
      if ( !ctx->zw_query_information_file< io_file_info_t::standard >( handle, &io_status, &file_info ) ) {
         ctx->zw_close( handle );
         return 0;
      }

      auto buf_size{ static_cast< std::uint32_t >( file_info.m_end_of_file ) };
      if ( !buf_size ) {
         ctx->zw_close( handle );
         return 0;
      }

      ctx->dbg_print( "* opened %ws\n", library_name );
      ctx->dbg_print( "\tsize:\t0x%lx\n", buf_size );
      ctx->dbg_print( "\thandle:\t0x%llx\n", handle );

      auto buf{ ctx->ex_allocate_pool( buf_size ) };
      if ( !buf ) {
         ctx->zw_close( handle );
         return 0;
      }

      if ( !ctx->zw_read_file( handle, buf, &io_status, buf_size ) ) {
         ctx->ex_free_pool( buf );
         ctx->zw_close( handle );
         return 0;
      }

      ctx->dbg_print( "\n* read file contents to ram\n" );
      ctx->dbg_print( "\tbuffer: 0x%llx\n", buf );

      std::int8_t* target_pe{};
      while ( !target_pe ) {
         ctx->dbg_print( "\n* waiting for process..." );
         target_pe = lookup_process_by_name( ctx, process_name );
      }

      ctx->dbg_print( "\n\n* found target\n" );
      ctx->dbg_print( "\tpe:\t\t0x%llx\n", target_pe );
      ctx->dbg_print( "\tid:\t\t0x%llx\n", ctx->ps_get_process_id( target_pe ) );

      kernel_apc_state apc_state{};
      ctx->ke_stack_attach_process( target_pe, &apc_state );

      auto pe_buf{ ctx->zw_allocate_virtual_memory< mm_alloc_type_t::commit | mm_alloc_type_t::reserve, 
         mm_page_protect_t::rwx >( ctx->ps_get_current_process( ), buf_size ) };
      if ( !pe_buf ) {
         ctx->dbg_print( "\n* failed to allocate memory... exiting\n" );
         ctx->ke_unstack_detach_process( &apc_state );
         ctx->ex_free_pool( buf );
         ctx->zw_close( handle );
         return 0;
      }

      ctx->dbg_print( "\n* allocated memory at 0x%llx\n", pe_buf );

      auto src_addr{ const_cast< std::int8_t* >( buf ) };
      auto dst_addr{ const_cast< std::int8_t* >( pe_buf ) };

      auto status = fix_relocs( dst_addr, src_addr );
      ctx->dbg_print( "* fix_relocs: 0x%lx\n", ( int )status );

      // we should give it GetModuleHandle
      // and GetProcAddress

      //fix_import( dst_addr, src_addr );

      // fix relocations
      // fix imports
      // write sections
      // call main
      // erase discarded sec
      // cleanup our pools
      // cleanup ourselves

      ctx->ke_unstack_detach_process( &apc_state );
      ctx->ex_free_pool( buf );
      ctx->zw_close( handle );
      return 1;
   }
}
