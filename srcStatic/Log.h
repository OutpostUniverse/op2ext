#pragma once

#include <string>


class Logger;


// Set logger for each of the logging levels
void SetLoggerError(Logger* newLogger);
void SetLoggerMessage(Logger* newLogger);
void SetLoggerDebug(Logger* newLogger);

// Log messages for each of the logging levels
void LogError(const std::string& message);
void LogMessage(const std::string& message);
void LogDebug(const std::string& message);


// Helpers to format log messages in standard ways
std::string FormatLogMessage(const std::string& message, const std::string& moduleName);
