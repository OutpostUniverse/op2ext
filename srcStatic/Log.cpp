#include "Log.h"
#include "op2ext-Internal.h"


// Use anonymous namespace to keep globals private
// Prevents other files from gaining access with a sneaky extern declaration
namespace {
  // Static (constant and zero) initialization happens before dynamic initialization
  // Such values are well defined if accessed by a global object constructor before Main/DllMain starts
  // This pointer is safely set to null before other globals are initialized
  Logger* logger = nullptr;
}


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
