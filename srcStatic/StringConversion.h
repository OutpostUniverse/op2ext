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


constexpr std::string_view Whitespace = " \t";

// Trim whitespace from both ends or either end of a string
std::string Trim(const std::string& stringToTrim, std::string_view whitespace = Whitespace);
std::string TrimFront(const std::string& stringToTrim, std::string_view whitespace = Whitespace);
std::string TrimBack(const std::string& stringToTrim, std::string_view whitespace = Whitespace);

// Split string on all instances of a delimiter charater and return result in a vector of strings
// The delimiter character is not included in the resulting strings
std::vector<std::string> Split(const std::string& stringToSplit, char delimiter);

// Split string on all instances of a delimiter character, and trim resulting strings, returning results in a vector
// The delimiter character is not included in the resulting strings
// If the delimiter character is a whitespace character, it is first used for splitting purposes
template <typename std::string(TrimFunction)(const std::string&, std::string_view) = Trim>
std::vector<std::string> SplitAndTrim(std::string stringToSplit, char delimiter, std::string_view whitespace = Whitespace) {
	auto items = Split(stringToSplit, delimiter);
	for (auto& item : items) {
		item = TrimFunction(item, whitespace);
	}
	return items;
}

// Parses a list of comma separated values
// Whitespace between elements is stripped
std::vector<std::string> ParseCsv(const std::string& csv, char delimiter = ',', std::string_view whitespace = Whitespace);

// Convert hex address value to string
std::string AddrToHexString(std::size_t addr);


// Gets UTC date/time string using the system clock
std::string GetDateTime();
