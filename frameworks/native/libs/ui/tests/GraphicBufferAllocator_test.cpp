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

#define LOG_TAG "GraphicBufferAllocatorTest"

#include <ui/GraphicBuffer.h>
#include <ui/GraphicBufferAllocator.h>
#include <ui/PixelFormat.h>

#include <gtest/gtest.h>

#include "mock/MockGrallocAllocator.h"

#include <algorithm>
#include <limits>

namespace android {

namespace {

constexpr uint32_t kTestWidth = 1024;
constexpr uint32_t kTestHeight = 1;
constexpr uint32_t kTestLayerCount = 1;
constexpr uint64_t kTestUsage = GraphicBuffer::USAGE_SW_WRITE_OFTEN;

} // namespace

using ::testing::DoAll;
using ::testing::Return;
using ::testing::SetArgPointee;

class TestableGraphicBufferAllocator : public GraphicBufferAllocator {
public:
    TestableGraphicBufferAllocator() {
        mAllocator = std::make_unique<const mock::MockGrallocAllocator>();
    }
    void setUpAllocateExpectations(status_t err, uint32_t stride) {
        std::cout << "Setting expected stride to " << stride << std::endl;
        EXPECT_CALL(*(reinterpret_cast<const mock::MockGrallocAllocator*>(mAllocator.get())),
                    allocate)
                .WillOnce(DoAll(SetArgPointee<6>(stride), Return(err)));
    }
    std::unique_ptr<const GrallocAllocator>& getAllocator() { return mAllocator; }
};

class GraphicBufferAllocatorTest : public testing::Test {
public:
    GraphicBufferAllocatorTest() : mAllocator() {}
    const TestableGraphicBufferAllocator& getAllocator() { return mAllocator; }

protected:
    TestableGraphicBufferAllocator mAllocator;
};

TEST_F(GraphicBufferAllocatorTest, AllocateNoError) {
    mAllocator.setUpAllocateExpectations(NO_ERROR, kTestWidth);
    android::PixelFormat format = PIXEL_FORMAT_RGBA_8888;
    uint32_t stride = 0;
    buffer_handle_t handle;
    status_t err = mAllocator.allocate(kTestWidth, kTestHeight, format, kTestLayerCount, kTestUsage,
                                       &handle, &stride, 0, "GraphicBufferAllocatorTest");
    ASSERT_EQ(NO_ERROR, err);
    ASSERT_EQ(kTestWidth, stride);
}

TEST_F(GraphicBufferAllocatorTest, AllocateZeroStride) {
    android::PixelFormat format = PIXEL_FORMAT_RGBA_8888;
    uint32_t expectedStride = 0;

    mAllocator.setUpAllocateExpectations(NO_ERROR, expectedStride);
    uint32_t stride = 0;
    buffer_handle_t handle;
    // a divide by zero would cause a crash
    status_t err = mAllocator.allocate(kTestWidth, kTestHeight, format, kTestLayerCount, kTestUsage,
                                       &handle, &stride, 0, "GraphicBufferAllocatorTest");
    ASSERT_EQ(NO_ERROR, err);
    ASSERT_EQ(expectedStride, stride);
}

TEST_F(GraphicBufferAllocatorTest, AllocateLargeStride) {
    uint32_t height = std::numeric_limits<uint32_t>::max();
    uint32_t bpp = 4;
    android::PixelFormat format = PIXEL_FORMAT_RGBA_8888;

    if (std::numeric_limits<size_t>::max() / height / bpp >= std::numeric_limits<uint32_t>::max()) {
        std::cout << "stride cannot cause overflow" << std::endl;
        GTEST_SUCCEED() << "stride cannot cause overflow";
        return;
    }
    uint32_t width = std::numeric_limits<size_t>::max() / height / bpp;

    uint32_t expectedStride = std::numeric_limits<uint32_t>::max();

    mAllocator.setUpAllocateExpectations(NO_ERROR, expectedStride);
    uint32_t stride = 0;
    buffer_handle_t handle;
    // an overflow would cause a crash
    status_t err = mAllocator.allocate(width, height, format, kTestLayerCount, kTestUsage, &handle,
                                       &stride, 0, "GraphicBufferAllocatorTest");
    ASSERT_EQ(NO_ERROR, err);
    ASSERT_EQ(expectedStride, stride);
}
} // namespace android
