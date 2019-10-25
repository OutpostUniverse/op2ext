#pragma once

#include "DllModule.h"
#include "../IniFile.h"


class IniModule : public DllModule
{
public:
	IniModule(IniSection iniSection);

	std::string Directory() override;

private:
	std::string DllName();

	IniSection iniSection;
};
