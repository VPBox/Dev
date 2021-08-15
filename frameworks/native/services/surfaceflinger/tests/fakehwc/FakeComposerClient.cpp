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

//#define LOG_NDEBUG 0
#undef LOG_TAG
#define LOG_TAG "FakeComposer"

#include "FakeComposerClient.h"

#include <gui/SurfaceComposerClient.h>

#include <log/log.h>

#include <gtest/gtest.h>

#include <inttypes.h>
#include <time.h>
#include <algorithm>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <set>
#include <thread>

constexpr Config NULL_DISPLAY_CONFIG = static_cast<Config>(0);

using namespace sftest;

using android::Condition;
using android::Mutex;

using Clock = std::chrono::steady_clock;
using TimePoint = std::chrono::time_point<Clock>;

namespace {

// Internal state of a layer in the HWC API.
class LayerImpl {
public:
    LayerImpl() = default;

    bool mValid = true;
    RenderState mRenderState;
    uint32_t mZ = 0;
};

// Struct for storing per frame rectangle state. Contains the render
// state shared to the test case. Basically a snapshot and a subset of
// LayerImpl sufficient to re-create the pixels of a layer for the
// frame.
struct FrameRect {
public:
    FrameRect(Layer layer_, const RenderState& state, uint32_t z_)
          : layer(layer_), renderState(state), z(z_) {}

    const Layer layer;
    const RenderState renderState;
    const uint32_t z;
};

// Collection of FrameRects forming one rendered frame. Could store
// related fences and other data in the future.
class Frame {
public:
    Frame() = default;
    std::vector<std::unique_ptr<FrameRect>> rectangles;
};

class DelayedEventGenerator {
public:
    explicit DelayedEventGenerator(std::function<void()> onTimerExpired)
          : mOnTimerExpired(onTimerExpired), mThread([this]() { loop(); }) {}

    ~DelayedEventGenerator() {
        ALOGI("DelayedEventGenerator exiting.");
        {
            std::unique_lock<std::mutex> lock(mMutex);
            mRunning = false;
            mWakeups.clear();
            mCondition.notify_one();
        }
        mThread.join();
        ALOGI("DelayedEventGenerator exited.");
    }

    void wakeAfter(std::chrono::nanoseconds waitTime) {
        std::unique_lock<std::mutex> lock(mMutex);
        mWakeups.insert(Clock::now() + waitTime);
        mCondition.notify_one();
    }

private:
    void loop() {
        while (true) {
            // Lock scope
            {
                std::unique_lock<std::mutex> lock(mMutex);
                mCondition.wait(lock, [this]() { return !mRunning || !mWakeups.empty(); });
                if (!mRunning && mWakeups.empty()) {
                    // This thread should only exit once the destructor has been called and all
                    // wakeups have been processed
                    return;
                }

                // At this point, mWakeups will not be empty

                TimePoint target = *(mWakeups.begin());
                auto status = mCondition.wait_until(lock, target);
                while (status == std::cv_status::no_timeout) {
                    // This was either a spurious wakeup or another wakeup was added, so grab the
                    // oldest point and wait again
                    target = *(mWakeups.begin());
                    status = mCondition.wait_until(lock, target);
                }

                // status must have been timeout, so we can finally clear this point
                mWakeups.erase(target);
            }
            // Callback *without* locks!
            mOnTimerExpired();
        }
    }

    std::function<void()> mOnTimerExpired;
    std::thread mThread;
    std::mutex mMutex;
    std::condition_variable mCondition;
    bool mRunning = true;
    std::set<TimePoint> mWakeups;
};

} // namespace

FakeComposerClient::FakeComposerClient()
      : mEventCallback(nullptr),
        mCurrentConfig(NULL_DISPLAY_CONFIG),
        mVsyncEnabled(false),
        mLayers(),
        mDelayedEventGenerator(
                std::make_unique<DelayedEventGenerator>([this]() { this->requestVSync(); })),
        mSurfaceComposer(nullptr) {}

FakeComposerClient::~FakeComposerClient() {}

bool FakeComposerClient::hasCapability(hwc2_capability_t /*capability*/) {
    return false;
}

std::string FakeComposerClient::dumpDebugInfo() {
    return {};
}

void FakeComposerClient::registerEventCallback(EventCallback* callback) {
    ALOGV("registerEventCallback");
    mEventCallback = callback;
    if (mEventCallback) {
        mEventCallback->onHotplug(PRIMARY_DISPLAY, IComposerCallback::Connection::CONNECTED);
    }
}

