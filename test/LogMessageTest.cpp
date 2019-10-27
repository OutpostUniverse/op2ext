#include "LogMessageTest.h"
#include "Log.h"


// For compile speed reasons these should be instantiated in their own implementation file
// They may look trivial, but construct and destruct all hidden gMock member objects
// In particular, this is where gMock actually performs verification of expectations
LoggerMock::LoggerMock() {}
LoggerMock::~LoggerMock() {}



void LogMessageTest::SetUp() {
	// Install test logger
	EXPECT_NO_THROW(SetLogger(&logger));
	EXPECT_NO_THROW(SetLoggerError(&loggerError));
	EXPECT_NO_THROW(SetLoggerDebug(&loggerDebug));
}

void LogMessageTest::TearDown() {
	// Remove test logger
	EXPECT_NO_THROW(SetLogger(nullptr));
	EXPECT_NO_THROW(SetLoggerError(nullptr));
	EXPECT_NO_THROW(SetLoggerDebug(nullptr));
}
