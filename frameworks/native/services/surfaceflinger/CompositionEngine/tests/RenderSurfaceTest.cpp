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

#include <cstdarg>
#include <cstdint>

#include <compositionengine/RenderSurfaceCreationArgs.h>
#include <compositionengine/impl/RenderSurface.h>
#include <compositionengine/mock/CompositionEngine.h>
#include <compositionengine/mock/Display.h>
#include <compositionengine/mock/DisplaySurface.h>
#include <compositionengine/mock/NativeWindow.h>
#include <compositionengine/mock/OutputLayer.h>
#include <gtest/gtest.h>
#include <renderengine/mock/RenderEngine.h>

#include "MockHWComposer.h"

namespace android::compositionengine {
namespace {

/* ------------------------------------------------------------------------
 * RenderSurfaceTest
 */

constexpr int32_t DEFAULT_DISPLAY_WIDTH = 1920;
constexpr int32_t DEFAULT_DISPLAY_HEIGHT = 1080;
constexpr std::optional<DisplayId> DEFAULT_DISPLAY_ID = std::make_optional(DisplayId{123u});
const std::string DEFAULT_DISPLAY_NAME = "Mock Display";

using testing::_;
using testing::ByMove;
using testing::DoAll;
using testing::Ref;
using testing::Return;
using testing::ReturnRef;
using testing::SetArgPointee;
using testing::StrictMock;

class RenderSurfaceTest : public testing::Test {
public:
    RenderSurfaceTest() {
        EXPECT_CALL(mDisplay, getId()).WillRepeatedly(ReturnRef(DEFAULT_DISPLAY_ID));
        EXPECT_CALL(mDisplay, getName()).WillRepeatedly(ReturnRef(DEFAULT_DISPLAY_NAME));
        EXPECT_CALL(mCompositionEngine, getHwComposer).WillRepeatedly(ReturnRef(mHwComposer));
        EXPECT_CALL(mCompositionEngine, getRenderEngine).WillRepeatedly(ReturnRef(mRenderEngine));
        EXPECT_CALL(*mNativeWindow, disconnect(NATIVE_WINDOW_API_EGL))
                .WillRepeatedly(Return(NO_ERROR));
    }
    ~RenderSurfaceTest() override = default;

