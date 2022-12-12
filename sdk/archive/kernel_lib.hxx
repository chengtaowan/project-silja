#pragma once

namespace sdk {
   namespace impl {
      struct module_impl {
         template< class type_t >
         [[ nodiscard ]]
         type_t find_export(
            const char* exp_name
         ) {
            auto lib{ reinterpret_cast< std::int8_t* >( m_dos_header ) };
            if ( !m_dos_header->is_valid( )
              || !m_nt_headers->is_valid( ) )
               return {};

            auto exp_dir{ m_nt_headers->m_export_table.as< export_directory >( lib ) };
            if ( !exp_dir->m_address_of_functions
              || !exp_dir->m_address_of_names
              || !exp_dir->m_address_of_names_ordinals )
               return {};

            auto name{ reinterpret_cast< std::int32_t* >( lib + exp_dir->m_address_of_names ) };
            auto func{ reinterpret_cast< std::int32_t* >( lib + exp_dir->m_address_of_functions ) };
            auto ords{ reinterpret_cast< std::int16_t* >( lib + exp_dir->m_address_of_names_ordinals ) };

            for ( std::int32_t i{}; i < exp_dir->m_number_of_names; i++ ) {
               auto cur_name{ lib + name[ i ] };
               auto cur_func{ lib + func[ ords[ i ] ] };

               if ( strcmp( exp_name, reinterpret_cast< char* >( cur_name ) ) == 0 )
                  return reinterpret_cast< type_t >( cur_func );
            }

            return {};
         }

         dos_header* m_dos_header;
         nt_headers* m_nt_headers;
      };

      struct kernel_impl : module_impl {
         [[ nodiscard ]]
         const std::int8_t* ps_create_system_thread(
            const auto callback,
            const auto argument
         ) {
            using fn_t = std::int32_t(
               std::int8_t** handle,
               const std::uint32_t access,
               const std::int8_t* obj_attributes,
               const std::int8_t* parent_process,
               const std::int8_t* cilent_id,
               const decltype( callback ),
               const decltype( argument )
            );

            static auto call{ find_export< fn_t* >( "PsCreateSystemThread" ) };
            if ( !call )
               return 0;

            std::int8_t* handle{};
            call( &handle, 0, 0, 0, 0, callback, argument );
            return handle;
         }

         const std::int8_t* ps_get_current_process( ) {
            return reinterpret_cast< std::int8_t* >( -1 );
         }

         const std::int8_t ke_stack_attach_process(
            const std::int8_t* process,
            const kernel_apc_state* apc_state
         ) {
            using fn_t = void(
               const std::int8_t* process,
               const kernel_apc_state* apc_state
            );

            static auto call{ find_export< fn_t* >( "KeStackAttachProcess") };
            if ( !call )
               return 0;

            call( process, apc_state );
            return 1;
         }

         const std::int8_t ke_unstack_detach_process(
            const kernel_apc_state* apc_state
         ) {
            using fn_t = void(
               const kernel_apc_state* apc_state
            );

            static auto call{ find_export< fn_t* >( "KeUnstackDetachProcess" ) };
            if ( !call )
               return 0;

            call( apc_state );
            return 1;
         }

         const std::int8_t zw_close(
            const std::int8_t* handle
         ) {
            using fn_t = std::int32_t(
               const std::int8_t* handle
            );

            static auto call{ find_export< fn_t* >( "ZwClose" ) };
            if ( !call )
               return 0;

            return call( handle ) == 0;
         }

         template< class... args_t >
         const std::int8_t dbg_print(
            const auto msg,
            const args_t... va_args
         ) {
            using fn_t = std::int32_t(
               const std::int32_t flag,
               const std::int32_t level,
               const char* msg,
               const args_t... va_args
            );

            static auto call{ find_export< fn_t* >( "DbgPrintEx" ) };
            if ( !call )
               return 0;

            return call( 0, 0, msg, va_args... ) == 0;
         }

         template< mm_page_protect_t protect >
         [[ nodiscard ]]
         const std::int8_t* mm_map_io_space(
            const std::int8_t* addr,
            const std::size_t size
         ) {
            using fn_t = std::int8_t*(
               const std::int8_t* addr,
               const std::size_t size,
               const std::uint32_t protect
            );

            static auto call{ find_export< fn_t* >( "MmMapIoSpaceEx" ) };
            if ( !call )
               return {};

            return call( addr, size, protect );
         }

