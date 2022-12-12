#pragma once

namespace sdk {
   struct non_paged_debug_info_t {
      std::uint16_t m_signature;
      std::uint16_t m_flags;
      std::uint32_t m_size;
      std::uint16_t m_machine;
      std::uint16_t m_characteristics;
      std::uint32_t m_time_date_stamp;
      std::uint32_t m_check_sum;
      std::uint32_t m_size_of_image;
      std::address_t m_image_base;
   };

   struct ldr_data_table_entry_t {
      ldr_data_table_entry_t* m_flink;
      ldr_data_table_entry_t* m_blink;
      std::address_t m_exception_table;
      std::uint32_t m_exception_table_size;
      std::int8_t m_pad0[0x4];
      std::address_t m_gp_value;
      non_paged_debug_info_t* m_non_paged_debug_info;
      std::address_t m_dll_base;
      std::address_t m_entry_point;
      std::uint32_t m_size_of_image;
      std::int8_t m_pad1[0x4];
      unicode_string_t m_full_dll_name;
      unicode_string_t m_base_dll_name;
      std::uint32_t m_flags;
      std::uint16_t m_load_count;
      std::uint16_t m_signature;
      std::address_t m_section_pointer;
      std::uint32_t m_check_sum;
      std::uint32_t m_coverage_section_size;
      std::address_t m_coverage_section;
      std::address_t m_loaded_imports;
      std::address_t m_spare;
      std::uint32_t m_size_of_image_not_rounded;
      std::uint32_t m_time_date_stamp;
   };
}
