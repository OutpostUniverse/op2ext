#include "DllModule.h"
#include "../WindowsErrorCode.h"
#include <stdexcept>

DllModule::DllModule(const std::string& moduleName)
	: GameModule(moduleName) { }

void DllModule::LoadModuleDll(const std::string& dllPath)
{
	// Try to load a DLL with the given name (possibly an empty string)
	HINSTANCE dllHandle = LoadLibraryA(dllPath.c_str());

	if (dllHandle == NULL) {
		throw std::runtime_error("Unable to load DLL " + dllPath + " from ini module section " +
			Name() + ". LoadLibrary " + GetLastErrorString());
	}

	this->moduleDllHandle = dllHandle;
}
