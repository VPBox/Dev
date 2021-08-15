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

#include <cstddef>  // max_align_t
#include <cstdint>
#include <new>  // placement new

#include <chre.h>

#include <general_test/basic_audio_test.h>
#include <general_test/basic_gnss_test.h>
#include <general_test/basic_sensor_tests.h>
#include <general_test/basic_wifi_test.h>
#include <general_test/estimated_host_time_test.h>
#include <general_test/event_between_apps_test.h>
#include <general_test/get_time_test.h>
#include <general_test/gnss_capabilities_test.h>
#include <general_test/heap_alloc_stress_test.h>
#include <general_test/heap_exhaustion_stability_test.h>
#include <general_test/hello_world_test.h>
#include <general_test/host_awake_suspend_test.h>
#include <general_test/logging_sanity_test.h>
#include <general_test/nanoapp_info_by_app_id_test.h>
#include <general_test/nanoapp_info_by_instance_id_test.h>
#include <general_test/nanoapp_info_events_test_observer.h>
#include <general_test/nanoapp_info_events_test_performer.h>
#include <general_test/send_event_test.h>
#include <general_test/send_event_stress_test.h>
#include <general_test/send_message_to_host_test.h>
#include <general_test/sensor_info_test.h>
#include <general_test/simple_heap_alloc_test.h>
#include <general_test/test.h>
#include <general_test/test_names.h>
#include <general_test/timer_cancel_test.h>
#include <general_test/timer_set_test.h>
#include <general_test/timer_stress_test.h>
#include <general_test/version_sanity_test.h>
#include <general_test/wifi_capabilities_test.h>
#include <general_test/wwan_capabilities_test.h>
#include <general_test/wwan_cell_info_test.h>
#include <shared/abort.h>
#include <shared/nano_endian.h>
#include <shared/nano_string.h>
#include <shared/send_message.h>

using nanoapp_testing::AbortBlame;
using nanoapp_testing::MessageType;
using nanoapp_testing::sendFatalFailureToHost;
using nanoapp_testing::sendInternalFailureToHost;


namespace general_test {

// The size of this array is checked at compile time by the static_assert
// in getNew().
alignas(alignof(max_align_t)) static uint8_t gGetNewBackingMemory[128];

template<typename N>
static N *getNew() {
  // We intentionally avoid using chreHeapAlloc() to reduce dependencies
  // for our tests, especially things like HelloWorld.  This obviously
  // cannot be called more than once, but our usage doesn't require it.
  static_assert(sizeof(gGetNewBackingMemory) >= sizeof(N),
                "getNew() backing memory is undersized");

  return new(gGetNewBackingMemory) N();
}

// TODO(b/32114261): Remove this variable.
bool gUseNycMessageHack = true;

class App {
 public:
  App() : mConstructionCookie(kConstructed),
          mCurrentTest(nullptr) {}

  ~App() {
    // Yes, it's very odd to actively set a value in our destructor.
    // However, since we're making a static instance of this class,
    // the space for this class will stick around (unlike heap memory
    // which might get reused), so we can still use this to perform
    // some testing.
    mConstructionCookie = kDestructed;
  }

  bool wasConstructed() const { return mConstructionCookie == kConstructed; }
  bool wasDestructed() const { return mConstructionCookie == kDestructed; }

  void handleEvent(uint32_t senderInstanceId, uint16_t eventType,
                   const void *eventData);

  void createTest(const void *eventData);
  void freeTest();

 private:
  uint32_t mConstructionCookie;
  Test *mCurrentTest;

  static constexpr uint32_t kConstructed = UINT32_C(0x51501984);
  static constexpr uint32_t kDestructed = UINT32_C(0x19845150);

