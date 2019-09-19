#include "ConsoleArgumentParser.h"
#include <gtest/gtest.h>
#include <vector>
#include <string>


TEST(ConsoleArgumentParser, NoArgument) {
	EXPECT_EQ("", FindModuleDirectory(std::vector<std::string> { }));
}

TEST(ConsoleArgumentParser, WellFormedNoSpaces)
{
	const std::string path("path");
	EXPECT_EQ(path, FindModuleDirectory(std::vector<std::string> { "/loadmod", path }));
}

TEST(ConsoleArgmunetParser, WellFormedSpaces)
{
	const std::string pathWithSpaces("path with spaces");
	EXPECT_EQ(pathWithSpaces, FindModuleDirectory(std::vector<std::string> { "/loadmod", pathWithSpaces }));
}


TEST(ConsoleArgumentParser, WrongSwitchName) {
	EXPECT_THROW(FindModuleDirectory(std::vector<std::string> { "/WrongSwitch" }), std::runtime_error);
}

TEST(ConsoleArgumentParser, NoSwitchArgument) {
	EXPECT_THROW(FindModuleDirectory(std::vector<std::string> { "/loadmod" }), std::runtime_error);
}

TEST(ConsoleArgumentParser, TooManyArguments) {
	EXPECT_THROW(FindModuleDirectory(std::vector<std::string> { "/loadmod", "path1", "path2" }), std::runtime_error);
}
