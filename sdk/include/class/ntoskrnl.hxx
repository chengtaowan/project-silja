#pragma once

namespace sdk {
   struct ntoskrnl_t : kernel_module_t {
      [[ nodiscard ]]
      std::handle_t ps_create_system_thread(
         auto callback,
         auto argument
      ) {
         static auto fn_addr{ find_export( "PsCreateSystemThread" ) };
         if ( !fn_addr )
            return {};

         using call_t = std::int32_t(
            std::handle_t* handle,
            std::uint32_t access_mask,
            std::address_t attributes,
            std::handle_t ctx_process,
            std::address_t client_id,
            decltype( callback ),
            decltype( argument )
         );

         std::handle_t handle{};
         ptr< call_t* >( fn_addr )( &handle, 0, 0, 0, 0, callback, argument );
         return handle;
      }

      std::int8_t zw_close(
         std::handle_t handle
      ) {
         static auto fn_addr{ find_export( "ZwClose" ) };
         if ( !fn_addr )
            return 0;

         using func_t = std::int32_t( std::handle_t handle );
         return ptr< func_t* >( fn_addr )( handle ) == 0;
      }

      template< class... args_t >
      std::int8_t dbg_print(
         const char* string,
         args_t... va_args
      ) {
         static auto fn_addr{ find_export( "DbgPrintEx" ) };
         if ( !fn_addr )
            return 0;

         using func_t = std::int32_t(
            std::uint32_t flag,
            std::uint32_t level,
            const char* format,
            args_t... va_args
         );

         return ptr< func_t* >( fn_addr )( 0, 0, string, va_args... ) == 0;
      }

      std::int8_t ke_stack_attach_process(
         std::address_t process,
         kapc_state_t* apc_state
      ) {
         static auto fn_addr{ find_export( "KeStackAttachProcess" ) };
         if ( !fn_addr )
            return 0;

         using func_t = void(
            std::address_t process,
            kapc_state_t* apc_state
         );

         ptr< func_t* >( fn_addr )( process, apc_state );
         return 1;
      }

      std::int8_t ke_unstack_detach_process(
         kapc_state_t* apc_state
      ) {
         static auto fn_addr{ find_export( "KeUnstackDetachProcess" ) };
         if ( !fn_addr )
            return 0;

         using func_t = void( kapc_state_t* apc_state );
         ptr< func_t* >( fn_addr )( apc_state );
         return 1;
      }

      std::address_t ps_initial_system_process( ) {
         static auto fn_addr{ find_export( "PsInitialSystemProcess" ) };
         if ( !fn_addr )
            return {};

         return *ptr< std::address_t* >( fn_addr );
      }

      std::int32_t ps_get_process_exit_status(
         std::address_t process
      ) {
         static auto fn_addr{ find_export( "PsGetProcessExitStatus" ) };
         if ( !fn_addr )
            return 0;

         using func_t = std::int32_t( std::address_t process );
         return ptr< func_t* >( fn_addr )( process );
      }

      std::int8_t ke_delay_execution_thread(
         std::size_t* wait_interval
      ) {
         static auto fn_addr{ find_export( "KeDelayExecutionThread" ) };
         if ( !fn_addr )
            return 0;

         using func_t = std::int32_t(
            std::int8_t processor_mode,
            std::int8_t alertable_wait,
            std::size_t* wait_interval
         );

         return ptr< func_t* >( fn_addr )( 0, 0, wait_interval ) == 0;
      }

      [[ nodiscard ]]
      std::address_t ps_get_current_thread( ) {
         static auto fn_addr{ find_export( "PsGetCurrentThread" ) };
         if ( !fn_addr )
            return {};

         using func_t = std::address_t( );
         return ptr< func_t* >( fn_addr )( );
      }

      [[ nodiscard ]]
      std::address_t ps_get_thread_id(
         std::address_t thread
      ) {
         static auto fn_addr{ find_export( "PsGetThreadId" ) };
         if ( !fn_addr )
            return {};

         using func_t = std::address_t( std::address_t thread );
         return ptr< func_t* >( fn_addr )( thread );
      }

      std::int8_t ex_destroy_handle(
         std::address_t handle_table       [[ maybe_unused ]],
         std::address_t handle             [[ maybe_unused ]],
         std::address_t handle_table_entry [[ maybe_unused ]]
      ) {
         // ExSweepSingleHandle( handle_table, handle_table_entry );
         // ExpFreeHandleTableEntry( handle_table, handle, handle_table_entry );
         return 1;
      }
   };
}

extern sdk::ntoskrnl_t* ntoskrnl;
