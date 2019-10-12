#include "IniFile.h"
#include "FileSystemHelper.h"
#include <gtest/gtest.h>


TEST(IniFile, NonExistentDataRead) {
	const std::string iniFileName{GetExeDirectory() + "TestIniFile.NonExistentData.ini"};
	IniFile iniFile(iniFileName);

	// Returns the full path
	EXPECT_EQ(iniFileName, iniFile.FileName());

	EXPECT_EQ("", iniFile.GetValue("", ""));
	EXPECT_EQ("", iniFile.GetValue("NonExistentSection", "NonExistentKey"));

	EXPECT_NO_THROW(iniFile.ClearSection(""));
	EXPECT_NO_THROW(iniFile.ClearSection("NonExistentSection"));

	EXPECT_NO_THROW(iniFile.ClearKey("", ""));
	EXPECT_NO_THROW(iniFile.ClearKey("", "NonExistentKey"));
	EXPECT_NO_THROW(iniFile.ClearKey("NonExistentSection", ""));
	EXPECT_NO_THROW(iniFile.ClearKey("NonExistentSection", "NonExistentKey"));

	auto iniSection = iniFile["NonExistentSection"];

	// Returns the full path
	EXPECT_EQ(iniFileName, iniFile.FileName());

	EXPECT_EQ("NonExistentSection", iniSection.SectionName());

	EXPECT_EQ("", iniSection[""]);
	EXPECT_EQ("", iniSection["NonExistentKey"]);
}

TEST(IniFile, StaticMethodsWriteRead) {
	const std::string iniFileName{GetExeDirectory() + "TestIniFile.StaticMethods.ini"};

	// Initially no data
	EXPECT_EQ("", IniFile::GetValue(iniFileName, "SectionName", "KeyName1"));
	EXPECT_EQ("", IniFile::GetValue(iniFileName, "SectionName", "KeyName2"));
	// Initially no Section and Key names
	EXPECT_EQ((std::vector<std::string>{}), IniFile::GetSectionNames(iniFileName));
	EXPECT_EQ((std::vector<std::string>{}), IniFile::GetKeyNames(iniFileName, "SectionName"));

	// Create data
	EXPECT_NO_THROW(IniFile::SetValue(iniFileName, "SectionName", "KeyName1", "SomeValue1"));
	EXPECT_NO_THROW(IniFile::SetValue(iniFileName, "SectionName", "KeyName2", "SomeValue2"));
	// Data now present
	EXPECT_EQ("SomeValue1", IniFile::GetValue(iniFileName, "SectionName", "KeyName1"));
	EXPECT_EQ("SomeValue2", IniFile::GetValue(iniFileName, "SectionName", "KeyName2"));
	// Section and Key names now present
	EXPECT_EQ((std::vector<std::string>{"SectionName"}), IniFile::GetSectionNames(iniFileName));
	EXPECT_EQ((std::vector<std::string>{"KeyName1", "KeyName2"}), IniFile::GetKeyNames(iniFileName, "SectionName"));

	// Delete a key
	EXPECT_NO_THROW(IniFile::ClearKey(iniFileName, "SectionName", "KeyName1"));
	// One value removed
	EXPECT_EQ("", IniFile::GetValue(iniFileName, "SectionName", "KeyName1"));
	EXPECT_EQ("SomeValue2", IniFile::GetValue(iniFileName, "SectionName", "KeyName2"));
	// Section and one Key name now present
	EXPECT_EQ((std::vector<std::string>{"SectionName"}), IniFile::GetSectionNames(iniFileName));
	EXPECT_EQ((std::vector<std::string>{"KeyName2"}), IniFile::GetKeyNames(iniFileName, "SectionName"));

	// Delete a section
	EXPECT_NO_THROW(IniFile::ClearSection(iniFileName, "SectionName"));
	// Both values removed
	EXPECT_EQ("", IniFile::GetValue(iniFileName, "SectionName", "KeyName1"));
	EXPECT_EQ("", IniFile::GetValue(iniFileName, "SectionName", "KeyName2"));
	// No Section and Key names
	EXPECT_EQ((std::vector<std::string>{}), IniFile::GetSectionNames(iniFileName));
	EXPECT_EQ((std::vector<std::string>{}), IniFile::GetKeyNames(iniFileName, "SectionName"));

	// Cleanup test file
	fs::remove(iniFileName);
}

