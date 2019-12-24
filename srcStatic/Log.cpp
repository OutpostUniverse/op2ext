#include "Log.h"
#include "Log/LogDestination.h"
#include "FsInclude.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>


// Use anonymous namespace to keep globals private
// Prevents other files from gaining access with a sneaky extern declaration
namespace {
	// Static (constant and zero) initialization happens before dynamic initialization
	// Such values are well defined if accessed by a global object constructor before Main/DllMain starts
	// This pointer is safely set to null before other globals are initialized
	LogDestination* logDestinationError = nullptr;
	LogDestination* logDestinationMessage = nullptr;
	LogDestination* logDestinationDebug = nullptr;
}


// Set logging output destination for each logging level
// Caller owns the log destination and is responsible for cleanup when log destination is no longer required
// Use `SetLogDestinationXXXX(nullptr);` to unset a log destination

// Set log destination for error logging level
void SetLogDestinationError(LogDestination* newLogLogDestination) {
	logDestinationError = newLogLogDestination;
}

// Set log destination for standard logging level
void SetLogDestinationMessage(LogDestination* newLogDestination) {
	logDestinationMessage = newLogDestination;
}

// Set log destination for debug logging level
void SetLogDestinationDebug(LogDestination* newLogDestination) {
	logDestinationDebug = newLogDestination;
}


// Output log message at Error logging level
// This is likely to produce some kind of pop-up to immediately inform the user
void LogError(const std::string& message)
{
	if (logDestinationError) {
		logDestinationError->Log(message);
	}
}

// Output log message at Message logging level
// This is likely to appear in some kind of log file
void LogMessage(const std::string& message) {
	if (logDestinationMessage) {
		logDestinationMessage->Log(message);
	}
}

// Output log message at Debug logging level
// This is likely to show only with the help of special debugging tools
void LogDebug(const std::string& message)
{
	if (logDestinationDebug) {
		logDestinationDebug->Log(message);
	}
}


std::string FormatLogMessage(const std::string& message, const std::string& moduleName)
{
	return "[" + moduleName + "] " + message;
}
