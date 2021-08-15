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

#define LOG_TAG "SurfaceTextureGLThreadToGL_test"
//#define LOG_NDEBUG 0

#include "SurfaceTextureGLThreadToGL.h"

namespace android {

TEST_F(SurfaceTextureGLThreadToGLTest,
        UpdateTexImageBeforeFrameFinishedCompletes) {
    class PT : public ProducerThread {
        virtual void render() {
            glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            swapBuffers();
        }
    };

    SetUpWindowAndContext();

    runProducerThread(new PT());

    mFC->waitForFrame();
    ASSERT_EQ(NO_ERROR, mST->updateTexImage());
    mFC->finishFrame();

    // TODO: Add frame verification once RGB TEX_EXTERNAL_OES is supported!
}

TEST_F(SurfaceTextureGLThreadToGLTest,
        UpdateTexImageAfterFrameFinishedCompletes) {
    class PT : public ProducerThread {
        virtual void render() {
            glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            swapBuffers();
        }
    };

    SetUpWindowAndContext();

    runProducerThread(new PT());

    mFC->waitForFrame();
    mFC->finishFrame();
    ASSERT_EQ(NO_ERROR, mST->updateTexImage());

    // TODO: Add frame verification once RGB TEX_EXTERNAL_OES is supported!
}

TEST_F(SurfaceTextureGLThreadToGLTest,
        RepeatedUpdateTexImageBeforeFrameFinishedCompletes) {
    enum { NUM_ITERATIONS = 1024 };

    class PT : public ProducerThread {
        virtual void render() {
            for (int i = 0; i < NUM_ITERATIONS; i++) {
                glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT);
                ALOGV("+swapBuffers");
                swapBuffers();
                ALOGV("-swapBuffers");
            }
        }
    };

    SetUpWindowAndContext();

    runProducerThread(new PT());

    for (int i = 0; i < NUM_ITERATIONS; i++) {
        mFC->waitForFrame();
        ALOGV("+updateTexImage");
        ASSERT_EQ(NO_ERROR, mST->updateTexImage());
        ALOGV("-updateTexImage");
        mFC->finishFrame();

        // TODO: Add frame verification once RGB TEX_EXTERNAL_OES is supported!
    }
}

TEST_F(SurfaceTextureGLThreadToGLTest,
        RepeatedUpdateTexImageAfterFrameFinishedCompletes) {
    enum { NUM_ITERATIONS = 1024 };

    class PT : public ProducerThread {
        virtual void render() {
            for (int i = 0; i < NUM_ITERATIONS; i++) {
                glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT);
                ALOGV("+swapBuffers");
                swapBuffers();
                ALOGV("-swapBuffers");
            }
        }
    };

    SetUpWindowAndContext();

    runProducerThread(new PT());

    for (int i = 0; i < NUM_ITERATIONS; i++) {
        mFC->waitForFrame();
        mFC->finishFrame();
        ALOGV("+updateTexImage");
        ASSERT_EQ(NO_ERROR, mST->updateTexImage());
        ALOGV("-updateTexImage");

        // TODO: Add frame verification once RGB TEX_EXTERNAL_OES is supported!
    }
}

// XXX: This test is disabled because it is currently hanging on some devices.
TEST_F(SurfaceTextureGLThreadToGLTest,
        DISABLED_RepeatedSwapBuffersWhileDequeueStalledCompletes) {
    enum { NUM_ITERATIONS = 64 };

    class PT : public ProducerThread {
        virtual void render() {
            for (int i = 0; i < NUM_ITERATIONS; i++) {
                glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT);
                ALOGV("+swapBuffers");
                swapBuffers();
                ALOGV("-swapBuffers");
            }
        }
    };

    SetUpWindowAndContext();

    runProducerThread(new PT());

    // Allow three frames to be rendered and queued before starting the
    // rendering in this thread.  For the latter two frames we don't call
    // updateTexImage so the next dequeue from the producer thread will block
    // waiting for a frame to become available.
    mFC->waitForFrame();
    mFC->finishFrame();

    // We must call updateTexImage to consume the first frame so that the
    // SurfaceTexture is able to reduce the buffer count to 2.  This is because
    // the GL driver may dequeue a buffer when the EGLSurface is created, and
    // that happens before we call setDefaultMaxBufferCount.  It's possible that the
    // driver does not dequeue a buffer at EGLSurface creation time, so we
    // cannot rely on this to cause the second dequeueBuffer call to block.
    ASSERT_EQ(NO_ERROR, mST->updateTexImage());

    mFC->waitForFrame();
    mFC->finishFrame();
    mFC->waitForFrame();
    mFC->finishFrame();

    // Sleep for 100ms to allow the producer thread's dequeueBuffer call to
    // block waiting for a buffer to become available.
    usleep(100000);

    // Render and present a number of images.  This thread should not be blocked
    // by the fact that the producer thread is blocking in dequeue.
    for (int i = 0; i < NUM_ITERATIONS; i++) {
        glClear(GL_COLOR_BUFFER_BIT);
        eglSwapBuffers(mEglDisplay, mEglSurface);
    }

    // Consume the two pending buffers to unblock the producer thread.
    ASSERT_EQ(NO_ERROR, mST->updateTexImage());
    ASSERT_EQ(NO_ERROR, mST->updateTexImage());

    // Consume the remaining buffers from the producer thread.
    for (int i = 0; i < NUM_ITERATIONS-3; i++) {
        mFC->waitForFrame();
        mFC->finishFrame();
        ALOGV("+updateTexImage");
        ASSERT_EQ(NO_ERROR, mST->updateTexImage());
        ALOGV("-updateTexImage");
    }
}

} // namespace android
