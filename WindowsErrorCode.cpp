#include "WindowsErrorCode.h"
#include "StringConversion.h"
#include <strsafe.h> //Header requires WindowsXP SP2 or greater

std::string GetLastErrorStdString(LPCTSTR lpszFunction)
{
	// Adapted from https://docs.microsoft.com/en-us/windows/desktop/Debug/retrieving-the-last-error-code

	LPTSTR lpMsgBuf;
	DWORD dw = GetLastError();

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		reinterpret_cast<LPTSTR>(&lpMsgBuf),
		0, NULL);

	LPVOID lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
		(lstrlen(lpMsgBuf) + lstrlen(lpszFunction) + 40) * sizeof(TCHAR));
	
	StringCchPrintf((LPTSTR)lpDisplayBuf,
		LocalSize(lpDisplayBuf) / sizeof(TCHAR),
		TEXT("%s failed with error %d: %s"),
		lpszFunction, dw, lpMsgBuf);

	std::string errorMessage = ConvertLpctstrToString(static_cast<LPCTSTR>(lpDisplayBuf));

	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);

	return errorMessage;
}
