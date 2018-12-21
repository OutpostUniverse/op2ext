#pragma once

#include <fstream>
#include <string>

class Logger 
{
public:
	Logger();
	~Logger();

	// Log a message internally from op2ext
	void Log(const std::string& message);

	// Log a message from an external module (dll).
	// @param messageSource is an address of the instruction in the calling function that facilitates identifying the source module.
	void Log(const std::string& message, const void* messageSource);

private:
	std::ofstream logFile;

	void WriteMessage(const std::string& message, const std::string& moduleName);
	std::string GetSystemDateTime() const;
};
