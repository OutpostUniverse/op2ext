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


std::size_t CopyStringViewIntoCharBuffer(std::string_view stringToCopy, char* buffer, std::size_t bufferSize)
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


std::string AddrToHexString(std::uintptr_t addr)
{
	std::ostringstream stringStream;
	stringStream << std::setfill('0') << std::setw(8) << std::hex << addr;
	return stringStream.str();
}

std::string AddrToHexString(const void* addr)
{
	return AddrToHexString(reinterpret_cast<std::uintptr_t>(addr));
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
