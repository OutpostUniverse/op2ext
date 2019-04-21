#include "IniModuleLoader.h"
#include <gtest/gtest.h>

TEST(IniModuleLoader, InitializeNoModules)
{
	IniModuleLoader iniModuleLoader;
	EXPECT_NO_THROW(iniModuleLoader.LoadModules());
	
	EXPECT_EQ("", iniModuleLoader.GetModuleName(0));
	EXPECT_EQ("", iniModuleLoader.GetModuleName(1));

	EXPECT_TRUE(iniModuleLoader.UnloadModules());
}
