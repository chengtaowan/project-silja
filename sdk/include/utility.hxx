#pragma once

namespace uti {
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
}
