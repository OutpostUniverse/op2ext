#include "ConsoleModuleLoader.h"
#include "FileSystemHelper.h"
#include <gtest/gtest.h>
#include <string>
#include <fstream>


TEST(ConsoleModuleLoader, NoModuleLoaded)
{
	ConsoleModuleLoader consoleModuleLoader({});

	// Throws an exception if the index is out of range
	EXPECT_THROW(consoleModuleLoader.GetModuleName(0), std::runtime_error);
	EXPECT_THROW(consoleModuleLoader.GetModuleDirectory(0), std::runtime_error);

	EXPECT_EQ(0u, consoleModuleLoader.Count());

	// Module name cannot be an empty string
	EXPECT_FALSE(consoleModuleLoader.IsModuleLoaded(""));
	EXPECT_FALSE(consoleModuleLoader.IsModuleLoaded("TEST"));

	// No module present to load, functions should return without doing anything
	EXPECT_NO_THROW(consoleModuleLoader.LoadModules());
	EXPECT_NO_THROW(consoleModuleLoader.RunModules());
	EXPECT_NO_THROW(consoleModuleLoader.UnloadModules());
}

TEST(ConsoleModuleLoader, ModuleWithoutDLL)
{
	const std::string moduleName("NoDllTest");

	// Test will need temporary module directory with no DLL present
	const auto moduleDirectory = fs::path(GetGameDirectory()) / moduleName;
	fs::create_directory(moduleDirectory);

	ConsoleModuleLoader consoleModuleLoader({moduleName});

	EXPECT_EQ(moduleDirectory, consoleModuleLoader.GetModuleDirectory(0));
	EXPECT_EQ(moduleName, consoleModuleLoader.GetModuleName(0));

	EXPECT_TRUE(consoleModuleLoader.IsModuleLoaded(moduleName));
	EXPECT_FALSE(consoleModuleLoader.IsModuleLoaded(""));
	EXPECT_FALSE(consoleModuleLoader.IsModuleLoaded("UnknownModule"));

	EXPECT_EQ(1u, consoleModuleLoader.Count());

	EXPECT_NO_THROW(consoleModuleLoader.LoadModules());
	EXPECT_NO_THROW(consoleModuleLoader.RunModules());
	EXPECT_NO_THROW(consoleModuleLoader.UnloadModules());

	// Cleanup test module directory
	// Use Win API directly since fs::remove doesn't work under Mingw
	EXPECT_NE(0, RemoveDirectoryW(moduleDirectory.wstring().c_str()));
}

TEST(ConsoleModuleLoader, ModuleWithEmptyDLL)
{
	const std::string moduleName("InvalidDllTest");

	// Test will need temporary module directory and invalid DLL file
	const auto moduleDirectory = fs::path(GetGameDirectory()) / moduleName;
	const auto dllFile = moduleDirectory / "op2mod.dll";
	// Create temporary module directory
	fs::create_directory(moduleDirectory);
	// Create empty DLL file
	// Temporary object, immediately destructed, side effect creates file of size 0
	std::ofstream(dllFile.string());

	ConsoleModuleLoader consoleModuleLoader({moduleName});

	EXPECT_EQ(moduleDirectory, consoleModuleLoader.GetModuleDirectory(0));
	EXPECT_EQ(moduleName, consoleModuleLoader.GetModuleName(0));

	EXPECT_TRUE(consoleModuleLoader.IsModuleLoaded(moduleName));
	EXPECT_FALSE(consoleModuleLoader.IsModuleLoaded(""));
	EXPECT_FALSE(consoleModuleLoader.IsModuleLoaded("UnknownModule"));

	EXPECT_EQ(1u, consoleModuleLoader.Count());

	// DLL file is empty and should be aborted
	EXPECT_NO_THROW(consoleModuleLoader.LoadModules());

	// Functions should return without doing anything since module load is aborted
	EXPECT_NO_THROW(consoleModuleLoader.RunModules());
	EXPECT_NO_THROW(consoleModuleLoader.UnloadModules());

	// Cleanup test module DLL and directory
	// Use Win API directly since fs::remove doesn't work under Mingw
	EXPECT_NE(0, DeleteFileW(dllFile.wstring().c_str()));
	EXPECT_NE(0, RemoveDirectoryW(moduleDirectory.wstring().c_str()));
}

TEST(ConsoleModuleLoader, MultiModule) {
	const auto exeDir = fs::path(GetGameDirectory());
	const std::vector<std::string> moduleNames{"Module1", "Module2"};

	// Create some empty test module directories
	for (const auto& moduleName : moduleNames) {
		fs::create_directory(exeDir / moduleName);
	}

	ConsoleModuleLoader consoleModuleLoader(moduleNames);

	EXPECT_EQ(2u, consoleModuleLoader.Count());
	EXPECT_EQ(moduleNames[0], consoleModuleLoader.GetModuleName(0));
	EXPECT_EQ(moduleNames[1], consoleModuleLoader.GetModuleName(1));

	EXPECT_TRUE(consoleModuleLoader.IsModuleLoaded(moduleNames[0]));
	EXPECT_TRUE(consoleModuleLoader.IsModuleLoaded(moduleNames[1]));

	EXPECT_FALSE(consoleModuleLoader.IsModuleLoaded(""));

	EXPECT_NO_THROW(consoleModuleLoader.LoadModules());
	EXPECT_NO_THROW(consoleModuleLoader.RunModules());
	EXPECT_NO_THROW(consoleModuleLoader.UnloadModules());

	// Cleanup test module directories
	for (const auto& moduleName : moduleNames) {
		// Use Win API directly since fs::remove doesn't work under Mingw
		EXPECT_NE(0, RemoveDirectoryW((exeDir / moduleName).wstring().c_str()));
	}
}
