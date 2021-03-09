#pragma once

#include "GameModule.h"
#include "IniFile.h"
#include <string>
#include <vector>
#include <memory>
#include <cstddef>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>


class ModuleLoader
{
public:
	ModuleLoader();
	ModuleLoader(IniFile iniFile, std::vector<std::string> consoleModuleNames);

	inline std::size_t Count() const { return modules.size(); }
	std::string GetModuleName(std::size_t index);
	std::string GetModuleDirectory(std::size_t index);

	void LoadModules();
	bool UnloadModules();
	void RunModules();

	bool IsModuleLoaded(std::string moduleName);

	void RegisterModule(std::unique_ptr<GameModule> newGameModule);

	GameModule* FindModule(HMODULE dllModule);

private:
	IniFile iniFile;
	const std::vector<std::string> consoleModuleNames;
	std::vector<std::unique_ptr<GameModule>> modules;

	void RegisterBuiltInModules();
	// Console module names are the relative path from the OPU folder (no trailing slash)
	void RegisterConsoleModules(std::vector<std::string>& moduleDirectories);
	void RegisterIniModules(std::vector<std::string>& moduleDirectories);
	bool IsModuleRequested(const std::string& sectionName, const std::string& moduleName, bool defaultValue = false);
};
