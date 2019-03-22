#pragma once

#include <fstream>
#include <string>

class Logger 
{
public:
	Logger();
	~Logger();

	void Log(const std::string& message, const std::string& moduleName = "op2ext.dll");

private:
	std::ofstream logFile;
};
