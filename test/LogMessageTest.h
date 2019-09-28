#include "Log.h"
#include "Logger.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>


class LoggerMock : public Logger {
public:
	LoggerMock();
	virtual ~LoggerMock();

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
