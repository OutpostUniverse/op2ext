#include <windows.h>
#include <string>
#include <vector>
#include <cstddef>

class ConsoleModuleLoader {
public:
	ConsoleModuleLoader(const std::string& moduleRelativeDirectory);
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
	std::string moduleName;

	void LoadModuleDll();
	void SetArtPath();
};
