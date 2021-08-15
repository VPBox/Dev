/*
 * Copyright 2018 The Android Open Source Project
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

#define LOG_TAG "BufferHubBufferTest"

#include <errno.h>
#include <sys/epoll.h>

#include <android/frameworks/bufferhub/1.0/IBufferHub.h>
#include <android/hardware_buffer.h>
#include <cutils/native_handle.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <hidl/ServiceManagement.h>
#include <hwbinder/IPCThreadState.h>
#include <ui/BufferHubBuffer.h>
#include <ui/BufferHubEventFd.h>

namespace android {

namespace {

using ::android::BufferHubDefs::isAnyClientAcquired;
using ::android::BufferHubDefs::isAnyClientGained;
using ::android::BufferHubDefs::isAnyClientPosted;
using ::android::BufferHubDefs::isClientAcquired;
using ::android::BufferHubDefs::isClientGained;
using ::android::BufferHubDefs::isClientPosted;
using ::android::BufferHubDefs::isClientReleased;
using ::android::BufferHubDefs::kMetadataHeaderSize;
using ::android::frameworks::bufferhub::V1_0::IBufferHub;
using ::testing::IsNull;
using ::testing::NotNull;

const int kWidth = 640;
const int kHeight = 480;
const int kLayerCount = 1;
const int kFormat = HAL_PIXEL_FORMAT_RGBA_8888;
const int kUsage = 0;
const AHardwareBuffer_Desc kDesc = {kWidth, kHeight,        kLayerCount,  kFormat,
                                    kUsage, /*stride=*/0UL, /*rfu0=*/0UL, /*rfu1=*/0ULL};
const size_t kUserMetadataSize = 1;

class BufferHubBufferTest : public ::testing::Test {
protected:
    void SetUp() override {
        android::hardware::ProcessState::self()->startThreadPool();

        if (!BufferHubServiceRunning()) {
            // TODO(b/112940221): Enforce the test cross all devices once BufferHub lands in Android
            // R for all Android varieties.
            GTEST_SKIP() << "Skip test as the BufferHub service is not running.";
        }
    }

    bool BufferHubServiceRunning() {
        sp<IBufferHub> bufferhub = IBufferHub::getService();
        return bufferhub.get() != nullptr;
    }
};

bool cmpAHardwareBufferDesc(const AHardwareBuffer_Desc& desc, const AHardwareBuffer_Desc& other) {
    // Not comparing stride because it's unknown before allocation
    return desc.format == other.format && desc.height == other.height &&
            desc.layers == other.layers && desc.usage == other.usage && desc.width == other.width;
}

class BufferHubBufferStateTransitionTest : public BufferHubBufferTest {
protected:
    void SetUp() override {
        BufferHubBufferTest::SetUp();

        if (IsSkipped()) {
            // If the base class' SetUp() stated the test should be skipped, we should short
            // circuit this sub-class' logic.
            return;
        }

        CreateTwoClientsOfABuffer();
    }

    std::unique_ptr<BufferHubBuffer> b1;
    uint32_t b1ClientMask = 0U;
    std::unique_ptr<BufferHubBuffer> b2;
    uint32_t b2ClientMask = 0U;

private:
    // Creates b1 and b2 as the clients of the same buffer for testing.
    void CreateTwoClientsOfABuffer();
};

void BufferHubBufferStateTransitionTest::CreateTwoClientsOfABuffer() {
    b1 = BufferHubBuffer::create(kWidth, kHeight, kLayerCount, kFormat, kUsage, kUserMetadataSize);
    ASSERT_THAT(b1, NotNull());
    b1ClientMask = b1->clientStateMask();
    ASSERT_NE(b1ClientMask, 0U);

    sp<NativeHandle> token = b1->duplicate();
    ASSERT_THAT(token, NotNull());

    b2 = BufferHubBuffer::import(token);
    ASSERT_THAT(b2, NotNull());

    b2ClientMask = b2->clientStateMask();
    ASSERT_NE(b2ClientMask, 0U);
    ASSERT_NE(b2ClientMask, b1ClientMask);
}

