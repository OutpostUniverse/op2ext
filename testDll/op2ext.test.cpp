#include "op2ext.h"
#include <gtest/gtest.h>
#include <string_view>
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
