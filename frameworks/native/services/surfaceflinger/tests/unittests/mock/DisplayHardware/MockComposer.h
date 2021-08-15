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

#include <gmock/gmock.h>

#include "DisplayHardware/ComposerHal.h"

namespace android {

class GraphicBuffer;

namespace Hwc2 {
namespace mock {

using android::hardware::graphics::common::V1_0::ColorTransform;
using android::hardware::graphics::common::V1_0::Transform;
using android::hardware::graphics::common::V1_1::RenderIntent;
using android::hardware::graphics::common::V1_2::ColorMode;
using android::hardware::graphics::common::V1_2::Dataspace;
using android::hardware::graphics::common::V1_2::Hdr;
using android::hardware::graphics::common::V1_2::PixelFormat;

using android::hardware::graphics::composer::V2_1::Config;
using android::hardware::graphics::composer::V2_1::Display;
using android::hardware::graphics::composer::V2_1::Error;
using android::hardware::graphics::composer::V2_1::IComposer;
using android::hardware::graphics::composer::V2_1::IComposerCallback;
using android::hardware::graphics::composer::V2_1::Layer;
using android::hardware::graphics::composer::V2_3::IComposerClient;

class Composer : public Hwc2::Composer {
public:
    Composer();
    ~Composer() override;

    MOCK_METHOD0(getCapabilities, std::vector<IComposer::Capability>());
    MOCK_METHOD0(dumpDebugInfo, std::string());
    MOCK_METHOD1(registerCallback, void(const sp<IComposerCallback>&));
    MOCK_METHOD0(isRemote, bool());
    MOCK_METHOD0(resetCommands, void());
    MOCK_METHOD0(executeCommands, Error());
    MOCK_METHOD0(getMaxVirtualDisplayCount, uint32_t());
    MOCK_CONST_METHOD0(isUsingVrComposer, bool());
    MOCK_METHOD4(createVirtualDisplay, Error(uint32_t, uint32_t, PixelFormat*, Display*));
    MOCK_METHOD1(destroyVirtualDisplay, Error(Display));
    MOCK_METHOD1(acceptDisplayChanges, Error(Display));
    MOCK_METHOD2(createLayer, Error(Display, Layer* outLayer));
    MOCK_METHOD2(destroyLayer, Error(Display, Layer));
    MOCK_METHOD2(getActiveConfig, Error(Display, Config*));
    MOCK_METHOD3(getChangedCompositionTypes,
                 Error(Display, std::vector<Layer>*, std::vector<IComposerClient::Composition>*));
    MOCK_METHOD2(getColorModes, Error(Display, std::vector<ColorMode>*));
    MOCK_METHOD4(getDisplayAttribute,
                 Error(Display, Config config, IComposerClient::Attribute, int32_t*));
    MOCK_METHOD2(getDisplayConfigs, Error(Display, std::vector<Config>*));
    MOCK_METHOD2(getDisplayName, Error(Display, std::string*));
    MOCK_METHOD4(getDisplayRequests,
                 Error(Display, uint32_t*, std::vector<Layer>*, std::vector<uint32_t>*));
    MOCK_METHOD2(getDisplayType, Error(Display, IComposerClient::DisplayType*));
    MOCK_METHOD2(getDozeSupport, Error(Display, bool*));
    MOCK_METHOD5(getHdrCapabilities, Error(Display, std::vector<Hdr>*, float*, float*, float*));
    MOCK_METHOD1(getPerFrameMetadataKeys,
                 std::vector<IComposerClient::PerFrameMetadataKey>(Display));
    MOCK_METHOD2(getDataspaceSaturationMatrix, Error(Dataspace, mat4*));
    MOCK_METHOD3(getDisplayIdentificationData, Error(Display, uint8_t*, std::vector<uint8_t>*));
    MOCK_METHOD3(getReleaseFences, Error(Display, std::vector<Layer>*, std::vector<int>*));
    MOCK_METHOD2(presentDisplay, Error(Display, int*));
    MOCK_METHOD2(setActiveConfig, Error(Display, Config));
    MOCK_METHOD6(setClientTarget,
                 Error(Display, uint32_t, const sp<GraphicBuffer>&, int, Dataspace,
                       const std::vector<IComposerClient::Rect>&));
    MOCK_METHOD3(setColorMode, Error(Display, ColorMode, RenderIntent));
    MOCK_METHOD3(setColorTransform, Error(Display, const float*, ColorTransform));
    MOCK_METHOD3(setOutputBuffer, Error(Display, const native_handle_t*, int));
    MOCK_METHOD2(setPowerMode, Error(Display, IComposerClient::PowerMode));
    MOCK_METHOD2(setVsyncEnabled, Error(Display, IComposerClient::Vsync));
    MOCK_METHOD1(setClientTargetSlotCount, Error(Display));
    MOCK_METHOD3(validateDisplay, Error(Display, uint32_t*, uint32_t*));
    MOCK_METHOD5(presentOrValidateDisplay, Error(Display, uint32_t*, uint32_t*, int*, uint32_t*));
    MOCK_METHOD4(setCursorPosition, Error(Display, Layer, int32_t, int32_t));
    MOCK_METHOD5(setLayerBuffer, Error(Display, Layer, uint32_t, const sp<GraphicBuffer>&, int));
    MOCK_METHOD3(setLayerSurfaceDamage,
                 Error(Display, Layer, const std::vector<IComposerClient::Rect>&));
    MOCK_METHOD3(setLayerBlendMode, Error(Display, Layer, IComposerClient::BlendMode));
    MOCK_METHOD3(setLayerColor, Error(Display, Layer, const IComposerClient::Color&));
    MOCK_METHOD3(setLayerCompositionType, Error(Display, Layer, IComposerClient::Composition));
    MOCK_METHOD3(setLayerDataspace, Error(Display, Layer, Dataspace));
    MOCK_METHOD3(setLayerPerFrameMetadata,
                 Error(Display, Layer, const std::vector<IComposerClient::PerFrameMetadata>&));
    MOCK_METHOD3(setLayerDisplayFrame, Error(Display, Layer, const IComposerClient::Rect&));
    MOCK_METHOD3(setLayerPlaneAlpha, Error(Display, Layer, float));
    MOCK_METHOD3(setLayerSidebandStream, Error(Display, Layer, const native_handle_t*));
    MOCK_METHOD3(setLayerSourceCrop, Error(Display, Layer, const IComposerClient::FRect&));
    MOCK_METHOD3(setLayerTransform, Error(Display, Layer, Transform));
    MOCK_METHOD3(setLayerVisibleRegion,
                 Error(Display, Layer, const std::vector<IComposerClient::Rect>&));
    MOCK_METHOD3(setLayerZOrder, Error(Display, Layer, uint32_t));
    MOCK_METHOD4(setLayerInfo, Error(Display, Layer, uint32_t, uint32_t));
    MOCK_METHOD3(getRenderIntents, Error(Display, ColorMode, std::vector<RenderIntent>*));
    MOCK_METHOD3(setLayerColorTransform, Error(Display, Layer, const float*));
    MOCK_METHOD4(getDisplayedContentSamplingAttributes,
                 Error(Display, PixelFormat*, Dataspace*, uint8_t*));
    MOCK_METHOD4(setDisplayContentSamplingEnabled, Error(Display, bool, uint8_t, uint64_t));
    MOCK_METHOD4(getDisplayedContentSample,
                 Error(Display, uint64_t, uint64_t, DisplayedFrameStats*));
    MOCK_METHOD2(getDisplayCapabilities, Error(Display, std::vector<DisplayCapability>*));
    MOCK_METHOD3(setLayerPerFrameMetadataBlobs,
                 Error(Display, Layer, const std::vector<IComposerClient::PerFrameMetadataBlob>&));
    MOCK_METHOD2(setDisplayBrightness, Error(Display, float));
};

} // namespace mock
} // namespace Hwc2
} // namespace android
