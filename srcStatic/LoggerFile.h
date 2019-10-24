#pragma once

#include <fstream>
#include <string>
#include "Logger.h"


class LoggerFile : public Logger
{
public:
	LoggerFile();

	void Log(const std::string& message) override;

private:
	std::ofstream logFile;
};
