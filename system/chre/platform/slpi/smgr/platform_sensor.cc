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
#include <cinttypes>

#include "chre/platform/platform_sensor.h"

extern "C" {

#include "fixed_point.h"
#include "sns_smgr_api_v01.h"
#include "sns_smgr_internal_api_v02.h"
#include "sns_usmr.h"
#include "timetick.h"

}  // extern "C"

#include "chre_api/chre/sensor.h"
#include "chre/core/event_loop_manager.h"
#include "chre/core/sensor.h"
#include "chre/core/timer_pool.h"
#include "chre/platform/assert.h"
#include "chre/platform/fatal_error.h"
#include "chre/platform/log.h"
#include "chre/platform/shared/platform_sensor_util.h"
#include "chre/platform/slpi/uimg_util.h"
#include "chre/platform/slpi/smgr/platform_sensor_util.h"
#include "chre/platform/slpi/smgr/smgr_client.h"
#include "chre/platform/slpi/smgr/smr_helper.h"
#include "chre/platform/system_time.h"
#include "chre/util/macros.h"

#ifdef CHREX_SENSOR_SUPPORT
#include "chre/extensions/platform/slpi/smgr/platform_sensor_util.h"
#include "chrex_variant_smgr_sensor_id.h"
#endif  // CHREX_SENSOR_SUPPORT

// As SMGR doesn't support passive sensor request, it's now implemented on the
// client (CHRE) side using a combination of the SNS_SMGR_INTERNAL_API_V02 and a
// modified SNS_SMGR_API_V01.
//
// Here's a summary of its design:
// 1. A sensor status monitor is added in addSensorMonitor() to receive the
//    SNS_SMGR_SENSOR_STATUS_MONITOR_IND_V02 message the first time a sensor is
//    requested.
// 2. When a request is made in PlatformSensor::applyRequest(), it checkes
//    whether it's allowed at that point and makes a corresponding QMI request.
//    1) The request is allowed if
//       - it's an active or an off request, or
//       - it's a passive request and the merged mode (to be explained
//         shortly) is active or there exist other SMGR clients.
//    2) If the request is allowed, a QMI request to add the sensor request is
//       made. Otherwise, a QMI request to remove the sensor request is made to
//       handle the potential active-and-allowed to passive-and-disallowed
//       transition.
//    3) The merged mode of a sensor is the strongest mode of all sensor
//       requests of the same sensor ID, with active > passive > off.
// 3. When SNS_SMGR_SENSOR_STATUS_MONITOR_IND_V02 from SMGR is received, a new
//    timer is set for kStatusDelayIntervalNanos in the future for each
//    sensorId. Any future updates that occur before the timer fires are
//    ignored.
// 4. Once the timer fires, an asynchronous SNS_SMGR_CLIENT_REQUEST_INFO_REQ_V01
//    message is sent to query SMGR on the existence of other clients.
//    - If a transition from absence-to-presence of other clients is detected,
//      all pending passive requests are made.
//    - If a transition from presence-to-absence of other clients is deteted,
//      all passive requests are removed if the merged mode is passive.
//
// Note that currently the sensor status monitor indication only supports
// primary sensor status change. So for a secondary sensor that can be requested
// without an accompanying primary sensor (Light), this design doesn't work.
// In PlatformSensor::applyRequest(), a passive Light sensor request is
// overridden to be an active one.

namespace chre {
namespace {

//! The constant used to convert from SMGR to Android unit for magnetometer.
constexpr float kMicroTeslaPerGauss = 100.0f;

//! The maximum number of CHRE sensors that share the same SMGR sensor ID.
constexpr size_t kMaxNumSensorsPerSensorId = 3;

//! The value to override a default interval request.
constexpr uint64_t kDefaultInterval = Seconds(1).toRawNanoseconds();

//! The offset in nanoseconds each 32-bit tick rollover introduces in timestamp
constexpr uint64_t kTickRolloverOffset =
    ((1ULL << 32) * Seconds(1).toRawNanoseconds()) / TIMETICK_NOMINAL_FREQ_HZ;

//! The delay in nanoseconds between receiving a sensor status change
//! and updating the sensor status.
constexpr Nanoseconds kStatusDelayIntervalNanos = Milliseconds(20);

smr_client_hndl gPlatformSensorServiceSmrClientHandle;
smr_client_hndl gPlatformSensorInternalServiceSmrClientHandle;

//! A struct to store the number of SMGR clients of a sensor ID.
struct SensorMonitor {
  uint8_t sensorId;
  bool otherClientPresent;
};

//! A vector that tracks the SensorMonitor of each supported sensor ID.
DynamicVector<SensorMonitor> gSensorMonitors;

//! Forward declarations
bool makeAllPendingRequests(uint8_t sensorId);
bool removeAllPassiveRequests(uint8_t sensorId);

/**
 * Obtains the element index of gSensorMonitors that corresponds to the
 * specified sensor ID. If it's not present, gSensorMonitors.size() is returned.
 *
 * @return The index of the element that belongs to sensorId.
 */
size_t getSensorMonitorIndex(uint8_t sensorId) {
  size_t i;
  for (i = 0; i < gSensorMonitors.size(); i++) {
    if (gSensorMonitors[i].sensorId == sensorId) {
      break;
    }
  }
  return i;
}

/**
 * Converts a sensorId, dataType and calType as provided by SMGR to a
 * SensorType as used by platform-independent CHRE code. This is useful in
 * sensor discovery.
 *
 * @param sensorId The sensorID as provided by the SMGR request for sensor info.
 * @param dataType The dataType for the sesnor as provided by the SMGR request
 *                 for sensor info.
 * @param calType The calibration type (CAL_SEL) as defined in the SMGR API.
 * @return Returns the platform-independent sensor type or Unknown if no
 *         match is found.
 */
SensorType getSensorTypeFromSensorId(uint8_t sensorId, uint8_t dataType,
                                     uint8_t calType) {
  // Here be dragons. These constants below are defined in
  // sns_smgr_common_v01.h. Refer to the section labelled "Define sensor
  // identifier" for more details. This function relies on the ordering of
  // constants provided by their API. Do not change these values without care.
  // You have been warned!
  if (dataType == SNS_SMGR_DATA_TYPE_PRIMARY_V01) {
    if (sensorId >= SNS_SMGR_ID_ACCEL_V01
        && sensorId < SNS_SMGR_ID_GYRO_V01) {
      if (calType == SNS_SMGR_CAL_SEL_FULL_CAL_V01) {
        return SensorType::Accelerometer;
      } else if (calType == SNS_SMGR_CAL_SEL_FACTORY_CAL_V01) {
        return SensorType::UncalibratedAccelerometer;
      }
    } else if (sensorId >= SNS_SMGR_ID_GYRO_V01
        && sensorId < SNS_SMGR_ID_MAG_V01) {
      if (calType == SNS_SMGR_CAL_SEL_FULL_CAL_V01) {
        return SensorType::Gyroscope;
      } else if (calType == SNS_SMGR_CAL_SEL_FACTORY_CAL_V01) {
        return SensorType::UncalibratedGyroscope;
      }
    } else if (sensorId >= SNS_SMGR_ID_MAG_V01
        && sensorId < SNS_SMGR_ID_PRESSURE_V01) {
      if (calType == SNS_SMGR_CAL_SEL_FULL_CAL_V01) {
        return SensorType::GeomagneticField;
      } else if (calType == SNS_SMGR_CAL_SEL_FACTORY_CAL_V01) {
        return SensorType::UncalibratedGeomagneticField;
      }
    } else if (sensorId >= SNS_SMGR_ID_PRESSURE_V01
        && sensorId < SNS_SMGR_ID_PROX_LIGHT_V01) {
      return SensorType::Pressure;
    } else if (sensorId >= SNS_SMGR_ID_PROX_LIGHT_V01
        && sensorId < SNS_SMGR_ID_HUMIDITY_V01) {
      return SensorType::Proximity;
    } else if (sensorId == SNS_SMGR_ID_OEM_SENSOR_09_V01) {
      return SensorType::StationaryDetect;
    } else if (sensorId == SNS_SMGR_ID_OEM_SENSOR_10_V01) {
      return SensorType::InstantMotion;
#ifdef CHREX_SENSOR_SUPPORT
    } else if (sensorId == CHREX_VENDOR_TYPE0_SENSOR_ID) {
      return SensorType::VendorType0;
#endif  // CHREX_SENSOR_SUPPORT
    }
  } else if (dataType == SNS_SMGR_DATA_TYPE_SECONDARY_V01) {
    if (sensorId >= SNS_SMGR_ID_ACCEL_V01
        && sensorId < SNS_SMGR_ID_GYRO_V01) {
      return SensorType::AccelerometerTemperature;
    } else if (sensorId >= SNS_SMGR_ID_GYRO_V01
        && sensorId < SNS_SMGR_ID_MAG_V01) {
      return SensorType::GyroscopeTemperature;
    } else if ((sensorId >= SNS_SMGR_ID_PROX_LIGHT_V01
        && sensorId < SNS_SMGR_ID_HUMIDITY_V01)
        || (sensorId >= SNS_SMGR_ID_ULTRA_VIOLET_V01
            && sensorId < SNS_SMGR_ID_OBJECT_TEMP_V01)) {
      return SensorType::Light;
    }
  }

  return SensorType::Unknown;
}

/**
 * Converts a reportId as provided by SMGR to a SensorType.
 *
 * @param reportId The reportID as provided by the SMGR buffering index.
 * @return Returns the sensorType that corresponds to the reportId.
 */
SensorType getSensorTypeFromReportId(uint8_t reportId) {
  SensorType sensorType;
  if (reportId < static_cast<uint8_t>(SensorType::SENSOR_TYPE_COUNT)) {
    sensorType = static_cast<SensorType>(reportId);
  } else {
    sensorType = SensorType::Unknown;
  }
  return sensorType;
}

/**
 * Converts a PlatformSensor to a unique report ID through SensorType. This is
 * useful in making sensor request.
 *
 * @param sensorId The sensorID as provided by the SMGR request for sensor info.
 * @param dataType The dataType for the sesnor as provided by the SMGR request
 *                 for sensor info.
 * @param calType The calibration type (CAL_SEL) as defined in the SMGR API.
 * @return Returns a unique report ID that is based on SensorType.
 */
uint8_t getReportId(uint8_t sensorId, uint8_t dataType, uint8_t calType) {
  SensorType sensorType = getSensorTypeFromSensorId(
      sensorId, dataType, calType);

  CHRE_ASSERT_LOG(sensorType != SensorType::Unknown,
                  "sensorId %" PRIu8 ", dataType %" PRIu8 ", calType %" PRIu8,
                  sensorId, dataType, calType);
  return static_cast<uint8_t>(sensorType);
}

/**
 * Checks whether the corresponding sensor is a sencondary temperature sensor.
 *
 * @param reportId The reportID as provided by the SMGR buffering index.
 * @return true if the sensor is a secondary temperature sensor.
 */
bool isSecondaryTemperature(uint8_t reportId) {
  SensorType sensorType = getSensorTypeFromReportId(reportId);
  return (sensorType == SensorType::AccelerometerTemperature
          || sensorType == SensorType::GyroscopeTemperature);
}

/**
 * Verifies whether the buffering index's report ID matches the expected
 * indices length.
 *
 * @return true if it's a valid pair of indices length and report ID.
 */
bool isValidIndicesLength(
    const sns_smgr_buffering_ind_msg_v01& bufferingIndMsg) {
  return ((bufferingIndMsg.Indices_len == 1
           && !isSecondaryTemperature(bufferingIndMsg.ReportId))
          || (bufferingIndMsg.Indices_len == 2
              && isSecondaryTemperature(bufferingIndMsg.ReportId)));
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
      FATAL_ERROR("Failed to allocate last event memory for SensorType %d",
                  static_cast<int>(sensorType));
    }
  }
  return event;
}

