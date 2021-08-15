/*
 * Copyright (C) 2015 The Android Open Source Project
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

#ifndef ANDROID_AUDIO_FIFO_H
#define ANDROID_AUDIO_FIFO_H

#include <atomic>
#include <stdlib.h>
#include <audio_utils/fifo_index.h>

#ifndef __cplusplus
#error C API is no longer supported
#endif

/** Indicates whether an index is also used for synchronization. */
enum audio_utils_fifo_sync {
    /** Index is not also used for synchronization; timeouts are done via clock_nanosleep(). */
    AUDIO_UTILS_FIFO_SYNC_SLEEP,
    /** Index is also used for synchronization as futex, and is mapped by one process. */
    AUDIO_UTILS_FIFO_SYNC_PRIVATE,
    /** Index is also used for synchronization as futex, and is mapped by one or more processes. */
    AUDIO_UTILS_FIFO_SYNC_SHARED,
};

/**
 * Base class for single-writer, single-reader or multi-reader, optionally blocking FIFO.
 * The base class manipulates frame indices only, and has no knowledge of frame sizes or the buffer.
 * At most one reader, called the "throttling reader", can block the writer.
 * The "fill level", or unread frame count, is defined with respect to the throttling reader.
 */
class audio_utils_fifo_base {

public:

    /**
     * Return the capacity, or statically configured maximum frame count.
     *
     * \return The capacity in frames.
     */
    uint32_t capacity() const
            { return mFrameCount; }

protected:

    /**
     * Construct FIFO base class
     *
     *  \param frameCount    Maximum usable frames to be stored in the FIFO > 0 && <= INT32_MAX,
     *                       aka "capacity".
     *                       If release()s always use the same count, and the count is a divisor of
     *                       (effective) \p frameCount, then the obtain()s won't ever be fragmented.
     *  \param writerRear    Writer's rear index.  Passed by reference because it must be non-NULL.
     *  \param throttleFront Pointer to the front index of at most one reader that throttles the
     *                       writer, or NULL for no throttling.
     */
    audio_utils_fifo_base(uint32_t frameCount, audio_utils_fifo_index& writerRear,
            audio_utils_fifo_index *throttleFront = NULL);
    /*virtual*/ ~audio_utils_fifo_base();

    /** Return a new index as the sum of a validated index and a specified increment.
     *
     * \param index     Caller should supply a validated mFront or mRear.
     * \param increment Value to be added to the index <= mFrameCount.
     *
     * \return The sum of index plus increment.
     */
    uint32_t sum(uint32_t index, uint32_t increment) const;

    /** Return the difference between two indices: rear - front.
     *
     * \param rear  Caller should supply an unvalidated mRear.
     * \param front Caller should supply an unvalidated mFront.
     * \param lost  If non-NULL, set to the approximate number of frames lost before
     *              re-synchronization when -EOVERFLOW occurs, or set to zero when no frames lost.
     * \param flush Whether to flush the entire buffer on -EOVERFLOW.
     *
     * \return The zero or positive difference <= mFrameCount, or a negative error code.
     * \retval -EIO        corrupted indices, no recovery is possible
     * \retval -EOVERFLOW  reader doesn't throttle writer, and frames were lost because reader
     *                     isn't keeping up with writer; see \p lost
     */
    int32_t diff(uint32_t rear, uint32_t front, size_t *lost = NULL, bool flush = false) const;

    /**
     * Mark the FIFO as shutdown (permanently unusable), usually due to an -EIO status from an API.
     * Thereafter, all APIs that return a status will return -EIO, and other APIs will be no-ops.
     */
    void shutdown() const;

    // These fields are const after initialization

    /** Maximum usable frames to be stored in the FIFO > 0 && <= INT32_MAX, aka "capacity". */
    const uint32_t mFrameCount;
    /** Equal to roundup(mFrameCount). */
    const uint32_t mFrameCountP2;

