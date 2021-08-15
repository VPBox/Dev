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

#define LOG_TAG "AImageReaderVendorTest"
//#define LOG_NDEBUG 0

#include <stdint.h>
#include <unistd.h>
#include <gtest/gtest.h>

#include <algorithm>
#include <mutex>
#include <string>
#include <variant>
#include <vector>
#include <stdio.h>
#include <stdio.h>
#include <stdio.h>

#include <android/log.h>
#include <camera/NdkCameraError.h>
#include <camera/NdkCameraManager.h>
#include <camera/NdkCameraDevice.h>
#include <camera/NdkCameraCaptureSession.h>
#include <media/NdkImage.h>
#include <media/NdkImageReader.h>
#include <cutils/native_handle.h>
#include <VendorTagDescriptor.h>

namespace {

static constexpr int kDummyFenceFd = -1;
static constexpr int kCaptureWaitUs = 100 * 1000;
static constexpr int kCaptureWaitRetry = 10;
static constexpr int kTestImageWidth = 640;
static constexpr int kTestImageHeight = 480;
static constexpr int kTestImageFormat = AIMAGE_FORMAT_YUV_420_888;

using android::hardware::camera::common::V1_0::helper::VendorTagDescriptorCache;
using ConfiguredWindows = std::set<native_handle_t *>;

class CameraHelper {
   public:
    CameraHelper(const char* id, ACameraManager *manager) :
            mImgReaderAnw(nullptr), mCameraId(id), mCameraManager(manager) {}
    ~CameraHelper() { closeCamera(); }

    struct PhysicalImgReaderInfo {
        const char* physicalCameraId;
        native_handle_t* anw;
    };

