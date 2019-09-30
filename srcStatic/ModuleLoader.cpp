#include "ModuleLoader.h"
#include "GameModules/IpDropDown.h"
#include "GameModules/IniModule.h"
#include "StringConversion.h"
#include "FileSystemHelper.h"
#include "Log.h"
#include <stdexcept>


void ModuleLoader::RegisterBuiltInModules()
{
	if (IsBuiltInModuleRequested("IPDropDown")) {
		RegisterModule(std::make_unique<IPDropDown>());
	}
}

void ModuleLoader::RegisterExternalModules()
{
	const auto sectionNames = GetModuleNames("ExternalModules");

	for (const auto& sectionName : sectionNames)
	{
		try {
			RegisterModule(std::make_unique<IniModule>(sectionName));
		}
		catch (const std::exception& e) {
			PostError(e.what());
		}
	}
}

bool ModuleLoader::IsBuiltInModuleRequested(const std::string& moduleName)
{
	const auto isModuleRequested = ToLower(GetOutpost2IniSetting("BuiltInModules", moduleName));

	if (isModuleRequested == "yes") {
		return true;
	}
	else if (isModuleRequested == "no" || isModuleRequested == "") {
		return false;
	}

	PostError("Built-in module named " + moduleName + " contains an innapropriate setting. It must be set to Yes or No");
	return false;
}

std::vector<std::string> ModuleLoader::GetModuleNames(const std::string& moduleType)
{
	auto sectionNamesString = GetOutpost2IniSetting("Game", moduleType);
	auto sectionNames = SplitAndTrim(sectionNamesString, ',');

	return sectionNames;
}

// An INI module name is the SectionName from the INI file with its config settings
std::string ModuleLoader::GetModuleName(std::size_t index)
{
	if (index >= modules.size()) {
		throw std::out_of_range("Module Loader was provided an index to lookup a module name that was outside the range of loaded modules.");
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
void ModuleLoader::RegisterModule(std::unique_ptr<GameModule> newGameModule)
{
	if (newGameModule == nullptr) {
		return;
	}

	if (IsModuleLoaded(newGameModule->Name())) {
		PostError("You may not add a module with an existing name. Duplicate copies of module name " + newGameModule->Name() + " found.");
		return;
	}

	modules.push_back(std::move(newGameModule));
};

void ModuleLoader::LoadModules()
{
	RegisterBuiltInModules();
	RegisterExternalModules();

	for (auto& gameModule : modules)
	{
		try {
			gameModule->Load();
		}
		catch (const std::exception& e) {
			PostError("Error loading module " + gameModule->Name() + ". " + std::string(e.what()));
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
			PostError("Error unloading module " + gameModule->Name() + ". " + std::string(e.what()));
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
			PostError("Error running module " + gameModule->Name() + ". " + std::string(e.what()));
		}
	}
}
