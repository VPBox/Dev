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

#ifndef CHRE_PLATFORM_LINUX_PLATFORM_SENSOR_BASE_H_
#define CHRE_PLATFORM_LINUX_PLATFORM_SENSOR_BASE_H_

#include <cstddef>

#include "chre/core/sensor_type.h"

namespace chre {

/**
 * Storage for the Linux implementation of the PlatformSensor class.
 */
class PlatformSensorBase {
 public:
  //! The maximum size of a Linux sensor string.
  static constexpr size_t kMaxSensorNameSize = 32;

  //! The name of this sensor for the Linux platform.
  char sensorName[kMaxSensorNameSize];

  /**
   * Copies the supplied event to the sensor's last event and marks last event
   * valid.
   *
   * @param event The pointer to the event to copy from.
   */
  void setLastEvent(const ChreSensorData *event);
};

}  // namespace chre

#endif  // CHRE_PLATFORM_LINUX_PLATFORM_SENSOR_BASE_H_
