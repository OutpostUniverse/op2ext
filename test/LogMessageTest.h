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

	void ExpectNoLogCall()
	{
		EXPECT_CALL(logger, Log(::testing::_, ::testing::_)).Times(0);
		EXPECT_CALL(loggerError, Log(::testing::_, ::testing::_)).Times(0);
	}

	void ExpectLogCall(const std::string& substring, int times)
	{
		EXPECT_CALL(logger, Log(::testing::HasSubstr(substring), "op2ext.dll")).Times(1);
		EXPECT_CALL(loggerError, Log(::testing::_, ::testing::_)).Times(0);
	}

	void ExpectErrorLogCall(const std::string& substring, int times)
	{
		EXPECT_CALL(logger, Log(::testing::HasSubstr(substring), "op2ext.dll")).Times(1);
		EXPECT_CALL(loggerError, Log(::testing::HasSubstr(substring), "op2ext.dll")).Times(1);
	}
};
