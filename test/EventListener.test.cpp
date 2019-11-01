#include "EventListener.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>


struct MockHandler {
	MockHandler() = default;

	// Unable to directly mock operator overloads, so mock indirectly
	MOCK_METHOD0(OperatorCall, void());
	void operator()() { OperatorCall(); }
};


TEST(EventListener, AddRaiseRemove) {
	EventListener<void()> eventListener;
	MockHandler mockHandler1;
	MockHandler mockHandler2;
	auto eventHandler1 = [&mockHandler1](){ mockHandler1(); };
	auto eventHandler2 = [&mockHandler2](){ mockHandler2(); };

	EXPECT_CALL(mockHandler1, OperatorCall()).Times(1);
	EXPECT_CALL(mockHandler2, OperatorCall()).Times(2);

	eventListener.Add(eventHandler2);
	eventListener.Raise(); // Call second handler once
	eventListener.Add(eventHandler1);
	eventListener.Raise(); // Call both handlers once
	eventListener.RemoveAll();
	eventListener.Raise(); // Call no handlers
}
