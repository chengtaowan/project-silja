#pragma once

namespace sdk {
   enum mm_page_priority_t {
      min,
      mid = 0x10,
      max = 0x20
   };

   enum mm_page_protect_t {
      r = 0x2,
      rw = 0x4,
      x = 0x10,
      rx = 0x20,
      rwx = 0x40
   };

   enum mm_alloc_type_t {
      commit = 0x1000,
      reserve = 0x2000,
      decommit = 0x4000,
      release = 0x8000
   };

   enum mm_copy_flag_t {
      phys = 0x1,
      virt
   };

   namespace impl {
      struct os_version_info_impl {
         std::uint32_t m_os_version_info_size;
         std::uint32_t m_major_version;
         std::uint32_t m_minor_version;
         std::uint32_t m_build_number;
         std::uint32_t m_platform_id;
         wchar_t m_csd_version[0x80];
      };

      struct list_entry_impl {
         list_entry_impl* m_flink;
         list_entry_impl* m_blink;
      };

      struct unicode_string_impl {
         std::uint16_t m_length;
         std::uint16_t m_maximum_length;
         std::int8_t m_pad0[0x4];
         wchar_t* m_buffer;
      };

      struct kernel_apc_state_impl {
         list_entry_impl m_apc_list_head[0x2];
         std::int8_t* m_process;
         std::uint8_t m_kernel_apc_in_progress;
         std::uint8_t m_kernel_apc_pending;
         std::uint8_t m_user_apc_pending;
         std::int8_t m_pad0[0x5];
      };
   }

   using os_version_info = impl::os_version_info_impl;
   using list_entry = impl::list_entry_impl;
   using unicode_string = impl::unicode_string_impl;
   using kernel_apc_state = impl::kernel_apc_state_impl;
}
