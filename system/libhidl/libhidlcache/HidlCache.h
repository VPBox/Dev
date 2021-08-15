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
#ifndef ANDROID_HARDWARE_HIDL_CACHE_H
#define ANDROID_HARDWARE_HIDL_CACHE_H

#include <utils/Log.h>

namespace android {
namespace hardware {

// A generic cache to map Key to sp<Value>. The cache records are kept with
// wp<Value>, so that it does not block the Value to be garbage collected
// when there's no other sp<> externally.
template <class Key, class Value, class Compare = std::less<Key>>
class HidlCache : public virtual RefBase {
    using Mutex = std::mutex;
    using Lock = std::lock_guard<Mutex>;

   public:
    //  A RAII class to manage lock/unlock HidlCache.
    class HidlCacheLock : public virtual RefBase {
       public:
        HidlCacheLock(sp<HidlCache> cache, const Key& key) : mCache(cache), mKey(key) {
            mCache->lock(mKey);
        }
        ~HidlCacheLock() { mCache->unlock(mKey); }

       private:
        sp<HidlCache> mCache;
        const Key mKey;
    };
    // lock the IMemory refered by key and keep it alive even if there's no
    // other memory block refers to.
    virtual bool lock(const Key& key);
    virtual sp<Value> unlock(const Key& key);
    virtual bool flush(const Key& key);
    // fetch the sp<Value> with key from cache,
    // make a new instance with fill() if it does not present currently.
    virtual sp<Value> fetch(const Key& key);
    virtual sp<HidlCacheLock> lockGuard(const Key& key) { return new HidlCacheLock(this, key); }

    virtual ~HidlCache() {}

   protected:
    friend void HidlCacheWhiteBoxTest();
    // This method shall be called with a lock held
    virtual sp<Value> fillLocked(const Key& key) = 0;

    // @return nullptr if it does not present currently.
    // @note This method shall be called with a lock held
    virtual sp<Value> getCachedLocked(const Key& key);
    bool cached(Key key) const { return mCached.count(key) > 0; }
    bool locked(Key key) const { return mLocked.count(key) > 0; }
    Mutex mMutex;

    std::map<Key, wp<Value>, Compare> mCached;
    std::map<Key, sp<Value>, Compare> mLocked;
};

template <class Key, class Value, class Compare>
bool HidlCache<Key, Value, Compare>::lock(const Key& key) {
    {
        Lock lock(mMutex);
        if (cached(key)) {
            sp<Value> im = mCached[key].promote();
            if (im != nullptr) {
                mLocked[key] = im;
                return true;
            } else {
                mCached.erase(key);
            }
        }
    }
    sp<Value> value = fetch(key);
    if (value == nullptr) {
        return false;
    } else {
        Lock lock(mMutex);
        mLocked[key] = value;
        return true;
    }
}

template <class Key, class Value, class Compare>
sp<Value> HidlCache<Key, Value, Compare>::unlock(const Key& key) {
    Lock lock(mMutex);
    if (locked(key)) {
        sp<Value> v = mLocked[key];
        mLocked.erase(key);
        return v;
    }
    return nullptr;
}

template <class Key, class Value, class Compare>
bool HidlCache<Key, Value, Compare>::flush(const Key& key) {
    Lock lock(mMutex);
    bool contain = cached(key);
    mCached.erase(key);
    return contain;
}

template <class Key, class Value, class Compare>
sp<Value> HidlCache<Key, Value, Compare>::getCachedLocked(const Key& key) {
    if (cached(key)) {
        wp<Value> cache = mCached[key];
        sp<Value> mem = cache.promote();
        if (mem != nullptr) {
            return mem;
        } else {
            mCached.erase(key);
        }
    }
    return nullptr;
}

template <class Key, class Value, class Compare>
sp<Value> HidlCache<Key, Value, Compare>::fetch(const Key& key) {
    Lock lock(mMutex);
    sp<Value> value = getCachedLocked(key);

    if (value == nullptr) {
        value = fillLocked(key);
    }
    return value;
}

}  // namespace hardware
}  // namespace android
#endif
