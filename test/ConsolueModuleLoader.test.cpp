#include "ConsoleModuleLoader.h"
#include <gtest/gtest.h>


TEST(ConsoleModuleLoader, NoModuleLoaded)
{
	ConsoleModuleLoader consoleModLoader;

	// Returns empty string if no module available
	EXPECT_EQ(consoleModLoader.GetModuleName(), "");

	EXPECT_TRUE(consoleModLoader.IsModuleLoaded(""));
	EXPECT_FALSE(consoleModLoader.IsModuleLoaded("TEST"));
}
