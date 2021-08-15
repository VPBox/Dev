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

#include "chre_api/chre/audio.h"

#include "chre/core/event_loop_manager.h"
#include "chre/util/macros.h"

#ifdef CHRE_AUDIO_SUPPORT_ENABLED
#include "chre/platform/platform_audio.h"
#endif  // CHRE_AUDIO_SUPPORT_ENABLED

using chre::EventLoopManager;
using chre::EventLoopManagerSingleton;
using chre::Nanoapp;

DLL_EXPORT bool chreAudioGetSource(uint32_t handle,
                                   struct chreAudioSource *audioSource) {
#ifdef CHRE_AUDIO_SUPPORT_ENABLED
  bool success = false;
  if (audioSource != nullptr) {
    success = EventLoopManagerSingleton::get()->getAudioRequestManager()
        .getAudioSource(handle, audioSource);
  }

  return success;
#else
  return false;
#endif  // CHRE_AUDIO_SUPPORT_ENABLED
}

DLL_EXPORT bool chreAudioConfigureSource(uint32_t handle, bool enable,
                                         uint64_t bufferDuration,
                                         uint64_t deliveryInterval) {
#ifdef CHRE_AUDIO_SUPPORT_ENABLED
  Nanoapp *nanoapp = EventLoopManager::validateChreApiCall(__func__);
  return EventLoopManagerSingleton::get()->getAudioRequestManager()
      .configureSource(nanoapp, handle, enable, bufferDuration,
                       deliveryInterval);
#else
  return false;
#endif  // CHRE_AUDIO_SUPPORT_ENABLED
}
