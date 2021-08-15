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

#include "chre/core/audio_request_manager.h"

#include "chre/core/event_loop_manager.h"
#include "chre/platform/fatal_error.h"
#include "chre/platform/system_time.h"
#include "chre/util/system/debug_dump.h"

/*
 * TODO(P1-62e045): Evict pending audio events from the event queue as needed.
 *
 * Under the following conditions, an audio data event may be posted to the
 * CHRE event queue when it should not be.
 *
 * 1. Nanoapp changes request
 * 2. Nanoapp removes request
 *
 * A previously scheduled audio data event may be residing in the event queue
 * and will be dispatched to the nanoapp after it has cancelled the request.
 *
 * The solution is to evict any audio events for a given audio handle that are
 * directed to a nanoapp before scheduling the next request to the platform.
 */

namespace chre {

void AudioRequestManager::init() {
  mPlatformAudio.init();

  size_t sourceCount = mPlatformAudio.getSourceCount();
  if (!mAudioRequestLists.reserve(sourceCount)) {
    FATAL_ERROR_OOM();
  }

  for (size_t i = 0; i < sourceCount; i++) {
    mAudioRequestLists.emplace_back();
  }
}

bool AudioRequestManager::configureSource(const Nanoapp *nanoapp,
                                          uint32_t handle,
                                          bool enable,
                                          uint64_t bufferDuration,
                                          uint64_t deliveryInterval) {
  uint32_t numSamples;
  return validateConfigureSourceArguments(handle, enable, bufferDuration,
                                          deliveryInterval, &numSamples)
      && doConfigureSource(nanoapp->getInstanceId(), handle, enable, numSamples,
                           Nanoseconds(deliveryInterval));
}

void AudioRequestManager::handleAudioDataEvent(
    const struct chreAudioDataEvent *audioDataEvent) {
  uint32_t handle = audioDataEvent->handle;
  if (handle >= mAudioRequestLists.size()) {
    LOGE("Received audio event for unknown handle %" PRIu32, handle);
  } else {
    mAudioRequestLists[handle].lastEventTimestamp
        = SystemTime::getMonotonicTime();
  }

  auto callback = [](uint16_t /* eventType */, void *eventData) {
    auto *event = static_cast<struct chreAudioDataEvent *>(eventData);
    EventLoopManagerSingleton::get()->getAudioRequestManager()
        .handleAudioDataEventSync(event);
  };

  // Cast off the event const so that it can be provided to the free callback as
  // non-const. The event is provided to nanoapps as const and the runtime
  // itself will not modify this memory so this is safe.
  EventLoopManagerSingleton::get()->deferCallback(
      SystemCallbackType::AudioHandleDataEvent,
      const_cast<struct chreAudioDataEvent *>(audioDataEvent), callback);
}

void AudioRequestManager::handleAudioAvailability(uint32_t handle, bool available) {
  struct CallbackState {
    uint32_t handle;
    bool available;
  };

  auto *cbState = memoryAlloc<CallbackState>();
  if (cbState == nullptr) {
    LOG_OOM();
  } else {
    cbState->handle = handle;
    cbState->available = available;

    auto callback = [](uint16_t /* eventType */, void *eventData) {
      auto *state = static_cast<CallbackState *>(eventData);
      EventLoopManagerSingleton::get()->getAudioRequestManager()
          .handleAudioAvailabilitySync(state->handle, state->available);
      memoryFree(state);
    };

    EventLoopManagerSingleton::get()->deferCallback(
        SystemCallbackType::AudioAvailabilityChange, cbState, callback);
  }
}

void AudioRequestManager::logStateToBuffer(char *buffer, size_t *bufferPos,
                                           size_t bufferSize) const {
  debugDumpPrint(buffer, bufferPos, bufferSize, "\nAudio:\n");
  for (size_t i = 0; i < mAudioRequestLists.size(); i++) {
    uint32_t handle = static_cast<uint32_t>(i);
    struct chreAudioSource source;
    mPlatformAudio.getAudioSource(handle, &source);

    Nanoseconds timeSinceLastAudioEvent = SystemTime::getMonotonicTime()
        - mAudioRequestLists[i].lastEventTimestamp;
    debugDumpPrint(buffer, bufferPos, bufferSize,
        " handle=%" PRIu32 ", name=\"%s\", available=%d, sampleRate=%" PRIu32
        ", buffer(ms)=[%" PRIu64 ",%" PRIu64 "], format=%" PRIu8
        ", timeSinceLastAudioEvent(ms)=%" PRIu64 "\n",
        handle, source.name, mAudioRequestLists[i].available, source.sampleRate,
        Milliseconds(Nanoseconds(source.minBufferDuration)).getMilliseconds(),
        Milliseconds(Nanoseconds(source.maxBufferDuration)).getMilliseconds(),
        source.format, Milliseconds(timeSinceLastAudioEvent).getMilliseconds());

    for (const auto& request : mAudioRequestLists[i].requests) {
      for (const auto& instanceId : request.instanceIds) {
        debugDumpPrint(buffer, bufferPos, bufferSize,
            "  nanoappId=%" PRIu32 ", numSamples=%" PRIu32
            ", interval(ms)=%" PRIu64 "\n", instanceId, request.numSamples,
            Milliseconds(Nanoseconds(request.deliveryInterval))
                .getMilliseconds());
      }
    }
  }
}

bool AudioRequestManager::validateConfigureSourceArguments(
    uint32_t handle, bool enable, uint64_t bufferDuration,
    uint64_t deliveryInterval, uint32_t *numSamples) {
  bool success = false;
  if (handle >= mAudioRequestLists.size()) {
    LOGE("Provided audio handle out of range");
  } else if (enable) {
    chreAudioSource audioSource;
    if (!mPlatformAudio.getAudioSource(handle, &audioSource)) {
      LOGE("Failed to query for audio source");
    } else if (bufferDuration > deliveryInterval) {
      LOGE("Buffer duration must be less than or equal to delivery interval");
    } else if (bufferDuration < audioSource.minBufferDuration
               || bufferDuration > audioSource.maxBufferDuration) {
      LOGE("Invalid buffer duration %" PRIu64 " not in range [%" PRIu64
           ",%" PRIu64 "]", bufferDuration, audioSource.minBufferDuration,
           audioSource.maxBufferDuration);
    } else {
      *numSamples = getSampleCountFromRateAndDuration(
          audioSource.sampleRate, Nanoseconds(bufferDuration));
      success = true;
    }
  } else {
    // Disabling the request, no need to validate bufferDuration or
    // deliveryInterval.
    success = true;
  }
  return success;
}

bool AudioRequestManager::doConfigureSource(
    uint32_t instanceId, uint32_t handle, bool enable, uint32_t numSamples,
    Nanoseconds deliveryInterval) {
  size_t requestIndex;
  size_t requestInstanceIdIndex;
  auto *audioRequest = findAudioRequestByInstanceId(
      handle, instanceId, &requestIndex, &requestInstanceIdIndex);

  AudioRequestList& requestList = mAudioRequestLists[handle];
  size_t lastNumRequests = requestList.requests.size();

  bool success = false;
  if (audioRequest == nullptr) {
    if (enable) {
      success = createAudioRequest(handle, instanceId, numSamples,
                                   deliveryInterval);
    } else {
      LOGW("Nanoapp disabling nonexistent audio request");
    }
  } else {
    if (audioRequest->instanceIds.size() > 1) {
      // If there are other clients listening in this configuration, remove
      // just the instance ID.
      audioRequest->instanceIds.erase(requestInstanceIdIndex);
    } else {
      // If this is the last client listening in this configuration, remove
      // the entire request.
      requestList.requests.erase(requestIndex);
    }

    // If the client is disabling, there is nothing to do, otherwise a request
    // must be created successfully.
    success = (!enable || createAudioRequest(handle, instanceId, numSamples,
                                             deliveryInterval));
  }

  if (success) {
    scheduleNextAudioDataEvent(handle);
    updatePlatformHandleEnabled(handle, lastNumRequests);
  }

  return success;
}

void AudioRequestManager::updatePlatformHandleEnabled(
    uint32_t handle, size_t lastNumRequests) {
  size_t numRequests = mAudioRequestLists[handle].requests.size();
  if (lastNumRequests == 0 && numRequests > 0) {
    mPlatformAudio.setHandleEnabled(handle, true /* enabled */);
  } else if (lastNumRequests > 0 && numRequests == 0) {
    mPlatformAudio.setHandleEnabled(handle, false /* enabled */);
  }
}

bool AudioRequestManager::createAudioRequest(
    uint32_t handle, uint32_t instanceId, uint32_t numSamples,
    Nanoseconds deliveryInterval) {
  AudioRequestList& requestList = mAudioRequestLists[handle];

  size_t matchingRequestIndex;
  auto *matchingAudioRequest = findAudioRequestByConfiguration(
      handle, numSamples, deliveryInterval, &matchingRequestIndex);

  bool success = false;
  if (matchingAudioRequest != nullptr) {
    if (!matchingAudioRequest->instanceIds.push_back(instanceId)) {
      LOG_OOM();
    } else {
      success = true;
    }
  } else {
    Nanoseconds timeNow = SystemTime::getMonotonicTime();
    Nanoseconds nextEventTimestamp = timeNow + deliveryInterval;
    if (!requestList.requests.emplace_back(numSamples, deliveryInterval,
                                           nextEventTimestamp)) {
      LOG_OOM();
    } else if (!requestList.requests.back().instanceIds.push_back(instanceId)) {
      requestList.requests.pop_back();
      LOG_OOM();
    } else {
      success = true;
    }
  }

  if (success) {
    postAudioSamplingChangeEvent(instanceId, handle, requestList.available);
  }

  return success;
}

AudioRequestManager::AudioRequest *AudioRequestManager::
    findAudioRequestByInstanceId(
        uint32_t handle, uint32_t instanceId, size_t *index,
        size_t *instanceIdIndex) {
  AudioRequest *foundAudioRequest = nullptr;
  auto& requests = mAudioRequestLists[handle].requests;
  for (size_t i = 0; i < requests.size(); i++) {
    auto& audioRequest = requests[i];
    size_t foundInstanceIdIndex = audioRequest.instanceIds.find(instanceId);
    if (foundInstanceIdIndex != audioRequest.instanceIds.size()) {
      foundAudioRequest = &audioRequest;
      *index = i;
      *instanceIdIndex = foundInstanceIdIndex;
      break;
    }
  }

  return foundAudioRequest;
}

AudioRequestManager::AudioRequest *AudioRequestManager::
    findAudioRequestByConfiguration(
        uint32_t handle, uint32_t numSamples, Nanoseconds deliveryInterval,
        size_t *index) {
  AudioRequest *foundAudioRequest = nullptr;
  auto& requests = mAudioRequestLists[handle].requests;
  for (size_t i = 0; i < requests.size(); i++) {
    auto& audioRequest = requests[i];
    if (audioRequest.numSamples == numSamples
        && audioRequest.deliveryInterval == deliveryInterval) {
      foundAudioRequest = &audioRequest;
      *index = i;
      break;
    }
  }

  return foundAudioRequest;
}

AudioRequestManager::AudioRequest *AudioRequestManager::findNextAudioRequest(
    uint32_t handle) {
  Nanoseconds earliestNextEventTimestamp = Nanoseconds(UINT64_MAX);
  AudioRequest *nextRequest = nullptr;

  auto& reqList = mAudioRequestLists[handle];
  for (auto& req : reqList.requests) {
    if (req.nextEventTimestamp < earliestNextEventTimestamp) {
      earliestNextEventTimestamp = req.nextEventTimestamp;
      nextRequest = &req;
    }
  }

  return nextRequest;
}

void AudioRequestManager::handleAudioDataEventSync(
    struct chreAudioDataEvent *event) {
  uint32_t handle = event->handle;
  if (handle < mAudioRequestLists.size()) {
    auto& reqList = mAudioRequestLists[handle];
    AudioRequest *nextAudioRequest = reqList.nextAudioRequest;
    if (nextAudioRequest != nullptr) {
      postAudioDataEventFatal(event, nextAudioRequest->instanceIds);
      nextAudioRequest->nextEventTimestamp = SystemTime::getMonotonicTime()
          + nextAudioRequest->deliveryInterval;
    } else {
      LOGW("Received audio data event with no pending audio request");
      mPlatformAudio.releaseAudioDataEvent(event);
    }

    scheduleNextAudioDataEvent(handle);
  } else {
    LOGE("Audio data event handle out of range: %" PRIu32, handle);
  }
}

void AudioRequestManager::handleAudioAvailabilitySync(uint32_t handle,
                                                      bool available) {
  if (handle < mAudioRequestLists.size()) {
    if (mAudioRequestLists[handle].available != available) {
      mAudioRequestLists[handle].available = available;
      postAudioSamplingChangeEvents(handle);
    }

    scheduleNextAudioDataEvent(handle);
  } else {
    LOGE("Audio availability handle out of range: %" PRIu32, handle);
  }
}

void AudioRequestManager::scheduleNextAudioDataEvent(uint32_t handle) {
  auto& reqList = mAudioRequestLists[handle];
  AudioRequest *nextRequest = findNextAudioRequest(handle);

  // Clear the next request and it will be reset below if needed.
  reqList.nextAudioRequest = nullptr;
  if (reqList.available && nextRequest != nullptr) {
    Nanoseconds curTime = SystemTime::getMonotonicTime();
    Nanoseconds eventDelay = Nanoseconds(0);
    if (nextRequest->nextEventTimestamp > curTime) {
      eventDelay = nextRequest->nextEventTimestamp - curTime;
    }
    reqList.nextAudioRequest = nextRequest;
    mPlatformAudio.requestAudioDataEvent(
        handle, nextRequest->numSamples, eventDelay);
  } else {
    mPlatformAudio.cancelAudioDataEventRequest(handle);
  }
}

void AudioRequestManager::postAudioSamplingChangeEvents(uint32_t handle) {
  const auto& requestList = mAudioRequestLists[handle];
  for (const auto& request : requestList.requests) {
    for (const auto& instanceId : request.instanceIds) {
      postAudioSamplingChangeEvent(instanceId, handle, requestList.available);
    }
  }
}

void AudioRequestManager::postAudioSamplingChangeEvent(uint32_t instanceId,
                                                       uint32_t handle,
                                                       bool available) {
  auto *event = memoryAlloc<struct chreAudioSourceStatusEvent>();
  event->handle = handle;
  event->status.enabled = true;
  event->status.suspended = !available;

  EventLoopManagerSingleton::get()->getEventLoop().postEventOrDie(
      CHRE_EVENT_AUDIO_SAMPLING_CHANGE, event, freeEventDataCallback,
      instanceId);
}

void AudioRequestManager::postAudioDataEventFatal(
    struct chreAudioDataEvent *event,
    const DynamicVector<uint32_t>& instanceIds) {
  if (instanceIds.empty()) {
    LOGW("Received audio data event for no clients");
    mPlatformAudio.releaseAudioDataEvent(event);
  } else {
    for (const auto &instanceId : instanceIds) {
      EventLoopManagerSingleton::get()->getEventLoop().postEventOrDie(
          CHRE_EVENT_AUDIO_DATA, event, freeAudioDataEventCallback, instanceId);
    }

    mAudioDataEventRefCounts.emplace_back(
        event, static_cast<uint32_t>(instanceIds.size()));
  }
}

void AudioRequestManager::handleFreeAudioDataEvent(
    struct chreAudioDataEvent *audioDataEvent) {
  size_t audioDataEventRefCountIndex =
      mAudioDataEventRefCounts.find(AudioDataEventRefCount(audioDataEvent));
  if (audioDataEventRefCountIndex == mAudioDataEventRefCounts.size()) {
    LOGE("Freeing invalid audio data event");
  } else {
    auto& audioDataEventRefCount =
        mAudioDataEventRefCounts[audioDataEventRefCountIndex];
    if (audioDataEventRefCount.refCount == 0) {
      LOGE("Attempting to free an event with zero published events");
    } else {
      audioDataEventRefCount.refCount--;
      if (audioDataEventRefCount.refCount == 0) {
        mAudioDataEventRefCounts.erase(audioDataEventRefCountIndex);
        mPlatformAudio.releaseAudioDataEvent(audioDataEvent);
      }
    }
  }
}

void AudioRequestManager::freeAudioDataEventCallback(uint16_t eventType,
                                                     void *eventData) {
  auto *event = static_cast<struct chreAudioDataEvent *>(eventData);
  EventLoopManagerSingleton::get()->getAudioRequestManager()
      .handleFreeAudioDataEvent(event);
}

}  // namespace chre
