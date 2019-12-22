#pragma once

#include "../GameModule.h"

// Allows designating walls for construction in a 3x3 area around where
// other earthworkers are already working.
// Adapted from Caught in the Crossfire 2 mission code

class EarthworkerProximityTasking : public GameModule
{
public:
	EarthworkerProximityTasking();

	void Load() override;
	bool Unload() override { return true; }
};
