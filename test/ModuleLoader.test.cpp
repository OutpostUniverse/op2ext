#include "GameModules/IpDropDown.h"
#include "GameModules/IniModule.h"
#include "ModuleLoader.h"
#include <gtest/gtest.h>
#include <memory>
#include <stdexcept>

// Use to test that two modules with the same name but different casing is rejected from registering with the ModuleLoader
// Windows file system and .ini key value pairs are case insensitive
class DifferentCasedNameModule : public GameModule
{
public:
	DifferentCasedNameModule(const std::string& name) : GameModule(name) {}

	void Load() override {}
	bool Unload() override { return true; }
};

TEST(ModuleLoader, NoModulesLoaded)
{
	ModuleLoader moduleManager;
	EXPECT_EQ(0u, moduleManager.Count());
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
	EXPECT_EQ(0u, moduleManager.Count());
}

TEST(ModuleLoader, InternalModulePassed)
{
	ModuleLoader moduleManager;
	std::unique_ptr<GameModule> ipDropDown = std::make_unique<IPDropDown>();

	moduleManager.RegisterModule(ipDropDown);
	
	// Ensure ipDropDown is transfered into moduleManager
	EXPECT_TRUE(ipDropDown == nullptr);
	
	EXPECT_EQ(1u, moduleManager.Count());

	// Check module name search is case insensitive
	EXPECT_TRUE(moduleManager.IsModuleLoaded("IPDROPDOWN"));
	EXPECT_FALSE(moduleManager.IsModuleLoaded(""));

	EXPECT_EQ("IPDropDown", moduleManager.GetModuleName(0));

	EXPECT_NO_THROW(moduleManager.LoadModules());
	EXPECT_NO_THROW(moduleManager.RunModules());
	EXPECT_NO_THROW(moduleManager.UnloadModules());
}

TEST(ModuleLoader, RejectCaseSensitiveNames)
{
	ModuleLoader moduleLoader;

	EXPECT_NO_THROW(moduleLoader.RegisterModule(static_cast<std::unique_ptr<GameModule>>(std::make_unique<DifferentCasedNameModule>("TestModule"))));
	EXPECT_EQ(1u, moduleLoader.Count());

	// Ensure ModuleManager does not allow multiple modules with same name but different casing
	EXPECT_NO_THROW(moduleLoader.RegisterModule(static_cast<std::unique_ptr<GameModule>>(std::make_unique<DifferentCasedNameModule>("testmodule"))));
	EXPECT_EQ(1u, moduleLoader.Count());
}
