#include "Log/Logger.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>


class LoggerMock : public Logger {
public:
	LoggerMock();
	virtual ~LoggerMock();

	MOCK_METHOD1(Log, void(const std::string& message));
};


class LogMessageTest : public ::testing::Test {
protected:
	void SetUp() override;
	void TearDown() override;

	LoggerMock loggerError;
	LoggerMock loggerMessage;
	LoggerMock loggerDebug;
};
