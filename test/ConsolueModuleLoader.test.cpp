#include "ConsoleModuleLoader.h"
#include <gtest/gtest.h>

TEST(ConsoleModuleLoader, GetModuleDirectory)
{
	ConsoleModuleLoader consoleModLoader;

	// Returns empty string if no module available
	EXPECT_EQ(consoleModLoader.GetModuleDirectory(), "");
}

TEST(ConsoleModuleLoader, GetModuleName)
{
	ConsoleModuleLoader consoleModLoader;

	// Returns empty string if no module available
	EXPECT_EQ(consoleModLoader.GetModuleName(), "");
}
