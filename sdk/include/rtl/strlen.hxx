#pragma once

namespace std {
   [[ nodiscard ]]
   size_t strlen(
      const char* string
   ) {
      size_t count{};
      while ( *string != '\0' ) string++;
      return count;
   }

   [[ nodiscard ]]
   size_t wcslen(
      const wchar_t* wstring
   ) {
      size_t count{};
      while ( *wstring != L'\0' ) wstring++;
      return count;
   }
}
