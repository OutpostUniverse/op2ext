#include "StringConversion.h"
#include <gtest/gtest.h>
#include <string>
#include <type_traits>


TEST(StringConversion, ConvertLPWToString)
{
	std::string output;

	// Convert empty string
	EXPECT_TRUE(ConvertLPWToString(output, L""));
	EXPECT_EQ("", output);

	// Convert non-empty string
	EXPECT_TRUE(ConvertLPWToString(output, L"Hello world"));
	EXPECT_EQ("Hello world", output);
}

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

TEST(StringConversion, SplitString)
{
	// Empty string
	EXPECT_EQ(std::vector<std::string>{}, SplitString("", ','));

	// Single entry
	EXPECT_EQ(std::vector<std::string>{"A"}, SplitString("A", ','));

	// Empty multi entry
	// EXPECT_EQ((std::vector<std::string>{"", ""}), SplitString(",", ','));
	// EXPECT_EQ((std::vector<std::string>{"", "", ""}), SplitString(",,", ','));
	// EXPECT_EQ((std::vector<std::string>{"", "", "", ""}), SplitString(",,,", ','));

	// Multi entry
	EXPECT_EQ((std::vector<std::string>{"A", "B"}), SplitString("A,B", ','));
	EXPECT_EQ((std::vector<std::string>{"A", "B", "C"}), SplitString("A,B,C", ','));

	// Multi entry with spaces
	EXPECT_EQ((std::vector<std::string>{"A", "B"}), SplitString("A, B", ','));
	EXPECT_EQ((std::vector<std::string>{"A", "B"}), SplitString("A ,B", ','));
	EXPECT_EQ((std::vector<std::string>{"A", "B"}), SplitString("A , B", ','));
	EXPECT_EQ((std::vector<std::string>{"A", "B", "C"}), SplitString("A, B, C", ','));
	EXPECT_EQ((std::vector<std::string>{"A", "B", "C"}), SplitString("A ,B ,C", ','));
	EXPECT_EQ((std::vector<std::string>{"A", "B", "C"}), SplitString("A , B , C", ','));

	// Embedded spaces
	EXPECT_EQ((std::vector<std::string>{"A A"}), SplitString("A A", ','));
	EXPECT_EQ((std::vector<std::string>{"A A", "B  B", "C\tC"}), SplitString("A A,B  B,C\tC", ','));

	// Mixed spaces with embedded spaces
	EXPECT_EQ((std::vector<std::string>{"A A", "B  B", "C\tC"}), SplitString("A A , B  B , C\tC", ','));

	// Space separated
	EXPECT_EQ((std::vector<std::string>{"A", "B", "C"}), SplitString("A B C", ' '));
	// EXPECT_EQ((std::vector<std::string>{"", "A", "B", "C", ""}), SplitString(" A B C ", ' '));
	EXPECT_EQ((std::vector<std::string>{"A\tB\tC"}), SplitString("A\tB\tC", ' '));
	EXPECT_EQ((std::vector<std::string>{"A", "B", "C"}), SplitString("A \tB\t C", ' '));
	EXPECT_EQ((std::vector<std::string>{"A", "B", "C"}), SplitString("\tA\t \tB\t \tC\t", ' '));
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
