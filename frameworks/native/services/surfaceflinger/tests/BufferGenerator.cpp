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

#include <gui/BufferItemConsumer.h>
#include <gui/Surface.h>

#include <GLES3/gl3.h>
#include <math/vec2.h>
#include <math/vec3.h>
#include <math/vec4.h>

#include "BufferGenerator.h"
#include "BufferGeneratorShader.h"

namespace android {

/* Used to receive the surfaces and fences from egl. The egl buffers are thrown
 * away. The fences are sent to the requester via a callback */
class SurfaceManager {
public:
    /* Returns a fence from egl */
    using BufferCallback = std::function<void(const sp<GraphicBuffer>& buffer, int32_t fence)>;

    /* Listens for a new frame, detaches the buffer and returns the fence
     * through saved callback. */
    class BufferListener : public ConsumerBase::FrameAvailableListener {
    public:
        BufferListener(sp<IGraphicBufferConsumer> consumer, BufferCallback callback)
              : mConsumer(consumer), mCallback(callback) {}

        void onFrameAvailable(const BufferItem& /*item*/) {
            BufferItem item;

            if (mConsumer->acquireBuffer(&item, 0)) return;
            if (mConsumer->detachBuffer(item.mSlot)) return;

            mCallback(item.mGraphicBuffer, item.mFence->dup());
        }

    private:
        sp<IGraphicBufferConsumer> mConsumer;
        BufferCallback mCallback;
    };

    /* Creates a buffer listener that waits on a new frame from the buffer
     * queue. */
    void initialize(uint32_t width, uint32_t height, android_pixel_format_t format,
                    BufferCallback callback) {
        sp<IGraphicBufferProducer> producer;
        sp<IGraphicBufferConsumer> consumer;
        BufferQueue::createBufferQueue(&producer, &consumer);

        consumer->setDefaultBufferSize(width, height);
        consumer->setDefaultBufferFormat(format);

        mBufferItemConsumer = new BufferItemConsumer(consumer, 0);

        mListener = new BufferListener(consumer, callback);
        mBufferItemConsumer->setFrameAvailableListener(mListener);

        mSurface = new Surface(producer, true);
    }

    /* Used by Egl manager. The surface is never displayed. */
    sp<Surface> getSurface() const { return mSurface; }

private:
    sp<BufferItemConsumer> mBufferItemConsumer;
    sp<BufferListener> mListener;
    /* Used by Egl manager. The surface is never displayed */
    sp<Surface> mSurface;
};

/* Used to generate valid fences. It is not possible to create a dummy sync
 * fence for testing. Egl can generate buffers along with a valid fence.
 * The buffer cannot be guaranteed to be the same format across all devices so
 * a CPU filled buffer is used instead. The Egl fence is used along with the
 * CPU filled buffer. */
class EglManager {
public:
    EglManager()
          : mEglDisplay(EGL_NO_DISPLAY), mEglSurface(EGL_NO_SURFACE), mEglContext(EGL_NO_CONTEXT) {}

    ~EglManager() { cleanup(); }

