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
#include <cutils/log.h>
#include <ui/Rect.h>

#define HWC2_INCLUDE_STRINGIFICATION
#define HWC2_USE_CPP11
#include <hardware/hwcomposer2.h>
#undef HWC2_INCLUDE_STRINGIFICATION
#undef HWC2_USE_CPP11

#include "Hwc2TestBuffer.h"
#include "Hwc2TestProperties.h"

Hwc2TestBufferArea::Hwc2TestBufferArea(Hwc2TestCoverage coverage,
        const Area& displayArea)
    : Hwc2TestProperty(mBufferAreas, mCompositionSupport),
      mScalars((coverage == Hwc2TestCoverage::Complete)? mCompleteScalars:
            (coverage == Hwc2TestCoverage::Basic)? mBasicScalars:
            mDefaultScalars),
      mDisplayArea(displayArea)
{
    update();
}

std::string Hwc2TestBufferArea::dump() const
{
    std::stringstream dmp;
    const Area& curr = get();
    dmp << "\tbuffer area: width " << curr.width << ", height " << curr.height
            << "\n";
    return dmp.str();
}

void Hwc2TestBufferArea::setDependent(Hwc2TestBuffer* buffer)
{
    mBuffer = buffer;
    if (buffer) {
        buffer->updateBufferArea(get());
    }
}

void Hwc2TestBufferArea::setDependent(Hwc2TestSourceCrop* sourceCrop)
{
    mSourceCrop = sourceCrop;
    if (mSourceCrop) {
        mSourceCrop->updateBufferArea(get());
    }
}

void Hwc2TestBufferArea::setDependent(Hwc2TestSurfaceDamage* surfaceDamage)
{
    mSurfaceDamage = surfaceDamage;
    if (mSurfaceDamage) {
        mSurfaceDamage->updateBufferArea(get());
    }
}

void Hwc2TestBufferArea::update()
{
    mBufferAreas.clear();

    if (mDisplayArea.width == 0 && mDisplayArea.height == 0) {
        mBufferAreas.push_back({0, 0});
        return;
    }

    for (auto scalar : mScalars) {
        mBufferAreas.push_back({static_cast<int32_t>(scalar * mDisplayArea.width),
                static_cast<int32_t>(scalar * mDisplayArea.height)});
    }

    updateDependents();
}

void Hwc2TestBufferArea::updateDependents()
{
    const Area& curr = get();

    if (mBuffer)
        mBuffer->updateBufferArea(curr);
    if (mSourceCrop)
        mSourceCrop->updateBufferArea(curr);
    if (mSurfaceDamage)
        mSurfaceDamage->updateBufferArea(curr);
}

const std::vector<float> Hwc2TestBufferArea::mDefaultScalars = {
    1.0f,
};

const std::vector<float> Hwc2TestBufferArea::mBasicScalars = {
    1.0f, 0.5f,
};

const std::vector<float> Hwc2TestBufferArea::mCompleteScalars = {
    1.0f, 0.75f, 0.5f
};


Hwc2TestBlendMode::Hwc2TestBlendMode(Hwc2TestCoverage coverage)
    : Hwc2TestProperty(coverage, mCompleteBlendModes, mBasicBlendModes,
            mDefaultBlendModes, mCompositionSupport) { }

std::string Hwc2TestBlendMode::dump() const
{
    std::stringstream dmp;
    dmp << "\tblend mode: " << getBlendModeName(get()) << "\n";
    return dmp.str();
}

void Hwc2TestBlendMode::setDependent(Hwc2TestColor* color)
{
    mColor = color;
    updateDependents();
}

void Hwc2TestBlendMode::updateDependents()
{
    if (mColor)
        mColor->updateBlendMode(get());
}

const std::vector<hwc2_blend_mode_t> Hwc2TestBlendMode::mDefaultBlendModes = {
    HWC2_BLEND_MODE_NONE,
};

const std::vector<hwc2_blend_mode_t> Hwc2TestBlendMode::mBasicBlendModes = {
    HWC2_BLEND_MODE_NONE,
    HWC2_BLEND_MODE_PREMULTIPLIED,
};

