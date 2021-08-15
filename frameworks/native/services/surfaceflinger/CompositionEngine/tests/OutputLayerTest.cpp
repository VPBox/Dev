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

#include <compositionengine/impl/OutputLayer.h>
#include <compositionengine/mock/CompositionEngine.h>
#include <compositionengine/mock/Layer.h>
#include <compositionengine/mock/LayerFE.h>
#include <compositionengine/mock/Output.h>
#include <gtest/gtest.h>

#include "MockHWC2.h"
#include "MockHWComposer.h"
#include "RectMatcher.h"

namespace android::compositionengine {
namespace {

using testing::_;
using testing::Return;
using testing::ReturnRef;
using testing::StrictMock;

constexpr DisplayId DEFAULT_DISPLAY_ID = DisplayId{42};

constexpr auto TR_IDENT = 0u;
constexpr auto TR_FLP_H = HAL_TRANSFORM_FLIP_H;
constexpr auto TR_FLP_V = HAL_TRANSFORM_FLIP_V;
constexpr auto TR_ROT_90 = HAL_TRANSFORM_ROT_90;
constexpr auto TR_ROT_180 = TR_FLP_H | TR_FLP_V;
constexpr auto TR_ROT_270 = TR_ROT_90 | TR_ROT_180;

const std::string kOutputName{"Test Output"};

class OutputLayerTest : public testing::Test {
public:
    OutputLayerTest() {
        EXPECT_CALL(*mLayerFE, getDebugName()).WillRepeatedly(Return("Test LayerFE"));
        EXPECT_CALL(mOutput, getName()).WillRepeatedly(ReturnRef(kOutputName));

        EXPECT_CALL(*mLayer, getState()).WillRepeatedly(ReturnRef(mLayerState));
        EXPECT_CALL(mOutput, getState()).WillRepeatedly(ReturnRef(mOutputState));
    }

    ~OutputLayerTest() override = default;

    compositionengine::mock::Output mOutput;
    std::shared_ptr<compositionengine::mock::Layer> mLayer{
            new StrictMock<compositionengine::mock::Layer>()};
    sp<compositionengine::mock::LayerFE> mLayerFE{
            new StrictMock<compositionengine::mock::LayerFE>()};
    impl::OutputLayer mOutputLayer{mOutput, mLayer, mLayerFE};

    impl::LayerCompositionState mLayerState;
    impl::OutputCompositionState mOutputState;
};

/*
 * Basic construction
 */

TEST_F(OutputLayerTest, canInstantiateOutputLayer) {}

/*
 * OutputLayer::initialize()
 */

TEST_F(OutputLayerTest, initializingOutputLayerWithoutHwcDoesNothingInteresting) {
    StrictMock<compositionengine::mock::CompositionEngine> compositionEngine;

    mOutputLayer.initialize(compositionEngine, std::nullopt);

    EXPECT_FALSE(mOutputLayer.getState().hwc);
}

TEST_F(OutputLayerTest, initializingOutputLayerWithHwcDisplayCreatesHwcLayer) {
    StrictMock<compositionengine::mock::CompositionEngine> compositionEngine;
    StrictMock<android::mock::HWComposer> hwc;
    StrictMock<HWC2::mock::Layer> hwcLayer;

    EXPECT_CALL(compositionEngine, getHwComposer()).WillOnce(ReturnRef(hwc));
    EXPECT_CALL(hwc, createLayer(DEFAULT_DISPLAY_ID)).WillOnce(Return(&hwcLayer));

    mOutputLayer.initialize(compositionEngine, DEFAULT_DISPLAY_ID);

    const auto& outputLayerState = mOutputLayer.getState();
    ASSERT_TRUE(outputLayerState.hwc);

    const auto& hwcState = *outputLayerState.hwc;
    EXPECT_EQ(&hwcLayer, hwcState.hwcLayer.get());

    EXPECT_CALL(hwc, destroyLayer(DEFAULT_DISPLAY_ID, &hwcLayer));
    mOutputLayer.editState().hwc.reset();
}

/*
 * OutputLayer::calculateOutputDisplayFrame()
 */

struct OutputLayerDisplayFrameTest : public OutputLayerTest {
    OutputLayerDisplayFrameTest() {
        // Set reasonable default values for a simple case. Each test will
        // set one specific value to something different.

        mLayerState.frontEnd.geomActiveTransparentRegion = Region{};
        mLayerState.frontEnd.geomLayerTransform = ui::Transform{TR_IDENT};
        mLayerState.frontEnd.geomBufferSize = Rect{0, 0, 1920, 1080};
        mLayerState.frontEnd.geomBufferUsesDisplayInverseTransform = false;
        mLayerState.frontEnd.geomCrop = Rect{0, 0, 1920, 1080};
        mLayerState.frontEnd.geomLayerBounds = FloatRect{0.f, 0.f, 1920.f, 1080.f};

        mOutputState.viewport = Rect{0, 0, 1920, 1080};
        mOutputState.transform = ui::Transform{TR_IDENT};
    }

