#include "VolList.h"
#include "OP2Memory.h"
#include "GlobalDefines.h"
#include <utility>
#include <cstddef>

VolList::VolList()
{
	volSearchEntryList = buffer;
}

VolList::~VolList() { }

void VolList::AddVolFile(std::string volPath)
{
	if (IsFull()) {
		PostErrorMessage("VolList.cpp", __LINE__, "Too many vol files loaded. Ignoring the vol file '" + volPath + "'");
		return;
	}
	
	volPaths.push_back(std::move(volPath));

	OutputDebug("Add file to VolList: " + volPath + "\n");
}

void VolList::LoadVolFiles()
{
	CreateVolSearchEntryList();

	VolSearchEntry *vol = &volSearchEntryList[0];
	int *vol2 = &volSearchEntryList[0].unknown1;
	int *vol3 = &volSearchEntryList[volPaths.size() - 1].unknown1;
	VolSearchEntry *vol4 = &volSearchEntryList[volPaths.size() - 2];

	// Change operand of the MOV instruction
	Op2MemSetDword((void*)0x00471070, vol);

	// VolumeList+4 a bunch of other subs want. Better change those
	//memcpy((void*)0x00471142, &vol2, 4);
	Op2MemSetDword((void*)0x004711DA, vol2);
	Op2MemSetDword((void*)0x00471206, vol2);
	Op2MemSetDword((void*)0x0047136C, vol2);
	Op2MemSetDword((void*)0x004713AA, vol2);
	Op2MemSetDword((void*)0x004713D1, vol2);
	Op2MemSetDword((void*)0x00471439, vol2);
	Op2MemSetDword((void*)0x00471474, vol2);

	// The 2nd element of the null entry some stuff wants
	Op2MemSetDword((void*)0x0047126E, vol3);
	Op2MemSetDword((void*)0x0047128B, vol3);
	Op2MemSetDword((void*)0x00471389, vol3);
	Op2MemSetDword((void*)0x004713E8, vol3);
	Op2MemSetDword((void*)0x004713EF, vol3);
	Op2MemSetDword((void*)0x00471408, vol3);
	Op2MemSetDword((void*)0x00471457, vol3);

	Op2MemSetDword((void*)0x0047111F, vol4);
}

bool VolList::IsFull() const
{
	return volPaths.size() >= MaxVolumeCount;
}

// After calling CreateVolSearchEntryList, do not change the contents of volPaths. 
// If Small String Optimization (SSO) is applied by the compiler, the associated pointers will become invalid.
void VolList::CreateVolSearchEntryList()
{
	for (std::size_t i = 0; i < volPaths.size(); ++i) {
		volSearchEntryList[i] = VolSearchEntry{ volPaths[i].c_str(), 0, 1, 0 };
	}

	// Add end of volFileEntries search item.
	volSearchEntryList[volPaths.size()] = VolSearchEntry{ nullptr, 0, 1, 0 };
}
