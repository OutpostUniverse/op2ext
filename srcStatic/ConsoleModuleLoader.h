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
	std::string GetModuleName();

private:
	HINSTANCE modDllHandle = nullptr;

	std::string FindModuleDirectory();
	void LoadModuleDll();
	void ParseCommandLine(std::vector<std::string>& arguments);
	bool ParseArgumentName(std::string& argument);
	std::string ParseLoadModCommand(std::vector<std::string> arguments);
	std::string FormModRelativeDirectory(std::vector<std::string> arguments);
	void SetArtPath();
};
