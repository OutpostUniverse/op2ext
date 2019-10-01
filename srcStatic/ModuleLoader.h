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
	ModuleLoader(IniFile iniFile);

	inline std::size_t Count() const { return modules.size(); }
	bool IsModuleLoaded(std::string moduleName);

	// The module's name is the same as the source ini file SectionName
	std::string GetModuleName(std::size_t index);

	// Module Manager takes ownership of GameModule object
	void RegisterModule(std::unique_ptr<GameModule> newGameModule);

	void LoadModules();
	bool UnloadModules();
	void RunModules();

private:
	IniFile iniFile;
	std::vector<std::unique_ptr<GameModule>> modules;

	void RegisterBuiltInModules();
	void RegisterExternalModules();
	bool IsBuiltInModuleRequested(const std::string& moduleName);
	std::vector<std::string> GetModuleNames(const std::string& moduleType);
};
