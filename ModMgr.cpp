#include "ModMgr.h"

#include "op2ext.h"
#include <sstream>
#include <algorithm>
#include <iterator>
#include <filesystem>

namespace fs = std::experimental::filesystem;

// GetCurrentModDir exists outside of ModMgr class to maintain backwards compatibility.
EXPORT char* GetCurrentModDir()
{ 
	CommandLineModuleManager modManager;
	std::string modDirectory = modManager.GetCurrentModuleDirectory();

	if (modDirectory.empty()) {
		return nullptr;
	}

	char* cStr = new char[modDirectory.length() + 1];
	strcpy_s(cStr, modDirectory.length() + 1, modDirectory.c_str());

	return cStr;
}

// Returns an empty string if either no module is found or the module request is ill-formed.
std::string CommandLineModuleManager::GetCurrentModuleDirectory()
{
	DBG("In ModMgr.cpp: GetCurrentModDir()\n");

	std::vector<std::string> arguments;
	ParseCommandLine(arguments);

	if (arguments.size() == 0) {
		return std::string();
	}

	std::string switchName = arguments[0];
	arguments.erase(arguments.begin());

	if (!ParseArgumentName(switchName)) {
		return std::string();
	}

	if (switchName != "loadmod") {
		PostErrorMessage("ModMgr.cpp", __LINE__, "Provided switch is not supported: " + switchName);
		return std::string();
	}

	return ParseLoadModCommand(arguments);
}

void CommandLineModuleManager::ApplyMods()
{
	std::string modDir = GetCurrentModuleDirectory();

	if (modDir.empty()) {
		return;
	}

	ApplyMod(modDir);
}

void CommandLineModuleManager::ParseCommandLine(std::vector<std::string>& arguments)
{
	//Function GetCommandLineA returns a LPSTR (long pointer to a string). For use in ANSI (A postfix).
	std::string commandLine = GetCommandLineA();

	std::istringstream iss(commandLine);
	std::copy(std::istream_iterator<std::string>(iss), std::istream_iterator<std::string>(), std::back_inserter(arguments));

	arguments.erase(arguments.begin()); //Remove Application Name from arguments.
}

bool CommandLineModuleManager::ParseArgumentName(std::string& argument)
{
	if (argument[0] != '/' && argument[0] != '-') {
		PostErrorMessage("ModMgr.cpp", __LINE__, "A switch was expected but not found. Prefix switch name with '/' or '-'.");
		argument.clear();
		return false;
	}

	argument.erase(argument.begin(), argument.begin() + 1); //Removes leading - or /
	std::transform(argument.begin(), argument.end(), argument.begin(), ::tolower);

	return true;
}

std::string CommandLineModuleManager::ParseLoadModCommand(std::vector<std::string> arguments)
{
	if (arguments.empty()) {
		PostErrorMessage("ModMgr.cpp", __LINE__, "No relative directory argument provided for the switch loadmod");
		return std::string();
	}

	fs::path p(GetGameDirectory());

	std::string modRelativeDirectory;
	for (auto & argument : arguments) {
		modRelativeDirectory += argument;
	}

	try
	{
		std::string modDirectory = p.append(modRelativeDirectory).string();

		if (GetFileAttributesA(modDirectory.c_str()) == -1) {
			PostErrorMessage("ModMgr.cpp", __LINE__, "Mod directory does not exist: " + modDirectory);
			return std::string();
		}

		return modDirectory;
	}
	catch (std::exception e)
	{
		PostErrorMessage("ModMgr.cpp", __LINE__, "Unable to parse mod directory");
		return std::string();
	}
}

void CommandLineModuleManager::ApplyMod(std::string modDir)
{
	// Check if directory exists.
	if (GetFileAttributesA(modDir.c_str()) == -1) {
		PostErrorMessage("ModMgr.cpp", __LINE__, "Mod directory does not exist");
		return;
	}

	// BRETT208 18NOV17: THIS DEBUG LINE IS NOT DELETED WHEN OP2 IS CLOSED OUT. 
	//    IT CAUSES STOCK COLONY MAPS NOT TO WORK ON FOLLOWON USES WHEN NO MOD IS LOADED AFTER A SESSION LOADING MULTITEK2.
	//    IT APPEARS THE DEBUG SECTION IS NOT DELETED BY LINE 163 EVEN THOUGH THAT CODE RUNS WHEN OUTPOST 2 EXITS.
	//    I BELIEVE THE DESTROY FUNCTION IS ATTEMPTING TO DESTROY IT IN THE CURRENT WORKING DIRECTORY AND NOT THE EXECUTABLE'S DIRECTORY.
	// Set that magic DEBUG\ART_PATH value in the .ini
	WritePrivateProfileString("DEBUG", "ART_PATH", modDir.c_str(), ".\\outpost2.ini");

	std::string dllName = fs::path(modDir).append("\\op2mod.dll").string();
	modDllHandle = LoadLibrary(dllName.c_str());

	if (modDllHandle) {
		// Call its mod_init func
		FARPROC startFunc = GetProcAddress(modDllHandle, "mod_init");
		if (startFunc) {
			startFunc();
		}
	}
}

void CommandLineModuleManager::UnApplyMod()
{
	// Call the mod DLL mod_destroy func
	if (modDllHandle)
	{
		FARPROC stopFunc = GetProcAddress(modDllHandle, "mod_destroy");
		if (stopFunc) {
			stopFunc();
		}

		// Unload the DLL
		FreeLibrary(modDllHandle);
	}

	// Destroy the DEBUG section
	WritePrivateProfileString("DEBUG", nullptr, nullptr, ".\\outpost2.ini");
}

void CommandLineModuleManager::ModStartup()
{
	// Startup a module by calling its run func
	if (modDllHandle)
	{
		// Call its mod_run func
		FARPROC runFunc = GetProcAddress(modDllHandle, "mod_run");
		if (runFunc) {
			runFunc();
		}
	}
}
