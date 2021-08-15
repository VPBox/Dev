#include <android/hardware_buffer.h>
#include <android/log.h>
#include <dvr/dvr_api.h>
#include <dvr/dvr_display_types.h>
#include <dvr/dvr_surface.h>

#include <gtest/gtest.h>

#include "dvr_api_test.h"

#define LOG_TAG "dvr_display-test"

#ifndef ALOGD
#define ALOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#endif

class DvrDisplayTest : public DvrApiTest {
 protected:
  void SetUp() override {
    DvrApiTest::SetUp();
    int ret = api_.GetNativeDisplayMetrics(sizeof(display_metrics_),
                                           &display_metrics_);
    ASSERT_EQ(ret, 0) << "Failed to get display metrics.";
    ALOGD(
        "display_width: %d, display_height: %d, display_x_dpi: %d, "
        "display_y_dpi: %d, vsync_period_ns: %d.",
        display_metrics_.display_width, display_metrics_.display_height,
        display_metrics_.display_x_dpi, display_metrics_.display_y_dpi,
        display_metrics_.vsync_period_ns);
  }

  void TearDown() override {
    if (write_queue_ != nullptr) {
      api_.WriteBufferQueueDestroy(write_queue_);
      write_queue_ = nullptr;
    }
    if (direct_surface_ != nullptr) {
      api_.SurfaceDestroy(direct_surface_);
      direct_surface_ = nullptr;
    }
    DvrApiTest::TearDown();
  }

  /* Convert a write buffer to an android hardware buffer and fill in
   * color_textures evenly to the buffer.
   * AssertionError if the width of the buffer is not equal to the input width,
   * AssertionError if the height of the buffer is not equal to the input
   * height.
   */
  void FillWriteBuffer(DvrWriteBuffer* write_buffer,
                       const std::vector<uint32_t>& color_textures,
                       uint32_t width, uint32_t height);

  // Write buffer queue properties.
  static constexpr uint64_t kUsage = AHARDWAREBUFFER_USAGE_GPU_SAMPLED_IMAGE |
                                     AHARDWAREBUFFER_USAGE_GPU_COLOR_OUTPUT |
                                     AHARDWAREBUFFER_USAGE_CPU_WRITE_OFTEN;
  uint32_t kFormat = AHARDWAREBUFFER_FORMAT_R8G8B8A8_UNORM;
  static constexpr size_t kMetadataSize = 0;
  static constexpr int kTimeoutMs = 1000;  // Time for getting buffer.
  uint32_t kLayerCount = 1;
  DvrWriteBufferQueue* write_queue_ = nullptr;
  DvrSurface* direct_surface_ = nullptr;

  // Device display properties.
  DvrNativeDisplayMetrics display_metrics_;
};

TEST_F(DvrDisplayTest, DisplayWithOneBuffer) {
  // Create a direct surface.
  std::vector<DvrSurfaceAttribute> direct_surface_attributes = {
      {.key = DVR_SURFACE_ATTRIBUTE_DIRECT,
       .value.type = DVR_SURFACE_ATTRIBUTE_TYPE_BOOL,
       .value.bool_value = true},
      {.key = DVR_SURFACE_ATTRIBUTE_Z_ORDER,
       .value.type = DVR_SURFACE_ATTRIBUTE_TYPE_INT32,
       .value.int32_value = 10},
      {.key = DVR_SURFACE_ATTRIBUTE_VISIBLE,
       .value.type = DVR_SURFACE_ATTRIBUTE_TYPE_BOOL,
       .value.bool_value = true},
  };
  int ret =
      api_.SurfaceCreate(direct_surface_attributes.data(),
                         direct_surface_attributes.size(), &direct_surface_);
  ASSERT_EQ(ret, 0) << "Failed to create direct surface.";

  // Create a buffer queue with the direct surface.
  constexpr size_t kCapacity = 1;
  uint32_t width = display_metrics_.display_width;
  uint32_t height = display_metrics_.display_height;
  ret = api_.SurfaceCreateWriteBufferQueue(
      direct_surface_, width, height, kFormat, kLayerCount, kUsage, kCapacity,
      kMetadataSize, &write_queue_);
  EXPECT_EQ(0, ret) << "Failed to create buffer queue.";
  ASSERT_NE(nullptr, write_queue_) << "Write buffer queue should not be null.";

  // Get buffer from WriteBufferQueue.
  DvrWriteBuffer* write_buffer = nullptr;
  DvrNativeBufferMetadata out_meta;
  int out_fence_fd = -1;
  ret = api_.WriteBufferQueueGainBuffer(write_queue_, kTimeoutMs, &write_buffer,
                                        &out_meta, &out_fence_fd);
  EXPECT_EQ(0, ret) << "Failed to get the buffer.";
  ASSERT_NE(nullptr, write_buffer) << "Gained buffer should not be null.";

  // Color the write buffer.
  FillWriteBuffer(write_buffer,
                  {0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff, 0x00000000},
                  width, height);

  // Post buffer.
  int ready_fence_fd = -1;
  ret = api_.WriteBufferQueuePostBuffer(write_queue_, write_buffer, &out_meta,
                                        ready_fence_fd);
  EXPECT_EQ(0, ret) << "Failed to post the buffer.";

  sleep(5);  // For visual check on the device under test.
  // Should observe three primary colors on the screen center.
}

