#include <android/hardware_buffer.h>
#include <dvr/dvr_buffer.h>
#include <dvr/dvr_config.h>
#include <dvr/dvr_shared_buffers.h>
#include <dvr/dvr_surface.h>
#include <system/graphics.h>

#include <gtest/gtest.h>

namespace android {
namespace dvr {

namespace {

TEST(DvrGlobalBufferTest, TestGlobalBuffersSameName) {
  const DvrGlobalBufferKey buffer_key = 101;
  DvrBuffer* buffer1 = nullptr;
  int ret1 = dvrSetupGlobalBuffer(buffer_key, 10, 0, &buffer1);
  ASSERT_EQ(0, ret1);
  ASSERT_NE(nullptr, buffer1);

  DvrBuffer* buffer2 = nullptr;
  int ret2 = dvrSetupGlobalBuffer(buffer_key, 10, 0, &buffer2);
  ASSERT_EQ(0, ret2);
  ASSERT_NE(nullptr, buffer2);

  AHardwareBuffer* hardware_buffer1 = nullptr;
  int e1 = dvrBufferGetAHardwareBuffer(buffer1, &hardware_buffer1);
  ASSERT_EQ(0, e1);
  ASSERT_NE(nullptr, hardware_buffer1);

  AHardwareBuffer* hardware_buffer2 = nullptr;
  int e2 = dvrBufferGetAHardwareBuffer(buffer2, &hardware_buffer2);
  ASSERT_EQ(0, e2);
  ASSERT_NE(nullptr, hardware_buffer2);

  AHardwareBuffer_Desc desc1 = {};
  AHardwareBuffer_describe(hardware_buffer1, &desc1);
  AHardwareBuffer_Desc desc2 = {};
  AHardwareBuffer_describe(hardware_buffer2, &desc2);
  ASSERT_EQ(desc1.width, 10u);
  ASSERT_EQ(desc1.height, 1u);
  ASSERT_EQ(desc1.layers, 1u);
  ASSERT_EQ(desc1.format, HAL_PIXEL_FORMAT_BLOB);
  ASSERT_EQ(desc1.usage, 0u);
  ASSERT_EQ(desc2.width, 10u);
  ASSERT_EQ(desc2.height, 1u);
  ASSERT_EQ(desc2.layers, 1u);
  ASSERT_EQ(desc2.format, HAL_PIXEL_FORMAT_BLOB);
  ASSERT_EQ(desc2.usage, 0u);

  dvrBufferDestroy(buffer1);
  dvrBufferDestroy(buffer2);

  DvrBuffer* buffer3 = nullptr;
  int e3 = dvrGetGlobalBuffer(buffer_key, &buffer3);
  ASSERT_NE(nullptr, buffer3);
  ASSERT_EQ(0, e3);

  AHardwareBuffer* hardware_buffer3 = nullptr;
  int e4 = dvrBufferGetAHardwareBuffer(buffer3, &hardware_buffer3);
  ASSERT_EQ(0, e4);
  ASSERT_NE(nullptr, hardware_buffer3);

  AHardwareBuffer_Desc desc3 = {};
  AHardwareBuffer_describe(hardware_buffer3, &desc3);
  ASSERT_EQ(desc3.width, 10u);
  ASSERT_EQ(desc3.height, 1u);
  ASSERT_EQ(desc3.layers, 1u);
  ASSERT_EQ(desc3.format, HAL_PIXEL_FORMAT_BLOB);
  ASSERT_EQ(desc3.usage, 0u);

  dvrBufferDestroy(buffer3);

  AHardwareBuffer_release(hardware_buffer1);
  AHardwareBuffer_release(hardware_buffer2);
  AHardwareBuffer_release(hardware_buffer3);
}

TEST(DvrGlobalBufferTest, TestMultipleGlobalBuffers) {
  const DvrGlobalBufferKey buffer_key1 = 102;
  const DvrGlobalBufferKey buffer_key2 = 103;
  DvrBuffer* setup_buffer1 = nullptr;
  int ret1 = dvrSetupGlobalBuffer(buffer_key1, 10, 0, &setup_buffer1);
  ASSERT_EQ(0, ret1);
  ASSERT_NE(nullptr, setup_buffer1);
  dvrBufferDestroy(setup_buffer1);

  DvrBuffer* setup_buffer2 = nullptr;
  int ret2 = dvrSetupGlobalBuffer(buffer_key2, 10, 0, &setup_buffer2);
  ASSERT_EQ(0, ret2);
  ASSERT_NE(nullptr, setup_buffer2);
  dvrBufferDestroy(setup_buffer2);

  DvrBuffer* buffer1 = nullptr;
  int e1 = dvrGetGlobalBuffer(buffer_key1, &buffer1);
  ASSERT_NE(nullptr, buffer1);
  ASSERT_EQ(0, e1);
  dvrBufferDestroy(buffer1);

  DvrBuffer* buffer2 = nullptr;
  int e2 = dvrGetGlobalBuffer(buffer_key2, &buffer2);
  ASSERT_NE(nullptr, buffer2);
  ASSERT_EQ(0, e2);
  dvrBufferDestroy(buffer2);
}

TEST(DvrGlobalBufferTest, TestGlobalBufferUsage) {
  const DvrGlobalBufferKey buffer_key = 100;

  // Set usage to AHARDWAREBUFFER_USAGE_VIDEO_ENCODE. We use this because
  // internally AHARDWAREBUFFER_USAGE_VIDEO_ENCODE is converted to
  // GRALLOC1_CONSUMER_USAGE_VIDEO_ENCODER, and these two values are different.
  // If all is good, when we get the AHardwareBuffer, it should be converted
  // back to AHARDWAREBUFFER_USAGE_VIDEO_ENCODE.
  const uint64_t usage = AHARDWAREBUFFER_USAGE_VIDEO_ENCODE;

  DvrBuffer* setup_buffer = nullptr;
  int e1 = dvrSetupGlobalBuffer(buffer_key, 10, usage, &setup_buffer);
  ASSERT_NE(nullptr, setup_buffer);
  ASSERT_EQ(0, e1);

  AHardwareBuffer* hardware_buffer = nullptr;
  int e2 = dvrBufferGetAHardwareBuffer(setup_buffer, &hardware_buffer);
  ASSERT_EQ(0, e2);
  ASSERT_NE(nullptr, hardware_buffer);

  AHardwareBuffer_Desc desc = {};
  AHardwareBuffer_describe(hardware_buffer, &desc);
  ASSERT_EQ(usage, desc.usage);

  dvrBufferDestroy(setup_buffer);
  AHardwareBuffer_release(hardware_buffer);
}

TEST(DvrGlobalBufferTest, TestGlobalBufferCarriesData) {
  const DvrGlobalBufferKey buffer_name = 110;

  uint64_t usage = AHARDWAREBUFFER_USAGE_CPU_READ_OFTEN |
                   AHARDWAREBUFFER_USAGE_CPU_WRITE_OFTEN;
  constexpr size_t size = 1024 * sizeof(uint64_t);
  constexpr uint64_t value = 0x123456787654321;

  {
    // Allocate some data and set it to something.
    DvrBuffer* setup_buffer = nullptr;
    int e1 = dvrSetupGlobalBuffer(buffer_name, size, usage, &setup_buffer);
    ASSERT_NE(nullptr, setup_buffer);
    ASSERT_EQ(0, e1);

    AHardwareBuffer* hardware_buffer = nullptr;
    int e2 = dvrBufferGetAHardwareBuffer(setup_buffer, &hardware_buffer);
    ASSERT_EQ(0, e2);
    ASSERT_NE(nullptr, hardware_buffer);

    void* buffer;
    int e3 = AHardwareBuffer_lock(hardware_buffer, usage, -1, nullptr, &buffer);
    ASSERT_EQ(0, e3);
    ASSERT_NE(nullptr, buffer);
    // Verify that the buffer pointer is at least 16 byte aligned.
    ASSERT_EQ(0U, reinterpret_cast<uintptr_t>(buffer) & (16 - 1));

    uint64_t* data = static_cast<uint64_t*>(buffer);
    constexpr size_t num_values = size / sizeof(uint64_t);
    for (size_t i = 0; i < num_values; ++i) {
      data[i] = value;
    }

    int32_t fence = -1;
    int e4 = AHardwareBuffer_unlock(hardware_buffer, &fence);
    ASSERT_EQ(0, e4);

    dvrBufferDestroy(setup_buffer);
    AHardwareBuffer_release(hardware_buffer);
  }

  {
    // Get the buffer and check that all the data is still present.
    DvrBuffer* setup_buffer = nullptr;
    int e1 = dvrGetGlobalBuffer(buffer_name, &setup_buffer);
    ASSERT_NE(nullptr, setup_buffer);
    ASSERT_EQ(0, e1);

    AHardwareBuffer* hardware_buffer = nullptr;
    int e2 = dvrBufferGetAHardwareBuffer(setup_buffer, &hardware_buffer);
    ASSERT_EQ(0, e2);
    ASSERT_NE(nullptr, hardware_buffer);

    void* buffer;
    int e3 = AHardwareBuffer_lock(hardware_buffer, usage, -1, nullptr, &buffer);
    ASSERT_EQ(0, e3);
    ASSERT_NE(nullptr, buffer);
    // Verify that the buffer pointer is at least 16 byte aligned.
    ASSERT_EQ(0U, reinterpret_cast<uintptr_t>(buffer) & (16 - 1));

    uint64_t* data = static_cast<uint64_t*>(buffer);
    constexpr size_t num_values = size / sizeof(uint64_t);
    bool is_equal = true;
    for (size_t i = 0; i < num_values; ++i) {
      is_equal &= (data[i] == value);
    }
    ASSERT_TRUE(is_equal);

    int32_t fence = -1;
    int e4 = AHardwareBuffer_unlock(hardware_buffer, &fence);
    ASSERT_EQ(0, e4);

    dvrBufferDestroy(setup_buffer);
    AHardwareBuffer_release(hardware_buffer);
  }
}

TEST(DvrGlobalBufferTest, TestGlobalBufferZeroed) {
  const DvrGlobalBufferKey buffer_name = 120;

  // Allocate 1MB and check that it is all zeros.
  uint64_t usage = AHARDWAREBUFFER_USAGE_CPU_READ_OFTEN |
                   AHARDWAREBUFFER_USAGE_CPU_WRITE_OFTEN;
  constexpr size_t size = 1024 * 1024;
  DvrBuffer* setup_buffer = nullptr;
  int e1 = dvrSetupGlobalBuffer(buffer_name, size, usage, &setup_buffer);
  ASSERT_NE(nullptr, setup_buffer);
  ASSERT_EQ(0, e1);

  AHardwareBuffer* hardware_buffer = nullptr;
  int e2 = dvrBufferGetAHardwareBuffer(setup_buffer, &hardware_buffer);
  ASSERT_EQ(0, e2);
  ASSERT_NE(nullptr, hardware_buffer);

  void* buffer;
  int e3 = AHardwareBuffer_lock(hardware_buffer, usage, -1, nullptr, &buffer);
  ASSERT_EQ(0, e3);
  ASSERT_NE(nullptr, buffer);
  // Verify that the buffer pointer is at least 16 byte aligned.
  ASSERT_EQ(0U, reinterpret_cast<uintptr_t>(buffer) & (16 - 1));

  uint64_t* data = static_cast<uint64_t*>(buffer);
  constexpr size_t num_values = size / sizeof(uint64_t);
  uint64_t zero = 0;
  for (size_t i = 0; i < num_values; ++i) {
    zero |= data[i];
  }
  ASSERT_EQ(0U, zero);

  int32_t fence = -1;
  int e4 = AHardwareBuffer_unlock(hardware_buffer, &fence);
  ASSERT_EQ(0, e4);

  dvrBufferDestroy(setup_buffer);
  AHardwareBuffer_release(hardware_buffer);
}

TEST(DvrGlobalBufferTest, TestVrflingerConfigBuffer) {
  const DvrGlobalBufferKey buffer_name =
      DvrGlobalBuffers::kVrFlingerConfigBufferKey;

  // First delete any existing buffer so we can test the failure case.
  dvrDeleteGlobalBuffer(buffer_name);

  const uint64_t usage = AHARDWAREBUFFER_USAGE_CPU_READ_OFTEN |
                         AHARDWAREBUFFER_USAGE_CPU_WRITE_RARELY;

  size_t correct_size = DvrConfigRing::MemorySize();
  size_t wrong_size = DvrConfigRing::MemorySize(0);

  // Setup an invalid config buffer (too small) and assert that it fails.
  DvrBuffer* setup_buffer = nullptr;
  int e1 = dvrSetupGlobalBuffer(buffer_name, wrong_size, usage, &setup_buffer);
  ASSERT_EQ(nullptr, setup_buffer);
  ASSERT_GT(0, e1);

  // Setup a correct config buffer.
  int e2 =
      dvrSetupGlobalBuffer(buffer_name, correct_size, usage, &setup_buffer);
  ASSERT_NE(nullptr, setup_buffer);
  ASSERT_EQ(0, e2);

  dvrBufferDestroy(setup_buffer);
}

}  // namespace

}  // namespace dvr
}  // namespace android
