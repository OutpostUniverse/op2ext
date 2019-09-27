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