    /**
     * Equal to mFrameCountP2 - mFrameCount, the number of "wasted" frames after the end of mBuffer.
     * Only the indices are wasted, not any memory.
     */
    const uint32_t mFudgeFactor;

    /** Reference to writer's rear index. */
    audio_utils_fifo_index&         mWriterRear;
    /** Indicates how synchronization is done for mWriterRear. */
    const audio_utils_fifo_sync     mWriterRearSync;

    /**
     * Pointer to the front index of at most one reader that throttles the writer,
     * or NULL for no throttling.
     */
    audio_utils_fifo_index* const   mThrottleFront;
    /** Indicates how synchronization is done for mThrottleFront. */
    const audio_utils_fifo_sync     mThrottleFrontSync;

    /** Whether FIFO is marked as shutdown due to detection of an "impossible" error condition. */
    mutable bool                    mIsShutdown;
};

////////////////////////////////////////////////////////////////////////////////

/**
 * Same as audio_utils_fifo_base, but understands frame sizes and knows about the buffer but does
 * not own it.
 */
class audio_utils_fifo : public audio_utils_fifo_base {

    friend class audio_utils_fifo_reader;
    friend class audio_utils_fifo_writer;
    friend class audio_utils_fifo_writer32;

public:

    /**
     * Construct a FIFO object: multi-process.
     *
     *  \param frameCount  Maximum usable frames to be stored in the FIFO > 0 && <= INT32_MAX,
     *                     aka "capacity".
     *                     If writes and reads always use the same count, and the count is a divisor
     *                     of \p frameCount, then the writes and reads won't do a partial transfer.
     *  \param frameSize   Size of each frame in bytes > 0,
     *                     \p frameSize * \p frameCount <= INT32_MAX.
     *  \param buffer      Pointer to a non-NULL caller-allocated buffer of \p frameCount frames.
     *  \param writerRear  Writer's rear index.  Passed by reference because it must be non-NULL.
     *  \param throttleFront Pointer to the front index of at most one reader that throttles the
     *                       writer, or NULL for no throttling.
     */
    audio_utils_fifo(uint32_t frameCount, uint32_t frameSize, void *buffer,
            audio_utils_fifo_index& writerRear, audio_utils_fifo_index *throttleFront = NULL);

    /**
     * Construct a FIFO object: single-process.
     *  \param frameCount  Maximum usable frames to be stored in the FIFO > 0 && <= INT32_MAX,
     *                     aka "capacity".
     *                     If writes and reads always use the same count, and the count is a divisor
     *                     of \p frameCount, then the writes and reads won't do a partial transfer.
     *  \param frameSize   Size of each frame in bytes > 0,
     *                     \p frameSize * \p frameCount <= INT32_MAX.
     *  \param buffer      Pointer to a non-NULL caller-allocated buffer of \p frameCount frames.
     *  \param throttlesWriter Whether there is one reader that throttles the writer.
     */
    audio_utils_fifo(uint32_t frameCount, uint32_t frameSize, void *buffer,
            bool throttlesWriter = true);

    /*virtual*/ ~audio_utils_fifo();

    /**
     * Return the frame size in bytes.
     *
     * \return frame size in bytes, always > 0.
     */
    uint32_t frameSize() const
            { return mFrameSize; }

    /**
     * Return a pointer to the caller-allocated buffer.
     *
     * \return non-NULL pointer to buffer.
     */
    void *buffer() const
            { return mBuffer; }

private:
    // These fields are const after initialization
    const uint32_t mFrameSize;  // size of each frame in bytes
    void * const   mBuffer;     // non-NULL pointer to caller-allocated buffer
                                // of size mFrameCount frames

    // only used for single-process constructor
    audio_utils_fifo_index      mSingleProcessSharedRear;

    // only used for single-process constructor when throttlesWriter == true
    audio_utils_fifo_index      mSingleProcessSharedFront;
};

