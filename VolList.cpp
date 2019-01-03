#include "VolList.h"
#include "OP2Memory.h"
#include "GlobalDefines.h"
#include <utility>


void VolList::AddVolFile(std::string volPath)
{
	OutputDebug("Add file to VolList: " + volPath + "\n");
	volPaths.push_back(std::move(volPath));
}

// Patch reference to the original VolSearchEntry[] in Outpost2.exe to point to a replacement
// Note: Addresses of the original array (at various offsets) are hardcoded into several instructions
void VolList::LoadVolFiles()
{
	CreateVolSearchEntryList();

	// Addresses at the start of the array are used for loop initial conditions
	auto* arrayStart1 = &volSearchEntryList[0].pFilename;
	auto* arrayStart2 = &volSearchEntryList[0].volFileRStream;
	// Addresses at the end of the array are used for loop termination conditions
	auto* arrayEnd1 = &volSearchEntryList[volSearchEntryList.size()].pFilename;
	auto* arrayEnd2 = &volSearchEntryList[volSearchEntryList.size()].volFileRStream;

	// Patch instructions so hardcoded references to old array now point to new array

	Op2MemSetDword((void*)0x00471070, arrayStart1);

	Op2MemSetDword((void*)0x00471142, arrayStart2);
	Op2MemSetDword((void*)0x004711DA, arrayStart2);
	Op2MemSetDword((void*)0x00471206, arrayStart2);
	Op2MemSetDword((void*)0x0047136C, arrayStart2);
	Op2MemSetDword((void*)0x004713AA, arrayStart2);
	Op2MemSetDword((void*)0x004713D1, arrayStart2);
	Op2MemSetDword((void*)0x00471439, arrayStart2);
	Op2MemSetDword((void*)0x00471474, arrayStart2);

	Op2MemSetDword((void*)0x0047111F, arrayEnd1);

	Op2MemSetDword((void*)0x0047115E, arrayEnd2);
	Op2MemSetDword((void*)0x0047126E, arrayEnd2);
	Op2MemSetDword((void*)0x0047128B, arrayEnd2);
	Op2MemSetDword((void*)0x00471389, arrayEnd2);
	Op2MemSetDword((void*)0x004713E8, arrayEnd2);
	Op2MemSetDword((void*)0x004713EF, arrayEnd2);
	Op2MemSetDword((void*)0x00471408, arrayEnd2);
	Op2MemSetDword((void*)0x00471457, arrayEnd2);
}

// After calling CreateVolSearchEntryList, do not change the contents of volPaths.
// Changes to strings (such as by a vector re-allocation which moves strings), will invalidate cached c_str() pointers.
// In particular, MSVC and other compilers implement the Small String Optimization (SSO).
// SSO will exhibit problems for small strings when improperly cached c_str() pointers are invalidated by a move.
void VolList::CreateVolSearchEntryList()
{
	volSearchEntryList.clear();

	for (const auto& volPath : volPaths) {
		volSearchEntryList.push_back(VolSearchEntry::Init(volPath.c_str()));
	}

	// Add end of volFileEntries search item (terminator entry)
	volSearchEntryList.push_back(VolSearchEntry::Init(nullptr));
}