void FakeComposerClient::unregisterEventCallback() {
    ALOGV("unregisterEventCallback");
    mEventCallback = nullptr;
}

void FakeComposerClient::hotplugDisplay(Display display, IComposerCallback::Connection state) {
    if (mEventCallback) {
        mEventCallback->onHotplug(display, state);
    }
}

void FakeComposerClient::refreshDisplay(Display display) {
    if (mEventCallback) {
        mEventCallback->onRefresh(display);
    }
}

uint32_t FakeComposerClient::getMaxVirtualDisplayCount() {
    ALOGV("getMaxVirtualDisplayCount");
    return 1;
}

Error FakeComposerClient::createVirtualDisplay(uint32_t /*width*/, uint32_t /*height*/,
                                               PixelFormat* /*format*/, Display* /*outDisplay*/) {
    ALOGV("createVirtualDisplay");
    return Error::NONE;
}

Error FakeComposerClient::destroyVirtualDisplay(Display /*display*/) {
    ALOGV("destroyVirtualDisplay");
    return Error::NONE;
}

Error FakeComposerClient::createLayer(Display /*display*/, Layer* outLayer) {
    ALOGV("createLayer");
    *outLayer = mLayers.size();
    auto newLayer = std::make_unique<LayerImpl>();
    mLayers.push_back(std::move(newLayer));
    return Error::NONE;
}

Error FakeComposerClient::destroyLayer(Display /*display*/, Layer layer) {
    ALOGV("destroyLayer");
    mLayers[layer]->mValid = false;
    return Error::NONE;
}

Error FakeComposerClient::getActiveConfig(Display /*display*/, Config* outConfig) {
    ALOGV("getActiveConfig");

    // TODO Assert outConfig != nullptr

    // TODO This is my reading of the
    // IComposerClient::getActiveConfig, but returning BAD_CONFIG
    // seems to not fit SurfaceFlinger plans. See version 2 below.
    // if (mCurrentConfig == NULL_DISPLAY_CONFIG) {
    //     return Error::BAD_CONFIG;
    // }
    //*outConfig = mCurrentConfig;
    *outConfig = 1; // Very special config for you my friend
    return Error::NONE;
}

Error FakeComposerClient::getClientTargetSupport(Display /*display*/, uint32_t /*width*/,
                                                 uint32_t /*height*/, PixelFormat /*format*/,
                                                 Dataspace /*dataspace*/) {
    ALOGV("getClientTargetSupport");
    return Error::NONE;
}

Error FakeComposerClient::getColorModes(Display /*display*/, hidl_vec<ColorMode>* /*outModes*/) {
    ALOGV("getColorModes");
    return Error::NONE;
}

Error FakeComposerClient::getDisplayAttribute(Display display, Config config,
                                              IComposerClient::Attribute attribute,
                                              int32_t* outValue) {
    ALOGV("getDisplayAttribute (%d, %d, %d, %p)", static_cast<int>(display),
          static_cast<int>(config), static_cast<int>(attribute), outValue);

    // TODO: SOOO much fun to be had with these alone
    switch (attribute) {
        case IComposerClient::Attribute::WIDTH:
            *outValue = 1920;
            break;
        case IComposerClient::Attribute::HEIGHT:
            *outValue = 1080;
            break;
        case IComposerClient::Attribute::VSYNC_PERIOD:
            *outValue = 1666666666;
            break; // TOOD: Tests break down if lowered to 16ms?
        case IComposerClient::Attribute::DPI_X:
            *outValue = 240;
            break;
        case IComposerClient::Attribute::DPI_Y:
            *outValue = 240;
            break;
        default:
            LOG_ALWAYS_FATAL("Say what!?! New attribute");
    }

    return Error::NONE;
}

Error FakeComposerClient::getDisplayConfigs(Display /*display*/, hidl_vec<Config>* outConfigs) {
    ALOGV("getDisplayConfigs");
    // TODO assert display == 1, outConfigs != nullptr

    outConfigs->resize(1);
    (*outConfigs)[0] = 1;

    return Error::NONE;
}

Error FakeComposerClient::getDisplayName(Display /*display*/, hidl_string* /*outName*/) {
    ALOGV("getDisplayName");
    return Error::NONE;
}

