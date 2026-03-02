#include "vmtimetadata.h"
#include <gtest/gtest.h>
#include <cstdint>
#include <string>

/**
 * @brief Helper function to get the byte at a specific index in a string.
 * @param s The string to access.
 * @param i The index of the byte.
 * @return The byte at the specified index.
 */
static uint8_t byteAt(const std::string& s, std::size_t i) {
    return static_cast<uint8_t>(s.at(i));
}


/**
 * @brief Tests that the VMTIBuilder correctly copies data to the VMTIMetadata object.
 */
TEST(VMTIBuilderTest, CopiesData) {
    VMTITarget target{0x01, 5, 6, 7, 8, 9, 10};

    VMTIMetadata metadata = VMTIBuilder()
        .setFrameWidth(1920)
        .setFrameHeight(1080)
        .addTarget(target)
        .build();

    ASSERT_EQ(metadata.frameWidth, 1920);
    ASSERT_EQ(metadata.frameHeight, 1080);
    ASSERT_EQ(metadata.targets.size(), 1);
    ASSERT_EQ(metadata.targets[0].targetID, 0x01);
}