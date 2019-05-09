#include "ConsoleModuleLoader.h"
#include "FileSystemHelper.h"
#include "StringConversion.h"
#include <gtest/gtest.h>
#include <string>


TEST(ConsoleModuleLoader, NoModuleLoaded)
{
	ConsoleModuleLoader consoleModLoader("");

	// Returns empty string if no module available
	EXPECT_EQ("", consoleModLoader.GetModuleName());
	EXPECT_EQ("", consoleModLoader.GetModuleDirectory());

	EXPECT_EQ(0u, consoleModLoader.Count());

	// Module name cannot be an empty string
	EXPECT_FALSE(consoleModLoader.IsModuleLoaded(""));
	EXPECT_FALSE(consoleModLoader.IsModuleLoaded("TEST"));

	// No module present to load, functions should return without doing anything
	EXPECT_NO_THROW(consoleModLoader.LoadModule());
	EXPECT_NO_THROW(consoleModLoader.RunModule());
	EXPECT_NO_THROW(consoleModLoader.UnloadModule());
}

TEST(ConsoleModuleLoader, ModuleWithoutDLL)
{
	const auto moduleName("NoDllTest");
	ConsoleModuleLoader consoleModLoader(moduleName);

	const auto moduleDirectory = fs::path(GetGameDirectory()) / moduleName;
	EXPECT_EQ(moduleDirectory, consoleModLoader.GetModuleDirectory());
	EXPECT_EQ(ToLower(moduleName), consoleModLoader.GetModuleName());

	EXPECT_TRUE(consoleModLoader.IsModuleLoaded(moduleName));
	EXPECT_FALSE(consoleModLoader.IsModuleLoaded(""));
	EXPECT_FALSE(consoleModLoader.IsModuleLoaded("UnknownModule"));

	EXPECT_EQ(1u, consoleModLoader.Count());

	// No DLL to load, but will attempt to set memory addresses
	EXPECT_NO_THROW(consoleModLoader.LoadModule());
	EXPECT_NO_THROW(consoleModLoader.RunModule());
	EXPECT_NO_THROW(consoleModLoader.UnloadModule());
}

TEST(ConsoleModuleLoader, ModuleWithEmptyDLL)
{
	const std::string moduleName("InvalidDllTest");
	ConsoleModuleLoader consoleModLoader(moduleName);

	const auto moduleDirectory = fs::path(GetGameDirectory()) / moduleName;
	EXPECT_EQ(moduleDirectory, consoleModLoader.GetModuleDirectory());
	EXPECT_EQ(ToLower(moduleName), consoleModLoader.GetModuleName());

	EXPECT_TRUE(consoleModLoader.IsModuleLoaded(moduleName));
	EXPECT_FALSE(consoleModLoader.IsModuleLoaded(""));
	EXPECT_FALSE(consoleModLoader.IsModuleLoaded("UnknownModule"));

	EXPECT_EQ(1u, consoleModLoader.Count());

	// DLL file is empty and should be aborted
	EXPECT_NO_THROW(consoleModLoader.LoadModule());

	// Functions should return without doing anything since module load is aborted
	EXPECT_NO_THROW(consoleModLoader.RunModule());
	EXPECT_NO_THROW(consoleModLoader.UnloadModule());
}
