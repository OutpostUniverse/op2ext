#include "StringConversion.h"
#include <sstream>
#include <algorithm>


std::string WrapRawString(const char* str)
{
	return std::string(str);
}
std::wstring WrapRawString(const wchar_t* str)
{
	return std::wstring(str);
}

std::string ConvertWideToNarrow(std::wstring_view inputWideString, UINT codepage)
{
	constexpr DWORD dwFlags = 0;

	// The WideCharToMultiByte function considers it an error to pass an input string of length 0
	// Early exit if input string is empty
	if (inputWideString.empty()) {
		return std::string();
	}

	// First determine the required buffer size (but don't convert)
	auto requiredBufferSize = WideCharToMultiByte(codepage, dwFlags, inputWideString.data(), inputWideString.size(), nullptr, 0, nullptr, nullptr);
	if (requiredBufferSize == 0) {
		throw std::runtime_error("Wide to narrow string conversion failure: Unable to determine output buffer size");
	}

	std::string outputString;
	// Allocate space for converted string
	outputString.resize(requiredBufferSize);
	// Perform the actual conversion
	auto convertedSize = WideCharToMultiByte(codepage, dwFlags, inputWideString.data(), inputWideString.size(), outputString.data(), outputString.size(), nullptr, nullptr);
	if (convertedSize == 0) {
		throw std::runtime_error("Wide to narrow string conversion failure: Unable to convert string");
	}
	// Clip output string to however many characters were actually converted
	outputString.erase(convertedSize);

	return outputString;
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
