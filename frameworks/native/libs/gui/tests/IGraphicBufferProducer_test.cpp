/*
 * Copyright (C) 2013 The Android Open Source Project
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

#define LOG_TAG "IGraphicBufferProducer_test"
//#define LOG_NDEBUG 0

#include "DummyConsumer.h"

#include <gtest/gtest.h>

#include <utils/String8.h>
#include <utils/threads.h>

#include <ui/GraphicBuffer.h>

#include <gui/BufferQueue.h>
#include <gui/IProducerListener.h>

#include <system/window.h>

#include <vector>

#define ASSERT_OK(x) ASSERT_EQ(OK, (x))
#define EXPECT_OK(x) EXPECT_EQ(OK, (x))

#define TEST_TOKEN ((IProducerListener*)(NULL))
#define TEST_API NATIVE_WINDOW_API_CPU
#define TEST_API_OTHER NATIVE_WINDOW_API_EGL // valid API that's not TEST_API
#define TEST_CONTROLLED_BY_APP false
#define TEST_PRODUCER_USAGE_BITS (0)

#ifndef USE_BUFFER_HUB_AS_BUFFER_QUEUE
#define USE_BUFFER_HUB_AS_BUFFER_QUEUE 0
#endif

namespace android {

namespace {
    // Default dimensions before setDefaultBufferSize is called
    const uint32_t DEFAULT_WIDTH = 1;
    const uint32_t DEFAULT_HEIGHT = 1;

    // Default format before setDefaultBufferFormat is called
    const PixelFormat DEFAULT_FORMAT = HAL_PIXEL_FORMAT_RGBA_8888;

    // Default transform hint before setTransformHint is called
    const uint32_t DEFAULT_TRANSFORM_HINT = 0;

    // TODO: Make these constants in header
    const int DEFAULT_CONSUMER_USAGE_BITS = 0;

    // Parameters for a generic "valid" input for queueBuffer.
    const int64_t QUEUE_BUFFER_INPUT_TIMESTAMP = 1384888611;
    const bool QUEUE_BUFFER_INPUT_IS_AUTO_TIMESTAMP = false;
    const android_dataspace QUEUE_BUFFER_INPUT_DATASPACE = HAL_DATASPACE_UNKNOWN;
    const Rect QUEUE_BUFFER_INPUT_RECT = Rect(DEFAULT_WIDTH, DEFAULT_HEIGHT);
    const int QUEUE_BUFFER_INPUT_SCALING_MODE = 0;
    const int QUEUE_BUFFER_INPUT_TRANSFORM = 0;
    const sp<Fence> QUEUE_BUFFER_INPUT_FENCE = Fence::NO_FENCE;

    // Enums to control which IGraphicBufferProducer backend to test.
    enum IGraphicBufferProducerTestCode {
        USE_BUFFER_QUEUE_PRODUCER = 0,
        USE_BUFFER_HUB_PRODUCER,
    };
}; // namespace anonymous

class IGraphicBufferProducerTest : public ::testing::TestWithParam<uint32_t> {
protected:

    IGraphicBufferProducerTest() {}

    virtual void SetUp() {
        const ::testing::TestInfo* const testInfo =
            ::testing::UnitTest::GetInstance()->current_test_info();
        ALOGV("Begin test: %s.%s", testInfo->test_case_name(),
                testInfo->name());

        mDC = new DummyConsumer;

        switch (GetParam()) {
            case USE_BUFFER_QUEUE_PRODUCER: {
                BufferQueue::createBufferQueue(&mProducer, &mConsumer);
                break;
            }
            case USE_BUFFER_HUB_PRODUCER: {
                BufferQueue::createBufferHubQueue(&mProducer, &mConsumer);
                break;
            }
            default: {
                // Should never reach here.
                LOG_ALWAYS_FATAL("Invalid test params: %u", GetParam());
                break;
            }
        }

        // Test check: Can't connect producer if no consumer yet
        if (GetParam() == USE_BUFFER_QUEUE_PRODUCER) {
            // TODO(b/73267953): Make BufferHub honor producer and consumer connection.
            ASSERT_EQ(NO_INIT, TryConnectProducer());
        }

        // Must connect consumer before producer connects will succeed.
        ASSERT_OK(mConsumer->consumerConnect(mDC, /*controlledByApp*/false));
    }

    virtual void TearDown() {
        const ::testing::TestInfo* const testInfo =
            ::testing::UnitTest::GetInstance()->current_test_info();
        ALOGV("End test:   %s.%s", testInfo->test_case_name(),
                testInfo->name());
    }

    status_t TryConnectProducer() {
        IGraphicBufferProducer::QueueBufferOutput output;
        return mProducer->connect(TEST_TOKEN,
                                  TEST_API,
                                  TEST_CONTROLLED_BY_APP,
                                  &output);
        // TODO: use params to vary token, api, producercontrolledbyapp, etc
    }

    // Connect to a producer in a 'correct' fashion.
    //   Precondition: Consumer is connected.
    void ConnectProducer() {
        ASSERT_OK(TryConnectProducer());
    }

    // Create a generic "valid" input for queueBuffer
    // -- uses the default buffer format, width, etc.
    static IGraphicBufferProducer::QueueBufferInput CreateBufferInput() {
        return QueueBufferInputBuilder().build();
    }

    // Builder pattern to slightly vary *almost* correct input
    // -- avoids copying and pasting
    struct QueueBufferInputBuilder {
        QueueBufferInputBuilder() {
           timestamp = QUEUE_BUFFER_INPUT_TIMESTAMP;
           isAutoTimestamp = QUEUE_BUFFER_INPUT_IS_AUTO_TIMESTAMP;
           dataSpace = QUEUE_BUFFER_INPUT_DATASPACE;
           crop = QUEUE_BUFFER_INPUT_RECT;
           scalingMode = QUEUE_BUFFER_INPUT_SCALING_MODE;
           transform = QUEUE_BUFFER_INPUT_TRANSFORM;
           fence = QUEUE_BUFFER_INPUT_FENCE;
        }

        IGraphicBufferProducer::QueueBufferInput build() {
            return IGraphicBufferProducer::QueueBufferInput(
                    timestamp,
                    isAutoTimestamp,
                    dataSpace,
                    crop,
                    scalingMode,
                    transform,
                    fence);
        }

        QueueBufferInputBuilder& setTimestamp(int64_t timestamp) {
            this->timestamp = timestamp;
            return *this;
        }

        QueueBufferInputBuilder& setIsAutoTimestamp(bool isAutoTimestamp) {
            this->isAutoTimestamp = isAutoTimestamp;
            return *this;
        }

        QueueBufferInputBuilder& setDataSpace(android_dataspace dataSpace) {
            this->dataSpace = dataSpace;
            return *this;
        }

        QueueBufferInputBuilder& setCrop(Rect crop) {
            this->crop = crop;
            return *this;
        }

        QueueBufferInputBuilder& setScalingMode(int scalingMode) {
            this->scalingMode = scalingMode;
            return *this;
        }

        QueueBufferInputBuilder& setTransform(uint32_t transform) {
            this->transform = transform;
            return *this;
        }

        QueueBufferInputBuilder& setFence(sp<Fence> fence) {
            this->fence = fence;
            return *this;
        }

    private:
        int64_t timestamp;
        bool isAutoTimestamp;
        android_dataspace dataSpace;
        Rect crop;
        int scalingMode;
        uint32_t transform;
        sp<Fence> fence;
    }; // struct QueueBufferInputBuilder

    // To easily store dequeueBuffer results into containers
    struct DequeueBufferResult {
        int slot;
        sp<Fence> fence;
    };

    status_t dequeueBuffer(uint32_t w, uint32_t h, uint32_t format, uint32_t usage, DequeueBufferResult* result) {
        return mProducer->dequeueBuffer(&result->slot, &result->fence, w, h, format, usage,
                                        nullptr, nullptr);
    }

    void setupDequeueRequestBuffer(int *slot, sp<Fence> *fence,
            sp<GraphicBuffer> *buffer)
    {
        ASSERT_TRUE(slot != nullptr);
        ASSERT_TRUE(fence != nullptr);
        ASSERT_TRUE(buffer != nullptr);

        ASSERT_NO_FATAL_FAILURE(ConnectProducer());


        ASSERT_EQ(OK,
                  ~IGraphicBufferProducer::BUFFER_NEEDS_REALLOCATION &
                          (mProducer->dequeueBuffer(slot, fence, DEFAULT_WIDTH, DEFAULT_HEIGHT,
                                                    DEFAULT_FORMAT, TEST_PRODUCER_USAGE_BITS,
                                                    nullptr, nullptr)));

        EXPECT_LE(0, *slot);
        EXPECT_GT(BufferQueue::NUM_BUFFER_SLOTS, *slot);

        // Request the buffer (pre-requisite for queueing)
        ASSERT_OK(mProducer->requestBuffer(*slot, buffer));
    }

