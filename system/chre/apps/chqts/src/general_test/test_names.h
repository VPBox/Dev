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

#ifndef _GTS_NANOAPPS_GENERAL_TEST_TEST_NAMES_H_
#define _GTS_NANOAPPS_GENERAL_TEST_TEST_NAMES_H_

#include <cstdint>

/**
 * NOTE: These values are manually synced in the GTS Java's
 *     ContextHubTestConstants.java.  If you make a change here, be sure
 *     to update ContextHubTestContants.java as well.
 */

namespace general_test {

/**
 * Names of the tests we support.
 */
enum class TestNames : uint32_t {
  /**
   * Value which should never be used.
   *
   * This starts at CONTEXT_HUB_TYPE_PRIVATE_MSG_BASE.
   */
  kInvalidTest = 0x0400,

  /**
   * Test: HelloWorldTest
   */
  kHelloWorld = 0x0401,

  /**
   * Test: SimpleHeapAllocTest
   */
  kSimpleHeapAlloc = 0x0402,

  /**
   * Test: HeapAllocStressTest
   */
  kHeapAllocStress = 0x0403,

  /**
   * Test: GetTimeTest
   */
  kGetTime = 0x0404,

  /**
   * Test: EventBetweenApps0
   */
  kEventBetweenApps0 = 0x0405,

  /**
   * Test: EventBetweenApps1
   */
  kEventBetweenApps1 = 0x0406,

  /**
   * Test: SendEventTest
   */
  kSendEvent = 0x0407,

  /**
   * Test: BasicAccelerometerTest
   */
  kBasicAccelerometer = 0x0408,

  /**
   * Test: BasicInstantMotionDetectTest
   */
  kBasicInstantMotionDetect = 0x0409,

  /**
   * Test: BasicStationaryDetectTest
   */
  kBasicStationaryDetect = 0x040A,

  /**
   * Test: BasicGyroscopeTest
   */
  kBasicGyroscope = 0x040B,

  /**
   * Test: BasicMagnetometerTest
   */
  kBasicMagnetometer = 0x040C,

  /**
   * Test: BasicBarometerTest
   */
  kBasicBarometer = 0x040D,

  /**
   * Test: BasicLightSensorTest
   */
  kBasicLightSensor = 0x040E,

  /**
   * Test: BasicProximityTest
   */
  kBasicProximity = 0x040F,

  /**
   * Test: VersionSanityTest
   */
  kVersionSanity = 0x0410,

  /**
   * Test: LoggingSanityTest
   */
  kLoggingSanity = 0x0411,

  /**
   * Test: SendMessageToHostTest
   */
  kSendMessageToHost = 0x0412,

  /**
   * Test: TimerSetTest
   */
  kTimerSet = 0x0413,

  /**
   * Test: TimerCancelTest
   */
  kTimerCancel = 0x0414,

  /**
   * Test: TimerStressTest
   */
  kTimerStress = 0x0415,

  /**
   * Test: SendEventStressTest
   */
  kSendEventStress = 0x0416,

  /**
   * Test: HeapExhaustionStabilityTest
   */
  kHeapExhaustionStability = 0x0417,

  /**
   * Test: GnssCapabilitiesTest
   */
  kGnssCapabilities = 0x0418,

  /**
   * Test: WifiCapablitiesTest
   */
  kWifiCapabilities = 0x0419,

  /**
   * Test: WwanCapabilitiesTest
   */
  kWwanCapabilities = 0x041A,

  /**
   * Test: SensorInfoTest
   */
  kSensorInfo = 0x041B,

  /**
   * Test: WwanCellInfoTest
   */
  kWwanCellInfoTest = 0x041C,

  /**
   * Test: EstimatedHostTimeTest
   */
  kEstimatedHostTime = 0x041D,

  /**
   * Test: NanoappInfoByAppId
   */
  kNanoappInfoByAppId = 0x041E,

  /**
   * Test: NanoappInfoByInstanceId
   */
  kNanoappInfoByInstanceId = 0x041F,

  /**
   * Test: NanoAppInfoEventsTest
   */
  kNanoAppInfoEventsPerformer = 0x0420,

  /**
   * Test: NanoAppInfoEventsTest
   */
  kNanoAppInfoEventsObserver = 0x0421,

  /**
   * Test: BasicAudioTest
   */
  kBasicAudioTest = 0x0422,

  /**
   * Test: HostAwakeSuspendTest
   */
  kHostAwakeSuspend = 0x0423,

  /**
   * Test: BasicGnssTest
   */
  kBasicGnssTest = 0x0424,

  /**
   * Test: BasicWifiTest
   */
  kBasicWifiTest = 0x0425,
};

}  // namespace general_test


#endif  // _GTS_NANOAPPS_GENERAL_TEST_TEST_NAMES_H_
