#include "LoggerFile.h"
#include "FileSystemHelper.h"
#include <gtest/gtest.h>
#include <system_error>
#include <cstdint>

const fs::path logPath = fs::path(GetGameDirectory()).append("Outpost2Log.txt");

TEST(LoggerFile, LogFileExists)
{
	LoggerFile logger;

	std::error_code errorCode;
	ASSERT_TRUE(fs::exists(logPath, errorCode));
}

TEST(LoggerFile, MessageLogged)
{
	LoggerFile logger;

	const uintmax_t preFileSize = fs::file_size(logPath);
	EXPECT_NO_THROW(logger.Log("Test Log Message"));

	ASSERT_GT(fs::file_size(logPath), preFileSize);
}