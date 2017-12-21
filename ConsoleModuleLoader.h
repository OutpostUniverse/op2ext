#include "op2ext.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <string>
#include <vector>

class ConsoleModuleLoader {
public:
	ConsoleModuleLoader();
	void ApplyMods();
	void UnApplyMod();
	void ModStartup();
	std::string GetModuleDirectory();

private:
	std::string moduleDirectory;
	HMODULE modDllHandle = nullptr;

	std::string FindModuleDirectory();
	void ApplyMod(std::string modDir);
	void ParseCommandLine(std::vector<std::string>& arguments);
	bool ParseArgumentName(std::string& argument);
	std::string ParseLoadModCommand(std::vector<std::string> arguments);
	std::string FormModRelativeDirectory(std::vector<std::string> arguments);
};

extern ConsoleModuleLoader consoleModLoader;