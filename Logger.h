#pragma once

#include <fstream>
#include <string>

class Logger 
{
public:
	Logger();
	~Logger();

	void Log(const std::string& moduleName, const std::string& message);

private:
	std::ofstream logFile;

	static std::string GetSystemDateTime();
};
