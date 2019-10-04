#pragma once

#include "../GameModule.h"

class IPDropDown : public GameModule
{
public:
	IPDropDown();

	void Load() override;
	bool Unload() override { return true; }
};
