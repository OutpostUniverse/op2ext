#pragma once

#include "DllModule.h"
#include "../IniFile.h"


class IniModule : public DllModule
{
public:
	IniModule(IniSection iniSection);

private:
	IniSection iniSection;
};