TEST(IniFile, IniFileWriteRead) {
	const std::string iniFileName{GetExeDirectory() + "TestIniFile.WriteRead.ini"};
	IniFile iniFile(iniFileName);

	// Initially no data
	EXPECT_EQ("", iniFile.GetValue("SectionName", "KeyName1"));
	EXPECT_EQ("", iniFile.GetValue("SectionName", "KeyName2"));
	// Initially no Section and Key names
	EXPECT_EQ((std::vector<std::string>{}), iniFile.GetSectionNames());
	EXPECT_EQ((std::vector<std::string>{}), iniFile.GetKeyNames("SectionName"));

	// Create data
	EXPECT_NO_THROW(iniFile.SetValue("SectionName", "KeyName1", "SomeValue1"));
	EXPECT_NO_THROW(iniFile.SetValue("SectionName", "KeyName2", "SomeValue2"));
	// Data now present
	EXPECT_EQ("SomeValue1", iniFile.GetValue("SectionName", "KeyName1"));
	EXPECT_EQ("SomeValue2", iniFile.GetValue("SectionName", "KeyName2"));
	// Section and Key names now present
	EXPECT_EQ((std::vector<std::string>{"SectionName"}), iniFile.GetSectionNames());
	EXPECT_EQ((std::vector<std::string>{"KeyName1", "KeyName2"}), iniFile.GetKeyNames("SectionName"));

	// Delete a key
	EXPECT_NO_THROW(iniFile.ClearKey("SectionName", "KeyName1"));
	// One value removed
	EXPECT_EQ("", iniFile.GetValue("SectionName", "KeyName1"));
	EXPECT_EQ("SomeValue2", iniFile.GetValue("SectionName", "KeyName2"));
	// Section and one Key name now present
	EXPECT_EQ((std::vector<std::string>{"SectionName"}), iniFile.GetSectionNames());
	EXPECT_EQ((std::vector<std::string>{"KeyName2"}), iniFile.GetKeyNames("SectionName"));

	// Delete a section
	EXPECT_NO_THROW(iniFile.ClearSection("SectionName"));
	// Both values removed
	EXPECT_EQ("", iniFile.GetValue("SectionName", "KeyName1"));
	EXPECT_EQ("", iniFile.GetValue("SectionName", "KeyName2"));
	// No Section and Key names
	EXPECT_EQ((std::vector<std::string>{}), iniFile.GetSectionNames());
	EXPECT_EQ((std::vector<std::string>{}), iniFile.GetKeyNames("SectionName"));

	// Cleanup test file
	fs::remove(iniFileName);
}

TEST(IniFile, IniSectionWriteRead) {
	const std::string iniFileName{GetExeDirectory() + "TestIniSection.WriteRead.ini"};
	IniSection iniSection(iniFileName, "SectionName");

	// Initially no data
	EXPECT_EQ("", iniSection.GetValue("KeyName1"));
	EXPECT_EQ("", iniSection.GetValue("KeyName2"));
	// Alternate syntax returns same data
	EXPECT_EQ("", iniSection["KeyName1"]);
	EXPECT_EQ("", iniSection["KeyName2"]);
	// Initially no Key names
	EXPECT_EQ((std::vector<std::string>{}), iniSection.GetKeyNames());

	// Create data
	EXPECT_NO_THROW(iniSection.SetValue("KeyName1", "SomeValue1"));
	EXPECT_NO_THROW(iniSection.SetValue("KeyName2", "SomeValue2"));
	// Data now present
	EXPECT_EQ("SomeValue1", iniSection.GetValue("KeyName1"));
	EXPECT_EQ("SomeValue2", iniSection.GetValue("KeyName2"));
	// Alternate syntax returns same data
	EXPECT_EQ("SomeValue1", iniSection["KeyName1"]);
	EXPECT_EQ("SomeValue2", iniSection["KeyName2"]);
	// Key names now present
	EXPECT_EQ((std::vector<std::string>{"KeyName1", "KeyName2"}), iniSection.GetKeyNames());

	// Delete a key
	EXPECT_NO_THROW(iniSection.ClearKey("KeyName1"));
	// One value removed
	EXPECT_EQ("", iniSection.GetValue("KeyName1"));
	EXPECT_EQ("SomeValue2", iniSection.GetValue("KeyName2"));
	// Alternate syntax returns same data
	EXPECT_EQ("", iniSection["KeyName1"]);
	EXPECT_EQ("SomeValue2", iniSection["KeyName2"]);
	// One Key name now present
	EXPECT_EQ((std::vector<std::string>{"KeyName2"}), iniSection.GetKeyNames());

	// Delete a section
	EXPECT_NO_THROW(iniSection.ClearSection());
	// Both values removed
	EXPECT_EQ("", iniSection.GetValue("KeyName1"));
	EXPECT_EQ("", iniSection.GetValue("KeyName2"));
	// Alternate syntax returns same data
	EXPECT_EQ("", iniSection["KeyName1"]);
	EXPECT_EQ("", iniSection["KeyName2"]);
	// No Key names
	EXPECT_EQ((std::vector<std::string>{}), iniSection.GetKeyNames());

	// Cleanup test file
	fs::remove(iniFileName);
}