Error FakeComposerClient::getDisplayType(Display /*display*/,
                                         IComposerClient::DisplayType* outType) {
    ALOGV("getDisplayType");
    // TODO: This setting nothing on the output had no effect on initial trials. Is first display
    // assumed to be physical?
    *outType = static_cast<IComposerClient::DisplayType>(HWC2_DISPLAY_TYPE_PHYSICAL);
    return Error::NONE;
}

Error FakeComposerClient::getDozeSupport(Display /*display*/, bool* /*outSupport*/) {
    ALOGV("getDozeSupport");
    return Error::NONE;
}

Error FakeComposerClient::getHdrCapabilities(Display /*display*/, hidl_vec<Hdr>* /*outTypes*/,
                                             float* /*outMaxLuminance*/,
                                             float* /*outMaxAverageLuminance*/,
                                             float* /*outMinLuminance*/) {
    ALOGV("getHdrCapabilities");
    return Error::NONE;
}

Error FakeComposerClient::setActiveConfig(Display /*display*/, Config config) {
    ALOGV("setActiveConfig");
    mCurrentConfig = config;
    return Error::NONE;
}

Error FakeComposerClient::setColorMode(Display /*display*/, ColorMode /*mode*/) {
    ALOGV("setColorMode");
    return Error::NONE;
}

Error FakeComposerClient::setPowerMode(Display /*display*/, IComposerClient::PowerMode /*mode*/) {
    ALOGV("setPowerMode");
    return Error::NONE;
}

Error FakeComposerClient::setVsyncEnabled(Display /*display*/, IComposerClient::Vsync enabled) {
    mVsyncEnabled = (enabled == IComposerClient::Vsync::ENABLE);
    ALOGV("setVsyncEnabled(%s)", mVsyncEnabled ? "ENABLE" : "DISABLE");
    return Error::NONE;
}

Error FakeComposerClient::setColorTransform(Display /*display*/, const float* /*matrix*/,
                                            int32_t /*hint*/) {
    ALOGV("setColorTransform");
    return Error::NONE;
}

Error FakeComposerClient::setClientTarget(Display /*display*/, buffer_handle_t /*target*/,
                                          int32_t /*acquireFence*/, int32_t /*dataspace*/,
                                          const std::vector<hwc_rect_t>& /*damage*/) {
    ALOGV("setClientTarget");
    return Error::NONE;
}

Error FakeComposerClient::setOutputBuffer(Display /*display*/, buffer_handle_t /*buffer*/,
                                          int32_t /*releaseFence*/) {
    ALOGV("setOutputBuffer");
    return Error::NONE;
}

Error FakeComposerClient::validateDisplay(
        Display /*display*/, std::vector<Layer>* /*outChangedLayers*/,
        std::vector<IComposerClient::Composition>* /*outCompositionTypes*/,
        uint32_t* /*outDisplayRequestMask*/, std::vector<Layer>* /*outRequestedLayers*/,
        std::vector<uint32_t>* /*outRequestMasks*/) {
    ALOGV("validateDisplay");
    // TODO: Assume touching nothing means All Korrekt!
    return Error::NONE;
}

Error FakeComposerClient::acceptDisplayChanges(Display /*display*/) {
    ALOGV("acceptDisplayChanges");
    // Didn't ask for changes because software is omnipotent.
    return Error::NONE;
}

bool layerZOrdering(const std::unique_ptr<FrameRect>& a, const std::unique_ptr<FrameRect>& b) {
    return a->z <= b->z;
}

Error FakeComposerClient::presentDisplay(Display /*display*/, int32_t* /*outPresentFence*/,
                                         std::vector<Layer>* /*outLayers*/,
                                         std::vector<int32_t>* /*outReleaseFences*/) {
    ALOGV("presentDisplay");
    // TODO Leaving layers and their fences out for now. Doing so
    // means that we've already processed everything. Important to
    // test that the fences are respected, though. (How?)

    std::unique_ptr<Frame> newFrame(new Frame);
    for (uint64_t layer = 0; layer < mLayers.size(); layer++) {
        const LayerImpl& layerImpl = *mLayers[layer];

        if (!layerImpl.mValid) continue;

        auto rect = std::make_unique<FrameRect>(layer, layerImpl.mRenderState, layerImpl.mZ);
        newFrame->rectangles.push_back(std::move(rect));
    }
    std::sort(newFrame->rectangles.begin(), newFrame->rectangles.end(), layerZOrdering);
    {
        Mutex::Autolock _l(mStateMutex);
        mFrames.push_back(std::move(newFrame));
        mFramesAvailable.broadcast();
    }
    return Error::NONE;
}

