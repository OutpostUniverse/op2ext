#include "ModuleLoader.h"
#include <gtest/gtest.h>

TEST(ModuleLoader, InitializeNoModules)
{
	ModuleLoader moduleLoader;
	EXPECT_NO_THROW(moduleLoader.LoadModules());
	
	EXPECT_EQ("", moduleLoader.GetModuleName(0));
	EXPECT_EQ("", moduleLoader.GetModuleName(1));

	EXPECT_FALSE(moduleLoader.IsModuleLoaded(""));
	EXPECT_FALSE(moduleLoader.IsModuleLoaded("TEST"));

	EXPECT_EQ(0u, moduleLoader.Count());

	EXPECT_TRUE(moduleLoader.UnloadModules());
}
