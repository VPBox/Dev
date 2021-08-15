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

#ifndef CHRE_CORE_SENSOR_TYPE_H_
#define CHRE_CORE_SENSOR_TYPE_H_

#include <cstdint>

#include "chre_api/chre/sensor.h"

namespace chre {

//! The union of possible CHRE sensor data event type with one sample.
union ChreSensorData {
  struct chreSensorThreeAxisData threeAxisData;
  struct chreSensorOccurrenceData occurrenceData;
  struct chreSensorFloatData floatData;
  struct chreSensorByteData byteData;
};

/**
 * This SensorType is designed to wrap constants provided by the CHRE API
 * to improve type-safety. In addition, an unknown sensor type is provided
 * for dealing with sensors that are not defined as per the CHRE API
 * specification. The details of these sensors are left to the CHRE API
 * sensor definitions.
 */
enum class SensorType : uint8_t {
  Unknown,
  Accelerometer,
  InstantMotion,
  StationaryDetect,
  Gyroscope,
  GeomagneticField,
  Pressure,
  Light,
  Proximity,
  StepDetect,
  AccelerometerTemperature,
  GyroscopeTemperature,
  GeomagneticFieldTemperature,
  UncalibratedAccelerometer,
  UncalibratedGyroscope,
  UncalibratedGeomagneticField,

  VendorType0,
  VendorType1,
  VendorType2,
  VendorType3,
  VendorType4,
  VendorType5,
  VendorType6,
  VendorType7,
  VendorType8,

  // Note to future developers: don't forget to update the implementation of
  // 1) getSensorTypeName,
  // 2) getSensorTypeFromUnsignedInt,
  // 3) getUnsignedIntFromSensorType,
  // 4) getSensorSampleTypeFromSensorType
  // 5) sensorTypeIsOneShot
  // 6) sensorTypeIsOnChange
  // when adding or removing a new entry here :)
  // Have a nice day.

