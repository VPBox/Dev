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

#include <sstream>

#include <ui/Rect.h>

#include "Hwc2TestClientTarget.h"

int Hwc2TestClientTarget::getBuffer(const Hwc2TestLayers& testLayers,
        const std::set<hwc2_layer_t>& clientLayers,
        const std::set<hwc2_layer_t>& clearLayers, bool flipClientTarget,
        const Area& displayArea, buffer_handle_t* outHandle,
        int32_t* outAcquireFence)
{
    if (!flipClientTarget) {
        bool needsClientTarget = false;

        for (auto clientLayer : clientLayers) {
            if (testLayers.getVisibleRegion(clientLayer).numRects > 0) {
                needsClientTarget = true;
                break;
            }
        }

        if (!needsClientTarget) {
           *outHandle = nullptr;
           *outAcquireFence = -1;
           return 0;
        }
    }

    return mBuffer.get(outHandle, outAcquireFence, displayArea,
            &testLayers, &clientLayers, &clearLayers);
}


Hwc2TestClientTargetSupport::Hwc2TestClientTargetSupport(
        Hwc2TestCoverage coverage, const Area& displayArea)
    : mBufferArea(coverage, displayArea),
      mDataspace(coverage),
      mSurfaceDamage(coverage)
{
    mBufferArea.setDependent(&mSurfaceDamage);
}

std::string Hwc2TestClientTargetSupport::dump() const
{
    std::stringstream dmp;

    dmp << "client target: \n";

    for (auto property : properties) {
        dmp << property->dump();
    }

    return dmp.str();
}

void Hwc2TestClientTargetSupport::reset()
{
    for (auto property : properties) {
        property->reset();
    }
}

bool Hwc2TestClientTargetSupport::advance()
{
    for (auto property : properties) {
        if (property->advance())
            return true;
    }
    return false;
}

Area Hwc2TestClientTargetSupport::getBufferArea() const
{
    return mBufferArea.get();
}

android::ui::Dataspace Hwc2TestClientTargetSupport::getDataspace() const
{
    return mDataspace.get();
}

const hwc_region_t Hwc2TestClientTargetSupport::getSurfaceDamage() const
{
    return mSurfaceDamage.get();
}
