#pragma once

namespace uti {
   [[ nodiscard ]]
   std::int8_t is_supported_build( ) {
      static auto fn_addr{ ntoskrnl->find_export( "NtBuildNumber" ) };
      if ( !fn_addr )
         return 0;

      auto nt_build{ *ptr< std::uint32_t* >( fn_addr ) & 0xffff };
      return nt_build == sdk::nt_build_t::win11_22h2
          || nt_build == sdk::nt_build_t::win11_21h1
          || nt_build == sdk::nt_build_t::win10_20h1
          || nt_build == sdk::nt_build_t::win10_19h1;
   }
}
