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

#ifndef ANDROID_AUDIO_FIFO_INDEX_H
#define ANDROID_AUDIO_FIFO_INDEX_H

#include <atomic>
#include <stdint.h>
#include <time.h>

/**
 * An index that may optionally be placed in shared memory.
 * Must be Plain Old Data (POD), so no virtual methods are allowed.
 * If in shared memory, exactly one process must explicitly call the constructor via placement new.
 * \see #audio_utils_fifo_sync
 */
class audio_utils_fifo_index {

public:
    audio_utils_fifo_index() : mIndex(0) { }
    ~audio_utils_fifo_index() { }

    /**
     * Load value of index now with memory order 'acquire'.
     *
     * \return Index value
     */
    uint32_t loadAcquire();

    /**
     * Store new value into index now with memory order 'release'.
     *
     * \param value New value to store into index
     */
    void storeRelease(uint32_t value);

    // TODO op should be set in the constructor.
    /**
     * Wait for value of index to change from the specified expected value.
     *
     * \param op      Either FUTEX_WAIT or FUTEX_WAIT_PRIVATE.
     * \param expected Current/expected value of index.
     * \param timeout Indicates the maximum time to block while waiting for value to change.
     *                NULL means to block forever.
     *                Time is expressed as relative CLOCK_MONOTONIC.
     *                We use the Linux kernel representation of time for the very lowest levels,
     *                and save other representations for the APIs that are built on top of this.
     *                This permits APIs to choose the int64_t representation if desired, or the
     *                Linux representation without a double conversion.
     *
     * \return Zero for success, or a negative error code as specified at "man 2 futex".
     */
    int wait(int op, uint32_t expected, const struct timespec *timeout);

    // TODO op should be set in the constructor.
    /**
     * Wake up any threads waiting for the value of index to change.
     *
     * \param op      Either FUTEX_WAIT or FUTEX_WAIT_PRIVATE.
     * \param waiters Maximum number of waiting threads to wake up.
     *
     * \return Actual number of threads woken up.
     */
    int wake(int op, int waiters = 1);

    // specialized use only, prefer loadAcquire in most cases
    uint32_t loadConsume();

private:
    // Linux futex is 32 bits regardless of platform.
    // It would make more sense to declare this as atomic_uint32_t, but there is no such type name.
    // TODO Support 64-bit index with 32-bit futex in low-order bits.
    std::atomic_uint_least32_t  mIndex; // accessed by both sides using atomic operations
    static_assert(sizeof(mIndex) == sizeof(uint32_t), "mIndex must be 32 bits");
};

static_assert(sizeof(audio_utils_fifo_index) == sizeof(uint32_t),
        "audio_utils_fifo_index must be 32 bits");

// TODO
// From a design POV, these next two classes should be related.
// Extract a base class (that shares their property of being a reference to a fifo index)
// This should be good in the case if there is a need for generic manipulations on these references.
// Or can we perhaps define a template and make two specializations of it.
// This is in the case when compile-time polymorphism is sufficient.

/**
 * A reference to an audio_utils_fifo_index with deferred store-release and deferred wake.
 *
 * TODO Currently the index and futex share the same 32-bit cell.
 * In the future, the index may optionally be increased to 64-bits,
 * and the futex will either be the low-order 32 bits of the index,
 * or an unrelated 32-bit cell used as a set of event flags.
 * Both of these will require a change to the API.
 */
class RefIndexDeferredStoreReleaseDeferredWake
{
public:
    RefIndexDeferredStoreReleaseDeferredWake(audio_utils_fifo_index& index);
    ~RefIndexDeferredStoreReleaseDeferredWake();

    // Place 'value' into the cache, but do not store it to memory yet.
    void set(uint32_t value);

    // If there is a new value in the cache, store it now with memory order 'release'.
    void writeback();

    // Place 'value' into the cache and then store it with memory order 'release'.
    void writethrough(uint32_t value);

    // op is FUTEX_WAKE or FUTEX_WAKE_PRIVATE
    // TODO op should be set in the constructor, and should be abstracted.
    // waiters is number of waiting threads to wake up
    void wakeDeferred(int op, int waiters = 1);
    void wakeNowIfNeeded();
    // TODO op should be set in the constructor.
    void wakeNow(int op, int waiters = 1);

private:
    audio_utils_fifo_index& mIndex;     // reference to associated index
    uint32_t                mValue;     // cached value to be stored
    bool                    mWriteback; // whether the cached value needs to be stored
    int                     mWaiters;   // number of waiters to wake
    int                     mWakeOp;    // which kind of wake operation to use
};

/**
 * A reference to an audio_utils_fifo_index with cached load-acquire, and deferred wait.
 *
 * TODO Same as RefIndexDeferredStoreReleaseDeferredWake.
 */
class RefIndexCachedLoadAcquireDeferredWait
{
public:
    RefIndexCachedLoadAcquireDeferredWait(audio_utils_fifo_index& index);
    ~RefIndexCachedLoadAcquireDeferredWait();

    // If value is already cached, return the cached value.
    // Otherwise load now with memory order 'acquire', cache for later use, and return the value.
    uint32_t    get();

    // If value is already cached, this is a no-op.
    // Otherwise load now with memory order 'acquire' and cache the value for later use.
    void        prefetch();

    // Discard any value in the cache.
    void        invalidate();

#if 0
    /**
     * Load a fresh value for index, ignoring any previously cached information.
     */
    uint32_t    readthrough();
#endif

    // TODO This is an immediate wait, but we needed deferred wait
    /**
     * Wait for value of index to change from when it was most recently read with get().
     * To avoid a race condition, the caller must have already read the index with get(),
     * and then made the decision to call wait() based on that value.
     *
     * \param op      Either FUTEX_WAIT or FUTEX_WAIT_PRIVATE.
     * \param timeout Indicates the maximum time to block while waiting for value to change.
     *                NULL means to block forever.
     *                Time is expressed as relative CLOCK_MONOTONIC.
     *                See above for explanation of why representation is struct timespec.
     *
     * \return Zero for success, or a negative error code as specified at "man 2 futex".
     * \retval -EINVAL caller did not call get() prior to wait()
     */
    int         wait(int op, const struct timespec *timeout);

private:
    audio_utils_fifo_index& mIndex;     // reference to associated index
    uint32_t                mValue;     // most recently cached value
    bool                    mLoaded;    // whether mValue is valid
};

#endif  // !ANDROID_AUDIO_FIFO_INDEX_H
