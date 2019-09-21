#include "StringConversion.h"
#include <gtest/gtest.h>
#include <string>
#include <type_traits>


TEST(StringConversion, ConvertLpctstrToStringNarrow)
{
	LPCSTR rawString = "test string";
	auto result = ConvertLpctstrToString(rawString);
	EXPECT_EQ(rawString, result);
	EXPECT_TRUE((std::is_same<std::string, decltype(result)>::value));
}

TEST(StringConversion, ConvertLpctstrToStringWide)
{
	LPCWSTR rawString = L"test string";
	auto result = ConvertLpctstrToString(rawString);
	EXPECT_EQ(rawString, result);
	EXPECT_TRUE((std::is_same<std::wstring, decltype(result)>::value));
}

TEST(StringConversion, ToLower)
{
	EXPECT_EQ("test", ToLower("TEST"));
	EXPECT_EQ("", ToLower(""));
}

TEST(StringConversion, ToLowerInPlace)
{
	std::string str("TEST");
	EXPECT_EQ("test", ToLowerInPlace(str));
	EXPECT_EQ("test", str);

	// Test Empty string
	str = "";
	EXPECT_EQ("", ToLowerInPlace(str));
	EXPECT_EQ("", str);
}