    // Retaining the error code in case the caller needs to analyze it.
    std::variant<int, ConfiguredWindows> initCamera(native_handle_t* imgReaderAnw,
            const std::vector<PhysicalImgReaderInfo>& physicalImgReaders,
            bool usePhysicalSettings) {
        ConfiguredWindows configuredWindows;
        if (imgReaderAnw == nullptr) {
            ALOGE("Cannot initialize camera before image reader get initialized.");
            return -1;
        }
        if (mIsCameraReady) {
            ALOGE("initCamera should only be called once.");
            return -1;
        }

        int ret;
        mImgReaderAnw = imgReaderAnw;

        ret = ACameraManager_openCamera(mCameraManager, mCameraId, &mDeviceCb, &mDevice);
        if (ret != AMEDIA_OK || mDevice == nullptr) {
            ALOGE("Failed to open camera, ret=%d, mDevice=%p.", ret, mDevice);
            return ret;
        }

        // Create capture session
        ret = ACaptureSessionOutputContainer_create(&mOutputs);
        if (ret != AMEDIA_OK) {
            ALOGE("ACaptureSessionOutputContainer_create failed, ret=%d", ret);
            return ret;
        }
        ret = ACaptureSessionOutput_create(mImgReaderAnw, &mImgReaderOutput);
        if (ret != AMEDIA_OK) {
            ALOGE("ACaptureSessionOutput_create failed, ret=%d", ret);
            return ret;
        }
        ret = ACaptureSessionOutputContainer_add(mOutputs, mImgReaderOutput);
        if (ret != AMEDIA_OK) {
            ALOGE("ACaptureSessionOutputContainer_add failed, ret=%d", ret);
            return ret;
        }
        configuredWindows.insert(mImgReaderAnw);
        std::vector<const char*> idPointerList;
        std::set<const native_handle_t*> physicalStreamMap;
        for (auto& physicalStream : physicalImgReaders) {
            ACaptureSessionOutput* sessionOutput = nullptr;
            ret = ACaptureSessionPhysicalOutput_create(physicalStream.anw,
                    physicalStream.physicalCameraId, &sessionOutput);
            if (ret != ACAMERA_OK) {
                ALOGE("ACaptureSessionPhysicalOutput_create failed, ret=%d", ret);
                return ret;
            }
            ret = ACaptureSessionOutputContainer_add(mOutputs, sessionOutput);
            if (ret != AMEDIA_OK) {
                ALOGE("ACaptureSessionOutputContainer_add failed, ret=%d", ret);
                return ret;
            }
            ret = ACameraDevice_isSessionConfigurationSupported(mDevice, mOutputs);
            if (ret != ACAMERA_OK && ret != ACAMERA_ERROR_UNSUPPORTED_OPERATION) {
                ALOGW("ACameraDevice_isSessionConfigurationSupported failed, ret=%d camera id %s",
                      ret, mCameraId);
                ACaptureSessionOutputContainer_remove(mOutputs, sessionOutput);
                ACaptureSessionOutput_free(sessionOutput);
                continue;
            }
            configuredWindows.insert(physicalStream.anw);
            // Assume that at most one physical stream per physical camera.
            mPhysicalCameraIds.push_back(physicalStream.physicalCameraId);
            idPointerList.push_back(physicalStream.physicalCameraId);
            physicalStreamMap.insert(physicalStream.anw);
            mSessionPhysicalOutputs.push_back(sessionOutput);
        }
        ACameraIdList cameraIdList;
        cameraIdList.numCameras = idPointerList.size();
        cameraIdList.cameraIds = idPointerList.data();

        ret = ACameraDevice_createCaptureSession(mDevice, mOutputs, &mSessionCb, &mSession);
        if (ret != AMEDIA_OK) {
            ALOGE("ACameraDevice_createCaptureSession failed, ret=%d", ret);
            return ret;
        }

        // Create capture request
        if (usePhysicalSettings) {
            ret = ACameraDevice_createCaptureRequest_withPhysicalIds(mDevice,
                    TEMPLATE_STILL_CAPTURE, &cameraIdList, &mStillRequest);
        } else {
            ret = ACameraDevice_createCaptureRequest(mDevice,
                    TEMPLATE_STILL_CAPTURE, &mStillRequest);
        }
        if (ret != AMEDIA_OK) {
            ALOGE("ACameraDevice_createCaptureRequest failed, ret=%d", ret);
            return ret;
        }
        ret = ACameraOutputTarget_create(mImgReaderAnw, &mReqImgReaderOutput);
        if (ret != AMEDIA_OK) {
            ALOGE("ACameraOutputTarget_create failed, ret=%d", ret);
            return ret;
        }
        ret = ACaptureRequest_addTarget(mStillRequest, mReqImgReaderOutput);
        if (ret != AMEDIA_OK) {
            ALOGE("ACaptureRequest_addTarget failed, ret=%d", ret);
            return ret;
        }

        for (auto& physicalStream : physicalImgReaders) {
            if (physicalStreamMap.find(physicalStream.anw) == physicalStreamMap.end()) {
                ALOGI("Skipping physicalStream anw=%p", physicalStream.anw);
                continue;
            }
            ACameraOutputTarget* outputTarget = nullptr;
            ret = ACameraOutputTarget_create(physicalStream.anw, &outputTarget);
            if (ret != AMEDIA_OK) {
                ALOGE("ACameraOutputTarget_create failed, ret=%d", ret);
                return ret;
            }
            ret = ACaptureRequest_addTarget(mStillRequest, outputTarget);
            if (ret != AMEDIA_OK) {
                ALOGE("ACaptureRequest_addTarget failed, ret=%d", ret);
                return ret;
            }
            mReqPhysicalOutputs.push_back(outputTarget);
        }

        mIsCameraReady = true;
        return configuredWindows;
    }


    bool isCameraReady() { return mIsCameraReady; }

    void closeCamera() {
        // Destroy capture request
        if (mReqImgReaderOutput) {
            ACameraOutputTarget_free(mReqImgReaderOutput);
            mReqImgReaderOutput = nullptr;
        }
        for (auto& outputTarget : mReqPhysicalOutputs) {
            ACameraOutputTarget_free(outputTarget);
        }
        mReqPhysicalOutputs.clear();
        if (mStillRequest) {
            ACaptureRequest_free(mStillRequest);
            mStillRequest = nullptr;
        }
        // Destroy capture session
        if (mSession != nullptr) {
            ACameraCaptureSession_close(mSession);
            mSession = nullptr;
        }
        if (mImgReaderOutput) {
            ACaptureSessionOutput_free(mImgReaderOutput);
            mImgReaderOutput = nullptr;
        }
        for (auto& extraOutput : mSessionPhysicalOutputs) {
            ACaptureSessionOutput_free(extraOutput);
        }
        mSessionPhysicalOutputs.clear();
        if (mOutputs) {
            ACaptureSessionOutputContainer_free(mOutputs);
            mOutputs = nullptr;
        }
        // Destroy camera device
        if (mDevice) {
            ACameraDevice_close(mDevice);
            mDevice = nullptr;
        }
        mIsCameraReady = false;
    }

