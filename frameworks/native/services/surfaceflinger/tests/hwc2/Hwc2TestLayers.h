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

#ifndef _HWC2_TEST_LAYERS_H
#define _HWC2_TEST_LAYERS_H

#include <map>

#define HWC2_INCLUDE_STRINGIFICATION
#define HWC2_USE_CPP11
#include <hardware/hwcomposer2.h>
#undef HWC2_INCLUDE_STRINGIFICATION
#undef HWC2_USE_CPP11

#include "Hwc2TestProperties.h"
#include "Hwc2TestLayer.h"

class Hwc2TestLayers {
public:
    Hwc2TestLayers(const std::vector<hwc2_layer_t>& layers,
            Hwc2TestCoverage coverage, const Area& displayArea);

    Hwc2TestLayers(const std::vector<hwc2_layer_t>& layers,
            Hwc2TestCoverage coverage, const Area& displayArea,
            const std::unordered_map<Hwc2TestPropertyName,
            Hwc2TestCoverage>& coverageExceptions);

    std::string dump() const;

    void reset();

    bool advance();
    bool advanceVisibleRegions();

    /* Test cases with multiple layers and property values can take quite some
     * time to run. A significant amount of time can be spent on test cases
     * where one layer is changing property values but is not visible. To
     * decrease runtime, this function can be called. Removes layouts where a
     * layer is completely blocked from view. It also removes layouts that do
     * not cover the entire display.*/
    bool optimizeLayouts();

    bool contains(hwc2_layer_t layer) const;

    int  getBuffer(hwc2_layer_t layer, buffer_handle_t* outHandle,
            int32_t* outAcquireFence);

    hwc2_blend_mode_t      getBlendMode(hwc2_layer_t layer) const;
    Area                   getBufferArea(hwc2_layer_t layer) const;
    hwc_color_t            getColor(hwc2_layer_t layer) const;
    hwc2_composition_t     getComposition(hwc2_layer_t layer) const;
    hwc_rect_t             getCursorPosition(hwc2_layer_t layer) const;
    android::ui::Dataspace     getDataspace(hwc2_layer_t layer) const;
    hwc_rect_t             getDisplayFrame(hwc2_layer_t layer) const;
    android_pixel_format_t getFormat(hwc2_layer_t layer) const;
    float                  getPlaneAlpha(hwc2_layer_t layer) const;
    hwc_frect_t            getSourceCrop(hwc2_layer_t layer) const;
    hwc_region_t           getSurfaceDamage(hwc2_layer_t layer) const;
    hwc_transform_t        getTransform(hwc2_layer_t layer) const;
    hwc_region_t           getVisibleRegion(hwc2_layer_t layer) const;
    uint32_t               getZOrder(hwc2_layer_t layer) const;

private:
    bool setVisibleRegions();

    std::map<hwc2_layer_t, Hwc2TestLayer> mTestLayers;

    Area mDisplayArea;

    bool mOptimize = false;
};

#endif /* ifndef _HWC2_TEST_LAYERS_H */
