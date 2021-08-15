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

#include <cstring>

#include "chre/core/event_loop_manager.h"
#include "chre/platform/host_link.h"
#include "chre/platform/log.h"
#include "chre/platform/shared/pal_system_api.h"
#include "chre/platform/slpi/power_control_util.h"

namespace chre {
namespace {

void handleAudioDataEvent(struct chreAudioDataEvent *event) {
  EventLoopManagerSingleton::get()->getAudioRequestManager()
      .handleAudioDataEvent(event);
}

void handleAudioAvailability(uint32_t handle, bool available) {
  LOGD("SPI audio handle %" PRIu32 " available: %d", handle, available);
  EventLoopManagerSingleton::get()->getAudioRequestManager()
      .handleAudioAvailability(handle, available);
}

}  // anonymous namespace

const chrePalAudioCallbacks PlatformAudioBase::sAudioCallbacks = {
  handleAudioDataEvent,
  handleAudioAvailability,
};

PlatformAudio::PlatformAudio() {}

PlatformAudio::~PlatformAudio() {
  if (mAudioApi != nullptr) {
    LOGD("Platform audio closing");
    prePalApiCall();
    mAudioApi->close();
    LOGD("Platform audio closed");
  }
}

void PlatformAudio::init() {
  prePalApiCall();
  mAudioApi = chrePalAudioGetApi(CHRE_PAL_AUDIO_API_CURRENT_VERSION);
  if (mAudioApi != nullptr) {
    if (!mAudioApi->open(&gChrePalSystemApi, &sAudioCallbacks)) {
      LOGD("Audio PAL open returned false");
      mAudioApi = nullptr;
    } else {
      LOGD("Opened audio PAL version 0x%08" PRIx32, mAudioApi->moduleVersion);
    }
  } else {
    LOGW("Requested audio PAL (version 0x%08" PRIx32 ") not found",
         CHRE_PAL_AUDIO_API_CURRENT_VERSION);
  }
}

void PlatformAudio::setHandleEnabled(uint32_t handle, bool enabled) {
  uint32_t lastNumAudioClients = mNumAudioClients;

  if (enabled) {
    mNumAudioClients++;
  } else if (mNumAudioClients > 0) {
    mNumAudioClients--;
  } else {
    LOGE("Invalid request to change handle enabled state");
  }

  if (lastNumAudioClients == 0 && mNumAudioClients > 0) {
    mTargetAudioEnabled = true;
    if (!mCurrentAudioEnabled) {
      LOGD("Enabling audio");
      mCurrentAudioEnabled = true;
      sendAudioRequest();
    }
  } else if (lastNumAudioClients > 0 && mNumAudioClients == 0) {
    mTargetAudioEnabled = false;
    if (EventLoopManagerSingleton::get()->getEventLoop()
            .getPowerControlManager().hostIsAwake()) {
      onHostAwake();
    } else {
      LOGD("Deferring disable audio");
    }
  }
}

bool PlatformAudio::requestAudioDataEvent(uint32_t handle,
                                          uint32_t numSamples,
                                          Nanoseconds eventDelay) {
  bool success = false;
  if (mAudioApi != nullptr) {
    prePalApiCall();
    success = mAudioApi->requestAudioDataEvent(
        handle, numSamples, eventDelay.toRawNanoseconds());
  }

  return success;
}

void PlatformAudio::cancelAudioDataEventRequest(uint32_t handle) {
  if (mAudioApi != nullptr) {
    prePalApiCall();
    mAudioApi->cancelAudioDataEvent(handle);
  }
}

void PlatformAudio::releaseAudioDataEvent(struct chreAudioDataEvent *event) {
  if (mAudioApi != nullptr) {
    prePalApiCall();
    mAudioApi->releaseAudioDataEvent(event);
  }
}

size_t PlatformAudio::getSourceCount() {
  size_t sourceCount = 0;
  if (mAudioApi != nullptr) {
    prePalApiCall();
    sourceCount = mAudioApi->getSourceCount();
  }

  return sourceCount;
}

bool PlatformAudio::getAudioSource(uint32_t handle,
                                   struct chreAudioSource *source) const {
  bool success = false;
  if (mAudioApi != nullptr) {
    prePalApiCall();
    success = mAudioApi->getAudioSource(handle, source);
  }

  return success;
}

void PlatformAudioBase::onHostAwake() {
  if (mCurrentAudioEnabled && !mTargetAudioEnabled) {
    LOGD("Disabling audio");
    mCurrentAudioEnabled = mTargetAudioEnabled;
    sendAudioRelease();
  }
}

}  // namespace chre