private: // hide from test body
    sp<DummyConsumer> mDC;

protected: // accessible from test body
    sp<IGraphicBufferProducer> mProducer;
    sp<IGraphicBufferConsumer> mConsumer;
};

TEST_P(IGraphicBufferProducerTest, ConnectFirst_ReturnsError) {
    IGraphicBufferProducer::QueueBufferOutput output;

    // NULL output returns BAD_VALUE
    EXPECT_EQ(BAD_VALUE, mProducer->connect(TEST_TOKEN,
                                            TEST_API,
                                            TEST_CONTROLLED_BY_APP,
                                            /*output*/nullptr));

    // Invalid API returns bad value
    EXPECT_EQ(BAD_VALUE, mProducer->connect(TEST_TOKEN,
                                            /*api*/0xDEADBEEF,
                                            TEST_CONTROLLED_BY_APP,
                                            &output));

    // TODO: get a token from a dead process somehow
}

TEST_P(IGraphicBufferProducerTest, ConnectAgain_ReturnsError) {
    ASSERT_NO_FATAL_FAILURE(ConnectProducer());

    // Can't connect when there is already a producer connected
    IGraphicBufferProducer::QueueBufferOutput output;
    EXPECT_EQ(BAD_VALUE, mProducer->connect(TEST_TOKEN,
                                            TEST_API,
                                            TEST_CONTROLLED_BY_APP,
                                            &output));

    ASSERT_OK(mConsumer->consumerDisconnect());
    // Can't connect when IGBP is abandoned
    if (GetParam() == USE_BUFFER_QUEUE_PRODUCER) {
        // TODO(b/73267953): Make BufferHub honor producer and consumer connection.
        EXPECT_EQ(NO_INIT, mProducer->connect(TEST_TOKEN,
                                              TEST_API,
                                              TEST_CONTROLLED_BY_APP,
                                              &output));
    }
}