/**
 * Constructs and initializes a sensor, and adds it to the sensor list.
 *
 * @param sensorInfo The sensorInfo as provided by the SMGR.
 * @param calType The calibration type (CAL_SEL) as defined in the SMGR API.
 * @param sensor The sensor list.
 */
void addSensor(const sns_smgr_sensor_datatype_info_s_v01& sensorInfo,
               uint8_t calType, DynamicVector<Sensor> *sensors) {
  Sensor sensor;
  sensor.sensorId = sensorInfo.SensorID;
  sensor.dataType = sensorInfo.DataType;
  sensor.calType = calType;
  size_t bytesToCopy = std::min(sizeof(sensor.sensorName) - 1,
                                static_cast<size_t>(sensorInfo.SensorName_len));
  memcpy(sensor.sensorName, sensorInfo.SensorName, bytesToCopy);
  sensor.sensorName[bytesToCopy] = '\0';

  // Override one-shot sensor's minInterval to default
  SensorType sensorType = getSensorTypeFromSensorId(
      sensorInfo.SensorID, sensorInfo.DataType, calType);
  sensor.minInterval = sensorTypeIsOneShot(sensorType) ?
      CHRE_SENSOR_INTERVAL_DEFAULT : static_cast<uint64_t>(
          Seconds(1).toRawNanoseconds() / sensorInfo.MaxSampleRate);

  // Allocates memory for on-change sensor's last event.
  sensor.lastEvent = allocateLastEvent(sensorType, &sensor.lastEventSize);

  sensor.isSensorOff = true;
  sensor.samplingStatus.enabled = false;
  sensor.samplingStatus.interval = CHRE_SENSOR_INTERVAL_DEFAULT;
  sensor.samplingStatus.latency = CHRE_SENSOR_LATENCY_DEFAULT;

  if (!sensors->push_back(std::move(sensor))) {
    FATAL_ERROR("Failed to allocate new sensor: out of memory");
  }
}

/**
 * Converts SMGR ticks to nanoseconds as a uint64_t.
 *
 * @param ticks The number of ticks.
 * @return The number of nanoseconds represented by the ticks value.
 */
uint64_t getNanosecondsFromSmgrTicks(uint32_t ticks) {
  return (ticks * Seconds(1).toRawNanoseconds()) / TIMETICK_NOMINAL_FREQ_HZ;
}

void populateSensorDataHeader(
    SensorType sensorType, chreSensorDataHeader *header,
    const sns_smgr_buffering_sample_index_s_v01& sensorIndex) {
  // Compensate for header timestamp's 32-bit rollovers
  uint64_t slpiTime = SystemTime::getMonotonicTime().toRawNanoseconds();
  uint64_t baseTime = getNanosecondsFromSmgrTicks(
      sensorIndex.FirstSampleTimestamp);
  while (slpiTime > baseTime + kTickRolloverOffset / 2) {
    baseTime += kTickRolloverOffset;
  }
  header->reserved = 0;
  header->baseTimestamp = baseTime;
  header->sensorHandle = getSensorHandleFromSensorType(sensorType);
  header->readingCount = sensorIndex.SampleCount;
  header->accuracy = CHRE_SENSOR_ACCURACY_UNKNOWN;
}

void populateThreeAxisEvent(
    const sns_smgr_buffering_ind_msg_v01& bufferingIndMsg,
    SensorType sensorType, chreSensorThreeAxisData *data,
    const sns_smgr_buffering_sample_index_s_v01& sensorIndex) {
  populateSensorDataHeader(sensorType, &data->header, sensorIndex);

  for (size_t i = 0; i < sensorIndex.SampleCount; i++) {
    const sns_smgr_buffering_sample_s_v01& sensorData =
        bufferingIndMsg.Samples[i + sensorIndex.FirstSampleIdx];

    // TimeStampOffset has max value of < 2 sec so it will not overflow here.
    data->readings[i].timestampDelta =
        getNanosecondsFromSmgrTicks(sensorData.TimeStampOffset);

    // Convert from SMGR's NED coordinate to Android coordinate.
    data->readings[i].x = FX_FIXTOFLT_Q16_SP(sensorData.Data[1]);
    data->readings[i].y = FX_FIXTOFLT_Q16_SP(sensorData.Data[0]);
    data->readings[i].z = -FX_FIXTOFLT_Q16_SP(sensorData.Data[2]);

    // Convert from Gauss to micro Tesla
    if (sensorType == SensorType::GeomagneticField
        || sensorType == SensorType::UncalibratedGeomagneticField) {
      data->readings[i].x *= kMicroTeslaPerGauss;
      data->readings[i].y *= kMicroTeslaPerGauss;
      data->readings[i].z *= kMicroTeslaPerGauss;
    }
  }
}

