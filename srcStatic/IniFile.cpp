#include "IniFile.h"
#include <utility>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>


IniFile::IniFile(std::string fileName)
	: fileName(std::move(fileName))
{
}

const std::string& IniFile::FileName() const {
	return fileName;
}

std::string IniFile::GetValue(const std::string& sectionName, const std::string& keyName) const {
	return IniFile::GetValue(fileName, sectionName, keyName);
}

IniSection IniFile::operator[](std::string sectionName) const {
	return IniSection(fileName, std::move(sectionName));
}

void IniFile::ClearSection(const std::string& sectionName) {
	WritePrivateProfileStringA(sectionName.c_str(), nullptr, nullptr, fileName.c_str());
}

void IniFile::SetValue(const std::string& sectionName, const std::string& keyName, const std::string& value) {
	WritePrivateProfileStringA(sectionName.c_str(), keyName.c_str(), value.c_str(), fileName.c_str());
}

// Allow fetching values without explicitly creating an object
// This can be used for convenient one-off use
// This may be less efficienct for repeated use to fetch multiple values
std::string IniFile::GetValue(const std::string& fileName, const std::string& sectionName, const std::string& keyName) {
	const std::size_t bufferInterval = 1024;
	auto currentBufferSize = bufferInterval;
	std::string profileString;
	DWORD returnSize;

	while (true)
	{
		profileString.resize(currentBufferSize);

		//GetPrivateProfileString's return value is the number of characters copied to the buffer,
		// not including the terminating null character.
		// A full buffer could be nSize - 2 if either lpAppName or lpKeyName are NULL AND the supplied buffer is too small
		returnSize = GetPrivateProfileStringA(sectionName.c_str(), keyName.c_str(), "", &profileString[0], currentBufferSize, fileName.c_str());

		if (returnSize + 2 < currentBufferSize) {
			break;
		}

		currentBufferSize += bufferInterval;
	}

	profileString.resize(returnSize);

	return profileString;
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
	return IniFile::GetValue(fileName, sectionName, keyName);
}

void IniSection::ClearSection() {
	WritePrivateProfileStringA(sectionName.c_str(), nullptr, nullptr, fileName.c_str());
}

void IniSection::SetValue(const std::string& keyName, const std::string& value) {
	WritePrivateProfileStringA(sectionName.c_str(), keyName.c_str(), value.c_str(), fileName.c_str());
}
