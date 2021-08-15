/*
 * Copyright (C) 2016 The Android Open Source Project
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
#define LOG_TAG "libhidlmemory"

#include <map>
#include <mutex>
#include <string>

#include <hidlmemory/mapping.h>

#include <android-base/logging.h>
#include <hidl/HidlSupport.h>
#include "HidlMemoryCache.h"

using android::hardware::HidlMemoryCache;
using android::hidl::memory::block::V1_0::MemoryBlock;
using android::hidl::memory::token::V1_0::IMemoryToken;
using android::hidl::memory::V1_0::IMemory;

namespace android {
namespace hardware {

sp<IMemory> mapMemory(const ::android::hidl::memory::block::V1_0::MemoryBlock& block) {
    sp<HidlMemoryCache> c = HidlMemoryCache::getInstance();
    return c->map(block);
}

sp<RefBase> lockMemoryCache(const sp<::android::hidl::memory::token::V1_0::IMemoryToken> key) {
    sp<HidlMemoryCache> c = HidlMemoryCache::getInstance();
    return c->lockGuard(key);
}

}  // namespace hardware
}  // namespace android
