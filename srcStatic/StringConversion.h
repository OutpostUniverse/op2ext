// Functions that convert into and out of c-style strings and std::strings

#pragma once

#include <windows.h>
#include <string>
#include <vector>
#include <cstddef>


// Converts a LPWSTR to std::string
std::string ConvertWideToNarrow(std::wstring_view inputWideString, UINT codepage = CP_ACP);

// Convert a LPCTSTR to std::string
// At compile time LPCTSTR will convert to either LPCSTR or LPCWSTR based on the UNICODE setting
std::string ConvertLpctstrToString(LPCSTR str);
std::wstring ConvertLpctstrToString(LPCWSTR str);

// Returns 0 on success
// Returns needed buffer size (including space for null terminator) if the destination buffer is too small
std::size_t CopyStdStringIntoCharBuffer(const std::string& stringToCopy, char* buffer, std::size_t bufferSize);

// Converts all characters in string lower case
std::string& ToLowerInPlace(std::string& x);

// Returns a new string where all characters have been converted to lower case
std::string ToLower(std::string x);


std::string Trim(const std::string& stringToTrim, const std::string& whitespace = " \t");
std::string TrimFront(const std::string& stringToTrim, const std::string& whitespace = " \t");
std::string TrimBack(const std::string& stringToTrim, const std::string& whitespace = " \t");

std::vector<std::string> Split(std::string stringToSplit, char delimiter);

template <typename std::string(TrimFunction)(const std::string&, const std::string&) = Trim>
std::vector<std::string> SplitAndTrim(std::string stringToSplit, char delimiter, const std::string& whitespace = " \t") {
	auto items = Split(stringToSplit, delimiter);
	for (auto& item : items) {
		item = TrimFunction(item, whitespace);
	}
	return items;
}
