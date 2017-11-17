#include "VolList.h"
#include "op2ext.h"


// Static buffer, to avoid dynamic memory allocation before heap is initialized
const int VolSearchBufferSize = 32;
VolSearchEntry buffer[VolSearchBufferSize];


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

VolList::VolList()
{
	volSearchEntryList = buffer;
}

VolList::~VolList() { }

void VolList::AddVolFile(std::string volPath)
{
	if (MaxVolFileCountReached()) {
		return;
	}

	volPaths.push_back(std::vector<char>(volPath.c_str(), volPath.c_str() + volPath.size() + 1u));
	
	InitializeVolSearchEntry(&volPaths[volPaths.size() - 1][0]);

	DBG("VolList::AddItem(\"");
	DBG(volPath.c_str());
	DBG("\");\n");
}

void VolList::EndList()
{	
	// Add end of volFileEntries search item.
	InitializeVolSearchEntry((char*)0);
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

bool VolList::MaxVolFileCountReached()
{
	if (numberOfVolFiles > VolSearchBufferSize - 2) {
		MessageBox(0, "Too many .vol files loaded. Ignoring.", "Addon Vol file error", 0);
		return true;
	}

	return false;
}

void VolList::InitializeVolSearchEntry(char* pVolPath)
{
	numberOfVolFiles++;

	volSearchEntryList[numberOfVolFiles - 1].unknown1 = 0;
	volSearchEntryList[numberOfVolFiles - 1].flags = 1;
	volSearchEntryList[numberOfVolFiles - 1].unknown2 = 0;
	volSearchEntryList[numberOfVolFiles - 1].pFilename = pVolPath;
}