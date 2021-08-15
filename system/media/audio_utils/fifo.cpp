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

//#define LOG_NDEBUG 0
#define LOG_TAG "audio_utils_fifo"

#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

#include <audio_utils/clock_nanosleep.h>
#include <audio_utils/fifo.h>
#include <audio_utils/futex.h>
#include <audio_utils/roundup.h>
#include <log/log.h>
#include <system/audio.h> // FALLTHROUGH_INTENDED
#include <utils/Errors.h>

audio_utils_fifo_base::audio_utils_fifo_base(uint32_t frameCount,
        audio_utils_fifo_index& writerRear, audio_utils_fifo_index *throttleFront)
        __attribute__((no_sanitize("integer"))) :
    mFrameCount(frameCount), mFrameCountP2(roundup(frameCount)),
    mFudgeFactor(mFrameCountP2 - mFrameCount),
    // FIXME need an API to configure the sync types
    mWriterRear(writerRear), mWriterRearSync(AUDIO_UTILS_FIFO_SYNC_SHARED),
    mThrottleFront(throttleFront), mThrottleFrontSync(AUDIO_UTILS_FIFO_SYNC_SHARED),
    mIsShutdown(false)
{
    // actual upper bound on frameCount will depend on the frame size
    LOG_ALWAYS_FATAL_IF(frameCount == 0 || frameCount > ((uint32_t) INT32_MAX));
}

audio_utils_fifo_base::~audio_utils_fifo_base()
{
}

uint32_t audio_utils_fifo_base::sum(uint32_t index, uint32_t increment) const
        __attribute__((no_sanitize("integer")))
{
    if (mFudgeFactor > 0) {
        uint32_t mask = mFrameCountP2 - 1;
        ALOG_ASSERT((index & mask) < mFrameCount);
        ALOG_ASSERT(increment <= mFrameCountP2);
        if ((index & mask) + increment >= mFrameCount) {
            increment += mFudgeFactor;
        }
        index += increment;
        ALOG_ASSERT((index & mask) < mFrameCount);
        return index;
    } else {
        return index + increment;
    }
}

int32_t audio_utils_fifo_base::diff(uint32_t rear, uint32_t front, size_t *lost, bool flush) const
        __attribute__((no_sanitize("integer")))
{
    // TODO replace multiple returns by a single return point so this isn't needed
    if (lost != NULL) {
        *lost = 0;
    }
    if (mIsShutdown) {
        return -EIO;
    }
    uint32_t diff = rear - front;
    if (mFudgeFactor > 0) {
        uint32_t mask = mFrameCountP2 - 1;
        uint32_t rearOffset = rear & mask;
        uint32_t frontOffset = front & mask;
        if (rearOffset >= mFrameCount || frontOffset >= mFrameCount) {
            ALOGE("%s frontOffset=%u rearOffset=%u mFrameCount=%u",
                    __func__, frontOffset, rearOffset, mFrameCount);
            shutdown();
            return -EIO;
        }
        // genDiff is the difference between the generation count fields of rear and front,
        // and is always a multiple of mFrameCountP2.
        uint32_t genDiff = (rear & ~mask) - (front & ~mask);
        // It's OK for writer to be one generation beyond reader,
        // but reader has lost frames if writer is further than one generation beyond.
        if (genDiff > mFrameCountP2) {
            if (lost != NULL) {
                // Calculate the number of lost frames as the raw difference,
                // less the mFrameCount frames that are still valid and can be read on retry,
                // less the wasted indices that don't count as true lost frames.
                *lost = diff - (flush ? 0 : mFrameCount) - mFudgeFactor * (genDiff/mFrameCountP2);
            }
            return -EOVERFLOW;
        }
        // If writer is one generation beyond reader, skip over the wasted indices.
        if (genDiff > 0) {
            diff -= mFudgeFactor;
            // Note is still possible for diff > mFrameCount. BCD 16 - BCD 1 shows the problem.
            // genDiff is 16, fudge is 6, decimal diff is 15 = (22 - 1 - 6).
            // So we need to check diff for overflow one more time. See "if" a few lines below.
        }
    }
    // FIFO should not be overfull
    if (diff > mFrameCount) {
        if (lost != NULL) {
            *lost = diff - (flush ? 0 : mFrameCount);
        }
        return -EOVERFLOW;
    }
    return (int32_t) diff;
}

