#include "sdk/sdk.hxx"

sdk::w32kbase_t* w32kbase{};
sdk::w32kfull_t* w32kfull{};
sdk::ntoskrnl_t* ntoskrnl{};

void sys_init( ) {
   if ( !uti::is_supported_build( ) ) {
      ntoskrnl->dbg_print( "[silja] unsupported build... exiting" );
      return;
   }

   ntoskrnl->dbg_print( "[silja] hello world!" );

   // uti::unlink_handle
   // uti::unlink_thread
   // uti::borrow_thread
}

void sys_main(
   const std::address_t* data
) {
   static std::address_t copy[ ] = {
      data[ 0 ], // win32kbase.sys
      data[ 1 ], // win32kfull.sys
      data[ 2 ]  // ntoskrnl.exe
   };

   w32kbase = ptr< sdk::w32kbase_t* >( &copy[ 0 ] );
   w32kfull = ptr< sdk::w32kfull_t* >( &copy[ 1 ] );
   ntoskrnl = ptr< sdk::ntoskrnl_t* >( &copy[ 2 ] );

   ntoskrnl->dbg_print( "[silja] w32kbase 0x%llx\n", w32kbase );
   ntoskrnl->dbg_print( "[silja] w32kfull 0x%llx\n", w32kfull );
   ntoskrnl->dbg_print( "[silja] ntoskrnl 0x%llx\n", ntoskrnl );

   auto thread{ ntoskrnl->ps_create_system_thread( &sys_init, 0 ) };
   if ( thread )
      ntoskrnl->zw_close( thread );
}
