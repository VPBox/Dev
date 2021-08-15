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

#include <general_test/heap_exhaustion_stability_test.h>

#include <cinttypes>
#include <cstddef>

#include <shared/send_message.h>
#include <shared/time_util.h>

#include <chre.h>

using nanoapp_testing::kOneMillisecondInNanoseconds;
using nanoapp_testing::kOneSecondInNanoseconds;
using nanoapp_testing::sendFailureToHost;
using nanoapp_testing::sendFatalFailureToHost;
using nanoapp_testing::sendSuccessToHost;

/*
 * We set an "exhaustion timer" to go off when we're ready for the test to
 * be over.  Then we exhaust the heap.
 *
 * We try a series of chre*() calls with the heap exhausted.  For many of
 * these calls, we're less interested in them succeeding than in the system
 * just not crashing.  However, for things which claim success, we require
 * they succeed.
 *
 * To track the things which claim success, we have two "stages", kTimerStage
 * and kEventStage.
 *
 * When the "exhaustion timer" fires, we free our memory, and make sure our
 * stages have all succeeded.
 */

namespace general_test {

// Note: We use pointers to the 'duration' to serve as our timer event data.
// Thus we make this "static const" instead of "constexpr", as we expect
// them to have backing memory.

static const uint64_t kExhaustionDuration = 5 * kOneSecondInNanoseconds;
static const uint64_t kShortDuration = 10 * kOneMillisecondInNanoseconds;

constexpr uint16_t kEventType = CHRE_EVENT_FIRST_USER_VALUE;

constexpr uint32_t kTimerStage = 0;
constexpr uint32_t kEventStage = 1;

void HeapExhaustionStabilityTest::exhaustHeap() {
  constexpr size_t kNumPtrs = 256;
  mExhaustionPtrs = reinterpret_cast<void**>(
      chreHeapAlloc(kNumPtrs * sizeof(*mExhaustionPtrs)));
  if (mExhaustionPtrs == nullptr) {
    // Oh, the irony.
    sendFatalFailureToHost(
        "Insufficient free heap to exhaust the heap.");
  }

  // We start by trying to allocate massive sizes (256MB to start).
  // When we're not able to allocate massive sizes, we cut the size in
  // half.  We repeat until we've either done kNumPtrs allocations,
  // or reduced our allocation size below 16 bytes.
  uint32_t allocSize = 1024 * 1024 * 256;
  for (mExhaustionPtrCount = 0;
       mExhaustionPtrCount < kNumPtrs;
       mExhaustionPtrCount++) {
    void *ptr = chreHeapAlloc(allocSize);
    while (ptr == nullptr) {
      allocSize /= 2;
      if (allocSize < 4) {
        break;
      }
      ptr = chreHeapAlloc(allocSize);
    }
    if (ptr == nullptr) {
      break;
    }
    mExhaustionPtrs[mExhaustionPtrCount] = ptr;
  }
  if (mExhaustionPtrCount == 0) {
    sendFatalFailureToHost(
        "Failed to allocate anything for heap exhaustion");
  }
}

void HeapExhaustionStabilityTest::freeMemory() {
  for (size_t i = 0; i < mExhaustionPtrCount; i++) {
    chreHeapFree(mExhaustionPtrs[i]);
  }
  chreHeapFree(mExhaustionPtrs);
}

HeapExhaustionStabilityTest::HeapExhaustionStabilityTest()
  : Test(CHRE_API_VERSION_1_0) {
}

void HeapExhaustionStabilityTest::setUp(uint32_t messageSize,
                                        const void * /* message */) {
  mInMethod = true;
  if (messageSize != 0) {
    sendFatalFailureToHost(
        "HeapExhaustionStability message expects 0 additional bytes, "
        "got ", &messageSize);
  }

  if (chreTimerSet(kExhaustionDuration, &kExhaustionDuration, true) ==
      CHRE_TIMER_INVALID) {
    sendFatalFailureToHost("Unable to set initial timer");
  }

  exhaustHeap();

  testLog(messageSize);
  testSetTimer();
  testSendEvent();
  testSensor();
  // TODO(b/32114261): This method currently doesn't test anything.
  testMessageToHost();

  // Some of the above 'test' methods might trigger events.  Even if they
  // don't, the kExhaustionDuration timer we set earlier should trigger
  // eventually, and that's when we'll conclude the test.
  mInMethod = false;
}

void HeapExhaustionStabilityTest::testLog(uint32_t zero) {
  // This doesn't need to land in the log (and indeed we have no automated
  // means of checking that right now anyway), but it shouldn't crash.
  chreLog(CHRE_LOG_INFO, "Test log %s, zero: %" PRId32, "message", zero);
}

void HeapExhaustionStabilityTest::testSetTimer() {
  if (chreTimerSet(kShortDuration, &kShortDuration, true) !=
      CHRE_TIMER_INVALID) {
    // CHRE claims we were able to set this timer.  We'll
    // mark this stage a success when the timer fires.
  } else {
    // CHRE was not able to set this timer.  That's okay, since we're
    // out of heap.  We'll mark this stage as a success.
    markSuccess(kTimerStage);
  }
}

void HeapExhaustionStabilityTest::testSendEvent() {
  if (chreSendEvent(kEventType, nullptr, nullptr, chreGetInstanceId())) {
    // CHRE claims we were able to send this event.  We'll make
    // this stage a success when the event is received.
  } else {
    // CHRE was not able to send this event.  That's okay, since we're
    // out of heap.  We'll mark this stage as a success.
    markSuccess(kEventStage);
  }
}

void HeapExhaustionStabilityTest::testSensor() {
  static constexpr uint8_t kSensorType = CHRE_SENSOR_TYPE_ACCELEROMETER;
  uint32_t handle;
  if (!chreSensorFindDefault(kSensorType, &handle)) {
    // We still expect this to succeed without any heap left.
    sendFatalFailureToHost("chreSensorFindDefault failed");
  }
  chreSensorInfo info;
  if (!chreGetSensorInfo(handle, &info)) {
    // We still expect this to succeed, since we're supplying the memory.
    sendFatalFailureToHost("chreGetSensorInfo failed");
  }
  if (info.sensorType != kSensorType) {
    sendFatalFailureToHost("Invalid sensor info provided");
  }

  chreSensorSamplingStatus samplingStatus;
  if (!chreGetSensorSamplingStatus(handle, &samplingStatus)) {
    // We still expect this to succeed, since we're supplying the memory.
    sendFatalFailureToHost("chreGetSensorSamplingStatus failed");
  }

  // TODO: We might want to consider calling chreSensorConfigure() for a
  //     more robust test of this.  However, we don't expect sensor events to
  //     necessarily get delivered under heap exhaustion, so it's unclear
  //     how we'd make sure we eventually tell the system we're DONE with
  //     the sensor (setting a timer isn't assured to work at this point).
}

void HeapExhaustionStabilityTest::testMessageToHost() {
  // TODO(b/32114261): We should invoke sendMessageToHost() here.
  //     Unfortunately, this is a real pain due to this bug, as we need to
  //     duplicate much of the contents of shared/send_message.cc to
  //     add the hack-around bytes (the method itself will internally
  //     fail if the send attempt fails, but we're in a state where
  //     we'll allow a failed send attempt).  Or we need to take this
  //     off of the General test infrastructure to allow raw byte sending.
  //     That seems not worth the effort for NYC, and just easier to wait
  //     until OMC when this is much easier to implement.
  // OMC Note: When we've fixed this bug, and added a send here, we'll
  //     need to make this no longer Simple protocol, since this nanoapp
  //     might send a message.
}

void HeapExhaustionStabilityTest::handleEvent(uint32_t senderInstanceId,
                                              uint16_t eventType,
                                              const void* eventData) {
  if (mInMethod) {
    sendFatalFailureToHost("handleEvent invoked while another nanoapp "
                           "method is running");
  }
  mInMethod = true;

  if (eventType == CHRE_EVENT_TIMER) {
    handleTimer(senderInstanceId, eventData);
  } else if (eventType == kEventType) {
    handleSelfEvent(senderInstanceId, eventData);
  } else {
    unexpectedEvent(eventType);
  }
  mInMethod = false;
}

void HeapExhaustionStabilityTest::handleTimer(uint32_t senderInstanceId,
                                              const void *eventData) {
  if (senderInstanceId != CHRE_INSTANCE_ID) {
    sendFatalFailureToHost("handleTimer with unexpected sender:",
                           &senderInstanceId);
  }
  if (eventData == &kShortDuration) {
    // This was the timer we triggered while the heap was exhausted.
    markSuccess(kTimerStage);

  } else if (eventData == &kExhaustionDuration) {
    // Our test is done.
    freeMemory();
    if (mFinishedBitmask != kAllFinished) {
      sendFatalFailureToHost("Done with test, but not all stages "
                             "done.", &mFinishedBitmask);
    }
    sendSuccessToHost();

  } else {
    sendFatalFailureToHost("Unexpected timer eventData");
  }
}

void HeapExhaustionStabilityTest::handleSelfEvent(uint32_t senderInstanceId,
                                                  const void *eventData) {
  if (senderInstanceId != chreGetInstanceId()) {
    sendFatalFailureToHost("handleSelfEvent with unexpected sender:",
                           &senderInstanceId);
  }
  if (eventData != nullptr) {
    sendFatalFailureToHost("Unexpected data for event to self");
  }
  markSuccess(kEventStage);
}

void HeapExhaustionStabilityTest::markSuccess(uint32_t stage) {
  chreLog(CHRE_LOG_DEBUG, "Stage %" PRIu32 " succeeded", stage);
  uint32_t finishedBit = (1 << stage);
  if ((kAllFinished & finishedBit) == 0) {
    sendFatalFailureToHost("markSuccess bad stage", &stage);
  }
  if ((mFinishedBitmask & finishedBit) != 0) {
    // This could be when a timer/event method returned 'false', but
    // actually did end up triggering an event.
    sendFatalFailureToHost("markSuccess stage triggered twice", &stage);
  }
  mFinishedBitmask |= finishedBit;
  // Note that unlike many markSuccess() implementations, we do not
  // check against kAllFinished here.  That happens when the
  // timer for kExhaustionDuration fires.
}


}  // namespace general_test
