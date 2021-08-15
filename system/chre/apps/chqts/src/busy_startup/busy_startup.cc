/*
 * Copyright (C) 2016 Android Open Source Project
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

/**
 * Nanoapp which performs a number of operations within nanoappStart().
 *
 * This nanoapp is to confirm a number of CHRE methods can be invoked from
 * within nanoappStart().  There are other tests which test each of these
 * CHRE methods more in depth.  We're just doing a sanity check that calling
 * from nanoappStart() works at all.
 *
 * Specifically, we're testing:
 * o chreHeapAlloc() and chreHeapFree()
 * o chreGetInstanceId()
 * o chreSendEvent() [*]
 * o chreTimerSet() [*]
 * o chreSensorFindDefault() and chreSensorConfigure() [*]
 * o chreSendMessageToHost() [**]
 *
 * [*] These require nanoappHandleEvent() to be called successfully in order
 *     to confirm.
 * [**] This is confirmed by the host receiving this message.
 *
 * This isn't a "general" test, so it doesn't have a standard communication
 * protocol.  Notably, the Host doesn't send any messages to this nanoapp.
 *
 * Protocol:
 * Nanoapp to Host: kContinue
 * Nanoapp to Host: kSuccess
 */

#include <cinttypes>

#include <chre.h>

#include <shared/send_message.h>
#include <shared/time_util.h>
#include <shared/test_success_marker.h>

using nanoapp_testing::MessageType;
using nanoapp_testing::TestSuccessMarker;
using nanoapp_testing::sendMessageToHost;
using nanoapp_testing::sendFatalFailureToHost;
using nanoapp_testing::sendFatalFailureToHostUint8;
using nanoapp_testing::sendSuccessToHost;

static bool gInMethod = false;
static uint32_t gInstanceId;
static uint32_t gTimerId;
static uint32_t gSensorHandle;

/**
 * Busy startup stages and total number of stages.
 */
enum BusyStartupStage {
  BUSY_STARTUP_STAGE_SELF_EVENT = 0,
  BUSY_STARTUP_STAGE_TIMER,
  BUSY_STARTUP_STAGE_SENSOR,
  BUSY_STARTUP_STAGE_COUNT,
};

//! TestSuccessMarker object to mark success of a stage.
TestSuccessMarker gTestSuccessMarker =
    TestSuccessMarker(BUSY_STARTUP_STAGE_COUNT);

constexpr uint16_t kEventType = CHRE_EVENT_FIRST_USER_VALUE;

static void checkSelfEvent(uint16_t eventType, const uint32_t *eventData) {
  if (eventType != kEventType) {
    uint32_t e = eventType;
    sendFatalFailureToHost("Event from self, bad event type:", &e);
  }
  if (eventData == nullptr) {
    sendFatalFailureToHost("Event from self, null data");
  }
  if (*eventData != gInstanceId) {
    sendFatalFailureToHost("Event from self, bad data:", eventData);
  }
  gTestSuccessMarker.markStageAndSuccessOnFinish(BUSY_STARTUP_STAGE_SELF_EVENT);
}

static void checkTimerEvent(const uint32_t *eventData) {
  if (eventData == nullptr) {
    sendFatalFailureToHost("TimerEvent, null data");
  }
  if (*eventData != gInstanceId) {
    sendFatalFailureToHost("TimerEvent, bad data:", eventData);
  }
  gTestSuccessMarker.markStageAndSuccessOnFinish(BUSY_STARTUP_STAGE_TIMER);
}

static void checkSensorEvent(const void *eventData) {
  const chreSensorDataHeader* header =
      static_cast<const chreSensorDataHeader *>(eventData);
  if (header == nullptr) {
    sendFatalFailureToHost("sensorEvent, null data");
  }
  if (header->sensorHandle != gSensorHandle) {
    sendFatalFailureToHost("sensorEvent for wrong handle",
                           &header->sensorHandle);
  }
  if (header->readingCount == 0) {
    sendFatalFailureToHost("sensorEvent has readingCount of 0");
  }
  if (header->reserved != 0) {
    sendFatalFailureToHost("sensorEvent has non-zero reserved field");
  }

  if (chreGetApiVersion() < CHRE_API_VERSION_1_3) {
    if (header->accuracy != 0) {
      sendFatalFailureToHost("sensorEvent has non-zero reserved field");
    }
  } else if (header->accuracy > CHRE_SENSOR_ACCURACY_HIGH) {
    sendFatalFailureToHostUint8("Sensor accuracy is not within valid range: ",
                                header->accuracy);
  }

  gTestSuccessMarker.markStageAndSuccessOnFinish(BUSY_STARTUP_STAGE_SENSOR);
}

