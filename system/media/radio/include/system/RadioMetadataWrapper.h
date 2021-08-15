#ifndef ANDROID_RADIO_METADATA_WRAPPER_H
#define ANDROID_RADIO_METADATA_WRAPPER_H

#include <system/radio_metadata.h>

namespace android {

class ANDROID_API RadioMetadataWrapper
{
public:
    explicit RadioMetadataWrapper(radio_metadata_t **metadata);

    virtual ~RadioMetadataWrapper();

private:
    radio_metadata_t **mMetadata;
};

}  // namespace android

#endif // ANDROID_RADIO_METADATA_WRAPPER_H
