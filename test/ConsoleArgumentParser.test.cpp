#include "ConsoleArgumentParser.h"
#include <gtest/gtest.h>
#include <vector>
#include <string>


TEST(ConsoleArgumentParser, NoArgument) {
	EXPECT_TRUE(FindModuleDirectories({ }).empty());
}

TEST(ConsoleArgumentParser, WellFormedNoSpaces)
{
	const std::string path("path");
	const auto directories = FindModuleDirectories({ "/loadmod", path });

	EXPECT_EQ(1u, directories.size());
	EXPECT_EQ(path, directories[0]);
}

TEST(ConsoleArgmunetParser, WellFormedSpaces)
{
	const std::string path("path with spaces");
	const auto directories = FindModuleDirectories({ "/loadmod", path });
	
	EXPECT_EQ(1u, directories.size());
	EXPECT_EQ(path, directories[0]);
}


TEST(ConsoleArgumentParser, WrongSwitchName) {
	EXPECT_THROW(FindModuleDirectories({ "/WrongSwitch" }), std::runtime_error);
}

TEST(ConsoleArgumentParser, NoSwitchArgument) {
	EXPECT_THROW(FindModuleDirectories({ "/loadmod" }), std::runtime_error);
}

TEST(ConsoleArgumentParser, MultipleArguments) {
	const std::string path1("path1");
	const std::string path2("path2");

	const auto directories = FindModuleDirectories({ "/loadmod", path1, path2 });

	EXPECT_EQ(2u, directories.size());
	EXPECT_EQ(path1, directories[0]);
	EXPECT_EQ(path2, directories[1]);
}
