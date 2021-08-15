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
#ifndef ANDROID_HARDWARE_CACHE_MAPPING_H
#define ANDROID_HARDWARE_CACHE_MAPPING_H

#include <android/hidl/memory/1.0/IMemory.h>
#include <android/hidl/memory/block/1.0/types.h>
#include <android/hidl/memory/token/1.0/IMemoryToken.h>

namespace android {
namespace hardware {

/**
 * Returns the IMemory instance corresponding to a MemoryBlock. The heap that
 * a MemoryBlock belongs to is stored in an internal cache to reduce the number
 * of invocations to the mapMemory(hidl_memory)
 *
 * Note, a cache entry is maintained by reference count and may be flushed when
 * the count decrease to zero. Performance critical part that does not want its
 * caches to be flushed can use HidlMemoryCacheLock.
 */
sp<::android::hidl::memory::V1_0::IMemory> mapMemory(
    const ::android::hidl::memory::block::V1_0::MemoryBlock& block);

/**
 * Internally, there's a cache pool to keep IMemory instances for heap regions
 * that are referred by the MemoryBlock. During development, this
 * lockMemoryCache(...) method helps to diagnosis whether the cache is effective
 * for a specific key. It returns a RAII object used to lock an IMemory instance
 * referred by the key and keep it alive even if the instance is not referred by
 * any MemoryBlock. If the cache in interest is already effective. It won't differ
 * much in performance w/ wo/ the lockMemoryCache()
 *
 * @note An IMemory instance that is returned from the mapMemory() is
 *       initialized in an unlocked state.
 */
sp<RefBase> lockMemoryCache(const sp<::android::hidl::memory::token::V1_0::IMemoryToken> key);

}  // namespace hardware
}  // namespace android
#endif
