#include "vmtibuilder.h"

#include <cassert>
#include <cstdint>
#include <string>

static uint8_t byteAt(const std::string& s, std::size_t i) {
    return static_cast<uint8_t>(s.at(i));
}

static void testBuilderCopiesData() {
    VMTITarget target{0x01020304, 5, 6, 7, 8, 9, 10};

    VMTIMetadata metadata = VMTIBuilder()
        .setVersionNumber(2)
        .setFrameWidth(1920)
        .setFrameHeight(1080)
        .addTarget(target)
        .build();

    assert(metadata.versionNumber == 2);
    assert(metadata.frameWidth == 1920);
    assert(metadata.frameHeight == 1080);
    assert(metadata.targets.size() == 1);
    assert(metadata.targets[0].targetID == 0x01020304);
}

static void testMetadataEncodesVariableLengthBigEndianPixels() {
    VMTIMetadata metadata;
    metadata.versionNumber = 1;
    metadata.frameWidth = 300;
    metadata.frameHeight = 200;

    VMTITarget target{};
    target.targetID = 0x11223344;
    target.bboxTopLeftX = 0;
    target.bboxTopLeftY = 1;    // pixel = 300 (0x012C) -> 2 bytes
    target.bboxBottomRightX = 4;
    target.bboxBottomRightY = 4; // pixel = 1204 (0x04B4) -> 2 bytes
    target.targetPriority = 0xAA;
    target.targetConfidence = 0x55;
    metadata.targets.push_back(target);

    const std::string encoded = metadata.toString();

    // Find first target (0x0A), then validate payload structure.
    std::size_t i = 0;
    while (i < encoded.size() && byteAt(encoded, i) != 0x0A) {
        ++i;
    }
    assert(i + 1 < encoded.size());

    const uint8_t targetLen = byteAt(encoded, i + 1);
    assert(targetLen == 18);

    std::size_t p = i + 2;
    assert(byteAt(encoded, p++) == 0x11);
    assert(byteAt(encoded, p++) == 0x22);
    assert(byteAt(encoded, p++) == 0x33);
    assert(byteAt(encoded, p++) == 0x44);

    assert(byteAt(encoded, p++) == 0x02);
    assert(byteAt(encoded, p++) == 0x02);
    assert(byteAt(encoded, p++) == 0x01);
    assert(byteAt(encoded, p++) == 0x2C);

    assert(byteAt(encoded, p++) == 0x03);
    assert(byteAt(encoded, p++) == 0x02);
    assert(byteAt(encoded, p++) == 0x04);
    assert(byteAt(encoded, p++) == 0xB4);

    assert(byteAt(encoded, p++) == 0x04);
    assert(byteAt(encoded, p++) == 0x01);
    assert(byteAt(encoded, p++) == 0xAA);

    assert(byteAt(encoded, p++) == 0x05);
    assert(byteAt(encoded, p++) == 0x01);
    assert(byteAt(encoded, p++) == 0x55);
}

int main() {
    testBuilderCopiesData();
    testMetadataEncodesVariableLengthBigEndianPixels();
    return 0;
}
