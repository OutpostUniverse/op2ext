#include "OP2Memory.h"
#include <gtest/gtest.h>


extern bool memoryPatchingEnabled;


class Op2RelinkCallTest : public ::testing::Test {
protected:
	void SetUp() override {
		memoryPatchingEnabled = true;
	}
	void TearDown() override {
		memoryPatchingEnabled = false;
	}

	static constexpr unsigned char CallOpcode = 0xE8;
	#pragma pack(push, 1)
	struct CallInstruction {
		unsigned char opcode;
		uint32_t relativeAddress;
	};
	#pragma pack(pop)
	static_assert(5 == sizeof(CallInstruction));

	CallInstruction callInstruction = {CallOpcode, 0x00000000};
	const uintptr_t callInstructionAddr = reinterpret_cast<uintptr_t>(&callInstruction);
};


TEST_F(Op2RelinkCallTest, GoodOpcode) {
	// Modification should succeed
	EXPECT_TRUE(Op2RelinkCall(callInstructionAddr, nullptr));
	// Opcode should stay the same
	EXPECT_EQ(CallOpcode, callInstruction.opcode);
	// Relative address should change
	EXPECT_NE(0x00000000u, callInstruction.relativeAddress);
}

TEST_F(Op2RelinkCallTest, GoodOpcodeSelfCall) {
	// Modification should succeed, now calls itself
	EXPECT_TRUE(Op2RelinkCall(callInstructionAddr, &callInstruction));
	// Opcode should stay the same
	EXPECT_EQ(CallOpcode, callInstruction.opcode);
	// Relative address should point back to itself, at -sizeof(CallInstruction)
	// To avoid unary minus with unsigned types: -x = ~x + 1
	EXPECT_EQ(~sizeof(CallInstruction) + 1, callInstruction.relativeAddress);
}

TEST_F(Op2RelinkCallTest, BadOpcode) {
	// Set invalid opcode
	callInstruction.opcode = 0x00;
	// Modifcation should fail
	EXPECT_FALSE(Op2RelinkCall(callInstructionAddr, nullptr));
	// Nothing should change
	EXPECT_EQ(0x00, callInstruction.opcode);
	EXPECT_EQ(0x00000000u, callInstruction.relativeAddress);
}


class MethodPointerTest : public ::testing::Test {
protected:
	class ExampleClass {
	public:
		int ExampleMethod(int i) {
			return i + 1;
		}
	};
	using MemberPointerType = decltype(&ExampleClass::ExampleMethod);

	ExampleClass exampleObject;
};


TEST_F(MethodPointerTest, GetMethodAddress) {
	EXPECT_EQ(0x00000000u, GetMethodAddress<MemberPointerType>(nullptr));
	EXPECT_NE(0x00000000u, GetMethodAddress(&ExampleClass::ExampleMethod));
}

TEST_F(MethodPointerTest, GetMethodVoidPointer) {
	EXPECT_EQ(nullptr, GetMethodVoidPointer<MemberPointerType>(nullptr));
	EXPECT_NE(nullptr, GetMethodVoidPointer(&ExampleClass::ExampleMethod));
}

TEST_F(MethodPointerTest, GetMethodPointer) {
	EXPECT_EQ(nullptr, GetMethodPointer<MemberPointerType>(0x00000000u));
	EXPECT_NE(nullptr, GetMethodPointer<MemberPointerType>(0x0043210Fu));
}

TEST_F(MethodPointerTest, AddressPointerAddressRoundTrip) {
	EXPECT_EQ(0x00000000u, GetMethodAddress(GetMethodPointer<MemberPointerType>(0x00000000u)));
	EXPECT_EQ(0x0043210Fu, GetMethodAddress(GetMethodPointer<MemberPointerType>(0x0043210Fu)));
}

TEST_F(MethodPointerTest, PointerAddressPointerRoundTrip) {
	EXPECT_EQ(nullptr, GetMethodPointer<MemberPointerType>(GetMethodAddress<MemberPointerType>(nullptr)));
	EXPECT_EQ(&ExampleClass::ExampleMethod, GetMethodPointer<MemberPointerType>(GetMethodAddress(&ExampleClass::ExampleMethod)));
}

TEST_F(MethodPointerTest, GetMethodPointerUse) {
	// Get fixed size_t address of member method (perhaps deduced by disassembler)
	const auto methodAddress = GetMethodAddress(&ExampleClass::ExampleMethod);

	// Cast the raw address back to a usable pointer
	const auto methodPointer = GetMethodPointer<MemberPointerType>(methodAddress);
	// Make sure method pointer is usable on target object
	EXPECT_EQ(2, (exampleObject.*methodPointer)(1));
}
