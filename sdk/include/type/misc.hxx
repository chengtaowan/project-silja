#pragma once

namespace sdk {
   struct list_entry_t {
      list_entry_t* m_flink;
      list_entry_t* m_blink;
   };

   struct unicode_string_t {
      std::uint16_t m_length;
      std::uint16_t m_maximum_length;
      std::int8_t m_pad0[0x4];
      wchar_t* m_buffer;
   };

   struct kapc_state_t {
      list_entry_t m_apc_list_head[0x2];
      std::address_t m_process;
      std::uint8_t m_in_progress_flags;
      std::uint8_t m_kernel_apc_pending;
      std::uint8_t m_user_apc_pending;
      std::int8_t m_pad0[0x5];
   };

   struct os_version_info_impl {
      std::uint32_t m_os_version_info_size;
      std::uint32_t m_major_version;
      std::uint32_t m_minor_version;
      std::uint32_t m_build_number;
      std::uint32_t m_platform_id;
      wchar_t m_csd_version[0x80];
   };
}
