#include "Log.h"
#include "Logger.h"
#include <gtest/gtest.h>


TEST(Log, UnsetLoggerIsSafe) {
	EXPECT_NO_THROW(SetLogger(nullptr));
	EXPECT_NO_THROW(Log("This goes nowhere"));
}
