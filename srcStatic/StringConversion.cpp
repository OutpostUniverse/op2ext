// Deprecation warning suppression must come before any standard library includes
#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#include "StringConversion.h"
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <locale>
#include <codecvt>
#include <chrono> // std::chrono::system_clock::now
#include <ctime> // gmtime


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
std::string Trim(const std::string& stringToTrim, std::string_view whitespace)
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
std::string TrimFront(const std::string& stringToTrim, std::string_view whitespace)
{
	auto stringBegin = stringToTrim.find_first_not_of(whitespace);

	if (stringBegin == std::string::npos) {
		return std::string(); // no content provided
	}

	return stringToTrim.substr(stringBegin, std::string::npos);
}

// Trim whitespace from back of string, returning copy of substring
std::string TrimBack(const std::string& stringToTrim, std::string_view whitespace)
{
	auto stringEnd = stringToTrim.find_last_not_of(whitespace);

	return stringToTrim.substr(0, stringEnd + 1);
}


std::vector<std::string> Split(const std::string& stringToSplit, char delimiter)
{
	std::vector<std::string> strings;

	std::size_t start = 0;
	std::size_t end = stringToSplit.find(delimiter);
	while (end != std::string::npos) {
		strings.push_back(stringToSplit.substr(start, end - start));
		start = end + 1;
		end = stringToSplit.find(delimiter, start);
	}
	strings.push_back(stringToSplit.substr(start, end - start));

	return strings;
}

std::vector<std::string> ParseCsv(const std::string& csv, char delimiter, std::string_view whitespace)
{
	// First check if input string is empty (or only whitespace)
	auto trimmedCsv = Trim(csv, whitespace);
	if (trimmedCsv.empty()) {
		return {}; // Empty (no data)
	}

	// Split data into vector of strings
	// At least one data element will be returned
	return SplitAndTrim(trimmedCsv, delimiter, whitespace);
}


std::string AddrToHexString(std::size_t addr)
{
	std::ostringstream stringStream;
	stringStream << std::setfill('0') << std::setw(8) << std::hex << addr;
	return stringStream.str();
}


std::string GetDateTime()
{
	auto currentClock = std::chrono::system_clock::now();
	auto time = std::chrono::system_clock::to_time_t(currentClock);
	std::tm unpackedTime;
	if(gmtime_s(&unpackedTime, &time)) {
		return "<Time conversion failed>";
	}

	std::stringstream stringStream;
	stringStream << std::put_time(&unpackedTime, "%Y-%m-%d %H:%M:%S UTC");

	return stringStream.str();
}
