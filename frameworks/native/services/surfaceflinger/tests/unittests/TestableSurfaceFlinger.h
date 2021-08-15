/*
 * Copyright (C) 2018 The Android Open Source Project
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

#pragma once

#include <compositionengine/Display.h>
#include <compositionengine/Layer.h>
#include <compositionengine/OutputLayer.h>
#include <compositionengine/impl/CompositionEngine.h>
#include <compositionengine/impl/LayerCompositionState.h>
#include <compositionengine/impl/OutputLayerCompositionState.h>

#include "BufferQueueLayer.h"
#include "BufferStateLayer.h"
#include "ColorLayer.h"
#include "ContainerLayer.h"
#include "DisplayDevice.h"
#include "FakePhaseOffsets.h"
#include "Layer.h"
#include "NativeWindowSurface.h"
#include "Scheduler/MessageQueue.h"
#include "StartPropertySetThread.h"
#include "SurfaceFlinger.h"
#include "SurfaceFlingerFactory.h"
#include "SurfaceInterceptor.h"

#include "TimeStats/TimeStats.h"

namespace android {

class EventThread;

namespace renderengine {

class RenderEngine;

} // namespace renderengine

namespace Hwc2 {

class Composer;

} // namespace Hwc2

namespace surfaceflinger::test {

class Factory final : public surfaceflinger::Factory {
public:
    ~Factory() = default;

    std::unique_ptr<DispSync> createDispSync(const char*, bool, int64_t) override {
        // TODO: Use test-fixture controlled factory
        return nullptr;
    }

    std::unique_ptr<EventControlThread> createEventControlThread(
            std::function<void(bool)>) override {
        // TODO: Use test-fixture controlled factory
        return nullptr;
    }

    std::unique_ptr<HWComposer> createHWComposer(const std::string&) override {
        // TODO: Use test-fixture controlled factory
        return nullptr;
    }

    std::unique_ptr<MessageQueue> createMessageQueue() override {
        // TODO: Use test-fixture controlled factory
        return std::make_unique<android::impl::MessageQueue>();
    }

    std::unique_ptr<scheduler::PhaseOffsets> createPhaseOffsets() override {
        return std::make_unique<scheduler::FakePhaseOffsets>();
    }

    std::unique_ptr<Scheduler> createScheduler(std::function<void(bool)>,
                                               const scheduler::RefreshRateConfigs&) override {
        // TODO: Use test-fixture controlled factory
        return nullptr;
    }

    std::unique_ptr<SurfaceInterceptor> createSurfaceInterceptor(SurfaceFlinger* flinger) override {
        // TODO: Use test-fixture controlled factory
        return std::make_unique<android::impl::SurfaceInterceptor>(flinger);
    }

    sp<StartPropertySetThread> createStartPropertySetThread(bool timestampPropertyValue) override {
        // TODO: Use test-fixture controlled factory
        return new StartPropertySetThread(timestampPropertyValue);
    }

    sp<DisplayDevice> createDisplayDevice(DisplayDeviceCreationArgs&& creationArgs) override {
        // TODO: Use test-fixture controlled factory
        return new DisplayDevice(std::move(creationArgs));
    }

    sp<GraphicBuffer> createGraphicBuffer(uint32_t width, uint32_t height, PixelFormat format,
                                          uint32_t layerCount, uint64_t usage,
                                          std::string requestorName) override {
        // TODO: Use test-fixture controlled factory
        return new GraphicBuffer(width, height, format, layerCount, usage, requestorName);
    }

    void createBufferQueue(sp<IGraphicBufferProducer>* outProducer,
                           sp<IGraphicBufferConsumer>* outConsumer,
                           bool consumerIsSurfaceFlinger) override {
        if (!mCreateBufferQueue) return;
        mCreateBufferQueue(outProducer, outConsumer, consumerIsSurfaceFlinger);
    }

    std::unique_ptr<surfaceflinger::NativeWindowSurface> createNativeWindowSurface(
            const sp<IGraphicBufferProducer>& producer) override {
        if (!mCreateNativeWindowSurface) return nullptr;
        return mCreateNativeWindowSurface(producer);
    }

    std::unique_ptr<compositionengine::CompositionEngine> createCompositionEngine() override {
        return compositionengine::impl::createCompositionEngine();
    }

    sp<BufferQueueLayer> createBufferQueueLayer(const LayerCreationArgs&) override {
        // TODO: Use test-fixture controlled factory
        return nullptr;
    }

    sp<BufferStateLayer> createBufferStateLayer(const LayerCreationArgs&) override {
        // TODO: Use test-fixture controlled factory
        return nullptr;
    }

    sp<ColorLayer> createColorLayer(const LayerCreationArgs&) override {
        // TODO: Use test-fixture controlled factory
        return nullptr;
    }

    sp<ContainerLayer> createContainerLayer(const LayerCreationArgs&) override {
        // TODO: Use test-fixture controlled factory
        return nullptr;
    }

    std::shared_ptr<TimeStats> createTimeStats() override {
        // TODO: Use test-fixture controlled factory
        return std::make_shared<android::impl::TimeStats>();
    }

    using CreateBufferQueueFunction =
            std::function<void(sp<IGraphicBufferProducer>* /* outProducer */,
                               sp<IGraphicBufferConsumer>* /* outConsumer */,
                               bool /* consumerIsSurfaceFlinger */)>;
    CreateBufferQueueFunction mCreateBufferQueue;

    using CreateNativeWindowSurfaceFunction =
            std::function<std::unique_ptr<surfaceflinger::NativeWindowSurface>(
                    const sp<IGraphicBufferProducer>&)>;
    CreateNativeWindowSurfaceFunction mCreateNativeWindowSurface;

    using CreateCompositionEngineFunction =
            std::function<std::unique_ptr<compositionengine::CompositionEngine>()>;
    CreateCompositionEngineFunction mCreateCompositionEngine;
};

} // namespace surfaceflinger::test

