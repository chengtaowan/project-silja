#include "sdk/sdk.hxx"

sdk::w32kbase_t* w32kbase{};
sdk::w32kfull_t* w32kfull{};
sdk::ntoskrnl_t* ntoskrnl{};

const void sys_init( ) {
   if ( !uti::is_supported_build( ) )
      return;

   uti::for_each_process( [ & ]( std::address_t process ) {
      // :3
   } );

   ntoskrnl->dbg_print( "* supported version :D\n" );
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
   if ( !w32kbase
     || !w32kfull
     || !ntoskrnl )
      return;

   auto thread{ ntoskrnl->ps_create_system_thread( &sys_init, 0 ) };
   if ( thread )
      ntoskrnl->zw_close( thread );
}
