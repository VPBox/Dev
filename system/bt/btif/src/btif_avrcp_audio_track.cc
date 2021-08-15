/*
 * Copyright 2015 The Android Open Source Project
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

//#define LOG_NDEBUG 0
#define LOG_TAG "bt_btif_avrcp_audio_track"

#include "btif_avrcp_audio_track.h"

#include <base/logging.h>
#include <media/AudioTrack.h>
#include <utils/StrongPointer.h>

#include "bt_target.h"
#include "osi/include/log.h"

using namespace android;

typedef struct { android::sp<android::AudioTrack> track; } BtifAvrcpAudioTrack;

#if (DUMP_PCM_DATA == TRUE)
FILE* outputPcmSampleFile;
char outputFilename[50] = "/data/misc/bluedroid/output_sample.pcm";
#endif

void* BtifAvrcpAudioTrackCreate(int trackFreq, int bits_per_sample,
                                int channelType) {
  audio_format_t format;
  switch (bits_per_sample) {
    default:
    case 16:
      format = AUDIO_FORMAT_PCM_16_BIT;
      break;
    case 24:
      format = AUDIO_FORMAT_PCM_24_BIT_PACKED;
      break;
    case 32:
      format = AUDIO_FORMAT_PCM_32_BIT;
      break;
  }
  LOG_VERBOSE(LOG_TAG,
              "%s Track.cpp: btCreateTrack freq %d format 0x%x channel %d ",
              __func__, trackFreq, format, channelType);
  sp<android::AudioTrack> track = new android::AudioTrack(
      AUDIO_STREAM_MUSIC, trackFreq, format, channelType,
      (size_t)0 /*frameCount*/, (audio_output_flags_t)AUDIO_OUTPUT_FLAG_FAST,
      NULL /*callback_t*/, NULL /*void* user*/, 0 /*notificationFrames*/,
      AUDIO_SESSION_ALLOCATE, android::AudioTrack::TRANSFER_SYNC);
  CHECK(track != NULL);

  BtifAvrcpAudioTrack* trackHolder = new BtifAvrcpAudioTrack;
  CHECK(trackHolder != NULL);
  trackHolder->track = track;

  if (trackHolder->track->initCheck() != 0) {
    return nullptr;
  }

#if (DUMP_PCM_DATA == TRUE)
  outputPcmSampleFile = fopen(outputFilename, "ab");
#endif
  trackHolder->track->setVolume(1, 1);
  return (void*)trackHolder;
}

void BtifAvrcpAudioTrackStart(void* handle) {
  if (handle == NULL) {
    LOG_ERROR(LOG_TAG, "%s: handle is null!", __func__);
    return;
  }
  BtifAvrcpAudioTrack* trackHolder = static_cast<BtifAvrcpAudioTrack*>(handle);
  CHECK(trackHolder != NULL);
  CHECK(trackHolder->track != NULL);
  LOG_VERBOSE(LOG_TAG, "%s Track.cpp: btStartTrack", __func__);
  trackHolder->track->start();
}

void BtifAvrcpAudioTrackStop(void* handle) {
  if (handle == NULL) {
    LOG_DEBUG(LOG_TAG, "%s handle is null.", __func__);
    return;
  }
  BtifAvrcpAudioTrack* trackHolder = static_cast<BtifAvrcpAudioTrack*>(handle);
  if (trackHolder != NULL && trackHolder->track != NULL) {
    LOG_VERBOSE(LOG_TAG, "%s Track.cpp: btStartTrack", __func__);
    trackHolder->track->stop();
  }
}

void BtifAvrcpAudioTrackDelete(void* handle) {
  if (handle == NULL) {
    LOG_DEBUG(LOG_TAG, "%s handle is null.", __func__);
    return;
  }
  BtifAvrcpAudioTrack* trackHolder = static_cast<BtifAvrcpAudioTrack*>(handle);
  if (trackHolder != NULL && trackHolder->track != NULL) {
    LOG_VERBOSE(LOG_TAG, "%s Track.cpp: btStartTrack", __func__);
    delete trackHolder;
  }

#if (DUMP_PCM_DATA == TRUE)
  if (outputPcmSampleFile) {
    fclose(outputPcmSampleFile);
  }
  outputPcmSampleFile = NULL;
#endif
}

void BtifAvrcpAudioTrackPause(void* handle) {
  if (handle == NULL) {
    LOG_DEBUG(LOG_TAG, "%s handle is null.", __func__);
    return;
  }
  BtifAvrcpAudioTrack* trackHolder = static_cast<BtifAvrcpAudioTrack*>(handle);
  if (trackHolder != NULL && trackHolder->track != NULL) {
    LOG_VERBOSE(LOG_TAG, "%s Track.cpp: btStartTrack", __func__);
    trackHolder->track->pause();
    trackHolder->track->flush();
  }
}

void BtifAvrcpSetAudioTrackGain(void* handle, float gain) {
  if (handle == NULL) {
    LOG_DEBUG(LOG_TAG, "%s handle is null.", __func__);
    return;
  }
  BtifAvrcpAudioTrack* trackHolder = static_cast<BtifAvrcpAudioTrack*>(handle);
  if (trackHolder != NULL && trackHolder->track != NULL) {
    LOG_VERBOSE(LOG_TAG, "%s set gain %f", __func__, gain);
    trackHolder->track->setVolume(gain);
  }
}

int BtifAvrcpAudioTrackWriteData(void* handle, void* audioBuffer,
                                 int bufferlen) {
  BtifAvrcpAudioTrack* trackHolder = static_cast<BtifAvrcpAudioTrack*>(handle);
  CHECK(trackHolder != NULL);
  CHECK(trackHolder->track != NULL);
  int retval = -1;
#if (DUMP_PCM_DATA == TRUE)
  if (outputPcmSampleFile) {
    fwrite((audioBuffer), 1, (size_t)bufferlen, outputPcmSampleFile);
  }
#endif
  retval = trackHolder->track->write(audioBuffer, (size_t)bufferlen);
  LOG_VERBOSE(LOG_TAG, "%s Track.cpp: btWriteData len = %d ret = %d", __func__,
              bufferlen, retval);
  return retval;
}
