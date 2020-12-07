#include "IniModule.h"
#include "../FsInclude.h"
#include "../FileSystemHelper.h"
#include <utility>
#include <stdexcept>


IniModule::IniModule(IniSection iniSection)
	: DllModule(iniSection.SectionName()), iniSection(std::move(iniSection))
{
	try {
		// Get the DLL name from the corresponding section
		const fs::path path = DllName();
		if (path.has_filename()) {
			LoadModuleDll((path.is_absolute() ? path : GetOpuDirectory() / path).string());
		}
	}
	catch (const std::exception& error) {
		throw std::runtime_error("Unable to load dll for module " + Name() + ". " + std::string(error.what()));
	}
}


std::string IniModule::Directory()
{
	// For ini modules, either one of Dll= or Path= should be set.
	fs::path iniSetting(iniSection.GetValue("Path"));

	if (iniSetting.empty()) {
		iniSetting = DllName();
		iniSetting.remove_filename();
	}

	return iniSetting.string();
}

std::string IniModule::DllName()
{
	return iniSection.GetValue("Dll", iniSection.SectionName() + "\\");
}