void populateFloatEvent(
    const sns_smgr_buffering_ind_msg_v01& bufferingIndMsg,
    SensorType sensorType, chreSensorFloatData *data,
    const sns_smgr_buffering_sample_index_s_v01& sensorIndex) {
  populateSensorDataHeader(sensorType, &data->header, sensorIndex);

  for (size_t i = 0; i < sensorIndex.SampleCount; i++) {
    const sns_smgr_buffering_sample_s_v01& sensorData =
        bufferingIndMsg.Samples[i + sensorIndex.FirstSampleIdx];

    // TimeStampOffset has max value of < 2 sec so it will not overflow.
    data->readings[i].timestampDelta =
        getNanosecondsFromSmgrTicks(sensorData.TimeStampOffset);
    data->readings[i].value = FX_FIXTOFLT_Q16_SP(sensorData.Data[0]);
  }
}

void populateByteEvent(
    const sns_smgr_buffering_ind_msg_v01& bufferingIndMsg,
    SensorType sensorType, chreSensorByteData *data,
    const sns_smgr_buffering_sample_index_s_v01& sensorIndex) {
  populateSensorDataHeader(sensorType, &data->header, sensorIndex);

  for (size_t i = 0; i < sensorIndex.SampleCount; i++) {
    const sns_smgr_buffering_sample_s_v01& sensorData =
        bufferingIndMsg.Samples[i + sensorIndex.FirstSampleIdx];

    // TimeStampOffset has max value of < 2 sec so it will not overflow.
    data->readings[i].timestampDelta =
        getNanosecondsFromSmgrTicks(sensorData.TimeStampOffset);
    // Zero out fields invalid and padding0.
    data->readings[i].value = 0;
    // SMGR reports 1 in Q16 for near, and 0 for far.
    data->readings[i].isNear = sensorData.Data[0] ? 1 : 0;
  }
}

void populateOccurrenceEvent(
    const sns_smgr_buffering_ind_msg_v01& bufferingIndMsg,
    SensorType sensorType, chreSensorOccurrenceData *data,
    const sns_smgr_buffering_sample_index_s_v01& sensorIndex) {
  populateSensorDataHeader(sensorType, &data->header, sensorIndex);

  for (size_t i = 0; i < sensorIndex.SampleCount; i++) {
    const sns_smgr_buffering_sample_s_v01& sensorData =
        bufferingIndMsg.Samples[i + sensorIndex.FirstSampleIdx];

    // TimeStampOffset has max value of < 2 sec so it will not overflow.
    data->readings[i].timestampDelta =
        getNanosecondsFromSmgrTicks(sensorData.TimeStampOffset);
  }
}

/**
 * Allocate event memory according to SensorType and populate event readings.
 */
void *allocateAndPopulateEvent(
    const sns_smgr_buffering_ind_msg_v01& bufferingIndMsg,
    SensorType sensorType,
    const sns_smgr_buffering_sample_index_s_v01& sensorIndex) {
  SensorSampleType sampleType = getSensorSampleTypeFromSensorType(sensorType);
  size_t memorySize = sizeof(chreSensorDataHeader);
  switch (sampleType) {
    case SensorSampleType::ThreeAxis: {
      memorySize += sensorIndex.SampleCount *
          sizeof(chreSensorThreeAxisData::chreSensorThreeAxisSampleData);
      auto *event =
          static_cast<chreSensorThreeAxisData *>(memoryAlloc(memorySize));
      if (event != nullptr) {
        populateThreeAxisEvent(bufferingIndMsg, sensorType, event, sensorIndex);
      }
      return event;
    }

    case SensorSampleType::Float: {
      memorySize += sensorIndex.SampleCount *
          sizeof(chreSensorFloatData::chreSensorFloatSampleData);
      auto *event =
          static_cast<chreSensorFloatData *>(memoryAlloc(memorySize));
      if (event != nullptr) {
        populateFloatEvent(bufferingIndMsg, sensorType, event, sensorIndex);
      }
      return event;
    }

    case SensorSampleType::Byte: {
      memorySize += sensorIndex.SampleCount *
          sizeof(chreSensorByteData::chreSensorByteSampleData);
      auto *event =
          static_cast<chreSensorByteData *>(memoryAlloc(memorySize));
      if (event != nullptr) {
        populateByteEvent(bufferingIndMsg, sensorType, event, sensorIndex);
      }
      return event;
    }

    case SensorSampleType::Occurrence: {
      memorySize += sensorIndex.SampleCount *
          sizeof(chreSensorOccurrenceData::chreSensorOccurrenceSampleData);
      auto *event =
          static_cast<chreSensorOccurrenceData *>(memoryAlloc(memorySize));
      if (event != nullptr) {
        populateOccurrenceEvent(
            bufferingIndMsg, sensorType, event, sensorIndex);
      }
      return event;
    }

#ifdef CHREX_SENSOR_SUPPORT
    case SensorSampleType::Vendor0:
      return allocateAndPopulateVendor0Event(
          bufferingIndMsg, sensorType, sensorIndex,
          populateSensorDataHeader, getNanosecondsFromSmgrTicks);
#endif  // CHREX_SENSOR_SUPPORT

    default:
      LOGW("Unhandled sensor data %" PRIu8, static_cast<uint8_t>(sensorType));
      return nullptr;
  }
}

/**
 * Handles sensor data provided by the SMGR framework.
 *
 * @param bufferingIndMsg Decoded buffering indication message
 */
void handleSensorDataIndication(
    const sns_smgr_buffering_ind_msg_v01& bufferingIndMsg) {
  // We only requested one sensor per request except for a secondary
  // temperature sensor.
  bool validReport = isValidIndicesLength(bufferingIndMsg);
  CHRE_ASSERT_LOG(validReport,
                  "Got buffering indication from %" PRIu32
                  " sensors with report ID %" PRIu8,
                  bufferingIndMsg.Indices_len,
                  bufferingIndMsg.ReportId);
  if (validReport) {
    // Identify the index for the desired sensor. It is always 0 except
    // possibly for a secondary temperature sensor.
    uint32_t index = 0;
    if (isSecondaryTemperature(bufferingIndMsg.ReportId)) {
      index = (bufferingIndMsg.Indices[0].DataType
               == SNS_SMGR_DATA_TYPE_SECONDARY_V01) ? 0 : 1;
    }
    const sns_smgr_buffering_sample_index_s_v01& sensorIndex =
        bufferingIndMsg.Indices[index];

    // Use ReportID to identify sensors as
    // bufferingIndMsg.Samples[i].Flags are not populated.
    SensorType sensorType = getSensorTypeFromReportId(
        bufferingIndMsg.ReportId);
    if (sensorType == SensorType::Unknown) {
      LOGW("Received sensor sample for unknown sensor %" PRIu8 " %" PRIu8,
           sensorIndex.SensorId, sensorIndex.DataType);
    } else if (sensorIndex.SampleCount == 0) {
      LOGW("Received sensorType %d event with 0 sample",
           static_cast<int>(sensorType));
    } else {
      void *eventData = allocateAndPopulateEvent(
          bufferingIndMsg, sensorType, sensorIndex);
      auto *header = static_cast< chreSensorDataHeader *>(eventData);
      if (eventData == nullptr) {
        LOGW("Dropping event due to allocation failure");
      } else if (header->readingCount == 0) {
        LOGW("Dropping zero readingCount event");
        memoryFree(eventData);
      } else {
        // Schedule a deferred callback to update on-change sensor's last
        // event in the main thread.
        if (sensorTypeIsOnChange(sensorType)) {
          updateLastEvent(sensorType, eventData);
        }

        EventLoopManagerSingleton::get()
            ->getSensorRequestManager()
            .handleSensorEvent(sensorType, eventData);
      }
    }
  }  // if (validReport)
}

/**
 * This callback is invoked by the SMR framework when an asynchronous message is
 * delivered. Unhandled messages are logged.
 *
 * @param handle Handle for the SMR client this indication was received on.
 * @param messageId The message ID number.
 * @param buffer Buffer containing decoded (C struct) message data.
 * @param bufferLength Size of the decoded buffer in bytes.
 * @param callbackData Data that is provided as a context to this callback. This
 *                     is not used in this context.
 *
 * @see smr_client_ind_cb
 */
