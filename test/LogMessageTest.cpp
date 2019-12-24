#include "LogMessageTest.h"
#include "Log.h"


// For compile speed reasons these should be instantiated in their own implementation file
// They may look trivial, but construct and destruct all hidden gMock member objects
// In particular, this is where gMock actually performs verification of expectations
LogDestinationMock::LogDestinationMock() {}
LogDestinationMock::~LogDestinationMock() {}



void LogMessageTest::SetUp() {
	// Install test log destinations
	EXPECT_NO_THROW(SetLogDestinationError(&logDestinationError));
	EXPECT_NO_THROW(SetLogDestinationMessage(&logDestinationMessage));
	EXPECT_NO_THROW(SetLogDestinationDebug(&logDestinationDebug));
}

void LogMessageTest::TearDown() {
	// Remove test log destinations
	EXPECT_NO_THROW(SetLogDestinationError(nullptr));
	EXPECT_NO_THROW(SetLogDestinationMessage(nullptr));
	EXPECT_NO_THROW(SetLogDestinationDebug(nullptr));
}
