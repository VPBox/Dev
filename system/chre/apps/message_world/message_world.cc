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

#include <chre.h>
#include <cinttypes>

#include "chre/util/nanoapp/log.h"

#define LOG_TAG "[MsgWorld]"

#ifdef CHRE_NANOAPP_INTERNAL
namespace chre {
namespace {
#endif  // CHRE_NANOAPP_INTERNAL

namespace {

constexpr uint32_t kMessageType = 1234;
uint8_t gMessageData[CHRE_MESSAGE_TO_HOST_MAX_SIZE] = {1, 2, 3, 4, 5, 6, 7, 8};

void messageFreeCallback(void *message, size_t messageSize) {
  LOGI("Got message free callback for message @"
       " %p (match? %d) size %zu (match? %d)",
       message, (message == gMessageData),
       messageSize, (messageSize == sizeof(gMessageData)));
  if (!chreSendEvent(CHRE_EVENT_FIRST_USER_VALUE, nullptr, nullptr,
                     chreGetInstanceId())) {
    LOGE("Failed to send event");
  }
}

}  // anonymous namespace

bool nanoappStart() {
  LOGI("App started as instance %" PRIu32, chreGetInstanceId());

  bool success = chreSendMessageToHostEndpoint(
      gMessageData, sizeof(gMessageData), kMessageType,
      CHRE_HOST_ENDPOINT_BROADCAST, messageFreeCallback);
  LOGI("Sent message to host from start callback, result %d", success);
  return true;
}

void nanoappHandleEvent(uint32_t senderInstanceId,
                        uint16_t eventType,
                        const void *eventData) {
  if (eventType == CHRE_EVENT_MESSAGE_FROM_HOST) {
    auto *msg = static_cast<const chreMessageFromHostData *>(eventData);
    LOGI("Got message from host with type %" PRIu32 " size %" PRIu32
         " data @ %p hostEndpoint 0x%" PRIx16,
         msg->messageType, msg->messageSize, msg->message, msg->hostEndpoint);
    if (senderInstanceId != CHRE_INSTANCE_ID) {
      LOGE("Message from host came from unexpected instance ID %" PRIu32,
           senderInstanceId);
    }

    bool success = chreSendMessageToHostEndpoint(
      gMessageData, sizeof(gMessageData), kMessageType,
      CHRE_HOST_ENDPOINT_BROADCAST, messageFreeCallback);
    LOGI("Result of sending reply: %d", success);
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

CHRE_STATIC_NANOAPP_INIT(MessageWorld, chre::kMessageWorldAppId, 0);
#endif  // CHRE_NANOAPP_INTERNAL
