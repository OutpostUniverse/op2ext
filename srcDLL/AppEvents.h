#pragma once

#include "EventListener.h"


class AppEvents
{
public:
	AppEvents() = default;
	void Activate();

	EventListener<void()> onInit;
	EventListener<void()> onLoadShell;
	EventListener<void()> onShutDown;
};