    int takePicture() {
        int seqId;
        return ACameraCaptureSession_capture(mSession, &mCaptureCallbacks, 1, &mStillRequest,
                                             &seqId);
    }

    int takeLogicalCameraPicture() {
        int seqId;
        return ACameraCaptureSession_logicalCamera_capture(mSession, &mLogicalCaptureCallbacks,
                1, &mStillRequest, &seqId);
    }

    bool checkCallbacks(int pictureCount) {
        std::lock_guard<std::mutex> lock(mMutex);
        if (mCompletedCaptureCallbackCount != pictureCount) {
            ALOGE("Completed capture callaback count not as expected. expected %d actual %d",
                  pictureCount, mCompletedCaptureCallbackCount);
            return false;
        }
        return true;
    }

   private:
    ACameraDevice_StateCallbacks mDeviceCb{this, nullptr, nullptr};
    ACameraCaptureSession_stateCallbacks mSessionCb{ this, nullptr, nullptr, nullptr};

    native_handle_t* mImgReaderAnw = nullptr;  // not owned by us.

    // Camera device
    ACameraDevice* mDevice = nullptr;
    // Capture session
    ACaptureSessionOutputContainer* mOutputs = nullptr;
    ACaptureSessionOutput* mImgReaderOutput = nullptr;
    std::vector<ACaptureSessionOutput*> mSessionPhysicalOutputs;

    ACameraCaptureSession* mSession = nullptr;
    // Capture request
    ACaptureRequest* mStillRequest = nullptr;
    ACameraOutputTarget* mReqImgReaderOutput = nullptr;
    std::vector<ACameraOutputTarget*> mReqPhysicalOutputs;

    bool mIsCameraReady = false;
    const char* mCameraId;
    ACameraManager* mCameraManager;
    int mCompletedCaptureCallbackCount = 0;
    std::mutex mMutex;
    ACameraCaptureSession_captureCallbacks mCaptureCallbacks = {
        // TODO: Add tests for other callbacks
        this, // context
        nullptr, // onCaptureStarted
        nullptr, // onCaptureProgressed
        [](void* ctx , ACameraCaptureSession *, ACaptureRequest *,
                                          const ACameraMetadata *) {
            CameraHelper *ch = static_cast<CameraHelper *>(ctx);
            std::lock_guard<std::mutex> lock(ch->mMutex);
            ch->mCompletedCaptureCallbackCount++;
        },
        nullptr, // onCaptureFailed
        nullptr, // onCaptureSequenceCompleted
        nullptr, // onCaptureSequenceAborted
        nullptr, // onCaptureBufferLost
    };

    std::vector<std::string> mPhysicalCameraIds;
    ACameraCaptureSession_logicalCamera_captureCallbacks mLogicalCaptureCallbacks = {
        // TODO: Add tests for other callbacks
        this, // context
        nullptr, // onCaptureStarted
        nullptr, // onCaptureProgressed
        [](void* ctx , ACameraCaptureSession *, ACaptureRequest *,
                const ACameraMetadata *, size_t physicalResultCount,
                const char** physicalCameraIds, const ACameraMetadata** physicalResults) {
            CameraHelper *ch = static_cast<CameraHelper *>(ctx);
            std::lock_guard<std::mutex> lock(ch->mMutex);
            ASSERT_EQ(physicalResultCount, ch->mPhysicalCameraIds.size());
            for (size_t i = 0; i < physicalResultCount; i++) {
                ASSERT_TRUE(physicalCameraIds[i] != nullptr);
                ASSERT_TRUE(physicalResults[i] != nullptr);
                ASSERT_NE(std::find(ch->mPhysicalCameraIds.begin(),
                        ch->mPhysicalCameraIds.end(), physicalCameraIds[i]),
                        ch->mPhysicalCameraIds.end());

                // Verify frameNumber and sensorTimestamp exist in physical
                // result metadata
                ACameraMetadata_const_entry entry;
                ACameraMetadata_getConstEntry(
                        physicalResults[i], ACAMERA_SYNC_FRAME_NUMBER, &entry);
                ASSERT_EQ(entry.count, 1);
                ACameraMetadata_getConstEntry(
                        physicalResults[i], ACAMERA_SENSOR_TIMESTAMP, &entry);
                ASSERT_EQ(entry.count, 1);
            }
            ch->mCompletedCaptureCallbackCount++;
        },
        [] (void * /*ctx*/, ACameraCaptureSession* /*session*/, ACaptureRequest* /*request*/,
                ALogicalCameraCaptureFailure* failure) {
            if (failure->physicalCameraId) {
                ALOGD("%s: Physical camera id: %s result failure", __FUNCTION__,
                        failure->physicalCameraId);
            }
        },
        nullptr, // onCaptureSequenceCompleted
        nullptr, // onCaptureSequenceAborted
        nullptr, // onCaptureBufferLost
    };
};

class ImageReaderTestCase {
   public:
    ImageReaderTestCase(int32_t width,
                        int32_t height,
                        int32_t format,
                        uint64_t usage,
                        int32_t maxImages,
                        bool async)
        : mWidth(width),
          mHeight(height),
          mFormat(format),
          mUsage(usage),
          mMaxImages(maxImages),
          mAsync(async) {}