const std::vector<hwc2_blend_mode_t> Hwc2TestBlendMode::mCompleteBlendModes = {
    HWC2_BLEND_MODE_NONE,
    HWC2_BLEND_MODE_PREMULTIPLIED,
    HWC2_BLEND_MODE_COVERAGE,
};


Hwc2TestColor::Hwc2TestColor(Hwc2TestCoverage coverage,
        hwc2_blend_mode_t blendMode)
    : Hwc2TestProperty(mColors, mCompositionSupport),
      mBaseColors((coverage == Hwc2TestCoverage::Complete)? mCompleteBaseColors:
            (coverage == Hwc2TestCoverage::Basic)? mBasicBaseColors:
            mDefaultBaseColors),
      mBlendMode(blendMode)
{
    update();
}

std::string Hwc2TestColor::dump() const
{
    std::stringstream dmp;
    const hwc_color_t& color = get();
    dmp << "\tcolor: r " << std::to_string(color.r) << ", g "
            << std::to_string(color.g) << ", b " << std::to_string(color.b)
            << ", a " << std::to_string(color.a) << "\n";
    return dmp.str();
}

void Hwc2TestColor::updateBlendMode(hwc2_blend_mode_t blendMode)
{
    mBlendMode = blendMode;
    update();
}

void Hwc2TestColor::update()
{
    if (mBlendMode != HWC2_BLEND_MODE_PREMULTIPLIED) {
        mColors = mBaseColors;
        return;
    }

    mColors.clear();

    for (const hwc_color_t& baseColor : mBaseColors) {
        if (baseColor.a >= baseColor.r && baseColor.a >= baseColor.g
                && baseColor.a >= baseColor.b) {
            mColors.push_back(baseColor);
        }
    }

}

const std::vector<hwc_color_t> Hwc2TestColor::mDefaultBaseColors = {
    {UINT8_MAX, UINT8_MAX, UINT8_MAX, UINT8_MAX},
};

const std::vector<hwc_color_t> Hwc2TestColor::mBasicBaseColors = {
    {UINT8_MAX, UINT8_MAX, UINT8_MAX, UINT8_MAX},
    {        0,         0,         0,         0},
};

const std::vector<hwc_color_t> Hwc2TestColor::mCompleteBaseColors = {
    {UINT8_MAX, UINT8_MAX, UINT8_MAX, UINT8_MAX},
    {UINT8_MAX, UINT8_MAX, UINT8_MAX,         0},
    {UINT8_MAX, UINT8_MAX,         0, UINT8_MAX},
    {UINT8_MAX, UINT8_MAX,         0,         0},
    {UINT8_MAX,         0, UINT8_MAX, UINT8_MAX},
    {UINT8_MAX,         0, UINT8_MAX,         0},
    {UINT8_MAX,         0,         0, UINT8_MAX},
    {UINT8_MAX,         0,         0,         0},
    {        0, UINT8_MAX, UINT8_MAX, UINT8_MAX},
    {        0, UINT8_MAX, UINT8_MAX,         0},
    {        0, UINT8_MAX,         0, UINT8_MAX},
    {        0, UINT8_MAX,         0,         0},
    {        0,         0, UINT8_MAX, UINT8_MAX},
    {        0,         0, UINT8_MAX,         0},
    {        0,         0,         0, UINT8_MAX},
    {        0,         0,         0,         0},
};


Hwc2TestComposition::Hwc2TestComposition(Hwc2TestCoverage coverage)
    : Hwc2TestProperty(coverage, mCompleteCompositions, mBasicCompositions,
            mDefaultCompositions, mCompositionSupport) { }

std::string Hwc2TestComposition::dump() const
{
    std::stringstream dmp;
    dmp << "\tcomposition: " << getCompositionName(get()) << "\n";
    return dmp.str();
}

const std::vector<hwc2_composition_t> Hwc2TestComposition::mDefaultCompositions = {
    HWC2_COMPOSITION_DEVICE,
};

const std::vector<hwc2_composition_t> Hwc2TestComposition::mBasicCompositions = {
    HWC2_COMPOSITION_CLIENT,
    HWC2_COMPOSITION_DEVICE,
};

const std::vector<hwc2_composition_t> Hwc2TestComposition::mCompleteCompositions = {
    HWC2_COMPOSITION_CLIENT,
    HWC2_COMPOSITION_DEVICE,
    HWC2_COMPOSITION_SOLID_COLOR,
    HWC2_COMPOSITION_CURSOR,
    HWC2_COMPOSITION_SIDEBAND,
};


