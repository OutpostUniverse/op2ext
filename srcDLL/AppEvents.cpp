#include "AppEvents.h"
#include "OP2Memory.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>


namespace {
	AppEvents* activeAppEvents = nullptr;
}


void AppEvents::Activate() {
	activeAppEvents = this;
}


// TApp is an exported class from Outpost2.exe.
// We need to replace some of its methods with a compatible signature
class TApp
{
public:
	int Init();
	void ShutDown();

	static HINSTANCE WINAPI LoadShell(LPCSTR lpLibFileName);
};



// Replacement method for TApp:Init(), originally in Outpost2.exe
int TApp::Init()
{
	// Trigger event
	if (activeAppEvents) {
		activeAppEvents->onInit.Raise();
	}

	// Call original function
	return (this->*GetMethodPointer<decltype(&TApp::Init)>(0x00485B20))();
}

// Replacement method for TApp:ShutDown(), originally in Outpost2.exe
void TApp::ShutDown()
{
	// Call original function
	(this->*GetMethodPointer<decltype(&TApp::ShutDown)>(0x004866E0))();

	// Trigger event
	if (activeAppEvents) {
		activeAppEvents->onShutDown.Raise();
	}
}

// Replacement method for call to LoadLibrary, which the game calls to load OP2Shell.dll
// Must use WINAPI macro (__stdcall specifier) to ensure callee cleans the stack
// By default, for plain functions, the caller cleans the stack, rather than the callee
HINSTANCE WINAPI TApp::LoadShell(LPCSTR lpLibFileName)
{
	// First try to load it
	HINSTANCE hInstance = LoadLibraryA(lpLibFileName);

	if (hInstance)
	{
		// Trigger event
		if (activeAppEvents) {
			activeAppEvents->onLoadShell.Raise();
		}
	}

	return hInstance;
}


// This is a new jump table entry used to load OP2Shell.dll
// By redirecting the call instruction to use this entry, we can hook
// the call to LoadLibrary and insert custom event code
// The jump table entry needs a long term fixed address (global variable)
constexpr auto replacementLoadLibrary = &TApp::LoadShell;

bool InstallTAppEventHooks()
{
	// Replace call to gTApp.Init with custom routine
	if (!Op2RelinkCall(0x004A8877, GetMethodVoidPointer(&TApp::Init))) {
		return false;
	}

	// Replace call to gTApp.ShutDown with custom routine
	Op2RelinkCall(0x004A88A5, GetMethodVoidPointer(&TApp::ShutDown));

	// Replace call to LoadLibrary with custom routine (address is indirect)
	Op2MemSetDword(0x00486E0A, &replacementLoadLibrary);

	return true;
}
