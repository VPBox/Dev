/*
 * Copyright (C) 2018 The Android Open Source Project
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

#include <gmock/gmock.h>

#include <gui/IGraphicBufferConsumer.h>

#include <utils/RefBase.h>

namespace android {
namespace mock {

class GraphicBufferConsumer : public BnGraphicBufferConsumer, public virtual android::RefBase {
public:
    GraphicBufferConsumer();
    ~GraphicBufferConsumer() override;

    MOCK_METHOD3(acquireBuffer, status_t(BufferItem*, nsecs_t, uint64_t));
    MOCK_METHOD1(detachBuffer, status_t(int));
    MOCK_METHOD2(attachBuffer, status_t(int*, const sp<GraphicBuffer>&));
    MOCK_METHOD5(releaseBuffer, status_t(int, uint64_t, EGLDisplay, EGLSyncKHR, const sp<Fence>&));
    MOCK_METHOD2(consumerConnect, status_t(const sp<IConsumerListener>&, bool));
    MOCK_METHOD0(consumerDisconnect, status_t());
    MOCK_METHOD1(getReleasedBuffers, status_t(uint64_t*));
    MOCK_METHOD2(setDefaultBufferSize, status_t(uint32_t, uint32_t));
    MOCK_METHOD1(setMaxBufferCount, status_t(int));
    MOCK_METHOD1(setMaxAcquiredBufferCount, status_t(int));
    MOCK_METHOD1(setConsumerName, status_t(const String8&));
    MOCK_METHOD1(setDefaultBufferFormat, status_t(PixelFormat));
    MOCK_METHOD1(setDefaultBufferDataSpace, status_t(android_dataspace));
    MOCK_METHOD1(setConsumerUsageBits, status_t(uint64_t));
    MOCK_METHOD1(setConsumerIsProtected, status_t(bool));
    MOCK_METHOD1(setTransformHint, status_t(uint32_t));
    MOCK_CONST_METHOD1(getSidebandStream, status_t(sp<NativeHandle>*));
    MOCK_METHOD2(getOccupancyHistory, status_t(bool, std::vector<OccupancyTracker::Segment>*));
    MOCK_METHOD0(discardFreeBuffers, status_t());
    MOCK_CONST_METHOD2(dumpState, status_t(const String8&, String8*));
};

} // namespace mock
} // namespace android
