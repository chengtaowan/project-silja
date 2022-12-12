#pragma once

namespace sdk {
   namespace impl {
      enum hash_const_t : const std::size_t {
         prime = 0x100000001b3,
         basis = 0xcbf29ce484222325
      };

      [[ nodiscard ]]
      constexpr std::size_t fnv_1a(
         const char* char_str,
         const std::size_t size
      ) {
         std::size_t hash{ hash_const_t::basis };

         for ( std::size_t i{}; i < size; i++ ) {
            hash ^= char_str[ i ];
            hash *= hash_const_t::prime;
         }

         return hash;
      }

      [[ nodiscard ]]
      constexpr std::size_t fnv_1a(
         const wchar_t* wchar_str,
         const std::size_t size
      ) {
         std::size_t hash{ hash_const_t::basis };

         while ( *wchar_str != L'\0' ) {
            hash ^= *wchar_str;
            hash *= hash_const_t::prime;

            wchar_str++;
         }

         return hash;
      }
   }

   template< class type_t >
   [[ nodiscard ]]
   constexpr std::size_t fnv_1a(
      const type_t string
   ) {
      return impl::fnv_1a( string );
   }

   template< class type_t >
   [[ nodiscard ]]
   const std::size_t fnv_1a(
      const type_t string
   ) {
      std::size_t hash{ impl::fnv_1a( string ) };
      return hash;
   }
}
