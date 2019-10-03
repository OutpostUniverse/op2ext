#include "StringConversion.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <string>
#include <type_traits>


TEST(StringConversion, WrapRawStringNarrow)
{
	const char* rawString = "test string";
	auto result = WrapRawString(rawString);
	EXPECT_EQ(rawString, result);
	EXPECT_TRUE((std::is_same<std::string, decltype(result)>::value));
}

TEST(StringConversion, WrapRawStringWide)
{
	const wchar_t* rawString = L"test string";
	auto result = WrapRawString(rawString);
	EXPECT_EQ(rawString, result);
	EXPECT_TRUE((std::is_same<std::wstring, decltype(result)>::value));
}

TEST(StringConversion, ConvertWideToNarrow)
{
	// Convert empty string
	EXPECT_EQ("", ConvertWideToNarrow(L""));

	// Convert non-empty string
	EXPECT_EQ("Hello world", ConvertWideToNarrow(L"Hello world"));
}

TEST(StringConversion, ConvertNarrowToWide)
{
	// Convert empty string
	EXPECT_EQ(L"", ConvertNarrowToWide(""));

	// Convert non-empty string
	EXPECT_EQ(L"Hello world", ConvertNarrowToWide("Hello world"));
}


TEST(StringConversion, CopyStdStringIntoCharBufferSafeNoOutputConditions)
{
	constexpr char nonce = 'Z';
	char buffer[8];

	// The nonce should never be overwritten
	buffer[0] = nonce;

	// Null buffer of size zero
	EXPECT_EQ(1u, CopyStdStringIntoCharBuffer("", nullptr, 0));
	EXPECT_EQ(nonce, buffer[0]);
	EXPECT_EQ(2u, CopyStdStringIntoCharBuffer("A", nullptr, 0));
	EXPECT_EQ(nonce, buffer[0]);

	// Null buffer with non-zero size
	EXPECT_EQ(1u, CopyStdStringIntoCharBuffer("", nullptr, sizeof(buffer)));
	EXPECT_EQ(nonce, buffer[0]);
	EXPECT_EQ(2u, CopyStdStringIntoCharBuffer("A", nullptr, sizeof(buffer)));
	EXPECT_EQ(nonce, buffer[0]);

	// Valid buffer but with size zero
	EXPECT_EQ(1u, CopyStdStringIntoCharBuffer("", buffer, 0));
	EXPECT_EQ(nonce, buffer[0]);
	EXPECT_EQ(2u, CopyStdStringIntoCharBuffer("A", buffer, 0));
	EXPECT_EQ(nonce, buffer[0]);
}

