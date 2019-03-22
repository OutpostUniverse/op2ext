#include <gtest/gtest.h>
#include <array>


size_t CopyStdStringIntoCharBuffer(const std::string& stringToCopy, char* buffer, size_t bufferSize);

using bufferType = std::array<char, 8>;


TEST(CopyStdStringIntoCharBuffer, NullDestination) {
	EXPECT_EQ(5, CopyStdStringIntoCharBuffer(std::string("Test"), nullptr, 0));
}

TEST(CopyStdStringIntoCharBuffer, EmptyDestination) {
	bufferType buffer{};
	EXPECT_EQ(5, CopyStdStringIntoCharBuffer(std::string("Test"), buffer.data(), 0));
	EXPECT_EQ(bufferType{}, buffer);
}

TEST(CopyStdStringIntoCharBuffer, TooShortSizeDestination) {
	bufferType buffer{};
	// Copy with no room for full string + null terminator
	// Should copy most of the string, and null terminate
	EXPECT_EQ(5, CopyStdStringIntoCharBuffer(std::string("Test"), buffer.data(), 4));
	EXPECT_EQ("Tes", std::string(buffer.data()));
}

TEST(CopyStdStringIntoCharBuffer, MinimumSizeDestination) {
	bufferType buffer{};
	EXPECT_EQ(0, CopyStdStringIntoCharBuffer(std::string("Test"), buffer.data(), 5));
	EXPECT_EQ("Test", std::string(buffer.data()));
}

TEST(CopyStdStringIntoCharBuffer, NormalDestination) {
	bufferType buffer{};
	EXPECT_EQ(0, CopyStdStringIntoCharBuffer(std::string("Test"), buffer.data(), sizeof(buffer)));
	EXPECT_EQ("Test", std::string(buffer.data()));
}
