#include "LoggerFile.h"
#include "Log.h"
#include "FileSystemHelper.h"
#include <cstddef>
#include <chrono>
#include <ctime> //gmtime
#include <iomanip> //put_time
#include <sstream> // stringstream

std::string GetSystemDateTime();


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


std::string GetSystemDateTime()
{
	auto currentClock = std::chrono::system_clock::now();
	auto time = std::chrono::system_clock::to_time_t(currentClock);
	std::tm unpackedTime;
	if(gmtime_s(&unpackedTime, &time)) {
		return "<Time conversion failed>";
	}

	std::stringstream stringStream;
	stringStream << std::put_time(&unpackedTime, "%F %T UTC");

	return stringStream.str();
}
