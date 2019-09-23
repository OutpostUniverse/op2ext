// Functions that convert into and out of c-style strings and std::strings

#pragma once

#include <windows.h>
#include <string>
#include <vector>
#include <cstddef>

// Converts a LPWSTR to std::string
bool ConvertLPWToString(std::string& stringOut, const LPWSTR pw, UINT codepage = CP_ACP);

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


// Defines how leading and trailing characters of a string are trimmed.
enum class TrimOption
{
	None,
	Trailing,
	Leading,
	Both,
};

std::vector<std::string> SplitString(std::string stringToSplit, char delimiter, TrimOption trimOption = TrimOption::Both);
std::string TrimString(const std::string& stringToTrim, TrimOption trimOption = TrimOption::Both, const std::string& whitespace = " \t");

std::string Trim(const std::string& stringToTrim, const std::string& whitespace = " \t");
std::string TrimFront(const std::string& stringToTrim, const std::string& whitespace = " \t");
std::string TrimBack(const std::string& stringToTrim, const std::string& whitespace = " \t");
