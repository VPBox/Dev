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

#ifndef CHRE_PLATFORM_SLPI_SEE_SEE_CAL_HELPER_H_
#define CHRE_PLATFORM_SLPI_SEE_SEE_CAL_HELPER_H_

extern "C" {

#include "sns_client.h"

}  // extern "C"

#include "sns_suid.pb.h"

#include "chre/core/sensor_type.h"
#include "chre/platform/mutex.h"
#include "chre/util/non_copyable.h"
#include "chre/util/optional.h"

namespace chre {

class SeeHelper;

/**
 * Helps manage and apply sensor calibration data provided through SEE.
 */
class SeeCalHelper : public NonCopyable {
 public:
  /**
   * Applies cached calibration (if any) to raw 3-axis sensor readings.
   * Thread-safe.
   *
   * @param sensorType Type of sensor that generated the sample
   * @param input 3-axis raw sample {x,y,z}
   * @param output Location to store sample with calibration applied (can be
   *               same as input)
   */
  void applyCalibration(SensorType sensorType, const float input[3],
                        float output[3]) const;

  /**
   * Returns the cached calibration data. If the calibration data is available,
   * this method will store all fields in the provided chreSensorThreeAxisData
   * pointer, where the sample count is one. Thread-safe.
   *
   * @param sensorType Type of sensor to retrieve calibration data from, should
   *                   be the type of a runtime-calibrated sensor
   * @param biasData A non-null pointer to store the calibration data, not used
   *                 if the calibration data is not available
   *
   * @return true if calibration data is successfully stored, false otherwise
   */
  bool getBias(
      SensorType sensorType, struct chreSensorThreeAxisData *biasData) const;

  /**
   * Get the cached SUID of a calibration sensor that corresponds to the
   * specified sensorType.
   *
   * @param sensorType The sensor type of the calibration sensor.
   *
   * @return A constant reference to the calibration sensor's SUID if present.
   *         Otherwise, a reference to sns_suid_sensor_init_zero is returned.
   */
  const sns_std_suid& getCalSuidFromSensorType(SensorType sensorType) const;

  /**
   * Uses the supplied SeeHelper instance to register for updates to all
   * supported SEE calibration sensors. The SeeHelper instance should then pass
   * decoded calibration data to updateCalibration() and use applyCalibration()
   * as needed.
   *
   * @param seeHelper SeeHelper instance to use when looking up calibration
   *                  sensor SUIDs and registering for their output
   *
   * @return true if all SEE calibration sensors were successfully registered
   */
  bool registerForCalibrationUpdates(SeeHelper& seeHelper);

  /**
   * Updates the cached calibration data used in subsequent calls to
   * applyCalibration. Thread-safe.
   *
   * @param suid Sensor UID associated with the incoming calibration data
   * @param hasBias true if bias was decoded from the proto
   * @param bias 3-axis bias; only valid if hasBias is true
   * @param hasScale true if scale was decoded from the proto
   * @param scale 3-axis scale factor; only valid if hasScale is true
   * @param hasMatrix true if matrix was decoded from the proto
   * @param matrix 3x3 compensation matrix; only valid if hasMatrix is true
   * @param accuracy CHRE accuracy rating of the calibration quality (see
   *     CHRE_SENSOR_ACCURACY)
   * @param timestamp The timestamp of the calibration event
   *
   * @see CHRE_SENSOR_ACCURACY
   */
  void updateCalibration(const sns_std_suid& suid, bool hasBias, float bias[3],
                         bool hasScale, float scale[3], bool hasMatrix,
                         float matrix[9], uint8_t accuracy, uint64_t timestamp);

  /**
   * @param suid SUID of the calibration sensor
   *
   * @return the SensorType corresponding to this physical sensor
   */
  SensorType getSensorTypeFromSuid(const sns_std_suid& suid) const;

 private:
  //! A struct to store a sensor's calibration data
  struct SeeCalData {
    float bias[3];
    float scale[3];
    float matrix[9];
    bool hasBias;
    bool hasScale;
    bool hasMatrix;
    uint8_t accuracy;
    uint64_t timestamp;
  };

  //! A struct to store a cal sensor's UID and its cal data.
  struct SeeCalInfo {
    Optional<sns_std_suid> suid;
    SeeCalData cal;
  };

  //! The list of SEE cal sensors supported.
  enum class SeeCalSensor : size_t {
    AccelCal,
    GyroCal,
    MagCal,
    NumCalSensors,
  };

  //! A convenience constant.
  static constexpr size_t kNumSeeCalSensors = static_cast<size_t>(
      SeeCalSensor::NumCalSensors);

  //! Protects access to calibration data, which may be used in multiple threads
  mutable Mutex mMutex;

  //! Cal info of all the cal sensors.
  SeeCalInfo mCalInfo[kNumSeeCalSensors] = {};

  //! Map SensorType to associated index in mCalInfo
  static size_t getCalIndexFromSensorType(SensorType sensorType);

  //! Map index in mCalInfo to SEE sensor data type string
  static const char *getDataTypeForCalSensorIndex(size_t calSensorIndex);

  //! Map SUID to associated index in mCalInfo
  size_t getCalIndexFromSuid(const sns_std_suid& suid) const;
};

}  // namespace chre

#endif  // CHRE_PLATFORM_SLPI_SEE_SEE_CAL_HELPER_H_
