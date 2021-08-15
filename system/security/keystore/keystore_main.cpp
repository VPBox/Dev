/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#define LOG_TAG "keystore"

#include <android-base/logging.h>
#include <android/hidl/manager/1.1/IServiceManager.h>
#include <android/security/keystore/IKeystoreService.h>
#include <android/system/wifi/keystore/1.0/IKeystore.h>
#include <binder/IPCThreadState.h>
#include <binder/IServiceManager.h>
#include <hidl/HidlTransportSupport.h>
#include <keymasterV4_0/Keymaster3.h>
#include <keymasterV4_0/Keymaster4.h>
#include <utils/StrongPointer.h>
#include <wifikeystorehal/keystore.h>

#include <keystore/keystore_hidl_support.h>
#include <keystore/keystore_return_types.h>

#include "KeyStore.h"
#include "key_store_service.h"
#include "legacy_keymaster_device_wrapper.h"
#include "permissions.h"

/* KeyStore is a secured storage for key-value pairs. In this implementation,
 * each file stores one key-value pair. Keys are encoded in file names, and
 * values are encrypted with checksums. The encryption key is protected by a
 * user-defined password. To keep things simple, buffers are always larger than
 * the maximum space we needed, so boundary checks on buffers are omitted. */

using ::android::sp;
using ::android::hardware::configureRpcThreadpool;
using ::android::system::wifi::keystore::V1_0::IKeystore;
using ::android::system::wifi::keystore::V1_0::implementation::Keystore;
using ::android::hidl::manager::V1_1::IServiceManager;
using ::android::hardware::hidl_string;
using ::android::hardware::hidl_vec;
using ::android::hardware::keymaster::V4_0::SecurityLevel;
using ::android::hardware::keymaster::V4_0::HmacSharingParameters;
using ::android::hardware::keymaster::V4_0::ErrorCode;

using ::keystore::keymaster::support::Keymaster;
using ::keystore::keymaster::support::Keymaster3;
using ::keystore::keymaster::support::Keymaster4;

using keystore::KeymasterDevices;

template <typename Wrapper>
KeymasterDevices enumerateKeymasterDevices(IServiceManager* serviceManager) {
    KeymasterDevices result;
    serviceManager->listByInterface(
        Wrapper::WrappedIKeymasterDevice::descriptor, [&](const hidl_vec<hidl_string>& names) {
            auto try_get_device = [&](const auto& name, bool fail_silent) {
                auto device = Wrapper::WrappedIKeymasterDevice::getService(name);
                if (fail_silent && !device) return;
                CHECK(device) << "Failed to get service for \""
                              << Wrapper::WrappedIKeymasterDevice::descriptor
                              << "\" with interface name \"" << name << "\"";

                sp<Keymaster> kmDevice(new Wrapper(device, name));
                auto halVersion = kmDevice->halVersion();
                SecurityLevel securityLevel = halVersion.securityLevel;
                LOG(INFO) << "found " << Wrapper::WrappedIKeymasterDevice::descriptor
                          << " with interface name " << name << " and seclevel "
                          << toString(securityLevel);
                CHECK(static_cast<uint32_t>(securityLevel) < result.size())
                    << "Security level of \"" << Wrapper::WrappedIKeymasterDevice::descriptor
                    << "\" with interface name \"" << name << "\" out of range";
                auto& deviceSlot = result[securityLevel];
                if (deviceSlot) {
                    if (!fail_silent) {
                        LOG(WARNING) << "Implementation of \""
                                     << Wrapper::WrappedIKeymasterDevice::descriptor
                                     << "\" with interface name \"" << name
                                     << "\" and security level: " << toString(securityLevel)
                                     << " Masked by other implementation of Keymaster";
                    }
                } else {
                    deviceSlot = kmDevice;
                }
            };
            bool has_default = false;
            for (auto& n : names) {
                try_get_device(n, false);
                if (n == "default") has_default = true;
            }
            // Make sure that we always check the default device. If we enumerate only what is
            // known to hwservicemanager, we miss a possible passthrough HAL.
            if (!has_default) {
                try_get_device("default", true /* fail_silent */);
            }
        });
    return result;
}

