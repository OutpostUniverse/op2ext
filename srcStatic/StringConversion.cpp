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

bool ConvertLPWToString(std::string& stringOut, LPCWSTR inputWideString, UINT codepage)
{
	// Code adapted from: https://gist.github.com/icreatetoeducate/4019717

	bool result = false;
	std::string outputString;

	// First determine the required buffer size (but don't convert)
	int bsz = WideCharToMultiByte(codepage, 0, inputWideString, -1, 0, 0, 0, 0);

	if (bsz > 0) {
		// Allocate space for converted string
		outputString.resize(bsz);
		// Perform the actual conversion
		int rc = WideCharToMultiByte(codepage, 0, inputWideString, -1, outputString.data(), bsz, 0, 0);
		if (rc != 0) {
			outputString.erase(bsz - 1);
			stringOut = outputString;
			result = true;
		}
	}

	return result;
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