TEST_P(IGraphicBufferProducerTest, Disconnect_Succeeds) {
    ASSERT_NO_FATAL_FAILURE(ConnectProducer());

    ASSERT_OK(mProducer->disconnect(TEST_API));
}


TEST_P(IGraphicBufferProducerTest, Disconnect_ReturnsError) {
    ASSERT_NO_FATAL_FAILURE(ConnectProducer());

    // Must disconnect with same API number
    ASSERT_EQ(BAD_VALUE, mProducer->disconnect(TEST_API_OTHER));
    // API must not be out of range
    ASSERT_EQ(BAD_VALUE, mProducer->disconnect(/*api*/0xDEADBEEF));

    // TODO: somehow kill mProducer so that this returns DEAD_OBJECT
}

TEST_P(IGraphicBufferProducerTest, Query_Succeeds) {
    ASSERT_NO_FATAL_FAILURE(ConnectProducer());

    int32_t value = -1;
    EXPECT_OK(mProducer->query(NATIVE_WINDOW_WIDTH, &value));
    EXPECT_EQ(DEFAULT_WIDTH, static_cast<uint32_t>(value));

    EXPECT_OK(mProducer->query(NATIVE_WINDOW_HEIGHT, &value));
    EXPECT_EQ(DEFAULT_HEIGHT, static_cast<uint32_t>(value));

    EXPECT_OK(mProducer->query(NATIVE_WINDOW_FORMAT, &value));
    EXPECT_EQ(DEFAULT_FORMAT, value);

    EXPECT_OK(mProducer->query(NATIVE_WINDOW_MIN_UNDEQUEUED_BUFFERS, &value));
    EXPECT_LE(0, value);
    EXPECT_GE(BufferQueue::NUM_BUFFER_SLOTS, value);

    EXPECT_OK(mProducer->query(NATIVE_WINDOW_CONSUMER_RUNNING_BEHIND, &value));
    EXPECT_FALSE(value); // Can't run behind when we haven't touched the queue

    EXPECT_OK(mProducer->query(NATIVE_WINDOW_CONSUMER_USAGE_BITS, &value));
    EXPECT_EQ(DEFAULT_CONSUMER_USAGE_BITS, value);

}

