#include "StringConversion.h"

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
