#pragma once

namespace sdk {
   enum sym_magic_t {
      header = 0x4244,
      record = 0x5449
   };

   struct sym_header_t {
      std::int16_t m_magic;
      std::int16_t m_next;
      char m_path[0x1];

      [[ nodiscard ]]
      constexpr bool is_valid( ) {
         return m_magic == sym_magic_t::header;
      }
   };

   struct sym_record_t {
      std::int16_t m_magic;
      std::int32_t m_rva;
      std::int64_t m_next;

      [[ nodiscard ]]
      constexpr bool is_valid( ) {
         return m_magic == sym_magic_t::record;
      }
   };
}
