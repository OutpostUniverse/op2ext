#include "LoggerDebug.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#ifdef _WIN32
const std::string newline("\r\n");
#elif defined macintosh // OS 9
const std::string newline("\r");
#else
const std::string newline("\n") // Mac OS X uses \n
#endif

void LoggerDebug::Log(const std::string& message)
{
	OutputDebugString(std::string(message + newline).c_str());
}
