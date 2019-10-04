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
	std::string GetModuleName(std::size_t index);

	void LoadModules();
	bool UnloadModules();
	void RunModules();

	bool IsModuleLoaded(std::string moduleName);

	void RegisterModule(std::unique_ptr<GameModule> newGameModule);

private:
	IniFile iniFile;
	std::vector<std::unique_ptr<GameModule>> modules;

	void RegisterBuiltInModules();
	void RegisterExternalModules();
	bool IsBuiltInModuleRequested(const std::string& moduleName);
	std::vector<std::string> GetModuleNames(const std::string& moduleType);
};