void audio_utils_fifo_base::shutdown() const
{
    ALOGE("%s", __func__);
    mIsShutdown = true;
}

////////////////////////////////////////////////////////////////////////////////

audio_utils_fifo::audio_utils_fifo(uint32_t frameCount, uint32_t frameSize, void *buffer,
        audio_utils_fifo_index& writerRear, audio_utils_fifo_index *throttleFront)
        __attribute__((no_sanitize("integer"))) :
    audio_utils_fifo_base(frameCount, writerRear, throttleFront),
    mFrameSize(frameSize), mBuffer(buffer)
{
    // maximum value of frameCount * frameSize is INT32_MAX (2^31 - 1), not 2^31, because we need to
    // be able to distinguish successful and error return values from read and write.
    LOG_ALWAYS_FATAL_IF(frameCount == 0 || frameSize == 0 || buffer == NULL ||
            frameCount > ((uint32_t) INT32_MAX) / frameSize);
}

audio_utils_fifo::audio_utils_fifo(uint32_t frameCount, uint32_t frameSize, void *buffer,
        bool throttlesWriter) :
    audio_utils_fifo(frameCount, frameSize, buffer, mSingleProcessSharedRear,
        throttlesWriter ?  &mSingleProcessSharedFront : NULL)
{
}

audio_utils_fifo::~audio_utils_fifo()
{
}

////////////////////////////////////////////////////////////////////////////////

audio_utils_fifo_provider::audio_utils_fifo_provider(audio_utils_fifo& fifo) :
    mFifo(fifo), mObtained(0), mTotalReleased(0)
{
}

audio_utils_fifo_provider::~audio_utils_fifo_provider()
{
}

////////////////////////////////////////////////////////////////////////////////

audio_utils_fifo_writer::audio_utils_fifo_writer(audio_utils_fifo& fifo) :
    audio_utils_fifo_provider(fifo), mLocalRear(0),
    mArmLevel(fifo.mFrameCount), mTriggerLevel(0),
    mIsArmed(true), // because initial fill level of zero is < mArmLevel
    mEffectiveFrames(fifo.mFrameCount)
{
}

audio_utils_fifo_writer::~audio_utils_fifo_writer()
{
}

ssize_t audio_utils_fifo_writer::write(const void *buffer, size_t count,
        const struct timespec *timeout)
        __attribute__((no_sanitize("integer")))
{
    audio_utils_iovec iovec[2];
    ssize_t availToWrite = obtain(iovec, count, timeout);
    if (availToWrite > 0) {
        memcpy((char *) mFifo.mBuffer + iovec[0].mOffset * mFifo.mFrameSize, buffer,
                iovec[0].mLength * mFifo.mFrameSize);
        if (iovec[1].mLength > 0) {
            memcpy((char *) mFifo.mBuffer + iovec[1].mOffset * mFifo.mFrameSize,
                    (char *) buffer + (iovec[0].mLength * mFifo.mFrameSize),
                    iovec[1].mLength * mFifo.mFrameSize);
        }
        release(availToWrite);
    }
    return availToWrite;
}

