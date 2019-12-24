#include "Log.h"
#include "LogMessageTest.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>


TEST(Log, UnsetLogDestinationIsSafe) {
	EXPECT_NO_THROW(SetLogDestinationMessage(nullptr));
	EXPECT_NO_THROW(LogMessage("This goes nowhere"));
}


TEST_F(LogMessageTest, ActiveLogDestinationReceivesMessages) {
	const auto message = std::string("LogDestination should receive this");
	EXPECT_CALL(logDestinationMessage, Log(message));
	EXPECT_NO_THROW(LogMessage(message));
}