TEST_F(BufferHubBufferTest, CreateBufferFails) {
    // Buffer Creation will fail: BLOB format requires height to be 1.
    auto b1 = BufferHubBuffer::create(kWidth, /*height=*/2, kLayerCount,
                                      /*format=*/HAL_PIXEL_FORMAT_BLOB, kUsage, kUserMetadataSize);

    EXPECT_THAT(b1, IsNull());

    // Buffer Creation will fail: user metadata size too large.
    auto b2 = BufferHubBuffer::create(kWidth, kHeight, kLayerCount, kFormat, kUsage,
                                      /*userMetadataSize=*/std::numeric_limits<size_t>::max());

    EXPECT_THAT(b2, IsNull());

    // Buffer Creation will fail: user metadata size too large.
    const size_t userMetadataSize = std::numeric_limits<size_t>::max() - kMetadataHeaderSize;
    auto b3 = BufferHubBuffer::create(kWidth, kHeight, kLayerCount, kFormat, kUsage,
                                      userMetadataSize);

    EXPECT_THAT(b3, IsNull());
}

TEST_F(BufferHubBufferTest, CreateBuffer) {
    auto b1 = BufferHubBuffer::create(kWidth, kHeight, kLayerCount, kFormat, kUsage,
                                      kUserMetadataSize);
    ASSERT_THAT(b1, NotNull());
    EXPECT_TRUE(b1->isConnected());
    EXPECT_TRUE(b1->isValid());
    EXPECT_TRUE(cmpAHardwareBufferDesc(b1->desc(), kDesc));
    EXPECT_EQ(b1->userMetadataSize(), kUserMetadataSize);
}

TEST_F(BufferHubBufferTest, DuplicateAndImportBuffer) {
    auto b1 = BufferHubBuffer::create(kWidth, kHeight, kLayerCount, kFormat, kUsage,
                                      kUserMetadataSize);
    ASSERT_THAT(b1, NotNull());
    EXPECT_TRUE(b1->isValid());

    sp<NativeHandle> token = b1->duplicate();
    ASSERT_THAT(token, NotNull());

    // The detached buffer should still be valid.
    EXPECT_TRUE(b1->isConnected());
    EXPECT_TRUE(b1->isValid());

    std::unique_ptr<BufferHubBuffer> b2 = BufferHubBuffer::import(token);

    ASSERT_THAT(b2, NotNull());
    EXPECT_TRUE(b2->isValid());

    EXPECT_TRUE(cmpAHardwareBufferDesc(b1->desc(), b2->desc()));
    EXPECT_EQ(b1->userMetadataSize(), b2->userMetadataSize());

    // These two buffer instances are based on the same physical buffer under the
    // hood, so they should share the same id.
    EXPECT_EQ(b1->id(), b2->id());
    // We use clientStateMask() to tell those two instances apart.
    EXPECT_NE(b1->clientStateMask(), b2->clientStateMask());

    // Both buffer instances should be in released state currently.
    EXPECT_TRUE(b1->isReleased());
    EXPECT_TRUE(b2->isReleased());

    // The event fd should behave like duped event fds.
    const BufferHubEventFd& eventFd1 = b1->eventFd();
    ASSERT_GE(eventFd1.get(), 0);
    const BufferHubEventFd& eventFd2 = b2->eventFd();
    ASSERT_GE(eventFd2.get(), 0);

    base::unique_fd epollFd(epoll_create(64));
    ASSERT_GE(epollFd.get(), 0);

    // Add eventFd1 to epoll set, and signal eventFd2.
    epoll_event e = {.events = EPOLLIN | EPOLLET, .data = {.u32 = 0}};
    ASSERT_EQ(epoll_ctl(epollFd.get(), EPOLL_CTL_ADD, eventFd1.get(), &e), 0) << strerror(errno);

    std::array<epoll_event, 1> events;
    EXPECT_EQ(epoll_wait(epollFd.get(), events.data(), events.size(), 0), 0);

    eventFd2.signal();
    EXPECT_EQ(epoll_wait(epollFd.get(), events.data(), events.size(), 0), 1);

    // The epoll fd is edge triggered, so it only responds to the eventFd once.
    EXPECT_EQ(epoll_wait(epollFd.get(), events.data(), events.size(), 0), 0);

    eventFd2.signal();
    eventFd2.clear();
    EXPECT_EQ(epoll_wait(epollFd.get(), events.data(), events.size(), 0), 0);
}

