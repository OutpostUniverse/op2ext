#include "VolList.h"
#include <gtest/gtest.h>


TEST(VolList, EmptyAdd)
{
	VolList volList;

	EXPECT_NO_THROW(volList.AddVolFile(""));
	EXPECT_NO_THROW(volList.AddVolFilesFromDirectory(""));
}