Hwc2TestDataspace::Hwc2TestDataspace(Hwc2TestCoverage coverage)
    : Hwc2TestProperty(coverage, completeDataspaces, basicDataspaces,
            defaultDataspaces, mCompositionSupport) { }

std::string Hwc2TestDataspace::dump() const
{
    std::stringstream dmp;
    dmp << "\tdataspace: " << static_cast<int32_t>(get()) << "\n";
    return dmp.str();
}

const std::vector<android::ui::Dataspace> Hwc2TestDataspace::defaultDataspaces = {
    android::ui::Dataspace::UNKNOWN,
};

const std::vector<android::ui::Dataspace> Hwc2TestDataspace::basicDataspaces = {
    android::ui::Dataspace::UNKNOWN,
    android::ui::Dataspace::V0_SRGB,
};

const std::vector<android::ui::Dataspace> Hwc2TestDataspace::completeDataspaces = {
    android::ui::Dataspace::UNKNOWN,
    android::ui::Dataspace::ARBITRARY,
    android::ui::Dataspace::STANDARD_SHIFT,
    android::ui::Dataspace::STANDARD_MASK,
    android::ui::Dataspace::STANDARD_UNSPECIFIED,
    android::ui::Dataspace::STANDARD_BT709,
    android::ui::Dataspace::STANDARD_BT601_625,
    android::ui::Dataspace::STANDARD_BT601_625_UNADJUSTED,
    android::ui::Dataspace::STANDARD_BT601_525,
    android::ui::Dataspace::STANDARD_BT601_525_UNADJUSTED,
    android::ui::Dataspace::STANDARD_BT2020,
    android::ui::Dataspace::STANDARD_BT2020_CONSTANT_LUMINANCE,
    android::ui::Dataspace::STANDARD_BT470M,
    android::ui::Dataspace::STANDARD_FILM,
    android::ui::Dataspace::TRANSFER_SHIFT,
    android::ui::Dataspace::TRANSFER_MASK,
    android::ui::Dataspace::TRANSFER_UNSPECIFIED,
    android::ui::Dataspace::TRANSFER_LINEAR,
    android::ui::Dataspace::TRANSFER_SRGB,
    android::ui::Dataspace::TRANSFER_SMPTE_170M,
    android::ui::Dataspace::TRANSFER_GAMMA2_2,
    android::ui::Dataspace::TRANSFER_GAMMA2_8,
    android::ui::Dataspace::TRANSFER_ST2084,
    android::ui::Dataspace::TRANSFER_HLG,
    android::ui::Dataspace::RANGE_SHIFT,
    android::ui::Dataspace::RANGE_MASK,
    android::ui::Dataspace::RANGE_UNSPECIFIED,
    android::ui::Dataspace::RANGE_FULL,
    android::ui::Dataspace::RANGE_LIMITED,
    android::ui::Dataspace::SRGB_LINEAR,
    android::ui::Dataspace::V0_SRGB_LINEAR,
    android::ui::Dataspace::SRGB,
    android::ui::Dataspace::V0_SRGB,
    android::ui::Dataspace::JFIF,
    android::ui::Dataspace::V0_JFIF,
    android::ui::Dataspace::BT601_625,
    android::ui::Dataspace::V0_BT601_625,
    android::ui::Dataspace::BT601_525,
    android::ui::Dataspace::V0_BT601_525,
    android::ui::Dataspace::BT709,
    android::ui::Dataspace::V0_BT709,
    android::ui::Dataspace::DEPTH,
};


Hwc2TestDisplayDimension::Hwc2TestDisplayDimension(Hwc2TestCoverage coverage)
    : Hwc2TestProperty(
            (coverage == Hwc2TestCoverage::Complete)? mCompleteDisplayDimensions:
            (coverage == Hwc2TestCoverage::Basic)? mBasicDisplayDimensions:
            mDefaultDisplayDimensions, mCompositionSupport) { }

std::string Hwc2TestDisplayDimension::dump() const
{
    std::stringstream dmp;
    const UnsignedArea& curr = get();
    dmp << "\tdisplay dimension: " << curr.width<< " x " << curr.height<< "\n";
    return dmp.str();
}

