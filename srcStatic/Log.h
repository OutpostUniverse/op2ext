#pragma once

#include <string>


class LogDestination;


// Set log destination for each of the logging levels
void SetLogDestinationError(LogDestination* newLogDestination);
void SetLogDestinationMessage(LogDestination* newLogDestination);
void SetLogDestinationDebug(LogDestination* newLogDestination);

// Log messages for each of the logging levels
void LogError(const std::string& message);
void LogMessage(const std::string& message);
void LogDebug(const std::string& message);


// Helpers to format log messages in standard ways
std::string FormatLogMessage(const std::string& message, const std::string& moduleName);
