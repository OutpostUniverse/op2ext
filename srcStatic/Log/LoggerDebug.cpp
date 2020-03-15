#include "LoggerDebug.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>


void LoggerDebug::Log(const std::string& message)
{
	OutputDebugString((message + "\n").c_str());
}
