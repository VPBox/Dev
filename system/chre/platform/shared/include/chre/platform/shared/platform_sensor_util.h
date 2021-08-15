/*
 * Copyright (C) 2018 The Android Open Source Project
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

#ifndef CHRE_PLATFORM_SHARED_PLATFORM_SENSOR_UTIL_H_
#define CHRE_PLATFORM_SHARED_PLATFORM_SENSOR_UTIL_H_

#include "chre/core/sensor_type.h"

namespace chre {

/**
 * A helper function that updates the last event of a sensor in the main thread.
 * Platform should call this function only for an on-change sensor.
 *
 * @param sensorType The SensorType of the sensor.
 * @param eventData A non-null pointer to the sensor's CHRE event data.
 */
void updateLastEvent(SensorType sensorType, const void *eventData);

/**
 * A helper chreEventCompleteFunction that handles freeing sensor data and
 * removing all requests associated with the sensor type if it represents a
 * one-shot sensor.
 *
 * NOTE: This function assumes the eventData was allocated using the memoryAlloc
 * platform function.
 *
 * @see postEvent
 * @param eventType The type of sample event that eventData represents.
 * @param eventData The sensor event data that should be freed.
 */
void sensorDataEventFree(uint16_t eventType, void *eventData);

}  // namespace chre

#endif  // CHRE_PLATFORM_SHARED_PLATFORM_SENSOR_UTIL_H_
