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

#ifndef CHRE_PLATFORM_LINUX_AUDIO_SOURCE_H_
#define CHRE_PLATFORM_LINUX_AUDIO_SOURCE_H_

#include <sndfile.h>
#include <string>

#include "chre_api/chre/audio.h"
#include "chre/platform/system_timer.h"
#include "chre/util/non_copyable.h"
#include "chre/util/time.h"

namespace chre {

/**
 * Maintains the state of one audio source for the simulation environment.
 */
class AudioSource : public NonCopyable {
 public:
  /**
   * Constructs an audio source.
   *
   * @param audioFilename the filename for the file to open and playback.
   * @param minBufferSize the minimum buffer size, in seconds, to provide to a
   *        nanoapp.
   * @param maxBufferSize the maximum buffer size, in seconds, to provide to a
   *        nanoapp.
   */
  AudioSource(const std::string& audioFilename,
              double minBufferDuration, double maxBufferDuration);

  /**
   * Releases the audio buffer created during initialization.
   */
  ~AudioSource();

  //! The audio file to open and playback to the nanoapp.
  const std::string audioFilename;

  //! The minimum buffer duration for this audio source.
  const Nanoseconds minBufferDuration;

  //! The maximum buffer duration for this audio source.
  const Nanoseconds maxBufferDuration;

  //! The libsndfile for this audio file.
  SNDFILE *audioFile = nullptr;

  //! The libsndfile audio info for this source.
  SF_INFO audioInfo;

  //! The number of samples for the current request to this source.
  uint32_t numSamples;

  //! The amount of time to wait before delivering this event.
  Nanoseconds eventDelay;

  //! The timer used to delay sending audio data to CHRE.
  SystemTimer timer;

  //! The audio data event to publish to CHRE.
  chreAudioDataEvent dataEvent;
};

}  // namespace chre

#endif  // CHRE_PLATFORM_LINUX_AUDIO_SOURCE_H_