/**
 * Describes one virtually contiguous fragment of a logically contiguous slice.
 * Compare to struct iovec for readv(2) and writev(2).
 */
struct audio_utils_iovec {
    /** Offset of fragment in frames, relative to mBuffer, undefined if mLength == 0 */
    uint32_t    mOffset;
    /** Length of fragment in frames, 0 means fragment is empty */
    uint32_t    mLength;
};

////////////////////////////////////////////////////////////////////////////////

/**
 * Based on frameworks/av/include/media/AudioBufferProvider.h
 */
class audio_utils_fifo_provider {
public:
    audio_utils_fifo_provider(audio_utils_fifo& fifo);
    virtual ~audio_utils_fifo_provider();

    /**
     * Obtain access to a logically contiguous slice of a stream, represented by \p iovec.
     * For the reader(s), the slice is initialized and has read-only access.
     * For the writer, the slice is uninitialized and has read/write access.
     * It is permitted to call obtain() multiple times without an intervening release().
     * Each call resets the notion of most recently obtained slice.
     *
     * \param iovec Non-NULL pointer to a pair of fragment descriptors.
     *              On entry, the descriptors may be uninitialized.
     *              On exit, the descriptors are initialized and refer to each of the two fragments.
     *              iovec[0] describes the initial fragment of the slice, and
     *              iovec[1] describes the remaining non-virtually-contiguous fragment.
     *              Empty iovec[0] implies that iovec[1] is also empty.
     *              iovec[0].mOffset and iovec[1].mOffset are always < capacity.
     *              Typically iovec[1].mOffset is zero, but don't assume that.
     * \param count The maximum number of frames to obtain.
     *              See setHysteresis() for something which is close to, but not the same as,
     *              a minimum.
     * \param timeout Indicates the maximum time to block for at least one frame.
     *                NULL and {0, 0} both mean non-blocking.
     *                Time is expressed as relative CLOCK_MONOTONIC.
     *                As an optimization, if \p timeout->tv_sec is the maximum positive value for
     *                time_t (LONG_MAX), then the implementation treats it as infinite timeout.
     *                See fifo_index.h for explanation of why representation is struct timespec.
     *
     * \return Actual number of frames available, if greater than or equal to zero.
     *         Guaranteed to be <= \p count and == iovec[0].mLength + iovec[1].mLength.
     *         For a reader this is also guaranteed to be <= capacity.
     *         For a writer this is also guaranteed to be <= effective buffer size,
     *         even if there is no reader that throttles writer.
     *
     *  \retval -EIO        corrupted indices, no recovery is possible
     *  \retval -EOVERFLOW  reader doesn't throttle writer, and frames were lost because reader
     *                      isn't keeping up with writer; see \p lost
     *  \retval -ETIMEDOUT  count is greater than zero, timeout is non-NULL and not {0, 0},
     *                      timeout expired, and no frames were available after the timeout.
     *  \retval -EINTR      count is greater than zero, timeout is non-NULL and not {0, 0}, timeout
     *                      was interrupted by a signal, and no frames were available after signal.
     *  \retval -EWOULDBLOCK count is greater than zero, timeout is non-NULL and not {0, 0},
     *                      futex wait failed due to benign race, and unable to converge after
     *                      retrying.  Should usually handle like -EINTR.
     *
     * Applications should treat all of these as equivalent to zero available frames,
     * except they convey extra information as to the cause.
     * After any error, both iovec[0] and iovec[1] will be empty.
     */
    virtual ssize_t obtain(audio_utils_iovec iovec[2], size_t count = SIZE_MAX,
            const struct timespec *timeout = NULL) = 0;

    /**
     * Release access to a portion of the most recently obtained slice.
     * It is permitted to call release() multiple times without an intervening obtain().
     *
     * \param count Number of frames to release.  The cumulative number of frames released must not
     *              exceed the number of frames most recently obtained.
     *              If it ever happens, then the FIFO will be marked unusable with shutdown().
     */
    virtual void release(size_t count) = 0;

