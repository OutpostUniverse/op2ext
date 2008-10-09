// VolList.h: interface for the VolList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VOLLIST_H__C60CEA1D_F2FD_46A2_B937_D5ABDB4DCF43__INCLUDED_)
#define AFX_VOLLIST_H__C60CEA1D_F2FD_46A2_B937_D5ABDB4DCF43__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

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

#endif // !defined(AFX_VOLLIST_H__C60CEA1D_F2FD_46A2_B937_D5ABDB4DCF43__INCLUDED_)