KeymasterDevices initializeKeymasters() {
    auto serviceManager = android::hidl::manager::V1_1::IServiceManager::getService();
    CHECK(serviceManager.get()) << "Failed to get ServiceManager";
    auto result = enumerateKeymasterDevices<Keymaster4>(serviceManager.get());
    auto softKeymaster = result[SecurityLevel::SOFTWARE];
    if (!result[SecurityLevel::TRUSTED_ENVIRONMENT]) {
        result = enumerateKeymasterDevices<Keymaster3>(serviceManager.get());
    }
    if (softKeymaster) result[SecurityLevel::SOFTWARE] = softKeymaster;
    if (result[SecurityLevel::SOFTWARE] && !result[SecurityLevel::TRUSTED_ENVIRONMENT]) {
        LOG(WARNING) << "No secure Keymaster implementation found, but device offers insecure"
                        " Keymaster HAL. Using as default.";
        result[SecurityLevel::TRUSTED_ENVIRONMENT] = result[SecurityLevel::SOFTWARE];
        result[SecurityLevel::SOFTWARE] = nullptr;
    }
    if (!result[SecurityLevel::SOFTWARE]) {
        auto fbdev = android::keystore::makeSoftwareKeymasterDevice();
        CHECK(fbdev.get()) << "Unable to create Software Keymaster Device";
        result[SecurityLevel::SOFTWARE] = new Keymaster3(fbdev, "Software");
    }
    return result;
}

int main(int argc, char* argv[]) {

    android::OtherSystemServiceLoopRun();

    using android::hardware::hidl_string;
    CHECK(argc >= 2) << "A directory must be specified!";
    CHECK(chdir(argv[1]) != -1) << "chdir: " << argv[1] << ": " << strerror(errno);

    auto kmDevices = initializeKeymasters();

    CHECK(kmDevices[SecurityLevel::SOFTWARE]) << "Missing software Keymaster device";
    CHECK(kmDevices[SecurityLevel::TRUSTED_ENVIRONMENT])
        << "Error no viable keymaster device found";

    CHECK(configure_selinux() != -1) << "Failed to configure SELinux.";

    auto halVersion = kmDevices[SecurityLevel::TRUSTED_ENVIRONMENT]->halVersion();

    // If the hardware is keymaster 2.0 or higher we will not allow the fallback device for import
    // or generation of keys. The fallback device is only used for legacy keys present on the
    // device.
    SecurityLevel minimalAllowedSecurityLevelForNewKeys =
        halVersion.majorVersion >= 2 ? SecurityLevel::TRUSTED_ENVIRONMENT : SecurityLevel::SOFTWARE;

    android::sp<keystore::KeyStore> keyStore(
        new keystore::KeyStore(kmDevices, minimalAllowedSecurityLevelForNewKeys));
    keyStore->initialize();
    android::sp<android::IServiceManager> sm = android::defaultServiceManager();
    android::sp<keystore::KeyStoreService> service = new keystore::KeyStoreService(keyStore);
    service->setRequestingSid(true);
    android::status_t ret = sm->addService(android::String16("android.security.keystore"), service);
    CHECK(ret == android::OK) << "Couldn't register binder service!";

    /**
     * Register the wifi keystore HAL service to run in passthrough mode.
     * This will spawn off a new thread which will service the HIDL
     * transactions.
     */
    configureRpcThreadpool(1, false /* callerWillJoin */);
    android::sp<IKeystore> wifiKeystoreHalService = new Keystore();
    android::status_t err = wifiKeystoreHalService->registerAsService();
    CHECK(ret == android::OK) << "Cannot register wifi keystore HAL service: " << err;

    /*
     * This thread is just going to process Binder transactions.
     */
    android::IPCThreadState::self()->joinThreadPool();
    return 1;
}
