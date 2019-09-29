#include "LoggerFile.h"
#include "FileSystemHelper.h"
#include <gtest/gtest.h>


const fs::path logPath = fs::path(GetGameDirectory()).append("Outpost2Log.txt");


TEST(LoggerFile, LogFileExists)
{
	// Creating a logger should open or create a log file
	LoggerFile logger;
	ASSERT_TRUE(fs::exists(logPath));
}

TEST(LoggerFile, MessageLogged)
{
	LoggerFile logger;

	const auto preFileSize = fs::file_size(logPath);
	EXPECT_NO_THROW(logger.Log("Test Log Message"));

	ASSERT_GT(fs::file_size(logPath), preFileSize);
}
