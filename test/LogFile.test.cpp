#include "Log/LogFile.h"
#include "FileSystemHelper.h"
#include "FsInclude.h"
#include <gtest/gtest.h>


const auto logPath = fs::path(GetExeDirectory()).append("Outpost2Log.txt").string();


TEST(LogFile, LogFileExists)
{
	// Creating a log destination should open or create a log file
	LogFile logDestination;
	ASSERT_TRUE(Exists(logPath));
}

TEST(LogFile, MessageLogged)
{
	LogFile logDestination;

	const auto preFileSize = fs::file_size(logPath);
	EXPECT_NO_THROW(logDestination.Log("Test Log Message"));

	ASSERT_GT(fs::file_size(logPath), preFileSize);
}
