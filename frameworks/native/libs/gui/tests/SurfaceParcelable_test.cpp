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

#define LOG_TAG "SurfaceParcelable_test"

#include <gtest/gtest.h>

#include <binder/IServiceManager.h>
#include <binder/ProcessState.h>
#include <gui/BufferHubProducer.h>
#include <gui/BufferQueue.h>
#include <gui/view/Surface.h>
#include <utils/Log.h>

namespace android {

static const String16 kTestServiceName = String16("SurfaceParcelableTestService");
static const String16 kSurfaceName = String16("TEST_SURFACE");
static const uint32_t kBufferWidth = 100;
static const uint32_t kBufferHeight = 1;
static const uint32_t kBufferFormat = HAL_PIXEL_FORMAT_BLOB;

enum SurfaceParcelableTestServiceCode {
    CREATE_BUFFER_QUEUE_SURFACE = IBinder::FIRST_CALL_TRANSACTION,
    CREATE_BUFFER_HUB_SURFACE,
};

class SurfaceParcelableTestService : public BBinder {
public:
    SurfaceParcelableTestService() {
        // BufferQueue
        BufferQueue::createBufferQueue(&mBufferQueueProducer, &mBufferQueueConsumer);

        // BufferHub
        dvr::ProducerQueueConfigBuilder configBuilder;
        mProducerQueue = dvr::ProducerQueue::Create(configBuilder.SetDefaultWidth(kBufferWidth)
                                                            .SetDefaultHeight(kBufferHeight)
                                                            .SetDefaultFormat(kBufferFormat)
                                                            .Build(),
                                                    dvr::UsagePolicy{});
        mBufferHubProducer = BufferHubProducer::Create(mProducerQueue);
    }

    ~SurfaceParcelableTestService() = default;

    virtual status_t onTransact(uint32_t code, const Parcel& /*data*/, Parcel* reply,
                                uint32_t /*flags*/ = 0) {
        switch (code) {
            case CREATE_BUFFER_QUEUE_SURFACE: {
                view::Surface surfaceShim;
                surfaceShim.name = kSurfaceName;
                surfaceShim.graphicBufferProducer = mBufferQueueProducer;
                return surfaceShim.writeToParcel(reply);
            }
            case CREATE_BUFFER_HUB_SURFACE: {
                view::Surface surfaceShim;
                surfaceShim.name = kSurfaceName;
                surfaceShim.graphicBufferProducer = mBufferHubProducer;
                return surfaceShim.writeToParcel(reply);
            }
            default:
                return UNKNOWN_TRANSACTION;
        };
    }

protected:
    sp<IGraphicBufferProducer> mBufferQueueProducer;
    sp<IGraphicBufferConsumer> mBufferQueueConsumer;

    std::shared_ptr<dvr::ProducerQueue> mProducerQueue;
    sp<IGraphicBufferProducer> mBufferHubProducer;
};

static int runBinderServer() {
    ProcessState::self()->startThreadPool();

    sp<IServiceManager> sm = defaultServiceManager();
    sp<SurfaceParcelableTestService> service = new SurfaceParcelableTestService;
    sm->addService(kTestServiceName, service, false);

    ALOGI("Binder server running...");

    while (true) {
        int stat, retval;
        retval = wait(&stat);
        if (retval == -1 && errno == ECHILD) {
            break;
        }
    }

    ALOGI("Binder server exiting...");
    return 0;
}

class SurfaceParcelableTest : public ::testing::TestWithParam<uint32_t> {
protected:
    virtual void SetUp() {
        mService = defaultServiceManager()->getService(kTestServiceName);
        if (mService == nullptr) {
            ALOGE("Failed to connect to the test service.");
            return;
        }

        ALOGI("Binder service is ready for client.");
    }

    status_t GetSurface(view::Surface* surfaceShim) {
        ALOGI("...Test: %d", GetParam());

        uint32_t opCode = GetParam();
        Parcel data;
        Parcel reply;
        status_t error = mService->transact(opCode, data, &reply);
        if (error != NO_ERROR) {
            ALOGE("Failed to get surface over binder, error=%d.", error);
            return error;
        }

        error = surfaceShim->readFromParcel(&reply);
        if (error != NO_ERROR) {
            ALOGE("Failed to get surface from parcel, error=%d.", error);
            return error;
        }

        return NO_ERROR;
    }

private:
    sp<IBinder> mService;
};

TEST_P(SurfaceParcelableTest, SendOverBinder) {
    view::Surface surfaceShim;
    EXPECT_EQ(GetSurface(&surfaceShim), NO_ERROR);
    EXPECT_EQ(surfaceShim.name, kSurfaceName);
    EXPECT_FALSE(surfaceShim.graphicBufferProducer == nullptr);
}

INSTANTIATE_TEST_CASE_P(SurfaceBackends, SurfaceParcelableTest,
                        ::testing::Values(CREATE_BUFFER_QUEUE_SURFACE, CREATE_BUFFER_HUB_SURFACE));

} // namespace android

int main(int argc, char** argv) {
    pid_t pid = fork();
    if (pid == 0) {
        android::ProcessState::self()->startThreadPool();
        ::testing::InitGoogleTest(&argc, argv);
        return RUN_ALL_TESTS();

    } else {
        ALOGI("Test process pid: %d.", pid);
        return android::runBinderServer();
    }
}
