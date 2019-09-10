#include "ModuleLoader.h"
#include "WindowsErrorCode.h"
#include "FileSystemHelper.h"
#include "StringConversion.h"
#include "GlobalDefines.h"
#include <stdexcept>

// Load all active modules specified in the .ini file
void IniModuleLoader::LoadModules()
{
	const auto moduleNames = GetModuleNames("ExternalModules");

	for (const auto& moduleName : moduleNames)
	{
		LoadModule(moduleName);
	}
}

void IniModuleLoader::LoadModule(std::string sectionName)
{
	IniModuleEntry moduleEntry;
	moduleEntry.iniSectionName = sectionName;

	try {
		moduleEntry.handle = LoadModuleDll(sectionName);
	}
	catch (const std::exception& error) {
		PostErrorMessage(__FILE__, __LINE__, error.what());
		return;
	}

	CallModuleInitialization(moduleEntry, sectionName);

	moduleEntry.destroyModFunc = (DestroyModFunc)GetProcAddress(moduleEntry.handle, "DestroyMod");

	modules.push_back(moduleEntry);
}

// Unload all active modules specified in the .ini file
bool IniModuleLoader::UnloadModules()
{
	bool result = true;

	for (IniModuleEntry moduleEntry : modules)
	{
		if (!CallModuleDestruction(moduleEntry)) {
			result = false;
		}

		FreeLibrary(moduleEntry.handle);
	}

	modules.clear();

	return result;
}

std::string IniModuleLoader::GetModuleName(std::size_t index)
{
	if (index >= modules.size()) {
		return "";
	}

	return modules[index].iniSectionName;
}

bool IniModuleLoader::IsModuleLoaded(std::string moduleName)
{
	ToLowerInPlace(moduleName);

	for (const auto& module : modules) {
		if (moduleName == ToLower(module.iniSectionName)) {
			return true;
		}
	}
	
	return false;
}

std::vector<std::string> IniModuleLoader::GetModuleNames(const std::string& moduleType)
{
	std::string sectionNames = GetOutpost2IniSetting("Game", moduleType);
	std::vector<std::string> sectionNamesSplit = SplitString(sectionNames, ',', TrimOption::Both);

	return sectionNamesSplit;
}

HINSTANCE IniModuleLoader::LoadModuleDll(const std::string& sectionName)
{
	// Get the DLL name from the corresponding section
	std::string dllName = GetOutpost2IniSetting(sectionName, "Dll");

	// Try to load a DLL with the given name (possibly "")
	HINSTANCE dllHandle = LoadLibrary(dllName.c_str());

	if (dllHandle == 0) {
		throw std::runtime_error("Unable to load DLL " + dllName + " from ini module section " +
			sectionName + "." + GetLastErrorStdString(TEXT("LoadLibrary")));
	}

	return dllHandle;
}

void IniModuleLoader::CallModuleInitialization(IniModuleEntry& moduleEntry, std::string sectionName)
{
	// Try to find an initialization function
	InitModFunc initModFunc = (InitModFunc)GetProcAddress(moduleEntry.handle, "InitMod");

	// Call the InitMod function if it exists
	if (initModFunc != 0) {
		initModFunc(sectionName.c_str());
	}
}

bool IniModuleLoader::CallModuleDestruction(IniModuleEntry& moduleEntry)
{
	if (moduleEntry.destroyModFunc != 0) {
		return moduleEntry.destroyModFunc();
	}

	return true;
}