// iovec == NULL is not part of the public API, but internally it means don't set mObtained
ssize_t audio_utils_fifo_writer::obtain(audio_utils_iovec iovec[2], size_t count,
        const struct timespec *timeout)
        __attribute__((no_sanitize("integer")))
{
    int err = 0;
    size_t availToWrite;
    if (mFifo.mThrottleFront != NULL) {
        int retries = kRetries;
        uint32_t front;
        for (;;) {
            front = mFifo.mThrottleFront->loadAcquire();
            // returns -EIO if mIsShutdown
            int32_t filled = mFifo.diff(mLocalRear, front);
            if (filled < 0) {
                // on error, return an empty slice
                err = filled;
                availToWrite = 0;
                break;
            }
            availToWrite = mEffectiveFrames > (uint32_t) filled ?
                    mEffectiveFrames - (uint32_t) filled : 0;
            // TODO pull out "count == 0"
            if (count == 0 || availToWrite > 0 || timeout == NULL ||
                    (timeout->tv_sec == 0 && timeout->tv_nsec == 0)) {
                break;
            }
            // TODO add comments
            // TODO abstract out switch and replace by general sync object
            //      the high level code (synchronization, sleep, futex, iovec) should be completely
            //      separate from the low level code (indexes, available, masking).
            int op = FUTEX_WAIT;
            switch (mFifo.mThrottleFrontSync) {
            case AUDIO_UTILS_FIFO_SYNC_SLEEP:
                err = audio_utils_clock_nanosleep(CLOCK_MONOTONIC, 0 /*flags*/, timeout,
                        NULL /*remain*/);
                if (err < 0) {
                    LOG_ALWAYS_FATAL_IF(errno != EINTR, "unexpected err=%d errno=%d", err, errno);
                    err = -errno;
                } else {
                    err = -ETIMEDOUT;
                }
                break;
            case AUDIO_UTILS_FIFO_SYNC_PRIVATE:
                op = FUTEX_WAIT_PRIVATE;
                FALLTHROUGH_INTENDED;
            case AUDIO_UTILS_FIFO_SYNC_SHARED:
                if (timeout->tv_sec == LONG_MAX) {
                    timeout = NULL;
                }
                err = mFifo.mThrottleFront->wait(op, front, timeout);
                if (err < 0) {
                    switch (errno) {
                    case EWOULDBLOCK:
                        // Benign race condition with partner: mFifo.mThrottleFront->mIndex
                        // changed value between the earlier atomic_load_explicit() and sys_futex().
                        // Try to load index again, but give up if we are unable to converge.
                        if (retries-- > 0) {
                            // bypass the "timeout = NULL;" below
                            continue;
                        }
                        FALLTHROUGH_INTENDED;
                    case EINTR:
                    case ETIMEDOUT:
                        err = -errno;
                        break;
                    default:
                        LOG_ALWAYS_FATAL("unexpected err=%d errno=%d", err, errno);
                        break;
                    }
                }
                break;
            default:
                LOG_ALWAYS_FATAL("mFifo.mThrottleFrontSync=%d", mFifo.mThrottleFrontSync);
                break;
            }
            timeout = NULL;
        }
    } else {
        if (mFifo.mIsShutdown) {
            err = -EIO;
            availToWrite = 0;
        } else {
            availToWrite = mEffectiveFrames;
        }
    }
    if (availToWrite > count) {
        availToWrite = count;
    }
    uint32_t rearOffset = mLocalRear & (mFifo.mFrameCountP2 - 1);
    size_t part1 = mFifo.mFrameCount - rearOffset;
    if (part1 > availToWrite) {
        part1 = availToWrite;
    }
    size_t part2 = part1 > 0 ? availToWrite - part1 : 0;
    // return slice
    if (iovec != NULL) {
        iovec[0].mOffset = rearOffset;
        iovec[0].mLength = part1;
        iovec[1].mOffset = 0;
        iovec[1].mLength = part2;
        mObtained = availToWrite;
    }
    return availToWrite > 0 ? availToWrite : err;
}

void audio_utils_fifo_writer::release(size_t count)
        __attribute__((no_sanitize("integer")))
{
    // no need to do an early check for mIsShutdown, because the extra code executed is harmless
    if (count > 0) {
        if (count > mObtained) {
            ALOGE("%s(count=%zu) > mObtained=%u", __func__, count, mObtained);
            mFifo.shutdown();
            return;
        }
        if (mFifo.mThrottleFront != NULL) {
            uint32_t front = mFifo.mThrottleFront->loadAcquire();
            // returns -EIO if mIsShutdown
            int32_t filled = mFifo.diff(mLocalRear, front);
            mLocalRear = mFifo.sum(mLocalRear, count);
            mFifo.mWriterRear.storeRelease(mLocalRear);
            // TODO add comments
            int op = FUTEX_WAKE;
            switch (mFifo.mWriterRearSync) {
            case AUDIO_UTILS_FIFO_SYNC_SLEEP:
                break;
            case AUDIO_UTILS_FIFO_SYNC_PRIVATE:
                op = FUTEX_WAKE_PRIVATE;
                FALLTHROUGH_INTENDED;
            case AUDIO_UTILS_FIFO_SYNC_SHARED:
                if (filled >= 0) {
                    if ((uint32_t) filled < mArmLevel) {
                        mIsArmed = true;
                    }
                    if (mIsArmed && filled + count > mTriggerLevel) {
                        int err = mFifo.mWriterRear.wake(op, INT32_MAX /*waiters*/);
                        // err is number of processes woken up
                        if (err < 0) {
                            LOG_ALWAYS_FATAL("%s: unexpected err=%d errno=%d",
                                    __func__, err, errno);
                        }
                        mIsArmed = false;
                    }
                }
                break;
            default:
                LOG_ALWAYS_FATAL("mFifo.mWriterRearSync=%d", mFifo.mWriterRearSync);
                break;
            }
        } else {
            mLocalRear = mFifo.sum(mLocalRear, count);
            mFifo.mWriterRear.storeRelease(mLocalRear);
        }
        mObtained -= count;
        mTotalReleased += count;
    }
}