TEST(StringConversion, CopyStdStringIntoCharBuffer)
{
	constexpr char nonce = 'Z';
	constexpr char null = '\0';
	char buffer[8];

	// Empty string (must still null terminate)
	buffer[0] = nonce;
	EXPECT_EQ(0u, CopyStdStringIntoCharBuffer("", buffer, sizeof(buffer)));
	EXPECT_EQ(null, buffer[0]);
	// Short string (extra buffer space to spare)
	buffer[1] = nonce;
	EXPECT_EQ(0u, CopyStdStringIntoCharBuffer("A", buffer, sizeof(buffer)));
	EXPECT_EQ(null, buffer[1]);
	// Precise sized string (just enough room for null terminator)
	buffer[7] = nonce;
	EXPECT_EQ(0u, CopyStdStringIntoCharBuffer("1234567", buffer, sizeof(buffer)));
	EXPECT_EQ(null, buffer[7]);
	// Overflow sized string (just barely no room for null terminator)
	buffer[7] = nonce;
	EXPECT_EQ(9u, CopyStdStringIntoCharBuffer("12345678", buffer, sizeof(buffer)));
	EXPECT_EQ(null, buffer[7]);
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

TEST(StringConversion, Split)
{
	// Empty string
	// EXPECT_EQ(std::vector<std::string>{}, Split("", ','));

	// Single entry
	EXPECT_EQ(std::vector<std::string>{"A"}, Split("A", ','));

	// Empty multi entry
	// EXPECT_EQ((std::vector<std::string>{"", ""}), Split(",", ','));
	// EXPECT_EQ((std::vector<std::string>{"", "", ""}), Split(",,", ','));
	// EXPECT_EQ((std::vector<std::string>{"", "", "", ""}), Split(",,,", ','));

	// Multi entry
	EXPECT_EQ((std::vector<std::string>{"A", "B"}), Split("A,B", ','));
	EXPECT_EQ((std::vector<std::string>{"A", "B", "C"}), Split("A,B,C", ','));

	// Multi entry with spaces (no trimming)
	EXPECT_EQ((std::vector<std::string>{"A", " B"}), Split("A, B", ','));
	EXPECT_EQ((std::vector<std::string>{"A ", "B"}), Split("A ,B", ','));
	EXPECT_EQ((std::vector<std::string>{"A ", " B"}), Split("A , B", ','));
	EXPECT_EQ((std::vector<std::string>{"A", " B", " C"}), Split("A, B, C", ','));
	EXPECT_EQ((std::vector<std::string>{"A ", "B ", "C"}), Split("A ,B ,C", ','));
	EXPECT_EQ((std::vector<std::string>{"A ", " B ", " C"}), Split("A , B , C", ','));

	// Space separated
	EXPECT_EQ((std::vector<std::string>{"A", "B", "C"}), Split("A B C", ' '));
	// EXPECT_EQ((std::vector<std::string>{"", "A", "B", "C", ""}), Split(" A B C ", ' '));
	EXPECT_EQ((std::vector<std::string>{"A\tB\tC"}), Split("A\tB\tC", ' '));
}

TEST(StringConversion, SplitAndTrim)
{
	// Empty string
	// EXPECT_EQ(std::vector<std::string>{}, SplitAndTrim("", ','));

	// Single entry
	EXPECT_EQ(std::vector<std::string>{"A"}, SplitAndTrim("A", ','));

	// Empty multi entry
	// EXPECT_EQ((std::vector<std::string>{"", ""}), SplitAndTrim(",", ','));
	// EXPECT_EQ((std::vector<std::string>{"", "", ""}), SplitAndTrim(",,", ','));
	// EXPECT_EQ((std::vector<std::string>{"", "", "", ""}), SplitAndTrim(",,,", ','));

	// Multi entry
	EXPECT_EQ((std::vector<std::string>{"A", "B"}), SplitAndTrim("A,B", ','));
	EXPECT_EQ((std::vector<std::string>{"A", "B", "C"}), SplitAndTrim("A,B,C", ','));

	// Multi entry with spaces
	EXPECT_EQ((std::vector<std::string>{"A", "B"}), SplitAndTrim("A, B", ','));
	EXPECT_EQ((std::vector<std::string>{"A", "B"}), SplitAndTrim("A ,B", ','));
	EXPECT_EQ((std::vector<std::string>{"A", "B"}), SplitAndTrim("A , B", ','));
	EXPECT_EQ((std::vector<std::string>{"A", "B", "C"}), SplitAndTrim("A, B, C", ','));
	EXPECT_EQ((std::vector<std::string>{"A", "B", "C"}), SplitAndTrim("A ,B ,C", ','));
	EXPECT_EQ((std::vector<std::string>{"A", "B", "C"}), SplitAndTrim("A , B , C", ','));

	// Embedded spaces
	EXPECT_EQ((std::vector<std::string>{"A A"}), SplitAndTrim("A A", ','));
	EXPECT_EQ((std::vector<std::string>{"A A", "B  B", "C\tC"}), SplitAndTrim("A A,B  B,C\tC", ','));

	// Mixed spaces with embedded spaces
	EXPECT_EQ((std::vector<std::string>{"A A", "B  B", "C\tC"}), SplitAndTrim("A A , B  B , C\tC", ','));

	// Space separated
	EXPECT_EQ((std::vector<std::string>{"A", "B", "C"}), SplitAndTrim("A B C", ' '));
	// EXPECT_EQ((std::vector<std::string>{"", "A", "B", "C", ""}), SplitAndTrim(" A B C ", ' '));
	EXPECT_EQ((std::vector<std::string>{"A\tB\tC"}), SplitAndTrim("A\tB\tC", ' '));
	EXPECT_EQ((std::vector<std::string>{"A", "B", "C"}), SplitAndTrim("A \tB\t C", ' '));
	EXPECT_EQ((std::vector<std::string>{"A", "B", "C"}), SplitAndTrim("\tA\t \tB\t \tC\t", ' '));
}

TEST(StringConversion, SplitAndTrimTrimFront)
{
	// Multi entry with spaces (trim only front spaces)
	EXPECT_EQ((std::vector<std::string>{"A", "B"}), SplitAndTrim<TrimFront>("A, B", ','));
	EXPECT_EQ((std::vector<std::string>{"A ", "B"}), SplitAndTrim<TrimFront>("A ,B", ','));
	EXPECT_EQ((std::vector<std::string>{"A ", "B"}), SplitAndTrim<TrimFront>("A , B", ','));
	EXPECT_EQ((std::vector<std::string>{"A", "B", "C"}), SplitAndTrim<TrimFront>("A, B, C", ','));
	EXPECT_EQ((std::vector<std::string>{"A ", "B ", "C"}), SplitAndTrim<TrimFront>("A ,B ,C", ','));
	EXPECT_EQ((std::vector<std::string>{"A ", "B ", "C"}), SplitAndTrim<TrimFront>("A , B , C", ','));
}

TEST(StringConversion, ParseCsv)
{
	// Parsing empty (or whitespace only) string gives no data
	EXPECT_EQ((std::vector<std::string>{}), ParseCsv(""));
	EXPECT_EQ((std::vector<std::string>{}), ParseCsv(" "));
	EXPECT_EQ((std::vector<std::string>{}), ParseCsv("\t"));
	EXPECT_EQ((std::vector<std::string>{}), ParseCsv(" \t"));

	// Parsing returns placeholders for implied data
	// EXPECT_EQ((std::vector<std::string>{"", ""}), ParseCsv(","));
	// EXPECT_EQ((std::vector<std::string>{"A", ""}), ParseCsv("A, "));
	EXPECT_EQ((std::vector<std::string>{"", "B"}), ParseCsv(", B"));

	// Regular data parsing
	EXPECT_EQ((std::vector<std::string>{"A"}), ParseCsv("A"));
	EXPECT_EQ((std::vector<std::string>{"A", "B"}), ParseCsv("A, B"));
	EXPECT_EQ((std::vector<std::string>{"A", "B", "C"}), ParseCsv("A, B, C"));
}


TEST(StringConversion, AddrToHexString)
{
	// Correctly pads with 0
	EXPECT_EQ("00000000", AddrToHexString(0));
	EXPECT_EQ("00000000", AddrToHexString(00000000));
	// Note casing of hex values
	EXPECT_EQ("deadbeef", AddrToHexString(0xDEADBEEF));
}

TEST(StringConversion, GetDateTime)
{
	auto dateTime = GetDateTime();
	EXPECT_THAT(dateTime, ::testing::HasSubstr("UTC")); // UTC time zone marker
	EXPECT_THAT(dateTime, ::testing::ContainsRegex("\\d\\d\\d\\d-\\d\\d-\\d\\d")); // ISO 8601 date format
	EXPECT_THAT(dateTime, ::testing::ContainsRegex("\\d\\d:\\d\\d:\\d\\d")); // ISO 8601 time format
}
