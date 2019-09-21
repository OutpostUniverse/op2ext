#include "Log.h"
#include "op2ext-Internal.h"


// Set logging output destination
// Caller owns the logger and is responsible for cleanup when logger is no longer required
// Use `SetLogger(nullptr);` to unset a logger
void SetLogger(Logger* newLogger) {
	logger = newLogger;
}

void Log(const std::string& message, const std::string& moduleName) {
	// Make sure a logger has been set first
	if (logger) {
		// Delegate to internal logger
		logger->Log(message, moduleName);
	}
}