void Hwc2TestDisplayDimension::setDependent(Hwc2TestVirtualBuffer* buffer)
{
    mBuffers.insert(buffer);
    updateDependents();
}

void Hwc2TestDisplayDimension::updateDependents()
{
    const UnsignedArea& curr = get();

    for (Hwc2TestVirtualBuffer* buffer : mBuffers)
        buffer->updateBufferArea({static_cast<int32_t>(curr.width),
                static_cast<int32_t>(curr.height)});
}

const std::vector<UnsignedArea>
        Hwc2TestDisplayDimension::mDefaultDisplayDimensions = {
    {1920, 1080},
};

const std::vector<UnsignedArea>
        Hwc2TestDisplayDimension::mBasicDisplayDimensions = {
    {640, 480},
    {1280, 720},
    {1920, 1080},
    {1920, 1200},
};

const std::vector<UnsignedArea>
        Hwc2TestDisplayDimension::mCompleteDisplayDimensions = {
    {320, 240},
    {480, 320},
    {640, 480},
    {1280, 720},
    {1920, 1080},
    {1920, 1200},
    {2560, 1440},
    {2560, 1600},
    {3840, 2160},
    {4096, 2160},
};


Hwc2TestDisplayFrame::Hwc2TestDisplayFrame(Hwc2TestCoverage coverage,
        const Area& displayArea)
    : Hwc2TestProperty(mDisplayFrames, mCompositionSupport),
      mFrectScalars((coverage == Hwc2TestCoverage::Complete)? mCompleteFrectScalars:
            (coverage == Hwc2TestCoverage::Basic)? mBasicFrectScalars:
            mDefaultFrectScalars),
      mDisplayArea(displayArea)
{
    update();
}

std::string Hwc2TestDisplayFrame::dump() const
{
    std::stringstream dmp;
    const hwc_rect_t& displayFrame = get();
    dmp << "\tdisplay frame: left " << displayFrame.left << ", top "
            << displayFrame.top << ", right " << displayFrame.right
            << ", bottom " << displayFrame.bottom << "\n";
    return dmp.str();
}

void Hwc2TestDisplayFrame::update()
{
    mDisplayFrames.clear();

    if (mDisplayArea.width == 0 && mDisplayArea.height == 0) {
        mDisplayFrames.push_back({0, 0, 0, 0});
        return;
    }

    for (const auto& frectScalar : mFrectScalars) {
        mDisplayFrames.push_back({
                static_cast<int>(frectScalar.left * mDisplayArea.width),
                static_cast<int>(frectScalar.top * mDisplayArea.height),
                static_cast<int>(frectScalar.right * mDisplayArea.width),
                static_cast<int>(frectScalar.bottom * mDisplayArea.height)});
    }
}

const std::vector<hwc_frect_t> Hwc2TestDisplayFrame::mDefaultFrectScalars = {
    {0.0, 0.0, 1.0, 1.0},
};

const std::vector<hwc_frect_t> Hwc2TestDisplayFrame::mBasicFrectScalars = {
    {0.0, 0.0, 1.0, 1.0},
    {0.0, 0.0, 1.0, 0.05},
    {0.0, 0.95, 1.0, 1.0},
};

const std::vector<hwc_frect_t> Hwc2TestDisplayFrame::mCompleteFrectScalars = {
    {0.0, 0.0, 1.0, 1.0},
    {0.0, 0.05, 1.0, 0.95},
    {0.0, 0.05, 1.0, 1.0},
    {0.0, 0.0, 1.0, 0.05},
    {0.0, 0.95, 1.0, 1.0},
    {0.25, 0.0, 0.75, 0.35},
    {0.25, 0.25, 0.75, 0.75},
};


Hwc2TestPlaneAlpha::Hwc2TestPlaneAlpha(Hwc2TestCoverage coverage)
    : Hwc2TestProperty(coverage, mCompletePlaneAlphas, mBasicPlaneAlphas,
            mDefaultPlaneAlphas, mCompositionSupport) { }

std::string Hwc2TestPlaneAlpha::dump() const
{
    std::stringstream dmp;
    dmp << "\tplane alpha: " << get() << "\n";
    return dmp.str();
}

