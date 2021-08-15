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
#include <ui/Fence.h>
#include <ui/FloatRect.h>
#include <ui/GraphicBuffer.h>
#include <ui/GraphicTypes.h>
#include <ui/Rect.h>
#include <ui/Region.h>
#include <ui/Transform.h>

#include "DisplayHardware/HWC2.h"

namespace HWC2 {
namespace mock {

class Layer : public HWC2::Layer {
public:
    Layer();
    ~Layer() override;

    MOCK_CONST_METHOD0(getId, hwc2_layer_t());

    MOCK_METHOD2(setCursorPosition, Error(int32_t, int32_t));
    MOCK_METHOD3(setBuffer,
                 Error(uint32_t, const android::sp<android::GraphicBuffer>&,
                       const android::sp<android::Fence>&));
    MOCK_METHOD1(setSurfaceDamage, Error(const android::Region&));
    MOCK_METHOD1(setBlendMode, Error(BlendMode));
    MOCK_METHOD1(setColor, Error(hwc_color_t));
    MOCK_METHOD1(setCompositionType, Error(Composition));
    MOCK_METHOD1(setDataspace, Error(android::ui::Dataspace));
    MOCK_METHOD2(setPerFrameMetadata, Error(const int32_t, const android::HdrMetadata&));
    MOCK_METHOD1(setDisplayFrame, Error(const android::Rect&));
    MOCK_METHOD1(setPlaneAlpha, Error(float));
    MOCK_METHOD1(setSidebandStream, Error(const native_handle_t*));
    MOCK_METHOD1(setSourceCrop, Error(const android::FloatRect&));
    MOCK_METHOD1(setTransform, Error(Transform));
    MOCK_METHOD1(setVisibleRegion, Error(const android::Region&));
    MOCK_METHOD1(setZOrder, Error(uint32_t));
    MOCK_METHOD2(setInfo, Error(uint32_t, uint32_t));

    MOCK_METHOD1(setColorTransform, Error(const android::mat4&));
};

} // namespace mock
} // namespace HWC2
