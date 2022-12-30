#pragma once

namespace sdk {
   enum nt_build_t {
      win11_22h2 = 0x585d,
      win11_21h1 = 0x55f0,
      win10_20h1 = 0x4a61
   };

   struct list_entry_t {
      list_entry_t* m_flink;
      list_entry_t* m_blink;
   };

   struct unicode_string_t {
      std::uint16_t m_length;
      std::uint16_t m_maximum_length;
      wchar_t* m_buffer;
   };

   struct kapc_state_t {
      list_entry_t m_apc_list_head[0x2];
      std::address_t m_process;
      std::uint8_t m_in_progress_flags;
      std::uint8_t m_kernel_apc_pending;
      std::uint8_t m_user_apc_pending;
   };
}
