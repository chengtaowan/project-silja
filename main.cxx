#include "sdk/sdk.hxx"

sdk::w32kbase_t* w32kbase{};
sdk::w32kfull_t* w32kfull{};
sdk::ntoskrnl_t* ntoskrnl{};

void sys_draw( ) {
   auto dwm{ uti::process_by_name( L"dwm.exe" ) };
   if ( !dwm )
      return;

   sdk::kapc_state_t apc{};
   ntoskrnl->ke_stack_attach_process( dwm, &apc );

   ntoskrnl->dbg_print( "[silja] dwm.exe 0x%llx\n", dwm );

   ntoskrnl->ke_unstack_detach_process( &apc );
}

void sys_read( ) { /* ;3 */ }

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

   std::addr_t draw_thread{};
   std::addr_t read_thread{};

   ntoskrnl->ps_create_system_thread( &draw_thread, &sys_draw, 0 );
   ntoskrnl->ps_create_system_thread( &read_thread, &sys_read, 0 );
   
   if ( draw_thread ) ntoskrnl->zw_close( draw_thread );
   if ( read_thread ) ntoskrnl->zw_close( read_thread );
}
