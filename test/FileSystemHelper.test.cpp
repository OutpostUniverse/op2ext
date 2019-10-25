#include "FileSystemHelper.h"
#include "FsInclude.h"
#include <gtest/gtest.h>
#include <fstream>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>


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
	const auto exeFolder = GetExeDirectory();
	const auto testFolder = std::string("FindFilesTest/");
	const auto testPath = fs::path(exeFolder) / testFolder;

	// Create test folder, and fill it with test subfolders and files
	fs::create_directories(testPath);
	fs::create_directories(testPath / "FakeVolDir.vol/");
	std::ofstream((testPath / "Empty.vol").string());
	std::ofstream((testPath / "FilenameWithNoExtension").string());

	// No unexpected files
	EXPECT_EQ(std::vector<std::string>({}), FindFilesWithExtension(exeFolder, testFolder, ".NonExistentExtension"));
	// No directories
	EXPECT_EQ(std::vector<std::string>({}), FindFilesWithExtension(exeFolder, testFolder, "./"));
	// Files with no extension
	EXPECT_EQ(std::vector<std::string>({testFolder + "FilenameWithNoExtension"}), FindFilesWithExtension(exeFolder, testFolder, ""));
	// Files with given extension (but not directories)
	EXPECT_EQ(std::vector<std::string>({testFolder + "Empty.vol"}), FindFilesWithExtension(exeFolder, testFolder, ".vol"));

	// Cleanup (Mingw filesystem library has trouble removing directories)
	fs::remove(testPath / "Empty.vol");
	fs::remove(testPath / "FilenameWithNoExtension");
	RemoveDirectoryW((testPath / "FakeVolDir.vol").wstring().c_str());
	RemoveDirectoryW((testPath).wstring().c_str());
}
