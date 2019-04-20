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
	ToLowerInPlace(str);
	EXPECT_EQ("test", str);

	// Test Empty string
	str = "";
	ASSERT_NO_THROW(ToLowerInPlace(str));
	EXPECT_EQ("", str);
}
