#pragma once

namespace uti {
   template< std::int32_t flag, class type_t >
   [[ nodiscard ]]
   type_t read(
      auto address
   ) {
      type_t buffer{};
      auto src_addr{ ptr< std::addr_t >( address ) };
      auto dst_addr{ ptr< std::addr_t >( &buffer ) };
      if ( !src_addr || !dst_addr )
         return {};

      for ( std::size_t i{}; i < sizeof( type_t ); i++ )
         ntoskrnl->mm_copy_memory< flag >( dst_addr + i, src_addr + i, 1 );
      return buffer;
   }

   template< std::int32_t flag, class type_t >
   void write(
      auto address,
      auto buffer
   ) {
      auto src_addr{ ptr< std::addr_t >( &buffer ) };
      auto dst_addr{ ptr< std::addr_t >( address ) };
      if ( !src_addr || !dst_addr )
         return;

      for ( std::size_t i{}; i < sizeof( type_t ); i++ ) {
         if constexpr ( flag == sdk::mm_copy_flag_t::virt )
            ntoskrnl->mm_copy_memory< flag >( dst_addr + i, src_addr + i, 1 );

         if constexpr ( flag == sdk::mm_copy_flag_t::phys ) {
            auto io_space{ ntoskrnl->mm_map_io_space_ex
               < sdk::mm_prot_flag_t::page_rw >( dst_addr + i, 1 ) };
            if ( !io_space )
               return;

            ntoskrnl->mm_copy_memory< sdk::mm_copy_flag_t::virt >( io_space, src_addr + i, 1 );
            ntoskrnl->mm_unmap_io_space( io_space, 1 );
         }
      }
   }

   [[ nodiscard ]]
   std::addr_t process_by_name(
      auto process_name
   ) {
      auto init_pe{ ntoskrnl->ps_active_process_head( ) };
      if ( !init_pe )
         return {};

      auto link_va{ ptr< std::addr_t >( init_pe ) - ntoskrnl->ps_initial_system_process( ) };
      if ( !link_va )
         return {};

      for ( auto it{ init_pe->m_flink }; it != init_pe; it = it->m_flink ) {

         auto it_process{ ptr< std::addr_t >( it ) - link_va };
         if ( !it_process || !ntoskrnl->is_valid_process( it_process ) )
            continue;

         auto uni_name{ ntoskrnl->ps_query_full_process_image_name( it_process ) };
         if ( !uni_name->m_buffer
           || !uni_name->m_length
           || !uni_name->m_maximum_length )
            continue;

         if ( std::wcsstr( uni_name->m_buffer, process_name ) )
            return it_process;
      }
      return {};
   }
}
