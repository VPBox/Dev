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

#include <chre.h>
#include <cinttypes>
#include <cmath>

#include "chre/util/macros.h"
#include "chre/util/nanoapp/audio.h"
#include "chre/util/nanoapp/log.h"
#include "chre/util/time.h"
#include "kiss_fftr.h"

#define LOG_TAG "[AudioWorld]"

#ifdef CHRE_NANOAPP_INTERNAL
namespace chre {
namespace {
#endif  // CHRE_NANOAPP_INTERNAL

using chre::Milliseconds;
using chre::Nanoseconds;

//! The number of frequencies to generate an FFT over.
constexpr size_t kNumFrequencies = 128;

//! State for Kiss FFT and logging.
uint8_t gKissFftBuffer[4096];
kiss_fftr_cfg gKissFftConfig;
kiss_fft_cpx gKissFftOutput[(kNumFrequencies / 2) + 1];
Milliseconds gFirstAudioEventTimestamp = Milliseconds(0);

/**
 * Returns a graphical representation of a uint16_t value.
 *
 * @param value the value to visualize.
 * @return a character that visually represents the value.
 */
char getFftCharForValue(uint16_t value) {
  constexpr uint16_t kFftLowLimit = 128;
  constexpr uint16_t kFftMedLimit = 256;
  constexpr uint16_t kFftHighLimit = 512;  // Texas Hold'em ༼⁰o⁰；༽
  constexpr uint16_t kFftVeryHighLimit = 1024;

  if (value < kFftLowLimit) {
    return ' ';
  } else if (value >= kFftLowLimit && value < kFftMedLimit) {
    return '_';
  } else if (value >= kFftMedLimit && value < kFftHighLimit) {
    return '.';
  } else if (value >= kFftHighLimit && value < kFftVeryHighLimit) {
    return 'x';
  } else {
    return 'X';
  }
}

/**
 * Initializes Kiss FFT.
 */
void initKissFft() {
  size_t kissFftBufferSize = sizeof(gKissFftBuffer);
  gKissFftConfig = kiss_fftr_alloc(kNumFrequencies, false,
                                   gKissFftBuffer, &kissFftBufferSize);
  if (gKissFftConfig == NULL) {
    LOGE("Failed to init Kiss FFT, needs minimum %zu buffer size",
         kissFftBufferSize);
  } else {
    LOGI("Initialized Kiss FFT, using %zu/%zu of the buffer",
         kissFftBufferSize, sizeof(gKissFftBuffer));
  }
}

/**
 * Logs an audio data event with an FFT visualization of the received audio
 * data.
 *
 * @param event the audio data event to log.
 */
void handleAudioDataEvent(const struct chreAudioDataEvent *event) {
  kiss_fftr(gKissFftConfig, event->samplesS16, gKissFftOutput);

  char fftStr[ARRAY_SIZE(gKissFftOutput) + 1];
  fftStr[ARRAY_SIZE(gKissFftOutput)] = '\0';

  for (size_t i = 0; i < ARRAY_SIZE(gKissFftOutput); i++) {
    float value = sqrtf(powf(gKissFftOutput[i].r, 2)
        + powf(gKissFftOutput[i].i, 2));
    fftStr[i] = getFftCharForValue(static_cast<uint16_t>(value));
  }

  Milliseconds timestamp = Milliseconds(Nanoseconds(event->timestamp));
  if (gFirstAudioEventTimestamp == Milliseconds(0)) {
    gFirstAudioEventTimestamp = timestamp;
  }

  Milliseconds adjustedTimestamp = timestamp - gFirstAudioEventTimestamp;
  LOGD("Audio data - FFT [%s] at %" PRIu64 "ms with %" PRIu32 " samples",
       fftStr, adjustedTimestamp.getMilliseconds(), event->sampleCount);
}

void handleAudioSamplingChangeEvent(
    const struct chreAudioSourceStatusEvent *event) {
  LOGD("Audio sampling status event for handle %" PRIu32 ", suspended: %d",
       event->handle, event->status.suspended);
}

bool nanoappStart() {
  LOGI("Started");

  struct chreAudioSource audioSource;
  for (uint32_t i = 0; chreAudioGetSource(i, &audioSource); i++) {
    LOGI("Found audio source '%s' with %" PRIu32 "Hz %s data",
         audioSource.name, audioSource.sampleRate,
         chre::getChreAudioFormatString(audioSource.format));
    LOGI("  buffer duration: [%" PRIu64 "ns, %" PRIu64 "ns]",
        audioSource.minBufferDuration, audioSource.maxBufferDuration);

    if (i == 0) {
      // Only request audio data from the first source, but continue discovery.
      if (chreAudioConfigureSource(i, true,
          audioSource.minBufferDuration, audioSource.minBufferDuration)) {
        LOGI("Requested audio from handle %" PRIu32 " successfully", i);
      } else {
        LOGE("Failed to request audio from handle %" PRIu32, i);
      }
    }
  }

  initKissFft();
  return true;
}

void nanoappHandleEvent(uint32_t senderInstanceId,
                        uint16_t eventType,
                        const void *eventData) {
  switch (eventType) {
    case CHRE_EVENT_AUDIO_DATA:
      handleAudioDataEvent(
          static_cast<const struct chreAudioDataEvent *>(eventData));
      break;
    case CHRE_EVENT_AUDIO_SAMPLING_CHANGE:
      handleAudioSamplingChangeEvent(
          static_cast<const struct chreAudioSourceStatusEvent *>(eventData));
      break;
    default:
      LOGW("Unknown event received");
      break;
  }
}

void nanoappEnd() {
  LOGI("Stopped");
}

#ifdef CHRE_NANOAPP_INTERNAL
}  // anonymous namespace
}  // namespace chre

#include "chre/util/nanoapp/app_id.h"
#include "chre/platform/static_nanoapp_init.h"

CHRE_STATIC_NANOAPP_INIT(AudioWorld, chre::kAudioWorldAppId, 0);
#endif  // CHRE_NANOAPP_INTERNAL