TEST_P(IGraphicBufferProducerTest, Query_ReturnsError) {
    ASSERT_NO_FATAL_FAILURE(ConnectProducer());

    // One past the end of the last 'query' enum value. Update this if we add more enums.
    const int NATIVE_WINDOW_QUERY_LAST_OFF_BY_ONE = NATIVE_WINDOW_BUFFER_AGE + 1;

    int value;
    // What was out of range
    EXPECT_EQ(BAD_VALUE, mProducer->query(/*what*/-1, &value));
    EXPECT_EQ(BAD_VALUE, mProducer->query(/*what*/0xDEADBEEF, &value));
    EXPECT_EQ(BAD_VALUE, mProducer->query(NATIVE_WINDOW_QUERY_LAST_OFF_BY_ONE, &value));

    // Some enums from window.h are 'invalid'
    EXPECT_EQ(BAD_VALUE, mProducer->query(NATIVE_WINDOW_QUEUES_TO_WINDOW_COMPOSER, &value));
    EXPECT_EQ(BAD_VALUE, mProducer->query(NATIVE_WINDOW_CONCRETE_TYPE, &value));
    EXPECT_EQ(BAD_VALUE, mProducer->query(NATIVE_WINDOW_DEFAULT_WIDTH, &value));
    EXPECT_EQ(BAD_VALUE, mProducer->query(NATIVE_WINDOW_DEFAULT_HEIGHT, &value));
    EXPECT_EQ(BAD_VALUE, mProducer->query(NATIVE_WINDOW_TRANSFORM_HINT, &value));
    // TODO: Consider documented the above enums as unsupported or make a new enum for IGBP

    // Value was NULL
    EXPECT_EQ(BAD_VALUE, mProducer->query(NATIVE_WINDOW_FORMAT, /*value*/nullptr));

    ASSERT_OK(mConsumer->consumerDisconnect());

    // BQ was abandoned
    if (GetParam() == USE_BUFFER_QUEUE_PRODUCER) {
        // TODO(b/73267953): Make BufferHub honor producer and consumer connection.
        EXPECT_EQ(NO_INIT, mProducer->query(NATIVE_WINDOW_FORMAT, &value));
    }

    // TODO: other things in window.h that are supported by Surface::query
    // but not by BufferQueue::query
}

// TODO: queue under more complicated situations not involving just a single buffer
TEST_P(IGraphicBufferProducerTest, Queue_Succeeds) {
    ASSERT_NO_FATAL_FAILURE(ConnectProducer());

    int dequeuedSlot = -1;
    sp<Fence> dequeuedFence;

    ASSERT_EQ(OK,
              ~IGraphicBufferProducer::BUFFER_NEEDS_REALLOCATION &
                      (mProducer->dequeueBuffer(&dequeuedSlot, &dequeuedFence, DEFAULT_WIDTH,
                                                DEFAULT_HEIGHT, DEFAULT_FORMAT,
                                                TEST_PRODUCER_USAGE_BITS, nullptr, nullptr)));

    EXPECT_LE(0, dequeuedSlot);
    EXPECT_GT(BufferQueue::NUM_BUFFER_SLOTS, dequeuedSlot);

    // Request the buffer (pre-requisite for queueing)
    sp<GraphicBuffer> dequeuedBuffer;
    ASSERT_OK(mProducer->requestBuffer(dequeuedSlot, &dequeuedBuffer));

    // A generic "valid" input
    IGraphicBufferProducer::QueueBufferInput input = CreateBufferInput();
    IGraphicBufferProducer::QueueBufferOutput output;

    // Queue the buffer back into the BQ
    ASSERT_OK(mProducer->queueBuffer(dequeuedSlot, input, &output));

    {
        EXPECT_EQ(DEFAULT_WIDTH, output.width);
        EXPECT_EQ(DEFAULT_HEIGHT, output.height);
        EXPECT_EQ(DEFAULT_TRANSFORM_HINT, output.transformHint);

        // Since queueBuffer was called exactly once
        if (GetParam() == USE_BUFFER_QUEUE_PRODUCER) {
            // TODO(b/70041889): BufferHubProducer need to support metadata: numPendingBuffers
            EXPECT_EQ(1u, output.numPendingBuffers);
            // TODO(b/70041952): BufferHubProducer need to support metadata: nextFrameNumber
            EXPECT_EQ(2u, output.nextFrameNumber);
        }
    }

    // Buffer was not in the dequeued state
    EXPECT_EQ(BAD_VALUE, mProducer->queueBuffer(dequeuedSlot, input, &output));
}