         const std::int8_t mm_unmap_io_space(
            const std::int8_t* addr,
            const std::size_t size
         ) {
            using fn_t = void(
               const std::int8_t* addr,
               const std::size_t size
            );

            static auto call{ find_export< fn_t* >( "MmUnmapIoSpace" ) };
            if ( !call )
               return 0;

            call( addr, size );
            return 1;
         }

         template< mm_copy_flag_t flag >
         const std::size_t mm_copy_memory(
            const std::int8_t* src_addr,
            const std::int8_t* dst_addr,
            const std::size_t size
         ) {
            using fn_t = std::int32_t(
               const std::int8_t* dst_addr,
               const std::int8_t* src_addr,
               const std::size_t size,
               const std::int32_t flag,
               const std::size_t* count
            );

            static auto call{ find_export< fn_t* >("MmCopyMemory") };
            if (!call)
               return 0;

            std::size_t count{};
            call( dst_addr, src_addr, size, flag, &count );
            return count;
         }

         [[ nodiscard ]]
         const os_version_info rtl_get_version( ) {
            using fn_t = std::int32_t(
               const os_version_info* info
            );
            
            static auto call{ find_export< fn_t* >( "RtlGetVersion" ) };
            if ( !call )
               return {};

            os_version_info os_info{};
            os_info.m_os_version_info_size = sizeof( os_info );

            call( &os_info );
            return os_info;
         }

         template< mm_copy_flag_t flag, class type_t >
         [[ nodiscard ]]
         type_t read(
            const auto addr
         ) {
            type_t buf{};
            auto src_addr{ reinterpret_cast< std::int8_t* >( addr ) };
            auto dst_addr{ reinterpret_cast< std::int8_t* >( &buf ) };

            for ( std::size_t i{}; i < sizeof( type_t ); i++ )
               mm_copy_memory< flag >( src_addr + i, dst_addr + i, 1u );
            return buf;
         }

         template< mm_copy_flag_t flag, class type_t >
         const std::int8_t write(
            const auto addr,
            type_t buf
         ) {
            auto src_addr{ reinterpret_cast< std::int8_t* >( &buf ) };
            auto dst_addr{ reinterpret_cast< std::int8_t* >( addr ) };

            for ( std::size_t i{}; i < sizeof( type_t ); i++ ) {
               if constexpr ( flag == mm_copy_flag_t::phys ) {
                  auto ctx{ mm_map_io_space< mm_page_protect_t::rw >( dst_addr, 1u ) };
                  if ( !ctx )
                     return 0;

                  mm_copy_memory< flag >( src_addr + i, ctx, 1u );
                  mm_unmap_io_space( ctx, 1u );
               } else
                  mm_copy_memory< flag >( src_addr + i, dst_addr + i, 1u );
            }
            return 1;
         }

         [[ nodiscard ]]
         const std::int8_t* ex_allocate_pool(
            const std::size_t size
         ) {
            using fn_t = std::int8_t* (
               const std::int32_t type,
               const std::size_t size
            );

            static auto call{ find_export< fn_t* >( "ExAllocatePool" ) };
            if ( !call )
               return {};

            return call( 0, size );
         }

         const std::int8_t ex_free_pool(
            const std::int8_t* addr
         ) {
            using fn_t = void(
               const std::int8_t* addr
            );

            static auto call{ find_export< fn_t* >( "ExFreePool" ) };
            if ( !call )
               return 0;

            call( addr );
            return 1;
         }

         const std::int8_t rtl_init_unicode_string(
            const unicode_string* buf,
            const wchar_t* wchar_str
         ) {
            using fn_t = void(
               const unicode_string* buf,
               const wchar_t* wchar_str
            );

            static auto call{ find_export< fn_t* >( "RtlInitUnicodeString" ) };
            if ( !call )
               return 0;

            call( buf, wchar_str );
            return 1;
         }

         constexpr void rtl_init_object_attributes(
            object_attributes* buf,
            unicode_string* uni_str
         ) {
            buf->m_length = sizeof( object_attributes );
            buf->m_object_name = uni_str;

            buf->m_security_quality_of_service = {};
            buf->m_security_descriptor = {};
            buf->m_root_directory = {};
            buf->m_attributes = {};
         }

         template< io_access_mask_t access, io_open_options_t open >
         const std::int8_t* zw_open_file(
            const object_attributes* attributes,
            const io_status_block* io_status
         ) {
            using fn_t = std::int32_t(
               std::int8_t** handle,
               const std::int32_t access_mask,
               const object_attributes* attributes,
               const io_status_block* io_status,
               const std::int32_t share_access,
               const std::int32_t open_option
            );

            static auto call{ find_export< fn_t* >( "ZwOpenFile" ) };
            if ( !call )
               return 0;

            std::int8_t* handle{};
            call( &handle, access, attributes, io_status, 0, open );
            return handle;
         }

