#include "Log/LogDistributor.h"
#include "LogMessageTest.h"


TEST(LoggerDistributor, LoggerDistributor) {
	LoggerMock log1;
	LoggerMock log2;
	LoggerMock log3;
	LoggerDistributor logDistributor({&log1, &log2, &log3});

	EXPECT_CALL(log1, Log("Log message")).Times(1);
	EXPECT_CALL(log2, Log("Log message")).Times(1);
	EXPECT_CALL(log3, Log("Log message")).Times(1);
	logDistributor.Log("Log message");
}
