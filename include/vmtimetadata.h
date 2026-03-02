#ifndef VMTI_METADATA_H
#define VMTI_METADATA_H

#include <cstdint>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

struct VMTITarget {
    uint32_t targetID;
    uint32_t bboxTopLeftX;
    uint32_t bboxTopLeftY;
    uint32_t bboxBottomRightX;
    uint32_t bboxBottomRightY;
    uint8_t targetPriority;
    uint8_t targetConfidence;

    uint32_t bboxTopLeftPixel(uint32_t width, uint32_t height) const {
        (void)height;
        return bboxTopLeftY * width + bboxTopLeftX;
    }

    uint32_t bboxBottomRightPixel(uint32_t width, uint32_t height) const {
        (void)height;
        return bboxBottomRightY * width + bboxBottomRightX;
    }
};

class VMTIMetadata {
public:
    uint8_t versionNumber;
    uint32_t frameWidth;
    uint32_t frameHeight;
    std::vector<VMTITarget> targets;

    VMTIMetadata()
        : versionNumber(0), frameWidth(0), frameHeight(0) {}

    std::string toString() const {
        std::string result;
        std::string targetsStr;

        const uint8_t targetCount = static_cast<uint8_t>(targets.size() & 0xFF);

        result.push_back(0x04);
        result.push_back(0x01);
        result.push_back(static_cast<char>(versionNumber));

        result.push_back(0x05);
        result.push_back(0x01);
        result.push_back(static_cast<char>(targetCount));

        result.push_back(0x06);
        result.push_back(0x01);
        result.push_back(static_cast<char>(targetCount));

        result.push_back(0x08);
        result.push_back(0x02);
        result.push_back(static_cast<char>((frameWidth >> 8) & 0xFF));
        result.push_back(static_cast<char>(frameWidth & 0xFF));

        result.push_back(0x09);
        result.push_back(0x02);
        result.push_back(static_cast<char>((frameHeight >> 8) & 0xFF));
        result.push_back(static_cast<char>(frameHeight & 0xFF));

        auto bytesNeeded = [](uint32_t value) -> uint8_t {
            if (value > 0xFFFFFF) return 4;
            if (value > 0xFFFF) return 3;
            if (value > 0xFF) return 2;
            return 1;
        };

        auto appendBigEndian = [](std::string& out, uint32_t value, uint8_t bytes) {
            for (int shift = static_cast<int>((bytes - 1) * 8); shift >= 0; shift -= 8) {
                out.push_back(static_cast<char>((value >> shift) & 0xFF));
            }
        };

        for (const auto& target : targets) {
            std::string targetStr;

            targetStr.push_back(static_cast<char>((target.targetID >> 24) & 0xFF));
            targetStr.push_back(static_cast<char>((target.targetID >> 16) & 0xFF));
            targetStr.push_back(static_cast<char>((target.targetID >> 8) & 0xFF));
            targetStr.push_back(static_cast<char>(target.targetID & 0xFF));

            const uint32_t topLeftPixel = target.bboxTopLeftPixel(frameWidth, frameHeight);
            const uint8_t topLeftPixelBytes = bytesNeeded(topLeftPixel);
            targetStr.push_back(0x02);
            targetStr.push_back(static_cast<char>(topLeftPixelBytes));
            appendBigEndian(targetStr, topLeftPixel, topLeftPixelBytes);

            const uint32_t bottomRightPixel = target.bboxBottomRightPixel(frameWidth, frameHeight);
            const uint8_t bottomRightPixelBytes = bytesNeeded(bottomRightPixel);
            targetStr.push_back(0x03);
            targetStr.push_back(static_cast<char>(bottomRightPixelBytes));
            appendBigEndian(targetStr, bottomRightPixel, bottomRightPixelBytes);

            targetStr.push_back(0x04);
            targetStr.push_back(0x01);
            targetStr.push_back(static_cast<char>(target.targetPriority));

            targetStr.push_back(0x05);
            targetStr.push_back(0x01);
            targetStr.push_back(static_cast<char>(target.targetConfidence));

            targetsStr.push_back(0x0A);
            targetsStr.push_back(static_cast<char>(targetStr.size()));
            targetsStr += targetStr;
        }

        result.push_back(0x0B);
        result.push_back(static_cast<char>(targetsStr.size()));
        result += targetsStr;

        return result;
    }

    std::string toHexString() const {
        const std::string encoded = toString();
        std::ostringstream hex;

        hex << std::hex << std::setfill('0');
        for (std::size_t i = 0; i < encoded.size(); ++i) {
            if (i > 0) {
                if (i % 16 == 0) {
                    hex << '\n';
                } else {
                    hex << ' ';
                }
            }

            const unsigned char byte = static_cast<unsigned char>(encoded[i]);
            hex << std::setw(2) << static_cast<unsigned int>(byte);
        }

        hex << '\n';

        return hex.str();
    }
};

class VMTIBuilder {
public:
    VMTIBuilder() : _metadata(std::make_unique<VMTIMetadata>()) {}
    ~VMTIBuilder() = default;

    VMTIBuilder& setVersionNumber(uint8_t version) {
        _metadata->versionNumber = version;
        return *this;
    }

    VMTIBuilder& setFrameWidth(uint32_t width) {
        _metadata->frameWidth = width;
        return *this;
    }

    VMTIBuilder& setFrameHeight(uint32_t height) {
        _metadata->frameHeight = height;
        return *this;
    }

    VMTIBuilder& addTarget(const VMTITarget& target) {
        _metadata->targets.push_back(target);
        return *this;
    }

    VMTIMetadata build() {
        VMTIMetadata result = *_metadata;
        return result;
    }

private:
    std::unique_ptr<VMTIMetadata> _metadata;
};

#endif // VMTI_METADATA_H
