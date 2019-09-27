#include "StringConversion.h"
// Deprecation warning suppression must come before any standard library includes
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#include <sstream>
#include <algorithm>
#include <locale>
#include <codecvt>


std::string WrapRawString(const char* str)
{
	return std::string(str);
}
std::wstring WrapRawString(const wchar_t* str)
{
	return std::wstring(str);
}


// Define the type of converter we want for UTF-8 <=> UTF-16 conversions
// Wide strings might use `wchar_t` or `char16_t`, so specify the type we want
using ConverterType = std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>>;

std::string ConvertWideToNarrow(const std::wstring& inputString)
{
	// Create converter and convert string
	return ConverterType().to_bytes(inputString);
}

std::wstring ConvertNarrowToWide(const std::string& inputString)
{
	// Create converter and convert string
	return ConverterType().from_bytes(inputString);
}


std::size_t CopyStdStringIntoCharBuffer(const std::string& stringToCopy, char* buffer, std::size_t bufferSize)
{
	// Precheck valid pointer and non-zero buffer size to avoid wrap around or null termination problems
	if (buffer != nullptr && bufferSize > 0) {
		// Copy as much of the buffer as possible
		buffer[stringToCopy.copy(buffer, bufferSize - 1)] = 0;
		// Return success if there was sufficient room
		if (bufferSize > stringToCopy.size()) {
			return 0;
		}
	}
	// Unable to copy the whole string, so return needed buffer size
	return stringToCopy.size() + 1;
}

std::string& ToLowerInPlace(std::string& x) {
	std::transform(x.begin(), x.end(), x.begin(), ::tolower);

	return x;
}

std::string ToLower(std::string x) {
	std::transform(x.begin(), x.end(), x.begin(), ::tolower);

	return x;
}


// Trim whitespace from both ends of string, returning copy of substring
std::string Trim(const std::string& stringToTrim, const std::string& whitespace)
{
	auto stringBegin = stringToTrim.find_first_not_of(whitespace);

	if (stringBegin == std::string::npos) {
		return std::string(); // no content provided
	}

	auto stringEnd = stringToTrim.find_last_not_of(whitespace);

	const auto range = stringEnd - stringBegin + 1;
	return stringToTrim.substr(stringBegin, range);
}

// Trim whitespace from front of string, returning copy of substring
std::string TrimFront(const std::string& stringToTrim, const std::string& whitespace)
{
	auto stringBegin = stringToTrim.find_first_not_of(whitespace);

	if (stringBegin == std::string::npos) {
		return std::string(); // no content provided
	}

	return stringToTrim.substr(stringBegin, std::string::npos);
}

// Trim whitespace from back of string, returning copy of substring
std::string TrimBack(const std::string& stringToTrim, const std::string& whitespace)
{
	auto stringEnd = stringToTrim.find_last_not_of(whitespace);

	return stringToTrim.substr(0, stringEnd + 1);
}


std::vector<std::string> Split(std::string stringToSplit, char delimiter)
{
	std::vector<std::string> strings;

	std::istringstream stringStream(stringToSplit);
	std::string currentToken;

	while (std::getline(stringStream, currentToken, delimiter)) {
		strings.push_back(currentToken);
	}

	return strings;
}
