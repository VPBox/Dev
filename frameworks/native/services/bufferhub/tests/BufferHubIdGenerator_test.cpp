#include <bufferhub/BufferHubIdGenerator.h>
#include <gtest/gtest.h>

namespace android {
namespace frameworks {
namespace bufferhub {
namespace V1_0 {
namespace implementation {

namespace {

class BufferHubIdGeneratorTest : public testing::Test {
protected:
    BufferHubIdGenerator* mIdGenerator = &BufferHubIdGenerator::getInstance();
};

TEST_F(BufferHubIdGeneratorTest, TestGenerateAndFreeID) {
    int id = mIdGenerator->getId();
    EXPECT_GE(id, 0);

    mIdGenerator->freeId(id);
}

TEST_F(BufferHubIdGeneratorTest, TestGenerateUniqueIncrementalID) {
    // 10 IDs should not overflow the UniqueIdGenerator to cause a roll back to start, so the
    // resulting IDs should still keep incresing.
    const int kTestSize = 10;
    int ids[kTestSize];
    for (int i = 0; i < kTestSize; ++i) {
        ids[i] = mIdGenerator->getId();
        EXPECT_GE(ids[i], 0);
        if (i >= 1) {
            EXPECT_GT(ids[i], ids[i - 1]);
        }
    }

    for (int i = 0; i < kTestSize; ++i) {
        mIdGenerator->freeId(ids[i]);
    }
}

} // namespace

} // namespace implementation
} // namespace V1_0
} // namespace bufferhub
} // namespace frameworks
} // namespace android