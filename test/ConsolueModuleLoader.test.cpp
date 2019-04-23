#include "ConsoleModuleLoader.h"
#include <gtest/gtest.h>
#include <string>


TEST(ConsoleModuleLoader, NoModuleLoaded)
{
	ConsoleModuleLoader consoleModLoader;

	// Returns empty string if no module available
	EXPECT_EQ("", consoleModLoader.GetModuleName());

	EXPECT_EQ(0, consoleModLoader.Count());
	// Module name cannot be an empty string
	EXPECT_FALSE(consoleModLoader.IsModuleLoaded(""));
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

TEST(ConsoleModuleLoader, IsModuleLoaded)
{
	const std::string testModule("TestModule");
	ConsoleModuleLoader consoleModuleLoader(testModule);

	EXPECT_TRUE(consoleModuleLoader.IsModuleLoaded(testModule));

	EXPECT_FALSE(consoleModuleLoader.IsModuleLoaded(""));
	EXPECT_FALSE(consoleModuleLoader.IsModuleLoaded("UnknownModule"));
}

TEST(ConsoleModuleLoader, Count)
{
	ConsoleModuleLoader consoleModuleLoader("TestModule");
	EXPECT_EQ(1, consoleModuleLoader.Count());
}
