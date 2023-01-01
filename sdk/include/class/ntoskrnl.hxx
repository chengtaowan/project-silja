#pragma once

namespace sdk {
   struct ntoskrnl_t : kernel_module_t {
      [[ nodiscard ]]
      std::handle_t ps_create_system_thread(
         auto callback
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
            std::address_t argument
         );

         std::handle_t handle{};
         ptr< call_t* >( fn_addr )( &handle, 0, 0, 0, 0, callback, 0 );
         return handle;
      }

      std::uint32_t nt_build_number( ) {
         static auto fn_addr{ find_export( "NtBuildNumber" ) };
         if ( !fn_addr )
            return 0;

         return *ptr< std::uint32_t* >( fn_addr ) & 0xffff;
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

      template< std::int32_t type >
      std::int8_t zw_query_system_information(
         std::address_t info,
         std::uint32_t info_size,
         std::size_t* out_size
      ) {
         static auto fn_addr{ find_export( "ZwQuerySystemInformation" ) };
         if ( !fn_addr )
            return 0;

         using func_t = std::int32_t(
            std::int32_t type,
            std::address_t info,
            std::int32_t info_size,
            std::size_t* out_size
         );

         return ptr< func_t* >( fn_addr )( type, info, info_size, out_size ) == 0;
      }

      template< std::int32_t type >
      std::address_t ex_allocate_pool(
         std::size_t pool_size
      ) {
         static auto fn_addr{ find_export( "ExAllocatePoolWithTag" ) };
         if ( !fn_addr )
            return 0;

         using func_t = std::address_t(
            std::int32_t type,
            std::size_t pool_size,
            std::int32_t pool_tag
         );

         return ptr< func_t* >( fn_addr )( type, pool_size, 0 );
      }

      std::int8_t ex_free_pool(
         std::address_t pool
      ) {
         static auto fn_addr{ find_export( "ExFreePoolWithTag" ) };
         if ( !fn_addr )
            return 0;

         using func_t = void(
            std::address_t pool,
            std::int32_t pool_tag
         );

         ptr< func_t* >( fn_addr )( pool, 0 );
         return 1;
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
   };
}

extern sdk::ntoskrnl_t* ntoskrnl;
