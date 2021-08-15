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

#ifndef CHRE_CORE_SENSOR_H_
#define CHRE_CORE_SENSOR_H_

#include "chre/platform/platform_sensor.h"
#include "chre/util/non_copyable.h"
#include "chre/util/optional.h"

namespace chre {

/**
 * Represents a sensor in the system that is exposed to nanoapps in CHRE.
 *
 * Note that like chre::Nanoapp, this class uses inheritance to separate the
 * common code (Sensor) from common interface with platform-specific
 * implementation (PlatformSensor) from the fully platform-specific part
 * (PlatformSensorBase). However, this inheritance relationship does *not* imply
 * polymorphism, and this object must only be referred to via the most-derived
 * type, i.e. chre::Sensor.
 */
class Sensor : public PlatformSensor {
 public:
  /**
   * Constructs a sensor in an unspecified state. Should not be called directly
   * by common code, as platform-specific initialization of the Sensor object is
   * required for it to be usable.
   *
   * @see PlatformSensor::getSensors
   */
  Sensor() = default;

  Sensor(Sensor&& other) = default;
  Sensor& operator=(Sensor&& other) = default;

  /**
   * Obtains a reference to the latest request that has been accepted by the
   * platform.
   *
   * @return A const reference to the SensorRequest.
   */
  const SensorRequest& getRequest() const {
    return mSensorRequest;
  }

  /**
   * Sets the current request of this sensor. If this request is a change from
   * the previous request, it is sent to the underlying platform. If isValid()
   * returns false this function will also return false and do nothing.
   *
   * @param request The new request for this sensor.
   * @return true if there was no change required or the platform has set the
   *         request successfully.
   */
  bool setRequest(const SensorRequest& request);

 private:
  //! The most recent sensor request accepted by the platform.
  SensorRequest mSensorRequest;
};

}  // namespace chre

#endif  // CHRE_CORE_SENSOR_H_
