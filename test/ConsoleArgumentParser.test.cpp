#include "op2ext-Internal.h"
#include "ConsoleArgumentParser.h"
#include "FileSystemHelper.h"
#include <gtest/gtest.h>
#include <vector>
#include <string>
#include <cstdint>
#include <functional>

::testing::AssertionResult FindModuleDirectoryIsLogged(const std::vector<std::string>& arguments, const std::string& expectResult);


TEST(ConsoleArgumentParser, NoArgument) {
	EXPECT_FALSE(FindModuleDirectoryIsLogged(std::vector<std::string> { }, ""));
}

TEST(ConsoleArgumentParser, WellFormedNoSpaces)
{
	const std::string path("path");
	EXPECT_FALSE(FindModuleDirectoryIsLogged(std::vector<std::string> { "/loadmod", path}, path));
}

TEST(ConsoleArgmunetParser, WellFormedSpaces)
{
	const std::string pathWithSpaces("path with spaces");
	EXPECT_FALSE(FindModuleDirectoryIsLogged(std::vector<std::string> { "/loadmod", pathWithSpaces }, pathWithSpaces));
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


// Returns true if calling FindModuleDirectory provides input to the logger by checking log size
::testing::AssertionResult FindModuleDirectoryIsLogged(
	const std::vector<std::string>& arguments, const std::string& expectResult)
{
	const auto logPath = fs::path(GetGameDirectory()).append("Outpost2Log.txt");
	const uintmax_t preFileSize = fs::file_size(logPath);

	EXPECT_EQ(expectResult, FindModuleDirectory(arguments));

	if (fs::file_size(logPath) > preFileSize) {
		return ::testing::AssertionSuccess() << "Message logged";
	}

	return ::testing::AssertionFailure() << "Message not logged";
}