const std::vector<float> Hwc2TestPlaneAlpha::mDefaultPlaneAlphas = {
    1.0f,
};

const std::vector<float> Hwc2TestPlaneAlpha::mBasicPlaneAlphas = {
    1.0f, 0.0f,
};

const std::vector<float> Hwc2TestPlaneAlpha::mCompletePlaneAlphas = {
    1.0f, 0.75f, 0.5f, 0.25f, 0.0f,
};


Hwc2TestSourceCrop::Hwc2TestSourceCrop(Hwc2TestCoverage coverage,
        const Area& bufferArea)
    : Hwc2TestProperty(mSourceCrops, mCompositionSupport),
      mFrectScalars((coverage == Hwc2TestCoverage::Complete)? mCompleteFrectScalars:
            (coverage == Hwc2TestCoverage::Basic)? mBasicFrectScalars:
            mDefaultFrectScalars),
      mBufferArea(bufferArea)
{
    update();
}

std::string Hwc2TestSourceCrop::dump() const
{
    std::stringstream dmp;
    const hwc_frect_t& sourceCrop = get();
    dmp << "\tsource crop: left " << sourceCrop.left << ", top "
            << sourceCrop.top << ", right " << sourceCrop.right << ", bottom "
            << sourceCrop.bottom << "\n";
    return dmp.str();
}

void Hwc2TestSourceCrop::updateBufferArea(const Area& bufferArea)
{
    mBufferArea = bufferArea;
    update();
}

void Hwc2TestSourceCrop::update()
{
    mSourceCrops.clear();

    if (mBufferArea.width == 0 && mBufferArea.height == 0) {
        mSourceCrops.push_back({0, 0, 0, 0});
        return;
    }

    for (const auto& frectScalar : mFrectScalars) {
        mSourceCrops.push_back({
                frectScalar.left * mBufferArea.width,
                frectScalar.top * mBufferArea.height,
                frectScalar.right * mBufferArea.width,
                frectScalar.bottom * mBufferArea.height});
    }
}

const std::vector<hwc_frect_t> Hwc2TestSourceCrop::mDefaultFrectScalars = {
    {0.0, 0.0, 1.0, 1.0},
};

const std::vector<hwc_frect_t> Hwc2TestSourceCrop::mBasicFrectScalars = {
    {0.0, 0.0, 1.0, 1.0},
    {0.0, 0.0, 0.5, 0.5},
    {0.5, 0.5, 1.0, 1.0},
};

const std::vector<hwc_frect_t> Hwc2TestSourceCrop::mCompleteFrectScalars = {
    {0.0, 0.0, 1.0, 1.0},
    {0.0, 0.0, 0.5, 0.5},
    {0.5, 0.5, 1.0, 1.0},
    {0.0, 0.0, 0.25, 0.25},
    {0.25, 0.25, 0.75, 0.75},
};


Hwc2TestSurfaceDamage::Hwc2TestSurfaceDamage(Hwc2TestCoverage coverage)
    : Hwc2TestProperty(mSurfaceDamages, mCompositionSupport),
      mRegionScalars((coverage == Hwc2TestCoverage::Complete)? mCompleteRegionScalars:
            (coverage == Hwc2TestCoverage::Basic)? mBasicRegionScalars:
            mDefaultRegionScalars)
{
    update();
}

Hwc2TestSurfaceDamage::~Hwc2TestSurfaceDamage()
{
    freeSurfaceDamages();
}

std::string Hwc2TestSurfaceDamage::dump() const
{
    std::stringstream dmp;

    const hwc_region_t& curr = get();
    dmp << "\tsurface damage: region count " << curr.numRects << "\n";
    for (size_t i = 0; i < curr.numRects; i++) {
        const hwc_rect_t& rect = curr.rects[i];
        dmp << "\t\trect: left " << rect.left << ", top " << rect.top
                << ", right " << rect.right << ", bottom " << rect.bottom << "\n";
    }

    return dmp.str();
}

void Hwc2TestSurfaceDamage::updateBufferArea(const Area& bufferArea)
{
    mBufferArea = bufferArea;
    update();
}

