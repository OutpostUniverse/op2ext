#pragma once

#include <string>


class Logger;


// Set logger for each of the logging levels
void SetLogger(Logger* newLogger);
void SetLoggerError(Logger* newLogger);
void SetLoggerDebug(Logger* newLogger);

// Log messages for each of the logging levels
void Log(const std::string& message);
void LogDebug(const std::string& message);
void LogError(const std::string& message);


// Helpers to format log messages in standard ways
std::string FormatLogMessage(const std::string& message, const std::string& moduleName = "op2ext.dll");
std::string FormatLogMessage(const std::string& message, const std::string& sourcePathFilename, long lineInSourceCode);
