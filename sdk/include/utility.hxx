#pragma once

namespace uti {
   template< class type_t >
   type_t read(
      auto process,
      auto address
   ) {
      auto current{ ntoskrnl->ps_get_current_process( ) };
      if ( !current )
         return {};

      type_t buffer{};
      std::size_t bytes_read{};

      ntoskrnl->mm_copy_virtual_memory( process, address, current, &buffer, sizeof( type_t ), &bytes_read );
      return buffer;
   }

   template< class type_t >
   void write(
      auto process,
      auto address,
      auto buffer
   ) {
      auto current{ ntoskrnl->ps_get_current_process( ) };
      if ( !current )
         return;

      std::size_t bytes_written{};
      ntoskrnl->mm_copy_virtual_memory( current, &buffer, process, address, sizeof( type_t ), &bytes_written );
   }

   [[ nodiscard ]]
   std::addr_t get_process_ldr(
      auto process
   ) {
      static auto fn_addr{ ntoskrnl->find_export( "PsQueryProcessCommandLine" ) };
      if ( !fn_addr )
         return {};

      while ( fn_addr[0x0] != 0x48
           || fn_addr[0x1] != 0x8b
           || fn_addr[0x2] != 0x40 )
         fn_addr++;

      auto ldr_va{ *ptr< std::int8_t* >( &fn_addr[0x3] ) - sizeof( std::addr_t ) };
      if ( !ldr_va )
         return {};

      return read< std::addr_t >
         ( process, ntoskrnl->ps_get_process_peb( process ) + ldr_va );
   }

   [[ nodiscard ]]
   std::addr_t get_process_by_name(
      auto process_name
   ) {
      auto init_pe{ ntoskrnl->ps_active_process_head( ) };
      if ( !init_pe )
         return {};

      auto link_va{ ptr< std::addr_t >( init_pe ) - ntoskrnl->ps_initial_system_process( ) };
      if ( !link_va )
         return {};

      for ( auto it{ init_pe->m_flink }; it; it = it->m_flink ) {

         auto it_process{ ptr< std::addr_t >( it ) - link_va };
         if ( !it_process || !ntoskrnl->ps_is_process_status_pending( it_process ) )
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