  //! The number of sensor types including unknown. This entry must be last.
  SENSOR_TYPE_COUNT,
};

/**
 * This SensorSampleType is designed to help classify SensorType's data type in
 * event handling.
 */
enum class SensorSampleType {
  Byte,
  Float,
  Occurrence,
  ThreeAxis,
  Vendor0,
  Vendor1,
  Vendor2,
  Vendor3,
  Vendor4,
  Vendor5,
  Vendor6,
  Vendor7,
  Vendor8,
  Unknown,
};

/**
 * Returns a string representation of the given sensor type.
 *
 * @param sensorType The sensor type to obtain a string for.
 * @return A string representation of the sensor type.
 */
const char *getSensorTypeName(SensorType sensorType);

/**
 * Returns a sensor sample event type for a given sensor type. The sensor type
 * must not be SensorType::Unknown. This is a fatal error.
 *
 * @param sensorType The type of the sensor.
 * @return The event type for a sensor sample of the given sensor type.
 */
uint16_t getSampleEventTypeForSensorType(SensorType sensorType);

/**
 * Returns a sensor type for a given sensor sample event type.
 *
 * @param eventType The event type for a sensor sample.
 * @return The type of the sensor.
 */
SensorType getSensorTypeForSampleEventType(uint16_t eventType);

/**
 * @return An index into an array for a given sensor type. This is useful to map
 * sensor type to array index quickly. The range returned corresponds to any
 * SensorType except for Unknown starting from zero to the maximum value sensor
 * with no gaps.
 */
constexpr size_t getSensorTypeArrayIndex(SensorType sensorType) {
  return static_cast<size_t>(sensorType) - 1;
}

/**
 * @return The number of valid sensor types in the SensorType enum.
 */
constexpr size_t getSensorTypeCount() {
  // The number of valid entries in the SensorType enum (not including Unknown).
  return static_cast<size_t>(SensorType::SENSOR_TYPE_COUNT) - 1;
}

/**
 * Translates an unsigned integer as provided by a CHRE-compliant nanoapp to a
 * SensorType. If the integer sensor type does not match one of the internal
 * sensor types, SensorType::Unknown is returned.
 *
 * @param sensorType The integer sensor type.
 * @return The strongly-typed sensor if a match is found or SensorType::Unknown.
 */
SensorType getSensorTypeFromUnsignedInt(uint8_t sensorType);

/**
 * Translates a SensorType to an unsigned integer as provided by CHRE API. If
 * the sensor type is SensorType::Unknown, 0 is returned.
 *
 * @param sensorType The strongly-typed sensor.
 * @return The integer sensor type if sensorType is not SensorType::Unknown.
 */
uint8_t getUnsignedIntFromSensorType(SensorType sensorType);

/**
 * Provides a stable handle for a CHRE sensor type. This handle is exposed to
 * CHRE nanoapps as a way to refer to sensors that they are subscribing to. This
 * API is not expected to be called with SensorType::Unknown as nanoapps are not
 * able to subscribe to the Unknown sensor type.
 *
 * @param sensorType The type of the sensor to obtain a handle for.
 * @return The handle for a given sensor.
 */
constexpr uint32_t getSensorHandleFromSensorType(SensorType sensorType) {
  return static_cast<uint32_t>(sensorType);
}

/**
 * Maps a sensor handle to a SensorType or returns SensorType::Unknown if the
 * provided handle is invalid.
 *
 * @param handle The sensor handle for a sensor.
 * @return The sensor type for a given handle.
 */
constexpr SensorType getSensorTypeFromSensorHandle(uint32_t handle) {
  return (handle > static_cast<uint32_t>(SensorType::Unknown)
          && handle < static_cast<uint32_t>(SensorType::SENSOR_TYPE_COUNT))
      ? static_cast<SensorType>(handle) : SensorType::Unknown;
}

/**
 * Obtains the temperature sensor type of the specified sensor type.
 *
 * @param sensorType The sensor type to obtain its temperature sensor type for.
 * @return The temperature sensor type or SensorType::Unknown if not supported
 *         by CHRE.
 */
SensorType getTempSensorType(SensorType sensorType);

/**
 * Maps a sensorType to its SensorSampleType.
 *
 * @param sensorType The type of the sensor to obtain its SensorSampleType for.
 * @return The SensorSampleType of the sensorType.
 */
SensorSampleType getSensorSampleTypeFromSensorType(SensorType sensorType);

/**
 * This SensorMode is designed to wrap constants provided by the CHRE API to
 * imrpove type-safety. The details of these modes are left to the CHRE API mode
 * definitions contained in the chreSensorConfigureMode enum.
 */
enum class SensorMode {
  Off,
  ActiveContinuous,
  ActiveOneShot,
  PassiveContinuous,
  PassiveOneShot,
};

/**
 * @return true if the sensor mode is considered to be active and would cause a
 *         sensor to be powered on in order to get sensor data.
 */
constexpr bool sensorModeIsActive(SensorMode sensorMode) {
  return (sensorMode == SensorMode::ActiveContinuous
      || sensorMode == SensorMode::ActiveOneShot);
}

/**
 * @return true if the sensor mode is considered to be passive and would not
 *         cause a sensor to be powered on in order to get sensor data.
 */
constexpr bool sensorModeIsPassive(SensorMode sensorMode) {
  return (sensorMode == SensorMode::PassiveContinuous
      || sensorMode == SensorMode::PassiveOneShot);
}

/**
 * @return true if the sensor mode is considered to be contunuous.
 */
constexpr bool sensorModeIsContinuous(SensorMode sensorMode) {
  return (sensorMode == SensorMode::ActiveContinuous
      || sensorMode == SensorMode::PassiveContinuous);
}

/**
 * @return true if the sensor mode is considered to be one-shot.
 */
constexpr bool sensorModeIsOneShot(SensorMode sensorMode) {
  return (sensorMode == SensorMode::ActiveOneShot
      || sensorMode == SensorMode::PassiveOneShot);
}

/**
 * Translates a CHRE API enum sensor mode to a SensorMode. This function also
 * performs input validation and will default to SensorMode::Off if the provided
 * value is not a valid enum value.
 *
 * @param enumSensorMode A potentially unsafe CHRE API enum sensor mode.
 * @return Returns a SensorMode given a CHRE API enum sensor mode.
 */
SensorMode getSensorModeFromEnum(enum chreSensorConfigureMode enumSensorMode);

/**
 * Indicates whether the sensor type is a one-shot sensor.
 *
 * @param sensorType The sensor type of the sensor.
 * @return true if the sensor is a one-shot sensor.
 */
bool sensorTypeIsOneShot(SensorType sensorType);

/**
 * Indicates whether the sensor type is an on-change sensor.
 *
 * @param sensorType The sensor type of the sensor.
 * @return true if the sensor is an on-change sensor.
 */
bool sensorTypeIsOnChange(SensorType sensorType);

/**
 * Indicates whether the sensor type is a continuous sensor.
 *
 * @param sensorType The sensor type of the sensor.
 * @return true if the sensor is a continuous sensor.
 */
bool sensorTypeIsContinuous(SensorType sensorType);

/**
 * Indicates whether the sensor type reports bias events.
 *
 * @param sensorType The sensor type of the sensor.
 * @return true if the sensor reports bias events.
 */
bool sensorTypeReportsBias(SensorType sensorType);

/**
 * @param sensorType The sensor type.
 * @param eventType A non-null pointer to where the event type is stored.
 *
 * @return true if the sensor type reports bias events.
 */
bool getSensorBiasEventType(SensorType sensorType, uint16_t *eventType);

/**
 * @param sensorType The sensor type.
 *
 * @return true if the sensor is a runtime-calibrated sensor.
 */
bool sensorTypeIsCalibrated(SensorType sensorType);

/**
 * @param sensorType The sensor type.
 *
 * @return The corresponding runtime-calibrated sensor type. If the sensor does
 *     not have one, then the input sensorType is returned.
 */
SensorType toCalibratedSensorType(SensorType sensorType);

/**
 * @param sensorType The sensor type.
 *
 * @return The corresponding uncalibrated sensor type. If the sensor does not
 *     have one, then the input sensorType is returned.
 */
SensorType toUncalibratedSensorType(SensorType sensorType);

/**
 * @param sensorType The sensor type.
 *
 * @return true if the sensor type is for a valid sensor.
 */
inline bool isValidSensorType(SensorType sensorType) {
  // TODO: Consider asserting that sensorType < SensorType::SENSOR_TYPE_COUNT
  //       once assertion can be enabled without costing extra memory overhead.
  return sensorType > SensorType::Unknown
      && sensorType < SensorType::SENSOR_TYPE_COUNT;
}

}  // namespace chre

#endif  // CHRE_CORE_SENSOR_TYPE_H_
