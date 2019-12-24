#include "LogFile.h"
#include "../Log.h"
#include "../FileSystemHelper.h"
#include "../StringConversion.h"
#include <cstddef>


LogFile::LogFile() :
	logFile(GetExeDirectory() + "\\Outpost2Log.txt", std::ios::app | std::ios::out | std::ios::binary)
{
	if (!logFile.is_open()) {
		LogError("Unable to create or open Outpost2Log.txt");
	}
}

void LogFile::Log(const std::string& message)
{
	logFile << GetDateTime() << " " << message << std::endl;
}
