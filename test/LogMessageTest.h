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
	void SetUp() override;
	void TearDown() override;

	LoggerMock logger;
	LoggerMock loggerError;
	LoggerMock loggerDebug;
};
