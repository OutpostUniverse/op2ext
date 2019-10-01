#pragma once

#include <string>


class IniSection;


// Represents an entire .ini file
class IniFile {
public:

	IniFile(std::string fileName);

	const std::string& FileName() const;
	std::string GetValue(const std::string& sectionName, const std::string& keyName) const;
	IniSection operator[](std::string sectionName) const;

	void ClearSection(const std::string& sectionName);
	void SetValue(const std::string& sectionName, const std::string& keyName, const std::string& value);

private:
	const std::string fileName;
};


// Represents a single section within a .ini file
class IniSection {
public:
	IniSection(std::string fileName, std::string sectionName);

	const std::string& FileName() const;
	const std::string& SectionName() const;
	std::string operator[](std::string keyName) const;

	void ClearSection();
	void SetValue(const std::string& keyName, const std::string& value);

private:
	const std::string fileName;
	const std::string sectionName;
};
