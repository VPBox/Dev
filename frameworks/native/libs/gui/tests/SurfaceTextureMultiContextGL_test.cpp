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

#define LOG_TAG "SurfaceTextureMultiContextGL_test"
//#define LOG_NDEBUG 0

#include "SurfaceTextureMultiContextGL.h"

#include "FillBuffer.h"

#include <GLES/glext.h>

namespace android {

TEST_F(SurfaceTextureMultiContextGLTest, UpdateFromMultipleContextsFails) {
    ASSERT_EQ(OK, native_window_api_connect(mANW.get(), NATIVE_WINDOW_API_CPU));
    ASSERT_NO_FATAL_FAILURE(produceOneRGBA8Frame(mANW));

    // Latch the texture contents on the primary context.
    mFW->waitForFrame();
    ASSERT_EQ(OK, mST->updateTexImage());

    // Attempt to latch the texture on the secondary context.
    ASSERT_TRUE(eglMakeCurrent(mEglDisplay, mEglSurface, mEglSurface,
            mSecondEglContext));
    ASSERT_EQ(EGL_SUCCESS, eglGetError());
    ASSERT_EQ(INVALID_OPERATION, mST->updateTexImage());
}

TEST_F(SurfaceTextureMultiContextGLTest, DetachFromContextSucceeds) {
    ASSERT_EQ(OK, native_window_api_connect(mANW.get(), NATIVE_WINDOW_API_CPU));
    ASSERT_NO_FATAL_FAILURE(produceOneRGBA8Frame(mANW));

    // Latch the texture contents on the primary context.
    mFW->waitForFrame();
    ASSERT_EQ(OK, mST->updateTexImage());

    // Detach from the primary context.
    ASSERT_EQ(OK, mST->detachFromContext());

    // Check that the GL texture was deleted.
    EXPECT_EQ(GL_FALSE, glIsTexture(TEX_ID));
}

TEST_F(SurfaceTextureMultiContextGLTest,
        DetachFromContextSucceedsAfterProducerDisconnect) {
    ASSERT_EQ(OK, native_window_api_connect(mANW.get(), NATIVE_WINDOW_API_CPU));
    ASSERT_NO_FATAL_FAILURE(produceOneRGBA8Frame(mANW));

    // Latch the texture contents on the primary context.
    mFW->waitForFrame();
    ASSERT_EQ(OK, mST->updateTexImage());

    // Detach from the primary context.
    native_window_api_disconnect(mANW.get(), NATIVE_WINDOW_API_CPU);
    ASSERT_EQ(OK, mST->detachFromContext());

    // Check that the GL texture was deleted.
    EXPECT_EQ(GL_FALSE, glIsTexture(TEX_ID));
}

TEST_F(SurfaceTextureMultiContextGLTest, DetachFromContextFailsWhenAbandoned) {
    ASSERT_EQ(OK, native_window_api_connect(mANW.get(), NATIVE_WINDOW_API_CPU));
    ASSERT_NO_FATAL_FAILURE(produceOneRGBA8Frame(mANW));

    // Latch the texture contents on the primary context.
    mFW->waitForFrame();
    ASSERT_EQ(OK, mST->updateTexImage());

    // Attempt to detach from the primary context.
    mST->abandon();
    ASSERT_EQ(NO_INIT, mST->detachFromContext());
}

TEST_F(SurfaceTextureMultiContextGLTest, DetachFromContextFailsWhenDetached) {
    ASSERT_EQ(OK, native_window_api_connect(mANW.get(), NATIVE_WINDOW_API_CPU));
    ASSERT_NO_FATAL_FAILURE(produceOneRGBA8Frame(mANW));

    // Latch the texture contents on the primary context.
    mFW->waitForFrame();
    ASSERT_EQ(OK, mST->updateTexImage());

    // Detach from the primary context.
    ASSERT_EQ(OK, mST->detachFromContext());

    // Attempt to detach from the primary context again.
    ASSERT_EQ(INVALID_OPERATION, mST->detachFromContext());
}

TEST_F(SurfaceTextureMultiContextGLTest, DetachFromContextFailsWithNoDisplay) {
    ASSERT_EQ(OK, native_window_api_connect(mANW.get(), NATIVE_WINDOW_API_CPU));
    ASSERT_NO_FATAL_FAILURE(produceOneRGBA8Frame(mANW));

    // Latch the texture contents on the primary context.
    mFW->waitForFrame();
    ASSERT_EQ(OK, mST->updateTexImage());

    // Make there be no current display.
    ASSERT_TRUE(eglMakeCurrent(mEglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE,
            EGL_NO_CONTEXT));
    ASSERT_EQ(EGL_SUCCESS, eglGetError());

    // Attempt to detach from the primary context.
    ASSERT_EQ(INVALID_OPERATION, mST->detachFromContext());
}

TEST_F(SurfaceTextureMultiContextGLTest, DetachFromContextFailsWithNoContext) {
    ASSERT_EQ(OK, native_window_api_connect(mANW.get(), NATIVE_WINDOW_API_CPU));
    ASSERT_NO_FATAL_FAILURE(produceOneRGBA8Frame(mANW));

    // Latch the texture contents on the primary context.
    mFW->waitForFrame();
    ASSERT_EQ(OK, mST->updateTexImage());

    // Make current context be incorrect.
    ASSERT_TRUE(eglMakeCurrent(mEglDisplay, mEglSurface, mEglSurface,
            mSecondEglContext));
    ASSERT_EQ(EGL_SUCCESS, eglGetError());

    // Attempt to detach from the primary context.
    ASSERT_EQ(INVALID_OPERATION, mST->detachFromContext());
}

TEST_F(SurfaceTextureMultiContextGLTest, UpdateTexImageFailsWhenDetached) {
    ASSERT_EQ(OK, native_window_api_connect(mANW.get(), NATIVE_WINDOW_API_CPU));
    ASSERT_NO_FATAL_FAILURE(produceOneRGBA8Frame(mANW));

    // Detach from the primary context.
    ASSERT_EQ(OK, mST->detachFromContext());

    // Attempt to latch the texture contents on the primary context.
    mFW->waitForFrame();
    ASSERT_EQ(INVALID_OPERATION, mST->updateTexImage());
}

TEST_F(SurfaceTextureMultiContextGLTest, AttachToContextSucceeds) {
    ASSERT_EQ(OK, native_window_api_connect(mANW.get(), NATIVE_WINDOW_API_CPU));
    ASSERT_NO_FATAL_FAILURE(produceOneRGBA8Frame(mANW));

    // Latch the texture contents on the primary context.
    mFW->waitForFrame();
    ASSERT_EQ(OK, mST->updateTexImage());

    // Detach from the primary context.
    ASSERT_EQ(OK, mST->detachFromContext());

    // Attach to the secondary context.
    ASSERT_TRUE(eglMakeCurrent(mEglDisplay, mEglSurface, mEglSurface,
            mSecondEglContext));
    ASSERT_EQ(OK, mST->attachToContext(SECOND_TEX_ID));

    // Verify that the texture object was created and bound.
    GLint texBinding = -1;
    glGetIntegerv(GL_TEXTURE_BINDING_EXTERNAL_OES, &texBinding);
    EXPECT_EQ(SECOND_TEX_ID, texBinding);

    // Try to use the texture from the secondary context.
    glClearColor(0.2, 0.2, 0.2, 0.2);
    glClear(GL_COLOR_BUFFER_BIT);
    glViewport(0, 0, 1, 1);
    mSecondTextureRenderer->drawTexture();
    ASSERT_TRUE(checkPixel( 0,  0,  35,  35,  35,  35));
    ASSERT_EQ(GLenum(GL_NO_ERROR), glGetError());
}

TEST_F(SurfaceTextureMultiContextGLTest,
        AttachToContextSucceedsAfterProducerDisconnect) {
    ASSERT_EQ(OK, native_window_api_connect(mANW.get(), NATIVE_WINDOW_API_CPU));
    ASSERT_NO_FATAL_FAILURE(produceOneRGBA8Frame(mANW));

    // Latch the texture contents on the primary context.
    mFW->waitForFrame();
    ASSERT_EQ(OK, mST->updateTexImage());

    // Detach from the primary context.
    native_window_api_disconnect(mANW.get(), NATIVE_WINDOW_API_CPU);
    ASSERT_EQ(OK, mST->detachFromContext());

    // Attach to the secondary context.
    ASSERT_TRUE(eglMakeCurrent(mEglDisplay, mEglSurface, mEglSurface,
            mSecondEglContext));
    ASSERT_EQ(OK, mST->attachToContext(SECOND_TEX_ID));

    // Verify that the texture object was created and bound.
    GLint texBinding = -1;
    glGetIntegerv(GL_TEXTURE_BINDING_EXTERNAL_OES, &texBinding);
    EXPECT_EQ(SECOND_TEX_ID, texBinding);

    // Try to use the texture from the secondary context.
    glClearColor(0.2, 0.2, 0.2, 0.2);
    glClear(GL_COLOR_BUFFER_BIT);
    glViewport(0, 0, 1, 1);
    mSecondTextureRenderer->drawTexture();
    ASSERT_EQ(GLenum(GL_NO_ERROR), glGetError());
    ASSERT_TRUE(checkPixel( 0,  0,  35,  35,  35,  35));
}

TEST_F(SurfaceTextureMultiContextGLTest,
        AttachToContextSucceedsBeforeUpdateTexImage) {
    ASSERT_EQ(OK, native_window_api_connect(mANW.get(), NATIVE_WINDOW_API_CPU));
    ASSERT_NO_FATAL_FAILURE(produceOneRGBA8Frame(mANW));

    // Detach from the primary context.
    native_window_api_disconnect(mANW.get(), NATIVE_WINDOW_API_CPU);
    ASSERT_EQ(OK, mST->detachFromContext());

    // Attach to the secondary context.
    ASSERT_TRUE(eglMakeCurrent(mEglDisplay, mEglSurface, mEglSurface,
            mSecondEglContext));
    ASSERT_EQ(OK, mST->attachToContext(SECOND_TEX_ID));

    // Verify that the texture object was created and bound.
    GLint texBinding = -1;
    glGetIntegerv(GL_TEXTURE_BINDING_EXTERNAL_OES, &texBinding);
    EXPECT_EQ(SECOND_TEX_ID, texBinding);

    // Latch the texture contents on the primary context.
    mFW->waitForFrame();
    ASSERT_EQ(OK, mST->updateTexImage());

    // Try to use the texture from the secondary context.
    glClearColor(0.2, 0.2, 0.2, 0.2);
    glClear(GL_COLOR_BUFFER_BIT);
    glViewport(0, 0, 1, 1);
    mSecondTextureRenderer->drawTexture();
    ASSERT_EQ(GLenum(GL_NO_ERROR), glGetError());
    ASSERT_TRUE(checkPixel( 0,  0,  35,  35,  35,  35));
}

TEST_F(SurfaceTextureMultiContextGLTest, AttachToContextFailsWhenAbandoned) {
    ASSERT_EQ(OK, native_window_api_connect(mANW.get(), NATIVE_WINDOW_API_CPU));
    ASSERT_NO_FATAL_FAILURE(produceOneRGBA8Frame(mANW));

    // Latch the texture contents on the primary context.
    mFW->waitForFrame();
    ASSERT_EQ(OK, mST->updateTexImage());

    // Detach from the primary context.
    ASSERT_EQ(OK, mST->detachFromContext());

    // Attempt to attach to the secondary context.
    mST->abandon();

    // Attempt to attach to the primary context.
    ASSERT_EQ(NO_INIT, mST->attachToContext(SECOND_TEX_ID));
}

TEST_F(SurfaceTextureMultiContextGLTest, AttachToContextFailsWhenAttached) {
    ASSERT_EQ(OK, native_window_api_connect(mANW.get(), NATIVE_WINDOW_API_CPU));
    ASSERT_NO_FATAL_FAILURE(produceOneRGBA8Frame(mANW));

    // Latch the texture contents on the primary context.
    mFW->waitForFrame();
    ASSERT_EQ(OK, mST->updateTexImage());

    // Attempt to attach to the primary context.
    ASSERT_EQ(INVALID_OPERATION, mST->attachToContext(SECOND_TEX_ID));
}

TEST_F(SurfaceTextureMultiContextGLTest,
        AttachToContextFailsWhenAttachedBeforeUpdateTexImage) {
    ASSERT_EQ(OK, native_window_api_connect(mANW.get(), NATIVE_WINDOW_API_CPU));
    ASSERT_NO_FATAL_FAILURE(produceOneRGBA8Frame(mANW));

    // Attempt to attach to the primary context.
    ASSERT_EQ(INVALID_OPERATION, mST->attachToContext(SECOND_TEX_ID));
}

TEST_F(SurfaceTextureMultiContextGLTest, AttachToContextFailsWithNoDisplay) {
    ASSERT_EQ(OK, native_window_api_connect(mANW.get(), NATIVE_WINDOW_API_CPU));
    ASSERT_NO_FATAL_FAILURE(produceOneRGBA8Frame(mANW));

    // Latch the texture contents on the primary context.
    mFW->waitForFrame();
    ASSERT_EQ(OK, mST->updateTexImage());

    // Detach from the primary context.
    ASSERT_EQ(OK, mST->detachFromContext());

    // Make there be no current display.
    ASSERT_TRUE(eglMakeCurrent(mEglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE,
            EGL_NO_CONTEXT));
    ASSERT_EQ(EGL_SUCCESS, eglGetError());

    // Attempt to attach with no context current.
    ASSERT_EQ(INVALID_OPERATION, mST->attachToContext(SECOND_TEX_ID));
}

TEST_F(SurfaceTextureMultiContextGLTest, AttachToContextSucceedsTwice) {
    ASSERT_EQ(OK, native_window_api_connect(mANW.get(), NATIVE_WINDOW_API_CPU));
    ASSERT_NO_FATAL_FAILURE(produceOneRGBA8Frame(mANW));

    // Latch the texture contents on the primary context.
    mFW->waitForFrame();
    ASSERT_EQ(OK, mST->updateTexImage());

    // Detach from the primary context.
    ASSERT_EQ(OK, mST->detachFromContext());

    // Attach to the secondary context.
    ASSERT_TRUE(eglMakeCurrent(mEglDisplay, mEglSurface, mEglSurface,
            mSecondEglContext));
    ASSERT_EQ(OK, mST->attachToContext(SECOND_TEX_ID));

    // Detach from the secondary context.
    ASSERT_EQ(OK, mST->detachFromContext());

    // Attach to the tertiary context.
    ASSERT_TRUE(eglMakeCurrent(mEglDisplay, mEglSurface, mEglSurface,
            mThirdEglContext));
    ASSERT_EQ(OK, mST->attachToContext(THIRD_TEX_ID));

    // Verify that the texture object was created and bound.
    GLint texBinding = -1;
    glGetIntegerv(GL_TEXTURE_BINDING_EXTERNAL_OES, &texBinding);
    EXPECT_EQ(THIRD_TEX_ID, texBinding);

    // Try to use the texture from the tertiary context.
    glClearColor(0.2, 0.2, 0.2, 0.2);
    glClear(GL_COLOR_BUFFER_BIT);
    glViewport(0, 0, 1, 1);
    mThirdTextureRenderer->drawTexture();
    ASSERT_EQ(GLenum(GL_NO_ERROR), glGetError());
    ASSERT_TRUE(checkPixel( 0,  0,  35,  35,  35,  35));
}

TEST_F(SurfaceTextureMultiContextGLTest,
        AttachToContextSucceedsTwiceBeforeUpdateTexImage) {
    ASSERT_EQ(OK, native_window_api_connect(mANW.get(), NATIVE_WINDOW_API_CPU));
    ASSERT_NO_FATAL_FAILURE(produceOneRGBA8Frame(mANW));

    // Detach from the primary context.
    ASSERT_EQ(OK, mST->detachFromContext());

    // Attach to the secondary context.
    ASSERT_TRUE(eglMakeCurrent(mEglDisplay, mEglSurface, mEglSurface,
            mSecondEglContext));
    ASSERT_EQ(OK, mST->attachToContext(SECOND_TEX_ID));

    // Detach from the secondary context.
    ASSERT_EQ(OK, mST->detachFromContext());

    // Attach to the tertiary context.
    ASSERT_TRUE(eglMakeCurrent(mEglDisplay, mEglSurface, mEglSurface,
            mThirdEglContext));
    ASSERT_EQ(OK, mST->attachToContext(THIRD_TEX_ID));

    // Verify that the texture object was created and bound.
    GLint texBinding = -1;
    glGetIntegerv(GL_TEXTURE_BINDING_EXTERNAL_OES, &texBinding);
    EXPECT_EQ(THIRD_TEX_ID, texBinding);

    // Latch the texture contents on the tertiary context.
    mFW->waitForFrame();
    ASSERT_EQ(OK, mST->updateTexImage());

    // Try to use the texture from the tertiary context.
    glClearColor(0.2, 0.2, 0.2, 0.2);
    glClear(GL_COLOR_BUFFER_BIT);
    glViewport(0, 0, 1, 1);
    mThirdTextureRenderer->drawTexture();
    ASSERT_EQ(GLenum(GL_NO_ERROR), glGetError());
    ASSERT_TRUE(checkPixel( 0,  0,  35,  35,  35,  35));
}

TEST_F(SurfaceTextureMultiContextGLTest,
        UpdateTexImageSucceedsForBufferConsumedBeforeDetach) {
    ASSERT_EQ(OK, native_window_api_connect(mANW.get(), NATIVE_WINDOW_API_CPU));

    // produce two frames and consume them both on the primary context
    ASSERT_NO_FATAL_FAILURE(produceOneRGBA8Frame(mANW));
    mFW->waitForFrame();
    ASSERT_EQ(OK, mST->updateTexImage());

    ASSERT_NO_FATAL_FAILURE(produceOneRGBA8Frame(mANW));
    mFW->waitForFrame();
    ASSERT_EQ(OK, mST->updateTexImage());

    // produce one more frame
    ASSERT_NO_FATAL_FAILURE(produceOneRGBA8Frame(mANW));

    // Detach from the primary context and attach to the secondary context
    ASSERT_EQ(OK, mST->detachFromContext());
    ASSERT_TRUE(eglMakeCurrent(mEglDisplay, mEglSurface, mEglSurface,
            mSecondEglContext));
    ASSERT_EQ(OK, mST->attachToContext(SECOND_TEX_ID));

    // Consume final frame on secondary context
    mFW->waitForFrame();
    ASSERT_EQ(OK, mST->updateTexImage());
}

TEST_F(SurfaceTextureMultiContextGLTest,
       AttachAfterDisplayTerminatedSucceeds) {
    ASSERT_EQ(OK, native_window_api_connect(mANW.get(), NATIVE_WINDOW_API_CPU));

    // produce two frames and consume them both on the primary context
    ASSERT_NO_FATAL_FAILURE(produceOneRGBA8Frame(mANW));
    mFW->waitForFrame();
    ASSERT_EQ(OK, mST->updateTexImage());

    ASSERT_NO_FATAL_FAILURE(produceOneRGBA8Frame(mANW));
    mFW->waitForFrame();
    ASSERT_EQ(OK, mST->updateTexImage());

    // produce one more frame
    ASSERT_NO_FATAL_FAILURE(produceOneRGBA8Frame(mANW));

    // Detach from the primary context.
    ASSERT_EQ(OK, mST->releaseTexImage());
    ASSERT_EQ(OK, mST->detachFromContext());

    // Terminate and then initialize the display. All contexts, surfaces
    // and images are invalid at this point.
    EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    ASSERT_NE(EGL_NO_DISPLAY, mEglDisplay);
    EGLint majorVersion = 0;
    EGLint minorVersion = 0;
    EXPECT_TRUE(eglTerminate(display));
    EXPECT_TRUE(eglInitialize(mEglDisplay, &majorVersion, &minorVersion));
    ASSERT_EQ(EGL_SUCCESS, eglGetError());

    // The surface is invalid so create it again.
    EGLint pbufferAttribs[] = {
        EGL_WIDTH, 64,
        EGL_HEIGHT, 64,
        EGL_NONE };
    mEglSurface = eglCreatePbufferSurface(mEglDisplay, mGlConfig,
            pbufferAttribs);

    // The second context is invalid so create it again.
    mSecondEglContext = eglCreateContext(mEglDisplay, mGlConfig,
            EGL_NO_CONTEXT, getContextAttribs());
    ASSERT_EQ(EGL_SUCCESS, eglGetError());
    ASSERT_NE(EGL_NO_CONTEXT, mSecondEglContext);

    ASSERT_TRUE(eglMakeCurrent(mEglDisplay, mEglSurface, mEglSurface,
            mSecondEglContext));
    ASSERT_EQ(EGL_SUCCESS, eglGetError());

    // Now attach to and consume final frame on secondary context.
    ASSERT_EQ(OK, mST->attachToContext(SECOND_TEX_ID));
    mFW->waitForFrame();
    ASSERT_EQ(OK, mST->updateTexImage());
}


} // namespace android
