#include "vmtibuilder.h"

VMTIBuilder::VMTIBuilder() : _metadata(std::make_unique<VMTIMetadata>()) {}

VMTIBuilder::~VMTIBuilder() = default;

VMTIBuilder& VMTIBuilder::setVersionNumber(uint8_t version) {
    _metadata->versionNumber = version;
    return *this;
}

VMTIBuilder& VMTIBuilder::setFrameWidth(uint32_t width) {
    _metadata->frameWidth = width;
    return *this;
}

VMTIBuilder& VMTIBuilder::setFrameHeight(uint32_t height) {
    _metadata->frameHeight = height;
    return *this;
}

VMTIBuilder& VMTIBuilder::addTarget(const VMTITarget& target) {
    _metadata->targets.push_back(target);
    return *this;
}

VMTIMetadata VMTIBuilder::build() {
    VMTIMetadata result = *_metadata;
    return result;
}