extern "C" void nanoappHandleEvent(uint32_t senderInstanceId,
                                   uint16_t eventType,
                                   const void* eventData) {
  if (gInMethod) {
    sendFatalFailureToHost("CHRE reentered nanoapp");
  }
  gInMethod = true;
  const uint32_t *intData = static_cast<const uint32_t *>(eventData);
  if (senderInstanceId == gInstanceId) {
    checkSelfEvent(eventType, intData);

  } else if (senderInstanceId == CHRE_INSTANCE_ID) {
    if (eventType == CHRE_EVENT_TIMER) {
      checkTimerEvent(intData);
    } else if (eventType == CHRE_EVENT_SENSOR_ACCELEROMETER_DATA) {
      checkSensorEvent(eventData);
    } else if (eventType == CHRE_EVENT_SENSOR_SAMPLING_CHANGE) {
      // This could have been generated when we configured the
      // sensor.  We just ignore it.
    } else {
      uint32_t e = eventType;
      sendFatalFailureToHost("Unexpected event from CHRE:", &e);
    }
  } else {
    sendFatalFailureToHost("Unexpected senderInstanceId",
                           &senderInstanceId);
  }
  gInMethod = false;
}

extern "C" bool nanoappStart(void) {
  gInMethod = true;
  void *ptr = chreHeapAlloc(15);
  if (ptr == nullptr) {
    // TODO(b/32326854): We're not able to send messages from
    //     nanoappStart(), so we just use chreLog() here, and make
    //     the user look through the logs to determine why this failed.
    chreLog(CHRE_LOG_ERROR, "Unable to malloc in start");
    return false;
  }
  gInstanceId = chreGetInstanceId();
  if (gInstanceId == CHRE_INSTANCE_ID) {
    chreLog(CHRE_LOG_ERROR, "Got bad instance ID in start");
    return false;
  }

  // Send an event to ourself.
  if (!chreSendEvent(kEventType, &gInstanceId, nullptr, gInstanceId)) {
    chreLog(CHRE_LOG_ERROR, "Failed chreSendEvent in start");
    return false;
  }

  // One shot timer that should trigger very quickly.
  gTimerId = chreTimerSet(1, &gInstanceId, true);
  if (gTimerId == CHRE_TIMER_INVALID) {
    chreLog(CHRE_LOG_ERROR, "Failed chreTimerSet in start");
    return false;
  }

  // We don't have a way to confirm the 'free' worked, we'll just look
  // to see that we didn't crash.  We intentionally move this 'free' to
  // be not immediately after the 'alloc', and still before we're done
  // calling other methods.
  chreHeapFree(ptr);

  // Confirm we can find and configure a sensor.
  if (!chreSensorFindDefault(CHRE_SENSOR_TYPE_ACCELEROMETER,
                             &gSensorHandle)) {
    chreLog(CHRE_LOG_ERROR, "Failed sensorFindDefault in start");
    return false;
  }

  // Configure accel request at 50 Hz (reasonable rate, e.g. for AR)
  // TODO: Add a way to find the range of possible sample rates
  if (!chreSensorConfigure(gSensorHandle,
                           CHRE_SENSOR_CONFIGURE_MODE_CONTINUOUS,
                           20 * nanoapp_testing::kOneMillisecondInNanoseconds,
                           CHRE_SENSOR_LATENCY_ASAP)) {
    chreLog(CHRE_LOG_ERROR, "Failed sensorConfigure in start");
    return false;
  }

  // TODO(b/32326854): Confirm we can send a message to the host.

  gInMethod = false;
  return true;
}

extern "C" void nanoappEnd(void) {
  if (!chreSensorConfigureModeOnly(gSensorHandle,
                                   CHRE_SENSOR_CONFIGURE_MODE_DONE)) {
    sendFatalFailureToHost("Unable to configure sensor mode to DONE");
  }

  if (gInMethod) {
    // This message won't be noticed by the host; but hopefully the
    // fatal failure prevents a clean unload of the app and fails the test.
    sendFatalFailureToHost("nanoappEnd called in reentrant manner");
  }
}
