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

#ifndef _HWC2_TEST_CLIENT_TARGET_H
#define _HWC2_TEST_CLIENT_TARGET_H

#include <set>

#define HWC2_INCLUDE_STRINGIFICATION
#define HWC2_USE_CPP11
#include <hardware/hwcomposer2.h>
#undef HWC2_INCLUDE_STRINGIFICATION
#undef HWC2_USE_CPP11

#include "Hwc2TestProperties.h"
#include "Hwc2TestLayers.h"

/* Generates client target buffers from client composition layers */
class Hwc2TestClientTarget {
public:
    int getBuffer(const Hwc2TestLayers& layers,
            const std::set<hwc2_layer_t>& clientLayers,
            const std::set<hwc2_layer_t>& clearLayers,
            bool clearClientTarget, const Area& displayArea,
            buffer_handle_t* outHandle, int32_t* outAcquireFence);

private:
    Hwc2TestClientTargetBuffer mBuffer;
};

/* Generates valid client targets to test which ones the device will support */
class Hwc2TestClientTargetSupport {
public:
    Hwc2TestClientTargetSupport(Hwc2TestCoverage coverage,
            const Area& displayArea);

    std::string dump() const;

    void reset();
    bool advance();

    Area getBufferArea() const;
    android::ui::Dataspace getDataspace() const;
    const hwc_region_t getSurfaceDamage() const;

private:
    std::array<Hwc2TestContainer*, 3> properties = {{
        &mDataspace, &mSurfaceDamage, &mBufferArea
    }};

    Hwc2TestBufferArea mBufferArea;
    Hwc2TestDataspace mDataspace;
    Hwc2TestSurfaceDamage mSurfaceDamage;
};

#endif /* ifndef _HWC2_TEST_CLIENT_TARGET_H */