TEST_F(DvrDisplayTest, DisplayWithDoubleBuffering) {
  // Create a direct surface.
  std::vector<DvrSurfaceAttribute> direct_surface_attributes = {
      {.key = DVR_SURFACE_ATTRIBUTE_DIRECT,
       .value.type = DVR_SURFACE_ATTRIBUTE_TYPE_BOOL,
       .value.bool_value = true},
      {.key = DVR_SURFACE_ATTRIBUTE_Z_ORDER,
       .value.type = DVR_SURFACE_ATTRIBUTE_TYPE_INT32,
       .value.int32_value = 10},
      {.key = DVR_SURFACE_ATTRIBUTE_VISIBLE,
       .value.type = DVR_SURFACE_ATTRIBUTE_TYPE_BOOL,
       .value.bool_value = true},
  };
  int ret =
      api_.SurfaceCreate(direct_surface_attributes.data(),
                         direct_surface_attributes.size(), &direct_surface_);
  ASSERT_EQ(ret, 0) << "Failed to create direct surface.";

  // Create a buffer queue with the direct surface.
  constexpr size_t kCapacity = 2;
  uint32_t width = display_metrics_.display_width;
  uint32_t height = display_metrics_.display_height;
  ret = api_.SurfaceCreateWriteBufferQueue(
      direct_surface_, width, height, kFormat, kLayerCount, kUsage, kCapacity,
      kMetadataSize, &write_queue_);
  EXPECT_EQ(0, ret) << "Failed to create buffer queue.";
  ASSERT_NE(nullptr, write_queue_) << "Write buffer queue should not be null.";

  int num_display_cycles_in_5s = 5 / (display_metrics_.vsync_period_ns / 1e9);
  ALOGD("The number of display cycles: %d", num_display_cycles_in_5s);
  int bufferhub_id_prev_write_buffer = -1;
  for (int i = 0; i < num_display_cycles_in_5s; ++i) {
    // Get a buffer from the WriteBufferQueue.
    DvrWriteBuffer* write_buffer = nullptr;
    DvrNativeBufferMetadata out_meta;
    int out_fence_fd = -1;
    ret = api_.WriteBufferQueueGainBuffer(
        write_queue_, kTimeoutMs, &write_buffer, &out_meta, &out_fence_fd);
    EXPECT_EQ(0, ret) << "Failed to get the a write buffer.";
    ASSERT_NE(nullptr, write_buffer) << "The gained buffer should not be null.";

    int bufferhub_id = api_.WriteBufferGetId(write_buffer);
    ALOGD("Display cycle: %d, bufferhub id of the write buffer: %d", i,
          bufferhub_id);
    EXPECT_NE(bufferhub_id_prev_write_buffer, bufferhub_id)
        << "Double buffering should be using the two buffers in turns, not "
           "reusing the same write buffer.";
    bufferhub_id_prev_write_buffer = bufferhub_id;

    // Color the write buffer.
    if (i % 2) {
      FillWriteBuffer(write_buffer, {0xffff0000, 0xff00ff00, 0xff0000ff}, width,
                      height);
    } else {
      FillWriteBuffer(write_buffer, {0xff00ff00, 0xff0000ff, 0xffff0000}, width,
                      height);
    }

    // Post the write buffer.
    int ready_fence_fd = -1;
    ret = api_.WriteBufferQueuePostBuffer(write_queue_, write_buffer, &out_meta,
                                          ready_fence_fd);
    EXPECT_EQ(0, ret) << "Failed to post the buffer.";
  }
  // Should observe blinking screen in secondary colors
  // although it is actually displaying primary colors.
}