ssize_t audio_utils_fifo_writer::available()
{
    // iovec == NULL is not part of the public API, but internally it means don't set mObtained
    return obtain(NULL /*iovec*/, SIZE_MAX /*count*/, NULL /*timeout*/);
}

void audio_utils_fifo_writer::resize(uint32_t frameCount)
{
    // cap to range [0, mFifo.mFrameCount]
    if (frameCount > mFifo.mFrameCount) {
        frameCount = mFifo.mFrameCount;
    }
    // if we reduce the effective frame count, update hysteresis points to be within the new range
    if (frameCount < mEffectiveFrames) {
        if (mArmLevel > frameCount) {
            mArmLevel = frameCount;
        }
        if (mTriggerLevel > frameCount) {
            mTriggerLevel = frameCount;
        }
    }
    mEffectiveFrames = frameCount;
}

uint32_t audio_utils_fifo_writer::size() const
{
    return mEffectiveFrames;
}

void audio_utils_fifo_writer::setHysteresis(uint32_t lowLevelArm, uint32_t highLevelTrigger)
{
    // cap to range [0, mEffectiveFrames]
    if (lowLevelArm > mEffectiveFrames) {
        lowLevelArm = mEffectiveFrames;
    }
    if (highLevelTrigger > mEffectiveFrames) {
        highLevelTrigger = mEffectiveFrames;
    }
    // TODO this is overly conservative; it would be better to arm based on actual fill level
    if (lowLevelArm > mArmLevel) {
        mIsArmed = true;
    }
    mArmLevel = lowLevelArm;
    mTriggerLevel = highLevelTrigger;
}

void audio_utils_fifo_writer::getHysteresis(uint32_t *armLevel, uint32_t *triggerLevel) const
{
    *armLevel = mArmLevel;
    *triggerLevel = mTriggerLevel;
}

////////////////////////////////////////////////////////////////////////////////

audio_utils_fifo_reader::audio_utils_fifo_reader(audio_utils_fifo& fifo, bool throttlesWriter,
        bool flush) :
    audio_utils_fifo_provider(fifo),

    // If we throttle the writer, then initialize our front index to zero so that we see all data
    // currently in the buffer.
    // Otherwise, ignore everything currently in the buffer by initializing our front index to the
    // current value of writer's rear.  This avoids an immediate -EOVERFLOW (overrun) in the case
    // where reader starts out more than one buffer behind writer.  The initial catch-up does not
    // contribute towards the totalLost, totalFlushed, or totalReleased counters.
    mLocalFront(throttlesWriter ? 0 : mFifo.mWriterRear.loadConsume()),

    mThrottleFront(throttlesWriter ? mFifo.mThrottleFront : NULL),
    mFlush(flush),
    mArmLevel(-1), mTriggerLevel(mFifo.mFrameCount),
    mIsArmed(true), // because initial fill level of zero is > mArmLevel
    mTotalLost(0), mTotalFlushed(0)
{
}

audio_utils_fifo_reader::~audio_utils_fifo_reader()
{
    // TODO Need a way to pass throttle capability to the another reader, should one reader exit.
}

