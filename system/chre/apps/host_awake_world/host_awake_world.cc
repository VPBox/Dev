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

#include <chre.h>

#include "chre/util/nanoapp/log.h"

#define LOG_TAG "[HostAwakeWorld]"

/**
 * A nanoapp that sets a periodic timer to send broadcast messages to the host
 * only if it is awake.
 */

#ifdef CHRE_NANOAPP_INTERNAL
namespace chre {
namespace {
#endif  // CHRE_NANOAPP_INTERNAL

uint8_t gMessageBuffer[] = {0, 1, 2, 3, 4};

uint32_t gMessageTimerHandle;

bool nanoappStart() {
  LOGI("Host awake world start");
  gMessageTimerHandle = chreTimerSet(5000000 /* 5 ms */,
                                     &gMessageTimerHandle,
                                     false /* oneShot */);
  chreConfigureHostSleepStateEvents(true /* enable */);

  return (gMessageTimerHandle != CHRE_TIMER_INVALID);
}

void handleTimerEvent(const void *eventData) {
  const uint32_t *timerHandle = static_cast<const uint32_t *>(eventData);
  if (*timerHandle == gMessageTimerHandle) {
    if (chreIsHostAwake()) {
      chreSendMessageToHostEndpoint(
          gMessageBuffer, sizeof(gMessageBuffer), 1234 /* messageType */,
          CHRE_HOST_ENDPOINT_BROADCAST, nullptr /* freeCallback */);
    }
  } else {
    LOGE("Received unexpected timer event");
  }
}

void nanoappHandleEvent(uint32_t senderInstanceId,
                        uint16_t eventType,
                        const void *eventData) {
  switch (eventType) {
    case CHRE_EVENT_TIMER:
      handleTimerEvent(eventData);
      break;
    case CHRE_EVENT_HOST_AWAKE:
      LOGI("Received host awake event");
      break;
    case CHRE_EVENT_HOST_ASLEEP:
      LOGI("Received host asleep event");
      break;
    default:
      LOGW("Unknown event received");
      break;
  }
}

void nanoappEnd() {
  LOGI("Host awake world end");
}

#ifdef CHRE_NANOAPP_INTERNAL
}  // anonymous namespace
}  // namespace chre

#include "chre/util/nanoapp/app_id.h"
#include "chre/platform/static_nanoapp_init.h"

CHRE_STATIC_NANOAPP_INIT(HostAwakeWorld, chre::kHostAwakeWorldAppId, 0);
#endif  // CHRE_NANOAPP_INTERNAL
