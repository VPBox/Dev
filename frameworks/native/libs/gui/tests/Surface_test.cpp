/*
 * Copyright (C) 2011 The Android Open Source Project
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

#include "DummyConsumer.h"

#include <gtest/gtest.h>

#include <android/hardware/configstore/1.0/ISurfaceFlingerConfigs.h>
#include <binder/ProcessState.h>
#include <configstore/Utils.h>
#include <cutils/properties.h>
#include <inttypes.h>
#include <gui/BufferItemConsumer.h>
#include <gui/IDisplayEventConnection.h>
#include <gui/IProducerListener.h>
#include <gui/ISurfaceComposer.h>
#include <gui/Surface.h>
#include <gui/SurfaceComposerClient.h>
#include <private/gui/ComposerService.h>
#include <ui/Rect.h>
#include <utils/String8.h>

#include <limits>
#include <thread>

namespace android {

using namespace std::chrono_literals;
// retrieve wide-color and hdr settings from configstore
using namespace android::hardware::configstore;
using namespace android::hardware::configstore::V1_0;
using ui::ColorMode;

using Transaction = SurfaceComposerClient::Transaction;

static bool hasWideColorDisplay =
        getBool<ISurfaceFlingerConfigs, &ISurfaceFlingerConfigs::hasWideColorDisplay>(false);

static bool hasHdrDisplay =
        getBool<ISurfaceFlingerConfigs, &ISurfaceFlingerConfigs::hasHDRDisplay>(false);

class FakeSurfaceComposer;
class FakeProducerFrameEventHistory;

static constexpr uint64_t NO_FRAME_INDEX = std::numeric_limits<uint64_t>::max();

class DummySurfaceListener : public SurfaceListener {
public:
    DummySurfaceListener(bool enableReleasedCb = false) :
            mEnableReleaseCb(enableReleasedCb),
            mBuffersReleased(0) {}
    virtual ~DummySurfaceListener() = default;

    virtual void onBufferReleased() {
        mBuffersReleased++;
    }
    virtual bool needsReleaseNotify() {
        return mEnableReleaseCb;
    }
    virtual void onBuffersDiscarded(const std::vector<sp<GraphicBuffer>>& buffers) {
        mDiscardedBuffers.insert(mDiscardedBuffers.end(), buffers.begin(), buffers.end());
    }

    int getReleaseNotifyCount() const {
        return mBuffersReleased;
    }
    const std::vector<sp<GraphicBuffer>>& getDiscardedBuffers() const {
        return mDiscardedBuffers;
    }
private:
    // No need to use lock given the test triggers the listener in the same
    // thread context.
    bool mEnableReleaseCb;
    int32_t mBuffersReleased;
    std::vector<sp<GraphicBuffer>> mDiscardedBuffers;
};

class SurfaceTest : public ::testing::Test {
protected:
    SurfaceTest() {
        ProcessState::self()->startThreadPool();
    }

    virtual void SetUp() {
        mComposerClient = new SurfaceComposerClient;
        ASSERT_EQ(NO_ERROR, mComposerClient->initCheck());

        // TODO(brianderson): The following sometimes fails and is a source of
        //   test flakiness.
        mSurfaceControl = mComposerClient->createSurface(
                String8("Test Surface"), 32, 32, PIXEL_FORMAT_RGBA_8888, 0);

        ASSERT_TRUE(mSurfaceControl != nullptr);
        ASSERT_TRUE(mSurfaceControl->isValid());

        Transaction t;
        ASSERT_EQ(NO_ERROR, t.setLayer(mSurfaceControl, 0x7fffffff)
                .show(mSurfaceControl)
                .apply());

        mSurface = mSurfaceControl->getSurface();
        ASSERT_TRUE(mSurface != nullptr);
    }

    virtual void TearDown() {
        mComposerClient->dispose();
    }

    void testSurfaceListener(bool hasSurfaceListener, bool enableReleasedCb,
            int32_t extraDiscardedBuffers) {
        sp<IGraphicBufferProducer> producer;
        sp<IGraphicBufferConsumer> consumer;
        BufferQueue::createBufferQueue(&producer, &consumer);

        sp<DummyConsumer> dummyConsumer(new DummyConsumer);
        consumer->consumerConnect(dummyConsumer, false);
        consumer->setConsumerName(String8("TestConsumer"));

        sp<Surface> surface = new Surface(producer);
        sp<ANativeWindow> window(surface);
        sp<DummySurfaceListener> listener;
        if (hasSurfaceListener) {
            listener = new DummySurfaceListener(enableReleasedCb);
        }
        ASSERT_EQ(OK, surface->connect(
                NATIVE_WINDOW_API_CPU,
                /*reportBufferRemoval*/true,
                /*listener*/listener));
        const int BUFFER_COUNT = 4 + extraDiscardedBuffers;
        ASSERT_EQ(NO_ERROR, native_window_set_buffer_count(window.get(), BUFFER_COUNT));

        ANativeWindowBuffer* buffers[BUFFER_COUNT];
        // Dequeue first to allocate a number of buffers
        for (int i = 0; i < BUFFER_COUNT; i++) {
            ASSERT_EQ(NO_ERROR, native_window_dequeue_buffer_and_wait(window.get(), &buffers[i]));
        }
        for (int i = 0; i < BUFFER_COUNT; i++) {
            ASSERT_EQ(NO_ERROR, window->cancelBuffer(window.get(), buffers[i], -1));
        }

        ANativeWindowBuffer* buffer;
        // Fill BUFFER_COUNT-1 buffers
        for (int i = 0; i < BUFFER_COUNT-1; i++) {
            ASSERT_EQ(NO_ERROR, native_window_dequeue_buffer_and_wait(window.get(), &buffer));
            ASSERT_EQ(NO_ERROR, window->queueBuffer(window.get(), buffer, -1));
        }

        // Dequeue 1 buffer
        ASSERT_EQ(NO_ERROR, native_window_dequeue_buffer_and_wait(window.get(), &buffer));

        // Acquire and free 1+extraDiscardedBuffers buffer, check onBufferReleased is called.
        std::vector<BufferItem> releasedItems;
        releasedItems.resize(1+extraDiscardedBuffers);
        for (int i = 0; i < releasedItems.size(); i++) {
            ASSERT_EQ(NO_ERROR, consumer->acquireBuffer(&releasedItems[i], 0));
            ASSERT_EQ(NO_ERROR, consumer->releaseBuffer(releasedItems[i].mSlot,
                    releasedItems[i].mFrameNumber, EGL_NO_DISPLAY, EGL_NO_SYNC_KHR,
                    Fence::NO_FENCE));
        }
        int32_t expectedReleaseCb = (enableReleasedCb ? releasedItems.size() : 0);
        if (hasSurfaceListener) {
            ASSERT_EQ(expectedReleaseCb, listener->getReleaseNotifyCount());
        }

        // Acquire 1 buffer, leaving 1+extraDiscardedBuffers filled buffer in queue
        BufferItem item;
        ASSERT_EQ(NO_ERROR, consumer->acquireBuffer(&item, 0));

        // Discard free buffers
        ASSERT_EQ(NO_ERROR, consumer->discardFreeBuffers());

        if (hasSurfaceListener) {
            ASSERT_EQ(expectedReleaseCb, listener->getReleaseNotifyCount());

            // Check onBufferDiscarded is called with correct buffer
            auto discardedBuffers = listener->getDiscardedBuffers();
            ASSERT_EQ(discardedBuffers.size(), releasedItems.size());
            for (int i = 0; i < releasedItems.size(); i++) {
                ASSERT_EQ(discardedBuffers[i], releasedItems[i].mGraphicBuffer);
            }

            ASSERT_EQ(expectedReleaseCb, listener->getReleaseNotifyCount());
        }

        // Disconnect the surface
        ASSERT_EQ(NO_ERROR, surface->disconnect(NATIVE_WINDOW_API_CPU));
    }

    sp<Surface> mSurface;
    sp<SurfaceComposerClient> mComposerClient;
    sp<SurfaceControl> mSurfaceControl;
};

TEST_F(SurfaceTest, CreateSurfaceReturnsErrorBadClient) {
    mComposerClient->dispose();
    ASSERT_EQ(NO_INIT, mComposerClient->initCheck());

    sp<SurfaceControl> sc;
    status_t err = mComposerClient->createSurfaceChecked(
            String8("Test Surface"), 32, 32, PIXEL_FORMAT_RGBA_8888, &sc, 0);
    ASSERT_EQ(NO_INIT, err);
}

TEST_F(SurfaceTest, QueuesToWindowComposerIsTrueWhenVisible) {
    sp<ANativeWindow> anw(mSurface);
    int result = -123;
    int err = anw->query(anw.get(), NATIVE_WINDOW_QUEUES_TO_WINDOW_COMPOSER,
            &result);
    EXPECT_EQ(NO_ERROR, err);
    EXPECT_EQ(1, result);
}

TEST_F(SurfaceTest, QueuesToWindowComposerIsTrueWhenPurgatorized) {
    mSurfaceControl.clear();
    // Wait for the async clean-up to complete.
    std::this_thread::sleep_for(50ms);

    sp<ANativeWindow> anw(mSurface);
    int result = -123;
    int err = anw->query(anw.get(), NATIVE_WINDOW_QUEUES_TO_WINDOW_COMPOSER,
            &result);
    EXPECT_EQ(NO_ERROR, err);
    EXPECT_EQ(1, result);
}

// This test probably doesn't belong here.
TEST_F(SurfaceTest, ScreenshotsOfProtectedBuffersDontSucceed) {
    sp<ANativeWindow> anw(mSurface);

    // Verify the screenshot works with no protected buffers.
    sp<ISurfaceComposer> sf(ComposerService::getComposerService());

    const sp<IBinder> display = sf->getInternalDisplayToken();
    ASSERT_FALSE(display == nullptr);

    sp<GraphicBuffer> outBuffer;
    bool ignored;
    ASSERT_EQ(NO_ERROR,
              sf->captureScreen(display, &outBuffer, ignored, ui::Dataspace::V0_SRGB,
                                ui::PixelFormat::RGBA_8888, Rect(), 64, 64, false));

    ASSERT_EQ(NO_ERROR, native_window_api_connect(anw.get(),
            NATIVE_WINDOW_API_CPU));
    // Set the PROTECTED usage bit and verify that the screenshot fails.  Note
    // that we need to dequeue a buffer in order for it to actually get
    // allocated in SurfaceFlinger.
    ASSERT_EQ(NO_ERROR, native_window_set_usage(anw.get(),
            GRALLOC_USAGE_PROTECTED));
    ASSERT_EQ(NO_ERROR, native_window_set_buffer_count(anw.get(), 3));
    ANativeWindowBuffer* buf = nullptr;

    status_t err = native_window_dequeue_buffer_and_wait(anw.get(), &buf);
    if (err) {
        // we could fail if GRALLOC_USAGE_PROTECTED is not supported.
        // that's okay as long as this is the reason for the failure.
        // try again without the GRALLOC_USAGE_PROTECTED bit.
        ASSERT_EQ(NO_ERROR, native_window_set_usage(anw.get(), 0));
        ASSERT_EQ(NO_ERROR, native_window_dequeue_buffer_and_wait(anw.get(),
                &buf));
        return;
    }
    ASSERT_EQ(NO_ERROR, anw->cancelBuffer(anw.get(), buf, -1));

    for (int i = 0; i < 4; i++) {
        // Loop to make sure SurfaceFlinger has retired a protected buffer.
        ASSERT_EQ(NO_ERROR, native_window_dequeue_buffer_and_wait(anw.get(),
                &buf));
        ASSERT_EQ(NO_ERROR, anw->queueBuffer(anw.get(), buf, -1));
    }
    ASSERT_EQ(NO_ERROR,
              sf->captureScreen(display, &outBuffer, ignored, ui::Dataspace::V0_SRGB,
                                ui::PixelFormat::RGBA_8888, Rect(), 64, 64, false));
}

