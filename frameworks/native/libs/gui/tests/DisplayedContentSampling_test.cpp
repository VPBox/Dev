/*
 * Copyright 2018 The Android Open Source Project
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

#include <gtest/gtest.h>

#include <binder/ProcessState.h>
#include <gui/ISurfaceComposer.h>
#include <gui/Surface.h>
#include <gui/SurfaceComposerClient.h>
#include <inttypes.h>

namespace android {

using Transaction = SurfaceComposerClient::Transaction;

static constexpr uint32_t INVALID_MASK = 0x10;
class DisplayedContentSamplingTest : public ::testing::Test {
protected:
    void SetUp() {
        mComposerClient = new SurfaceComposerClient;
        ASSERT_EQ(OK, mComposerClient->initCheck());
        mDisplayToken = mComposerClient->getInternalDisplayToken();
        ASSERT_TRUE(mDisplayToken);
    }

    bool shouldSkipTest() {
        ui::PixelFormat format;
        ui::Dataspace dataspace;
        status_t status =
                mComposerClient->getDisplayedContentSamplingAttributes(mDisplayToken, &format,
                                                                       &dataspace, &componentMask);
        if (status == PERMISSION_DENIED) {
            SUCCEED() << "permissions denial, skipping test";
            return true;
        }
        if (status == INVALID_OPERATION) {
            SUCCEED() << "optional function not supported, skipping test";
            return true;
        }
        return false;
    }

    sp<SurfaceComposerClient> mComposerClient;
    sp<IBinder> mDisplayToken;
    uint8_t componentMask = 0;
};

TEST_F(DisplayedContentSamplingTest, GetDisplayedContentSamplingAttributesAreSane) {
    // tradefed infrastructure does not support use of GTEST_SKIP
    if (shouldSkipTest()) return;

    ui::PixelFormat format;
    ui::Dataspace dataspace;
    status_t status =
            mComposerClient->getDisplayedContentSamplingAttributes(mDisplayToken, &format,
                                                                   &dataspace, &componentMask);
    EXPECT_EQ(OK, status);
    EXPECT_LE(componentMask, INVALID_MASK);
}

TEST_F(DisplayedContentSamplingTest, EnableWithInvalidMaskReturnsBadValue) {
    if (shouldSkipTest()) return;

    status_t status =
            mComposerClient->setDisplayContentSamplingEnabled(mDisplayToken, true, INVALID_MASK, 0);
    EXPECT_EQ(BAD_VALUE, status);
}

TEST_F(DisplayedContentSamplingTest, EnableAndDisableSucceed) {
    if (shouldSkipTest()) return;

    status_t status = mComposerClient->setDisplayContentSamplingEnabled(mDisplayToken, true,
                                                                        componentMask, 10);
    EXPECT_EQ(OK, status);

    status = mComposerClient->setDisplayContentSamplingEnabled(mDisplayToken, false, componentMask,
                                                               0);
    EXPECT_EQ(OK, status);
}

TEST_F(DisplayedContentSamplingTest, SelectivelyDisableComponentOk) {
    if (shouldSkipTest()) return;

    status_t status = mComposerClient->setDisplayContentSamplingEnabled(mDisplayToken, true,
                                                                        componentMask, 0);
    EXPECT_EQ(OK, status);

    // Clear the lowest bit.
    componentMask &= (componentMask - 1);
    status = mComposerClient->setDisplayContentSamplingEnabled(mDisplayToken, false, componentMask,
                                                               0);
    EXPECT_EQ(OK, status);
}

TEST_F(DisplayedContentSamplingTest, SampleCollectionCoherentWithSupportMask) {
    if (shouldSkipTest()) return;

    DisplayedFrameStats stats;
    status_t status = mComposerClient->getDisplayedContentSample(mDisplayToken, 0, 0, &stats);
    EXPECT_EQ(OK, status);
    if (stats.numFrames <= 0) return;

    if (componentMask & (0x1 << 0)) EXPECT_NE(0, stats.component_0_sample.size());
    if (componentMask & (0x1 << 1)) EXPECT_NE(0, stats.component_1_sample.size());
    if (componentMask & (0x1 << 2)) EXPECT_NE(0, stats.component_2_sample.size());
    if (componentMask & (0x1 << 3)) EXPECT_NE(0, stats.component_3_sample.size());
}

} // namespace android
