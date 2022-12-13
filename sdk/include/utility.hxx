#pragma once

namespace uti {
   [[ nodiscard ]]
   std::address_t find_cid_table( ) {
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
}
