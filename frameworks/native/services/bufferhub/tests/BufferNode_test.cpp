#include <errno.h>

#include <bufferhub/BufferNode.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <ui/BufferHubDefs.h>
#include <ui/GraphicBufferMapper.h>

namespace android {
namespace frameworks {
namespace bufferhub {
namespace V1_0 {
namespace implementation {

namespace {

using testing::NotNull;

const uint32_t kWidth = 640;
const uint32_t kHeight = 480;
const uint32_t kLayerCount = 1;
const uint32_t kFormat = 1;
const uint64_t kUsage = 0;
const size_t kUserMetadataSize = 0;

class BufferNodeTest : public ::testing::Test {
protected:
    void SetUp() override {
        mBufferNode =
                new BufferNode(kWidth, kHeight, kLayerCount, kFormat, kUsage, kUserMetadataSize);
        ASSERT_TRUE(mBufferNode->isValid());
    }

    void TearDown() override {
        if (mBufferNode != nullptr) {
            delete mBufferNode;
        }
    }

    BufferNode* mBufferNode = nullptr;
};

TEST_F(BufferNodeTest, TestCreateBufferNode) {
    EXPECT_EQ(mBufferNode->userMetadataSize(), kUserMetadataSize);
    // Test the handle just allocated is good (i.e. able to be imported)
    GraphicBufferMapper& mapper = GraphicBufferMapper::get();
    const native_handle_t* outHandle;
    status_t ret =
            mapper.importBuffer(mBufferNode->bufferHandle(), mBufferNode->bufferDesc().width,
                                mBufferNode->bufferDesc().height, mBufferNode->bufferDesc().layers,
                                mBufferNode->bufferDesc().format, mBufferNode->bufferDesc().usage,
                                mBufferNode->bufferDesc().stride, &outHandle);
    EXPECT_EQ(ret, OK);
    EXPECT_THAT(outHandle, NotNull());
}

TEST_F(BufferNodeTest, TestaddNewActiveClientsBitToMask_twoNewClients) {
    uint32_t newClientStateMask1 = mBufferNode->addNewActiveClientsBitToMask();
    EXPECT_EQ(mBufferNode->getActiveClientsBitMask(), newClientStateMask1);

    // Request and add a new client_state_mask again.
    // Active clients bit mask should be the union of the two new
    // client_state_masks.
    uint32_t newClientStateMask2 = mBufferNode->addNewActiveClientsBitToMask();
    EXPECT_EQ(mBufferNode->getActiveClientsBitMask(), newClientStateMask1 | newClientStateMask2);
}

TEST_F(BufferNodeTest, TestaddNewActiveClientsBitToMask_32NewClients) {
    uint32_t newClientStateMask = 0U;
    uint32_t currentMask = 0U;
    uint32_t expectedMask = 0U;

    for (int i = 0; i < BufferHubDefs::kMaxNumberOfClients; ++i) {
        newClientStateMask = mBufferNode->addNewActiveClientsBitToMask();
        EXPECT_NE(newClientStateMask, 0U);
        EXPECT_FALSE(newClientStateMask & currentMask);
        expectedMask = currentMask | newClientStateMask;
        currentMask = mBufferNode->getActiveClientsBitMask();
        EXPECT_EQ(currentMask, expectedMask);
    }

    // Method should fail upon requesting for more than maximum allowable clients.
    newClientStateMask = mBufferNode->addNewActiveClientsBitToMask();
    EXPECT_EQ(newClientStateMask, 0U);
    EXPECT_EQ(errno, E2BIG);
}

TEST_F(BufferNodeTest, TestRemoveActiveClientsBitFromMask) {
    mBufferNode->addNewActiveClientsBitToMask();
    uint32_t currentMask = mBufferNode->getActiveClientsBitMask();
    uint32_t newClientStateMask = mBufferNode->addNewActiveClientsBitToMask();
    EXPECT_NE(mBufferNode->getActiveClientsBitMask(), currentMask);

    mBufferNode->removeClientsBitFromMask(newClientStateMask);
    EXPECT_EQ(mBufferNode->getActiveClientsBitMask(), currentMask);

    // Remove the test_mask again to the active client bit mask should not modify
    // the value of active clients bit mask.
    mBufferNode->removeClientsBitFromMask(newClientStateMask);
    EXPECT_EQ(mBufferNode->getActiveClientsBitMask(), currentMask);
}

} // namespace

} // namespace implementation
} // namespace V1_0
} // namespace bufferhub
} // namespace frameworks
} // namespace android
