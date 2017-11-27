#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <string>
#include <vector>
#include <list>

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

private:
	// Export (not absolutely required, but should be used if any additional parameters are read from the .ini file)
	typedef void(*InitModFunc)(char* iniSectionName);
	typedef bool(*DestroyModFunc)();

	struct IniModuleEntry
	{
		HINSTANCE handle;
		DestroyModFunc destroyModFunc;
	};

	std::list<IniModuleEntry> moduleList;

	std::vector<std::string> GetModuleNames();
	bool LoadModuleDll(IniModuleEntry& moduleEntry, std::string sectionName);
	void CallModuleInitialization(IniModuleEntry& currentModule, std::string sectionName);
};