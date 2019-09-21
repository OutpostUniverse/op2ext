#include "WindowsErrorCode.h"
#include "StringConversion.h"
#include "LocalResource.h"
#include <windows.h>

std::string GetLastErrorString()
{
	LocalResource<LPTSTR> lpMsgBuf;
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
	auto errorMessage = "Error " + std::to_string(lastErrorCode) + ": " + errorCodeMessage;

	return errorMessage;
}
