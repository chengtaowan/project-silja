#include "sdk/sdk.hxx"

namespace ctx {
   sdk::w32kbase_t* w32kbase{};
   sdk::w32kfull_t* w32kfull{};
   sdk::ntoskrnl_t* ntoskrnl{};
}

const void sys_init( ) {
   // :3
}

const void sys_main(
   const std::address_t* data
) {
   static std::address_t copy[ ] = {
      data[ 0 ], // win32kbase.sys
      data[ 1 ], // win32kfull.sys
      data[ 2 ]  // ntoskrnl.exe
   };

   ctx::w32kbase = ptr< sdk::w32kbase_t* >( &copy[ 0 ] );
   ctx::w32kfull = ptr< sdk::w32kfull_t* >( &copy[ 1 ] );
   ctx::ntoskrnl = ptr< sdk::ntoskrnl_t* >( &copy[ 2 ] );
   if ( !ctx::w32kbase
     || !ctx::w32kfull
     || !ctx::ntoskrnl )
      return;

   auto thread{ ctx::ntoskrnl->ps_create_system_thread( &sys_init, 0 ) };
   if ( thread )
      ctx::ntoskrnl->zw_close( thread );
}
