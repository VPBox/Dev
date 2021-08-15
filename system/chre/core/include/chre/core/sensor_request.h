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

#ifndef CHRE_CORE_SENSOR_REQUEST_H_
#define CHRE_CORE_SENSOR_REQUEST_H_

#include <cstdint>

#include "chre_api/chre/sensor.h"
#include "chre/core/nanoapp.h"
#include "chre/core/sensor_type.h"
#include "chre/util/time.h"

namespace chre {

//! Maximum of non-default interval and latency values in nanoseconds to ensure
//! no overflow in CHRE operations.
constexpr uint64_t kMaxIntervalLatencyNs = (UINT64_MAX - 1) / 2;

/**
 * Models a request for sensor data. This class implements the API set forth by
 * the RequestMultiplexer container.
 */
class SensorRequest {
 public:
  /**
   * Default constructs a sensor request to the minimal possible configuration.
   * The sensor is disabled and the interval and latency are both set to zero.
   */
  SensorRequest();

  /**
   * Constructs a sensor request given a mode, interval and latency. Non-default
   * interval or latency higher than kMaxIntervalLatencyNs will be capped.
   *
   * @param mode The mode of the sensor request.
   * @param interval The interval between samples.
   * @param latency The maximum amount of time to batch samples before
   *        delivering to a client.
   */
  SensorRequest(SensorMode mode, Nanoseconds interval, Nanoseconds latency);

  /**
   * Constructs a sensor request given an owning nanoapp, mode, interval and
   * latency. Non-default interval or latency higher than kMaxIntervalLatencyNs
   * will be capped.
   *
   * @param instanceId The instance ID of the nanoapp that made this request.
   * @param mode The mode of the sensor request.
   * @param interval The interval between samples.
   * @param latency The maximum amount of time to batch samples before
   *        delivering to a client.
   */
  SensorRequest(uint32_t instanceId, SensorMode mode, Nanoseconds interval,
                Nanoseconds latency);

  /**
   * Performs an equivalency comparison of two sensor requests. This determines
   * if the effective request for sensor data is the same as another.
   *
   * @param request The request to compare against.
   * @return Returns true if this request is equivalent to another.
   */
  bool isEquivalentTo(const SensorRequest& request) const;

  /**
   * Assigns the current request to the maximal superset of the mode, rate
   * and latency of the other request.
   *
   * @param request The other request to compare the attributes of.
   * @return true if any of the attributes of this request changed.
   */
  bool mergeWith(const SensorRequest& request);

  /**
   * @return Returns the interval of samples for this request.
   */
  Nanoseconds getInterval() const {
    return mInterval;
  }

  /**
   * @return Returns the maximum amount of time samples can be batched prior to
   * dispatching to the client.
   */
  Nanoseconds getLatency() const {
    return mLatency;
  }

  /**
   * @return The mode of this request.
   */
  SensorMode getMode() const {
    return mMode;
  }

  /**
   * @return The instance ID of the nanoapp that owns this request.
   */
  uint32_t getInstanceId() const {
    return mInstanceId;
  }

 private:
  //! The nanoapp that made this request or zero when unset. This will be
  //! kInvalidInstanceId when returned by the generateIntersectionOf method.
  uint32_t mInstanceId = kInvalidInstanceId;

  //! The interval between samples for this request.
  Nanoseconds mInterval;

  //! The maximum amount of time samples can be batched prior to dispatching to
  //! the client
  Nanoseconds mLatency;

  //! The mode of this request.
  SensorMode mMode;
};

}  // namespace chre

#endif  // CHRE_CORE_SENSOR_REQUEST_H_
