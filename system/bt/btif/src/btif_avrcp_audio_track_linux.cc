/*
 * Copyright 2018 The Android Open Source Project
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

#include "btif_avrcp_audio_track.h"

void* BtifAvrcpAudioTrackCreate(int trackFreq, int bits_per_sample,
                                int channelType) {
  return nullptr;
}

void BtifAvrcpAudioTrackStart(void* handle) {}

void BtifAvrcpAudioTrackStop(void* handle) {}

void BtifAvrcpAudioTrackDelete(void* handle) {}

void BtifAvrcpAudioTrackPause(void* handle) {}

void BtifAvrcpSetAudioTrackGain(void* handle, float gain) {}

int BtifAvrcpAudioTrackWriteData(void* handle, void* audioBuffer,
                                 int bufferlen) {
  return 0;
}
