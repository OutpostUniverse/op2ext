#include "Log.h"
#include "LogMessageTest.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>


TEST(Log, UnsetLoggerIsSafe) {
	EXPECT_NO_THROW(SetLoggerMessage(nullptr));
	EXPECT_NO_THROW(LogMessage("This goes nowhere"));
}


TEST_F(LogMessageTest, ActiveLoggerReceivesMessages) {
	const auto message = std::string("LogDestination should receive this");
	EXPECT_CALL(loggerMessage, Log(message));
	EXPECT_NO_THROW(LogMessage(message));
}
