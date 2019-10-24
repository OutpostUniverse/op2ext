#include "FileSystemHelper.h"
#include "FsInclude.h"
#include <gtest/gtest.h>
#include <fstream>


TEST(FileSystemHelper, GetExeDirectory) {
	auto exeDirectory = fs::path(GetExeDirectory());
	auto exeDirectoryString = exeDirectory.string();
	EXPECT_TRUE(exeDirectory.is_absolute()) << exeDirectoryString + " is not an absolute path.";
	EXPECT_TRUE(Exists(exeDirectoryString)) << exeDirectoryString + " does not exist on filesystem.";
	EXPECT_TRUE(IsDirectory(exeDirectoryString)) << exeDirectoryString + " is not a directory.";
}

TEST(FileSystemHelper, GetOutpost2IniPath) {
	auto iniPath = fs::path(GetOutpost2IniPath());
	EXPECT_EQ("outpost2.ini", iniPath.filename().string()) << iniPath;
}

TEST(FileSystemHelper, GetOutpost2IniSetting)
{
	// Create some test data
	auto iniPath = GetOutpost2IniPath();
	std::ofstream stream(iniPath);
	stream << "[Game]" << std::endl;
	stream << "Music=1" << std::endl;
	stream.close();

	EXPECT_EQ("1", GetOutpost2IniSetting("Game", "Music"));
	
	// Check Case Insensitive
	EXPECT_EQ("1", GetOutpost2IniSetting("game", "MUSIC"));

	EXPECT_EQ("", GetOutpost2IniSetting("BadSectionName", "Music"));
	EXPECT_EQ("", GetOutpost2IniSetting("Game", "BadKey"));
	EXPECT_EQ("", GetOutpost2IniSetting("BadSectionName", "BadKey"));

	// Cleanup test ini file
	fs::remove(iniPath);
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


TEST(FileSystemHelper, FindFilesWithExtension)
{
	const auto exePath = GetExeDirectory();
	EXPECT_EQ(std::vector<std::string>({}), FindFilesWithExtension(exePath, "", ".NonExistentExtension"));
}
