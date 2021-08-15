/*
 * Copyright 2013 The Android Open Source Project
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

#define LOG_TAG "SurfaceTextureFBO_test"
//#define LOG_NDEBUG 0

#include "SurfaceTextureFBO.h"

namespace android {

// This test is intended to verify that proper synchronization is done when
// rendering into an FBO.
TEST_F(SurfaceTextureFBOTest, BlitFromCpuFilledBufferToFbo) {
    const int texWidth = 64;
    const int texHeight = 64;

    ASSERT_EQ(NO_ERROR, native_window_api_connect(mANW.get(),
            NATIVE_WINDOW_API_CPU));
    ASSERT_EQ(NO_ERROR, native_window_set_buffers_dimensions(mANW.get(),
            texWidth, texHeight));
    ASSERT_EQ(NO_ERROR, native_window_set_buffers_format(mANW.get(),
            HAL_PIXEL_FORMAT_RGBA_8888));
    ASSERT_EQ(NO_ERROR, native_window_set_usage(mANW.get(),
            GRALLOC_USAGE_SW_READ_OFTEN | GRALLOC_USAGE_SW_WRITE_OFTEN));

    android_native_buffer_t* anb;
    ASSERT_EQ(NO_ERROR, native_window_dequeue_buffer_and_wait(mANW.get(),
            &anb));
    ASSERT_TRUE(anb != nullptr);

    sp<GraphicBuffer> buf(GraphicBuffer::from(anb));

    // Fill the buffer with green
    uint8_t* img = nullptr;
    buf->lock(GRALLOC_USAGE_SW_WRITE_OFTEN, (void**)(&img));
    fillRGBA8BufferSolid(img, texWidth, texHeight, buf->getStride(), 0, 255,
            0, 255);
    buf->unlock();
    ASSERT_EQ(NO_ERROR, mANW->queueBuffer(mANW.get(), buf->getNativeBuffer(),
            -1));

    ASSERT_EQ(NO_ERROR, mST->updateTexImage());

    glBindFramebuffer(GL_FRAMEBUFFER, mFbo);
    drawTexture();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    for (int i = 0; i < 4; i++) {
        SCOPED_TRACE(String8::format("frame %d", i).string());

        ASSERT_EQ(NO_ERROR, native_window_dequeue_buffer_and_wait(mANW.get(),
                &anb));
        ASSERT_TRUE(anb != nullptr);

        buf = GraphicBuffer::from(anb);

        // Fill the buffer with red
        ASSERT_EQ(NO_ERROR, buf->lock(GRALLOC_USAGE_SW_WRITE_OFTEN,
                (void**)(&img)));
        fillRGBA8BufferSolid(img, texWidth, texHeight, buf->getStride(), 255, 0,
                0, 255);
        ASSERT_EQ(NO_ERROR, buf->unlock());
        ASSERT_EQ(NO_ERROR, mANW->queueBuffer(mANW.get(),
                buf->getNativeBuffer(), -1));

        ASSERT_EQ(NO_ERROR, mST->updateTexImage());

        drawTexture();

        EXPECT_TRUE(checkPixel( 24, 39, 255, 0, 0, 255));
    }

    glBindFramebuffer(GL_FRAMEBUFFER, mFbo);

    EXPECT_TRUE(checkPixel( 24, 39, 0, 255, 0, 255));
}

} // namespace android
