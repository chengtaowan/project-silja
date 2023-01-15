#include "sdk/sdk.hxx"

sdk::w32kbase_t* w32kbase{};
sdk::w32kfull_t* w32kfull{};
sdk::ntoskrnl_t* ntoskrnl{};

constexpr wchar_t* game_name{ L"ProcessHacker.exe" };
constexpr wchar_t* base_name{ L"ProcessHacker.exe" };

void sys_init( ) {
   //std::addr_t game{};
   //std::addr_t base{};

   //while ( !game ) game = uti::get_process_by_name( game_name );
   //while ( !base ) base = uti::get_module_by_name( game, base_name );

   //ntoskrnl->dbg_print( "[silja] game 0x%llx\n", game );
   //ntoskrnl->dbg_print( "[silja] base 0x%llx\n", base );

   //ntoskrnl->dbg_print( "[silja] read %lx\n", uti::read< short >( game, base ) );
}

void sys_main(
   std::addr_t* imports
) {
   static std::addr_t copy[] = {
      imports[0x0], // win32kbase.sys
      imports[0x1], // win32kfull.sys
      imports[0x2]  // ntoskrnl.exe
   };

   w32kbase = ptr< sdk::w32kbase_t* >( &copy[0x0] );
   w32kfull = ptr< sdk::w32kfull_t* >( &copy[0x1] );
   ntoskrnl = ptr< sdk::ntoskrnl_t* >( &copy[0x2] );

   ntoskrnl->dbg_print( "[silja] w32kbase 0x%llx\n", w32kbase );
   ntoskrnl->dbg_print( "[silja] w32kfull 0x%llx\n", w32kfull );
   ntoskrnl->dbg_print( "[silja] ntoskrnl 0x%llx\n", ntoskrnl );

   if ( ntoskrnl->nt_build_number( ) != sdk::nt_build_t::win11_22h2
     && ntoskrnl->nt_build_number( ) != sdk::nt_build_t::win11_21h1
     && ntoskrnl->nt_build_number( ) != sdk::nt_build_t::win10_20h1 )
      return;

   std::addr_t init_thread{};
   ntoskrnl->ps_create_system_thread( &init_thread, &sys_init, 0 );
   if ( init_thread )
      ntoskrnl->zw_close( init_thread );
}