class TestableSurfaceFlinger {
public:
    // Extend this as needed for accessing SurfaceFlinger private (and public)
    // functions.

    void setupRenderEngine(std::unique_ptr<renderengine::RenderEngine> renderEngine) {
        mFlinger->mCompositionEngine->setRenderEngine(std::move(renderEngine));
    }

    void setupComposer(std::unique_ptr<Hwc2::Composer> composer) {
        mFlinger->mCompositionEngine->setHwComposer(
                std::make_unique<impl::HWComposer>(std::move(composer)));
    }

    using CreateBufferQueueFunction = surfaceflinger::test::Factory::CreateBufferQueueFunction;
    void setCreateBufferQueueFunction(CreateBufferQueueFunction f) {
        mFactory.mCreateBufferQueue = f;
    }

    using CreateNativeWindowSurfaceFunction =
            surfaceflinger::test::Factory::CreateNativeWindowSurfaceFunction;
    void setCreateNativeWindowSurface(CreateNativeWindowSurfaceFunction f) {
        mFactory.mCreateNativeWindowSurface = f;
    }

    void setInternalDisplayPrimaries(const ui::DisplayPrimaries& primaries) {
        memcpy(&mFlinger->mInternalDisplayPrimaries, &primaries, sizeof(ui::DisplayPrimaries));
    }

    using HotplugEvent = SurfaceFlinger::HotplugEvent;

    auto& mutableLayerCurrentState(sp<Layer> layer) { return layer->mCurrentState; }
    auto& mutableLayerDrawingState(sp<Layer> layer) { return layer->mDrawingState; }

    void setLayerSidebandStream(sp<Layer> layer, sp<NativeHandle> sidebandStream) {
        layer->mDrawingState.sidebandStream = sidebandStream;
        layer->mSidebandStream = sidebandStream;
        layer->getCompositionLayer()->editState().frontEnd.sidebandStream = sidebandStream;
    }

