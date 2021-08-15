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

/**
 * A nanoapp that will provide the host endpoint ID of a pinging client.
 *
 * When a message is received, this nanoapp will send back a message indicating
 * the client's host endpoint ID.
 */

#include <cinttypes>
#include <cstdint>
#include <cstring>

#include <chre.h>
#include <shared/send_message.h>

using nanoapp_testing::sendFatalFailureToHost;

namespace chre {
namespace {

//! A buffer to store the host endpoint ID of a messaging client.
uint8_t messageBuffer[2];

} // anonymous namespace

extern "C" void nanoappHandleEvent(uint32_t senderInstanceId,
                                   uint16_t eventType,
                                   const void* eventData) {
  if (eventType == CHRE_EVENT_MESSAGE_FROM_HOST) {
    auto *msg = static_cast<const chreMessageFromHostData *>(eventData);

    if (senderInstanceId != CHRE_INSTANCE_ID) {
      sendFatalFailureToHost("Invalid sender instance ID:", &senderInstanceId);
    }

    messageBuffer[0] = (msg->hostEndpoint & 0xff00) >> 8;
    messageBuffer[1] = (msg->hostEndpoint & 0xff);
    if (!chreSendMessageToHostEndpoint(
            static_cast<void*>(messageBuffer), sizeof(messageBuffer),
            msg->messageType, msg->hostEndpoint,
            nullptr /* messageFreeCallback */)) {
      sendFatalFailureToHost("Failed to send message to host");
    }
  }
}

extern "C" bool nanoappStart(void) {
  return true;
}

extern "C" void nanoappEnd(void) {
}

}  // namespace chre
