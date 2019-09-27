#pragma once

#include "GameModule.h"
#include <string>
#include <vector>
#include <memory>


class ModuleLoader
{
public:
	inline std::size_t Count() const { return modules.size(); }
	bool IsModuleLoaded(std::string moduleName);

	std::string GetModuleName(std::size_t index);

	void RegisterModule(std::unique_ptr<GameModule> newGameModule);

	void LoadModules();
	bool UnloadModules();
	void RunModules();

private:
	std::vector<std::unique_ptr<GameModule>> modules;

	void RegisterBuiltInModules();
	void RegisterExternalModules();
	bool IsBuiltInModuleRequested(const std::string& moduleName);
	std::vector<std::string> GetModuleNames(const std::string& moduleType);
};