    void setLayerCompositionType(sp<Layer> layer, HWC2::Composition type) {
        auto outputLayer = layer->findOutputLayerForDisplay(mFlinger->getDefaultDisplayDevice());
        LOG_ALWAYS_FATAL_IF(!outputLayer);
        auto& state = outputLayer->editState();
        LOG_ALWAYS_FATAL_IF(!outputLayer->getState().hwc);
        (*state.hwc).hwcCompositionType = static_cast<Hwc2::IComposerClient::Composition>(type);
    };

    void setLayerPotentialCursor(sp<Layer> layer, bool potentialCursor) {
        layer->mPotentialCursor = potentialCursor;
    }

    /* ------------------------------------------------------------------------
     * Forwarding for functions being tested
     */

    auto createDisplay(const String8& displayName, bool secure) {
        return mFlinger->createDisplay(displayName, secure);
    }

    auto destroyDisplay(const sp<IBinder>& displayToken) {
        return mFlinger->destroyDisplay(displayToken);
    }

    auto resetDisplayState() { return mFlinger->resetDisplayState(); }

    auto setupNewDisplayDeviceInternal(const wp<IBinder>& displayToken,
                                       const std::optional<DisplayId>& displayId,
                                       const DisplayDeviceState& state,
                                       const sp<compositionengine::DisplaySurface>& dispSurface,
                                       const sp<IGraphicBufferProducer>& producer) {
        return mFlinger->setupNewDisplayDeviceInternal(displayToken, displayId, state, dispSurface,
                                                       producer);
    }

    auto handleTransactionLocked(uint32_t transactionFlags) {
        Mutex::Autolock _l(mFlinger->mStateLock);
        return mFlinger->handleTransactionLocked(transactionFlags);
    }

    auto onHotplugReceived(int32_t sequenceId, hwc2_display_t display,
                           HWC2::Connection connection) {
        return mFlinger->onHotplugReceived(sequenceId, display, connection);
    }

    auto setDisplayStateLocked(const DisplayState& s) {
        Mutex::Autolock _l(mFlinger->mStateLock);
        return mFlinger->setDisplayStateLocked(s);
    }

    // Allow reading display state without locking, as if called on the SF main thread.
    auto onInitializeDisplays() NO_THREAD_SAFETY_ANALYSIS {
        return mFlinger->onInitializeDisplays();
    }

    // Allow reading display state without locking, as if called on the SF main thread.
    auto setPowerModeInternal(const sp<DisplayDevice>& display,
                              int mode) NO_THREAD_SAFETY_ANALYSIS {
        return mFlinger->setPowerModeInternal(display, mode);
    }

    auto onMessageReceived(int32_t what) { return mFlinger->onMessageReceived(what); }

    auto captureScreenImplLocked(
            const RenderArea& renderArea, SurfaceFlinger::TraverseLayersFunction traverseLayers,
            ANativeWindowBuffer* buffer, bool useIdentityTransform, bool forSystem, int* outSyncFd) {
        bool ignored;
        return mFlinger->captureScreenImplLocked(renderArea, traverseLayers, buffer,
                                                 useIdentityTransform, forSystem, outSyncFd,
                                                 ignored);
    }

    auto traverseLayersInDisplay(const sp<const DisplayDevice>& display,
                                 const LayerVector::Visitor& visitor) {
        return mFlinger->SurfaceFlinger::traverseLayersInDisplay(display, visitor);
    }

    auto getDisplayNativePrimaries(const sp<IBinder>& displayToken,
                                   ui::DisplayPrimaries &primaries) {
        return mFlinger->SurfaceFlinger::getDisplayNativePrimaries(displayToken, primaries);
    }

    /* ------------------------------------------------------------------------
     * Read-only access to private data to assert post-conditions.
     */

    const auto& getAnimFrameTracker() const { return mFlinger->mAnimFrameTracker; }
    const auto& getHasPoweredOff() const { return mFlinger->mHasPoweredOff; }
    const auto& getVisibleRegionsDirty() const { return mFlinger->mVisibleRegionsDirty; }
    auto& getHwComposer() const {
        return static_cast<impl::HWComposer&>(mFlinger->getHwComposer());
    }

