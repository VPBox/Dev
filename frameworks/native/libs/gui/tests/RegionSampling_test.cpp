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

#include <gtest/gtest.h>
#include <thread>

#include <binder/ProcessState.h>
#include <gui/DisplayEventReceiver.h>
#include <gui/IRegionSamplingListener.h>
#include <gui/ISurfaceComposer.h>
#include <gui/Surface.h>
#include <gui/SurfaceComposerClient.h>
#include <private/gui/ComposerService.h>
#include <utils/Looper.h>

using namespace std::chrono_literals;

namespace android::test {

struct ChoreographerSync {
    ChoreographerSync(DisplayEventReceiver& receiver) : receiver_(receiver) {}
    ~ChoreographerSync() = default;

    void notify() const {
        std::unique_lock<decltype(mutex_)> lk(mutex_);

        auto check_event = [](auto const& ev) -> bool {
            return ev.header.type == DisplayEventReceiver::DISPLAY_EVENT_VSYNC;
        };
        DisplayEventReceiver::Event ev_;
        int evs = receiver_.getEvents(&ev_, 1);
        auto vsync_event_found = check_event(ev_);
        while (evs) {
            evs = receiver_.getEvents(&ev_, 1);
            vsync_event_found |= check_event(ev_);
        }

        if (vsync_event_found) {
            notification_arrived_ = true;
            cv_.notify_all();
        }
    }

    void wait_vsync_notify() const {
        std::unique_lock<decltype(mutex_)> lk(mutex_);
        cv_.wait(lk, [this] { return notification_arrived_; });
        notification_arrived_ = false;
    }

private:
    ChoreographerSync(ChoreographerSync const&) = delete;
    ChoreographerSync& operator=(ChoreographerSync const&) = delete;

    std::mutex mutable mutex_;
    std::condition_variable mutable cv_;
    bool mutable notification_arrived_ = false;
    DisplayEventReceiver& receiver_;
};

struct ChoreographerSim {
    static std::unique_ptr<ChoreographerSim> make() {
        auto receiver = std::make_unique<DisplayEventReceiver>();
        if (!receiver || receiver->initCheck() == NO_INIT) {
            ALOGE("No display reciever");
            return nullptr;
        }
        return std::unique_ptr<ChoreographerSim>(new ChoreographerSim(std::move(receiver)));
    }

    ~ChoreographerSim() {
        poll_ = false;
        looper->wake();
        choreographer_thread_.join();
    }

    void request_render_wait(std::function<void()> const& render_fn) {
        display_event_receiver_->requestNextVsync();
        choreographer_.wait_vsync_notify();
        render_fn();

        // Purpose is to make sure that the content is latched by the time we sample.
        // Waiting one vsync after queueing could still race with vsync, so wait for two, after
        // which the content is pretty reliably on screen.
        display_event_receiver_->requestNextVsync();
        choreographer_.wait_vsync_notify();
        display_event_receiver_->requestNextVsync();
        choreographer_.wait_vsync_notify();
    }

private:
    ChoreographerSim(std::unique_ptr<DisplayEventReceiver> receiver)
          : display_event_receiver_{std::move(receiver)},
            choreographer_{*display_event_receiver_},
            looper{new Looper(false)} {
        choreographer_thread_ = std::thread([this] {
            auto vsync_notify_fd = display_event_receiver_->getFd();
            looper->addFd(vsync_notify_fd, 0, Looper::EVENT_INPUT,
                          [](int /*fd*/, int /*events*/, void* data) -> int {
                              if (!data) return 0;
                              reinterpret_cast<ChoreographerSync*>(data)->notify();
                              return 1;
                          },
                          const_cast<void*>(reinterpret_cast<void const*>(&choreographer_)));

            while (poll_) {
                auto const poll_interval =
                        std::chrono::duration_cast<std::chrono::milliseconds>(1s).count();
                auto rc = looper->pollOnce(poll_interval);
                if ((rc != Looper::POLL_CALLBACK) && (rc != Looper::POLL_WAKE))
                    ALOGW("Vsync Looper returned: %i\n", rc);
            }
        });
    }

    ChoreographerSim(ChoreographerSim const&) = delete;
    ChoreographerSim& operator=(ChoreographerSim const&) = delete;

    std::unique_ptr<DisplayEventReceiver> const display_event_receiver_;
    ChoreographerSync const choreographer_;
    sp<Looper> looper;
    std::thread choreographer_thread_;
    std::atomic<bool> poll_{true};
};

struct Listener : BnRegionSamplingListener {
    void onSampleCollected(float medianLuma) override {
        std::unique_lock<decltype(mutex)> lk(mutex);
        received = true;
        mLuma = medianLuma;
        cv.notify_all();
    };
    bool wait_event(std::chrono::milliseconds timeout) {
        std::unique_lock<decltype(mutex)> lk(mutex);
        return cv.wait_for(lk, timeout, [this] { return received; });
    }

    float luma() {
        std::unique_lock<decltype(mutex)> lk(mutex);
        return mLuma;
    }

    void reset() {
        std::unique_lock<decltype(mutex)> lk(mutex);
        received = false;
    }

private:
    std::condition_variable cv;
    std::mutex mutex;
    bool received = false;
    float mLuma = -0.0f;
};

// Hoisted to TestSuite setup to avoid flake in test (b/124675919)
std::unique_ptr<ChoreographerSim> gChoreographerSim = nullptr;

struct RegionSamplingTest : ::testing::Test {
protected:
    RegionSamplingTest() { ProcessState::self()->startThreadPool(); }

