#pragma once

namespace sdk {
   struct ntoskrnl_t : kernel_module_t {
      template< class... args_t >
      std::int8_t dbg_print(
         auto format,
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

         return ptr< func_t* >( fn_addr )( 0, 0,
            format, va_args... ) == nt_status_t::success;
      }

      std::addr_t ps_get_current_process( ) {
         static auto fn_addr{ find_export( "PsGetCurrentProcess" ) };
         if ( !fn_addr )
            return {};

         using func_t = std::addr_t( );
         return ptr< func_t* >( fn_addr )( );
      }

      std::addr_t ps_get_current_thread( ) {
         static auto fn_addr{ find_export( "PsGetCurrentThread" ) };
         if ( !fn_addr )
            return {};

         using func_t = std::addr_t( );
         return ptr< func_t* >( fn_addr )( );
      }

      std::int32_t mm_copy_virtual_memory(
         auto src_process,
         auto src_address,
         auto dst_process,
         auto dst_address,
         auto size,
         auto bytes_copied
      ) {
         static auto fn_addr{ find_export( "MmCopyVirtualMemory" ) };
         if ( !fn_addr )
            return 0;

         using func_t = std::int32_t(
            std::addr_t src_process,
            decltype( src_address ),
            std::addr_t dst_process,
            decltype( dst_address ),
            std::size_t size,
            std::int8_t mode,
            std::size_t* bytes_copied
         );

         return ptr< func_t* >( fn_addr )( src_process, 
            src_address, dst_process, dst_address, size, 0, bytes_copied );
      }

      std::int8_t ps_create_system_thread(
         auto handle,
         auto start_routine,
         auto start_context
      ) {
         static auto fn_addr{ find_export( "PsCreateSystemThread" ) };
         if ( !fn_addr )
            return 0;

         using func_t = std::int32_t(
            decltype( handle ),
            std::int32_t access,
            std::addr_t object,
            std::addr_t parent,
            std::addr_t client_id,
            decltype( start_routine ), 
            decltype( start_context )
         );

         return ptr< func_t* >( fn_addr )( handle, 0, 0, 0, 0, 
            start_routine, start_context ) == nt_status_t::success;
      }

      [[ nodiscard ]]
      std::int32_t nt_build_number( ) {
         static auto fn_addr{ find_export( "KeCapturePersistentThreadState" ) };
         if ( !fn_addr )
            return 0;

         while ( fn_addr[0x0] != 0x0f
              || fn_addr[0x1] != 0xb7
              || fn_addr[0x2] != 0x05 )
            fn_addr++;

         return *ptr< std::int32_t* >
            ( &fn_addr[0x7] + *ptr< std::int32_t* >( &fn_addr[0x3] ) ) & 0xffff;
      }

      std::int8_t zw_close(
         auto handle
      ) {
         static auto fn_addr{ find_export( "ZwClose" ) };
         if ( !fn_addr )
            return 0;

         using func_t = std::int32_t( std::addr_t handle );
         return ptr< func_t* >( fn_addr )( handle ) == nt_status_t::success;
      }

      [[ nodiscard ]]
      std::addr_t ps_get_process_peb(
         auto process
      ) {
         static auto fn_addr{ find_export( "PsQueryProcessCommandLine" ) };
         if ( !fn_addr )
            return {};

         while ( fn_addr[0x0] != 0x49
              || fn_addr[0x1] != 0x8b
              || fn_addr[0x2] != 0x85 )
            fn_addr++;

         return *ptr< std::addr_t* >
            ( process + *ptr< std::int32_t* >( &fn_addr[0x3] ) );
      }

      [[ nodiscard ]]
      std::uintptr_t ps_get_process_cr3(
         auto process
      ) {
         static auto fn_addr{ find_export( "KeCapturePersistentThreadState" ) };
         if ( !fn_addr )
            return {};
         
         while ( fn_addr[0x0] != 0x48
              || fn_addr[0x1] != 0x8b 
              || fn_addr[0x2] != 0x48 )
            fn_addr++;

         return *ptr< std::uintptr_t* >
            ( process + *ptr< std::int8_t* >( &fn_addr[0x3] ) );
      }

      [[ nodiscard ]]
      std::addr_t ps_initial_system_process( ) {
         static auto fn_addr{ find_export( "PsInitialSystemProcess" ) };
         if ( !fn_addr )
            return {};

         return *ptr< std::addr_t* >( fn_addr );
      }

      [[ nodiscard ]]
      list_entry_t* ps_active_process_head( ) {
         static auto fn_addr{ find_export( "KeCapturePersistentThreadState" ) };
         if ( !fn_addr )
            return {};

         while ( fn_addr[0x0] != 0x20
              || fn_addr[0x1] != 0x48
              || fn_addr[0x2] != 0x8d )
            fn_addr++;

         return *ptr< list_entry_t** >
            ( &fn_addr[0x8] + *ptr< std::int32_t* >( &fn_addr[0x4] ) );
      }

