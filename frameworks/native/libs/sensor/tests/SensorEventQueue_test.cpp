/*
 * Copyright (C) 2019 The Android Open Source Project
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

#include <stdint.h>

#include <gtest/gtest.h>
#include <utils/Errors.h>

#include <android/sensor.h>
#include <hardware/sensors-base.h>
#include <sensor/SensorManager.h>
#include <sensor/SensorEventQueue.h>

namespace android {

class SensorEventQueueTest : public ::testing::Test {
protected:
    typedef std::vector<int32_t> Events;

    SensorEventQueueTest() {};

    virtual void SetUp() override {
        SensorManager& manager = SensorManager::getInstanceForPackage(String16("SensorEventQueueTest"));
        mQueue = manager.createEventQueue();
    }

    void configureAdditionalInfo(bool enable) {
        mQueue->requestAdditionalInfo = enable;
    }

    Events filterEvents(const Events &types) const {
        // Convert the events into SensorEvent array
        ASensorEvent* events = new ASensorEvent[types.size()];
        for (size_t i = 0; i < types.size(); i++) {
            events[i].type = types[i];
        }

        // Filter the events
        ssize_t filteredCount = mQueue->filterEvents(events, types.size());

        // Copy the result into an output vector
        Events result;
        for (size_t i = 0; i < filteredCount; i++) {
            result.push_back(events[i].type);
        }
        delete[] events;

        return result;
    }

    Events getExpectedEvents(const Events &events) const {
        Events output;
        for (size_t i = 0; i != events.size(); i++) {
            // Copy events if the event queue is configured to receive them
            if (events[i] != SENSOR_TYPE_ADDITIONAL_INFO || mQueue->requestAdditionalInfo) {
                output.push_back(events[i]);
            }
        }
        return output;
    }

    void runFilterTest(const Events& events) {
        Events filtered = filterEvents(events);
        Events expected = getExpectedEvents(events);
        EXPECT_EQ(expected.size(), filtered.size());
        EXPECT_EQ(expected, filtered);
    }

private:
    sp<SensorEventQueue> mQueue;
};

TEST_F(SensorEventQueueTest, FilterZeroEvents) {
    configureAdditionalInfo(false /* enable */);
    runFilterTest({});
}

TEST_F(SensorEventQueueTest, FilterEvents_ReceiveAdditionalInfo) {
    configureAdditionalInfo(true /* enable */);
    runFilterTest({SENSOR_TYPE_ADDITIONAL_INFO,
                   SENSOR_TYPE_ACCELEROMETER,
                   SENSOR_TYPE_GYROSCOPE,
                   SENSOR_TYPE_ADDITIONAL_INFO,
                   SENSOR_TYPE_ADDITIONAL_INFO,
                   SENSOR_TYPE_MAGNETIC_FIELD});
}

TEST_F(SensorEventQueueTest, FilterEvents_RemoveAll) {
    configureAdditionalInfo(false /* enable */);
    runFilterTest({SENSOR_TYPE_ADDITIONAL_INFO,
                   SENSOR_TYPE_ADDITIONAL_INFO,
                   SENSOR_TYPE_ADDITIONAL_INFO});
}

TEST_F(SensorEventQueueTest, FilterEvents_RemoveFirst) {
    configureAdditionalInfo(false /* enable */);
    runFilterTest({SENSOR_TYPE_ADDITIONAL_INFO,
                   SENSOR_TYPE_ACCELEROMETER,
                   SENSOR_TYPE_GYROSCOPE,
                   SENSOR_TYPE_MAGNETIC_FIELD});
}

TEST_F(SensorEventQueueTest, FilterEvents_RemoveAllButOne) {
    configureAdditionalInfo(false /* enable */);
    runFilterTest({SENSOR_TYPE_ADDITIONAL_INFO,
                   SENSOR_TYPE_ADDITIONAL_INFO,
                   SENSOR_TYPE_ACCELEROMETER,
                   SENSOR_TYPE_ADDITIONAL_INFO});
}

TEST_F(SensorEventQueueTest, FilterEvents_RemoveLast) {
    configureAdditionalInfo(false /* enable */);
    runFilterTest({SENSOR_TYPE_ACCELEROMETER,
                   SENSOR_TYPE_GYROSCOPE,
                   SENSOR_TYPE_MAGNETIC_FIELD,
                   SENSOR_TYPE_ADDITIONAL_INFO});
}

TEST_F(SensorEventQueueTest, FilterEvents_RemoveConsecutive) {
    configureAdditionalInfo(false /* enable */);
    runFilterTest({SENSOR_TYPE_MAGNETIC_FIELD,
                   SENSOR_TYPE_ADDITIONAL_INFO,
                   SENSOR_TYPE_ADDITIONAL_INFO,
                   SENSOR_TYPE_ADDITIONAL_INFO,
                   SENSOR_TYPE_ADDITIONAL_INFO,
                   SENSOR_TYPE_ADDITIONAL_INFO,
                   SENSOR_TYPE_ACCELEROMETER});
}

TEST_F(SensorEventQueueTest, FilterEvents_RemoveInterleaved) {
    configureAdditionalInfo(false /* enable */);
    runFilterTest({SENSOR_TYPE_ACCELEROMETER,
                   SENSOR_TYPE_GYROSCOPE,
                   SENSOR_TYPE_ADDITIONAL_INFO,
                   SENSOR_TYPE_ACCELEROMETER,
                   SENSOR_TYPE_GYROSCOPE,
                   SENSOR_TYPE_ADDITIONAL_INFO,
                   SENSOR_TYPE_MAGNETIC_FIELD});
}

TEST_F(SensorEventQueueTest, FilterEvents_ReconfigureAdditionalInfo) {
    configureAdditionalInfo(false /* enable */);
    const Events events = {SENSOR_TYPE_ACCELEROMETER,
                           SENSOR_TYPE_GYROSCOPE,
                           SENSOR_TYPE_ADDITIONAL_INFO,
                           SENSOR_TYPE_MAGNETIC_FIELD,
                           SENSOR_TYPE_ADDITIONAL_INFO};
    runFilterTest(events);

    // Update setting to request Additional Info
    configureAdditionalInfo(true /* enable */);
    runFilterTest(events);

    // Update setting to stop requesting Additional Info
    configureAdditionalInfo(true /* enable */);
    runFilterTest(events);
}

} // namespace android
