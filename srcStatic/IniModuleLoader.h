#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string>
#include <vector>
#include <cstddef>

/**
Loads all modules defined in the Outpost2.ini file.
Modules defined in the Outpost2.ini file are meant to be in use all the time with the game.
To load a single module for a single game session, use the ModMgr.
*/
class IniModuleLoader
{
public:
	void LoadModules();
	bool UnloadModules();
	std::string GetModuleName(std::size_t index);

private:
	// Export (not absolutely required, but should be used if any additional parameters are read from the .ini file)
	typedef void(*InitModFunc)(const char* iniSectionName);
	typedef bool(*DestroyModFunc)();

	struct IniModuleEntry
	{
		HINSTANCE handle;
		DestroyModFunc destroyModFunc;
		// The module's name is the same as the source ini file SectionName
		std::string name;
	};

	std::vector<IniModuleEntry> modules;

	std::vector<std::string> GetSectionNames();
	void LoadModule(std::string sectionName);
	void LoadModuleDll(IniModuleEntry& moduleEntry);
	void CallModuleInitialization(IniModuleEntry& currentModule, std::string sectionName);
	bool CallModuleDestruction(IniModuleEntry& currentModule);
};
