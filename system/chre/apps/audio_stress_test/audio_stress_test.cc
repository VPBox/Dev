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
#include <cinttypes>

#include "chre/util/macros.h"
#include "chre/util/nanoapp/audio.h"
#include "chre/util/nanoapp/log.h"
#include "chre/util/time.h"

#define LOG_TAG "[AudioStress]"

/**
 * @file
 *
 * This nanoapp is designed to subscribe to audio for varying durations of
 * time and verify that audio data is delivered when it is expected to be.
 */

using chre::Milliseconds;
using chre::Nanoseconds;
using chre::Seconds;

namespace {

//! The required buffer size for the stress test.
constexpr Nanoseconds kBufferDuration = Nanoseconds(Seconds(2));

//! The required sample format for the stress test.
constexpr uint8_t kBufferFormat = CHRE_AUDIO_DATA_FORMAT_16_BIT_SIGNED_PCM;

//! The required sample rate for the stress test.
constexpr uint32_t kBufferSampleRate = 16000;

//! The maximum amount of time that audio will not be delivered for.
constexpr Seconds kMaxAudioGap = Seconds(300);

//! The list of durations to subscribe to audio for. Even durations are for when
//! audio is enabled and odd is for when audio is disabled.
constexpr Milliseconds kStressPlan[] = {
  // Enabled, Disabled
  Milliseconds(20000), Milliseconds(20000),
  Milliseconds(30000), Milliseconds(200),
  Milliseconds(10000), Milliseconds(1000),
  Milliseconds(10000), Milliseconds(1999),
  Milliseconds(8000), Milliseconds(60000),
  Milliseconds(1000), Milliseconds(1000),
  Milliseconds(1000), Milliseconds(1000),
  Milliseconds(1000), Milliseconds(1000),
  Milliseconds(1000), Milliseconds(1000),
  Milliseconds(1000), Milliseconds(1000),
  Milliseconds(1000), Milliseconds(1000),
  Milliseconds(1000), Milliseconds(1000),
  Milliseconds(1000), Milliseconds(1000),
};

//! The discovered audio handle found at startup.
uint32_t gAudioHandle;

//! The current position in the stress plan.
size_t gTestPosition = 0;

//! The timer handle to advance through the stress test.
uint32_t gTimerHandle;

//! Whether or not audio is currently suspended. If audio is delivered when this
//! is set to true, this is considered a test failure.
bool gAudioIsSuspended = true;

//! The timestamp of the last audio data event.
Nanoseconds gLastAudioTimestamp;

/**
 * @return true when the current test phase is expecting audio data events to be
 *         delivered.
 */
bool audioIsExpected() {
  // Even test intervals are expected to return audio events. The current test
  // interval is gTestPosition - 1 so there is no need to invert the bit.
  return (gTestPosition % 2);
}

/**
 * Discovers an audio source to use for the stress test. The gAudioHandle will
 * be set if the audio source was found.
 *
 * @return true if a matching source was discovered successfully.
 */
bool discoverAudioHandle() {
  bool success = false;
  struct chreAudioSource source;
  for (uint32_t i = 0; !success && chreAudioGetSource(i, &source); i++) {
    LOGI("Found audio source '%s' with %" PRIu32 "Hz %s data",
         source.name, source.sampleRate,
         chre::getChreAudioFormatString(source.format));
    LOGI("  buffer duration: [%" PRIu64 "ns, %" PRIu64 "ns]",
        source.minBufferDuration, source.maxBufferDuration);

    if (source.sampleRate == kBufferSampleRate
        && source.minBufferDuration <= kBufferDuration.toRawNanoseconds()
        && source.maxBufferDuration >= kBufferDuration.toRawNanoseconds()
        && source.format == kBufferFormat) {
      gAudioHandle = i;
      success = true;
    }
  }

  if (!success) {
    LOGW("Failed to find suitable audio source");
  }

  return success;
}

void checkTestPassing() {
  auto lastAudioDuration = Nanoseconds(chreGetTime()) - gLastAudioTimestamp;
  if (lastAudioDuration > kMaxAudioGap) {
    LOGE("Test fail - audio not received for %" PRIu64 "ns",
         lastAudioDuration.toRawNanoseconds());
    chreAbort(-1);
  }
}

bool requestAudioForCurrentTestState(const Nanoseconds& testStateDuration) {
  bool success = false;
  LOGD("Test stage %zu", gTestPosition);
  if (audioIsExpected()) {
    if (!chreAudioConfigureSource(gAudioHandle, true, kBufferDuration.toRawNanoseconds(),
                                  kBufferDuration.toRawNanoseconds())) {
      LOGE("Failed to enable audio");
    } else {
      LOGI("Enabled audio for %" PRIu64, testStateDuration.toRawNanoseconds());
      success = true;
    }
  } else {
    if (!chreAudioConfigureSource(0, false, 0, 0)) {
      LOGE("Failed to disable audio");
    } else {
      LOGI("Disabled audio for %" PRIu64, testStateDuration.toRawNanoseconds());
      success = true;
    }
  }

  return success;
}

bool advanceTestPosition() {
  checkTestPassing();
  gTimerHandle = chreTimerSet(kStressPlan[gTestPosition].toRawNanoseconds(),
                              nullptr, true /* oneShot */);
  bool success = (gTimerHandle != CHRE_TIMER_INVALID);
  if (!success) {
    LOGE("Failed to set timer");
  } else {
    // Grab the duration prior to incrementing the test position.
    Nanoseconds timerDuration = kStressPlan[gTestPosition++];
    if (gTestPosition >= ARRAY_SIZE(kStressPlan)) {
      gTestPosition = 0;
    }

    success = requestAudioForCurrentTestState(timerDuration);
  }

  return success;
}

void handleTimerEvent() {
  if (!advanceTestPosition()) {
    LOGE("Test fail");
  }
}

void handleAudioDataEvent(const chreAudioDataEvent *audioDataEvent) {
  LOGI("Handling audio data event");
  gLastAudioTimestamp = Nanoseconds(audioDataEvent->timestamp);

  if (gAudioIsSuspended) {
    LOGE("Test fail - received audio when suspended");
  } else if (!audioIsExpected()) {
    LOGE("Test fail - received audio unexpectedly");
  } else {
    LOGI("Test passing - received audio when expected");
  }
}

void handleAudioSamplingChangeEvent(
    const chreAudioSourceStatusEvent *audioSourceStatusEvent) {
  LOGI("Handling audio sampling change event - suspended: %d",
       audioSourceStatusEvent->status.suspended);
  gAudioIsSuspended = audioSourceStatusEvent->status.suspended;
}

}  // namespace


bool nanoappStart() {
  LOGI("start");
  gLastAudioTimestamp = Nanoseconds(chreGetTime());
  return (discoverAudioHandle() && advanceTestPosition());
}

void nanoappHandleEvent(uint32_t senderInstanceId,
                        uint16_t eventType,
                        const void *eventData) {
  switch (eventType) {
    case CHRE_EVENT_TIMER:
      handleTimerEvent();
      break;

    case CHRE_EVENT_AUDIO_DATA:
      handleAudioDataEvent(
          static_cast<const chreAudioDataEvent *>(eventData));
      break;

    case CHRE_EVENT_AUDIO_SAMPLING_CHANGE:
      handleAudioSamplingChangeEvent(
          static_cast<const chreAudioSourceStatusEvent *>(eventData));
      break;

    default:
      LOGW("Unexpected event %" PRIu16, eventType);
      break;
  }
}

void nanoappEnd() {
  LOGI("stop");
}
