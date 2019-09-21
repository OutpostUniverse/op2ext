#pragma once

#include <fstream>
#include <string>
#include "Logger.h"


class LoggerFile : public Logger
{
public:
	LoggerFile();

	void Log(const std::string& message, const std::string& moduleName = "op2ext.dll") override;

private:
	std::ofstream logFile;
};
