#include "op2ext.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <string>
#include <vector>

class CommandLineModuleManager {
public:
	void ApplyMods();
	void UnApplyMod();
	void ModStartup();
	std::string GetCurrentModuleDirectory();

private:
	HMODULE modDllHandle = nullptr;

	void ApplyMod(std::string modDir);
	void ParseCommandLine(std::vector<std::string>& arguments);
	bool ParseArgumentName(std::string& argument);
	std::string ParseLoadModCommand(std::vector<std::string> arguments);
};

extern CommandLineModuleManager modManager;

EXPORT char* GetCurrentModDir();