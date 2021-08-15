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
#ifndef ANDROID_HARDWARD_HIDLMEMORY_CACHE_H
#define ANDROID_HARDWARD_HIDLMEMORY_CACHE_H

#include <android/hidl/memory/block/1.0/types.h>
#include <android/hidl/memory/token/1.0/IMemoryToken.h>
#include <hidl/HidlBinderSupport.h>
#include <hwbinder/IBinder.h>
#include <utils/RefBase.h>
#include "HidlCache.h"

namespace android {
namespace hardware {

struct IMemoryTokenCompare {
    using IMemoryToken = ::android::hidl::memory::token::V1_0::IMemoryToken;
    bool operator()(const sp<IMemoryToken>& lhs, const sp<IMemoryToken>& rhs) const {
        sp<IBinder> lb = toBinder<IMemoryToken>(lhs);
        sp<IBinder> rb = toBinder<IMemoryToken>(rhs);
        return lb < rb;
    }
};

// The HidlMemoryCache is a singleton class to provides cache for
// IMemoryToken => ::android::hidl::memory::V1_0::IMemory
// It's an abstraction layer on top of the IMapper and supports, but is
// not limited to, the Ashmem type HidlMemory.
class HidlMemoryCache
    : public virtual HidlCache<sp<::android::hidl::memory::token::V1_0::IMemoryToken>,
                               ::android::hidl::memory::V1_0::IMemory, IMemoryTokenCompare> {
    using IMemoryToken = ::android::hidl::memory::token::V1_0::IMemoryToken;
    using IMemory = ::android::hidl::memory::V1_0::IMemory;
    using MemoryBlock = ::android::hidl::memory::block::V1_0::MemoryBlock;

   public:
    virtual sp<IMemory> map(const MemoryBlock& block);
    // get the singleton
    static sp<HidlMemoryCache> getInstance();

   protected:
    HidlMemoryCache() {}
    virtual sp<IMemory> fillLocked(const sp<IMemoryToken>& key) override;
};

}  // namespace hardware
}  // namespace android

#endif
