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
#include "chre/core/event_loop.h"
#include "chre/core/event_loop_manager.h"
#include "chre/platform/assert.h"
#include "chre/platform/log.h"
#include "chre/platform/static_nanoapp_init.h"
#include "chre/util/nanoapp/app_id.h"
#include "chre/util/time.h"

/**
 * @file
 * A nanoapp exclusively for testing, which unloads the spammer nanoapp after a
 * short delay. Must only be compiled as a static/internal nanoapp.
 */

namespace chre {
namespace {

constexpr uint32_t kAppVersion = 99;

void handleUnload(uint16_t /* eventType */, void * /* data */) {
  EventLoop& eventLoop = EventLoopManagerSingleton::get()->getEventLoop();
  uint32_t instanceId;

  LOGD("About to unload spammer nanoapp");
  if (!eventLoop.findNanoappInstanceIdByAppId(kSpammerAppId, &instanceId)) {
    LOGE("Couldn't unload nanoapp: not found");
  } else if (!eventLoop.unloadNanoapp(instanceId, true)) {
    LOGE("Failed to unload nanoapp");
  }
}

bool nanoappStart() {
  LOGI("Unload tester started as instance %" PRIu32, chreGetInstanceId());

  constexpr uint64_t kTimerDuration = Seconds(2).toRawNanoseconds();
  uint32_t timerHandle = chreTimerSet(kTimerDuration,
      nullptr, true /* oneShot */);
  CHRE_ASSERT_LOG(timerHandle != CHRE_TIMER_INVALID, "Couldn't start timer!");

  bool eventSent = chreSendEvent(CHRE_EVENT_FIRST_USER_VALUE, nullptr, nullptr,
                                 chreGetInstanceId());
  CHRE_ASSERT_LOG(eventSent, "Couldn't send event to self!");

  struct chreNanoappInfo info;
  bool gotInfo = chreGetNanoappInfoByInstanceId(chreGetInstanceId(), &info);
  CHRE_ASSERT(gotInfo);
  CHRE_ASSERT(info.appId == chreGetAppId());
  CHRE_ASSERT(info.appId == kUnloadTesterAppId);
  CHRE_ASSERT(info.version == kAppVersion);
  CHRE_ASSERT(info.instanceId == chreGetInstanceId());

  chreConfigureNanoappInfoEvents(true);
  return true;
}

void nanoappHandleEvent(uint32_t senderInstanceId,
                        uint16_t eventType,
                        const void *eventData) {
  if (eventType == CHRE_EVENT_FIRST_USER_VALUE
      && senderInstanceId == chreGetInstanceId()) {
    struct chreNanoappInfo info;
    if (!chreGetNanoappInfoByAppId(kSpammerAppId, &info)) {
      LOGW("Couldn't get spammer's app info - not running?");
    }
  } else if (eventType == CHRE_EVENT_TIMER) {
    // We can't do the unload from the context of another nanoapp's handle
    // event callback, so get into the system context
    EventLoopManagerSingleton::get()->deferCallback(
        SystemCallbackType::HandleUnloadNanoapp, nullptr, handleUnload);
  } else if (eventType == CHRE_EVENT_NANOAPP_STARTED
      || eventType == CHRE_EVENT_NANOAPP_STOPPED) {
    const auto *info = static_cast<const chreNanoappInfo *>(eventData);
    if (info->appId == kSpammerAppId) {
      LOGD("Received %s event for spammer instance %" PRIu32,
           (eventType == CHRE_EVENT_NANOAPP_STARTED) ? "start" : "stop",
           info->instanceId);
    }
  }
}

void nanoappEnd() {}

}  // anonymous namespace
}  // namespace chre

CHRE_STATIC_NANOAPP_INIT(UnloadTester, chre::kUnloadTesterAppId, kAppVersion);
