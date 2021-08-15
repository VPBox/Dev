/*
 * Copyright (C) 2016 The Android Open Source Project
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

#ifndef _HWC2_TEST_LAYER_H
#define _HWC2_TEST_LAYER_H

#include <android-base/unique_fd.h>
#include <unordered_map>

#include "Hwc2TestBuffer.h"
#include "Hwc2TestProperties.h"

#define HWC2_INCLUDE_STRINGIFICATION
#define HWC2_USE_CPP11
#include <hardware/hwcomposer2.h>
#undef HWC2_INCLUDE_STRINGIFICATION
#undef HWC2_USE_CPP11

class Hwc2TestLayer {
public:
    Hwc2TestLayer(Hwc2TestCoverage coverage, const Area& displayArea);

    Hwc2TestLayer(Hwc2TestCoverage coverage, const Area& displayArea,
            const std::unordered_map<Hwc2TestPropertyName,
            Hwc2TestCoverage>& coverage_exceptions);

    std::string dump() const;

    int getBuffer(buffer_handle_t* outHandle,
            android::base::unique_fd* outAcquireFence);
    int getBuffer(buffer_handle_t* outHandle, int32_t* outAcquireFence);

    void setZOrder(uint32_t zOrder);
    void setVisibleRegion(const android::Region& region);

    void reset();
    bool advance();

    hwc2_blend_mode_t      getBlendMode() const;
    Area                   getBufferArea() const;
    hwc_color_t            getColor() const;
    hwc2_composition_t     getComposition() const;
    hwc_rect_t             getCursorPosition() const;
    android::ui::Dataspace     getDataspace() const;
    hwc_rect_t             getDisplayFrame() const;
    float                  getPlaneAlpha() const;
    hwc_frect_t            getSourceCrop() const;
    hwc_region_t           getSurfaceDamage() const;
    hwc_transform_t        getTransform() const;
    hwc_region_t           getVisibleRegion() const;
    uint32_t               getZOrder() const;

    bool advanceBlendMode();
    bool advanceBufferArea();
    bool advanceColor();
    bool advanceComposition();
    bool advanceCursorPosition();
    bool advanceDataspace();
    bool advanceDisplayFrame();
    bool advancePlaneAlpha();
    bool advanceSourceCrop();
    bool advanceSurfaceDamage();
    bool advanceTransform();
    bool advanceVisibleRegion();

private:
    std::array<Hwc2TestContainer*, 10> mProperties = {{
        &mTransform, &mColor, &mDataspace, &mPlaneAlpha, &mSourceCrop,
        &mSurfaceDamage, &mBlendMode, &mBufferArea, &mDisplayFrame,
        &mComposition
    }};

    Hwc2TestBuffer mBuffer;

    Hwc2TestBlendMode mBlendMode;
    Hwc2TestBufferArea mBufferArea;
    Hwc2TestColor mColor;
    Hwc2TestComposition mComposition;
    Hwc2TestDataspace mDataspace;
    Hwc2TestDisplayFrame mDisplayFrame;
    Hwc2TestPlaneAlpha mPlaneAlpha;
    Hwc2TestSourceCrop mSourceCrop;
    Hwc2TestSurfaceDamage mSurfaceDamage;
    Hwc2TestTransform mTransform;
    Hwc2TestVisibleRegion mVisibleRegion;

    uint32_t mZOrder = UINT32_MAX;
};

#endif /* ifndef _HWC2_TEST_LAYER_H */