void Hwc2TestSurfaceDamage::update()
{
    freeSurfaceDamages();

    if (mBufferArea.width == 0 && mBufferArea.height == 0) {
        mSurfaceDamages.push_back({0, nullptr});
        return;
    }

    hwc_region_t damage;

    for (const auto& regionScalar : mRegionScalars) {
        damage.numRects = regionScalar.size();

        if (damage.numRects > 0) {
            hwc_rect_t* rects = new hwc_rect_t[damage.numRects];
            if (!rects) {
                ALOGW("failed to allocate new hwc_rect_t array");
                continue;
            }

            for (size_t i = 0; i < damage.numRects; i++) {
                rects[i].left = regionScalar[i].left * mBufferArea.width;
                rects[i].top = regionScalar[i].top * mBufferArea.height;
                rects[i].right = regionScalar[i].right * mBufferArea.width;
                rects[i].bottom = regionScalar[i].bottom * mBufferArea.height;
            }

            damage.rects = static_cast<hwc_rect_t const*>(rects);
        } else {
            damage.rects = nullptr;
        }

        mSurfaceDamages.push_back(damage);
    }
}

void Hwc2TestSurfaceDamage::freeSurfaceDamages()
{
    for (const auto& surfaceDamage : mSurfaceDamages) {
        if (surfaceDamage.numRects > 0 && surfaceDamage.rects)
            delete[] surfaceDamage.rects;
    }
    mSurfaceDamages.clear();
}

const std::vector<std::vector<hwc_frect_t>> Hwc2TestSurfaceDamage::mDefaultRegionScalars = {
    {{}},
};

const std::vector<std::vector<hwc_frect_t>> Hwc2TestSurfaceDamage::mBasicRegionScalars = {
    {{}},
    {{0.0, 0.0, 1.0, 1.0}},
};

const std::vector<std::vector<hwc_frect_t>> Hwc2TestSurfaceDamage::mCompleteRegionScalars = {
    {{}},
    {{0.0, 0.0, 1.0, 1.0}},
    {{0.0, 0.0, 0.5, 0.5}, {0.5, 0.5, 1.0, 1.0}},
};


Hwc2TestTransform::Hwc2TestTransform(Hwc2TestCoverage coverage)
    : Hwc2TestProperty(coverage, mCompleteTransforms, mBasicTransforms,
            mDefaultTransforms, mCompositionSupport) { }

std::string Hwc2TestTransform::dump() const
{
    std::stringstream dmp;
    dmp << "\ttransform: " << getTransformName(get()) << "\n";
    return dmp.str();
}

const std::vector<hwc_transform_t> Hwc2TestTransform::mDefaultTransforms = {
    static_cast<hwc_transform_t>(0),
};

const std::vector<hwc_transform_t> Hwc2TestTransform::mBasicTransforms = {
    static_cast<hwc_transform_t>(0),
    HWC_TRANSFORM_FLIP_H,
    HWC_TRANSFORM_FLIP_V,
    HWC_TRANSFORM_ROT_90,
};

const std::vector<hwc_transform_t> Hwc2TestTransform::mCompleteTransforms = {
    static_cast<hwc_transform_t>(0),
    HWC_TRANSFORM_FLIP_H,
    HWC_TRANSFORM_FLIP_V,
    HWC_TRANSFORM_ROT_90,
    HWC_TRANSFORM_ROT_180,
    HWC_TRANSFORM_ROT_270,
    HWC_TRANSFORM_FLIP_H_ROT_90,
    HWC_TRANSFORM_FLIP_V_ROT_90,
};


Hwc2TestVisibleRegion::~Hwc2TestVisibleRegion()
{
    release();
}

std::string Hwc2TestVisibleRegion::dump() const
{
    std::stringstream dmp;

    const hwc_region_t& curr = get();
    dmp << "\tvisible region: region count " << curr.numRects << "\n";
    for (size_t i = 0; i < curr.numRects; i++) {
        const hwc_rect_t& rect = curr.rects[i];
        dmp << "\t\trect: left " << rect.left << ", top " << rect.top
                << ", right " << rect.right << ", bottom " << rect.bottom << "\n";
    }

    return dmp.str();
}

