#include "ModuleLoader.h"
#include "GameModules/IpDropDown.h"
#include "GameModules/IniModule.h"
#include "StringConversion.h"
#include "FileSystemHelper.h"


ModuleLoader::ModuleLoader() 
{
	RegisterInternalModules();
	RegisterExternalModules();
}

void ModuleLoader::RegisterInternalModules()
{
	const auto internalModuleNames = GetModuleNames("InternalModules");

	for (const auto& moduleName : internalModuleNames) {
		if ("ipdropdown" == ToLower(moduleName)) {
			RegisterModule(static_cast<std::unique_ptr<GameModule>>(std::make_unique<IPDropDown>()));
		}
		else {
			PostErrorMessage(__FILE__, __LINE__, "A module named " + moduleName + " was requested to load from the Outpost2.ini file. This module cannot be found.");
		}
	}
}

void ModuleLoader::RegisterExternalModules()
{
	const auto sectionNames = GetModuleNames("ExternalModules");

	for (const auto& sectionName : sectionNames)
	{
		try {
			RegisterModule(static_cast<std::unique_ptr<GameModule>>(std::make_unique<IniModule>(sectionName)));
		}
		catch (const std::exception& e) {
			PostErrorMessage(__FILE__, __LINE__, e.what());
		}
	}
}

std::vector<std::string> ModuleLoader::GetModuleNames(const std::string& moduleType)
{
	std::string sectionNames = GetOutpost2IniSetting("Game", moduleType);
	std::vector<std::string> sectionNamesSplit = SplitString(sectionNames, ',', TrimOption::Both);

	return sectionNamesSplit;
}

std::string ModuleLoader::GetModuleName(std::size_t index)
{
	if (index >= modules.size()) {
		return "";
	}

	return modules[index]->Name();
}

bool ModuleLoader::IsModuleLoaded(std::string moduleName)
{
	ToLowerInPlace(moduleName);

	for (const auto& module : modules) {
		if (moduleName == ToLower(module->Name())) {
			return true;
		}
	}

	return false;
}

// Module Manager takes ownership of GameModule object
void ModuleLoader::RegisterModule(std::unique_ptr<GameModule>& newGameModule)
{
	if (newGameModule == nullptr) {
		return;
	}

	for (const auto& currentGameModule : modules) {
		if (newGameModule->Name() == currentGameModule->Name()) {
			PostErrorMessage(__FILE__, __LINE__, "You may not add a module with an existing name. Duplicate copies of module name " + newGameModule->Name() + " found.");
			return;
		}
	}

	modules.push_back(std::move(newGameModule));
};

void ModuleLoader::RegisterModule(std::unique_ptr<GameModule>&& newGameModule)
{
	RegisterModule(newGameModule);
}

void ModuleLoader::LoadModules()
{
	for (auto& gameModule : modules)
	{
		try {
			gameModule->Load();
		}
		catch (const std::exception& e) {
			PostErrorMessage(__FILE__, __LINE__, "Error loading module " + gameModule->Name() + ". " + std::string(e.what()));
		}
	}
}

bool ModuleLoader::UnloadModules()
{
	bool areAllModulesProperlyDestroyed = true;

	for (auto& gameModule : modules)
	{
		try {
			bool isModuleProperlyDestroyed = gameModule->Unload();

			if (!isModuleProperlyDestroyed) {
				areAllModulesProperlyDestroyed = false;
			}
		}
		catch (const std::exception& e) {
			PostErrorMessage(__FILE__, __LINE__, "Error unloading module " + gameModule->Name() + ". " + std::string(e.what()));
			areAllModulesProperlyDestroyed = false;
		}
	}

	modules.clear();

	return areAllModulesProperlyDestroyed;
}

void ModuleLoader::RunModules()
{
	for (auto& gameModule : modules)
	{
		try {
			gameModule->Run();
		}
		catch (const std::exception& e) {
			PostErrorMessage(__FILE__, __LINE__, "Error running module " + gameModule->Name() + ". " + std::string(e.what()));
		}
	}
}
