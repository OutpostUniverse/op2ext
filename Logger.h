#pragma once

#include <fstream>
#include <string>

class Logger 
{
public:
	Logger();
	~Logger();

	void Log(const std::string& message, void* messageSource);

private:
	std::ofstream logFile;

	std::string GetSystemDateTime() const;
};
