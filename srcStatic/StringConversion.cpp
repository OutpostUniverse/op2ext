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

bool ConvertLPWToString(std::string& stringOut, const LPWSTR pw, UINT codepage)
{
	// Code adapted from: https://gist.github.com/icreatetoeducate/4019717

	bool result = false;
	char* p = 0;

	int bsz = WideCharToMultiByte(codepage, 0, pw, -1, 0, 0, 0, 0);

	if (bsz > 0) {
		p = new char[bsz];
		int rc = WideCharToMultiByte(codepage, 0, pw, -1, p, bsz, 0, 0);
		if (rc != 0) {
			p[bsz - 1] = 0;
			stringOut = p;
			result = true;
		}
	}

	delete[] p;

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
