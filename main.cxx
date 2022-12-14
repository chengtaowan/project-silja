#include "sdk/sdk.hxx"

sdk::w32kbase_t* w32kbase{};
sdk::w32kfull_t* w32kfull{};
sdk::ntoskrnl_t* ntoskrnl{};

constexpr wchar_t* game_process{ L"Calculator.exe" };
constexpr wchar_t* draw_process{ L"dwm.exe" };

const void sys_init( ) {
   if ( !uti::is_supported_build( ) ) {
      uti::log( "unsupported build... exiting" );
      return;
   }

   auto draw{ uti::process_by_name( draw_process ) };
   if ( !draw ) {
      uti::log( "failed to get dwm.exe process" );
      return;
   }

   sdk::kapc_state_t apc{};
   ntoskrnl->ke_stack_attach_process( draw, &apc );

   // uti::unlink_handle
   // uti::unlink_thread
   // uti::borrow_thread

   auto game{ uti::process_by_name( game_process ) };
   if ( !game ) {
      uti::log( "failed to get game process\n" );
      return;
   }

   uti::log( "draw process %ws at 0x%llx\n", draw_process, draw );
   uti::log( "game process %ws at 0x%llx\n", game_process, game );

   for ( ;; ) {
      while ( !uti::is_valid_process( game ) )
         game = uti::process_by_name( L"Calculator.exe" );

      // ;3
   }
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

   uti::log( "w32kbase 0x%llx\n", w32kbase );
   uti::log( "w32kfull 0x%llx\n", w32kfull );
   uti::log( "ntoskrnl 0x%llx\n", ntoskrnl );

   auto thread{ ntoskrnl->ps_create_system_thread( &sys_init, 0 ) };
   if ( thread )
      ntoskrnl->zw_close( thread );
}
