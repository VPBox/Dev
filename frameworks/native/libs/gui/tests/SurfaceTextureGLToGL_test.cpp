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

#define LOG_TAG "SurfaceTextureGLToGL_test"
//#define LOG_NDEBUG 0

#include "SurfaceTextureGLToGL.h"

namespace android {

TEST_F(SurfaceTextureGLToGLTest, TransformHintGetsRespected) {
    const uint32_t texWidth = 32;
    const uint32_t texHeight = 64;

    mST->setDefaultBufferSize(texWidth, texHeight);
    mST->setTransformHint(NATIVE_WINDOW_TRANSFORM_ROT_90);

    // This test requires 3 buffers to avoid deadlock because we're
    // both producer and consumer, and only using one thread. Set max dequeued
    // to 2, and max acquired already defaults to 1.
    ASSERT_EQ(OK, mSTC->setMaxDequeuedBufferCount(2));

    SetUpWindowAndContext();

    // Do the producer side of things
    EXPECT_TRUE(eglMakeCurrent(mEglDisplay, mProducerEglSurface,
            mProducerEglSurface, mProducerEglContext));
    ASSERT_EQ(EGL_SUCCESS, eglGetError());

    // Start a buffer with our chosen size and transform hint moving
    // through the system.
    glClear(GL_COLOR_BUFFER_BIT);  // give the driver something to do
    eglSwapBuffers(mEglDisplay, mProducerEglSurface);
    mST->updateTexImage();  // consume it
    // Swap again.
    glClear(GL_COLOR_BUFFER_BIT);
    eglSwapBuffers(mEglDisplay, mProducerEglSurface);
    mST->updateTexImage();

    // The current buffer should either show the effects of the transform
    // hint (in the form of an inverse transform), or show that the
    // transform hint has been ignored.
    sp<GraphicBuffer> buf = mST->getCurrentBuffer();
    if (mST->getCurrentTransform() == NATIVE_WINDOW_TRANSFORM_ROT_270) {
        ASSERT_EQ(texWidth, buf->getHeight());
        ASSERT_EQ(texHeight, buf->getWidth());
    } else {
        ASSERT_EQ(texWidth, buf->getWidth());
        ASSERT_EQ(texHeight, buf->getHeight());
    }

    // Reset the transform hint and confirm that it takes.
    mST->setTransformHint(0);
    glClear(GL_COLOR_BUFFER_BIT);
    eglSwapBuffers(mEglDisplay, mProducerEglSurface);
    mST->updateTexImage();
    glClear(GL_COLOR_BUFFER_BIT);
    eglSwapBuffers(mEglDisplay, mProducerEglSurface);
    mST->updateTexImage();

    buf = mST->getCurrentBuffer();
    ASSERT_EQ((uint32_t) 0, mST->getCurrentTransform());
    ASSERT_EQ(texWidth, buf->getWidth());
    ASSERT_EQ(texHeight, buf->getHeight());
}

TEST_F(SurfaceTextureGLToGLTest, TexturingFromGLFilledRGBABufferPow2) {
    const int texWidth = 64;
    const int texHeight = 64;

    mST->setDefaultBufferSize(texWidth, texHeight);

    // This test requires 3 buffers to complete run on a single thread.
    // Set max dequeued to 2, and max acquired already defaults to 1.
    ASSERT_EQ(OK, mSTC->setMaxDequeuedBufferCount(2));

    SetUpWindowAndContext();

    // Do the producer side of things
    EXPECT_TRUE(eglMakeCurrent(mEglDisplay, mProducerEglSurface,
            mProducerEglSurface, mProducerEglContext));
    ASSERT_EQ(EGL_SUCCESS, eglGetError());

    // This is needed to ensure we pick up a buffer of the correct size.
    eglSwapBuffers(mEglDisplay, mProducerEglSurface);

    glClearColor(0.6, 0.6, 0.6, 0.6);
    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_SCISSOR_TEST);
    glScissor(4, 4, 4, 4);
    glClearColor(1.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glScissor(24, 48, 4, 4);
    glClearColor(0.0, 1.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glScissor(37, 17, 4, 4);
    glClearColor(0.0, 0.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    eglSwapBuffers(mEglDisplay, mProducerEglSurface);

    // Do the consumer side of things
    EXPECT_TRUE(eglMakeCurrent(mEglDisplay, mEglSurface, mEglSurface,
            mEglContext));
    ASSERT_EQ(EGL_SUCCESS, eglGetError());

    glDisable(GL_SCISSOR_TEST);

    // Skip the first frame, which was empty
    ASSERT_EQ(NO_ERROR, mST->updateTexImage());
    ASSERT_EQ(NO_ERROR, mST->updateTexImage());

    glClearColor(0.2, 0.2, 0.2, 0.2);
    glClear(GL_COLOR_BUFFER_BIT);

    glViewport(0, 0, texWidth, texHeight);
    drawTexture();

    EXPECT_TRUE(checkPixel( 0,  0, 153, 153, 153, 153));
    EXPECT_TRUE(checkPixel(63,  0, 153, 153, 153, 153));
    EXPECT_TRUE(checkPixel(63, 63, 153, 153, 153, 153));
    EXPECT_TRUE(checkPixel( 0, 63, 153, 153, 153, 153));

    EXPECT_TRUE(checkPixel( 4,  7, 255,   0,   0, 255));
    EXPECT_TRUE(checkPixel(25, 51,   0, 255,   0, 255));
    EXPECT_TRUE(checkPixel(40, 19,   0,   0, 255, 255));
    EXPECT_TRUE(checkPixel(29, 51, 153, 153, 153, 153));
    EXPECT_TRUE(checkPixel( 5, 32, 153, 153, 153, 153));
    EXPECT_TRUE(checkPixel(13,  8, 153, 153, 153, 153));
    EXPECT_TRUE(checkPixel(46,  3, 153, 153, 153, 153));
    EXPECT_TRUE(checkPixel(30, 33, 153, 153, 153, 153));
    EXPECT_TRUE(checkPixel( 6, 52, 153, 153, 153, 153));
    EXPECT_TRUE(checkPixel(55, 33, 153, 153, 153, 153));
    EXPECT_TRUE(checkPixel(16, 29, 153, 153, 153, 153));
    EXPECT_TRUE(checkPixel( 1, 30, 153, 153, 153, 153));
    EXPECT_TRUE(checkPixel(41, 37, 153, 153, 153, 153));
    EXPECT_TRUE(checkPixel(46, 29, 153, 153, 153, 153));
    EXPECT_TRUE(checkPixel(15, 25, 153, 153, 153, 153));
    EXPECT_TRUE(checkPixel( 3, 52, 153, 153, 153, 153));
}

TEST_F(SurfaceTextureGLToGLTest, EglDestroySurfaceUnrefsBuffers) {
    SetUpWindowAndContext();
    sp<GraphicBuffer> buffers[2];

    // This test requires async mode to run on a single thread.
    EXPECT_TRUE(eglMakeCurrent(mEglDisplay, mProducerEglSurface,
            mProducerEglSurface, mProducerEglContext));
    ASSERT_EQ(EGL_SUCCESS, eglGetError());
    EXPECT_TRUE(eglSwapInterval(mEglDisplay, 0));
    ASSERT_EQ(EGL_SUCCESS, eglGetError());

    for (int i = 0; i < 2; i++) {
        // Produce a frame
        EXPECT_TRUE(eglMakeCurrent(mEglDisplay, mProducerEglSurface,
                mProducerEglSurface, mProducerEglContext));
        ASSERT_EQ(EGL_SUCCESS, eglGetError());
        glClear(GL_COLOR_BUFFER_BIT);
        eglSwapBuffers(mEglDisplay, mProducerEglSurface);

        // Consume a frame
        EXPECT_TRUE(eglMakeCurrent(mEglDisplay, mEglSurface, mEglSurface,
                mEglContext));
        ASSERT_EQ(EGL_SUCCESS, eglGetError());
        mFW->waitForFrame();
        ASSERT_EQ(NO_ERROR, mST->updateTexImage());
        buffers[i] = mST->getCurrentBuffer();
    }

    // Destroy the GL texture object to release its ref on buffers[2].
    GLuint texID = TEX_ID;
    glDeleteTextures(1, &texID);

    // Destroy the EGLSurface
    EXPECT_TRUE(eglDestroySurface(mEglDisplay, mProducerEglSurface));
    ASSERT_EQ(EGL_SUCCESS, eglGetError());
    mProducerEglSurface = EGL_NO_SURFACE;

    // This test should have the only reference to buffer 0.
    EXPECT_EQ(1, buffers[0]->getStrongCount());

    // The GLConsumer should hold one reference to buffer 1 in its
    // mCurrentTextureImage member and another reference in mEglSlots. The third
    // reference is in this test.
    EXPECT_EQ(3, buffers[1]->getStrongCount());
}

TEST_F(SurfaceTextureGLToGLTest, EglDestroySurfaceAfterAbandonUnrefsBuffers) {
    SetUpWindowAndContext();
    sp<GraphicBuffer> buffers[3];

    // This test requires async mode to run on a single thread.
    EXPECT_TRUE(eglMakeCurrent(mEglDisplay, mProducerEglSurface,
            mProducerEglSurface, mProducerEglContext));
    ASSERT_EQ(EGL_SUCCESS, eglGetError());
    EXPECT_TRUE(eglSwapInterval(mEglDisplay, 0));
    ASSERT_EQ(EGL_SUCCESS, eglGetError());

    for (int i = 0; i < 3; i++) {
        // Produce a frame
        EXPECT_TRUE(eglMakeCurrent(mEglDisplay, mProducerEglSurface,
                mProducerEglSurface, mProducerEglContext));
        ASSERT_EQ(EGL_SUCCESS, eglGetError());
        glClear(GL_COLOR_BUFFER_BIT);
        EXPECT_TRUE(eglSwapBuffers(mEglDisplay, mProducerEglSurface));
        ASSERT_EQ(EGL_SUCCESS, eglGetError());

        // Consume a frame
        EXPECT_TRUE(eglMakeCurrent(mEglDisplay, mEglSurface, mEglSurface,
                mEglContext));
        ASSERT_EQ(EGL_SUCCESS, eglGetError());
        mFW->waitForFrame();
        ASSERT_EQ(NO_ERROR, mST->updateTexImage());
        buffers[i] = mST->getCurrentBuffer();
    }

    // Abandon the GLConsumer, releasing the ref that the GLConsumer has
    // on buffers[2].
    mST->abandon();

    // Destroy the GL texture object to release its ref on buffers[2].
    GLuint texID = TEX_ID;
    glDeleteTextures(1, &texID);

    // Destroy the EGLSurface.
    EXPECT_TRUE(eglDestroySurface(mEglDisplay, mProducerEglSurface));
    ASSERT_EQ(EGL_SUCCESS, eglGetError());
    mProducerEglSurface = EGL_NO_SURFACE;

    EXPECT_EQ(1, buffers[1]->getStrongCount());

    // Depending on how lazily the GL driver dequeues buffers, we may end up
    // with either two or three total buffers.  If there are three, each entry
    // of the buffers array will be unique and there should only be one
    // reference (the one in this test). If there are two the first and last
    // element in the array will be equal meaning that buffer representing both
    // 0 and 2 will have two references (one for 0 and one for 2).
    if (buffers[2] != buffers[0]) {
        EXPECT_EQ(1, buffers[0]->getStrongCount());
        EXPECT_EQ(1, buffers[2]->getStrongCount());
    } else {
        EXPECT_EQ(2, buffers[0]->getStrongCount());
    }
}

TEST_F(SurfaceTextureGLToGLTest, EglMakeCurrentBeforeConsumerDeathUnrefsBuffers) {
    SetUpWindowAndContext();
    sp<GraphicBuffer> buffer;

    EXPECT_TRUE(eglMakeCurrent(mEglDisplay, mProducerEglSurface,
            mProducerEglSurface, mProducerEglContext));

    // Produce a frame
    glClear(GL_COLOR_BUFFER_BIT);
    EXPECT_TRUE(eglSwapBuffers(mEglDisplay, mProducerEglSurface));
    ASSERT_EQ(EGL_SUCCESS, eglGetError());

    // Destroy the EGLSurface.
    EXPECT_TRUE(eglDestroySurface(mEglDisplay, mProducerEglSurface));
    ASSERT_EQ(EGL_SUCCESS, eglGetError());
    mProducerEglSurface = EGL_NO_SURFACE;
    mSTC.clear();
    mANW.clear();
    mTextureRenderer.clear();

    // Consume a frame
    ASSERT_EQ(NO_ERROR, mST->updateTexImage());
    buffer = mST->getCurrentBuffer();

    // Destroy the GL texture object to release its ref
    GLuint texID = TEX_ID;
    glDeleteTextures(1, &texID);

    // make un-current, all references to buffer should be gone
    EXPECT_TRUE(eglMakeCurrent(mEglDisplay, EGL_NO_SURFACE,
            EGL_NO_SURFACE, EGL_NO_CONTEXT));

    // Destroy consumer
    mST.clear();

    EXPECT_EQ(1, buffer->getStrongCount());
}

TEST_F(SurfaceTextureGLToGLTest, EglMakeCurrentAfterConsumerDeathUnrefsBuffers) {
    SetUpWindowAndContext();
    sp<GraphicBuffer> buffer;

    EXPECT_TRUE(eglMakeCurrent(mEglDisplay, mProducerEglSurface,
            mProducerEglSurface, mProducerEglContext));

    // Produce a frame
    glClear(GL_COLOR_BUFFER_BIT);
    EXPECT_TRUE(eglSwapBuffers(mEglDisplay, mProducerEglSurface));
    ASSERT_EQ(EGL_SUCCESS, eglGetError());

    // Destroy the EGLSurface.
    EXPECT_TRUE(eglDestroySurface(mEglDisplay, mProducerEglSurface));
    ASSERT_EQ(EGL_SUCCESS, eglGetError());
    mProducerEglSurface = EGL_NO_SURFACE;
    mSTC.clear();
    mANW.clear();
    mTextureRenderer.clear();

    // Consume a frame
    ASSERT_EQ(NO_ERROR, mST->updateTexImage());
    buffer = mST->getCurrentBuffer();

    // Destroy the GL texture object to release its ref
    GLuint texID = TEX_ID;
    glDeleteTextures(1, &texID);

    // Destroy consumer
    mST.clear();

    // make un-current, all references to buffer should be gone
    EXPECT_TRUE(eglMakeCurrent(mEglDisplay, EGL_NO_SURFACE,
            EGL_NO_SURFACE, EGL_NO_CONTEXT));

    EXPECT_EQ(1, buffer->getStrongCount());
}

TEST_F(SurfaceTextureGLToGLTest, TexturingFromUserSizedGLFilledBuffer) {
    enum { texWidth = 64 };
    enum { texHeight = 64 };

    // This test requires 3 buffers to complete run on a single thread.
    // Set max dequeued to 2, and max acquired already defaults to 1.
    ASSERT_EQ(OK, mSTC->setMaxDequeuedBufferCount(2));

    SetUpWindowAndContext();

    // Set the user buffer size.
    native_window_set_buffers_user_dimensions(mANW.get(), texWidth, texHeight);

    // Do the producer side of things
    EXPECT_TRUE(eglMakeCurrent(mEglDisplay, mProducerEglSurface,
            mProducerEglSurface, mProducerEglContext));
    ASSERT_EQ(EGL_SUCCESS, eglGetError());

    // This is needed to ensure we pick up a buffer of the correct size.
    eglSwapBuffers(mEglDisplay, mProducerEglSurface);

    glClearColor(0.6, 0.6, 0.6, 0.6);
    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_SCISSOR_TEST);
    glScissor(4, 4, 1, 1);
    glClearColor(1.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    eglSwapBuffers(mEglDisplay, mProducerEglSurface);

    // Do the consumer side of things
    EXPECT_TRUE(eglMakeCurrent(mEglDisplay, mEglSurface, mEglSurface,
            mEglContext));
    ASSERT_EQ(EGL_SUCCESS, eglGetError());

    glDisable(GL_SCISSOR_TEST);

    // Skip the first frame, which was empty
    ASSERT_EQ(NO_ERROR, mST->updateTexImage());
    ASSERT_EQ(NO_ERROR, mST->updateTexImage());

    glClearColor(0.2, 0.2, 0.2, 0.2);
    glClear(GL_COLOR_BUFFER_BIT);

    glViewport(0, 0, texWidth, texHeight);
    drawTexture();

    EXPECT_TRUE(checkPixel( 0,  0, 153, 153, 153, 153));
    EXPECT_TRUE(checkPixel(63,  0, 153, 153, 153, 153));
    EXPECT_TRUE(checkPixel(63, 63, 153, 153, 153, 153));
    EXPECT_TRUE(checkPixel( 0, 63, 153, 153, 153, 153));

    EXPECT_TRUE(checkPixel( 4,  4, 255,   0,   0, 255));
    EXPECT_TRUE(checkPixel( 5,  5, 153, 153, 153, 153));
    EXPECT_TRUE(checkPixel( 3,  3, 153, 153, 153, 153));
    EXPECT_TRUE(checkPixel(45, 52, 153, 153, 153, 153));
    EXPECT_TRUE(checkPixel(12, 36, 153, 153, 153, 153));
}

TEST_F(SurfaceTextureGLToGLTest, TexturingFromPreRotatedUserSizedGLFilledBuffer) {
    enum { texWidth = 64 };
    enum { texHeight = 16 };

    // This test requires 3 buffers to complete run on a single thread.
    // Set max dequeued to 2, and max acquired already defaults to 1.
    ASSERT_EQ(OK, mSTC->setMaxDequeuedBufferCount(2));

    SetUpWindowAndContext();

    // Set the transform hint.
    mST->setTransformHint(NATIVE_WINDOW_TRANSFORM_ROT_90);

    // Set the user buffer size.
    native_window_set_buffers_user_dimensions(mANW.get(), texWidth, texHeight);

    // Do the producer side of things
    EXPECT_TRUE(eglMakeCurrent(mEglDisplay, mProducerEglSurface,
            mProducerEglSurface, mProducerEglContext));
    ASSERT_EQ(EGL_SUCCESS, eglGetError());

    // This is needed to ensure we pick up a buffer of the correct size and the
    // new rotation hint.
    eglSwapBuffers(mEglDisplay, mProducerEglSurface);

    glClearColor(0.6, 0.6, 0.6, 0.6);
    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_SCISSOR_TEST);
    glScissor(24, 4, 1, 1);
    glClearColor(1.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    eglSwapBuffers(mEglDisplay, mProducerEglSurface);

    // Do the consumer side of things
    EXPECT_TRUE(eglMakeCurrent(mEglDisplay, mEglSurface, mEglSurface,
            mEglContext));
    ASSERT_EQ(EGL_SUCCESS, eglGetError());

    glDisable(GL_SCISSOR_TEST);

    // Skip the first frame, which was empty
    ASSERT_EQ(NO_ERROR, mST->updateTexImage());
    ASSERT_EQ(NO_ERROR, mST->updateTexImage());

    glClearColor(0.2, 0.2, 0.2, 0.2);
    glClear(GL_COLOR_BUFFER_BIT);

    glViewport(0, 0, texWidth, texHeight);
    drawTexture();

    EXPECT_TRUE(checkPixel( 0,  0, 153, 153, 153, 153));
    EXPECT_TRUE(checkPixel(63,  0, 153, 153, 153, 153));
    EXPECT_TRUE(checkPixel(63, 15, 153, 153, 153, 153));
    EXPECT_TRUE(checkPixel( 0, 15, 153, 153, 153, 153));

    EXPECT_TRUE(checkPixel(24,  4, 255,   0,   0, 255));
    EXPECT_TRUE(checkPixel(25,  5, 153, 153, 153, 153));
    EXPECT_TRUE(checkPixel(23,  3, 153, 153, 153, 153));
    EXPECT_TRUE(checkPixel(45, 13, 153, 153, 153, 153));
    EXPECT_TRUE(checkPixel(12,  8, 153, 153, 153, 153));
}

TEST_F(SurfaceTextureGLToGLTest, TexturingFromPreRotatedGLFilledBuffer) {
    enum { texWidth = 64 };
    enum { texHeight = 16 };

    // This test requires 3 buffers to complete run on a single thread.
    // Set max dequeued to 2, and max acquired already defaults to 1.
    ASSERT_EQ(OK, mSTC->setMaxDequeuedBufferCount(2));

    SetUpWindowAndContext();

    // Set the transform hint.
    mST->setTransformHint(NATIVE_WINDOW_TRANSFORM_ROT_90);

    // Set the default buffer size.
    mST->setDefaultBufferSize(texWidth, texHeight);

    // Do the producer side of things
    EXPECT_TRUE(eglMakeCurrent(mEglDisplay, mProducerEglSurface,
            mProducerEglSurface, mProducerEglContext));
    ASSERT_EQ(EGL_SUCCESS, eglGetError());

    // This is needed to ensure we pick up a buffer of the correct size and the
    // new rotation hint.
    eglSwapBuffers(mEglDisplay, mProducerEglSurface);

    glClearColor(0.6, 0.6, 0.6, 0.6);
    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_SCISSOR_TEST);
    glScissor(24, 4, 1, 1);
    glClearColor(1.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    eglSwapBuffers(mEglDisplay, mProducerEglSurface);

    // Do the consumer side of things
    EXPECT_TRUE(eglMakeCurrent(mEglDisplay, mEglSurface, mEglSurface,
            mEglContext));
    ASSERT_EQ(EGL_SUCCESS, eglGetError());

    glDisable(GL_SCISSOR_TEST);

    // Skip the first frame, which was empty
    ASSERT_EQ(NO_ERROR, mST->updateTexImage());
    ASSERT_EQ(NO_ERROR, mST->updateTexImage());

    glClearColor(0.2, 0.2, 0.2, 0.2);
    glClear(GL_COLOR_BUFFER_BIT);

    glViewport(0, 0, texWidth, texHeight);
    drawTexture();

    EXPECT_TRUE(checkPixel( 0,  0, 153, 153, 153, 153));
    EXPECT_TRUE(checkPixel(63,  0, 153, 153, 153, 153));
    EXPECT_TRUE(checkPixel(63, 15, 153, 153, 153, 153));
    EXPECT_TRUE(checkPixel( 0, 15, 153, 153, 153, 153));

    EXPECT_TRUE(checkPixel(24,  4, 255,   0,   0, 255));
    EXPECT_TRUE(checkPixel(25,  5, 153, 153, 153, 153));
    EXPECT_TRUE(checkPixel(23,  3, 153, 153, 153, 153));
    EXPECT_TRUE(checkPixel(45, 13, 153, 153, 153, 153));
    EXPECT_TRUE(checkPixel(12,  8, 153, 153, 153, 153));
}

} // namespace android
