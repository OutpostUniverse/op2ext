#include "op2ext.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <string>
#include <vector>

class ConsoleModuleLoader {
public:
	ConsoleModuleLoader();
	void LoadModule();
	void UnloadModule();
	void RunModule();
	std::string GetModuleDirectory();

private:
	HMODULE modDllHandle = nullptr;

	std::string FindModuleDirectory();
	void ParseCommandLine(std::vector<std::string>& arguments);
	bool ParseArgumentName(std::string& argument);
	std::string ParseLoadModCommand(std::vector<std::string> arguments);
	std::string FormModRelativeDirectory(std::vector<std::string> arguments);
	void SetArtPath();
};