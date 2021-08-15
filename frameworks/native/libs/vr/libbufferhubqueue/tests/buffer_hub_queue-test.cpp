#include <base/logging.h>
#include <binder/Parcel.h>
#include <dvr/dvr_api.h>
#include <private/dvr/buffer_hub_queue_client.h>
#include <private/dvr/consumer_buffer.h>
#include <private/dvr/producer_buffer.h>

#include <gtest/gtest.h>
#include <poll.h>
#include <sys/eventfd.h>

#include <vector>

// Enable/disable debug logging.
#define TRACE 0

namespace android {
namespace dvr {

using pdx::LocalChannelHandle;
using pdx::LocalHandle;

namespace {

constexpr uint32_t kBufferWidth = 100;
constexpr uint32_t kBufferHeight = 1;
constexpr uint32_t kBufferLayerCount = 1;
constexpr uint32_t kBufferFormat = HAL_PIXEL_FORMAT_BLOB;
constexpr uint64_t kBufferUsage = GRALLOC_USAGE_SW_READ_RARELY;
constexpr int kTimeoutMs = 100;
constexpr int kNoTimeout = 0;

class BufferHubQueueTest : public ::testing::Test {
 public:
  bool CreateProducerQueue(const ProducerQueueConfig& config,
                           const UsagePolicy& usage) {
    producer_queue_ = ProducerQueue::Create(config, usage);
    return producer_queue_ != nullptr;
  }

  bool CreateConsumerQueue() {
    if (producer_queue_) {
      consumer_queue_ = producer_queue_->CreateConsumerQueue();
      return consumer_queue_ != nullptr;
    } else {
      return false;
    }
  }

  bool CreateQueues(const ProducerQueueConfig& config,
                    const UsagePolicy& usage) {
    return CreateProducerQueue(config, usage) && CreateConsumerQueue();
  }

  void AllocateBuffer(size_t* slot_out = nullptr) {
    // Create producer buffer.
    auto status = producer_queue_->AllocateBuffer(kBufferWidth, kBufferHeight,
                                                  kBufferLayerCount,
                                                  kBufferFormat, kBufferUsage);

    ASSERT_TRUE(status.ok());
    size_t slot = status.take();
    if (slot_out)
      *slot_out = slot;
  }

  bool WaitAndHandleOnce(BufferHubQueue* queue, int timeout_ms) {
    pollfd pfd{queue->queue_fd(), POLLIN, 0};
    int ret;
    do {
      ret = poll(&pfd, 1, timeout_ms);
    } while (ret == -1 && errno == EINTR);

    if (ret < 0) {
      ALOGW("Failed to poll queue %d's event fd, error: %s.", queue->id(),
            strerror(errno));
      return false;
    } else if (ret == 0) {
      return false;
    }
    return queue->HandleQueueEvents();
  }

