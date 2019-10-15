#include "FileSystemHelper.h"
#include "FsInclude.h"
#include "IniFile.h"
#include "ModuleLoader.h"
#include "TestLog.h"
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
	ResetTestLog();

	const auto iniFilename = fs::path(GetExeDirectory()) / fs::path("IniModuleTest.ini");
	WriteExternalModuleIniFile(iniFilename.string(), { "Test = yes" });

	ModuleLoader moduleLoader(IniFile(iniFilename.string()), {});

	// No DLL found. An error should post, but program continues to run 
	EXPECT_NO_THROW(moduleLoader.LoadModules());
	EXPECT_EQ(0u, moduleLoader.Count());

	EXPECT_EQ(1u, GetErrorLogger().Count());
	EXPECT_TRUE(GetErrorLogger().Pop("Unable to load dll for module"));

	fs::remove(iniFilename);
}

TEST(IniModule, InappropriateValue)
{
	ResetTestLog();

	const auto iniFilename = fs::path(GetExeDirectory()) / fs::path("IniModuleTest.ini");
	WriteExternalModuleIniFile(iniFilename.string(), { "Test = InappropriateValue" });

	ModuleLoader moduleLoader(IniFile(iniFilename.string()), {});

	// Inappropriate value for if module should be loaded. An error should post, but program continues to run 
	EXPECT_NO_THROW(moduleLoader.LoadModules());
	EXPECT_EQ(0u, moduleLoader.Count());

	EXPECT_EQ(1u, GetErrorLogger().Count());
	EXPECT_TRUE(GetErrorLogger().Pop("contains an innapropriate setting of"));

	fs::remove(iniFilename);
}
