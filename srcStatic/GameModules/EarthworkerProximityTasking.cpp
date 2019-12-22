#include "EarthworkerProximityTasking.h"
#include "../OP2Memory.h"

EarthworkerProximityTasking::EarthworkerProximityTasking() : GameModule("EarthworkerProximityTasking") { }

void EarthworkerProximityTasking::Load()
{
	constexpr unsigned short earthworkerFix = 0x9090;

	Op2MemCopy(0x438E07, sizeof(earthworkerFix), &earthworkerFix);
}
