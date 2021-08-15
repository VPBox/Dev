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

#include "chre/platform/shared/platform_sensor_util.h"

#include <cinttypes>

#include "chre/core/event_loop_manager.h"
#include "chre/core/sensor.h"
#include "chre/platform/assert.h"
#include "chre/util/memory.h"

namespace chre {

void updateLastEvent(SensorType sensorType, const void *eventData) {
  CHRE_ASSERT(eventData);

  auto *header = static_cast<const chreSensorDataHeader *>(eventData);
  if (header->readingCount != 1) {
    // TODO: better error handling when there are more than one samples.
    LOGE("%" PRIu16 " samples in an event for on-change sensor %" PRIu8,
         header->readingCount, static_cast<uint8_t>(sensorType));
  } else {
    struct CallbackData {
      SensorType sensorType;
      const ChreSensorData *event;
    };
    auto *callbackData = memoryAlloc<CallbackData>();
    if (callbackData == nullptr) {
      LOG_OOM();
    } else {
      callbackData->sensorType = sensorType;
      callbackData->event = static_cast<const ChreSensorData *>(eventData);

      auto callback = [](uint16_t /* type */, void *data) {
        auto *cbData = static_cast<CallbackData *>(data);

        Sensor *sensor = EventLoopManagerSingleton::get()
            ->getSensorRequestManager().getSensor(cbData->sensorType);

        // Mark last event as valid only if the sensor is enabled. Event data
        // may arrive after sensor is disabled.
        if (sensor != nullptr
            && sensor->getRequest().getMode() != SensorMode::Off) {
          sensor->setLastEvent(cbData->event);
        }
        memoryFree(cbData);
      };

      // Schedule a deferred callback.
      EventLoopManagerSingleton::get()->deferCallback(
          SystemCallbackType::SensorLastEventUpdate, callbackData, callback);
    }
  }
}

void sensorDataEventFree(uint16_t eventType, void *eventData) {
  // TODO: Consider using a MemoryPool.
  memoryFree(eventData);

  // Remove all requests if it's a one-shot sensor and only after data has been
  // delivered to all clients.
  SensorType sensorType = getSensorTypeForSampleEventType(eventType);
  if (sensorTypeIsOneShot(sensorType)) {
    EventLoopManagerSingleton::get()
        ->getSensorRequestManager()
        .removeAllRequests(sensorType);
  }
}

}  // namespace chre
