/*
 * Copyright (C) 2012 The Android Open Source Project
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

#define LOG_TAG "BufferQueue_test"
//#define LOG_NDEBUG 0

#include "DummyConsumer.h"

#include <gui/BufferItem.h>
#include <gui/BufferQueue.h>
#include <gui/IProducerListener.h>

#include <ui/GraphicBuffer.h>

#include <binder/IPCThreadState.h>
#include <binder/IServiceManager.h>
#include <binder/ProcessState.h>

#include <utils/String8.h>
#include <utils/threads.h>

#include <system/window.h>

#include <gtest/gtest.h>

#include <thread>

using namespace std::chrono_literals;

namespace android {

class BufferQueueTest : public ::testing::Test {

public:
protected:
    BufferQueueTest() {
        const ::testing::TestInfo* const testInfo =
            ::testing::UnitTest::GetInstance()->current_test_info();
        ALOGV("Begin test: %s.%s", testInfo->test_case_name(),
                testInfo->name());
    }

    ~BufferQueueTest() {
        const ::testing::TestInfo* const testInfo =
            ::testing::UnitTest::GetInstance()->current_test_info();
        ALOGV("End test:   %s.%s", testInfo->test_case_name(),
                testInfo->name());
    }

    void GetMinUndequeuedBufferCount(int* bufferCount) {
        ASSERT_TRUE(bufferCount != nullptr);
        ASSERT_EQ(OK, mProducer->query(NATIVE_WINDOW_MIN_UNDEQUEUED_BUFFERS,
                    bufferCount));
        ASSERT_GE(*bufferCount, 0);
    }

    void createBufferQueue() {
        BufferQueue::createBufferQueue(&mProducer, &mConsumer);
    }

    void testBufferItem(const IGraphicBufferProducer::QueueBufferInput& input,
            const BufferItem& item) {
        int64_t timestamp;
        bool isAutoTimestamp;
        android_dataspace dataSpace;
        Rect crop;
        int scalingMode;
        uint32_t transform;
        sp<Fence> fence;

        input.deflate(&timestamp, &isAutoTimestamp, &dataSpace, &crop,
                &scalingMode, &transform, &fence, nullptr);
        ASSERT_EQ(timestamp, item.mTimestamp);
        ASSERT_EQ(isAutoTimestamp, item.mIsAutoTimestamp);
        ASSERT_EQ(dataSpace, item.mDataSpace);
        ASSERT_EQ(crop, item.mCrop);
        ASSERT_EQ(static_cast<uint32_t>(scalingMode), item.mScalingMode);
        ASSERT_EQ(transform, item.mTransform);
        ASSERT_EQ(fence, item.mFence);
    }

    sp<IGraphicBufferProducer> mProducer;
    sp<IGraphicBufferConsumer> mConsumer;
};

static const uint32_t TEST_DATA = 0x12345678u;

// XXX: Tests that fork a process to hold the BufferQueue must run before tests
// that use a local BufferQueue, or else Binder will get unhappy
//
// In one instance this was a crash in the createBufferQueue where the
// binder call to create a buffer allocator apparently got garbage back.
// See b/36592665.
TEST_F(BufferQueueTest, DISABLED_BufferQueueInAnotherProcess) {
    const String16 PRODUCER_NAME = String16("BQTestProducer");
    const String16 CONSUMER_NAME = String16("BQTestConsumer");

    pid_t forkPid = fork();
    ASSERT_NE(forkPid, -1);

    if (forkPid == 0) {
        // Child process
        sp<IGraphicBufferProducer> producer;
        sp<IGraphicBufferConsumer> consumer;
        BufferQueue::createBufferQueue(&producer, &consumer);
        sp<IServiceManager> serviceManager = defaultServiceManager();
        serviceManager->addService(PRODUCER_NAME, IInterface::asBinder(producer));
        serviceManager->addService(CONSUMER_NAME, IInterface::asBinder(consumer));
        ProcessState::self()->startThreadPool();
        IPCThreadState::self()->joinThreadPool();
        LOG_ALWAYS_FATAL("Shouldn't be here");
    }

    sp<IServiceManager> serviceManager = defaultServiceManager();
    sp<IBinder> binderProducer =
        serviceManager->getService(PRODUCER_NAME);
    mProducer = interface_cast<IGraphicBufferProducer>(binderProducer);
    EXPECT_TRUE(mProducer != nullptr);
    sp<IBinder> binderConsumer =
        serviceManager->getService(CONSUMER_NAME);
    mConsumer = interface_cast<IGraphicBufferConsumer>(binderConsumer);
    EXPECT_TRUE(mConsumer != nullptr);

    sp<DummyConsumer> dc(new DummyConsumer);
    ASSERT_EQ(OK, mConsumer->consumerConnect(dc, false));
    IGraphicBufferProducer::QueueBufferOutput output;
    ASSERT_EQ(OK,
            mProducer->connect(nullptr, NATIVE_WINDOW_API_CPU, false, &output));

    int slot;
    sp<Fence> fence;
    sp<GraphicBuffer> buffer;
    ASSERT_EQ(IGraphicBufferProducer::BUFFER_NEEDS_REALLOCATION,
              mProducer->dequeueBuffer(&slot, &fence, 0, 0, 0, GRALLOC_USAGE_SW_WRITE_OFTEN,
                                       nullptr, nullptr));
    ASSERT_EQ(OK, mProducer->requestBuffer(slot, &buffer));

    uint32_t* dataIn;
    ASSERT_EQ(OK, buffer->lock(GraphicBuffer::USAGE_SW_WRITE_OFTEN,
            reinterpret_cast<void**>(&dataIn)));
    *dataIn = TEST_DATA;
    ASSERT_EQ(OK, buffer->unlock());

    IGraphicBufferProducer::QueueBufferInput input(0, false,
            HAL_DATASPACE_UNKNOWN, Rect(0, 0, 1, 1),
            NATIVE_WINDOW_SCALING_MODE_FREEZE, 0, Fence::NO_FENCE);
    ASSERT_EQ(OK, mProducer->queueBuffer(slot, input, &output));

    BufferItem item;
    ASSERT_EQ(OK, mConsumer->acquireBuffer(&item, 0));

    uint32_t* dataOut;
    ASSERT_EQ(OK, item.mGraphicBuffer->lock(GraphicBuffer::USAGE_SW_READ_OFTEN,
            reinterpret_cast<void**>(&dataOut)));
    ASSERT_EQ(*dataOut, TEST_DATA);
    ASSERT_EQ(OK, item.mGraphicBuffer->unlock());
}

TEST_F(BufferQueueTest, AcquireBuffer_ExceedsMaxAcquireCount_Fails) {
    createBufferQueue();
    sp<DummyConsumer> dc(new DummyConsumer);
    mConsumer->consumerConnect(dc, false);
    IGraphicBufferProducer::QueueBufferOutput qbo;
    mProducer->connect(new DummyProducerListener, NATIVE_WINDOW_API_CPU, false,
            &qbo);
    mProducer->setMaxDequeuedBufferCount(3);

    int slot;
    sp<Fence> fence;
    sp<GraphicBuffer> buf;
    IGraphicBufferProducer::QueueBufferInput qbi(0, false,
            HAL_DATASPACE_UNKNOWN, Rect(0, 0, 1, 1),
            NATIVE_WINDOW_SCALING_MODE_FREEZE, 0, Fence::NO_FENCE);
    BufferItem item;

    for (int i = 0; i < 2; i++) {
        ASSERT_EQ(IGraphicBufferProducer::BUFFER_NEEDS_REALLOCATION,
                  mProducer->dequeueBuffer(&slot, &fence, 1, 1, 0, GRALLOC_USAGE_SW_READ_OFTEN,
                                           nullptr, nullptr));
        ASSERT_EQ(OK, mProducer->requestBuffer(slot, &buf));
        ASSERT_EQ(OK, mProducer->queueBuffer(slot, qbi, &qbo));
        ASSERT_EQ(OK, mConsumer->acquireBuffer(&item, 0));
    }

    ASSERT_EQ(IGraphicBufferProducer::BUFFER_NEEDS_REALLOCATION,
              mProducer->dequeueBuffer(&slot, &fence, 1, 1, 0, GRALLOC_USAGE_SW_READ_OFTEN,
                                       nullptr, nullptr));
    ASSERT_EQ(OK, mProducer->requestBuffer(slot, &buf));
    ASSERT_EQ(OK, mProducer->queueBuffer(slot, qbi, &qbo));

    // Acquire the third buffer, which should fail.
    ASSERT_EQ(INVALID_OPERATION, mConsumer->acquireBuffer(&item, 0));
}

TEST_F(BufferQueueTest, SetMaxAcquiredBufferCountWithIllegalValues_ReturnsError) {
    createBufferQueue();
    sp<DummyConsumer> dc(new DummyConsumer);
    mConsumer->consumerConnect(dc, false);

    EXPECT_EQ(OK, mConsumer->setMaxBufferCount(10));
    EXPECT_EQ(BAD_VALUE, mConsumer->setMaxAcquiredBufferCount(10));

    IGraphicBufferProducer::QueueBufferOutput qbo;
    mProducer->connect(new DummyProducerListener, NATIVE_WINDOW_API_CPU, false,
            &qbo);
    mProducer->setMaxDequeuedBufferCount(3);

    int minBufferCount;
    ASSERT_NO_FATAL_FAILURE(GetMinUndequeuedBufferCount(&minBufferCount));
    EXPECT_EQ(BAD_VALUE, mConsumer->setMaxAcquiredBufferCount(
                minBufferCount - 1));

    EXPECT_EQ(BAD_VALUE, mConsumer->setMaxAcquiredBufferCount(0));
    EXPECT_EQ(BAD_VALUE, mConsumer->setMaxAcquiredBufferCount(-3));
    EXPECT_EQ(BAD_VALUE, mConsumer->setMaxAcquiredBufferCount(
            BufferQueue::MAX_MAX_ACQUIRED_BUFFERS+1));
    EXPECT_EQ(BAD_VALUE, mConsumer->setMaxAcquiredBufferCount(100));

    int slot;
    sp<Fence> fence;
    sp<GraphicBuffer> buf;
    IGraphicBufferProducer::QueueBufferInput qbi(0, false,
            HAL_DATASPACE_UNKNOWN, Rect(0, 0, 1, 1),
            NATIVE_WINDOW_SCALING_MODE_FREEZE, 0, Fence::NO_FENCE);
    BufferItem item;
    EXPECT_EQ(OK, mConsumer->setMaxAcquiredBufferCount(3));
    for (int i = 0; i < 3; i++) {
        ASSERT_EQ(IGraphicBufferProducer::BUFFER_NEEDS_REALLOCATION,
                  mProducer->dequeueBuffer(&slot, &fence, 1, 1, 0, GRALLOC_USAGE_SW_READ_OFTEN,
                                           nullptr, nullptr));
        ASSERT_EQ(OK, mProducer->requestBuffer(slot, &buf));
        ASSERT_EQ(OK, mProducer->queueBuffer(slot, qbi, &qbo));
        ASSERT_EQ(OK, mConsumer->acquireBuffer(&item, 0));
    }

    EXPECT_EQ(BAD_VALUE, mConsumer->setMaxAcquiredBufferCount(2));
}

TEST_F(BufferQueueTest, SetMaxAcquiredBufferCountWithLegalValues_Succeeds) {
    createBufferQueue();
    sp<DummyConsumer> dc(new DummyConsumer);
    mConsumer->consumerConnect(dc, false);

    IGraphicBufferProducer::QueueBufferOutput qbo;
    mProducer->connect(new DummyProducerListener, NATIVE_WINDOW_API_CPU, false,
            &qbo);
    mProducer->setMaxDequeuedBufferCount(2);

    int minBufferCount;
    ASSERT_NO_FATAL_FAILURE(GetMinUndequeuedBufferCount(&minBufferCount));

    EXPECT_EQ(OK, mConsumer->setMaxAcquiredBufferCount(1));
    EXPECT_EQ(OK, mConsumer->setMaxAcquiredBufferCount(2));
    EXPECT_EQ(OK, mConsumer->setMaxAcquiredBufferCount(minBufferCount));

    int slot;
    sp<Fence> fence;
    sp<GraphicBuffer> buf;
    IGraphicBufferProducer::QueueBufferInput qbi(0, false,
            HAL_DATASPACE_UNKNOWN, Rect(0, 0, 1, 1),
            NATIVE_WINDOW_SCALING_MODE_FREEZE, 0, Fence::NO_FENCE);
    BufferItem item;

    ASSERT_EQ(IGraphicBufferProducer::BUFFER_NEEDS_REALLOCATION,
              mProducer->dequeueBuffer(&slot, &fence, 1, 1, 0, GRALLOC_USAGE_SW_READ_OFTEN,
                                       nullptr, nullptr));
    ASSERT_EQ(OK, mProducer->requestBuffer(slot, &buf));
    ASSERT_EQ(OK, mProducer->queueBuffer(slot, qbi, &qbo));
    ASSERT_EQ(OK, mConsumer->acquireBuffer(&item, 0));

    EXPECT_EQ(OK, mConsumer->setMaxAcquiredBufferCount(3));

    for (int i = 0; i < 2; i++) {
        ASSERT_EQ(IGraphicBufferProducer::BUFFER_NEEDS_REALLOCATION,
                  mProducer->dequeueBuffer(&slot, &fence, 1, 1, 0, GRALLOC_USAGE_SW_READ_OFTEN,
                                           nullptr, nullptr));
        ASSERT_EQ(OK, mProducer->requestBuffer(slot, &buf));
        ASSERT_EQ(OK, mProducer->queueBuffer(slot, qbi, &qbo));
        ASSERT_EQ(OK, mConsumer->acquireBuffer(&item, 0));
    }

    EXPECT_EQ(OK, mConsumer->setMaxAcquiredBufferCount(
            BufferQueue::MAX_MAX_ACQUIRED_BUFFERS));
}

TEST_F(BufferQueueTest, SetMaxBufferCountWithLegalValues_Succeeds) {
    createBufferQueue();
    sp<DummyConsumer> dc(new DummyConsumer);
    mConsumer->consumerConnect(dc, false);

    // Test shared buffer mode
    EXPECT_EQ(OK, mConsumer->setMaxAcquiredBufferCount(1));
}

TEST_F(BufferQueueTest, SetMaxBufferCountWithIllegalValues_ReturnsError) {
    createBufferQueue();
    sp<DummyConsumer> dc(new DummyConsumer);
    mConsumer->consumerConnect(dc, false);

    EXPECT_EQ(BAD_VALUE, mConsumer->setMaxBufferCount(0));
    EXPECT_EQ(BAD_VALUE, mConsumer->setMaxBufferCount(
            BufferQueue::NUM_BUFFER_SLOTS + 1));

    EXPECT_EQ(OK, mConsumer->setMaxAcquiredBufferCount(5));
    EXPECT_EQ(BAD_VALUE, mConsumer->setMaxBufferCount(3));
}

TEST_F(BufferQueueTest, DetachAndReattachOnProducerSide) {
    createBufferQueue();
    sp<DummyConsumer> dc(new DummyConsumer);
    ASSERT_EQ(OK, mConsumer->consumerConnect(dc, false));
    IGraphicBufferProducer::QueueBufferOutput output;
    ASSERT_EQ(OK, mProducer->connect(new DummyProducerListener,
            NATIVE_WINDOW_API_CPU, false, &output));

    ASSERT_EQ(BAD_VALUE, mProducer->detachBuffer(-1)); // Index too low
    ASSERT_EQ(BAD_VALUE, mProducer->detachBuffer(
                BufferQueueDefs::NUM_BUFFER_SLOTS)); // Index too high
    ASSERT_EQ(BAD_VALUE, mProducer->detachBuffer(0)); // Not dequeued

    int slot;
    sp<Fence> fence;
    sp<GraphicBuffer> buffer;
    ASSERT_EQ(IGraphicBufferProducer::BUFFER_NEEDS_REALLOCATION,
              mProducer->dequeueBuffer(&slot, &fence, 0, 0, 0, GRALLOC_USAGE_SW_WRITE_OFTEN,
                                       nullptr, nullptr));
    ASSERT_EQ(BAD_VALUE, mProducer->detachBuffer(slot)); // Not requested
    ASSERT_EQ(OK, mProducer->requestBuffer(slot, &buffer));
    ASSERT_EQ(OK, mProducer->detachBuffer(slot));
    ASSERT_EQ(BAD_VALUE, mProducer->detachBuffer(slot)); // Not dequeued

    sp<GraphicBuffer> safeToClobberBuffer;
    // Can no longer request buffer from this slot
    ASSERT_EQ(BAD_VALUE, mProducer->requestBuffer(slot, &safeToClobberBuffer));

    uint32_t* dataIn;
    ASSERT_EQ(OK, buffer->lock(GraphicBuffer::USAGE_SW_WRITE_OFTEN,
            reinterpret_cast<void**>(&dataIn)));
    *dataIn = TEST_DATA;
    ASSERT_EQ(OK, buffer->unlock());

    int newSlot;
    ASSERT_EQ(BAD_VALUE, mProducer->attachBuffer(nullptr, safeToClobberBuffer));
    ASSERT_EQ(BAD_VALUE, mProducer->attachBuffer(&newSlot, nullptr));

    ASSERT_EQ(OK, mProducer->attachBuffer(&newSlot, buffer));
    IGraphicBufferProducer::QueueBufferInput input(0, false,
            HAL_DATASPACE_UNKNOWN, Rect(0, 0, 1, 1),
            NATIVE_WINDOW_SCALING_MODE_FREEZE, 0, Fence::NO_FENCE);
    ASSERT_EQ(OK, mProducer->queueBuffer(newSlot, input, &output));

    BufferItem item;
    ASSERT_EQ(OK, mConsumer->acquireBuffer(&item, static_cast<nsecs_t>(0)));

    uint32_t* dataOut;
    ASSERT_EQ(OK, item.mGraphicBuffer->lock(GraphicBuffer::USAGE_SW_READ_OFTEN,
            reinterpret_cast<void**>(&dataOut)));
    ASSERT_EQ(*dataOut, TEST_DATA);
    ASSERT_EQ(OK, item.mGraphicBuffer->unlock());
}

TEST_F(BufferQueueTest, DetachAndReattachOnConsumerSide) {
    createBufferQueue();
    sp<DummyConsumer> dc(new DummyConsumer);
    ASSERT_EQ(OK, mConsumer->consumerConnect(dc, false));
    IGraphicBufferProducer::QueueBufferOutput output;
    ASSERT_EQ(OK, mProducer->connect(new DummyProducerListener,
            NATIVE_WINDOW_API_CPU, false, &output));

    int slot;
    sp<Fence> fence;
    sp<GraphicBuffer> buffer;
    ASSERT_EQ(IGraphicBufferProducer::BUFFER_NEEDS_REALLOCATION,
              mProducer->dequeueBuffer(&slot, &fence, 0, 0, 0, GRALLOC_USAGE_SW_WRITE_OFTEN,
                                       nullptr, nullptr));
    ASSERT_EQ(OK, mProducer->requestBuffer(slot, &buffer));
    IGraphicBufferProducer::QueueBufferInput input(0, false,
            HAL_DATASPACE_UNKNOWN, Rect(0, 0, 1, 1),
            NATIVE_WINDOW_SCALING_MODE_FREEZE, 0, Fence::NO_FENCE);
    ASSERT_EQ(OK, mProducer->queueBuffer(slot, input, &output));

    ASSERT_EQ(BAD_VALUE, mConsumer->detachBuffer(-1)); // Index too low
    ASSERT_EQ(BAD_VALUE, mConsumer->detachBuffer(
            BufferQueueDefs::NUM_BUFFER_SLOTS)); // Index too high
    ASSERT_EQ(BAD_VALUE, mConsumer->detachBuffer(0)); // Not acquired

    BufferItem item;
    ASSERT_EQ(OK, mConsumer->acquireBuffer(&item, static_cast<nsecs_t>(0)));

    ASSERT_EQ(OK, mConsumer->detachBuffer(item.mSlot));
    ASSERT_EQ(BAD_VALUE, mConsumer->detachBuffer(item.mSlot)); // Not acquired

    uint32_t* dataIn;
    ASSERT_EQ(OK, item.mGraphicBuffer->lock(
            GraphicBuffer::USAGE_SW_WRITE_OFTEN,
            reinterpret_cast<void**>(&dataIn)));
    *dataIn = TEST_DATA;
    ASSERT_EQ(OK, item.mGraphicBuffer->unlock());

    int newSlot;
    sp<GraphicBuffer> safeToClobberBuffer;
    ASSERT_EQ(BAD_VALUE, mConsumer->attachBuffer(nullptr, safeToClobberBuffer));
    ASSERT_EQ(BAD_VALUE, mConsumer->attachBuffer(&newSlot, nullptr));
    ASSERT_EQ(OK, mConsumer->attachBuffer(&newSlot, item.mGraphicBuffer));

    ASSERT_EQ(OK, mConsumer->releaseBuffer(newSlot, 0, EGL_NO_DISPLAY,
            EGL_NO_SYNC_KHR, Fence::NO_FENCE));

    ASSERT_EQ(IGraphicBufferProducer::BUFFER_NEEDS_REALLOCATION,
              mProducer->dequeueBuffer(&slot, &fence, 0, 0, 0, GRALLOC_USAGE_SW_WRITE_OFTEN,
                                       nullptr, nullptr));
    ASSERT_EQ(OK, mProducer->requestBuffer(slot, &buffer));

    uint32_t* dataOut;
    ASSERT_EQ(OK, buffer->lock(GraphicBuffer::USAGE_SW_READ_OFTEN,
            reinterpret_cast<void**>(&dataOut)));
    ASSERT_EQ(*dataOut, TEST_DATA);
    ASSERT_EQ(OK, buffer->unlock());
}

TEST_F(BufferQueueTest, MoveFromConsumerToProducer) {
    createBufferQueue();
    sp<DummyConsumer> dc(new DummyConsumer);
    ASSERT_EQ(OK, mConsumer->consumerConnect(dc, false));
    IGraphicBufferProducer::QueueBufferOutput output;
    ASSERT_EQ(OK, mProducer->connect(new DummyProducerListener,
            NATIVE_WINDOW_API_CPU, false, &output));

    int slot;
    sp<Fence> fence;
    sp<GraphicBuffer> buffer;
    ASSERT_EQ(IGraphicBufferProducer::BUFFER_NEEDS_REALLOCATION,
              mProducer->dequeueBuffer(&slot, &fence, 0, 0, 0, GRALLOC_USAGE_SW_WRITE_OFTEN,
                                       nullptr, nullptr));
    ASSERT_EQ(OK, mProducer->requestBuffer(slot, &buffer));

    uint32_t* dataIn;
    ASSERT_EQ(OK, buffer->lock(GraphicBuffer::USAGE_SW_WRITE_OFTEN,
            reinterpret_cast<void**>(&dataIn)));
    *dataIn = TEST_DATA;
    ASSERT_EQ(OK, buffer->unlock());

    IGraphicBufferProducer::QueueBufferInput input(0, false,
            HAL_DATASPACE_UNKNOWN, Rect(0, 0, 1, 1),
            NATIVE_WINDOW_SCALING_MODE_FREEZE, 0, Fence::NO_FENCE);
    ASSERT_EQ(OK, mProducer->queueBuffer(slot, input, &output));

    BufferItem item;
    ASSERT_EQ(OK, mConsumer->acquireBuffer(&item, static_cast<nsecs_t>(0)));
    ASSERT_EQ(OK, mConsumer->detachBuffer(item.mSlot));

    int newSlot;
    ASSERT_EQ(OK, mProducer->attachBuffer(&newSlot, item.mGraphicBuffer));
    ASSERT_EQ(OK, mProducer->queueBuffer(newSlot, input, &output));
    ASSERT_EQ(OK, mConsumer->acquireBuffer(&item, static_cast<nsecs_t>(0)));

    uint32_t* dataOut;
    ASSERT_EQ(OK, item.mGraphicBuffer->lock(GraphicBuffer::USAGE_SW_READ_OFTEN,
            reinterpret_cast<void**>(&dataOut)));
    ASSERT_EQ(*dataOut, TEST_DATA);
    ASSERT_EQ(OK, item.mGraphicBuffer->unlock());
}

TEST_F(BufferQueueTest, TestDisallowingAllocation) {
    createBufferQueue();
    sp<DummyConsumer> dc(new DummyConsumer);
    ASSERT_EQ(OK, mConsumer->consumerConnect(dc, true));
    IGraphicBufferProducer::QueueBufferOutput output;
    ASSERT_EQ(OK, mProducer->connect(new DummyProducerListener,
            NATIVE_WINDOW_API_CPU, true, &output));

    static const uint32_t WIDTH = 320;
    static const uint32_t HEIGHT = 240;

    ASSERT_EQ(OK, mConsumer->setDefaultBufferSize(WIDTH, HEIGHT));

    int slot;
    sp<Fence> fence;
    sp<GraphicBuffer> buffer;
    // This should return an error since it would require an allocation
    ASSERT_EQ(OK, mProducer->allowAllocation(false));
    ASSERT_EQ(WOULD_BLOCK,
              mProducer->dequeueBuffer(&slot, &fence, 0, 0, 0, GRALLOC_USAGE_SW_WRITE_OFTEN,
                                       nullptr, nullptr));

    // This should succeed, now that we've lifted the prohibition
    ASSERT_EQ(OK, mProducer->allowAllocation(true));
    ASSERT_EQ(IGraphicBufferProducer::BUFFER_NEEDS_REALLOCATION,
              mProducer->dequeueBuffer(&slot, &fence, 0, 0, 0, GRALLOC_USAGE_SW_WRITE_OFTEN,
                                       nullptr, nullptr));

    // Release the previous buffer back to the BufferQueue
    mProducer->cancelBuffer(slot, fence);

    // This should fail since we're requesting a different size
    ASSERT_EQ(OK, mProducer->allowAllocation(false));
    ASSERT_EQ(WOULD_BLOCK,
              mProducer->dequeueBuffer(&slot, &fence, WIDTH * 2, HEIGHT * 2, 0,
                                       GRALLOC_USAGE_SW_WRITE_OFTEN, nullptr, nullptr));
}

TEST_F(BufferQueueTest, TestGenerationNumbers) {
    createBufferQueue();
    sp<DummyConsumer> dc(new DummyConsumer);
    ASSERT_EQ(OK, mConsumer->consumerConnect(dc, true));
    IGraphicBufferProducer::QueueBufferOutput output;
    ASSERT_EQ(OK, mProducer->connect(new DummyProducerListener,
            NATIVE_WINDOW_API_CPU, true, &output));

    ASSERT_EQ(OK, mProducer->setGenerationNumber(1));

    // Get one buffer to play with
    int slot;
    sp<Fence> fence;
    ASSERT_EQ(IGraphicBufferProducer::BUFFER_NEEDS_REALLOCATION,
              mProducer->dequeueBuffer(&slot, &fence, 0, 0, 0, 0, nullptr, nullptr));

    sp<GraphicBuffer> buffer;
    ASSERT_EQ(OK, mProducer->requestBuffer(slot, &buffer));

    // Ensure that the generation number we set propagates to allocated buffers
    ASSERT_EQ(1U, buffer->getGenerationNumber());

    ASSERT_EQ(OK, mProducer->detachBuffer(slot));

    ASSERT_EQ(OK, mProducer->setGenerationNumber(2));

    // These should fail, since we've changed the generation number on the queue
    int outSlot;
    ASSERT_EQ(BAD_VALUE, mProducer->attachBuffer(&outSlot, buffer));
    ASSERT_EQ(BAD_VALUE, mConsumer->attachBuffer(&outSlot, buffer));

    buffer->setGenerationNumber(2);

    // This should succeed now that we've changed the buffer's generation number
    ASSERT_EQ(OK, mProducer->attachBuffer(&outSlot, buffer));

    ASSERT_EQ(OK, mProducer->detachBuffer(outSlot));

    // This should also succeed with the new generation number
    ASSERT_EQ(OK, mConsumer->attachBuffer(&outSlot, buffer));
}

TEST_F(BufferQueueTest, TestSharedBufferModeWithoutAutoRefresh) {
    createBufferQueue();
    sp<DummyConsumer> dc(new DummyConsumer);
    ASSERT_EQ(OK, mConsumer->consumerConnect(dc, true));
    IGraphicBufferProducer::QueueBufferOutput output;
    ASSERT_EQ(OK, mProducer->connect(new DummyProducerListener,
            NATIVE_WINDOW_API_CPU, true, &output));

    ASSERT_EQ(OK, mProducer->setSharedBufferMode(true));

    // Get a buffer
    int sharedSlot;
    sp<Fence> fence;
    sp<GraphicBuffer> buffer;
    ASSERT_EQ(IGraphicBufferProducer::BUFFER_NEEDS_REALLOCATION,
              mProducer->dequeueBuffer(&sharedSlot, &fence, 0, 0, 0, 0, nullptr, nullptr));
    ASSERT_EQ(OK, mProducer->requestBuffer(sharedSlot, &buffer));

    // Queue the buffer
    IGraphicBufferProducer::QueueBufferInput input(0, false,
            HAL_DATASPACE_UNKNOWN, Rect(0, 0, 1, 1),
            NATIVE_WINDOW_SCALING_MODE_FREEZE, 0, Fence::NO_FENCE);
    ASSERT_EQ(OK, mProducer->queueBuffer(sharedSlot, input, &output));

    // Repeatedly queue and dequeue a buffer from the producer side, it should
    // always return the same one. And we won't run out of buffers because it's
    // always the same one and because async mode gets enabled.
    int slot;
    for (int i = 0; i < 5; i++) {
        ASSERT_EQ(OK, mProducer->dequeueBuffer(&slot, &fence, 0, 0, 0, 0, nullptr, nullptr));
        ASSERT_EQ(sharedSlot, slot);
        ASSERT_EQ(OK, mProducer->queueBuffer(sharedSlot, input, &output));
    }

    // acquire the buffer
    BufferItem item;
    ASSERT_EQ(OK, mConsumer->acquireBuffer(&item, 0));
    ASSERT_EQ(sharedSlot, item.mSlot);
    testBufferItem(input, item);
    ASSERT_EQ(true, item.mQueuedBuffer);
    ASSERT_EQ(false, item.mAutoRefresh);

    ASSERT_EQ(OK, mConsumer->releaseBuffer(item.mSlot, item.mFrameNumber,
            EGL_NO_DISPLAY, EGL_NO_SYNC_KHR, Fence::NO_FENCE));

    // attempt to acquire a second time should return no buffer available
    ASSERT_EQ(IGraphicBufferConsumer::NO_BUFFER_AVAILABLE,
            mConsumer->acquireBuffer(&item, 0));
}

TEST_F(BufferQueueTest, TestSharedBufferModeWithAutoRefresh) {
    createBufferQueue();
    sp<DummyConsumer> dc(new DummyConsumer);
    ASSERT_EQ(OK, mConsumer->consumerConnect(dc, true));
    IGraphicBufferProducer::QueueBufferOutput output;
    ASSERT_EQ(OK, mProducer->connect(new DummyProducerListener,
            NATIVE_WINDOW_API_CPU, true, &output));

    ASSERT_EQ(OK, mProducer->setSharedBufferMode(true));
    ASSERT_EQ(OK, mProducer->setAutoRefresh(true));

    // Get a buffer
    int sharedSlot;
    sp<Fence> fence;
    sp<GraphicBuffer> buffer;
    ASSERT_EQ(IGraphicBufferProducer::BUFFER_NEEDS_REALLOCATION,
              mProducer->dequeueBuffer(&sharedSlot, &fence, 0, 0, 0, 0, nullptr, nullptr));
    ASSERT_EQ(OK, mProducer->requestBuffer(sharedSlot, &buffer));

    // Queue the buffer
    IGraphicBufferProducer::QueueBufferInput input(0, false,
            HAL_DATASPACE_UNKNOWN, Rect(0, 0, 1, 1),
            NATIVE_WINDOW_SCALING_MODE_FREEZE, 0, Fence::NO_FENCE);
    ASSERT_EQ(OK, mProducer->queueBuffer(sharedSlot, input, &output));

    // Repeatedly acquire and release a buffer from the consumer side, it should
    // always return the same one.
    BufferItem item;
    for (int i = 0; i < 5; i++) {
        ASSERT_EQ(OK, mConsumer->acquireBuffer(&item, 0));
        ASSERT_EQ(sharedSlot, item.mSlot);
        testBufferItem(input, item);
        ASSERT_EQ(i == 0, item.mQueuedBuffer);
        ASSERT_EQ(true, item.mAutoRefresh);

        ASSERT_EQ(OK, mConsumer->releaseBuffer(item.mSlot, item.mFrameNumber,
                EGL_NO_DISPLAY, EGL_NO_SYNC_KHR, Fence::NO_FENCE));
    }

    // Repeatedly queue and dequeue a buffer from the producer side, it should
    // always return the same one.
    int slot;
    for (int i = 0; i < 5; i++) {
        ASSERT_EQ(OK, mProducer->dequeueBuffer(&slot, &fence, 0, 0, 0, 0, nullptr, nullptr));
        ASSERT_EQ(sharedSlot, slot);
        ASSERT_EQ(OK, mProducer->queueBuffer(sharedSlot, input, &output));
    }

    // Repeatedly acquire and release a buffer from the consumer side, it should
    // always return the same one. First grabbing them from the queue and then
    // when the queue is empty, returning the shared buffer.
    for (int i = 0; i < 10; i++) {
        ASSERT_EQ(OK, mConsumer->acquireBuffer(&item, 0));
        ASSERT_EQ(sharedSlot, item.mSlot);
        ASSERT_EQ(0, item.mTimestamp);
        ASSERT_EQ(false, item.mIsAutoTimestamp);
        ASSERT_EQ(HAL_DATASPACE_UNKNOWN, item.mDataSpace);
        ASSERT_EQ(Rect(0, 0, 1, 1), item.mCrop);
        ASSERT_EQ(NATIVE_WINDOW_SCALING_MODE_FREEZE, item.mScalingMode);
        ASSERT_EQ(0u, item.mTransform);
        ASSERT_EQ(Fence::NO_FENCE, item.mFence);
        ASSERT_EQ(i == 0, item.mQueuedBuffer);
        ASSERT_EQ(true, item.mAutoRefresh);

        ASSERT_EQ(OK, mConsumer->releaseBuffer(item.mSlot, item.mFrameNumber,
                EGL_NO_DISPLAY, EGL_NO_SYNC_KHR, Fence::NO_FENCE));
    }
}

TEST_F(BufferQueueTest, TestSharedBufferModeUsingAlreadyDequeuedBuffer) {
    createBufferQueue();
    sp<DummyConsumer> dc(new DummyConsumer);
    ASSERT_EQ(OK, mConsumer->consumerConnect(dc, true));
    IGraphicBufferProducer::QueueBufferOutput output;
    ASSERT_EQ(OK, mProducer->connect(new DummyProducerListener,
            NATIVE_WINDOW_API_CPU, true, &output));

    // Dequeue a buffer
    int sharedSlot;
    sp<Fence> fence;
    sp<GraphicBuffer> buffer;
    ASSERT_EQ(IGraphicBufferProducer::BUFFER_NEEDS_REALLOCATION,
              mProducer->dequeueBuffer(&sharedSlot, &fence, 0, 0, 0, 0, nullptr, nullptr));
    ASSERT_EQ(OK, mProducer->requestBuffer(sharedSlot, &buffer));

    // Enable shared buffer mode
    ASSERT_EQ(OK, mProducer->setSharedBufferMode(true));

    // Queue the buffer
    IGraphicBufferProducer::QueueBufferInput input(0, false,
            HAL_DATASPACE_UNKNOWN, Rect(0, 0, 1, 1),
            NATIVE_WINDOW_SCALING_MODE_FREEZE, 0, Fence::NO_FENCE);
    ASSERT_EQ(OK, mProducer->queueBuffer(sharedSlot, input, &output));

    // Repeatedly queue and dequeue a buffer from the producer side, it should
    // always return the same one. And we won't run out of buffers because it's
    // always the same one and because async mode gets enabled.
    int slot;
    for (int i = 0; i < 5; i++) {
        ASSERT_EQ(OK, mProducer->dequeueBuffer(&slot, &fence, 0, 0, 0, 0, nullptr, nullptr));
        ASSERT_EQ(sharedSlot, slot);
        ASSERT_EQ(OK, mProducer->queueBuffer(sharedSlot, input, &output));
    }

    // acquire the buffer
    BufferItem item;
    ASSERT_EQ(OK, mConsumer->acquireBuffer(&item, 0));
    ASSERT_EQ(sharedSlot, item.mSlot);
    testBufferItem(input, item);
    ASSERT_EQ(true, item.mQueuedBuffer);
    ASSERT_EQ(false, item.mAutoRefresh);

    ASSERT_EQ(OK, mConsumer->releaseBuffer(item.mSlot, item.mFrameNumber,
            EGL_NO_DISPLAY, EGL_NO_SYNC_KHR, Fence::NO_FENCE));

    // attempt to acquire a second time should return no buffer available
    ASSERT_EQ(IGraphicBufferConsumer::NO_BUFFER_AVAILABLE,
            mConsumer->acquireBuffer(&item, 0));
}

TEST_F(BufferQueueTest, TestTimeouts) {
    createBufferQueue();
    sp<DummyConsumer> dc(new DummyConsumer);
    ASSERT_EQ(OK, mConsumer->consumerConnect(dc, true));
    IGraphicBufferProducer::QueueBufferOutput output;
    ASSERT_EQ(OK, mProducer->connect(new DummyProducerListener,
            NATIVE_WINDOW_API_CPU, true, &output));

    // Fill up the queue. Since the controlledByApp flags are set to true, this
    // queue should be in non-blocking mode, and we should be recycling the same
    // two buffers
    for (int i = 0; i < 5; ++i) {
        int slot = BufferQueue::INVALID_BUFFER_SLOT;
        sp<Fence> fence = Fence::NO_FENCE;
        auto result = mProducer->dequeueBuffer(&slot, &fence, 0, 0, 0, 0, nullptr, nullptr);
        if (i < 2) {
            ASSERT_EQ(IGraphicBufferProducer::BUFFER_NEEDS_REALLOCATION,
                    result);
        } else {
            ASSERT_EQ(OK, result);
        }
        sp<GraphicBuffer> buffer;
        ASSERT_EQ(OK, mProducer->requestBuffer(slot, &buffer));
        IGraphicBufferProducer::QueueBufferInput input(0ull, true,
                HAL_DATASPACE_UNKNOWN, Rect::INVALID_RECT,
                NATIVE_WINDOW_SCALING_MODE_FREEZE, 0, Fence::NO_FENCE);
        IGraphicBufferProducer::QueueBufferOutput output{};
        ASSERT_EQ(OK, mProducer->queueBuffer(slot, input, &output));
    }

    const auto TIMEOUT = ms2ns(250);
    mProducer->setDequeueTimeout(TIMEOUT);

    // Setting a timeout will change the BufferQueue into blocking mode (with
    // one droppable buffer in the queue and one free from the previous
    // dequeue/queues), so dequeue and queue two more buffers: one to replace
    // the current droppable buffer, and a second to max out the buffer count
    sp<GraphicBuffer> buffer; // Save a buffer to attach later
    for (int i = 0; i < 2; ++i) {
        int slot = BufferQueue::INVALID_BUFFER_SLOT;
        sp<Fence> fence = Fence::NO_FENCE;
        ASSERT_EQ(OK, mProducer->dequeueBuffer(&slot, &fence, 0, 0, 0, 0, nullptr, nullptr));
        ASSERT_EQ(OK, mProducer->requestBuffer(slot, &buffer));
        IGraphicBufferProducer::QueueBufferInput input(0ull, true,
                HAL_DATASPACE_UNKNOWN, Rect::INVALID_RECT,
                NATIVE_WINDOW_SCALING_MODE_FREEZE, 0, Fence::NO_FENCE);
        ASSERT_EQ(OK, mProducer->queueBuffer(slot, input, &output));
    }

    int slot = BufferQueue::INVALID_BUFFER_SLOT;
    sp<Fence> fence = Fence::NO_FENCE;
    auto startTime = systemTime();
    ASSERT_EQ(TIMED_OUT, mProducer->dequeueBuffer(&slot, &fence, 0, 0, 0, 0, nullptr, nullptr));
    ASSERT_GE(systemTime() - startTime, TIMEOUT);

    // We're technically attaching the same buffer multiple times (since we
    // queued it previously), but that doesn't matter for this test
    startTime = systemTime();
    ASSERT_EQ(TIMED_OUT, mProducer->attachBuffer(&slot, buffer));
    ASSERT_GE(systemTime() - startTime, TIMEOUT);
}

TEST_F(BufferQueueTest, CanAttachWhileDisallowingAllocation) {
    createBufferQueue();
    sp<DummyConsumer> dc(new DummyConsumer);
    ASSERT_EQ(OK, mConsumer->consumerConnect(dc, true));
    IGraphicBufferProducer::QueueBufferOutput output;
    ASSERT_EQ(OK, mProducer->connect(new DummyProducerListener,
            NATIVE_WINDOW_API_CPU, true, &output));

    int slot = BufferQueue::INVALID_BUFFER_SLOT;
    sp<Fence> sourceFence;
    ASSERT_EQ(IGraphicBufferProducer::BUFFER_NEEDS_REALLOCATION,
              mProducer->dequeueBuffer(&slot, &sourceFence, 0, 0, 0, 0, nullptr, nullptr));
    sp<GraphicBuffer> buffer;
    ASSERT_EQ(OK, mProducer->requestBuffer(slot, &buffer));
    ASSERT_EQ(OK, mProducer->detachBuffer(slot));

    ASSERT_EQ(OK, mProducer->allowAllocation(false));

    slot = BufferQueue::INVALID_BUFFER_SLOT;
    ASSERT_EQ(OK, mProducer->attachBuffer(&slot, buffer));
}

TEST_F(BufferQueueTest, CanRetrieveLastQueuedBuffer) {
    createBufferQueue();
    sp<DummyConsumer> dc(new DummyConsumer);
    ASSERT_EQ(OK, mConsumer->consumerConnect(dc, false));
    IGraphicBufferProducer::QueueBufferOutput output;
    ASSERT_EQ(OK, mProducer->connect(new DummyProducerListener,
            NATIVE_WINDOW_API_CPU, false, &output));

    // Dequeue and queue the first buffer, storing the handle
    int slot = BufferQueue::INVALID_BUFFER_SLOT;
    sp<Fence> fence;
    ASSERT_EQ(IGraphicBufferProducer::BUFFER_NEEDS_REALLOCATION,
              mProducer->dequeueBuffer(&slot, &fence, 0, 0, 0, 0, nullptr, nullptr));
    sp<GraphicBuffer> firstBuffer;
    ASSERT_EQ(OK, mProducer->requestBuffer(slot, &firstBuffer));

    IGraphicBufferProducer::QueueBufferInput input(0ull, true,
        HAL_DATASPACE_UNKNOWN, Rect::INVALID_RECT,
        NATIVE_WINDOW_SCALING_MODE_FREEZE, 0, Fence::NO_FENCE);
    ASSERT_EQ(OK, mProducer->queueBuffer(slot, input, &output));

    // Dequeue a second buffer
    slot = BufferQueue::INVALID_BUFFER_SLOT;
    ASSERT_EQ(IGraphicBufferProducer::BUFFER_NEEDS_REALLOCATION,
              mProducer->dequeueBuffer(&slot, &fence, 0, 0, 0, 0, nullptr, nullptr));
    sp<GraphicBuffer> secondBuffer;
    ASSERT_EQ(OK, mProducer->requestBuffer(slot, &secondBuffer));

    // Ensure it's a new buffer
    ASSERT_NE(firstBuffer->getNativeBuffer()->handle,
            secondBuffer->getNativeBuffer()->handle);

    // Queue the second buffer
    ASSERT_EQ(OK, mProducer->queueBuffer(slot, input, &output));

    // Acquire and release both buffers
    for (size_t i = 0; i < 2; ++i) {
        BufferItem item;
        ASSERT_EQ(OK, mConsumer->acquireBuffer(&item, 0));
        ASSERT_EQ(OK, mConsumer->releaseBuffer(item.mSlot, item.mFrameNumber,
                    EGL_NO_DISPLAY, EGL_NO_SYNC_KHR, Fence::NO_FENCE));
    }

    // Make sure we got the second buffer back
    sp<GraphicBuffer> returnedBuffer;
    sp<Fence> returnedFence;
    float transform[16];
    ASSERT_EQ(OK,
            mProducer->getLastQueuedBuffer(&returnedBuffer, &returnedFence,
            transform));
    ASSERT_EQ(secondBuffer->getNativeBuffer()->handle,
            returnedBuffer->getNativeBuffer()->handle);
}

TEST_F(BufferQueueTest, TestOccupancyHistory) {
    createBufferQueue();
    sp<DummyConsumer> dc(new DummyConsumer);
    ASSERT_EQ(OK, mConsumer->consumerConnect(dc, false));
    IGraphicBufferProducer::QueueBufferOutput output;
    ASSERT_EQ(OK, mProducer->connect(new DummyProducerListener,
            NATIVE_WINDOW_API_CPU, false, &output));

    int slot = BufferQueue::INVALID_BUFFER_SLOT;
    sp<Fence> fence = Fence::NO_FENCE;
    sp<GraphicBuffer> buffer = nullptr;
    IGraphicBufferProducer::QueueBufferInput input(0ull, true,
        HAL_DATASPACE_UNKNOWN, Rect::INVALID_RECT,
        NATIVE_WINDOW_SCALING_MODE_FREEZE, 0, Fence::NO_FENCE);
    BufferItem item{};

    // Preallocate, dequeue, request, and cancel 3 buffers so we don't get
    // BUFFER_NEEDS_REALLOCATION below
    int slots[3] = {};
    mProducer->setMaxDequeuedBufferCount(3);
    for (size_t i = 0; i < 3; ++i) {
        status_t result =
                mProducer->dequeueBuffer(&slots[i], &fence, 0, 0, 0, 0, nullptr, nullptr);
        ASSERT_EQ(IGraphicBufferProducer::BUFFER_NEEDS_REALLOCATION, result);
        ASSERT_EQ(OK, mProducer->requestBuffer(slots[i], &buffer));
    }
    for (size_t i = 0; i < 3; ++i) {
        ASSERT_EQ(OK, mProducer->cancelBuffer(slots[i], Fence::NO_FENCE));
    }

    // Create 3 segments

    // The first segment is a two-buffer segment, so we only put one buffer into
    // the queue at a time
    for (size_t i = 0; i < 5; ++i) {
        ASSERT_EQ(OK, mProducer->dequeueBuffer(&slot, &fence, 0, 0, 0, 0, nullptr, nullptr));
        ASSERT_EQ(OK, mProducer->queueBuffer(slot, input, &output));
        ASSERT_EQ(OK, mConsumer->acquireBuffer(&item, 0));
        ASSERT_EQ(OK, mConsumer->releaseBuffer(item.mSlot, item.mFrameNumber,
                EGL_NO_DISPLAY, EGL_NO_SYNC_KHR, Fence::NO_FENCE));
        std::this_thread::sleep_for(16ms);
    }

    // Sleep between segments
    std::this_thread::sleep_for(500ms);

    // The second segment is a double-buffer segment. It starts the same as the
    // two-buffer segment, but then at the end, we put two buffers in the queue
    // at the same time before draining it.
    for (size_t i = 0; i < 5; ++i) {
        ASSERT_EQ(OK, mProducer->dequeueBuffer(&slot, &fence, 0, 0, 0, 0, nullptr, nullptr));
        ASSERT_EQ(OK, mProducer->queueBuffer(slot, input, &output));
        ASSERT_EQ(OK, mConsumer->acquireBuffer(&item, 0));
        ASSERT_EQ(OK, mConsumer->releaseBuffer(item.mSlot, item.mFrameNumber,
                EGL_NO_DISPLAY, EGL_NO_SYNC_KHR, Fence::NO_FENCE));
        std::this_thread::sleep_for(16ms);
    }
    ASSERT_EQ(OK, mProducer->dequeueBuffer(&slot, &fence, 0, 0, 0, 0, nullptr, nullptr));
    ASSERT_EQ(OK, mProducer->queueBuffer(slot, input, &output));
    ASSERT_EQ(OK, mProducer->dequeueBuffer(&slot, &fence, 0, 0, 0, 0, nullptr, nullptr));
    ASSERT_EQ(OK, mProducer->queueBuffer(slot, input, &output));
    ASSERT_EQ(OK, mConsumer->acquireBuffer(&item, 0));
    ASSERT_EQ(OK, mConsumer->releaseBuffer(item.mSlot, item.mFrameNumber,
            EGL_NO_DISPLAY, EGL_NO_SYNC_KHR, Fence::NO_FENCE));
    std::this_thread::sleep_for(16ms);
    ASSERT_EQ(OK, mConsumer->acquireBuffer(&item, 0));
    ASSERT_EQ(OK, mConsumer->releaseBuffer(item.mSlot, item.mFrameNumber,
            EGL_NO_DISPLAY, EGL_NO_SYNC_KHR, Fence::NO_FENCE));

    // Sleep between segments
    std::this_thread::sleep_for(500ms);

    // The third segment is a triple-buffer segment, so the queue is switching
    // between one buffer and two buffers deep.
    ASSERT_EQ(OK, mProducer->dequeueBuffer(&slot, &fence, 0, 0, 0, 0, nullptr, nullptr));
    ASSERT_EQ(OK, mProducer->queueBuffer(slot, input, &output));
    for (size_t i = 0; i < 5; ++i) {
        ASSERT_EQ(OK, mProducer->dequeueBuffer(&slot, &fence, 0, 0, 0, 0, nullptr, nullptr));
        ASSERT_EQ(OK, mProducer->queueBuffer(slot, input, &output));
        ASSERT_EQ(OK, mConsumer->acquireBuffer(&item, 0));
        ASSERT_EQ(OK, mConsumer->releaseBuffer(item.mSlot, item.mFrameNumber,
                    EGL_NO_DISPLAY, EGL_NO_SYNC_KHR, Fence::NO_FENCE));
        std::this_thread::sleep_for(16ms);
    }
    ASSERT_EQ(OK, mConsumer->acquireBuffer(&item, 0));
    ASSERT_EQ(OK, mConsumer->releaseBuffer(item.mSlot, item.mFrameNumber,
            EGL_NO_DISPLAY, EGL_NO_SYNC_KHR, Fence::NO_FENCE));

    // Now we read the segments
    std::vector<OccupancyTracker::Segment> history;
    ASSERT_EQ(OK, mConsumer->getOccupancyHistory(false, &history));

    // Since we didn't force a flush, we should only get the first two segments
    // (since the third segment hasn't been closed out by the appearance of a
    // new segment yet)
    ASSERT_EQ(2u, history.size());

    // The first segment (which will be history[1], since the newest segment
    // should be at the front of the vector) should be a two-buffer segment,
    // which implies that the occupancy average should be between 0 and 1, and
    // usedThirdBuffer should be false
    const auto& firstSegment = history[1];
    ASSERT_EQ(5u, firstSegment.numFrames);
    ASSERT_LT(0, firstSegment.occupancyAverage);
    ASSERT_GT(1, firstSegment.occupancyAverage);
    ASSERT_EQ(false, firstSegment.usedThirdBuffer);

    // The second segment should be a double-buffered segment, which implies that
    // the occupancy average should be between 0 and 1, but usedThirdBuffer
    // should be true
    const auto& secondSegment = history[0];
    ASSERT_EQ(7u, secondSegment.numFrames);
    ASSERT_LT(0, secondSegment.occupancyAverage);
    ASSERT_GT(1, secondSegment.occupancyAverage);
    ASSERT_EQ(true, secondSegment.usedThirdBuffer);

    // If we read the segments again without flushing, we shouldn't get any new
    // segments
    ASSERT_EQ(OK, mConsumer->getOccupancyHistory(false, &history));
    ASSERT_EQ(0u, history.size());

    // Read the segments again, this time forcing a flush so we get the third
    // segment
    ASSERT_EQ(OK, mConsumer->getOccupancyHistory(true, &history));
    ASSERT_EQ(1u, history.size());

    // This segment should be a triple-buffered segment, which implies that the
    // occupancy average should be between 1 and 2, and usedThirdBuffer should
    // be true
    const auto& thirdSegment = history[0];
    ASSERT_EQ(6u, thirdSegment.numFrames);
    ASSERT_LT(1, thirdSegment.occupancyAverage);
    ASSERT_GT(2, thirdSegment.occupancyAverage);
    ASSERT_EQ(true, thirdSegment.usedThirdBuffer);
}

struct BufferDiscardedListener : public BnProducerListener {
public:
    BufferDiscardedListener() = default;
    virtual ~BufferDiscardedListener() = default;

    virtual void onBufferReleased() {}
    virtual bool needsReleaseNotify() { return false; }
    virtual void onBuffersDiscarded(const std::vector<int32_t>& slots) {
        mDiscardedSlots.insert(mDiscardedSlots.end(), slots.begin(), slots.end());
    }

    const std::vector<int32_t>& getDiscardedSlots() const { return mDiscardedSlots; }
private:
    // No need to use lock given the test triggers the listener in the same
    // thread context.
    std::vector<int32_t> mDiscardedSlots;
};

TEST_F(BufferQueueTest, TestDiscardFreeBuffers) {
    createBufferQueue();
    sp<DummyConsumer> dc(new DummyConsumer);
    ASSERT_EQ(OK, mConsumer->consumerConnect(dc, false));
    IGraphicBufferProducer::QueueBufferOutput output;
    sp<BufferDiscardedListener> pl(new BufferDiscardedListener);
    ASSERT_EQ(OK, mProducer->connect(pl,
            NATIVE_WINDOW_API_CPU, false, &output));

    int slot = BufferQueue::INVALID_BUFFER_SLOT;
    sp<Fence> fence = Fence::NO_FENCE;
    sp<GraphicBuffer> buffer = nullptr;
    IGraphicBufferProducer::QueueBufferInput input(0ull, true,
        HAL_DATASPACE_UNKNOWN, Rect::INVALID_RECT,
        NATIVE_WINDOW_SCALING_MODE_FREEZE, 0, Fence::NO_FENCE);
    BufferItem item{};

    // Preallocate, dequeue, request, and cancel 4 buffers so we don't get
    // BUFFER_NEEDS_REALLOCATION below
    int slots[4] = {};
    mProducer->setMaxDequeuedBufferCount(4);
    for (size_t i = 0; i < 4; ++i) {
        status_t result =
                mProducer->dequeueBuffer(&slots[i], &fence, 0, 0, 0, 0, nullptr, nullptr);
        ASSERT_EQ(IGraphicBufferProducer::BUFFER_NEEDS_REALLOCATION, result);
        ASSERT_EQ(OK, mProducer->requestBuffer(slots[i], &buffer));
    }
    for (size_t i = 0; i < 4; ++i) {
        ASSERT_EQ(OK, mProducer->cancelBuffer(slots[i], Fence::NO_FENCE));
    }

    // Get buffers in all states: dequeued, filled, acquired, free

    // Fill 3 buffers
    ASSERT_EQ(OK, mProducer->dequeueBuffer(&slot, &fence, 0, 0, 0, 0, nullptr, nullptr));
    ASSERT_EQ(OK, mProducer->queueBuffer(slot, input, &output));
    ASSERT_EQ(OK, mProducer->dequeueBuffer(&slot, &fence, 0, 0, 0, 0, nullptr, nullptr));
    ASSERT_EQ(OK, mProducer->queueBuffer(slot, input, &output));
    ASSERT_EQ(OK, mProducer->dequeueBuffer(&slot, &fence, 0, 0, 0, 0, nullptr, nullptr));
    ASSERT_EQ(OK, mProducer->queueBuffer(slot, input, &output));
    // Dequeue 1 buffer
    ASSERT_EQ(OK, mProducer->dequeueBuffer(&slot, &fence, 0, 0, 0, 0, nullptr, nullptr));

    // Acquire and free 1 buffer
    ASSERT_EQ(OK, mConsumer->acquireBuffer(&item, 0));
    ASSERT_EQ(OK, mConsumer->releaseBuffer(item.mSlot, item.mFrameNumber,
                    EGL_NO_DISPLAY, EGL_NO_SYNC_KHR, Fence::NO_FENCE));
    int releasedSlot = item.mSlot;

    // Acquire 1 buffer, leaving 1 filled buffer in queue
    ASSERT_EQ(OK, mConsumer->acquireBuffer(&item, 0));

    // Now discard the free buffers
    ASSERT_EQ(OK, mConsumer->discardFreeBuffers());

    // Check onBuffersDiscarded is called with correct slots
    auto buffersDiscarded = pl->getDiscardedSlots();
    ASSERT_EQ(buffersDiscarded.size(), 1);
    ASSERT_EQ(buffersDiscarded[0], releasedSlot);

    // Check no free buffers in dump
    String8 dumpString;
    mConsumer->dumpState(String8{}, &dumpString);

    // Parse the dump to ensure that all buffer slots that are FREE also
    // have a null GraphicBuffer
    // Fragile - assumes the following format for the dump for a buffer entry:
    // ":%p\][^:]*state=FREE" where %p is the buffer pointer in hex.
    ssize_t idx = dumpString.find("state=FREE");
    while (idx != -1) {
        ssize_t bufferPtrIdx = idx - 1;
        while (bufferPtrIdx > 0) {
            if (dumpString[bufferPtrIdx] == ':') {
                bufferPtrIdx++;
                break;
            }
            bufferPtrIdx--;
        }
        ASSERT_GT(bufferPtrIdx, 0) << "Can't parse queue dump to validate";
        ssize_t nullPtrIdx = dumpString.find("0x0]", bufferPtrIdx);
        ASSERT_EQ(bufferPtrIdx, nullPtrIdx) << "Free buffer not discarded";
        idx = dumpString.find("FREE", idx + 1);
    }
}

TEST_F(BufferQueueTest, TestBufferReplacedInQueueBuffer) {
    createBufferQueue();
    sp<DummyConsumer> dc(new DummyConsumer);
    ASSERT_EQ(OK, mConsumer->consumerConnect(dc, true));
    IGraphicBufferProducer::QueueBufferOutput output;
    ASSERT_EQ(OK, mProducer->connect(new DummyProducerListener,
            NATIVE_WINDOW_API_CPU, true, &output));
    ASSERT_EQ(OK, mConsumer->setMaxAcquiredBufferCount(1));

    int slot = BufferQueue::INVALID_BUFFER_SLOT;
    sp<Fence> fence = Fence::NO_FENCE;
    sp<GraphicBuffer> buffer = nullptr;
    IGraphicBufferProducer::QueueBufferInput input(0ull, true,
        HAL_DATASPACE_UNKNOWN, Rect::INVALID_RECT,
        NATIVE_WINDOW_SCALING_MODE_FREEZE, 0, Fence::NO_FENCE);
    BufferItem item{};

    // Preallocate, dequeue, request, and cancel 2 buffers so we don't get
    // BUFFER_NEEDS_REALLOCATION below
    int slots[2] = {};
    ASSERT_EQ(OK, mProducer->setMaxDequeuedBufferCount(2));
    for (size_t i = 0; i < 2; ++i) {
        status_t result =
                mProducer->dequeueBuffer(&slots[i], &fence, 0, 0, 0, 0, nullptr, nullptr);
        ASSERT_EQ(IGraphicBufferProducer::BUFFER_NEEDS_REALLOCATION, result);
        ASSERT_EQ(OK, mProducer->requestBuffer(slots[i], &buffer));
    }
    for (size_t i = 0; i < 2; ++i) {
        ASSERT_EQ(OK, mProducer->cancelBuffer(slots[i], Fence::NO_FENCE));
    }

    // Fill 2 buffers without consumer consuming them. Verify that all
    // queued buffer returns proper bufferReplaced flag
    ASSERT_EQ(OK, mProducer->dequeueBuffer(&slot, &fence, 0, 0, 0, 0, nullptr, nullptr));
    ASSERT_EQ(OK, mProducer->queueBuffer(slot, input, &output));
    ASSERT_EQ(false, output.bufferReplaced);
    ASSERT_EQ(OK, mProducer->dequeueBuffer(&slot, &fence, 0, 0, 0, 0, nullptr, nullptr));
    ASSERT_EQ(OK, mProducer->queueBuffer(slot, input, &output));
    ASSERT_EQ(true, output.bufferReplaced);
}

TEST_F(BufferQueueTest, TestStaleBufferHandleSentAfterDisconnect) {
    createBufferQueue();
    sp<DummyConsumer> dc(new DummyConsumer);
    ASSERT_EQ(OK, mConsumer->consumerConnect(dc, true));
    IGraphicBufferProducer::QueueBufferOutput output;
    sp<IProducerListener> dummyListener(new DummyProducerListener);
    ASSERT_EQ(OK, mProducer->connect(dummyListener, NATIVE_WINDOW_API_CPU,
            true, &output));

    int slot = BufferQueue::INVALID_BUFFER_SLOT;
    sp<Fence> fence = Fence::NO_FENCE;
    sp<GraphicBuffer> buffer = nullptr;
    IGraphicBufferProducer::QueueBufferInput input(0ull, true,
            HAL_DATASPACE_UNKNOWN, Rect::INVALID_RECT,
            NATIVE_WINDOW_SCALING_MODE_FREEZE, 0, Fence::NO_FENCE);

    // Dequeue, request, and queue one buffer
    status_t result = mProducer->dequeueBuffer(&slot, &fence, 0, 0, 0, 0, nullptr, nullptr);
    ASSERT_EQ(IGraphicBufferProducer::BUFFER_NEEDS_REALLOCATION, result);
    ASSERT_EQ(OK, mProducer->requestBuffer(slot, &buffer));
    ASSERT_EQ(OK, mProducer->queueBuffer(slot, input, &output));

    // Acquire and release the buffer. Upon acquiring, the buffer handle should
    // be non-null since this is the first time we've acquired this slot.
    BufferItem item;
    ASSERT_EQ(OK, mConsumer->acquireBuffer(&item, 0));
    ASSERT_EQ(slot, item.mSlot);
    ASSERT_NE(nullptr, item.mGraphicBuffer.get());
    ASSERT_EQ(OK, mConsumer->releaseBuffer(item.mSlot, item.mFrameNumber,
            EGL_NO_DISPLAY, EGL_NO_SYNC_KHR, Fence::NO_FENCE));

    // Dequeue and queue the buffer again
    ASSERT_EQ(OK, mProducer->dequeueBuffer(&slot, &fence, 0, 0, 0, 0, nullptr, nullptr));
    ASSERT_EQ(OK, mProducer->queueBuffer(slot, input, &output));

    // Acquire and release the buffer again. Upon acquiring, the buffer handle
    // should be null since this is not the first time we've acquired this slot.
    ASSERT_EQ(OK, mConsumer->acquireBuffer(&item, 0));
    ASSERT_EQ(slot, item.mSlot);
    ASSERT_EQ(nullptr, item.mGraphicBuffer.get());
    ASSERT_EQ(OK, mConsumer->releaseBuffer(item.mSlot, item.mFrameNumber,
            EGL_NO_DISPLAY, EGL_NO_SYNC_KHR, Fence::NO_FENCE));

    // Dequeue and queue the buffer again
    ASSERT_EQ(OK, mProducer->dequeueBuffer(&slot, &fence, 0, 0, 0, 0, nullptr, nullptr));
    ASSERT_EQ(OK, mProducer->queueBuffer(slot, input, &output));

    // Disconnect the producer end. This should clear all of the slots and mark
    // the buffer in the queue as stale.
    ASSERT_EQ(OK, mProducer->disconnect(NATIVE_WINDOW_API_CPU));

    // Acquire the buffer again. Upon acquiring, the buffer handle should not be
    // null since the queued buffer should have been marked as stale, which
    // should trigger the BufferQueue to resend the buffer handle.
    ASSERT_EQ(OK, mConsumer->acquireBuffer(&item, 0));
    ASSERT_EQ(slot, item.mSlot);
    ASSERT_NE(nullptr, item.mGraphicBuffer.get());
}

TEST_F(BufferQueueTest, TestProducerConnectDisconnect) {
    createBufferQueue();
    sp<DummyConsumer> dc(new DummyConsumer);
    ASSERT_EQ(OK, mConsumer->consumerConnect(dc, true));
    IGraphicBufferProducer::QueueBufferOutput output;
    sp<IProducerListener> dummyListener(new DummyProducerListener);
    ASSERT_EQ(NO_INIT, mProducer->disconnect(NATIVE_WINDOW_API_CPU));
    ASSERT_EQ(OK, mProducer->connect(
            dummyListener, NATIVE_WINDOW_API_CPU, true, &output));
    ASSERT_EQ(BAD_VALUE, mProducer->connect(
            dummyListener, NATIVE_WINDOW_API_MEDIA, true, &output));

    ASSERT_EQ(BAD_VALUE, mProducer->disconnect(NATIVE_WINDOW_API_MEDIA));
    ASSERT_EQ(OK, mProducer->disconnect(NATIVE_WINDOW_API_CPU));
    ASSERT_EQ(NO_INIT, mProducer->disconnect(NATIVE_WINDOW_API_CPU));
}

} // namespace android
