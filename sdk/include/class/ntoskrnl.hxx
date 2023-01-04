#pragma once

namespace sdk {
   struct ntoskrnl_t : kernel_module_t {
      template< class... args_t >
      nt_status_t dbg_print(
         auto format,
         args_t... va_args
      ) {
         static auto fn_addr{ find_export( "DbgPrintEx" ) };
         if ( !fn_addr )
            return nt_status_t::unsuccess;

         using func_t = nt_status_t(
            std::uint32_t flag,
            std::uint32_t level,
            const char* format,
            args_t... va_args
         );

         return ptr< func_t* >( fn_addr )( 0, 0, format, va_args... );
      }

      nt_status_t ps_create_system_thread(
         auto handle,
         auto start_routine,
         auto start_context
      ) {
         static auto fn_addr{ find_export( "PsCreateSystemThread" ) };
         if ( !fn_addr )
            return nt_status_t::unsuccess;

         using func_t = nt_status_t(
            decltype( handle ),
            std::int32_t access,
            std::addr_t object,
            std::addr_t parent,
            std::addr_t client_id,
            decltype( start_routine ), 
            decltype( start_context )
         );

         return ptr< func_t* >( fn_addr )
            ( handle, 0, 0, 0, 0, start_routine, start_context );
      }

      std::int32_t nt_build_number( ) {
         static auto fn_addr{ find_export( "NtBuildNumber" ) };
         if ( !fn_addr )
            return 0;

         return *ptr< std::int32_t* >( fn_addr ) & 0xffff;
      }

      nt_status_t zw_close(
         auto handle
      ) {
         static auto fn_addr{ find_export( "ZwClose" ) };
         if ( !fn_addr )
            return nt_status_t::unsuccess;

         using func_t = nt_status_t( std::addr_t );
         return ptr< func_t* >( fn_addr )( handle );
      }

      nt_status_t ke_stack_attach_process(
         auto process,
         auto apc_state
      ) {
         static auto fn_addr{ find_export( "KeStackAttachProcess" ) };
         if ( !fn_addr )
            return nt_status_t::unsuccess;

         using func_t = void(
            std::addr_t process,
            kapc_state_t* apc_state
         );

         ptr< func_t* >( fn_addr )( process, apc_state );
         return nt_status_t::success;
      }

      nt_status_t ke_unstack_detach_process(
         auto apc_state
      ) {
         static auto fn_addr{ find_export( "KeUnstackDetachProcess" ) };
         if ( !fn_addr )
            return nt_status_t::unsuccess;

         using func_t = void( kapc_state_t* );
         ptr< func_t* >( fn_addr )( apc_state );

         return nt_status_t::success;
      }

      std::addr_t ps_initial_system_process( ) {
         static auto fn_addr{ find_export( "PsInitialSystemProcess" ) };
         if ( !fn_addr )
            return {};

         return *ptr< std::addr_t* >( fn_addr );
      }

      nt_status_t ps_get_process_exit_status(
         auto process
      ) {
         static auto fn_addr{ find_export( "PsGetProcessExitStatus" ) };
         if ( !fn_addr )
            return nt_status_t::unsuccess;

         using func_t = nt_status_t( std::addr_t );
         return ptr< func_t* >( fn_addr )( process );
      }

      nt_status_t ps_lookup_process_by_process_id(
         auto process_id,
         auto process
      ) {
         static auto fn_addr{ find_export( "PsLookupProcessByProcessId" ) };
         if ( !fn_addr )
            return {};

         using func_t = nt_status_t(
            std::size_t process_id,
            std::addr_t* process
         );

         return ptr< func_t* >( fn_addr )( process_id, process );
      }

      [[ nodiscard ]]
      std::addr_t psp_cid_table( ) {
         static auto fn_addr{ find_export( "PsLookupProcessByProcessId" ) };
         if ( !fn_addr )
            return {};

         while ( fn_addr[ 0x0 ] != 0xe8 
              || fn_addr[ 0x5 ] != 0x48
              || fn_addr[ 0x6 ] != 0x8b )
            fn_addr++;

         auto psp_rva{ &fn_addr[ 0x5 ] + *ptr< std::int32_t* >( &fn_addr[ 0x1 ] ) };
         if ( !psp_rva )
            return {};

         while ( psp_rva[ 0x0 ] != 0x48
              || psp_rva[ 0x1 ] != 0x8b
              || psp_rva[ 0x2 ] != 0x05 )
            psp_rva++;

         auto cid_table{ &psp_rva[ 0x7 ] + *ptr< std::int32_t* >( &psp_rva[ 0x3 ] ) };
         if ( !cid_table )
            return {};

         return *ptr< std::addr_t* >( cid_table );
      }

      unicode_string_t* ps_query_full_process_image_name(
         std::addr_t process
      ) {
         static auto fn_addr{ find_export( "SeLocateProcessImageName" ) };
         if ( !fn_addr )
            return {};

         while ( fn_addr[ 0x0 ] != 0xec
              || fn_addr[ 0x1 ] != 0x28
              || fn_addr[ 0x2 ] != 0xe8 )
            fn_addr++;

         auto ps_rva{ &fn_addr[ 0x7 ] + *ptr< std::int32_t* >( &fn_addr[ 0x3 ] ) };
         if ( !ps_rva )
            return {};

         while ( ps_rva[ 0x0 ] != 0x0f
              || ps_rva[ 0x1 ] != 0x85
              || ps_rva[ 0x6 ] != 0x48 )
            ps_rva++;

         return *ptr< unicode_string_t** >( process + *ptr< std::int32_t* >( &ps_rva[ 0x9 ] ) );
      }

      std::addr_t exp_lookup_handle_table_entry(
         std::addr_t handle_table,
         std::size_t handle_cid
      ) {
         static auto fn_addr{ find_export( "ExEnumHandleTable" ) };
         if ( !fn_addr )
            return {};

         while ( fn_addr[ 0x0 ] != 0x8b 
              || fn_addr[ 0x1 ] != 0xd1
              || fn_addr[ 0x2 ] != 0xe8 )
            fn_addr++;

         auto exp_rva{ &fn_addr[ 0x7 ] + *ptr< std::int32_t* >( &fn_addr[ 0x3 ] ) };
         if ( !exp_rva )
            return {};

         using func_t = std::addr_t(
            std::addr_t handle_table,
            std::size_t handle_cid
         );

         return ptr< func_t* >( exp_rva )( handle_table, handle_cid );
      }

      std::addr_t exp_get_handle_pointer(
         std::addr_t handle
      ) {
         static auto fn_addr{ find_export( "NtWriteFile" ) };
         if ( !fn_addr )
            return {};

         while ( fn_addr[ 0x0 ] != 0x4d 
              || fn_addr[ 0x1 ] != 0xe7
              || fn_addr[ 0x2 ] != 0xe8 )
            fn_addr++;

         auto exp_rva{ &fn_addr[ 0x7 ] + *ptr< std::int32_t* >( &fn_addr[ 0x3 ] ) };
         if ( !exp_rva )
            return {};

         using func_t = std::addr_t( std::addr_t handle );
         return ptr< func_t* >( exp_rva )( handle );
      }
   };
}

extern sdk::ntoskrnl_t* ntoskrnl;
