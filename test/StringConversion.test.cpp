#include "StringConversion.h"
#include <gtest/gtest.h>
#include <string>

TEST(StringConversion, ToLower)
{
	EXPECT_EQ("test", ToLower("TEST"));
	EXPECT_EQ("", ToLower(""));
}

TEST(StringConversion, ToLowerInPlace)
{
	std::string str("TEST");
	EXPECT_EQ("test", ToLowerInPlace(str));

	// Test Empty string
	str = "";
	EXPECT_EQ("", ToLowerInPlace(str));
}
