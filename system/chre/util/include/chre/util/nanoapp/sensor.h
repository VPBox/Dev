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

#ifndef CHRE_UTIL_NANOAPP_SENSOR_H_
#define CHRE_UTIL_NANOAPP_SENSOR_H_

#include <cstdint>

namespace chre {

/**
 * Returns the name of a sensor for a given event type.
 *
 * @param eventType the type of a sensor sample event.
 * @return the name of the sensor that produced this event type.
 */
const char *getSensorNameForEventType(uint16_t eventType);

/**
 * Returns the name of a given CHRE API sensor type.
 *
 * @param sensorType the type of a sensor.
 * @return the name of the sensor.
 */
const char *getSensorTypeName(uint8_t sensorType);

}  // namespace chre

#endif  // CHRE_UTIL_NANOAPP_SENSOR_H_
