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

#include <chre.h>
#include <cinttypes>

#include "chre/util/macros.h"
#include "chre/util/nanoapp/log.h"
#include "chre/util/time.h"

#define LOG_TAG "[GnssWorld]"

#ifdef CHRE_NANOAPP_INTERNAL
namespace chre {
namespace {
#endif  // CHRE_NANOAPP_INTERNAL

//! Control which test(s) to run
constexpr bool kEnableLocationTest = true;
constexpr bool kEnableMeasurementTest = true;

//! A dummy cookie to pass into the session async and timer request.
const uint32_t kLocationSessionCookie = 0x1337;
const uint32_t kMeasurementSessionCookie = 0xdaad;

//! The minimum time to the next fix for a location.
constexpr chre::Milliseconds kLocationMinTimeToNextFix(0);

//! The interval in seconds between updates.
const uint32_t kReportIntervals[] = {
  30,
  15,
  30,
  15,
  0,
  10,
};

//! Whether a specific Gnss capability is supported by the platform
bool gLocationSupported = false;
bool gMeasurementSupported = false;

uint32_t gLocationTimerHandle;
uint32_t gLocationTimerCount = 0;

uint32_t gMeasurementTimerHandle;
uint32_t gMeasurementTimerCount = 0;

//! Whether an async result has been received.
bool gLocationAsyncResultReceived = false;
bool gMeasurementAsyncResultReceived = false;

void makeLocationRequest() {
  uint32_t interval = kReportIntervals[gLocationTimerCount++];
  LOGI("Modifying location update interval to %" PRIu32 " sec", interval);

  if (interval > 0) {
    if (chreGnssLocationSessionStartAsync(
          interval * 1000,
          kLocationMinTimeToNextFix.getMilliseconds(),
          &kLocationSessionCookie)) {
      LOGI("Location session start request sent");
    } else {
      LOGE("Error sending location session start request");
    }
  } else {
    if (chreGnssLocationSessionStopAsync(
          &kLocationSessionCookie)) {
      LOGI("Location session stop request sent");
    } else {
      LOGE("Error sending location session stop request");
    }
  }

  // set a timer to verify reception of async result.
  gLocationTimerHandle = chreTimerSet(
      CHRE_GNSS_ASYNC_RESULT_TIMEOUT_NS, /* 5 sec in CHRE 1.1 */
      &kLocationSessionCookie, true /* oneShot */);
}

void makeMeasurementRequest() {
  uint32_t interval = kReportIntervals[gMeasurementTimerCount++];
  LOGI("Modifying measurement update interval to %" PRIu32 " sec", interval);

  if (interval > 0) {
    if (chreGnssMeasurementSessionStartAsync(
          interval * 1000, &kMeasurementSessionCookie)) {
      LOGI("Measurement session start request sent");
    } else {
      LOGE("Error sending measurement session start request");
    }
  } else {
    if (chreGnssMeasurementSessionStopAsync(
          &kMeasurementSessionCookie)) {
      LOGI("Measurement session stop request sent");
    } else {
      LOGE("Error sending measurement session stop request");
    }
  }

  // set a timer to verify reception of async result.
  gMeasurementTimerHandle = chreTimerSet(
      CHRE_GNSS_ASYNC_RESULT_TIMEOUT_NS, /* 5 sec in CHRE 1.1 */
      &kMeasurementSessionCookie, true /* oneShot */);
}

void handleTimerEvent(const void *eventData) {
  bool validData = true;

  bool supported;
  const char *name;
  uint32_t timerCount;
  bool *asyncResultReceived;
  void (*makeRequest)();

  if (eventData == &kLocationSessionCookie) {
    supported = gLocationSupported;
    name = "location";
    timerCount = gLocationTimerCount;
    asyncResultReceived = &gLocationAsyncResultReceived;
    makeRequest = makeLocationRequest;
  } else if (eventData == &kMeasurementSessionCookie) {
    supported = gMeasurementSupported;
    name = "measurement";
    timerCount = gMeasurementTimerCount;
    asyncResultReceived = &gMeasurementAsyncResultReceived;
    makeRequest = makeMeasurementRequest;
  } else {
    validData = false;
    LOGE("Invalid timer cookie");
  }

  if (validData) {
    LOGI("%s timer event received, count %" PRIu32, name, timerCount);
    if (!*asyncResultReceived) {
      LOGE("%s async result not received!", name);
    }
    *asyncResultReceived = false;

    if (supported && timerCount < ARRAY_SIZE(kReportIntervals)) {
      makeRequest();
    }
  }
}

const char *getNameStringFromRequestType(uint8_t requestType) {
  switch (requestType) {
    case CHRE_GNSS_REQUEST_TYPE_LOCATION_SESSION_START:
    case CHRE_GNSS_REQUEST_TYPE_LOCATION_SESSION_STOP:
      return "location";
    case CHRE_GNSS_REQUEST_TYPE_MEASUREMENT_SESSION_START:
    case CHRE_GNSS_REQUEST_TYPE_MEASUREMENT_SESSION_STOP:
      return "measurement";
    default:
      return nullptr;
  }
}

const char *getActionStringFromRequestType(uint8_t requestType) {
  switch (requestType) {
    case CHRE_GNSS_REQUEST_TYPE_LOCATION_SESSION_START:
    case CHRE_GNSS_REQUEST_TYPE_MEASUREMENT_SESSION_START:
      return "start";
    case CHRE_GNSS_REQUEST_TYPE_LOCATION_SESSION_STOP:
    case CHRE_GNSS_REQUEST_TYPE_MEASUREMENT_SESSION_STOP:
      return "stop";
    default:
      return nullptr;
  }
}

void handleGnssAsyncResult(const chreAsyncResult *result) {
  const char *name = getNameStringFromRequestType(result->requestType);
  const char *action = getActionStringFromRequestType(result->requestType);
  bool *received = nullptr;
  const uint32_t *cookie;

  switch (result->requestType) {
    case CHRE_GNSS_REQUEST_TYPE_LOCATION_SESSION_START:
    case CHRE_GNSS_REQUEST_TYPE_LOCATION_SESSION_STOP:
      received = &gLocationAsyncResultReceived;
      cookie = &kLocationSessionCookie;
      break;

    case CHRE_GNSS_REQUEST_TYPE_MEASUREMENT_SESSION_START:
    case CHRE_GNSS_REQUEST_TYPE_MEASUREMENT_SESSION_STOP:
      received = &gMeasurementAsyncResultReceived;
      cookie = &kMeasurementSessionCookie;
      break;

    default:
      LOGE("Received invalid async result %" PRIu8, result->requestType);
      break;
  }

  if (received != nullptr) {
    *received = true;
    if (result->success) {
      LOGI("GNSS %s %s success", name, action);
    } else {
      LOGE("GNSS %s %s failure: %" PRIu8, name, action, result->errorCode);
    }

    if (result->cookie != cookie) {
      LOGE("GNSS %s session %s request cookie mismatch", name, action);
    }
  }
}

void handleGnssLocationEvent(const chreGnssLocationEvent *event) {
  LOGI("Received location: %" PRId32 ", %" PRId32, event->latitude_deg_e7,
       event->longitude_deg_e7);
  LOGI("  timestamp (ms): %" PRIu64, event->timestamp);
  LOGI("  altitude (m): %f", event->altitude);
  LOGI("  speed (m/s): %f", event->speed);
  LOGI("  bearing (deg): %f", event->bearing);
  LOGI("  accuracy: %f", event->accuracy);
  LOGI("  flags: %" PRIx16, event->flags);
  LOGI("  altitude_accuracy: %f", event->altitude_accuracy);
  LOGI("  speed_accuracy: %f", event->speed_accuracy);
  LOGI("  bearing_accuracy: %f", event->bearing_accuracy);
}

void handleGnssDataEvent(const chreGnssDataEvent *event) {
  LOGI("Received data: %" PRIu8 " measurements", event->measurement_count);

  const struct chreGnssMeasurement *measurement = event->measurements;
  for (uint8_t i = 0; i < event->measurement_count; i++) {
    LOGI("%" PRIu8 ": const %" PRIu8 ", cn0 %f",
         i, measurement->constellation, measurement->c_n0_dbhz);
    measurement++;
  }
}

bool nanoappStart() {
  LOGI("App started as instance %" PRIu32, chreGetInstanceId());

  const char *gnssCapabilitiesStr;
  uint32_t gnssCapabilities = chreGnssGetCapabilities();
  switch (gnssCapabilities) {
    case CHRE_GNSS_CAPABILITIES_LOCATION
        | CHRE_GNSS_CAPABILITIES_MEASUREMENTS:
      gnssCapabilitiesStr = "LOCATION | MEASUREMENTS";
      gLocationSupported = true;
      gMeasurementSupported = true;
      break;
    case CHRE_GNSS_CAPABILITIES_LOCATION:
      gnssCapabilitiesStr = "LOCATION";
      gLocationSupported = true;
      break;
    case CHRE_GNSS_CAPABILITIES_MEASUREMENTS:
      gnssCapabilitiesStr = "MEASUREMENTS";
      gMeasurementSupported = true;
      break;
    case CHRE_GNSS_CAPABILITIES_NONE:
      gnssCapabilitiesStr = "NONE";
      break;
    default:
      gnssCapabilitiesStr = "INVALID";
  }

  LOGI("Detected GNSS support as: %s (%" PRIu32 ")",
       gnssCapabilitiesStr, gnssCapabilities);

  if (gLocationSupported && kEnableLocationTest) {
    makeLocationRequest();
  }

  if (gMeasurementSupported && kEnableMeasurementTest) {
    makeMeasurementRequest();
  }

  return true;
}

void nanoappHandleEvent(uint32_t senderInstanceId,
                        uint16_t eventType,
                        const void *eventData) {
  switch (eventType) {
    case CHRE_EVENT_GNSS_ASYNC_RESULT:
      handleGnssAsyncResult(static_cast<const chreAsyncResult *>(eventData));
      break;
    case CHRE_EVENT_GNSS_LOCATION:
      handleGnssLocationEvent(
          static_cast<const chreGnssLocationEvent *>(eventData));
      break;
    case CHRE_EVENT_GNSS_DATA:
      handleGnssDataEvent(static_cast<const chreGnssDataEvent *>(eventData));
      break;
    case CHRE_EVENT_TIMER:
      handleTimerEvent(eventData);
      break;
    default:
      LOGW("Unhandled event type %" PRIu16, eventType);
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

CHRE_STATIC_NANOAPP_INIT(GnssWorld, chre::kGnssWorldAppId, 0);
#endif  // CHRE_NANOAPP_INTERNAL
