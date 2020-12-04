#include "ConsoleArgumentParser.h"
#include <gtest/gtest.h>
#include <vector>
#include <string>


TEST(ConsoleArgumentParser, GetOptionNoArguments) {
	const std::vector<std::string> arguments;

	EXPECT_EQ(arguments.end(), GetCommandOption(arguments.begin(), arguments.end(), "option"));
}

TEST(ConsoleArgumentParser, OptionExistsNoArguments) {
	const std::vector<std::string> arguments;

	EXPECT_FALSE(CommandOptionExists(arguments.begin(), arguments.end(), "option"));
}

TEST(ConsoleArgumentParser, GetOptionWellFormed) {
	const std::vector<std::string> arguments = { "/option1", "a", "-option2", "b c" };
	const auto it = GetCommandOption(arguments.begin(), arguments.end(), "option1");

	EXPECT_EQ("a",            *it);
	EXPECT_EQ("b c",          *GetCommandOption(it,                arguments.end(), "option2"));
	EXPECT_EQ(arguments.end(), GetCommandOption(arguments.begin(), arguments.end(), "option3"));
}

TEST(ConsoleArgumentParser, OptionExistsWellFormed) {
	const std::vector<std::string> arguments = { "/option1", "-option2" };

	EXPECT_TRUE(CommandOptionExists(arguments.begin(),  arguments.end(), "option1"));
	EXPECT_TRUE(CommandOptionExists(arguments.begin(),  arguments.end(), "option2"));
	EXPECT_FALSE(CommandOptionExists(arguments.begin(), arguments.end(), "option3"));
}

TEST(ConsoleArgumentParser, GetOptionNoSwitchArgument) {
	const std::vector<std::string> arguments = { "/option" };

	EXPECT_TRUE(CommandOptionExists(arguments.begin(), arguments.end(), "option"));
	EXPECT_EQ(arguments.end(), GetCommandOption(arguments.begin(), arguments.end(), "option"));
}

TEST(ConsoleArgumentParser, GetOptionNoSwitchPrefix) {
	const std::vector<std::string> arguments = { "option", "argument" };

	EXPECT_EQ(arguments.end(), GetCommandOption(arguments.begin(), arguments.end(), "option"));
}

TEST(ConsoleArgumentParser, OptionExistsNoSwitchPrefix) {
	const std::vector<std::string> arguments = { "option" };

	EXPECT_FALSE(CommandOptionExists(arguments.begin(), arguments.end(), "option"));
}


TEST(ConsoleArgumentParser, FindDirectoriesNoArgument) {
	EXPECT_TRUE(FindModuleDirectories({ }).empty());
}

TEST(ConsoleArgumentParser, FindDirectoriesWellFormedNoSpaces)
{
	const std::string path("path");
	const auto directories = FindModuleDirectories({ "/loadmod", path });

	EXPECT_EQ(1u, directories.size());
	EXPECT_EQ(path, directories[0]);
}

TEST(ConsoleArgumentParser, FindDirectoriesWellFormedSpaces)
{
	const std::string path("path with spaces");
	const auto directories = FindModuleDirectories({ "/loadmod", path });
	
	EXPECT_EQ(1u, directories.size());
	EXPECT_EQ(path, directories[0]);
}


TEST(ConsoleArgumentParser, FindDirectoriesWrongSwitchName) {
	EXPECT_TRUE(FindModuleDirectories({ "/WrongSwitch" }).empty());
}

TEST(ConsoleArgumentParser, FindDirectoriesNoSwitchArgument) {
	EXPECT_TRUE(FindModuleDirectories({ "/loadmod" }).empty());
}

TEST(ConsoleArgumentParser, FindDirectoriesRepeatedNoSwitchArgument) {
	EXPECT_TRUE(FindModuleDirectories({ "/loadmod", "/loadmod", "-loadmod", "-loadmod" }).empty());
}

TEST(ConsoleArgumentParser, FindDirectoriesMultipleArguments) {
	const std::string path1("path1");
	const std::string path2("path2");

	const auto directories = FindModuleDirectories({ "/loadmod", path1, "/loadmod", path2 });

	EXPECT_EQ(2u, directories.size());
	EXPECT_EQ(path1, directories[0]);
	EXPECT_EQ(path2, directories[1]);
}
