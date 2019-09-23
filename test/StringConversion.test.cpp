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

TEST(StringConversion, Trim)
{
	// Empty string
	EXPECT_EQ("", Trim(""));

	// Only whitespace
	EXPECT_EQ("", Trim(" "));
	EXPECT_EQ("", Trim("\t"));
	EXPECT_EQ("", Trim("\t "));
	EXPECT_EQ("", Trim(" \t"));
	EXPECT_EQ("", Trim("\t\t"));

	// Only no whitespace
	EXPECT_EQ("A", Trim("A"));
	EXPECT_EQ("AA", Trim("AA"));
	EXPECT_EQ("ABC", Trim("ABC"));

	// Leading whitespace
	EXPECT_EQ("A", Trim(" A"));
	EXPECT_EQ("A", Trim("\tA"));

	// Trailing whitespace
	EXPECT_EQ("A", Trim("A "));
	EXPECT_EQ("A", Trim("A\t"));

	// Leading and trailing whitespace
	EXPECT_EQ("A", Trim(" A "));
	EXPECT_EQ("A", Trim("\tA\t"));
	EXPECT_EQ("A", Trim("  A  "));
	EXPECT_EQ("A", Trim(" \tA\t "));
	EXPECT_EQ("A", Trim("\t A \t"));
	EXPECT_EQ("A", Trim("\t\tA\t\t"));

	// Embedded whitespace
	EXPECT_EQ("A A", Trim("A A"));
	EXPECT_EQ("A\tA", Trim("A\tA"));
	EXPECT_EQ("A  A", Trim("A  A"));
	EXPECT_EQ("A \tA", Trim("A \tA"));
	EXPECT_EQ("A\t A", Trim("A\t A"));
	EXPECT_EQ("A\t\tA", Trim("A\t\tA"));
}

TEST(StringConversion, TrimFront)
{
	// Empty string
	EXPECT_EQ("", TrimFront(""));

	// Only whitespace
	EXPECT_EQ("", TrimFront(" "));
	EXPECT_EQ("", TrimFront("\t"));
	EXPECT_EQ("", TrimFront("\t "));
	EXPECT_EQ("", TrimFront(" \t"));
	EXPECT_EQ("", TrimFront("\t\t"));

	// Only no whitespace
	EXPECT_EQ("A", TrimFront("A"));
	EXPECT_EQ("AA", TrimFront("AA"));
	EXPECT_EQ("ABC", TrimFront("ABC"));

	// Leading whitespace
	EXPECT_EQ("A", TrimFront(" A"));
	EXPECT_EQ("A", TrimFront("\tA"));

	// Trailing whitespace
	EXPECT_EQ("A ", TrimFront("A "));
	EXPECT_EQ("A\t", TrimFront("A\t"));

	// Leading and trailing whitespace
	EXPECT_EQ("A ", TrimFront(" A "));
	EXPECT_EQ("A\t", TrimFront("\tA\t"));
	EXPECT_EQ("A  ", TrimFront("  A  "));
	EXPECT_EQ("A\t ", TrimFront(" \tA\t "));
	EXPECT_EQ("A \t", TrimFront("\t A \t"));
	EXPECT_EQ("A\t\t", TrimFront("\t\tA\t\t"));

	// Embedded whitespace
	EXPECT_EQ("A A", TrimFront("A A"));
	EXPECT_EQ("A\tA", TrimFront("A\tA"));
	EXPECT_EQ("A  A", TrimFront("A  A"));
	EXPECT_EQ("A \tA", TrimFront("A \tA"));
	EXPECT_EQ("A\t A", TrimFront("A\t A"));
	EXPECT_EQ("A\t\tA", TrimFront("A\t\tA"));
}

TEST(StringConversion, TrimBack)
{
	// Empty string
	EXPECT_EQ("", TrimBack(""));

	// Only whitespace
	EXPECT_EQ("", TrimBack(" "));
	EXPECT_EQ("", TrimBack("\t"));
	EXPECT_EQ("", TrimBack("\t "));
	EXPECT_EQ("", TrimBack(" \t"));
	EXPECT_EQ("", TrimBack("\t\t"));

	// Only no whitespace
	EXPECT_EQ("A", TrimBack("A"));
	EXPECT_EQ("AA", TrimBack("AA"));
	EXPECT_EQ("ABC", TrimBack("ABC"));

	// Leading whitespace
	EXPECT_EQ(" A", TrimBack(" A"));
	EXPECT_EQ("\tA", TrimBack("\tA"));

	// Trailing whitespace
	EXPECT_EQ("A", TrimBack("A "));
	EXPECT_EQ("A", TrimBack("A\t"));

	// Leading and trailing whitespace
	EXPECT_EQ(" A", TrimBack(" A "));
	EXPECT_EQ("\tA", TrimBack("\tA\t"));
	EXPECT_EQ("  A", TrimBack("  A  "));
	EXPECT_EQ(" \tA", TrimBack(" \tA\t "));
	EXPECT_EQ("\t A", TrimBack("\t A \t"));
	EXPECT_EQ("\t\tA", TrimBack("\t\tA\t\t"));

	// Embedded whitespace
	EXPECT_EQ("A A", TrimBack("A A"));
	EXPECT_EQ("A\tA", TrimBack("A\tA"));
	EXPECT_EQ("A  A", TrimBack("A  A"));
	EXPECT_EQ("A \tA", TrimBack("A \tA"));
	EXPECT_EQ("A\t A", TrimBack("A\t A"));
	EXPECT_EQ("A\t\tA", TrimBack("A\t\tA"));
}
