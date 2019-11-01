#pragma once

#include <vector>
#include <functional>
#include <algorithm>
#include <utility>


template <typename MethodSignature>
class EventListener;


template <typename ReturnType, typename ...Args>
class EventListener<ReturnType(Args...)>
{
public:
	using MethodSignature = ReturnType(Args...);

	EventListener() = default;

	void Add(std::function<MethodSignature> eventHandler) {
		eventHandlers.push_back(std::move(eventHandler));
	}
	void RemoveAll() {
		eventHandlers.clear();
	}

	void Raise(Args... args) {
		for (auto& eventHandler : eventHandlers) {
			eventHandler(args...);
		}
	}

private:
	std::vector<std::function<MethodSignature>> eventHandlers;
};