    ~ImageReaderTestCase() {
        if (mImgReaderAnw) {
            AImageReader_delete(mImgReader);
            // No need to call native_handle_t_release on imageReaderAnw
        }
    }

    int initImageReader() {
        if (mImgReader != nullptr || mImgReaderAnw != nullptr) {
            ALOGE("Cannot re-initalize image reader, mImgReader=%p, mImgReaderAnw=%p", mImgReader,
                  mImgReaderAnw);
            return -1;
        }

        media_status_t ret = AImageReader_newWithUsage(
                mWidth, mHeight, mFormat, mUsage, mMaxImages, &mImgReader);
        if (ret != AMEDIA_OK || mImgReader == nullptr) {
            ALOGE("Failed to create new AImageReader, ret=%d, mImgReader=%p", ret, mImgReader);
            return -1;
        }

        ret = AImageReader_setImageListener(mImgReader, &mReaderAvailableCb);
        if (ret != AMEDIA_OK) {
            ALOGE("Failed to set image available listener, ret=%d.", ret);
            return ret;
        }

        ret = AImageReader_setBufferRemovedListener(mImgReader, &mReaderDetachedCb);
        if (ret != AMEDIA_OK) {
            ALOGE("Failed to set buffer detaching listener, ret=%d.", ret);
            return ret;
        }

        ret = AImageReader_getWindowNativeHandle(mImgReader, &mImgReaderAnw);
        if (ret != AMEDIA_OK || mImgReaderAnw == nullptr) {
            ALOGE("Failed to get native_handle_t from AImageReader, ret=%d, mImgReaderAnw=%p.", ret,
                  mImgReaderAnw);
            return -1;
        }

        return 0;
    }

    native_handle_t* getNativeWindow() { return mImgReaderAnw; }

    int getAcquiredImageCount() {
        std::lock_guard<std::mutex> lock(mMutex);
        return mAcquiredImageCount;
    }