void Hwc2TestVisibleRegion::set(const android::Region& visibleRegion)
{
    release();

    size_t size = 0;
    const android::Rect* rects = visibleRegion.getArray(&size);

    mVisibleRegion.numRects = size;
    mVisibleRegion.rects = nullptr;

    if (size > 0) {
        hwc_rect_t* hwcRects = new hwc_rect_t[size];
        for (size_t i = 0; i < size; i++) {
            hwcRects[i].left = rects[i].left;
            hwcRects[i].top = rects[i].top;
            hwcRects[i].right = rects[i].right;
            hwcRects[i].bottom = rects[i].bottom;
        }
        mVisibleRegion.rects = hwcRects;
    }
}

hwc_region_t Hwc2TestVisibleRegion::get() const
{
    return mVisibleRegion;
}

void Hwc2TestVisibleRegion::release()
{
    if (mVisibleRegion.numRects > 0 && mVisibleRegion.rects)
        delete[] mVisibleRegion.rects;
    mVisibleRegion.rects = nullptr;
    mVisibleRegion.numRects = 0;
}

/* Identifies which layer properties are supported by each composition type.
 * hwc2_composition_t values range from:
 *  HWC2_COMPOSITION_INVALID = 0,
 *  HWC2_COMPOSITION_CLIENT = 1,
 *  HWC2_COMPOSITION_DEVICE = 2,
 *  HWC2_COMPOSITION_SOLID_COLOR = 3,
 *  HWC2_COMPOSITION_CURSOR = 4,
 *  HWC2_COMPOSITION_SIDEBAND = 5,
 *
 * Each property array can be indexed by a hwc2_composition_t value.
 * By using an array instead of a more complex data structure, runtimes for
 * some test cases showed a noticeable improvement.
 */

/*  INVALID  CLIENT   DEVICE   COLOR    CURSOR   SIDEBAND */
const std::array<bool, 6> Hwc2TestBufferArea::mCompositionSupport = {{
    false,   true,    true,    false,   true,    true,
}};

/*  INVALID  CLIENT   DEVICE   COLOR    CURSOR   SIDEBAND */
const std::array<bool, 6> Hwc2TestBlendMode::mCompositionSupport = {{
    false,   true,    true,    false,   true,    true,
}};

/*  INVALID  CLIENT   DEVICE   COLOR    CURSOR   SIDEBAND */
const std::array<bool, 6> Hwc2TestColor::mCompositionSupport = {{
    false,   false,   false,   true,    false,   false,
}};

/*  INVALID  CLIENT   DEVICE   COLOR    CURSOR   SIDEBAND */
const std::array<bool, 6> Hwc2TestComposition::mCompositionSupport = {{
    false,   true,    true,    true,    true,    true,
}};

/*  INVALID  CLIENT   DEVICE   COLOR    CURSOR   SIDEBAND */
const std::array<bool, 6> Hwc2TestDataspace::mCompositionSupport = {{
    false,   true,    true,    true,    true,    false,
}};

/*  INVALID  CLIENT   DEVICE   COLOR    CURSOR   SIDEBAND */
const std::array<bool, 6> Hwc2TestDisplayDimension::mCompositionSupport = {{
    false,   true,    true,    true,    true,    true,
}};

/*  INVALID  CLIENT   DEVICE   COLOR    CURSOR   SIDEBAND */
const std::array<bool, 6> Hwc2TestDisplayFrame::mCompositionSupport = {{
    false,   true,    true,    true,    false,   true,
}};

/*  INVALID  CLIENT   DEVICE   COLOR    CURSOR   SIDEBAND */
const std::array<bool, 6> Hwc2TestPlaneAlpha::mCompositionSupport = {{
    false,   true,    true,    true,    true,    true,
}};

/*  INVALID  CLIENT   DEVICE   COLOR    CURSOR   SIDEBAND */
const std::array<bool, 6> Hwc2TestSourceCrop::mCompositionSupport = {{
    false,   true,    true,    false,   true,    false,
}};

/*  INVALID  CLIENT   DEVICE   COLOR    CURSOR   SIDEBAND */
const std::array<bool, 6> Hwc2TestSurfaceDamage::mCompositionSupport = {{
    false,   false,   true,    false,   true,    false,
}};

/*  INVALID  CLIENT   DEVICE   COLOR    CURSOR   SIDEBAND */
const std::array<bool, 6> Hwc2TestTransform::mCompositionSupport = {{
    false,   true,    true,    false,   true,    true,
}};
