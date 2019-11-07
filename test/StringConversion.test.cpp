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


TEST(StringConversion, CopyStringViewIntoCharBufferSafeNoOutputConditions)
{
	constexpr char nonce = 'Z';
	char buffer[8];

	// The nonce should never be overwritten
	buffer[0] = nonce;

	// Null buffer of size zero
	EXPECT_EQ(1u, CopyStringViewIntoCharBuffer("", nullptr, 0));
	EXPECT_EQ(nonce, buffer[0]);
	EXPECT_EQ(2u, CopyStringViewIntoCharBuffer("A", nullptr, 0));
	EXPECT_EQ(nonce, buffer[0]);

	// Null buffer with non-zero size
	EXPECT_EQ(1u, CopyStringViewIntoCharBuffer("", nullptr, sizeof(buffer)));
	EXPECT_EQ(nonce, buffer[0]);
	EXPECT_EQ(2u, CopyStringViewIntoCharBuffer("A", nullptr, sizeof(buffer)));
	EXPECT_EQ(nonce, buffer[0]);

	// Valid buffer but with size zero
	EXPECT_EQ(1u, CopyStringViewIntoCharBuffer("", buffer, 0));
	EXPECT_EQ(nonce, buffer[0]);
	EXPECT_EQ(2u, CopyStringViewIntoCharBuffer("A", buffer, 0));
	EXPECT_EQ(nonce, buffer[0]);
}

TEST(StringConversion, CopyStringViewIntoCharBuffer)
{
	constexpr char nonce = 'Z';
	constexpr char null = '\0';
	char buffer[8];

	// Empty string (must still null terminate)
	buffer[0] = nonce;
	EXPECT_EQ(0u, CopyStringViewIntoCharBuffer("", buffer, sizeof(buffer)));
	EXPECT_EQ(null, buffer[0]);
	// Short string (extra buffer space to spare)
	buffer[1] = nonce;
	EXPECT_EQ(0u, CopyStringViewIntoCharBuffer("A", buffer, sizeof(buffer)));
	EXPECT_EQ(null, buffer[1]);
	// Precise sized string (just enough room for null terminator)
	buffer[7] = nonce;
	EXPECT_EQ(0u, CopyStringViewIntoCharBuffer("1234567", buffer, sizeof(buffer)));
	EXPECT_EQ(null, buffer[7]);
	// Overflow sized string (just barely no room for null terminator)
	buffer[7] = nonce;
	EXPECT_EQ(9u, CopyStringViewIntoCharBuffer("12345678", buffer, sizeof(buffer)));
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


TEST(StringConversion, AddrToHexString)
{
	// Correctly pads with 0
	EXPECT_EQ("00000000", AddrToHexString(0u));
	EXPECT_EQ("00000000", AddrToHexString(00000000u));
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
