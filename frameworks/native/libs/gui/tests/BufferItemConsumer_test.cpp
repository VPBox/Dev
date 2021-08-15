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

#define LOG_TAG "BufferItemConsumer_test"
//#define LOG_NDEBUG 0

#include <gtest/gtest.h>
#include <gui/BufferItemConsumer.h>
#include <gui/IProducerListener.h>
#include <gui/Surface.h>

namespace android {

static constexpr int kWidth = 100;
static constexpr int kHeight = 100;
static constexpr int kMaxLockedBuffers = 3;
static constexpr int kFormat = HAL_PIXEL_FORMAT_RGBA_8888;
static constexpr int kFrameSleepUs = 30 * 1000;

class BufferItemConsumerTest : public ::testing::Test {
   protected:
    struct BufferFreedListener
        : public BufferItemConsumer::BufferFreedListener {
        explicit BufferFreedListener(BufferItemConsumerTest* test)
            : mTest(test) {}
        void onBufferFreed(const wp<GraphicBuffer>& /* gBuffer */) override {
            mTest->HandleBufferFreed();
        }
        BufferItemConsumerTest* mTest;
    };

    void SetUp() override {
        BufferQueue::createBufferQueue(&mProducer, &mConsumer);
        mBIC =
            new BufferItemConsumer(mConsumer, kFormat, kMaxLockedBuffers, true);
        String8 name("BufferItemConsumer_Under_Test");
        mBIC->setName(name);
        mBFL = new BufferFreedListener(this);
        mBIC->setBufferFreedListener(mBFL);

        sp<IProducerListener> producerListener = new DummyProducerListener();
        IGraphicBufferProducer::QueueBufferOutput bufferOutput;
        ASSERT_EQ(NO_ERROR,
                  mProducer->connect(producerListener, NATIVE_WINDOW_API_CPU,
                                     true, &bufferOutput));
        ASSERT_EQ(NO_ERROR,
                  mProducer->setMaxDequeuedBufferCount(kMaxLockedBuffers));
    }

    int GetFreedBufferCount() {
        std::lock_guard<std::mutex> lock(mMutex);
        return mFreedBufferCount;
    }

    void HandleBufferFreed() {
        std::lock_guard<std::mutex> lock(mMutex);
        mFreedBufferCount++;
        ALOGV("HandleBufferFreed, mFreedBufferCount=%d", mFreedBufferCount);
    }

    void DequeueBuffer(int* outSlot) {
        ASSERT_NE(outSlot, nullptr);

        int slot;
        sp<Fence> outFence;
        status_t ret = mProducer->dequeueBuffer(&slot, &outFence, kWidth, kHeight, 0, 0,
                                                nullptr, nullptr);
        ASSERT_GE(ret, 0);

        ALOGV("dequeueBuffer: slot=%d", slot);
        if (ret & IGraphicBufferProducer::BUFFER_NEEDS_REALLOCATION) {
            ret = mProducer->requestBuffer(slot, &mBuffers[slot]);
            ASSERT_EQ(NO_ERROR, ret);
        }
        *outSlot = slot;
    }

    void QueueBuffer(int slot) {
        ALOGV("enqueueBuffer: slot=%d", slot);
        IGraphicBufferProducer::QueueBufferInput bufferInput(
            0ULL, true, HAL_DATASPACE_UNKNOWN, Rect::INVALID_RECT,
            NATIVE_WINDOW_SCALING_MODE_FREEZE, 0, Fence::NO_FENCE);
        IGraphicBufferProducer::QueueBufferOutput bufferOutput;
        status_t ret = mProducer->queueBuffer(slot, bufferInput, &bufferOutput);
        ASSERT_EQ(NO_ERROR, ret);
    }

    void AcquireBuffer(int* outSlot) {
        ASSERT_NE(outSlot, nullptr);
        BufferItem buffer;
        status_t ret = mBIC->acquireBuffer(&buffer, 0, false);
        ASSERT_EQ(NO_ERROR, ret);

        ALOGV("acquireBuffer: slot=%d", buffer.mSlot);
        *outSlot = buffer.mSlot;
    }

    void ReleaseBuffer(int slot) {
        ALOGV("releaseBuffer: slot=%d", slot);
        BufferItem buffer;
        buffer.mSlot = slot;
        buffer.mGraphicBuffer = mBuffers[slot];
        status_t ret = mBIC->releaseBuffer(buffer, Fence::NO_FENCE);
        ASSERT_EQ(NO_ERROR, ret);
    }


    std::mutex mMutex;
    int mFreedBufferCount{0};

    sp<BufferItemConsumer> mBIC;
    sp<BufferFreedListener> mBFL;
    sp<IGraphicBufferProducer> mProducer;
    sp<IGraphicBufferConsumer> mConsumer;
    sp<GraphicBuffer> mBuffers[BufferQueueDefs::NUM_BUFFER_SLOTS];
};

// Test that detaching buffer from consumer side triggers onBufferFreed.
TEST_F(BufferItemConsumerTest, TriggerBufferFreed_DetachBufferFromConsumer) {
    int slot;
    // Producer: generate a dummy buffer.
    DequeueBuffer(&slot);
    QueueBuffer(slot);

    ASSERT_EQ(0, GetFreedBufferCount());
    // Consumer: acquire the buffer and then detach it.
    AcquireBuffer(&slot);
    status_t ret = mBIC->detachBuffer(slot);
    ASSERT_EQ(NO_ERROR, ret);

    // Sleep to give some time for callbacks to happen.
    usleep(kFrameSleepUs);
    ASSERT_EQ(1, GetFreedBufferCount());
}

// Test that detaching buffer from producer side triggers onBufferFreed.
TEST_F(BufferItemConsumerTest, TriggerBufferFreed_DetachBufferFromProducer) {
    int slot;
    // Let buffer go through the cycle at least once.
    DequeueBuffer(&slot);
    QueueBuffer(slot);
    AcquireBuffer(&slot);
    ReleaseBuffer(slot);

    ASSERT_EQ(0, GetFreedBufferCount());

    // Producer: generate the buffer again.
    DequeueBuffer(&slot);

    // Producer: detach the buffer.
    status_t ret = mProducer->detachBuffer(slot);
    ASSERT_EQ(NO_ERROR, ret);

    // Sleep to give some time for callbacks to happen.
    usleep(kFrameSleepUs);
    ASSERT_EQ(1, GetFreedBufferCount());
}

// Test that abandoning BufferItemConsumer triggers onBufferFreed.
TEST_F(BufferItemConsumerTest, TriggerBufferFreed_AbandonBufferItemConsumer) {
    int slot;
    // Let buffer go through the cycle at least once.
    DequeueBuffer(&slot);
    QueueBuffer(slot);
    AcquireBuffer(&slot);
    ReleaseBuffer(slot);

    // Abandon the BufferItemConsumer.
    mBIC->abandon();

    // Sleep to give some time for callbacks to happen.
    usleep(kFrameSleepUs);
    ASSERT_EQ(1, GetFreedBufferCount());
}

// Test that delete BufferItemConsumer triggers onBufferFreed.
TEST_F(BufferItemConsumerTest, TriggerBufferFreed_DeleteBufferItemConsumer) {
    int slot;
    // Let buffer go through the cycle at least once.
    DequeueBuffer(&slot);
    QueueBuffer(slot);
    AcquireBuffer(&slot);
    ReleaseBuffer(slot);

    // Delete the BufferItemConsumer.
    mBIC.clear();

    // Sleep to give some time for callbacks to happen.
    usleep(kFrameSleepUs);
    ASSERT_EQ(1, GetFreedBufferCount());
}

}  // namespace android
