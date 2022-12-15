#include "sdk/sdk.hxx"

sdk::w32kbase_t* w32kbase{};
sdk::w32kfull_t* w32kfull{};
sdk::ntoskrnl_t* ntoskrnl{};

const void sys_init( ) {
   if ( !uti::is_supported_build( ) ) {
      uti::log( "unsupported build... exiting" );
      return;
   }

   // uti::unlink_handle
   // uti::unlink_thread
   // uti::borrow_thread
}

const void sys_main(
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

   uti::log( "w32kbase 0x%llx\n", w32kbase );
   uti::log( "w32kfull 0x%llx\n", w32kfull );
   uti::log( "ntoskrnl 0x%llx\n", ntoskrnl );

   auto thread{ ntoskrnl->ps_create_system_thread( &sys_init, 0 ) };
   if ( thread )
      ntoskrnl->zw_close( thread );
}
