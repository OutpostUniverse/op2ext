#include "Log.h"
#include "Logger.h"
#include <gtest/gtest.h>


TEST(Log, UnsetLoggerIsSafe) {
	EXPECT_NO_THROW(SetLogger(nullptr));
	EXPECT_NO_THROW(Log("This goes nowhere"));
}


#if __has_include(<gmock/gmock.h>)

#include <gmock/gmock.h>

class MockLogger : public Logger {
public:
	MOCK_METHOD2(Log, void(const std::string& message, const std::string& moduleName));
};

TEST(Log, ActiveLoggerReceivesMessages) {
	const auto message = std::string("Logger should receive this");

	MockLogger logger;
	EXPECT_CALL(logger, Log(message, "op2ext.dll"));

	EXPECT_NO_THROW(SetLogger(&logger));
	EXPECT_NO_THROW(Log(message));
	EXPECT_NO_THROW(SetLogger(nullptr));
}

#endif