    Rect calculateOutputDisplayFrame() {
        mLayerState.frontEnd.geomInverseLayerTransform =
                mLayerState.frontEnd.geomLayerTransform.inverse();

        return mOutputLayer.calculateOutputDisplayFrame();
    }
};

TEST_F(OutputLayerDisplayFrameTest, correctForSimpleDefaultCase) {
    const Rect expected{0, 0, 1920, 1080};
    EXPECT_THAT(calculateOutputDisplayFrame(), RectEq(expected));
}

TEST_F(OutputLayerDisplayFrameTest, fullActiveTransparentRegionReturnsEmptyFrame) {
    mLayerState.frontEnd.geomActiveTransparentRegion = Region{Rect{0, 0, 1920, 1080}};
    const Rect expected{0, 0, 0, 0};
    EXPECT_THAT(calculateOutputDisplayFrame(), RectEq(expected));
}

TEST_F(OutputLayerDisplayFrameTest, cropAffectsDisplayFrame) {
    mLayerState.frontEnd.geomCrop = Rect{100, 200, 300, 500};
    const Rect expected{100, 200, 300, 500};
    EXPECT_THAT(calculateOutputDisplayFrame(), RectEq(expected));
}

TEST_F(OutputLayerDisplayFrameTest, cropAffectsDisplayFrameRotated) {
    mLayerState.frontEnd.geomCrop = Rect{100, 200, 300, 500};
    mLayerState.frontEnd.geomLayerTransform.set(HAL_TRANSFORM_ROT_90, 1920, 1080);
    const Rect expected{1420, 100, 1720, 300};
    EXPECT_THAT(calculateOutputDisplayFrame(), RectEq(expected));
}

TEST_F(OutputLayerDisplayFrameTest, emptyGeomCropIsNotUsedToComputeFrame) {
    mLayerState.frontEnd.geomCrop = Rect{};
    const Rect expected{0, 0, 1920, 1080};
    EXPECT_THAT(calculateOutputDisplayFrame(), RectEq(expected));
}

TEST_F(OutputLayerDisplayFrameTest, geomLayerSnapToBoundsAffectsFrame) {
    mLayerState.frontEnd.geomLayerBounds = FloatRect{0.f, 0.f, 960.f, 540.f};
    const Rect expected{0, 0, 960, 540};
    EXPECT_THAT(calculateOutputDisplayFrame(), RectEq(expected));
}

TEST_F(OutputLayerDisplayFrameTest, viewportAffectsFrame) {
    mOutputState.viewport = Rect{0, 0, 960, 540};
    const Rect expected{0, 0, 960, 540};
    EXPECT_THAT(calculateOutputDisplayFrame(), RectEq(expected));
}

TEST_F(OutputLayerDisplayFrameTest, outputTransformAffectsDisplayFrame) {
    mOutputState.transform = ui::Transform{HAL_TRANSFORM_ROT_90};
    const Rect expected{-1080, 0, 0, 1920};
    EXPECT_THAT(calculateOutputDisplayFrame(), RectEq(expected));
}

/*
 * OutputLayer::calculateOutputRelativeBufferTransform()
 */

TEST_F(OutputLayerTest, calculateOutputRelativeBufferTransformTestsNeeded) {
    mLayerState.frontEnd.geomBufferUsesDisplayInverseTransform = false;

    struct Entry {
        uint32_t layer;
        uint32_t buffer;
        uint32_t display;
        uint32_t expected;
    };
    // Not an exhaustive list of cases, but hopefully enough.
    const std::array<Entry, 24> testData = {
            // clang-format off
            //             layer       buffer      display     expected
            /*  0 */ Entry{TR_IDENT,   TR_IDENT,   TR_IDENT,   TR_IDENT},
            /*  1 */ Entry{TR_IDENT,   TR_IDENT,   TR_ROT_90,  TR_ROT_90},
            /*  2 */ Entry{TR_IDENT,   TR_IDENT,   TR_ROT_180, TR_ROT_180},
            /*  3 */ Entry{TR_IDENT,   TR_IDENT,   TR_ROT_270, TR_ROT_270},

            /*  4 */ Entry{TR_IDENT,   TR_FLP_H,   TR_IDENT,   TR_FLP_H ^ TR_IDENT},
            /*  5 */ Entry{TR_IDENT,   TR_FLP_H,   TR_ROT_90,  TR_FLP_H ^ TR_ROT_90},
            /*  6 */ Entry{TR_IDENT,   TR_FLP_H,   TR_ROT_180, TR_FLP_H ^ TR_ROT_180},
            /*  7 */ Entry{TR_IDENT,   TR_FLP_H,   TR_ROT_270, TR_FLP_H ^ TR_ROT_270},

            /*  8 */ Entry{TR_IDENT,   TR_FLP_V,   TR_IDENT,   TR_FLP_V},
            /*  9 */ Entry{TR_IDENT,   TR_ROT_90,  TR_ROT_90,  TR_ROT_180},
            /* 10 */ Entry{TR_IDENT,   TR_ROT_180, TR_ROT_180, TR_IDENT},
            /* 11 */ Entry{TR_IDENT,   TR_ROT_270, TR_ROT_270, TR_ROT_180},

            /* 12 */ Entry{TR_ROT_90,  TR_IDENT,   TR_IDENT,   TR_IDENT ^ TR_ROT_90},
            /* 13 */ Entry{TR_ROT_90,  TR_FLP_H,   TR_ROT_90,  TR_FLP_H ^ TR_ROT_180},
            /* 14 */ Entry{TR_ROT_90,  TR_IDENT,   TR_ROT_180, TR_IDENT ^ TR_ROT_270},
            /* 15 */ Entry{TR_ROT_90,  TR_FLP_H,   TR_ROT_270, TR_FLP_H ^ TR_IDENT},

            /* 16 */ Entry{TR_ROT_180, TR_FLP_H,   TR_IDENT,   TR_FLP_H ^ TR_ROT_180},
            /* 17 */ Entry{TR_ROT_180, TR_IDENT,   TR_ROT_90,  TR_IDENT ^ TR_ROT_270},
            /* 18 */ Entry{TR_ROT_180, TR_FLP_H,   TR_ROT_180, TR_FLP_H ^ TR_IDENT},
            /* 19 */ Entry{TR_ROT_180, TR_IDENT,   TR_ROT_270, TR_IDENT ^ TR_ROT_90},

            /* 20 */ Entry{TR_ROT_270, TR_IDENT,   TR_IDENT,   TR_IDENT ^ TR_ROT_270},
            /* 21 */ Entry{TR_ROT_270, TR_FLP_H,   TR_ROT_90,  TR_FLP_H ^ TR_IDENT},
            /* 22 */ Entry{TR_ROT_270, TR_FLP_H,   TR_ROT_180, TR_FLP_H ^ TR_ROT_90},
            /* 23 */ Entry{TR_ROT_270, TR_IDENT,   TR_ROT_270, TR_IDENT ^ TR_ROT_180},
            // clang-format on
    };

    for (size_t i = 0; i < testData.size(); i++) {
        const auto& entry = testData[i];

        mLayerState.frontEnd.geomLayerTransform.set(entry.layer, 1920, 1080);
        mLayerState.frontEnd.geomBufferTransform = entry.buffer;
        mOutputState.orientation = entry.display;

        auto actual = mOutputLayer.calculateOutputRelativeBufferTransform();
        EXPECT_EQ(entry.expected, actual) << "entry " << i;
    }
}

/*
 * OutputLayer::writeStateToHWC()
 */

struct OutputLayerWriteStateToHWCTest : public OutputLayerTest {
    static constexpr HWC2::Error kError = HWC2::Error::Unsupported;
    static constexpr FloatRect kSourceCrop{11.f, 12.f, 13.f, 14.f};
    static constexpr uint32_t kZOrder = 21u;
    static constexpr Hwc2::Transform kBufferTransform = static_cast<Hwc2::Transform>(31);
    static constexpr Hwc2::IComposerClient::BlendMode kBlendMode =
            static_cast<Hwc2::IComposerClient::BlendMode>(41);
    static constexpr float kAlpha = 51.f;
    static constexpr uint32_t kType = 61u;
    static constexpr uint32_t kAppId = 62u;

