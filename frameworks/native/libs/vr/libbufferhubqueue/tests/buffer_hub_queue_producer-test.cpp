#include <base/logging.h>
#include <gui/BufferHubProducer.h>
#include <gui/IProducerListener.h>
#include <gui/Surface.h>
#include <pdx/default_transport/channel_parcelable.h>

#include <gtest/gtest.h>

namespace android {
namespace dvr {

using pdx::LocalHandle;

namespace {

// Default dimensions before setDefaultBufferSize is called by the consumer.
constexpr uint32_t kDefaultWidth = 1;
constexpr uint32_t kDefaultHeight = 1;

// Default format before setDefaultBufferFormat is called by the consumer.
constexpr PixelFormat kDefaultFormat = HAL_PIXEL_FORMAT_RGBA_8888;
constexpr int kDefaultConsumerUsageBits = 0;

// Default transform hint before setTransformHint is called by the consumer.
constexpr uint32_t kDefaultTransformHint = 0;

constexpr int kTestApi = NATIVE_WINDOW_API_CPU;
constexpr int kTestApiOther = NATIVE_WINDOW_API_EGL;
constexpr int kTestApiInvalid = 0xDEADBEEF;
constexpr int kTestProducerUsageBits = 0;
constexpr bool kTestControlledByApp = true;

// Builder pattern to slightly vary *almost* correct input
// -- avoids copying and pasting
struct QueueBufferInputBuilder {
  IGraphicBufferProducer::QueueBufferInput build() {
    return IGraphicBufferProducer::QueueBufferInput(
        mTimestamp, mIsAutoTimestamp, mDataSpace, mCrop, mScalingMode,
        mTransform, mFence);
  }

  QueueBufferInputBuilder& setTimestamp(int64_t timestamp) {
    this->mTimestamp = timestamp;
    return *this;
  }

  QueueBufferInputBuilder& setIsAutoTimestamp(bool isAutoTimestamp) {
    this->mIsAutoTimestamp = isAutoTimestamp;
    return *this;
  }

  QueueBufferInputBuilder& setDataSpace(android_dataspace dataSpace) {
    this->mDataSpace = dataSpace;
    return *this;
  }

  QueueBufferInputBuilder& setCrop(Rect crop) {
    this->mCrop = crop;
    return *this;
  }

  QueueBufferInputBuilder& setScalingMode(int scalingMode) {
    this->mScalingMode = scalingMode;
    return *this;
  }

  QueueBufferInputBuilder& setTransform(uint32_t transform) {
    this->mTransform = transform;
    return *this;
  }

  QueueBufferInputBuilder& setFence(sp<Fence> fence) {
    this->mFence = fence;
    return *this;
  }

 private:
  int64_t mTimestamp{1384888611};
  bool mIsAutoTimestamp{false};
  android_dataspace mDataSpace{HAL_DATASPACE_UNKNOWN};
  Rect mCrop{Rect(kDefaultWidth, kDefaultHeight)};
  int mScalingMode{0};
  uint32_t mTransform{0};
  sp<Fence> mFence{Fence::NO_FENCE};
};

// This is a test that covers our implementation of bufferhubqueue-based
// IGraphicBufferProducer.
class BufferHubQueueProducerTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    const ::testing::TestInfo* const testInfo =
        ::testing::UnitTest::GetInstance()->current_test_info();
    ALOGD_IF(TRACE, "Begin test: %s.%s", testInfo->test_case_name(),
             testInfo->name());

    auto config = ProducerQueueConfigBuilder().Build();
    auto queue = ProducerQueue::Create(config, UsagePolicy{});
    ASSERT_TRUE(queue != nullptr);

    mProducer = BufferHubProducer::Create(std::move(queue));
    ASSERT_TRUE(mProducer != nullptr);
    mSurface = new Surface(mProducer, true);
    ASSERT_TRUE(mSurface != nullptr);
  }

