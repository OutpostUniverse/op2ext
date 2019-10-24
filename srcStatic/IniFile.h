#pragma once

#include <string>
#include <vector>


class IniSection;


// Represents an entire .ini file
class IniFile {
public:

	IniFile(std::string fileName);

	const std::string& FileName() const;
	std::string GetValue(const std::string& sectionName, const std::string& keyName, const std::string& defaultValue = "") const;
	IniSection operator[](std::string sectionName) const;
	std::vector<std::string> GetSectionNames() const;
	std::vector<std::string> GetKeyNames(const std::string& sectionName);

	void ClearSection(const std::string& sectionName);
	void ClearKey(const std::string& sectionName, const std::string& keyName);
	void SetValue(const std::string& sectionName, const std::string& keyName, const std::string& value);

	static std::string GetValue(const std::string& fileName, const std::string& sectionName, const std::string& keyName, const std::string& defaultValue);
	static std::vector<std::string> GetSectionNames(const std::string& fileName);
	static std::vector<std::string> GetKeyNames(const std::string& fileName, const std::string& sectionName);
	static void ClearSection(const std::string& fileName, const std::string& sectionName);
	static void ClearKey(const std::string& fileName, const std::string& sectionName, const std::string& keyName);
	static void SetValue(const std::string& fileName, const std::string& sectionName, const std::string& keyName, const std::string& value);

private:
	const std::string fileName;

	static std::string GetIniString(const char* fileName, const char* sectionName, const char* keyName, const char* defaultValue = "");
	static std::vector<std::string> SplitResultOnNull(std::string arrayBuffer);
};


// Represents a single section within a .ini file
class IniSection {
public:
	IniSection(std::string fileName, std::string sectionName);

	const std::string& FileName() const;
	const std::string& SectionName() const;
	std::string GetValue(const std::string& keyName, const std::string& defaultValue = "") const;
	std::string operator[](std::string keyName) const;
	std::vector<std::string> GetKeyNames();

	void ClearSection();
	void ClearKey(const std::string& keyName);
	void SetValue(const std::string& keyName, const std::string& value);

private:
	const std::string fileName;
	const std::string sectionName;
};
