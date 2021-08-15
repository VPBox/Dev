#include <android/log.h>
#include <android/native_window.h>
#include <dvr/dvr_api.h>
#include <dvr/dvr_buffer_queue.h>

#include <gtest/gtest.h>

#include <array>
#include <unordered_map>

#include "dvr_api_test.h"

#define LOG_TAG "dvr_buffer_queue-test"

#ifndef ALOGD
#define ALOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#endif

#ifndef ALOGD_IF
#define ALOGD_IF(cond, ...) \
  ((__predict_false(cond)) ? ((void)ALOGD(__VA_ARGS__)) : (void)0)
#endif

namespace {

static constexpr uint32_t kBufferWidth = 100;
static constexpr uint32_t kBufferHeight = 1;
static constexpr uint32_t kLayerCount = 1;
static constexpr uint32_t kBufferFormat = AHARDWAREBUFFER_FORMAT_BLOB;
static constexpr uint64_t kBufferUsage = AHARDWAREBUFFER_USAGE_CPU_READ_OFTEN;
static constexpr size_t kQueueCapacity = 3;

class DvrBufferQueueTest : public DvrApiTest {
 public:
  static void BufferAvailableCallback(void* context) {
    DvrBufferQueueTest* thiz = static_cast<DvrBufferQueueTest*>(context);
    thiz->HandleBufferAvailable();
  }

  static void BufferRemovedCallback(DvrReadBuffer* buffer, void* context) {
    DvrBufferQueueTest* thiz = static_cast<DvrBufferQueueTest*>(context);
    thiz->HandleBufferRemoved(buffer);
  }

 protected:
  void TearDown() override {
    if (write_queue_ != nullptr) {
      api_.WriteBufferQueueDestroy(write_queue_);
      write_queue_ = nullptr;
    }
    DvrApiTest::TearDown();
  }

  void HandleBufferAvailable() {
    buffer_available_count_ += 1;
    ALOGD_IF(TRACE, "Buffer avaiable, count=%d", buffer_available_count_);
  }

  void HandleBufferRemoved(DvrReadBuffer* buffer) {
    buffer_removed_count_ += 1;
    ALOGD_IF(TRACE, "Buffer removed, buffer=%p, count=%d", buffer,
             buffer_removed_count_);
  }