  // Connect to a producer in a 'correct' fashion.
  void ConnectProducer() {
    IGraphicBufferProducer::QueueBufferOutput output;
    // Can connect the first time.
    ASSERT_EQ(OK, mProducer->connect(kDummyListener, kTestApi,
                                     kTestControlledByApp, &output));
  }

  // Dequeue a buffer in a 'correct' fashion.
  //   Precondition: Producer is connected.
  void DequeueBuffer(int* outSlot) {
    sp<Fence> fence;
    ASSERT_NO_FATAL_FAILURE(DequeueBuffer(outSlot, &fence));
  }

  void DequeueBuffer(int* outSlot, sp<Fence>* outFence) {
    ASSERT_NE(nullptr, outSlot);
    ASSERT_NE(nullptr, outFence);

    int ret = mProducer->dequeueBuffer(
        outSlot, outFence, kDefaultWidth, kDefaultHeight, kDefaultFormat,
        kTestProducerUsageBits, nullptr, nullptr);
    // BUFFER_NEEDS_REALLOCATION can be either on or off.
    ASSERT_EQ(0, ~IGraphicBufferProducer::BUFFER_NEEDS_REALLOCATION & ret);

    // Slot number should be in boundary.
    ASSERT_LE(0, *outSlot);
    ASSERT_GT(BufferQueueDefs::NUM_BUFFER_SLOTS, *outSlot);
  }

  // Create a generic "valid" input for queueBuffer
  // -- uses the default buffer format, width, etc.
  static IGraphicBufferProducer::QueueBufferInput CreateBufferInput() {
    return QueueBufferInputBuilder().build();
  }

  const sp<IProducerListener> kDummyListener{new DummyProducerListener};

