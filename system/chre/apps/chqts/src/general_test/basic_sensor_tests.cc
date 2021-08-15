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

#include <general_test/basic_sensor_tests.h>

#include <shared/send_message.h>

using nanoapp_testing::sendFatalFailureToHost;

namespace general_test {

static void checkFloat(float value, float extremeLow, float extremeHigh) {
  if ((value < extremeLow) || (value > extremeHigh)) {
    uint32_t i = static_cast<uint32_t>(value);
    sendFatalFailureToHost("Value beyond extreme.  As int:", &i);
  }
}

static void checkTimestampDelta(uint32_t delta, size_t index) {
  if (index == 0) {
    // This delta is allowed (and expected, but not required) to be 0.
    return;
  }
  if (delta == 0) {
    uint32_t indexInt = static_cast<uint32_t>(index);
    sendFatalFailureToHost("timestampDelta was 0 for reading index ",
                           &indexInt);
  }
}

static void sanityCheckThreeAxisData(const void *eventData, float extremeLow,
                                     float extremeHigh) {
  auto data = static_cast<const chreSensorThreeAxisData*>(eventData);
  for (size_t i = 0; i < data->header.readingCount; i++) {
    checkTimestampDelta(data->readings[i].timestampDelta, i);
    for (size_t j = 0; j < 3; j++) {
      checkFloat(data->readings[i].values[j], extremeLow, extremeHigh);
    }
  }
}

static void sanityCheckFloatData(const void *eventData, float extremeLow,
                                 float extremeHigh) {
  auto data = static_cast<const chreSensorFloatData*>(eventData);
  for (size_t i = 0; i < data->header.readingCount; i++) {
    checkTimestampDelta(data->readings[i].timestampDelta, i);
    checkFloat(data->readings[i].value, extremeLow, extremeHigh);
  }
}

void BasicAccelerometerTest::confirmDataIsSane(const void* eventData) {
  constexpr float kExtreme = 70.5f;  // Apollo 16 on reentry (7.19g)
  sanityCheckThreeAxisData(eventData, -kExtreme, kExtreme);
}

void BasicInstantMotionDetectTest::confirmDataIsSane(const void* eventData) {
  // Nothing to sanity check.
}

void BasicStationaryDetectTest::confirmDataIsSane(const void* eventData) {
  // Nothing to sanity check.
}

void BasicGyroscopeTest::confirmDataIsSane(const void* eventData) {
  constexpr float kExtreme = 9420.0f;  // Zippe centrifuge
  sanityCheckThreeAxisData(eventData, -kExtreme, kExtreme);
}

void BasicMagnetometerTest::confirmDataIsSane(const void* eventData) {
  constexpr float kExtreme = 9400000.0f;  // Strength of medical MRI
  sanityCheckThreeAxisData(eventData, -kExtreme, kExtreme);
}

void BasicBarometerTest::confirmDataIsSane(const void* eventData) {
  constexpr float kExtremeLow = 337.0f;  // Mount Everest summit
  constexpr float kExtremeHigh = 1067.0f;  // Dead Sea
  sanityCheckFloatData(eventData, kExtremeLow, kExtremeHigh);
}

void BasicLightSensorTest::confirmDataIsSane(const void* eventData) {
  constexpr float kExtreme = 300000.0f; // 3x the Sun
  sanityCheckFloatData(eventData, 0.0f, kExtreme);
}

void BasicProximityTest::confirmDataIsSane(const void* eventData) {
  auto data = static_cast<const chreSensorByteData*>(eventData);
  for (size_t i = 0; i < data->header.readingCount; i++) {
    checkTimestampDelta(data->readings[i].timestampDelta, i);
    // 'invalid' is a sane reading for v1.1 or lower.  But our padding should
    // always be zero'd.
    if (mApiVersion >= CHRE_API_VERSION_1_2 && data->readings[i].invalid) {
      sendFatalFailureToHost("Invalid flag must not be set for proximity");
    }
    if (data->readings[i].padding0 != 0) {
      uint32_t padding = data->readings[i].padding0;
      sendFatalFailureToHost("padding0 is data is non-zero:", &padding);
    }
  }
}


}  // namespace general_test
