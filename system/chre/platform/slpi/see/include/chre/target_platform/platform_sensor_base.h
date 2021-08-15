/*
 * Copyright (C) 2017 The Android Open Source Project
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

#ifndef CHRE_PLATFORM_SLPI_SEE_PLATFORM_SENSOR_BASE_H_
#define CHRE_PLATFORM_SLPI_SEE_PLATFORM_SENSOR_BASE_H_

#include "chre/core/sensor_request.h"
#include "chre/platform/slpi/see/see_helper.h"

namespace chre {

//! The max length of sensorName
constexpr size_t kSensorNameMaxLen = 64;

/**
 * Storage for the SLPI SEE implementation of the PlatformSensor class.
 */
class PlatformSensorBase {
 public:
  /**
   * Initializes the members of PlatformSensorBase.
   */
  void initBase(
      SensorType sensorType, uint64_t mMinInterval, const char *sensorName,
      ChreSensorData *lastEvent, size_t lastEventSize, bool passiveSupported);

  /**
   * Copies the supplied event to the sensor's last event and marks last event
   * valid.
   *
   * @param event The pointer to the event to copy from.
   */
  void setLastEvent(const ChreSensorData *event);

  /**
   * Sets the current status of this sensor in the CHRE API format.
   *
   * @param status The current sampling status.
   */
  void setSamplingStatus(const struct chreSensorSamplingStatus& status);

  //! Stores the last received sampling status from SEE for this sensor making
  //! it easier to dedup updates that come in later from SEE.
  SeeHelperCallbackInterface::SamplingStatusData mLastReceivedSamplingStatus {};

 protected:
  //! The sensor type of this sensor.
  SensorType mSensorType;

  //! The minimum interval of this sensor.
  uint64_t mMinInterval;

  //! The name (type and model) of this sensor.
  char mSensorName[kSensorNameMaxLen];

  //! Pointer to dynamically allocated memory to store the last event. Only
  //! non-null if this is an on-change sensor.
  ChreSensorData *mLastEvent = nullptr;

  //! The amount of memory we've allocated in lastEvent (this varies depending
  //! on the sensor type)
  size_t mLastEventSize = 0;

  //! Set to true only when this is an on-change sensor that is currently active
  //! and we have a copy of the most recent event in lastEvent.
  bool mLastEventValid = false;

  //! Whether this sensor supports passive sensor requests.
  bool mPassiveSupported = false;

  //! Stores the sampling status for all CHRE clients of this sensor.
  struct chreSensorSamplingStatus mSamplingStatus;
};

}  // namespace chre

#endif  // CHRE_PLATFORM_SLPI_SEE_PLATFORM_SENSOR_BASE_H_
