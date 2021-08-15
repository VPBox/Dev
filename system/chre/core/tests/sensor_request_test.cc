/*
 * Copyright (C) 2016 The Android Open Source Project
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

#include "gtest/gtest.h"

#include "chre/core/sensor_request.h"

using chre::Nanoseconds;
using chre::SensorMode;
using chre::SensorRequest;
using chre::SensorType;
using chre::kMaxIntervalLatencyNs;

TEST(SensorType, LosslessSensorHandleToSensorTypeAndBack) {
  // Verify that converting a sensor to a handle and from a handle back to a
  // sensor is a lossless conversion. The specific value of the handle is
  // unimportant, as long as it can be consistently converted back and forth.
  SensorType sensorType = SensorType::Pressure;
  uint32_t sensorHandle = getSensorHandleFromSensorType(sensorType);
  sensorType = chre::getSensorTypeFromSensorHandle(sensorHandle);
  EXPECT_EQ(sensorType, SensorType::Pressure);

  sensorType = SensorType::Proximity;
  sensorHandle = getSensorHandleFromSensorType(sensorType);
  sensorType = chre::getSensorTypeFromSensorHandle(sensorHandle);
  EXPECT_EQ(sensorType, SensorType::Proximity);
}

TEST(SensorType, SensorHandleToSensorTypeUnknownHandles) {
  EXPECT_EQ(chre::getSensorTypeFromSensorHandle(0), SensorType::Unknown);
  EXPECT_EQ(chre::getSensorTypeFromSensorHandle(10000), SensorType::Unknown);
}

TEST(SensorRequest, DefaultMinimalPriority) {
  SensorRequest request;
  EXPECT_EQ(request.getInterval(), Nanoseconds(CHRE_SENSOR_INTERVAL_DEFAULT));
  EXPECT_EQ(request.getLatency(), Nanoseconds(CHRE_SENSOR_LATENCY_DEFAULT));
  EXPECT_EQ(request.getMode(), SensorMode::Off);
}

TEST(SensorRequest, ActiveContinuousIsHigherPriorityThanActiveOneShot) {
  SensorRequest activeContinuous(SensorMode::ActiveContinuous,
                                 Nanoseconds(0), Nanoseconds(0));
  SensorRequest activeOneShot(SensorMode::ActiveOneShot,
                              Nanoseconds(0), Nanoseconds(0));
  SensorRequest mergedRequest;
  EXPECT_TRUE(mergedRequest.mergeWith(activeContinuous));
  EXPECT_FALSE(mergedRequest.mergeWith(activeOneShot));
  EXPECT_EQ(mergedRequest.getInterval(), Nanoseconds(0));
  EXPECT_EQ(mergedRequest.getLatency(), Nanoseconds(0));
  EXPECT_EQ(mergedRequest.getMode(), SensorMode::ActiveContinuous);
}

TEST(SensorRequest, ActiveOneShotIsHigherPriorityThanPassiveContinuous) {
  SensorRequest activeOneShot(SensorMode::ActiveOneShot,
                              Nanoseconds(0), Nanoseconds(0));
  SensorRequest passiveContinuous(SensorMode::PassiveContinuous,
                                  Nanoseconds(0), Nanoseconds(0));
  SensorRequest mergedRequest;
  EXPECT_TRUE(mergedRequest.mergeWith(activeOneShot));
  EXPECT_FALSE(mergedRequest.mergeWith(passiveContinuous));
  EXPECT_EQ(mergedRequest.getInterval(), Nanoseconds(0));
  EXPECT_EQ(mergedRequest.getLatency(), Nanoseconds(0));
  EXPECT_EQ(mergedRequest.getMode(), SensorMode::ActiveOneShot);

}

TEST(SensorRequest, PassiveContinuousIsHigherPriorityThanPassiveOneShot) {
  SensorRequest passiveContinuous(SensorMode::PassiveContinuous,
                                  Nanoseconds(0), Nanoseconds(0));
  SensorRequest passiveOneShot(SensorMode::PassiveOneShot,
                               Nanoseconds(0), Nanoseconds(0));
  SensorRequest mergedRequest;
  EXPECT_TRUE(mergedRequest.mergeWith(passiveContinuous));
  EXPECT_FALSE(mergedRequest.mergeWith(passiveOneShot));
  EXPECT_EQ(mergedRequest.getInterval(), Nanoseconds(0));
  EXPECT_EQ(mergedRequest.getLatency(), Nanoseconds(0));
  EXPECT_EQ(mergedRequest.getMode(), SensorMode::PassiveContinuous);
}

TEST(SensorRequest, PassiveOneShotIsHigherPriorityThanOff) {
  SensorRequest passiveOneShot(SensorMode::PassiveOneShot,
                               Nanoseconds(0), Nanoseconds(0));
  SensorRequest off(SensorMode::Off, Nanoseconds(0), Nanoseconds(0));
  SensorRequest mergedRequest;
  EXPECT_TRUE(mergedRequest.mergeWith(passiveOneShot));
  EXPECT_FALSE(mergedRequest.mergeWith(off));
  EXPECT_EQ(mergedRequest.getInterval(), Nanoseconds(0));
  EXPECT_EQ(mergedRequest.getLatency(), Nanoseconds(0));
  EXPECT_EQ(mergedRequest.getMode(), SensorMode::PassiveOneShot);
}

TEST(SensorRequest, LowerLatencyIsHigherPriorityThanHigherLatency) {
  SensorRequest lowLatencyRequest(SensorMode::ActiveContinuous,
                                  Nanoseconds(10), Nanoseconds(10));
  SensorRequest highLatencyRequest(SensorMode::ActiveOneShot,
                                   Nanoseconds(10), Nanoseconds(100));
  SensorRequest mergedRequest;
  EXPECT_TRUE(mergedRequest.mergeWith(lowLatencyRequest));
  EXPECT_FALSE(mergedRequest.mergeWith(highLatencyRequest));
  EXPECT_EQ(mergedRequest.getInterval(), Nanoseconds(10));
  EXPECT_EQ(mergedRequest.getLatency(), Nanoseconds(10));
  EXPECT_EQ(mergedRequest.getMode(), SensorMode::ActiveContinuous);
}

TEST(SensorRequest, HigherFrequencyIsHigherPriorityThanLowerFrequency) {
  SensorRequest lowFreqRequest(SensorMode::ActiveOneShot,
                               Nanoseconds(100), Nanoseconds(10));
  SensorRequest highFreqRequest(SensorMode::ActiveContinuous,
                                Nanoseconds(10), Nanoseconds(10));
  SensorRequest mergedRequest;
  EXPECT_TRUE(mergedRequest.mergeWith(lowFreqRequest));
  EXPECT_TRUE(mergedRequest.mergeWith(highFreqRequest));
  EXPECT_EQ(mergedRequest.getInterval(), Nanoseconds(10));
  EXPECT_EQ(mergedRequest.getLatency(), Nanoseconds(10));
  EXPECT_EQ(mergedRequest.getMode(), SensorMode::ActiveContinuous);
}

TEST(SensorRequest, OnlyDefaultFrequency) {
  SensorRequest defaultFreqRequest(SensorMode::ActiveContinuous,
                                   Nanoseconds(CHRE_SENSOR_INTERVAL_DEFAULT),
                                   Nanoseconds(0));
  SensorRequest mergedRequest;
  EXPECT_TRUE(mergedRequest.mergeWith(defaultFreqRequest));
  EXPECT_EQ(mergedRequest.getInterval(),
            Nanoseconds(CHRE_SENSOR_INTERVAL_DEFAULT));
  EXPECT_EQ(mergedRequest.getLatency(), Nanoseconds(0));
  EXPECT_EQ(mergedRequest.getMode(), SensorMode::ActiveContinuous);
}

TEST(SensorRequest, NonDefaultAndDefaultFrequency) {
  SensorRequest defaultFreqRequest(SensorMode::ActiveContinuous,
                                   Nanoseconds(CHRE_SENSOR_INTERVAL_DEFAULT),
                                   Nanoseconds(0));
  SensorRequest nonDefaultFreqRequest(SensorMode::ActiveContinuous,
                                      Nanoseconds(20000000), Nanoseconds(0));
  SensorRequest mergedRequest;
  EXPECT_TRUE(mergedRequest.mergeWith(defaultFreqRequest));
  EXPECT_TRUE(mergedRequest.mergeWith(nonDefaultFreqRequest));
  EXPECT_EQ(mergedRequest.getInterval(), Nanoseconds(20000000));
  EXPECT_EQ(mergedRequest.getLatency(), Nanoseconds(0));
  EXPECT_EQ(mergedRequest.getMode(), SensorMode::ActiveContinuous);
}

TEST(SensorRequest, OnlyAsapLatency) {
  SensorRequest asapLatencyRequest(SensorMode::ActiveContinuous,
                                   Nanoseconds(10),
                                   Nanoseconds(CHRE_SENSOR_LATENCY_ASAP));
  SensorRequest mergedRequest;
  EXPECT_TRUE(mergedRequest.mergeWith(asapLatencyRequest));
  EXPECT_EQ(mergedRequest.getInterval(), Nanoseconds(10));
  EXPECT_EQ(mergedRequest.getLatency(),
            Nanoseconds(CHRE_SENSOR_LATENCY_ASAP));
  EXPECT_EQ(mergedRequest.getMode(), SensorMode::ActiveContinuous);
}

TEST(SensorRequest, NonAsapAndAsapLatency) {
  SensorRequest asapLatencyRequest(SensorMode::ActiveContinuous,
                                   Nanoseconds(10),
                                   Nanoseconds(CHRE_SENSOR_LATENCY_ASAP));
  SensorRequest nonAsapLatencyRequest(SensorMode::ActiveContinuous,
                                      Nanoseconds(10),
                                      Nanoseconds(2000));
  SensorRequest mergedRequest;
  EXPECT_TRUE(mergedRequest.mergeWith(asapLatencyRequest));
  EXPECT_FALSE(mergedRequest.mergeWith(nonAsapLatencyRequest));
  EXPECT_EQ(mergedRequest.getInterval(), Nanoseconds(10));
  EXPECT_EQ(mergedRequest.getLatency(),
            Nanoseconds(CHRE_SENSOR_LATENCY_ASAP));
  EXPECT_EQ(mergedRequest.getMode(), SensorMode::ActiveContinuous);
}

TEST(SensorRequest, OnlyDefaultLatency) {
  SensorRequest defaultLatencyRequest(SensorMode::ActiveContinuous,
                                      Nanoseconds(10),
                                      Nanoseconds(CHRE_SENSOR_LATENCY_DEFAULT));
  SensorRequest mergedRequest;
  EXPECT_TRUE(mergedRequest.mergeWith(defaultLatencyRequest));
  EXPECT_EQ(mergedRequest.getInterval(), Nanoseconds(10));
  EXPECT_EQ(mergedRequest.getLatency(),
            Nanoseconds(CHRE_SENSOR_LATENCY_DEFAULT));
  EXPECT_EQ(mergedRequest.getMode(), SensorMode::ActiveContinuous);
}

TEST(SensorRequest, NonDefaultAndDefaultLatency) {
  SensorRequest defaultLatencyRequest(SensorMode::ActiveContinuous,
                                      Nanoseconds(10),
                                      Nanoseconds(CHRE_SENSOR_LATENCY_DEFAULT));
  SensorRequest nonDefaultLatencyRequest(SensorMode::ActiveContinuous,
                                         Nanoseconds(10),
                                         Nanoseconds(2000));
  SensorRequest mergedRequest;
  EXPECT_TRUE(mergedRequest.mergeWith(defaultLatencyRequest));
  EXPECT_TRUE(mergedRequest.mergeWith(nonDefaultLatencyRequest));
  EXPECT_EQ(mergedRequest.getInterval(), Nanoseconds(10));
  EXPECT_EQ(mergedRequest.getLatency(), Nanoseconds(2000));
  EXPECT_EQ(mergedRequest.getMode(), SensorMode::ActiveContinuous);
}

TEST(SensorRequest, MergeWithOff) {
  SensorRequest request(SensorMode::ActiveContinuous,
                        Nanoseconds(10),
                        Nanoseconds(100));
  SensorRequest otherRequest(SensorMode::Off,
                             Nanoseconds(1),
                             Nanoseconds(1));
  EXPECT_FALSE(request.mergeWith(otherRequest));
  EXPECT_EQ(request.getMode(), SensorMode::ActiveContinuous);
  EXPECT_EQ(request.getInterval(), Nanoseconds(10));
  EXPECT_EQ(request.getLatency(), Nanoseconds(100));
}

TEST(SensorRequest, MaxNonDefaultIntervalAndLatency) {
  SensorRequest request(SensorMode::ActiveContinuous,
                        Nanoseconds(CHRE_SENSOR_INTERVAL_DEFAULT - 1),
                        Nanoseconds(CHRE_SENSOR_LATENCY_DEFAULT - 1));
  EXPECT_EQ(request.getMode(), SensorMode::ActiveContinuous);
  EXPECT_EQ(request.getInterval(), Nanoseconds(kMaxIntervalLatencyNs));
  EXPECT_EQ(request.getLatency(), Nanoseconds(kMaxIntervalLatencyNs));
}

TEST(SensorRequest, HighRateLowLatencyAndLowRateHighLatency) {
  SensorRequest Request0(SensorMode::ActiveContinuous,
                         Nanoseconds(100), Nanoseconds(0));
  SensorRequest Request1(SensorMode::ActiveContinuous,
                         Nanoseconds(10), Nanoseconds(2000));
  SensorRequest mergedRequest;
  EXPECT_TRUE(mergedRequest.mergeWith(Request0));
  EXPECT_EQ(mergedRequest.getInterval(), Nanoseconds(100));
  EXPECT_EQ(mergedRequest.getLatency(), Nanoseconds(0));
  EXPECT_EQ(mergedRequest.getMode(), SensorMode::ActiveContinuous);

  EXPECT_TRUE(mergedRequest.mergeWith(Request1));
  EXPECT_EQ(mergedRequest.getInterval(), Nanoseconds(10));
  EXPECT_EQ(mergedRequest.getLatency(), Nanoseconds(90));
  EXPECT_EQ(mergedRequest.getMode(), SensorMode::ActiveContinuous);
}

TEST(SensorRequest, LowRateHighLatencyAndHighRateLowLatency) {
  SensorRequest Request0(SensorMode::ActiveContinuous,
                         Nanoseconds(100), Nanoseconds(0));
  SensorRequest Request1(SensorMode::ActiveContinuous,
                         Nanoseconds(10), Nanoseconds(2000));
  SensorRequest mergedRequest;
  EXPECT_TRUE(mergedRequest.mergeWith(Request1));
  EXPECT_EQ(mergedRequest.getInterval(), Nanoseconds(10));
  EXPECT_EQ(mergedRequest.getLatency(), Nanoseconds(2000));
  EXPECT_EQ(mergedRequest.getMode(), SensorMode::ActiveContinuous);

  EXPECT_TRUE(mergedRequest.mergeWith(Request0));
  EXPECT_EQ(mergedRequest.getInterval(), Nanoseconds(10));
  EXPECT_EQ(mergedRequest.getLatency(), Nanoseconds(90));
  EXPECT_EQ(mergedRequest.getMode(), SensorMode::ActiveContinuous);
}
