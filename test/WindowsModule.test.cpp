#include "WindowsModule.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>


TEST(WindowsModule, FindModuleName) {
	EXPECT_THAT(FindModuleName(_ReturnAddress()), ::testing::EndsWith(".exe"));
}
