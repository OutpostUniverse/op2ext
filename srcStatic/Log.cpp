#include "Log.h"
#include "Logger.h"
#include "LoggerMessageBox.h"
#include "FsInclude.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>


// Use anonymous namespace to keep globals private
// Prevents other files from gaining access with a sneaky extern declaration
namespace {
	// Static (constant and zero) initialization happens before dynamic initialization
	// Such values are well defined if accessed by a global object constructor before Main/DllMain starts
	// This pointer is safely set to null before other globals are initialized
	Logger* loggerMessage = nullptr;
	Logger* loggerError = nullptr;
	Logger* loggerDebug = nullptr;
}


// Set logging output destination for each logging level
// Caller owns the logger and is responsible for cleanup when logger is no longer required
// Use `SetLoggerX(nullptr);` to unset a logger

// Set logger for standard logging level
void SetLoggerMessage(Logger* newLogger) {
	loggerMessage = newLogger;
}

// Set logger for error logging level
void SetLoggerError(Logger* newLogger) {
	loggerError = newLogger;
}

// Set logger for debug logging level
void SetLoggerDebug(Logger* newLogger) {
	loggerDebug = newLogger;
}


// Output log message at standard logging level
void LogMessage(const std::string& message) {
	// Make sure a logger has been set first
	if (loggerMessage) {
		// Delegate to internal logger
		loggerMessage->Log(message);
	}
}


// Output log message at Debug logging level
// Currently this code doesn't support redirection of logging output
void LogDebug(const std::string& message)
{
	if (loggerDebug) {
		loggerDebug->Log(message);
	}
}

// Output log message at Error logging level
// Currently this is designed to produce a pop-up error message box
void LogError(const std::string& message)
{
	if (loggerError) {
		loggerError->Log(message);
	}
}


std::string FormatLogMessage(const std::string& message, const std::string& moduleName)
{
	return "[" + moduleName + "] " + message;
}