void platformSensorServiceIndicationCallback(
    smr_client_hndl handle, unsigned int messageId, void *decodedInd,
    unsigned int decodedIndLen, void *callbackData) {
  switch (messageId) {
    case SNS_SMGR_BUFFERING_IND_V01: {
      CHRE_ASSERT(decodedIndLen >= sizeof(sns_smgr_buffering_ind_msg_v01));
      auto *bufferingInd =
          static_cast<sns_smgr_buffering_ind_msg_v01 *>(decodedInd);
      handleSensorDataIndication(*bufferingInd);
      break;
    }
    default:
      LOGW("Received unhandled sensor service message: 0x%x", messageId);
      break;
  };
}

/**
 * Populates the supplied SensorTypes array with SensorTypes of the specified
 * sensor ID.
 *
 * @param sensorId The sensor ID as provided by the SMGR.
 * @param sensorTypes A non-null pointer to a SensorType array of size at least
 *        kMaxNumSensorsPerSensorId.
 */
size_t populateSensorTypeArrayFromSensorId(uint8_t sensorId,
                                           SensorType *sensorTypes) {
  static_assert(kMaxNumSensorsPerSensorId >= 3,
                "This function assumes kMaxNumSensorsPerSensorId >= 3");
  CHRE_ASSERT(sensorTypes);

  size_t numSensorTypes = 0;
  if (sensorTypes != nullptr) {
    if (sensorId >= SNS_SMGR_ID_ACCEL_V01
          && sensorId < SNS_SMGR_ID_GYRO_V01) {
      sensorTypes[0] = SensorType::Accelerometer;
      sensorTypes[1] = SensorType::UncalibratedAccelerometer;
      sensorTypes[2] = SensorType::AccelerometerTemperature;
      numSensorTypes = 3;
    } else if (sensorId >= SNS_SMGR_ID_GYRO_V01
          && sensorId < SNS_SMGR_ID_MAG_V01) {
      sensorTypes[0] = SensorType::Gyroscope;
      sensorTypes[1] = SensorType::UncalibratedGyroscope;
      sensorTypes[2] = SensorType::GyroscopeTemperature;
      numSensorTypes = 3;
    } else if (sensorId >= SNS_SMGR_ID_MAG_V01
          && sensorId < SNS_SMGR_ID_PRESSURE_V01) {
      sensorTypes[0] = SensorType::GeomagneticField;
      sensorTypes[1] = SensorType::UncalibratedGeomagneticField;
      numSensorTypes = 2;
    } else {
      SensorType sensorType = getSensorTypeFromSensorId(sensorId,
          SNS_SMGR_DATA_TYPE_PRIMARY_V01, SNS_SMGR_CAL_SEL_FULL_CAL_V01);
      if (sensorType != SensorType::Unknown) {
        sensorTypes[0] = sensorType;
        numSensorTypes = 1;
      }
    }
  }
  return numSensorTypes;
}

/**
 * Obtains the merged SensorMode of the specified sensor ID, with sensorType's
 * sensor request replaced by the supplied request.
 *
 * @param sensorId The sensor ID as provided by the SMGR.
 * @param sensorType The SensorType whose sensor request is to be replaced by
 *        the supplied request.
 * @param request The sensor request to replace the existing one.
 * @return The merged SensorMode.
 */
SensorMode getMergedMode(uint8_t sensorId, SensorType sensorType,
                         const SensorRequest& request) {
  // Identify sensor requests to merge
  SensorType sensorTypes[kMaxNumSensorsPerSensorId];
  size_t numSensorTypes = populateSensorTypeArrayFromSensorId(
      sensorId, sensorTypes);

  // merge requests
  SensorRequest mergedRequest;
  for (size_t i = 0; i < numSensorTypes; i++) {
    const Sensor *sensor = EventLoopManagerSingleton::get()
      ->getSensorRequestManager().getSensor(sensorTypes[i]);
    if (sensor != nullptr) {
      mergedRequest.mergeWith(
          (sensorTypes[i] == sensorType) ? request : sensor->getRequest());
    }
  }
  return mergedRequest.getMode();
}

/**
 * Makes or removes passive sensor requests when the presence of other SMGR
 * clients changes.
 *
 * @param sensorID The sensor ID being monitored.
 * @param otherClientPresent The presence of other SMGR clients.
 */
void onOtherClientPresenceChange(uint8_t sensorId, bool otherClientPresent) {
  bool makeAllRequests = otherClientPresent;

  SensorRequest dummyRequest;
  SensorMode mode = getMergedMode(sensorId, SensorType::Unknown, dummyRequest);
  bool removeAllRequests = (sensorModeIsPassive(mode) && !otherClientPresent);

  bool requestMade = false;
  if (makeAllRequests) {
    requestMade = makeAllPendingRequests(sensorId);
  } else if (removeAllRequests) {
    requestMade = removeAllPassiveRequests(sensorId);
  }

  if (requestMade) {
    LOGD("%s: id %" PRIu8 ", otherClientPresent %d, mode %d",
         makeAllRequests ? "+" : "-", sensorId, otherClientPresent,
         static_cast<size_t>(mode));
  }
}

/**
 * Retrieves first valid sensor that has the given sensor ID. Can be
 * invoked from any thread.
 *
 * @param sensorID The sensor handle that should be used to search
 *     the current list of sensors.
 * @return The first non-null Sensor that matches the given sensor handle or
 *     nullptr if no match is found.
 */
Sensor *getFirstValidSensor(uint8_t sensorId) {
  SensorType sensorTypes[kMaxNumSensorsPerSensorId];
  size_t numSensorTypes = populateSensorTypeArrayFromSensorId(
      sensorId, sensorTypes);

  Sensor *sensor = nullptr;
  for (size_t i = 0; i < numSensorTypes; i++) {
    sensor = EventLoopManagerSingleton::get()
        ->getSensorRequestManager().getSensor(sensorTypes[i]);
    if (sensor != nullptr) {
      break;
    }
  }
  return sensor;
}

/**
 * Processes the latest client request info response for the given sensor ID.
 * Must be invoked from the CHRE thread.
 *
 * @param resp The SMGR client request info response.
 * @param sensorId The sensor ID the response is for.
 * @param transpErr The error related to the request.
 */
void onClientRequestInfoResponse(
    const sns_smgr_client_request_info_resp_msg_v01& resp,
    uint8_t sensorId,
    smr_err transpErr) {
  size_t index = getSensorMonitorIndex(sensorId);
  if (transpErr != SMR_NO_ERR) {
    LOGE("Error receiving client request info: %" PRIu8, transpErr);
  } else if (resp.resp.sns_result_t != SNS_RESULT_SUCCESS_V01) {
    LOGE("Client request info failed with error: %" PRIu8 ", id %" PRIu8,
         resp.resp.sns_err_t, sensorId);
  } else if (index == gSensorMonitors.size()) {
    LOGE("Sensor status monitor update of invalid sensor ID %" PRIu8, sensorId);
  } else {
    bool otherClientPresent = resp.other_client_present;
    if (gSensorMonitors[index].otherClientPresent != otherClientPresent) {
      onOtherClientPresenceChange(sensorId, otherClientPresent);
      gSensorMonitors[index].otherClientPresent = otherClientPresent;
    }
  }
}

/**
 * Makes an asynchronous request to SMGR to receive the latest client
 * request info.
 *
 * @param sensorId The handle to the sensor whose status has changed.
 */
