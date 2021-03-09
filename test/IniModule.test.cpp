#include "FileSystemHelper.h"
#include "FsInclude.h"
#include "IniFile.h"
#include "ModuleLoader.h"
#include "LogMessageTest.h"
#include <fstream>
#include <string>
#include <string_view>
#include <gtest/gtest.h>


class IniModuleTest : public LogMessageTest {
protected:
	void TearDown() {
		fs::remove(iniFilename);
		LogMessageTest::TearDown();
	}

	// sectionPairs is a non-line terminated list of keys and values. EG: TestModule = No
	void WriteExternalModuleSectionToIniFile(const std::vector<std::string_view>& sectionPairs)
	{
		std::ofstream iniFileStream(iniFilename.string());

		iniFileStream << "[ExternalModules]" << std::endl;
		for (const auto& sectionPair : sectionPairs) {
			iniFileStream << sectionPair << std::endl;
		}
	}

	const fs::path iniFilename = fs::path(GetExeDirectory()) / fs::path("IniModuleTest.ini");
};


TEST_F(IniModuleTest, NoDll)
{
	// Add active module named test to ini file
	WriteExternalModuleSectionToIniFile({ "Test = yes" });

	// Register a non-existent dll filename to the test module in the ini file
	std::ofstream iniFileStream(iniFilename.string(), std::ios_base::app);
	iniFileStream << std::endl;
	iniFileStream << "[Test]" << std::endl;
	iniFileStream << "Dll = Missing.dll" << std::endl;

	ModuleLoader moduleLoader(IniFile(iniFilename.string()), {});

	// No DLL found. An error should post, but program continues to run 
	EXPECT_CALL(loggerError, Log(::testing::HasSubstr("Unable to load dll for module"))).Times(1);
	EXPECT_NO_THROW(moduleLoader.LoadModules());
}

TEST_F(IniModuleTest, InappropriateValue)
{
	WriteExternalModuleSectionToIniFile({ "Test = InappropriateValue" });

	ModuleLoader moduleLoader(IniFile(iniFilename.string()), {});

	// Inappropriate value for if module should be loaded. An error should post, but program continues to run 
	EXPECT_CALL(loggerError, Log(::testing::HasSubstr("contains an innapropriate setting of"))).Times(1);
	EXPECT_NO_THROW(moduleLoader.LoadModules());
}
