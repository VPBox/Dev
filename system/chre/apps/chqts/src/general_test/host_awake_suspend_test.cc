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

#include <general_test/host_awake_suspend_test.h>

#include <chre.h>
#include <shared/send_message.h>

/*
 * Test to check expected functionality of host awake/suspend APIs.
 */
namespace general_test {

HostAwakeSuspendTest::HostAwakeSuspendTest()
    : Test(CHRE_API_VERSION_1_2) {
}

void HostAwakeSuspendTest::setUp(
    uint32_t messageSize, const void * /* message */) {
  if (messageSize != 0) {
    nanoapp_testing::sendFatalFailureToHost(
        "Expected 0 byte message, got more bytes:", &messageSize);
  } else {
    // Invoke to verify that this API is implemented
    chreConfigureHostSleepStateEvents(true /* enable */);

    // Assume that GTS tests are performed under USB connection,
    // in which the host should be awake.
    if (!chreIsHostAwake()) {
      nanoapp_testing::sendFatalFailureToHost(
          "Host must be awake during testing");
    }

    nanoapp_testing::sendSuccessToHost();
  }
}

void HostAwakeSuspendTest::handleEvent(uint32_t /* senderInstanceId */,
                                       uint16_t eventType,
                                       const void * /* eventData */) {
  unexpectedEvent(eventType);
}

} // namespace general_test
