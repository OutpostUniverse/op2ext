#include "Log/LogDestination.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>


class LogDestinationMock : public LogDestination {
public:
	LogDestinationMock();
	virtual ~LogDestinationMock();

	MOCK_METHOD1(Log, void(const std::string& message));
};


class LogMessageTest : public ::testing::Test {
protected:
	void SetUp() override;
	void TearDown() override;

	LogDestinationMock logDestinationError;
	LogDestinationMock logDestinationMessage;
	LogDestinationMock logDestinationDebug;
};
