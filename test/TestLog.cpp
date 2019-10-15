#include "TestLog.h"
#include "TestLogger.h"


// Use anonymous namespace to keep globals private
namespace {
	TestLogger logger;
	TestLogger errorLogger;
}

void ResetTestLog()
{
	logger.Clear();
	errorLogger.Clear();

	SetLogger(&logger);
	SetLoggerError(&errorLogger);
}

TestLogger& GetLogger()
{
	return logger;
}

TestLogger& GetErrorLogger()
{
	return errorLogger;
}