  DvrWriteBufferQueue* write_queue_ = nullptr;
  int buffer_available_count_{0};
  int buffer_removed_count_{0};
};

TEST_F(DvrBufferQueueTest, WriteQueueCreateDestroy) {
  int ret = api_.WriteBufferQueueCreate(
      kBufferWidth, kBufferHeight, kBufferFormat, kLayerCount, kBufferUsage,
      /*capacity=*/0, sizeof(DvrNativeBufferMetadata), &write_queue_);
  ASSERT_EQ(0, ret);

  api_.WriteBufferQueueDestroy(write_queue_);
  write_queue_ = nullptr;
}

TEST_F(DvrBufferQueueTest, WriteQueueGetCapacity) {
  int ret = api_.WriteBufferQueueCreate(
      kBufferWidth, kBufferHeight, kBufferFormat, kLayerCount, kBufferUsage,
      kQueueCapacity, sizeof(DvrNativeBufferMetadata), &write_queue_);
  ASSERT_EQ(0, ret);

  size_t capacity = api_.WriteBufferQueueGetCapacity(write_queue_);

  ALOGD_IF(TRACE, "TestWrite_QueueGetCapacity, capacity=%zu", capacity);
  ASSERT_EQ(kQueueCapacity, capacity);
}

TEST_F(DvrBufferQueueTest, CreateReadQueueFromWriteQueue) {
  int ret = api_.WriteBufferQueueCreate(
      kBufferWidth, kBufferHeight, kBufferFormat, kLayerCount, kBufferUsage,
      /*capacity=*/0, sizeof(DvrNativeBufferMetadata), &write_queue_);
  ASSERT_EQ(0, ret);

  DvrReadBufferQueue* read_queue = nullptr;
  ret = api_.WriteBufferQueueCreateReadQueue(write_queue_, &read_queue);

  ASSERT_EQ(0, ret);
  ASSERT_NE(nullptr, read_queue);

  api_.ReadBufferQueueDestroy(read_queue);
}

TEST_F(DvrBufferQueueTest, CreateReadQueueFromReadQueue) {
  int ret = api_.WriteBufferQueueCreate(
      kBufferWidth, kBufferHeight, kBufferFormat, kLayerCount, kBufferUsage,
      /*capacity=*/0, sizeof(DvrNativeBufferMetadata), &write_queue_);
  ASSERT_EQ(0, ret);

  DvrReadBufferQueue* read_queue1 = nullptr;
  DvrReadBufferQueue* read_queue2 = nullptr;
  ret = api_.WriteBufferQueueCreateReadQueue(write_queue_, &read_queue1);

  ASSERT_EQ(0, ret);
  ASSERT_NE(nullptr, read_queue1);

  ret = api_.ReadBufferQueueCreateReadQueue(read_queue1, &read_queue2);
  ASSERT_EQ(0, ret);
  ASSERT_NE(nullptr, read_queue2);
  ASSERT_NE(read_queue1, read_queue2);

  api_.ReadBufferQueueDestroy(read_queue1);
  api_.ReadBufferQueueDestroy(read_queue2);
}

TEST_F(DvrBufferQueueTest, GainBuffer) {
  int ret = api_.WriteBufferQueueCreate(
      kBufferWidth, kBufferHeight, kBufferFormat, kLayerCount, kBufferUsage,
      kQueueCapacity, sizeof(DvrNativeBufferMetadata), &write_queue_);
  ASSERT_EQ(ret, 0);

  DvrWriteBuffer* wb = nullptr;
  EXPECT_FALSE(api_.WriteBufferIsValid(wb));

  DvrNativeBufferMetadata meta;
  int fence_fd = -1;
  ret = api_.WriteBufferQueueGainBuffer(write_queue_, /*timeout=*/0, &wb, &meta,
                                        &fence_fd);
  ASSERT_EQ(ret, 0);
  EXPECT_EQ(fence_fd, -1);
  EXPECT_NE(wb, nullptr);
  EXPECT_TRUE(api_.WriteBufferIsValid(wb));
}

TEST_F(DvrBufferQueueTest, AcquirePostGainRelease) {
  int ret = api_.WriteBufferQueueCreate(
      kBufferWidth, kBufferHeight, kBufferFormat, kLayerCount, kBufferUsage,
      kQueueCapacity, sizeof(DvrNativeBufferMetadata), &write_queue_);
  ASSERT_EQ(ret, 0);

  DvrReadBufferQueue* read_queue = nullptr;
  DvrReadBuffer* rb = nullptr;
  DvrWriteBuffer* wb = nullptr;
  DvrNativeBufferMetadata meta1;
  DvrNativeBufferMetadata meta2;
  int fence_fd = -1;

  ret = api_.WriteBufferQueueCreateReadQueue(write_queue_, &read_queue);

  ASSERT_EQ(ret, 0);
  ASSERT_NE(read_queue, nullptr);

  api_.ReadBufferQueueSetBufferAvailableCallback(
      read_queue, &BufferAvailableCallback, this);

  // Gain buffer for writing.
  ret = api_.WriteBufferQueueGainBuffer(write_queue_, /*timeout=*/0, &wb,
                                        &meta1, &fence_fd);
  ASSERT_EQ(ret, 0);
  ASSERT_NE(wb, nullptr);
  ASSERT_TRUE(api_.WriteBufferIsValid(wb));
  ALOGD_IF(TRACE, "TestDequeuePostDequeueRelease, gain buffer %p, fence_fd=%d",
           wb, fence_fd);
  close(fence_fd);

  // Post buffer to the read_queue.
  meta1.timestamp = 42;
  ret = api_.WriteBufferQueuePostBuffer(write_queue_, wb, &meta1, /*fence=*/-1);
  ASSERT_EQ(ret, 0);
  ASSERT_FALSE(api_.WriteBufferIsValid(wb));
  wb = nullptr;

  // Acquire buffer for reading.
  ret = api_.ReadBufferQueueAcquireBuffer(read_queue, /*timeout=*/10, &rb,
                                          &meta2, &fence_fd);
  ASSERT_EQ(ret, 0);
  ASSERT_NE(rb, nullptr);

  // Dequeue is successfully, BufferAvailableCallback should be fired once.
  ASSERT_EQ(buffer_available_count_, 1);
  ASSERT_TRUE(api_.ReadBufferIsValid(rb));

  // Metadata should be passed along from producer to consumer properly.
  ASSERT_EQ(meta1.timestamp, meta2.timestamp);

  ALOGD_IF(TRACE,
           "TestDequeuePostDequeueRelease, acquire buffer %p, fence_fd=%d", rb,
           fence_fd);
  close(fence_fd);

  // Release buffer to the write_queue.
  ret = api_.ReadBufferQueueReleaseBuffer(read_queue, rb, &meta2,
                                          /*release_fence_fd=*/-1);
  ASSERT_EQ(ret, 0);
  ASSERT_FALSE(api_.ReadBufferIsValid(rb));
  rb = nullptr;

  // TODO(b/34387835) Currently buffer allocation has to happen after all queues
  // are initialized.
  size_t capacity = api_.ReadBufferQueueGetCapacity(read_queue);

  ALOGD_IF(TRACE, "TestDequeuePostDequeueRelease, capacity=%zu", capacity);
  ASSERT_EQ(kQueueCapacity, capacity);

  api_.ReadBufferQueueDestroy(read_queue);
}

TEST_F(DvrBufferQueueTest, GetANativeWindow) {
  int ret = api_.WriteBufferQueueCreate(
      kBufferWidth, kBufferHeight, kBufferFormat, kLayerCount, kBufferUsage,
      /*capacity=*/0, /*user_metadata_size=*/0, &write_queue_);
  ASSERT_EQ(0, ret);
  ASSERT_NE(nullptr, write_queue_);

  ANativeWindow* window = nullptr;
  ret = api_.WriteBufferQueueGetANativeWindow(write_queue_, &window);
  ASSERT_EQ(0, ret);
  ASSERT_NE(nullptr, window);

  uint32_t width = ANativeWindow_getWidth(window);
  uint32_t height = ANativeWindow_getHeight(window);
  uint32_t format = ANativeWindow_getFormat(window);
  ASSERT_EQ(kBufferWidth, width);
  ASSERT_EQ(kBufferHeight, height);
  ASSERT_EQ(kBufferFormat, format);
}

// Create buffer queue of three buffers and dequeue three buffers out of it.
// Before each dequeue operation, we resize the buffer queue and expect the
// queue always return buffer with desired dimension.
TEST_F(DvrBufferQueueTest, ResizeBuffer) {
  int ret = api_.WriteBufferQueueCreate(
      kBufferWidth, kBufferHeight, kBufferFormat, kLayerCount, kBufferUsage,
      kQueueCapacity, sizeof(DvrNativeBufferMetadata), &write_queue_);
  ASSERT_EQ(0, ret);

  int fence_fd = -1;

  DvrNativeBufferMetadata meta;
  DvrReadBufferQueue* read_queue = nullptr;
  DvrWriteBuffer* wb1 = nullptr;
  DvrWriteBuffer* wb2 = nullptr;
  DvrWriteBuffer* wb3 = nullptr;
  AHardwareBuffer* ahb1 = nullptr;
  AHardwareBuffer* ahb2 = nullptr;
  AHardwareBuffer* ahb3 = nullptr;
  AHardwareBuffer_Desc buffer_desc;

  ret = api_.WriteBufferQueueCreateReadQueue(write_queue_, &read_queue);

  ASSERT_EQ(0, ret);
  ASSERT_NE(nullptr, read_queue);

  api_.ReadBufferQueueSetBufferRemovedCallback(read_queue,
                                               &BufferRemovedCallback, this);

  // Handle all pending events on the read queue.
  ret = api_.ReadBufferQueueHandleEvents(read_queue);
  ASSERT_EQ(0, ret);

  size_t capacity = api_.ReadBufferQueueGetCapacity(read_queue);
  ALOGD_IF(TRACE, "TestResizeBuffer, capacity=%zu", capacity);
  ASSERT_EQ(kQueueCapacity, capacity);

  // Resize before dequeuing.
  constexpr uint32_t w1 = 10;
  ret = api_.WriteBufferQueueResizeBuffer(write_queue_, w1, kBufferHeight);
  ASSERT_EQ(0, ret);

  // Gain first buffer for writing. All buffers will be resized.
  ret = api_.WriteBufferQueueGainBuffer(write_queue_, /*timeout=*/0, &wb1,
                                        &meta, &fence_fd);
  ASSERT_EQ(0, ret);
  ASSERT_TRUE(api_.WriteBufferIsValid(wb1));
  ALOGD_IF(TRACE, "TestResizeBuffer, gain buffer %p", wb1);
  close(fence_fd);

  // Check the buffer dimension.
  ret = api_.WriteBufferGetAHardwareBuffer(wb1, &ahb1);
  ASSERT_EQ(0, ret);
  AHardwareBuffer_describe(ahb1, &buffer_desc);
  ASSERT_EQ(w1, buffer_desc.width);
  ASSERT_EQ(kBufferHeight, buffer_desc.height);
  AHardwareBuffer_release(ahb1);

  // For the first resize, all buffers are reallocated.
  int expected_buffer_removed_count = kQueueCapacity;
  ret = api_.ReadBufferQueueHandleEvents(read_queue);
  ASSERT_EQ(0, ret);
  ASSERT_EQ(expected_buffer_removed_count, buffer_removed_count_);

  // Resize the queue. We are testing with blob format, keep height to be 1.
  constexpr uint32_t w2 = 20;
  ret = api_.WriteBufferQueueResizeBuffer(write_queue_, w2, kBufferHeight);
  ASSERT_EQ(0, ret);

  // The next buffer we dequeued should have new width.
  ret = api_.WriteBufferQueueGainBuffer(write_queue_, /*timeout=*/0, &wb2,
                                        &meta, &fence_fd);
  ASSERT_EQ(0, ret);
  ASSERT_TRUE(api_.WriteBufferIsValid(wb2));
  ALOGD_IF(TRACE, "TestResizeBuffer, gain buffer %p, fence_fd=%d", wb2,
           fence_fd);
  close(fence_fd);

  // Check the buffer dimension, should be new width
  ret = api_.WriteBufferGetAHardwareBuffer(wb2, &ahb2);
  ASSERT_EQ(0, ret);
  AHardwareBuffer_describe(ahb2, &buffer_desc);
  ASSERT_EQ(w2, buffer_desc.width);
  AHardwareBuffer_release(ahb2);

  // For the second resize, all but one buffers are reallocated.
  expected_buffer_removed_count += (kQueueCapacity - 1);
  ret = api_.ReadBufferQueueHandleEvents(read_queue);
  ASSERT_EQ(0, ret);
  ASSERT_EQ(expected_buffer_removed_count, buffer_removed_count_);

  // Resize the queue for the third time.
  constexpr uint32_t w3 = 30;
  ret = api_.WriteBufferQueueResizeBuffer(write_queue_, w3, kBufferHeight);
  ASSERT_EQ(0, ret);

  // The next buffer we dequeued should have new width.
  ret = api_.WriteBufferQueueGainBuffer(write_queue_, /*timeout=*/0, &wb3,
                                        &meta, &fence_fd);
  ASSERT_EQ(0, ret);
  ASSERT_TRUE(api_.WriteBufferIsValid(wb3));
  ALOGD_IF(TRACE, "TestResizeBuffer, gain buffer %p, fence_fd=%d", wb3,
           fence_fd);
  close(fence_fd);

  // Check the buffer dimension, should be new width
  ret = api_.WriteBufferGetAHardwareBuffer(wb3, &ahb3);
  ASSERT_EQ(0, ret);
  AHardwareBuffer_describe(ahb3, &buffer_desc);
  ASSERT_EQ(w3, buffer_desc.width);
  AHardwareBuffer_release(ahb3);

  // For the third resize, all but two buffers are reallocated.
  expected_buffer_removed_count += (kQueueCapacity - 2);
  ret = api_.ReadBufferQueueHandleEvents(read_queue);
  ASSERT_EQ(0, ret);
  ASSERT_EQ(expected_buffer_removed_count, buffer_removed_count_);

  api_.ReadBufferQueueDestroy(read_queue);
}

TEST_F(DvrBufferQueueTest, ReadQueueEventFd) {
  int ret = api_.WriteBufferQueueCreate(
      kBufferWidth, kBufferHeight, kBufferFormat, kLayerCount, kBufferUsage,
      kQueueCapacity, sizeof(DvrNativeBufferMetadata), &write_queue_);
  ASSERT_EQ(0, ret);

  DvrReadBufferQueue* read_queue = nullptr;
  ret = api_.WriteBufferQueueCreateReadQueue(write_queue_, &read_queue);

  ASSERT_EQ(0, ret);
  ASSERT_NE(nullptr, read_queue);

  int event_fd = api_.ReadBufferQueueGetEventFd(read_queue);
  ASSERT_GT(event_fd, 0);
}

// Verifies a Dvr{Read,Write}BufferQueue contains the same set of
// Dvr{Read,Write}Buffer(s) during their lifecycles. And for the same buffer_id,
// the corresponding AHardwareBuffer handle stays the same.
TEST_F(DvrBufferQueueTest, StableBufferIdAndHardwareBuffer) {
  int ret = api_.WriteBufferQueueCreate(
      kBufferWidth, kBufferHeight, kBufferFormat, kLayerCount, kBufferUsage,
      kQueueCapacity, sizeof(DvrNativeBufferMetadata), &write_queue_);
  ASSERT_EQ(0, ret);

  int fence_fd = -1;
  DvrReadBufferQueue* read_queue = nullptr;
  EXPECT_EQ(0, api_.WriteBufferQueueCreateReadQueue(write_queue_, &read_queue));

  // Read buffers.
  std::array<DvrReadBuffer*, kQueueCapacity> rbs;
  // Write buffers.
  std::array<DvrWriteBuffer*, kQueueCapacity> wbs;
  // Buffer metadata.
  std::array<DvrNativeBufferMetadata, kQueueCapacity> metas;
  // Hardware buffers for Read buffers.
  std::unordered_map<int, AHardwareBuffer*> rhbs;
  // Hardware buffers for Write buffers.
  std::unordered_map<int, AHardwareBuffer*> whbs;

  constexpr int kNumTests = 100;

  // This test runs the following operations many many times. Thus we prefer to
  // use ASSERT_XXX rather than EXPECT_XXX to avoid spamming the output.
  std::function<void(size_t i)> Gain = [&](size_t i) {
    int ret = api_.WriteBufferQueueGainBuffer(write_queue_, /*timeout=*/10,
                                              &wbs[i], &metas[i], &fence_fd);
    ASSERT_EQ(ret, 0);
    ASSERT_LT(fence_fd, 0);  // expect invalid fence.
    ASSERT_TRUE(api_.WriteBufferIsValid(wbs[i]));
    int buffer_id = api_.WriteBufferGetId(wbs[i]);
    ASSERT_GT(buffer_id, 0);

    AHardwareBuffer* hb = nullptr;
    ASSERT_EQ(0, api_.WriteBufferGetAHardwareBuffer(wbs[i], &hb));

    auto whb_it = whbs.find(buffer_id);
    if (whb_it == whbs.end()) {
      // If this is a new buffer id, check that total number of unique
      // hardware buffers won't exceed queue capacity.
      ASSERT_LT(whbs.size(), kQueueCapacity);
      whbs.emplace(buffer_id, hb);
    } else {
      // If this is a buffer id we have seen before, check that the
      // buffer_id maps to the same AHardwareBuffer handle.
      ASSERT_EQ(hb, whb_it->second);
    }
  };

  std::function<void(size_t i)> Post = [&](size_t i) {
    ASSERT_TRUE(api_.WriteBufferIsValid(wbs[i]));

    metas[i].timestamp++;
    int ret = api_.WriteBufferQueuePostBuffer(write_queue_, wbs[i], &metas[i],
                                              /*fence=*/-1);
    ASSERT_EQ(ret, 0);
  };

  std::function<void(size_t i)> Acquire = [&](size_t i) {
    int ret = api_.ReadBufferQueueAcquireBuffer(read_queue, /*timeout=*/10,
                                                &rbs[i], &metas[i], &fence_fd);
    ASSERT_EQ(ret, 0);
    ASSERT_LT(fence_fd, 0);  // expect invalid fence.
    ASSERT_TRUE(api_.ReadBufferIsValid(rbs[i]));

    int buffer_id = api_.ReadBufferGetId(rbs[i]);
    ASSERT_GT(buffer_id, 0);

    AHardwareBuffer* hb = nullptr;
    ASSERT_EQ(0, api_.ReadBufferGetAHardwareBuffer(rbs[i], &hb));

    auto rhb_it = rhbs.find(buffer_id);
    if (rhb_it == rhbs.end()) {
      // If this is a new buffer id, check that total number of unique hardware
      // buffers won't exceed queue capacity.
      ASSERT_LT(rhbs.size(), kQueueCapacity);
      rhbs.emplace(buffer_id, hb);
    } else {
      // If this is a buffer id we have seen before, check that the buffer_id
      // maps to the same AHardwareBuffer handle.
      ASSERT_EQ(hb, rhb_it->second);
    }
  };

  std::function<void(size_t i)> Release = [&](size_t i) {
    ASSERT_TRUE(api_.ReadBufferIsValid(rbs[i]));

    int ret = api_.ReadBufferQueueReleaseBuffer(read_queue, rbs[i], &metas[i],
                                                /*release_fence_fd=*/-1);
    ASSERT_EQ(ret, 0);
  };

  // Scenario one:
  for (int i = 0; i < kNumTests; i++) {
    // Gain all write buffers.
    for (size_t i = 0; i < kQueueCapacity; i++) {
      ASSERT_NO_FATAL_FAILURE(Gain(i));
    }
    // Post all write buffers.
    for (size_t i = 0; i < kQueueCapacity; i++) {
      ASSERT_NO_FATAL_FAILURE(Post(i));
    }
    // Acquire all read buffers.
    for (size_t i = 0; i < kQueueCapacity; i++) {
      ASSERT_NO_FATAL_FAILURE(Acquire(i));
    }
    // Release all read buffers.
    for (size_t i = 0; i < kQueueCapacity; i++) {
      ASSERT_NO_FATAL_FAILURE(Release(i));
    }
  }

  // Scenario two:
  for (int i = 0; i < kNumTests; i++) {
    // Gain and post all write buffers.
    for (size_t i = 0; i < kQueueCapacity; i++) {
      ASSERT_NO_FATAL_FAILURE(Gain(i));
      ASSERT_NO_FATAL_FAILURE(Post(i));
    }
    // Acquire and release all read buffers.
    for (size_t i = 0; i < kQueueCapacity; i++) {
      ASSERT_NO_FATAL_FAILURE(Acquire(i));
      ASSERT_NO_FATAL_FAILURE(Release(i));
    }
  }

  // Scenario three:
  for (int i = 0; i < kNumTests; i++) {
    // Gain all write buffers then post them in reversed order.
    for (size_t i = 0; i < kQueueCapacity; i++) {
      ASSERT_NO_FATAL_FAILURE(Gain(i));
    }
    for (size_t i = 0; i < kQueueCapacity; i++) {
      ASSERT_NO_FATAL_FAILURE(Post(kQueueCapacity - 1 - i));
    }

    // Acquire all write buffers then release them in reversed order.
    for (size_t i = 0; i < kQueueCapacity; i++) {
      ASSERT_NO_FATAL_FAILURE(Acquire(i));
    }
    for (size_t i = 0; i < kQueueCapacity; i++) {
      ASSERT_NO_FATAL_FAILURE(Release(kQueueCapacity - 1 - i));
    }
  }
}

TEST_F(DvrBufferQueueTest, ConsumerReleaseAfterProducerDestroy) {
  int ret = api_.WriteBufferQueueCreate(
      kBufferWidth, kBufferHeight, kBufferFormat, kLayerCount, kBufferUsage,
      kQueueCapacity, sizeof(DvrNativeBufferMetadata), &write_queue_);
  ASSERT_EQ(ret, 0);

  DvrReadBufferQueue* read_queue = nullptr;
  DvrReadBuffer* rb = nullptr;
  DvrWriteBuffer* wb = nullptr;
  DvrNativeBufferMetadata meta1;
  DvrNativeBufferMetadata meta2;
  int fence_fd = -1;

  ret = api_.WriteBufferQueueCreateReadQueue(write_queue_, &read_queue);
  ASSERT_EQ(ret, 0);

  api_.ReadBufferQueueSetBufferAvailableCallback(
      read_queue, &BufferAvailableCallback, this);

  // Gain buffer for writing.
  ret = api_.WriteBufferQueueGainBuffer(write_queue_, /*timeout=*/0, &wb,
                                        &meta1, &fence_fd);
  ASSERT_EQ(ret, 0);
  close(fence_fd);

  // Post buffer to the read_queue.
  ret = api_.WriteBufferQueuePostBuffer(write_queue_, wb, &meta1, /*fence=*/-1);
  ASSERT_EQ(ret, 0);
  wb = nullptr;

  // Acquire buffer for reading.
  ret = api_.ReadBufferQueueAcquireBuffer(read_queue, /*timeout=*/10, &rb,
                                          &meta2, &fence_fd);
  ASSERT_EQ(ret, 0);
  close(fence_fd);

  // Destroy the write buffer queue and make sure the reader queue is picking
  // these events up.
  api_.WriteBufferQueueDestroy(write_queue_);
  ret = api_.ReadBufferQueueHandleEvents(read_queue);
  ASSERT_EQ(0, ret);

  // Release buffer to the write_queue.
  ret = api_.ReadBufferQueueReleaseBuffer(read_queue, rb, &meta2,
                                          /*release_fence_fd=*/-1);
  ASSERT_EQ(ret, 0);
  rb = nullptr;

  api_.ReadBufferQueueDestroy(read_queue);
}

}  // namespace
