#pragma once

#include <windows.h>
#include <string>
#include <vector>
#include <cstddef>


// Wraps either a narrow or wide raw string into either a std::string or std::wstring
// Decision is made at compile time, though can be deferred until template instantiation
// This may also potentially be used if a type depends on a #define setting.
// Win32 Example: The UNICODE setting controls TCHAR, which controls LPTSTR and LPCTSTR
// Function overload selection then results in the wrapped type std::string or std::wstring
std::string WrapRawString(const char* str);
std::wstring WrapRawString(const wchar_t* str);

// Converts a wide string to a multibyte narrow string
std::string ConvertWideToNarrow(std::wstring_view inputWideString);

// Copies a std::string to a raw character buffer
// This is used to interface with C code
// Returns 0 on success
// Returns needed buffer size (including space for null terminator) if the destination buffer is too small
std::size_t CopyStdStringIntoCharBuffer(const std::string& stringToCopy, char* buffer, std::size_t bufferSize);

// Converts all characters in string lower case
std::string& ToLowerInPlace(std::string& x);

// Returns a new string where all characters have been converted to lower case
std::string ToLower(std::string x);


// Trim whitespace from both ends or either end of a string
std::string Trim(const std::string& stringToTrim, const std::string& whitespace = " \t");
std::string TrimFront(const std::string& stringToTrim, const std::string& whitespace = " \t");
std::string TrimBack(const std::string& stringToTrim, const std::string& whitespace = " \t");

// Split string on all instances of a delimiter charater and return result in a vector of strings
// The delimiter character is not included in the resulting strings
std::vector<std::string> Split(std::string stringToSplit, char delimiter);

// Split string on all instances of a delimiter character, and trim resulting strings, returning results in a vector
// The delimiter character is not included in the resulting strings
// If the delimiter character is a whitespace character, it is first used for splitting purposes
template <typename std::string(TrimFunction)(const std::string&, const std::string&) = Trim>
std::vector<std::string> SplitAndTrim(std::string stringToSplit, char delimiter, const std::string& whitespace = " \t") {
	auto items = Split(stringToSplit, delimiter);
	for (auto& item : items) {
		item = TrimFunction(item, whitespace);
	}
	return items;
}
