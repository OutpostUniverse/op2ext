#include "LogDebug.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>


void LogDestinationDebug::Log(const std::string& message)
{
	OutputDebugString(message.c_str());
}