void onStatusChange(uint8_t sensorId) {
  // Sensor already verified to be valid before onStatusChange is called.
  Sensor *sensor = getFirstValidSensor(sensorId);
  // Invalidate timer first so a status update isn't potentially
  // missed.
  sensor->timerHandle = CHRE_TIMER_INVALID;

  size_t index = getSensorMonitorIndex(sensorId);
  if (index == gSensorMonitors.size()) {
    LOGE("Sensor status monitor update of invalid sensor ID %" PRIu8, sensorId);
  } else {
    // Use the asynchronous sensor status monitor indication message as a cue
    // to query and obtain the latest client request info. Since the status
    // changes are processed on a delay, the current client status is out of
    // date so query the latest status asynchronously to avoid holding up the
    // CHRE thread.
    auto infoRequest =
        MakeUniqueZeroFill<sns_smgr_client_request_info_req_msg_v01>();
    auto infoResponse = MakeUnique<sns_smgr_client_request_info_resp_msg_v01>();

    if (infoRequest.isNull() || infoResponse.isNull()) {
      LOG_OOM();
    } else {
      // Enables passing the sensor ID through the event data pointer to avoid
      // allocating memory
      union NestedSensorId {
        void *eventData;
        uint8_t sensorId;
      };
      NestedSensorId nestedId = {};
      nestedId.sensorId = sensorId;

      SmrReqCallback<sns_smgr_client_request_info_resp_msg_v01> callback =
          [](UniquePtr<sns_smgr_client_request_info_resp_msg_v01> resp,
             void *data,
             smr_err transpErr) {
        NestedSensorId nestedIdCb;
        nestedIdCb.eventData = data;
        onClientRequestInfoResponse(*resp.get(),
                                    nestedIdCb.sensorId, transpErr);
      };

      infoRequest->sensor_id = sensorId;
      smr_err smrStatus = getSmrHelper()->sendReqAsync(
          gPlatformSensorServiceSmrClientHandle,
          SNS_SMGR_CLIENT_REQUEST_INFO_REQ_V01,
          &infoRequest, &infoResponse, callback, nestedId.eventData);
      if (smrStatus != SMR_NO_ERR) {
        LOGE("Error requesting client request info: %d", smrStatus);
      }
    }
  }
}

/**
 * Posts a CHRE_EVENT_SENSOR_SAMPLING_CHANGE event to the specified Nanoapp.
 *
 * @param instaceId The instance ID of the nanoapp with an open request
 * @param eventRef A reference of the sampling status event to be posted.
 */
void postSamplingStatusEvent(uint32_t instanceId, uint32_t sensorHandle,
                             const struct chreSensorSamplingStatus& status) {
  // TODO: add a generic reference counted pointer class and use it for Event
  // to share across interested nanoapps.
  auto *event = memoryAlloc<struct chreSensorSamplingStatusEvent>();
  if (event == nullptr) {
    LOGE("Failed to allocate memory for sampling status change event");
  } else {
    event->sensorHandle = sensorHandle;
    memcpy(&event->status, &status, sizeof(event->status));

    EventLoopManagerSingleton::get()->getEventLoop().postEventOrDie(
        CHRE_EVENT_SENSOR_SAMPLING_CHANGE, event, freeEventDataCallback,
        instanceId);
  }
}

/**
 * Updates the sampling status after the sensor request is accepted by SMGR.
 */
void updateSamplingStatus(Sensor *sensor, const SensorRequest& request) {
  // With SMGR's implementation, sampling interval will be filtered to be the
  // same as requested. Latency can be shorter if there were other SMGR clients
  // with proc_type also set to SNS_PROC_SSC_V01.
  // If the request is passive, 'enabled' may change over time and needs to be
  // updated.
  if (sensor != nullptr) {
    bool postUpdate = false;
    struct chreSensorSamplingStatus& status = sensor->samplingStatus;
    bool enabled = (request.getMode() != SensorMode::Off);
    if (status.enabled != enabled) {
      postUpdate = true;
      status.enabled = enabled;
    }
    if (!sensorTypeIsOneShot(sensor->getSensorType())) {
      if (status.interval != request.getInterval().toRawNanoseconds()) {
        postUpdate = true;
        status.interval = request.getInterval().toRawNanoseconds();
      }
      if (status.latency != request.getLatency().toRawNanoseconds()) {
        postUpdate = true;
        status.latency = request.getLatency().toRawNanoseconds();
      }
    }

    if (postUpdate) {
      uint32_t sensorHandle = getSensorHandleFromSensorType(
          sensor->getSensorType());

      // Only post to Nanoapps with an open request.
      auto& requests = EventLoopManagerSingleton::get()->
          getSensorRequestManager().getRequests(sensor->getSensorType());
      for (const auto& req : requests) {
        postSamplingStatusEvent(req.getInstanceId(), sensorHandle, status);
      }
    }
  }
}

/**
 * Handles sensor status provided by the SMGR framework.
 *
 * @param smgrMonitorIndMsg Indication message received from SMGR
 */
void handleSensorStatusMonitorIndication(
    const sns_smgr_sensor_status_monitor_ind_msg_v02& smgrMonitorIndMsg) {
  uint8_t sensorId = smgrMonitorIndMsg.sensor_id;

  // Only use one Sensor to avoid multiple timers per sensorId.
  Sensor *sensor = getFirstValidSensor(sensorId);
  if (sensor == nullptr) {
    LOGE("Sensor ID: %" PRIu8 " in status update doesn't correspond to "
         "valid sensor.", sensorId);
  // SMGR should send all callbacks back on the same thread which
  // means the following code won't result in any timers overriding one
  // another.
  } else if (sensor->timerHandle.load() == CHRE_TIMER_INVALID) {
    // Enables passing the sensor ID through the event data pointer to avoid
    // allocating memory
    union NestedSensorId {
      void *eventData;
      uint8_t sensorId;
    };
    NestedSensorId nestedId = {};
    nestedId.sensorId = sensorId;

    auto callback = [](uint16_t /* type */, void *data) {
      NestedSensorId nestedIdCb;
      nestedIdCb.eventData = data;
      onStatusChange(nestedIdCb.sensorId);
    };

    // Schedule a delayed callback to handle sensor status change on the main
    // thread.
    TimerHandle timer = EventLoopManagerSingleton::get()->setDelayedCallback(
        SystemCallbackType::SensorStatusUpdate,
        nestedId.eventData,
        callback,
        kStatusDelayIntervalNanos);
    sensor->timerHandle = timer;
  }
}

/**
 * This callback is invoked by the SMR framework when an asynchronous message is
 * delivered. Unhandled messages are logged.
 *
 * @param handle Handle for the SMR client this indication was received on.
 * @param messageId The message ID number.
 * @param decodedInd Buffer containing decoded (C struct) message data.
 * @param decodedIndLen Size of the decoded buffer in bytes.
 * @param callbackData Data that is provided as a context to this callback. This
 *                     is not used in this context.
 *
 * @see smr_client_ind_cb
 */
void platformSensorInternalServiceIndicationCallback(
    smr_client_hndl handle, unsigned int messageId, void *decodedInd,
    unsigned int decodedIndLen, void *callbackData) {
  switch (messageId) {
    case SNS_SMGR_SENSOR_STATUS_MONITOR_IND_V02: {
      CHRE_ASSERT(decodedIndLen >=
                  sizeof(sns_smgr_sensor_status_monitor_ind_msg_v02));
      auto *monitorInd =
          static_cast<sns_smgr_sensor_status_monitor_ind_msg_v02 *>(decodedInd);
      handleSensorStatusMonitorIndication(*monitorInd);
      break;
    }
    default:
      LOGW("Received unhandled sensor internal service message: 0x%x",
           messageId);
      break;
  };
}

/**
 * Adds or removes an SMGR sensor monitor for the specified sensor ID.
 *
 * @param sensorId The sensor ID to add/remove sensor status monitor for.
 * @param enable true to add and false to remove the status monitor.
 */
void setSensorMonitorRequest(uint8_t sensorId, bool enable) {
  auto monitorRequest =
      MakeUniqueZeroFill<sns_smgr_sensor_status_monitor_req_msg_v02>();
  auto monitorResponse =
      MakeUnique<sns_smgr_sensor_status_monitor_resp_msg_v02>();

  if (monitorRequest.isNull() || monitorResponse.isNull()) {
    LOGE("Failed to allocate monitor request/response");
  } else {
    monitorRequest->sensor_id = sensorId;
    monitorRequest->registering = enable;

    smr_err status = getSmrHelper()->sendReqSync(
        gPlatformSensorInternalServiceSmrClientHandle,
        SNS_SMGR_SENSOR_STATUS_MONITOR_REQ_V02,
        &monitorRequest, &monitorResponse);
    if (status != SMR_NO_ERR) {
      LOGE("Error setting sensor status monitor: %d", status);
    } else if (monitorResponse->resp.sns_result_t != SNS_RESULT_SUCCESS_V01) {
      LOGE("Sensor status monitor request failed with error: %" PRIu8
           " sensor ID %" PRIu8 " enable %d",
           monitorResponse->resp.sns_err_t, sensorId, enable);
    }
  }
}

/**
 * Adds and initializes a sensor monitor for the specified sensor ID if it
 * doesn't exist yet.
 *
 * @param sensorId The sensor ID to request monitor for.
 */
