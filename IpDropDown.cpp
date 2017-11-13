#include <winsock.h>		// inet_addr, gethostbyname
#include "IpDropDown.h"
#include "op2ext.h"


char ipStrings[10][47];
int numIpStrings = 0;


BOOL __stdcall EnableWindowNew(HWND hWnd, BOOL bEnable)
{
	// enable the window, of course
	BOOL result = EnableWindow(hWnd, bEnable);

	// populate the list with strings
	char tmpStr[4];
	numIpStrings = 0;
	for (int i = 0; i < 10; i++)
	{
		_itoa_s(i, tmpStr, 10);
		GetPrivateProfileString("IPHistory", tmpStr, NULL, ipStrings[i], MAX_PATH, ".\\outpost2.ini");
		if (strlen(ipStrings[i]) > 0)
		{
			SendMessage(hWnd, CB_ADDSTRING, 0, (LPARAM)ipStrings[i]);
			numIpStrings++;
		}
	}
	return result;
}


unsigned long __stdcall inet_addrNew(const char *cp)
{
	// First of all see if the IP or hostname was valid
	unsigned long result = inet_addr(cp);
	if (result == INADDR_NONE)
	{
		// try to gethostbyname it
		hostent *he = gethostbyname(cp);
		if (!he)
			return INADDR_NONE;
		else
			result = (unsigned long)*he->h_addr_list[0];
	}

	// IP lookup ok, so look for duplicates and remove from list
	for (int i=0; i<numIpStrings; i++)
	{
		if (strcmp(ipStrings[i], cp) != 0)
			continue;
		
		// found duplicate, push all the other items up on top of it
		for (int j=i; j<numIpStrings-1; j++)
			strcpy_s(ipStrings[j], ipStrings[j+1]);
		if (numIpStrings > 0)
			numIpStrings--;
	}

	if (numIpStrings < 10)
		numIpStrings++;

	// Next, push everything else down
	for (int i = numIpStrings - 1; i > 0; i--)
		strcpy_s(ipStrings[i], ipStrings[i-1]);
	
	// Copy it in
	strcpy_s(ipStrings[0], cp);
	// Now write all the strings out
	WritePrivateProfileString("IPHistory", NULL, NULL, ".\\outpost2.ini");
	char tmpStr[4];
	for (int i = 0; i < numIpStrings; i++) {
		_itoa_s(i, tmpStr, 10);
		WritePrivateProfileString("IPHistory", tmpStr, ipStrings[i], ".\\outpost2.ini");
	}

	return result;
}



// Data constants for following function
DWORD newEnableWindowAddr = (DWORD)EnableWindowNew;
DWORD newInetAddr = (DWORD)inet_addrNew;
DWORD *populateComboBoxAddr = (DWORD*)0x004197C1;
DWORD *saveIpTextAddr = (DWORD*)0x004C0E36;
void *nopDataAddr = (void*)0x0041988F;

void InstallIpDropDown()
{
	// patch the call to EnableWindow so we can add strings.
	Op2MemSetDword(populateComboBoxAddr, (int)&newEnableWindowAddr);
	Op2MemSetDword(saveIpTextAddr, (int)&newInetAddr);
	Op2MemSet(nopDataAddr, 0x90, 14);
}
