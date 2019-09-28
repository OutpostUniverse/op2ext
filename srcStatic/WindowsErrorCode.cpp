#include "WindowsErrorCode.h"
#include "LocalResource.h"
#include <windows.h>

std::string GetLastErrorString()
{
	LocalResource<LPCSTR> lpMsgBuf;
	DWORD lastErrorCode = GetLastError();

	// Sanity check for reinterpret_cast used below
	static_assert(sizeof(LPSTR) == sizeof(lpMsgBuf));

	FormatMessageA(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		nullptr,
		lastErrorCode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPSTR>(&lpMsgBuf),
		0,
		nullptr
	);

	auto errorCodeMessage = std::string(lpMsgBuf.get());
	auto errorMessage = "Error " + std::to_string(lastErrorCode) + ": " + errorCodeMessage;

	return errorMessage;
}