TEST_F(BufferHubBufferTest, ImportFreedBuffer) {
    auto b1 = BufferHubBuffer::create(kWidth, kHeight, kLayerCount, kFormat, kUsage,
                                      kUserMetadataSize);
    ASSERT_THAT(b1, NotNull());
    EXPECT_TRUE(b1->isValid());

    sp<NativeHandle> token = b1->duplicate();
    ASSERT_THAT(token, NotNull());

    // Explicitly destroy b1. Backend buffer should be freed and token becomes invalid
    b1.reset();

    std::unique_ptr<BufferHubBuffer> b2 = BufferHubBuffer::import(token);

    // Import should fail with INVALID_TOKEN
    EXPECT_THAT(b2, IsNull());
}

// nullptr must not crash the service
TEST_F(BufferHubBufferTest, ImportNullToken) {
    auto b1 = BufferHubBuffer::import(nullptr);
    EXPECT_THAT(b1, IsNull());
}

TEST_F(BufferHubBufferTest, ImportInvalidToken) {
    native_handle_t* token = native_handle_create(/*numFds=*/0, /*numInts=*/1);
    token->data[0] = 0;

    sp<NativeHandle> tokenHandle = NativeHandle::create(token, /*ownHandle=*/true);
    auto b1 = BufferHubBuffer::import(tokenHandle);

    EXPECT_THAT(b1, IsNull());
}

TEST_F(BufferHubBufferStateTransitionTest, GainBuffer_fromReleasedState) {
    ASSERT_TRUE(b1->isReleased());

    // Successful gaining the buffer should change the buffer state bit of b1 to
    // gained state, other client state bits to released state.
    EXPECT_EQ(b1->gain(), 0);
    EXPECT_TRUE(isClientGained(b1->bufferState(), b1ClientMask));
}

TEST_F(BufferHubBufferStateTransitionTest, GainBuffer_fromGainedState) {
    ASSERT_EQ(b1->gain(), 0);
    auto currentBufferState = b1->bufferState();
    ASSERT_TRUE(isClientGained(currentBufferState, b1ClientMask));

    // Gaining from gained state by the same client should not return error.
    EXPECT_EQ(b1->gain(), 0);

    // Gaining from gained state by another client should return error.
    EXPECT_EQ(b2->gain(), -EBUSY);
}

TEST_F(BufferHubBufferStateTransitionTest, GainBuffer_fromAcquiredState) {
    ASSERT_EQ(b1->gain(), 0);
    ASSERT_EQ(b1->post(), 0);
    ASSERT_EQ(b2->acquire(), 0);
    ASSERT_TRUE(isAnyClientAcquired(b1->bufferState()));

    // Gaining from acquired state should fail.
    EXPECT_EQ(b1->gain(), -EBUSY);
    EXPECT_EQ(b2->gain(), -EBUSY);
}

TEST_F(BufferHubBufferStateTransitionTest, GainBuffer_fromOtherClientInPostedState) {
    ASSERT_EQ(b1->gain(), 0);
    ASSERT_EQ(b1->post(), 0);
    ASSERT_TRUE(isAnyClientPosted(b1->bufferState()));

    // Gaining a buffer who has other posted client should succeed.
    EXPECT_EQ(b1->gain(), 0);
}

TEST_F(BufferHubBufferStateTransitionTest, GainBuffer_fromSelfInPostedState) {
    ASSERT_EQ(b1->gain(), 0);
    ASSERT_EQ(b1->post(), 0);
    ASSERT_TRUE(isAnyClientPosted(b1->bufferState()));

    // A posted client should be able to gain the buffer when there is no other clients in
    // acquired state.
    EXPECT_EQ(b2->gain(), 0);
}

