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
	EXPECT_EQ("test", str);

	// Test Empty string
	str = "";
	EXPECT_EQ("", ToLowerInPlace(str));
	EXPECT_EQ("", str);
}

TEST(StringConversion, TrimString)
{
	// Empty string
	EXPECT_EQ("", TrimString(""));

	// Only whitespace
	EXPECT_EQ("", TrimString(" "));
	EXPECT_EQ("", TrimString("\t"));
	EXPECT_EQ("", TrimString("\t "));
	EXPECT_EQ("", TrimString(" \t"));
	EXPECT_EQ("", TrimString("\t\t"));

	// Only no whitespace
	EXPECT_EQ("A", TrimString("A"));
	EXPECT_EQ("AA", TrimString("AA"));
	EXPECT_EQ("ABC", TrimString("ABC"));

	// Leading whitespace
	EXPECT_EQ("A", TrimString(" A"));
	EXPECT_EQ("A", TrimString("\tA"));

	// Trailing whitespace
	EXPECT_EQ("A", TrimString("A "));
	EXPECT_EQ("A", TrimString("A\t"));

	// Leading and trailing whitespace
	EXPECT_EQ("A", TrimString(" A "));
	EXPECT_EQ("A", TrimString("\tA\t"));
	EXPECT_EQ("A", TrimString("  A  "));
	EXPECT_EQ("A", TrimString(" \tA\t "));
	EXPECT_EQ("A", TrimString("\t A \t"));
	EXPECT_EQ("A", TrimString("\t\tA\t\t"));

	// Embedded whitespace
	EXPECT_EQ("A A", TrimString("A A"));
	EXPECT_EQ("A\tA", TrimString("A\tA"));
	EXPECT_EQ("A  A", TrimString("A  A"));
	EXPECT_EQ("A \tA", TrimString("A \tA"));
	EXPECT_EQ("A\t A", TrimString("A\t A"));
	EXPECT_EQ("A\t\tA", TrimString("A\t\tA"));
}