TEST_F(SurfaceTest, ConcreteTypeIsSurface) {
    sp<ANativeWindow> anw(mSurface);
    int result = -123;
    int err = anw->query(anw.get(), NATIVE_WINDOW_CONCRETE_TYPE, &result);
    EXPECT_EQ(NO_ERROR, err);
    EXPECT_EQ(NATIVE_WINDOW_SURFACE, result);
}

TEST_F(SurfaceTest, LayerCountIsOne) {
    sp<ANativeWindow> anw(mSurface);
    int result = -123;
    int err = anw->query(anw.get(), NATIVE_WINDOW_LAYER_COUNT, &result);
    EXPECT_EQ(NO_ERROR, err);
    EXPECT_EQ(1, result);
}

TEST_F(SurfaceTest, QueryConsumerUsage) {
    const int TEST_USAGE_FLAGS =
            GRALLOC_USAGE_SW_READ_OFTEN | GRALLOC_USAGE_HW_RENDER;
    sp<IGraphicBufferProducer> producer;
    sp<IGraphicBufferConsumer> consumer;
    BufferQueue::createBufferQueue(&producer, &consumer);
    sp<BufferItemConsumer> c = new BufferItemConsumer(consumer,
            TEST_USAGE_FLAGS);
    sp<Surface> s = new Surface(producer);

    sp<ANativeWindow> anw(s);

    int flags = -1;
    int err = anw->query(anw.get(), NATIVE_WINDOW_CONSUMER_USAGE_BITS, &flags);

    ASSERT_EQ(NO_ERROR, err);
    ASSERT_EQ(TEST_USAGE_FLAGS, flags);
}

TEST_F(SurfaceTest, QueryDefaultBuffersDataSpace) {
    const android_dataspace TEST_DATASPACE = HAL_DATASPACE_V0_SRGB;
    sp<IGraphicBufferProducer> producer;
    sp<IGraphicBufferConsumer> consumer;
    BufferQueue::createBufferQueue(&producer, &consumer);
    sp<CpuConsumer> cpuConsumer = new CpuConsumer(consumer, 1);

    cpuConsumer->setDefaultBufferDataSpace(TEST_DATASPACE);

    sp<Surface> s = new Surface(producer);

    sp<ANativeWindow> anw(s);

    android_dataspace dataSpace;

    int err = anw->query(anw.get(), NATIVE_WINDOW_DEFAULT_DATASPACE,
            reinterpret_cast<int*>(&dataSpace));

    ASSERT_EQ(NO_ERROR, err);
    ASSERT_EQ(TEST_DATASPACE, dataSpace);
}

TEST_F(SurfaceTest, SettingGenerationNumber) {
    sp<IGraphicBufferProducer> producer;
    sp<IGraphicBufferConsumer> consumer;
    BufferQueue::createBufferQueue(&producer, &consumer);
    sp<CpuConsumer> cpuConsumer = new CpuConsumer(consumer, 1);
    sp<Surface> surface = new Surface(producer);
    sp<ANativeWindow> window(surface);

    // Allocate a buffer with a generation number of 0
    ANativeWindowBuffer* buffer;
    int fenceFd;
    ASSERT_EQ(NO_ERROR, native_window_api_connect(window.get(),
            NATIVE_WINDOW_API_CPU));
    ASSERT_EQ(NO_ERROR, window->dequeueBuffer(window.get(), &buffer, &fenceFd));
    ASSERT_EQ(NO_ERROR, window->cancelBuffer(window.get(), buffer, fenceFd));

    // Detach the buffer and check its generation number
    sp<GraphicBuffer> graphicBuffer;
    sp<Fence> fence;
    ASSERT_EQ(NO_ERROR, surface->detachNextBuffer(&graphicBuffer, &fence));
    ASSERT_EQ(0U, graphicBuffer->getGenerationNumber());

    ASSERT_EQ(NO_ERROR, surface->setGenerationNumber(1));
    buffer = static_cast<ANativeWindowBuffer*>(graphicBuffer.get());

    // This should change the generation number of the GraphicBuffer
    ASSERT_EQ(NO_ERROR, surface->attachBuffer(buffer));

    // Check that the new generation number sticks with the buffer
    ASSERT_EQ(NO_ERROR, window->cancelBuffer(window.get(), buffer, -1));
    ASSERT_EQ(NO_ERROR, window->dequeueBuffer(window.get(), &buffer, &fenceFd));
    graphicBuffer = static_cast<GraphicBuffer*>(buffer);
    ASSERT_EQ(1U, graphicBuffer->getGenerationNumber());
}

TEST_F(SurfaceTest, GetConsumerName) {
    sp<IGraphicBufferProducer> producer;
    sp<IGraphicBufferConsumer> consumer;
    BufferQueue::createBufferQueue(&producer, &consumer);

    sp<DummyConsumer> dummyConsumer(new DummyConsumer);
    consumer->consumerConnect(dummyConsumer, false);
    consumer->setConsumerName(String8("TestConsumer"));

    sp<Surface> surface = new Surface(producer);
    sp<ANativeWindow> window(surface);
    native_window_api_connect(window.get(), NATIVE_WINDOW_API_CPU);

    EXPECT_STREQ("TestConsumer", surface->getConsumerName().string());
}

TEST_F(SurfaceTest, GetWideColorSupport) {
    sp<IGraphicBufferProducer> producer;
    sp<IGraphicBufferConsumer> consumer;
    BufferQueue::createBufferQueue(&producer, &consumer);

    sp<DummyConsumer> dummyConsumer(new DummyConsumer);
    consumer->consumerConnect(dummyConsumer, false);
    consumer->setConsumerName(String8("TestConsumer"));

    sp<Surface> surface = new Surface(producer);
    sp<ANativeWindow> window(surface);
    native_window_api_connect(window.get(), NATIVE_WINDOW_API_CPU);

    bool supported;
    surface->getWideColorSupport(&supported);

    // NOTE: This test assumes that device that supports
    // wide-color (as indicated by BoardConfig) must also
    // have a wide-color primary display.
    // That assumption allows this test to cover devices
    // that advertised a wide-color color mode without
    // actually supporting wide-color to pass this test
    // as well as the case of a device that does support
    // wide-color (via BoardConfig) and has a wide-color
    // primary display.
    // NOT covered at this time is a device that supports
    // wide color in the BoardConfig but does not support
    // a wide-color color mode on the primary display.
    ASSERT_EQ(hasWideColorDisplay, supported);
}

TEST_F(SurfaceTest, GetHdrSupport) {
    sp<IGraphicBufferProducer> producer;
    sp<IGraphicBufferConsumer> consumer;
    BufferQueue::createBufferQueue(&producer, &consumer);

    sp<DummyConsumer> dummyConsumer(new DummyConsumer);
    consumer->consumerConnect(dummyConsumer, false);
    consumer->setConsumerName(String8("TestConsumer"));

    sp<Surface> surface = new Surface(producer);
    sp<ANativeWindow> window(surface);
    native_window_api_connect(window.get(), NATIVE_WINDOW_API_CPU);

    bool supported;
    status_t result = surface->getHdrSupport(&supported);
    ASSERT_EQ(NO_ERROR, result);

    // NOTE: This is not a CTS test.
    // This test verifies that when the BoardConfig TARGET_HAS_HDR_DISPLAY
    // is TRUE, getHdrSupport is also true.
    // TODO: Add check for an HDR color mode on the primary display.
    ASSERT_EQ(hasHdrDisplay, supported);
}

TEST_F(SurfaceTest, SetHdrMetadata) {
    sp<IGraphicBufferProducer> producer;
    sp<IGraphicBufferConsumer> consumer;
    BufferQueue::createBufferQueue(&producer, &consumer);

    sp<DummyConsumer> dummyConsumer(new DummyConsumer);
    consumer->consumerConnect(dummyConsumer, false);
    consumer->setConsumerName(String8("TestConsumer"));

    sp<Surface> surface = new Surface(producer);
    sp<ANativeWindow> window(surface);
    native_window_api_connect(window.get(), NATIVE_WINDOW_API_CPU);

    bool supported;
    status_t result = surface->getHdrSupport(&supported);
    ASSERT_EQ(NO_ERROR, result);

    if (!hasHdrDisplay || !supported) {
        return;
    }
    const android_smpte2086_metadata smpte2086 = {
        {0.680, 0.320},
        {0.265, 0.690},
        {0.150, 0.060},
        {0.3127, 0.3290},
        100.0,
        0.1,
    };
    const android_cta861_3_metadata cta861_3 = {
        78.0,
        62.0,
    };

    std::vector<uint8_t> hdr10plus;
    hdr10plus.push_back(0xff);

    int error = native_window_set_buffers_smpte2086_metadata(window.get(), &smpte2086);
    ASSERT_EQ(error, NO_ERROR);
    error = native_window_set_buffers_cta861_3_metadata(window.get(), &cta861_3);
    ASSERT_EQ(error, NO_ERROR);
    error = native_window_set_buffers_hdr10_plus_metadata(window.get(), hdr10plus.size(),
                                                          hdr10plus.data());
    ASSERT_EQ(error, NO_ERROR);
}

TEST_F(SurfaceTest, DynamicSetBufferCount) {
    sp<IGraphicBufferProducer> producer;
    sp<IGraphicBufferConsumer> consumer;
    BufferQueue::createBufferQueue(&producer, &consumer);

    sp<DummyConsumer> dummyConsumer(new DummyConsumer);
    consumer->consumerConnect(dummyConsumer, false);
    consumer->setConsumerName(String8("TestConsumer"));

    sp<Surface> surface = new Surface(producer);
    sp<ANativeWindow> window(surface);

    ASSERT_EQ(NO_ERROR, native_window_api_connect(window.get(),
            NATIVE_WINDOW_API_CPU));
    native_window_set_buffer_count(window.get(), 4);

    int fence;
    ANativeWindowBuffer* buffer;
    ASSERT_EQ(NO_ERROR, window->dequeueBuffer(window.get(), &buffer, &fence));
    native_window_set_buffer_count(window.get(), 3);
    ASSERT_EQ(NO_ERROR, window->queueBuffer(window.get(), buffer, fence));
    native_window_set_buffer_count(window.get(), 2);
    ASSERT_EQ(NO_ERROR, window->dequeueBuffer(window.get(), &buffer, &fence));
    ASSERT_EQ(NO_ERROR, window->queueBuffer(window.get(), buffer, fence));
}

