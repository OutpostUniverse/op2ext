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