    /**
     * Determine the number of frames that could be obtained or read/written without blocking.
     * There's an inherent race condition: the value may soon be obsolete so shouldn't be trusted.
     * available() may be called after obtain(), but doesn't affect the number of releasable frames.
     * The implementation unfortunately prevents the method from being marked 'const'.
     *
     * \return Number of available frames, if greater than or equal to zero.
     *  \retval -EIO        corrupted indices, no recovery is possible
     *  \retval -EOVERFLOW  reader doesn't throttle writer, and frames were lost because reader
     *                      isn't keeping up with writer
     */
    virtual ssize_t available() = 0;

    /**
     * Return the capacity, or statically configured maximum frame count.
     *
     * \return The capacity in frames.
     */
    uint32_t capacity() const
            { return mFifo.capacity(); }

    /**
     * Return the total number of frames released since construction.
     * For a reader, this includes lost and flushed frames.
     *
     * \return Total frames released.
     */
    uint64_t totalReleased() const
            { return mTotalReleased; }

    /** Return a reference to the associated FIFO. */
    audio_utils_fifo& fifo()    { return mFifo; }

protected:
    audio_utils_fifo&   mFifo;

    /** Number of frames obtained at most recent obtain(), less total number of frames released. */
    uint32_t    mObtained;

    /** Number of times to retry a futex wait that fails with EWOULDBLOCK. */
    static const int kRetries = 2;

    /**
     * Total number of frames released since construction.
     * For a reader, this includes lost and flushed frames.
     */
    uint64_t    mTotalReleased;
};

////////////////////////////////////////////////////////////////////////////////

/**
 * Used to write to a FIFO.  There should be exactly one writer per FIFO.
 * The writer is multi-thread safe with respect to reader(s),
 * but not with respect to multiple threads calling the writer API.
 */
class audio_utils_fifo_writer : public audio_utils_fifo_provider {

public:
    /**
     * Single-process and multi-process use same constructor here,
     * but different FIFO constructors.
     *
     * \param fifo Associated FIFO.  Passed by reference because it must be non-NULL.
     */
    explicit audio_utils_fifo_writer(audio_utils_fifo& fifo);
    virtual ~audio_utils_fifo_writer();

    /**
     * Write to FIFO.  Resets the number of releasable frames to zero.
     *
     * \param buffer  Pointer to source buffer containing \p count frames of data.
     *                Pointer must be non-NULL if \p count is greater than zero.
     * \param count   Desired number of frames to write.
     * \param timeout Indicates the maximum time to block for at least one frame.
     *                NULL and {0, 0} both mean non-blocking.
     *                Time is expressed as relative CLOCK_MONOTONIC.
     *                As an optimization, if \p timeout->tv_sec is the maximum positive value for
     *                time_t (LONG_MAX), then the implementation treats it as infinite timeout.
     *                See fifo_index.h for explanation of why representation is struct timespec.
     *
     * \return Actual number of frames written, if greater than or equal to zero.
     *         Guaranteed to be <= \p count.
     *         Also guaranteed to be <= effective buffer size,
     *         even if there is no reader that throttles writer.
     *         The actual transfer count may be zero if the FIFO is full,
     *         or partial if the FIFO was almost full.
     *  \retval -EIO       corrupted indices, no recovery is possible
     *  \retval -ETIMEDOUT count is greater than zero, timeout is non-NULL and not {0, 0},
     *                     timeout expired, and no frames were available after the timeout.
     *  \retval -EINTR     count is greater than zero, timeout is non-NULL and not {0, 0}, timeout
     *                     was interrupted by a signal, and no frames were available after signal.
     *  \retval -EWOULDBLOCK count is greater than zero, timeout is non-NULL and not {0, 0},
     *                      futex wait failed due to benign race, and unable to converge after
     *                      retrying.  Should usually handle like -EINTR.
     */
    ssize_t write(const void *buffer, size_t count, const struct timespec *timeout = NULL);

