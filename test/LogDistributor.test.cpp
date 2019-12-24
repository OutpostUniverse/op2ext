#include "Log/LogDistributor.h"
#include "LogMessageTest.h"


TEST(LogDistributor, LogDistributor) {
	LogDestinationMock log1;
	LogDestinationMock log2;
	LogDestinationMock log3;
	LogDistributor logDistributor({&log1, &log2, &log3});

	EXPECT_CALL(log1, Log("Log message")).Times(1);
	EXPECT_CALL(log2, Log("Log message")).Times(1);
	EXPECT_CALL(log3, Log("Log message")).Times(1);
	logDistributor.Log("Log message");
}
