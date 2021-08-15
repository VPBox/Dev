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

#include <algorithm>

#include "chre/core/sensor_request.h"
#include "chre/platform/assert.h"
#include "chre/platform/fatal_error.h"

namespace chre {
namespace {

Nanoseconds getBatchInterval(const SensorRequest& request) {
  // With capping in SensorRequest constructor, interval + latency < UINT64_MAX.
  // When the return value is default, request latency (instead of batch
  // interval) will be used to compute the merged latency.
  if (request.getInterval() == Nanoseconds(CHRE_SENSOR_INTERVAL_DEFAULT)
      || request.getLatency() == Nanoseconds(CHRE_SENSOR_LATENCY_DEFAULT)) {
    return Nanoseconds(CHRE_SENSOR_BATCH_INTERVAL_DEFAULT);
  } else {
    return request.getInterval() + request.getLatency();
  }
}

}  // namespace

SensorRequest::SensorRequest()
    : SensorRequest(SensorMode::Off,
                    Nanoseconds(CHRE_SENSOR_INTERVAL_DEFAULT),
                    Nanoseconds(CHRE_SENSOR_LATENCY_DEFAULT)) {}

SensorRequest::SensorRequest(SensorMode mode, Nanoseconds interval,
                             Nanoseconds latency)
    : SensorRequest(kInvalidInstanceId, mode, interval, latency) {}

SensorRequest::SensorRequest(uint32_t instanceId, SensorMode mode,
                             Nanoseconds interval, Nanoseconds latency)
    : mInstanceId(instanceId), mInterval(interval), mLatency(latency),
      mMode(mode) {
  // cap non-default interval/latency to ensure no overflow in CHRE internal
  // operations.
  if (interval != Nanoseconds(CHRE_SENSOR_INTERVAL_DEFAULT)) {
    mInterval = std::min(interval, Nanoseconds(kMaxIntervalLatencyNs));
  }
  if (latency != Nanoseconds(CHRE_SENSOR_LATENCY_DEFAULT)) {
    mLatency = std::min(latency, Nanoseconds(kMaxIntervalLatencyNs));
  }
}

bool SensorRequest::isEquivalentTo(const SensorRequest& request) const {
  return (mMode == request.mMode
      && mInterval == request.mInterval
      && mLatency == request.mLatency);
}

bool SensorRequest::mergeWith(const SensorRequest& request) {
  bool attributesChanged = false;
  if (request.mMode != SensorMode::Off) {
    // Calculate minimum batch interval before mInterval is modified.
    Nanoseconds batchInterval = std::min(getBatchInterval(*this),
                                         getBatchInterval(request));

    if (request.mInterval < mInterval) {
      mInterval = request.mInterval;
      attributesChanged = true;
    }

    if (batchInterval == Nanoseconds(CHRE_SENSOR_BATCH_INTERVAL_DEFAULT)) {
      // If batchInterval is default, it can't be effectively calculated.
      // Use request.mLatency for more aggressive latency merging in this case.
      Nanoseconds latency = request.mLatency;
      if (latency < mLatency) {
        mLatency = latency;
        attributesChanged = true;
      }
    } else {
      Nanoseconds latency = (batchInterval - mInterval);

      // Note that while batchInterval can only shrink after merging, latency
      // can grow if the merged interval is lower.
      // Also, it's guaranteed that latency <= kMaxIntervalLatencyNs.
      if (latency != mLatency) {
        mLatency = latency;
        attributesChanged = true;
      }
    }

    // Compute the highest priority mode. Active continuous is the highest
    // priority and passive one-shot is the lowest.
    SensorMode maximalSensorMode = SensorMode::Off;
    if (mMode == SensorMode::ActiveContinuous
        || request.mMode == SensorMode::ActiveContinuous) {
      maximalSensorMode = SensorMode::ActiveContinuous;
    } else if (mMode == SensorMode::ActiveOneShot
        || request.mMode == SensorMode::ActiveOneShot) {
      maximalSensorMode = SensorMode::ActiveOneShot;
    } else if (mMode == SensorMode::PassiveContinuous
        || request.mMode == SensorMode::PassiveContinuous) {
      maximalSensorMode = SensorMode::PassiveContinuous;
    } else if (mMode == SensorMode::PassiveOneShot
        || request.mMode == SensorMode::PassiveOneShot) {
      maximalSensorMode = SensorMode::PassiveOneShot;
    } else {
      CHRE_ASSERT(false);
    }

    if (mMode != maximalSensorMode) {
      mMode = maximalSensorMode;
      attributesChanged = true;
    }
  }

  return attributesChanged;
}

}  // namespace chre