    int initialize(sp<Surface> surface) {
        mSurface = surface;

        mEglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
        if (mEglDisplay == EGL_NO_DISPLAY) return false;

        EGLint major;
        EGLint minor;
        if (!eglInitialize(mEglDisplay, &major, &minor)) {
            ALOGW("Could not initialize EGL");
            return false;
        }

        /* We're going to use a 1x1 pbuffer surface later on
         * The configuration distance doesn't really matter for what we're
         * trying to do */
        EGLint configAttrs[] = {EGL_RENDERABLE_TYPE,
                                EGL_OPENGL_ES2_BIT,
                                EGL_RED_SIZE,
                                8,
                                EGL_GREEN_SIZE,
                                8,
                                EGL_BLUE_SIZE,
                                8,
                                EGL_ALPHA_SIZE,
                                0,
                                EGL_DEPTH_SIZE,
                                24,
                                EGL_STENCIL_SIZE,
                                0,
                                EGL_NONE};

        EGLConfig configs[1];
        EGLint configCnt;
        if (!eglChooseConfig(mEglDisplay, configAttrs, configs, 1, &configCnt)) {
            ALOGW("Could not select EGL configuration");
            eglReleaseThread();
            eglTerminate(mEglDisplay);
            return false;
        }

        if (configCnt <= 0) {
            ALOGW("Could not find EGL configuration");
            eglReleaseThread();
            eglTerminate(mEglDisplay);
            return false;
        }

        /* These objects are initialized below but the default "null" values are
         * used to cleanup properly at any point in the initialization sequence */
        EGLint attrs[] = {EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE};
        mEglContext = eglCreateContext(mEglDisplay, configs[0], EGL_NO_CONTEXT, attrs);
        if (mEglContext == EGL_NO_CONTEXT) {
            ALOGW("Could not create EGL context");
            cleanup();
            return false;
        }

        EGLint majorVersion;
        if (!eglQueryContext(mEglDisplay, mEglContext, EGL_CONTEXT_CLIENT_VERSION, &majorVersion)) {
            ALOGW("Could not query EGL version");
            cleanup();
            return false;
        }

        if (majorVersion != 3) {
            ALOGW("Unsupported EGL version");
            cleanup();
            return false;
        }

        EGLint surfaceAttrs[] = {EGL_NONE};
        mEglSurface = eglCreateWindowSurface(mEglDisplay, configs[0], mSurface.get(), surfaceAttrs);
        if (mEglSurface == EGL_NO_SURFACE) {
            ALOGW("Could not create EGL surface");
            cleanup();
            return false;
        }

        if (!eglMakeCurrent(mEglDisplay, mEglSurface, mEglSurface, mEglContext)) {
            ALOGW("Could not change current EGL context");
            cleanup();
            return false;
        }

        return true;
    }

    void makeCurrent() const { eglMakeCurrent(mEglDisplay, mEglSurface, mEglSurface, mEglContext); }

    void present() const { eglSwapBuffers(mEglDisplay, mEglSurface); }

private:
    void cleanup() {
        if (mEglDisplay == EGL_NO_DISPLAY) return;
        if (mEglSurface != EGL_NO_SURFACE) eglDestroySurface(mEglDisplay, mEglSurface);
        if (mEglContext != EGL_NO_CONTEXT) eglDestroyContext(mEglDisplay, mEglContext);

        eglMakeCurrent(mEglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        eglReleaseThread();
        eglTerminate(mEglDisplay);
    }

    sp<Surface> mSurface;
    EGLDisplay mEglDisplay;
    EGLSurface mEglSurface;
    EGLContext mEglContext;
};

class Program {
public:
    ~Program() {
        if (mInitialized) {
            glDetachShader(mProgram, mVertexShader);
            glDetachShader(mProgram, mFragmentShader);

            glDeleteShader(mVertexShader);
            glDeleteShader(mFragmentShader);

            glDeleteProgram(mProgram);
        }
    }

    bool initialize(const char* vertex, const char* fragment) {
        mVertexShader = buildShader(vertex, GL_VERTEX_SHADER);
        if (!mVertexShader) {
            return false;
        }

        mFragmentShader = buildShader(fragment, GL_FRAGMENT_SHADER);
        if (!mFragmentShader) {
            return false;
        }

        mProgram = glCreateProgram();
        glAttachShader(mProgram, mVertexShader);
        glAttachShader(mProgram, mFragmentShader);

        glLinkProgram(mProgram);

        GLint status;
        glGetProgramiv(mProgram, GL_LINK_STATUS, &status);
        if (status != GL_TRUE) {
            GLint length = 0;
            glGetProgramiv(mProgram, GL_INFO_LOG_LENGTH, &length);
            if (length > 1) {
                GLchar log[length];
                glGetProgramInfoLog(mProgram, length, nullptr, &log[0]);
                ALOGE("%s", log);
            }
            ALOGE("Error while linking shaders");
            return false;
        }
        mInitialized = true;
        return true;
    }

    void use() const { glUseProgram(mProgram); }

