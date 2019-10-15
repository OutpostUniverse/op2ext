#include "FileSystemHelper.h"
#include "FsInclude.h"
#include "IniFile.h"
#include "ModuleLoader.h"
#include <fstream>
#include <string>
#include <string_view>
#include <gtest/gtest.h>


// sectionPairs is a non-line terminated list of keys and values. EG: TestModule = No
void WriteExternalModuleIniFile(const std::string& iniFilename, const std::vector<std::string_view> sectionPairs)
{
	std::ofstream iniFileStream(iniFilename);

	iniFileStream << "[ExternalModules]" << std::endl;

	for (const auto& sectionPair : sectionPairs)
	{
		iniFileStream << sectionPair << std::endl;
	}

	iniFileStream.close();
}

TEST(IniModule, NoDll)
{
	const auto iniFilename = fs::path(GetExeDirectory()) / fs::path("IniModuleTest.ini");
	WriteExternalModuleIniFile(iniFilename.string(), { "Test = yes" });

	ModuleLoader moduleLoader(IniFile(iniFilename.string()), {});

	// No DLL found. An error should post, but program continues to run 
	EXPECT_NO_THROW(moduleLoader.LoadModules());
	EXPECT_EQ(0u, moduleLoader.Count());

	fs::remove(iniFilename);
}

TEST(IniModule, InappropriateValue)
{
	const auto iniFilename = fs::path(GetExeDirectory()) / fs::path("IniModuleTest.ini");
	WriteExternalModuleIniFile(iniFilename.string(), { "Test = InappropriateValue" });

	ModuleLoader moduleLoader(IniFile(iniFilename.string()), {});

	// Inappropriate value for if module should be loaded. An error should post, but program continues to run 
	EXPECT_NO_THROW(moduleLoader.LoadModules());
	EXPECT_EQ(0u, moduleLoader.Count());

	fs::remove(iniFilename);
}
