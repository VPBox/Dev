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

#ifndef _GTS_NANOAPPS_GENERAL_TEST_BASIC_SENSOR_TEST_BASE_H_
#define _GTS_NANOAPPS_GENERAL_TEST_BASIC_SENSOR_TEST_BASE_H_

#include <general_test/test.h>

#include <chre.h>

namespace general_test {

/**
 * Abstract base class for basic sensor tests.
 *
 * This repeats a similar test for several different sensor types.  Children
 * classes must implement the abstract methods to define details about the
 * sensor.
 *
 * This uses a Simple Protocol between the Host and Nanoapp.
 */
class BasicSensorTestBase : public Test {
 public:
  BasicSensorTestBase();

 protected:
  void handleEvent(uint32_t senderInstanceId, uint16_t eventType,
                   const void* eventData) override;
  void setUp(uint32_t messageSize, const void *message) override;

  /**
   * Abstract method indicating which sensor type this is.
   *
   * @returns One of the CHRE_SENSOR_TYPE_* constants.
   */
  virtual uint8_t getSensorType() const = 0;

  /**
   * Abstract method indicating if this sensor is required for a valid
   * CHRE implementation.
   *
   * A CHRE isn't useful without certain sensors available.
   *
   * @returns true if this is sensor is required; false otherwise.
   */
  virtual bool isRequiredSensor() const = 0;

  /**
   * Abstract method indicating if this is an on-change sensor.
   *
   * @returns true if this sensor is on-change; false otherwise.
   */
  virtual bool isOnChangeSensor() const = 0;

  /**
   * Abstract method indicating if this is a one-shot sensor.
   *
   * @returns true if this sensor is one-shot; false otherwise.
   */
  virtual bool isOneShotSensor() const = 0;

  /**
   * Abstract method which makes sure the given data is sane.
   *
   * This is a very loose test, and some sensors may provide no checking
   * at all here.  But some sensor might be able to provide a basic check
   * (for example, a barometer claiming 0 hPa is broken (assuming the tests
   * aren't running in outer space)).
   *
   * @returns If the data is absurd, this function will not return (it
   *     will trigger a fatal error report).  This function returning
   *     is evidence the data is sane.
   */
  virtual void confirmDataIsSane(const void* eventData) = 0;

 private:
  enum State {
    kPreStart,
    kPreConfigure,
    kExpectingInitialDataEvent,
    kExpectingLastDataEvent,
    kFinished
  };

  // Catch if CHRE performs reentrant calls for handleEvent()
  bool mInMethod;
  // If some external user changes the sampling status of our sensor,
  // we shouldn't perform some of the checking, because it will be flaky.
  bool mExternalSamplingStatusChange;
  State mState;
  uint32_t mInstanceId;
  uint32_t mSensorHandle;
  uint64_t mPreTimestamp;
  uint64_t mFirstEventTimestamp;
  uint64_t mLastEventTimestamp;
  uint64_t mDoneTimestamp;
  chreSensorSamplingStatus mOriginalStatus;
  chreSensorSamplingStatus mNewStatus;

  void startTest();
  void finishTest();
  void checkPassiveConfigure();
  void handleBiasEvent(uint16_t eventType,
                       const chreSensorThreeAxisData* eventData);
  void handleSamplingChangeEvent(
      const chreSensorSamplingStatusEvent* eventData);
  void handleSensorDataEvent(const void* eventData);
  void sanityCheckHeader(const chreSensorDataHeader* header,
                         bool modifyTimestamps,
                         uint64_t eventDuration);
};

}  // namespace general_test


#endif  // _GTS_NANOAPPS_GENERAL_TEST_BASIC_SENSOR_TEST_BASE_H_
