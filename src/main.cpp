#include "vmtimetadata.h"

#include <array>
#include <iostream>

/**
 * @brief Main entry point for the VMTI example application.
 *
 * This program demonstrates how to use the VMTIBuilder to create VMTI metadata
 * with a predefined set of targets and then prints the resulting metadata
 * to the console in a hexadecimal format.
 *
 * @return 0 on successful execution.
 */
int main() {
    // Define a sample array of targets.
    std::array<VMTITarget, 2> targets{{
        {
            .targetID = 1,
            .bboxTopLeftX = 50,
            .bboxTopLeftY = 50,
            .bboxBottomRightX = 100,
            .bboxBottomRightY = 100,
            .targetPriority = 1,
            .targetConfidence = 10,
        },
        {
            .targetID = 2,
            .bboxTopLeftX = 120,
            .bboxTopLeftY = 80,
            .bboxBottomRightX = 180,
            .bboxBottomRightY = 150,
            .targetPriority = 2,
            .targetConfidence = 20,
        },
    }};

    // Use the builder to construct the metadata.
    VMTIBuilder builder;
    builder
        .setVersionNumber(2)
        .setFrameWidth(1920)
        .setFrameHeight(1080);

    for (const auto& target : targets) {
        builder.addTarget(target);
    }

    // Build the final metadata object.
    VMTIMetadata metadata = builder.build();

    // Output the metadata as a hex string.
    std::cout << metadata.toHexString();

    return 0;
}