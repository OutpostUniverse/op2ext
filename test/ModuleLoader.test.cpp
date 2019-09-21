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
	ModuleLoader moduleLoader;
	EXPECT_EQ(0u, moduleLoader.Count());
	EXPECT_THROW(moduleLoader.GetModuleName(0), std::out_of_range);
	EXPECT_FALSE(moduleLoader.IsModuleLoaded("Test"));
	EXPECT_NO_THROW(moduleLoader.LoadModules());
	EXPECT_NO_THROW(moduleLoader.RunModules());
	EXPECT_NO_THROW(moduleLoader.UnloadModules());
}

TEST(ModuleLoader, NullModulePassed)
{
	ModuleLoader moduleLoader;
	std::unique_ptr<GameModule> gameModule;
	EXPECT_NO_THROW(moduleLoader.RegisterModule(std::move(gameModule)));
	
	// A null unique pointer does not count as a loaded module
	EXPECT_EQ(0u, moduleLoader.Count());
}

TEST(ModuleLoader, BuiltInModulePassed)
{
	ModuleLoader moduleLoader;
	std::unique_ptr<GameModule> ipDropDown = std::make_unique<IPDropDown>();

	moduleLoader.RegisterModule(std::move(ipDropDown));
	
	// Ensure ipDropDown is transfered into moduleLoader
	EXPECT_TRUE(ipDropDown == nullptr);
	
	EXPECT_EQ(1u, moduleLoader.Count());

	// Check module name search is case insensitive
	EXPECT_TRUE(moduleLoader.IsModuleLoaded("IPDROPDOWN"));
	EXPECT_FALSE(moduleLoader.IsModuleLoaded(""));

	EXPECT_EQ("IPDropDown", moduleLoader.GetModuleName(0));

	EXPECT_NO_THROW(moduleLoader.LoadModules());
	EXPECT_NO_THROW(moduleLoader.RunModules());
	EXPECT_NO_THROW(moduleLoader.UnloadModules());
}

TEST(ModuleLoader, RejectCaseInsensitiveDuplicateNames)
{
	ModuleLoader moduleLoader;

	EXPECT_NO_THROW(moduleLoader.RegisterModule(std::make_unique<DifferentCasedNameModule>("TestModule")));
	EXPECT_EQ(1u, moduleLoader.Count());

	// Ensure ModuleManager does not allow multiple modules with same name but different casing
	EXPECT_NO_THROW(moduleLoader.RegisterModule(std::make_unique<DifferentCasedNameModule>("testmodule")));
	EXPECT_EQ(1u, moduleLoader.Count());
}