    // Implement audio_utils_fifo_provider
    virtual ssize_t obtain(audio_utils_iovec iovec[2], size_t count = SIZE_MAX,
            const struct timespec *timeout = NULL);
    virtual void release(size_t count);
    virtual ssize_t available();

    /**
     * Set the current effective buffer size.
     * Any filled frames already written or released to the buffer are unaltered, and pending
     * releasable frames from obtain() may be release()ed.  However subsequent write() and obtain()
     * will be limited such that the total filled frame count is <= the effective buffer size.
     * The default effective buffer size is mFifo.mFrameCount.
     * Reducing the effective buffer size may update the hysteresis levels; see getHysteresis().
     *
     * \param frameCount    effective buffer size in frames. Capped to range [0, mFifo.mFrameCount].
     */
    void resize(uint32_t frameCount);

    /**
     * Get the current effective buffer size.
     * This value is not exposed to reader(s), and so must be conveyed via an out-of-band channel.
     *
     * \return effective buffer size in frames
     */
    uint32_t size() const;

    /**
     * Set the hysteresis levels for the writer to wake blocked readers.
     * Hysteresis can decrease the number of context switches between writer and a blocking reader.
     * A non-empty write() or release() will wake readers
     * only if the fill level was < \p armLevel before the write() or release(),
     * and then the fill level became > \p triggerLevel afterwards.
     * The default value for \p armLevel is mFifo.mFrameCount, which means always armed.
     * The default value for \p triggerLevel is zero,
     * which means every write() or release() will wake the readers.
     * For hysteresis, \p armLevel must be <= \p triggerLevel + 1.
     * Increasing \p armLevel will arm for wakeup, regardless of the current fill level.
     *
     * \param armLevel      Arm for wakeup when fill level < this value.
     *                      Capped to range [0, effective buffer size].
     * \param triggerLevel  Trigger wakeup when armed and fill level > this value.
     *                      Capped to range [0, effective buffer size].
     */
    void setHysteresis(uint32_t armLevel, uint32_t triggerLevel);

    /**
     * Get the hysteresis levels for waking readers.
     *
     * \param armLevel      Set to the current arm level in frames.
     * \param triggerLevel  Set to the current trigger level in frames.
     */
    void getHysteresis(uint32_t *armLevel, uint32_t *triggerLevel) const;

private:
    // Accessed by writer only using ordinary operations
    uint32_t    mLocalRear; // frame index of next frame slot available to write, or write index

    // TODO make a separate class and associate with the synchronization object
    uint32_t    mArmLevel;          // arm if filled < arm level before release()
    uint32_t    mTriggerLevel;      // trigger if armed and filled > trigger level after release()
    bool        mIsArmed;           // whether currently armed

    uint32_t    mEffectiveFrames;   // current effective buffer size, <= mFifo.mFrameCount
};

////////////////////////////////////////////////////////////////////////////////

/**
 * Used to read from a FIFO.  There can be one or more readers per FIFO,
 * and at most one of those readers can throttle the writer.
 * All other readers must keep up with the writer or they will lose frames.
 * Each reader is multi-thread safe with respect to the writer and any other readers,
 * but not with respect to multiple threads calling the reader API.
 */
class audio_utils_fifo_reader : public audio_utils_fifo_provider {

public:
    /**
     * Single-process and multi-process use same constructor here,
     * but different FIFO constructors.
     *
     * \param fifo            Associated FIFO.  Passed by reference because it must be non-NULL.
     * \param throttlesWriter Whether this reader throttles the writer.
     *                        At most one reader can specify throttlesWriter == true.
     *                        A non-throttling reader does not see any data written
     *                        prior to construction of the reader.
     * \param flush           Whether to flush (discard) the entire buffer on -EOVERFLOW.
     *                        The advantage of flushing is that it increases the chance that next
     *                        read will be successful.  The disadvantage is that it loses more data.
     */
    explicit audio_utils_fifo_reader(audio_utils_fifo& fifo, bool throttlesWriter = true,
                                     bool flush = false);
    virtual ~audio_utils_fifo_reader();

