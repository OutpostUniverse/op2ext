// VolList.cpp: implementation of the VolList class.
//
//////////////////////////////////////////////////////////////////////

#include "VolList.h"
#include "op2ext.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

VolList::VolList()
{
	// initialize variables
	numItems = 0;
	itemList = 0;
}

VolList::~VolList()
{
	// uninitialize everything (except the last two entries)
	for (unsigned int i = 0; i < numItems-2; i++)
		free(itemList[i].pFileName);

	// delete the list itself
	free(itemList);
}

void VolList::AddItem(char *volToAdd)
{
	// Add a VOL to the list
	numItems++;
	
	// Resize the array
	itemList = (struct VolSearchEntry *)realloc(itemList, numItems * sizeof(struct VolSearchEntry));

	// Initialize the element
	itemList[numItems-1].unknown1 = 0;
	itemList[numItems-1].flags = 1;
	itemList[numItems-1].unknown2 = 0;
	itemList[numItems-1].pFileName = strdup(volToAdd);

	DBG("VolList::AddItem(\"");
	DBG(volToAdd);
	DBG("\");\n");
}

void VolList::EndList()
{
	// End the list
	numItems += 2;
	itemList = (struct VolSearchEntry *)realloc(itemList, numItems * sizeof(struct VolSearchEntry));

	// allocate memory for the string
	itemList[numItems-2].pFileName = (char*)0;
	itemList[numItems-2].unknown1 = 0;
	itemList[numItems-2].flags = 1;
	itemList[numItems-2].unknown2 = 0;
	itemList[numItems-1].pFileName = VOLMAGIC;
	itemList[numItems-1].unknown1 = 0;
	itemList[numItems-1].flags = 1;
	itemList[numItems-1].unknown2 = 0;

	//vol3 = itemList[numVols]->unknown1;
	//vol4 = itemList[numVols+1];
}

void VolList::Install()
{
	// Install into OP2's memory
	DWORD oldAttr;
	// First, unprotect all memory we will be using
	VirtualProtect((LPVOID)0x00471070, 4, PAGE_EXECUTE_READWRITE, &oldAttr);
	VirtualProtect((LPVOID)0x004711DA, 4, PAGE_EXECUTE_READWRITE, &oldAttr);
	VirtualProtect((LPVOID)0x00471206, 4, PAGE_EXECUTE_READWRITE, &oldAttr);
	VirtualProtect((LPVOID)0x0047136C, 4, PAGE_EXECUTE_READWRITE, &oldAttr);
	VirtualProtect((LPVOID)0x004713AA, 4, PAGE_EXECUTE_READWRITE, &oldAttr);
	VirtualProtect((LPVOID)0x004713D1, 4, PAGE_EXECUTE_READWRITE, &oldAttr);
	VirtualProtect((LPVOID)0x00471439, 4, PAGE_EXECUTE_READWRITE, &oldAttr);
	VirtualProtect((LPVOID)0x00471474, 4, PAGE_EXECUTE_READWRITE, &oldAttr);
	VirtualProtect((LPVOID)0x0047126E, 4, PAGE_EXECUTE_READWRITE, &oldAttr);
	VirtualProtect((LPVOID)0x0047128B, 4, PAGE_EXECUTE_READWRITE, &oldAttr);
	VirtualProtect((LPVOID)0x00471389, 4, PAGE_EXECUTE_READWRITE, &oldAttr);
	VirtualProtect((LPVOID)0x004713E8, 4, PAGE_EXECUTE_READWRITE, &oldAttr);
	VirtualProtect((LPVOID)0x004713EF, 4, PAGE_EXECUTE_READWRITE, &oldAttr);
	VirtualProtect((LPVOID)0x00471408, 4, PAGE_EXECUTE_READWRITE, &oldAttr);
	VirtualProtect((LPVOID)0x00471457, 4, PAGE_EXECUTE_READWRITE, &oldAttr);
	VirtualProtect((LPVOID)0x0047111F, 4, PAGE_EXECUTE_READWRITE, &oldAttr);

	VolSearchEntry *vol = &itemList[0];
	int *vol2 = &itemList[0].unknown1;
	int *vol3 = &itemList[numItems].unknown1;
	VolSearchEntry *vol4 = &itemList[numItems-1];

	// Change operand of the MOV instruction
	memcpy((void*)0x00471070, &vol, 4);

	// VolumeList+4 a bunch of other subs want. Better change those
	//memcpy((void*)0x00471142, &vol2, 4);
	memcpy((void*)0x004711DA, &vol2, 4);
	memcpy((void*)0x00471206, &vol2, 4);
	memcpy((void*)0x0047136C, &vol2, 4);
	memcpy((void*)0x004713AA, &vol2, 4);
	memcpy((void*)0x004713D1, &vol2, 4);
	memcpy((void*)0x00471439, &vol2, 4);
	memcpy((void*)0x00471474, &vol2, 4);

	// The 2nd element of the null entry some stuff wants
	memcpy((void*)0x0047126E, &vol3, 4);
	memcpy((void*)0x0047128B, &vol3, 4);
	memcpy((void*)0x00471389, &vol3, 4);
	memcpy((void*)0x004713E8, &vol3, 4);
	memcpy((void*)0x004713EF, &vol3, 4);
	memcpy((void*)0x00471408, &vol3, 4);
	memcpy((void*)0x00471457, &vol3, 4);

	memcpy((void*)0x0047111F, &vol4, 4);
}