Error FakeComposerClient::setLayerCursorPosition(Display /*display*/, Layer /*layer*/,
                                                 int32_t /*x*/, int32_t /*y*/) {
    ALOGV("setLayerCursorPosition");
    return Error::NONE;
}

Error FakeComposerClient::setLayerBuffer(Display /*display*/, Layer layer, buffer_handle_t buffer,
                                         int32_t acquireFence) {
    ALOGV("setLayerBuffer");
    LayerImpl& l = getLayerImpl(layer);
    if (buffer != l.mRenderState.mBuffer) {
        l.mRenderState.mSwapCount++; // TODO: Is setting to same value a swap or not?
    }
    l.mRenderState.mBuffer = buffer;
    l.mRenderState.mAcquireFence = acquireFence;

    return Error::NONE;
}

Error FakeComposerClient::setLayerSurfaceDamage(Display /*display*/, Layer /*layer*/,
                                                const std::vector<hwc_rect_t>& /*damage*/) {
    ALOGV("setLayerSurfaceDamage");
    return Error::NONE;
}

Error FakeComposerClient::setLayerBlendMode(Display /*display*/, Layer layer, int32_t mode) {
    ALOGV("setLayerBlendMode");
    getLayerImpl(layer).mRenderState.mBlendMode = static_cast<hwc2_blend_mode_t>(mode);
    return Error::NONE;
}

Error FakeComposerClient::setLayerColor(Display /*display*/, Layer layer,
                                        IComposerClient::Color color) {
    ALOGV("setLayerColor");
    getLayerImpl(layer).mRenderState.mLayerColor.r = color.r;
    getLayerImpl(layer).mRenderState.mLayerColor.g = color.g;
    getLayerImpl(layer).mRenderState.mLayerColor.b = color.b;
    getLayerImpl(layer).mRenderState.mLayerColor.a = color.a;
    return Error::NONE;
}

Error FakeComposerClient::setLayerCompositionType(Display /*display*/, Layer /*layer*/,
                                                  int32_t /*type*/) {
    ALOGV("setLayerCompositionType");
    return Error::NONE;
}

Error FakeComposerClient::setLayerDataspace(Display /*display*/, Layer /*layer*/,
                                            int32_t /*dataspace*/) {
    ALOGV("setLayerDataspace");
    return Error::NONE;
}

Error FakeComposerClient::setLayerDisplayFrame(Display /*display*/, Layer layer,
                                               const hwc_rect_t& frame) {
    ALOGV("setLayerDisplayFrame (%d, %d, %d, %d)", frame.left, frame.top, frame.right,
          frame.bottom);
    getLayerImpl(layer).mRenderState.mDisplayFrame = frame;
    return Error::NONE;
}

Error FakeComposerClient::setLayerPlaneAlpha(Display /*display*/, Layer layer, float alpha) {
    ALOGV("setLayerPlaneAlpha");
    getLayerImpl(layer).mRenderState.mPlaneAlpha = alpha;
    return Error::NONE;
}

Error FakeComposerClient::setLayerSidebandStream(Display /*display*/, Layer /*layer*/,
                                                 buffer_handle_t /*stream*/) {
    ALOGV("setLayerSidebandStream");
    return Error::NONE;
}

Error FakeComposerClient::setLayerSourceCrop(Display /*display*/, Layer layer,
                                             const hwc_frect_t& crop) {
    ALOGV("setLayerSourceCrop");
    getLayerImpl(layer).mRenderState.mSourceCrop = crop;
    return Error::NONE;
}

Error FakeComposerClient::setLayerTransform(Display /*display*/, Layer layer, int32_t transform) {
    ALOGV("setLayerTransform");
    getLayerImpl(layer).mRenderState.mTransform = static_cast<hwc_transform_t>(transform);
    return Error::NONE;
}

Error FakeComposerClient::setLayerVisibleRegion(Display /*display*/, Layer layer,
                                                const std::vector<hwc_rect_t>& visible) {
    ALOGV("setLayerVisibleRegion");
    getLayerImpl(layer).mRenderState.mVisibleRegion = visible;
    return Error::NONE;
}

Error FakeComposerClient::setLayerZOrder(Display /*display*/, Layer layer, uint32_t z) {
    ALOGV("setLayerZOrder");
    getLayerImpl(layer).mZ = z;
    return Error::NONE;
}

