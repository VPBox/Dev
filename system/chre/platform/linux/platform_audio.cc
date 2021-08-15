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

#include "chre/platform/platform_audio.h"

#include <cinttypes>

#include "chre/core/event_loop_manager.h"
#include "chre/platform/fatal_error.h"
#include "chre/platform/log.h"
#include "chre/platform/system_time.h"
#include "chre/util/dynamic_vector.h"

namespace chre {
namespace {

//! The list of audio sources provided by the simulator.
DynamicVector<UniquePtr<AudioSource>> gAudioSources;

}

PlatformAudio::PlatformAudio() {}

PlatformAudio::~PlatformAudio() {}

void PlatformAudio::init() {
  // TODO: Implement this.
}

void audioSourceCallback(void *cookie) {
  auto *audioSource = static_cast<AudioSource *>(cookie);

  auto& dataEvent = audioSource->dataEvent;
  Nanoseconds samplingTime =
      AudioRequestManager::getDurationFromSampleCountAndRate(
        audioSource->numSamples,
        static_cast<uint32_t>(audioSource->audioInfo.samplerate));
  dataEvent.timestamp = (SystemTime::getMonotonicTime() - samplingTime)
      .toRawNanoseconds();
  dataEvent.sampleCount = audioSource->numSamples;

  if (dataEvent.format == CHRE_AUDIO_DATA_FORMAT_16_BIT_SIGNED_PCM) {
    uint32_t intervalNumSamples =
        AudioRequestManager::getSampleCountFromRateAndDuration(
          static_cast<uint32_t>(audioSource->audioInfo.samplerate),
          audioSource->eventDelay);
    if (intervalNumSamples > audioSource->numSamples) {
      sf_count_t seekAmount = intervalNumSamples - audioSource->numSamples;
      sf_seek(audioSource->audioFile, -seekAmount, SEEK_CUR);
    }

    sf_count_t readCount = sf_read_short(
        audioSource->audioFile,
        const_cast<int16_t *>(dataEvent.samplesS16),
        static_cast<sf_count_t>(dataEvent.sampleCount));
    if (readCount != dataEvent.sampleCount) {
      LOGI("TODO: File done, suspend the source");
    } else {
      EventLoopManagerSingleton::get()->getAudioRequestManager()
          .handleAudioDataEvent(&audioSource->dataEvent);
    }
  } else {
    FATAL_ERROR("Unimplemented data format");
  }
}

void PlatformAudio::setHandleEnabled(uint32_t handle, bool enabled) {
  // TODO: Implement this.
}

bool PlatformAudio::requestAudioDataEvent(uint32_t handle,
                                          uint32_t numSamples,
                                          Nanoseconds eventDelay) {
  LOGD("Request for audio data made for handle %" PRIu32 " with %" PRIu32
       " samples and %" PRIu64 " delivery interval", handle, numSamples,
       eventDelay.toRawNanoseconds());
  auto& source = gAudioSources[handle];
  source->numSamples = numSamples;
  source->eventDelay = eventDelay;
  return source->timer.set(audioSourceCallback, source.get(), eventDelay);
}

void PlatformAudio::cancelAudioDataEventRequest(uint32_t handle) {
  LOGD("Cancelling audio request for handle %" PRIu32, handle);
  auto& source = gAudioSources[handle];
  source->timer.cancel();
}

void PlatformAudio::releaseAudioDataEvent(struct chreAudioDataEvent *event) {
  // TODO(P1-41459d): Implement this API in terms of libsndfile.
}

size_t PlatformAudio::getSourceCount() {
  return gAudioSources.size();
}

bool PlatformAudio::getAudioSource(uint32_t handle,
                                   chreAudioSource *audioSource) const {
  bool success = (handle < gAudioSources.size());
  if (success) {
    const auto& source = gAudioSources[handle];
    // TODO(P1-b9ff35): Ensure that name never exceeds 40 bytes in length.
    audioSource->name = source->audioFilename.c_str();
    audioSource->sampleRate =
        static_cast<uint32_t>(source->audioInfo.samplerate);
    audioSource->minBufferDuration =
        source->minBufferDuration.toRawNanoseconds();
    audioSource->maxBufferDuration =
        source->maxBufferDuration.toRawNanoseconds();
    audioSource->format = source->dataEvent.format;
  }

  return success;
}

void PlatformAudioBase::addAudioSource(UniquePtr<AudioSource>& source) {
  LOGI("Adding audio source - filename: %s, min buf size: %" PRIu64 "ms, "
       "max buf size: %" PRIu64 "ms", source->audioFilename.c_str(),
       Milliseconds(source->minBufferDuration).getMilliseconds(),
       Milliseconds(source->maxBufferDuration).getMilliseconds());
  auto& audioInfo = source->audioInfo;
  source->audioFile = sf_open(source->audioFilename.c_str(), SFM_READ,
                              &audioInfo);
  auto sampleCount = AudioRequestManager::getSampleCountFromRateAndDuration(
      static_cast<uint32_t>(source->audioInfo.samplerate),
      source->maxBufferDuration);
  if (source->audioFile == nullptr) {
    FATAL_ERROR("Failed to open provided audio file %s",
                source->audioFilename.c_str());
  } else if ((audioInfo.format & SF_FORMAT_ULAW) == SF_FORMAT_ULAW) {
    source->dataEvent.format = CHRE_AUDIO_DATA_FORMAT_8_BIT_U_LAW;
    source->dataEvent.samplesULaw8 = static_cast<uint8_t *>(
        malloc(sizeof(uint8_t) * sampleCount));
  } else if ((audioInfo.format & SF_FORMAT_PCM_16) == SF_FORMAT_PCM_16) {
    source->dataEvent.format = CHRE_AUDIO_DATA_FORMAT_16_BIT_SIGNED_PCM;
    source->dataEvent.samplesS16 = static_cast<int16_t *>(
        malloc(sizeof(uint16_t) * sampleCount));
  } else {
    FATAL_ERROR("Invalid format 0x%08x", audioInfo.format);
  }

  source->dataEvent.version = CHRE_AUDIO_DATA_EVENT_VERSION;
  memset(source->dataEvent.reserved, 0, sizeof(source->dataEvent.reserved));
  source->dataEvent.handle = static_cast<uint32_t>(gAudioSources.size());
  source->dataEvent.sampleRate =
      static_cast<uint32_t>(source->audioInfo.samplerate);
  gAudioSources.push_back(std::move(source));
}

}  // namespace chre
