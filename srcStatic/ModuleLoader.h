#pragma once

#include "GameModule.h"
#include "IniFile.h"
#include <string>
#include <vector>
#include <memory>


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

private:
	IniFile iniFile;
	const std::vector<std::string> consoleModuleNames;
	std::vector<std::unique_ptr<GameModule>> modules;

	void RegisterBuiltInModules();
	// Console module names are the relative path from the game folder (no trailing slash)
	void RegisterConsoleModules();
	void RegisterIniModules();
	bool IsModuleRequested(const std::string& sectionName, const std::string& moduleName);
};