TEST_F(SurfaceTest, GetAndFlushRemovedBuffers) {
    sp<IGraphicBufferProducer> producer;
    sp<IGraphicBufferConsumer> consumer;
    BufferQueue::createBufferQueue(&producer, &consumer);

    sp<DummyConsumer> dummyConsumer(new DummyConsumer);
    consumer->consumerConnect(dummyConsumer, false);
    consumer->setConsumerName(String8("TestConsumer"));

    sp<Surface> surface = new Surface(producer);
    sp<ANativeWindow> window(surface);
    sp<DummyProducerListener> listener = new DummyProducerListener();
    ASSERT_EQ(OK, surface->connect(
            NATIVE_WINDOW_API_CPU,
            /*listener*/listener,
            /*reportBufferRemoval*/true));
    const int BUFFER_COUNT = 4;
    ASSERT_EQ(NO_ERROR, native_window_set_buffer_count(window.get(), BUFFER_COUNT));

    sp<GraphicBuffer> detachedBuffer;
    sp<Fence> outFence;
    int fences[BUFFER_COUNT];
    ANativeWindowBuffer* buffers[BUFFER_COUNT];
    // Allocate buffers because detachNextBuffer requires allocated buffers
    for (int i = 0; i < BUFFER_COUNT; i++) {
        ASSERT_EQ(NO_ERROR, window->dequeueBuffer(window.get(), &buffers[i], &fences[i]));
    }
    for (int i = 0; i < BUFFER_COUNT; i++) {
        ASSERT_EQ(NO_ERROR, window->cancelBuffer(window.get(), buffers[i], fences[i]));
    }

    // Test detached buffer is correctly reported
    ASSERT_EQ(NO_ERROR, surface->detachNextBuffer(&detachedBuffer, &outFence));
    std::vector<sp<GraphicBuffer>> removedBuffers;
    ASSERT_EQ(OK, surface->getAndFlushRemovedBuffers(&removedBuffers));
    ASSERT_EQ(1u, removedBuffers.size());
    ASSERT_EQ(detachedBuffer->handle, removedBuffers.at(0)->handle);
    // Test the list is flushed one getAndFlushRemovedBuffers returns
    ASSERT_EQ(OK, surface->getAndFlushRemovedBuffers(&removedBuffers));
    ASSERT_EQ(0u, removedBuffers.size());


    // Test removed buffer list is cleanup after next dequeueBuffer call
    ASSERT_EQ(NO_ERROR, surface->detachNextBuffer(&detachedBuffer, &outFence));
    ASSERT_EQ(NO_ERROR, window->dequeueBuffer(window.get(), &buffers[0], &fences[0]));
    ASSERT_EQ(OK, surface->getAndFlushRemovedBuffers(&removedBuffers));
    ASSERT_EQ(0u, removedBuffers.size());
    ASSERT_EQ(NO_ERROR, window->cancelBuffer(window.get(), buffers[0], fences[0]));

    // Test removed buffer list is cleanup after next detachNextBuffer call
    ASSERT_EQ(NO_ERROR, surface->detachNextBuffer(&detachedBuffer, &outFence));
    ASSERT_EQ(NO_ERROR, surface->detachNextBuffer(&detachedBuffer, &outFence));
    ASSERT_EQ(OK, surface->getAndFlushRemovedBuffers(&removedBuffers));
    ASSERT_EQ(1u, removedBuffers.size());
    ASSERT_EQ(detachedBuffer->handle, removedBuffers.at(0)->handle);

    // Re-allocate buffers since all buffers are detached up to now
    for (int i = 0; i < BUFFER_COUNT; i++) {
        ASSERT_EQ(NO_ERROR, window->dequeueBuffer(window.get(), &buffers[i], &fences[i]));
    }
    for (int i = 0; i < BUFFER_COUNT; i++) {
        ASSERT_EQ(NO_ERROR, window->cancelBuffer(window.get(), buffers[i], fences[i]));
    }

    ASSERT_EQ(NO_ERROR, surface->detachNextBuffer(&detachedBuffer, &outFence));
    ASSERT_EQ(NO_ERROR, surface->attachBuffer(detachedBuffer.get()));
    ASSERT_EQ(OK, surface->getAndFlushRemovedBuffers(&removedBuffers));
    // Depends on which slot GraphicBufferProducer impl pick, the attach call might
    // get 0 or 1 buffer removed.
    ASSERT_LE(removedBuffers.size(), 1u);
}

TEST_F(SurfaceTest, SurfaceListenerTest) {
    // Test discarding 1 free buffers with no listener
    testSurfaceListener(/*hasListener*/false, /*enableReleaseCb*/false, /*extraDiscardedBuffers*/0);
    // Test discarding 2 free buffers with no listener
    testSurfaceListener(/*hasListener*/false, /*enableReleaseCb*/false, /*extraDiscardedBuffers*/1);
    // Test discarding 1 free buffers with a listener, disabling onBufferReleased
    testSurfaceListener(/*hasListener*/true, /*enableReleasedCb*/false, /*extraDiscardedBuffers*/0);
    // Test discarding 2 free buffers with a listener, disabling onBufferReleased
    testSurfaceListener(/*hasListener*/true, /*enableReleasedCb*/false, /*extraDiscardedBuffers*/1);
    // Test discarding 1 free buffers with a listener, enabling onBufferReleased
    testSurfaceListener(/*hasListener*/true, /*enableReleasedCb*/true, /*extraDiscardedBuffers*/0);
    // Test discarding 3 free buffers with a listener, enabling onBufferReleased
    testSurfaceListener(/*hasListener*/true, /*enableReleasedCb*/true, /*extraDiscardedBuffers*/2);
}

TEST_F(SurfaceTest, TestGetLastDequeueStartTime) {
    sp<ANativeWindow> anw(mSurface);
    ASSERT_EQ(NO_ERROR, native_window_api_connect(anw.get(), NATIVE_WINDOW_API_CPU));

    ANativeWindowBuffer* buffer = nullptr;
    int32_t fenceFd = -1;

    nsecs_t before = systemTime(CLOCK_MONOTONIC);
    anw->dequeueBuffer(anw.get(), &buffer, &fenceFd);
    nsecs_t after = systemTime(CLOCK_MONOTONIC);

    nsecs_t lastDequeueTime = mSurface->getLastDequeueStartTime();
    ASSERT_LE(before, lastDequeueTime);
    ASSERT_GE(after, lastDequeueTime);
}

class FakeConsumer : public BnConsumerListener {
public:
    void onFrameAvailable(const BufferItem& /*item*/) override {}
    void onBuffersReleased() override {}
    void onSidebandStreamChanged() override {}

    void addAndGetFrameTimestamps(
            const NewFrameEventsEntry* newTimestamps,
            FrameEventHistoryDelta* outDelta) override {
        if (newTimestamps) {
            if (mGetFrameTimestampsEnabled) {
                EXPECT_GT(mNewFrameEntryOverride.frameNumber, 0u) <<
                        "Test should set mNewFrameEntryOverride before queuing "
                        "a frame.";
                EXPECT_EQ(newTimestamps->frameNumber,
                        mNewFrameEntryOverride.frameNumber) <<
                        "Test attempting to add NewFrameEntryOverride with "
                        "incorrect frame number.";
                mFrameEventHistory.addQueue(mNewFrameEntryOverride);
                mNewFrameEntryOverride.frameNumber = 0;
            }
            mAddFrameTimestampsCount++;
            mLastAddedFrameNumber = newTimestamps->frameNumber;
        }
        if (outDelta) {
            mFrameEventHistory.getAndResetDelta(outDelta);
            mGetFrameTimestampsCount++;
        }
        mAddAndGetFrameTimestampsCallCount++;
    }

    bool mGetFrameTimestampsEnabled = false;

    ConsumerFrameEventHistory mFrameEventHistory;
    int mAddAndGetFrameTimestampsCallCount = 0;
    int mAddFrameTimestampsCount = 0;
    int mGetFrameTimestampsCount = 0;
    uint64_t mLastAddedFrameNumber = NO_FRAME_INDEX;

    NewFrameEventsEntry mNewFrameEntryOverride = { 0, 0, 0, nullptr };
};


class FakeSurfaceComposer : public ISurfaceComposer{
public:
    ~FakeSurfaceComposer() override {}

    void setSupportsPresent(bool supportsPresent) {
        mSupportsPresent = supportsPresent;
    }

    sp<ISurfaceComposerClient> createConnection() override { return nullptr; }
    sp<IDisplayEventConnection> createDisplayEventConnection(
            ISurfaceComposer::VsyncSource, ISurfaceComposer::ConfigChanged) override {
        return nullptr;
    }
    sp<IBinder> createDisplay(const String8& /*displayName*/,
            bool /*secure*/) override { return nullptr; }
    void destroyDisplay(const sp<IBinder>& /*display */) override {}
    std::vector<PhysicalDisplayId> getPhysicalDisplayIds() const override { return {}; }
    sp<IBinder> getPhysicalDisplayToken(PhysicalDisplayId) const override { return nullptr; }
    void setTransactionState(const Vector<ComposerState>& /*state*/,
                             const Vector<DisplayState>& /*displays*/, uint32_t /*flags*/,
                             const sp<IBinder>& /*applyToken*/,
                             const InputWindowCommands& /*inputWindowCommands*/,
                             int64_t /*desiredPresentTime*/, const client_cache_t& /*cachedBuffer*/,
                             const std::vector<ListenerCallbacks>& /*listenerCallbacks*/) override {
    }

    void bootFinished() override {}
    bool authenticateSurfaceTexture(
            const sp<IGraphicBufferProducer>& /*surface*/) const override {
        return false;
    }

    status_t getSupportedFrameTimestamps(std::vector<FrameEvent>* outSupported)
            const override {
        *outSupported = {
                FrameEvent::REQUESTED_PRESENT,
                FrameEvent::ACQUIRE,
                FrameEvent::LATCH,
                FrameEvent::FIRST_REFRESH_START,
                FrameEvent::LAST_REFRESH_START,
                FrameEvent::GPU_COMPOSITION_DONE,
                FrameEvent::DEQUEUE_READY,
                FrameEvent::RELEASE
        };
        if (mSupportsPresent) {
            outSupported->push_back(
                        FrameEvent::DISPLAY_PRESENT);
        }
        return NO_ERROR;
    }