    void HandleImageAvailable(AImageReader* reader) {
        std::lock_guard<std::mutex> lock(mMutex);

        AImage* outImage = nullptr;
        media_status_t ret;

        // Make sure AImage will be deleted automatically when it goes out of
        // scope.
        auto imageDeleter = [this](AImage* img) {
            if (mAsync) {
                AImage_deleteAsync(img, kDummyFenceFd);
            } else {
                AImage_delete(img);
            }
        };
        std::unique_ptr<AImage, decltype(imageDeleter)> img(nullptr, imageDeleter);

        if (mAsync) {
            int outFenceFd = 0;
            // Verity that outFenceFd's value will be changed by
            // AImageReader_acquireNextImageAsync.
            ret = AImageReader_acquireNextImageAsync(reader, &outImage, &outFenceFd);
            if (ret != AMEDIA_OK || outImage == nullptr || outFenceFd == 0) {
                ALOGE("Failed to acquire image, ret=%d, outIamge=%p, outFenceFd=%d.", ret, outImage,
                      outFenceFd);
                return;
            }
            img.reset(outImage);
        } else {
            ret = AImageReader_acquireNextImage(reader, &outImage);
            if (ret != AMEDIA_OK || outImage == nullptr) {
                ALOGE("Failed to acquire image, ret=%d, outIamge=%p.", ret, outImage);
                return;
            }
            img.reset(outImage);
        }

        AHardwareBuffer* outBuffer = nullptr;
        ret = AImage_getHardwareBuffer(img.get(), &outBuffer);
        if (ret != AMEDIA_OK || outBuffer == nullptr) {
            ALOGE("Faild to get hardware buffer, ret=%d, outBuffer=%p.", ret, outBuffer);
            return;
        }

        // No need to release AHardwareBuffer, since we don't acquire additional
        // reference to it.
        AHardwareBuffer_Desc outDesc;
        AHardwareBuffer_describe(outBuffer, &outDesc);
        int32_t imageWidth = 0;
        int32_t imageHeight = 0;
        int32_t bufferWidth = static_cast<int32_t>(outDesc.width);
        int32_t bufferHeight = static_cast<int32_t>(outDesc.height);

        AImage_getWidth(outImage, &imageWidth);
        AImage_getHeight(outImage, &imageHeight);
        if (imageWidth != mWidth || imageHeight != mHeight) {
            ALOGE("Mismatched output image dimension: expected=%dx%d, actual=%dx%d", mWidth,
                  mHeight, imageWidth, imageHeight);
            return;
        }

        if (mFormat == AIMAGE_FORMAT_RGBA_8888 ||
            mFormat == AIMAGE_FORMAT_RGBX_8888 ||
            mFormat == AIMAGE_FORMAT_RGB_888 ||
            mFormat == AIMAGE_FORMAT_RGB_565 ||
            mFormat == AIMAGE_FORMAT_RGBA_FP16 ||
            mFormat == AIMAGE_FORMAT_YUV_420_888 ||
            mFormat == AIMAGE_FORMAT_Y8) {
            // Check output buffer dimension for certain formats. Don't do this for blob based
            // formats.
            if (bufferWidth != mWidth || bufferHeight != mHeight) {
                ALOGE("Mismatched output buffer dimension: expected=%dx%d, actual=%dx%d", mWidth,
                      mHeight, bufferWidth, bufferHeight);
                return;
            }
        }

        if ((outDesc.usage & mUsage) != mUsage) {
            ALOGE("Mismatched output buffer usage: actual (%" PRIu64 "), expected (%" PRIu64 ")",
                  outDesc.usage, mUsage);
            return;
        }

        uint8_t* data = nullptr;
        int dataLength = 0;
        ret = AImage_getPlaneData(img.get(), 0, &data, &dataLength);
        if (mUsage & AHARDWAREBUFFER_USAGE_CPU_READ_OFTEN) {
            // When we have CPU_READ_OFTEN usage bits, we can lock the image.
            if (ret != AMEDIA_OK || data == nullptr || dataLength < 0) {
                ALOGE("Failed to access CPU data, ret=%d, data=%p, dataLength=%d", ret, data,
                      dataLength);
                return;
            }
        } else {
            if (ret != AMEDIA_IMGREADER_CANNOT_LOCK_IMAGE || data != nullptr || dataLength != 0) {
                ALOGE("Shouldn't be able to access CPU data, ret=%d, data=%p, dataLength=%d", ret,
                      data, dataLength);
                return;
            }
        }
        // Only increase mAcquiredImageCount if all checks pass.
        mAcquiredImageCount++;
    }

    static void onImageAvailable(void* obj, AImageReader* reader) {
        ImageReaderTestCase* thiz = reinterpret_cast<ImageReaderTestCase*>(obj);
        thiz->HandleImageAvailable(reader);
    }

    static void
    onBufferRemoved(void* /*obj*/, AImageReader* /*reader*/, AHardwareBuffer* /*buffer*/) {
        // No-op, just to check the listener can be set properly.
    }

   private:
    int32_t mWidth;
    int32_t mHeight;
    int32_t mFormat;
    uint64_t mUsage;
    int32_t mMaxImages;
    bool mAsync;

    std::mutex mMutex;
    int mAcquiredImageCount{0};

    AImageReader* mImgReader = nullptr;
    native_handle_t* mImgReaderAnw = nullptr;

