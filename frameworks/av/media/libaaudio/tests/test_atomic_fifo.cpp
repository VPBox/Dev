/*
 * Copyright (C) 2018 The Android Open Source Project
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

#include <iostream>

#include <gtest/gtest.h>
#include <stdlib.h>

#include "fifo/FifoBuffer.h"
#include "fifo/FifoController.h"

using android::fifo_frames_t;
using android::fifo_counter_t;
using android::FifoController;
using android::FifoBuffer;
using android::WrappingBuffer;

TEST(test_fifo_controller, fifo_indices) {
    // Values are arbitrary primes designed to trigger edge cases.
    constexpr int capacity = 83;
    constexpr int threshold = 47;
    FifoController   fifoController(capacity, threshold);
    ASSERT_EQ(capacity, fifoController.getCapacity());
    ASSERT_EQ(threshold, fifoController.getThreshold());

    ASSERT_EQ(0, fifoController.getReadCounter());
    ASSERT_EQ(0, fifoController.getWriteCounter());
    ASSERT_EQ(0, fifoController.getFullFramesAvailable());
    ASSERT_EQ(threshold, fifoController.getEmptyFramesAvailable());

    // Pretend to write some data.
    constexpr int advance1 = 23;
    fifoController.advanceWriteIndex(advance1);
    int advanced = advance1;
    ASSERT_EQ(0, fifoController.getReadCounter());
    ASSERT_EQ(0, fifoController.getReadIndex());
    ASSERT_EQ(advanced, fifoController.getWriteCounter());
    ASSERT_EQ(advanced, fifoController.getWriteIndex());
    ASSERT_EQ(advanced, fifoController.getFullFramesAvailable());
    ASSERT_EQ(threshold - advanced, fifoController.getEmptyFramesAvailable());

    // Pretend to read the data.
    fifoController.advanceReadIndex(advance1);
    ASSERT_EQ(advanced, fifoController.getReadCounter());
    ASSERT_EQ(advanced, fifoController.getReadIndex());
    ASSERT_EQ(advanced, fifoController.getWriteCounter());
    ASSERT_EQ(advanced, fifoController.getWriteIndex());
    ASSERT_EQ(0, fifoController.getFullFramesAvailable());
    ASSERT_EQ(threshold, fifoController.getEmptyFramesAvailable());

    // Write past end of buffer.
    constexpr int advance2 = 13 + capacity - advance1;
    fifoController.advanceWriteIndex(advance2);
    advanced += advance2;
    ASSERT_EQ(advance1, fifoController.getReadCounter());
    ASSERT_EQ(advance1, fifoController.getReadIndex());
    ASSERT_EQ(advanced, fifoController.getWriteCounter());
    ASSERT_EQ(advanced - capacity, fifoController.getWriteIndex());
    ASSERT_EQ(advance2, fifoController.getFullFramesAvailable());
    ASSERT_EQ(threshold - advance2, fifoController.getEmptyFramesAvailable());
}

TEST(test_fifo_controller, fifo_wrap_around_zero) {
    constexpr int capacity = 7; // arbitrary prime
    constexpr int threshold = capacity;
    FifoController fifoController(capacity, threshold);
    ASSERT_EQ(capacity, fifoController.getCapacity());
    ASSERT_EQ(threshold, fifoController.getThreshold());

    fifoController.setReadCounter(-10); // a bit less than negative capacity
    for (int i = 0; i < 20; i++) {
        EXPECT_EQ(i - 10, fifoController.getReadCounter());
        EXPECT_GE(fifoController.getReadIndex(), 0);
        EXPECT_LT(fifoController.getReadIndex(), capacity);
        fifoController.advanceReadIndex(1);
    }

    fifoController.setWriteCounter(-10);
    for (int i = 0; i < 20; i++) {
        EXPECT_EQ(i - 10, fifoController.getWriteCounter());
        EXPECT_GE(fifoController.getWriteIndex(), 0);
        EXPECT_LT(fifoController.getWriteIndex(), capacity);
        fifoController.advanceWriteIndex(1);
    }
}


// TODO consider using a template for other data types.

// Create a big array and then use a region in the middle for the  unit tests.
// Then we can scan the rest of the array to see if it got clobbered.
static constexpr fifo_frames_t kBigArraySize = 1024;
static constexpr fifo_frames_t kFifoDataOffset = 128; // starting index of FIFO data
static constexpr int16_t       kSafeDataValue = 0x7654; // original value of BigArray

class TestFifoBuffer {
public:
    explicit TestFifoBuffer(fifo_frames_t capacity, fifo_frames_t threshold = 0)
        : mFifoBuffer(sizeof(int16_t), capacity,
                      &mReadIndex,
                      &mWriteIndex,
                      &mVeryBigArray[kFifoDataOffset]) // address of start of FIFO data
    {

        // Assume a frame is one int16_t.
        // For reading and writing.
        if (threshold <= 0) {
            threshold = capacity;
        }
        mFifoBuffer.setThreshold(threshold);
        mThreshold = threshold;

        for (fifo_frames_t i = 0; i < kBigArraySize; i++) {
            mVeryBigArray[i] = kSafeDataValue;
        }
    }

    void checkMisc() {
        ASSERT_EQ((int32_t)(2 * sizeof(int16_t)), mFifoBuffer.convertFramesToBytes(2));
        ASSERT_EQ(mThreshold, mFifoBuffer.getThreshold());
    }

    void verifyAddressInRange(void *p, void *valid, size_t numBytes) {
        uintptr_t p_int = (uintptr_t) p;
        uintptr_t valid_int = (uintptr_t) valid;
        EXPECT_GE(p_int, valid_int);
        EXPECT_LT(p_int, (valid_int + numBytes));
    }

    void verifyStorageIntegrity() {
        for (fifo_frames_t i = 0; i < kFifoDataOffset; i++) {
            EXPECT_EQ(mVeryBigArray[i], kSafeDataValue);
        }
        fifo_frames_t firstFrameAfter = kFifoDataOffset + mFifoBuffer.getBufferCapacityInFrames();
        for (fifo_frames_t i = firstFrameAfter; i < kBigArraySize; i++) {
            EXPECT_EQ(mVeryBigArray[i], kSafeDataValue);
        }
    }

    // Verify that the available frames in each part add up correctly.
    void verifyWrappingBuffer() {
        WrappingBuffer wrappingBuffer;


        // Does the sum of the two parts match the available value returned?
        // For EmptyRoom
        fifo_frames_t framesAvailable =
                mFifoBuffer.getEmptyFramesAvailable();
        fifo_frames_t wrapAvailable = mFifoBuffer.getEmptyRoomAvailable(&wrappingBuffer);
        EXPECT_EQ(framesAvailable, wrapAvailable);
        fifo_frames_t bothAvailable = wrappingBuffer.numFrames[0] + wrappingBuffer.numFrames[1];
        EXPECT_EQ(framesAvailable, bothAvailable);
        // For FullData
        framesAvailable =
                mFifoBuffer.getFullFramesAvailable();
        wrapAvailable = mFifoBuffer.getFullDataAvailable(&wrappingBuffer);
        EXPECT_EQ(framesAvailable, wrapAvailable);
        bothAvailable = wrappingBuffer.numFrames[0] + wrappingBuffer.numFrames[1];
        EXPECT_EQ(framesAvailable, bothAvailable);

        // Are frame counts in legal range?
        fifo_frames_t capacity = mFifoBuffer.getBufferCapacityInFrames();
        EXPECT_GE(wrappingBuffer.numFrames[0], 0);
        EXPECT_LE(wrappingBuffer.numFrames[0], capacity);
        EXPECT_GE(wrappingBuffer.numFrames[1], 0);
        EXPECT_LE(wrappingBuffer.numFrames[1], capacity);

        // Are addresses within the FIFO data area?
        size_t validBytes = capacity * sizeof(int16_t);
        if (wrappingBuffer.numFrames[0]) {
            verifyAddressInRange(wrappingBuffer.data[0], mFifoStorage, validBytes);
            uint8_t *last = ((uint8_t *)wrappingBuffer.data[0])
                            + mFifoBuffer.convertFramesToBytes(wrappingBuffer.numFrames[0]) - 1;
            verifyAddressInRange(last, mFifoStorage, validBytes);
        }
        if (wrappingBuffer.numFrames[1]) {
            verifyAddressInRange(wrappingBuffer.data[1], mFifoStorage, validBytes);
            uint8_t *last = ((uint8_t *)wrappingBuffer.data[1])
                            + mFifoBuffer.convertFramesToBytes(wrappingBuffer.numFrames[1]) - 1;
            verifyAddressInRange(last, mFifoStorage, validBytes);
        }

    }

    // Write data but do not overflow.
    void writeMultipleDataFrames(fifo_frames_t numFrames) {
        fifo_frames_t framesAvailable =
                mFifoBuffer.getEmptyFramesAvailable();
        fifo_frames_t framesToWrite = std::min(framesAvailable, numFrames);
        for (int i = 0; i < framesToWrite; i++) {
            mData[i] = mNextWriteIndex++;
        }
        fifo_frames_t actual = mFifoBuffer.write(mData, framesToWrite);
        ASSERT_EQ(framesToWrite, actual);
    }

    // Read whatever data is available, Do not underflow.
    void verifyMultipleDataFrames(fifo_frames_t numFrames) {
        fifo_frames_t framesAvailable =
                mFifoBuffer.getFullFramesAvailable();
        fifo_frames_t framesToRead = std::min(framesAvailable, numFrames);
        fifo_frames_t actual = mFifoBuffer.read(mData, framesToRead);
        ASSERT_EQ(framesToRead, actual);
        for (int i = 0; i < framesToRead; i++) {
            ASSERT_EQ(mNextVerifyIndex++, mData[i]);
        }
    }

    // Read specified number of frames
    void verifyRequestedData(fifo_frames_t numFrames) {
        fifo_frames_t framesAvailable =
                mFifoBuffer.getFullFramesAvailable();
        ASSERT_LE(numFrames, framesAvailable);
        fifo_frames_t framesToRead = std::min(framesAvailable, numFrames);
        fifo_frames_t actual = mFifoBuffer.read(mData, framesToRead);
        ASSERT_EQ(actual, numFrames);
        for (int i = 0; i < actual; i++) {
            ASSERT_EQ(mNextVerifyIndex++, mData[i]);
        }
    }

    // Wrap around the end of the buffer.
    void checkWrappingWriteRead() {
        constexpr int frames1 = 43;
        constexpr int frames2 = 15;

        writeMultipleDataFrames(frames1);
        verifyWrappingBuffer();
        verifyRequestedData(frames1);
        verifyWrappingBuffer();

        writeMultipleDataFrames(frames2);
        verifyWrappingBuffer();
        verifyRequestedData(frames2);
        verifyWrappingBuffer();

        verifyStorageIntegrity();
    }

    // Write and Read a specific amount of data.
    void checkWriteRead() {
        const fifo_frames_t capacity = mFifoBuffer.getBufferCapacityInFrames();
        // Wrap around with the smaller region in the second half.
        const int frames1 = capacity - 4;
        const int frames2 = 7; // arbitrary, small
        writeMultipleDataFrames(frames1);
        verifyRequestedData(frames1);
        writeMultipleDataFrames(frames2);
        verifyRequestedData(frames2);

        verifyStorageIntegrity();
    }

    // Write and Read a specific amount of data.
    void checkWriteReadSmallLarge() {
        const fifo_frames_t capacity = mFifoBuffer.getBufferCapacityInFrames();
        // Wrap around with the larger region in the second half.
        const int frames1 = capacity - 4;
        const int frames2 = capacity - 9; // arbitrary, large
        writeMultipleDataFrames(frames1);
        verifyRequestedData(frames1);
        writeMultipleDataFrames(frames2);
        verifyRequestedData(frames2);

        verifyStorageIntegrity();
    }

    // Randomly read or write up to the maximum amount of data.
    void checkRandomWriteRead() {
        for (int i = 0; i < 20; i++) {
            fifo_frames_t framesEmpty =
                    mFifoBuffer.getEmptyFramesAvailable();
            fifo_frames_t numFrames = (fifo_frames_t)(drand48() * framesEmpty);
            writeMultipleDataFrames(numFrames);

            fifo_frames_t framesFull =
                    mFifoBuffer.getFullFramesAvailable();
            numFrames = (fifo_frames_t)(drand48() * framesFull);
            verifyMultipleDataFrames(numFrames);
        }

        verifyStorageIntegrity();
    }

    // Write and Read a specific amount of data.
    void checkNegativeCounters() {
        fifo_counter_t counter = -9876;
        mFifoBuffer.setWriteCounter(counter);
        mFifoBuffer.setReadCounter(counter);
        checkWrappingWriteRead();
    }

    // Wrap over the boundary at 0x7FFFFFFFFFFFFFFF
    // Note that the behavior of a signed overflow is technically undefined.
    void checkHalfWrap() {
        fifo_counter_t counter = INT64_MAX - 10;
        mFifoBuffer.setWriteCounter(counter);
        mFifoBuffer.setReadCounter(counter);
        ASSERT_GT(mFifoBuffer.getWriteCounter(), 0);
        checkWrappingWriteRead();
        ASSERT_LT(mFifoBuffer.getWriteCounter(), 0); // did we wrap past INT64_MAX?
    }

    // Wrap over the boundary at 0xFFFFFFFFFFFFFFFF
    void checkFullWrap() {
        fifo_counter_t counter = -10;
        mFifoBuffer.setWriteCounter(counter);
        mFifoBuffer.setReadCounter(counter);
        ASSERT_LT(mFifoBuffer.getWriteCounter(), 0);
        writeMultipleDataFrames(20);
        ASSERT_GT(mFifoBuffer.getWriteCounter(), 0); // did we wrap past zero?
        verifyStorageIntegrity();
    }

    FifoBuffer     mFifoBuffer;
    fifo_frames_t  mNextWriteIndex = 0;
    fifo_frames_t  mNextVerifyIndex = 0;
    fifo_frames_t  mThreshold;

    fifo_counter_t mReadIndex = 0;
    fifo_counter_t mWriteIndex = 0;
    int16_t        mVeryBigArray[kBigArraySize]; // Use the middle of this array for the FIFO.
    int16_t       *mFifoStorage = &mVeryBigArray[kFifoDataOffset]; // Start here for storage.
    int16_t        mData[kBigArraySize]{};
};

TEST(test_fifo_buffer, fifo_write_read) {
    constexpr int capacity = 51; // arbitrary
    TestFifoBuffer tester(capacity);
    tester.checkMisc();
    tester.checkWriteRead();
}

TEST(test_fifo_buffer, fifo_wrapping_write_read) {
    constexpr int capacity = 59; // arbitrary, a little bigger this time
    TestFifoBuffer tester(capacity);
    tester.checkWrappingWriteRead();
}

TEST(test_fifo_buffer, fifo_read_write_small_large) {
    constexpr int capacity = 51; // arbitrary
    TestFifoBuffer tester(capacity);
    tester.checkWriteReadSmallLarge();
}

TEST(test_fifo_buffer, fifo_random_read_write) {
    constexpr int capacity = 51; // arbitrary
    TestFifoBuffer tester(capacity);
    tester.checkRandomWriteRead();
}

TEST(test_fifo_buffer, fifo_random_threshold) {
    constexpr int capacity = 67; // arbitrary
    constexpr int threshold = 37; // arbitrary
    TestFifoBuffer tester(capacity, threshold);
    tester.checkRandomWriteRead();
}

TEST(test_fifo_buffer, fifo_negative_counters) {
    constexpr int capacity = 49; // arbitrary
    TestFifoBuffer tester(capacity);
    tester.checkNegativeCounters();
}

TEST(test_fifo_buffer, fifo_half_wrap) {
    constexpr int capacity = 57; // arbitrary
    TestFifoBuffer tester(capacity);
    tester.checkHalfWrap();
}

TEST(test_fifo_buffer, fifo_full_wrap) {
    constexpr int capacity = 57; // arbitrary
    TestFifoBuffer tester(capacity);
    tester.checkFullWrap();
}
