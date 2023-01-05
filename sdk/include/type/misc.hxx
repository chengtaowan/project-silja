#pragma once

namespace sdk {
   enum nt_status_t {
      success,
      alerted = 0x101,
      timeout = 0x102,
      pending = 0x103
   };
   
   enum nt_build_t {
      win11_22h2 = 0x585d,
      win11_21h1 = 0x55f0,
      win10_20h1 = 0x4a61
   };

   struct list_entry_t {
      list_entry_t* m_flink;
      list_entry_t* m_blink;
   };

   struct kapc_state_t {
      list_entry_t m_apc_list_head[0x2];
      std::addr_t m_process;
      std::uint8_t m_in_progress_flags;
      std::uint8_t m_kernel_apc_pending;
      std::uint8_t m_user_apc_pending;
   };

   struct unicode_string_t {
      std::uint16_t m_length;
      std::uint16_t m_maximum_length;
      wchar_t* m_buffer;
   };

   enum mm_copy_flag_t { phys, virt };
}