    const auto& getCompositorTiming() const { return mFlinger->getBE().mCompositorTiming; }

    /* ------------------------------------------------------------------------
     * Read-write access to private data to set up preconditions and assert
     * post-conditions.
     */

    auto& mutableHasWideColorDisplay() { return SurfaceFlinger::hasWideColorDisplay; }
    auto& mutablePrimaryDisplayOrientation() { return SurfaceFlinger::primaryDisplayOrientation; }
    auto& mutableUseColorManagement() { return SurfaceFlinger::useColorManagement; }

    auto& mutableCurrentState() { return mFlinger->mCurrentState; }
    auto& mutableDisplayColorSetting() { return mFlinger->mDisplayColorSetting; }
    auto& mutableDisplays() { return mFlinger->mDisplays; }
    auto& mutableDrawingState() { return mFlinger->mDrawingState; }
    auto& mutableEventQueue() { return mFlinger->mEventQueue; }
    auto& mutableGeometryInvalid() { return mFlinger->mGeometryInvalid; }
    auto& mutableInterceptor() { return mFlinger->mInterceptor; }
    auto& mutableMainThreadId() { return mFlinger->mMainThreadId; }
    auto& mutablePendingHotplugEvents() { return mFlinger->mPendingHotplugEvents; }
    auto& mutablePhysicalDisplayTokens() { return mFlinger->mPhysicalDisplayTokens; }
    auto& mutableTexturePool() { return mFlinger->mTexturePool; }
    auto& mutableTransactionFlags() { return mFlinger->mTransactionFlags; }
    auto& mutableUseHwcVirtualDisplays() { return mFlinger->mUseHwcVirtualDisplays; }
    auto& mutablePowerAdvisor() { return mFlinger->mPowerAdvisor; }

    auto& mutableComposerSequenceId() { return mFlinger->getBE().mComposerSequenceId; }
    auto& mutableHwcDisplayData() { return getHwComposer().mDisplayData; }
    auto& mutableHwcPhysicalDisplayIdMap() { return getHwComposer().mPhysicalDisplayIdMap; }
    auto& mutableInternalHwcDisplayId() { return getHwComposer().mInternalHwcDisplayId; }
    auto& mutableExternalHwcDisplayId() { return getHwComposer().mExternalHwcDisplayId; }
    auto& mutableScheduler() { return mFlinger->mScheduler; }
    auto& mutableAppConnectionHandle() { return mFlinger->mAppConnectionHandle; }
    auto& mutableSfConnectionHandle() { return mFlinger->mSfConnectionHandle; }
    auto& mutableRefreshRateConfigs() { return mFlinger->mRefreshRateConfigs; }

    ~TestableSurfaceFlinger() {
        // All these pointer and container clears help ensure that GMock does
        // not report a leaked object, since the SurfaceFlinger instance may
        // still be referenced by something despite our best efforts to destroy
        // it after each test is done.
        mutableDisplays().clear();
        mutableCurrentState().displays.clear();
        mutableDrawingState().displays.clear();
        mutableEventQueue().reset();
        mutableInterceptor().reset();
        mutableScheduler().reset();
        mFlinger->mCompositionEngine->setHwComposer(std::unique_ptr<HWComposer>());
        mFlinger->mCompositionEngine->setRenderEngine(
                std::unique_ptr<renderengine::RenderEngine>());
    }

    /* ------------------------------------------------------------------------
     * Wrapper classes for Read-write access to private data to set up
     * preconditions and assert post-conditions.
     */
    struct HWC2Display : public HWC2::impl::Display {
        HWC2Display(Hwc2::Composer& composer,
                    const std::unordered_set<HWC2::Capability>& capabilities, hwc2_display_t id,
                    HWC2::DisplayType type)
              : HWC2::impl::Display(composer, capabilities, id, type) {}
        ~HWC2Display() {
            // Prevents a call to disable vsyncs.
            mType = HWC2::DisplayType::Invalid;
        }

