#include "Log.h"
#include "LogMessageTest.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>


TEST(Log, UnsetLoggerIsSafe) {
	EXPECT_NO_THROW(SetLogger(nullptr));
	EXPECT_NO_THROW(Log("This goes nowhere"));
}


TEST_F(LogMessageTest, ActiveLoggerReceivesMessages) {
	const auto message = std::string("Logger should receive this");
	EXPECT_CALL(logger, Log(message));
	EXPECT_NO_THROW(Log(message));
}