    /**
     * Read from FIFO.  Resets the number of releasable frames to zero.
     *
     * \param buffer  Pointer to destination buffer to be filled with up to \p count frames of data.
     *                Pointer must be non-NULL if \p count is greater than zero.
     * \param count   Desired number of frames to read.
     * \param timeout Indicates the maximum time to block for at least one frame.
     *                NULL and {0, 0} both mean non-blocking.
     *                Time is expressed as relative CLOCK_MONOTONIC.
     *                As an optimization, if \p timeout->tv_sec is the maximum positive value for
     *                time_t (LONG_MAX), then the implementation treats it as infinite timeout.
     *                See fifo_index.h for explanation of why representation is struct timespec.
     * \param lost    If non-NULL, set to the approximate number of frames lost before
     *                re-synchronization when -EOVERFLOW occurs, or set to zero when no frames lost.
     *
     * \return Actual number of frames read, if greater than or equal to zero.
     *         Guaranteed to be <= \p count.
     *         Also guaranteed to be <= capacity.
     *         The actual transfer count may be zero if the FIFO is empty,
     *         or partial if the FIFO was almost empty.
     *  \retval -EIO        corrupted indices, no recovery is possible
     *  \retval -EOVERFLOW  reader doesn't throttle writer, and frames were lost because reader
     *                      isn't keeping up with writer; see \p lost
     *  \retval -ETIMEDOUT  count is greater than zero, timeout is non-NULL and not {0, 0},
     *                      timeout expired, and no frames were available after the timeout.
     *  \retval -EINTR      count is greater than zero, timeout is non-NULL and not {0, 0}, timeout
     *                      was interrupted by a signal, and no frames were available after signal.
     *  \retval -EWOULDBLOCK count is greater than zero, timeout is non-NULL and not {0, 0},
     *                      futex wait failed due to benign race, and unable to converge after
     *                      retrying.  Should usually handle like -EINTR.
     */
    ssize_t read(void *buffer, size_t count, const struct timespec *timeout = NULL,
            size_t *lost = NULL);

    // Implement audio_utils_fifo_provider
    virtual ssize_t obtain(audio_utils_iovec iovec[2], size_t count = SIZE_MAX,
            const struct timespec *timeout = NULL);
    virtual void release(size_t count);
    virtual ssize_t available();

    /**
     * Same as audio_utils_fifo_provider::obtain, except has an additional parameter \p lost.
     *
     * \param iovec   See audio_utils_fifo_provider::obtain.
     * \param count   See audio_utils_fifo_provider::obtain.
     * \param timeout See audio_utils_fifo_provider::obtain.
     * \param lost    If non-NULL, set to the approximate number of frames lost before
     *                re-synchronization when -EOVERFLOW occurs, or set to zero when no frames lost.
     * \return See audio_utils_fifo_provider::obtain for 'Returns' and 'Return values'.
     */
    ssize_t obtain(audio_utils_iovec iovec[2], size_t count, const struct timespec *timeout,
            size_t *lost);

    /**
     * Determine the number of frames that could be obtained or read without blocking.
     * There's an inherent race condition: the value may soon be obsolete so shouldn't be trusted.
     * available() may be called after obtain(), but doesn't affect the number of releasable frames.
     * The implementation unfortunately prevents the method from being marked 'const'.
     *
     * \param lost    If non-NULL, set to the approximate number of frames lost before
     *                re-synchronization when -EOVERFLOW occurs, or set to zero when no frames lost.
     *
     * \return Number of available frames, if greater than or equal to zero.
     *  \retval -EIO        corrupted indices, no recovery is possible
     *  \retval -EOVERFLOW  reader doesn't throttle writer, and frames were lost because reader
     *                      isn't keeping up with writer; see \p lost
     */
    ssize_t available(size_t *lost);

