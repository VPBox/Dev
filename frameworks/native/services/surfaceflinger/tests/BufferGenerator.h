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

#pragma once

#include <condition_variable>
#include <mutex>

#include <ui/GraphicBuffer.h>

namespace android {

class SurfaceManager;
class EglManager;
class Program;

class BufferGenerator {
public:
    BufferGenerator();
    ~BufferGenerator();

    /* Get a new fence */
    status_t get(sp<GraphicBuffer>* outBuffer, sp<Fence>* outFence);

    /* Static callback that sets the fence on a particular instance */
    static void setBuffer(const sp<GraphicBuffer>& buffer, int32_t fence, void* fenceGenerator);

private:
    bool mInitialized = false;

    std::unique_ptr<SurfaceManager> mSurfaceManager;
    std::unique_ptr<EglManager> mEglManager;
    std::unique_ptr<Program> mProgram;

    std::condition_variable_any mConditionVariable;

    sp<GraphicBuffer> mGraphicBuffer;
    int32_t mFence = -1;
    bool mPending = false;

    using Epoch = std::chrono::time_point<std::chrono::steady_clock>;
    Epoch mEpoch = std::chrono::steady_clock::now();
};

} // namespace android
