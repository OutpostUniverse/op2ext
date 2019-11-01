#include "IpDropDown.h"
#include "../FileSystemHelper.h"
#include "../OP2Memory.h"
#include "../IniFile.h"
#include <winsock.h> // From winsock.h, using functions inet_addr & gethostbyname.
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string>


BOOL __stdcall EnableWindowNew(HWND hWnd, BOOL bEnable);
unsigned long __stdcall inet_addrNew(const char* cp);
void WriteAddressesToIniFile();

char ipStrings[10][47];
int numIpStrings = 0;

// Data constants for InstallIpDropDown
constexpr auto newEnableWindowAddr = &EnableWindowNew;
constexpr auto newInetAddr = &inet_addrNew;
constexpr std::size_t populateComboBoxAddr = 0x004197C1;
constexpr std::size_t saveIpTextAddr = 0x004C0E36;
constexpr std::size_t nopDataAddr = 0x0041988F;


IPDropDown::IPDropDown()
	: GameModule("IPDropDown")
{
}

void IPDropDown::Load()
{
	// patch the call to EnableWindow so we can add strings.
	Op2MemSetDword(populateComboBoxAddr, &newEnableWindowAddr);
	Op2MemSetDword(saveIpTextAddr, &newInetAddr);
	Op2MemSet(nopDataAddr, 0x90, 14);
}

BOOL __stdcall EnableWindowNew(HWND hWnd, BOOL bEnable)
{
	// enable the window
	BOOL result = EnableWindow(hWnd, bEnable);

	auto iniSection = IniSection(GetOutpost2IniPath(), "IPHistory");

	// populate the list with strings
	numIpStrings = 0;
	for (int i = 0; i < 10; i++)
	{
		std::string ipString = iniSection[std::to_string(i)];
		strcpy_s(ipStrings[i], ipString.c_str());

		if (strlen(ipStrings[i]) > 0)
		{
			SendMessage(hWnd, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(ipStrings[i]));
			numIpStrings++;
		}
	}

	return result;
}

unsigned long __stdcall inet_addrNew(const char* cp)
{
	// Check if the IP or hostname was valid
	unsigned long result = inet_addr(cp);
	if (result == INADDR_NONE)
	{
		hostent* he = gethostbyname(cp);

		if (!he) {
			return INADDR_NONE;
		}

		result = (unsigned long)*he->h_addr_list[0];
	}

	// IP lookup ok, so look for duplicates and remove from list
	for (int i = 0; i < numIpStrings; i++)
	{
		if (strcmp(ipStrings[i], cp) != 0) {
			continue;
		}

		// found duplicate, push all the other items up on top of it
		for (int j = i; j < numIpStrings - 1; j++) {
			strcpy_s(ipStrings[j], ipStrings[j + 1]);
		}

		if (numIpStrings > 0) {
			numIpStrings--;
		}
	}

	if (numIpStrings < 10) {
		numIpStrings++;
	}

	// Next, push everything else down
	for (int i = numIpStrings - 1; i > 0; i--) {
		strcpy_s(ipStrings[i], ipStrings[i - 1]);
	}

	// Copy it in
	strcpy_s(ipStrings[0], cp);

	WriteAddressesToIniFile();

	return result;
}

void WriteAddressesToIniFile()
{
	auto iniSection = IniSection(GetOutpost2IniPath(), "IPHistory");

	iniSection.ClearSection();
	for (int i = 0; i < numIpStrings; i++) {
		iniSection.SetValue(std::to_string(i), ipStrings[i]);
	}
}
