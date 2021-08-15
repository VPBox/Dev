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

#include "chre/core/sensor_type.h"

#include "chre/platform/assert.h"

#ifdef CHREX_SENSOR_SUPPORT
#include "chre/extensions/platform/vendor_sensor_types.h"
#endif  // CHREX_SENSOR_SUPPORT

namespace chre {

const char *getSensorTypeName(SensorType sensorType) {
  switch (sensorType) {
    case SensorType::Unknown:
      return "Unknown";
    case SensorType::Accelerometer:
      return "Accelerometer";
    case SensorType::InstantMotion:
      return "Instant Motion";
    case SensorType::StationaryDetect:
      return "Stationary Detect";
    case SensorType::Gyroscope:
      return "Gyroscope";
    case SensorType::GeomagneticField:
      return "Geomagnetic Field";
    case SensorType::Pressure:
      return "Pressure";
    case SensorType::Light:
      return "Light";
    case SensorType::Proximity:
      return "Proximity";
    case SensorType::StepDetect:
      return "Step Detect";
    case SensorType::AccelerometerTemperature:
      return "Accelerometer Temp";
    case SensorType::GyroscopeTemperature:
      return "Gyroscope Temp";
    case SensorType::GeomagneticFieldTemperature:
      return "Geomagnetic Field Temp";
    case SensorType::UncalibratedAccelerometer:
      return "Uncal Accelerometer";
    case SensorType::UncalibratedGyroscope:
      return "Uncal Gyroscope";
    case SensorType::UncalibratedGeomagneticField:
      return "Uncal Geomagnetic Field";
    case SensorType::VendorType0:
      return "Vendor Type 0";
    case SensorType::VendorType1:
      return "Vendor Type 1";
    case SensorType::VendorType2:
      return "Vendor Type 2";
    case SensorType::VendorType3:
      return "Vendor Type 3";
    case SensorType::VendorType4:
      return "Vendor Type 4";
    case SensorType::VendorType5:
      return "Vendor Type 5";
    case SensorType::VendorType6:
      return "Vendor Type 6";
    case SensorType::VendorType7:
      return "Vendor Type 7";
    case SensorType::VendorType8:
      return "Vendor Type 8";
    default:
      CHRE_ASSERT(false);
      return "";
  }
}

uint16_t getSampleEventTypeForSensorType(SensorType sensorType) {
  // The enum values of SensorType may not map to the defined values in the
  // CHRE API.
  uint8_t sensorTypeValue = getUnsignedIntFromSensorType(sensorType);
  return CHRE_EVENT_SENSOR_DATA_EVENT_BASE + sensorTypeValue;
}

SensorType getSensorTypeForSampleEventType(uint16_t eventType) {
  return getSensorTypeFromUnsignedInt(
      static_cast<uint8_t>(eventType - CHRE_EVENT_SENSOR_DATA_EVENT_BASE));
}

SensorType getSensorTypeFromUnsignedInt(uint8_t sensorType) {
  switch (sensorType) {
    case CHRE_SENSOR_TYPE_ACCELEROMETER:
      return SensorType::Accelerometer;
    case CHRE_SENSOR_TYPE_INSTANT_MOTION_DETECT:
      return SensorType::InstantMotion;
    case CHRE_SENSOR_TYPE_STATIONARY_DETECT:
      return SensorType::StationaryDetect;
    case CHRE_SENSOR_TYPE_GYROSCOPE:
      return SensorType::Gyroscope;
    case CHRE_SENSOR_TYPE_GEOMAGNETIC_FIELD:
      return SensorType::GeomagneticField;
    case CHRE_SENSOR_TYPE_PRESSURE:
      return SensorType::Pressure;
    case CHRE_SENSOR_TYPE_LIGHT:
      return SensorType::Light;
    case CHRE_SENSOR_TYPE_PROXIMITY:
      return SensorType::Proximity;
    case CHRE_SENSOR_TYPE_STEP_DETECT:
      return SensorType::StepDetect;
    case CHRE_SENSOR_TYPE_ACCELEROMETER_TEMPERATURE:
      return SensorType::AccelerometerTemperature;
    case CHRE_SENSOR_TYPE_GYROSCOPE_TEMPERATURE:
      return SensorType::GyroscopeTemperature;
    case CHRE_SENSOR_TYPE_GEOMAGNETIC_FIELD_TEMPERATURE:
      return SensorType::GeomagneticFieldTemperature;
    case CHRE_SENSOR_TYPE_UNCALIBRATED_ACCELEROMETER:
      return SensorType::UncalibratedAccelerometer;
    case CHRE_SENSOR_TYPE_UNCALIBRATED_GYROSCOPE:
      return SensorType::UncalibratedGyroscope;
    case CHRE_SENSOR_TYPE_UNCALIBRATED_GEOMAGNETIC_FIELD:
      return SensorType::UncalibratedGeomagneticField;
    case (CHRE_SENSOR_TYPE_VENDOR_START + 0):
      return SensorType::VendorType0;
    case (CHRE_SENSOR_TYPE_VENDOR_START + 1):
      return SensorType::VendorType1;
    case (CHRE_SENSOR_TYPE_VENDOR_START + 2):
      return SensorType::VendorType2;
    case (CHRE_SENSOR_TYPE_VENDOR_START + 3):
      return SensorType::VendorType3;
    case (CHRE_SENSOR_TYPE_VENDOR_START + 4):
      return SensorType::VendorType4;
    case (CHRE_SENSOR_TYPE_VENDOR_START + 5):
      return SensorType::VendorType5;
    case (CHRE_SENSOR_TYPE_VENDOR_START + 6):
      return SensorType::VendorType6;
    case (CHRE_SENSOR_TYPE_VENDOR_START + 7):
      return SensorType::VendorType7;
    case (CHRE_SENSOR_TYPE_VENDOR_START + 8):
      return SensorType::VendorType8;
    default:
      return SensorType::Unknown;
  }
}

uint8_t getUnsignedIntFromSensorType(SensorType sensorType) {
  switch (sensorType) {
    case SensorType::Accelerometer:
      return CHRE_SENSOR_TYPE_ACCELEROMETER;
    case SensorType::InstantMotion:
      return CHRE_SENSOR_TYPE_INSTANT_MOTION_DETECT;
    case SensorType::StationaryDetect:
      return CHRE_SENSOR_TYPE_STATIONARY_DETECT;
    case SensorType::Gyroscope:
      return CHRE_SENSOR_TYPE_GYROSCOPE;
    case SensorType::GeomagneticField:
      return CHRE_SENSOR_TYPE_GEOMAGNETIC_FIELD;
    case SensorType::Pressure:
      return CHRE_SENSOR_TYPE_PRESSURE;
    case SensorType::Light:
      return CHRE_SENSOR_TYPE_LIGHT;
    case SensorType::Proximity:
      return CHRE_SENSOR_TYPE_PROXIMITY;
    case SensorType::StepDetect:
      return CHRE_SENSOR_TYPE_STEP_DETECT;
    case SensorType::AccelerometerTemperature:
      return CHRE_SENSOR_TYPE_ACCELEROMETER_TEMPERATURE;
    case SensorType::GyroscopeTemperature:
      return CHRE_SENSOR_TYPE_GYROSCOPE_TEMPERATURE;
    case SensorType::GeomagneticFieldTemperature:
      return CHRE_SENSOR_TYPE_GEOMAGNETIC_FIELD_TEMPERATURE;
    case SensorType::UncalibratedAccelerometer:
      return CHRE_SENSOR_TYPE_UNCALIBRATED_ACCELEROMETER;
    case SensorType::UncalibratedGyroscope:
      return CHRE_SENSOR_TYPE_UNCALIBRATED_GYROSCOPE;
    case SensorType::UncalibratedGeomagneticField:
      return CHRE_SENSOR_TYPE_UNCALIBRATED_GEOMAGNETIC_FIELD;
    case SensorType::VendorType0:
      return (CHRE_SENSOR_TYPE_VENDOR_START + 0);
    case SensorType::VendorType1:
      return (CHRE_SENSOR_TYPE_VENDOR_START + 1);
    case SensorType::VendorType2:
      return (CHRE_SENSOR_TYPE_VENDOR_START + 2);
    case SensorType::VendorType3:
      return (CHRE_SENSOR_TYPE_VENDOR_START + 3);
    case SensorType::VendorType4:
      return (CHRE_SENSOR_TYPE_VENDOR_START + 4);
    case SensorType::VendorType5:
      return (CHRE_SENSOR_TYPE_VENDOR_START + 5);
    case SensorType::VendorType6:
      return (CHRE_SENSOR_TYPE_VENDOR_START + 6);
    case SensorType::VendorType7:
      return (CHRE_SENSOR_TYPE_VENDOR_START + 7);
    case SensorType::VendorType8:
      return (CHRE_SENSOR_TYPE_VENDOR_START + 8);
    default:
      // Update implementation to prevent undefined or SensorType::Unknown from
      // being used.
      CHRE_ASSERT(false);
      return 0;
  }
}

SensorType getTempSensorType(SensorType sensorType) {
  switch (sensorType) {
    case SensorType::Accelerometer:
    case SensorType::UncalibratedAccelerometer:
      return SensorType::AccelerometerTemperature;
    case SensorType::Gyroscope:
    case SensorType::UncalibratedGyroscope:
      return SensorType::GyroscopeTemperature;
    case SensorType::GeomagneticField:
    case SensorType::UncalibratedGeomagneticField:
      return SensorType::GeomagneticFieldTemperature;
    default:
      return SensorType::Unknown;
  }
}

SensorSampleType getSensorSampleTypeFromSensorType(SensorType sensorType) {
  switch (sensorType) {
    case SensorType::Accelerometer:
    case SensorType::Gyroscope:
    case SensorType::GeomagneticField:
    case SensorType::UncalibratedAccelerometer:
    case SensorType::UncalibratedGyroscope:
    case SensorType::UncalibratedGeomagneticField:
      return SensorSampleType::ThreeAxis;
    case SensorType::Pressure:
    case SensorType::Light:
    case SensorType::AccelerometerTemperature:
    case SensorType::GyroscopeTemperature:
    case SensorType::GeomagneticFieldTemperature:
      return SensorSampleType::Float;
    case SensorType::InstantMotion:
    case SensorType::StationaryDetect:
    case SensorType::StepDetect:
      return SensorSampleType::Occurrence;
    case SensorType::Proximity:
      return SensorSampleType::Byte;
#ifdef CHREX_SENSOR_SUPPORT
    case SensorType::VendorType0:
      return SensorSampleType::Vendor0;
    case SensorType::VendorType1:
      return SensorSampleType::Vendor1;
    case SensorType::VendorType2:
      return SensorSampleType::Vendor2;
    case SensorType::VendorType3:
      return SensorSampleType::Vendor3;
    case SensorType::VendorType4:
      return SensorSampleType::Vendor4;
    case SensorType::VendorType5:
      return SensorSampleType::Vendor5;
    case SensorType::VendorType6:
      return SensorSampleType::Vendor6;
    case SensorType::VendorType7:
      return SensorSampleType::Vendor7;
    case SensorType::VendorType8:
      return SensorSampleType::Vendor8;
#endif  // CHREX_SENSOR_SUPPORT
    case SensorType::Unknown:
      return SensorSampleType::Unknown;
    default:
      // Update implementation to prevent undefined from being used.
      CHRE_ASSERT(false);
      return SensorSampleType::Unknown;
  }
}

SensorMode getSensorModeFromEnum(enum chreSensorConfigureMode enumSensorMode) {
  switch (enumSensorMode) {
    case CHRE_SENSOR_CONFIGURE_MODE_DONE:
      return SensorMode::Off;
    case CHRE_SENSOR_CONFIGURE_MODE_CONTINUOUS:
      return SensorMode::ActiveContinuous;
    case CHRE_SENSOR_CONFIGURE_MODE_ONE_SHOT:
      return SensorMode::ActiveOneShot;
    case CHRE_SENSOR_CONFIGURE_MODE_PASSIVE_CONTINUOUS:
      return SensorMode::PassiveContinuous;
    case CHRE_SENSOR_CONFIGURE_MODE_PASSIVE_ONE_SHOT:
      return SensorMode::PassiveOneShot;
    default:
      // Default to off since it is the least harmful and has no power impact.
      return SensorMode::Off;
  }
}

bool sensorTypeIsOneShot(SensorType sensorType) {
  return (sensorType == SensorType::InstantMotion
          || sensorType == SensorType::StationaryDetect
#ifdef CHREX_SENSOR_SUPPORT
          || extension::vendorSensorTypeIsOneShot(sensorType)
#endif
         );
}

bool sensorTypeIsOnChange(SensorType sensorType) {
  return (sensorType == SensorType::Light
          || sensorType == SensorType::Proximity
#ifdef CHREX_SENSOR_SUPPORT
          || extension::vendorSensorTypeIsOnChange(sensorType)
#endif
         );
}

bool sensorTypeIsContinuous(SensorType sensorType) {
  return (!sensorTypeIsOneShot(sensorType)
          && !sensorTypeIsOnChange(sensorType));
}

bool sensorTypeReportsBias(SensorType sensorType) {
  uint16_t eventType;
  return getSensorBiasEventType(sensorType, &eventType);
}

bool getSensorBiasEventType(SensorType sensorType, uint16_t *eventType) {
  CHRE_ASSERT(eventType != nullptr);
  bool success = false;
  if (eventType != nullptr) {
    success = true;
    switch (sensorType) {
      case SensorType::Accelerometer:
        *eventType = CHRE_EVENT_SENSOR_ACCELEROMETER_BIAS_INFO;
        break;
      case SensorType::UncalibratedAccelerometer:
        *eventType = CHRE_EVENT_SENSOR_UNCALIBRATED_ACCELEROMETER_BIAS_INFO;
        break;
      case SensorType::Gyroscope:
        *eventType = CHRE_EVENT_SENSOR_GYROSCOPE_BIAS_INFO;
        break;
      case SensorType::UncalibratedGyroscope:
        *eventType = CHRE_EVENT_SENSOR_UNCALIBRATED_GYROSCOPE_BIAS_INFO;
        break;
      case SensorType::GeomagneticField:
        *eventType = CHRE_EVENT_SENSOR_GEOMAGNETIC_FIELD_BIAS_INFO;
        break;
      case SensorType::UncalibratedGeomagneticField:
        *eventType = CHRE_EVENT_SENSOR_UNCALIBRATED_GEOMAGNETIC_FIELD_BIAS_INFO;
        break;
      default:
#ifdef CHREX_SENSOR_SUPPORT
        success =
            extension::vendorGetSensorBiasEventType(sensorType, eventType);
#else
        success = false;
#endif
    }
  }

  return success;
}

bool sensorTypeIsCalibrated(SensorType sensorType) {
  return (sensorType == SensorType::Accelerometer
          || sensorType == SensorType::Gyroscope
          || sensorType == SensorType::GeomagneticField);
}

SensorType toCalibratedSensorType(SensorType sensorType) {
  switch (sensorType) {
    case SensorType::UncalibratedAccelerometer:
      return SensorType::Accelerometer;
    case SensorType::UncalibratedGyroscope:
      return SensorType::Gyroscope;
    case SensorType::UncalibratedGeomagneticField:
      return SensorType::GeomagneticField;
    default:
      /* empty */
      break;
  }

  return sensorType;
}

SensorType toUncalibratedSensorType(SensorType sensorType) {
  switch (sensorType) {
    case SensorType::Accelerometer:
      return SensorType::UncalibratedAccelerometer;
    case SensorType::Gyroscope:
      return SensorType::UncalibratedGyroscope;
    case SensorType::GeomagneticField:
      return SensorType::UncalibratedGeomagneticField;
    default:
      /* empty */
      break;
  }

  return sensorType;
}

}  // namespace chre