    AImageReader_ImageListener mReaderAvailableCb{this, onImageAvailable};
    AImageReader_BufferRemovedListener mReaderDetachedCb{this, onBufferRemoved};
};


class AImageReaderVendorTest : public ::testing::Test {
  public:
    void SetUp() override {
        mCameraManager = ACameraManager_create();
        if (mCameraManager == nullptr) {
            ALOGE("Failed to create ACameraManager.");
            return;
        }

        camera_status_t ret = ACameraManager_getCameraIdList(mCameraManager, &mCameraIdList);
        if (ret != ACAMERA_OK) {
            ALOGE("Failed to get cameraIdList: ret=%d", ret);
            return;
        }
        // TODO: Add more rigorous tests for vendor tags
        ASSERT_NE(VendorTagDescriptorCache::getGlobalVendorTagCache(), nullptr);
        if (mCameraIdList->numCameras < 1) {
            ALOGW("Device has no camera on board.");
            return;
        }
    }
    void TearDown() override {
        // Destroy camera manager
        if (mCameraIdList) {
            ACameraManager_deleteCameraIdList(mCameraIdList);
            mCameraIdList = nullptr;
        }
        if (mCameraManager) {
            ACameraManager_delete(mCameraManager);
            mCameraManager = nullptr;
        }
    }

    bool takePictures(const char* id, uint64_t readerUsage, int readerMaxImages,
            bool readerAsync, int pictureCount) {
        int ret = 0;

        ImageReaderTestCase testCase(
                kTestImageWidth, kTestImageHeight, kTestImageFormat, readerUsage, readerMaxImages,
                readerAsync);
        ret = testCase.initImageReader();
        if (ret < 0) {
            ALOGE("Unable to initialize ImageReader");
            return false;
        }

        CameraHelper cameraHelper(id, mCameraManager);
        std::variant<int, ConfiguredWindows> retInit =
                cameraHelper.initCamera(testCase.getNativeWindow(), {}/*physicalImageReaders*/,
                                        false/*usePhysicalSettings*/);
        int *retp = std::get_if<int>(&retInit);
        if (retp) {
            ALOGE("Unable to initialize camera helper");
            return false;
        }

        if (!cameraHelper.isCameraReady()) {
            ALOGW("Camera is not ready after successful initialization. It's either due to camera "
                  "on board lacks BACKWARDS_COMPATIBLE capability or the device does not have "
                  "camera on board.");
            return true;
        }

        for (int i = 0; i < pictureCount; i++) {
            ret = cameraHelper.takePicture();
            if (ret < 0) {
                ALOGE("Unable to take picture");
                return false;
            }
        }

        // Sleep until all capture finished
        for (int i = 0; i < kCaptureWaitRetry * pictureCount; i++) {
            usleep(kCaptureWaitUs);
            if (testCase.getAcquiredImageCount() == pictureCount) {
                ALOGI("Session take ~%d ms to capture %d images", i * kCaptureWaitUs / 1000,
                      pictureCount);
                break;
            }
        }
        return testCase.getAcquiredImageCount() == pictureCount &&
                cameraHelper.checkCallbacks(pictureCount);
    }

    bool testTakePicturesNative(const char* id) {
        for (auto& readerUsage :
             {AHARDWAREBUFFER_USAGE_CPU_READ_OFTEN}) {
            for (auto& readerMaxImages : {1, 4, 8}) {
                for (auto& readerAsync : {true, false}) {
                    for (auto& pictureCount : {1, 4, 8}) {
                        if (!takePictures(id, readerUsage, readerMaxImages,
                                readerAsync, pictureCount)) {
                            ALOGE("Test takePictures failed for test case usage=%" PRIu64
                                  ", maxImages=%d, async=%d, pictureCount=%d",
                                  readerUsage, readerMaxImages, readerAsync, pictureCount);
                            return false;
                        }
                    }
                }
            }
        }
        return true;
    }

    // Camera manager
    ACameraManager* mCameraManager = nullptr;
    ACameraIdList* mCameraIdList = nullptr;

