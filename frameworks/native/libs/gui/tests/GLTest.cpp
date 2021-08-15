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

#include "GLTest.h"

#include <gui/Surface.h>

#include <GLES2/gl2.h>

namespace android {

using Transaction = SurfaceComposerClient::Transaction;

static int abs(int value) {
    return value > 0 ? value : -value;
}

void GLTest::SetUp() {
    const ::testing::TestInfo* const testInfo =
        ::testing::UnitTest::GetInstance()->current_test_info();
    ALOGV("Begin test: %s.%s", testInfo->test_case_name(), testInfo->name());

    mEglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    ASSERT_EQ(EGL_SUCCESS, eglGetError());
    ASSERT_NE(EGL_NO_DISPLAY, mEglDisplay);

    EGLint majorVersion;
    EGLint minorVersion;
    EXPECT_TRUE(eglInitialize(mEglDisplay, &majorVersion, &minorVersion));
    ASSERT_EQ(EGL_SUCCESS, eglGetError());
    RecordProperty("EglVersionMajor", majorVersion);
    RecordProperty("EglVersionMinor", minorVersion);

    EGLint numConfigs = 0;
    EXPECT_TRUE(eglChooseConfig(mEglDisplay, getConfigAttribs(), &mGlConfig, 1,
            &numConfigs));
    ASSERT_EQ(EGL_SUCCESS, eglGetError());

    char* displaySecsEnv = getenv("GLTEST_DISPLAY_SECS");
    if (displaySecsEnv != nullptr) {
        mDisplaySecs = atoi(displaySecsEnv);
        if (mDisplaySecs < 0) {
            mDisplaySecs = 0;
        }
    } else {
        mDisplaySecs = 0;
    }

    if (mDisplaySecs > 0) {
        mComposerClient = new SurfaceComposerClient;
        ASSERT_EQ(NO_ERROR, mComposerClient->initCheck());

        mSurfaceControl = mComposerClient->createSurface(
                String8("Test Surface"), getSurfaceWidth(), getSurfaceHeight(),
                PIXEL_FORMAT_RGB_888, 0);

        ASSERT_TRUE(mSurfaceControl != nullptr);
        ASSERT_TRUE(mSurfaceControl->isValid());

        Transaction t;
        ASSERT_EQ(NO_ERROR, t.setLayer(mSurfaceControl, 0x7FFFFFFF)
                .show(mSurfaceControl)
                .apply());

        sp<ANativeWindow> window = mSurfaceControl->getSurface();
        mEglSurface = createWindowSurface(mEglDisplay, mGlConfig, window);
    } else {
        EGLint pbufferAttribs[] = {
            EGL_WIDTH, getSurfaceWidth(),
            EGL_HEIGHT, getSurfaceHeight(),
            EGL_NONE };

        mEglSurface = eglCreatePbufferSurface(mEglDisplay, mGlConfig,
                pbufferAttribs);
    }
    ASSERT_EQ(EGL_SUCCESS, eglGetError());
    ASSERT_NE(EGL_NO_SURFACE, mEglSurface);

    mEglContext = eglCreateContext(mEglDisplay, mGlConfig, EGL_NO_CONTEXT,
            getContextAttribs());
    ASSERT_EQ(EGL_SUCCESS, eglGetError());
    ASSERT_NE(EGL_NO_CONTEXT, mEglContext);

    EXPECT_TRUE(eglMakeCurrent(mEglDisplay, mEglSurface, mEglSurface,
            mEglContext));
    ASSERT_EQ(EGL_SUCCESS, eglGetError());

    EGLint w, h;
    EXPECT_TRUE(eglQuerySurface(mEglDisplay, mEglSurface, EGL_WIDTH, &w));
    ASSERT_EQ(EGL_SUCCESS, eglGetError());
    EXPECT_TRUE(eglQuerySurface(mEglDisplay, mEglSurface, EGL_HEIGHT, &h));
    ASSERT_EQ(EGL_SUCCESS, eglGetError());
    RecordProperty("EglSurfaceWidth", w);
    RecordProperty("EglSurfaceHeight", h);

    glViewport(0, 0, w, h);
    ASSERT_EQ(GLenum(GL_NO_ERROR), glGetError());
}

void GLTest::TearDown() {
    // Display the result
    if (mDisplaySecs > 0 && mEglSurface != EGL_NO_SURFACE) {
        eglSwapBuffers(mEglDisplay, mEglSurface);
        sleep(mDisplaySecs);
    }

    if (mComposerClient != nullptr) {
        mComposerClient->dispose();
    }
    if (mEglContext != EGL_NO_CONTEXT) {
        eglDestroyContext(mEglDisplay, mEglContext);
    }
    if (mEglSurface != EGL_NO_SURFACE) {
        eglDestroySurface(mEglDisplay, mEglSurface);
    }
    if (mEglDisplay != EGL_NO_DISPLAY) {
        eglMakeCurrent(mEglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE,
                EGL_NO_CONTEXT);
        eglTerminate(mEglDisplay);
    }
    ASSERT_EQ(EGL_SUCCESS, eglGetError());

    const ::testing::TestInfo* const testInfo =
        ::testing::UnitTest::GetInstance()->current_test_info();
    ALOGV("End test:   %s.%s", testInfo->test_case_name(), testInfo->name());
}

EGLint const* GLTest::getConfigAttribs() {
    static const EGLint sDefaultConfigAttribs[] = {
        EGL_SURFACE_TYPE, EGL_PBUFFER_BIT,
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
        EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
        EGL_ALPHA_SIZE, 8,
        EGL_DEPTH_SIZE, 16,
        EGL_STENCIL_SIZE, 8,
        EGL_NONE };

    return sDefaultConfigAttribs;
}

EGLint const* GLTest::getContextAttribs() {
    static const EGLint sDefaultContextAttribs[] = {
        EGL_CONTEXT_CLIENT_VERSION, 2,
        EGL_NONE };

    return sDefaultContextAttribs;
}

EGLint GLTest::getSurfaceWidth() {
    return 512;
}

EGLint GLTest::getSurfaceHeight() {
    return 512;
}

EGLSurface GLTest::createWindowSurface(EGLDisplay display, EGLConfig config,
                                       sp<ANativeWindow>& window) const {
    return eglCreateWindowSurface(display, config, window.get(), nullptr);
}

::testing::AssertionResult GLTest::checkPixel(int x, int y,
        int r, int g, int b, int a, int tolerance) {
    GLubyte pixel[4];
    String8 msg;
    glReadPixels(x, y, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixel);
    GLenum err = glGetError();
    if (err != GL_NO_ERROR) {
        msg += String8::format("error reading pixel: %#x", err);
        while ((err = glGetError()) != GL_NO_ERROR) {
            msg += String8::format(", %#x", err);
        }
        return ::testing::AssertionFailure(::testing::Message(msg.string()));
    }
    if (r >= 0 && abs(r - int(pixel[0])) > tolerance) {
        msg += String8::format("r(%d isn't %d)", pixel[0], r);
    }
    if (g >= 0 && abs(g - int(pixel[1])) > tolerance) {
        if (!msg.isEmpty()) {
            msg += " ";
        }
        msg += String8::format("g(%d isn't %d)", pixel[1], g);
    }
    if (b >= 0 && abs(b - int(pixel[2])) > tolerance) {
        if (!msg.isEmpty()) {
            msg += " ";
        }
        msg += String8::format("b(%d isn't %d)", pixel[2], b);
    }
    if (a >= 0 && abs(a - int(pixel[3])) > tolerance) {
        if (!msg.isEmpty()) {
            msg += " ";
        }
        msg += String8::format("a(%d isn't %d)", pixel[3], a);
    }
    if (!msg.isEmpty()) {
        return ::testing::AssertionFailure(::testing::Message(msg.string()));
    } else {
        return ::testing::AssertionSuccess();
    }
}

::testing::AssertionResult GLTest::assertRectEq(const Rect &r1, const Rect &r2,
                                                int tolerance) {
    String8 msg;

    if (abs(r1.left - r2.left) > tolerance) {
        msg += String8::format("left(%d isn't %d)", r1.left, r2.left);
    }
    if (abs(r1.top - r2.top) > tolerance) {
        if (!msg.isEmpty()) {
            msg += " ";
        }
        msg += String8::format("top(%d isn't %d)", r1.top, r2.top);
    }
    if (abs(r1.right - r2.right) > tolerance) {
        if (!msg.isEmpty()) {
            msg += " ";
        }
        msg += String8::format("right(%d isn't %d)", r1.right, r2.right);
    }
    if (abs(r1.bottom - r2.bottom) > tolerance) {
        if (!msg.isEmpty()) {
            msg += " ";
        }
        msg += String8::format("bottom(%d isn't %d)", r1.bottom, r2.bottom);
    }
    if (!msg.isEmpty()) {
        msg += String8::format(" R1: [%d %d %d %d] R2: [%d %d %d %d]",
                               r1.left, r1.top, r1.right, r1.bottom,
                               r2.left, r2.top, r2.right, r2.bottom);
        fprintf(stderr, "assertRectEq: %s\n", msg.string());
        return ::testing::AssertionFailure(::testing::Message(msg.string()));
    } else {
        return ::testing::AssertionSuccess();
    }
}

void GLTest::loadShader(GLenum shaderType, const char* pSource,
        GLuint* outShader) {
    GLuint shader = glCreateShader(shaderType);
    ASSERT_EQ(GLenum(GL_NO_ERROR), glGetError());
    if (shader) {
        glShaderSource(shader, 1, &pSource, nullptr);
        ASSERT_EQ(GLenum(GL_NO_ERROR), glGetError());
        glCompileShader(shader);
        ASSERT_EQ(GLenum(GL_NO_ERROR), glGetError());
        GLint compiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        ASSERT_EQ(GLenum(GL_NO_ERROR), glGetError());
        if (!compiled) {
            GLint infoLen = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
            ASSERT_EQ(GLenum(GL_NO_ERROR), glGetError());
            if (infoLen) {
                char* buf = (char*) malloc(infoLen);
                if (buf) {
                    glGetShaderInfoLog(shader, infoLen, nullptr, buf);
                    printf("Shader compile log:\n%s\n", buf);
                    free(buf);
                    FAIL();
                }
            } else {
                char* buf = (char*) malloc(0x1000);
                if (buf) {
                    glGetShaderInfoLog(shader, 0x1000, nullptr, buf);
                    printf("Shader compile log:\n%s\n", buf);
                    free(buf);
                    FAIL();
                }
            }
            glDeleteShader(shader);
            shader = 0;
        }
    }
    ASSERT_TRUE(shader != 0);
    *outShader = shader;
}

void GLTest::createProgram(const char* pVertexSource,
        const char* pFragmentSource, GLuint* outPgm) {
    GLuint vertexShader, fragmentShader;
    {
        SCOPED_TRACE("compiling vertex shader");
        ASSERT_NO_FATAL_FAILURE(loadShader(GL_VERTEX_SHADER, pVertexSource,
                &vertexShader));
    }
    {
        SCOPED_TRACE("compiling fragment shader");
        ASSERT_NO_FATAL_FAILURE(loadShader(GL_FRAGMENT_SHADER, pFragmentSource,
                &fragmentShader));
    }

    GLuint program = glCreateProgram();
    ASSERT_EQ(GLenum(GL_NO_ERROR), glGetError());
    if (program) {
        glAttachShader(program, vertexShader);
        ASSERT_EQ(GLenum(GL_NO_ERROR), glGetError());
        glAttachShader(program, fragmentShader);
        ASSERT_EQ(GLenum(GL_NO_ERROR), glGetError());
        glLinkProgram(program);
        GLint linkStatus = GL_FALSE;
        glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
        if (linkStatus != GL_TRUE) {
            GLint bufLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
            if (bufLength) {
                char* buf = (char*) malloc(bufLength);
                if (buf) {
                    glGetProgramInfoLog(program, bufLength, nullptr, buf);
                    printf("Program link log:\n%s\n", buf);
                    free(buf);
                    FAIL();
                }
            }
            glDeleteProgram(program);
            program = 0;
        }
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    ASSERT_TRUE(program != 0);
    *outPgm = program;
}

} // namespace android
