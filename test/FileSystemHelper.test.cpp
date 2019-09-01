#include "FileSystemHelper.h"
#include <gtest/gtest.h>
// #include <string>


TEST(FileSystemHelper, GetGameDirectory) {
	auto gameDirectory = fs::path(GetGameDirectory());
	EXPECT_TRUE(gameDirectory.is_absolute()) << gameDirectory;
	EXPECT_TRUE(fs::exists(gameDirectory)) << gameDirectory;
	EXPECT_TRUE(fs::is_directory(gameDirectory)) << gameDirectory;
}

TEST(FileSystemHelper, GetOutpost2IniPath) {
	auto iniPath = fs::path(GetOutpost2IniPath());
	EXPECT_EQ("outpost2.ini", iniPath.filename().string()) << iniPath;
}

TEST(FileSystemHelper, GetOutpost2IniSetting)
{
	EXPECT_EQ("1", GetOutpost2IniSetting("Game", "Music"));
	
	// Check Case Insensitive
	EXPECT_EQ("1", GetOutpost2IniSetting("game", "MUSIC"));

	EXPECT_EQ("", GetOutpost2IniSetting("BadSectionName", "Music"));
	EXPECT_EQ("", GetOutpost2IniSetting("Game", "BadKey"));
	EXPECT_EQ("", GetOutpost2IniSetting("BadSectionName", "BadKey"));
}
