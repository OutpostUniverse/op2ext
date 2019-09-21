#include "Log.h"
#include "LoggerFile.h"
#include "op2ext-Internal.h"


void Log(const std::string& message, const std::string& moduleName) {
	// Delegate to internal logger
	logger.Log(message, moduleName);
}
