#include "op2ext.h"
#include <gtest/gtest.h>
#include <string_view>
#include <fstream>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>


TEST(op2ext, GetGameDir_s) {
	constexpr char Nonce = 255;
	char gameDirectory[MAX_PATH] = {Nonce};

	// No crash on nullptr buffer
	EXPECT_NE(0u, GetGameDir_s(nullptr, 0));
	EXPECT_NE(0u, GetGameDir_s(nullptr, MAX_PATH));

	// Returns needed buffer size when supplied buffer is too small
	// Buffer is not modified beyond specified length
	EXPECT_NE(0u, GetGameDir_s(gameDirectory, 0));
	EXPECT_EQ(Nonce, gameDirectory[0]);

	// On success return value is 0, and buffer contains path of specified format
	EXPECT_EQ(0u, GetGameDir_s(gameDirectory, MAX_PATH));
	std::string_view gameDir{gameDirectory};
	// Path plus null terminator should fit within buffer size
	ASSERT_TRUE(gameDir.length() < MAX_PATH);
	// Path is null terminated (this is outside the string_view window)
	EXPECT_EQ(0, gameDirectory[gameDir.length()]);
	// Path ends with a trailing slash
	EXPECT_EQ('\\', gameDir.back());
}

TEST(op2ext, GetGameDir) {
	char gameDirectory[MAX_PATH];

	// GetGameDir is deprecated, so suppress compiler warning from test code
	// MSVC tags use of deprecated methods as C4996
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#pragma warning(suppress : 4996)
	EXPECT_NO_THROW(GetGameDir(gameDirectory));
#pragma GCC diagnostic pop

	std::string_view gameDir{gameDirectory};
	// Path plus null terminator should fit within buffer size
	ASSERT_TRUE(gameDir.length() < MAX_PATH);
	// Path is null terminated (this is outside the string_view window)
	EXPECT_EQ(0, gameDirectory[gameDir.length()]);
	// Path ends with a trailing slash
	EXPECT_EQ('\\', gameDir.back());
}

TEST(op2ext, GetGameDirMethodsGiveSameResult) {
	char gameDirectory[MAX_PATH];
	char gameDirectorySafe[MAX_PATH];

	// GetGameDir is deprecated, so suppress compiler warning from test code
	// MSVC tags use of deprecated methods as C4996
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#pragma warning(suppress : 4996)
	EXPECT_NO_THROW(GetGameDir(gameDirectory));
#pragma GCC diagnostic pop

	// On success return value is 0, and buffer contains path of specified format
	EXPECT_EQ(0u, GetGameDir_s(gameDirectorySafe, MAX_PATH));

	// Result of both methods sould match, assuming sufficient buffer space
	EXPECT_STREQ(gameDirectory, gameDirectorySafe);
}

TEST(op2ext, GetConsoleModDir_s) {
	constexpr char Nonce = 255;
	char consoleModDir[MAX_PATH] = {Nonce};

	// No crash on nullptr buffer
	EXPECT_NE(0u, GetConsoleModDir_s(nullptr, 0));
	EXPECT_NE(0u, GetConsoleModDir_s(nullptr, MAX_PATH));

	// Returns needed buffer size when supplied buffer is too small
	// Buffer is not modified beyond specified length
	EXPECT_NE(0u, GetConsoleModDir_s(consoleModDir, 0));
	EXPECT_EQ(Nonce, consoleModDir[0]);

	// On success return value is 0, and buffer contains path of specified format
	EXPECT_EQ(0u, GetConsoleModDir_s(consoleModDir, MAX_PATH));
	std::string_view consoleModDirView{consoleModDir};
	// Path plus null terminator should fit within buffer size
	ASSERT_TRUE(consoleModDirView.length() < MAX_PATH);
	// Path is null terminated (this is outside the string_view window)
	EXPECT_EQ(0, consoleModDir[consoleModDirView.length()]);
	// Path ends with a trailing slash
	EXPECT_EQ('\\', consoleModDirView.back());
}

TEST(op2ext, AddVolToList) {
	// Test Invalid volume filename
	// No exceptions are allowed across the C-only interface
	// This may result in logging
	EXPECT_NO_THROW(AddVolToList("VolumeDoesNotExist.vol"));

	// Set path to test VOL file
	char gameDir[MAX_PATH];
	GetGameDir_s(gameDir, sizeof(gameDir));
	const auto volPath = std::string(gameDir) + "TestVolume.vol";

	// Define the VOL file contents
	const unsigned char volData[] = {
		0x56, 0x4F, 0x4C, 0x20, // "VOL "
		0x1C, 0x00, 0x00, 0x80, // 28 byte section
		0x76, 0x6F, 0x6C, 0x68, // "volh"
		0x00, 0x00, 0x00, 0x80, // 00 byte section
		0x76, 0x6F, 0x6C, 0x73, // "vols"
		0x04, 0x00, 0x00, 0x80, // 04 byte section
		0x00, 0x00, 0x00, 0x00, // 00 byte string table
		0x76, 0x6F, 0x6C, 0x69, // "voli"
		0x00, 0x00, 0x00, 0x80, // 00 byte section
	};
	// Create test VOL file
	// Use base class since `ofstream` is `basic_ofstream<char>`
	// We want to `write` an array of `unsigned char`
	// Use temporary object that auto destructs and closes after write
	std::basic_ofstream<unsigned char>(volPath).write(volData, sizeof(volData));

	// Test loading 100 volumes
	// There should be no volume load limit
	for (auto i = 0; i < 100; ++i)
	{
		EXPECT_NO_THROW(AddVolToList(volPath.c_str()));
	}

	// Cleanup test VOL file
	// Use Win API directly since it's already imported
	EXPECT_NE(0, DeleteFileA(volPath.c_str()));
}

TEST(op2ext, SetSerialNumber) {
	// Can set any single digit with no errors
	EXPECT_NO_THROW(SetSerialNumber(0, 0, 0));
	EXPECT_NO_THROW(SetSerialNumber(9, 9, 9));
}

TEST(op2ext, Log) {
	// Can log any text message
	EXPECT_NO_THROW(Log("Test adding a message to log file"));
}

TEST(op2ext, IsModuleLoaded) {
	EXPECT_FALSE(IsModuleLoaded(""));
	EXPECT_FALSE(IsModuleLoaded("NonExistentModuleName"));
}

TEST(op2ext, IsConsoleModuleLoaded) {
	EXPECT_FALSE(IsModuleLoaded(""));
	EXPECT_FALSE(IsModuleLoaded("NonExistentModuleName"));
}

TEST(op2ext, IsIniModuleLoaded) {
	EXPECT_FALSE(IsModuleLoaded(""));
	EXPECT_FALSE(IsModuleLoaded("NonExistentModuleName"));
}

TEST(op2ext, GetLoadedModuleCount) {
	EXPECT_EQ(0u, GetLoadedModuleCount());
}
