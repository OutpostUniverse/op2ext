#include "IniFile.h"
#include <utility>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>


// Construct an object to represent a whole ini file
IniFile::IniFile(std::string fileName)
	: fileName(std::move(fileName))
{
}

// Get the ini file name
const std::string& IniFile::FileName() const {
	return fileName;
}

// Get the value of a key within a section
std::string IniFile::GetValue(const std::string& sectionName, const std::string& keyName) const {
	return IniFile::GetValue(fileName, sectionName, keyName);
}

// Get the named section as a whole
IniSection IniFile::operator[](std::string sectionName) const {
	return IniSection(fileName, std::move(sectionName));
}

// Remove an entire section along with all keys and values it contains
void IniFile::ClearSection(const std::string& sectionName) {
	IniFile::ClearSection(fileName, sectionName);
}

// Remove a single key and its associated value from a section
void IniFile::ClearKey(const std::string& sectionName, const std::string& keyName) {
	IniFile::ClearKey(fileName, sectionName, keyName);
}

// Set a new value for a given key in a given section
void IniFile::SetValue(const std::string& sectionName, const std::string& keyName, const std::string& value) {
	IniFile::SetValue(fileName, sectionName, keyName, value);
}


// Get the value of a key within a section of a named ini file
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

// Remove an entire section along with all keys and values it contains
void IniFile::ClearSection(const std::string& fileName, const std::string& sectionName) {
	WritePrivateProfileStringA(sectionName.c_str(), nullptr, nullptr, fileName.c_str());
}

// Remove a single key and its associated value from a section
void IniFile::ClearKey(const std::string& fileName, const std::string& sectionName, const std::string& keyName) {
	WritePrivateProfileStringA(sectionName.c_str(), keyName.c_str(), nullptr, fileName.c_str());
}

// Set a new value for a key within a section
void IniFile::SetValue(const std::string& fileName, const std::string& sectionName, const std::string& keyName, const std::string& value) {
	WritePrivateProfileStringA(sectionName.c_str(), keyName.c_str(), value.c_str(), fileName.c_str());
}


// Construct an object to represent a whole section of an ini file
IniSection::IniSection(std::string fileName, std::string sectionName)
	: fileName(std::move(fileName)), sectionName(std::move(sectionName))
{
}

// Get the ini file name
const std::string& IniSection::FileName() const {
	return fileName;
}

// Get the stored section name
const std::string& IniSection::SectionName() const {
	return sectionName;
}

// Get the value of a key within a section
std::string IniSection::GetValue(const std::string& keyName) const {
	return IniFile::GetValue(fileName, sectionName, keyName);
}

// Get the value of a key within a section
// Alternate syntax for GetValue
std::string IniSection::operator[](std::string keyName) const {
	return IniFile::GetValue(fileName, sectionName, keyName);
}

// Remove an entire section along with all keys and values it contains
void IniSection::ClearSection() {
	IniFile::ClearSection(fileName, sectionName);
}

// Remove a single key and its associated value from a section
void IniSection::ClearKey(const std::string& keyName) {
	IniFile::ClearKey(fileName, sectionName, keyName);
}

// Set a new value for a key within a section
void IniSection::SetValue(const std::string& keyName, const std::string& value) {
	IniFile::SetValue(fileName, sectionName, keyName, value);
}
