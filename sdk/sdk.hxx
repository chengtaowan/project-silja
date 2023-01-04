#pragma once

namespace std {
   using int8_t = signed char;
   using int16_t = short;
   using int32_t = int;
   using int64_t = long long;
   using uint8_t = unsigned char;
   using uint16_t = unsigned short;
   using uint32_t = unsigned int;
   using uint64_t = unsigned long long;

   using int_least8_t = signed char;
   using int_least16_t = short;
   using int_least32_t = int;
   using int_least64_t = long long;
   using uint_least8_t = unsigned char;
   using uint_least16_t = unsigned short;
   using uint_least32_t = unsigned int;
   using uint_least64_t = unsigned long long;

   using int_fast8_t = signed char;
   using int_fast16_t = int;
   using int_fast32_t = int;
   using int_fast64_t = long long;
   using uint_fast8_t = unsigned char;
   using uint_fast16_t = unsigned int;
   using uint_fast32_t = unsigned int;
   using uint_fast64_t = unsigned long long;

   using uintptr_t = unsigned long long;
   using size_t = unsigned long long;
   using intmax_t = long long;
   using uintmax_t = long long;
   using ptrdiff_t = long long;

   using addr_t = unsigned char*;
   using double_t = double;
   using float_t = float;
}

template< class type_t >
type_t ptr( auto obj ) { return ( type_t )obj; }

#include "include/rtl/strcmp.hxx"
#include "include/rtl/strlen.hxx"
#include "include/rtl/strstr.hxx"

#include "include/type/misc.hxx"
#include "include/type/pe.hxx"

#include "include/class/win32kbase.hxx"
#include "include/class/win32kfull.hxx"
#include "include/class/ntoskrnl.hxx"

#include "include/utility.hxx"
