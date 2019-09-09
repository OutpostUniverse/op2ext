#include "OP2Memory.h"
#include <gtest/gtest.h>


extern bool memoryCommandsDisabled;


class Op2RelinkCallTest : public ::testing::Test {
protected:
  void SetUp() override {
    memoryCommandsDisabled = false;
  }
  void TearDown() override {
    memoryCommandsDisabled = true;
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
  std::size_t callInstructionAddr = reinterpret_cast<std::size_t>(&callInstruction);
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
  // Relative address should point back to itself
  EXPECT_EQ(-sizeof(CallInstruction), callInstruction.relativeAddress);
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
