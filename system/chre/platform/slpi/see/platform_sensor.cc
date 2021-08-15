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

#include "chre/platform/platform_sensor.h"

#include "sns_std_sensor.pb.h"
#include "stringl.h"

#include <cmath>

#include "chre_api/chre/sensor.h"
#include "chre/core/event_loop_manager.h"
#include "chre/core/sensor.h"
#include "chre/platform/assert.h"
#include "chre/platform/fatal_error.h"
#include "chre/platform/log.h"
#include "chre/platform/shared/platform_sensor_util.h"
#include "chre/platform/slpi/power_control_util.h"
#include "chre/platform/slpi/see/see_client.h"
#include "chre/platform/system_time.h"

#ifdef CHREX_SENSOR_SUPPORT
#include "chre/extensions/platform/slpi/see/vendor_data_types.h"
#endif  // CHREX_SENSOR_SUPPORT

#ifdef CHRE_VARIANT_SUPPLIES_SEE_SENSORS_LIST
#include "see_sensors.h"
#endif  // CHRE_VARIANT_SUPPLIES_SEE_SENSORS_LIST

#ifndef CHRE_SEE_NUM_TEMP_SENSORS
// There are usually more than one 'sensor_temperature' sensors in SEE.
// Define this in the variant-specific makefile to avoid missing sensors in
// sensor discovery.
#error "CHRE_SEE_NUM_TEMP_SENSORS is not defined"
#endif

namespace chre {
namespace {

#ifdef CHRE_SLPI_UIMG_ENABLED
#ifndef CHREX_SENSOR_SUPPORT
// The current implementation uses vendor sensor type 3 to remap into accel,
// with requests made through QMI instead of QSockets, as SEE does not support
// micro-image batching in QCM.
#error "CHRE extensions are required for micro-image SEE support"
#endif  // CHREX_SENSOR_SUPPORT

bool isBigImageSensorType(SensorType sensorType) {
  return (sensorType == SensorType::VendorType3       // accel
          || sensorType == SensorType::VendorType6    // uncal accel
          || sensorType == SensorType::VendorType7    // uncal gyro
          || sensorType == SensorType::VendorType8);  // uncal mag
}

/**
 * Obtains the big-image sensor type given the specified data type and whether
 * the sensor is runtime-calibrated or not.
 */
SensorType getBigImageSensorTypeFromDataType(const char *dataType,
                                             bool calibrated) {
  SensorType sensorType = SensorType::Unknown;
  if (strcmp(dataType, "accel") == 0) {
    if (calibrated) {
      sensorType = SensorType::VendorType3;
    } else {
      sensorType = SensorType::VendorType6;
    }
  } else if (strcmp(dataType, "gyro") == 0 && !calibrated) {
    sensorType = SensorType::VendorType7;
  } else if (strcmp(dataType, "mag") == 0 && !calibrated) {
    sensorType = SensorType::VendorType8;
  }
  return sensorType;
}

/**
 * Obtains the micro-image sensor type given the specified sensor type.
 *
 * @param sensorType The sensor type to convert from.
 * @return The associated micro-image sensor type, or the input sensor type
 *     if not associated with one
 */
SensorType getUimgSensorType(SensorType sensorType) {
  switch (sensorType) {
    case SensorType::VendorType3:
      return SensorType::Accelerometer;
    case SensorType::VendorType6:
      return SensorType::UncalibratedAccelerometer;
    case SensorType::VendorType7:
      return SensorType::UncalibratedGyroscope;
    case SensorType::VendorType8:
      return SensorType::UncalibratedGeomagneticField;
    default:
      return sensorType;
  }
}
#endif  // CHRE_SLPI_UIMG_ENABLED

//! A class that implements SeeHelperCallbackInterface.
class SeeHelperCallback : public SeeHelperCallbackInterface {
  void onSamplingStatusUpdate(
      UniquePtr<SeeHelperCallbackInterface::SamplingStatusData>&& status)
      override;

  void onSensorDataEvent(
      SensorType sensorType, UniquePtr<uint8_t>&& eventData) override;

  void onHostWakeSuspendEvent(bool awake) override;

  void onSensorBiasEvent(UniquePtr<struct chreSensorThreeAxisData>&& biasData)
      override;