void addSensorMonitor(uint8_t sensorId) {
  size_t index = getSensorMonitorIndex(sensorId);
  if (index == gSensorMonitors.size()) {
    LOGD("Adding sensor status monitor for sensor ID %" PRIu8, sensorId);

    // Initialize sensor monitor status before making the request.
    SensorMonitor monitor;
    monitor.sensorId = sensorId;
    monitor.otherClientPresent = false;
    gSensorMonitors.push_back(monitor);

    // Make a request to add the status monitor
    setSensorMonitorRequest(sensorId, true);
  }
}

/**
 * Requests the sensors for a given sensor ID and appends them to the provided
 * list of sensors. If an error occurs, false is returned.
 *
 * @param sensorId The sensor ID to request sensor info for.
 * @param sensors The list of sensors to append newly found sensors to.
 * @return Returns false if an error occurs.
 */
bool getSensorsForSensorId(uint8_t sensorId,
                           DynamicVector<Sensor> *sensors) {
  bool success = false;
  auto sensorInfoRequest =
      MakeUniqueZeroFill<sns_smgr_single_sensor_info_req_msg_v01>();
  auto sensorInfoResponse =
      MakeUnique<sns_smgr_single_sensor_info_resp_msg_v01>();

  if (sensorInfoRequest.isNull() || sensorInfoResponse.isNull()) {
    LOGE("Failed to allocate sensor info msg");
  } else {
    sensorInfoRequest->SensorID = sensorId;

    smr_err status = getSmrHelper()->sendReqSync(
        gPlatformSensorServiceSmrClientHandle,
        SNS_SMGR_SINGLE_SENSOR_INFO_REQ_V01,
        &sensorInfoRequest, &sensorInfoResponse);

    if (status != SMR_NO_ERR) {
      LOGE("Error requesting single sensor info: %d", status);
    } else if (sensorInfoResponse->Resp.sns_result_t !=
                   SNS_RESULT_SUCCESS_V01) {
      LOGE("Single sensor info request failed with error: %d",
           sensorInfoResponse->Resp.sns_err_t);
    } else {
      const sns_smgr_sensor_info_s_v01& sensorInfoList =
          sensorInfoResponse->SensorInfo;
      for (uint32_t i = 0; i < sensorInfoList.data_type_info_len; i++) {
        const sns_smgr_sensor_datatype_info_s_v01& sensorInfo =
            sensorInfoList.data_type_info[i];
        LOGD("SensorID %" PRIu8 ", DataType %" PRIu8 ", MaxRate %" PRIu16
             "Hz, SensorName %s",
             sensorInfo.SensorID, sensorInfo.DataType,
             sensorInfo.MaxSampleRate, sensorInfo.SensorName);

        SensorType sensorType = getSensorTypeFromSensorId(
            sensorInfo.SensorID, sensorInfo.DataType,
            SNS_SMGR_CAL_SEL_FULL_CAL_V01);
        if (sensorType != SensorType::Unknown) {
          addSensor(sensorInfo, SNS_SMGR_CAL_SEL_FULL_CAL_V01, sensors);

          // Add an uncalibrated version if defined.
          SensorType uncalibratedType = getSensorTypeFromSensorId(
              sensorInfo.SensorID, sensorInfo.DataType,
              SNS_SMGR_CAL_SEL_FACTORY_CAL_V01);
          if (sensorType != uncalibratedType) {
            addSensor(sensorInfo, SNS_SMGR_CAL_SEL_FACTORY_CAL_V01, sensors);
          }
        }
      }
      success = true;
    }
  }

  return success;
}

/**
 * Converts a SensorMode into an SMGR request action. When the net request for
 * a sensor is considered to be active an add operation is required for the
 * SMGR request. When the sensor becomes inactive the request is deleted.
 *
 * @param mode The sensor mode.
 * @return Returns the SMGR request action given the sensor mode.
 */
uint8_t getSmgrRequestActionForMode(SensorMode mode) {
  if (mode != SensorMode::Off) {
    return SNS_SMGR_BUFFERING_ACTION_ADD_V01;
  } else {
    return SNS_SMGR_BUFFERING_ACTION_DELETE_V01;
  }
}

/**
 * Specify the sensor decimation type.
 *
 * @param sensorId The sensorID as provided by the SMGR.
 * @param dataType The dataType for the sesnor as provided by the SMGR.
 * return The decimation type as defined by the SMGR.
 */
uint8_t getDecimationType(uint8_t sensorId, uint8_t dataType) {
  // Request filtered data for accel and gyro to reduce noise aliasing in case
  // SMGR has other higher ODR clients.
  if ((sensorId == SNS_SMGR_ID_ACCEL_V01 || sensorId == SNS_SMGR_ID_GYRO_V01)
      && dataType == SNS_SMGR_DATA_TYPE_PRIMARY_V01) {
    return SNS_SMGR_DECIMATION_FILTER_V01;
  } else {
    return SNS_SMGR_DECIMATION_RECENT_SAMPLE_V01;
  }
}

/**
 * Populates a sns_smgr_buffering_req_msg_v01 struct to request sensor data.
 *
 * @param request The new request to set this sensor to.
 * @param sensorId The sensorID as provided by the SMGR request for sensor info.
 * @param dataType The dataType for the sesnor as provided by the SMGR request
 *                 for sensor info.
 * @param calType The calibration type (CAL_SEL) as defined in the SMGR API.
 * @param minInterval The minimum interval allowed by this sensor.
 * @param sensorDataRequest The pointer to the data request to be populated.
 */
void populateSensorRequest(
    const SensorRequest& chreRequest, uint8_t sensorId, uint8_t dataType,
    uint8_t calType, uint64_t minInterval,
    sns_smgr_buffering_req_msg_v01 *sensorRequest) {
  // Zero the fields in the request. All mandatory and unused fields are
  // specified to be set to false or zero so this is safe.
  memset(sensorRequest, 0, sizeof(*sensorRequest));

  // Reconstructs a request to deliver one-shot sensors' data ASAP and set
  // default interval to some meaningful number.
  bool isOneShot = sensorTypeIsOneShot(getSensorTypeFromSensorId(
      sensorId, dataType, calType));
  uint64_t cappedInterval = chreRequest.getInterval().toRawNanoseconds();
  if (cappedInterval == CHRE_SENSOR_INTERVAL_DEFAULT) {
    // For one-shot sensors, we've overridden minInterval to default in init.
    // However, for InstantMotion/StationaryDetect, making a request with
    // default interval will not trigger.
    cappedInterval =
        isOneShot ? kDefaultInterval : std::max(minInterval, kDefaultInterval);
  }
  SensorRequest request(chreRequest.getMode(), Nanoseconds(cappedInterval),
                        isOneShot ? Nanoseconds(0) : chreRequest.getLatency());

  // Build the request for one sensor at the requested rate. An add action for a
  // ReportID that is already in use causes a replacement of the last request.
  sensorRequest->ReportId = getReportId(sensorId, dataType, calType);
  sensorRequest->Action = getSmgrRequestActionForMode(request.getMode());

  // SMGR report interval should be (interval + latency). However, to handle
  // fractional-interval latency setting and to guarantee meeting chre request,
  // report interval is set to latency only. Also, lower-bound batchInterval as
  // request to SMGR would fail if batchInterval < interval.
  Nanoseconds batchInterval =
      std::max(request.getLatency(), request.getInterval());
  sensorRequest->ReportRate = intervalToSmgrQ16ReportRate(batchInterval);
  sensorRequest->Item_len = 1;  // One sensor per request if possible.
  sensorRequest->Item[0].SensorId = sensorId;
  sensorRequest->Item[0].DataType = dataType;
  sensorRequest->Item[0].Decimation = getDecimationType(sensorId, dataType);
  sensorRequest->Item[0].Calibration = calType;
  sensorRequest->Item[0].SamplingRate =
      intervalToSmgrSamplingRate(request.getInterval());

  // Add a dummy primary sensor to accompany a secondary temperature sensor.
  // This is requred by the SMGR. The primary sensor is requested with the same
  // (low) rate and the same latency, whose response data will be ignored.
  if (isSecondaryTemperature(sensorRequest->ReportId)) {
    sensorRequest->Item_len = 2;
    sensorRequest->Item[1].SensorId = sensorId;
    sensorRequest->Item[1].DataType = SNS_SMGR_DATA_TYPE_PRIMARY_V01;
    sensorRequest->Item[1].Decimation = getDecimationType(
        sensorId, SNS_SMGR_DATA_TYPE_PRIMARY_V01);
    sensorRequest->Item[1].Calibration = SNS_SMGR_CAL_SEL_FULL_CAL_V01;
    sensorRequest->Item[1].SamplingRate = sensorRequest->Item[0].SamplingRate;
  }

  // Synchronize fifo flushes with other clients that have SSC proc_type.
  // send_indications_during_suspend has no effect on data sent to SLPI.
  // Default is to synchronize with AP clients, which may shorten flush
  // intervals for data sent to the AP.
  sensorRequest->notify_suspend_valid = true;
  sensorRequest->notify_suspend.proc_type = SNS_PROC_SSC_V01;
  sensorRequest->notify_suspend.send_indications_during_suspend = true;
}