    /**
     * Flush (discard) all frames that could be obtained or read without blocking.
     * Note that flush is a method on a reader, so if the writer wants to flush
     * then it must communicate the request to the reader(s) via an out-of-band channel.
     *
     * \param lost    If non-NULL, set to the approximate number of frames lost before
     *                re-synchronization when -EOVERFLOW occurs, or set to zero when no frames lost.
     *
     * \return Number of flushed frames, if greater than or equal to zero.
     *         This number does not include any lost frames.
     *  \retval -EIO        corrupted indices, no recovery is possible
     *  \retval -EOVERFLOW  reader doesn't throttle writer, and frames were lost because reader
     *                      isn't keeping up with writer; see \p lost
     */
    ssize_t flush(size_t *lost = NULL);

    /**
     * Set the hysteresis levels for a throttling reader to wake a blocked writer.
     * Hysteresis can decrease the number of context switches between reader and a blocking writer.
     * A non-empty read() or release() by a throttling reader will wake the writer
     * only if the fill level was > \p armLevel before the read() or release(),
     * and then the fill level became < \p triggerLevel afterwards.
     * The default value for \p armLevel is -1, which means always armed.
     * The default value for \p triggerLevel is mFifo.mFrameCount,
     * which means every read() or release() will wake the writer.
     * For hysteresis, \p armLevel must be >= \p triggerLevel - 1.
     * Decreasing \p armLevel will arm for wakeup, regardless of the current fill level.
     * Note that the throttling reader is not directly aware of the writer's effective buffer size,
     * so any change in effective buffer size must be communicated indirectly.
     *
     * \param armLevel      Arm for wakeup when fill level > this value.
     *                      Capped to range [-1, mFifo.mFrameCount].
     * \param triggerLevel  Trigger wakeup when armed and fill level < this value.
     *                      Capped to range [0, mFifo.mFrameCount].
     */
    void setHysteresis(int32_t armLevel, uint32_t triggerLevel);

    /**
     * Get the hysteresis levels for waking readers.
     *
     * \param armLevel      Set to the current arm level in frames.
     * \param triggerLevel  Set to the current trigger level in frames.
     */
    void getHysteresis(int32_t *armLevel, uint32_t *triggerLevel) const;

    /**
     * Return the total number of lost frames since construction, due to reader not keeping up with
     * writer.  Does not include flushed frames.
     * It is necessary to call read(), obtain(), or flush() prior to calling this method,
     * in order to observe an increase in the total,
     * but it is not necessary for the 'lost' parameter of those prior calls to be non-NULL.
     *
     * \return Total lost frames.
     */
    uint64_t totalLost() const
            { return mTotalLost; }

    /**
     * Return the total number of flushed frames since construction.
     * Does not include lost frames.
     *
     * \return Total flushed frames.
     */
    uint64_t totalFlushed() const
            { return mTotalFlushed; }

private:
    // Accessed by reader only using ordinary operations
    uint32_t     mLocalFront;   // frame index of first frame slot available to read, or read index

    // Points to shared front index if this reader throttles writer, or NULL if we don't throttle
    // FIXME consider making it a boolean
    audio_utils_fifo_index*     mThrottleFront;

    bool        mFlush;             // whether to flush the entire buffer on -EOVERFLOW

    int32_t     mArmLevel;          // arm if filled > arm level before release()
    uint32_t    mTriggerLevel;      // trigger if armed and filled < trigger level after release()
    bool        mIsArmed;           // whether currently armed

    uint64_t    mTotalLost;         // total lost frames, does not include flushed frames
    uint64_t    mTotalFlushed;      // total flushed frames, does not include lost frames
};

#endif  // !ANDROID_AUDIO_FIFO_H
