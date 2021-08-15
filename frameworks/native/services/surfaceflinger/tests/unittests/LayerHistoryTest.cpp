#undef LOG_TAG
#define LOG_TAG "LayerHistoryUnittests"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <log/log.h>

#include <mutex>
#include <thread>

#include "Scheduler/LayerHistory.h"

using testing::_;
using testing::Return;

namespace android {
namespace scheduler {

class LayerHistoryTest : public testing::Test {
public:
    LayerHistoryTest();
    ~LayerHistoryTest() override;

protected:
    std::unique_ptr<LayerHistory> mLayerHistory;

    static constexpr float MIN_REFRESH_RATE = 30.f;
    static constexpr float MAX_REFRESH_RATE = 90.f;
    static constexpr auto RELEVANT_FRAME_THRESHOLD = 90u;
    static constexpr uint64_t THIRTY_FPS_INTERVAL = 33'333'333;

    void forceRelevancy(const std::unique_ptr<LayerHistory::LayerHandle>& testLayer) {
        mLayerHistory->setVisibility(testLayer, true);
        for (auto i = 0u; i < RELEVANT_FRAME_THRESHOLD; i++) {
            mLayerHistory->insert(testLayer, 0, false /*isHDR*/);
        }
    };
};

LayerHistoryTest::LayerHistoryTest() {
    mLayerHistory = std::make_unique<LayerHistory>();
}
LayerHistoryTest::~LayerHistoryTest() {}

namespace {
TEST_F(LayerHistoryTest, oneLayer) {
    std::unique_ptr<LayerHistory::LayerHandle> testLayer =
            mLayerHistory->createLayer("TestLayer", MIN_REFRESH_RATE, MAX_REFRESH_RATE);
    mLayerHistory->setVisibility(testLayer, true);
    for (auto i = 0u; i < RELEVANT_FRAME_THRESHOLD; i++) {
        EXPECT_FLOAT_EQ(0.f, mLayerHistory->getDesiredRefreshRateAndHDR().first);
        mLayerHistory->insert(testLayer, 0, false /*isHDR*/);
    }

    // Add a few more. This time we should get MAX refresh rate as the layer
    // becomes relevant
    static constexpr auto A_FEW = 10;
    for (auto i = 0u; i < A_FEW; i++) {
        EXPECT_FLOAT_EQ(MAX_REFRESH_RATE, mLayerHistory->getDesiredRefreshRateAndHDR().first);
        mLayerHistory->insert(testLayer, 0, false /*isHDR*/);
    }
}

TEST_F(LayerHistoryTest, oneHDRLayer) {
    std::unique_ptr<LayerHistory::LayerHandle> testLayer =
            mLayerHistory->createLayer("TestHDRLayer", MIN_REFRESH_RATE, MAX_REFRESH_RATE);
    mLayerHistory->setVisibility(testLayer, true);

    mLayerHistory->insert(testLayer, 0, true /*isHDR*/);
    EXPECT_FLOAT_EQ(0.0f, mLayerHistory->getDesiredRefreshRateAndHDR().first);
    EXPECT_EQ(true, mLayerHistory->getDesiredRefreshRateAndHDR().second);

    mLayerHistory->setVisibility(testLayer, false);
    EXPECT_FLOAT_EQ(0.0f, mLayerHistory->getDesiredRefreshRateAndHDR().first);
    EXPECT_EQ(false, mLayerHistory->getDesiredRefreshRateAndHDR().second);
}

TEST_F(LayerHistoryTest, explicitTimestamp) {
    std::unique_ptr<LayerHistory::LayerHandle> test30FpsLayer =
            mLayerHistory->createLayer("30FpsLayer", MIN_REFRESH_RATE, MAX_REFRESH_RATE);
    mLayerHistory->setVisibility(test30FpsLayer, true);

    nsecs_t startTime = systemTime();
    for (int i = 0; i < RELEVANT_FRAME_THRESHOLD; i++) {
        mLayerHistory->insert(test30FpsLayer, startTime + (i * THIRTY_FPS_INTERVAL),
                              false /*isHDR*/);
    }

    EXPECT_FLOAT_EQ(30.f, mLayerHistory->getDesiredRefreshRateAndHDR().first);
}

TEST_F(LayerHistoryTest, multipleLayers) {
    std::unique_ptr<LayerHistory::LayerHandle> testLayer =
            mLayerHistory->createLayer("TestLayer", MIN_REFRESH_RATE, MAX_REFRESH_RATE);
    mLayerHistory->setVisibility(testLayer, true);
    std::unique_ptr<LayerHistory::LayerHandle> test30FpsLayer =
            mLayerHistory->createLayer("30FpsLayer", MIN_REFRESH_RATE, MAX_REFRESH_RATE);
    mLayerHistory->setVisibility(test30FpsLayer, true);
    std::unique_ptr<LayerHistory::LayerHandle> testLayer2 =
            mLayerHistory->createLayer("TestLayer2", MIN_REFRESH_RATE, MAX_REFRESH_RATE);
    mLayerHistory->setVisibility(testLayer2, true);

    nsecs_t startTime = systemTime();
    for (int i = 0; i < RELEVANT_FRAME_THRESHOLD; i++) {
        mLayerHistory->insert(testLayer, 0, false /*isHDR*/);
    }
    EXPECT_FLOAT_EQ(MAX_REFRESH_RATE, mLayerHistory->getDesiredRefreshRateAndHDR().first);

    startTime = systemTime();
    for (int i = 0; i < RELEVANT_FRAME_THRESHOLD; i++) {
        mLayerHistory->insert(test30FpsLayer, startTime + (i * THIRTY_FPS_INTERVAL),
                              false /*isHDR*/);
    }
    EXPECT_FLOAT_EQ(MAX_REFRESH_RATE, mLayerHistory->getDesiredRefreshRateAndHDR().first);

    for (int i = 10; i < RELEVANT_FRAME_THRESHOLD; i++) {
        mLayerHistory->insert(test30FpsLayer, startTime + (i * THIRTY_FPS_INTERVAL),
                              false /*isHDR*/);
    }
    EXPECT_FLOAT_EQ(MAX_REFRESH_RATE, mLayerHistory->getDesiredRefreshRateAndHDR().first);

    // This frame is only around for 9 occurrences, so it doesn't throw
    // anything off.
    for (int i = 0; i < 9; i++) {
        mLayerHistory->insert(testLayer2, 0, false /*isHDR*/);
    }
    EXPECT_FLOAT_EQ(MAX_REFRESH_RATE, mLayerHistory->getDesiredRefreshRateAndHDR().first);
    // After 1200 ms frames become obsolete.
    std::this_thread::sleep_for(std::chrono::milliseconds(1500));

    mLayerHistory->insert(test30FpsLayer,
                          startTime + (RELEVANT_FRAME_THRESHOLD * THIRTY_FPS_INTERVAL),
                          false /*isHDR*/);
    EXPECT_FLOAT_EQ(30.f, mLayerHistory->getDesiredRefreshRateAndHDR().first);
}

} // namespace
} // namespace scheduler
} // namespace android