#pragma once

#include "../GameModule.h"

class IPDropDown : public GameModule
{
public:
	IPDropDown() : GameModule("IPDropDown") { }

	void Load() override;
	bool Unload() override { return true; }
};
