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

#include <ash.h>
#include <chre.h>
#include <cinttypes>

#include "chre/util/nanoapp/log.h"

/**
 * @file
 * A nanoapp exclusively for testing, which saves locally defined ashCalParam
 * of accel, gyro and mag to storage and lodas them back to check correctness.
 * The loaded back asCalParam should be identical to the saved one, subject to
 * storage quantization error.
 * It's suggested that a copy of the original storage be made before running
 * this test nanoapp so the original storage can be retained.
 * Note that ASH functionality is not required for CHRE.
 */

#define LOG_TAG "[AshWorld]"

#ifdef CHRE_NANOAPP_INTERNAL
namespace chre {
namespace {
#endif  // CHRE_NANOAPP_INTERNAL

uint32_t gCyclicTimerHandle;
uint32_t gCyclicTimerCount;

struct ashCalParams accCalParams = {
  .offset = {0.0, 1.0, 2.0},
  .offsetTempCelsius = 3.0,
  .tempSensitivity = {4.0, 5.0, 6.0},
  .tempIntercept = {7.0, 8.0, 9.0},
  .scaleFactor = {10.0, 11.0, 12.0},
  .crossAxis = {13.0, 14.0, 15.0},
  .offsetSource = 16,
  .offsetTempCelsiusSource = 17,
  .tempSensitivitySource = 18,
  .tempInterceptSource = 19,
  .scaleFactorSource = 20,
  .crossAxisSource = 21,
};

struct ashCalParams gyrCalParams = {
  .offset = {100.0, 101.0, 102.0},
  .offsetTempCelsius = 103.0,
  .tempSensitivity = {104.0, 105.0, 106.0},
  .tempIntercept = {107.0, 108.0, 109.0},
  .scaleFactor = {110.0, 111.0, 112.0},
  .crossAxis = {113.0, 114.0, 115.0},
  .offsetSource = 116,
  .offsetTempCelsiusSource = 117,
  .tempSensitivitySource = 118,
  .tempInterceptSource = 119,
  .scaleFactorSource = 120,
  .crossAxisSource = 121,
};

struct ashCalParams magCalParams = {
  .offset = {200.0, 201.0, 202.0},
  .offsetTempCelsius = 203.0,
  .tempSensitivity = {204.0, 205.0, 206.0},
  .tempIntercept = {207.0, 208.0, 209.0},
  .scaleFactor = {210.0, 211.0, 212.0},
  .crossAxis = {213.0, 214.0, 215.0},
  .offsetSource = 216,
  .offsetTempCelsiusSource = 217,
  .tempSensitivitySource = 218,
  .tempInterceptSource = 219,
  .scaleFactorSource = 220,
  .crossAxisSource = 221,
};

struct ashCalInfo accCalInfo = {
  .bias = {0.1f, -0.1f, 0.2f},
  .compMatrix = {1.0f, 0.1f, -0.1f, 0.2f, -0.2f, 1.0f, 0.3f, -0.3f, 1.0f},
  .accuracy = 1,
};

struct ashCalInfo gyrCalInfo = {
  .bias = {0.2f, -0.2f, 0.1f},
  .compMatrix = {1.0f, 0.2f, -0.2f, 0.4f, -0.4f, 1.0f, 0.6f, -0.6f, 1.0f},
  .accuracy = 2,
};

struct ashCalInfo magCalInfo = {
  .bias = {10, -10, 20},
  .compMatrix = {1.0f, -0.1f, 0.1f, -0.2f, 0.2f, 1.0f, -0.3f, 0.3f, 1.0f},
  .accuracy = 3,
};

bool nanoappStart() {
  LOGI("App started on platform ID %" PRIx64, chreGetPlatformId());

  gCyclicTimerHandle = chreTimerSet(3000000000 /* duration: 3sec */,
      &gCyclicTimerHandle /* data */,
      false /* oneShot */);
  gCyclicTimerCount = 0;
  return true;
}

void handleTimerEvent(const void *eventData) {
  LOGI("Cyclic timer event received %" PRIu32, gCyclicTimerCount);
  bool success = false;
  uint64_t tic = 0, toc = 0;

  uint8_t sensor = CHRE_SENSOR_TYPE_ACCELEROMETER;
  struct ashCalParams *sensorCalParams = &accCalParams;
  struct ashCalInfo *sensorCalInfo = &accCalInfo;
  if ((gCyclicTimerCount / 3) == 1) {
    sensor = CHRE_SENSOR_TYPE_GYROSCOPE;
    sensorCalParams = &gyrCalParams;
    sensorCalInfo = &gyrCalInfo;
  } else if ((gCyclicTimerCount / 3) == 2) {
    sensor = CHRE_SENSOR_TYPE_GEOMAGNETIC_FIELD;
    sensorCalParams = &magCalParams;
    sensorCalInfo = &magCalInfo;
  }

  if (gCyclicTimerCount >= 9) {
    chreTimerCancel(gCyclicTimerHandle);
    LOGI("Timer cancelled");
  } else if (gCyclicTimerCount % 3 == 0) {
    tic = chreGetTime();
    success = ashSaveCalibrationParams(sensor, sensorCalParams);
    toc = chreGetTime();
    LOGI("*** save sensor %" PRIu8 ": %s, time %" PRIu64 " us",
         sensor, success ? "success" : "failure", (toc - tic) / 1000);
  } else if (gCyclicTimerCount % 3 == 1) {
    struct ashCalParams p;
    tic = chreGetTime();
    success = ashLoadCalibrationParams(sensor, ASH_CAL_STORAGE_ASH, &p);
    toc = chreGetTime();
    LOGI("*** load sensor %" PRIu8 ": %s, time %" PRIu64 " us",
         sensor, success ? "success" : "fail", (toc - tic) / 1000);

    LOGI("offset %f %f %f", p.offset[0], p.offset[1], p.offset[2]);
    LOGI("offsetTempCelsius %f", p.offsetTempCelsius);
    LOGI("tempSensitivity %f %f %f", p.tempSensitivity[0],
         p.tempSensitivity[1], p.tempSensitivity[2]);
    LOGI("tempIntercept %f %f %f", p.tempIntercept[0],
         p.tempIntercept[1], p.tempIntercept[2]);
    LOGI("scaleFactor %f %f %f", p.scaleFactor[0],
         p.scaleFactor[1], p.scaleFactor[2]);
    LOGI("crossAxis %f %f %f", p.crossAxis[0], p.crossAxis[1], p.crossAxis[2]);
    LOGI("%" PRIu8 " %" PRIu8 " %" PRIu8 " %" PRIu8 " %" PRIu8 " %" PRIu8,
         p.offsetSource, p.offsetTempCelsiusSource, p.tempSensitivitySource,
         p.tempInterceptSource, p.scaleFactorSource, p.crossAxisSource);
  } else {
    tic = chreGetTime();
    success = ashSetCalibration(sensor, sensorCalInfo);
    toc = chreGetTime();
    LOGI("*** set sensor %" PRIu8 ": %s, time %" PRIu64 " us",
         sensor, success ? "success" : "failure", (toc - tic) / 1000);
  }

  gCyclicTimerCount++;
}

void nanoappHandleEvent(uint32_t senderInstanceId,
                        uint16_t eventType,
                        const void *eventData) {
  switch (eventType) {
    case CHRE_EVENT_TIMER:
      handleTimerEvent(eventData);
      break;
    default:
      LOGW("Unknown event received");
      break;
  }
}

void nanoappEnd() {
  LOGI("Stopped");
}

#ifdef CHRE_NANOAPP_INTERNAL
}  // anonymous namespace
}  // namespace chre

#include "chre/util/nanoapp/app_id.h"
#include "chre/platform/static_nanoapp_init.h"

CHRE_STATIC_NANOAPP_INIT(AshWorld, chre::kAshWorldAppId, 0);
#endif  // CHRE_NANOAPP_INTERNAL
