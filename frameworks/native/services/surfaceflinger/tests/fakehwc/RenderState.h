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

#include <hardware/hwcomposer2.h>

#include <vector>

namespace sftest {
// Description of a rendered rectangle.  Should only contain
// instructions necessary to rasterize the rectangle. The full scene
// is given as a sorted list of rectangles, bottom layer at index 0.
class RenderState {
public:
    RenderState() = default;
    // Default copy-ctor

    hwc_rect_t mDisplayFrame = {0, 0, 0, 0};
    hwc_frect_t mSourceCrop = {0.f, 0.f, 0.f, 0.f};
    std::vector<hwc_rect_t> mVisibleRegion;
    hwc2_blend_mode_t mBlendMode = HWC2_BLEND_MODE_NONE;
    buffer_handle_t mBuffer = 0;
    uint32_t mSwapCount = 0;   // How many set buffer calls to the layer.
    int32_t mAcquireFence = 0; // Probably should not be here.
    float mPlaneAlpha = 0.f;
    hwc_color_t mLayerColor = {0, 0, 0, 0};
    hwc_transform_t mTransform = static_cast<hwc_transform_t>(0);
};

} // namespace sftest
