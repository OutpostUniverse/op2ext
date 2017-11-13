// VolList.cpp: implementation of the VolList class.
//
//////////////////////////////////////////////////////////////////////

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
	// initialize variables
	numItems = 0;
	itemList = buffer;
}

VolList::~VolList()
{
}


void VolList::AddItem(const char *volToAdd)
{
	// Abort if too many items are added
	if (numItems > VolSearchBufferSize-2)
	{
		MessageBox(0, "Too many .vol files loaded. Ignoring.", "Addon Vol file error", 0);
		return;		// Abort
	}
	// Add a VOL to the list
	numItems++;
	
	// Initialize the element
	itemList[numItems-1].unknown1 = 0;
	itemList[numItems-1].flags = 1;
	itemList[numItems-1].unknown2 = 0;
	itemList[numItems-1].pFileName = volToAdd;

	DBG("VolList::AddItem(\"");
	DBG(volToAdd);
	DBG("\");\n");
}

void VolList::EndList()
{
	// End the list
	numItems += 1;

	// allocate memory for the string
	itemList[numItems-1].pFileName = (char*)0;
	itemList[numItems-1].unknown1 = 0;
	itemList[numItems-1].flags = 1;
	itemList[numItems-1].unknown2 = 0;
}

void VolList::Install()
{
	// Install into OP2's memory

	VolSearchEntry *vol = &itemList[0];
	int *vol2 = &itemList[0].unknown1;
	int *vol3 = &itemList[numItems].unknown1;
	VolSearchEntry *vol4 = &itemList[numItems-1];

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