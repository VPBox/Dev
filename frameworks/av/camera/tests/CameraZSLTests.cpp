/*
 * Copyright (C) 2017 The Android Open Source Project
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

#define LOG_NDEBUG 0
#define LOG_TAG "CameraZSLTests"

#include <gtest/gtest.h>

#include <binder/ProcessState.h>
#include <utils/Errors.h>
#include <utils/Log.h>
#include <gui/Surface.h>
#include <gui/SurfaceComposerClient.h>
#include <camera/CameraParameters.h>
#include <camera/CameraMetadata.h>
#include <camera/Camera.h>
#include <android/hardware/ICameraService.h>

using namespace android;
using namespace android::hardware;

class CameraZSLTests : public ::testing::Test,
    public ::android::hardware::BnCameraClient {
protected:

    CameraZSLTests() : numCameras(0), mPreviewBufferCount(0),
    mAutoFocusMessage(false), mSnapshotNotification(false) {}

    //Gtest interface
    void SetUp() override;
    void TearDown() override;

    //CameraClient interface
    void notifyCallback(int32_t msgType, int32_t, int32_t) override;
    void dataCallback(int32_t msgType, const sp<IMemory>&,
            camera_frame_metadata_t *) override;
    void dataCallbackTimestamp(nsecs_t, int32_t,
            const sp<IMemory>&) override {};
    void recordingFrameHandleCallbackTimestamp(nsecs_t,
            native_handle_t*) override {};
    void recordingFrameHandleCallbackTimestampBatch(
            const std::vector<nsecs_t>&,
            const std::vector<native_handle_t*>&) override {};

    status_t waitForPreviewStart();
    status_t waitForEvent(Mutex &mutex, Condition &condition, bool &flag);

    mutable Mutex mPreviewLock;
    mutable Condition mPreviewCondition;
    mutable Mutex mAutoFocusLock;
    mutable Condition mAutoFocusCondition;
    mutable Mutex mSnapshotLock;
    mutable Condition mSnapshotCondition;

    int32_t numCameras;
    size_t mPreviewBufferCount;
    sp<ICameraService> mCameraService;
    sp<SurfaceComposerClient> mComposerClient;
    bool mAutoFocusMessage;
    bool mSnapshotNotification;
    static const int32_t kPreviewThreshold  = 8;
    static const nsecs_t kPreviewTimeout    = 5000000000;  // 5 [s.]
    static const nsecs_t kEventTimeout      = 10000000000; // 10 [s.]
};

void CameraZSLTests::SetUp() {
    ::android::binder::Status rc;
    ProcessState::self()->startThreadPool();
    sp<IServiceManager> sm = defaultServiceManager();
    sp<IBinder> binder = sm->getService(String16("media.camera"));
    mCameraService = interface_cast<ICameraService>(binder);
    rc = mCameraService->getNumberOfCameras(
            hardware::ICameraService::CAMERA_TYPE_ALL, &numCameras);
    EXPECT_TRUE(rc.isOk());

    mComposerClient = new SurfaceComposerClient;
    ASSERT_EQ(NO_ERROR, mComposerClient->initCheck());
}

void CameraZSLTests::TearDown() {
    mCameraService.clear();
    mComposerClient->dispose();
}

void CameraZSLTests::notifyCallback(int32_t msgType, int32_t,
        int32_t) {
    if (CAMERA_MSG_FOCUS == msgType) {
        Mutex::Autolock l(mAutoFocusLock);
        mAutoFocusMessage = true;
        mAutoFocusCondition.broadcast();
    } else {
        ALOGV("%s: msgType: %d", __FUNCTION__, msgType);
    }
};

void CameraZSLTests::dataCallback(int32_t msgType, const sp<IMemory>& /*data*/,
        camera_frame_metadata_t *) {

    switch (msgType) {
    case CAMERA_MSG_PREVIEW_FRAME: {
        Mutex::Autolock l(mPreviewLock);
        mPreviewBufferCount++;
        mPreviewCondition.broadcast();
        break;
    }
    case CAMERA_MSG_COMPRESSED_IMAGE: {
        Mutex::Autolock l(mSnapshotLock);
        mSnapshotNotification = true;
        //TODO: Add checks on incoming Jpeg
        mSnapshotCondition.broadcast();
        break;
    }
    default:
        ALOGV("%s: msgType: %d", __FUNCTION__, msgType);
    }
};

status_t CameraZSLTests::waitForPreviewStart() {
    status_t rc = NO_ERROR;
    Mutex::Autolock l(mPreviewLock);
    mPreviewBufferCount = 0;

    while (mPreviewBufferCount < kPreviewThreshold) {
        rc = mPreviewCondition.waitRelative(mPreviewLock,
                kPreviewTimeout);
        if (NO_ERROR != rc) {
            break;
        }
    }

    return rc;
}

status_t CameraZSLTests::waitForEvent(Mutex &mutex,
        Condition &condition, bool &flag) {
    status_t rc = NO_ERROR;
    Mutex::Autolock l(mutex);
    flag = false;

    while (!flag) {
        rc = condition.waitRelative(mutex,
                kEventTimeout);
        if (NO_ERROR != rc) {
            break;
        }
    }

    return rc;
}