TEST_P(IGraphicBufferProducerTest, Queue_ReturnsError) {
    ASSERT_NO_FATAL_FAILURE(ConnectProducer());

    // Invalid slot number
    {
        // A generic "valid" input
        IGraphicBufferProducer::QueueBufferInput input = CreateBufferInput();
        IGraphicBufferProducer::QueueBufferOutput output;

        EXPECT_EQ(BAD_VALUE, mProducer->queueBuffer(/*slot*/-1, input, &output));
        EXPECT_EQ(BAD_VALUE, mProducer->queueBuffer(/*slot*/0xDEADBEEF, input, &output));
        EXPECT_EQ(BAD_VALUE, mProducer->queueBuffer(BufferQueue::NUM_BUFFER_SLOTS,
                                                    input, &output));
    }

    // Slot was not in the dequeued state (all slots start out in Free state)
    {
        IGraphicBufferProducer::QueueBufferInput input = CreateBufferInput();
        IGraphicBufferProducer::QueueBufferOutput output;

        EXPECT_EQ(BAD_VALUE, mProducer->queueBuffer(/*slot*/0, input, &output));
    }

    // Put the slot into the "dequeued" state for the rest of the test
    int dequeuedSlot = -1;
    sp<Fence> dequeuedFence;

    ASSERT_EQ(OK,
              ~IGraphicBufferProducer::BUFFER_NEEDS_REALLOCATION &
                      (mProducer->dequeueBuffer(&dequeuedSlot, &dequeuedFence, DEFAULT_WIDTH,
                                                DEFAULT_HEIGHT, DEFAULT_FORMAT,
                                                TEST_PRODUCER_USAGE_BITS, nullptr, nullptr)));

    // Slot was enqueued without requesting a buffer
    {
        IGraphicBufferProducer::QueueBufferInput input = CreateBufferInput();
        IGraphicBufferProducer::QueueBufferOutput output;

        EXPECT_EQ(BAD_VALUE, mProducer->queueBuffer(dequeuedSlot, input, &output));
    }

    // Request the buffer so that the rest of the tests don't fail on earlier checks.
    sp<GraphicBuffer> dequeuedBuffer;
    ASSERT_OK(mProducer->requestBuffer(dequeuedSlot, &dequeuedBuffer));

    // Fence was NULL
    {
        sp<Fence> nullFence = nullptr;

        IGraphicBufferProducer::QueueBufferInput input =
                QueueBufferInputBuilder().setFence(nullFence).build();
        IGraphicBufferProducer::QueueBufferOutput output;

        EXPECT_EQ(BAD_VALUE, mProducer->queueBuffer(dequeuedSlot, input, &output));
    }

    // Scaling mode was unknown
    {
        IGraphicBufferProducer::QueueBufferInput input =
                QueueBufferInputBuilder().setScalingMode(-1).build();
        IGraphicBufferProducer::QueueBufferOutput output;

        EXPECT_EQ(BAD_VALUE, mProducer->queueBuffer(dequeuedSlot, input, &output));

        input = QueueBufferInputBuilder().setScalingMode(0xDEADBEEF).build();

        EXPECT_EQ(BAD_VALUE, mProducer->queueBuffer(dequeuedSlot, input, &output));
    }

    // Crop rect is out of bounds of the buffer dimensions
    {
        IGraphicBufferProducer::QueueBufferInput input =
                QueueBufferInputBuilder().setCrop(Rect(DEFAULT_WIDTH + 1, DEFAULT_HEIGHT + 1))
                .build();
        IGraphicBufferProducer::QueueBufferOutput output;

        EXPECT_EQ(BAD_VALUE, mProducer->queueBuffer(dequeuedSlot, input, &output));
    }

    // Abandon the buffer queue so that the last test fails
    ASSERT_OK(mConsumer->consumerDisconnect());

    // The buffer queue has been abandoned.
    if (GetParam() == USE_BUFFER_QUEUE_PRODUCER) {
        IGraphicBufferProducer::QueueBufferInput input = CreateBufferInput();
        IGraphicBufferProducer::QueueBufferOutput output;

        // TODO(b/73267953): Make BufferHub honor producer and consumer connection.
        EXPECT_EQ(NO_INIT, mProducer->queueBuffer(dequeuedSlot, input, &output));
    }
}

TEST_P(IGraphicBufferProducerTest, CancelBuffer_DoesntCrash) {
    ASSERT_NO_FATAL_FAILURE(ConnectProducer());

    int dequeuedSlot = -1;
    sp<Fence> dequeuedFence;

    ASSERT_EQ(OK,
              ~IGraphicBufferProducer::BUFFER_NEEDS_REALLOCATION &
                      (mProducer->dequeueBuffer(&dequeuedSlot, &dequeuedFence, DEFAULT_WIDTH,
                                                DEFAULT_HEIGHT, DEFAULT_FORMAT,
                                                TEST_PRODUCER_USAGE_BITS, nullptr, nullptr)));

    // No return code, but at least test that it doesn't blow up...
    // TODO: add a return code
    mProducer->cancelBuffer(dequeuedSlot, dequeuedFence);
}

