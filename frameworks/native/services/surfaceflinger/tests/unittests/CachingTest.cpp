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

#undef LOG_TAG
#define LOG_TAG "CachingTest"

#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <gui/BufferQueue.h>
#include "BufferStateLayer.h"

namespace android {

class SlotGenerationTest : public testing::Test {
protected:
    BufferStateLayer::HwcSlotGenerator mHwcSlotGenerator;
    sp<GraphicBuffer> mBuffer1{new GraphicBuffer(1, 1, HAL_PIXEL_FORMAT_RGBA_8888, 1, 0)};
    sp<GraphicBuffer> mBuffer2{new GraphicBuffer(1, 1, HAL_PIXEL_FORMAT_RGBA_8888, 1, 0)};
    sp<GraphicBuffer> mBuffer3{new GraphicBuffer(10, 10, HAL_PIXEL_FORMAT_RGBA_8888, 1, 0)};
};

TEST_F(SlotGenerationTest, getHwcCacheSlot_Invalid) {
    sp<IBinder> binder = new BBinder();
    // test getting invalid client_cache_id
    client_cache_t id;
    uint32_t slot = mHwcSlotGenerator.getHwcCacheSlot(id);
    EXPECT_EQ(BufferQueue::INVALID_BUFFER_SLOT, slot);
}

TEST_F(SlotGenerationTest, getHwcCacheSlot_Basic) {
    sp<IBinder> binder = new BBinder();
    client_cache_t id;
    id.token = binder;
    id.id = 0;
    uint32_t slot = mHwcSlotGenerator.getHwcCacheSlot(id);
    EXPECT_EQ(BufferQueue::NUM_BUFFER_SLOTS - 1, slot);

    client_cache_t idB;
    idB.token = binder;
    idB.id = 1;
    slot = mHwcSlotGenerator.getHwcCacheSlot(idB);
    EXPECT_EQ(BufferQueue::NUM_BUFFER_SLOTS - 2, slot);

    slot = mHwcSlotGenerator.getHwcCacheSlot(idB);
    EXPECT_EQ(BufferQueue::NUM_BUFFER_SLOTS - 2, slot);

    slot = mHwcSlotGenerator.getHwcCacheSlot(id);
    EXPECT_EQ(BufferQueue::NUM_BUFFER_SLOTS - 1, slot);
}

TEST_F(SlotGenerationTest, getHwcCacheSlot_Reuse) {
    sp<IBinder> binder = new BBinder();
    std::vector<client_cache_t> ids;
    uint32_t cacheId = 0;
    // fill up cache
    for (uint32_t i = 0; i < BufferQueue::NUM_BUFFER_SLOTS; i++) {
        client_cache_t id;
        id.token = binder;
        id.id = cacheId;
        ids.push_back(id);

        uint32_t slot = mHwcSlotGenerator.getHwcCacheSlot(id);
        EXPECT_EQ(BufferQueue::NUM_BUFFER_SLOTS - (i + 1), slot);
        cacheId++;
    }
    for (uint32_t i = 0; i < BufferQueue::NUM_BUFFER_SLOTS; i++) {
        uint32_t slot = mHwcSlotGenerator.getHwcCacheSlot(ids[i]);
        EXPECT_EQ(BufferQueue::NUM_BUFFER_SLOTS - (i + 1), slot);
    }

    for (uint32_t i = 0; i < BufferQueue::NUM_BUFFER_SLOTS; i++) {
        client_cache_t id;
        id.token = binder;
        id.id = cacheId;
        uint32_t slot = mHwcSlotGenerator.getHwcCacheSlot(id);
        EXPECT_EQ(BufferQueue::NUM_BUFFER_SLOTS - (i + 1), slot);
        cacheId++;
    }
}
} // namespace android