//////////////////////////////////////////////////////////////////

void FakeComposerClient::requestVSync(uint64_t vsyncTime) {
    if (mEventCallback) {
        uint64_t timestamp = vsyncTime;
        ALOGV("Vsync");
        if (timestamp == 0) {
            struct timespec ts;
            clock_gettime(CLOCK_MONOTONIC, &ts);
            timestamp = ts.tv_sec * 1000 * 1000 * 1000 + ts.tv_nsec;
        }
        if (mSurfaceComposer != nullptr) {
            mSurfaceComposer->injectVSync(timestamp);
        } else {
            mEventCallback->onVsync(PRIMARY_DISPLAY, timestamp);
        }
    }
}

void FakeComposerClient::runVSyncAfter(std::chrono::nanoseconds wait) {
    mDelayedEventGenerator->wakeAfter(wait);
}

LayerImpl& FakeComposerClient::getLayerImpl(Layer handle) {
    // TODO Change these to an internal state check that can be
    // invoked from the gtest? GTest macros do not seem all that safe
    // when used outside the test class
    EXPECT_GE(handle, static_cast<Layer>(0));
    EXPECT_LT(handle, mLayers.size());
    return *(mLayers[handle]);
}

int FakeComposerClient::getFrameCount() const {
    return mFrames.size();
}

static std::vector<RenderState> extractRenderState(
        const std::vector<std::unique_ptr<FrameRect>>& internalRects) {
    std::vector<RenderState> result;
    result.reserve(internalRects.size());
    for (const std::unique_ptr<FrameRect>& rect : internalRects) {
        result.push_back(rect->renderState);
    }
    return result;
}

std::vector<RenderState> FakeComposerClient::getFrameRects(int frame) const {
    Mutex::Autolock _l(mStateMutex);
    return extractRenderState(mFrames[frame]->rectangles);
}

std::vector<RenderState> FakeComposerClient::getLatestFrame() const {
    Mutex::Autolock _l(mStateMutex);
    return extractRenderState(mFrames[mFrames.size() - 1]->rectangles);
}

void FakeComposerClient::runVSyncAndWait(std::chrono::nanoseconds maxWait) {
    int currentFrame = 0;
    {
        Mutex::Autolock _l(mStateMutex); // I hope this is ok...
        currentFrame = static_cast<int>(mFrames.size());
        requestVSync();
    }
    waitUntilFrame(currentFrame + 1, maxWait);
}

void FakeComposerClient::waitUntilFrame(int targetFrame, std::chrono::nanoseconds maxWait) const {
    Mutex::Autolock _l(mStateMutex);
    while (mFrames.size() < static_cast<size_t>(targetFrame)) {
        android::status_t result = mFramesAvailable.waitRelative(mStateMutex, maxWait.count());
        if (result == android::TIMED_OUT) {
            ALOGE("Waiting for frame %d (at frame %zu now) timed out after %lld ns", targetFrame,
                  mFrames.size(), maxWait.count());
            return;
        }
    }
}

void FakeComposerClient::clearFrames() {
    Mutex::Autolock _l(mStateMutex);
    mFrames.clear();
    for (const std::unique_ptr<LayerImpl>& layer : mLayers) {
        if (layer->mValid) {
            layer->mRenderState.mSwapCount = 0;
        }
    }
}

void FakeComposerClient::onSurfaceFlingerStart() {
    mSurfaceComposer = nullptr;
    do {
        mSurfaceComposer = new android::SurfaceComposerClient;
        android::status_t initResult = mSurfaceComposer->initCheck();
        if (initResult != android::NO_ERROR) {
            ALOGD("Init result: %d", initResult);
            mSurfaceComposer = nullptr;
            std::this_thread::sleep_for(10ms);
        }
    } while (mSurfaceComposer == nullptr);
    ALOGD("SurfaceComposerClient created");
    mSurfaceComposer->enableVSyncInjections(true);
}

void FakeComposerClient::onSurfaceFlingerStop() {
    mSurfaceComposer->dispose();
    mSurfaceComposer.clear();
}

// Includes destroyed layers, stored in order of creation.
int FakeComposerClient::getLayerCount() const {
    return mLayers.size();
}

Layer FakeComposerClient::getLayer(size_t index) const {
    // NOTE: If/when passing calls through to actual implementation,
    // this might get more involving.
    return static_cast<Layer>(index);
}
