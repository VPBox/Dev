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

#pragma once

#define HWC2_USE_CPP11
#define HWC2_INCLUDE_STRINGIFICATION
#include <composer-hal/2.1/ComposerClient.h>
#undef HWC2_USE_CPP11
#undef HWC2_INCLUDE_STRINGIFICATION
#include "RenderState.h"

// Needed for display type/ID enums
#include <hardware/hwcomposer_defs.h>

#include <utils/Condition.h>

#include <chrono>

using namespace android::hardware::graphics::composer::V2_1;
using namespace android::hardware::graphics::composer::V2_1::hal;
using namespace android::hardware;
using namespace std::chrono_literals;

namespace {
class LayerImpl;
class Frame;
class DelayedEventGenerator;
} // namespace

namespace android {
class SurfaceComposerClient;
} // namespace android

namespace sftest {

// NOTE: The ID's need to be exactly these. VR composer and parts of
// the SurfaceFlinger assume the display IDs to have these values
// despite the enum being documented as a display type.
// TODO: Reference to actual documentation
constexpr Display PRIMARY_DISPLAY = static_cast<Display>(HWC_DISPLAY_PRIMARY);
constexpr Display EXTERNAL_DISPLAY = static_cast<Display>(HWC_DISPLAY_EXTERNAL);

class FakeComposerClient : public ComposerHal {
public:
    FakeComposerClient();
    virtual ~FakeComposerClient();

    bool hasCapability(hwc2_capability_t capability) override;

    std::string dumpDebugInfo() override;
    void registerEventCallback(EventCallback* callback) override;
    void unregisterEventCallback() override;

    uint32_t getMaxVirtualDisplayCount() override;
    Error createVirtualDisplay(uint32_t width, uint32_t height, PixelFormat* format,
                               Display* outDisplay) override;
    Error destroyVirtualDisplay(Display display) override;
    Error createLayer(Display display, Layer* outLayer) override;
    Error destroyLayer(Display display, Layer layer) override;

    Error getActiveConfig(Display display, Config* outConfig) override;
    Error getClientTargetSupport(Display display, uint32_t width, uint32_t height,
                                 PixelFormat format, Dataspace dataspace) override;
    Error getColorModes(Display display, hidl_vec<ColorMode>* outModes) override;
    Error getDisplayAttribute(Display display, Config config, IComposerClient::Attribute attribute,
                              int32_t* outValue) override;
    Error getDisplayConfigs(Display display, hidl_vec<Config>* outConfigs) override;
    Error getDisplayName(Display display, hidl_string* outName) override;
    Error getDisplayType(Display display, IComposerClient::DisplayType* outType) override;
    Error getDozeSupport(Display display, bool* outSupport) override;
    Error getHdrCapabilities(Display display, hidl_vec<Hdr>* outTypes, float* outMaxLuminance,
                             float* outMaxAverageLuminance, float* outMinLuminance) override;

    Error setActiveConfig(Display display, Config config) override;
    Error setColorMode(Display display, ColorMode mode) override;
    Error setPowerMode(Display display, IComposerClient::PowerMode mode) override;
    Error setVsyncEnabled(Display display, IComposerClient::Vsync enabled) override;

    Error setColorTransform(Display display, const float* matrix, int32_t hint) override;
    Error setClientTarget(Display display, buffer_handle_t target, int32_t acquireFence,
                          int32_t dataspace, const std::vector<hwc_rect_t>& damage) override;
    Error setOutputBuffer(Display display, buffer_handle_t buffer, int32_t releaseFence) override;
    Error validateDisplay(Display display, std::vector<Layer>* outChangedLayers,
                          std::vector<IComposerClient::Composition>* outCompositionTypes,
                          uint32_t* outDisplayRequestMask, std::vector<Layer>* outRequestedLayers,
                          std::vector<uint32_t>* outRequestMasks) override;
    Error acceptDisplayChanges(Display display) override;
    Error presentDisplay(Display display, int32_t* outPresentFence, std::vector<Layer>* outLayers,
                         std::vector<int32_t>* outReleaseFences) override;

    Error setLayerCursorPosition(Display display, Layer layer, int32_t x, int32_t y) override;
    Error setLayerBuffer(Display display, Layer layer, buffer_handle_t buffer,
                         int32_t acquireFence) override;
    Error setLayerSurfaceDamage(Display display, Layer layer,
                                const std::vector<hwc_rect_t>& damage) override;
    Error setLayerBlendMode(Display display, Layer layer, int32_t mode) override;
    Error setLayerColor(Display display, Layer layer, IComposerClient::Color color) override;
    Error setLayerCompositionType(Display display, Layer layer, int32_t type) override;
    Error setLayerDataspace(Display display, Layer layer, int32_t dataspace) override;
    Error setLayerDisplayFrame(Display display, Layer layer, const hwc_rect_t& frame) override;
    Error setLayerPlaneAlpha(Display display, Layer layer, float alpha) override;
    Error setLayerSidebandStream(Display display, Layer layer, buffer_handle_t stream) override;
    Error setLayerSourceCrop(Display display, Layer layer, const hwc_frect_t& crop) override;
    Error setLayerTransform(Display display, Layer layer, int32_t transform) override;
    Error setLayerVisibleRegion(Display display, Layer layer,
                                const std::vector<hwc_rect_t>& visible) override;
    Error setLayerZOrder(Display display, Layer layer, uint32_t z) override;

    void setClient(ComposerClient* client);

    void requestVSync(uint64_t vsyncTime = 0);
    // We don't want tests hanging, so always use a timeout. Remember
    // to always check the number of frames with test ASSERT_!
    // Wait until next frame is rendered after requesting vsync.
    void runVSyncAndWait(std::chrono::nanoseconds maxWait = 100ms);
    void runVSyncAfter(std::chrono::nanoseconds wait);

    int getFrameCount() const;
    // We don't want tests hanging, so always use a timeout. Remember
    // to always check the number of frames with test ASSERT_!
    void waitUntilFrame(int targetFrame, std::chrono::nanoseconds maxWait = 100ms) const;
    std::vector<RenderState> getFrameRects(int frame) const;
    std::vector<RenderState> getLatestFrame() const;
    void clearFrames();

    void onSurfaceFlingerStart();
    void onSurfaceFlingerStop();

    int getLayerCount() const;
    Layer getLayer(size_t index) const;

    void hotplugDisplay(Display display, IComposerCallback::Connection state);
    void refreshDisplay(Display display);

private:
    LayerImpl& getLayerImpl(Layer handle);

    EventCallback* mEventCallback;
    Config mCurrentConfig;
    bool mVsyncEnabled;
    std::vector<std::unique_ptr<LayerImpl>> mLayers;
    std::vector<std::unique_ptr<Frame>> mFrames;
    // Using a pointer to hide the implementation into the CPP file.
    std::unique_ptr<DelayedEventGenerator> mDelayedEventGenerator;
    android::sp<android::SurfaceComposerClient> mSurfaceComposer; // For VSync injections
    mutable android::Mutex mStateMutex;
    mutable android::Condition mFramesAvailable;
};

} // namespace sftest
