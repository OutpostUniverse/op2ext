#pragma once

#include <string>


class Logger;


// Set logger for each of the logging levels
void SetLogger(Logger* newLogger);
void SetLoggerError(Logger* newLogger);

void Log(const std::string& message, const std::string& moduleName = "op2ext.dll");


// Outputs a debug string. Output is not logged by the logger.
void LogDebug(const std::string& message);

// Logs an error message with the logger and then posts it to user in a modal dialog box.
void LogError(const std::string& message);


std::string FormatLogMessage(const std::string& message, const std::string& moduleName = "op2ext.dll");
std::string FormatLogMessage(const std::string& message, const std::string& sourcePathFilename, long lineInSourceCode);


// Allow defaulting of the __FILE__ and __LINE__ macros at the point of call
// Macro name must be different from original function name
// Using the same name will cause an error when the compiler sees the underlying function definition
#define PostError(message) LogError(FormatLogMessage(message, __FILE__, __LINE__))
