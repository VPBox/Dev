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

#ifndef CHRE_PLATFORM_ANDROID_PLATFORM_AUDIO_BASE_H_
#define CHRE_PLATFORM_ANDROID_PLATFORM_AUDIO_BASE_H_

#include <aaudio/AAudio.h>
#include <vector>

#include "chre/platform/system_timer.h"
#include "chre/util/unique_ptr.h"

namespace chre {

/**
 * The base PlatformAudio class for the Android simulator to inject platform
 * specific functionality from.
 */
class PlatformAudioBase {
 protected:
  //! The timer used to defer reading audio samples until they are needed.
  SystemTimer mTimer;

  //! The number of samples to read for the current request.
  uint32_t mNumSamples;

  //! The amount of time to wait to deliver samples for the current request.
  Nanoseconds mEventDelay;

  //! The stream builder used to access Android Audio streams.
  AAudioStreamBuilder *mStreamBuilder;

  //! The stream to read from to access audio data.
  AAudioStream *mStream;

  //! The minimum buffer duration for Android audio data.
  int32_t mMinBufferDuration;

  //! The maximum buffer duration for Android audio data.
  int32_t mMaxBufferDuration;

  //! The buffer to read audio data into. This will be resized to the maximum
  //! buffer size at initialization time.
  std::vector<int16_t> mBuffer;

  //! The data event to post to CHRE. Only one can be published at a time so
  //! it is allocated here to simplify memory management.
  struct chreAudioDataEvent mDataEvent;

  /**
   * Initializes the chreAudioDataEvent owned by this object by setting initial
   * values for fields that do not change for the duration of the runtime.
   */
  void initAudioDataEvent();

  /**
   * A callback to invoke when the timer has elapsed for reading audio data.
   *
   * @param cookie A pointer to provide context to the implementation of this
   *        callback. This will be set to this instance of the PlatformAudio
   *        class.
   */
  static void audioReadCallback(void *cookie);
};

}  // namespace chre

#endif  // CHRE_PLATFORM_ANDROID_PLATFORM_AUDIO_BASE_H_
