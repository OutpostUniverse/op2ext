#pragma once

#include <string>


class Logger;


// Set logger for each of the logging levels
void SetLoggerMessage(Logger* newLogger);
void SetLoggerError(Logger* newLogger);
void SetLoggerDebug(Logger* newLogger);

// Log messages for each of the logging levels
void LogMessage(const std::string& message);
void LogDebug(const std::string& message);
void LogError(const std::string& message);


// Helpers to format log messages in standard ways
std::string FormatLogMessage(const std::string& message, const std::string& moduleName);
