#include <android/log.h>
#include <gtest/gtest.h>

#include "dvr_api_test.h"

namespace {

class DvrTrackingTest : public DvrApiTest {};

#if DVR_TRACKING_IMPLEMENTED

TEST_F(DvrTrackingTest, Implemented) {
  ASSERT_TRUE(api_.TrackingCameraCreate != nullptr);
  ASSERT_TRUE(api_.TrackingCameraStart != nullptr);
  ASSERT_TRUE(api_.TrackingCameraStop != nullptr);

  ASSERT_TRUE(api_.TrackingFeatureExtractorCreate != nullptr);
  ASSERT_TRUE(api_.TrackingFeatureExtractorDestroy != nullptr);
  ASSERT_TRUE(api_.TrackingFeatureExtractorStart != nullptr);
  ASSERT_TRUE(api_.TrackingFeatureExtractorStop != nullptr);
  ASSERT_TRUE(api_.TrackingFeatureExtractorProcessBuffer != nullptr);
}

TEST_F(DvrTrackingTest, CameraCreateFailsForInvalidInput) {
  int ret;
  ret = api_.TrackingCameraCreate(nullptr);
  EXPECT_EQ(ret, -EINVAL);

  DvrTrackingCamera* camera = reinterpret_cast<DvrTrackingCamera*>(42);
  ret = api_.TrackingCameraCreate(&camera);
  EXPECT_EQ(ret, -EINVAL);
}

TEST_F(DvrTrackingTest, CameraCreateDestroy) {
  DvrTrackingCamera* camera = nullptr;
  int ret = api_.TrackingCameraCreate(&camera);

  EXPECT_EQ(ret, 0);
  ASSERT_TRUE(camera != nullptr);

  api_.TrackingCameraDestroy(camera);
}

TEST_F(DvrTrackingTest, FeatureExtractorCreateFailsForInvalidInput) {
  int ret;
  ret = api_.TrackingFeatureExtractorCreate(nullptr);
  EXPECT_EQ(ret, -EINVAL);

  DvrTrackingFeatureExtractor* camera =
      reinterpret_cast<DvrTrackingFeatureExtractor*>(42);
  ret = api_.TrackingFeatureExtractorCreate(&camera);
  EXPECT_EQ(ret, -EINVAL);
}

TEST_F(DvrTrackingTest, FeatureExtractorCreateDestroy) {
  DvrTrackingFeatureExtractor* camera = nullptr;
  int ret = api_.TrackingFeatureExtractorCreate(&camera);

  EXPECT_EQ(ret, 0);
  ASSERT_TRUE(camera != nullptr);

  api_.TrackingFeatureExtractorDestroy(camera);
}

#else  // !DVR_TRACKING_IMPLEMENTED

TEST_F(DvrTrackingTest, NotImplemented) {
  ASSERT_TRUE(api_.TrackingCameraCreate != nullptr);
  ASSERT_TRUE(api_.TrackingCameraDestroy != nullptr);
  ASSERT_TRUE(api_.TrackingCameraStart != nullptr);
  ASSERT_TRUE(api_.TrackingCameraStop != nullptr);

  EXPECT_EQ(api_.TrackingCameraCreate(nullptr), -ENOSYS);
  EXPECT_EQ(api_.TrackingCameraStart(nullptr, nullptr), -ENOSYS);
  EXPECT_EQ(api_.TrackingCameraStop(nullptr), -ENOSYS);

  ASSERT_TRUE(api_.TrackingFeatureExtractorCreate != nullptr);
  ASSERT_TRUE(api_.TrackingFeatureExtractorDestroy != nullptr);
  ASSERT_TRUE(api_.TrackingFeatureExtractorStart != nullptr);
  ASSERT_TRUE(api_.TrackingFeatureExtractorStop != nullptr);
  ASSERT_TRUE(api_.TrackingFeatureExtractorProcessBuffer != nullptr);

  EXPECT_EQ(api_.TrackingFeatureExtractorCreate(nullptr), -ENOSYS);
  EXPECT_EQ(api_.TrackingFeatureExtractorStart(nullptr, nullptr, nullptr),
            -ENOSYS);
  EXPECT_EQ(api_.TrackingFeatureExtractorStop(nullptr), -ENOSYS);
  EXPECT_EQ(api_.TrackingFeatureExtractorProcessBuffer(nullptr, nullptr,
                                                       nullptr, nullptr),
            -ENOSYS);

  ASSERT_TRUE(api_.TrackingSensorsCreate != nullptr);
  ASSERT_TRUE(api_.TrackingSensorsDestroy != nullptr);
  ASSERT_TRUE(api_.TrackingSensorsStart != nullptr);
  ASSERT_TRUE(api_.TrackingSensorsStop != nullptr);

  EXPECT_EQ(api_.TrackingSensorsCreate(nullptr, nullptr), -ENOSYS);
  EXPECT_EQ(api_.TrackingSensorsStart(nullptr, nullptr, nullptr), -ENOSYS);
  EXPECT_EQ(api_.TrackingSensorsStop(nullptr), -ENOSYS);
}

#endif  // DVR_TRACKING_IMPLEMENTED

}  // namespace
