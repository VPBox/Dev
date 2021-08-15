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

#include <errno.h>
#include <limits.h>
#include <audio_utils/fifo_index.h>
#include <audio_utils/futex.h>

// These are not implemented within <audio_utils/fifo_index.h>
// so that we don't expose futex.

uint32_t audio_utils_fifo_index::loadAcquire()
{
    return atomic_load_explicit(&mIndex, std::memory_order_acquire);
}

// FIXME should inline this, so that writer32 can also inline it
void audio_utils_fifo_index::storeRelease(uint32_t value)
{
    atomic_store_explicit(&mIndex, value, std::memory_order_release);
}

int audio_utils_fifo_index::wait(int op, uint32_t expected, const struct timespec *timeout)
{
    return sys_futex(&mIndex, op, expected, timeout, NULL, 0);
}

int audio_utils_fifo_index::wake(int op, int waiters)
{
    return sys_futex(&mIndex, op, waiters, NULL, NULL, 0);
}

uint32_t audio_utils_fifo_index::loadConsume()
{
    return atomic_load_explicit(&mIndex, std::memory_order_consume);
}

////

RefIndexDeferredStoreReleaseDeferredWake::RefIndexDeferredStoreReleaseDeferredWake(
        audio_utils_fifo_index& index)
    : mIndex(index), mValue(0), mWriteback(false), mWaiters(0), mWakeOp(FUTEX_WAIT_PRIVATE)
{
}

RefIndexDeferredStoreReleaseDeferredWake::~RefIndexDeferredStoreReleaseDeferredWake()
{
    writeback();
    wakeNowIfNeeded();
}

void RefIndexDeferredStoreReleaseDeferredWake::set(uint32_t value) {
    mValue = value;
    mWriteback = true;
}

void RefIndexDeferredStoreReleaseDeferredWake::writeback()
{
    if (mWriteback) {
        // TODO When part of a collection, should use relaxed for all but the last writeback
        mIndex.storeRelease(mValue);
        mWriteback = false;
    }
}

void RefIndexDeferredStoreReleaseDeferredWake::writethrough(uint32_t value) {
    set(value);
    writeback();
}

void RefIndexDeferredStoreReleaseDeferredWake::wakeDeferred(int op, int waiters)
{
    if (waiters <= 0) {
        return;
    }
    // default is FUTEX_WAKE_PRIVATE
    if (op == FUTEX_WAKE) {
        mWakeOp = FUTEX_WAKE;
    }
    if (waiters < INT_MAX - mWaiters) {
        mWaiters += waiters;
    } else {
        mWaiters = INT_MAX;
    }
}

void RefIndexDeferredStoreReleaseDeferredWake::wakeNowIfNeeded()
{
    if (mWaiters > 0) {
        mIndex.wake(mWakeOp, mWaiters);
        mWaiters = 0;
        mWakeOp = FUTEX_WAKE_PRIVATE;
    }
}

void RefIndexDeferredStoreReleaseDeferredWake::wakeNow(int op, int waiters)
{
    wakeDeferred(op, waiters);
    wakeNowIfNeeded();
}

////

RefIndexCachedLoadAcquireDeferredWait::RefIndexCachedLoadAcquireDeferredWait(
        audio_utils_fifo_index& index)
    : mIndex(index), mValue(0), mLoaded(false)
{
}

RefIndexCachedLoadAcquireDeferredWait::~RefIndexCachedLoadAcquireDeferredWait()
{
}

uint32_t RefIndexCachedLoadAcquireDeferredWait::get()
{
    prefetch();
    return mValue;
}

void RefIndexCachedLoadAcquireDeferredWait::prefetch()
{
    if (!mLoaded) {
        // TODO When part of a collection, should use relaxed for all but the last load
        mValue = mIndex.loadAcquire();
        mLoaded = true;
    }
}

void RefIndexCachedLoadAcquireDeferredWait::invalidate()
{
    mLoaded = false;
}

#if 0
uint32_t RefIndexCachedLoadAcquireDeferredWait::readthrough()
{
    invalidate();
    return get();
}
#endif

int RefIndexCachedLoadAcquireDeferredWait::wait(int op, const struct timespec *timeout)
{
    if (!mLoaded) {
        return -EINVAL;
    }
    int err = mIndex.wait(op, mValue /*expected*/, timeout);
    invalidate();
    return err;
}
