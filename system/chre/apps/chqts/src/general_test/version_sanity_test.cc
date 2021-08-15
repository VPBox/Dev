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

#include <general_test/version_sanity_test.h>

#include <cstddef>

#include <shared/send_message.h>

#include <chre.h>

using nanoapp_testing::sendFatalFailureToHost;
using nanoapp_testing::sendSuccessToHost;

namespace general_test {

VersionSanityTest::VersionSanityTest()
    : Test(CHRE_API_VERSION_1_0) {
}

void VersionSanityTest::setUp(uint32_t messageSize, const void * /* message */) {
  if (messageSize != 0) {
    sendFatalFailureToHost(
        "VersionSanity message expects 0 additional bytes, got ",
        &messageSize);
  }

  if (mApiVersion < CHRE_API_VERSION_1_0) {
    sendFatalFailureToHost("API version less than 1.0", &mApiVersion);
  }
  if ((mApiVersion & UINT32_C(0xFFFF)) != 0) {
    sendFatalFailureToHost("API version has two LSB set", &mApiVersion);
  }
  uint32_t platformVersion = chreGetVersion();
  constexpr uint32_t kMajorMinorMask = UINT32_C(0xFFFF0000);
  // Both mApiVersion and platformVersion refer to what the CHRE was
  // built against, so they must agree in major and minor version.
  if ((platformVersion & kMajorMinorMask) != (mApiVersion & kMajorMinorMask)) {
    sendFatalFailureToHost("API and platform version mismatch",
                           &platformVersion);
  }

  // Confirm that our app (CHRE_API_VERSION) and CHRE (mApiVersion) were
  // both compiled against the same major version.  They're allowed to
  // differ in minor version.
  constexpr uint32_t kMajorMask = UINT32_C(0xFF000000);
  if ((mApiVersion & kMajorMask) != (CHRE_API_VERSION & kMajorMask)) {
    uint32_t appVersion = CHRE_API_VERSION;
    sendFatalFailureToHost("App built against different major version",
                           &appVersion);
  }

  uint64_t platformId = chreGetPlatformId();
  if ((platformId == UINT64_C(0)) || (platformId == UINT64_C(-1))) {
    sendFatalFailureToHost("Bogus platform ID");
  }
  // TODO(b/30077401): We should send the Platform ID back to the Host, and
  //     have the Host confirm this is the Platform ID we used for loading
  //     the nanoapp.

  sendSuccessToHost();
}

void VersionSanityTest::handleEvent(uint32_t senderInstanceId,
                                    uint16_t eventType, const void* eventData) {
  unexpectedEvent(eventType);
}

}  // namespace general_test
