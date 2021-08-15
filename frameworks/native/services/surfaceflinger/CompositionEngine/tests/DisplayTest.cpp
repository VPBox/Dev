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

#include <cmath>

#include <compositionengine/DisplayColorProfileCreationArgs.h>
#include <compositionengine/DisplayCreationArgs.h>
#include <compositionengine/DisplaySurface.h>
#include <compositionengine/RenderSurfaceCreationArgs.h>
#include <compositionengine/impl/Display.h>
#include <compositionengine/mock/CompositionEngine.h>
#include <compositionengine/mock/NativeWindow.h>
#include <compositionengine/mock/RenderSurface.h>
#include <gtest/gtest.h>

#include "MockHWComposer.h"

namespace android::compositionengine {
namespace {

using testing::Return;
using testing::ReturnRef;
using testing::StrictMock;

constexpr DisplayId DEFAULT_DISPLAY_ID = DisplayId{42};

class DisplayTest : public testing::Test {
public:
    ~DisplayTest() override = default;

    StrictMock<android::mock::HWComposer> mHwComposer;
    StrictMock<mock::CompositionEngine> mCompositionEngine;
    sp<mock::NativeWindow> mNativeWindow = new StrictMock<mock::NativeWindow>();
    impl::Display mDisplay{mCompositionEngine,
                           DisplayCreationArgsBuilder().setDisplayId(DEFAULT_DISPLAY_ID).build()};
};

/* ------------------------------------------------------------------------
 * Basic construction
 */

TEST_F(DisplayTest, canInstantiateDisplay) {
    {
        constexpr DisplayId display1 = DisplayId{123u};
        auto display =
                impl::createDisplay(mCompositionEngine,
                                    DisplayCreationArgsBuilder().setDisplayId(display1).build());
        EXPECT_FALSE(display->isSecure());
        EXPECT_FALSE(display->isVirtual());
        EXPECT_EQ(display1, display->getId());
    }

    {
        constexpr DisplayId display2 = DisplayId{546u};
        auto display = impl::createDisplay(mCompositionEngine,
                                           DisplayCreationArgsBuilder()
                                                   .setIsSecure(true)
                                                   .setDisplayId(display2)
                                                   .build());
        EXPECT_TRUE(display->isSecure());
        EXPECT_FALSE(display->isVirtual());
        EXPECT_EQ(display2, display->getId());
    }

    {
        constexpr DisplayId display3 = DisplayId{789u};
        auto display = impl::createDisplay(mCompositionEngine,
                                           DisplayCreationArgsBuilder()
                                                   .setIsVirtual(true)
                                                   .setDisplayId(display3)
                                                   .build());
        EXPECT_FALSE(display->isSecure());
        EXPECT_TRUE(display->isVirtual());
        EXPECT_EQ(display3, display->getId());
    }
}

/* ------------------------------------------------------------------------
 * Display::disconnect()
 */

TEST_F(DisplayTest, disconnectDisconnectsDisplay) {
    EXPECT_CALL(mCompositionEngine, getHwComposer()).WillRepeatedly(ReturnRef(mHwComposer));

    // The first call to disconnect will disconnect the display with the HWC and
    // set mHwcId to -1.
    EXPECT_CALL(mHwComposer, disconnectDisplay(DEFAULT_DISPLAY_ID)).Times(1);
    mDisplay.disconnect();
    EXPECT_FALSE(mDisplay.getId());

    // Subsequent calls will do nothing,
    EXPECT_CALL(mHwComposer, disconnectDisplay(DEFAULT_DISPLAY_ID)).Times(0);
    mDisplay.disconnect();
    EXPECT_FALSE(mDisplay.getId());
}

/* ------------------------------------------------------------------------
 * Display::setColorTransform()
 */

TEST_F(DisplayTest, setColorTransformSetsTransform) {
    // Identity matrix sets an identity state value
    const mat4 identity;

    EXPECT_CALL(mCompositionEngine, getHwComposer()).WillRepeatedly(ReturnRef(mHwComposer));

    EXPECT_CALL(mHwComposer, setColorTransform(DEFAULT_DISPLAY_ID, identity)).Times(1);

    mDisplay.setColorTransform(identity);

    EXPECT_EQ(HAL_COLOR_TRANSFORM_IDENTITY, mDisplay.getState().colorTransform);

    // Non-identity matrix sets a non-identity state value
    const mat4 nonIdentity = mat4() * 2;

    EXPECT_CALL(mHwComposer, setColorTransform(DEFAULT_DISPLAY_ID, nonIdentity)).Times(1);

    mDisplay.setColorTransform(nonIdentity);

    EXPECT_EQ(HAL_COLOR_TRANSFORM_ARBITRARY_MATRIX, mDisplay.getState().colorTransform);
}

/* ------------------------------------------------------------------------
 * Display::setColorMode()
 */

TEST_F(DisplayTest, setColorModeSetsModeUnlessNoChange) {
    mock::RenderSurface* renderSurface = new StrictMock<mock::RenderSurface>();
    mDisplay.setRenderSurfaceForTest(std::unique_ptr<RenderSurface>(renderSurface));

    EXPECT_CALL(mCompositionEngine, getHwComposer()).WillRepeatedly(ReturnRef(mHwComposer));

    // These values are expected to be the initial state.
    ASSERT_EQ(ui::ColorMode::NATIVE, mDisplay.getState().colorMode);
    ASSERT_EQ(ui::Dataspace::UNKNOWN, mDisplay.getState().dataspace);
    ASSERT_EQ(ui::RenderIntent::COLORIMETRIC, mDisplay.getState().renderIntent);

    // Otherwise if the values are unchanged, nothing happens
    mDisplay.setColorMode(ui::ColorMode::NATIVE, ui::Dataspace::UNKNOWN,
                          ui::RenderIntent::COLORIMETRIC);

    EXPECT_EQ(ui::ColorMode::NATIVE, mDisplay.getState().colorMode);
    EXPECT_EQ(ui::Dataspace::UNKNOWN, mDisplay.getState().dataspace);
    EXPECT_EQ(ui::RenderIntent::COLORIMETRIC, mDisplay.getState().renderIntent);

    // Otherwise if the values are different, updates happen
    EXPECT_CALL(*renderSurface, setBufferDataspace(ui::Dataspace::DISPLAY_P3)).Times(1);
    EXPECT_CALL(mHwComposer,
                setActiveColorMode(DEFAULT_DISPLAY_ID, ui::ColorMode::DISPLAY_P3,
                                   ui::RenderIntent::TONE_MAP_COLORIMETRIC))
            .Times(1);

    mDisplay.setColorMode(ui::ColorMode::DISPLAY_P3, ui::Dataspace::DISPLAY_P3,
                          ui::RenderIntent::TONE_MAP_COLORIMETRIC);

    EXPECT_EQ(ui::ColorMode::DISPLAY_P3, mDisplay.getState().colorMode);
    EXPECT_EQ(ui::Dataspace::DISPLAY_P3, mDisplay.getState().dataspace);
    EXPECT_EQ(ui::RenderIntent::TONE_MAP_COLORIMETRIC, mDisplay.getState().renderIntent);
}

TEST_F(DisplayTest, setColorModeDoesNothingForVirtualDisplay) {
    impl::Display virtualDisplay{mCompositionEngine,
                                 DisplayCreationArgs{false, true, DEFAULT_DISPLAY_ID}};

    virtualDisplay.setColorMode(ui::ColorMode::DISPLAY_P3, ui::Dataspace::DISPLAY_P3,
                                ui::RenderIntent::TONE_MAP_COLORIMETRIC);

    EXPECT_EQ(ui::ColorMode::NATIVE, virtualDisplay.getState().colorMode);
    EXPECT_EQ(ui::Dataspace::UNKNOWN, virtualDisplay.getState().dataspace);
    EXPECT_EQ(ui::RenderIntent::COLORIMETRIC, virtualDisplay.getState().renderIntent);
}

/* ------------------------------------------------------------------------
 * Display::createDisplayColorProfile()
 */

TEST_F(DisplayTest, createDisplayColorProfileSetsDisplayColorProfile) {
    EXPECT_TRUE(mDisplay.getDisplayColorProfile() == nullptr);
    mDisplay.createDisplayColorProfile(
            DisplayColorProfileCreationArgs{false, HdrCapabilities(), 0,
                                            DisplayColorProfileCreationArgs::HwcColorModes()});
    EXPECT_TRUE(mDisplay.getDisplayColorProfile() != nullptr);
}

/* ------------------------------------------------------------------------
 * Display::createRenderSurface()
 */

TEST_F(DisplayTest, createRenderSurfaceSetsRenderSurface) {
    EXPECT_CALL(*mNativeWindow, disconnect(NATIVE_WINDOW_API_EGL)).WillRepeatedly(Return(NO_ERROR));
    EXPECT_TRUE(mDisplay.getRenderSurface() == nullptr);
    mDisplay.createRenderSurface(RenderSurfaceCreationArgs{640, 480, mNativeWindow, nullptr});
    EXPECT_TRUE(mDisplay.getRenderSurface() != nullptr);
}

} // namespace
} // namespace android::compositionengine
