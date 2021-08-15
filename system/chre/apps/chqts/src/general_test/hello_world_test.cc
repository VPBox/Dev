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

#include <general_test/hello_world_test.h>

#include <shared/send_message.h>

using nanoapp_testing::sendFatalFailureToHost;
using nanoapp_testing::sendSuccessToHost;

namespace general_test {

HelloWorldTest::HelloWorldTest()
    : Test(CHRE_API_VERSION_1_0) {
}

void HelloWorldTest::setUp(uint32_t messageSize, const void * /* message */) {
  if (messageSize != 0) {
    sendFatalFailureToHost("Expected 0 byte message, got more bytes:",
                           &messageSize);
  } else {
    sendSuccessToHost();
  }
}

void HelloWorldTest::handleEvent(uint32_t /* senderInstanceId */,
                                 uint16_t eventType,
                                 const void* /* eventData */) {
  unexpectedEvent(eventType);
}

}  // namespace general_test
