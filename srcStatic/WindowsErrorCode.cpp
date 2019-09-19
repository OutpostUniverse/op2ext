#include "WindowsErrorCode.h"
#include "StringConversion.h"
#include <windows.h>

std::string GetLastErrorString(std::string functionName)
{
	// Adapted from https://docs.microsoft.com/en-us/windows/desktop/Debug/retrieving-the-last-error-code

	LPTSTR lpMsgBuf;
	DWORD lastErrorCode = GetLastError();

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		lastErrorCode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPTSTR>(&lpMsgBuf),
		0,
		NULL
	);

	auto errorCodeMessage = ConvertLpctstrToString(lpMsgBuf);

	auto errorMessage = functionName + " failed with error " + std::to_string(lastErrorCode) + ": " + errorCodeMessage;

	LocalFree(lpMsgBuf);

	return errorMessage;
}