/**
 * Determines whether a request is allowed. A passive request is not always
 * allowed.
 *
 * @param sensorType The SensorType of this request
 * @param request The intended sensor request
 * @return true if the request is allowed.
 */
bool isRequestAllowed(SensorType sensorType, const SensorRequest& request) {
  bool allowed = false;

  const Sensor *sensor = EventLoopManagerSingleton::get()
      ->getSensorRequestManager().getSensor(sensorType);
  if (sensor != nullptr) {
    if (sensorModeIsPassive(request.getMode())) {
      size_t index = getSensorMonitorIndex(sensor->sensorId);
      if (index == gSensorMonitors.size()) {
        LOGE("SensorId %" PRIu8 " doesn't have a monitor", sensor->sensorId);
      } else {
        SensorMode mergedMode = getMergedMode(
            sensor->sensorId, sensorType, request);
        bool otherClientPresent = gSensorMonitors[index].otherClientPresent;
        allowed = (sensorModeIsActive(mergedMode) || otherClientPresent);
        LOGD("sensorType %d allowed %d: mergedMode %d, otherClientPresent %d",
             static_cast<size_t>(sensorType), allowed,
             static_cast<int>(mergedMode), otherClientPresent);
      }
    } else {
      // If it's an ACTIVE or an OFF request, it's always allowed.
      allowed = true;
    }
  }
  return allowed;
}

/**
 * Makes a SNS_SMGR_BUFFERING_REQ request based on the arguments provided.
 *
 * @param sensorId The sensorID as provided by the SMGR.
 * @param dataType The dataType for the sesnor as provided by the MSGR.
 * @param calType The calibration type (CAL_SEL) as defined in the SMGR API.
 * @param minInterval The minimum interval of this sensor.
 * @param request The sensor request
 * @return true if the request has been made successfully.
 */
bool makeBufferingReq(uint8_t sensorId, uint8_t dataType, uint8_t calType,
                      uint64_t minInterval, const SensorRequest& request) {
  bool success = false;
  auto sensorRequest = MakeUniqueZeroFill<sns_smgr_buffering_req_msg_v01>();
  auto sensorResponse = MakeUnique<sns_smgr_buffering_resp_msg_v01>();

  if (sensorRequest.isNull() || sensorResponse.isNull()) {
    LOGE("Failed to allocate buffering msg");
  } else {
    populateSensorRequest(request, sensorId, dataType, calType,
                          minInterval, sensorRequest.get());

    smr_err status = getSmrHelper()->sendReqSync(
        gPlatformSensorServiceSmrClientHandle, SNS_SMGR_BUFFERING_REQ_V01,
        &sensorRequest, &sensorResponse);

    if (status != SMR_NO_ERR) {
      LOGE("Error requesting sensor data: %d", status);
    } else if (sensorResponse->Resp.sns_result_t != SNS_RESULT_SUCCESS_V01
        || (sensorResponse->AckNak != SNS_SMGR_RESPONSE_ACK_SUCCESS_V01
            && sensorResponse->AckNak != SNS_SMGR_RESPONSE_ACK_MODIFIED_V01)) {
      LOGE("Sensor data request failed with error: %d, AckNak: %d",
           sensorResponse->Resp.sns_err_t, sensorResponse->AckNak);
    } else {
      success = true;
    }
  }

  return success;
}

/**
 * Makes a SNS_SMGR_BUFFERING_REQ request if necessary.
 *
 * @param sensorType The sensor type of the request.
 * @param request The sensor request to be made.
 * @return true if the request has been accepted.
 */
bool makeRequest(SensorType sensorType, const SensorRequest& request) {
  bool success = false;

  Sensor *sensor = EventLoopManagerSingleton::get()->getSensorRequestManager()
      .getSensor(sensorType);
  if (sensor != nullptr) {
    // Do not make an off request if the sensor is already off. Otherwise, SMGR
    // returns an error.
    if (request.getMode() == SensorMode::Off) {
      success = sensor->isSensorOff;
    }

    // Make a SMGR buffering request if necessary.
    if (!success) {
      success = makeBufferingReq(sensor->sensorId, sensor->dataType,
                                 sensor->calType, sensor->minInterval, request);
    }
  }

  // TODO: handle makeBufferingReq failures
  if (success) {
    // Update internal states if request was accepted by SMGR.
    sensor->isSensorOff = (request.getMode() == SensorMode::Off);

    if (request.getMode() == SensorMode::Off) {
      sensor->lastEventValid = false;
    }

    updateSamplingStatus(sensor, request);
  }
  return success;
}

/**
 * Makes all pending requests of the specified sensor ID to SMGR.
 *
 * @param sensorId The sensor ID whose pending requests are to be made.
 * @return true if an ADD request has been accepted.
 */
bool makeAllPendingRequests(uint8_t sensorId) {
  // Identify sensor types to check for pending requests
  SensorType sensorTypes[kMaxNumSensorsPerSensorId];
  size_t numSensorTypes = populateSensorTypeArrayFromSensorId(
      sensorId, sensorTypes);
  bool accepted = false;
  for (size_t i = 0; i < numSensorTypes; i++) {
    const Sensor *sensor = EventLoopManagerSingleton::get()
        ->getSensorRequestManager().getSensor(sensorTypes[i]);

    // If sensor is off and the request is not off, it's a pending request.
    if (sensor != nullptr && sensor->isSensorOff
        && sensor->getRequest().getMode() != SensorMode::Off) {
      accepted |= makeRequest(sensorTypes[i], sensor->getRequest());
    }
  }
  return accepted;
}

/**
 * Identifies and removes passive requests that have been made to the SMGR, and
 * adds them to the sensor monitor.
 *
 * @param sensorId The sensor ID whose passive requests are to be removed.
 * @return true if a DELETE request has been accepted.
 */
bool removeAllPassiveRequests(uint8_t sensorId) {
  // Specify sensor types to check for passive requests
  SensorType sensorTypes[kMaxNumSensorsPerSensorId];
  size_t numSensorTypes = populateSensorTypeArrayFromSensorId(
      sensorId, sensorTypes);
  bool accepted = false;
  for (size_t i = 0; i < numSensorTypes; i++) {
    const Sensor *sensor = EventLoopManagerSingleton::get()
        ->getSensorRequestManager().getSensor(sensorTypes[i]);

    // Turn off sensors that have a passive request
    if (sensor != nullptr
        && sensorModeIsPassive(sensor->getRequest().getMode())) {
      SensorRequest offRequest;
      accepted |= makeRequest(sensorTypes[i], offRequest);
    }
  }
  return accepted;
}

}  // anonymous namespace

PlatformSensor::~PlatformSensor() {
  if (lastEvent != nullptr) {
    LOGD("Releasing lastEvent: id %" PRIu8 ", type %" PRIu8 ", cal %" PRIu8
         ", size %zu", sensorId, dataType, calType, lastEventSize);
    memoryFree(lastEvent);
  }
}

