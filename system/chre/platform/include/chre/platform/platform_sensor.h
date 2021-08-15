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

#ifndef CHRE_PLATFORM_PLATFORM_SENSOR_H_
#define CHRE_PLATFORM_PLATFORM_SENSOR_H_

#include "chre/core/sensor_request.h"
#include "chre/target_platform/platform_sensor_base.h"
#include "chre/util/dynamic_vector.h"
#include "chre/util/non_copyable.h"

namespace chre {

class Sensor;

/**
 * Defines the common interface to sensor functionality that is implemented in a
 * platform-specific way, and must be supported on every platform.
 *
 * @see Sensor
 */
class PlatformSensor : public PlatformSensorBase,
                       public NonCopyable {
 public:
  /**
   * Initializes the sensors subsystem. This must be called as part of the
   * initialization of the runtime.
   */
  static void init();

  /**
   * Deinitializes the sensors subsystem, including releasing any outstanding
   * sensor requests. This must be called as part of the deinitialization of the
   * runtime.
   */
  static void deinit();

  /**
   * Constructs Sensor objects for every CHRE-supported sensor in the system,
   * and puts them in the supplied DynamicVector, which should be empty when
   * passed in. If this method returns false the vector may be partially filled.
   *
   * NOTE: Some platform implementations depend on this list only being
   * constructed during initialization so it must remain fixed afterwards.
   *
   * @param sensors A non-null pointer to a DynamicVector to populate with the
   *                list of sensors.
   * @return true if the query was successful.
   */
  static bool getSensors(DynamicVector<Sensor> *sensors);

  /**
   * Obtains the SensorType of this platform sensor. The implementation of this
   * method is supplied by the platform as the mechanism for determining the
   * type may vary across platforms.
   *
   * @return The type of this sensor.
   */
  SensorType getSensorType() const;

  /**
   * @return This sensor's minimum supported sampling interval, in nanoseconds.
   */
  uint64_t getMinInterval() const;

  /**
   * Returns a descriptive name (e.g. type and model) for this sensor.
   *
   * @return A pointer to a string with storage duration at least as long as the
   *         lifetime of this object.
   */
  const char *getSensorName() const;

  /**
   * @return Pointer to this sensor's last data event. It returns a nullptr if
   *         the the platform doesn't provide it.
   */
  ChreSensorData *getLastEvent() const;

  /**
   * Gets the current status of this sensor in the CHRE API format.
   *
   * @param status A non-null pointer to chreSensorSamplingStatus to populate
   * @return true if the sampling status has been successfully obtained.
   */
  bool getSamplingStatus(struct chreSensorSamplingStatus *status) const;

  /**
   * Synchronously retrieves the current bias for a sensor that supports
   * data in the chreSensorThreeAxisData format.
   *
   * @param bias A non-null pointer to store the current bias data.
   *
   * @return false if sensor does not report bias data in the
   *     chreSensorThreeAxisData format.
   */
  bool getThreeAxisBias(struct chreSensorThreeAxisData *bias) const;

  /**
   * Makes a sensor flush request for a nanoapp asynchronously. When a flush
   * request made by this method is completed (i.e. all pending samples are
   * posted to the CHRE event queue), PlatformSensor should invoke
   * SensorRequestManager::handleFlushCompleteEvent().
   *
   * @return true if the request was accepted.
   */
  bool flushAsync();

 protected:
  /**
   * Default constructor that puts this instance in an unspecified state.
   * Additional platform-specific initialization will likely be necessary to put
   * this object in a usable state. Do not construct PlatformSensor directly;
   * instead construct via Sensor.
   */
  PlatformSensor() = default;

  PlatformSensor(PlatformSensor&& other);
  PlatformSensor& operator=(PlatformSensor&& other);

  /**
   * Perform any necessary cleanup of resources acquired in PlatformSensorBase.
   */
  ~PlatformSensor();

  /**
   * Sends the sensor request to the platform sensor. The implementation
   * of this method is supplied by the platform. If the request is
   * invalid/unsupported by this sensor, for example because it requests an
   * interval that is too short, then this function must return false. If
   * setting this new request fails due to a transient failure (example:
   * inability to communicate with the sensor) false must also be returned.
   *
   * If the request's latency is lower than its interval, the platform sensor
   * must deliver the first sample to clients as soon as it becomes available.
   *
   * @param request The new request to set this sensor to.
   * @return true if the platform sensor was successfully configured with the
   *         supplied request.
   */
  bool applyRequest(const SensorRequest& request);
};

}  // namespace chre

#endif  // CHRE_PLATFORM_PLATFORM_SENSOR_H_