    void setPowerMode(const sp<IBinder>& /*display*/, int /*mode*/) override {}
    status_t getDisplayConfigs(const sp<IBinder>& /*display*/,
            Vector<DisplayInfo>* /*configs*/) override { return NO_ERROR; }
    status_t getDisplayStats(const sp<IBinder>& /*display*/,
            DisplayStatInfo* /*stats*/) override { return NO_ERROR; }
    int getActiveConfig(const sp<IBinder>& /*display*/) override { return 0; }
    status_t setActiveConfig(const sp<IBinder>& /*display*/, int /*id*/)
            override {
        return NO_ERROR;
    }
    status_t getDisplayColorModes(const sp<IBinder>& /*display*/,
            Vector<ColorMode>* /*outColorModes*/) override {
        return NO_ERROR;
    }
    status_t getDisplayNativePrimaries(const sp<IBinder>& /*display*/,
            ui::DisplayPrimaries& /*primaries*/) override {
        return NO_ERROR;
    }
    ColorMode getActiveColorMode(const sp<IBinder>& /*display*/)
            override {
        return ColorMode::NATIVE;
    }
    status_t setActiveColorMode(const sp<IBinder>& /*display*/,
        ColorMode /*colorMode*/) override { return NO_ERROR; }
    status_t captureScreen(const sp<IBinder>& /*display*/, sp<GraphicBuffer>* /*outBuffer*/,
                           bool& /* outCapturedSecureLayers */,
                           const ui::Dataspace /*reqDataspace*/,
                           const ui::PixelFormat /*reqPixelFormat*/, Rect /*sourceCrop*/,
                           uint32_t /*reqWidth*/, uint32_t /*reqHeight*/,
                           bool /*useIdentityTransform*/, Rotation /*rotation*/,
                           bool /*captureSecureLayers*/) override {
        return NO_ERROR;
    }
    status_t captureScreen(uint64_t /*displayOrLayerStack*/, ui::Dataspace* /*outDataspace*/,
                           sp<GraphicBuffer>* /*outBuffer*/) override {
        return NO_ERROR;
    }
    virtual status_t captureLayers(
            const sp<IBinder>& /*parentHandle*/, sp<GraphicBuffer>* /*outBuffer*/,
            const ui::Dataspace /*reqDataspace*/, const ui::PixelFormat /*reqPixelFormat*/,
            const Rect& /*sourceCrop*/,
            const std::unordered_set<sp<IBinder>,
                                     ISurfaceComposer::SpHash<IBinder>>& /*excludeHandles*/,
            float /*frameScale*/, bool /*childrenOnly*/) override {
        return NO_ERROR;
    }
    status_t clearAnimationFrameStats() override { return NO_ERROR; }
    status_t getAnimationFrameStats(FrameStats* /*outStats*/) const override {
        return NO_ERROR;
    }
    status_t getHdrCapabilities(const sp<IBinder>& /*display*/,
            HdrCapabilities* /*outCapabilities*/) const override {
        return NO_ERROR;
    }
    status_t enableVSyncInjections(bool /*enable*/) override {
        return NO_ERROR;
    }
    status_t injectVSync(nsecs_t /*when*/) override { return NO_ERROR; }
    status_t getLayerDebugInfo(std::vector<LayerDebugInfo>* /*layers*/) const override {
        return NO_ERROR;
    }
    status_t getCompositionPreference(
            ui::Dataspace* /*outDefaultDataspace*/, ui::PixelFormat* /*outDefaultPixelFormat*/,
            ui::Dataspace* /*outWideColorGamutDataspace*/,
            ui::PixelFormat* /*outWideColorGamutPixelFormat*/) const override {
        return NO_ERROR;
    }
    status_t getDisplayedContentSamplingAttributes(const sp<IBinder>& /*display*/,
                                                   ui::PixelFormat* /*outFormat*/,
                                                   ui::Dataspace* /*outDataspace*/,
                                                   uint8_t* /*outComponentMask*/) const override {
        return NO_ERROR;
    }
    status_t setDisplayContentSamplingEnabled(const sp<IBinder>& /*display*/, bool /*enable*/,
                                              uint8_t /*componentMask*/,
                                              uint64_t /*maxFrames*/) const override {
        return NO_ERROR;
    }
    status_t getDisplayedContentSample(const sp<IBinder>& /*display*/, uint64_t /*maxFrames*/,
                                       uint64_t /*timestamp*/,
                                       DisplayedFrameStats* /*outStats*/) const override {
        return NO_ERROR;
    }

    status_t getColorManagement(bool* /*outGetColorManagement*/) const override { return NO_ERROR; }
    status_t getProtectedContentSupport(bool* /*outSupported*/) const override { return NO_ERROR; }

    status_t isWideColorDisplay(const sp<IBinder>&, bool*) const override { return NO_ERROR; }
    status_t getDisplayBrightnessSupport(const sp<IBinder>& /*displayToken*/,
                                         bool* /*outSupport*/) const override {
        return NO_ERROR;
    }
    status_t setDisplayBrightness(const sp<IBinder>& /*displayToken*/,
                                  float /*brightness*/) const override {
        return NO_ERROR;
    }

    status_t addRegionSamplingListener(const Rect& /*samplingArea*/,
                                       const sp<IBinder>& /*stopLayerHandle*/,
                                       const sp<IRegionSamplingListener>& /*listener*/) override {
        return NO_ERROR;
    }
    status_t removeRegionSamplingListener(
            const sp<IRegionSamplingListener>& /*listener*/) override {
        return NO_ERROR;
    }
    status_t setAllowedDisplayConfigs(const sp<IBinder>& /*displayToken*/,
                                      const std::vector<int32_t>& /*allowedConfigs*/) override {
        return NO_ERROR;
    }
    status_t getAllowedDisplayConfigs(const sp<IBinder>& /*displayToken*/,
                                      std::vector<int32_t>* /*outAllowedConfigs*/) override {
        return NO_ERROR;
    }
    status_t notifyPowerHint(int32_t /*hintId*/) override { return NO_ERROR; }
    status_t enterSelf() override {
        return NO_ERROR;
    }
    status_t exitSelf() override {
        return NO_ERROR;
    }

protected:
    IBinder* onAsBinder() override { return nullptr; }

private:
    bool mSupportsPresent{true};
};

class FakeProducerFrameEventHistory : public ProducerFrameEventHistory {
public:
    explicit FakeProducerFrameEventHistory(FenceToFenceTimeMap* fenceMap) : mFenceMap(fenceMap) {}

    ~FakeProducerFrameEventHistory() {}

    void updateAcquireFence(uint64_t frameNumber,
            std::shared_ptr<FenceTime>&& acquire) override {
        // Verify the acquire fence being added isn't the one from the consumer.
        EXPECT_NE(mConsumerAcquireFence, acquire);
        // Override the fence, so we can verify this was called by the
        // producer after the frame is queued.
        ProducerFrameEventHistory::updateAcquireFence(frameNumber,
                std::shared_ptr<FenceTime>(mAcquireFenceOverride));
    }

    void setAcquireFenceOverride(
            const std::shared_ptr<FenceTime>& acquireFenceOverride,
            const std::shared_ptr<FenceTime>& consumerAcquireFence) {
        mAcquireFenceOverride = acquireFenceOverride;
        mConsumerAcquireFence = consumerAcquireFence;
    }

protected:
    std::shared_ptr<FenceTime> createFenceTime(const sp<Fence>& fence)
            const override {
        return mFenceMap->createFenceTimeForTest(fence);
    }

    FenceToFenceTimeMap* mFenceMap{nullptr};

    std::shared_ptr<FenceTime> mAcquireFenceOverride{FenceTime::NO_FENCE};
    std::shared_ptr<FenceTime> mConsumerAcquireFence{FenceTime::NO_FENCE};
};


class TestSurface : public Surface {
public:
    TestSurface(const sp<IGraphicBufferProducer>& bufferProducer,
            FenceToFenceTimeMap* fenceMap)
        : Surface(bufferProducer),
          mFakeSurfaceComposer(new FakeSurfaceComposer) {
        mFakeFrameEventHistory = new FakeProducerFrameEventHistory(fenceMap);
        mFrameEventHistory.reset(mFakeFrameEventHistory);
    }

    ~TestSurface() override {}

    sp<ISurfaceComposer> composerService() const override {
        return mFakeSurfaceComposer;
    }

    nsecs_t now() const override {
        return mNow;
    }

    void setNow(nsecs_t now) {
        mNow = now;
    }

public:
    sp<FakeSurfaceComposer> mFakeSurfaceComposer;
    nsecs_t mNow = 0;

    // mFrameEventHistory owns the instance of FakeProducerFrameEventHistory,
    // but this raw pointer gives access to test functionality.
    FakeProducerFrameEventHistory* mFakeFrameEventHistory;
};


class GetFrameTimestampsTest : public ::testing::Test {
protected:
    struct FenceAndFenceTime {
        explicit FenceAndFenceTime(FenceToFenceTimeMap& fenceMap)
           : mFence(new Fence),
             mFenceTime(fenceMap.createFenceTimeForTest(mFence)) {}
        sp<Fence> mFence { nullptr };
        std::shared_ptr<FenceTime> mFenceTime { nullptr };
    };

    struct RefreshEvents {
        RefreshEvents(FenceToFenceTimeMap& fenceMap, nsecs_t refreshStart)
          : mFenceMap(fenceMap),
            kCompositorTiming(
                {refreshStart, refreshStart + 1, refreshStart + 2 }),
            kStartTime(refreshStart + 3),
            kGpuCompositionDoneTime(refreshStart + 4),
            kPresentTime(refreshStart + 5) {}

        void signalPostCompositeFences() {
            mFenceMap.signalAllForTest(
                        mGpuCompositionDone.mFence, kGpuCompositionDoneTime);
            mFenceMap.signalAllForTest(mPresent.mFence, kPresentTime);
        }

        FenceToFenceTimeMap& mFenceMap;

        FenceAndFenceTime mGpuCompositionDone { mFenceMap };
        FenceAndFenceTime mPresent { mFenceMap };

        const CompositorTiming kCompositorTiming;

        const nsecs_t kStartTime;
        const nsecs_t kGpuCompositionDoneTime;
        const nsecs_t kPresentTime;
    };

    struct FrameEvents {
        FrameEvents(FenceToFenceTimeMap& fenceMap, nsecs_t frameStartTime)
            : mFenceMap(fenceMap),
              kPostedTime(frameStartTime + 100),
              kRequestedPresentTime(frameStartTime + 200),
              kProducerAcquireTime(frameStartTime + 300),
              kConsumerAcquireTime(frameStartTime + 301),
              kLatchTime(frameStartTime + 500),
              kDequeueReadyTime(frameStartTime + 600),
              kReleaseTime(frameStartTime + 700),
              mRefreshes {
                    { mFenceMap, frameStartTime + 410 },
                    { mFenceMap, frameStartTime + 420 },
                    { mFenceMap, frameStartTime + 430 } } {}

        void signalQueueFences() {
            mFenceMap.signalAllForTest(
                        mAcquireConsumer.mFence, kConsumerAcquireTime);
            mFenceMap.signalAllForTest(
                        mAcquireProducer.mFence, kProducerAcquireTime);
        }

        void signalRefreshFences() {
            for (auto& re : mRefreshes) {
                re.signalPostCompositeFences();
            }
        }

        void signalReleaseFences() {
            mFenceMap.signalAllForTest(mRelease.mFence, kReleaseTime);
        }

        FenceToFenceTimeMap& mFenceMap;

        FenceAndFenceTime mAcquireConsumer { mFenceMap };
        FenceAndFenceTime mAcquireProducer { mFenceMap };
        FenceAndFenceTime mRelease { mFenceMap };

        const nsecs_t kPostedTime;
        const nsecs_t kRequestedPresentTime;
        const nsecs_t kProducerAcquireTime;
        const nsecs_t kConsumerAcquireTime;
        const nsecs_t kLatchTime;
        const nsecs_t kDequeueReadyTime;
        const nsecs_t kReleaseTime;

        RefreshEvents mRefreshes[3];
    };

    GetFrameTimestampsTest() {}

    virtual void SetUp() {
        BufferQueue::createBufferQueue(&mProducer, &mConsumer);
        mFakeConsumer = new FakeConsumer;
        mCfeh = &mFakeConsumer->mFrameEventHistory;
        mConsumer->consumerConnect(mFakeConsumer, false);
        mConsumer->setConsumerName(String8("TestConsumer"));
        mSurface = new TestSurface(mProducer, &mFenceMap);
        mWindow = mSurface;

        ASSERT_EQ(NO_ERROR, native_window_api_connect(mWindow.get(),
                NATIVE_WINDOW_API_CPU));
        native_window_set_buffer_count(mWindow.get(), 4);
    }

