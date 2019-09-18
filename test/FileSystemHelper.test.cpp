#include "FileSystemHelper.h"
#include <gtest/gtest.h>


TEST(FileSystemHelper, GetGameDirectory) {
	auto gameDirectory = fs::path(GetGameDirectory());
	// Work around MinGW failures for paths that end with a directory separator
	gameDirectory += ".";
	EXPECT_TRUE(gameDirectory.is_absolute()) << gameDirectory.string() + " is not an absolute path.";
	EXPECT_TRUE(fs::exists(gameDirectory)) << gameDirectory.string() + " does not exist on filesystem.";
	EXPECT_TRUE(fs::is_directory(gameDirectory)) << gameDirectory.string() + " is not a directory.";
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
