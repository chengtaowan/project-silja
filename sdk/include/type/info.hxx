#pragma once

namespace sdk {
   enum info_class_t {
      system_basic = 0x0,
      system_process = 0x5,
      system_module = 0xb
   };

   enum pool_type_t {
      nonpaged = 0x0,
      paged = 0x1,
      nonpaged_session = 0x20,
      page_session = 0x21
   };

   struct io_counters_t {
      std::size_t m_read_operation_count;
      std::size_t m_write_operation_count;
      std::size_t m_other_operation_count;
      std::size_t m_read_transfer_count;
      std::size_t m_write_transfer_count;
      std::size_t m_other_transfer_count;
   };

   struct vm_counters_t {
      std::size_t m_peak_virtual_size;
      std::size_t m_virtual_size;
      std::uint32_t m_page_fault_count;
      std::size_t m_peak_working_set_size;
      std::size_t m_working_set_size;
      std::size_t m_quota_peak_paged_pool_usage;
      std::size_t m_quota_paged_pool_usage;
      std::size_t m_quota_peak_nonpaged_pool_usage;
      std::size_t m_quota_nonpaged_pool_usage;
      std::size_t m_page_file_usage;
      std::size_t m_peak_page_file_usage;
      std::size_t m_private_page_count;
   };

   struct system_thread_t {
      std::uint64_t m_kernel_time;
      std::uint64_t m_user_time;
      std::uint64_t m_create_time;
      std::uint32_t m_wait_time;
      std::address_t m_start_address;
      std::address_t m_unique_process_id;
      std::address_t m_unique_thread_id;
      std::int32_t m_priority;
      std::int32_t m_base_priority;
      std::uint32_t m_context_switch_count;
      std::int32_t m_state;
      std::int32_t m_wait_reason;
   };

   struct system_process_t {
      std::uint32_t m_next_entry_delta;
      std::uint32_t m_thread_count;
      std::uint32_t m_res0[0x6];
      std::uint64_t m_create_time;
      std::uint64_t m_user_time;
      std::uint64_t m_kernel_time;
      unicode_string_t m_process_name;
      std::int32_t m_base_priority;
      std::size_t m_process_id;
      std::size_t m_inherited_from_process_id;
      std::uint32_t m_handle_count;
      std::uint32_t m_res1[0x2];
      vm_counters_t m_vm_counters;
      io_counters_t m_io_counters;
      system_thread_t m_threads[0x1];
   };
}
