#include "DllModule.h"
#include "../WindowsErrorCode.h"
#include <stdexcept>

DllModule::DllModule(const std::string& moduleName)
	: GameModule(moduleName) { }

void DllModule::LoadModuleDll(const std::string& dllPath)
{
	// Try to load a DLL with the given name (possibly an empty string)
	HINSTANCE dllHandle = LoadLibraryA(dllPath.c_str());

	if (dllHandle == nullptr) {
		throw std::runtime_error(
			"Unable to load DLL for module: " + Name() +
			" DLL path: " + dllPath +
			" LoadLibrary " + GetLastErrorString()
		);
	}

	this->moduleDllHandle = dllHandle;
}
