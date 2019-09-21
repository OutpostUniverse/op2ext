#pragma once

#include <fstream>
#include <string>

class LoggerFile
{
public:
	LoggerFile();
	~LoggerFile();

	void Log(const std::string& message, const std::string& moduleName = "op2ext.dll");

private:
	std::ofstream logFile;
};
