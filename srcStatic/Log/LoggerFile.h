#pragma once

#include <fstream>
#include <string>
#include "LogDestination.h"


class LoggerFile : public LogDestination
{
public:
	LoggerFile();

	void Log(const std::string& message) override;

private:
	std::ofstream logFile;
};
