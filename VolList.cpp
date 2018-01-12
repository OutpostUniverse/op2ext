#include "VolList.h"

#include "OP2Memory.h"
#include "GlobalDefines.h"

VolList::VolList()
{
	volSearchEntryList = buffer;
}

VolList::~VolList() { }

void VolList::AddVolFile(std::string volPath)
{
	if (MaxVolFileCountReached(volPath)) {
		return;
	}
	
	volPaths.push_back(std::vector<char>(volPath.c_str(), volPath.c_str() + volPath.size() + 1u));
	
	InitializeVolSearchEntry(&volPaths.back()[0]);

	OutputDebug("Add file to VolList: " + volPath + "\n");
}

void VolList::LoadVolFiles()
{
	EndList();

	VolSearchEntry *vol = &volSearchEntryList[0];
	int *vol2 = &volSearchEntryList[0].unknown1;
	int *vol3 = &volSearchEntryList[numberOfVolFiles].unknown1;
	VolSearchEntry *vol4 = &volSearchEntryList[numberOfVolFiles - 1];

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

bool VolList::MaxVolFileCountReached(std::string volPath)
{
	if (numberOfVolFiles > VolSearchBufferSize - 1) {
		PostErrorMessage("VolList.cpp", __LINE__, "Too many vol files loaded. Ignoring the vol file '" + volPath + "'");
		return true;
	}

	return false;
}

void VolList::InitializeVolSearchEntry(char* pVolPath)
{
	volSearchEntryList[numberOfVolFiles].unknown1 = 0;
	volSearchEntryList[numberOfVolFiles].flags = 1;
	volSearchEntryList[numberOfVolFiles].unknown2 = 0;
	volSearchEntryList[numberOfVolFiles].pFilename = pVolPath;

	numberOfVolFiles++;
}

void VolList::EndList()
{
	// Add end of volFileEntries search item.
	InitializeVolSearchEntry((char*)0);
}
