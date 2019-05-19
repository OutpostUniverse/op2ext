#include <ConsoleArgumentParser.h>
#include <gtest/gtest.h>
#include <vector>
#include <string>

TEST(ConsoleArgumentParser, NoArgument) {
	const std::vector<std::string> arguments;
	EXPECT_EQ("", FindModuleDirectory(arguments));
}

TEST(ConsoleArgumentParser, WrongSwitchName) {
	const std::vector<std::string> arguments{ "/WrongSwitch" };
	EXPECT_EQ("", FindModuleDirectory(arguments));
}

TEST(ConsoleArgumentParser, NoSwitchArgument) {
	const std::vector<std::string> arguments{ "/loadmod" };
	EXPECT_EQ("", FindModuleDirectory(arguments));
}

TEST(ConsoleArgumentParser, TooManyArguments) {
	const std::vector<std::string> arguments{ "/loadmod", "path1", "path2" };
	EXPECT_EQ("", FindModuleDirectory(arguments));
}

TEST(ConsoleArgumentParser, WellFormedNoSpaces) {
	const std::string path("path");
	const std::vector<std::string> arguments{ "/loadmod", path };
	EXPECT_EQ(path, FindModuleDirectory(arguments));
}

TEST(ConsoleArgmunetParser, WellFormedSpaces) {
	const std::string path("paths with spaces");
	const std::vector<std::string> arguments{ "/loadmod", path };
	EXPECT_EQ(path, FindModuleDirectory(arguments));
}
