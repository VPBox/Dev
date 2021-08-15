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

#ifndef CHRE_PLATFORM_SLPI_PLATFORM_AUDIO_BASE_H_
#define CHRE_PLATFORM_SLPI_PLATFORM_AUDIO_BASE_H_

#include "chre/pal/audio.h"
#include "chre/platform/shared/platform_pal.h"

namespace chre {

/**
 * The base PlatformAudio class for the SLPI to inject platform specific
 * functionality from.
 */
class PlatformAudioBase : public PlatformPal {
 public:
  /**
   * Invoked whenever the host goes awake. This is used to implement the
   * deferred audio disable operation. This is called on the CHRE thread.
   */
  void onHostAwake();

 protected:
  //! The instance of callbacks that are provided to the CHRE PAL.
  static const chrePalAudioCallbacks sAudioCallbacks;

  //! The instance of the CHRE PAL API for audio. This will be set to nullptr
  //! if the platform does not supply an implementation.
  const chrePalAudioApi *mAudioApi;

  //! The number of open audio clients. This is incremented/decremented by the
  //! setHandleEnabled platform API.
  uint32_t mNumAudioClients = 0;

  //! The current state of the audio feature enabled on the host.
  bool mCurrentAudioEnabled = false;

  //! The target state of the audio feature enabled on the host. This is used to
  //! support deferred disabling when the next AP wake occurs.
  bool mTargetAudioEnabled = false;
};

}  // namespace chre

#endif  // CHRE_PLATFORM_SLPI_PLATFORM_AUDIO_BASE_H_
