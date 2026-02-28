#ifndef VMTI_BUILDER_H
#define VMTI_BUILDER_H

#include "vmtimetadata.h"
#include <memory>

class VMTIBuilder {
public:
    VMTIBuilder();
    ~VMTIBuilder();

    VMTIBuilder& setVersionNumber(uint8_t version);
    VMTIBuilder& setNumTargetsSeen(uint32_t numTargets);
    VMTIBuilder& setNumTargetsReported(uint32_t numTargets);
    VMTIBuilder& setFrameWidth(uint32_t width);
    VMTIBuilder& setFrameHeight(uint32_t height);
    VMTIBuilder& addTarget(const VMTITarget& target);

    VMTIMetadata build();

private:
    std::unique_ptr<VMTIMetadata> _metadata;
};

#endif // VMTI_BUILDER_H
