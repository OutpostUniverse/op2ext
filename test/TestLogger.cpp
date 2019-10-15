#include "TestLogger.h"


// Use anonymous namespace to keep globals private
namespace {
	TestLogger errorLogger;
}

void ResetTestErrorLogger()
{
	errorLogger.Clear();
	SetLoggerError(&errorLogger);
}

TestLogger& GetErrorLogger()
{
	return errorLogger;
}


void TestLogger::Log(const std::string& message, const std::string& moduleName)
{
	messageStack.push_back(message);
}

std::string TestLogger::Pop()
{
	std::string message;

	if (messageStack.size() > 0) 
	{
		message = messageStack.back();
		messageStack.pop_back();
	}

	return message;
}

bool TestLogger::Pop(const std::string& comparison)
{
	return Pop().find(comparison) != std::string::npos;
}
