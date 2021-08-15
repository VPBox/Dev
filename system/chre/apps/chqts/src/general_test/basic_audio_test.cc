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

#include <general_test/basic_audio_test.h>

#include <shared/send_message.h>
#include <shared/time_util.h>

using nanoapp_testing::kOneSecondInNanoseconds;
using nanoapp_testing::sendFatalFailureToHost;
using nanoapp_testing::sendSuccessToHost;

namespace general_test {
namespace {

//! This is a reasonably high limit on the number of audio sources that a system
//! would expose. Use this to verify that there are no gaps in the source
//! handles.
constexpr uint32_t kMaxAudioSources = 128;

//! This is a reasonably high limit on the sample rate for a source that the
//! system would expose. Sampling rates above 96kHz are likely to be too high
//! for always-on low-power use-cases. Yes, this omits 192kHz, but that is
//! generally reserved for professional audio/recording and mixing applications.
//! Even 96kHz is a stretch, but capping it here allows room to grow. Expected
//! values are more like 16kHz.
constexpr uint32_t kMaxAudioSampleRate = 96000;

//! Provide a floor for the sampling rate of an audio source that the system
//! would expose. Nyquist theorem dictates that the maximum frequency that can
//! be reproduced from given sequence of samples is equal to half that of the
//! sampling rate. This sets a lower bound to try to detect bugs or glitches.
constexpr uint32_t kMinAudioSampleRate = 4000;

//! Provide a floor for buffer duration. This ensures that at the maximum
//! sample rate possible, a minimum number of samples will be delivered in
//! a batch.
constexpr uint64_t kMinBufferDuration =
    (kOneSecondInNanoseconds / kMaxAudioSampleRate) * 10;

//! Provide a ceiling for the maximum buffer duration. This is to catch buggy
//! descriptors of audio sources who expose very long buffers of data which are
//! not practical for always-on, low-power use-cases.
constexpr uint64_t kMaxBufferDuration = kOneSecondInNanoseconds * 120;

/**
 * @return true if the character is ASCII printable.
 */
bool isAsciiPrintable(char c) {
  // A simple enough test to verify that a character is printable. These
  // constants can be verified by reviewing an ASCII chart. All printable
  // characters that we care about for CHRE lie between these two bounds and are
  // contiguous.
  return (c >= ' ' && c <= '~');
}

/**
 * @return true if the supplied string is printable, null-terminated and not
 * longer than the supplied length (including null-terminator).
 */
bool verifyStringWithLength(const char *str, size_t length) {
  bool nullTerminatorFound = false;
  bool isPrintable = true;
  for (size_t i = 0; i < length; i++) {
    if (str[i] == '\0') {
      nullTerminatorFound = true;
      break;
    } else if (!isAsciiPrintable(str[i])) {
      isPrintable = false;
      break;
    }
  }

  return (isPrintable && nullTerminatorFound);
}

/**
 * Validates the fields of a chreAudioSource provided by the framework and posts
 * a failure if the source descriptor is malformed.
 *
 * @return true if the source was valid.
 */
bool validateAudioSource(uint32_t handle,
                         const struct chreAudioSource& source) {
  bool valid = false;
  if (!verifyStringWithLength(source.name, CHRE_AUDIO_SOURCE_NAME_MAX_SIZE)) {
    sendFatalFailureToHost(
        "Invalid audio source name for handle ", &handle);
  } else if (source.sampleRate > kMaxAudioSampleRate
      || source.sampleRate < kMinAudioSampleRate) {
    sendFatalFailureToHost(
        "Invalid audio sample rate for handle ", &handle);
  } else if (source.minBufferDuration < kMinBufferDuration
      || source.minBufferDuration > kMaxBufferDuration) {
    sendFatalFailureToHost(
        "Invalid min buffer duration for handle ", &handle);
  } else if (source.maxBufferDuration < kMinBufferDuration
      || source.maxBufferDuration > kMaxBufferDuration) {
    sendFatalFailureToHost(
        "Invalid max buffer duration for handle ", &handle);
  } else if (source.format != CHRE_AUDIO_DATA_FORMAT_8_BIT_U_LAW
      && source.format != CHRE_AUDIO_DATA_FORMAT_16_BIT_SIGNED_PCM) {
    sendFatalFailureToHost(
        "Invalid audio format for handle ", &handle);
  } else {
    valid = true;
  }

  return valid;
}

bool validateMinimumAudioSource(const struct chreAudioSource& source) {
  // CHQTS requires a 16kHz, PCM-format, 2 second buffer.
  constexpr uint32_t kRequiredSampleRate = 16000;
  constexpr uint64_t kRequiredBufferDuration = 2 * kOneSecondInNanoseconds;

  // Ensure that the minimum buffer size is less than or equal to the required
  // size.
  return (source.sampleRate == kRequiredSampleRate
      && source.minBufferDuration <= kRequiredBufferDuration
      && source.maxBufferDuration >= kRequiredBufferDuration
      && source.format == CHRE_AUDIO_DATA_FORMAT_16_BIT_SIGNED_PCM);
}

/**
 * Attempts to query for all audio sources up to kMaxAudioSources and posts a
 * failure if a gap is found in the handles or the provided descriptor is
 * invalid.
 */
void validateAudioSources() {
  uint32_t validHandleCount = 0;
  bool previousSourceFound = true;
  bool minimumRequirementMet = false;
  for (uint32_t handle = 0; handle < kMaxAudioSources; handle++) {
    struct chreAudioSource audioSource;
    bool sourceFound = chreAudioGetSource(handle, &audioSource);
    if (sourceFound) {
      validHandleCount++;
      if (!previousSourceFound) {
        sendFatalFailureToHost(
            "Gap detected in audio handles at ", &handle);
      } else {
        bool valid = validateAudioSource(handle, audioSource);
        if (valid && !minimumRequirementMet) {
          minimumRequirementMet = validateMinimumAudioSource(audioSource);
        }
      }
    }

    previousSourceFound = sourceFound;
  }

  if (validHandleCount > 0) {
    if (!minimumRequirementMet) {
      sendFatalFailureToHost(
          "Failed to meet minimum audio source requirements");
    }

    if (validHandleCount == kMaxAudioSources) {
      sendFatalFailureToHost(
          "System is reporting too many audio sources");
    }
  }
}

}  // anonymous namespace

BasicAudioTest::BasicAudioTest()
    : Test(CHRE_API_VERSION_1_2),
      mInMethod(false),
      mState(State::kPreStart) {}

void BasicAudioTest::setUp(uint32_t messageSize,
                           const void * /* message */) {
  if (messageSize != 0) {
    sendFatalFailureToHost(
        "Beginning message expects 0 additional bytes, got ",
        &messageSize);
  }

  validateAudioSources();
  sendSuccessToHost();
}

void BasicAudioTest::handleEvent(
    uint32_t senderInstanceId, uint16_t eventType, const void* eventData) {
  if (mInMethod) {
    sendFatalFailureToHost("handleEvent() invoked while already in method.");
  }

  mInMethod = true;

  if (mState == State::kPreStart) {
    unexpectedEvent(eventType);
  } else {
    // TODO: Handle audio data from sources, perform basic sanity checks on it.
  }

  mInMethod = false;
}

}  // namespace general_test