  sp<BufferHubProducer> mProducer;
  sp<Surface> mSurface;
};

TEST_F(BufferHubQueueProducerTest, ConnectFirst_ReturnsError) {
  IGraphicBufferProducer::QueueBufferOutput output;

  // NULL output returns BAD_VALUE
  EXPECT_EQ(BAD_VALUE, mProducer->connect(kDummyListener, kTestApi,
                                          kTestControlledByApp, nullptr));

  // Invalid API returns bad value
  EXPECT_EQ(BAD_VALUE, mProducer->connect(kDummyListener, kTestApiInvalid,
                                          kTestControlledByApp, &output));
}

TEST_F(BufferHubQueueProducerTest, ConnectAgain_ReturnsError) {
  ASSERT_NO_FATAL_FAILURE(ConnectProducer());

  // Can't connect when there is already a producer connected.
  IGraphicBufferProducer::QueueBufferOutput output;
  EXPECT_EQ(BAD_VALUE, mProducer->connect(kDummyListener, kTestApi,
                                          kTestControlledByApp, &output));
}

TEST_F(BufferHubQueueProducerTest, Disconnect_Succeeds) {
  ASSERT_NO_FATAL_FAILURE(ConnectProducer());

  ASSERT_EQ(OK, mProducer->disconnect(kTestApi));
}

TEST_F(BufferHubQueueProducerTest, Disconnect_ReturnsError) {
  ASSERT_NO_FATAL_FAILURE(ConnectProducer());

  // Must disconnect with same API number
  EXPECT_EQ(BAD_VALUE, mProducer->disconnect(kTestApiOther));
  // API must not be out of range
  EXPECT_EQ(BAD_VALUE, mProducer->disconnect(kTestApiInvalid));
}

TEST_F(BufferHubQueueProducerTest, Query_Succeeds) {
  ASSERT_NO_FATAL_FAILURE(ConnectProducer());

  int32_t value = -1;
  EXPECT_EQ(OK, mProducer->query(NATIVE_WINDOW_WIDTH, &value));
  EXPECT_EQ(kDefaultWidth, static_cast<uint32_t>(value));

  EXPECT_EQ(OK, mProducer->query(NATIVE_WINDOW_HEIGHT, &value));
  EXPECT_EQ(kDefaultHeight, static_cast<uint32_t>(value));

  EXPECT_EQ(OK, mProducer->query(NATIVE_WINDOW_FORMAT, &value));
  EXPECT_EQ(kDefaultFormat, value);

  EXPECT_EQ(OK, mProducer->query(NATIVE_WINDOW_MIN_UNDEQUEUED_BUFFERS, &value));
  EXPECT_LE(0, value);
  EXPECT_GE(BufferQueueDefs::NUM_BUFFER_SLOTS, value);

  EXPECT_EQ(OK,
            mProducer->query(NATIVE_WINDOW_CONSUMER_RUNNING_BEHIND, &value));
  EXPECT_FALSE(value);  // Can't run behind when we haven't touched the queue

  EXPECT_EQ(OK, mProducer->query(NATIVE_WINDOW_CONSUMER_USAGE_BITS, &value));
  EXPECT_EQ(kDefaultConsumerUsageBits, value);
}

TEST_F(BufferHubQueueProducerTest, Query_ReturnsError) {
  ASSERT_NO_FATAL_FAILURE(ConnectProducer());

  // One past the end of the last 'query' enum value. Update this if we add more
  // enums.
  const int NATIVE_WINDOW_QUERY_LAST_OFF_BY_ONE = NATIVE_WINDOW_BUFFER_AGE + 1;

  int value;
  // What was out of range
  EXPECT_EQ(BAD_VALUE, mProducer->query(/*what*/ -1, &value));
  EXPECT_EQ(BAD_VALUE, mProducer->query(/*what*/ 0xDEADBEEF, &value));
  EXPECT_EQ(BAD_VALUE,
            mProducer->query(NATIVE_WINDOW_QUERY_LAST_OFF_BY_ONE, &value));

  // Some enums from window.h are 'invalid'
  EXPECT_EQ(BAD_VALUE,
            mProducer->query(NATIVE_WINDOW_QUEUES_TO_WINDOW_COMPOSER, &value));
  EXPECT_EQ(BAD_VALUE, mProducer->query(NATIVE_WINDOW_CONCRETE_TYPE, &value));
  EXPECT_EQ(BAD_VALUE, mProducer->query(NATIVE_WINDOW_DEFAULT_WIDTH, &value));
  EXPECT_EQ(BAD_VALUE, mProducer->query(NATIVE_WINDOW_DEFAULT_HEIGHT, &value));
  EXPECT_EQ(BAD_VALUE, mProducer->query(NATIVE_WINDOW_TRANSFORM_HINT, &value));

  // Value was NULL
  EXPECT_EQ(BAD_VALUE, mProducer->query(NATIVE_WINDOW_FORMAT, /*value*/ NULL));
}

TEST_F(BufferHubQueueProducerTest, Queue_Succeeds) {
  int slot = -1;

  ASSERT_NO_FATAL_FAILURE(ConnectProducer());
  ASSERT_NO_FATAL_FAILURE(DequeueBuffer(&slot));

  // Request the buffer (pre-requisite for queueing)
  sp<GraphicBuffer> buffer;
  ASSERT_EQ(OK, mProducer->requestBuffer(slot, &buffer));

  // A generic "valid" input
  IGraphicBufferProducer::QueueBufferInput input = CreateBufferInput();
  IGraphicBufferProducer::QueueBufferOutput output;

  // Queue the buffer back into the BQ
  ASSERT_EQ(OK, mProducer->queueBuffer(slot, input, &output));

  EXPECT_EQ(kDefaultWidth, output.width);
  EXPECT_EQ(kDefaultHeight, output.height);
  EXPECT_EQ(kDefaultTransformHint, output.transformHint);

  // BufferHubQueue delivers buffers to consumer immediately.
  EXPECT_EQ(0u, output.numPendingBuffers);

  // Note that BufferHubQueue doesn't support nextFrameNumber as it seems to
  // be a SurfaceFlinger specific optimization.
  EXPECT_EQ(0u, output.nextFrameNumber);

  // Buffer was not in the dequeued state
  EXPECT_EQ(BAD_VALUE, mProducer->queueBuffer(slot, input, &output));
}

// Test invalid slot number
TEST_F(BufferHubQueueProducerTest, QueueInvalidSlot_ReturnsError) {
  ASSERT_NO_FATAL_FAILURE(ConnectProducer());

  // A generic "valid" input
  IGraphicBufferProducer::QueueBufferInput input = CreateBufferInput();
  IGraphicBufferProducer::QueueBufferOutput output;

  EXPECT_EQ(BAD_VALUE, mProducer->queueBuffer(/*slot*/ -1, input, &output));
  EXPECT_EQ(BAD_VALUE,
            mProducer->queueBuffer(/*slot*/ 0xDEADBEEF, input, &output));
  EXPECT_EQ(BAD_VALUE, mProducer->queueBuffer(BufferQueueDefs::NUM_BUFFER_SLOTS,
                                              input, &output));
}

// Slot was not in the dequeued state (all slots start out in Free state)
TEST_F(BufferHubQueueProducerTest, QueueNotDequeued_ReturnsError) {
  ASSERT_NO_FATAL_FAILURE(ConnectProducer());

  IGraphicBufferProducer::QueueBufferInput input = CreateBufferInput();
  IGraphicBufferProducer::QueueBufferOutput output;

  EXPECT_EQ(BAD_VALUE, mProducer->queueBuffer(/*slot*/ 0, input, &output));
}

// Slot was enqueued without requesting a buffer
TEST_F(BufferHubQueueProducerTest, QueueNotRequested_ReturnsError) {
  int slot = -1;

  ASSERT_NO_FATAL_FAILURE(ConnectProducer());
  ASSERT_NO_FATAL_FAILURE(DequeueBuffer(&slot));

  IGraphicBufferProducer::QueueBufferInput input = CreateBufferInput();
  IGraphicBufferProducer::QueueBufferOutput output;

  EXPECT_EQ(BAD_VALUE, mProducer->queueBuffer(slot, input, &output));
}

// Test when fence was NULL
TEST_F(BufferHubQueueProducerTest, QueueNoFence_ReturnsError) {
  int slot = -1;

  ASSERT_NO_FATAL_FAILURE(ConnectProducer());
  ASSERT_NO_FATAL_FAILURE(DequeueBuffer(&slot));

  sp<GraphicBuffer> buffer;
  ASSERT_EQ(OK, mProducer->requestBuffer(slot, &buffer));

  sp<Fence> nullFence = NULL;

  IGraphicBufferProducer::QueueBufferInput input =
      QueueBufferInputBuilder().setFence(nullFence).build();
  IGraphicBufferProducer::QueueBufferOutput output;

  EXPECT_EQ(BAD_VALUE, mProducer->queueBuffer(slot, input, &output));
}

// Test scaling mode was invalid
TEST_F(BufferHubQueueProducerTest, QueueTestInvalidScalingMode_ReturnsError) {
  int slot = -1;

  ASSERT_NO_FATAL_FAILURE(ConnectProducer());
  ASSERT_NO_FATAL_FAILURE(DequeueBuffer(&slot));

  sp<GraphicBuffer> buffer;
  ASSERT_EQ(OK, mProducer->requestBuffer(slot, &buffer));

  IGraphicBufferProducer::QueueBufferInput input =
      QueueBufferInputBuilder().setScalingMode(-1).build();
  IGraphicBufferProducer::QueueBufferOutput output;

  EXPECT_EQ(BAD_VALUE, mProducer->queueBuffer(slot, input, &output));

  input = QueueBufferInputBuilder().setScalingMode(0xDEADBEEF).build();

  EXPECT_EQ(BAD_VALUE, mProducer->queueBuffer(slot, input, &output));
}

// Test crop rect is out of bounds of the buffer dimensions
TEST_F(BufferHubQueueProducerTest, QueueCropOutOfBounds_ReturnsError) {
  int slot = -1;

  ASSERT_NO_FATAL_FAILURE(ConnectProducer());
  ASSERT_NO_FATAL_FAILURE(DequeueBuffer(&slot));

  sp<GraphicBuffer> buffer;
  ASSERT_EQ(OK, mProducer->requestBuffer(slot, &buffer));

  IGraphicBufferProducer::QueueBufferInput input =
      QueueBufferInputBuilder()
          .setCrop(Rect(kDefaultWidth + 1, kDefaultHeight + 1))
          .build();
  IGraphicBufferProducer::QueueBufferOutput output;

  EXPECT_EQ(BAD_VALUE, mProducer->queueBuffer(slot, input, &output));
}

TEST_F(BufferHubQueueProducerTest, CancelBuffer_Succeeds) {
  int slot = -1;
  sp<Fence> fence;

  ASSERT_NO_FATAL_FAILURE(ConnectProducer());
  ASSERT_NO_FATAL_FAILURE(DequeueBuffer(&slot, &fence));

  // Should be able to cancel buffer after a dequeue.
  EXPECT_EQ(OK, mProducer->cancelBuffer(slot, fence));
}

TEST_F(BufferHubQueueProducerTest, SetMaxDequeuedBufferCount_Succeeds) {
  return;
  ASSERT_NO_FATAL_FAILURE(ConnectProducer());

  int minUndequeuedBuffers;
  ASSERT_EQ(OK, mProducer->query(NATIVE_WINDOW_MIN_UNDEQUEUED_BUFFERS,
                                 &minUndequeuedBuffers));

  const int minBuffers = 1;
  const int maxBuffers =
      BufferQueueDefs::NUM_BUFFER_SLOTS - minUndequeuedBuffers;

  ASSERT_EQ(OK, mProducer->setAsyncMode(false)) << "async mode: " << false;
  ASSERT_EQ(OK, mProducer->setMaxDequeuedBufferCount(minBuffers))
      << "bufferCount: " << minBuffers;

  // Should now be able to dequeue up to minBuffers times
  // Should now be able to dequeue up to maxBuffers times
  int slot = -1;
  for (int i = 0; i < minBuffers; ++i) {
    ASSERT_NO_FATAL_FAILURE(DequeueBuffer(&slot));
  }

  ASSERT_EQ(OK, mProducer->setMaxDequeuedBufferCount(maxBuffers));

  // queue the first buffer to enable max dequeued buffer count checking
  IGraphicBufferProducer::QueueBufferInput input = CreateBufferInput();
  IGraphicBufferProducer::QueueBufferOutput output;
  sp<GraphicBuffer> buffer;
  ASSERT_EQ(OK, mProducer->requestBuffer(slot, &buffer));
  ASSERT_EQ(OK, mProducer->queueBuffer(slot, input, &output));

  sp<Fence> fence;
  for (int i = 0; i < maxBuffers; ++i) {
    ASSERT_NO_FATAL_FAILURE(DequeueBuffer(&slot, &fence));
  }

  // Cancel a buffer, so we can decrease the buffer count
  ASSERT_EQ(OK, mProducer->cancelBuffer(slot, fence));

  // Should now be able to decrease the max dequeued count by 1
  ASSERT_EQ(OK, mProducer->setMaxDequeuedBufferCount(maxBuffers - 1));
}

TEST_F(BufferHubQueueProducerTest, SetMaxDequeuedBufferCount_Fails) {
  ASSERT_NO_FATAL_FAILURE(ConnectProducer());

  int minUndequeuedBuffers;
  ASSERT_EQ(OK, mProducer->query(NATIVE_WINDOW_MIN_UNDEQUEUED_BUFFERS,
                                 &minUndequeuedBuffers));

  const int minBuffers = 1;
  const int maxBuffers =
      BufferQueueDefs::NUM_BUFFER_SLOTS - minUndequeuedBuffers;

  ASSERT_EQ(OK, mProducer->setAsyncMode(false)) << "async mode: " << false;
  // Buffer count was out of range
  EXPECT_EQ(BAD_VALUE, mProducer->setMaxDequeuedBufferCount(0))
      << "bufferCount: " << 0;
  EXPECT_EQ(BAD_VALUE, mProducer->setMaxDequeuedBufferCount(maxBuffers + 1))
      << "bufferCount: " << maxBuffers + 1;

  // Set max dequeue count to 2
  ASSERT_EQ(OK, mProducer->setMaxDequeuedBufferCount(2));
  // Dequeue 2 buffers
  int slot = -1;
  sp<Fence> fence;
  for (int i = 0; i < 2; i++) {
    ASSERT_EQ(OK, ~IGraphicBufferProducer::BUFFER_NEEDS_REALLOCATION &
                      (mProducer->dequeueBuffer(&slot, &fence, kDefaultWidth,
                                                kDefaultHeight, kDefaultFormat,
                                                kTestProducerUsageBits,
                                                nullptr, nullptr)))
        << "slot: " << slot;
  }

  // Client has too many buffers dequeued
  EXPECT_EQ(BAD_VALUE, mProducer->setMaxDequeuedBufferCount(1))
      << "bufferCount: " << minBuffers;
}

TEST_F(BufferHubQueueProducerTest,
       DisconnectedProducerReturnsError_dequeueBuffer) {
  int slot = -1;
  sp<Fence> fence;

  ASSERT_EQ(NO_INIT, mProducer->dequeueBuffer(&slot, &fence, kDefaultWidth,
                                              kDefaultHeight, kDefaultFormat,
                                              kTestProducerUsageBits,
                                              nullptr, nullptr));
}

TEST_F(BufferHubQueueProducerTest,
       DisconnectedProducerReturnsError_requestBuffer) {
  int slot = -1;
  sp<GraphicBuffer> buffer;

  ASSERT_NO_FATAL_FAILURE(ConnectProducer());
  ASSERT_NO_FATAL_FAILURE(DequeueBuffer(&slot));

  // Shouldn't be able to request buffer after disconnect.
  ASSERT_EQ(OK, mProducer->disconnect(kTestApi));
  ASSERT_EQ(NO_INIT, mProducer->requestBuffer(slot, &buffer));
}

TEST_F(BufferHubQueueProducerTest,
       DisconnectedProducerReturnsError_queueBuffer) {
  int slot = -1;
  sp<GraphicBuffer> buffer;

  ASSERT_NO_FATAL_FAILURE(ConnectProducer());
  ASSERT_NO_FATAL_FAILURE(DequeueBuffer(&slot));
  ASSERT_EQ(OK, mProducer->requestBuffer(slot, &buffer));

  // A generic "valid" input
  IGraphicBufferProducer::QueueBufferInput input = CreateBufferInput();
  IGraphicBufferProducer::QueueBufferOutput output;

  // Shouldn't be able to queue buffer after disconnect.
  ASSERT_EQ(OK, mProducer->disconnect(kTestApi));
  ASSERT_EQ(NO_INIT, mProducer->queueBuffer(slot, input, &output));
}

TEST_F(BufferHubQueueProducerTest,
       DisconnectedProducerReturnsError_cancelBuffer) {
  int slot = -1;
  sp<GraphicBuffer> buffer;

  ASSERT_NO_FATAL_FAILURE(ConnectProducer());
  ASSERT_NO_FATAL_FAILURE(DequeueBuffer(&slot));
  ASSERT_EQ(OK, mProducer->requestBuffer(slot, &buffer));

  // Shouldn't be able to cancel buffer after disconnect.
  ASSERT_EQ(OK, mProducer->disconnect(kTestApi));
  ASSERT_EQ(NO_INIT, mProducer->cancelBuffer(slot, Fence::NO_FENCE));
}

TEST_F(BufferHubQueueProducerTest, ConnectDisconnectReconnect) {
  int slot = -1;
  sp<GraphicBuffer> buffer;
  IGraphicBufferProducer::QueueBufferInput input = CreateBufferInput();
  IGraphicBufferProducer::QueueBufferOutput output;

  EXPECT_NO_FATAL_FAILURE(ConnectProducer());

  constexpr int maxDequeuedBuffers = 1;
  int minUndequeuedBuffers;
  EXPECT_EQ(OK, mProducer->query(NATIVE_WINDOW_MIN_UNDEQUEUED_BUFFERS,
                                 &minUndequeuedBuffers));
  EXPECT_EQ(OK, mProducer->setAsyncMode(false));
  EXPECT_EQ(OK, mProducer->setMaxDequeuedBufferCount(maxDequeuedBuffers));

  int maxCapacity = maxDequeuedBuffers + minUndequeuedBuffers;

  // Dequeue, request, and queue all buffers.
  for (int i = 0; i < maxCapacity; i++) {
    EXPECT_NO_FATAL_FAILURE(DequeueBuffer(&slot));
    EXPECT_EQ(OK, mProducer->requestBuffer(slot, &buffer));
    EXPECT_EQ(OK, mProducer->queueBuffer(slot, input, &output));
  }

  // Disconnect then reconnect.
  EXPECT_EQ(OK, mProducer->disconnect(kTestApi));
  EXPECT_NO_FATAL_FAILURE(ConnectProducer());

  // Dequeue, request, and queue all buffers.
  for (int i = 0; i < maxCapacity; i++) {
    EXPECT_NO_FATAL_FAILURE(DequeueBuffer(&slot));
    EXPECT_EQ(OK, mProducer->requestBuffer(slot, &buffer));
    EXPECT_EQ(OK, mProducer->queueBuffer(slot, input, &output));
  }

  EXPECT_EQ(OK, mProducer->disconnect(kTestApi));
}

TEST_F(BufferHubQueueProducerTest, TakeAsParcelable) {
  // Connected producer cannot be taken out as a parcelable.
  EXPECT_NO_FATAL_FAILURE(ConnectProducer());
  ProducerQueueParcelable producer_parcelable;
  EXPECT_EQ(mProducer->TakeAsParcelable(&producer_parcelable), BAD_VALUE);

  // Create a valid dummy producer parcelable.
  auto dummy_channel_parcelable =
      std::make_unique<pdx::default_transport::ChannelParcelable>(
          LocalHandle(0), LocalHandle(0), LocalHandle(0));
  EXPECT_TRUE(dummy_channel_parcelable->IsValid());
  ProducerQueueParcelable dummy_producer_parcelable(
      std::move(dummy_channel_parcelable));
  EXPECT_TRUE(dummy_producer_parcelable.IsValid());

  // Disconnect producer can be taken out, but only to an invalid parcelable.
  ASSERT_EQ(mProducer->disconnect(kTestApi), OK);
  EXPECT_EQ(mProducer->TakeAsParcelable(&dummy_producer_parcelable), BAD_VALUE);
  EXPECT_FALSE(producer_parcelable.IsValid());
  EXPECT_EQ(mProducer->TakeAsParcelable(&producer_parcelable), OK);
  EXPECT_TRUE(producer_parcelable.IsValid());

  // Should still be able to query buffer dimension after disconnect.
  int32_t value = -1;
  EXPECT_EQ(OK, mProducer->query(NATIVE_WINDOW_WIDTH, &value));
  EXPECT_EQ(static_cast<uint32_t>(value), kDefaultWidth);

  EXPECT_EQ(mProducer->query(NATIVE_WINDOW_HEIGHT, &value), OK);
  EXPECT_EQ(static_cast<uint32_t>(value), kDefaultHeight);

  EXPECT_EQ(mProducer->query(NATIVE_WINDOW_FORMAT, &value), OK);
  EXPECT_EQ(value, kDefaultFormat);

  // But connect to API will fail.
  IGraphicBufferProducer::QueueBufferOutput output;
  EXPECT_EQ(mProducer->connect(kDummyListener, kTestApi, kTestControlledByApp,
                               &output),
            BAD_VALUE);

  // Create a new producer from the parcelable and connect to kTestApi should
  // succeed.
  sp<BufferHubProducer> new_producer =
      BufferHubProducer::Create(std::move(producer_parcelable));
  ASSERT_TRUE(new_producer != nullptr);
  EXPECT_EQ(new_producer->connect(kDummyListener, kTestApi,
                                  kTestControlledByApp, &output),
            OK);
}

}  // namespace

}  // namespace dvr
}  // namespace android
