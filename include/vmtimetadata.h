/**
 * @file vmtimetadata.h
 * @brief Defines the VMTI (Video Moving Target Indicator) metadata structures and builder.
 *
 * This file contains the definitions for VMTIMetadata, VMTITarget, and VMTIBuilder,
 * which are used to construct and represent VMTI metadata packets.
 */
#ifndef VMTI_METADATA_H
#define VMTI_METADATA_H

#include <cstdint>
#include <iomanip>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

/**
 * @struct VMTITarget
 * @brief Represents a single target in the VMTI metadata.
 */
struct VMTITarget {
    uint8_t targetID;          ///< Unique identifier for the target.
    uint32_t bboxTopLeftX;      ///< Bounding box top-left X coordinate.
    uint32_t bboxTopLeftY;      ///< Bounding box top-left Y coordinate.
    uint32_t bboxBottomRightX;  ///< Bounding box bottom-right X coordinate.
    uint32_t bboxBottomRightY;  ///< Bounding box bottom-right Y coordinate.
    uint8_t targetPriority;     ///< Priority of the target.
    uint8_t targetConfidence;   ///< Confidence level of the target detection.

    /**
     * @brief Calculates the pixel offset for the top-left corner of the bounding box.
     * @param width The width of the frame.
     * @param height The height of the frame (unused).
     * @return The pixel offset from the top-left of the frame.
     */
    uint32_t bboxTopLeftPixel(uint32_t width, uint32_t height) const {
        (void)height;
        return bboxTopLeftY * width + bboxTopLeftX;
    }

    /**
     * @brief Calculates the pixel offset for the bottom-right corner of the bounding box.
     * @param width The width of the frame.
     * @param height The height of the frame (unused).
     * @return The pixel offset from the top-left of the frame.
     */
    uint32_t bboxBottomRightPixel(uint32_t width, uint32_t height) const {
        (void)height;
        return bboxBottomRightY * width + bboxBottomRightX;
    }
};

/**
 * @class VMTIMetadata
 * @brief Represents the VMTI metadata packet.
 */
class VMTIMetadata {
public:
    uint8_t versionNumber;              ///< VMTI format version number.
    uint32_t frameWidth;                ///< Width of the video frame.
    uint32_t frameHeight;               ///< Height of the video frame.
    std::vector<VMTITarget> targets;    ///< List of targets detected in the frame.

    /**
     * @brief Default constructor for VMTIMetadata.
     */
    VMTIMetadata()
        : versionNumber(4), frameWidth(0), frameHeight(0) {}

    /**
     * @brief Serializes the metadata to a byte string.
     * @return A string representing the serialized metadata.
     */
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

            targetsStr.push_back(targetStr.size());
            targetsStr += targetStr;
        }

        result.push_back(0x65);
        result.push_back(static_cast<char>(targetsStr.size()));
        result += targetsStr;

        return result;
    }

    /**
     * @brief Converts the serialized metadata to a hex string for display.
     * @return A formatted hex string.
     */
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

/**
 * @class VMTIBuilder
 * @brief A builder class for creating VMTIMetadata objects.
 */
class VMTIBuilder {
public:
    /**
     * @brief Default constructor for VMTIBuilder.
     */
    VMTIBuilder() : _metadata(std::make_unique<VMTIMetadata>()) {}
    /**
     * @brief Default destructor.
     */
    ~VMTIBuilder() = default;

    /**
     * @brief Sets the frame width for the VMTI metadata.
     * @param width The frame width.
     * @return A reference to the builder.
     */
    VMTIBuilder& setFrameWidth(uint32_t width) {
        _metadata->frameWidth = width;
        return *this;
    }

    /**
     * @brief Sets the frame height for the VMTI metadata.
     * @param height The frame height.
     * @return A reference to the builder.
     */
    VMTIBuilder& setFrameHeight(uint32_t height) {
        _metadata->frameHeight = height;
        return *this;
    }

    /**
     * @brief Adds a target to the VMTI metadata.
     * @param target The target to add.
     * @return A reference to the builder.
     */
    VMTIBuilder& addTarget(const VMTITarget& target) {
        _metadata->targets.push_back(target);
        return *this;
    }

    /**
     * @brief Builds the VMTIMetadata object.
     * @return The constructed VMTIMetadata object.
     */
    VMTIMetadata build() {
        VMTIMetadata result = *_metadata;
        return result;
    }

private:
    std::unique_ptr<VMTIMetadata> _metadata; ///< The metadata object being built.
};

#endif // VMTI_METADATA_H
