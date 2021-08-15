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

#include "chre/platform/slpi/see/see_cal_helper.h"

#include "chre/platform/assert.h"
#include "chre/platform/log.h"
#include "chre/platform/slpi/see/see_helper.h"
#include "chre/util/lock_guard.h"
#include "chre/util/macros.h"

namespace chre {

void SeeCalHelper::applyCalibration(SensorType sensorType, const float input[3],
                                    float output[3]) const {
  bool applied = false;
  size_t index = getCalIndexFromSensorType(sensorType);
  if (index < ARRAY_SIZE(mCalInfo)) {
    LockGuard<Mutex> lock(mMutex);

    // TODO: Support compensation matrix and scaling factor calibration
    if (mCalInfo[index].cal.hasBias) {
      for (size_t i = 0; i < 3; i++) {
        output[i] = input[i] - mCalInfo[index].cal.bias[i];
      }
      applied = true;
    }
  }

  if (!applied) {
    for (size_t i = 0; i < 3; i++) {
      output[i] = input[i];
    }
  }
}

bool SeeCalHelper::getBias(
    SensorType sensorType, struct chreSensorThreeAxisData *biasData) const {
  CHRE_ASSERT(biasData != nullptr);

  bool success = false;
  if (biasData != nullptr) {
    size_t index = getCalIndexFromSensorType(sensorType);
    if (index < ARRAY_SIZE(mCalInfo)) {
      LockGuard<Mutex> lock(mMutex);

      if (mCalInfo[index].cal.hasBias) {
        biasData->header.baseTimestamp = mCalInfo[index].cal.timestamp;
        biasData->header.sensorHandle =
            getSensorHandleFromSensorType(sensorType);
        biasData->header.readingCount = 1;
        biasData->header.accuracy = mCalInfo[index].cal.accuracy;
        biasData->header.reserved = 0;

        for (size_t i = 0; i < 3; i++) {
          biasData->readings[0].bias[i] = mCalInfo[index].cal.bias[i];
        }
        biasData->readings[0].timestampDelta = 0;
        success = true;
      }
    }
  }

  return success;
}

const sns_std_suid& SeeCalHelper::getCalSuidFromSensorType(
    SensorType sensorType) const {
  static sns_std_suid suid = sns_suid_sensor_init_zero;

  // Mutex not needed, SUID is not modified after init
  size_t calIndex = getCalIndexFromSensorType(sensorType);
  if (calIndex < ARRAY_SIZE(mCalInfo) && mCalInfo[calIndex].suid.has_value()) {
    suid = mCalInfo[calIndex].suid.value();
  }
  return suid;
}

bool SeeCalHelper::registerForCalibrationUpdates(SeeHelper& seeHelper) {
  bool success = true;

  // Find the cal sensor's SUID, assign it to mCalInfo, and make cal sensor data
  // request.
  DynamicVector<sns_std_suid> suids;
  for (size_t i = 0; i < ARRAY_SIZE(mCalInfo); i++) {
    const char *calType = getDataTypeForCalSensorIndex(i);
    if (!seeHelper.findSuidSync(calType, &suids)) {
      success = false;
      LOGE("Failed to find sensor '%s'", calType);
    } else {
      mCalInfo[i].suid = suids[0];
      if (!seeHelper.configureOnChangeSensor(suids[0], true /* enable */)) {
        success = false;
        LOGE("Failed to request '%s' data", calType);
      }
    }
  }

  return success;
}

void SeeCalHelper::updateCalibration(
    const sns_std_suid& suid, bool hasBias, float bias[3], bool hasScale,
    float scale[3], bool hasMatrix, float matrix[9], uint8_t accuracy,
    uint64_t timestamp) {
  size_t index = getCalIndexFromSuid(suid);
  if (index < ARRAY_SIZE(mCalInfo)) {
    LockGuard<Mutex> lock(mMutex);
    SeeCalData& calData = mCalInfo[index].cal;

    calData.hasBias = hasBias;
    if (hasBias) {
      memcpy(calData.bias, bias, sizeof(calData.bias));
    }

    calData.hasScale = hasScale;
    if (hasScale) {
      memcpy(calData.scale, scale, sizeof(calData.scale));
    }

    calData.hasMatrix = hasMatrix;
    if (hasMatrix) {
      memcpy(calData.matrix, matrix, sizeof(calData.matrix));
    }

    calData.accuracy = accuracy;
    calData.timestamp = timestamp;
  }
}

SensorType SeeCalHelper::getSensorTypeFromSuid(const sns_std_suid& suid) const {
  size_t calSensorIndex = getCalIndexFromSuid(suid);
  switch (static_cast<SeeCalSensor>(calSensorIndex)) {
    case SeeCalSensor::AccelCal:
      return SensorType::Accelerometer;
    case SeeCalSensor::GyroCal:
      return SensorType::Gyroscope;
    case SeeCalSensor::MagCal:
      return SensorType::GeomagneticField;
    default:
      CHRE_ASSERT(false);
  }
  return SensorType::Unknown;
}

size_t SeeCalHelper::getCalIndexFromSensorType(SensorType sensorType) {
  SeeCalSensor index;
  switch (sensorType) {
    case SensorType::Accelerometer:
      index = SeeCalSensor::AccelCal;
      break;
    case SensorType::Gyroscope:
      index = SeeCalSensor::GyroCal;
      break;
    case SensorType::GeomagneticField:
      index = SeeCalSensor::MagCal;
      break;
    default:
      index = SeeCalSensor::NumCalSensors;
  }
  return static_cast<size_t>(index);
}

const char *SeeCalHelper::getDataTypeForCalSensorIndex(size_t calSensorIndex) {
  switch (static_cast<SeeCalSensor>(calSensorIndex)) {
    case SeeCalSensor::AccelCal:
      return "accel_cal";
    case SeeCalSensor::GyroCal:
      return "gyro_cal";
    case SeeCalSensor::MagCal:
      return "mag_cal";
    default:
      CHRE_ASSERT(false);
  }
  return nullptr;
}

size_t SeeCalHelper::getCalIndexFromSuid(const sns_std_suid& suid) const {
  size_t i = 0;
  for (; i < ARRAY_SIZE(mCalInfo); i++) {
    if (mCalInfo[i].suid.has_value()
        && suidsMatch(suid, mCalInfo[i].suid.value())) {
      break;
    }
  }
  return i;
}

}  // namespace chre
