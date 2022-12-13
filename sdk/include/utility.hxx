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
   std::uint32_t is_supported_build( ) {
      static auto fn_addr{ ntoskrnl->find_export( "NtBuildNumber" ) };
      if ( !fn_addr )
         return 0;

      auto nt_build{ *ptr< std::uint32_t* >( fn_addr ) & 0xffff };
      return nt_build == sdk::build_number_t::nt10_20h1
          || nt_build == sdk::build_number_t::nt11_21h2
          || nt_build == sdk::build_number_t::nt11_22h2;
   }

   void for_each_process(
      auto callback
   ) {
      // walk process list and issue callback
   }
}