void PlatformSensor::init() {
  // Timeout for SMR client initialization, in milliseconds.
  constexpr uint32_t kSmrInitTimeoutMs = 10;

  SmrHelperSingleton::init();

  // sns_smgr_api_v01
  qmi_idl_service_object_type smgrSvcObj =
      SNS_SMGR_SVC_get_service_object_v01();
  if (smgrSvcObj == nullptr) {
    FATAL_ERROR("Failed to obtain the SNS SMGR service instance");
  }

  smr_err result = getSmrHelper()->waitForService(smgrSvcObj);
  if (result != SMR_NO_ERR) {
    FATAL_ERROR("Failed while waiting for SNS SMGR service");
  }

  // Note: giving nullptr for err_cb prevents this from degrading to a regular
  // QMI client if the service is not found.
  smr_err status = smr_client_init(
      smgrSvcObj, SMR_CLIENT_INSTANCE_ANY,
      platformSensorServiceIndicationCallback, nullptr /* ind_cb_data */,
      kSmrInitTimeoutMs, nullptr /* err_cb */, nullptr /* err_cb_data */,
      &gPlatformSensorServiceSmrClientHandle, isSlpiUimgSupported());
  if (status != SMR_NO_ERR) {
    FATAL_ERROR("Failed to initialize SMGR client: %d", status);
  }

  // sns_smgr_interal_api_v02
  qmi_idl_service_object_type smgrInternalSvcObj =
      SNS_SMGR_INTERNAL_SVC_get_service_object_v02();
  if (smgrInternalSvcObj == nullptr) {
    FATAL_ERROR("Failed to obtain the SNS SMGR internal service instance");
  }

  result = getSmrHelper()->waitForService(smgrInternalSvcObj);
  if (result != SMR_NO_ERR) {
    FATAL_ERROR("Failed while waiting for SNS SMGR internal service");
  }

  status = smr_client_init(
      smgrInternalSvcObj, SMR_CLIENT_INSTANCE_ANY,
      platformSensorInternalServiceIndicationCallback,
      nullptr /* ind_cb_data */, kSmrInitTimeoutMs, nullptr /* err_cb */,
      nullptr /* err_cb_data */, &gPlatformSensorInternalServiceSmrClientHandle,
      isSlpiUimgSupported());
  if (status != SMR_NO_ERR) {
    FATAL_ERROR("Failed to initialize SMGR internal client: %d", status);
  }
}

void PlatformSensor::deinit() {
  smr_err err = getSmrHelper()->releaseSync(
      gPlatformSensorServiceSmrClientHandle);
  if (err != SMR_NO_ERR) {
    LOGE("Failed to release SMGR client: %d", err);
  }
  gPlatformSensorServiceSmrClientHandle = nullptr;

  err = getSmrHelper()->releaseSync(
      gPlatformSensorInternalServiceSmrClientHandle);
  if (err != SMR_NO_ERR) {
    LOGE("Failed to release SMGR internal client: %d", err);
  }
  gPlatformSensorInternalServiceSmrClientHandle = nullptr;

  // Clearing all sensor status monitors. Releasing an SMR client also releases
  // all sensor status monitor requests.
  gSensorMonitors.clear();
  SmrHelperSingleton::deinit();
}

bool PlatformSensor::getSensors(DynamicVector<Sensor> *sensors) {
  CHRE_ASSERT(sensors);

  auto sensorListRequest =
      MakeUniqueZeroFill<sns_smgr_all_sensor_info_req_msg_v01>();
  auto sensorListResponse = MakeUnique<sns_smgr_all_sensor_info_resp_msg_v01>();

  smr_err status = getSmrHelper()->sendReqSync(
      gPlatformSensorServiceSmrClientHandle, SNS_SMGR_ALL_SENSOR_INFO_REQ_V01,
      &sensorListRequest, &sensorListResponse);

  bool success = false;
  if (status != SMR_NO_ERR) {
    LOGE("Error requesting sensor list: %d", status);
  } else if (sensorListResponse->Resp.sns_result_t != SNS_RESULT_SUCCESS_V01) {
    LOGE("Sensor list lequest failed with error: %d",
         sensorListResponse->Resp.sns_err_t);
  } else {
    success = true;
    for (uint32_t i = 0; i < sensorListResponse->SensorInfo_len; i++) {
      uint8_t sensorId = sensorListResponse->SensorInfo[i].SensorID;
      if (!getSensorsForSensorId(sensorId, sensors)) {
        success = false;
        break;
      }
    }
  }

  return success;
}

bool PlatformSensor::applyRequest(const SensorRequest& request) {
  bool success;

  if (!SmrHelperSingleton::isInitialized()) {
    // Off requests made as part of shutdown come after PlatformSensor::deinit()
    // which releases our SMGR clients, removing all requests. Report success in
    // this case.
    success = (request.getMode() == SensorMode::Off) ? true : false;
    CHRE_ASSERT_LOG(success, "Sensor request made before init/after deinit");
  } else {
    // Adds a sensor monitor the first time this sensor is requested.
    addSensorMonitor(this->sensorId);

    // As sensor status monior indication doesn't support secondary sensor
    // status change, Light sensor (a secondary one) is always overridden to be
    // requested with an active mode.
    bool passiveLight = (getSensorType() == SensorType::Light
                         && sensorModeIsPassive(request.getMode()));
    if (passiveLight) {
      LOGE("Passive request for Light sensor is not supported. "
           "Overriding request to active");
    }
    SensorRequest localRequest(
        passiveLight ? SensorMode::ActiveContinuous : request.getMode(),
        request.getInterval(), request.getLatency());

    // Determines whether a (passive) request is allowed at this point.
    bool requestAllowed = isRequestAllowed(getSensorType(), localRequest);

    // If request is not allowed, turn off the sensor. Otherwise, make request.
    SensorRequest offRequest;
    success = makeRequest(getSensorType(),
                          requestAllowed ? localRequest : offRequest);
  }
  return success;
}

bool PlatformSensor::flushAsync() {
  // NOTE: SMGR framework flushes all pending data when a new request comes in
  //       (ref sns_rh_sol_schedule_existing_report() in sns_rh_sol.c).
  //       In this implementation of flushAsync, we make a request identical to
  //       the existing sensor request, blocking on an asynchronous response,
  //       and assume that the flush request has completed when this identical
  //       sensor request is successfully handled and executed. This
  //       implementation mirrors the sensors HAL implementation of flush.
  bool success = false;
  Sensor *sensor = EventLoopManagerSingleton::get()->getSensorRequestManager()
      .getSensor(getSensorType());
  if (sensor != nullptr) {
    success = applyRequest(sensor->getRequest());
    if (success) {
      EventLoopManagerSingleton::get()->getSensorRequestManager()
          .handleFlushCompleteEvent(CHRE_ERROR_NONE, getSensorType());
    }
  }
  return success;
}

SensorType PlatformSensor::getSensorType() const {
  return getSensorTypeFromSensorId(this->sensorId, this->dataType,
                                   this->calType);
}

uint64_t PlatformSensor::getMinInterval() const {
  return minInterval;
}

const char *PlatformSensor::getSensorName() const {
  return sensorName;
}

PlatformSensor::PlatformSensor(PlatformSensor&& other) {
  // Our move assignment operator doesn't assume that "this" is initialized, so
  // we can just use that here
  *this = std::move(other);
}

PlatformSensor& PlatformSensor::operator=(PlatformSensor&& other) {
  // Note: if this implementation is ever changed to depend on "this" containing
  // initialized values, the move constructor implemenation must be updated
  sensorId = other.sensorId;
  dataType = other.dataType;
  calType = other.calType;
  memcpy(sensorName, other.sensorName, SNS_SMGR_MAX_SENSOR_NAME_SIZE_V01);
  minInterval = other.minInterval;

  lastEvent = other.lastEvent;
  other.lastEvent = nullptr;

  lastEventSize = other.lastEventSize;
  other.lastEventSize = 0;

  lastEventValid = other.lastEventValid;
  isSensorOff = other.isSensorOff;
  samplingStatus = other.samplingStatus;

  return *this;
}

ChreSensorData *PlatformSensor::getLastEvent() const {
  return (this->lastEventValid) ? this->lastEvent : nullptr;
}

bool PlatformSensor::getSamplingStatus(
    struct chreSensorSamplingStatus *status) const {
  CHRE_ASSERT(status);

  memcpy(status, &samplingStatus, sizeof(*status));
  return true;
}

bool PlatformSensor::getThreeAxisBias(
    struct chreSensorThreeAxisData *bias) const {
  // TODO: Implement this.
  return false;
}

void PlatformSensorBase::setLastEvent(const ChreSensorData *event) {
  memcpy(this->lastEvent, event, this->lastEventSize);
  this->lastEventValid = true;
}

smr_client_hndl getSensorServiceSmrClientHandle() {
  return gPlatformSensorServiceSmrClientHandle;
}

}  // namespace chre
