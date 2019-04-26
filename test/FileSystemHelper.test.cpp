#include "FileSystemHelper.h"
#include <gtest/gtest.h>


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

TEST(FileSystemHelper, GetOP2PrivateProfileString) {
	EXPECT_EQ("", GetOP2PrivateProfileString("NonExistentSection", "NonExistentKey"));
}