  void onFlushCompleteEvent(SensorType sensorType) override;
};

//! A struct to facilitate sensor discovery
struct SuidAttr {
  sns_std_suid suid;
  SeeAttributes attr;
};

#ifndef CHRE_VARIANT_SUPPLIES_SEE_SENSORS_LIST

//! The list of SEE platform sensor data types that CHRE intends to support.
//! The standardized strings are defined in sns_xxx.proto.
const char *kSeeDataTypes[] = {
  "accel",
  "gyro",
  "mag",
  "pressure",
  "ambient_light",
  "proximity",
  "motion_detect",
  "stationary_detect",
};

#endif  // CHRE_VARIANT_SUPPLIES_SEE_SENSORS_LIST

void handleMissingSensor() {
  // Try rebooting if a sensor is missing, which might help recover from a
  // transient failure/race condition at startup. But to avoid endless crashes,
  // only do this within 15 seconds of the timeout on initializing SEE - we rely
  // on knowledge that getMonotonicTime() maps into QTimer here, and QTimer only
  // resets when the entire system is rebooted (it continues increasing after
  // SLPI SSR).
#ifndef CHRE_LOG_ONLY_NO_SENSOR
  if (SystemTime::getMonotonicTime() < (kDefaultSeeWaitTimeout + Seconds(15))) {
    FATAL_ERROR("Missing required sensor(s)");
  } else
#endif
  {
    LOGE("Missing required sensor(s)");
  }
}

/**
 * Obtains the sensor type given the specified data type and whether the sensor
 * is runtime-calibrated or not.
 */
SensorType getSensorTypeFromDataType(const char *dataType, bool calibrated) {
  SensorType sensorType;
  if (strcmp(dataType, "accel") == 0) {
    if (calibrated) {
      sensorType = SensorType::Accelerometer;
    } else {
      sensorType = SensorType::UncalibratedAccelerometer;
    }
  } else if (strcmp(dataType, "gyro") == 0) {
    if (calibrated) {
      sensorType = SensorType::Gyroscope;
    } else {
      sensorType = SensorType::UncalibratedGyroscope;
    }
  } else if (strcmp(dataType, "mag") == 0) {
    if (calibrated) {
      sensorType = SensorType::GeomagneticField;
    } else {
      sensorType = SensorType::UncalibratedGeomagneticField;
    }
  } else if (strcmp(dataType, "pressure") == 0) {
    sensorType = SensorType::Pressure;
  } else if (strcmp(dataType, "ambient_light") == 0) {
    sensorType = SensorType::Light;
  } else if (strcmp(dataType, "proximity") == 0) {
    sensorType = SensorType::Proximity;
  } else if (strcmp(dataType, "motion_detect") == 0) {
    sensorType = SensorType::InstantMotion;
  } else if (strcmp(dataType, "stationary_detect") == 0) {
    sensorType = SensorType::StationaryDetect;
  } else if (strcmp(dataType, "step_detect") == 0) {
    sensorType = SensorType::StepDetect;
#ifdef CHREX_SENSOR_SUPPORT
  } else if (strcmp(dataType, kVendorDataTypes[0]) == 0) {
    sensorType = SensorType::VendorType0;
#endif  // CHREX_SENSOR_SUPPORT
  } else {
    sensorType = SensorType::Unknown;
  }
  return sensorType;
}

/**
 * Posts a CHRE_EVENT_SENSOR_SAMPLING_CHANGE event to the specified Nanoapp.
 *
 * @param instaceId The instance ID of the nanoapp with an open request.
 * @param sensorHandle The handle of the sensor.
 * @param status A reference of the sampling status to be posted.
 */
void postSamplingStatusEvent(uint32_t instanceId, uint32_t sensorHandle,
                             const struct chreSensorSamplingStatus& status) {
  auto *event = memoryAlloc<struct chreSensorSamplingStatusEvent>();
  if (event == nullptr) {
    LOG_OOM();
  } else {
    event->sensorHandle = sensorHandle;
    event->status = status;

    EventLoopManagerSingleton::get()->getEventLoop().postEventOrDie(
        CHRE_EVENT_SENSOR_SAMPLING_CHANGE, event, freeEventDataCallback,
        instanceId);
  }
}

/**
 * Helper function to post a bias event given the bias data.
 *
 * @param sensorType The sensor type to post the event for.
 * @param bias The bias data.
 */
void postSensorBiasEvent(SensorType sensorType,
                         const chreSensorThreeAxisData& bias) {
  uint16_t eventType;
  if (getSensorBiasEventType(sensorType, &eventType)) {
    auto *event = memoryAlloc<struct chreSensorThreeAxisData>();
    if (event == nullptr) {
      LOG_OOM();
    } else {
      *event = bias;
      event->header.sensorHandle = getSensorHandleFromSensorType(sensorType);
      EventLoopManagerSingleton::get()->getEventLoop().postEventOrDie(
          eventType, event, freeEventDataCallback);
    }
  }
}

/**
 * Updates the sampling status.
 *
 * This should only be called when the new SamplingStatusData is different
 * from the most recently processed SamplingStatusData to avoid duplicate
 * updates being posted to nanoapps.
 */
void updateSamplingStatus(
    const SeeHelperCallbackInterface::SamplingStatusData& update) {
  Sensor *sensor = EventLoopManagerSingleton::get()->getSensorRequestManager()
      .getSensor(update.sensorType);
  struct chreSensorSamplingStatus newStatus;

  if (sensor != nullptr && !sensorTypeIsOneShot(update.sensorType)
      && sensor->getSamplingStatus(&newStatus)) {
    if (update.enabledValid) {
      newStatus.enabled = update.status.enabled;
    }
    if (update.intervalValid) {
      newStatus.interval = update.status.interval;
    }
    if (update.latencyValid) {
      newStatus.latency = update.status.latency;
    }

    sensor->setSamplingStatus(newStatus);

    // Only post to Nanoapps with an open request.
    uint32_t sensorHandle = getSensorHandleFromSensorType(update.sensorType);
    const DynamicVector<SensorRequest>& requests =
        EventLoopManagerSingleton::get()->getSensorRequestManager()
        .getRequests(update.sensorType);
    for (const auto& req : requests) {
      postSamplingStatusEvent(req.getInstanceId(), sensorHandle, newStatus);
    }
  }
}

/**
 * Compares the given status updates and returns true if they are the same.
 *
 * A simple memcmp cannot be done because if a given field is not valid, then
 * the field may be different across updates, but doesn't indicate the update
 * is different.
 */
bool isSameStatusUpdate(
    const SeeHelperCallbackInterface::SamplingStatusData& status1,
    const SeeHelperCallbackInterface::SamplingStatusData& status2) {
  bool sameStatus = status1.enabledValid == status2.enabledValid;
  if (sameStatus && status1.enabledValid) {
    sameStatus &= status1.status.enabled == status2.status.enabled;
  }

  // Only check interval / latency fields if both status updates say the sensor
  // is enabled since CHRE doesn't care what the fields are set to if the sensor
  // is disabled.
  if (sameStatus && status1.status.enabled) {
    sameStatus &= status1.intervalValid == status2.intervalValid;
    if (sameStatus && status1.intervalValid) {
      sameStatus &= status1.status.interval == status2.status.interval;
    }

    sameStatus &= status1.latencyValid == status2.latencyValid;
    if (sameStatus && status1.latencyValid) {
      sameStatus &= status1.status.latency == status2.status.latency;
    }
  }

  return sameStatus;
}

void SeeHelperCallback::onSamplingStatusUpdate(
    UniquePtr<SeeHelperCallbackInterface::SamplingStatusData>&& status) {
  Sensor *sensor = EventLoopManagerSingleton::get()->getSensorRequestManager()
      .getSensor(status->sensorType);

  // TODO: Once the latency field is actually filled in by SEE, modify this
  // logic to avoid reacting if the latency and interval of the sensor are
  // updated separately, but contain the same info as before.
  if (sensor != nullptr &&
      !isSameStatusUpdate(sensor->mLastReceivedSamplingStatus, *status.get())) {
    sensor->mLastReceivedSamplingStatus = *status.get();

    auto callback = [](uint16_t /* type */, void *data) {
      auto cbData = UniquePtr<SeeHelperCallbackInterface::SamplingStatusData>(
          static_cast<SeeHelperCallbackInterface::SamplingStatusData *>(data));
      updateSamplingStatus(*cbData);
    };

    // Schedule a deferred callback to handle sensor status change in the main
    // thread.
    EventLoopManagerSingleton::get()->deferCallback(
        SystemCallbackType::SensorStatusUpdate, status.release(), callback);
 }
}

void SeeHelperCallback::onSensorDataEvent(
    SensorType sensorType, UniquePtr<uint8_t>&& eventData) {
  // Schedule a deferred callback to update on-change sensor's last event in
  // the main thread.
  if (sensorTypeIsOnChange(sensorType)) {
    updateLastEvent(sensorType, eventData.get());
  }

  EventLoopManagerSingleton::get()->getSensorRequestManager().handleSensorEvent(
      sensorType, eventData.get());
  eventData.release();
}

void SeeHelperCallback::onHostWakeSuspendEvent(bool awake) {
  if (EventLoopManagerSingleton::isInitialized()) {
    EventLoopManagerSingleton::get()->getEventLoop()
        .getPowerControlManager().onHostWakeSuspendEvent(awake);
  }
}

void SeeHelperCallback::onSensorBiasEvent(
    UniquePtr<struct chreSensorThreeAxisData>&& biasData) {
  SensorType sensorType = getSensorTypeFromSensorHandle(
      biasData->header.sensorHandle);

  uint16_t eventType;
  if (!sensorTypeIsCalibrated(sensorType) ||
      !getSensorBiasEventType(sensorType, &eventType)) {
    LOGE("Received bias event for unsupported sensor type %" PRIu8, sensorType);
  } else {
    // Posts a newly allocated event for the uncalibrated type
    postSensorBiasEvent(toUncalibratedSensorType(sensorType), *biasData.get());

    EventLoopManagerSingleton::get()->getEventLoop().postEventOrDie(
        eventType, biasData.release(), freeEventDataCallback);
  }
}

void SeeHelperCallback::onFlushCompleteEvent(SensorType sensorType) {
  if (EventLoopManagerSingleton::isInitialized()) {
    EventLoopManagerSingleton::get()->getSensorRequestManager()
        .handleFlushCompleteEvent(CHRE_ERROR_NONE, sensorType);
  }
}

/**
 * Allocates memory and specifies the memory size for an on-change sensor to
 * store its last data event.
 *
 * @param sensorType The sensorType of this sensor.
 * @param eventSize A non-null pointer to indicate the memory size allocated.
 * @return Pointer to the memory allocated.
 */
ChreSensorData *allocateLastEvent(SensorType sensorType, size_t *eventSize) {
  CHRE_ASSERT(eventSize);

  *eventSize = 0;
  ChreSensorData *event = nullptr;
  if (sensorTypeIsOnChange(sensorType)) {
    SensorSampleType sampleType = getSensorSampleTypeFromSensorType(sensorType);
    switch (sampleType) {
      case SensorSampleType::ThreeAxis:
        *eventSize = sizeof(chreSensorThreeAxisData);
        break;
      case SensorSampleType::Float:
        *eventSize = sizeof(chreSensorFloatData);
        break;
      case SensorSampleType::Byte:
        *eventSize = sizeof(chreSensorByteData);
        break;
      case SensorSampleType::Occurrence:
        *eventSize = sizeof(chreSensorOccurrenceData);
        break;
      default:
        CHRE_ASSERT_LOG(false, "Unhandled sample type");
        break;
    }

    event = static_cast<ChreSensorData *>(memoryAlloc(*eventSize));
    if (event == nullptr) {
      *eventSize = 0;
      FATAL_ERROR("Failed to allocate last event memory for SensorType %" PRIu8,
                  static_cast<uint8_t>(sensorType));
    }
  }
  return event;
}

/**
 * Constructs and initializes a sensor, and adds it to the sensor list.
 *
 * @param seeHelper SeeHelper instance to register sensor with
 * @param suid The SUID of the sensor as provided by SEE.
 * @param sensorType The sensor type of the sensor.
 * @param calibrated Whether the sensor is runtime-calibrated or not.
 * @param attr A reference to SeeAttrbutes.
 * @param sensor The sensor list.
 */
void addSensor(SeeHelper& seeHelper, SensorType sensorType,
               const sns_std_suid& suid, const SeeAttributes& attr,
               DynamicVector<Sensor> *sensors) {
  // Concatenate vendor and name with a space in between.
  char sensorName[kSensorNameMaxLen];
  strlcpy(sensorName, attr.vendor, sizeof(sensorName));
  strlcat(sensorName, " ", sizeof(sensorName));
  strlcat(sensorName, attr.name, sizeof(sensorName));

  // Override one-shot sensor's minInterval to default
  uint64_t minInterval = sensorTypeIsOneShot(sensorType) ?
      CHRE_SENSOR_INTERVAL_DEFAULT : static_cast<uint64_t>(
          ceilf(Seconds(1).toRawNanoseconds() / attr.maxSampleRate));

  // Allocates memory for on-change sensor's last event.
  size_t lastEventSize;
  ChreSensorData *lastEvent = allocateLastEvent(sensorType, &lastEventSize);

  // Constructs and initializes PlatformSensorBase.
  Sensor sensor;
  sensor.initBase(sensorType, minInterval, sensorName, lastEvent,
                  lastEventSize, attr.passiveRequest);

  if (!sensors->push_back(std::move(sensor))) {
    FATAL_ERROR("Failed to allocate new sensor: out of memory");
  }

  // Resample big image sensors to reduce system load during sw flush.
#ifdef CHRE_SLPI_UIMG_ENABLED
  bool resample = isBigImageSensorType(sensorType);
#else
  bool resample = false;
#endif
  bool prevRegistered;
  bool registered = seeHelper.registerSensor(
      sensorType, suid, resample, &prevRegistered);
  if (!registered && prevRegistered) {
    LOGW("SUID has been previously registered");
  } else if (!registered) {
    FATAL_ERROR("Failed to register SUID/SensorType mapping.");
  }
}

/**
 * Compare SEE reported stream type attribute to the expected one. Some SEE
 * sensors may support more than one stream type.
 */
bool isStreamTypeCorrect(SensorType sensorType, uint8_t streamType) {
  bool success = true;
  if ((sensorTypeIsContinuous(sensorType)
       && streamType != SNS_STD_SENSOR_STREAM_TYPE_STREAMING)
      || (sensorTypeIsOnChange(sensorType)
          && streamType != SNS_STD_SENSOR_STREAM_TYPE_ON_CHANGE)
      || (sensorTypeIsOneShot(sensorType)
          && streamType != SNS_STD_SENSOR_STREAM_TYPE_SINGLE_OUTPUT)) {
    success = false;
    LOGW("Inconsistent sensor type %" PRIu8 " and stream type %" PRIu8,
         static_cast<uint8_t>(sensorType), streamType);
  }
  return success;
}

/**
 * Obtains the list of SUIDs and their attributes that support the specified
 * data type.
 */
bool getSuidAndAttrs(SeeHelper& seeHelper, const char *dataType,
                     DynamicVector<SuidAttr> *suidAttrs, uint8_t minNumSuids) {
  DynamicVector<sns_std_suid> suids;
  bool success = seeHelper.findSuidSync(dataType, &suids, minNumSuids);
  if (!success) {
    LOGE("Failed to find sensor '%s'", dataType);
  } else {
    LOGD("Num of SUIDs found for '%s': %zu", dataType, suids.size());

    for (const auto& suid : suids) {
      SeeAttributes attr;
      if (!seeHelper.getAttributesSync(suid, &attr)) {
        success = false;
        LOGE("Failed to get attributes of SUID 0x%" PRIx64 " %" PRIx64,
             suid.suid_high, suid.suid_low);
      } else {
        LOGI("%s %s, hw id %" PRId64 ", max ODR %f Hz, stream type %" PRIu8
             " passive %d",
             attr.vendor, attr.name, attr.hwId, attr.maxSampleRate,
             attr.streamType, attr.passiveRequest);
        SuidAttr sensor = {
          .suid = suid,
          .attr = attr,
        };
        if (!suidAttrs->push_back(sensor)) {
          success = false;
          LOG_OOM();
        }
      }
    }
  }
  return success;
}

//! Check whether two sensors with the specified attrtibutes belong to the same
//! sensor hardware module.
bool sensorHwMatch(const SeeAttributes& attr0, const SeeAttributes& attr1) {
  // When HW ID is absent, it's default to 0 and won't be a factor.
  return ((strncmp(attr0.vendor, attr1.vendor, kSeeAttrStrValLen) == 0)
          && (strncmp(attr0.name, attr1.name, kSeeAttrStrValLen) == 0)
          && (attr0.hwId == attr1.hwId));
}

/**
 * Looks up SUID(s) associated with a given sensor data type string and sensor
 * type enum, registers them with SeeHelper, and adds a Sensor instance to the
 * supplied vector for use in CHRE. When given an uncalibrated sensor type, will
 * also look for and add the calibrated sensor type.
 *
 * @param seeHelper SeeHelper instance to use for lookup/registration
 * @param temperatureSensors List of previously discovered temperature sensor
 *        info to use for adding temp sensors associated with this sensor type
 * @param dataType SEE data type string
 * @param sensorType CHRE sensor type enum associated with dataType
 * @param skipAdditionalTypes if true, don't attempt to add
 *        calibrated/temperature sensor types associated with this sensorType
 * @param sensors Vector to append found sensor(s) to
 */
void findAndAddSensorsForType(
    SeeHelper& seeHelper, const DynamicVector<SuidAttr>& temperatureSensors,
    const char *dataType, SensorType sensorType, bool skipAdditionalTypes,
    DynamicVector<Sensor> *sensors) {
  DynamicVector<SuidAttr> primarySensors;
  if (!getSuidAndAttrs(seeHelper, dataType, &primarySensors,
                       1 /* minNumSuids */)) {
    handleMissingSensor();
  }

  for (const auto& primarySensor : primarySensors) {
    sns_std_suid suid = primarySensor.suid;
    SeeAttributes attr = primarySensor.attr;

    // Some sensors support both continuous and on-change streams.
    // If there are more than one SUIDs that support the data type,
    // choose the first one that has the expected stream type.
    if (isStreamTypeCorrect(sensorType, attr.streamType)) {
      addSensor(seeHelper, sensorType, suid, attr, sensors);

      if (!skipAdditionalTypes) {
        // Check if this sensor has a runtime-calibrated version.
        SensorType calibratedType = getSensorTypeFromDataType(
            dataType, true /* calibrated */);
        if (calibratedType != sensorType) {
          addSensor(seeHelper, calibratedType, suid, attr, sensors);
        }

        // Check if this sensor has a secondary temperature sensor.
        SensorType temperatureType = getTempSensorType(sensorType);
        if (temperatureType != SensorType::Unknown) {
          bool tempFound = false;
          for (const auto& tempSensor : temperatureSensors) {
            sns_std_suid tempSuid = tempSensor.suid;
            SeeAttributes tempAttr = tempSensor.attr;

            if (sensorHwMatch(attr, tempAttr)) {
              LOGD("Found matching temperature sensor type");
              tempFound = true;
              addSensor(seeHelper, temperatureType, tempSuid, tempAttr,
                        sensors);
              break;
            }
          }
          if (!tempFound) {
            LOGW("Temperature sensor type %" PRIu8 " not found!",
                 static_cast<uint8_t>(temperatureType));
          }
        }
      }
      break;
    }
  }
}

#ifdef CHRE_SLPI_UIMG_ENABLED
/**
 * Registers alternate sensor(s) to be used separately by big image nanoapps.
 */
void getBigImageSensors(DynamicVector<Sensor> *sensors) {
  CHRE_ASSERT(sensors);

  // Currently, just adding calibrated accel and uncal accel/gyro/mag as they
  // are the ones we know that big image nanoapps will need at a different
  // batching rate compared to uimg.
  const char *kBigImageDataTypes[] = {
    "accel",
    "gyro",
    "mag",
  };

  SeeHelper& seeHelper = *getBigImageSeeHelper();
  DynamicVector<SuidAttr> nullTemperatureSensorList;

  for (size_t i = 0; i < ARRAY_SIZE(kBigImageDataTypes); i++) {
    const char *dataType = kBigImageDataTypes[i];
    // Loop through potential cal/uncal sensors.
    for (size_t j = 0; j < 2; j++) {
      SensorType sensorType = getBigImageSensorTypeFromDataType(
          dataType, (j == 0) /* calibrated */);
      if (sensorType != SensorType::Unknown) {
        findAndAddSensorsForType(
            seeHelper, nullTemperatureSensorList, dataType, sensorType,
            true /* skipAdditionalTypes */, sensors);
      }
    }
  }
}
#endif  // CHRE_SLPI_UIMG_ENABLED

/**
 * Helper function to retrieve the SeeHelper for a given sensor type.
 * @param sensorType the sensor type
 * @return the appropriate (bimg or uimg) SeeHelper
 */
SeeHelper *getSeeHelperForSensorType(SensorType sensorType) {
  SeeHelper *seeHelper = getSeeHelper();
#ifdef CHRE_SLPI_UIMG_ENABLED
  if (isBigImageSensorType(sensorType)) {
    seeHelper = getBigImageSeeHelper();
    slpiForceBigImage();
  }
#endif

  return seeHelper;
}


}  // anonymous namespace

PlatformSensor::~PlatformSensor() {
  if (mLastEvent != nullptr) {
    LOGD("Releasing lastEvent: sensor %s, size %zu",
         getSensorTypeName(getSensorType()), mLastEventSize);
    memoryFree(mLastEvent);
  }
}

void PlatformSensor::init() {
  SeeHelperSingleton::init();

  static SeeHelperCallback seeHelperCallback;
  if (!getSeeHelper()->init(&seeHelperCallback)) {
    FATAL_ERROR("Failed to initialize SEE helper");
  }

#ifdef CHRE_SLPI_UIMG_ENABLED
  BigImageSeeHelperSingleton::init(getSeeHelper()->getCalHelper());
  if (!getBigImageSeeHelper()->init(&seeHelperCallback, kDefaultSeeWaitTimeout,
                                    true /* skipDefaultSensorInit */)) {
    FATAL_ERROR("Failed to init bimg SEE helper");
  }
#endif  // CHRE_SLPI_UIMG_ENABLED
}

void PlatformSensor::deinit() {
#ifdef CHRE_SLPI_UIMG_ENABLED
  BigImageSeeHelperSingleton::deinit();
#endif

  SeeHelperSingleton::deinit();
}

bool PlatformSensor::getSensors(DynamicVector<Sensor> *sensors) {
  CHRE_ASSERT(sensors);

  SeeHelper& seeHelper = *getSeeHelper();
  DynamicVector<SuidAttr> tempSensors;
  if (!getSuidAndAttrs(seeHelper, "sensor_temperature", &tempSensors,
                       CHRE_SEE_NUM_TEMP_SENSORS)) {
    handleMissingSensor();
  }

#ifndef CHREX_SENSOR_SUPPORT
  const char *kVendorDataTypes[] = {};
#endif  // CHREX_SENSOR_SUPPORT
  constexpr size_t kNumSeeTypes = ARRAY_SIZE(kSeeDataTypes);
  constexpr size_t kNumVendorTypes = ARRAY_SIZE(kVendorDataTypes);
  for (size_t i = 0; i < kNumSeeTypes + kNumVendorTypes; i++) {
    const char *dataType = (i < kNumSeeTypes)
        ? kSeeDataTypes[i] : kVendorDataTypes[i - kNumSeeTypes];

    SensorType sensorType = getSensorTypeFromDataType(
        dataType, false /* calibrated */);
    if (sensorType == SensorType::Unknown) {
      LOGE("Unknown sensor type found for '%s'", dataType);
      continue;
    }

    findAndAddSensorsForType(seeHelper, tempSensors, dataType, sensorType,
                             false /* skipAdditionalTypes */, sensors);
  }

#ifdef CHRE_SLPI_UIMG_ENABLED
  getBigImageSensors(sensors);
#endif

  return true;
}

bool PlatformSensor::applyRequest(const SensorRequest& request) {
  SeeSensorRequest req = {
    .sensorType = getSensorType(),
    .enable = (request.getMode() != SensorMode::Off),
    .passive = sensorModeIsPassive(request.getMode()),
    .samplingRateHz = static_cast<float>(
        kOneSecondInNanoseconds / request.getInterval().toRawNanoseconds()),
    // Override batch period to 0 for non-continuous sensors to ensure one
    // sample per batch.
    .batchPeriodUs = !sensorTypeIsContinuous(mSensorType) ? 0
        : static_cast<uint32_t>(request.getLatency().toRawNanoseconds()
                                / kOneMicrosecondInNanoseconds),
  };

  if (req.enable && req.passive && !mPassiveSupported) {
    LOGD("Promoting sensor %" PRIu8 " passive request to active",
         static_cast<uint8_t>(getSensorType()));
  }

  SeeHelper *seeHelper = getSeeHelperForSensorType(getSensorType());
  bool wasInUImage = slpiInUImage();
  bool success = seeHelper->makeRequest(req);

  // If we dropped into micro-image during that blocking call to SEE, go back to
  // big image. This won't happen if the calling nanoapp is a big image one, but
  // other code paths currently assume that we will only transition from big
  // image to micro-image from CHRE's perspective while it's waiting for an
  // event to arrive in its empty queue.
  // TODO: transition back to big image only when needed, at the point of
  // invoking a nanoapp's free event/message callback
  if (!wasInUImage && slpiInUImage()) {
    LOGD("Restoring big image operating mode");
    slpiForceBigImage();
  }

  if (success) {
    if (request.getMode() == SensorMode::Off) {
      mLastEventValid = false;
    }

    // TODO: remove setSamplingStatus when .latency is available in status
    // update from SEE.
    struct chreSensorSamplingStatus status;
    if (getSamplingStatus(&status)) {

      // If passive request is not supported by this SEE sensor, it won't be
      // dynamically enabled/disabled and its status stays the same as set here.
      if (!mPassiveSupported) {
        status.enabled = req.enable;
      }
      status.latency = req.batchPeriodUs * kOneMicrosecondInNanoseconds;
      setSamplingStatus(status);
    }
  }
  return success;
}

bool PlatformSensor::flushAsync() {
  SensorType sensorType = getSensorType();
  return getSeeHelperForSensorType(sensorType)->flush(sensorType);
}

SensorType PlatformSensor::getSensorType() const {
  return mSensorType;
}

uint64_t PlatformSensor::getMinInterval() const {
  return mMinInterval;
}

const char *PlatformSensor::getSensorName() const {
  return mSensorName;
}

PlatformSensor::PlatformSensor(PlatformSensor&& other) {
  // Our move assignment operator doesn't assume that "this" is initialized, so
  // we can just use that here.
  *this = std::move(other);
}

PlatformSensor& PlatformSensor::operator=(PlatformSensor&& other) {
  // Note: if this implementation is ever changed to depend on "this" containing
  // initialized values, the move constructor implemenation must be updated.
  mSensorType = other.mSensorType;
  mMinInterval = other.mMinInterval;
  memcpy(mSensorName, other.mSensorName, kSensorNameMaxLen);

  mLastEvent = other.mLastEvent;
  other.mLastEvent = nullptr;

  mLastEventSize = other.mLastEventSize;
  other.mLastEventSize = 0;

  mLastEventValid = other.mLastEventValid;
  mSamplingStatus = other.mSamplingStatus;
  mPassiveSupported = other.mPassiveSupported;

  return *this;
}

ChreSensorData *PlatformSensor::getLastEvent() const {
  return mLastEventValid ? mLastEvent : nullptr;
}

bool PlatformSensor::getSamplingStatus(
    struct chreSensorSamplingStatus *status) const {
  CHRE_ASSERT(status);