        auto& mutableIsConnected() { return this->mIsConnected; }
        auto& mutableConfigs() { return this->mConfigs; }
        auto& mutableLayers() { return this->mLayers; }
    };

    class FakeHwcDisplayInjector {
    public:
        static constexpr hwc2_display_t DEFAULT_HWC_DISPLAY_ID = 1000;
        static constexpr int32_t DEFAULT_WIDTH = 1920;
        static constexpr int32_t DEFAULT_HEIGHT = 1280;
        static constexpr int32_t DEFAULT_REFRESH_RATE = 16'666'666;
        static constexpr int32_t DEFAULT_DPI = 320;
        static constexpr int32_t DEFAULT_ACTIVE_CONFIG = 0;
        static constexpr int32_t DEFAULT_POWER_MODE = 2;

        FakeHwcDisplayInjector(DisplayId displayId, HWC2::DisplayType hwcDisplayType,
                               bool isPrimary)
              : mDisplayId(displayId), mHwcDisplayType(hwcDisplayType), mIsPrimary(isPrimary) {}

        auto& setHwcDisplayId(hwc2_display_t displayId) {
            mHwcDisplayId = displayId;
            return *this;
        }

        auto& setWidth(int32_t width) {
            mWidth = width;
            return *this;
        }

        auto& setHeight(int32_t height) {
            mHeight = height;
            return *this;
        }

        auto& setRefreshRate(int32_t refreshRate) {
            mRefreshRate = refreshRate;
            return *this;
        }

        auto& setDpiX(int32_t dpi) {
            mDpiX = dpi;
            return *this;
        }

        auto& setDpiY(int32_t dpi) {
            mDpiY = dpi;
            return *this;
        }

        auto& setActiveConfig(int32_t config) {
            mActiveConfig = config;
            return *this;
        }

        auto& setCapabilities(const std::unordered_set<HWC2::Capability>* capabilities) {
            mCapabilities = capabilities;
            return *this;
        }

        auto& setPowerMode(int mode) {
            mPowerMode = mode;
            return *this;
        }

        void inject(TestableSurfaceFlinger* flinger, Hwc2::Composer* composer) {
            static const std::unordered_set<HWC2::Capability> defaultCapabilities;
            if (mCapabilities == nullptr) mCapabilities = &defaultCapabilities;

            // Caution - Make sure that any values passed by reference here do
            // not refer to an instance owned by FakeHwcDisplayInjector. This
            // class has temporary lifetime, while the constructed HWC2::Display
            // is much longer lived.
            auto display = std::make_unique<HWC2Display>(*composer, *mCapabilities, mHwcDisplayId,
                                                         mHwcDisplayType);

            auto config = HWC2::Display::Config::Builder(*display, mActiveConfig);
            config.setWidth(mWidth);
            config.setHeight(mHeight);
            config.setVsyncPeriod(mRefreshRate);
            config.setDpiX(mDpiX);
            config.setDpiY(mDpiY);
            display->mutableConfigs().emplace(mActiveConfig, config.build());
            display->mutableIsConnected() = true;
            display->setPowerMode(static_cast<HWC2::PowerMode>(mPowerMode));

            flinger->mutableHwcDisplayData()[mDisplayId].hwcDisplay = display.get();

            if (mHwcDisplayType == HWC2::DisplayType::Physical) {
                flinger->mutableHwcPhysicalDisplayIdMap().emplace(mHwcDisplayId, mDisplayId);
                (mIsPrimary ? flinger->mutableInternalHwcDisplayId()
                            : flinger->mutableExternalHwcDisplayId()) = mHwcDisplayId;
            }

            flinger->mFakeHwcDisplays.push_back(std::move(display));
        }

    private:
        const DisplayId mDisplayId;
        const HWC2::DisplayType mHwcDisplayType;
        const bool mIsPrimary;

