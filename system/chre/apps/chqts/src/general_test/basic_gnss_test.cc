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

#include <general_test/basic_gnss_test.h>

#include <chre.h>
#include <shared/send_message.h>

/*
 * Test to check expected functionality of the CHRE GNSS APIs.
 * TODO: Currently the test only exists to verify that expected APIs are
 *       implemented and doesn't fail. Make the test more comprehensive by
 *       validating callback results, etc.
 */
namespace general_test {

namespace {

void testLocationSessionAsync() {
  if (!chreGnssLocationSessionStartAsync(
         1000 /* minIntervalMs */, 0 /* minTimeToNextFixMs */,
         nullptr /* cookie */)) {
    nanoapp_testing::sendFatalFailureToHost(
        "Failed to start a location session");
  }
  if (!chreGnssLocationSessionStopAsync(nullptr /* cookie */)) {
    nanoapp_testing::sendFatalFailureToHost(
        "Failed to stop a location session");
  }
}

} // anonymous namespace

BasicGnssTest::BasicGnssTest()
    : Test(CHRE_API_VERSION_1_1) {
}

void BasicGnssTest::setUp(
    uint32_t messageSize, const void * /* message */) {
  if (messageSize != 0) {
    nanoapp_testing::sendFatalFailureToHost(
        "Expected 0 byte message, got more bytes:", &messageSize);
  } else {
    uint32_t capabilities = chreGnssGetCapabilities();

    if (capabilities & CHRE_GNSS_CAPABILITIES_LOCATION) {
      testLocationSessionAsync();
      // TODO: Add tests for chreGnssMeasurementXxx and
      //       chreGnssConfigurePassiveLocationListener
    }

    nanoapp_testing::sendSuccessToHost();
  }
}

void BasicGnssTest::handleEvent(uint32_t /* senderInstanceId */,
                                uint16_t eventType,
                                const void * /* eventData */) {
  if (eventType != CHRE_EVENT_GNSS_ASYNC_RESULT
      && eventType != CHRE_EVENT_GNSS_LOCATION) {
    unexpectedEvent(eventType);
  }
}

} // namespace general_test