 protected:
  ProducerQueueConfigBuilder config_builder_;
  std::unique_ptr<ProducerQueue> producer_queue_;
  std::unique_ptr<ConsumerQueue> consumer_queue_;
};

TEST_F(BufferHubQueueTest, TestDequeue) {
  const int64_t nb_dequeue_times = 16;

  ASSERT_TRUE(CreateQueues(config_builder_.Build(), UsagePolicy{}));

  // Allocate only one buffer.
  AllocateBuffer();

  // But dequeue multiple times.
  for (int64_t i = 0; i < nb_dequeue_times; i++) {
    size_t slot;
    LocalHandle fence;
    DvrNativeBufferMetadata mi, mo;

    // Producer gains a buffer.
    auto p1_status = producer_queue_->Dequeue(kTimeoutMs, &slot, &mo, &fence);
    EXPECT_TRUE(p1_status.ok());
    auto p1 = p1_status.take();
    ASSERT_NE(p1, nullptr);

    // Producer posts the buffer.
    mi.index = i;
    EXPECT_EQ(p1->PostAsync(&mi, LocalHandle()), 0);

    // Consumer acquires a buffer.
    auto c1_status = consumer_queue_->Dequeue(kTimeoutMs, &slot, &mo, &fence);
    EXPECT_TRUE(c1_status.ok()) << c1_status.GetErrorMessage();
    auto c1 = c1_status.take();
    ASSERT_NE(c1, nullptr);
    EXPECT_EQ(mi.index, i);
    EXPECT_EQ(mo.index, i);

    // Consumer releases the buffer.
    EXPECT_EQ(c1->ReleaseAsync(&mi, LocalHandle()), 0);
  }
}

TEST_F(BufferHubQueueTest,
       TestDequeuePostedBufferIfNoAvailableReleasedBuffer_withConsumerBuffer) {
  ASSERT_TRUE(CreateQueues(config_builder_.Build(), UsagePolicy{}));

  // Allocate 3 buffers to use.
  const size_t test_queue_capacity = 3;
  for (int64_t i = 0; i < test_queue_capacity; i++) {
    AllocateBuffer();
  }
  EXPECT_EQ(producer_queue_->capacity(), test_queue_capacity);

  size_t producer_slot, consumer_slot;
  LocalHandle fence;
  DvrNativeBufferMetadata mi, mo;

  // Producer posts 2 buffers and remember their posted sequence.
  std::deque<size_t> posted_slots;
  for (int64_t i = 0; i < 2; i++) {
    auto p1_status =
        producer_queue_->Dequeue(kTimeoutMs, &producer_slot, &mo, &fence, true);
    EXPECT_TRUE(p1_status.ok());
    auto p1 = p1_status.take();
    ASSERT_NE(p1, nullptr);

    // Producer should not be gaining posted buffer when there are still
    // available buffers to gain.
    auto found_iter =
        std::find(posted_slots.begin(), posted_slots.end(), producer_slot);
    EXPECT_EQ(found_iter, posted_slots.end());
    posted_slots.push_back(producer_slot);

    // Producer posts the buffer.
    mi.index = i;
    EXPECT_EQ(0, p1->PostAsync(&mi, LocalHandle()));
  }

  // Consumer acquires one buffer.
  auto c1_status =
      consumer_queue_->Dequeue(kTimeoutMs, &consumer_slot, &mo, &fence);
  EXPECT_TRUE(c1_status.ok());
  auto c1 = c1_status.take();
  ASSERT_NE(c1, nullptr);
  // Consumer should get the oldest posted buffer. No checks here.
  // posted_slots[0] should be in acquired state now.
  EXPECT_EQ(mo.index, 0);
  // Consumer releases the buffer.
  EXPECT_EQ(c1->ReleaseAsync(&mi, LocalHandle()), 0);
  // posted_slots[0] should be in released state now.

  // Producer gain and post 2 buffers.
  for (int64_t i = 0; i < 2; i++) {
    auto p1_status =
        producer_queue_->Dequeue(kTimeoutMs, &producer_slot, &mo, &fence, true);
    EXPECT_TRUE(p1_status.ok());
    auto p1 = p1_status.take();
    ASSERT_NE(p1, nullptr);

    // The gained buffer should be the one in released state or the one haven't
    // been use.
    EXPECT_NE(posted_slots[1], producer_slot);

    mi.index = i + 2;
    EXPECT_EQ(0, p1->PostAsync(&mi, LocalHandle()));
  }

  // Producer gains a buffer.
  auto p1_status =
      producer_queue_->Dequeue(kTimeoutMs, &producer_slot, &mo, &fence, true);
  EXPECT_TRUE(p1_status.ok());
  auto p1 = p1_status.take();
  ASSERT_NE(p1, nullptr);

  // The gained buffer should be the oldest posted buffer.
  EXPECT_EQ(posted_slots[1], producer_slot);

  // Producer posts the buffer.
  mi.index = 4;
  EXPECT_EQ(0, p1->PostAsync(&mi, LocalHandle()));
}

TEST_F(BufferHubQueueTest,
       TestDequeuePostedBufferIfNoAvailableReleasedBuffer_noConsumerBuffer) {
  ASSERT_TRUE(CreateQueues(config_builder_.Build(), UsagePolicy{}));

  // Allocate 4 buffers to use.
  const size_t test_queue_capacity = 4;
  for (int64_t i = 0; i < test_queue_capacity; i++) {
    AllocateBuffer();
  }
  EXPECT_EQ(producer_queue_->capacity(), test_queue_capacity);

  // Post all allowed buffers and remember their posted sequence.
  std::deque<size_t> posted_slots;
  for (int64_t i = 0; i < test_queue_capacity; i++) {
    size_t slot;
    LocalHandle fence;
    DvrNativeBufferMetadata mi, mo;

    // Producer gains a buffer.
    auto p1_status =
        producer_queue_->Dequeue(kTimeoutMs, &slot, &mo, &fence, true);
    EXPECT_TRUE(p1_status.ok());
    auto p1 = p1_status.take();
    ASSERT_NE(p1, nullptr);

    // Producer should not be gaining posted buffer when there are still
    // available buffers to gain.
    auto found_iter = std::find(posted_slots.begin(), posted_slots.end(), slot);
    EXPECT_EQ(found_iter, posted_slots.end());
    posted_slots.push_back(slot);

    // Producer posts the buffer.
    mi.index = i;
    EXPECT_EQ(p1->PostAsync(&mi, LocalHandle()), 0);
  }

  // Gain posted buffers in sequence.
  const int64_t nb_dequeue_all_times = 2;
  for (int j = 0; j < nb_dequeue_all_times; ++j) {
    for (int i = 0; i < test_queue_capacity; ++i) {
      size_t slot;
      LocalHandle fence;
      DvrNativeBufferMetadata mi, mo;

      // Producer gains a buffer.
      auto p1_status =
          producer_queue_->Dequeue(kTimeoutMs, &slot, &mo, &fence, true);
      EXPECT_TRUE(p1_status.ok());
      auto p1 = p1_status.take();
      ASSERT_NE(p1, nullptr);

      // The gained buffer should be the oldest posted buffer.
      EXPECT_EQ(posted_slots[i], slot);

      // Producer posts the buffer.
      mi.index = i + test_queue_capacity * (j + 1);
      EXPECT_EQ(p1->PostAsync(&mi, LocalHandle()), 0);
    }
  }
}

TEST_F(BufferHubQueueTest, TestProducerConsumer) {
  const size_t kBufferCount = 16;
  size_t slot;
  DvrNativeBufferMetadata mi, mo;
  LocalHandle fence;

  ASSERT_TRUE(CreateQueues(config_builder_.Build(), UsagePolicy{}));

  for (size_t i = 0; i < kBufferCount; i++) {
    AllocateBuffer();

    // Producer queue has all the available buffers on initialize.
    ASSERT_EQ(producer_queue_->count(), i + 1);
    ASSERT_EQ(producer_queue_->capacity(), i + 1);

    // Consumer queue has no avaiable buffer on initialize.
    ASSERT_EQ(consumer_queue_->count(), 0U);
    // Consumer queue does not import buffers until a dequeue is issued.
    ASSERT_EQ(consumer_queue_->capacity(), i);
    // Dequeue returns timeout since no buffer is ready to consumer, but
    // this implicitly triggers buffer import and bump up |capacity|.
    auto status = consumer_queue_->Dequeue(kNoTimeout, &slot, &mo, &fence);
    ASSERT_FALSE(status.ok());
    ASSERT_EQ(ETIMEDOUT, status.error());
    ASSERT_EQ(consumer_queue_->capacity(), i + 1);
  }

  // Use eventfd as a stand-in for a fence.
  LocalHandle post_fence(eventfd(0, EFD_CLOEXEC | EFD_NONBLOCK));

  for (size_t i = 0; i < kBufferCount; i++) {
    // First time there is no buffer available to dequeue.
    auto consumer_status =
        consumer_queue_->Dequeue(kNoTimeout, &slot, &mo, &fence);
    ASSERT_FALSE(consumer_status.ok());
    ASSERT_EQ(consumer_status.error(), ETIMEDOUT);

    // Make sure Producer buffer is POSTED so that it's ready to Accquire
    // in the consumer's Dequeue() function.
    auto producer_status =
        producer_queue_->Dequeue(kTimeoutMs, &slot, &mo, &fence);
    ASSERT_TRUE(producer_status.ok());
    auto producer = producer_status.take();
    ASSERT_NE(nullptr, producer);

    mi.index = static_cast<int64_t>(i);
    ASSERT_EQ(producer->PostAsync(&mi, post_fence), 0);

    // Second time the just the POSTED buffer should be dequeued.
    consumer_status = consumer_queue_->Dequeue(kTimeoutMs, &slot, &mo, &fence);
    ASSERT_TRUE(consumer_status.ok());
    EXPECT_TRUE(fence.IsValid());

    auto consumer = consumer_status.take();
    ASSERT_NE(nullptr, consumer);
    ASSERT_EQ(mi.index, mo.index);
  }
}

TEST_F(BufferHubQueueTest, TestInsertBuffer) {
  ASSERT_TRUE(CreateProducerQueue(config_builder_.Build(), UsagePolicy{}));

  consumer_queue_ = producer_queue_->CreateConsumerQueue();
  ASSERT_TRUE(consumer_queue_ != nullptr);
  EXPECT_EQ(producer_queue_->capacity(), 0);
  EXPECT_EQ(consumer_queue_->capacity(), 0);

  std::shared_ptr<ProducerBuffer> p1 = ProducerBuffer::Create(
      kBufferWidth, kBufferHeight, kBufferFormat, kBufferUsage, 0);
  ASSERT_TRUE(p1 != nullptr);
  ASSERT_EQ(p1->GainAsync(), 0);

  // Inserting a posted buffer will fail.
  DvrNativeBufferMetadata meta;
  EXPECT_EQ(p1->PostAsync(&meta, LocalHandle()), 0);
  auto status_or_slot = producer_queue_->InsertBuffer(p1);
  EXPECT_FALSE(status_or_slot.ok());
  EXPECT_EQ(status_or_slot.error(), EINVAL);

  // Inserting a gained buffer will succeed.
  std::shared_ptr<ProducerBuffer> p2 = ProducerBuffer::Create(
      kBufferWidth, kBufferHeight, kBufferFormat, kBufferUsage);
  ASSERT_EQ(p2->GainAsync(), 0);
  ASSERT_TRUE(p2 != nullptr);
  status_or_slot = producer_queue_->InsertBuffer(p2);
  EXPECT_TRUE(status_or_slot.ok()) << status_or_slot.GetErrorMessage();
  // This is the first buffer inserted, should take slot 0.
  size_t slot = status_or_slot.get();
  EXPECT_EQ(slot, 0);

  // Wait and expect the consumer to kick up the newly inserted buffer.
  WaitAndHandleOnce(consumer_queue_.get(), kTimeoutMs);
  EXPECT_EQ(consumer_queue_->capacity(), 1ULL);
}

TEST_F(BufferHubQueueTest, TestRemoveBuffer) {
  ASSERT_TRUE(CreateProducerQueue(config_builder_.Build(), UsagePolicy{}));
  DvrNativeBufferMetadata mo;

  // Allocate buffers.
  const size_t kBufferCount = 4u;
  for (size_t i = 0; i < kBufferCount; i++) {
    AllocateBuffer();
  }
  ASSERT_EQ(kBufferCount, producer_queue_->count());
  ASSERT_EQ(kBufferCount, producer_queue_->capacity());

  consumer_queue_ = producer_queue_->CreateConsumerQueue();
  ASSERT_NE(nullptr, consumer_queue_);

  // Check that buffers are correctly imported on construction.
  EXPECT_EQ(kBufferCount, consumer_queue_->capacity());
  EXPECT_EQ(0u, consumer_queue_->count());

  // Dequeue all the buffers and keep track of them in an array. This prevents
  // the producer queue ring buffer ref counts from interfering with the tests.
  struct Entry {
    std::shared_ptr<ProducerBuffer> buffer;
    LocalHandle fence;
    size_t slot;
  };
  std::array<Entry, kBufferCount> buffers;

  for (size_t i = 0; i < kBufferCount; i++) {
    Entry* entry = &buffers[i];
    auto producer_status =
        producer_queue_->Dequeue(kTimeoutMs, &entry->slot, &mo, &entry->fence);
    ASSERT_TRUE(producer_status.ok());
    entry->buffer = producer_status.take();
    ASSERT_NE(nullptr, entry->buffer);
  }

  // Remove a buffer and make sure both queues reflect the change.
  ASSERT_TRUE(producer_queue_->RemoveBuffer(buffers[0].slot));
  EXPECT_EQ(kBufferCount - 1, producer_queue_->capacity());

  // As long as the removed buffer is still alive the consumer queue won't know
  // its gone.
  EXPECT_EQ(kBufferCount, consumer_queue_->capacity());
  EXPECT_FALSE(consumer_queue_->HandleQueueEvents());
  EXPECT_EQ(kBufferCount, consumer_queue_->capacity());

  // Release the removed buffer.
  buffers[0].buffer = nullptr;

  // Now the consumer queue should know it's gone.
  EXPECT_FALSE(WaitAndHandleOnce(consumer_queue_.get(), kTimeoutMs));
  ASSERT_EQ(kBufferCount - 1, consumer_queue_->capacity());

  // Allocate a new buffer. This should take the first empty slot.
  size_t slot;
  AllocateBuffer(&slot);
  ALOGE_IF(TRACE, "ALLOCATE %zu", slot);
  EXPECT_EQ(buffers[0].slot, slot);
  EXPECT_EQ(kBufferCount, producer_queue_->capacity());

  // The consumer queue should pick up the new buffer.
  EXPECT_EQ(kBufferCount - 1, consumer_queue_->capacity());
  EXPECT_FALSE(consumer_queue_->HandleQueueEvents());
  EXPECT_EQ(kBufferCount, consumer_queue_->capacity());

  // Remove and allocate a buffer.
  ASSERT_TRUE(producer_queue_->RemoveBuffer(buffers[1].slot));
  EXPECT_EQ(kBufferCount - 1, producer_queue_->capacity());
  buffers[1].buffer = nullptr;

  AllocateBuffer(&slot);
  ALOGE_IF(TRACE, "ALLOCATE %zu", slot);
  EXPECT_EQ(buffers[1].slot, slot);
  EXPECT_EQ(kBufferCount, producer_queue_->capacity());

  // The consumer queue should pick up the new buffer but the count shouldn't
  // change.
  EXPECT_EQ(kBufferCount, consumer_queue_->capacity());
  EXPECT_FALSE(consumer_queue_->HandleQueueEvents());
  EXPECT_EQ(kBufferCount, consumer_queue_->capacity());

  // Remove and allocate a buffer, but don't free the buffer right away.
  ASSERT_TRUE(producer_queue_->RemoveBuffer(buffers[2].slot));
  EXPECT_EQ(kBufferCount - 1, producer_queue_->capacity());

  AllocateBuffer(&slot);
  ALOGE_IF(TRACE, "ALLOCATE %zu", slot);
  EXPECT_EQ(buffers[2].slot, slot);
  EXPECT_EQ(kBufferCount, producer_queue_->capacity());

  EXPECT_EQ(kBufferCount, consumer_queue_->capacity());
  EXPECT_FALSE(consumer_queue_->HandleQueueEvents());
  EXPECT_EQ(kBufferCount, consumer_queue_->capacity());

  // Release the producer buffer to trigger a POLLHUP event for an already
  // removed buffer.
  buffers[2].buffer = nullptr;
  EXPECT_EQ(kBufferCount, consumer_queue_->capacity());
  EXPECT_FALSE(consumer_queue_->HandleQueueEvents());
  EXPECT_EQ(kBufferCount, consumer_queue_->capacity());
}

TEST_F(BufferHubQueueTest, TestMultipleConsumers) {
  // ProducerConfigureBuilder doesn't set Metadata{size}, which means there
  // is no metadata associated with this BufferQueue's buffer.
  ASSERT_TRUE(CreateProducerQueue(config_builder_.Build(), UsagePolicy{}));

  // Allocate buffers.
  const size_t kBufferCount = 4u;
  for (size_t i = 0; i < kBufferCount; i++) {
    AllocateBuffer();
  }
  ASSERT_EQ(kBufferCount, producer_queue_->count());

  // Build a silent consumer queue to test multi-consumer queue features.
  auto silent_queue = producer_queue_->CreateSilentConsumerQueue();
  ASSERT_NE(nullptr, silent_queue);

  // Check that silent queue doesn't import buffers on creation.
  EXPECT_EQ(silent_queue->capacity(), 0U);

  // Dequeue and post a buffer.
  size_t slot;
  LocalHandle fence;
  DvrNativeBufferMetadata mi, mo;
  auto producer_status =
      producer_queue_->Dequeue(kTimeoutMs, &slot, &mo, &fence);
  EXPECT_TRUE(producer_status.ok());
  auto producer_buffer = producer_status.take();
  ASSERT_NE(producer_buffer, nullptr);
  EXPECT_EQ(producer_buffer->PostAsync(&mi, {}), 0);
  // After post, check the number of remaining available buffers.
  EXPECT_EQ(producer_queue_->count(), kBufferCount - 1);

  // Currently we expect no buffer to be available prior to calling
  // WaitForBuffers/HandleQueueEvents.
  // TODO(eieio): Note this behavior may change in the future.
  EXPECT_EQ(silent_queue->count(), 0U);
  EXPECT_FALSE(silent_queue->HandleQueueEvents());
  EXPECT_EQ(silent_queue->count(), 0U);

  // Build a new consumer queue to test multi-consumer queue features.
  consumer_queue_ = silent_queue->CreateConsumerQueue();
  ASSERT_NE(consumer_queue_, nullptr);

  // Check that buffers are correctly imported on construction.
  EXPECT_EQ(consumer_queue_->capacity(), kBufferCount);
  // Buffers are only imported, but their availability is not checked until
  // first call to Dequeue().
  EXPECT_EQ(consumer_queue_->count(), 0U);

  // Reclaim released/ignored buffers.
  EXPECT_EQ(producer_queue_->count(), kBufferCount - 1);

  usleep(10000);
  WaitAndHandleOnce(producer_queue_.get(), kTimeoutMs);
  EXPECT_EQ(producer_queue_->count(), kBufferCount - 1);

  // Post another buffer.
  producer_status = producer_queue_->Dequeue(kTimeoutMs, &slot, &mo, &fence);
  EXPECT_TRUE(producer_status.ok());
  producer_buffer = producer_status.take();
  ASSERT_NE(producer_buffer, nullptr);
  EXPECT_EQ(producer_buffer->PostAsync(&mi, {}), 0);

  // Verify that the consumer queue receives it.
  size_t consumer_queue_count = consumer_queue_->count();
  WaitAndHandleOnce(consumer_queue_.get(), kTimeoutMs);
  EXPECT_GT(consumer_queue_->count(), consumer_queue_count);

  // Save the current consumer queue buffer count to compare after the dequeue.
  consumer_queue_count = consumer_queue_->count();

  // Dequeue and acquire/release (discard) buffers on the consumer end.
  auto consumer_status =
      consumer_queue_->Dequeue(kTimeoutMs, &slot, &mo, &fence);
  EXPECT_TRUE(consumer_status.ok());
  auto consumer_buffer = consumer_status.take();
  ASSERT_NE(consumer_buffer, nullptr);
  consumer_buffer->Discard();

  // Buffer should be returned to the producer queue without being handled by
  // the silent consumer queue.
  EXPECT_LT(consumer_queue_->count(), consumer_queue_count);
  EXPECT_EQ(producer_queue_->count(), kBufferCount - 2);

  WaitAndHandleOnce(producer_queue_.get(), kTimeoutMs);
  EXPECT_EQ(producer_queue_->count(), kBufferCount - 1);
}

struct TestUserMetadata {
  char a;
  int32_t b;
  int64_t c;
};

constexpr uint64_t kUserMetadataSize =
    static_cast<uint64_t>(sizeof(TestUserMetadata));

TEST_F(BufferHubQueueTest, TestUserMetadata) {
  ASSERT_TRUE(CreateQueues(
      config_builder_.SetMetadata<TestUserMetadata>().Build(), UsagePolicy{}));

  AllocateBuffer();

  std::vector<TestUserMetadata> user_metadata_list = {
      {'0', 0, 0}, {'1', 10, 3333}, {'@', 123, 1000000000}};

  for (auto user_metadata : user_metadata_list) {
    size_t slot;
    LocalHandle fence;
    DvrNativeBufferMetadata mi, mo;

    auto p1_status = producer_queue_->Dequeue(kTimeoutMs, &slot, &mo, &fence);
    EXPECT_TRUE(p1_status.ok());
    auto p1 = p1_status.take();
    ASSERT_NE(p1, nullptr);

    // TODO(b/69469185): Test against metadata from consumer once we implement
    // release metadata properly.
    // EXPECT_EQ(mo.user_metadata_ptr, 0U);
    // EXPECT_EQ(mo.user_metadata_size, 0U);

    mi.user_metadata_size = kUserMetadataSize;
    mi.user_metadata_ptr = reinterpret_cast<uint64_t>(&user_metadata);
    EXPECT_EQ(p1->PostAsync(&mi, {}), 0);
    auto c1_status = consumer_queue_->Dequeue(kTimeoutMs, &slot, &mo, &fence);
    EXPECT_TRUE(c1_status.ok()) << c1_status.GetErrorMessage();
    auto c1 = c1_status.take();
    ASSERT_NE(c1, nullptr);

    EXPECT_EQ(mo.user_metadata_size, kUserMetadataSize);
    auto out_user_metadata =
        reinterpret_cast<TestUserMetadata*>(mo.user_metadata_ptr);
    EXPECT_EQ(user_metadata.a, out_user_metadata->a);
    EXPECT_EQ(user_metadata.b, out_user_metadata->b);
    EXPECT_EQ(user_metadata.c, out_user_metadata->c);

    // When release, empty metadata is also legit.
    mi.user_metadata_size = 0U;
    mi.user_metadata_ptr = 0U;
    c1->ReleaseAsync(&mi, {});
  }
}

TEST_F(BufferHubQueueTest, TestUserMetadataMismatch) {
  ASSERT_TRUE(CreateQueues(
      config_builder_.SetMetadata<TestUserMetadata>().Build(), UsagePolicy{}));

  AllocateBuffer();

  TestUserMetadata user_metadata;
  size_t slot;
  LocalHandle fence;
  DvrNativeBufferMetadata mi, mo;
  auto p1_status = producer_queue_->Dequeue(kTimeoutMs, &slot, &mo, &fence);
  EXPECT_TRUE(p1_status.ok());
  auto p1 = p1_status.take();
  ASSERT_NE(p1, nullptr);

  // Post with mismatched user metadata size will fail. But the producer buffer
  // itself should stay untouched.
  mi.user_metadata_ptr = reinterpret_cast<uint64_t>(&user_metadata);
  mi.user_metadata_size = kUserMetadataSize + 1;
  EXPECT_EQ(p1->PostAsync(&mi, {}), -E2BIG);
  // Post with the exact same user metdata size can success.
  mi.user_metadata_ptr = reinterpret_cast<uint64_t>(&user_metadata);
  mi.user_metadata_size = kUserMetadataSize;
  EXPECT_EQ(p1->PostAsync(&mi, {}), 0);
}

TEST_F(BufferHubQueueTest, TestEnqueue) {
  ASSERT_TRUE(CreateQueues(config_builder_.SetMetadata<int64_t>().Build(),
                           UsagePolicy{}));
  AllocateBuffer();

  size_t slot;
  LocalHandle fence;
  DvrNativeBufferMetadata mo;
  auto p1_status = producer_queue_->Dequeue(kTimeoutMs, &slot, &mo, &fence);
  ASSERT_TRUE(p1_status.ok());
  auto p1 = p1_status.take();
  ASSERT_NE(nullptr, p1);

  producer_queue_->Enqueue(p1, slot, 0ULL);
  auto c1_status = consumer_queue_->Dequeue(kTimeoutMs, &slot, &mo, &fence);
  ASSERT_FALSE(c1_status.ok());
}

TEST_F(BufferHubQueueTest, TestAllocateBuffer) {
  ASSERT_TRUE(CreateQueues(config_builder_.Build(), UsagePolicy{}));

  size_t ps1;
  AllocateBuffer();
  LocalHandle fence;
  DvrNativeBufferMetadata mi, mo;
  auto p1_status = producer_queue_->Dequeue(kTimeoutMs, &ps1, &mo, &fence);
  ASSERT_TRUE(p1_status.ok());
  auto p1 = p1_status.take();
  ASSERT_NE(p1, nullptr);

  // producer queue is exhausted
  size_t ps2;
  auto p2_status = producer_queue_->Dequeue(kTimeoutMs, &ps2, &mo, &fence);
  ASSERT_FALSE(p2_status.ok());
  ASSERT_EQ(ETIMEDOUT, p2_status.error());

  // dynamically add buffer.
  AllocateBuffer();
  ASSERT_EQ(producer_queue_->count(), 1U);
  ASSERT_EQ(producer_queue_->capacity(), 2U);

  // now we can dequeue again
  p2_status = producer_queue_->Dequeue(kTimeoutMs, &ps2, &mo, &fence);
  ASSERT_TRUE(p2_status.ok());
  auto p2 = p2_status.take();
  ASSERT_NE(p2, nullptr);
  ASSERT_EQ(producer_queue_->count(), 0U);
  // p1 and p2 should have different slot number
  ASSERT_NE(ps1, ps2);

  // Consumer queue does not import buffers until |Dequeue| or |ImportBuffers|
  // are called. So far consumer_queue_ should be empty.
  ASSERT_EQ(consumer_queue_->count(), 0U);

  int64_t seq = 1;
  mi.index = seq;
  ASSERT_EQ(p1->PostAsync(&mi, {}), 0);

  size_t cs1, cs2;
  auto c1_status = consumer_queue_->Dequeue(kTimeoutMs, &cs1, &mo, &fence);
  ASSERT_TRUE(c1_status.ok()) << c1_status.GetErrorMessage();
  auto c1 = c1_status.take();
  ASSERT_NE(c1, nullptr);
  ASSERT_EQ(consumer_queue_->count(), 0U);
  ASSERT_EQ(consumer_queue_->capacity(), 2U);
  ASSERT_EQ(cs1, ps1);

  ASSERT_EQ(p2->PostAsync(&mi, {}), 0);
  auto c2_status = consumer_queue_->Dequeue(kTimeoutMs, &cs2, &mo, &fence);
  ASSERT_TRUE(c2_status.ok());
  auto c2 = c2_status.take();
  ASSERT_NE(c2, nullptr);
  ASSERT_EQ(cs2, ps2);
}

TEST_F(BufferHubQueueTest, TestAllocateTwoBuffers) {
  ASSERT_TRUE(CreateQueues(config_builder_.Build(), UsagePolicy{}));
  ASSERT_EQ(producer_queue_->capacity(), 0);
  auto status = producer_queue_->AllocateBuffers(
      kBufferWidth, kBufferHeight, kBufferLayerCount, kBufferFormat,
      kBufferUsage, /*buffer_count=*/2);
  ASSERT_TRUE(status.ok());
  std::vector<size_t> buffer_slots = status.take();
  ASSERT_EQ(buffer_slots.size(), 2);
  ASSERT_EQ(producer_queue_->capacity(), 2);
}

TEST_F(BufferHubQueueTest, TestAllocateZeroBuffers) {
  ASSERT_TRUE(CreateQueues(config_builder_.Build(), UsagePolicy{}));
  ASSERT_EQ(producer_queue_->capacity(), 0);
  auto status = producer_queue_->AllocateBuffers(
      kBufferWidth, kBufferHeight, kBufferLayerCount, kBufferFormat,
      kBufferUsage, /*buffer_count=*/0);
  ASSERT_TRUE(status.ok());
  std::vector<size_t> buffer_slots = status.take();
  ASSERT_EQ(buffer_slots.size(), 0);
  ASSERT_EQ(producer_queue_->capacity(), 0);
}

TEST_F(BufferHubQueueTest, TestUsageSetMask) {
  const uint32_t set_mask = GRALLOC_USAGE_SW_WRITE_OFTEN;
  ASSERT_TRUE(
      CreateQueues(config_builder_.Build(), UsagePolicy{set_mask, 0, 0, 0}));

  // When allocation, leave out |set_mask| from usage bits on purpose.
  auto status = producer_queue_->AllocateBuffer(
      kBufferWidth, kBufferHeight, kBufferLayerCount, kBufferFormat,
      kBufferUsage & ~set_mask);
  ASSERT_TRUE(status.ok());

  LocalHandle fence;
  size_t slot;
  DvrNativeBufferMetadata mo;
  auto p1_status = producer_queue_->Dequeue(kTimeoutMs, &slot, &mo, &fence);
  ASSERT_TRUE(p1_status.ok());
  auto p1 = p1_status.take();
  ASSERT_EQ(p1->usage() & set_mask, set_mask);
}

TEST_F(BufferHubQueueTest, TestUsageClearMask) {
  const uint32_t clear_mask = GRALLOC_USAGE_SW_WRITE_OFTEN;
  ASSERT_TRUE(
      CreateQueues(config_builder_.Build(), UsagePolicy{0, clear_mask, 0, 0}));

  // When allocation, add |clear_mask| into usage bits on purpose.
  auto status = producer_queue_->AllocateBuffer(
      kBufferWidth, kBufferHeight, kBufferLayerCount, kBufferFormat,
      kBufferUsage | clear_mask);
  ASSERT_TRUE(status.ok());

  LocalHandle fence;
  size_t slot;
  DvrNativeBufferMetadata mo;
  auto p1_status = producer_queue_->Dequeue(kTimeoutMs, &slot, &mo, &fence);
  ASSERT_TRUE(p1_status.ok());
  auto p1 = p1_status.take();
  ASSERT_EQ(p1->usage() & clear_mask, 0U);
}

TEST_F(BufferHubQueueTest, TestUsageDenySetMask) {
  const uint32_t deny_set_mask = GRALLOC_USAGE_SW_WRITE_OFTEN;
  ASSERT_TRUE(CreateQueues(config_builder_.SetMetadata<int64_t>().Build(),
                           UsagePolicy{0, 0, deny_set_mask, 0}));

  // Now that |deny_set_mask| is illegal, allocation without those bits should
  // be able to succeed.
  auto status = producer_queue_->AllocateBuffer(
      kBufferWidth, kBufferHeight, kBufferLayerCount, kBufferFormat,
      kBufferUsage & ~deny_set_mask);
  ASSERT_TRUE(status.ok());

  // While allocation with those bits should fail.
  status = producer_queue_->AllocateBuffer(kBufferWidth, kBufferHeight,
                                           kBufferLayerCount, kBufferFormat,
                                           kBufferUsage | deny_set_mask);
  ASSERT_FALSE(status.ok());
  ASSERT_EQ(EINVAL, status.error());
}

TEST_F(BufferHubQueueTest, TestUsageDenyClearMask) {
  const uint32_t deny_clear_mask = GRALLOC_USAGE_SW_WRITE_OFTEN;
  ASSERT_TRUE(CreateQueues(config_builder_.SetMetadata<int64_t>().Build(),
                           UsagePolicy{0, 0, 0, deny_clear_mask}));

  // Now that clearing |deny_clear_mask| is illegal (i.e. setting these bits are
  // mandatory), allocation with those bits should be able to succeed.
  auto status = producer_queue_->AllocateBuffer(
      kBufferWidth, kBufferHeight, kBufferLayerCount, kBufferFormat,
      kBufferUsage | deny_clear_mask);
  ASSERT_TRUE(status.ok());

  // While allocation without those bits should fail.
  status = producer_queue_->AllocateBuffer(kBufferWidth, kBufferHeight,
                                           kBufferLayerCount, kBufferFormat,
                                           kBufferUsage & ~deny_clear_mask);
  ASSERT_FALSE(status.ok());
  ASSERT_EQ(EINVAL, status.error());
}

TEST_F(BufferHubQueueTest, TestQueueInfo) {
  static const bool kIsAsync = true;
  ASSERT_TRUE(CreateQueues(config_builder_.SetIsAsync(kIsAsync)
                               .SetDefaultWidth(kBufferWidth)
                               .SetDefaultHeight(kBufferHeight)
                               .SetDefaultFormat(kBufferFormat)
                               .Build(),
                           UsagePolicy{}));

  EXPECT_EQ(producer_queue_->default_width(), kBufferWidth);
  EXPECT_EQ(producer_queue_->default_height(), kBufferHeight);
  EXPECT_EQ(producer_queue_->default_format(), kBufferFormat);
  EXPECT_EQ(producer_queue_->is_async(), kIsAsync);

  EXPECT_EQ(consumer_queue_->default_width(), kBufferWidth);
  EXPECT_EQ(consumer_queue_->default_height(), kBufferHeight);
  EXPECT_EQ(consumer_queue_->default_format(), kBufferFormat);
  EXPECT_EQ(consumer_queue_->is_async(), kIsAsync);
}

TEST_F(BufferHubQueueTest, TestFreeAllBuffers) {
  constexpr size_t kBufferCount = 2;

#define CHECK_NO_BUFFER_THEN_ALLOCATE(num_buffers)  \
  EXPECT_EQ(consumer_queue_->count(), 0U);          \
  EXPECT_EQ(consumer_queue_->capacity(), 0U);       \
  EXPECT_EQ(producer_queue_->count(), 0U);          \
  EXPECT_EQ(producer_queue_->capacity(), 0U);       \
  for (size_t i = 0; i < num_buffers; i++) {        \
    AllocateBuffer();                               \
  }                                                 \
  EXPECT_EQ(producer_queue_->count(), num_buffers); \
  EXPECT_EQ(producer_queue_->capacity(), num_buffers);

  size_t slot;
  LocalHandle fence;
  pdx::Status<void> status;
  pdx::Status<std::shared_ptr<ConsumerBuffer>> consumer_status;
  pdx::Status<std::shared_ptr<ProducerBuffer>> producer_status;
  std::shared_ptr<ConsumerBuffer> consumer_buffer;
  std::shared_ptr<ProducerBuffer> producer_buffer;
  DvrNativeBufferMetadata mi, mo;

  ASSERT_TRUE(CreateQueues(config_builder_.Build(), UsagePolicy{}));

  // Free all buffers when buffers are avaible for dequeue.
  CHECK_NO_BUFFER_THEN_ALLOCATE(kBufferCount);
  status = producer_queue_->FreeAllBuffers();
  EXPECT_TRUE(status.ok());

  // Free all buffers when one buffer is dequeued.
  CHECK_NO_BUFFER_THEN_ALLOCATE(kBufferCount);
  producer_status = producer_queue_->Dequeue(kTimeoutMs, &slot, &mo, &fence);
  ASSERT_TRUE(producer_status.ok());
  status = producer_queue_->FreeAllBuffers();
  EXPECT_TRUE(status.ok());

  // Free all buffers when all buffers are dequeued.
  CHECK_NO_BUFFER_THEN_ALLOCATE(kBufferCount);
  for (size_t i = 0; i < kBufferCount; i++) {
    producer_status = producer_queue_->Dequeue(kTimeoutMs, &slot, &mo, &fence);
    ASSERT_TRUE(producer_status.ok());
  }
  status = producer_queue_->FreeAllBuffers();
  EXPECT_TRUE(status.ok());

  // Free all buffers when one buffer is posted.
  CHECK_NO_BUFFER_THEN_ALLOCATE(kBufferCount);
  producer_status = producer_queue_->Dequeue(kTimeoutMs, &slot, &mo, &fence);
  ASSERT_TRUE(producer_status.ok());
  producer_buffer = producer_status.take();
  ASSERT_NE(nullptr, producer_buffer);
  ASSERT_EQ(0, producer_buffer->PostAsync(&mi, fence));
  status = producer_queue_->FreeAllBuffers();
  EXPECT_TRUE(status.ok());

  // Free all buffers when all buffers are posted.
  CHECK_NO_BUFFER_THEN_ALLOCATE(kBufferCount);
  for (size_t i = 0; i < kBufferCount; i++) {
    producer_status = producer_queue_->Dequeue(kTimeoutMs, &slot, &mo, &fence);
    ASSERT_TRUE(producer_status.ok());
    producer_buffer = producer_status.take();
    ASSERT_NE(producer_buffer, nullptr);
    ASSERT_EQ(producer_buffer->PostAsync(&mi, fence), 0);
  }
  status = producer_queue_->FreeAllBuffers();
  EXPECT_TRUE(status.ok());

  // Free all buffers when all buffers are acquired.
  CHECK_NO_BUFFER_THEN_ALLOCATE(kBufferCount);
  for (size_t i = 0; i < kBufferCount; i++) {
    producer_status = producer_queue_->Dequeue(kTimeoutMs, &slot, &mo, &fence);
    ASSERT_TRUE(producer_status.ok());
    producer_buffer = producer_status.take();
    ASSERT_NE(producer_buffer, nullptr);
    ASSERT_EQ(producer_buffer->PostAsync(&mi, fence), 0);
    consumer_status = consumer_queue_->Dequeue(kTimeoutMs, &slot, &mo, &fence);
    ASSERT_TRUE(consumer_status.ok()) << consumer_status.GetErrorMessage();
  }

  status = producer_queue_->FreeAllBuffers();
  EXPECT_TRUE(status.ok());

  // In addition to FreeAllBuffers() from the queue, it is also required to
  // delete all references to the ProducerBuffer (i.e. the PDX client).
  producer_buffer = nullptr;

  // Crank consumer queue events to pickup EPOLLHUP events on the queue.
  consumer_queue_->HandleQueueEvents();

  // One last check.
  CHECK_NO_BUFFER_THEN_ALLOCATE(kBufferCount);

#undef CHECK_NO_BUFFER_THEN_ALLOCATE
}

TEST_F(BufferHubQueueTest, TestProducerToParcelableNotEmpty) {
  ASSERT_TRUE(CreateQueues(config_builder_.SetMetadata<uint64_t>().Build(),
                           UsagePolicy{}));

  // Allocate only one buffer.
  AllocateBuffer();

  // Export should fail as the queue is not empty.
  auto status = producer_queue_->TakeAsParcelable();
  EXPECT_FALSE(status.ok());
}

TEST_F(BufferHubQueueTest, TestProducerExportToParcelable) {
  ASSERT_TRUE(CreateQueues(config_builder_.Build(), UsagePolicy{}));

  auto s1 = producer_queue_->TakeAsParcelable();
  EXPECT_TRUE(s1.ok());

  ProducerQueueParcelable output_parcelable = s1.take();
  EXPECT_TRUE(output_parcelable.IsValid());

  Parcel parcel;
  status_t res;
  res = output_parcelable.writeToParcel(&parcel);
  EXPECT_EQ(res, OK);

  // After written into parcelable, the output_parcelable is still valid has
  // keeps the producer channel alive.
  EXPECT_TRUE(output_parcelable.IsValid());

  // Creating producer buffer should fail.
  auto s2 = producer_queue_->AllocateBuffer(kBufferWidth, kBufferHeight,
                                            kBufferLayerCount, kBufferFormat,
                                            kBufferUsage);
  ASSERT_FALSE(s2.ok());

  // Reset the data position so that we can read back from the same parcel
  // without doing actually Binder IPC.
  parcel.setDataPosition(0);
  producer_queue_ = nullptr;

  // Recreate the producer queue from the parcel.
  ProducerQueueParcelable input_parcelable;
  EXPECT_FALSE(input_parcelable.IsValid());

  res = input_parcelable.readFromParcel(&parcel);
  EXPECT_EQ(res, OK);
  EXPECT_TRUE(input_parcelable.IsValid());

  EXPECT_EQ(producer_queue_, nullptr);
  producer_queue_ = ProducerQueue::Import(input_parcelable.TakeChannelHandle());
  EXPECT_FALSE(input_parcelable.IsValid());
  ASSERT_NE(producer_queue_, nullptr);

  // Newly created queue from the parcel can allocate buffer, post buffer to
  // consumer.
  EXPECT_NO_FATAL_FAILURE(AllocateBuffer());
  EXPECT_EQ(producer_queue_->count(), 1U);
  EXPECT_EQ(producer_queue_->capacity(), 1U);

  size_t slot;
  DvrNativeBufferMetadata producer_meta;
  DvrNativeBufferMetadata consumer_meta;
  LocalHandle fence;
  auto s3 = producer_queue_->Dequeue(0, &slot, &producer_meta, &fence);
  EXPECT_TRUE(s3.ok());

  std::shared_ptr<ProducerBuffer> p1 = s3.take();
  ASSERT_NE(p1, nullptr);

  producer_meta.timestamp = 42;
  EXPECT_EQ(p1->PostAsync(&producer_meta, LocalHandle()), 0);

  // Make sure the buffer can be dequeued from consumer side.
  auto s4 = consumer_queue_->Dequeue(kTimeoutMs, &slot, &consumer_meta, &fence);
  EXPECT_TRUE(s4.ok()) << s4.GetErrorMessage();
  EXPECT_EQ(consumer_queue_->capacity(), 1U);

  auto consumer = s4.take();
  ASSERT_NE(consumer, nullptr);
  EXPECT_EQ(producer_meta.timestamp, consumer_meta.timestamp);
}

TEST_F(BufferHubQueueTest, TestCreateConsumerParcelable) {
  ASSERT_TRUE(CreateProducerQueue(config_builder_.Build(), UsagePolicy{}));

  auto s1 = producer_queue_->CreateConsumerQueueParcelable();
  EXPECT_TRUE(s1.ok());
  ConsumerQueueParcelable output_parcelable = s1.take();
  EXPECT_TRUE(output_parcelable.IsValid());

  // Write to a Parcel new object.
  Parcel parcel;
  status_t res;
  res = output_parcelable.writeToParcel(&parcel);

  // Reset the data position so that we can read back from the same parcel
  // without doing actually Binder IPC.
  parcel.setDataPosition(0);

  // No consumer queue created yet.
  EXPECT_EQ(consumer_queue_, nullptr);

  // If the parcel contains a consumer queue, read into a
  // ProducerQueueParcelable should fail.
  ProducerQueueParcelable wrongly_typed_parcelable;
  EXPECT_FALSE(wrongly_typed_parcelable.IsValid());
  res = wrongly_typed_parcelable.readFromParcel(&parcel);
  EXPECT_EQ(res, -EINVAL);
  parcel.setDataPosition(0);

  // Create the consumer queue from the parcel.
  ConsumerQueueParcelable input_parcelable;
  EXPECT_FALSE(input_parcelable.IsValid());

  res = input_parcelable.readFromParcel(&parcel);
  EXPECT_EQ(res, OK);
  EXPECT_TRUE(input_parcelable.IsValid());

  consumer_queue_ = ConsumerQueue::Import(input_parcelable.TakeChannelHandle());
  EXPECT_FALSE(input_parcelable.IsValid());
  ASSERT_NE(consumer_queue_, nullptr);

  EXPECT_NO_FATAL_FAILURE(AllocateBuffer());
  EXPECT_EQ(producer_queue_->count(), 1U);
  EXPECT_EQ(producer_queue_->capacity(), 1U);

  size_t slot;
  DvrNativeBufferMetadata producer_meta;
  DvrNativeBufferMetadata consumer_meta;
  LocalHandle fence;
  auto s2 = producer_queue_->Dequeue(0, &slot, &producer_meta, &fence);
  EXPECT_TRUE(s2.ok());

  std::shared_ptr<ProducerBuffer> p1 = s2.take();
  ASSERT_NE(p1, nullptr);

  producer_meta.timestamp = 42;
  EXPECT_EQ(p1->PostAsync(&producer_meta, LocalHandle()), 0);

  // Make sure the buffer can be dequeued from consumer side.
  auto s3 = consumer_queue_->Dequeue(kTimeoutMs, &slot, &consumer_meta, &fence);
  EXPECT_TRUE(s3.ok()) << s3.GetErrorMessage();
  EXPECT_EQ(consumer_queue_->capacity(), 1U);

  auto consumer = s3.take();
  ASSERT_NE(consumer, nullptr);
  EXPECT_EQ(producer_meta.timestamp, consumer_meta.timestamp);
}

}  // namespace

}  // namespace dvr
}  // namespace android
