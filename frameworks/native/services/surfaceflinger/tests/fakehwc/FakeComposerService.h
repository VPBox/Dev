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

#pragma once

#include <composer-hal/2.1/ComposerClient.h>

using namespace android::hardware::graphics::composer::V2_1;
using namespace android::hardware::graphics::composer::V2_1::hal;
using android::hardware::Return;

namespace sftest {

class FakeComposerService : public IComposer {
public:
    explicit FakeComposerService(android::sp<ComposerClient>& client);
    virtual ~FakeComposerService();

    Return<void> getCapabilities(getCapabilities_cb hidl_cb) override;
    Return<void> dumpDebugInfo(dumpDebugInfo_cb hidl_cb) override;
    Return<void> createClient(createClient_cb hidl_cb) override;

private:
    android::sp<ComposerClient> mClient;
};

} // namespace sftest
