#pragma once

namespace sdk {
   enum io_open_options_t {
      sync_dir = 0x21,
      sync_non_dir = 0x60
   };
   
   enum io_access_mask_t {
      read = 0x1,
      write
   };

   enum io_file_info_t {
      directory = 0x1,
      standard = 0x5
   };

   namespace impl {
      struct file_directory_info_impl {
         std::uint32_t m_next_entry_offset;
         std::uint32_t m_file_index;
         std::uint64_t m_creation_time;
         std::uint64_t m_last_access_time;
         std::uint64_t m_last_write_time;
         std::uint64_t m_change_time;
         std::size_t m_end_of_file;
         std::size_t m_allocation_size;
         std::uint32_t m_file_attributes;
         std::uint32_t m_filename_length;
         wchar_t m_filename[0x1];
         std::int8_t m_pad[0x6];
      };

      struct file_standard_info_impl {
         std::size_t m_allocation_size;
         std::size_t m_end_of_file;
         std::uint32_t m_number_of_links;
         std::uint8_t m_delete_pending;
         std::uint8_t m_directory;
         std::int8_t m_pad0[0x2];
      };

      struct object_attributes_impl {
         std::uint32_t m_length;
         std::int8_t m_pad0[0x4];
         std::int8_t* m_root_directory;
         unicode_string_impl* m_object_name;
         std::uint32_t m_attributes;
         std::int8_t m_pad1[0x4];
         std::int8_t* m_security_descriptor;
         std::int8_t* m_security_quality_of_service;
      };

      struct io_status_block_impl {
         union {
            std::int32_t m_status;
            std::int8_t* m_pointer;
         };
         std::uint64_t m_information;
      };
   }

   using object_attributes = impl::object_attributes_impl;
   using io_status_block = impl::io_status_block_impl;
   using file_directory_info = impl::file_directory_info_impl;
   using file_standard_info = impl::file_standard_info_impl;

   using option_t = io_open_options_t;
   using access_t = io_access_mask_t;
}