ssize_t audio_utils_fifo_reader::read(void *buffer, size_t count, const struct timespec *timeout,
        size_t *lost)
        __attribute__((no_sanitize("integer")))
{
    audio_utils_iovec iovec[2];
    ssize_t availToRead = obtain(iovec, count, timeout, lost);
    if (availToRead > 0) {
        memcpy(buffer, (char *) mFifo.mBuffer + iovec[0].mOffset * mFifo.mFrameSize,
                iovec[0].mLength * mFifo.mFrameSize);
        if (iovec[1].mLength > 0) {
            memcpy((char *) buffer + (iovec[0].mLength * mFifo.mFrameSize),
                    (char *) mFifo.mBuffer + iovec[1].mOffset * mFifo.mFrameSize,
                    iovec[1].mLength * mFifo.mFrameSize);
        }
        release(availToRead);
    }
    return availToRead;
}

ssize_t audio_utils_fifo_reader::obtain(audio_utils_iovec iovec[2], size_t count,
        const struct timespec *timeout)
        __attribute__((no_sanitize("integer")))
{
    return obtain(iovec, count, timeout, NULL /*lost*/);
}

void audio_utils_fifo_reader::release(size_t count)
        __attribute__((no_sanitize("integer")))
{
    // no need to do an early check for mIsShutdown, because the extra code executed is harmless
    if (count > 0) {
        if (count > mObtained) {
            ALOGE("%s(count=%zu) > mObtained=%u", __func__, count, mObtained);
            mFifo.shutdown();
            return;
        }
        if (mThrottleFront != NULL) {
            uint32_t rear = mFifo.mWriterRear.loadAcquire();
            // returns -EIO if mIsShutdown
            int32_t filled = mFifo.diff(rear, mLocalFront);
            mLocalFront = mFifo.sum(mLocalFront, count);
            mThrottleFront->storeRelease(mLocalFront);
            // TODO add comments
            int op = FUTEX_WAKE;
            switch (mFifo.mThrottleFrontSync) {
            case AUDIO_UTILS_FIFO_SYNC_SLEEP:
                break;
            case AUDIO_UTILS_FIFO_SYNC_PRIVATE:
                op = FUTEX_WAKE_PRIVATE;
                FALLTHROUGH_INTENDED;
            case AUDIO_UTILS_FIFO_SYNC_SHARED:
                if (filled >= 0) {
                    if (filled > mArmLevel) {
                        mIsArmed = true;
                    }
                    if (mIsArmed && filled - count < mTriggerLevel) {
                        int err = mThrottleFront->wake(op, 1 /*waiters*/);
                        // err is number of processes woken up
                        if (err < 0 || err > 1) {
                            LOG_ALWAYS_FATAL("%s: unexpected err=%d errno=%d",
                                    __func__, err, errno);
                        }
                        mIsArmed = false;
                    }
                }
                break;
            default:
                LOG_ALWAYS_FATAL("mFifo.mThrottleFrontSync=%d", mFifo.mThrottleFrontSync);
                break;
            }
        } else {
            mLocalFront = mFifo.sum(mLocalFront, count);
        }
        mObtained -= count;
        mTotalReleased += count;
    }
}