  // TODO(b/32114261): Remove this method.
  chreMessageFromHostData
      adjustHostMessageForNYC(const chreMessageFromHostData *data);
};


// In the NYC version of the CHRE, the "reservedMessageType" isn't
// assured to be sent correctly from the host.  But we want our
// tests to be written using this field (it's cleaner).  So in NYC
// the host prefixes this value in the first four bytes of 'message',
// and here we reconstruct the message to be correct.
// TODO(b/32114261): Remove this method.
chreMessageFromHostData
App::adjustHostMessageForNYC(const chreMessageFromHostData *data) {
  if (!gUseNycMessageHack) {
    return *data;
  }
  chreMessageFromHostData ret;

  if (data->messageSize < sizeof(uint32_t)) {
    sendFatalFailureToHost("Undersized message in adjustHostMessageForNYC");
  }
  const uint8_t *messageBytes = static_cast<const uint8_t*>(data->message);
  nanoapp_testing::memcpy(&(ret.reservedMessageType), messageBytes,
                          sizeof(ret.reservedMessageType));
  ret.reservedMessageType =
      nanoapp_testing::littleEndianToHost(ret.reservedMessageType);
  ret.messageSize = data->messageSize - sizeof(ret.reservedMessageType);
  ret.message = messageBytes + sizeof(ret.reservedMessageType);
  return ret;
}


void App::handleEvent(uint32_t senderInstanceId, uint16_t eventType,
                      const void* eventData) {
  // TODO: When we get an API that fixes the reservedMessageType,
  //       then we should only use our adjustedData hack on APIs
  //       prior to it being fixed.  Eventually, we should remove
  //       this altogether.
  chreMessageFromHostData adjustedData;
  if (eventType == CHRE_EVENT_MESSAGE_FROM_HOST) {
    auto data = static_cast<const chreMessageFromHostData *>(eventData);
    adjustedData = adjustHostMessageForNYC(data);
    eventData = &adjustedData;
  }

  if (mCurrentTest != nullptr) {
    // Our test is in progress, so let it take control.
    mCurrentTest->testHandleEvent(senderInstanceId, eventType, eventData);
    return;
  }

  // No test in progress, so we expect this message to be the host telling
  // us which test to run.  We fail if it's anything else.
  if (eventType != CHRE_EVENT_MESSAGE_FROM_HOST) {
    uint32_t localEventType = eventType;
    sendFatalFailureToHost(
        "Unexpected event type with no established test:",
        &localEventType);
  }
  if (senderInstanceId != CHRE_INSTANCE_ID) {
    sendFatalFailureToHost(
        "Got MESSAGE_FROM_HOST not from CHRE_INSTANCE_ID:",
        &senderInstanceId);
  }
  createTest(eventData);
}

void App::createTest(const void *eventData) {
  if (mCurrentTest != nullptr) {
    sendInternalFailureToHost(
        "Got to createTest() with non-null mCurrentTest");
  }

  auto data = static_cast<const chreMessageFromHostData *>(eventData);
  switch (static_cast<TestNames>(data->reservedMessageType)) {
    using namespace general_test;

#define CASE(testName, className) \
    case TestNames::testName: \
         mCurrentTest = getNew<className>(); \
         break;

    CASE(kHelloWorld, HelloWorldTest);
    CASE(kSimpleHeapAlloc, SimpleHeapAllocTest);
    CASE(kHeapAllocStress, HeapAllocStressTest);
    CASE(kGetTime, GetTimeTest);
    CASE(kEventBetweenApps0, EventBetweenApps0);
    CASE(kEventBetweenApps1, EventBetweenApps1);
    CASE(kSendEvent, SendEventTest);
    CASE(kBasicAccelerometer, BasicAccelerometerTest);
    CASE(kBasicInstantMotionDetect, BasicInstantMotionDetectTest);
    CASE(kBasicStationaryDetect, BasicStationaryDetectTest);
    CASE(kBasicGyroscope, BasicGyroscopeTest);
    CASE(kBasicMagnetometer, BasicMagnetometerTest);
    CASE(kBasicBarometer, BasicBarometerTest);
    CASE(kBasicLightSensor, BasicLightSensorTest);
    CASE(kBasicProximity, BasicProximityTest);
    CASE(kVersionSanity, VersionSanityTest);
    CASE(kLoggingSanity, LoggingSanityTest);
    CASE(kSendMessageToHost, SendMessageToHostTest);
    CASE(kTimerSet, TimerSetTest);
    CASE(kTimerCancel, TimerCancelTest);
    CASE(kTimerStress, TimerStressTest);
    CASE(kSendEventStress, SendEventStressTest);
    CASE(kHeapExhaustionStability, HeapExhaustionStabilityTest);
    CASE(kGnssCapabilities, GnssCapabilitiesTest);
    CASE(kWifiCapabilities, WifiCapabilitiesTest);
    CASE(kWwanCapabilities, WwanCapabilitiesTest);
    CASE(kSensorInfo, SensorInfoTest);
    CASE(kWwanCellInfoTest, WwanCellInfoTest);
    CASE(kEstimatedHostTime, EstimatedHostTimeTest);
    CASE(kNanoappInfoByAppId, NanoappInfoByAppIdTest);
    CASE(kNanoappInfoByInstanceId, NanoappInfoByInstanceIdTest);
    CASE(kNanoAppInfoEventsPerformer, NanoAppInfoEventsTestPerformer);
    CASE(kNanoAppInfoEventsObserver, NanoAppInfoEventsTestObserver);
    CASE(kBasicAudioTest, BasicAudioTest);
    CASE(kHostAwakeSuspend, HostAwakeSuspendTest);
    CASE(kBasicGnssTest, BasicGnssTest);
    CASE(kBasicWifiTest, BasicWifiTest);

#undef CASE

    default:
    sendFatalFailureToHost("Unexpected message type:",
                           &(data->reservedMessageType));
  }

  if (mCurrentTest != nullptr) {
    mCurrentTest->testSetUp(data->messageSize, data->message);
  } else {
    sendInternalFailureToHost("createTest() ended with null mCurrentTest");
  }
}

void App::freeTest() {
  if (mCurrentTest == nullptr) {
    sendInternalFailureToHost("Nanoapp unloading without running any test");
  }
  mCurrentTest->~Test();
}

}  // namespace general_test

static general_test::App gApp;


extern "C" void nanoappHandleEvent(uint32_t senderInstanceId,
                                   uint16_t eventType,
                                   const void* eventData) {
  gApp.handleEvent(senderInstanceId, eventType, eventData);
}

static uint32_t zeroedBytes[13];

extern "C" bool nanoappStart(void) {
  // zeroedBytes is in the BSS and needs to be zero'd out.
  for (size_t i = 0; i < sizeof(zeroedBytes)/sizeof(zeroedBytes[0]); i++) {
    if (zeroedBytes[i] != 0) {
      return false;
    }
  }

  // A CHRE is required to call the constructor of our class prior to
  // reaching this point.
  return gApp.wasConstructed();
}

extern "C" void nanoappEnd(void) {
  if (gApp.wasDestructed()) {
    // It's not legal for us to send a message from here.  The best
    // we can do is abort, although there's no means for the end user
    // to see such a failure.
    // TODO: Figure out how to have this failure noticed.
    nanoapp_testing::abort(AbortBlame::kChreInNanoappEnd);
  }
  gApp.freeTest();

  // TODO: Unclear how we can test the global destructor being called,
  //     but that would be good to test.  Since it's supposed to happen
  //     after this call completes, it's difficult to test.
}
