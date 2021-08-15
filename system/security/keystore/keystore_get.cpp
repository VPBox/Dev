/*
 * Copyright (C) 2012 The Android Open Source Project
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

#include <android/security/keystore/IKeystoreService.h>
#include <binder/IServiceManager.h>

#include <keystore/keystore_get.h>
#include <vector>

using namespace android;
using namespace keystore;

ssize_t keystore_get(const char* key, size_t keyLength, uint8_t** value) {
    sp<IServiceManager> sm = initdefaultServiceManager();
    sp<IBinder> binder = sm->getService(String16("android.security.keystore"));
    sp<android::security::keystore::IKeystoreService> service =
        interface_cast<android::security::keystore::IKeystoreService>(binder);

    if (service == nullptr) {
        return -1;
    }

    ::std::vector<uint8_t> result;
    auto ret = service->get(String16(key, keyLength), -1, &result);
    if (!ret.isOk()) return -1;

    if (value) {
        *value = reinterpret_cast<uint8_t*>(malloc(result.size()));
        if (!*value) return -1;
        memcpy(*value, &result[0], result.size());
    }
    return result.size();
}