TEST_P(IGraphicBufferProducerTest, SetMaxDequeuedBufferCount_Succeeds) {
    ASSERT_NO_FATAL_FAILURE(ConnectProducer());
    int minUndequeuedBuffers;
    ASSERT_OK(mProducer->query(NATIVE_WINDOW_MIN_UNDEQUEUED_BUFFERS,
            &minUndequeuedBuffers));

    const int minBuffers = 1;
    const int maxBuffers = BufferQueue::NUM_BUFFER_SLOTS - minUndequeuedBuffers;

    ASSERT_OK(mProducer->setAsyncMode(false)) << "async mode: " << false;
    ASSERT_OK(mProducer->setMaxDequeuedBufferCount(minBuffers))
            << "bufferCount: " << minBuffers;

    // Should now be able to dequeue up to minBuffers times
    DequeueBufferResult result;
    for (int i = 0; i < minBuffers; ++i) {

        EXPECT_EQ(OK, ~IGraphicBufferProducer::BUFFER_NEEDS_REALLOCATION &
                (dequeueBuffer(DEFAULT_WIDTH, DEFAULT_HEIGHT, DEFAULT_FORMAT,
                              TEST_PRODUCER_USAGE_BITS, &result)))
                << "iteration: " << i << ", slot: " << result.slot;
    }

    ASSERT_OK(mProducer->setMaxDequeuedBufferCount(maxBuffers));

    // queue the first buffer to enable max dequeued buffer count checking
    IGraphicBufferProducer::QueueBufferInput input = CreateBufferInput();
    IGraphicBufferProducer::QueueBufferOutput output;
    sp<GraphicBuffer> buffer;
    ASSERT_OK(mProducer->requestBuffer(result.slot, &buffer));
    ASSERT_OK(mProducer->queueBuffer(result.slot, input, &output));


    // Should now be able to dequeue up to maxBuffers times
    int dequeuedSlot = -1;
    sp<Fence> dequeuedFence;
    for (int i = 0; i < maxBuffers; ++i) {
        EXPECT_EQ(OK,
                  ~IGraphicBufferProducer::BUFFER_NEEDS_REALLOCATION &
                          (mProducer->dequeueBuffer(&dequeuedSlot, &dequeuedFence, DEFAULT_WIDTH,
                                                    DEFAULT_HEIGHT, DEFAULT_FORMAT,
                                                    TEST_PRODUCER_USAGE_BITS, nullptr, nullptr)))
                << "iteration: " << i << ", slot: " << dequeuedSlot;
    }

    // Cancel a buffer, so we can decrease the buffer count
    ASSERT_OK(mProducer->cancelBuffer(dequeuedSlot, dequeuedFence));

    // Should now be able to decrease the max dequeued count by 1
    ASSERT_OK(mProducer->setMaxDequeuedBufferCount(maxBuffers-1));
}

TEST_P(IGraphicBufferProducerTest, SetMaxDequeuedBufferCount_Fails) {
    ASSERT_NO_FATAL_FAILURE(ConnectProducer());
    int minUndequeuedBuffers;
    ASSERT_OK(mProducer->query(NATIVE_WINDOW_MIN_UNDEQUEUED_BUFFERS,
                               &minUndequeuedBuffers));

    const int minBuffers = 1;
    const int maxBuffers = BufferQueue::NUM_BUFFER_SLOTS - minUndequeuedBuffers;

    ASSERT_OK(mProducer->setAsyncMode(false)) << "async mode: " << false;
    // Buffer count was out of range
    EXPECT_EQ(BAD_VALUE, mProducer->setMaxDequeuedBufferCount(0))
            << "bufferCount: " << 0;
    EXPECT_EQ(BAD_VALUE, mProducer->setMaxDequeuedBufferCount(maxBuffers + 1))
            << "bufferCount: " << maxBuffers + 1;

    // Set max dequeue count to 2
    ASSERT_OK(mProducer->setMaxDequeuedBufferCount(2));
    // Dequeue 2 buffers
    int dequeuedSlot = -1;
    sp<Fence> dequeuedFence;
    for (int i = 0; i < 2; i++) {
        ASSERT_EQ(OK,
                  ~IGraphicBufferProducer::BUFFER_NEEDS_REALLOCATION &
                          (mProducer->dequeueBuffer(&dequeuedSlot, &dequeuedFence, DEFAULT_WIDTH,
                                                    DEFAULT_HEIGHT, DEFAULT_FORMAT,
                                                    TEST_PRODUCER_USAGE_BITS, nullptr, nullptr)))
                << "slot: " << dequeuedSlot;
    }

    // Client has too many buffers dequeued
    EXPECT_EQ(BAD_VALUE, mProducer->setMaxDequeuedBufferCount(1))
            << "bufferCount: " << minBuffers;

    // Abandon buffer queue
    ASSERT_OK(mConsumer->consumerDisconnect());

    // Fail because the buffer queue was abandoned
    if (GetParam() == USE_BUFFER_QUEUE_PRODUCER) {
        // TODO(b/73267953): Make BufferHub honor producer and consumer connection.
        EXPECT_EQ(NO_INIT, mProducer->setMaxDequeuedBufferCount(minBuffers))
                << "bufferCount: " << minBuffers;
    }
}

