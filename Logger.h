#pragma once

#include <fstream>
#include <string>

class Logger 
{
public:
	Logger();
	~Logger();

	void Log(const std::string& message);

private:
	std::ofstream logFile;

	std::string GetSystemDateTime() const;
};
