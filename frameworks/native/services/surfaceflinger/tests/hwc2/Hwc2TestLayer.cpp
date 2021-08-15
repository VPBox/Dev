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

#include "Hwc2TestLayer.h"

Hwc2TestCoverage getCoverage(Hwc2TestPropertyName property,
        Hwc2TestCoverage coverage, const std::unordered_map<Hwc2TestPropertyName,
        Hwc2TestCoverage>& coverageExceptions) {
    auto exception = coverageExceptions.find(property);
    return (exception != coverageExceptions.end())? exception->second : coverage;
}

Hwc2TestLayer::Hwc2TestLayer(Hwc2TestCoverage coverage,
        const Area& displayArea)
    : Hwc2TestLayer(coverage, displayArea,
            std::unordered_map<Hwc2TestPropertyName, Hwc2TestCoverage>()) { }

Hwc2TestLayer::Hwc2TestLayer(Hwc2TestCoverage coverage,
        const Area& displayArea, const std::unordered_map<Hwc2TestPropertyName,
        Hwc2TestCoverage>& coverageExceptions)
    : mBlendMode(getCoverage(Hwc2TestPropertyName::BlendMode, coverage,
           coverageExceptions)),
      mBufferArea(getCoverage(Hwc2TestPropertyName::BufferArea, coverage,
           coverageExceptions), displayArea),
      mColor(getCoverage(Hwc2TestPropertyName::Color, coverage,
           coverageExceptions)),
      mComposition(getCoverage(Hwc2TestPropertyName::Composition, coverage,
           coverageExceptions)),
      mDataspace(getCoverage(Hwc2TestPropertyName::Dataspace, coverage,
           coverageExceptions)),
      mDisplayFrame(getCoverage(Hwc2TestPropertyName::DisplayFrame, coverage,
           coverageExceptions), displayArea),
      mPlaneAlpha(getCoverage(Hwc2TestPropertyName::PlaneAlpha, coverage,
           coverageExceptions)),
      mSourceCrop(getCoverage(Hwc2TestPropertyName::SourceCrop, coverage,
           coverageExceptions)),
      mSurfaceDamage(getCoverage(Hwc2TestPropertyName::SurfaceDamage, coverage,
           coverageExceptions)),
      mTransform(getCoverage(Hwc2TestPropertyName::Transform, coverage,
           coverageExceptions))
{
    mBufferArea.setDependent(&mBuffer);
    mBufferArea.setDependent(&mSourceCrop);
    mBufferArea.setDependent(&mSurfaceDamage);
    mBlendMode.setDependent(&mColor);
}

std::string Hwc2TestLayer::dump() const
{
    std::stringstream dmp;

    dmp << "layer: \n";

    for (auto property : mProperties) {
        dmp << property->dump();
    }

    dmp << mVisibleRegion.dump();
    dmp << "\tz order: " << mZOrder << "\n";

    return dmp.str();
}

int Hwc2TestLayer::getBuffer(buffer_handle_t* outHandle,
        android::base::unique_fd* outAcquireFence)
{
    int32_t acquireFence;
    int ret = mBuffer.get(outHandle, &acquireFence);
    outAcquireFence->reset(acquireFence);
    return ret;
}

int Hwc2TestLayer::getBuffer(buffer_handle_t* outHandle,
        int32_t* outAcquireFence)
{
    return mBuffer.get(outHandle, outAcquireFence);
}

void Hwc2TestLayer::setZOrder(uint32_t zOrder)
{
    mZOrder = zOrder;
}

void Hwc2TestLayer::setVisibleRegion(const android::Region& region)
{
    return mVisibleRegion.set(region);
}

void Hwc2TestLayer::reset()
{
    mVisibleRegion.release();

    for (auto property : mProperties) {
        property->reset();
    }
}

bool Hwc2TestLayer::advance()
{
    for (auto property : mProperties) {
        if (property->isSupported(mComposition.get()))
            if (property->advance())
                return true;
    }
    return false;
}

hwc2_blend_mode_t Hwc2TestLayer::getBlendMode() const
{
    return mBlendMode.get();
}

Area Hwc2TestLayer::getBufferArea() const
{
    return mBufferArea.get();
}

hwc_color_t Hwc2TestLayer::getColor() const
{
    return mColor.get();
}

hwc2_composition_t Hwc2TestLayer::getComposition() const
{
    return mComposition.get();
}

/* The cursor position corresponds to {displayFrame.left, displayFrame.top} */
hwc_rect_t Hwc2TestLayer::getCursorPosition() const
{
    return mDisplayFrame.get();
}

android::ui::Dataspace Hwc2TestLayer::getDataspace() const
{
    return mDataspace.get();
}

hwc_rect_t Hwc2TestLayer::getDisplayFrame() const
{
    return mDisplayFrame.get();
}

float Hwc2TestLayer::getPlaneAlpha() const
{
    return mPlaneAlpha.get();
}

hwc_frect_t Hwc2TestLayer::getSourceCrop() const
{
    return mSourceCrop.get();
}

hwc_region_t Hwc2TestLayer::getSurfaceDamage() const
{
    return mSurfaceDamage.get();
}

hwc_transform_t Hwc2TestLayer::getTransform() const
{
    return mTransform.get();
}

hwc_region_t Hwc2TestLayer::getVisibleRegion() const
{
    return mVisibleRegion.get();
}

uint32_t Hwc2TestLayer::getZOrder() const
{
    return mZOrder;
}

bool Hwc2TestLayer::advanceBlendMode()
{
    return mBlendMode.advance();
}

bool Hwc2TestLayer::advanceBufferArea()
{
    return mBufferArea.advance();
}

bool Hwc2TestLayer::advanceColor()
{
    return mColor.advance();
}

bool Hwc2TestLayer::advanceComposition()
{
    return mComposition.advance();
}

bool Hwc2TestLayer::advanceCursorPosition()
{
    return mDisplayFrame.advance();
}

bool Hwc2TestLayer::advanceDataspace()
{
    return mDataspace.advance();
}

bool Hwc2TestLayer::advanceDisplayFrame()
{
    return mDisplayFrame.advance();
}

bool Hwc2TestLayer::advancePlaneAlpha()
{
    return mPlaneAlpha.advance();
}

bool Hwc2TestLayer::advanceSourceCrop()
{
    return mSourceCrop.advance();
}

bool Hwc2TestLayer::advanceSurfaceDamage()
{
    return mSurfaceDamage.advance();
}

bool Hwc2TestLayer::advanceTransform()
{
    return mTransform.advance();
}

bool Hwc2TestLayer::advanceVisibleRegion()
{
    if (mPlaneAlpha.advance())
        return true;
    return mDisplayFrame.advance();
}