    StrictMock<android::mock::HWComposer> mHwComposer;
    StrictMock<renderengine::mock::RenderEngine> mRenderEngine;
    StrictMock<mock::CompositionEngine> mCompositionEngine;
    StrictMock<mock::Display> mDisplay;
    sp<mock::NativeWindow> mNativeWindow = new StrictMock<mock::NativeWindow>();
    sp<mock::DisplaySurface> mDisplaySurface = new StrictMock<mock::DisplaySurface>();
    impl::RenderSurface mSurface{mCompositionEngine, mDisplay,
                                 RenderSurfaceCreationArgs{DEFAULT_DISPLAY_WIDTH,
                                                           DEFAULT_DISPLAY_HEIGHT, mNativeWindow,
                                                           mDisplaySurface}};
};

/* ------------------------------------------------------------------------
 * Basic construction
 */

TEST_F(RenderSurfaceTest, canInstantiate) {
    EXPECT_TRUE(mSurface.isValid());
}

/* ------------------------------------------------------------------------
 * RenderSurface::initialize()
 */

TEST_F(RenderSurfaceTest, initializeConfiguresNativeWindow) {
    EXPECT_CALL(*mNativeWindow, connect(NATIVE_WINDOW_API_EGL)).WillOnce(Return(NO_ERROR));
    EXPECT_CALL(*mNativeWindow, setBuffersFormat(HAL_PIXEL_FORMAT_RGBA_8888))
            .WillOnce(Return(NO_ERROR));
    EXPECT_CALL(*mNativeWindow, setUsage(GRALLOC_USAGE_HW_RENDER)).WillOnce(Return(NO_ERROR));

    mSurface.initialize();
}

/* ------------------------------------------------------------------------
 * RenderSurface::getSize()
 */

TEST_F(RenderSurfaceTest, sizeReturnsConstructedSize) {
    const ui::Size expected{DEFAULT_DISPLAY_WIDTH, DEFAULT_DISPLAY_HEIGHT};

    EXPECT_EQ(expected, mSurface.getSize());
}

/* ------------------------------------------------------------------------
 * RenderSurface::getClientTargetAcquireFence()
 */

TEST_F(RenderSurfaceTest, getClientTargetAcquireFenceForwardsCall) {
    sp<Fence> fence = new Fence();

    EXPECT_CALL(*mDisplaySurface, getClientTargetAcquireFence()).WillOnce(ReturnRef(fence));

    EXPECT_EQ(fence.get(), mSurface.getClientTargetAcquireFence().get());
}

/* ------------------------------------------------------------------------
 * RenderSurface::setDisplaySize()
 */

TEST_F(RenderSurfaceTest, setDisplaySizeAppliesChange) {
    EXPECT_CALL(*mDisplaySurface, resizeBuffers(640, 480)).Times(1);

    mSurface.setDisplaySize(ui::Size(640, 480));
}

/* ------------------------------------------------------------------------
 * RenderSurface::setBufferDataspace()
 */

TEST_F(RenderSurfaceTest, setBufferDataspaceAppliesChange) {
    EXPECT_CALL(*mNativeWindow, setBuffersDataSpace(ui::Dataspace::DISPLAY_P3))
            .WillOnce(Return(NO_ERROR));

    mSurface.setBufferDataspace(ui::Dataspace::DISPLAY_P3);
}

/* ------------------------------------------------------------------------
 * RenderSurface::setProtected()
 */

TEST_F(RenderSurfaceTest, setProtectedTrueEnablesProtection) {
    EXPECT_FALSE(mSurface.isProtected());
    EXPECT_CALL(*mNativeWindow, setUsage(GRALLOC_USAGE_HW_RENDER | GRALLOC_USAGE_PROTECTED))
            .WillOnce(Return(NO_ERROR));

    mSurface.setProtected(true);
    EXPECT_TRUE(mSurface.isProtected());
}

TEST_F(RenderSurfaceTest, setProtectedFalseDisablesProtection) {
    EXPECT_FALSE(mSurface.isProtected());
    EXPECT_CALL(*mNativeWindow, setUsage(GRALLOC_USAGE_HW_RENDER)).WillOnce(Return(NO_ERROR));

    mSurface.setProtected(false);
    EXPECT_FALSE(mSurface.isProtected());
}

TEST_F(RenderSurfaceTest, setProtectedEnableAndDisable) {
    EXPECT_FALSE(mSurface.isProtected());
    EXPECT_CALL(*mNativeWindow, setUsage(GRALLOC_USAGE_HW_RENDER | GRALLOC_USAGE_PROTECTED))
            .WillOnce(Return(NO_ERROR));
    EXPECT_CALL(*mNativeWindow, setUsage(GRALLOC_USAGE_HW_RENDER)).WillOnce(Return(NO_ERROR));

    mSurface.setProtected(true);
    EXPECT_TRUE(mSurface.isProtected());
    mSurface.setProtected(false);
    EXPECT_FALSE(mSurface.isProtected());
}

TEST_F(RenderSurfaceTest, setProtectedEnableWithError) {
    EXPECT_FALSE(mSurface.isProtected());
    EXPECT_CALL(*mNativeWindow, setUsage(GRALLOC_USAGE_HW_RENDER | GRALLOC_USAGE_PROTECTED))
            .WillOnce(Return(INVALID_OPERATION));
    mSurface.setProtected(true);
    EXPECT_FALSE(mSurface.isProtected());
}

/* ------------------------------------------------------------------------
 * RenderSurface::beginFrame()
 */

TEST_F(RenderSurfaceTest, beginFrameAppliesChange) {
    EXPECT_CALL(*mDisplaySurface, beginFrame(true)).WillOnce(Return(NO_ERROR));

    EXPECT_EQ(NO_ERROR, mSurface.beginFrame(true));
}

/* ------------------------------------------------------------------------
 * RenderSurface::prepareFrame()
 */

TEST_F(RenderSurfaceTest, prepareFramePassesOutputLayersToHwc) {
    EXPECT_CALL(mHwComposer, prepare(*DEFAULT_DISPLAY_ID, Ref(mDisplay)))
            .WillOnce(Return(INVALID_OPERATION));

    EXPECT_EQ(INVALID_OPERATION, mSurface.prepareFrame());
}

TEST_F(RenderSurfaceTest, prepareFrameTakesEarlyOutOnHwcError) {
    EXPECT_CALL(mHwComposer, prepare(*DEFAULT_DISPLAY_ID, Ref(mDisplay)))
            .WillOnce(Return(INVALID_OPERATION));

    EXPECT_EQ(INVALID_OPERATION, mSurface.prepareFrame());
}

TEST_F(RenderSurfaceTest, prepareFrameHandlesMixedComposition) {
    EXPECT_CALL(mHwComposer, prepare(*DEFAULT_DISPLAY_ID, Ref(mDisplay)))
            .WillOnce(Return(NO_ERROR));
    EXPECT_CALL(mHwComposer, hasClientComposition(DEFAULT_DISPLAY_ID)).WillOnce(Return(true));
    EXPECT_CALL(mHwComposer, hasDeviceComposition(DEFAULT_DISPLAY_ID)).WillOnce(Return(true));

    EXPECT_CALL(*mDisplaySurface, prepareFrame(DisplaySurface::COMPOSITION_MIXED))
            .WillOnce(Return(INVALID_OPERATION));

    EXPECT_EQ(INVALID_OPERATION, mSurface.prepareFrame());
}

TEST_F(RenderSurfaceTest, prepareFrameHandlesOnlyGlesComposition) {
    EXPECT_CALL(mHwComposer, prepare(*DEFAULT_DISPLAY_ID, Ref(mDisplay)))
            .WillOnce(Return(NO_ERROR));
    EXPECT_CALL(mHwComposer, hasClientComposition(DEFAULT_DISPLAY_ID)).WillOnce(Return(true));
    EXPECT_CALL(mHwComposer, hasDeviceComposition(DEFAULT_DISPLAY_ID)).WillOnce(Return(false));

    EXPECT_CALL(*mDisplaySurface, prepareFrame(DisplaySurface::COMPOSITION_GLES))
            .WillOnce(Return(NO_ERROR));

    EXPECT_EQ(NO_ERROR, mSurface.prepareFrame());
}

TEST_F(RenderSurfaceTest, prepareFrameHandlesOnlyHwcComposition) {
    EXPECT_CALL(mHwComposer, prepare(*DEFAULT_DISPLAY_ID, Ref(mDisplay)))
            .WillOnce(Return(NO_ERROR));
    EXPECT_CALL(mHwComposer, hasClientComposition(DEFAULT_DISPLAY_ID)).WillOnce(Return(false));
    EXPECT_CALL(mHwComposer, hasDeviceComposition(DEFAULT_DISPLAY_ID)).WillOnce(Return(true));

    EXPECT_CALL(*mDisplaySurface, prepareFrame(DisplaySurface::COMPOSITION_HWC))
            .WillOnce(Return(NO_ERROR));

    EXPECT_EQ(NO_ERROR, mSurface.prepareFrame());
}

TEST_F(RenderSurfaceTest, prepareFrameHandlesNoComposition) {
    EXPECT_CALL(mHwComposer, prepare(*DEFAULT_DISPLAY_ID, Ref(mDisplay)))
            .WillOnce(Return(NO_ERROR));
    EXPECT_CALL(mHwComposer, hasClientComposition(DEFAULT_DISPLAY_ID)).WillOnce(Return(false));
    EXPECT_CALL(mHwComposer, hasDeviceComposition(DEFAULT_DISPLAY_ID)).WillOnce(Return(false));

    EXPECT_CALL(*mDisplaySurface, prepareFrame(DisplaySurface::COMPOSITION_HWC))
            .WillOnce(Return(NO_ERROR));

    EXPECT_EQ(NO_ERROR, mSurface.prepareFrame());
}

/* ------------------------------------------------------------------------
 * RenderSurface::dequeueBuffer()
 */

TEST_F(RenderSurfaceTest, dequeueBufferObtainsABuffer) {
    sp<GraphicBuffer> buffer = new GraphicBuffer();

    EXPECT_CALL(*mNativeWindow, dequeueBuffer(_, _))
            .WillOnce(
                    DoAll(SetArgPointee<0>(buffer.get()), SetArgPointee<1>(-1), Return(NO_ERROR)));

    base::unique_fd fence;
    EXPECT_EQ(buffer.get(), mSurface.dequeueBuffer(&fence).get());

    EXPECT_EQ(buffer.get(), mSurface.mutableGraphicBufferForTest().get());
}

/* ------------------------------------------------------------------------
 * RenderSurface::queueBuffer()
 */

TEST_F(RenderSurfaceTest, queueBufferHandlesNoClientComposition) {
    sp<GraphicBuffer> buffer = new GraphicBuffer();
    mSurface.mutableGraphicBufferForTest() = buffer;

    EXPECT_CALL(mHwComposer, hasClientComposition(DEFAULT_DISPLAY_ID)).WillOnce(Return(false));
    EXPECT_CALL(mHwComposer, hasFlipClientTargetRequest(DEFAULT_DISPLAY_ID))
            .WillOnce(Return(false));
    EXPECT_CALL(*mDisplaySurface, advanceFrame()).Times(1);

    mSurface.queueBuffer(base::unique_fd());

    EXPECT_EQ(buffer.get(), mSurface.mutableGraphicBufferForTest().get());
}

TEST_F(RenderSurfaceTest, queueBufferHandlesClientComposition) {
    sp<GraphicBuffer> buffer = new GraphicBuffer();
    mSurface.mutableGraphicBufferForTest() = buffer;

    EXPECT_CALL(mHwComposer, hasClientComposition(DEFAULT_DISPLAY_ID)).WillOnce(Return(true));
    EXPECT_CALL(*mNativeWindow, queueBuffer(buffer->getNativeBuffer(), -1))
            .WillOnce(Return(NO_ERROR));
    EXPECT_CALL(*mDisplaySurface, advanceFrame()).Times(1);

    mSurface.queueBuffer(base::unique_fd());

    EXPECT_EQ(nullptr, mSurface.mutableGraphicBufferForTest().get());
}

TEST_F(RenderSurfaceTest, queueBufferHandlesFlipClientTargetRequest) {
    sp<GraphicBuffer> buffer = new GraphicBuffer();
    mSurface.mutableGraphicBufferForTest() = buffer;

    EXPECT_CALL(mHwComposer, hasClientComposition(DEFAULT_DISPLAY_ID)).WillOnce(Return(false));
    EXPECT_CALL(mHwComposer, hasFlipClientTargetRequest(DEFAULT_DISPLAY_ID)).WillOnce(Return(true));
    EXPECT_CALL(*mNativeWindow, queueBuffer(buffer->getNativeBuffer(), -1))
            .WillOnce(Return(NO_ERROR));
    EXPECT_CALL(*mDisplaySurface, advanceFrame()).Times(1);

    mSurface.queueBuffer(base::unique_fd());

    EXPECT_EQ(nullptr, mSurface.mutableGraphicBufferForTest().get());
}

TEST_F(RenderSurfaceTest, queueBufferHandlesFlipClientTargetRequestWithNoBufferYetDequeued) {
    sp<GraphicBuffer> buffer = new GraphicBuffer();

    EXPECT_CALL(mHwComposer, hasClientComposition(DEFAULT_DISPLAY_ID)).WillOnce(Return(false));
    EXPECT_CALL(mHwComposer, hasFlipClientTargetRequest(DEFAULT_DISPLAY_ID)).WillOnce(Return(true));
    EXPECT_CALL(*mNativeWindow, dequeueBuffer(_, _))
            .WillOnce(
                    DoAll(SetArgPointee<0>(buffer.get()), SetArgPointee<1>(-1), Return(NO_ERROR)));
    EXPECT_CALL(*mNativeWindow, queueBuffer(buffer->getNativeBuffer(), -1))
            .WillOnce(Return(NO_ERROR));
    EXPECT_CALL(*mDisplaySurface, advanceFrame()).Times(1);

    mSurface.queueBuffer(base::unique_fd());

    EXPECT_EQ(nullptr, mSurface.mutableGraphicBufferForTest().get());
}

TEST_F(RenderSurfaceTest, queueBufferHandlesNativeWindowQueueBufferFailureOnVirtualDisplay) {
    sp<GraphicBuffer> buffer = new GraphicBuffer();
    mSurface.mutableGraphicBufferForTest() = buffer;

    EXPECT_CALL(mHwComposer, hasClientComposition(DEFAULT_DISPLAY_ID)).WillOnce(Return(true));
    EXPECT_CALL(*mNativeWindow, queueBuffer(buffer->getNativeBuffer(), -1))
            .WillOnce(Return(INVALID_OPERATION));
    EXPECT_CALL(mDisplay, isVirtual()).WillOnce(Return(true));
    EXPECT_CALL(*mNativeWindow, cancelBuffer(buffer->getNativeBuffer(), -1))
            .WillOnce(Return(NO_ERROR));
    EXPECT_CALL(*mDisplaySurface, advanceFrame()).Times(1);

    mSurface.queueBuffer(base::unique_fd());

    EXPECT_EQ(nullptr, mSurface.mutableGraphicBufferForTest().get());
}

/* ------------------------------------------------------------------------
 * RenderSurface::onPresentDisplayCompleted()
 */

TEST_F(RenderSurfaceTest, onPresentDisplayCompletedForwardsSignal) {
    EXPECT_CALL(*mDisplaySurface, onFrameCommitted()).Times(1);

    mSurface.onPresentDisplayCompleted();
}

/* ------------------------------------------------------------------------
 * RenderSurface::setViewportAndProjection()
 */

TEST_F(RenderSurfaceTest, setViewportAndProjectionAppliesChang) {
    mSurface.setSizeForTest(ui::Size(100, 200));

    EXPECT_CALL(mRenderEngine,
                setViewportAndProjection(100, 200, Rect(100, 200), ui::Transform::ROT_0))
            .Times(1);

    mSurface.setViewportAndProjection();
}

/* ------------------------------------------------------------------------
 * RenderSurface::flip()
 */

TEST_F(RenderSurfaceTest, flipForwardsSignal) {
    mSurface.setPageFlipCountForTest(500);

    mSurface.flip();

    EXPECT_EQ(501, mSurface.getPageFlipCount());
}

} // namespace
} // namespace android::compositionengine