    static const Rect kDisplayFrame;

    OutputLayerWriteStateToHWCTest() {
        auto& outputLayerState = mOutputLayer.editState();
        outputLayerState.hwc = impl::OutputLayerCompositionState::Hwc(mHwcLayer);

        outputLayerState.displayFrame = kDisplayFrame;
        outputLayerState.sourceCrop = kSourceCrop;
        outputLayerState.z = kZOrder;
        outputLayerState.bufferTransform = static_cast<Hwc2::Transform>(kBufferTransform);

        mLayerState.frontEnd.blendMode = kBlendMode;
        mLayerState.frontEnd.alpha = kAlpha;
        mLayerState.frontEnd.type = kType;
        mLayerState.frontEnd.appId = kAppId;
    }

    void expectGeometryCommonCalls() {
        EXPECT_CALL(*mHwcLayer, setDisplayFrame(kDisplayFrame)).WillOnce(Return(kError));
        EXPECT_CALL(*mHwcLayer, setSourceCrop(kSourceCrop)).WillOnce(Return(kError));
        EXPECT_CALL(*mHwcLayer, setZOrder(kZOrder)).WillOnce(Return(kError));
        EXPECT_CALL(*mHwcLayer, setTransform(static_cast<HWC2::Transform>(kBufferTransform)))
                .WillOnce(Return(kError));

        EXPECT_CALL(*mHwcLayer, setBlendMode(static_cast<HWC2::BlendMode>(kBlendMode)))
                .WillOnce(Return(kError));
        EXPECT_CALL(*mHwcLayer, setPlaneAlpha(kAlpha)).WillOnce(Return(kError));
        EXPECT_CALL(*mHwcLayer, setInfo(kType, kAppId)).WillOnce(Return(kError));
    }

    std::shared_ptr<HWC2::mock::Layer> mHwcLayer{std::make_shared<StrictMock<HWC2::mock::Layer>>()};
};

const Rect OutputLayerWriteStateToHWCTest::kDisplayFrame{1001, 1002, 1003, 10044};

TEST_F(OutputLayerWriteStateToHWCTest, doesNothingIfNoHWCState) {
    mOutputLayer.editState().hwc.reset();

    mOutputLayer.writeStateToHWC(true);
}

TEST_F(OutputLayerWriteStateToHWCTest, doesNothingIfNoHWCLayer) {
    mOutputLayer.editState().hwc = impl::OutputLayerCompositionState::Hwc(nullptr);

    mOutputLayer.writeStateToHWC(true);
}

TEST_F(OutputLayerWriteStateToHWCTest, canSetsAllState) {
    expectGeometryCommonCalls();

    mOutputLayer.writeStateToHWC(true);
}

} // namespace
} // namespace android::compositionengine