TEST_F(DvrDisplayTest, DisplayWithTwoHardwareLayers) {
  // Create the direct_surface_0 of z order 10 and direct_surface_1 of z
  // order 11.
  DvrSurface* direct_surface_0 = nullptr;
  std::vector<DvrSurfaceAttribute> direct_surface_0_attributes = {
      {.key = DVR_SURFACE_ATTRIBUTE_DIRECT,
       .value.type = DVR_SURFACE_ATTRIBUTE_TYPE_BOOL,
       .value.bool_value = true},
      {.key = DVR_SURFACE_ATTRIBUTE_Z_ORDER,
       .value.type = DVR_SURFACE_ATTRIBUTE_TYPE_INT32,
       .value.int32_value = 10},
      {.key = DVR_SURFACE_ATTRIBUTE_VISIBLE,
       .value.type = DVR_SURFACE_ATTRIBUTE_TYPE_BOOL,
       .value.bool_value = true},
  };
  int ret =
      api_.SurfaceCreate(direct_surface_0_attributes.data(),
                         direct_surface_0_attributes.size(), &direct_surface_0);
  EXPECT_EQ(ret, 0) << "Failed to create direct surface.";

  DvrSurface* direct_surface_1 = nullptr;
  std::vector<DvrSurfaceAttribute> direct_surface_1_attributes = {
      {.key = DVR_SURFACE_ATTRIBUTE_DIRECT,
       .value.type = DVR_SURFACE_ATTRIBUTE_TYPE_BOOL,
       .value.bool_value = true},
      {.key = DVR_SURFACE_ATTRIBUTE_Z_ORDER,
       .value.type = DVR_SURFACE_ATTRIBUTE_TYPE_INT32,
       .value.int32_value = 11},
      {.key = DVR_SURFACE_ATTRIBUTE_VISIBLE,
       .value.type = DVR_SURFACE_ATTRIBUTE_TYPE_BOOL,
       .value.bool_value = true},
  };
  ret =
      api_.SurfaceCreate(direct_surface_1_attributes.data(),
                         direct_surface_1_attributes.size(), &direct_surface_1);
  EXPECT_EQ(ret, 0) << "Failed to create direct surface.";

  // Create a buffer queue for each of the direct surfaces.
  constexpr size_t kCapacity = 1;
  uint32_t width = display_metrics_.display_width;
  uint32_t height = display_metrics_.display_height;

  DvrWriteBufferQueue* write_queue_0 = nullptr;
  ret = api_.SurfaceCreateWriteBufferQueue(
      direct_surface_0, width, height, kFormat, kLayerCount, kUsage, kCapacity,
      kMetadataSize, &write_queue_0);
  EXPECT_EQ(0, ret) << "Failed to create buffer queue.";
  EXPECT_NE(nullptr, write_queue_0) << "Write buffer queue should not be null.";

  DvrWriteBufferQueue* write_queue_1 = nullptr;
  ret = api_.SurfaceCreateWriteBufferQueue(
      direct_surface_1, width, height, kFormat, kLayerCount, kUsage, kCapacity,
      kMetadataSize, &write_queue_1);
  EXPECT_EQ(0, ret) << "Failed to create buffer queue.";
  EXPECT_NE(nullptr, write_queue_1) << "Write buffer queue should not be null.";

  // Get a buffer from each of the write buffer queues.
  DvrWriteBuffer* write_buffer_0 = nullptr;
  DvrNativeBufferMetadata out_meta_0;
  int out_fence_fd = -1;
  ret = api_.WriteBufferQueueGainBuffer(
      write_queue_0, kTimeoutMs, &write_buffer_0, &out_meta_0, &out_fence_fd);
  EXPECT_EQ(0, ret) << "Failed to get the buffer.";
  EXPECT_NE(nullptr, write_buffer_0) << "Gained buffer should not be null.";

  DvrWriteBuffer* write_buffer_1 = nullptr;
  DvrNativeBufferMetadata out_meta_1;
  out_fence_fd = -1;
  ret = api_.WriteBufferQueueGainBuffer(
      write_queue_1, kTimeoutMs, &write_buffer_1, &out_meta_1, &out_fence_fd);
  EXPECT_EQ(0, ret) << "Failed to get the buffer.";
  EXPECT_NE(nullptr, write_buffer_1) << "Gained buffer should not be null.";

  // Color the write buffers.
  FillWriteBuffer(write_buffer_0, {0xffff0000, 0xff00ff00, 0xff0000ff}, width,
                  height);
  FillWriteBuffer(write_buffer_1, {0x7f00ff00, 0x7f0000ff, 0x7fff0000}, width,
                  height);

  // Post buffers.
  int ready_fence_fd = -1;
  ret = api_.WriteBufferQueuePostBuffer(write_queue_0, write_buffer_0,
                                        &out_meta_0, ready_fence_fd);
  EXPECT_EQ(0, ret) << "Failed to post the buffer.";

  ready_fence_fd = -1;
  ret = api_.WriteBufferQueuePostBuffer(write_queue_1, write_buffer_1,
                                        &out_meta_1, ready_fence_fd);
  EXPECT_EQ(0, ret) << "Failed to post the buffer.";

  sleep(5);  // For visual check on the device under test.
  // Should observe three secondary colors.

  // Test finished. Clean up buffers and surfaces.
  if (write_queue_0 != nullptr) {
    api_.WriteBufferQueueDestroy(write_queue_0);
    write_queue_0 = nullptr;
  }
  if (write_queue_1 != nullptr) {
    api_.WriteBufferQueueDestroy(write_queue_1);
    write_queue_1 = nullptr;
  }
  if (direct_surface_0 != nullptr) {
    api_.SurfaceDestroy(direct_surface_0);
  }
  if (direct_surface_1 != nullptr) {
    api_.SurfaceDestroy(direct_surface_1);
  }
}