    static void SetUpTestSuite() {
        gChoreographerSim = ChoreographerSim::make();
        ASSERT_NE(gChoreographerSim, nullptr);
    }

    void SetUp() override {
        mSurfaceComposerClient = new SurfaceComposerClient;
        ASSERT_EQ(NO_ERROR, mSurfaceComposerClient->initCheck());

        mBackgroundLayer =
                mSurfaceComposerClient->createSurface(String8("Background RegionSamplingTest"), 0,
                                                      0, PIXEL_FORMAT_RGBA_8888,
                                                      ISurfaceComposerClient::eFXSurfaceColor);
        uint32_t layerPositionBottom = 0x7E000000;
        SurfaceComposerClient::Transaction{}
                .setLayer(mBackgroundLayer, layerPositionBottom)
                .setPosition(mBackgroundLayer, 100, 100)
                .setColor(mBackgroundLayer, half3{0.5, 0.5, 0.5})
                .show(mBackgroundLayer)
                .apply();

        mContentLayer = mSurfaceComposerClient->createSurface(String8("Content RegionSamplingTest"),
                                                              300, 300, PIXEL_FORMAT_RGBA_8888, 0);

        SurfaceComposerClient::Transaction{}
                .setLayer(mContentLayer, layerPositionBottom + 1)
                .setPosition(mContentLayer, 100, 100)
                .setColor(mContentLayer, half3{0.5, 0.5, 0.5})
                .show(mContentLayer)
                .apply();

        mTopLayer = mSurfaceComposerClient->createSurface(String8("TopLayer RegionSamplingTest"), 0,
                                                          0, PIXEL_FORMAT_RGBA_8888, 0);
        SurfaceComposerClient::Transaction{}
                .setLayer(mTopLayer, layerPositionBottom + 2)
                .setPosition(mTopLayer, 0, 0)
                .show(mBackgroundLayer)
                .apply();
    }

    void fill_render(uint32_t rgba_value) {
        auto surface = mContentLayer->getSurface();
        ANativeWindow_Buffer outBuffer;
        status_t status = surface->lock(&outBuffer, NULL);
        ASSERT_EQ(status, android::OK);
        auto b = reinterpret_cast<uint32_t*>(outBuffer.bits);
        for (auto i = 0; i < outBuffer.height; i++) {
            for (auto j = 0; j < outBuffer.width; j++) {
                b[j] = rgba_value;
            }
            b += outBuffer.stride;
        }

        gChoreographerSim->request_render_wait([&surface] { surface->unlockAndPost(); });
    }

    sp<SurfaceComposerClient> mSurfaceComposerClient;
    sp<SurfaceControl> mBackgroundLayer;
    sp<SurfaceControl> mContentLayer;
    sp<SurfaceControl> mTopLayer;

    uint32_t const rgba_green = 0xFF00FF00;
    float const luma_green = 0.7152;
    uint32_t const rgba_blue = 0xFFFF0000;
    float const luma_blue = 0.0722;
    float const error_margin = 0.01;
    float const luma_gray = 0.50;
};

TEST_F(RegionSamplingTest, DISABLED_CollectsLuma) {
    fill_render(rgba_green);

    sp<ISurfaceComposer> composer = ComposerService::getComposerService();
    sp<Listener> listener = new Listener();
    const Rect sampleArea{100, 100, 200, 200};
    composer->addRegionSamplingListener(sampleArea, mTopLayer->getHandle(), listener);

    EXPECT_TRUE(listener->wait_event(300ms)) << "timed out waiting for luma event to be received";
    EXPECT_NEAR(listener->luma(), luma_green, error_margin);

    composer->removeRegionSamplingListener(listener);
}

TEST_F(RegionSamplingTest, DISABLED_CollectsChangingLuma) {
    fill_render(rgba_green);

    sp<ISurfaceComposer> composer = ComposerService::getComposerService();
    sp<Listener> listener = new Listener();
    const Rect sampleArea{100, 100, 200, 200};
    composer->addRegionSamplingListener(sampleArea, mTopLayer->getHandle(), listener);

    EXPECT_TRUE(listener->wait_event(300ms)) << "timed out waiting for luma event to be received";
    EXPECT_NEAR(listener->luma(), luma_green, error_margin);

    listener->reset();

    fill_render(rgba_blue);
    EXPECT_TRUE(listener->wait_event(300ms))
            << "timed out waiting for 2nd luma event to be received";
    EXPECT_NEAR(listener->luma(), luma_blue, error_margin);

    composer->removeRegionSamplingListener(listener);
}

TEST_F(RegionSamplingTest, DISABLED_CollectsLumaFromTwoRegions) {
    fill_render(rgba_green);
    sp<ISurfaceComposer> composer = ComposerService::getComposerService();
    sp<Listener> greenListener = new Listener();
    const Rect greenSampleArea{100, 100, 200, 200};
    composer->addRegionSamplingListener(greenSampleArea, mTopLayer->getHandle(), greenListener);

    sp<Listener> grayListener = new Listener();
    const Rect graySampleArea{500, 100, 600, 200};
    composer->addRegionSamplingListener(graySampleArea, mTopLayer->getHandle(), grayListener);

    EXPECT_TRUE(grayListener->wait_event(300ms))
            << "timed out waiting for luma event to be received";
    EXPECT_NEAR(grayListener->luma(), luma_gray, error_margin);
    EXPECT_TRUE(greenListener->wait_event(300ms))
            << "timed out waiting for luma event to be received";
    EXPECT_NEAR(greenListener->luma(), luma_green, error_margin);

    composer->removeRegionSamplingListener(greenListener);
    composer->removeRegionSamplingListener(grayListener);
}

} // namespace android::test
