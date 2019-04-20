#include "StringConversion.h"
#include <algorithm>

std::string ConvertLpctstrToString(LPCSTR str)
{
#if defined(UNICODE) || defined(_UNICODE)
	throw std::runtime_error("Unicode is not supported");
#endif

	return std::string(str);
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

void ToLowerInPlace(std::string& x) {
	std::transform(x.begin(), x.end(), x.begin(), ::tolower);
}

std::string ToLower(std::string x) {
	std::transform(x.begin(), x.end(), x.begin(), ::tolower);

	return x;
}