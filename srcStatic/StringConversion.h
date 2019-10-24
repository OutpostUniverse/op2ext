#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <cstddef>


// Wraps either a narrow or wide raw string into either a std::string or std::wstring
// Decision is made at compile time, though can be deferred until template instantiation
// This may also potentially be used if a type depends on a #define setting.
// Win32 Example: The UNICODE setting controls TCHAR, which controls LPTSTR and LPCTSTR
// Function overload selection then results in the wrapped type std::string or std::wstring
std::string WrapRawString(const char* str);
std::wstring WrapRawString(const wchar_t* str);

// Convert between wide strings and multibyte narrow strings
std::string ConvertWideToNarrow(const std::wstring& inputWideString);
std::wstring ConvertNarrowToWide(const std::string& inputNarrowString);

// Copies a std::string_view into a raw character buffer
// This is used to interface with C code
// Returns 0 on success
// Returns needed buffer size (including space for null terminator) if the destination buffer is too small
std::size_t CopyStringViewIntoCharBuffer(std::string_view stringToCopy, char* buffer, std::size_t bufferSize);

// Converts all characters in string lower case
std::string& ToLowerInPlace(std::string& x);

// Returns a new string where all characters have been converted to lower case
std::string ToLower(std::string x);


// Convert hex address value to string
std::string AddrToHexString(std::size_t addr);


// Gets UTC date/time string using the system clock
std::string GetDateTime();
