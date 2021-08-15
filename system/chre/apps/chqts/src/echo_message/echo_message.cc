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

/**
 * A simple nanoapp to echoes a message from the host.
 *
 * This nanoapp will send received messages back to the host endpoint with the
 * same message contents.
 */

#include <cinttypes>
#include <cstdint>
#include <cstring>

#include <chre.h>
#include <shared/nano_string.h>
#include <shared/send_message.h>

namespace chre {
namespace {

using nanoapp_testing::sendFatalFailureToHost;

void messageFreeCallback(void *message, size_t size) {
  chreHeapFree(message);
}

extern "C" void nanoappHandleEvent(uint32_t senderInstanceId,
                                   uint16_t eventType,
                                   const void* eventData) {
  if (eventType == CHRE_EVENT_MESSAGE_FROM_HOST) {
    auto *msg = static_cast<const chreMessageFromHostData *>(eventData);

    if (senderInstanceId != CHRE_INSTANCE_ID) {
      sendFatalFailureToHost("Invalid sender instance ID:", &senderInstanceId);
    }

    uint8_t *messageBuffer =
        static_cast<uint8_t*>(chreHeapAlloc(msg->messageSize));
    if (msg->messageSize != 0 && messageBuffer == nullptr) {
      sendFatalFailureToHost("Failed to allocate memory for message buffer");
    }

    std::memcpy(static_cast<void*>(messageBuffer), const_cast<void*>(msg->message),
                msg->messageSize);

    if (!chreSendMessageToHostEndpoint(
            static_cast<void*>(messageBuffer), msg->messageSize,
            msg->messageType, msg->hostEndpoint, messageFreeCallback)) {
      sendFatalFailureToHost("Failed to send message to host");
    }
  }
}

extern "C" bool nanoappStart(void) {
  return true;
}

extern "C" void nanoappEnd(void) {
}

}  // anonymous namespace
}  // namespace chre