  memcpy(status, &mSamplingStatus, sizeof(*status));
  return true;
}

bool PlatformSensor::getThreeAxisBias(
    struct chreSensorThreeAxisData *bias) const {
  SensorType sensorType = getSensorType();
  SeeCalHelper *calHelper =
      getSeeHelperForSensorType(sensorType)->getCalHelper();

  bool success = sensorTypeReportsBias(sensorType);
  if (success) {
    // We use the runtime-calibrated sensor type here, per documentation
    // of SeeCalHelper::getBias(), but overwrite the sensorHandle to that of
    // the curent sensor, because the calibration data itself is equivalent
    // for both calibrated/uncalibrated sensor types.
#ifdef CHRE_SLPI_UIMG_ENABLED
    // Use the uimg runtime-calibrated sensor type to get the calibration
    // bias, since SeeCalHelper is unaware of the bimg/uimg differentiation.
    SensorType calSensorType =
        toCalibratedSensorType(getUimgSensorType(sensorType));
#else
    SensorType calSensorType = toCalibratedSensorType(sensorType);
#endif
    if (calHelper->getBias(calSensorType, bias)) {
      bias->header.sensorHandle = getSensorHandleFromSensorType(sensorType);
    } else {
      // Set to zero value + unknown accuracy per CHRE API requirements.
      memset(bias, 0, sizeof(chreSensorThreeAxisData));
      bias->header.accuracy = CHRE_SENSOR_ACCURACY_UNKNOWN;
    }
  }

  return success;
}

void PlatformSensorBase::initBase(
    SensorType sensorType,uint64_t minInterval, const char *sensorName,
    ChreSensorData *lastEvent, size_t lastEventSize, bool passiveSupported) {
  mSensorType = sensorType;
  mMinInterval = minInterval;
  memcpy(mSensorName, sensorName, kSensorNameMaxLen);
  mLastEvent = lastEvent;
  mLastEventSize = lastEventSize;

  mSamplingStatus.enabled = false;
  mSamplingStatus.interval = CHRE_SENSOR_INTERVAL_DEFAULT;
  mSamplingStatus.latency = CHRE_SENSOR_LATENCY_DEFAULT;

  mPassiveSupported = passiveSupported;
}

void PlatformSensorBase::setLastEvent(const ChreSensorData *event) {
  memcpy(mLastEvent, event, mLastEventSize);
  mLastEventValid = true;
}

void PlatformSensorBase::setSamplingStatus(
    const struct chreSensorSamplingStatus& status) {
  mSamplingStatus = status;
}

}  // namespace chre
