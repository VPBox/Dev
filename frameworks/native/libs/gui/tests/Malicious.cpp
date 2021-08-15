/*
 * Copyright 2017 The Android Open Source Project
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

#include <gui/BufferQueue.h>
#include <gui/IProducerListener.h>
#include <gui/Surface.h>

#include <android/native_window.h>

#include <gtest/gtest.h>

namespace android {
namespace test {

class ProxyBQP : public BnGraphicBufferProducer {
public:
    explicit ProxyBQP(const sp<IGraphicBufferProducer>& producer) : mProducer(producer) {}

    // Pass through calls to mProducer
    status_t requestBuffer(int slot, sp<GraphicBuffer>* buf) override {
        return mProducer->requestBuffer(slot, buf);
    }
    status_t setMaxDequeuedBufferCount(int maxDequeuedBuffers) override {
        return mProducer->setMaxDequeuedBufferCount(maxDequeuedBuffers);
    }
    status_t setAsyncMode(bool async) override { return mProducer->setAsyncMode(async); }
    status_t dequeueBuffer(int* slot, sp<Fence>* fence, uint32_t w, uint32_t h, PixelFormat format,
                           uint64_t usage, uint64_t* outBufferAge,
                           FrameEventHistoryDelta* outTimestamps) override {
        return mProducer->dequeueBuffer(slot, fence, w, h, format, usage, outBufferAge,
                                        outTimestamps);
    }
    status_t detachBuffer(int slot) override { return mProducer->detachBuffer(slot); }
    status_t detachNextBuffer(sp<GraphicBuffer>* outBuffer, sp<Fence>* outFence) override {
        return mProducer->detachNextBuffer(outBuffer, outFence);
    }
    status_t attachBuffer(int* outSlot, const sp<GraphicBuffer>& buffer) override {
        return mProducer->attachBuffer(outSlot, buffer);
    }
    status_t queueBuffer(int slot, const QueueBufferInput& input,
                         QueueBufferOutput* output) override {
        return mProducer->queueBuffer(slot, input, output);
    }
    status_t cancelBuffer(int slot, const sp<Fence>& fence) override {
        return mProducer->cancelBuffer(slot, fence);
    }
    int query(int what, int* value) override { return mProducer->query(what, value); }
    status_t connect(const sp<IProducerListener>& listener, int api, bool producerControlledByApp,
                     QueueBufferOutput* output) override {
        return mProducer->connect(listener, api, producerControlledByApp, output);
    }
    status_t disconnect(int api, DisconnectMode mode) override {
        return mProducer->disconnect(api, mode);
    }
    status_t setSidebandStream(const sp<NativeHandle>& stream) override {
        return mProducer->setSidebandStream(stream);
    }
    void allocateBuffers(uint32_t width, uint32_t height, PixelFormat format,
                         uint64_t usage) override {
        mProducer->allocateBuffers(width, height, format, usage);
    }
    status_t allowAllocation(bool allow) override { return mProducer->allowAllocation(allow); }
    status_t setGenerationNumber(uint32_t generationNumber) override {
        return mProducer->setGenerationNumber(generationNumber);
    }
    String8 getConsumerName() const override { return mProducer->getConsumerName(); }
    status_t setSharedBufferMode(bool sharedBufferMode) override {
        return mProducer->setSharedBufferMode(sharedBufferMode);
    }
    status_t setAutoRefresh(bool autoRefresh) override {
        return mProducer->setAutoRefresh(autoRefresh);
    }
    status_t setDequeueTimeout(nsecs_t timeout) override {
        return mProducer->setDequeueTimeout(timeout);
    }
    status_t getLastQueuedBuffer(sp<GraphicBuffer>* outBuffer, sp<Fence>* outFence,
                                 float outTransformMatrix[16]) override {
        return mProducer->getLastQueuedBuffer(outBuffer, outFence, outTransformMatrix);
    }
    void getFrameTimestamps(FrameEventHistoryDelta*) override {}
    status_t getUniqueId(uint64_t* outId) const override { return mProducer->getUniqueId(outId); }
    status_t getConsumerUsage(uint64_t* outUsage) const override {
        return mProducer->getConsumerUsage(outUsage);
    }

protected:
    sp<IGraphicBufferProducer> mProducer;
};

class MaliciousBQP : public ProxyBQP {
public:
    explicit MaliciousBQP(const sp<IGraphicBufferProducer>& producer) : ProxyBQP(producer) {}

    void beMalicious(int32_t value) { mMaliciousValue = value; }

    void setExpectedSlot(int32_t slot) { mExpectedSlot = slot; }

    // Override dequeueBuffer, optionally corrupting the returned slot number
    status_t dequeueBuffer(int* buf, sp<Fence>* fence, uint32_t width, uint32_t height,
                           PixelFormat format, uint64_t usage, uint64_t* outBufferAge,
                           FrameEventHistoryDelta* outTimestamps) override {
        EXPECT_EQ(BUFFER_NEEDS_REALLOCATION,
                  mProducer->dequeueBuffer(buf, fence, width, height, format, usage, outBufferAge,
                                           outTimestamps));
        EXPECT_EQ(mExpectedSlot, *buf);
        if (mMaliciousValue != 0) {
            *buf = mMaliciousValue;
            return NO_ERROR;
        } else {
            return BUFFER_NEEDS_REALLOCATION;
        }
    }

private:
    int32_t mMaliciousValue = 0;
    int32_t mExpectedSlot = 0;
};

class DummyListener : public BnConsumerListener {
public:
    void onFrameAvailable(const BufferItem&) override {}
    void onBuffersReleased() override {}
    void onSidebandStreamChanged() override {}
};

sp<MaliciousBQP> getMaliciousBQP() {
    sp<IGraphicBufferProducer> producer;
    sp<IGraphicBufferConsumer> consumer;
    BufferQueue::createBufferQueue(&producer, &consumer);
    sp<IConsumerListener> listener = new DummyListener;
    consumer->consumerConnect(listener, false);

    sp<MaliciousBQP> malicious = new MaliciousBQP(producer);
    return malicious;
}

TEST(Malicious, Bug36991414Max) {
    sp<MaliciousBQP> malicious = getMaliciousBQP();
    sp<Surface> surface = new Surface(malicious);

    ASSERT_EQ(NO_ERROR, surface->connect(NATIVE_WINDOW_API_CPU, nullptr, false));
    ANativeWindow_Buffer buffer;
    ASSERT_EQ(NO_ERROR, surface->lock(&buffer, nullptr));
    ASSERT_EQ(NO_ERROR, surface->unlockAndPost());

    malicious->setExpectedSlot(1);
    malicious->beMalicious(std::numeric_limits<int32_t>::max());
    ASSERT_EQ(FAILED_TRANSACTION, surface->lock(&buffer, nullptr));
}

TEST(Malicious, Bug36991414Min) {
    sp<MaliciousBQP> malicious = getMaliciousBQP();
    sp<Surface> surface = new Surface(malicious);

    ASSERT_EQ(NO_ERROR, surface->connect(NATIVE_WINDOW_API_CPU, nullptr, false));
    ANativeWindow_Buffer buffer;
    ASSERT_EQ(NO_ERROR, surface->lock(&buffer, nullptr));
    ASSERT_EQ(NO_ERROR, surface->unlockAndPost());

    malicious->setExpectedSlot(1);
    malicious->beMalicious(std::numeric_limits<int32_t>::min());
    ASSERT_EQ(FAILED_TRANSACTION, surface->lock(&buffer, nullptr));
}

TEST(Malicious, Bug36991414NegativeOne) {
    sp<MaliciousBQP> malicious = getMaliciousBQP();
    sp<Surface> surface = new Surface(malicious);

    ASSERT_EQ(NO_ERROR, surface->connect(NATIVE_WINDOW_API_CPU, nullptr, false));
    ANativeWindow_Buffer buffer;
    ASSERT_EQ(NO_ERROR, surface->lock(&buffer, nullptr));
    ASSERT_EQ(NO_ERROR, surface->unlockAndPost());

    malicious->setExpectedSlot(1);
    malicious->beMalicious(-1);
    ASSERT_EQ(FAILED_TRANSACTION, surface->lock(&buffer, nullptr));
}

TEST(Malicious, Bug36991414NumSlots) {
    sp<MaliciousBQP> malicious = getMaliciousBQP();
    sp<Surface> surface = new Surface(malicious);

    ASSERT_EQ(NO_ERROR, surface->connect(NATIVE_WINDOW_API_CPU, nullptr, false));
    ANativeWindow_Buffer buffer;
    ASSERT_EQ(NO_ERROR, surface->lock(&buffer, nullptr));
    ASSERT_EQ(NO_ERROR, surface->unlockAndPost());

    malicious->setExpectedSlot(1);
    malicious->beMalicious(BufferQueueDefs::NUM_BUFFER_SLOTS);
    ASSERT_EQ(FAILED_TRANSACTION, surface->lock(&buffer, nullptr));
}

} // namespace test
} // namespace android
