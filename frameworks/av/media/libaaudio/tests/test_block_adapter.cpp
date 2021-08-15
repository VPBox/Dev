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

#include <iostream>

#include <gtest/gtest.h>

#include "utility/FixedBlockAdapter.h"
#include "utility/FixedBlockWriter.h"
#include "utility/FixedBlockReader.h"

#define FIXED_BLOCK_SIZE   47
#define TEST_BUFFER_SIZE   103

// Pass varying sized blocks.
// Frames contain a sequential index, which are easily checked.
class TestBlockAdapter {
public:
    TestBlockAdapter()
            : mTestIndex(0), mLastIndex(0) {
    }

    ~TestBlockAdapter() = default;

    void fillSequence(int32_t *indexBuffer, int32_t frameCount) {
        ASSERT_LE(frameCount, TEST_BUFFER_SIZE);
        for (int i = 0; i < frameCount; i++) {
            indexBuffer[i] = mLastIndex++;
        }
    }

    int checkSequence(const int32_t *indexBuffer, int32_t frameCount) {
        // This is equivalent to calling an output callback.
        for (int i = 0; i < frameCount; i++) {
            int32_t expected = mTestIndex++;
            int32_t actual = indexBuffer[i];
            EXPECT_EQ(expected, actual);
            if (actual != expected) {
                return -1;
            }
        }
        return 0;
    }

    int32_t            mTestBuffer[TEST_BUFFER_SIZE];
    int32_t            mTestIndex;
    int32_t            mLastIndex;
};

class TestBlockWriter : public TestBlockAdapter, FixedBlockProcessor {
public:
    TestBlockWriter()
            : mFixedBlockWriter(*this) {
        mFixedBlockWriter.open(sizeof(int32_t) * FIXED_BLOCK_SIZE);
    }

    ~TestBlockWriter() {
        mFixedBlockWriter.close();
    }

    int32_t onProcessFixedBlock(uint8_t *buffer, int32_t numBytes) override {
        int32_t frameCount = numBytes / sizeof(int32_t);
        return checkSequence((int32_t *) buffer, frameCount);
    }

    // Simulate audio input from a variable sized callback.
    int32_t testInputWrite(int32_t variableCount) {
        fillSequence(mTestBuffer, variableCount);
        int32_t sizeBytes = variableCount * sizeof(int32_t);
        return mFixedBlockWriter.processVariableBlock((uint8_t *) mTestBuffer, sizeBytes);
    }

private:
    FixedBlockWriter mFixedBlockWriter;
};

class TestBlockReader : public TestBlockAdapter, FixedBlockProcessor {
public:
    TestBlockReader()
            : mFixedBlockReader(*this) {
        mFixedBlockReader.open(sizeof(int32_t) * FIXED_BLOCK_SIZE);
    }

    ~TestBlockReader() {
        mFixedBlockReader.close();
    }

    int32_t onProcessFixedBlock(uint8_t *buffer, int32_t numBytes) override {
        int32_t frameCount = numBytes / sizeof(int32_t);
        fillSequence((int32_t *) buffer, frameCount);
        return 0;
    }

    // Simulate audio output from a variable sized callback.
    int32_t testOutputRead(int32_t variableCount) {
        int32_t sizeBytes = variableCount * sizeof(int32_t);
        int32_t result = mFixedBlockReader.processVariableBlock((uint8_t *) mTestBuffer, sizeBytes);
        if (result >= 0) {
            result = checkSequence((int32_t *)mTestBuffer, variableCount);
        }
        return result;
    }

private:
    FixedBlockReader   mFixedBlockReader;
};


TEST(test_block_adapter, block_adapter_write) {
    TestBlockWriter tester;
    int result = 0;
    const int numLoops = 1000;

    for (int i = 0; i<numLoops && result == 0; i++) {
        long r = random();
        int32_t size = (r % TEST_BUFFER_SIZE);
        ASSERT_LE(size, TEST_BUFFER_SIZE);
        ASSERT_GE(size, 0);
        result = tester.testInputWrite(size);
    }
    ASSERT_EQ(0, result);
}

TEST(test_block_adapter, block_adapter_read) {
    TestBlockReader tester;
    int result = 0;
    const int numLoops = 1000;

    for (int i = 0; i < numLoops && result == 0; i++) {
        long r = random();
        int32_t size = (r % TEST_BUFFER_SIZE);
        ASSERT_LE(size, TEST_BUFFER_SIZE);
        ASSERT_GE(size, 0);
        result = tester.testOutputRead(size);
    }
    ASSERT_EQ(0, result);
};