void DvrDisplayTest::FillWriteBuffer(
    DvrWriteBuffer* write_buffer, const std::vector<uint32_t>& color_textures,
    uint32_t width, uint32_t height) {
  uint32_t num_colors = color_textures.size();
  // Convert the first write buffer to an android hardware buffer.
  AHardwareBuffer* ah_buffer = nullptr;
  int ret = api_.WriteBufferGetAHardwareBuffer(write_buffer, &ah_buffer);
  ASSERT_EQ(0, ret) << "Failed to get a hardware buffer from the write buffer.";
  ASSERT_NE(nullptr, ah_buffer) << "AHardware buffer should not be null.";
  AHardwareBuffer_Desc ah_buffer_describe;
  AHardwareBuffer_describe(ah_buffer, &ah_buffer_describe);
  ASSERT_EQ(ah_buffer_describe.format, kFormat)
      << "The format of the android hardware buffer is wrong.";
  ASSERT_EQ(ah_buffer_describe.layers, kLayerCount)
      << "The obtained android hardware buffer should have 2 layers.";
  ASSERT_EQ(ah_buffer_describe.width, width)
      << "The obtained android hardware buffer width is wrong.";
  ASSERT_EQ(ah_buffer_describe.height, height)
      << "The obtained android hardware buffer height is wrong.";
  // Change the content of the android hardware buffer.
  void* buffer_data = nullptr;
  int32_t fence = -1;
  ret = AHardwareBuffer_lock(ah_buffer, AHARDWAREBUFFER_USAGE_CPU_WRITE_OFTEN,
                             fence, nullptr, &buffer_data);
  ASSERT_EQ(0, ret) << "Failed to lock the hardware buffer.";
  ASSERT_NE(nullptr, buffer_data) << "Buffer data should not be null.";

  uint32_t num_pixels = width * height / num_colors;
  for (uint32_t color_index = 0; color_index < num_colors - 1; ++color_index) {
    uint32_t color_texture = color_textures[color_index];
    for (uint32_t i = 0; i < num_pixels; ++i) {
      memcpy(reinterpret_cast<void*>(reinterpret_cast<int64_t>(buffer_data) +
                                     (i + num_pixels * color_index) *
                                         sizeof(color_texture)),
             &color_texture, sizeof(color_texture));
    }
  }
  uint32_t color_texture = color_textures[num_colors - 1];
  uint32_t num_colored_pixels = num_pixels * (num_colors - 1);
  num_pixels = width * height - num_colored_pixels;
  for (uint32_t i = 0; i < num_pixels; ++i) {
    memcpy(reinterpret_cast<void*>(reinterpret_cast<int64_t>(buffer_data) +
                                   (i + num_colored_pixels) *
                                       sizeof(color_texture)),
           &color_texture, sizeof(color_texture));
  }
  fence = -1;
  ret = AHardwareBuffer_unlock(ah_buffer, &fence);
  EXPECT_EQ(0, ret) << "Failed to unlock the hardware buffer.";

  // Release the android hardware buffer.
  AHardwareBuffer_release(ah_buffer);
}
