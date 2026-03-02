#include "vmtimetadata.h"

#include <array>
#include <iostream>

int main() {
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
    VMTIBuilder builder;
    builder
        .setVersionNumber(2)
        .setFrameWidth(1920)
        .setFrameHeight(1080);

    for (const auto& target : targets) {
        builder.addTarget(target);
    }

    VMTIMetadata metadata = builder.build();

    std::cout << metadata.toHexString();

    return 0;
}