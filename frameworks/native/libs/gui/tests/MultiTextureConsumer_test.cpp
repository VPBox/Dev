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

#define LOG_TAG "MultiTextureConsumer_test"
//#define LOG_NDEBUG 0

#include "GLTest.h"

#include <gui/GLConsumer.h>
#include <gui/Surface.h>

#include <android/native_window.h>

#include <GLES/glext.h>

namespace android {

class MultiTextureConsumerTest : public GLTest {
protected:
    enum { TEX_ID = 123 };

    virtual void SetUp() {
        GLTest::SetUp();
        sp<IGraphicBufferProducer> producer;
        sp<IGraphicBufferConsumer> consumer;
        BufferQueue::createBufferQueue(&producer, &consumer);
        mGlConsumer = new GLConsumer(consumer, TEX_ID,
                GLConsumer::TEXTURE_EXTERNAL, true, false);
        mSurface = new Surface(producer);
        mANW = mSurface.get();

    }
    virtual void TearDown() {
        GLTest::TearDown();
    }
    virtual EGLint const* getContextAttribs() {
        return nullptr;
    }
    virtual EGLint const* getConfigAttribs() {
        static EGLint sDefaultConfigAttribs[] = {
            EGL_SURFACE_TYPE, EGL_PBUFFER_BIT,
            EGL_RED_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_BLUE_SIZE, 8,
            EGL_ALPHA_SIZE, 8,
            EGL_NONE };

        return sDefaultConfigAttribs;
    }
    sp<GLConsumer> mGlConsumer;
    sp<Surface> mSurface;
    ANativeWindow* mANW;
};

TEST_F(MultiTextureConsumerTest, EGLImageTargetWorks) {
    ANativeWindow_Buffer buffer;

    ASSERT_EQ(native_window_set_usage(mANW, GRALLOC_USAGE_SW_WRITE_OFTEN), NO_ERROR);
    ASSERT_EQ(native_window_set_buffers_format(mANW, HAL_PIXEL_FORMAT_RGBA_8888), NO_ERROR);

    glShadeModel(GL_FLAT);
    glDisable(GL_DITHER);
    glDisable(GL_CULL_FACE);
    glViewport(0, 0, getSurfaceWidth(), getSurfaceHeight());
    glOrthof(0, getSurfaceWidth(), 0, getSurfaceHeight(), 0, 1);
    glEnableClientState(GL_VERTEX_ARRAY);
    glColor4f(1, 1, 1, 1);

    glBindTexture(GL_TEXTURE_EXTERNAL_OES, TEX_ID);
    glTexParameterx(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterx(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameterx(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterx(GL_TEXTURE_EXTERNAL_OES, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    uint32_t texel = 0x80808080;
    glBindTexture(GL_TEXTURE_2D, TEX_ID+1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &texel);
    glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, TEX_ID+1);
    glEnable(GL_TEXTURE_2D);
    glTexEnvx(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_EXTERNAL_OES, TEX_ID);
    glEnable(GL_TEXTURE_EXTERNAL_OES);
    glTexEnvx(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glClear(GL_COLOR_BUFFER_BIT);
    for (int i=0 ; i<8 ; i++) {
        mSurface->lock(&buffer, nullptr);
        memset(buffer.bits, (i&7) * 0x20, buffer.stride * buffer.height * 4);
        mSurface->unlockAndPost();

        mGlConsumer->updateTexImage();

        GLfloat vertices[][2] = { {i*16.0f, 0}, {(i+1)*16.0f, 0}, {(i+1)*16.0f, 16.0f}, {i*16.0f, 16.0f} };
        glVertexPointer(2, GL_FLOAT, 0, vertices);
        glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

        ASSERT_EQ(GLenum(GL_NO_ERROR), glGetError());
    }

    for (int i=0 ; i<8 ; i++) {
        EXPECT_TRUE(checkPixel(i*16 + 8,  8, i*16, i*16, i*16, i*16, 0));
    }
}

} // namespace android
