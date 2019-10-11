#pragma once

// Part of IPDropDown is hardcoded within the Dialog resource ID 2010 inside out2res.dll. 
// Originally this was an edit field and has been modified to be a combo field.
// If IPDropDown is not loaded, the combo field will still be present.

#include "../GameModule.h"

class IPDropDown : public GameModule
{
public:
	IPDropDown();

	void Load() override;
	bool Unload() override { return true; }
};
