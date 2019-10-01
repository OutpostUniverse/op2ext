#include "IniFile.h"
#include <utility>


std::string GetPrivateProfileStdString(const std::string& sectionName, const std::string& key, const std::string& filename);


IniFile::IniFile(std::string fileName)
	: fileName(std::move(fileName))
{
}

const std::string& IniFile::FileName() const {
	return fileName;
}

std::string IniFile::GetValue(const std::string& sectionName, const std::string& keyName) const {
	return GetPrivateProfileStdString(sectionName, keyName, fileName);
}

IniSection IniFile::operator[](std::string sectionName) const {
	return IniSection(fileName, std::move(sectionName));
}


IniSection::IniSection(std::string fileName, std::string sectionName)
	: fileName(std::move(fileName)), sectionName(std::move(sectionName))
{
}

const std::string& IniSection::FileName() const {
	return fileName;
}

const std::string& IniSection::SectionName() const {
	return sectionName;
}

std::string IniSection::operator[](std::string keyName) const {
	return GetPrivateProfileStdString(sectionName, keyName, fileName);
}
