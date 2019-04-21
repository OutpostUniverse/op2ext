#include "ConsoleModuleLoader.h"
#include <gtest/gtest.h>
#include <string>


TEST(ConsoleModuleLoader, NoModuleLoaded)
{
	ConsoleModuleLoader consoleModLoader;

	// Returns empty string if no module available
	EXPECT_EQ("", consoleModLoader.GetModuleName());

	EXPECT_TRUE(consoleModLoader.IsModuleLoaded(""));
	EXPECT_FALSE(consoleModLoader.IsModuleLoaded("TEST"));
}

TEST(ConsoleModuleLoader, GetModuleDirectory)
{
	const std::string testModule("TestModule");
	ConsoleModuleLoader consoleModLoader(testModule);

	EXPECT_NE("", consoleModLoader.GetModuleDirectory());
	EXPECT_EQ(testModule, consoleModLoader.GetModuleDirectory());
}

TEST(ConsoleModuleLoader, GetModuleName)
{
	const std::string testModule("testmodule");
	ConsoleModuleLoader consoleModLoader(testModule);

	EXPECT_NE("", consoleModLoader.GetModuleName());
	EXPECT_EQ(testModule, consoleModLoader.GetModuleName());
}
