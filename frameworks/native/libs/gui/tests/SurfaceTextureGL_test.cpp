/*
 * Copyright (C) 2011 The Android Open Source Project
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

#define LOG_TAG "SurfaceTextureGL_test"
//#define LOG_NDEBUG 0

#include "SurfaceTextureGL.h"

#include "DisconnectWaiter.h"
#include "FillBuffer.h"

namespace android {

TEST_F(SurfaceTextureGLTest, TexturingFromCpuFilledYV12BufferNpot) {
    const int texWidth = 64;
    const int texHeight = 66;

    ASSERT_EQ(NO_ERROR, native_window_api_connect(mANW.get(),
            NATIVE_WINDOW_API_CPU));
    ASSERT_EQ(NO_ERROR, native_window_set_buffers_dimensions(mANW.get(),
            texWidth, texHeight));
    ASSERT_EQ(NO_ERROR, native_window_set_buffers_format(mANW.get(),
            HAL_PIXEL_FORMAT_YV12));
    ASSERT_EQ(NO_ERROR, native_window_set_usage(mANW.get(),
            GRALLOC_USAGE_SW_READ_OFTEN | GRALLOC_USAGE_SW_WRITE_OFTEN));

    ANativeWindowBuffer* anb;
    ASSERT_EQ(NO_ERROR, native_window_dequeue_buffer_and_wait(mANW.get(),
            &anb));
    ASSERT_TRUE(anb != nullptr);

    sp<GraphicBuffer> buf(GraphicBuffer::from(anb));

    // Fill the buffer with the a checkerboard pattern
    uint8_t* img = nullptr;
    buf->lock(GRALLOC_USAGE_SW_WRITE_OFTEN, (void**)(&img));
    fillYV12Buffer(img, texWidth, texHeight, buf->getStride());
    buf->unlock();
    ASSERT_EQ(NO_ERROR, mANW->queueBuffer(mANW.get(), buf->getNativeBuffer(),
            -1));

    ASSERT_EQ(NO_ERROR, mST->updateTexImage());

    glClearColor(0.2, 0.2, 0.2, 0.2);
    glClear(GL_COLOR_BUFFER_BIT);

    glViewport(0, 0, texWidth, texHeight);
    drawTexture();

    EXPECT_TRUE(checkPixel( 0,  0, 255, 127, 255, 255, 3));
    EXPECT_TRUE(checkPixel(63,  0,   0, 133,   0, 255, 3));
    EXPECT_TRUE(checkPixel(63, 65,   0, 133,   0, 255, 3));
    EXPECT_TRUE(checkPixel( 0, 65, 255, 127, 255, 255, 3));

    EXPECT_TRUE(checkPixel(22, 44, 255, 127, 255, 255, 3));
    EXPECT_TRUE(checkPixel(45, 52, 255, 127, 255, 255, 3));
    EXPECT_TRUE(checkPixel(52, 51,  98, 255,  73, 255, 3));
    EXPECT_TRUE(checkPixel( 7, 31, 155,   0, 118, 255, 3));
    EXPECT_TRUE(checkPixel(31,  9, 107,  24,  87, 255, 3));
    EXPECT_TRUE(checkPixel(29, 35, 255, 127, 255, 255, 3));
    EXPECT_TRUE(checkPixel(36, 22, 155,  29,   0, 255, 3));
}

TEST_F(SurfaceTextureGLTest, TexturingFromCpuFilledYV12BufferPow2) {
    const int texWidth = 64;
    const int texHeight = 64;

    ASSERT_EQ(NO_ERROR, native_window_api_connect(mANW.get(),
            NATIVE_WINDOW_API_CPU));
    ASSERT_EQ(NO_ERROR, native_window_set_buffers_dimensions(mANW.get(),
            texWidth, texHeight));
    ASSERT_EQ(NO_ERROR, native_window_set_buffers_format(mANW.get(),
            HAL_PIXEL_FORMAT_YV12));
    ASSERT_EQ(NO_ERROR, native_window_set_usage(mANW.get(),
            GRALLOC_USAGE_SW_READ_OFTEN | GRALLOC_USAGE_SW_WRITE_OFTEN));

    ANativeWindowBuffer* anb;
    ASSERT_EQ(NO_ERROR, native_window_dequeue_buffer_and_wait(mANW.get(),
            &anb));
    ASSERT_TRUE(anb != nullptr);

    sp<GraphicBuffer> buf(GraphicBuffer::from(anb));

    // Fill the buffer with the a checkerboard pattern
    uint8_t* img = nullptr;
    buf->lock(GRALLOC_USAGE_SW_WRITE_OFTEN, (void**)(&img));
    fillYV12Buffer(img, texWidth, texHeight, buf->getStride());
    buf->unlock();
    ASSERT_EQ(NO_ERROR, mANW->queueBuffer(mANW.get(), buf->getNativeBuffer(),
            -1));

    ASSERT_EQ(NO_ERROR, mST->updateTexImage());

    glClearColor(0.2, 0.2, 0.2, 0.2);
    glClear(GL_COLOR_BUFFER_BIT);

    glViewport(0, 0, texWidth, texHeight);
    drawTexture();

    EXPECT_TRUE(checkPixel( 0,  0,   0, 133,   0, 255));
    EXPECT_TRUE(checkPixel(63,  0, 255, 127, 255, 255));
    EXPECT_TRUE(checkPixel(63, 63,   0, 133,   0, 255));
    EXPECT_TRUE(checkPixel( 0, 63, 255, 127, 255, 255));

    EXPECT_TRUE(checkPixel(22, 19, 100, 255,  74, 255, 3));
    EXPECT_TRUE(checkPixel(45, 11, 100, 255,  74, 255, 3));
    EXPECT_TRUE(checkPixel(52, 12, 155,   0, 181, 255, 3));
    EXPECT_TRUE(checkPixel( 7, 32, 150, 237, 170, 255, 3));
    EXPECT_TRUE(checkPixel(31, 54,   0,  71, 117, 255, 3));
    EXPECT_TRUE(checkPixel(29, 28,   0, 133,   0, 255, 3));
    EXPECT_TRUE(checkPixel(36, 41, 100, 232, 255, 255, 3));
}

TEST_F(SurfaceTextureGLTest, TexturingFromCpuFilledYV12BufferWithCrop) {
    const int texWidth = 64;
    const int texHeight = 66;

    ASSERT_EQ(NO_ERROR, native_window_api_connect(mANW.get(),
            NATIVE_WINDOW_API_CPU));
    ASSERT_EQ(NO_ERROR, native_window_set_buffers_dimensions(mANW.get(),
            texWidth, texHeight));
    ASSERT_EQ(NO_ERROR, native_window_set_buffers_format(mANW.get(),
            HAL_PIXEL_FORMAT_YV12));
    ASSERT_EQ(NO_ERROR, native_window_set_usage(mANW.get(),
            GRALLOC_USAGE_SW_READ_OFTEN | GRALLOC_USAGE_SW_WRITE_OFTEN));

    android_native_rect_t crops[] = {
        {4, 6, 22, 36},
        {0, 6, 22, 36},
        {4, 0, 22, 36},
        {4, 6, texWidth, 36},
        {4, 6, 22, texHeight},
    };

    for (int i = 0; i < 5; i++) {
        const android_native_rect_t& crop(crops[i]);
        SCOPED_TRACE(String8::format("rect{ l: %d t: %d r: %d b: %d }",
                crop.left, crop.top, crop.right, crop.bottom).string());

        ASSERT_EQ(NO_ERROR, native_window_set_crop(mANW.get(), &crop));

        ANativeWindowBuffer* anb;
        ASSERT_EQ(NO_ERROR, native_window_dequeue_buffer_and_wait(mANW.get(),
                &anb));
        ASSERT_TRUE(anb != nullptr);

        sp<GraphicBuffer> buf(GraphicBuffer::from(anb));

        uint8_t* img = nullptr;
        buf->lock(GRALLOC_USAGE_SW_WRITE_OFTEN, (void**)(&img));
        fillYV12BufferRect(img, texWidth, texHeight, buf->getStride(), crop);
        buf->unlock();
        ASSERT_EQ(NO_ERROR, mANW->queueBuffer(mANW.get(),
                buf->getNativeBuffer(), -1));

        ASSERT_EQ(NO_ERROR, mST->updateTexImage());

        glClearColor(0.2, 0.2, 0.2, 0.2);
        glClear(GL_COLOR_BUFFER_BIT);

        glViewport(0, 0, 64, 64);
        drawTexture();

        EXPECT_TRUE(checkPixel( 0,  0,  82, 255,  35, 255));
        EXPECT_TRUE(checkPixel(63,  0,  82, 255,  35, 255));
        EXPECT_TRUE(checkPixel(63, 63,  82, 255,  35, 255));
        EXPECT_TRUE(checkPixel( 0, 63,  82, 255,  35, 255));

        EXPECT_TRUE(checkPixel(25, 14,  82, 255,  35, 255));
        EXPECT_TRUE(checkPixel(35, 31,  82, 255,  35, 255));
        EXPECT_TRUE(checkPixel(57,  6,  82, 255,  35, 255));
        EXPECT_TRUE(checkPixel( 5, 42,  82, 255,  35, 255));
        EXPECT_TRUE(checkPixel(32, 33,  82, 255,  35, 255));
        EXPECT_TRUE(checkPixel(16, 26,  82, 255,  35, 255));
        EXPECT_TRUE(checkPixel(46, 51,  82, 255,  35, 255));
    }
}

// This test is intended to catch synchronization bugs between the CPU-written
// and GPU-read buffers.
TEST_F(SurfaceTextureGLTest, TexturingFromCpuFilledYV12BuffersRepeatedly) {
    enum { texWidth = 16 };
    enum { texHeight = 16 };
    enum { numFrames = 1024 };

    ASSERT_EQ(NO_ERROR, native_window_api_connect(mANW.get(),
            NATIVE_WINDOW_API_CPU));
    ASSERT_EQ(NO_ERROR, native_window_set_buffers_dimensions(mANW.get(),
            texWidth, texHeight));
    ASSERT_EQ(NO_ERROR, native_window_set_buffers_format(mANW.get(),
            HAL_PIXEL_FORMAT_YV12));
    ASSERT_EQ(NO_ERROR, native_window_set_usage(mANW.get(),
            GRALLOC_USAGE_SW_WRITE_OFTEN));

    struct TestPixel {
        int x;
        int y;
    };
    const TestPixel testPixels[] = {
        {  4, 11 },
        { 12, 14 },
        {  7,  2 },
    };
    enum {numTestPixels = sizeof(testPixels) / sizeof(testPixels[0])};

    class ProducerThread : public Thread {
    public:
        ProducerThread(const sp<ANativeWindow>& anw,
                const TestPixel* testPixels):
                mANW(anw),
                mTestPixels(testPixels) {
        }

        virtual ~ProducerThread() {
        }

        virtual bool threadLoop() {
            for (int i = 0; i < numFrames; i++) {
                ANativeWindowBuffer* anb;
                if (native_window_dequeue_buffer_and_wait(mANW.get(),
                        &anb) != NO_ERROR) {
                    return false;
                }
                if (anb == nullptr) {
                    return false;
                }

                sp<GraphicBuffer> buf(GraphicBuffer::from(anb));

                const int yuvTexOffsetY = 0;
                int stride = buf->getStride();
                int yuvTexStrideY = stride;
                int yuvTexOffsetV = yuvTexStrideY * texHeight;
                int yuvTexStrideV = (yuvTexStrideY/2 + 0xf) & ~0xf;
                int yuvTexOffsetU = yuvTexOffsetV + yuvTexStrideV * texHeight/2;
                int yuvTexStrideU = yuvTexStrideV;

                uint8_t* img = nullptr;
                buf->lock(GRALLOC_USAGE_SW_WRITE_OFTEN, (void**)(&img));

                // Gray out all the test pixels first, so we're more likely to
                // see a failure if GL is still texturing from the buffer we
                // just dequeued.
                for (int j = 0; j < numTestPixels; j++) {
                    int x = mTestPixels[j].x;
                    int y = mTestPixels[j].y;
                    uint8_t value = 128;
                    img[y*stride + x] = value;
                }

                // Fill the buffer with gray.
                for (int y = 0; y < texHeight; y++) {
                    for (int x = 0; x < texWidth; x++) {
                        img[yuvTexOffsetY + y*yuvTexStrideY + x] = 128;
                        img[yuvTexOffsetU + (y/2)*yuvTexStrideU + x/2] = 128;
                        img[yuvTexOffsetV + (y/2)*yuvTexStrideV + x/2] = 128;
                    }
                }

                // Set the test pixels to either white or black.
                for (int j = 0; j < numTestPixels; j++) {
                    int x = mTestPixels[j].x;
                    int y = mTestPixels[j].y;
                    uint8_t value = 0;
                    if (j == (i % numTestPixels)) {
                        value = 255;
                    }
                    img[y*stride + x] = value;
                }

                buf->unlock();
                if (mANW->queueBuffer(mANW.get(), buf->getNativeBuffer(), -1)
                        != NO_ERROR) {
                    return false;
                }
            }
            return false;
        }

        sp<ANativeWindow> mANW;
        const TestPixel* mTestPixels;
    };

    sp<Thread> pt(new ProducerThread(mANW, testPixels));
    pt->run("ProducerThread");

    glViewport(0, 0, texWidth, texHeight);

    glClearColor(0.2, 0.2, 0.2, 0.2);
    glClear(GL_COLOR_BUFFER_BIT);

    // We wait for the first two frames up front so that the producer will be
    // likely to dequeue the buffer that's currently being textured from.
    mFW->waitForFrame();
    mFW->waitForFrame();

    for (int i = 0; i < numFrames; i++) {
        SCOPED_TRACE(String8::format("frame %d", i).string());

        // We must wait for each frame to come in because if we ever do an
        // updateTexImage call that doesn't consume a newly available buffer
        // then the producer and consumer will get out of sync, which will cause
        // a deadlock.
        if (i > 1) {
            mFW->waitForFrame();
        }
        ASSERT_EQ(NO_ERROR, mST->updateTexImage());
        drawTexture();

        for (int j = 0; j < numTestPixels; j++) {
            int x = testPixels[j].x;
            int y = testPixels[j].y;
            if (j == (i % numTestPixels)) {
                // We must y-invert the texture coords
                EXPECT_TRUE(checkPixel(x, texHeight-y-1, 255, 255, 255, 255));
            } else {
                // We must y-invert the texture coords
                EXPECT_TRUE(checkPixel(x, texHeight-y-1, 0, 0, 0, 255));
            }
        }
    }

    pt->requestExitAndWait();
}

TEST_F(SurfaceTextureGLTest, TexturingFromCpuFilledRGBABufferNpot) {
    const int texWidth = 64;
    const int texHeight = 66;

    ASSERT_EQ(NO_ERROR, native_window_api_connect(mANW.get(),
            NATIVE_WINDOW_API_CPU));
    ASSERT_EQ(NO_ERROR, native_window_set_buffers_dimensions(mANW.get(),
            texWidth, texHeight));
    ASSERT_EQ(NO_ERROR, native_window_set_buffers_format(mANW.get(),
            HAL_PIXEL_FORMAT_RGBA_8888));
    ASSERT_EQ(NO_ERROR, native_window_set_usage(mANW.get(),
            GRALLOC_USAGE_SW_READ_OFTEN | GRALLOC_USAGE_SW_WRITE_OFTEN));

    ASSERT_NO_FATAL_FAILURE(produceOneRGBA8Frame(mANW));

    ASSERT_EQ(NO_ERROR, mST->updateTexImage());

    glClearColor(0.2, 0.2, 0.2, 0.2);
    glClear(GL_COLOR_BUFFER_BIT);

    glViewport(0, 0, texWidth, texHeight);
    drawTexture();

    EXPECT_TRUE(checkPixel( 0,  0,  35,  35,  35,  35));
    EXPECT_TRUE(checkPixel(63,  0, 231, 231, 231, 231));
    EXPECT_TRUE(checkPixel(63, 65, 231, 231, 231, 231));
    EXPECT_TRUE(checkPixel( 0, 65,  35,  35,  35,  35));

    EXPECT_TRUE(checkPixel(15, 10,  35, 231, 231, 231));
    EXPECT_TRUE(checkPixel(23, 65, 231,  35, 231,  35));
    EXPECT_TRUE(checkPixel(19, 40,  35, 231,  35,  35));
    EXPECT_TRUE(checkPixel(38, 30, 231,  35,  35,  35));
    EXPECT_TRUE(checkPixel(42, 54,  35,  35,  35, 231));
    EXPECT_TRUE(checkPixel(37, 34,  35, 231, 231, 231));
    EXPECT_TRUE(checkPixel(31,  8, 231,  35,  35, 231));
    EXPECT_TRUE(checkPixel(37, 47, 231,  35, 231, 231));
    EXPECT_TRUE(checkPixel(25, 38,  35,  35,  35,  35));
    EXPECT_TRUE(checkPixel(49,  6,  35, 231,  35,  35));
    EXPECT_TRUE(checkPixel(54, 50,  35, 231, 231, 231));
    EXPECT_TRUE(checkPixel(27, 26, 231, 231, 231, 231));
    EXPECT_TRUE(checkPixel(10,  6,  35,  35, 231, 231));
    EXPECT_TRUE(checkPixel(29,  4,  35,  35,  35, 231));
    EXPECT_TRUE(checkPixel(55, 28,  35,  35, 231,  35));
    EXPECT_TRUE(checkPixel(58, 55,  35,  35, 231, 231));
}

TEST_F(SurfaceTextureGLTest, TexturingFromCpuFilledRGBABufferPow2) {
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

    ASSERT_NO_FATAL_FAILURE(produceOneRGBA8Frame(mANW));

    ASSERT_EQ(NO_ERROR, mST->updateTexImage());

    glClearColor(0.2, 0.2, 0.2, 0.2);
    glClear(GL_COLOR_BUFFER_BIT);

    glViewport(0, 0, texWidth, texHeight);
    drawTexture();

    EXPECT_TRUE(checkPixel( 0,  0, 231, 231, 231, 231));
    EXPECT_TRUE(checkPixel(63,  0,  35,  35,  35,  35));
    EXPECT_TRUE(checkPixel(63, 63, 231, 231, 231, 231));
    EXPECT_TRUE(checkPixel( 0, 63,  35,  35,  35,  35));

    EXPECT_TRUE(checkPixel(12, 46, 231, 231, 231,  35));
    EXPECT_TRUE(checkPixel(16,  1, 231, 231,  35, 231));
    EXPECT_TRUE(checkPixel(21, 12, 231,  35,  35, 231));
    EXPECT_TRUE(checkPixel(26, 51, 231,  35, 231,  35));
    EXPECT_TRUE(checkPixel( 5, 32,  35, 231, 231,  35));
    EXPECT_TRUE(checkPixel(13,  8,  35, 231, 231, 231));
    EXPECT_TRUE(checkPixel(46,  3,  35,  35, 231,  35));
    EXPECT_TRUE(checkPixel(30, 33,  35,  35,  35,  35));
    EXPECT_TRUE(checkPixel( 6, 52, 231, 231,  35,  35));
    EXPECT_TRUE(checkPixel(55, 33,  35, 231,  35, 231));
    EXPECT_TRUE(checkPixel(16, 29,  35,  35, 231, 231));
    EXPECT_TRUE(checkPixel( 1, 30,  35,  35,  35, 231));
    EXPECT_TRUE(checkPixel(41, 37,  35,  35, 231, 231));
    EXPECT_TRUE(checkPixel(46, 29, 231, 231,  35,  35));
    EXPECT_TRUE(checkPixel(15, 25,  35, 231,  35, 231));
    EXPECT_TRUE(checkPixel( 3, 52,  35, 231,  35,  35));
}

// Tests if GLConsumer and BufferQueue are robust enough
// to handle a special case where updateTexImage is called
// in the middle of disconnect.  This ordering is enforced
// by blocking in the disconnect callback.
TEST_F(SurfaceTextureGLTest, DisconnectStressTest) {

    class ProducerThread : public Thread {
    public:
        explicit ProducerThread(const sp<ANativeWindow>& anw):
                mANW(anw) {
        }

        virtual ~ProducerThread() {
        }

        virtual bool threadLoop() {
            ANativeWindowBuffer* anb;

            if (native_window_api_connect(mANW.get(), NATIVE_WINDOW_API_CPU) !=
                    NO_ERROR) {
                return false;
            }

            for (int numFrames =0 ; numFrames < 2; numFrames ++) {

                if (native_window_dequeue_buffer_and_wait(mANW.get(),
                        &anb) != NO_ERROR) {
                    return false;
                }
                if (anb == nullptr) {
                    return false;
                }
                if (mANW->queueBuffer(mANW.get(), anb, -1)
                        != NO_ERROR) {
                    return false;
                }
            }

            if (native_window_api_disconnect(mANW.get(), NATIVE_WINDOW_API_CPU)
                    != NO_ERROR) {
                return false;
            }

            return false;
        }

    private:
        sp<ANativeWindow> mANW;
    };

    sp<DisconnectWaiter> dw(new DisconnectWaiter());
    mConsumer->consumerConnect(dw, false);


    sp<Thread> pt(new ProducerThread(mANW));
    pt->run("ProducerThread");

    // eat a frame so GLConsumer will own an at least one slot
    dw->waitForFrame();
    EXPECT_EQ(OK,mST->updateTexImage());

    dw->waitForFrame();
    // Could fail here as GLConsumer thinks it still owns the slot
    // but bufferQueue has released all slots
    EXPECT_EQ(OK,mST->updateTexImage());

    dw->finishDisconnect();
}


// This test ensures that the GLConsumer clears the mCurrentTexture
// when it is disconnected and reconnected.  Otherwise it will
// attempt to release a buffer that it does not owned
TEST_F(SurfaceTextureGLTest, DisconnectClearsCurrentTexture) {
    ASSERT_EQ(OK, native_window_api_connect(mANW.get(),
            NATIVE_WINDOW_API_CPU));

    ANativeWindowBuffer *anb;

    EXPECT_EQ (OK, native_window_dequeue_buffer_and_wait(mANW.get(), &anb));
    EXPECT_EQ(OK, mANW->queueBuffer(mANW.get(), anb, -1));

    EXPECT_EQ (OK, native_window_dequeue_buffer_and_wait(mANW.get(), &anb));
    EXPECT_EQ(OK, mANW->queueBuffer(mANW.get(), anb, -1));

    EXPECT_EQ(OK,mST->updateTexImage());
    EXPECT_EQ(OK,mST->updateTexImage());

    ASSERT_EQ(OK, native_window_api_disconnect(mANW.get(),
            NATIVE_WINDOW_API_CPU));
    ASSERT_EQ(OK, native_window_api_connect(mANW.get(),
            NATIVE_WINDOW_API_CPU));

    EXPECT_EQ(OK, native_window_dequeue_buffer_and_wait(mANW.get(), &anb));
    EXPECT_EQ(OK, mANW->queueBuffer(mANW.get(), anb, -1));

    // Will fail here if mCurrentTexture is not cleared properly
    mFW->waitForFrame();
    EXPECT_EQ(OK,mST->updateTexImage());

    ASSERT_EQ(OK, native_window_api_disconnect(mANW.get(),
            NATIVE_WINDOW_API_CPU));
}

TEST_F(SurfaceTextureGLTest, ScaleToWindowMode) {
    ASSERT_EQ(OK, native_window_set_scaling_mode(mANW.get(),
        NATIVE_WINDOW_SCALING_MODE_SCALE_TO_WINDOW));

    // The producer image size
    ASSERT_EQ(OK, native_window_set_buffers_dimensions(mANW.get(), 512, 512));

    // The consumer image size (16 x 9) ratio
    mST->setDefaultBufferSize(1280, 720);

    ASSERT_EQ(OK, native_window_api_connect(mANW.get(),
            NATIVE_WINDOW_API_CPU));

    ANativeWindowBuffer *anb;

    android_native_rect_t odd = {23, 78, 123, 477};
    ASSERT_EQ(OK, native_window_set_crop(mANW.get(), &odd));
    EXPECT_EQ (OK, native_window_dequeue_buffer_and_wait(mANW.get(), &anb));
    EXPECT_EQ(OK, mANW->queueBuffer(mANW.get(), anb, -1));
    mFW->waitForFrame();
    EXPECT_EQ(OK, mST->updateTexImage());
    Rect r = mST->getCurrentCrop();
    assertRectEq(Rect(23, 78, 123, 477), r);

    ASSERT_EQ(OK, native_window_api_disconnect(mANW.get(),
            NATIVE_WINDOW_API_CPU));
}

// This test ensures the scaling mode does the right thing
// ie NATIVE_WINDOW_SCALING_MODE_CROP should crop
// the image such that it has the same aspect ratio as the
// default buffer size
TEST_F(SurfaceTextureGLTest, CroppedScalingMode) {
    ASSERT_EQ(OK, native_window_set_scaling_mode(mANW.get(),
        NATIVE_WINDOW_SCALING_MODE_SCALE_CROP));

    // The producer image size
    ASSERT_EQ(OK, native_window_set_buffers_dimensions(mANW.get(), 512, 512));

    // The consumer image size (16 x 9) ratio
    mST->setDefaultBufferSize(1280, 720);

    native_window_api_connect(mANW.get(), NATIVE_WINDOW_API_CPU);

    ANativeWindowBuffer *anb;

    // The crop is in the shape of (320, 180) === 16 x 9
    android_native_rect_t standard = {10, 20, 330, 200};
    ASSERT_EQ(OK, native_window_set_crop(mANW.get(), &standard));
    EXPECT_EQ (OK, native_window_dequeue_buffer_and_wait(mANW.get(), &anb));
    EXPECT_EQ(OK, mANW->queueBuffer(mANW.get(), anb, -1));
    mFW->waitForFrame();
    EXPECT_EQ(OK, mST->updateTexImage());
    Rect r = mST->getCurrentCrop();
    // crop should be the same as crop (same aspect ratio)
    assertRectEq(Rect(10, 20, 330, 200), r);

    // make this wider then desired aspect 239 x 100 (2.39:1)
    android_native_rect_t wide = {20, 30, 259, 130};
    ASSERT_EQ(OK, native_window_set_crop(mANW.get(), &wide));
    EXPECT_EQ (OK, native_window_dequeue_buffer_and_wait(mANW.get(), &anb));
    EXPECT_EQ(OK, mANW->queueBuffer(mANW.get(), anb, -1));
    mFW->waitForFrame();
    EXPECT_EQ(OK, mST->updateTexImage());
    r = mST->getCurrentCrop();
    // crop should be the same height, but have cropped left and right borders
    // offset is 30.6 px L+, R-
    assertRectEq(Rect(51, 30, 228, 130), r);

    // This image is taller then desired aspect 400 x 300 (4:3)
    android_native_rect_t narrow = {0, 0, 400, 300};
    ASSERT_EQ(OK, native_window_set_crop(mANW.get(), &narrow));
    EXPECT_EQ (OK, native_window_dequeue_buffer_and_wait(mANW.get(), &anb));
    EXPECT_EQ(OK, mANW->queueBuffer(mANW.get(), anb, -1));
    mFW->waitForFrame();
    EXPECT_EQ(OK, mST->updateTexImage());
    r = mST->getCurrentCrop();
    // crop should be the same width, but have cropped top and bottom borders
    // offset is 37.5 px
    assertRectEq(Rect(0, 37, 400, 262), r);

    native_window_api_disconnect(mANW.get(), NATIVE_WINDOW_API_CPU);
}

TEST_F(SurfaceTextureGLTest, AbandonUnblocksDequeueBuffer) {
    class ProducerThread : public Thread {
    public:
        explicit ProducerThread(const sp<ANativeWindow>& anw):
                mANW(anw),
                mDequeueError(NO_ERROR) {
        }

        virtual ~ProducerThread() {
        }

        virtual bool threadLoop() {
            Mutex::Autolock lock(mMutex);
            ANativeWindowBuffer* anb;

            if (native_window_api_connect(mANW.get(), NATIVE_WINDOW_API_CPU) !=
                    NO_ERROR) {
                return false;
            }

            // Frame 1
            if (native_window_dequeue_buffer_and_wait(mANW.get(),
                    &anb) != NO_ERROR) {
                return false;
            }
            if (anb == nullptr) {
                return false;
            }
            if (mANW->queueBuffer(mANW.get(), anb, -1)
                    != NO_ERROR) {
                return false;
            }

            // Frame 2
            if (native_window_dequeue_buffer_and_wait(mANW.get(),
                    &anb) != NO_ERROR) {
                return false;
            }
            if (anb == nullptr) {
                return false;
            }
            if (mANW->queueBuffer(mANW.get(), anb, -1)
                    != NO_ERROR) {
                return false;
            }

            // Frame 3 - error expected
            mDequeueError = native_window_dequeue_buffer_and_wait(mANW.get(),
                &anb);
            return false;
        }

        status_t getDequeueError() {
            Mutex::Autolock lock(mMutex);
            return mDequeueError;
        }

    private:
        sp<ANativeWindow> mANW;
        status_t mDequeueError;
        Mutex mMutex;
    };

    sp<Thread> pt(new ProducerThread(mANW));
    pt->run("ProducerThread");

    mFW->waitForFrame();
    mFW->waitForFrame();

    // Sleep for 100ms to allow the producer thread's dequeueBuffer call to
    // block waiting for a buffer to become available.
    usleep(100000);

    mST->abandon();

    pt->requestExitAndWait();
    ASSERT_EQ(NO_INIT,
            reinterpret_cast<ProducerThread*>(pt.get())->getDequeueError());
}

TEST_F(SurfaceTextureGLTest, InvalidWidthOrHeightFails) {
    int texHeight = 16;
    ANativeWindowBuffer* anb;

    ASSERT_EQ(NO_ERROR, native_window_api_connect(mANW.get(),
            NATIVE_WINDOW_API_CPU));

    GLint maxTextureSize;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);

    // make sure it works with small textures
    mST->setDefaultBufferSize(16, texHeight);
    EXPECT_EQ(NO_ERROR, native_window_dequeue_buffer_and_wait(mANW.get(),
            &anb));
    EXPECT_EQ(16, anb->width);
    EXPECT_EQ(texHeight, anb->height);
    EXPECT_EQ(NO_ERROR, mANW->queueBuffer(mANW.get(), anb, -1));
    EXPECT_EQ(NO_ERROR, mST->updateTexImage());

    // make sure it works with GL_MAX_TEXTURE_SIZE
    mST->setDefaultBufferSize(maxTextureSize, texHeight);
    EXPECT_EQ(NO_ERROR, native_window_dequeue_buffer_and_wait(mANW.get(),
            &anb));
    EXPECT_EQ(maxTextureSize, anb->width);
    EXPECT_EQ(texHeight, anb->height);
    EXPECT_EQ(NO_ERROR, mANW->queueBuffer(mANW.get(), anb, -1));
    EXPECT_EQ(NO_ERROR, mST->updateTexImage());

    // make sure it fails with GL_MAX_TEXTURE_SIZE+1
    mST->setDefaultBufferSize(maxTextureSize+1, texHeight);
    EXPECT_EQ(NO_ERROR, native_window_dequeue_buffer_and_wait(mANW.get(),
            &anb));
    EXPECT_EQ(maxTextureSize+1, anb->width);
    EXPECT_EQ(texHeight, anb->height);
    EXPECT_EQ(NO_ERROR, mANW->queueBuffer(mANW.get(), anb, -1));
    ASSERT_NE(NO_ERROR, mST->updateTexImage());
}

} // namespace android
