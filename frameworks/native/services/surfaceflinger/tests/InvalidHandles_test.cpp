/*
 * Copyright (C) 2019 The Android Open Source Project
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

#include <binder/Binder.h>

#include <gtest/gtest.h>

#include <gui/ISurfaceComposer.h>
#include <gui/SurfaceComposerClient.h>
#include <private/gui/ComposerService.h>
#include <ui/Rect.h>

namespace android {
namespace {

class NotALayer : public BBinder {};

/**
 * For all of these tests we make a SurfaceControl with an invalid layer handle
 * and verify we aren't able to trick SurfaceFlinger.
 */
class InvalidHandleTest : public ::testing::Test {
protected:
    sp<SurfaceComposerClient> mScc;
    sp<SurfaceControl> mNotSc;
    void SetUp() override {
        mScc = new SurfaceComposerClient;
        ASSERT_EQ(NO_ERROR, mScc->initCheck());
        mNotSc = makeNotSurfaceControl();
    }

    sp<SurfaceControl> makeNotSurfaceControl() {
        return new SurfaceControl(mScc, new NotALayer(), nullptr, true);
    }
};

TEST_F(InvalidHandleTest, createSurfaceInvalidHandle) {
    auto notSc = makeNotSurfaceControl();
    ASSERT_EQ(nullptr,
              mScc->createSurface(String8("lolcats"), 19, 47, PIXEL_FORMAT_RGBA_8888, 0,
                                  notSc.get())
                      .get());
}

TEST_F(InvalidHandleTest, captureLayersInvalidHandle) {
    sp<ISurfaceComposer> sf(ComposerService::getComposerService());
    sp<GraphicBuffer> outBuffer;

    ASSERT_EQ(NAME_NOT_FOUND,
              sf->captureLayers(mNotSc->getHandle(), &outBuffer, Rect::EMPTY_RECT, 1.0f));
}

} // namespace
} // namespace android
