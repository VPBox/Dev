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

#ifndef CHRE_PLATFORM_PLATFORM_AUDIO_H_
#define CHRE_PLATFORM_PLATFORM_AUDIO_H_

#include "chre_api/chre/audio.h"
#include "chre/target_platform/platform_audio_base.h"
#include "chre/util/non_copyable.h"
#include "chre/util/time.h"

namespace chre {

/**
 * Defines the common interface to audio functionality.
 */
class PlatformAudio : public PlatformAudioBase,
                      public NonCopyable {
 public:
  /**
   * Initializes the audio subsystem. This is invoked as part of the
   * construction of the EventLoopManager.
   */
  PlatformAudio();

  /**
   * Deinitializes the audio subsystem. This is invoked as part of the
   * destruction of the EventLoopManager.
   */
  ~PlatformAudio();

  /**
   * Initializes the platform-specific audio implementation. This is potentially
   * called at a later stage of initialization than the constructor to allow the
   * rest of CHRE to initialize. This permits use of deferCallback. This method
   * must be invoked before methods of this class can be invoked.
   */
  void init();

  /*
   * Allows the CHRE common code to notify the platform that the enabled state
   * of a given audio handle has changed. This will only be invoked with true
   * when the number of clients for the handle is greater than zero or false
   * when it is equal to zero.
   *
   * @param handle The handle for which audio enabled state is changing.
   * @param enabled true if an active request is open for this handle, false
   *        otherwise.
   */
  void setHandleEnabled(uint32_t handle, bool enabled);

  /**
   * Requests an audio data event from the platform for the provided handle. A
   * call to this method must cancel any previous request.
   *
   * The event requested here may contain data from previously posted events.
   * The concept is to allow the platform to manage its own buffers for audio
   * data. If a request comes in for 8000 samples of data and the most recent
   * request was for 4000 samples of data, the platform implementation may reuse
   * the existing 4000 samples of data and append 4000 samples of new data
   * (assuming that the arguments passed here allow that).
   *
   * Once a request for a given source has been made, the platform
   * implementation must maintain a buffer of previously collected audio samples
   * to provide when a request comes in for data in the past (up to the maximum
   * buffer size for this source). This happens when numSamples at the source
   * sample rate is a greater amount of time than eventDelay. This buffer can be
   * released once cancelAudioDataEventRequest has been invoked for a given
   * source.
   *
   * The event is provided to CHRE through the handleAudioDataEvent function of
   * the AudioRequestManager.
   *
   * @param handle The handle for which an audio event is requested.
   * @param numSamples The number of samples to send once the request has been
   *        completed.
   * @param eventDelay The amount of time that must pass before providing the
   *        data event to CHRE.
   */
  bool requestAudioDataEvent(uint32_t handle,
                             uint32_t numSamples,
                             Nanoseconds eventDelay);

  /**
   * Cancels the previous call to requestAudioDataEvent. No audio data is
   * allowed to be posted to CHRE after this function has been called and before
   * the next call to requestAudioDataEvent.
   *
   * @param handle The handle for which the most recent call to
   *        requestAudioDataEvent will be cancelled.
   */
  void cancelAudioDataEventRequest(uint32_t handle);

  /**
   * Releases a previously posted audio event. This will be invoked by CHRE to
   * say that all nanoapps have processed the previously scheduled data event.
   *
   * @param event An audio data event that was previously provided to
   *        CHRE as a result of a request for audio data.
   */
  void releaseAudioDataEvent(struct chreAudioDataEvent *event);

  /**
   * @return the number of sources supported by the implementation. The returned
   * value must be exactly one greater than the maximum supported audio handle.
   */
  size_t getSourceCount();

  /**
   * Obtains the audio source description for a given handle.
   *
   * @param handle the handle for the requested audio source.
   * @param audioSource the chreAudioSource to populate with details of the
   *     audio source. This pointer must never be null.
   */
  bool getAudioSource(uint32_t handle, chreAudioSource *audioSource) const;
};

}  // namespace chre

#endif  // CHRE_PLATFORM_PLATFORM_AUDIO_H_