    bool isCapabilitySupported(ACameraMetadata* staticInfo,
            acamera_metadata_enum_android_request_available_capabilities_t cap) {
        ACameraMetadata_const_entry entry;
        ACameraMetadata_getConstEntry(
                staticInfo, ACAMERA_REQUEST_AVAILABLE_CAPABILITIES, &entry);
        for (uint32_t i = 0; i < entry.count; i++) {
            if (entry.data.u8[i] == cap) {
                return true;
            }
        }
        return false;
    }

    bool isSizeSupportedForFormat(ACameraMetadata* staticInfo,
            int32_t format, int32_t width, int32_t height) {
        ACameraMetadata_const_entry entry;
        ACameraMetadata_getConstEntry(staticInfo,
                ACAMERA_SCALER_AVAILABLE_STREAM_CONFIGURATIONS, &entry);
        for (uint32_t i = 0; i < entry.count; i += 4) {
            if (entry.data.i32[i] == format &&
                    entry.data.i32[i+3] == ACAMERA_SCALER_AVAILABLE_STREAM_CONFIGURATIONS_OUTPUT &&
                    entry.data.i32[i+1] == width &&
                    entry.data.i32[i+2] == height) {
                return true;
            }
        }
        return false;
    }

    void findCandidateLogicalCamera(const char **cameraId,
            ACameraMetadata** staticMetadata,
            std::vector<const char*>* candidatePhysicalIds) {
        // Find first available logical camera
        for (int i = 0; i < mCameraIdList->numCameras; i++) {
            camera_status_t ret;
            ret = ACameraManager_getCameraCharacteristics(
                    mCameraManager, mCameraIdList->cameraIds[i], staticMetadata);
            ASSERT_EQ(ret, ACAMERA_OK);
            ASSERT_NE(*staticMetadata, nullptr);

            if (!isCapabilitySupported(*staticMetadata,
                    ACAMERA_REQUEST_AVAILABLE_CAPABILITIES_LOGICAL_MULTI_CAMERA)) {
                ACameraMetadata_free(*staticMetadata);
                *staticMetadata = nullptr;
                continue;
            }

            // Check returned physical camera Ids are valid
            size_t physicalCameraIdCnt = 0;
            const char*const* physicalCameraIds = nullptr;
            bool isLogicalCamera = ACameraMetadata_isLogicalMultiCamera(*staticMetadata,
                    &physicalCameraIdCnt, &physicalCameraIds);
            ASSERT_TRUE(isLogicalCamera);
            ASSERT_GE(physicalCameraIdCnt, 2);
            ACameraMetadata* physicalCameraMetadata = nullptr;
            candidatePhysicalIds->clear();
            for (size_t j = 0; j < physicalCameraIdCnt && candidatePhysicalIds->size() < 2; j++) {
                ASSERT_GT(strlen(physicalCameraIds[j]), 0);
                ret = ACameraManager_getCameraCharacteristics(
                        mCameraManager, physicalCameraIds[j], &physicalCameraMetadata);
                ASSERT_EQ(ret, ACAMERA_OK);
                ASSERT_NE(physicalCameraMetadata, nullptr);

                if (isSizeSupportedForFormat(physicalCameraMetadata, kTestImageFormat,
                        kTestImageWidth, kTestImageHeight)) {
                    candidatePhysicalIds->push_back(physicalCameraIds[j]);
                }
                ACameraMetadata_free(physicalCameraMetadata);
            }
            if (candidatePhysicalIds->size() == 2) {
                *cameraId = mCameraIdList->cameraIds[i];
                return;
            } else {
                ACameraMetadata_free(*staticMetadata);
                *staticMetadata = nullptr;
            }
        }
        *cameraId = nullptr;
        return;
    }