    void disableFrameTimestamps() {
        mFakeConsumer->mGetFrameTimestampsEnabled = false;
        native_window_enable_frame_timestamps(mWindow.get(), 0);
        mFrameTimestampsEnabled = false;
    }

    void enableFrameTimestamps() {
        mFakeConsumer->mGetFrameTimestampsEnabled = true;
        native_window_enable_frame_timestamps(mWindow.get(), 1);
        mFrameTimestampsEnabled = true;
    }

    int getAllFrameTimestamps(uint64_t frameId) {
        return native_window_get_frame_timestamps(mWindow.get(), frameId,
                &outRequestedPresentTime, &outAcquireTime, &outLatchTime,
                &outFirstRefreshStartTime, &outLastRefreshStartTime,
                &outGpuCompositionDoneTime, &outDisplayPresentTime,
                &outDequeueReadyTime, &outReleaseTime);
    }

    void resetTimestamps() {
        outRequestedPresentTime = -1;
        outAcquireTime = -1;
        outLatchTime = -1;
        outFirstRefreshStartTime = -1;
        outLastRefreshStartTime = -1;
        outGpuCompositionDoneTime = -1;
        outDisplayPresentTime = -1;
        outDequeueReadyTime = -1;
        outReleaseTime = -1;
    }

    uint64_t getNextFrameId() {
        uint64_t frameId = -1;
        int status = native_window_get_next_frame_id(mWindow.get(), &frameId);
        EXPECT_EQ(status, NO_ERROR);
        return frameId;
    }

    void dequeueAndQueue(uint64_t frameIndex) {
        int fence = -1;
        ANativeWindowBuffer* buffer = nullptr;
        ASSERT_EQ(NO_ERROR,
                mWindow->dequeueBuffer(mWindow.get(), &buffer, &fence));

        int oldAddFrameTimestampsCount =
                mFakeConsumer->mAddFrameTimestampsCount;

        FrameEvents* frame = &mFrames[frameIndex];
        uint64_t frameNumber = frameIndex + 1;

        NewFrameEventsEntry fe;
        fe.frameNumber = frameNumber;
        fe.postedTime = frame->kPostedTime;
        fe.requestedPresentTime = frame->kRequestedPresentTime;
        fe.acquireFence = frame->mAcquireConsumer.mFenceTime;
        mFakeConsumer->mNewFrameEntryOverride = fe;

        mSurface->mFakeFrameEventHistory->setAcquireFenceOverride(
                    frame->mAcquireProducer.mFenceTime,
                    frame->mAcquireConsumer.mFenceTime);

        ASSERT_EQ(NO_ERROR, mWindow->queueBuffer(mWindow.get(), buffer, fence));

        EXPECT_EQ(frameNumber, mFakeConsumer->mLastAddedFrameNumber);

        EXPECT_EQ(
                oldAddFrameTimestampsCount + (mFrameTimestampsEnabled ? 1 : 0),
                mFakeConsumer->mAddFrameTimestampsCount);
    }

    void addFrameEvents(
            bool gpuComposited, uint64_t iOldFrame, int64_t iNewFrame) {
        FrameEvents* oldFrame =
                (iOldFrame == NO_FRAME_INDEX) ? nullptr : &mFrames[iOldFrame];
        FrameEvents* newFrame = &mFrames[iNewFrame];

        uint64_t nOldFrame = (iOldFrame == NO_FRAME_INDEX) ? 0 : iOldFrame + 1;
        uint64_t nNewFrame = iNewFrame + 1;

        // Latch, Composite, and Release the frames in a plausible order.
        // Note: The timestamps won't necessarily match the order, but
        // that's okay for the purposes of this test.
        std::shared_ptr<FenceTime> gpuDoneFenceTime = FenceTime::NO_FENCE;

        // Composite the previous frame one more time, which helps verify
        // LastRefresh is updated properly.
        if (oldFrame != nullptr) {
            mCfeh->addPreComposition(nOldFrame,
                                     oldFrame->mRefreshes[2].kStartTime);
            gpuDoneFenceTime = gpuComposited ?
                    oldFrame->mRefreshes[2].mGpuCompositionDone.mFenceTime :
                    FenceTime::NO_FENCE;
            mCfeh->addPostComposition(nOldFrame, gpuDoneFenceTime,
                    oldFrame->mRefreshes[2].mPresent.mFenceTime,
                    oldFrame->mRefreshes[2].kCompositorTiming);
        }

        // Latch the new frame.
        mCfeh->addLatch(nNewFrame, newFrame->kLatchTime);

        mCfeh->addPreComposition(nNewFrame, newFrame->mRefreshes[0].kStartTime);
        gpuDoneFenceTime = gpuComposited ?
                newFrame->mRefreshes[0].mGpuCompositionDone.mFenceTime :
                FenceTime::NO_FENCE;
        // HWC2 releases the previous buffer after a new latch just before
        // calling postComposition.
        if (oldFrame != nullptr) {
            mCfeh->addRelease(nOldFrame, oldFrame->kDequeueReadyTime,
                    std::shared_ptr<FenceTime>(oldFrame->mRelease.mFenceTime));
        }
        mCfeh->addPostComposition(nNewFrame, gpuDoneFenceTime,
                newFrame->mRefreshes[0].mPresent.mFenceTime,
                newFrame->mRefreshes[0].kCompositorTiming);

        mCfeh->addPreComposition(nNewFrame, newFrame->mRefreshes[1].kStartTime);
        gpuDoneFenceTime = gpuComposited ?
                newFrame->mRefreshes[1].mGpuCompositionDone.mFenceTime :
                FenceTime::NO_FENCE;
        mCfeh->addPostComposition(nNewFrame, gpuDoneFenceTime,
                newFrame->mRefreshes[1].mPresent.mFenceTime,
                newFrame->mRefreshes[1].kCompositorTiming);
    }

    sp<IGraphicBufferProducer> mProducer;
    sp<IGraphicBufferConsumer> mConsumer;
    sp<FakeConsumer> mFakeConsumer;
    ConsumerFrameEventHistory* mCfeh;
    sp<TestSurface> mSurface;
    sp<ANativeWindow> mWindow;

    FenceToFenceTimeMap mFenceMap;

    bool mFrameTimestampsEnabled = false;

    int64_t outRequestedPresentTime = -1;
    int64_t outAcquireTime = -1;
    int64_t outLatchTime = -1;
    int64_t outFirstRefreshStartTime = -1;
    int64_t outLastRefreshStartTime = -1;
    int64_t outGpuCompositionDoneTime = -1;
    int64_t outDisplayPresentTime = -1;
    int64_t outDequeueReadyTime = -1;
    int64_t outReleaseTime = -1;

    FrameEvents mFrames[3] {
        { mFenceMap, 1000 }, { mFenceMap, 2000 }, { mFenceMap, 3000 } };
};


// This test verifies that the frame timestamps are not retrieved when not
// explicitly enabled via native_window_enable_frame_timestamps.
// We want to check this to make sure there's no overhead for users
// that don't need the timestamp information.
TEST_F(GetFrameTimestampsTest, DefaultDisabled) {
    int fence;
    ANativeWindowBuffer* buffer;

    EXPECT_EQ(0, mFakeConsumer->mAddFrameTimestampsCount);
    EXPECT_EQ(0, mFakeConsumer->mGetFrameTimestampsCount);

    const uint64_t fId = getNextFrameId();

    // Verify the producer doesn't get frame timestamps piggybacked on dequeue.
    ASSERT_EQ(NO_ERROR, mWindow->dequeueBuffer(mWindow.get(), &buffer, &fence));
    EXPECT_EQ(0, mFakeConsumer->mAddFrameTimestampsCount);
    EXPECT_EQ(0, mFakeConsumer->mGetFrameTimestampsCount);

    // Verify the producer doesn't get frame timestamps piggybacked on queue.
    // It is okay that frame timestamps are added in the consumer since it is
    // still needed for SurfaceFlinger dumps.
    ASSERT_EQ(NO_ERROR, mWindow->queueBuffer(mWindow.get(), buffer, fence));
    EXPECT_EQ(1, mFakeConsumer->mAddFrameTimestampsCount);
    EXPECT_EQ(0, mFakeConsumer->mGetFrameTimestampsCount);

    // Verify attempts to get frame timestamps fail.
    int result = getAllFrameTimestamps(fId);
    EXPECT_EQ(INVALID_OPERATION, result);
    EXPECT_EQ(0, mFakeConsumer->mGetFrameTimestampsCount);

    // Verify compositor timing query fails.
    nsecs_t compositeDeadline = 0;
    nsecs_t compositeInterval = 0;
    nsecs_t compositeToPresentLatency = 0;
    result = native_window_get_compositor_timing(mWindow.get(),
        &compositeDeadline, &compositeInterval, &compositeToPresentLatency);
    EXPECT_EQ(INVALID_OPERATION, result);
}

// This test verifies that the frame timestamps are retrieved if explicitly
// enabled via native_window_enable_frame_timestamps.
TEST_F(GetFrameTimestampsTest, EnabledSimple) {
    CompositorTiming initialCompositorTiming {
        1000000000, // 1s deadline
        16666667, // 16ms interval
        50000000, // 50ms present latency
    };
    mCfeh->initializeCompositorTiming(initialCompositorTiming);

    enableFrameTimestamps();

    // Verify the compositor timing query gets the initial compositor values
    // after timststamps are enabled; even before the first frame is queued
    // or dequeued.
    nsecs_t compositeDeadline = 0;
    nsecs_t compositeInterval = 0;
    nsecs_t compositeToPresentLatency = 0;
    mSurface->setNow(initialCompositorTiming.deadline - 1);
    int result = native_window_get_compositor_timing(mWindow.get(),
        &compositeDeadline, &compositeInterval, &compositeToPresentLatency);
    EXPECT_EQ(NO_ERROR, result);
    EXPECT_EQ(initialCompositorTiming.deadline, compositeDeadline);
    EXPECT_EQ(initialCompositorTiming.interval, compositeInterval);
    EXPECT_EQ(initialCompositorTiming.presentLatency,
              compositeToPresentLatency);

    int fence;
    ANativeWindowBuffer* buffer;

    EXPECT_EQ(0, mFakeConsumer->mAddFrameTimestampsCount);
    EXPECT_EQ(1, mFakeConsumer->mGetFrameTimestampsCount);

    const uint64_t fId1 = getNextFrameId();

    // Verify getFrameTimestamps is piggybacked on dequeue.
    ASSERT_EQ(NO_ERROR, mWindow->dequeueBuffer(mWindow.get(), &buffer, &fence));
    EXPECT_EQ(0, mFakeConsumer->mAddFrameTimestampsCount);
    EXPECT_EQ(2, mFakeConsumer->mGetFrameTimestampsCount);

    NewFrameEventsEntry f1;
    f1.frameNumber = 1;
    f1.postedTime = mFrames[0].kPostedTime;
    f1.requestedPresentTime = mFrames[0].kRequestedPresentTime;
    f1.acquireFence = mFrames[0].mAcquireConsumer.mFenceTime;
    mSurface->mFakeFrameEventHistory->setAcquireFenceOverride(
            mFrames[0].mAcquireProducer.mFenceTime,
            mFrames[0].mAcquireConsumer.mFenceTime);
    mFakeConsumer->mNewFrameEntryOverride = f1;
    mFrames[0].signalQueueFences();

    // Verify getFrameTimestamps is piggybacked on queue.
    ASSERT_EQ(NO_ERROR, mWindow->queueBuffer(mWindow.get(), buffer, fence));
    EXPECT_EQ(1, mFakeConsumer->mAddFrameTimestampsCount);
    EXPECT_EQ(1u, mFakeConsumer->mLastAddedFrameNumber);
    EXPECT_EQ(3, mFakeConsumer->mGetFrameTimestampsCount);

    // Verify queries for timestamps that the producer doesn't know about
    // triggers a call to see if the consumer has any new timestamps.
    result = getAllFrameTimestamps(fId1);
    EXPECT_EQ(NO_ERROR, result);
    EXPECT_EQ(4, mFakeConsumer->mGetFrameTimestampsCount);
}

