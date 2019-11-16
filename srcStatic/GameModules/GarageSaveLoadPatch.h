#pragma once

#include "../GameModule.h"

class GarageSaveLoadPatch : public GameModule
{
public:
	GarageSaveLoadPatch();

	void Load() override;
	bool Unload() override { return true; }
};