        hwc2_display_t mHwcDisplayId = DEFAULT_HWC_DISPLAY_ID;
        int32_t mWidth = DEFAULT_WIDTH;
        int32_t mHeight = DEFAULT_HEIGHT;
        int32_t mRefreshRate = DEFAULT_REFRESH_RATE;
        int32_t mDpiX = DEFAULT_DPI;
        int32_t mDpiY = DEFAULT_DPI;
        int32_t mActiveConfig = DEFAULT_ACTIVE_CONFIG;
        int32_t mPowerMode = DEFAULT_POWER_MODE;
        const std::unordered_set<HWC2::Capability>* mCapabilities = nullptr;
    };

    class FakeDisplayDeviceInjector {
    public:
        FakeDisplayDeviceInjector(TestableSurfaceFlinger& flinger,
                                  const std::optional<DisplayId>& displayId, bool isVirtual,
                                  bool isPrimary)
              : mFlinger(flinger), mCreationArgs(flinger.mFlinger.get(), mDisplayToken, displayId) {
            mCreationArgs.isVirtual = isVirtual;
            mCreationArgs.isPrimary = isPrimary;
        }

        sp<IBinder> token() const { return mDisplayToken; }

        DisplayDeviceState& mutableDrawingDisplayState() {
            return mFlinger.mutableDrawingState().displays.editValueFor(mDisplayToken);
        }

        DisplayDeviceState& mutableCurrentDisplayState() {
            return mFlinger.mutableCurrentState().displays.editValueFor(mDisplayToken);
        }

        const auto& getDrawingDisplayState() {
            return mFlinger.mutableDrawingState().displays.valueFor(mDisplayToken);
        }

        const auto& getCurrentDisplayState() {
            return mFlinger.mutableCurrentState().displays.valueFor(mDisplayToken);
        }

        auto& mutableDisplayDevice() { return mFlinger.mutableDisplays()[mDisplayToken]; }

        auto& setNativeWindow(const sp<ANativeWindow>& nativeWindow) {
            mCreationArgs.nativeWindow = nativeWindow;
            return *this;
        }

        auto& setDisplaySurface(const sp<compositionengine::DisplaySurface>& displaySurface) {
            mCreationArgs.displaySurface = displaySurface;
            return *this;
        }

        auto& setSecure(bool secure) {
            mCreationArgs.isSecure = secure;
            return *this;
        }

        auto& setPowerMode(int mode) {
            mCreationArgs.initialPowerMode = mode;
            return *this;
        }

        auto& setHwcColorModes(
                const std::unordered_map<ui::ColorMode, std::vector<ui::RenderIntent>>
                        hwcColorModes) {
            mCreationArgs.hwcColorModes = hwcColorModes;
            return *this;
        }

        auto& setHasWideColorGamut(bool hasWideColorGamut) {
            mCreationArgs.hasWideColorGamut = hasWideColorGamut;
            return *this;
        }

        sp<DisplayDevice> inject() {
            DisplayDeviceState state;
            state.displayId = mCreationArgs.isVirtual ? std::nullopt : mCreationArgs.displayId;
            state.isSecure = mCreationArgs.isSecure;

            sp<DisplayDevice> device = new DisplayDevice(std::move(mCreationArgs));
            mFlinger.mutableDisplays().emplace(mDisplayToken, device);
            mFlinger.mutableCurrentState().displays.add(mDisplayToken, state);
            mFlinger.mutableDrawingState().displays.add(mDisplayToken, state);

            if (!mCreationArgs.isVirtual) {
                LOG_ALWAYS_FATAL_IF(!state.displayId);
                mFlinger.mutablePhysicalDisplayTokens()[*state.displayId] = mDisplayToken;
            }

            return device;
        }

    private:
        TestableSurfaceFlinger& mFlinger;
        sp<BBinder> mDisplayToken = new BBinder();
        DisplayDeviceCreationArgs mCreationArgs;
    };

    surfaceflinger::test::Factory mFactory;
    sp<SurfaceFlinger> mFlinger = new SurfaceFlinger(mFactory, SurfaceFlinger::SkipInitialization);

    // We need to keep a reference to these so they are properly destroyed.
    std::vector<std::unique_ptr<HWC2Display>> mFakeHwcDisplays;
};

} // namespace android
