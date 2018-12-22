#include "Logger.h"

#include "GlobalDefines.h"
#include "WindowsModule.h"
#include <cstddef>
#include <chrono>
#include <ctime> //gmtime
#include <iomanip> //put_time
#include <sstream> // stringstream

std::string GetSystemDateTime();


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

void Logger::Log(const std::string& message, const std::string& moduleName)
{	
	logFile << GetSystemDateTime() << " [" << moduleName << "] " << message << std::endl;
}


std::string GetSystemDateTime()
{
	auto currentClock = std::chrono::system_clock::now();
	auto time = std::chrono::system_clock::to_time_t(currentClock);

	std::stringstream stringStream;
	stringStream << std::put_time(std::gmtime(&time), "%F %T UTC");

	return stringStream.str();
}