    void testLogicalCameraPhysicalStream(bool usePhysicalSettings) {
        const char* cameraId = nullptr;
        ACameraMetadata* staticMetadata = nullptr;
        std::vector<const char*> physicalCameraIds;

        findCandidateLogicalCamera(&cameraId, &staticMetadata, &physicalCameraIds);
        if (cameraId == nullptr) {
            // Couldn't find logical camera to test
            return;
        }

        // Test streaming the logical multi-camera
        uint64_t readerUsage = AHARDWAREBUFFER_USAGE_CPU_READ_OFTEN;
        int32_t readerMaxImages = 8;
        bool readerAsync = false;
        const int pictureCount = 6;
        std::vector<ImageReaderTestCase*> testCases;
        for (size_t i = 0; i < 3; i++) {
            ImageReaderTestCase* testCase = new ImageReaderTestCase(
                    kTestImageWidth, kTestImageHeight, kTestImageFormat, readerUsage,
                    readerMaxImages, readerAsync);
            ASSERT_EQ(testCase->initImageReader(), 0);
            testCases.push_back(testCase);
        }

        CameraHelper cameraHelper(cameraId, mCameraManager);
        std::vector<CameraHelper::PhysicalImgReaderInfo> physicalImgReaderInfo;
        physicalImgReaderInfo.push_back({physicalCameraIds[0], testCases[1]->getNativeWindow()});
        physicalImgReaderInfo.push_back({physicalCameraIds[1], testCases[2]->getNativeWindow()});

        std::variant<int, ConfiguredWindows> retInit =
                cameraHelper.initCamera(testCases[0]->getNativeWindow(), physicalImgReaderInfo,
                                        usePhysicalSettings);
        int *retp = std::get_if<int>(&retInit);
        ASSERT_EQ(retp, nullptr);
        ConfiguredWindows *configuredWindowsp = std::get_if<ConfiguredWindows>(&retInit);
        ASSERT_NE(configuredWindowsp, nullptr);
        ASSERT_LE(configuredWindowsp->size(), testCases.size());
        int ret = 0;
        if (!cameraHelper.isCameraReady()) {
            ALOGW("Camera is not ready after successful initialization. It's either due to camera "
                  "on board lacks BACKWARDS_COMPATIBLE capability or the device does not have "
                  "camera on board.");
            return;
        }

        for (int i = 0; i < pictureCount; i++) {
            ret = cameraHelper.takeLogicalCameraPicture();
            ASSERT_EQ(ret, 0);
        }

        // Sleep until all capture finished
        for (int i = 0; i < kCaptureWaitRetry * pictureCount; i++) {
            usleep(kCaptureWaitUs);
            if (testCases[0]->getAcquiredImageCount() == pictureCount) {
                ALOGI("Session take ~%d ms to capture %d images", i * kCaptureWaitUs / 1000,
                      pictureCount);
                break;
            }
        }
        for(auto &testCase : testCases) {
            auto it = configuredWindowsp->find(testCase->getNativeWindow());
            if (it == configuredWindowsp->end()) {
                continue;
            }
            ALOGI("Testing window %p", testCase->getNativeWindow());
            ASSERT_EQ(testCase->getAcquiredImageCount(), pictureCount);
        }

        ASSERT_TRUE(cameraHelper.checkCallbacks(pictureCount));

        ACameraMetadata_free(staticMetadata);
    }
};

TEST_F(AImageReaderVendorTest, CreateWindowNativeHandle) {
    // We always use the first camera.
    const char* cameraId = mCameraIdList->cameraIds[0];
    ASSERT_TRUE(cameraId != nullptr);

    ACameraMetadata* staticMetadata = nullptr;
    camera_status_t ret = ACameraManager_getCameraCharacteristics(
            mCameraManager, cameraId, &staticMetadata);
    ASSERT_EQ(ret, ACAMERA_OK);
    ASSERT_NE(staticMetadata, nullptr);

    bool isBC = isCapabilitySupported(staticMetadata,
            ACAMERA_REQUEST_AVAILABLE_CAPABILITIES_BACKWARD_COMPATIBLE);

    uint32_t namedTag = 0;
    // Test that ACameraMetadata_getTagFromName works as expected for public tag
    // names
    camera_status_t status = ACameraManager_getTagFromName(mCameraManager, cameraId,
            "android.control.aeMode", &namedTag);

    ASSERT_EQ(status, ACAMERA_OK);
    ASSERT_EQ(namedTag, ACAMERA_CONTROL_AE_MODE);

    ACameraMetadata_free(staticMetadata);

    if (!isBC) {
        ALOGW("Camera does not support BACKWARD_COMPATIBLE.");
        return;
    }

    EXPECT_TRUE(testTakePicturesNative(cameraId));
}

TEST_F(AImageReaderVendorTest, LogicalCameraPhysicalStream) {
    testLogicalCameraPhysicalStream(false/*usePhysicalSettings*/);
    testLogicalCameraPhysicalStream(true/*usePhysicalSettings*/);
}

}  // namespace