TEST_F(BufferHubBufferStateTransitionTest, PostBuffer_fromOtherInGainedState) {
    ASSERT_EQ(b1->gain(), 0);
    ASSERT_TRUE(isClientGained(b1->bufferState(), b1ClientMask));

    EXPECT_EQ(b2->post(), -EBUSY);
}

TEST_F(BufferHubBufferStateTransitionTest, PostBuffer_fromSelfInGainedState) {
    ASSERT_EQ(b1->gain(), 0);
    ASSERT_TRUE(isClientGained(b1->bufferState(), b1ClientMask));

    EXPECT_EQ(b1->post(), 0);
    auto currentBufferState = b1->bufferState();
    EXPECT_TRUE(isClientReleased(currentBufferState, b1ClientMask));
    EXPECT_TRUE(isClientPosted(currentBufferState, b2ClientMask));
}

TEST_F(BufferHubBufferStateTransitionTest, PostBuffer_fromPostedState) {
    ASSERT_EQ(b1->gain(), 0);
    ASSERT_EQ(b1->post(), 0);
    ASSERT_TRUE(isAnyClientPosted(b1->bufferState()));

    // Post from posted state should fail.
    EXPECT_EQ(b1->post(), -EBUSY);
    EXPECT_EQ(b2->post(), -EBUSY);
}

TEST_F(BufferHubBufferStateTransitionTest, PostBuffer_fromAcquiredState) {
    ASSERT_EQ(b1->gain(), 0);
    ASSERT_EQ(b1->post(), 0);
    ASSERT_EQ(b2->acquire(), 0);
    ASSERT_TRUE(isAnyClientAcquired(b1->bufferState()));

    // Posting from acquired state should fail.
    EXPECT_EQ(b1->post(), -EBUSY);
    EXPECT_EQ(b2->post(), -EBUSY);
}

TEST_F(BufferHubBufferStateTransitionTest, PostBuffer_fromReleasedState) {
    ASSERT_TRUE(b1->isReleased());

    // Posting from released state should fail.
    EXPECT_EQ(b1->post(), -EBUSY);
    EXPECT_EQ(b2->post(), -EBUSY);
}

TEST_F(BufferHubBufferStateTransitionTest, AcquireBuffer_fromSelfInPostedState) {
    ASSERT_EQ(b1->gain(), 0);
    ASSERT_EQ(b1->post(), 0);
    ASSERT_TRUE(isClientPosted(b1->bufferState(), b2ClientMask));

    // Acquire from posted state should pass.
    EXPECT_EQ(b2->acquire(), 0);
}

TEST_F(BufferHubBufferStateTransitionTest, AcquireBuffer_fromOtherInPostedState) {
    ASSERT_EQ(b1->gain(), 0);
    ASSERT_EQ(b1->post(), 0);
    ASSERT_TRUE(isClientPosted(b1->bufferState(), b2ClientMask));

    // Acquire from released state should fail, although there are other clients
    // in posted state.
    EXPECT_EQ(b1->acquire(), -EBUSY);
}

TEST_F(BufferHubBufferStateTransitionTest, AcquireBuffer_fromSelfInAcquiredState) {
    ASSERT_EQ(b1->gain(), 0);
    ASSERT_EQ(b1->post(), 0);
    ASSERT_EQ(b2->acquire(), 0);
    auto currentBufferState = b1->bufferState();
    ASSERT_TRUE(isClientAcquired(currentBufferState, b2ClientMask));

    // Acquiring from acquired state by the same client should not error out.
    EXPECT_EQ(b2->acquire(), 0);
}

TEST_F(BufferHubBufferStateTransitionTest, AcquireBuffer_fromReleasedState) {
    ASSERT_TRUE(b1->isReleased());

    // Acquiring form released state should fail.
    EXPECT_EQ(b1->acquire(), -EBUSY);
    EXPECT_EQ(b2->acquire(), -EBUSY);
}