// iovec == NULL is not part of the public API, but internally it means don't set mObtained
ssize_t audio_utils_fifo_reader::obtain(audio_utils_iovec iovec[2], size_t count,
        const struct timespec *timeout, size_t *lost)
        __attribute__((no_sanitize("integer")))
{
    int err = 0;
    int retries = kRetries;
    uint32_t rear;
    for (;;) {
        rear = mFifo.mWriterRear.loadAcquire();
        // TODO pull out "count == 0"
        if (count == 0 || rear != mLocalFront || timeout == NULL ||
                (timeout->tv_sec == 0 && timeout->tv_nsec == 0)) {
            break;
        }
        // TODO add comments
        int op = FUTEX_WAIT;
        switch (mFifo.mWriterRearSync) {
        case AUDIO_UTILS_FIFO_SYNC_SLEEP:
            err = audio_utils_clock_nanosleep(CLOCK_MONOTONIC, 0 /*flags*/, timeout,
                    NULL /*remain*/);
            if (err < 0) {
                LOG_ALWAYS_FATAL_IF(errno != EINTR, "unexpected err=%d errno=%d", err, errno);
                err = -errno;
            } else {
                err = -ETIMEDOUT;
            }
            break;
        case AUDIO_UTILS_FIFO_SYNC_PRIVATE:
            op = FUTEX_WAIT_PRIVATE;
            FALLTHROUGH_INTENDED;
        case AUDIO_UTILS_FIFO_SYNC_SHARED:
            if (timeout->tv_sec == LONG_MAX) {
                timeout = NULL;
            }
            err = mFifo.mWriterRear.wait(op, rear, timeout);
            if (err < 0) {
                switch (errno) {
                case EWOULDBLOCK:
                    // Benign race condition with partner: mFifo.mWriterRear->mIndex
                    // changed value between the earlier atomic_load_explicit() and sys_futex().
                    // Try to load index again, but give up if we are unable to converge.
                    if (retries-- > 0) {
                        // bypass the "timeout = NULL;" below
                        continue;
                    }
                    FALLTHROUGH_INTENDED;
                case EINTR:
                case ETIMEDOUT:
                    err = -errno;
                    break;
                default:
                    LOG_ALWAYS_FATAL("unexpected err=%d errno=%d", err, errno);
                    break;
                }
            }
            break;
        default:
            LOG_ALWAYS_FATAL("mFifo.mWriterRearSync=%d", mFifo.mWriterRearSync);
            break;
        }
        timeout = NULL;
    }
    size_t ourLost;
    if (lost == NULL) {
        lost = &ourLost;
    }
    // returns -EIO if mIsShutdown
    int32_t filled = mFifo.diff(rear, mLocalFront, lost, mFlush);
    mTotalLost += *lost;
    mTotalReleased += *lost;
    if (filled < 0) {
        if (filled == -EOVERFLOW) {
            // catch up with writer, but preserve the still valid frames in buffer
            mLocalFront = rear - (mFlush ? 0 : mFifo.mFrameCountP2 /*sic*/);
        }
        // on error, return an empty slice
        err = filled;
        filled = 0;
    }
    size_t availToRead = (size_t) filled;
    if (availToRead > count) {
        availToRead = count;
    }
    uint32_t frontOffset = mLocalFront & (mFifo.mFrameCountP2 - 1);
    size_t part1 = mFifo.mFrameCount - frontOffset;
    if (part1 > availToRead) {
        part1 = availToRead;
    }
    size_t part2 = part1 > 0 ? availToRead - part1 : 0;
    // return slice
    if (iovec != NULL) {
        iovec[0].mOffset = frontOffset;
        iovec[0].mLength = part1;
        iovec[1].mOffset = 0;
        iovec[1].mLength = part2;
        mObtained = availToRead;
    }
    return availToRead > 0 ? availToRead : err;
}

ssize_t audio_utils_fifo_reader::available()
{
    return available(NULL /*lost*/);
}

ssize_t audio_utils_fifo_reader::available(size_t *lost)
{
    // iovec == NULL is not part of the public API, but internally it means don't set mObtained
    return obtain(NULL /*iovec*/, SIZE_MAX /*count*/, NULL /*timeout*/, lost);
}

ssize_t audio_utils_fifo_reader::flush(size_t *lost)
{
    audio_utils_iovec iovec[2];
    ssize_t ret = obtain(iovec, SIZE_MAX /*count*/, NULL /*timeout*/, lost);
    if (ret > 0) {
        size_t flushed = (size_t) ret;
        release(flushed);
        mTotalFlushed += flushed;
        ret = flushed;
    }
    return ret;
}

void audio_utils_fifo_reader::setHysteresis(int32_t armLevel, uint32_t triggerLevel)
{
    // cap to range [0, mFifo.mFrameCount]
    if (armLevel < 0) {
        armLevel = -1;
    } else if ((uint32_t) armLevel > mFifo.mFrameCount) {
        armLevel = mFifo.mFrameCount;
    }
    if (triggerLevel > mFifo.mFrameCount) {
        triggerLevel = mFifo.mFrameCount;
    }
    // TODO this is overly conservative; it would be better to arm based on actual fill level
    if (armLevel < mArmLevel) {
        mIsArmed = true;
    }
    mArmLevel = armLevel;
    mTriggerLevel = triggerLevel;
}

void audio_utils_fifo_reader::getHysteresis(int32_t *armLevel, uint32_t *triggerLevel) const
{
    *armLevel = mArmLevel;
    *triggerLevel = mTriggerLevel;
}
