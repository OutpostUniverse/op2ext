#include <windows.h>
#include <string>
#include <vector>
#include <cstddef>

class ConsoleModuleLoader {
public:
	ConsoleModuleLoader();
	// Test Constructor
	ConsoleModuleLoader(const std::string& testModuleDirectory);
	void LoadModule();
	void UnloadModule();
	void RunModule();
	std::string GetModuleDirectory();
	std::string GetModuleName();
	std::size_t Count();
	// Returns false if passed an empty string (Module name cannot be empty)
	bool IsModuleLoaded(std::string moduleName);

private:
	HINSTANCE modDllHandle = nullptr;

	std::string FindModuleDirectory();
	void LoadModuleDll();
	std::vector<std::string> GetCommandLineArguments();
	bool ParseArgumentName(std::string& argument);
	std::string ParseLoadModCommand(std::vector<std::string> arguments);
	std::string FormModRelativeDirectory(std::vector<std::string> arguments);
	void SetArtPath();
};
