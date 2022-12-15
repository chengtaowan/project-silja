#pragma once

namespace sdk {
   enum sym_magic_t {
      symbol = 0x4244,
      record = 0x5449
   };

   enum sym_type_t {
      global,
      member,
      function
   };

   struct sym_header_t {
      std::int16_t m_magic;
      std::int16_t m_count;
      std::int16_t m_next;
      char m_version[0x1];

      [[ nodiscard ]]
      constexpr bool is_valid( ) {
         return m_magic == sym_magic_t::symbol;
      }
   };

   struct sym_record_t {
      std::int16_t m_magic;
      std::int8_t m_type;
      std::int32_t m_rva;
      std::int16_t m_next;
      char m_name[0x1];

      [[ nodiscard ]]
      constexpr bool is_valid( ) {
         return m_magic == sym_magic_t::record;
      }
   };
}
