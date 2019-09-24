#include "StringConversion.h"
#include <sstream>
#include <algorithm>

std::string ConvertLpctstrToString(LPCSTR str)
{
	return std::string(str);
}
std::wstring ConvertLpctstrToString(LPCWSTR str)
{
	return std::wstring(str);
}

std::string ConvertLPWToString(std::wstring_view inputWideString, UINT codepage)
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


std::vector<std::string> SplitString(std::string stringToSplit, char delimiter, TrimOption trimOption)
{
	std::vector<std::string> strings;

	std::istringstream stringStream(stringToSplit);
	std::string currentToken;

	while (std::getline(stringStream, currentToken, delimiter)) {
		currentToken = TrimString(currentToken, trimOption);
		strings.push_back(currentToken);
	}

	return strings;
}

// Trims both leading and trailing whitespace. The 'whitespace' character may be custom defined.
std::string TrimString(const std::string& stringToTrim, TrimOption trimOption, const std::string& whitespace)
{
	if (trimOption == TrimOption::None) {
		return std::string();
	}

	std::size_t stringBegin = 0;
	if (trimOption == TrimOption::Leading || trimOption == TrimOption::Both) {
		stringBegin = stringToTrim.find_first_not_of(whitespace);
	}

	if (stringBegin == std::string::npos) {
		return std::string(); // no content provided
	}

	auto stringEnd = stringToTrim.length();
	if (trimOption == TrimOption::Trailing || trimOption == TrimOption::Both) {
		stringEnd = stringToTrim.find_last_not_of(whitespace);
	}

	const auto range = stringEnd - stringBegin + 1;

	return stringToTrim.substr(stringBegin, range);
}
