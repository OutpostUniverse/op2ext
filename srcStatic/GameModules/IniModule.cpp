#include "IniModule.h"
#include <stdexcept>


IniModule::IniModule(IniSection iniSection)
	: DllModule(iniSection.SectionName()), iniSection(std::move(iniSection))
{
	try {
		// Get the DLL name from the corresponding section
		LoadModuleDll(iniSection["Dll"]);
	}
	catch (const std::exception& error) {
		throw std::runtime_error("Unable to load dll for module " + Name() + ". " + std::string(error.what()));
	}
};
