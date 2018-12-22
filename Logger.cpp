#include "Logger.h"

#include "GlobalDefines.h"
#include <cstddef>
#include <chrono>
#include <ctime> //gmtime
#include <iomanip> //put_time
#include <sstream> // stringstream

Logger::Logger() : logFile("Outpost2Log.txt", std::ios::app | std::ios::out | std::ios::binary) 
{
	if (!logFile.is_open()) {
		PostErrorMessage("Logger.cpp", __LINE__, "Unable to create or open Outpost2Log.txt");
	}
}

Logger::~Logger()
{
	logFile.close();
}

void Logger::Log(const std::string& moduleName, const std::string& message)
{	
	logFile << GetSystemDateTime() << " UTC [" << moduleName << "] " << message << std::endl;
}

std::string Logger::GetSystemDateTime() const
{
	auto currentClock = std::chrono::system_clock::now();
	auto time = std::chrono::system_clock::to_time_t(currentClock);

	std::stringstream stringStream;
	stringStream << std::put_time(std::gmtime(&time), "%F %T");

	return stringStream.str();
}
