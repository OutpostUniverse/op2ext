#include "ModuleLoader.h"
#include "GameModules/IpDropDown.h"
#include "GameModules/ConsoleModule.h"
#include "GameModules/IniModule.h"
#include "StringConversion.h"
#include "FileSystemHelper.h"
#include "FsInclude.h"
#include "ResourceSearchPath.h"
#include "Log.h"
#include "ConsoleArgumentParser.h"
#include <utility>
#include <algorithm>
#include <stdexcept>


ModuleLoader::ModuleLoader()
	: iniFile(IniFile(GetOutpost2IniPath())), consoleModuleNames(FindModuleDirectories())
{
}

ModuleLoader::ModuleLoader(IniFile iniFile, std::vector<std::string> consoleModuleNames)
	: iniFile(std::move(iniFile)), consoleModuleNames(consoleModuleNames)
{

}

void ModuleLoader::RegisterBuiltInModules()
{
	if (IsModuleRequested("BuiltInModules", "IPDropDown")) {
		RegisterModule(std::make_unique<IPDropDown>());
	}
}

void ModuleLoader::RegisterConsoleModules()
{
	if (consoleModuleNames.empty()) {
		return; // No console modules to load
	}

	// Temporary check. This will eventually become an error.
	if (consoleModuleNames.size() == 1 && consoleModuleNames[0].empty()) {
		return; // No console modules to load
	}

	if (std::any_of(consoleModuleNames.begin(), consoleModuleNames.end(), [](const std::string& consoleModuleName) { return consoleModuleName.empty(); })) {
		PostError("All console module names must be non-empty.");
		return;
	}

	std::vector<std::string> moduleDirectories;
	moduleDirectories.reserve(consoleModuleNames.size());

	for (const auto& moduleName : consoleModuleNames)
	{
		try {
			auto consoleModule = std::make_unique<ConsoleModule>(moduleName);
			moduleDirectories.push_back((fs::path(GetExeDirectory()) / consoleModule->Directory()).string());

			RegisterModule(std::move(consoleModule));
		}
		catch (const std::exception& e) {
			PostError("Unable to load console module " + moduleName + ". " + e.what());
		}
	}

	ResourceSearchPath::Set(std::move(moduleDirectories));
}

void ModuleLoader::RegisterIniModules()
{
	for (const auto moduleName : iniFile.GetKeyNames("ExternalModules"))
	{
		if (IsModuleRequested("ExternalModules", moduleName)) 
		{
			try {
				RegisterModule(std::make_unique<IniModule>(iniFile[moduleName]));
			}
			catch (const std::exception& e) {
				PostError("Unable to load ini module " + moduleName + ". " + e.what());
			}
		}
	}
}

bool ModuleLoader::IsModuleRequested(const std::string& sectionName, const std::string& moduleName)
{
	const auto isModuleRequested = ToLower(iniFile.GetValue(sectionName, moduleName));

	if (isModuleRequested == "yes") {
		return true;
	}
	else if (isModuleRequested == "no" || isModuleRequested == "") {
		return false;
	}

	PostError("Module named " + moduleName + " contains an innapropriate setting of " + isModuleRequested + ". It must be set to Yes or No");
	return false;
}

// An INI module name is the SectionName from the INI file with its config settings
std::string ModuleLoader::GetModuleName(std::size_t index)
{
	if (index >= modules.size()) {
		throw std::out_of_range("Module Loader was provided an index to lookup a module name that was outside the range of loaded modules.");
	}

	return modules[index]->Name();
}

std::string ModuleLoader::GetModuleDirectory(std::size_t index)
{
	if (index >= modules.size()) {
		throw std::out_of_range("GetModuleDirectory: Invalid module index: " + std::to_string(index));
	}

	return modules[index].get()->Directory();
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
	RegisterConsoleModules();
	RegisterBuiltInModules();
	RegisterIniModules();

	// Abort early to avoid hooking file search path if not needed
	if (modules.empty()) {
		return;
	}

	// Setup loading of additional resources from module folders
	ResourceSearchPath::Activate();



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
