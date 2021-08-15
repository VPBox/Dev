#define LOG_TAG "RadioMetadataWrapper"
//#define LOG_NDEBUG 0

#include <system/RadioMetadataWrapper.h>
#include <log/log.h>

namespace android {

RadioMetadataWrapper::RadioMetadataWrapper(radio_metadata_t **metadata)
    : mMetadata(metadata)
{
    *mMetadata = nullptr;

    auto status = radio_metadata_allocate(mMetadata, 0, 0);
    if (status != 0) {
        // *mMetadata remains NULL if allocation fails
        ALOGE("Failed to allocate metadata buffer");
    }
}

RadioMetadataWrapper::~RadioMetadataWrapper()
{
    radio_metadata_deallocate(*mMetadata);
}

}  // namespace android