      [[ nodiscard ]]
      std::int8_t ps_is_process_status_pending(
         auto process
      ) {
         static auto fn_addr{ find_export( "PsGetProcessExitStatus" ) };
         if ( !fn_addr )
            return 0;

         using func_t = std::int32_t( std::addr_t process );
         return ptr< func_t* >( fn_addr )( process ) == nt_status_t::pending;
      }

      std::int8_t ps_lookup_process_by_process_id(
         auto process_id,
         auto process
      ) {
         static auto fn_addr{ find_export( "PsLookupProcessByProcessId" ) };
         if ( !fn_addr )
            return 0;

         using func_t = std::int32_t(
            std::size_t process_id,
            std::addr_t* process
         );

         return ptr< func_t* >( fn_addr )
            ( process_id, process ) == nt_status_t::success;
      }

      [[ nodiscard ]]
      unicode_string_t* ps_query_full_process_image_name(
         std::addr_t process
      ) {
         static auto fn_addr{ find_export( "SeLocateProcessImageName" ) };
         if ( !fn_addr )
            return {};

         while ( fn_addr[0x0] != 0xec
              || fn_addr[0x1] != 0x28
              || fn_addr[0x2] != 0xe8 )
            fn_addr++;

         auto ps_rva{ &fn_addr[0x7] + *ptr< std::int32_t* >( &fn_addr[0x3] ) };
         if ( !ps_rva )
            return {};

         while ( ps_rva[0x0] != 0x0f
              || ps_rva[0x1] != 0x85
              || ps_rva[0x6] != 0x48 )
            ps_rva++;

         return *ptr< unicode_string_t** >
            ( process + *ptr< std::int32_t* >( &ps_rva[0x9] ) );
      }

      void ke_stack_attach_process(
         auto process,
         auto apc_state
      ) {
         static auto fn_addr{ find_export( "KeStackAttachProcess" ) };
         if ( !fn_addr )
            return;

         using func_t = void(
            std::addr_t process,
            apc_state_t* apc_state
         );

         ptr< func_t* >( fn_addr )( process, apc_state );
      }

      //MmCopyVirtualMemory(
      //   IN PEPROCESS FromProcess,
      //   IN CONST VOID* FromAddress,
      //   IN PEPROCESS ToProcess,
      //   OUT PVOID ToAddress,
      //   IN SIZE_T BufferSize,
      //   IN KPROCESSOR_MODE PreviousMode,
      //   OUT PSIZE_T NumberOfBytesCopied
      //)

      //[[ nodiscard ]]
      //std::addr_t psp_cid_table( ) {
      //   static auto fn_addr{ find_export( "PsLookupProcessByProcessId" ) };
      //   if ( !fn_addr )
      //      return {};

      //   while ( fn_addr[0x0] != 0xe8 
      //        || fn_addr[0x5] != 0x48
      //        || fn_addr[0x6] != 0x8b )
      //      fn_addr++;

      //   auto psp_rva{ &fn_addr[0x5] + *ptr< std::int32_t* >( &fn_addr[0x1] ) };
      //   if ( !psp_rva )
      //      return {};

      //   while ( psp_rva[0x0] != 0x48
      //        || psp_rva[0x1] != 0x8b
      //        || psp_rva[0x2] != 0x05 )
      //      psp_rva++;

      //   auto cid_table{ &psp_rva[0x7] + *ptr< std::int32_t* >( &psp_rva[0x3] ) };
      //   if ( !cid_table )
      //      return {};

      //   return *ptr< std::addr_t* >( cid_table );
      //}

      //std::addr_t exp_lookup_handle_table_entry(
      //   std::addr_t handle_table,
      //   std::size_t handle_cid
      //) {
      //   static auto fn_addr{ find_export( "ExEnumHandleTable" ) };
      //   if ( !fn_addr )
      //      return {};

      //   while ( fn_addr[0x0] != 0x8b 
      //        || fn_addr[0x1] != 0xd1
      //        || fn_addr[0x2] != 0xe8 )
      //      fn_addr++;

      //   auto exp_rva{ &fn_addr[0x7] + *ptr< std::int32_t* >( &fn_addr[0x3] ) };
      //   if ( !exp_rva )
      //      return {};

      //   using func_t = std::addr_t(
      //      std::addr_t handle_table,
      //      std::size_t handle_cid
      //   );

      //   return ptr< func_t* >( exp_rva )( handle_table, handle_cid );
      //}

      //std::addr_t exp_get_handle_pointer(
      //   std::addr_t handle
      //) {
      //   static auto fn_addr{ find_export( "NtWriteFile" ) };
      //   if ( !fn_addr )
      //      return {};

      //   while ( fn_addr[0x0] != 0x4d 
      //        || fn_addr[0x1] != 0xe7
      //        || fn_addr[0x2] != 0xe8 )
      //      fn_addr++;

      //   auto exp_rva{ &fn_addr[0x7] + *ptr< std::int32_t* >( &fn_addr[0x3] ) };
      //   if ( !exp_rva )
      //      return {};

      //   using func_t = std::addr_t( std::addr_t handle );
      //   return ptr< func_t* >( exp_rva )( handle );
      //}
   };
}

extern sdk::ntoskrnl_t* ntoskrnl;
