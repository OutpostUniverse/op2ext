#pragma once

#include "GameModules/GameModule.h"
#include "GlobalDefines.h"
#include <string>
#include <vector>
#include <memory>


class ModuleLoader
{
public:
	ModuleLoader();

	inline std::size_t Count() const { return modules.size(); }
	bool IsModuleLoaded(std::string moduleName);

	// The module's name is the same as the source ini file SectionName
	std::string GetModuleName(std::size_t index);

	// Module Manager takes ownership of GameModule object
	void RegisterModule(std::unique_ptr<GameModule>& newGameModule);
	void RegisterModule(std::unique_ptr<GameModule> && newGameModule);

	void LoadModules();
	bool UnloadModules();
	void RunModules();

private:
	std::vector<std::unique_ptr<GameModule>> modules;

	void RegisterInternalModules();
	void RegisterExternalModules();
	std::vector<std::string> GetModuleNames(const std::string& moduleType);
};