TEST_F(GetFrameTimestampsTest, QueryPresentSupported) {
    bool displayPresentSupported = true;
    mSurface->mFakeSurfaceComposer->setSupportsPresent(displayPresentSupported);

    // Verify supported bits are forwarded.
    int supportsPresent = -1;
    mWindow.get()->query(mWindow.get(),
            NATIVE_WINDOW_FRAME_TIMESTAMPS_SUPPORTS_PRESENT, &supportsPresent);
    EXPECT_EQ(displayPresentSupported, supportsPresent);
}

TEST_F(GetFrameTimestampsTest, QueryPresentNotSupported) {
    bool displayPresentSupported = false;
    mSurface->mFakeSurfaceComposer->setSupportsPresent(displayPresentSupported);

    // Verify supported bits are forwarded.
    int supportsPresent = -1;
    mWindow.get()->query(mWindow.get(),
            NATIVE_WINDOW_FRAME_TIMESTAMPS_SUPPORTS_PRESENT, &supportsPresent);
    EXPECT_EQ(displayPresentSupported, supportsPresent);
}

TEST_F(GetFrameTimestampsTest, SnapToNextTickBasic) {
    nsecs_t phase = 4000;
    nsecs_t interval = 1000;

    // Timestamp in previous interval.
    nsecs_t timestamp = 3500;
    EXPECT_EQ(4000, ProducerFrameEventHistory::snapToNextTick(
            timestamp, phase, interval));

    // Timestamp in next interval.
    timestamp = 4500;
    EXPECT_EQ(5000, ProducerFrameEventHistory::snapToNextTick(
            timestamp, phase, interval));

    // Timestamp multiple intervals before.
    timestamp = 2500;
    EXPECT_EQ(3000, ProducerFrameEventHistory::snapToNextTick(
            timestamp, phase, interval));

    // Timestamp multiple intervals after.
    timestamp = 6500;
    EXPECT_EQ(7000, ProducerFrameEventHistory::snapToNextTick(
            timestamp, phase, interval));

    // Timestamp on previous interval.
    timestamp = 3000;
    EXPECT_EQ(3000, ProducerFrameEventHistory::snapToNextTick(
            timestamp, phase, interval));

    // Timestamp on next interval.
    timestamp = 5000;
    EXPECT_EQ(5000, ProducerFrameEventHistory::snapToNextTick(
            timestamp, phase, interval));

    // Timestamp equal to phase.
    timestamp = 4000;
    EXPECT_EQ(4000, ProducerFrameEventHistory::snapToNextTick(
            timestamp, phase, interval));
}

// int(big_timestamp / interval) < 0, which can cause a crash or invalid result
// if the number of intervals elapsed is internally stored in an int.
TEST_F(GetFrameTimestampsTest, SnapToNextTickOverflow) {
      nsecs_t phase = 0;
      nsecs_t interval = 4000;
      nsecs_t big_timestamp = 8635916564000;
      int32_t intervals = big_timestamp / interval;

      EXPECT_LT(intervals, 0);
      EXPECT_EQ(8635916564000, ProducerFrameEventHistory::snapToNextTick(
            big_timestamp, phase, interval));
      EXPECT_EQ(8635916564000, ProducerFrameEventHistory::snapToNextTick(
            big_timestamp, big_timestamp, interval));
}

// This verifies the compositor timing is updated by refresh events
// and piggy backed on a queue, dequeue, and enabling of timestamps..
TEST_F(GetFrameTimestampsTest, CompositorTimingUpdatesBasic) {
    CompositorTiming initialCompositorTiming {
        1000000000, // 1s deadline
        16666667, // 16ms interval
        50000000, // 50ms present latency
    };
    mCfeh->initializeCompositorTiming(initialCompositorTiming);

    enableFrameTimestamps();

    // We get the initial values before any frames are submitted.
    nsecs_t compositeDeadline = 0;
    nsecs_t compositeInterval = 0;
    nsecs_t compositeToPresentLatency = 0;
    mSurface->setNow(initialCompositorTiming.deadline - 1);
    int result = native_window_get_compositor_timing(mWindow.get(),
        &compositeDeadline, &compositeInterval, &compositeToPresentLatency);
    EXPECT_EQ(NO_ERROR, result);
    EXPECT_EQ(initialCompositorTiming.deadline, compositeDeadline);
    EXPECT_EQ(initialCompositorTiming.interval, compositeInterval);
    EXPECT_EQ(initialCompositorTiming.presentLatency,
              compositeToPresentLatency);

    dequeueAndQueue(0);
    addFrameEvents(true, NO_FRAME_INDEX, 0);

    // Still get the initial values because the frame events for frame 0
    // didn't get a chance to piggyback on a queue or dequeue yet.
    result = native_window_get_compositor_timing(mWindow.get(),
        &compositeDeadline, &compositeInterval, &compositeToPresentLatency);
    EXPECT_EQ(NO_ERROR, result);
    EXPECT_EQ(initialCompositorTiming.deadline, compositeDeadline);
    EXPECT_EQ(initialCompositorTiming.interval, compositeInterval);
    EXPECT_EQ(initialCompositorTiming.presentLatency,
              compositeToPresentLatency);

    dequeueAndQueue(1);
    addFrameEvents(true, 0, 1);

    // Now expect the composite values associated with frame 1.
    mSurface->setNow(mFrames[0].mRefreshes[1].kCompositorTiming.deadline);
    result = native_window_get_compositor_timing(mWindow.get(),
        &compositeDeadline, &compositeInterval, &compositeToPresentLatency);
    EXPECT_EQ(NO_ERROR, result);
    EXPECT_EQ(mFrames[0].mRefreshes[1].kCompositorTiming.deadline,
            compositeDeadline);
    EXPECT_EQ(mFrames[0].mRefreshes[1].kCompositorTiming.interval,
            compositeInterval);
    EXPECT_EQ(mFrames[0].mRefreshes[1].kCompositorTiming.presentLatency,
            compositeToPresentLatency);

    dequeueAndQueue(2);
    addFrameEvents(true, 1, 2);

    // Now expect the composite values associated with frame 2.
    mSurface->setNow(mFrames[1].mRefreshes[1].kCompositorTiming.deadline);
    result = native_window_get_compositor_timing(mWindow.get(),
        &compositeDeadline, &compositeInterval, &compositeToPresentLatency);
    EXPECT_EQ(NO_ERROR, result);
    EXPECT_EQ(mFrames[1].mRefreshes[1].kCompositorTiming.deadline,
            compositeDeadline);
    EXPECT_EQ(mFrames[1].mRefreshes[1].kCompositorTiming.interval,
            compositeInterval);
    EXPECT_EQ(mFrames[1].mRefreshes[1].kCompositorTiming.presentLatency,
            compositeToPresentLatency);

    // Re-enabling frame timestamps should get the latest values.
    disableFrameTimestamps();
    enableFrameTimestamps();

    // Now expect the composite values associated with frame 3.
    mSurface->setNow(mFrames[2].mRefreshes[1].kCompositorTiming.deadline);
    result = native_window_get_compositor_timing(mWindow.get(),
        &compositeDeadline, &compositeInterval, &compositeToPresentLatency);
    EXPECT_EQ(NO_ERROR, result);
    EXPECT_EQ(mFrames[2].mRefreshes[1].kCompositorTiming.deadline,
            compositeDeadline);
    EXPECT_EQ(mFrames[2].mRefreshes[1].kCompositorTiming.interval,
            compositeInterval);
    EXPECT_EQ(mFrames[2].mRefreshes[1].kCompositorTiming.presentLatency,
            compositeToPresentLatency);
}

// This verifies the compositor deadline properly snaps to the the next
// deadline based on the current time.
TEST_F(GetFrameTimestampsTest, CompositorTimingDeadlineSnaps) {
    CompositorTiming initialCompositorTiming {
        1000000000, // 1s deadline
        16666667, // 16ms interval
        50000000, // 50ms present latency
    };
    mCfeh->initializeCompositorTiming(initialCompositorTiming);

    enableFrameTimestamps();

    nsecs_t compositeDeadline = 0;
    nsecs_t compositeInterval = 0;
    nsecs_t compositeToPresentLatency = 0;

    // A "now" just before the deadline snaps to the deadline.
    mSurface->setNow(initialCompositorTiming.deadline - 1);
    int result = native_window_get_compositor_timing(mWindow.get(),
        &compositeDeadline, &compositeInterval, &compositeToPresentLatency);
    EXPECT_EQ(NO_ERROR, result);
    EXPECT_EQ(initialCompositorTiming.deadline, compositeDeadline);
    nsecs_t expectedDeadline = initialCompositorTiming.deadline;
    EXPECT_EQ(expectedDeadline, compositeDeadline);

    dequeueAndQueue(0);
    addFrameEvents(true, NO_FRAME_INDEX, 0);

    // A "now" just after the deadline snaps properly.
    mSurface->setNow(initialCompositorTiming.deadline + 1);
    result = native_window_get_compositor_timing(mWindow.get(),
        &compositeDeadline, &compositeInterval, &compositeToPresentLatency);
    EXPECT_EQ(NO_ERROR, result);
    expectedDeadline =
            initialCompositorTiming.deadline +initialCompositorTiming.interval;
    EXPECT_EQ(expectedDeadline, compositeDeadline);

    dequeueAndQueue(1);
    addFrameEvents(true, 0, 1);

    // A "now" just after the next interval snaps properly.
    mSurface->setNow(
            mFrames[0].mRefreshes[1].kCompositorTiming.deadline +
            mFrames[0].mRefreshes[1].kCompositorTiming.interval + 1);
    result = native_window_get_compositor_timing(mWindow.get(),
        &compositeDeadline, &compositeInterval, &compositeToPresentLatency);
    EXPECT_EQ(NO_ERROR, result);
    expectedDeadline =
            mFrames[0].mRefreshes[1].kCompositorTiming.deadline +
            mFrames[0].mRefreshes[1].kCompositorTiming.interval * 2;
    EXPECT_EQ(expectedDeadline, compositeDeadline);

    dequeueAndQueue(2);
    addFrameEvents(true, 1, 2);

    // A "now" over 1 interval before the deadline snaps properly.
    mSurface->setNow(
            mFrames[1].mRefreshes[1].kCompositorTiming.deadline -
            mFrames[1].mRefreshes[1].kCompositorTiming.interval - 1);
    result = native_window_get_compositor_timing(mWindow.get(),
        &compositeDeadline, &compositeInterval, &compositeToPresentLatency);
    EXPECT_EQ(NO_ERROR, result);
    expectedDeadline =
            mFrames[1].mRefreshes[1].kCompositorTiming.deadline -
            mFrames[1].mRefreshes[1].kCompositorTiming.interval;
    EXPECT_EQ(expectedDeadline, compositeDeadline);

    // Re-enabling frame timestamps should get the latest values.
    disableFrameTimestamps();
    enableFrameTimestamps();

    // A "now" over 2 intervals before the deadline snaps properly.
    mSurface->setNow(
            mFrames[2].mRefreshes[1].kCompositorTiming.deadline -
            mFrames[2].mRefreshes[1].kCompositorTiming.interval * 2 - 1);
    result = native_window_get_compositor_timing(mWindow.get(),
        &compositeDeadline, &compositeInterval, &compositeToPresentLatency);
    EXPECT_EQ(NO_ERROR, result);
    expectedDeadline =
            mFrames[2].mRefreshes[1].kCompositorTiming.deadline -
            mFrames[2].mRefreshes[1].kCompositorTiming.interval * 2;
    EXPECT_EQ(expectedDeadline, compositeDeadline);
}

