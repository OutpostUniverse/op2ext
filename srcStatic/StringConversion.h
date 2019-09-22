// Functions that convert into and out of c-style strings and std::strings

#pragma once

#include <windows.h>
#include <string>
#include <vector>
#include <cstddef>

// Converts a LPWSTR to std::string
bool ConvertLPWToString(std::string& stringOut, const LPWSTR pw, UINT codepage = CP_ACP);

// Convert a LPCTSTR to std::string. Throws an exception if code is compiled with Unicode character set.
std::string ConvertLpctstrToString(LPCSTR str);

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
	Trailing,
	Leading,
	Both,
	None
};

std::vector<std::string> SplitString(std::string stringToSplit, char delimiter, TrimOption trimOption = TrimOption::Both);
std::string TrimString(const std::string& stringToTrim, TrimOption trimOption = TrimOption::Both, const std::string& whitespace = " \t");
