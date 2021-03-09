#include "ModuleLoader.h"
#include "FileSystemHelper.h"
#include "FsInclude.h"
#include "IniFile.h"
#include <windows.h>
#include <gtest/gtest.h>
#include <string>
#include <fstream>
#include <stdexcept>


TEST(ConsoleModuleLoader, ModuleWithoutDLL)
{
	const std::string moduleName("NoDllTest");

	// Test will need temporary module directory with no DLL present
	// Ensure module directory ends with a trailing slash

	const auto moduleDirectory = fs::path(GetOpuDirectory()) / moduleName;
	fs::create_directories(moduleDirectory);

	const std::string iniFileName{ GetExeDirectory() + "TestIniFile.NonExistentData.ini" };
	IniFile iniFile(iniFileName);
	ModuleLoader moduleLoader(iniFile, {moduleName});
	
	EXPECT_NO_THROW(moduleLoader.LoadModules());
	EXPECT_NO_THROW(moduleLoader.RunModules());

	EXPECT_TRUE(moduleLoader.IsModuleLoaded(moduleName));
	EXPECT_FALSE(moduleLoader.IsModuleLoaded(""));
	EXPECT_FALSE(moduleLoader.IsModuleLoaded("UnknownModule"));

	// Because IsModuleLoaded returned true previously, moduleName will find a match
	for (std::size_t i = 0; i < moduleLoader.Count(); ++i) {
		if (moduleName == moduleLoader.GetModuleName(i)) {
			EXPECT_EQ(moduleName + "\\", moduleLoader.GetModuleDirectory(i));
			break;
		}
	}

	EXPECT_NO_THROW(moduleLoader.UnloadModules());

	// Cleanup test module directory
	// Use Win API directly since fs::remove doesn't work under Mingw
	EXPECT_NE(0, RemoveDirectoryW(moduleDirectory.wstring().c_str()));
}

TEST(ConsoleModuleLoader, ModuleWithEmptyDLL)
{
	const std::string moduleName("InvalidDllTest");

	// Test will need temporary module directory and invalid DLL file
	// Ensure module directory ends with a trailing slash
	const auto moduleDirectory = fs::path(GetOpuDirectory()) / moduleName;
	const auto dllFile = moduleDirectory / "op2mod.dll";
	
	// Create temporary module directory
	fs::create_directories(moduleDirectory);

	// Create empty DLL file
	// Temporary object, immediately destructed, side effect creates file of size 0
	std::ofstream(dllFile.string());

	const std::string iniFileName{ GetExeDirectory() + "TestIniFile.NonExistentData.ini" };
	IniFile iniFile(iniFileName);
	ModuleLoader moduleLoader(iniFile, {moduleName});

	// Check that an invalid module does not throw an exception when loaded
	EXPECT_NO_THROW(moduleLoader.LoadModules());
	EXPECT_NO_THROW(moduleLoader.RunModules());

	// Ensure module does not exist
	EXPECT_FALSE(moduleLoader.IsModuleLoaded(moduleName));

	EXPECT_NO_THROW(moduleLoader.UnloadModules());

	// Cleanup test module DLL and directory
	// Use Win API directly since fs::remove doesn't work under Mingw
	EXPECT_NE(0, DeleteFileW(dllFile.wstring().c_str()));
	EXPECT_NE(0, RemoveDirectoryW(moduleDirectory.wstring().c_str()));
}

TEST(ConsoleModuleLoader, MultiModule) {
	const auto opuDirectory = fs::path(GetOpuDirectory());
	const std::vector<std::string> moduleNames{"Module1", "Module2"};

	// Create some empty test module directories
	for (const auto& moduleName : moduleNames) {
		fs::create_directories(opuDirectory / moduleName);
	}

	const std::string iniFileName{ GetExeDirectory() + "TestIniFile.NonExistentData.ini" };
	IniFile iniFile(iniFileName);
	ModuleLoader moduleLoader(iniFile, moduleNames);

	EXPECT_NO_THROW(moduleLoader.LoadModules());
	EXPECT_NO_THROW(moduleLoader.RunModules());

	// Ensure both modules are loaded. (More modules could load if built in modules are present)
	EXPECT_GE(moduleLoader.Count(), 2u);

	EXPECT_TRUE(moduleLoader.IsModuleLoaded(moduleNames[0]));
	EXPECT_TRUE(moduleLoader.IsModuleLoaded(moduleNames[1]));

	EXPECT_FALSE(moduleLoader.IsModuleLoaded(""));

	EXPECT_NO_THROW(moduleLoader.UnloadModules());

	// Cleanup test module directories
	for (const auto& moduleName : moduleNames) {
		// Use Win API directly since fs::remove doesn't work under Mingw
		EXPECT_NE(0, RemoveDirectoryW((opuDirectory / moduleName).wstring().c_str()));
	}
}