TEST_P(IGraphicBufferProducerTest, SetAsyncMode_Succeeds) {
    if (GetParam() == USE_BUFFER_HUB_PRODUCER) {
        // TODO(b/36724099): Add support for BufferHubProducer::setAsyncMode(true)
        return;
    }

    ASSERT_OK(mConsumer->setMaxAcquiredBufferCount(1)) << "maxAcquire: " << 1;
    ASSERT_NO_FATAL_FAILURE(ConnectProducer());
    ASSERT_OK(mProducer->setAsyncMode(true)) << "async mode: " << true;
    ASSERT_OK(mProducer->setMaxDequeuedBufferCount(1)) << "maxDequeue: " << 1;

    int dequeuedSlot = -1;
    sp<Fence> dequeuedFence;
    IGraphicBufferProducer::QueueBufferInput input = CreateBufferInput();
    IGraphicBufferProducer::QueueBufferOutput output;
    sp<GraphicBuffer> dequeuedBuffer;

    // Should now be able to queue/dequeue as many buffers as we want without
    // blocking
    for (int i = 0; i < 5; ++i) {
        ASSERT_EQ(OK,
                  ~IGraphicBufferProducer::BUFFER_NEEDS_REALLOCATION &
                          (mProducer->dequeueBuffer(&dequeuedSlot, &dequeuedFence, DEFAULT_WIDTH,
                                                    DEFAULT_HEIGHT, DEFAULT_FORMAT,
                                                    TEST_PRODUCER_USAGE_BITS, nullptr, nullptr)))
                << "slot : " << dequeuedSlot;
        ASSERT_OK(mProducer->requestBuffer(dequeuedSlot, &dequeuedBuffer));
        ASSERT_OK(mProducer->queueBuffer(dequeuedSlot, input, &output));
    }
}

TEST_P(IGraphicBufferProducerTest, SetAsyncMode_Fails) {
    ASSERT_NO_FATAL_FAILURE(ConnectProducer());
    // Prerequisite to fail out a valid setBufferCount call
    {
        int dequeuedSlot = -1;
        sp<Fence> dequeuedFence;

        ASSERT_EQ(OK,
                  ~IGraphicBufferProducer::BUFFER_NEEDS_REALLOCATION &
                          (mProducer->dequeueBuffer(&dequeuedSlot, &dequeuedFence, DEFAULT_WIDTH,
                                                    DEFAULT_HEIGHT, DEFAULT_FORMAT,
                                                    TEST_PRODUCER_USAGE_BITS, nullptr, nullptr)))
                << "slot: " << dequeuedSlot;
    }

    // Abandon buffer queue
    ASSERT_OK(mConsumer->consumerDisconnect());

    // Fail because the buffer queue was abandoned
    if (GetParam() == USE_BUFFER_QUEUE_PRODUCER) {
        // TODO(b/36724099): Make BufferHub honor producer and consumer connection.
        EXPECT_EQ(NO_INIT, mProducer->setAsyncMode(false)) << "asyncMode: " << false;
    }
}

TEST_P(IGraphicBufferProducerTest,
        DisconnectedProducerReturnsError_dequeueBuffer) {
    int slot = -1;
    sp<Fence> fence;

    ASSERT_EQ(NO_INIT,
              mProducer->dequeueBuffer(&slot, &fence, DEFAULT_WIDTH, DEFAULT_HEIGHT, DEFAULT_FORMAT,
                                       TEST_PRODUCER_USAGE_BITS, nullptr, nullptr));
}

TEST_P(IGraphicBufferProducerTest,
        DisconnectedProducerReturnsError_detachNextBuffer) {
    sp<Fence> fence;
    sp<GraphicBuffer> buffer;

    ASSERT_EQ(NO_INIT, mProducer->detachNextBuffer(&buffer, &fence));
}

