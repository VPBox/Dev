#ifndef ANDROID_SYSTEM_WIFI_KEYSTORE_V1_0_KEYSTORE_H
#define ANDROID_SYSTEM_WIFI_KEYSTORE_V1_0_KEYSTORE_H

#include <android/system/wifi/keystore/1.0/IKeystore.h>
#include <hidl/MQDescriptor.h>
#include <hidl/Status.h>

#include <android/security/keystore/IKeystoreService.h>
#include <binder/IServiceManager.h>

namespace android {
namespace system {
namespace wifi {
namespace keystore {
namespace V1_0 {
namespace implementation {

using ::android::hidl::base::V1_0::DebugInfo;
using ::android::hidl::base::V1_0::IBase;
using ::android::hardware::hidl_array;
using ::android::hardware::hidl_memory;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::sp;
using ::android::system::wifi::keystore::V1_0::IKeystore;

struct Keystore : public IKeystore {
  public:
    // Methods from ::android::hardware::wifi::keystore::V1_0::IKeystore follow.
    Return<void> getBlob(const hidl_string& key, getBlob_cb _hidl_cb) override;
    Return<void> getPublicKey(
            const hidl_string& keyId, getPublicKey_cb _hidl_cb) override;
    Return<void> sign(
            const hidl_string& keyId, const hidl_vec<uint8_t>& dataToSign,
            sign_cb _hidl_cb) override;
};

extern "C" IKeystore* HIDL_FETCH_IKeystore(const char* name);

}  // namespace implementation
}  // namespace V1_0
}  // namespace keystore
}  // namespace wifi
}  // namespace system
}  // namespace android

#endif  // ANDROID_SYSTEM_WIFI_KEYSTORE_V1_0_KEYSTORE_H
