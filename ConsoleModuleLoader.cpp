#include "ConsoleModuleLoader.h"

#include "OP2Memory.h"
#include "FileSystemHelper.h"
#include <sstream>
#include <algorithm>
#include <iterator>
#include <filesystem>

namespace fs = std::experimental::filesystem;

std::string moduleDirectory;

ConsoleModuleLoader::ConsoleModuleLoader()
{
	moduleDirectory = FindModuleDirectory();
}

std::string ConsoleModuleLoader::GetModuleDirectory()
{
	return moduleDirectory;
}

int __fastcall GetArtPath(void*, int, char*, char*, char *destBuffer, int bufferSize, char *defaultValue) 
{
	strcpy_s(destBuffer, bufferSize, moduleDirectory.c_str());
	return moduleDirectory.size();
}

// Returns an empty string if no module is found or if the module request is ill-formed.
std::string ConsoleModuleLoader::FindModuleDirectory()
{
	std::vector<std::string> arguments;
	ParseCommandLine(arguments);

	if (arguments.size() == 0) {
		return std::string();
	}

	std::string switchName = arguments[0];
	arguments.erase(arguments.begin()); //Remove switchName from arguments

	if (!ParseArgumentName(switchName)) {
		return std::string();
	}

	if (switchName != "loadmod") {
		PostErrorMessage("ConsoleModuleLoader.cpp", __LINE__, "Provided switch is not supported: " + switchName);
		return std::string();
	}

	return ParseLoadModCommand(arguments);
}

void ConsoleModuleLoader::LoadModule()
{
	if (moduleDirectory.empty()) {
		return;
	}

	// Check if directory exists.
	if (GetFileAttributesA(moduleDirectory.c_str()) == -1) {
		PostErrorMessage("ConsoleModuleLoader.cpp", __LINE__, "Module directory does not exist");
		return;
	}

	SetArtPath();

	std::string dllName = fs::path(moduleDirectory).append("\\op2mod.dll").string();
	modDllHandle = LoadLibrary(dllName.c_str());

	if (modDllHandle) {
		// Call its mod_init func
		FARPROC startFunc = GetProcAddress(modDllHandle, "mod_init");
		if (startFunc) {
			startFunc();
		}
	}
}

void ConsoleModuleLoader::ParseCommandLine(std::vector<std::string>& arguments)
{
	std::string commandLine = GetCommandLine();

	std::istringstream iss(commandLine);
	std::copy(std::istream_iterator<std::string>(iss), std::istream_iterator<std::string>(), std::back_inserter(arguments));

	arguments.erase(arguments.begin()); //Remove Application Name from arguments.
}

bool ConsoleModuleLoader::ParseArgumentName(std::string& argument)
{
	if (argument[0] != '/' && argument[0] != '-') {
		PostErrorMessage("ConsoleModuleLoader.cpp", __LINE__, "A switch was expected but not found. Prefix switch name with '/' or '-'.");
		argument.clear();
		return false;
	}

	argument.erase(argument.begin(), argument.begin() + 1); //Removes leading - or /
	std::transform(argument.begin(), argument.end(), argument.begin(), ::tolower);

	return true;
}

std::string ConsoleModuleLoader::ParseLoadModCommand(std::vector<std::string> arguments)
{
	if (arguments.empty()) {
		PostErrorMessage("ConsoleModuleLoader.cpp", __LINE__, "No relative directory argument provided for the switch loadmod");
		return std::string();
	}

	try
	{
		std::string modRelativeDirectory = FormModRelativeDirectory(arguments);

		std::string modDirectory = fs::path(GetGameDirectory()).append(modRelativeDirectory).string();

		if (GetFileAttributesA(modDirectory.c_str()) == -1) {
			PostErrorMessage("ConsoleModuleLoader.cpp", __LINE__, "Module directory does not exist: " + modDirectory);
			return std::string();
		}

		return modDirectory;
	}
	catch (std::exception e)
	{
		PostErrorMessage("ConsoleModuleLoader.cpp", __LINE__, "Unable to parse module directory");
		return std::string();
	}
}

std::string ConsoleModuleLoader::FormModRelativeDirectory(std::vector<std::string> arguments)
{
	std::string modRelativeDirectory;

	for (size_t i = 0; i < arguments.size(); i++) {
		modRelativeDirectory += arguments[i];

		if (i < arguments.size() - 1) {
			modRelativeDirectory += " ";
		}
	}

	return modRelativeDirectory;
}

// Sets a directory called ART_PATH that is searched before looking in the root executable's directory.
// If an asset (vol, clm, video file, music1.wav, etc) is found in ART_PATH's directory, it is loaded instead
void ConsoleModuleLoader::SetArtPath()
{
	// This value may also be set using the DEBUG section of the .ini file, using the property ART_PATH.
	// If set in .ini file, ART_PATH must be deleted at end of session or will persist between plays.

	// Insert hooks to make OP2 look for files in the module's directory
	// In ResManager::GetFilePath
	Op2MemSetDword((void*)0x004715C5, (DWORD)&GetArtPath - (loadOffset + (DWORD)0x004715C5 + sizeof(void*)));
	// In ResManager::CreateStream
	Op2MemSetDword((void*)0x00471B87, (DWORD)&GetArtPath - (loadOffset + (DWORD)0x00471B87 + sizeof(void*)));
}

void ConsoleModuleLoader::UnloadModule()
{
	if (modDllHandle)
	{
		FARPROC destroyModFunc = GetProcAddress(modDllHandle, "mod_destroy");
		if (destroyModFunc) {
			destroyModFunc();
		}

		FreeLibrary(modDllHandle);
	}
}

void ConsoleModuleLoader::RunModule()
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
