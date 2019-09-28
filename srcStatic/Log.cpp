#include "Log.h"
#include "Logger.h"
#include "LoggerMessageBox.h"
#include "FileSystemHelper.h"
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


// Set logging output destination
// Caller owns the logger and is responsible for cleanup when logger is no longer required
// Use `SetLogger(nullptr);` to unset a logger

void SetLogger(Logger* newLogger) {
	logger = newLogger;
}

void SetLoggerError(Logger* newLogger) {
	loggerError = newLogger;
}


void Log(const std::string& message, const std::string& moduleName) {
	// Make sure a logger has been set first
	if (logger) {
		// Delegate to internal logger
		logger->Log(message, moduleName);
	}
}



void OutputDebug(const std::string& message)
{
#ifdef DEBUG
	OutputDebugString(message.c_str());
#endif
}

void PostErrorMessage(const std::string& errorMessage, const std::string& sourcePathFilename, long lineInSourceCode)
{
	// __FILE__ returns absolute filename. Strip the absolute path to reduce clutter in log output
	auto sourceFilename = fs::path(sourcePathFilename).filename().string();

	const std::string formattedMessage = sourceFilename + ", Line: " + std::to_string(lineInSourceCode) + ": " + errorMessage;
	Log(formattedMessage);
	if (loggerError) {
		loggerError->Log(formattedMessage);
	}
}