// This verifies the timestamps recorded in the consumer's
// FrameTimestampsHistory are properly retrieved by the producer for the
// correct frames.
TEST_F(GetFrameTimestampsTest, TimestampsAssociatedWithCorrectFrame) {
    enableFrameTimestamps();

    const uint64_t fId1 = getNextFrameId();
    dequeueAndQueue(0);
    mFrames[0].signalQueueFences();

    const uint64_t fId2 = getNextFrameId();
    dequeueAndQueue(1);
    mFrames[1].signalQueueFences();

    addFrameEvents(true, NO_FRAME_INDEX, 0);
    mFrames[0].signalRefreshFences();
    addFrameEvents(true, 0, 1);
    mFrames[0].signalReleaseFences();
    mFrames[1].signalRefreshFences();

    // Verify timestamps are correct for frame 1.
    resetTimestamps();
    int result = getAllFrameTimestamps(fId1);
    EXPECT_EQ(NO_ERROR, result);
    EXPECT_EQ(mFrames[0].kRequestedPresentTime, outRequestedPresentTime);
    EXPECT_EQ(mFrames[0].kProducerAcquireTime, outAcquireTime);
    EXPECT_EQ(mFrames[0].kLatchTime, outLatchTime);
    EXPECT_EQ(mFrames[0].mRefreshes[0].kStartTime, outFirstRefreshStartTime);
    EXPECT_EQ(mFrames[0].mRefreshes[2].kStartTime, outLastRefreshStartTime);
    EXPECT_EQ(mFrames[0].mRefreshes[0].kGpuCompositionDoneTime,
            outGpuCompositionDoneTime);
    EXPECT_EQ(mFrames[0].mRefreshes[0].kPresentTime, outDisplayPresentTime);
    EXPECT_EQ(mFrames[0].kDequeueReadyTime, outDequeueReadyTime);
    EXPECT_EQ(mFrames[0].kReleaseTime, outReleaseTime);

    // Verify timestamps are correct for frame 2.
    resetTimestamps();
    result = getAllFrameTimestamps(fId2);
    EXPECT_EQ(NO_ERROR, result);
    EXPECT_EQ(mFrames[1].kRequestedPresentTime, outRequestedPresentTime);
    EXPECT_EQ(mFrames[1].kProducerAcquireTime, outAcquireTime);
    EXPECT_EQ(mFrames[1].kLatchTime, outLatchTime);
    EXPECT_EQ(mFrames[1].mRefreshes[0].kStartTime, outFirstRefreshStartTime);
    EXPECT_EQ(mFrames[1].mRefreshes[1].kStartTime, outLastRefreshStartTime);
    EXPECT_EQ(mFrames[1].mRefreshes[0].kGpuCompositionDoneTime,
            outGpuCompositionDoneTime);
    EXPECT_EQ(mFrames[1].mRefreshes[0].kPresentTime, outDisplayPresentTime);
    EXPECT_EQ(NATIVE_WINDOW_TIMESTAMP_PENDING, outDequeueReadyTime);
    EXPECT_EQ(NATIVE_WINDOW_TIMESTAMP_PENDING, outReleaseTime);
}

// This test verifies the acquire fence recorded by the consumer is not sent
// back to the producer and the producer saves its own fence.
TEST_F(GetFrameTimestampsTest, QueueTimestampsNoSync) {
    enableFrameTimestamps();

    // Dequeue and queue frame 1.
    const uint64_t fId1 = getNextFrameId();
    dequeueAndQueue(0);

    // Verify queue-related timestamps for f1 are available immediately in the
    // producer without asking the consumer again, even before signaling the
    // acquire fence.
    resetTimestamps();
    int oldCount = mFakeConsumer->mGetFrameTimestampsCount;
    int result = native_window_get_frame_timestamps(mWindow.get(), fId1,
            &outRequestedPresentTime, &outAcquireTime, nullptr, nullptr,
            nullptr, nullptr, nullptr, nullptr, nullptr);
    EXPECT_EQ(oldCount, mFakeConsumer->mGetFrameTimestampsCount);
    EXPECT_EQ(NO_ERROR, result);
    EXPECT_EQ(mFrames[0].kRequestedPresentTime, outRequestedPresentTime);
    EXPECT_EQ(NATIVE_WINDOW_TIMESTAMP_PENDING, outAcquireTime);

    // Signal acquire fences. Verify a sync call still isn't necessary.
    mFrames[0].signalQueueFences();

    oldCount = mFakeConsumer->mGetFrameTimestampsCount;
    result = native_window_get_frame_timestamps(mWindow.get(), fId1,
            &outRequestedPresentTime, &outAcquireTime, nullptr, nullptr,
            nullptr, nullptr, nullptr, nullptr, nullptr);
    EXPECT_EQ(oldCount, mFakeConsumer->mGetFrameTimestampsCount);
    EXPECT_EQ(NO_ERROR, result);
    EXPECT_EQ(mFrames[0].kRequestedPresentTime, outRequestedPresentTime);
    EXPECT_EQ(mFrames[0].kProducerAcquireTime, outAcquireTime);

    // Dequeue and queue frame 2.
    const uint64_t fId2 = getNextFrameId();
    dequeueAndQueue(1);

    // Verify queue-related timestamps for f2 are available immediately in the
    // producer without asking the consumer again, even before signaling the
    // acquire fence.
    resetTimestamps();
    oldCount = mFakeConsumer->mGetFrameTimestampsCount;
    result = native_window_get_frame_timestamps(mWindow.get(), fId2,
            &outRequestedPresentTime, &outAcquireTime, nullptr, nullptr,
            nullptr, nullptr, nullptr, nullptr, nullptr);
    EXPECT_EQ(oldCount, mFakeConsumer->mGetFrameTimestampsCount);
    EXPECT_EQ(NO_ERROR, result);
    EXPECT_EQ(mFrames[1].kRequestedPresentTime, outRequestedPresentTime);
    EXPECT_EQ(NATIVE_WINDOW_TIMESTAMP_PENDING, outAcquireTime);

    // Signal acquire fences. Verify a sync call still isn't necessary.
    mFrames[1].signalQueueFences();

    oldCount = mFakeConsumer->mGetFrameTimestampsCount;
    result = native_window_get_frame_timestamps(mWindow.get(), fId2,
            &outRequestedPresentTime, &outAcquireTime, nullptr, nullptr,
            nullptr, nullptr, nullptr, nullptr, nullptr);
    EXPECT_EQ(oldCount, mFakeConsumer->mGetFrameTimestampsCount);
    EXPECT_EQ(NO_ERROR, result);
    EXPECT_EQ(mFrames[1].kRequestedPresentTime, outRequestedPresentTime);
    EXPECT_EQ(mFrames[1].kProducerAcquireTime, outAcquireTime);
}

TEST_F(GetFrameTimestampsTest, ZeroRequestedTimestampsNoSync) {
    enableFrameTimestamps();

    // Dequeue and queue frame 1.
    dequeueAndQueue(0);
    mFrames[0].signalQueueFences();

    // Dequeue and queue frame 2.
    const uint64_t fId2 = getNextFrameId();
    dequeueAndQueue(1);
    mFrames[1].signalQueueFences();

    addFrameEvents(true, NO_FRAME_INDEX, 0);
    mFrames[0].signalRefreshFences();
    addFrameEvents(true, 0, 1);
    mFrames[0].signalReleaseFences();
    mFrames[1].signalRefreshFences();

    // Verify a request for no timestamps doesn't result in a sync call.
    int oldCount = mFakeConsumer->mGetFrameTimestampsCount;
    int result = native_window_get_frame_timestamps(mWindow.get(), fId2,
            nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
            nullptr, nullptr);
    EXPECT_EQ(NO_ERROR, result);
    EXPECT_EQ(oldCount, mFakeConsumer->mGetFrameTimestampsCount);
}

