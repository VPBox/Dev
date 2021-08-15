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

#include <cinttypes>

extern "C" {

#include "fixed_point.h"
#include "sns_smgr_api_v01.h"

}  // extern "C"

#include "ash_api/ash.h"
#include "chre/platform/assert.h"
#include "chre/platform/log.h"
#include "chre/platform/memory.h"
#include "chre/platform/slpi/smgr/smgr_client.h"
#include "chre_api/chre/sensor.h"

using chre::getSmrHelper;
using chre::getSensorServiceSmrClientHandle;
using chre::MakeUnique;
using chre::MakeUniqueZeroFill;
using chre::memoryAlloc;
using chre::memoryFree;
using chre::UniquePtr;

namespace {

//! The constant to convert magnetometer readings from uT in Android to Gauss
//! in SMGR.
constexpr float kGaussPerMicroTesla = 0.01f;

/**
 * @param sensorType One of the CHRE_SENSOR_TYPE_* constants.
 * @return true if runtime sensor calibration is supported on this platform.
 */
bool isCalibrationSupported(uint8_t sensorType) {
  switch (sensorType) {
    case CHRE_SENSOR_TYPE_ACCELEROMETER:
    case CHRE_SENSOR_TYPE_GYROSCOPE:
    case CHRE_SENSOR_TYPE_GEOMAGNETIC_FIELD:
      return true;
    default:
      return false;
  }
}

/**
 * @param sensorType One of the CHRE_SENSOR_TYPE_* constants.
 * @return The sensor ID of the sensor type as defined in the SMGR API.
 */
uint8_t getSensorId(uint8_t sensorType) {
  switch (sensorType) {
    case CHRE_SENSOR_TYPE_ACCELEROMETER:
      return SNS_SMGR_ID_ACCEL_V01;
    case CHRE_SENSOR_TYPE_GYROSCOPE:
      return SNS_SMGR_ID_GYRO_V01;
    case CHRE_SENSOR_TYPE_GEOMAGNETIC_FIELD:
      return SNS_SMGR_ID_MAG_V01;
    default:
      return 0;
  }
}

/**
 * Populates the calibration request mesasge.
 *
 * @param sensorType One of the CHRE_SENSOR_TYPE_* constants.
 * @param calInfo The sensor calibraion info supplied by the user.
 * @param calRequest The SMGR cal request message to be populated.
 */
void populateCalRequest(uint8_t sensorType, const ashCalInfo *calInfo,
                        sns_smgr_sensor_cal_req_msg_v01 *calRequest) {
  CHRE_ASSERT(calInfo);
  CHRE_ASSERT(calRequest);

  calRequest->usage = SNS_SMGR_CAL_DYNAMIC_V01;
  calRequest->SensorId = getSensorId(sensorType);
  calRequest->DataType = SNS_SMGR_DATA_TYPE_PRIMARY_V01;

  // Convert from micro Tesla to Gauss for magnetometer bias
  float scaling = 1.0f;
  if (sensorType == CHRE_SENSOR_TYPE_GEOMAGNETIC_FIELD) {
    scaling = kGaussPerMicroTesla;
  }

  // Convert from Android to SMGR's NED coordinate and invert the sign as SMGR
  // defines Sc = CM * (Su + Bias) in sns_rh_calibrate_cm_and_bias().
  calRequest->ZeroBias_len = 3;
  calRequest->ZeroBias[0] = FX_FLTTOFIX_Q16(-calInfo->bias[1] * scaling);
  calRequest->ZeroBias[1] = FX_FLTTOFIX_Q16(-calInfo->bias[0] * scaling);
  calRequest->ZeroBias[2] = FX_FLTTOFIX_Q16(calInfo->bias[2] * scaling);

  // ScaleFactor will be over-written by compensation matrix.
  calRequest->ScaleFactor_len = 3;
  calRequest->ScaleFactor[0] = FX_FLTTOFIX_Q16(1.0f);
  calRequest->ScaleFactor[1] = FX_FLTTOFIX_Q16(1.0f);
  calRequest->ScaleFactor[2] = FX_FLTTOFIX_Q16(1.0f);

  // Convert from Android to SMGR's NED coordinate.
  calRequest->CompensationMatrix_valid = true;
  calRequest->CompensationMatrix_len = 9;
  calRequest->CompensationMatrix[0] = FX_FLTTOFIX_Q16(calInfo->compMatrix[4]);
  calRequest->CompensationMatrix[1] = FX_FLTTOFIX_Q16(calInfo->compMatrix[3]);
  calRequest->CompensationMatrix[2] = FX_FLTTOFIX_Q16(-calInfo->compMatrix[5]);
  calRequest->CompensationMatrix[3] = FX_FLTTOFIX_Q16(calInfo->compMatrix[1]);
  calRequest->CompensationMatrix[4] = FX_FLTTOFIX_Q16(calInfo->compMatrix[0]);
  calRequest->CompensationMatrix[5] = FX_FLTTOFIX_Q16(-calInfo->compMatrix[2]);
  calRequest->CompensationMatrix[6] = FX_FLTTOFIX_Q16(-calInfo->compMatrix[7]);
  calRequest->CompensationMatrix[7] = FX_FLTTOFIX_Q16(-calInfo->compMatrix[6]);
  calRequest->CompensationMatrix[8] = FX_FLTTOFIX_Q16(calInfo->compMatrix[8]);

  calRequest->CalibrationAccuracy_valid = true;
  calRequest->CalibrationAccuracy = calInfo->accuracy;
}

}  // namespace

DLL_EXPORT bool ashSetCalibration(uint8_t sensorType,
                                  const struct ashCalInfo *calInfo) {
  bool success = false;
  if (!isCalibrationSupported(sensorType)) {
    LOGE("Attempting to set calibration of sensor %" PRIu8, sensorType);
  } else {
    // Allocate request and response for sensor calibraton.
    auto calRequest = MakeUniqueZeroFill<sns_smgr_sensor_cal_req_msg_v01>();
    auto calResponse = MakeUnique<sns_smgr_sensor_cal_resp_msg_v01>();
    if (calRequest.isNull() || calResponse.isNull()) {
      LOGE("Failed to allocated sensor cal memory");
    } else {
      populateCalRequest(sensorType, calInfo, calRequest.get());

      smr_err status = getSmrHelper()->sendReqSync(
          getSensorServiceSmrClientHandle(), SNS_SMGR_CAL_REQ_V01,
          &calRequest, &calResponse);

      if (status != SMR_NO_ERR) {
        LOGE("Error setting sensor calibration: status %d", status);
      } else if (calResponse->Resp.sns_result_t != SNS_RESULT_SUCCESS_V01) {
        LOGE("Setting sensor calibration failed with error: %" PRIu8,
             calResponse->Resp.sns_err_t);
      } else {
        success = true;
      }
    }
  }
  return success;
}