TEST_F(BufferHubBufferStateTransitionTest, AcquireBuffer_fromGainedState) {
    ASSERT_EQ(b1->gain(), 0);
    ASSERT_TRUE(isAnyClientGained(b1->bufferState()));

    // Acquiring from gained state should fail.
    EXPECT_EQ(b1->acquire(), -EBUSY);
    EXPECT_EQ(b2->acquire(), -EBUSY);
}

TEST_F(BufferHubBufferStateTransitionTest, ReleaseBuffer_fromSelfInReleasedState) {
    ASSERT_TRUE(b1->isReleased());

    EXPECT_EQ(b1->release(), 0);
}

TEST_F(BufferHubBufferStateTransitionTest, ReleaseBuffer_fromSelfInGainedState) {
    ASSERT_TRUE(b1->isReleased());
    ASSERT_EQ(b1->gain(), 0);
    ASSERT_TRUE(isAnyClientGained(b1->bufferState()));

    EXPECT_EQ(b1->release(), 0);
}

TEST_F(BufferHubBufferStateTransitionTest, ReleaseBuffer_fromSelfInPostedState) {
    ASSERT_EQ(b1->gain(), 0);
    ASSERT_EQ(b1->post(), 0);
    ASSERT_TRUE(isAnyClientPosted(b1->bufferState()));

    EXPECT_EQ(b2->release(), 0);
}

TEST_F(BufferHubBufferStateTransitionTest, ReleaseBuffer_fromSelfInAcquiredState) {
    ASSERT_EQ(b1->gain(), 0);
    ASSERT_EQ(b1->post(), 0);
    ASSERT_EQ(b2->acquire(), 0);
    ASSERT_TRUE(isAnyClientAcquired(b1->bufferState()));

    EXPECT_EQ(b2->release(), 0);
}

TEST_F(BufferHubBufferStateTransitionTest, BasicUsage) {
    // 1 producer buffer and 1 consumer buffer initialised in testcase setup.
    // Test if this set of basic operation succeed:
    // Producer post three times to the consumer, and released by consumer.
    for (int i = 0; i < 3; ++i) {
        ASSERT_EQ(b1->gain(), 0);
        ASSERT_EQ(b1->post(), 0);
        ASSERT_EQ(b2->acquire(), 0);
        ASSERT_EQ(b2->release(), 0);
    }
}

TEST_F(BufferHubBufferTest, createNewConsumerAfterGain) {
    // Create a poducer buffer and gain.
    std::unique_ptr<BufferHubBuffer> b1 =
            BufferHubBuffer::create(kWidth, kHeight, kLayerCount, kFormat, kUsage,
                                    kUserMetadataSize);
    ASSERT_THAT(b1, NotNull());
    ASSERT_EQ(b1->gain(), 0);

    // Create a consumer of the buffer and test if the consumer can acquire the
    // buffer if producer posts.
    sp<NativeHandle> token = b1->duplicate();
    ASSERT_THAT(token, NotNull());

    std::unique_ptr<BufferHubBuffer> b2 = BufferHubBuffer::import(token);

    ASSERT_THAT(b2, NotNull());
    ASSERT_NE(b1->clientStateMask(), b2->clientStateMask());

    ASSERT_EQ(b1->post(), 0);
    EXPECT_EQ(b2->acquire(), 0);
}

TEST_F(BufferHubBufferTest, createNewConsumerAfterPost) {
    // Create a poducer buffer and post.
    std::unique_ptr<BufferHubBuffer> b1 =
            BufferHubBuffer::create(kWidth, kHeight, kLayerCount, kFormat, kUsage,
                                    kUserMetadataSize);
    ASSERT_EQ(b1->gain(), 0);
    ASSERT_EQ(b1->post(), 0);

    // Create a consumer of the buffer and test if the consumer can acquire the
    // buffer if producer posts.
    sp<NativeHandle> token = b1->duplicate();
    ASSERT_THAT(token, NotNull());

    std::unique_ptr<BufferHubBuffer> b2 = BufferHubBuffer::import(token);

    ASSERT_THAT(b2, NotNull());
    ASSERT_NE(b1->clientStateMask(), b2->clientStateMask());

    EXPECT_EQ(b2->acquire(), 0);
}

} // namespace

} // namespace android
