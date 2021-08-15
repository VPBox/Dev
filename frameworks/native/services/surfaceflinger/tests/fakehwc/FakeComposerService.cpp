/*
 * Copyright (C) 2017 The Android Open Source Project
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

#define LOG_NDEBUG 0
#undef LOG_TAG
#define LOG_TAG "FakeHwcService"
#include <log/log.h>

#include "FakeComposerService.h"

using namespace android::hardware;

namespace sftest {

FakeComposerService::FakeComposerService(android::sp<ComposerClient>& client) : mClient(client) {}

FakeComposerService::~FakeComposerService() {
    ALOGI("Maybe killing client %p", mClient.get());
    // Rely on sp to kill the client.
}

Return<void> FakeComposerService::getCapabilities(getCapabilities_cb hidl_cb) {
    ALOGI("FakeComposerService::getCapabilities");
    hidl_cb(hidl_vec<Capability>());
    return Void();
}

Return<void> FakeComposerService::dumpDebugInfo(dumpDebugInfo_cb hidl_cb) {
    ALOGI("FakeComposerService::dumpDebugInfo");
    hidl_cb(hidl_string());
    return Void();
}

Return<void> FakeComposerService::createClient(createClient_cb hidl_cb) {
    ALOGI("FakeComposerService::createClient %p", mClient.get());
    if (!mClient->init()) {
        LOG_ALWAYS_FATAL("failed to initialize ComposerClient");
    }
    hidl_cb(Error::NONE, mClient);
    return Void();
}

} // namespace sftest
