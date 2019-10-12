#include "FileSystemHelper.h"
#include "FsInclude.h"
#include <gtest/gtest.h>


TEST(FileSystemHelper, GetExeDirectory) {
	auto gameDirectory = fs::path(GetExeDirectory());
	auto gameDirectoryString = gameDirectory.string();
	EXPECT_TRUE(gameDirectory.is_absolute()) << gameDirectoryString + " is not an absolute path.";
	EXPECT_TRUE(Exists(gameDirectoryString)) << gameDirectoryString + " does not exist on filesystem.";
	EXPECT_TRUE(IsDirectory(gameDirectoryString)) << gameDirectoryString + " is not a directory.";
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


TEST(FileSystemHelper, IsDirectory)
{
	EXPECT_TRUE(IsDirectory("/"));
	EXPECT_TRUE(IsDirectory("./"));

	EXPECT_FALSE(IsDirectory("NonExistentPath"));
	EXPECT_FALSE(IsDirectory("NonExistentPath/"));
}

TEST(FileSystemHelper, Exists)
{
	EXPECT_TRUE(Exists("/"));
	EXPECT_TRUE(Exists("./"));

	EXPECT_FALSE(Exists("NonExistentPath"));
	EXPECT_FALSE(Exists("NonExistentPath/"));
}
