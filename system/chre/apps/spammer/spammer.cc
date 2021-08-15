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

#include <cinttypes>

#include "chre_api/chre.h"
#include "chre/platform/assert.h"
#include "chre/platform/log.h"
#include "chre/platform/static_nanoapp_init.h"
#include "chre/util/nanoapp/app_id.h"

/**
 * @file
 * A nanoapp exclusively for testing, which sends a constant stream of messages
 * and events. Must only be compiled as a static/internal nanoapp.
 */

namespace chre {
namespace {

constexpr uint32_t kMessageType = 1234;
uint8_t gMessageData[] = {1, 2, 3, 4, 5, 6, 7, 8};
bool gRunning = false;
uint64_t gTotalEventCount = 0;

constexpr uint16_t kMessageSentEvent = CHRE_EVENT_FIRST_USER_VALUE;
constexpr uint16_t kRepeatedEvent = kMessageSentEvent + 1;
constexpr uint16_t kEmptyEvent = kRepeatedEvent + 2;

void *kMessageSentEventData = reinterpret_cast<void *>(0x100);
void *kRepeatedEventData = reinterpret_cast<void *>(0x200);

void messageSentEventFreeCallback(uint16_t eventType, void *data) {
  CHRE_ASSERT(gRunning);
  CHRE_ASSERT(eventType == kMessageSentEvent);
  CHRE_ASSERT(data == kMessageSentEventData);
}

void messageFreeCallback(void *message, size_t messageSize) {
  CHRE_ASSERT(gRunning);
  if (!chreSendEvent(kMessageSentEvent, kMessageSentEventData,
                     messageSentEventFreeCallback, chreGetInstanceId())) {
    LOGE("Couldn't send event from message free callback");
  }
}

void repeatedEventFreeCallback(uint16_t eventType, void *data) {
  CHRE_ASSERT(gRunning);
  CHRE_ASSERT(eventType == kRepeatedEvent);
  CHRE_ASSERT(data == kRepeatedEventData);
}

bool nanoappStart() {
  gRunning = true;
  LOGI("Spammer started as instance %" PRIu32, chreGetInstanceId());

  if (!chreSendMessageToHostEndpoint(
          gMessageData, sizeof(gMessageData), kMessageType,
          CHRE_HOST_ENDPOINT_BROADCAST, messageFreeCallback)) {
    LOGE("Couldn't send message from start callback");
  }

  if (!chreSendEvent(kRepeatedEvent, kRepeatedEventData,
                     repeatedEventFreeCallback, chreGetInstanceId())) {
    LOGE("Couldn't send first repeated event");
  }
  if (!chreSendEvent(kEmptyEvent, nullptr, nullptr, chreGetInstanceId())) {
    LOGE("Couldn't send first empty event");
  }

  return true;
}

void nanoappHandleEvent(uint32_t senderInstanceId,
                        uint16_t eventType,
                        const void *eventData) {
  CHRE_ASSERT(gRunning);
  gTotalEventCount++;
  if (eventType == kMessageSentEvent) {
    CHRE_ASSERT(eventData == kMessageSentEventData);
    if (!chreSendMessageToHostEndpoint(
            gMessageData, sizeof(gMessageData), kMessageType,
            CHRE_HOST_ENDPOINT_BROADCAST, messageFreeCallback)) {
      // Note: commented out to prevent logspam because Linux simulator
      // currently always returns false
      //LOGE("Couldn't send message");
    }
  } else if (eventType == kRepeatedEvent) {
    CHRE_ASSERT(eventData == kRepeatedEventData);
    if (!chreSendEvent(kRepeatedEvent, kRepeatedEventData,
                       repeatedEventFreeCallback, chreGetInstanceId())) {
      LOGE("Couldn't send repeated event");
    }
  } else if (eventType == kEmptyEvent) {
    CHRE_ASSERT(eventData == nullptr);
    if (!chreSendEvent(kEmptyEvent, nullptr, nullptr, chreGetInstanceId())) {
      LOGE("Couldn't send empty event");
    }
  }
}

void nanoappEnd() {
  LOGI("Stopped - handled %" PRIu64 " total events", gTotalEventCount);
  gRunning = false;
}

}  // anonymous namespace
}  // namespace chre

CHRE_STATIC_NANOAPP_INIT(Spammer, chre::kSpammerAppId, 0);
