#include "GameModules/IpDropDown.h"
#include "ModuleLoader.h"
#include <gtest/gtest.h>
#include <memory>


TEST(ModuleLoader, NoModulesLoaded)
{
	ModuleLoader moduleManager;
	EXPECT_EQ(0, moduleManager.Count());
	EXPECT_EQ("", moduleManager.GetModuleName(0));
	EXPECT_FALSE(moduleManager.IsModuleLoaded("Test"));
	EXPECT_NO_THROW(moduleManager.LoadModules());
	EXPECT_NO_THROW(moduleManager.RunModules());
	EXPECT_NO_THROW(moduleManager.UnloadModules());
}

TEST(ModuleLoader, NullModulePassed)
{
	ModuleLoader moduleManager;
	std::unique_ptr<GameModule> gameModule;
	EXPECT_NO_THROW(moduleManager.RegisterModule(gameModule));
	
	// A null unique pointer does not count as a loaded module
	EXPECT_EQ(0, moduleManager.Count());
}

TEST(ModuleLoader, InternalModulePassed)
{
	ModuleLoader moduleManager;
	std::unique_ptr<GameModule> ipDropDown = std::make_unique<IPDropDown>();

	moduleManager.RegisterModule(ipDropDown);
	
	// Ensure ipDropDown is transfered into moduleManager
	EXPECT_TRUE(ipDropDown == nullptr);
	
	EXPECT_EQ(1, moduleManager.Count());

	// Check module name search is case insensitive
	EXPECT_TRUE(moduleManager.IsModuleLoaded("IPDROPDOWN"));
	EXPECT_FALSE(moduleManager.IsModuleLoaded(""));

	EXPECT_EQ("IPDropDown", moduleManager.GetModuleName(0));

	EXPECT_NO_THROW(moduleManager.LoadModules());
	EXPECT_NO_THROW(moduleManager.RunModules());
	EXPECT_NO_THROW(moduleManager.UnloadModules());
}
