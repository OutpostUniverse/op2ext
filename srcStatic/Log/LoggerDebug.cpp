#include "LoggerDebug.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

const std::string newline("\n");

void LoggerDebug::Log(const std::string& message)
{
	OutputDebugString((message + newline).c_str());
}
