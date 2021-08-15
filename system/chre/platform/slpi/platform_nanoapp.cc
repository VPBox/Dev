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

#include "chre/platform/platform_nanoapp.h"

#include "chre/core/event_loop_manager.h"
#include "chre/platform/assert.h"
#include "chre/platform/log.h"
#include "chre/platform/memory.h"
#include "chre/platform/shared/nanoapp_dso_util.h"
#include "chre/platform/shared/nanoapp_support_lib_dso.h"
#include "chre/platform/slpi/memory.h"
#include "chre/platform/slpi/power_control_util.h"
#include "chre/util/system/debug_dump.h"
#include "chre_api/chre/version.h"

#include "dlfcn.h"

#include <inttypes.h>
#include <string.h>

namespace chre {
#if defined(CHRE_SLPI_SEE) && defined(CHRE_SLPI_UIMG_ENABLED)
namespace{
void rewriteToChreEventType(uint16_t *eventType) {
  CHRE_ASSERT(eventType);

  // HACK: as SEE does not support software batching in uimg via
  // QCM/uQSockets, we rewrite requests for accel and uncal accel/gyro/mag
  // from big image nanoapps to respective vendor types in
  // chreSensorFindDefault(), which is implemented as sensor data routed
  // through CM/QMI and supports batching. Rewrite sensor data arriving
  // on this event type to the vanilla sensor event type so that this appears
  // transparent to the nanoapp.
  // TODO(P2-5673a9): work with QC to determine a better long-term solution
  constexpr uint16_t kAccelBigImageEventType =
      (CHRE_EVENT_SENSOR_DATA_EVENT_BASE + CHRE_SENSOR_TYPE_VENDOR_START + 3);
  constexpr uint16_t kUncalAccelBigImageEventType =
      (CHRE_EVENT_SENSOR_DATA_EVENT_BASE + CHRE_SENSOR_TYPE_VENDOR_START + 6);
  constexpr uint16_t kUncalGyroBigImageEventType =
      (CHRE_EVENT_SENSOR_DATA_EVENT_BASE + CHRE_SENSOR_TYPE_VENDOR_START + 7);
  constexpr uint16_t kUncalMagBigImageEventType =
      (CHRE_EVENT_SENSOR_DATA_EVENT_BASE + CHRE_SENSOR_TYPE_VENDOR_START + 8);

  if (*eventType == kAccelBigImageEventType) {
    *eventType = CHRE_EVENT_SENSOR_ACCELEROMETER_DATA;
  } else if (*eventType == kUncalAccelBigImageEventType) {
    *eventType = CHRE_EVENT_SENSOR_UNCALIBRATED_ACCELEROMETER_DATA;
  } else if (*eventType == kUncalGyroBigImageEventType) {
    *eventType = CHRE_EVENT_SENSOR_UNCALIBRATED_GYROSCOPE_DATA;
  } else if (*eventType == kUncalMagBigImageEventType) {
    *eventType = CHRE_EVENT_SENSOR_UNCALIBRATED_GEOMAGNETIC_FIELD_DATA;
  }
}

/**
 * Helper function to get the sensor type of a big-image variant of a sensor.
 *
 * @param sensorType The sensor type to convert from.
 *
 * @return The sensor type of the corresponding big-image sensor, or the input
 *     sensor type if one does not exist.
 */
SensorType getBigImageSensorType(SensorType sensorType) {
  switch (sensorType) {
    case SensorType::Accelerometer:
      return SensorType::VendorType3;
    case SensorType::UncalibratedAccelerometer:
      return SensorType::VendorType6;
    case SensorType::UncalibratedGyroscope:
      return SensorType::VendorType7;
    case SensorType::UncalibratedGeomagneticField:
      return SensorType::VendorType8;
    default:
      return sensorType;
  }
}

/**
 * Helper function to get the handle of a big-image variant of a sensor.
 *
 * @param sensorHandle The sensor handle to convert from.
 *
 * @return The handle of the corresponding big-image sensor, or the input sensor
 *     handle if one does not exist.
 */
uint32_t getBigImageSensorHandle(uint32_t sensorHandle) {
  SensorType sensorType = getSensorTypeFromSensorHandle(sensorHandle);
  sensorType = getBigImageSensorType(sensorType);
  return getSensorHandleFromSensorType(sensorType);
}

/**
 * @return true if the given event type is a bias info event.
 */
bool isBiasEventType(uint16_t eventType) {
  return eventType == CHRE_EVENT_SENSOR_ACCELEROMETER_BIAS_INFO ||
      eventType == CHRE_EVENT_SENSOR_UNCALIBRATED_ACCELEROMETER_BIAS_INFO ||
      eventType == CHRE_EVENT_SENSOR_GYROSCOPE_BIAS_INFO ||
      eventType == CHRE_EVENT_SENSOR_UNCALIBRATED_GYROSCOPE_BIAS_INFO ||
      eventType == CHRE_EVENT_SENSOR_GEOMAGNETIC_FIELD_BIAS_INFO ||
      eventType == CHRE_EVENT_SENSOR_UNCALIBRATED_GEOMAGNETIC_FIELD_BIAS_INFO;
}

} //  anonymous namespace
#endif  // defined(CHRE_SLPI_SEE) && defined(CHRE_SLPI_UIMG_ENABLED)

PlatformNanoapp::~PlatformNanoapp() {
  closeNanoapp();
  if (mAppBinary != nullptr) {
    memoryFreeBigImage(mAppBinary);
  }
}

bool PlatformNanoapp::start() {
  // Invoke the start entry point after successfully opening the app
  if (!isUimgApp()) {
    slpiForceBigImage();
  }

  return openNanoapp() && mAppInfo->entryPoints.start();
}

void PlatformNanoapp::handleEvent(uint32_t senderInstanceId,
                                  uint16_t eventType,
                                  const void *eventData) {
  if (!isUimgApp()) {
    slpiForceBigImage();

#if defined(CHRE_SLPI_SEE) && defined(CHRE_SLPI_UIMG_ENABLED)
    rewriteToChreEventType(&eventType);
#endif  // defined(CHRE_SLPI_SEE) && defined(CHRE_SLPI_UIMG_ENABLED)
  }

#if defined(CHRE_SLPI_SEE) && defined(CHRE_SLPI_UIMG_ENABLED)
  // NOTE: Since SeeCalHelper does not internally differentiate calibration
  //       between big/micro image, convert the sensor handle to the appropriate
  //       one when delivering a bias info event to the nanoapp.
  chreSensorThreeAxisData bias;
  if (eventData != nullptr && !isUimgApp() && isBiasEventType(eventType)) {
    bias = *static_cast<const chreSensorThreeAxisData *>(eventData);
    bias.header.sensorHandle =
        getBigImageSensorHandle(bias.header.sensorHandle);
    eventData = &bias;
  }
#endif  // defined(CHRE_SLPI_SEE) && defined(CHRE_SLPI_UIMG_ENABLED)

  mAppInfo->entryPoints.handleEvent(senderInstanceId, eventType, eventData);
}

void PlatformNanoapp::end() {
  if (!isUimgApp()) {
    slpiForceBigImage();
  }

  mAppInfo->entryPoints.end();
  closeNanoapp();
}

bool PlatformNanoappBase::setAppInfo(
    uint64_t appId, uint32_t appVersion, const char *appFilename) {
  CHRE_ASSERT(!isLoaded());
  mExpectedAppId = appId;
  mExpectedAppVersion = appVersion;
  size_t appFilenameLen = strlen(appFilename) + 1;
  mAppFilename = static_cast<char *>(memoryAllocBigImage(appFilenameLen));

  bool success = false;
  if (mAppFilename == nullptr) {
    LOG_OOM();
  } else {
    memcpy(static_cast<void *>(mAppFilename), appFilename, appFilenameLen);
    success = true;
  }

  return success;
}

bool PlatformNanoappBase::reserveBuffer(
    uint64_t appId, uint32_t appVersion, size_t appBinaryLen) {
  CHRE_ASSERT(!isLoaded());
  bool success = false;
  constexpr size_t kMaxAppSize = 2 * 1024 * 1024;  // 2 MiB

  if (appBinaryLen > kMaxAppSize) {
    LOGE("Rejecting app size %zu above limit %zu", appBinaryLen, kMaxAppSize);
  } else {
    mAppBinary = memoryAllocBigImage(appBinaryLen);
    if (mAppBinary == nullptr) {
      LOGE("Couldn't allocate %zu byte buffer for nanoapp 0x%016" PRIx64,
           appBinaryLen, appId);
    } else {
      mExpectedAppId = appId;
      mExpectedAppVersion = appVersion;
      mAppBinaryLen = appBinaryLen;
      success = true;
    }
  }

  return success;
}

bool PlatformNanoappBase::copyNanoappFragment(
    const void *buffer, size_t bufferLen) {
  CHRE_ASSERT(!isLoaded());

  bool success = true;
  if (mBytesLoaded + bufferLen > mAppBinaryLen) {
    LOGE("Overflow: cannot load %zu bytes to %zu/%zu nanoapp binary buffer",
         bufferLen, mBytesLoaded, mAppBinaryLen);
    success = false;
  } else {
    uint8_t *binaryBuffer = static_cast<uint8_t *>(mAppBinary) + mBytesLoaded;
    memcpy(binaryBuffer, buffer, bufferLen);
    mBytesLoaded += bufferLen;
  }

  return success;
}

void PlatformNanoappBase::loadStatic(const struct chreNslNanoappInfo *appInfo) {
  CHRE_ASSERT(!isLoaded());
  mIsStatic = true;
  mAppInfo = appInfo;
}

bool PlatformNanoappBase::isLoaded() const {
  return (mIsStatic || (mAppBinary != nullptr && mBytesLoaded == mAppBinaryLen)
          || mDsoHandle != nullptr || mAppFilename != nullptr);
}

bool PlatformNanoappBase::isUimgApp() const {
  return mIsUimgApp;
}

void PlatformNanoappBase::closeNanoapp() {
  if (mDsoHandle != nullptr) {
    mAppInfo = nullptr;
    if (dlclose(mDsoHandle) != 0) {
      LOGE("dlclose failed: %s", dlerror());
    }
    mDsoHandle = nullptr;
  }
}

bool PlatformNanoappBase::openNanoapp() {
  bool success = false;

  if (mIsStatic) {
    success = true;
  } else if (mAppBinary != nullptr) {
    success = openNanoappFromBuffer();
  } else if (mAppFilename != nullptr) {
    success = openNanoappFromFile();
  } else {
    CHRE_ASSERT(false);
  }

  // Ensure any allocated memory hanging around is properly cleaned up.
  if (!success) {
    closeNanoapp();
  }

  // Save this flag locally since it may be referenced while the system is in
  // micro-image
  if (mAppInfo != nullptr) {
    mIsUimgApp = mAppInfo->isTcmNanoapp;
  }

  return success;
}

bool PlatformNanoappBase::openNanoappFromBuffer() {
  CHRE_ASSERT(mAppBinary != nullptr);
  CHRE_ASSERT_LOG(mDsoHandle == nullptr, "Re-opening nanoapp");

  // Populate a filename string (just a requirement of the dlopenbuf API)
  constexpr size_t kMaxFilenameLen = 17;
  char filename[kMaxFilenameLen];
  snprintf(filename, sizeof(filename), "%016" PRIx64, mExpectedAppId);

  mDsoHandle = dlopenbuf(
      filename, static_cast<const char *>(mAppBinary),
      static_cast<int>(mAppBinaryLen), RTLD_NOW);
  memoryFreeBigImage(mAppBinary);
  mAppBinary = nullptr;

  return verifyNanoappInfo();
}

bool PlatformNanoappBase::openNanoappFromFile() {
  CHRE_ASSERT(mAppFilename != nullptr);
  CHRE_ASSERT_LOG(mDsoHandle == nullptr, "Re-opening nanoapp");

  mDsoHandle = dlopen(mAppFilename, RTLD_NOW);
  memoryFreeBigImage(mAppFilename);
  mAppFilename = nullptr;

  return verifyNanoappInfo();
}

bool PlatformNanoappBase::verifyNanoappInfo() {
  bool success = false;

  if (mDsoHandle == nullptr) {
    LOGE("No nanoapp info to verify: %s", dlerror());
  } else {
    mAppInfo = static_cast<const struct chreNslNanoappInfo *>(
        dlsym(mDsoHandle, CHRE_NSL_DSO_NANOAPP_INFO_SYMBOL_NAME));
    if (mAppInfo == nullptr) {
      LOGE("Failed to find app info symbol: %s", dlerror());
    } else {
      success = validateAppInfo(mExpectedAppId, mExpectedAppVersion, mAppInfo);
      if (!success) {
        mAppInfo = nullptr;
      } else {
        LOGI("Successfully loaded nanoapp: %s (0x%016" PRIx64 ") version 0x%"
             PRIx32 " (%s) uimg %d system %d", mAppInfo->name, mAppInfo->appId,
             mAppInfo->appVersion, getAppVersionString(),
             mAppInfo->isTcmNanoapp, mAppInfo->isSystemNanoapp);
      }
    }
  }

  return success;
}

const char *PlatformNanoappBase::getAppVersionString() const {
  const char *versionString = "<undefined>";
  if (mAppInfo != nullptr && mAppInfo->structMinorVersion >= 2 
      && mAppInfo->appVersionString != NULL) {
    size_t appVersionStringLength = strlen(mAppInfo->appVersionString);

    size_t offset = 0;
    for (size_t i = 0; i < appVersionStringLength; i++) {
      size_t newOffset = i + 1;
      if (mAppInfo->appVersionString[i] == '@'
          && newOffset < appVersionStringLength) {
        offset = newOffset;
        break;
      }
    }

    versionString = &mAppInfo->appVersionString[offset];
  }

  return versionString;
}

uint64_t PlatformNanoapp::getAppId() const {
  return (mAppInfo != nullptr) ? mAppInfo->appId : mExpectedAppId;
}

uint32_t PlatformNanoapp::getAppVersion() const {
  return (mAppInfo != nullptr) ? mAppInfo->appVersion : mExpectedAppVersion;
}

uint32_t PlatformNanoapp::getTargetApiVersion() const {
  return (mAppInfo != nullptr) ? mAppInfo->targetApiVersion : 0;
}

bool PlatformNanoapp::isSystemNanoapp() const {
  // Right now, we assume that system nanoapps are always static nanoapps. Since
  // mAppInfo can only be null either prior to loading the app (in which case
  // this function is not expected to return a valid value anyway), or when a
  // dynamic nanoapp is not running, "false" is the correct return value in that
  // case.
  return (mAppInfo != nullptr) ? mAppInfo->isSystemNanoapp : false;
}

void PlatformNanoapp::logStateToBuffer(char *buffer, size_t *bufferPos,
                                       size_t bufferSize) const {
  if (mAppInfo != nullptr) {
    debugDumpPrint(buffer, bufferPos, bufferSize,
                   " %s: vendor=\"%s\" commit=\"%s\"",
                   mAppInfo->name, mAppInfo->vendor, getAppVersionString());
  }
}

}  // namespace chre
