#include "ConsoleModule.h"
#include "../FileSystemHelper.h"
#include "../FsInclude.h"
#include "../WindowsErrorCode.h"
#include <stdexcept>


ConsoleModule::ConsoleModule(const std::string& moduleName) : DllModule(moduleName), 
	moduleDirectory(moduleName + "\\")
{
	auto absoluteModuleDirectory = fs::path(GetExeDirectory()) / moduleDirectory;

	if (!IsDirectory(absoluteModuleDirectory.string())) {
		throw std::runtime_error("Unable to access the provided module directory: " + 
			moduleDirectory + " : " + GetLastErrorString());
	}

	const auto dllPath = (absoluteModuleDirectory / "op2mod.dll").string();
	if (!Exists(dllPath)) {
		return; // Some console modules do not contain dlls
	}

	LoadModuleDll(dllPath);
}