    void bindVec4(GLint location, vec4 v) const { glUniform4f(location, v.x, v.y, v.z, v.w); }

    void bindVec3(GLint location, const vec3* v, uint32_t count) const {
        glUniform3fv(location, count, &(v->x));
    }

    void bindFloat(GLint location, float v) { glUniform1f(location, v); }

private:
    GLuint buildShader(const char* source, GLenum type) const {
        GLuint shader = glCreateShader(type);
        glShaderSource(shader, 1, &source, nullptr);
        glCompileShader(shader);

        GLint status;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
        if (status != GL_TRUE) {
            ALOGE("Error while compiling shader of type 0x%x:\n===\n%s\n===", type, source);
            // Some drivers return wrong values for GL_INFO_LOG_LENGTH
            // use a fixed size instead
            GLchar log[512];
            glGetShaderInfoLog(shader, sizeof(log), nullptr, &log[0]);
            ALOGE("Shader info log: %s", log);
            return 0;
        }

        return shader;
    }

    GLuint mProgram = 0;
    GLuint mVertexShader = 0;
    GLuint mFragmentShader = 0;
    bool mInitialized = false;
};

BufferGenerator::BufferGenerator()
      : mSurfaceManager(new SurfaceManager), mEglManager(new EglManager), mProgram(new Program) {
    const float width = 1000.0;
    const float height = 1000.0;

    auto setBufferWithContext =
            std::bind(setBuffer, std::placeholders::_1, std::placeholders::_2, this);
    mSurfaceManager->initialize(width, height, HAL_PIXEL_FORMAT_RGBA_8888, setBufferWithContext);

    if (!mEglManager->initialize(mSurfaceManager->getSurface())) return;

    mEglManager->makeCurrent();

    if (!mProgram->initialize(VERTEX_SHADER, FRAGMENT_SHADER)) return;
    mProgram->use();
    mProgram->bindVec4(0, vec4{width, height, 1.0f / width, 1.0f / height});
    mProgram->bindVec3(2, &SPHERICAL_HARMONICS[0], 4);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, &TRIANGLE[0]);

    mInitialized = true;
}

BufferGenerator::~BufferGenerator() {
    mEglManager->makeCurrent();
}

status_t BufferGenerator::get(sp<GraphicBuffer>* outBuffer, sp<Fence>* outFence) {
    // mMutex is used to protect get() from getting called by multiple threads at the same time
    static std::mutex mMutex;
    std::lock_guard lock(mMutex);

    if (!mInitialized) {
        if (outBuffer) {
            *outBuffer = nullptr;
        }
        if (*outFence) {
            *outFence = nullptr;
        }
        return -EINVAL;
    }

    // Generate a buffer and fence. They will be returned through the setBuffer callback
    mEglManager->makeCurrent();

    glClear(GL_COLOR_BUFFER_BIT);

    const std::chrono::duration<float> time = std::chrono::steady_clock::now() - mEpoch;
    mProgram->bindFloat(1, time.count());

    glDrawArrays(GL_TRIANGLES, 0, 3);

    mPending = true;
    mEglManager->present();

    // Wait for the setBuffer callback
    if (!mConditionVariable.wait_for(mMutex, std::chrono::seconds(2),
                                     [this] { return !mPending; })) {
        ALOGE("failed to set buffer and fence");
        return -ETIME;
    }

    // Return buffer and fence
    if (outBuffer) {
        *outBuffer = mGraphicBuffer;
    }
    if (outFence) {
        *outFence = new Fence(mFence);
    } else {
        close(mFence);
    }
    mGraphicBuffer = nullptr;
    mFence = -1;

    return NO_ERROR;
}

// static
void BufferGenerator::setBuffer(const sp<GraphicBuffer>& buffer, int32_t fence,
                                void* bufferGenerator) {
    BufferGenerator* generator = static_cast<BufferGenerator*>(bufferGenerator);
    generator->mGraphicBuffer = buffer;
    generator->mFence = fence;
    generator->mPending = false;
    generator->mConditionVariable.notify_all();
}

} // namespace android
