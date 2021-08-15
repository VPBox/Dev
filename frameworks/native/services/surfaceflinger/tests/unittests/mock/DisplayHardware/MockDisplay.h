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

#pragma once

#include <gmock/gmock.h>

#include "DisplayHardware/HWC2.h"

using HWC2::Error;
using HWC2::Layer;

namespace android {
namespace Hwc2 {
namespace mock {

class Display : public HWC2::Display {
public:
    Display();
    ~Display();

    MOCK_CONST_METHOD0(getId, hwc2_layer_t());
    MOCK_CONST_METHOD0(isConnected, bool());
    MOCK_METHOD1(setConnected, void(bool));
    MOCK_CONST_METHOD0(getCapabilities, const std::unordered_set<HWC2::DisplayCapability>&());

    MOCK_METHOD0(acceptChanges, Error());
    MOCK_METHOD1(createLayer, Error(Layer**));
    MOCK_METHOD1(destroyLayer, Error(Layer*));
    MOCK_CONST_METHOD1(getActiveConfig, Error(std::shared_ptr<const Config>*));
    MOCK_CONST_METHOD1(getActiveConfigIndex, Error(int* outIndex));
    MOCK_METHOD1(getChangedCompositionTypes, Error(std::unordered_map<Layer*, HWC2::Composition>*));
    MOCK_CONST_METHOD1(getColorModes, Error(std::vector<android::ui::ColorMode>*));

    MOCK_CONST_METHOD0(getSupportedPerFrameMetadata, int32_t());
    MOCK_CONST_METHOD2(getRenderIntents,
                       Error(android::ui::ColorMode, std::vector<android::ui::RenderIntent>*));
    MOCK_METHOD2(getDataspaceSaturationMatrix, Error(android::ui::Dataspace, android::mat4*));
    MOCK_CONST_METHOD0(getConfigs, std::vector<std::shared_ptr<const Config>>());

    MOCK_CONST_METHOD1(getName, Error(std::string*));
    MOCK_METHOD2(getRequests,
                 Error(HWC2::DisplayRequest*, std::unordered_map<Layer*, HWC2::LayerRequest>*));
    MOCK_CONST_METHOD1(getType, Error(HWC2::DisplayType*));
    MOCK_CONST_METHOD1(supportsDoze, Error(bool*));
    MOCK_CONST_METHOD1(getHdrCapabilities, Error(android::HdrCapabilities*));
    MOCK_CONST_METHOD3(getDisplayedContentSamplingAttributes,
                       Error(android::ui::PixelFormat*, android::ui::Dataspace*, uint8_t*));
    MOCK_CONST_METHOD3(setDisplayContentSamplingEnabled, Error(bool, uint8_t, uint64_t));
    MOCK_CONST_METHOD3(getDisplayedContentSample,
                       Error(uint64_t, uint64_t, android::DisplayedFrameStats*));
    MOCK_CONST_METHOD1(getReleaseFences,
                       Error(std::unordered_map<Layer*, android::sp<android::Fence>>* outFences));
    MOCK_METHOD1(present, Error(android::sp<android::Fence>*));
    MOCK_METHOD1(setActiveConfig, Error(const std::shared_ptr<const HWC2::Display::Config>&));
    MOCK_METHOD4(setClientTarget,
                 Error(uint32_t, const android::sp<android::GraphicBuffer>&,
                       const android::sp<android::Fence>&, android::ui::Dataspace));
    MOCK_METHOD2(setColorMode, Error(android::ui::ColorMode, android::ui::RenderIntent));
    MOCK_METHOD2(setColorTransform, Error(const android::mat4&, android_color_transform_t));
    MOCK_METHOD2(setOutputBuffer,
                 Error(const android::sp<android::GraphicBuffer>&,
                       const android::sp<android::Fence>&));
    MOCK_METHOD1(setPowerMode, Error(HWC2::PowerMode));
    MOCK_METHOD1(setVsyncEnabled, Error(HWC2::Vsync));
    MOCK_METHOD2(validate, Error(uint32_t*, uint32_t*));
    MOCK_METHOD4(presentOrValidate,
                 Error(uint32_t*, uint32_t*, android::sp<android::Fence>*, uint32_t*));
    MOCK_CONST_METHOD1(setDisplayBrightness, Error(float));
};

} // namespace mock
} // namespace Hwc2
} // namespace android
