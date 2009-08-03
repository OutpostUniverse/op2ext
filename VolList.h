#ifndef VolList_H
#define VolList_H


#define WIN32_LEAN_AND_MEAN
#include <windows.h>


// VOL entry
struct VolSearchEntry {
	char *pFileName;
	int unknown1;
	int flags;
	int unknown2;
};

#define VOLMAGIC ((char *)0x004E3910) // game checks for this at end of list

class VolList  
{
public:
	VolList();
	virtual ~VolList();

	void AddItem(char *volToAdd);
	void EndList();
	void Install();

	unsigned int numItems;
	VolSearchEntry *itemList;

};


#endif	// VolList_H
