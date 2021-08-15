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

#include <gui/IGraphicBufferProducer.h>

#include <utils/RefBase.h>

namespace android {
namespace mock {

class GraphicBufferProducer : public BnGraphicBufferProducer, public virtual android::RefBase {
public:
    GraphicBufferProducer();
    ~GraphicBufferProducer() override;

    MOCK_METHOD2(requestBuffer, status_t(int, sp<GraphicBuffer>*));
    MOCK_METHOD1(setMaxDequeuedBufferCount, status_t(int));
    MOCK_METHOD1(setAsyncMode, status_t(bool));
    MOCK_METHOD8(dequeueBuffer,
                 status_t(int*, sp<Fence>*, uint32_t, uint32_t, PixelFormat, uint64_t, uint64_t*,
                          FrameEventHistoryDelta*));
    MOCK_METHOD1(detachBuffer, status_t(int));
    MOCK_METHOD2(detachNextBuffer, status_t(sp<GraphicBuffer>*, sp<Fence>*));
    MOCK_METHOD2(attachBuffer, status_t(int*, const sp<GraphicBuffer>&));
    MOCK_METHOD3(queueBuffer, status_t(int, const QueueBufferInput&, QueueBufferOutput*));
    MOCK_METHOD2(cancelBuffer, status_t(int, const sp<Fence>&));
    MOCK_METHOD2(query, int(int, int*));
    MOCK_METHOD4(connect, status_t(const sp<IProducerListener>&, int, bool, QueueBufferOutput*));
    MOCK_METHOD2(disconnect, status_t(int, DisconnectMode));
    MOCK_METHOD1(setSidebandStream, status_t(const sp<NativeHandle>&));
    MOCK_METHOD4(allocateBuffers, void(uint32_t, uint32_t, PixelFormat, uint64_t));
    MOCK_METHOD1(allowAllocation, status_t(bool));
    MOCK_METHOD1(setGenerationNumber, status_t(uint32_t));
    MOCK_CONST_METHOD0(getConsumerName, String8());
    MOCK_METHOD1(setSharedBufferMode, status_t(bool));
    MOCK_METHOD1(setAutoRefresh, status_t(bool));
    MOCK_METHOD1(setDequeueTimeout, status_t(nsecs_t));
    MOCK_METHOD3(getLastQueuedBuffer, status_t(sp<GraphicBuffer>*, sp<Fence>*, float[16]));
    MOCK_METHOD1(getFrameTimestamps, void(FrameEventHistoryDelta*));
    MOCK_CONST_METHOD1(getUniqueId, status_t(uint64_t*));
    MOCK_CONST_METHOD1(getConsumerUsage, status_t(uint64_t*));
};

} // namespace mock
} // namespace android