// This test verifies that fences can signal and update timestamps producer
// side without an additional sync call to the consumer.
TEST_F(GetFrameTimestampsTest, FencesInProducerNoSync) {
    enableFrameTimestamps();

    // Dequeue and queue frame 1.
    const uint64_t fId1 = getNextFrameId();
    dequeueAndQueue(0);
    mFrames[0].signalQueueFences();

    // Dequeue and queue frame 2.
    dequeueAndQueue(1);
    mFrames[1].signalQueueFences();

    addFrameEvents(true, NO_FRAME_INDEX, 0);
    addFrameEvents(true, 0, 1);

    // Verify available timestamps are correct for frame 1, before any
    // fence has been signaled.
    // Note: A sync call is necessary here since the events triggered by
    // addFrameEvents didn't get to piggyback on the earlier queues/dequeues.
    resetTimestamps();
    int oldCount = mFakeConsumer->mGetFrameTimestampsCount;
    int result = getAllFrameTimestamps(fId1);
    EXPECT_EQ(oldCount + 1, mFakeConsumer->mGetFrameTimestampsCount);
    EXPECT_EQ(NO_ERROR, result);
    EXPECT_EQ(mFrames[0].kRequestedPresentTime, outRequestedPresentTime);
    EXPECT_EQ(mFrames[0].kProducerAcquireTime, outAcquireTime);
    EXPECT_EQ(mFrames[0].kLatchTime, outLatchTime);
    EXPECT_EQ(mFrames[0].mRefreshes[0].kStartTime, outFirstRefreshStartTime);
    EXPECT_EQ(mFrames[0].mRefreshes[2].kStartTime, outLastRefreshStartTime);
    EXPECT_EQ(NATIVE_WINDOW_TIMESTAMP_PENDING, outGpuCompositionDoneTime);
    EXPECT_EQ(NATIVE_WINDOW_TIMESTAMP_PENDING, outDisplayPresentTime);
    EXPECT_EQ(mFrames[0].kDequeueReadyTime, outDequeueReadyTime);
    EXPECT_EQ(NATIVE_WINDOW_TIMESTAMP_PENDING, outReleaseTime);

    // Verify available timestamps are correct for frame 1 again, before any
    // fence has been signaled.
    // This time a sync call should not be necessary.
    resetTimestamps();
    oldCount = mFakeConsumer->mGetFrameTimestampsCount;
    result = getAllFrameTimestamps(fId1);
    EXPECT_EQ(oldCount, mFakeConsumer->mGetFrameTimestampsCount);
    EXPECT_EQ(NO_ERROR, result);
    EXPECT_EQ(mFrames[0].kRequestedPresentTime, outRequestedPresentTime);
    EXPECT_EQ(mFrames[0].kProducerAcquireTime, outAcquireTime);
    EXPECT_EQ(mFrames[0].kLatchTime, outLatchTime);
    EXPECT_EQ(mFrames[0].mRefreshes[0].kStartTime, outFirstRefreshStartTime);
    EXPECT_EQ(mFrames[0].mRefreshes[2].kStartTime, outLastRefreshStartTime);
    EXPECT_EQ(NATIVE_WINDOW_TIMESTAMP_PENDING, outGpuCompositionDoneTime);
    EXPECT_EQ(NATIVE_WINDOW_TIMESTAMP_PENDING, outDisplayPresentTime);
    EXPECT_EQ(mFrames[0].kDequeueReadyTime, outDequeueReadyTime);
    EXPECT_EQ(NATIVE_WINDOW_TIMESTAMP_PENDING, outReleaseTime);

    // Signal the fences for frame 1.
    mFrames[0].signalRefreshFences();
    mFrames[0].signalReleaseFences();

    // Verify all timestamps are available without a sync call.
    resetTimestamps();
    oldCount = mFakeConsumer->mGetFrameTimestampsCount;
    result = getAllFrameTimestamps(fId1);
    EXPECT_EQ(oldCount, mFakeConsumer->mGetFrameTimestampsCount);
    EXPECT_EQ(NO_ERROR, result);
    EXPECT_EQ(mFrames[0].kRequestedPresentTime, outRequestedPresentTime);
    EXPECT_EQ(mFrames[0].kProducerAcquireTime, outAcquireTime);
    EXPECT_EQ(mFrames[0].kLatchTime, outLatchTime);
    EXPECT_EQ(mFrames[0].mRefreshes[0].kStartTime, outFirstRefreshStartTime);
    EXPECT_EQ(mFrames[0].mRefreshes[2].kStartTime, outLastRefreshStartTime);
    EXPECT_EQ(mFrames[0].mRefreshes[0].kGpuCompositionDoneTime,
            outGpuCompositionDoneTime);
    EXPECT_EQ(mFrames[0].mRefreshes[0].kPresentTime, outDisplayPresentTime);
    EXPECT_EQ(mFrames[0].kDequeueReadyTime, outDequeueReadyTime);
    EXPECT_EQ(mFrames[0].kReleaseTime, outReleaseTime);
}

// This test verifies that if the frame wasn't GPU composited but has a refresh
// event a sync call isn't made to get the GPU composite done time since it will
// never exist.
TEST_F(GetFrameTimestampsTest, NoGpuNoSync) {
    enableFrameTimestamps();

    // Dequeue and queue frame 1.
    const uint64_t fId1 = getNextFrameId();
    dequeueAndQueue(0);
    mFrames[0].signalQueueFences();

    // Dequeue and queue frame 2.
    dequeueAndQueue(1);
    mFrames[1].signalQueueFences();

    addFrameEvents(false, NO_FRAME_INDEX, 0);
    addFrameEvents(false, 0, 1);

    // Verify available timestamps are correct for frame 1, before any
    // fence has been signaled.
    // Note: A sync call is necessary here since the events triggered by
    // addFrameEvents didn't get to piggyback on the earlier queues/dequeues.
    resetTimestamps();
    int oldCount = mFakeConsumer->mGetFrameTimestampsCount;
    int result = getAllFrameTimestamps(fId1);
    EXPECT_EQ(oldCount + 1, mFakeConsumer->mGetFrameTimestampsCount);
    EXPECT_EQ(NO_ERROR, result);
    EXPECT_EQ(mFrames[0].kRequestedPresentTime, outRequestedPresentTime);
    EXPECT_EQ(mFrames[0].kProducerAcquireTime, outAcquireTime);
    EXPECT_EQ(mFrames[0].kLatchTime, outLatchTime);
    EXPECT_EQ(mFrames[0].mRefreshes[0].kStartTime, outFirstRefreshStartTime);
    EXPECT_EQ(mFrames[0].mRefreshes[2].kStartTime, outLastRefreshStartTime);
    EXPECT_EQ(NATIVE_WINDOW_TIMESTAMP_INVALID, outGpuCompositionDoneTime);
    EXPECT_EQ(NATIVE_WINDOW_TIMESTAMP_PENDING, outDisplayPresentTime);
    EXPECT_EQ(mFrames[0].kDequeueReadyTime, outDequeueReadyTime);
    EXPECT_EQ(NATIVE_WINDOW_TIMESTAMP_PENDING, outReleaseTime);

    // Signal the fences for frame 1.
    mFrames[0].signalRefreshFences();
    mFrames[0].signalReleaseFences();

    // Verify all timestamps, except GPU composition, are available without a
    // sync call.
    resetTimestamps();
    oldCount = mFakeConsumer->mGetFrameTimestampsCount;
    result = getAllFrameTimestamps(fId1);
    EXPECT_EQ(oldCount, mFakeConsumer->mGetFrameTimestampsCount);
    EXPECT_EQ(NO_ERROR, result);
    EXPECT_EQ(mFrames[0].kRequestedPresentTime, outRequestedPresentTime);
    EXPECT_EQ(mFrames[0].kProducerAcquireTime, outAcquireTime);
    EXPECT_EQ(mFrames[0].kLatchTime, outLatchTime);
    EXPECT_EQ(mFrames[0].mRefreshes[0].kStartTime, outFirstRefreshStartTime);
    EXPECT_EQ(mFrames[0].mRefreshes[2].kStartTime, outLastRefreshStartTime);
    EXPECT_EQ(NATIVE_WINDOW_TIMESTAMP_INVALID, outGpuCompositionDoneTime);
    EXPECT_EQ(mFrames[0].mRefreshes[0].kPresentTime, outDisplayPresentTime);
    EXPECT_EQ(mFrames[0].kDequeueReadyTime, outDequeueReadyTime);
    EXPECT_EQ(mFrames[0].kReleaseTime, outReleaseTime);
}

// This test verifies that if the certain timestamps can't possibly exist for
// the most recent frame, then a sync call is not done.
TEST_F(GetFrameTimestampsTest, NoReleaseNoSync) {
    enableFrameTimestamps();

    // Dequeue and queue frame 1.
    const uint64_t fId1 = getNextFrameId();
    dequeueAndQueue(0);
    mFrames[0].signalQueueFences();

    // Dequeue and queue frame 2.
    const uint64_t fId2 = getNextFrameId();
    dequeueAndQueue(1);
    mFrames[1].signalQueueFences();

    addFrameEvents(false, NO_FRAME_INDEX, 0);
    addFrameEvents(false, 0, 1);

    // Verify available timestamps are correct for frame 1, before any
    // fence has been signaled.
    // Note: A sync call is necessary here since the events triggered by
    // addFrameEvents didn't get to piggyback on the earlier queues/dequeues.
    resetTimestamps();
    int oldCount = mFakeConsumer->mGetFrameTimestampsCount;
    int result = getAllFrameTimestamps(fId1);
    EXPECT_EQ(oldCount + 1, mFakeConsumer->mGetFrameTimestampsCount);
    EXPECT_EQ(NO_ERROR, result);
    EXPECT_EQ(mFrames[0].kRequestedPresentTime, outRequestedPresentTime);
    EXPECT_EQ(mFrames[0].kProducerAcquireTime, outAcquireTime);
    EXPECT_EQ(mFrames[0].kLatchTime, outLatchTime);
    EXPECT_EQ(mFrames[0].mRefreshes[0].kStartTime, outFirstRefreshStartTime);
    EXPECT_EQ(mFrames[0].mRefreshes[2].kStartTime, outLastRefreshStartTime);
    EXPECT_EQ(NATIVE_WINDOW_TIMESTAMP_INVALID, outGpuCompositionDoneTime);
    EXPECT_EQ(NATIVE_WINDOW_TIMESTAMP_PENDING, outDisplayPresentTime);
    EXPECT_EQ(mFrames[0].kDequeueReadyTime, outDequeueReadyTime);
    EXPECT_EQ(NATIVE_WINDOW_TIMESTAMP_PENDING, outReleaseTime);

    mFrames[0].signalRefreshFences();
    mFrames[0].signalReleaseFences();
    mFrames[1].signalRefreshFences();

    // Verify querying for all timestmaps of f2 does not do a sync call. Even
    // though the lastRefresh, dequeueReady, and release times aren't
    // available, a sync call should not occur because it's not possible for f2
    // to encounter the final value for those events until another frame is
    // queued.
    resetTimestamps();
    oldCount = mFakeConsumer->mGetFrameTimestampsCount;
    result = getAllFrameTimestamps(fId2);
    EXPECT_EQ(oldCount, mFakeConsumer->mGetFrameTimestampsCount);
    EXPECT_EQ(NO_ERROR, result);
    EXPECT_EQ(mFrames[1].kRequestedPresentTime, outRequestedPresentTime);
    EXPECT_EQ(mFrames[1].kProducerAcquireTime, outAcquireTime);
    EXPECT_EQ(mFrames[1].kLatchTime, outLatchTime);
    EXPECT_EQ(mFrames[1].mRefreshes[0].kStartTime, outFirstRefreshStartTime);
    EXPECT_EQ(mFrames[1].mRefreshes[1].kStartTime, outLastRefreshStartTime);
    EXPECT_EQ(NATIVE_WINDOW_TIMESTAMP_INVALID, outGpuCompositionDoneTime);
    EXPECT_EQ(mFrames[1].mRefreshes[0].kPresentTime, outDisplayPresentTime);
    EXPECT_EQ(NATIVE_WINDOW_TIMESTAMP_PENDING, outDequeueReadyTime);
    EXPECT_EQ(NATIVE_WINDOW_TIMESTAMP_PENDING, outReleaseTime);
}

// This test verifies there are no sync calls for present times
// when they aren't supported and that an error is returned.

TEST_F(GetFrameTimestampsTest, PresentUnsupportedNoSync) {
    enableFrameTimestamps();
    mSurface->mFakeSurfaceComposer->setSupportsPresent(false);

    // Dequeue and queue frame 1.
    const uint64_t fId1 = getNextFrameId();
    dequeueAndQueue(0);

    // Verify a query for the Present times do not trigger a sync call if they
    // are not supported.
    resetTimestamps();
    int oldCount = mFakeConsumer->mGetFrameTimestampsCount;
    int result = native_window_get_frame_timestamps(mWindow.get(), fId1,
            nullptr, nullptr, nullptr, nullptr, nullptr, nullptr,
            &outDisplayPresentTime, nullptr, nullptr);
    EXPECT_EQ(oldCount, mFakeConsumer->mGetFrameTimestampsCount);
    EXPECT_EQ(BAD_VALUE, result);
    EXPECT_EQ(-1, outDisplayPresentTime);
}

} // namespace android
