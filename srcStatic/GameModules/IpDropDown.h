#pragma once

#include "GameModule.h"

class IPDropDown : public GameModule
{
public:
	IPDropDown() : GameModule("IPDropDown") { }

	void Initialize() override;
	bool Destroy() override { return true; }
};
