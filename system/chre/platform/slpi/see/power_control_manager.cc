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

#include "chre/platform/power_control_manager.h"

#include "chre/platform/slpi/power_control_util.h"
#include "chre/platform/slpi/see/island_vote_client.h"
#include "chre/platform/system_time.h"
#include "chre/util/lock_guard.h"

namespace chre {

PowerControlManagerBase::PowerControlManagerBase() {
#ifdef CHRE_THREAD_UTIL_ENABLED
  sns_client_create_thread_utilization_client(&mThreadUtilClient);
#endif  // CHRE_THREAD_UTIL_ENABLED
}

PowerControlManagerBase::~PowerControlManagerBase() {
#ifdef CHRE_THREAD_UTIL_ENABLED
  sns_client_remove_thread_utilization_client(mThreadUtilClient);
#endif  // CHRE_THREAD_UTIL_ENABLED
}

bool PowerControlManagerBase::voteBigImage(bool bigImage) {
  return IslandVoteClientSingleton::get()->voteBigImage(bigImage);
}

void PowerControlManagerBase::onHostWakeSuspendEvent(bool awake) {
  if (mHostIsAwake != awake) {
    mHostIsAwake = awake;

    EventLoopManagerSingleton::get()->getEventLoop().postEventOrDie(
        mHostIsAwake ? CHRE_EVENT_HOST_AWAKE : CHRE_EVENT_HOST_ASLEEP,
        nullptr /* eventData */, nullptr /* freeCallback */);

#ifdef CHRE_AUDIO_SUPPORT_ENABLED
    if (awake) {
      auto callback = [](uint16_t /* eventType */, void * /* eventData*/) {
        EventLoopManagerSingleton::get()->getAudioRequestManager()
            .getPlatformAudio().onHostAwake();
      };

      EventLoopManagerSingleton::get()->deferCallback(
          SystemCallbackType::AudioHandleHostAwake, nullptr, callback);
    }
#endif  // CHRE_AUDIO_SUPPORT_ENABLED
  }
}

void PowerControlManager::postEventLoopProcess(size_t numPendingEvents) {
#ifdef CHRE_THREAD_UTIL_ENABLED
  // Although this execution point does not actually represent the start
  // of the CHRE thread's activity, we only care about cases where the
  // CHRE's event queue is highly backlogged for voting higher clock rates.
  if (mIsThreadIdle && numPendingEvents != 0) {
    sns_client_thread_utilization_start(mThreadUtilClient);
    mIsThreadIdle = false;
  } else if (!mIsThreadIdle) {
    // Update the time profile as frequently as possible so that clock updates
    // are not deferred until all events are processed.
    sns_client_thread_utilization_stop(mThreadUtilClient);
    if (numPendingEvents != 0) {
      sns_client_thread_utilization_start(mThreadUtilClient);
    } else {
      mIsThreadIdle = true;
    }
  }
#endif  // CHRE_THREAD_UTIL_ENABLED

  if (numPendingEvents == 0 && !slpiInUImage()) {
    voteBigImage(false /* bigImage */);
  }
}

bool PowerControlManager::hostIsAwake() {
  return mHostIsAwake;
}

} // namespace chre
