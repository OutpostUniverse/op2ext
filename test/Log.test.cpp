#include "Log.h"
#include "Logger.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>


TEST(Log, UnsetLoggerIsSafe) {
	EXPECT_NO_THROW(SetLogger(nullptr));
	EXPECT_NO_THROW(Log("This goes nowhere"));
}


class LoggerMock : public Logger {
public:
	MOCK_METHOD2(Log, void(const std::string& message, const std::string& moduleName));
};


class LogMessageTest : public ::testing::Test {
protected:
	void SetUp() override {
		// Install test logger
		EXPECT_NO_THROW(SetLogger(&logger));
	}
	void TearDown() override {
		// Remove test logger
		EXPECT_NO_THROW(SetLogger(nullptr));
	}

	LoggerMock logger;
};


TEST_F(LogMessageTest, ActiveLoggerReceivesMessages) {
	const auto message = std::string("Logger should receive this");
	EXPECT_CALL(logger, Log(message, "op2ext.dll"));
	EXPECT_NO_THROW(Log(message));
}