TEST_P(IGraphicBufferProducerTest,
        DisconnectedProducerReturnsError_requestBuffer) {
    ASSERT_NO_FATAL_FAILURE(ConnectProducer());

    int slot = -1;
    sp<Fence> fence;

    ASSERT_EQ(OK,
              ~IGraphicBufferProducer::BUFFER_NEEDS_REALLOCATION &
                      (mProducer->dequeueBuffer(&slot, &fence, DEFAULT_WIDTH, DEFAULT_HEIGHT,
                                                DEFAULT_FORMAT, TEST_PRODUCER_USAGE_BITS,
                                                nullptr, nullptr)));

    EXPECT_LE(0, slot);
    EXPECT_GT(BufferQueue::NUM_BUFFER_SLOTS, slot);

    ASSERT_OK(mProducer->disconnect(TEST_API));

    sp<GraphicBuffer> buffer;

    ASSERT_EQ(NO_INIT, mProducer->requestBuffer(slot, &buffer));
}


TEST_P(IGraphicBufferProducerTest,
        DisconnectedProducerReturnsError_detachBuffer) {
    int slot = -1;
    sp<Fence> fence;
    sp<GraphicBuffer> buffer;

    setupDequeueRequestBuffer(&slot, &fence, &buffer);

    ASSERT_OK(mProducer->disconnect(TEST_API));

    ASSERT_EQ(NO_INIT, mProducer->detachBuffer(slot));
}

TEST_P(IGraphicBufferProducerTest,
        DisconnectedProducerReturnsError_queueBuffer) {
    int slot = -1;
    sp<Fence> fence;
    sp<GraphicBuffer> buffer;

    setupDequeueRequestBuffer(&slot, &fence, &buffer);

    ASSERT_OK(mProducer->disconnect(TEST_API));

    // A generic "valid" input
    IGraphicBufferProducer::QueueBufferInput input = CreateBufferInput();
    IGraphicBufferProducer::QueueBufferOutput output;

    ASSERT_EQ(NO_INIT, mProducer->queueBuffer(slot, input, &output));
}

TEST_P(IGraphicBufferProducerTest,
        DisconnectedProducerReturnsError_cancelBuffer) {
    int slot = -1;
    sp<Fence> fence;
    sp<GraphicBuffer> buffer;

    setupDequeueRequestBuffer(&slot, &fence, &buffer);

    ASSERT_OK(mProducer->disconnect(TEST_API));

    ASSERT_EQ(NO_INIT, mProducer->cancelBuffer(slot, fence));
}

TEST_P(IGraphicBufferProducerTest,
        DisconnectedProducerReturnsError_attachBuffer) {
    int slot = -1;
    sp<Fence> fence;
    sp<GraphicBuffer> buffer;

    setupDequeueRequestBuffer(&slot, &fence, &buffer);
    ASSERT_TRUE(buffer != nullptr);

    ASSERT_OK(mProducer->detachBuffer(slot));
    EXPECT_OK(buffer->initCheck());

    ASSERT_OK(mProducer->disconnect(TEST_API));

    ASSERT_EQ(NO_INIT, mProducer->attachBuffer(&slot, buffer));
}

TEST_P(IGraphicBufferProducerTest, DetachThenAttach_Succeeds) {
    int slot = -1;
    sp<Fence> fence;
    sp<GraphicBuffer> buffer;

    setupDequeueRequestBuffer(&slot, &fence, &buffer);
    ASSERT_TRUE(buffer != nullptr);

    ASSERT_OK(mProducer->detachBuffer(slot));
    EXPECT_OK(buffer->initCheck());

    EXPECT_OK(mProducer->attachBuffer(&slot, buffer));
    EXPECT_OK(buffer->initCheck());
}

#if USE_BUFFER_HUB_AS_BUFFER_QUEUE
INSTANTIATE_TEST_CASE_P(IGraphicBufferProducerBackends, IGraphicBufferProducerTest,
                        ::testing::Values(USE_BUFFER_QUEUE_PRODUCER, USE_BUFFER_HUB_PRODUCER));
#else
// TODO(b/70046255): Remove the #ifdef here and always tests both backends once BufferHubQueue can
// pass all existing libgui tests.
INSTANTIATE_TEST_CASE_P(IGraphicBufferProducerBackends, IGraphicBufferProducerTest,
                        ::testing::Values(USE_BUFFER_QUEUE_PRODUCER));
#endif

} // namespace android
