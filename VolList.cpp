#include "VolList.h"
#include "OP2Memory.h"
#include "GlobalDefines.h"
#include <utility>

VolList::~VolList() { }

void VolList::AddVolFile(std::string volPath)
{
	volPaths.push_back(std::move(volPath));

	OutputDebug("Add file to VolList: " + volPath + "\n");
}

// Patch reference to the original VolSearchEntry[] in Outpost2.exe to point to a replacement
// Note: Addresses of the original array (at various offsets) are hardcoded into several instructions
void VolList::LoadVolFiles()
{
	std::size_t volEntryListSize = CreateVolSearchEntryList();

	auto* vol = &volSearchEntryList[0];
	auto* vol2 = &volSearchEntryList[0].unknown1;
	auto* vol3 = &volSearchEntryList[volEntryListSize].unknown1;
	auto* vol4 = &volSearchEntryList[volEntryListSize - 1];

	// Patch instruction references to old array

	Op2MemSetDword((void*)0x00471070, vol);

	//memcpy((void*)0x00471142, &vol2, 4);
	Op2MemSetDword((void*)0x004711DA, vol2);
	Op2MemSetDword((void*)0x00471206, vol2);
	Op2MemSetDword((void*)0x0047136C, vol2);
	Op2MemSetDword((void*)0x004713AA, vol2);
	Op2MemSetDword((void*)0x004713D1, vol2);
	Op2MemSetDword((void*)0x00471439, vol2);
	Op2MemSetDword((void*)0x00471474, vol2);

	Op2MemSetDword((void*)0x0047126E, vol3);
	Op2MemSetDword((void*)0x0047128B, vol3);
	Op2MemSetDword((void*)0x00471389, vol3);
	Op2MemSetDword((void*)0x004713E8, vol3);
	Op2MemSetDword((void*)0x004713EF, vol3);
	Op2MemSetDword((void*)0x00471408, vol3);
	Op2MemSetDword((void*)0x00471457, vol3);

	Op2MemSetDword((void*)0x0047111F, vol4);
}

// After calling CreateVolSearchEntryList, do not change the contents of volPaths.
// Changes to strings (such as by a vector re-allocation which moves strings), will invalidate cached c_str() pointers.
// In particular, MSVC and other compilers implement the Small String Optimization (SSO).
// SSO will exhibit problems for small strings when improperly cached c_str() pointers are invalidated by a move.
std::size_t VolList::CreateVolSearchEntryList()
{
	// Buffer must include an extra terminator entry for an end of list marker
	volSearchEntryList = std::make_unique<VolSearchEntry[]>(volPaths.size() + 1);

	std::size_t volEntryListSize = 0;
	for (volEntryListSize; volEntryListSize < volPaths.size(); ++volEntryListSize) {
		volSearchEntryList[volEntryListSize] = VolSearchEntry{ volPaths[volEntryListSize].c_str(), 0, 1, 0 };
	}

	// Add end of volFileEntries search item.
	volSearchEntryList[volEntryListSize] = VolSearchEntry{ nullptr, 0, 1, 0 };
	volEntryListSize++;

	return volEntryListSize;
}