TEST_F(CameraZSLTests, TestAllPictureSizes) {
    ::android::binder::Status rc;

    for (int32_t cameraId = 0; cameraId < numCameras; cameraId++) {
        sp<Surface> previewSurface;
        sp<SurfaceControl> surfaceControl;
        sp<ICamera> cameraDevice;

        String16 cameraIdStr = String16(String8::format("%d", cameraId));
        bool isSupported = false;
        rc = mCameraService->supportsCameraApi(cameraIdStr,
                hardware::ICameraService::API_VERSION_1, &isSupported);
        EXPECT_TRUE(rc.isOk());

        // We only care about camera Camera1 ZSL support.
        if (!isSupported) {
            continue;
        }

        CameraMetadata metadata;
        rc = mCameraService->getCameraCharacteristics(cameraIdStr, &metadata);
        if (!rc.isOk()) {
            // The test is relevant only for cameras with Hal 3.x
            // support.
            continue;
        }
        EXPECT_FALSE(metadata.isEmpty());
        camera_metadata_entry_t availableCapabilities =
                metadata.find(ANDROID_REQUEST_AVAILABLE_CAPABILITIES);
        EXPECT_TRUE(0 < availableCapabilities.count);
        bool isReprocessSupported = false;
        const uint8_t *caps = availableCapabilities.data.u8;
        for (size_t i = 0; i < availableCapabilities.count; i++) {
            if (ANDROID_REQUEST_AVAILABLE_CAPABILITIES_PRIVATE_REPROCESSING ==
                caps[i]) {
                isReprocessSupported = true;
                break;
            }
        }
        if (!isReprocessSupported) {
            // ZSL relies on this feature
            continue;
        }

        rc = mCameraService->connect(this, cameraId,
                String16("ZSLTest"), hardware::ICameraService::USE_CALLING_UID,
                hardware::ICameraService::USE_CALLING_PID, &cameraDevice);
        EXPECT_TRUE(rc.isOk());

        CameraParameters params(cameraDevice->getParameters());

        String8 focusModes(params.get(
                CameraParameters::KEY_SUPPORTED_FOCUS_MODES));
        bool isAFSupported = false;
        const char *focusMode = nullptr;
        if (focusModes.contains(CameraParameters::FOCUS_MODE_AUTO)) {
            // If supported 'auto' should be set by default
            isAFSupported = true;
        } else if (focusModes.contains(
                CameraParameters::FOCUS_MODE_CONTINUOUS_PICTURE)) {
            isAFSupported = true;
            focusMode = CameraParameters::FOCUS_MODE_CONTINUOUS_PICTURE;
        } else if (focusModes.contains(
                CameraParameters::FOCUS_MODE_CONTINUOUS_VIDEO)) {
            isAFSupported = true;
            focusMode = CameraParameters::FOCUS_MODE_CONTINUOUS_VIDEO;
        } else if (focusModes.contains(CameraParameters::FOCUS_MODE_MACRO)) {
            isAFSupported = true;
            focusMode = CameraParameters::FOCUS_MODE_MACRO;
        }

        if (!isAFSupported) {
            // AF state is needed
            continue;
        }

        if (nullptr != focusMode) {
            params.set(CameraParameters::KEY_FOCUS_MODE, focusMode);
            ASSERT_EQ(NO_ERROR, cameraDevice->setParameters(params.flatten()));
        }

        int previewWidth, previewHeight;
        params.getPreviewSize(&previewWidth, &previewHeight);
        ASSERT_TRUE((0 < previewWidth) && (0 < previewHeight));

        surfaceControl = mComposerClient->createSurface(
                String8("Test Surface"),
                previewWidth, previewHeight,
                CameraParameters::previewFormatToEnum(
                        params.getPreviewFormat()),
                GRALLOC_USAGE_HW_RENDER);

        ASSERT_TRUE(nullptr != surfaceControl.get());
        ASSERT_TRUE(surfaceControl->isValid());

        SurfaceComposerClient::Transaction{}
                .setLayer(surfaceControl, 0x7fffffff)
                .show(surfaceControl)
                .apply();

        previewSurface = surfaceControl->getSurface();
        ASSERT_TRUE(previewSurface != NULL);
        ASSERT_EQ(NO_ERROR, cameraDevice->setPreviewTarget(
                previewSurface->getIGraphicBufferProducer()));

        cameraDevice->setPreviewCallbackFlag(
                CAMERA_FRAME_CALLBACK_FLAG_CAMCORDER);

        Vector<Size> pictureSizes;
        params.getSupportedPictureSizes(pictureSizes);
        for (size_t i = 0; i < pictureSizes.size(); i++) {
            params.setPictureSize(pictureSizes[i].width,
                    pictureSizes[i].height);
            ASSERT_EQ(NO_ERROR, cameraDevice->setParameters(params.flatten()));
            ASSERT_EQ(NO_ERROR, cameraDevice->startPreview());
            ASSERT_EQ(NO_ERROR, waitForPreviewStart());

            ASSERT_EQ(NO_ERROR, cameraDevice->autoFocus());
            ASSERT_EQ(NO_ERROR, waitForEvent(mAutoFocusLock,
                    mAutoFocusCondition, mAutoFocusMessage));

            ASSERT_EQ(NO_ERROR,
                    cameraDevice->takePicture(CAMERA_MSG_COMPRESSED_IMAGE));
            ASSERT_EQ(NO_ERROR, waitForEvent(mSnapshotLock, mSnapshotCondition,
                    mSnapshotNotification));
        }

        cameraDevice->stopPreview();
        rc = cameraDevice->disconnect();
        EXPECT_TRUE(rc.isOk());
    }
}
