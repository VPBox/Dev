/*
 * Copyright 2019 The Android Open Source Project
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

#include <compositionengine/impl/HwcBufferCache.h>
#include <gtest/gtest.h>
#include <gui/BufferQueue.h>
#include <ui/GraphicBuffer.h>

namespace android::compositionengine {
namespace {

class TestableHwcBufferCache : public impl::HwcBufferCache {
public:
    void getHwcBuffer(int slot, const sp<GraphicBuffer>& buffer, uint32_t* outSlot,
                      sp<GraphicBuffer>* outBuffer) {
        HwcBufferCache::getHwcBuffer(slot, buffer, outSlot, outBuffer);
    }
};

class HwcBufferCacheTest : public testing::Test {
public:
    ~HwcBufferCacheTest() override = default;

    void testSlot(const int inSlot, const uint32_t expectedSlot) {
        uint32_t outSlot;
        sp<GraphicBuffer> outBuffer;

        // The first time, the output  is the same as the input
        mCache.getHwcBuffer(inSlot, mBuffer1, &outSlot, &outBuffer);
        EXPECT_EQ(expectedSlot, outSlot);
        EXPECT_EQ(mBuffer1, outBuffer);

        // The second time with the same buffer, the outBuffer is nullptr.
        mCache.getHwcBuffer(inSlot, mBuffer1, &outSlot, &outBuffer);
        EXPECT_EQ(expectedSlot, outSlot);
        EXPECT_EQ(nullptr, outBuffer.get());

        // With a new buffer, the outBuffer is the input.
        mCache.getHwcBuffer(inSlot, mBuffer2, &outSlot, &outBuffer);
        EXPECT_EQ(expectedSlot, outSlot);
        EXPECT_EQ(mBuffer2, outBuffer);

        // Again, the second request with the same buffer sets outBuffer to nullptr.
        mCache.getHwcBuffer(inSlot, mBuffer2, &outSlot, &outBuffer);
        EXPECT_EQ(expectedSlot, outSlot);
        EXPECT_EQ(nullptr, outBuffer.get());

        // Setting a slot to use nullptr lookslike works, but note that
        // the output values make it look like no new buffer is being set....
        mCache.getHwcBuffer(inSlot, sp<GraphicBuffer>(), &outSlot, &outBuffer);
        EXPECT_EQ(expectedSlot, outSlot);
        EXPECT_EQ(nullptr, outBuffer.get());
    }

    impl::HwcBufferCache mCache;
    sp<GraphicBuffer> mBuffer1{new GraphicBuffer(1, 1, HAL_PIXEL_FORMAT_RGBA_8888, 1, 0)};
    sp<GraphicBuffer> mBuffer2{new GraphicBuffer(1, 1, HAL_PIXEL_FORMAT_RGBA_8888, 1, 0)};
};

TEST_F(HwcBufferCacheTest, cacheWorksForSlotZero) {
    testSlot(0, 0);
}

TEST_F(HwcBufferCacheTest, cacheWorksForMaxSlot) {
    testSlot(BufferQueue::NUM_BUFFER_SLOTS - 1, BufferQueue::NUM_BUFFER_SLOTS - 1);
}

TEST_F(HwcBufferCacheTest, cacheMapsNegativeSlotToZero) {
    testSlot(-123, 0);
}

} // namespace
} // namespace android::compositionengine
