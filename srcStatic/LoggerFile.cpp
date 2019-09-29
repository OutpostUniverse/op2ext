#include "LoggerFile.h"
#include "Log.h"
#include "FileSystemHelper.h"
#include "StringConversion.h"
#include <cstddef>


LoggerFile::LoggerFile() :
	logFile(GetGameDirectory() + "\\Outpost2Log.txt", std::ios::app | std::ios::out | std::ios::binary)
{
	if (!logFile.is_open()) {
		PostError("Unable to create or open Outpost2Log.txt");
	}
}

void LoggerFile::Log(const std::string& message, const std::string& moduleName)
{
	logFile << GetSystemDateTime() << " [" << moduleName << "] " << message << std::endl;
}
