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

#include "chre/core/audio_request_manager.h"
#include "chre/core/event_loop_manager.h"
#include "chre/platform/fatal_error.h"
#include "chre/platform/log.h"
#include "chre/platform/system_time.h"

namespace chre {

namespace {

//! The fixed sampling rate for audio data when running CHRE on Android.
constexpr uint32_t kAndroidAudioSampleRate = 16000;

//! The minimum buffer size in samples.
constexpr uint32_t kAndroidAudioMinBufferSize = kAndroidAudioSampleRate / 10;

//! The maximum buffer size in samples.
constexpr uint32_t kAndroidAudioMaxBufferSize = kAndroidAudioSampleRate * 10;

}  // namespace

void PlatformAudioBase::audioReadCallback(void *cookie) {
  auto *platformAudio = static_cast<PlatformAudio *>(cookie);

  auto& dataEvent = platformAudio->mDataEvent;
  Nanoseconds samplingTime =
      AudioRequestManager::getDurationFromSampleCountAndRate(
        platformAudio->mNumSamples, kAndroidAudioSampleRate);
  dataEvent.timestamp = (SystemTime::getMonotonicTime() - samplingTime)
      .toRawNanoseconds();

  if (dataEvent.format == CHRE_AUDIO_DATA_FORMAT_16_BIT_SIGNED_PCM) {
    uint32_t intervalNumSamples =
        AudioRequestManager::getSampleCountFromRateAndDuration(
          kAndroidAudioSampleRate, platformAudio->mEventDelay);

    // Determine how much new audio data is required to be read from the device.
    // Samples that are already buffered by this implementation may be reused.
    int16_t *audioBuffer = platformAudio->mBuffer.data();
    uint32_t readAmount = platformAudio->mNumSamples;
    if (intervalNumSamples > platformAudio->mNumSamples) {
      uint32_t seekAmount = intervalNumSamples - platformAudio->mNumSamples;
      audioBuffer = &platformAudio->mBuffer.data()[seekAmount];
      readAmount = platformAudio->mNumSamples - seekAmount;
    }

    // Perform a blocking read. A timeout of 1 nanoasecond is passed here to
    // ensure that we read exactly the amount of requested audio frames. The
    // timer ensures that we wait approximately long enough to read the
    // requested number of samples and the timeout ensures that they match
    // exactly.
    int32_t framesRead = AAudioStream_read(
        platformAudio->mStream, audioBuffer, readAmount, 1);
    if (framesRead != static_cast<int32_t>(platformAudio->mNumSamples)) {
      FATAL_ERROR("Failed to read requested number of audio samples");
    } else {
      EventLoopManagerSingleton::get()->getAudioRequestManager()
          .handleAudioDataEvent(&dataEvent);
    }
  } else {
    FATAL_ERROR("Unimplemented data format");
  }
}

PlatformAudio::PlatformAudio() {
  if (!mTimer.init()) {
    FATAL_ERROR("Failed to initialize audio timer");
  }

  aaudio_result_t result = AAudio_createStreamBuilder(&mStreamBuilder);
  if (result != AAUDIO_OK) {
    FATAL_ERROR("Failed to create audio stream builder with %" PRId32, result);
  }

  AAudioStreamBuilder_setDirection(mStreamBuilder, AAUDIO_DIRECTION_INPUT);
  AAudioStreamBuilder_setSharingMode(mStreamBuilder,
                                     AAUDIO_SHARING_MODE_SHARED);
  AAudioStreamBuilder_setSampleRate(mStreamBuilder, kAndroidAudioSampleRate);
  AAudioStreamBuilder_setChannelCount(mStreamBuilder, 1);
  AAudioStreamBuilder_setFormat(mStreamBuilder, AAUDIO_FORMAT_PCM_I16);
  AAudioStreamBuilder_setBufferCapacityInFrames(mStreamBuilder,
                                                kAndroidAudioMaxBufferSize);

  result = AAudioStreamBuilder_openStream(mStreamBuilder, &mStream);
  if (result != AAUDIO_OK) {
    FATAL_ERROR("Failed to create audio stream with %" PRId32, result);
  }

  int32_t bufferSize = AAudioStream_getBufferCapacityInFrames(mStream);
  LOGD("Created audio stream with %" PRId32 " frames buffer size",
       bufferSize);

  mMinBufferDuration = AudioRequestManager::getDurationFromSampleCountAndRate(
      kAndroidAudioMinBufferSize, kAndroidAudioSampleRate).toRawNanoseconds();
  mMaxBufferDuration = AudioRequestManager::getDurationFromSampleCountAndRate(
      bufferSize, kAndroidAudioSampleRate).toRawNanoseconds();

  result = AAudioStream_requestStart(mStream);
  if (result != AAUDIO_OK) {
    FATAL_ERROR("Failed to start audio stream with %" PRId32, result);
  }

  mBuffer.resize(bufferSize);
  initAudioDataEvent();
}

PlatformAudio::~PlatformAudio() {
  AAudioStream_close(mStream);
  AAudioStreamBuilder_delete(mStreamBuilder);
}

void PlatformAudio::init() {
  // TODO: Implement this.
}

void PlatformAudio::setHandleEnabled(uint32_t handle, bool enabled) {
  // TODO: Implement this.
}

bool PlatformAudio::requestAudioDataEvent(uint32_t handle,
                                          uint32_t numSamples,
                                          Nanoseconds eventDelay) {
  mNumSamples = numSamples;
  mEventDelay = eventDelay;
  mDataEvent.sampleCount = numSamples;
  return mTimer.set(audioReadCallback, this, eventDelay);
}

void PlatformAudio::cancelAudioDataEventRequest(uint32_t handle) {
  mTimer.cancel();
}

void PlatformAudio::releaseAudioDataEvent(struct chreAudioDataEvent *event) {
}

size_t PlatformAudio::getSourceCount() {
  // Hardcoded at one as the Android platform only surfaces the default mic.
  return 1;
}

bool PlatformAudio::getAudioSource(uint32_t handle,
                                   chreAudioSource *audioSource) const {
  bool success = false;
  if (handle == 0) {
    audioSource->name = "Default Android Audio Input";
    audioSource->sampleRate = kAndroidAudioSampleRate;
    audioSource->minBufferDuration = mMinBufferDuration;
    audioSource->maxBufferDuration = mMaxBufferDuration;
    audioSource->format = mDataEvent.format;
    success = true;
  }

  return success;
}

void PlatformAudioBase::initAudioDataEvent() {
  mDataEvent.version = CHRE_AUDIO_DATA_EVENT_VERSION;
  memset(mDataEvent.reserved, 0, sizeof(mDataEvent.reserved));
  mDataEvent.handle = 0;
  mDataEvent.sampleRate = kAndroidAudioSampleRate;
  mDataEvent.format = CHRE_AUDIO_DATA_FORMAT_16_BIT_SIGNED_PCM;
  mDataEvent.samplesS16 = mBuffer.data();
}

}  // namespace chre
