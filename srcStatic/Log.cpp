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
	Logger* logger = nullptr;
	Logger* loggerError = nullptr;
}


// Set logging output destination for each logging level
// Caller owns the logger and is responsible for cleanup when logger is no longer required
// Use `SetLoggerX(nullptr);` to unset a logger

// Set logger for standard logging level
void SetLogger(Logger* newLogger) {
	logger = newLogger;
}

// Set logger for error logging level
void SetLoggerError(Logger* newLogger) {
	loggerError = newLogger;
}


// Output log message at standard logging level
void Log(const std::string& message, const std::string& moduleName) {
	// Make sure a logger has been set first
	if (logger) {
		// Delegate to internal logger
		logger->Log(message, moduleName);
	}
}


// Output log message at Debug logging level
// Currently this code doesn't support redirection of logging output
void LogDebug(const std::string& message)
{
#ifdef DEBUG
	OutputDebugString(message.c_str());
#endif
}

// Output log message at Error logging level
// Currently this is designed to produce a pop-up error message box
void PostErrorMessage(const std::string& errorMessage, const std::string& sourcePathFilename, long lineInSourceCode)
{
	// __FILE__ returns absolute filename. Strip the absolute path to reduce clutter in log output
	auto sourceFilename = fs::path(sourcePathFilename).filename().string();

	const std::string formattedMessage = sourceFilename + ", Line: " + std::to_string(lineInSourceCode) + ": " + errorMessage;
	if (loggerError) {
		loggerError->Log(formattedMessage);
	}
}
