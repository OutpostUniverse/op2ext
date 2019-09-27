#include "ConsoleModuleLoader.h"
#include "FileSystemHelper.h"
#include <gtest/gtest.h>
#include <string>


TEST(ConsoleModuleLoader, NoModuleLoaded)
{
	ConsoleModuleLoader consoleModLoader("");

	// Throws an exception if the index is out of range
	EXPECT_THROW(consoleModLoader.GetModuleName(0), std::runtime_error);
	EXPECT_THROW(consoleModLoader.GetModuleDirectory(0), std::runtime_error);

	EXPECT_EQ(0u, consoleModLoader.Count());

	// Module name cannot be an empty string
	EXPECT_FALSE(consoleModLoader.IsModuleLoaded(""));
	EXPECT_FALSE(consoleModLoader.IsModuleLoaded("TEST"));

	// No module present to load, functions should return without doing anything
	EXPECT_NO_THROW(consoleModLoader.LoadModules());
	EXPECT_NO_THROW(consoleModLoader.RunModules());
	EXPECT_NO_THROW(consoleModLoader.UnloadModules());
}

TEST(ConsoleModuleLoader, ModuleWithoutDLL)
{
	const auto moduleName("NoDllTest");
	ConsoleModuleLoader consoleModLoader(moduleName);

	const auto moduleDirectory = fs::path(GetGameDirectory()) / moduleName;
	EXPECT_EQ(moduleDirectory, consoleModLoader.GetModuleDirectory(0));
	EXPECT_EQ(moduleName, consoleModLoader.GetModuleName(0));

	EXPECT_TRUE(consoleModLoader.IsModuleLoaded(moduleName));
	EXPECT_FALSE(consoleModLoader.IsModuleLoaded(""));
	EXPECT_FALSE(consoleModLoader.IsModuleLoaded("UnknownModule"));

	EXPECT_EQ(1u, consoleModLoader.Count());

	EXPECT_NO_THROW(consoleModLoader.LoadModules());
	EXPECT_NO_THROW(consoleModLoader.RunModules());
	EXPECT_NO_THROW(consoleModLoader.UnloadModules());
}

TEST(ConsoleModuleLoader, ModuleWithEmptyDLL)
{
	const std::string moduleName("InvalidDllTest");
	ConsoleModuleLoader consoleModLoader(moduleName);

	const auto moduleDirectory = fs::path(GetGameDirectory()) / moduleName;
	EXPECT_EQ(moduleDirectory, consoleModLoader.GetModuleDirectory(0));
	EXPECT_EQ(moduleName, consoleModLoader.GetModuleName(0));

	EXPECT_TRUE(consoleModLoader.IsModuleLoaded(moduleName));
	EXPECT_FALSE(consoleModLoader.IsModuleLoaded(""));
	EXPECT_FALSE(consoleModLoader.IsModuleLoaded("UnknownModule"));

	EXPECT_EQ(1u, consoleModLoader.Count());

	// DLL file is empty and should be aborted
	EXPECT_NO_THROW(consoleModLoader.LoadModules());

	// Functions should return without doing anything since module load is aborted
	EXPECT_NO_THROW(consoleModLoader.RunModules());
	EXPECT_NO_THROW(consoleModLoader.UnloadModules());
}
