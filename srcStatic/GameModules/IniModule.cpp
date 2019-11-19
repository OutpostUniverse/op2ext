#include "IniModule.h"
#include "../FsInclude.h"
#include <utility>
#include <stdexcept>


IniModule::IniModule(IniSection iniSection)
	: DllModule(iniSection.SectionName()), iniSection(std::move(iniSection))
{
	try {
		// Get the DLL name from the corresponding section
		LoadModuleDll(DllName());
	}
	catch (const std::exception& error) {
		throw std::runtime_error("Unable to load dll for module " + Name() + ". " + std::string(error.what()));
	}
}


std::string IniModule::Directory()
{
	auto dllSetting = DllName();
	return fs::path(dllSetting).remove_filename().string();
}

std::string IniModule::DllName()
{
	auto defaultDllName = (fs::path(Name()) / "op2mod.dll").string();
	return iniSection.GetValue("Dll", defaultDllName);
}