         const std::int8_t zw_read_file(
            const std::int8_t* handle,
            const std::int8_t* buffer,
            const io_status_block* io_status,
            const std::uint32_t size
         ) {
            using fn_t = std::int32_t(
               const std::int8_t* handle,
               const std::int8_t* event,
               const std::int8_t* apc_routine,
               const std::int8_t* apc_context,
               const io_status_block* io_status,
               const std::int8_t* buffer,
               const std::uint32_t size,
               const std::ptrdiff_t skip,
               const std::int8_t* reserved
            );

            static auto call{ find_export< fn_t* >( "ZwReadFile" ) };
            if ( !call )
               return 0;

            return call( handle, 0, 0, 0, io_status, buffer, size, 0, 0 ) == 0;
         }

         template< io_file_info_t info_class >
         const std::int8_t zw_query_information_file(
            const std::int8_t* handle,
            const io_status_block* io_status,
            const auto file_info
         ) {
            using fn_t = std::int32_t(
               const std::int8_t* handle,
               const io_status_block* io_status,
               const decltype( file_info ),
               const std::int32_t size,
               const std::int32_t info_class
            );

            static auto call{ find_export< fn_t* >( "ZwQueryInformationFile" ) };
            if ( !call )
               return 0;

            return call( handle, io_status, file_info, 
               sizeof( decltype( file_info ) ), info_class ) == 0;
         }

         [[ nodiscard ]]
         const std::int8_t* ps_initial_system_process( ) {
            return *find_export< std::int8_t** >( "PsInitialSystemProcess" );
         }

         const std::int8_t se_locate_process_image_name(
            const std::int8_t* process,
            unicode_string* image_name
         ) {
            using fn_t = std::int32_t(
               const std::int8_t* process,
               unicode_string** image_name
            );

            static auto call{ find_export< fn_t* >( "SeLocateProcessImageName" ) };
            if ( !call )
               return 0;

            unicode_string* buffer{};
            call( process, &buffer );
            *image_name = *buffer;
            return 1;
         }

         std::int8_t* ps_lookup_process_by_process_id(
            const std::size_t process_id
         ) {
            using fn_t = std::int32_t(
               const std::size_t process_id,
               std::int8_t** process
            );

            static auto call{ find_export< fn_t* >( "PsLookupProcessByProcessId" ) };
            if ( !call )
               return {};

            std::int8_t* process{};
            call( process_id, &process );
            return process;
         }

         const std::size_t ps_get_process_id(
            const std::int8_t* process
         ) {
            using fn_t = std::size_t(
               const std::int8_t* process
            );

            static auto call{ find_export< fn_t* >( "PsGetProcessId" ) };
            if ( !call )
               return 0;

            return call( process );
         }

         const std::int8_t ps_get_process_exit_status(
            const std::int8_t* process
         ) {
            using fn_t = std::int32_t(
               const std::int8_t* process
            );

            static auto call{ find_export< fn_t* >( "PsGetProcessExitStatus" ) };
            if ( !call )
               return 0;

            return call( process ) == 0x103;
         }

         template< std::uint32_t type, std::uint32_t protect >
         const std::int8_t* zw_allocate_virtual_memory(
            const std::int8_t* process,
            const std::size_t size
         ) {
            using fn_t = std::int32_t(
               const std::int8_t* process,
               std::int8_t** address,
               const std::uintptr_t zero_bits,
               const std::size_t* size,
               const std::uint32_t type,
               const std::uint32_t protect
            );

            static auto call{ find_export< fn_t* >( "ZwAllocateVirtualMemory" ) };
            if ( !call )
               return {};

            std::int8_t* address{};
            call( process, &address, 0, &size, type, protect );
            return address;
         }

         template< std::uint32_t type >
         const std::int8_t zw_free_virtual_memory(
            const std::int8_t* process,
            const std::int8_t* address
         ) {
            using fn_t = std::int32_t(
               const std::int8_t* process,
               const std::int8_t** address,
               const std::size_t* size,
               const std::uint32_t type
            );

            static auto call{ find_export< fn_t* >( "ZwFreeVirtualMemory" ) };
            if ( !call )
               return {};

            return call( process, &address, 0, type ) == 0;
         }
      };
   }

   using module = impl::module_impl;
   using kernel = impl::kernel_impl;
}
