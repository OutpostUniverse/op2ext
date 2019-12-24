#pragma once

#include <fstream>
#include <string>
#include "LogDestination.h"


class LogFile : public LogDestination
{
public:
	LogFile();

	void Log(const std::string& message) override;

private:
	std::ofstream logFile;
};
