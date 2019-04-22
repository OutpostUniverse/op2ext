#include "ConsoleModuleLoader.h"
#include "StringConversion.h"
#include "WindowsErrorCode.h"
#include "OP2Memory.h"
#include "FileSystemHelper.h"
#include "GlobalDefines.h"
#include <sstream>
#include <algorithm>
#include <iterator>
#include <stdexcept>
#include <cstddef>

std::string moduleDirectory;

ConsoleModuleLoader::ConsoleModuleLoader()
{
	moduleDirectory = FindModuleDirectory();
}

ConsoleModuleLoader::ConsoleModuleLoader(const std::string& testModuleDirectory)
{
	OutputDebugString("Console Module constructed in test mode.");

	moduleDirectory = testModuleDirectory;
}

std::string ConsoleModuleLoader::GetModuleDirectory()
{
	return moduleDirectory;
}

std::string ConsoleModuleLoader::GetModuleName()
{
	return ToLower(moduleDirectory);
}

bool ConsoleModuleLoader::IsModuleLoaded()
{
	return moduleDirectory != "";
}

bool ConsoleModuleLoader::IsModuleLoaded(std::string moduleName)
{
	return ToLowerInPlace(moduleName) == GetModuleName();
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
	if (GetFileAttributesA(moduleDirectory.c_str()) == INVALID_FILE_ATTRIBUTES) {
		PostErrorMessage("ConsoleModuleLoader.cpp", __LINE__, "Module directory does not exist");
		return;
	}

	SetArtPath();

	LoadModuleDll();
}

void ConsoleModuleLoader::LoadModuleDll()
{
	const std::string dllName = fs::path(moduleDirectory).append("\\op2mod.dll").string();

	if (!fs::exists(dllName)) {
		return; // Some console modules do not contain dlls
	}

	modDllHandle = LoadLibrary(dllName.c_str());

	if (modDllHandle) {
		// Call module's mod_init function
		FARPROC startFunc = GetProcAddress(modDllHandle, "mod_init");
		if (startFunc) {
			startFunc();
		}
	}
	else {
		const std::string errorMessage("Unable to load console module's dll from " + dllName +
			". " + GetLastErrorStdString(TEXT("LoadLibrary")));

		PostErrorMessage("ConsoleModuleLoader.cpp", __LINE__, errorMessage);
	}
}

void ConsoleModuleLoader::ParseCommandLine(std::vector<std::string>& arguments)
{
	arguments.clear();
	int argumentCount;

	LPWSTR *commandLineArgs = CommandLineToArgvW(GetCommandLineW(), &argumentCount);
	if (commandLineArgs == nullptr) {
		PostErrorMessage("ConsoleModuleLoader.cpp", __LINE__, "Unable to retrieve command line arguments attached to Outpost2.exe.");
	}
	else {
		try {
			// Ignore the first argument, which is the path of the executable.
			for (int i = 1; i < argumentCount; i++) {
				std::string argument;
				if (!ConvertLPWToString(argument, commandLineArgs[i])) {
					PostErrorMessage("ConsoleModuleLoader.cpp", __LINE__, "Unable to cast the " + std::to_string(i) +
						" command line argument from LPWSTR to char*. Further parsing of command line arguments aborted.");
					break;
				}
				arguments.push_back(argument);
			}
		}
		// Catch STL produced exceptions
		catch (std::exception& e) {
			PostErrorMessage("ConsoleModuleLoader.cpp", __LINE__, "Error occurred attempting to parse command line arguments. Further parshing of command line arguments aborted. Internal Error: " + std::string(e.what()));
		}
	}

	LocalFree(commandLineArgs);
}

bool ConsoleModuleLoader::ParseArgumentName(std::string& argument)
{
	if (argument[0] != '/' && argument[0] != '-') {
		std::string message("A switch was expected but not found. Prefix switch name with '/' or '-'. The following statement was found instead: " + argument);
		PostErrorMessage("ConsoleModuleLoader.cpp", __LINE__, message);
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

		if (GetFileAttributesA(modDirectory.c_str()) == INVALID_FILE_ATTRIBUTES) {
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

	for (std::size_t i = 0; i < arguments.size(); i++) {
		modRelativeDirectory += arguments[i];

		if (i < arguments.size() - 1) {
			modRelativeDirectory += " ";
		}
	}

	return modRelativeDirectory;
}

// Sets a directory called ART_PATH that is searched before looking in the root executable's directory.
// If an asset (vol, clm, video file, music1.wav, .map, tech file, etc) is found in ART_PATH's directory,
// it is loaded instead
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
