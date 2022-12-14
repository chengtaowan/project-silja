#pragma once

namespace uti {
   [[ nodiscard ]]
   std::address_t psp_cid_table( ) {
      auto fn_addr{ ntoskrnl->find_export( "PsLookupThreadByThreadId" ) };
      if ( !fn_addr )
         return {};

      while ( fn_addr[ 0 ] != 0x48
           || fn_addr[ 1 ] != 0x8b
           || fn_addr[ 2 ] != 0x05 ) 
         fn_addr++;

      auto cid_table{ *ptr< std::int32_t* >( &fn_addr[ 3 ] ) + &fn_addr[ 7 ] };
      if ( !cid_table )
         return {};

      return *ptr< std::address_t* >( cid_table );
   }

   [[ nodiscard ]]
   std::int8_t is_valid_process(
      std::address_t process
   ) {
      auto exit_code{ ntoskrnl->ps_get_process_exit_status( process ) };
      if ( !exit_code )
         return 0;
      
      return exit_code == sdk::process_status_t::still_active;
   }

   [[ nodiscard ]]
   std::uint32_t is_supported_build( ) {
      static auto fn_addr{ ntoskrnl->find_export( "NtBuildNumber" ) };
      if ( !fn_addr )
         return 0;

      auto nt_build{ *ptr< std::uint32_t* >( fn_addr ) & 0xffff };
      return nt_build == sdk::build_number_t::nt10_20h1
          || nt_build == sdk::build_number_t::nt11_21h2
          || nt_build == sdk::build_number_t::nt11_22h2;
   }

   std::int8_t wait_for_ms(
      std::size_t wait_time
   ) {
      wait_time *= -10000;
      return ntoskrnl->ke_delay_execution_thread( &wait_time );
   }
   
   template< class... args_t >
   std::int8_t log(
      const char* string,
      args_t... va_args
   ) {
      return ntoskrnl->dbg_print( "[silja] " )
          && ntoskrnl->dbg_print( string, va_args... );
   }

   [[ nodiscard ]]
   std::address_t process_by_name(
      const wchar_t* process_name
   ) {
      auto it{ ntoskrnl->ps_initial_system_process( ) };
      if ( !it )
         return {};

      constexpr std::int32_t process_links{ 0x7c0 };
      constexpr std::int32_t creation_info{ 0x5c0 };

      while ( it = *ptr< std::address_t* >( it + process_links ) - process_links, it ) {
         if ( !it )
            continue;

         auto path{ *ptr< sdk::unicode_string_t** >( it + creation_info ) };
         if ( !path
           || !path->m_length
           || !path->m_buffer )
            continue;

         if ( std::wcsstr( path->m_buffer, process_name ) )
            return it;
      }
      return {};
   }
}
