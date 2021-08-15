/*
 * Copyright (C) 2016 The Android Open Source Project
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

#include <cinttypes>
#include <cstddef>
#include <cstring>

#include "chre/core/event_loop_manager.h"
#include "chre/core/wifi_request_manager.h"
#include "chre/platform/fatal_error.h"
#include "chre/platform/log.h"
#include "chre/platform/system_time.h"
#include "chre/util/system/debug_dump.h"
#include "chre_api/chre/version.h"

namespace chre {

WifiRequestManager::WifiRequestManager() {
  // Reserve space for at least one scan monitoring nanoapp. This ensures that
  // the first asynchronous push_back will succeed. Future push_backs will be
  // synchronous and failures will be returned to the client.
  if (!mScanMonitorNanoapps.reserve(1)) {
    FATAL_ERROR_OOM();
  }
}

void WifiRequestManager::init() {
  mPlatformWifi.init();
}

uint32_t WifiRequestManager::getCapabilities() {
  return mPlatformWifi.getCapabilities();
}

bool WifiRequestManager::configureScanMonitor(Nanoapp *nanoapp, bool enable,
                                              const void *cookie) {
  CHRE_ASSERT(nanoapp);

  bool success = false;
  uint32_t instanceId = nanoapp->getInstanceId();
  bool hasScanMonitorRequest = nanoappHasScanMonitorRequest(instanceId);
  if (!mPendingScanMonitorRequests.empty()) {
    success = addScanMonitorRequestToQueue(nanoapp, enable, cookie);
  } else if (scanMonitorIsInRequestedState(enable, hasScanMonitorRequest)) {
    // The scan monitor is already in the requested state. A success event can
    // be posted immediately.
    success = postScanMonitorAsyncResultEvent(instanceId, true /* success */,
                                              enable, CHRE_ERROR_NONE, cookie);
  } else if (scanMonitorStateTransitionIsRequired(enable,
                                                  hasScanMonitorRequest)) {
    success = addScanMonitorRequestToQueue(nanoapp, enable, cookie);
    if (success) {
      success = mPlatformWifi.configureScanMonitor(enable);
      if (!success) {
        mPendingScanMonitorRequests.pop_back();
        LOGE("Failed to enable the scan monitor for nanoapp instance %" PRIu32,
             instanceId);
      }
    }
  } else {
    CHRE_ASSERT_LOG(false, "Invalid scan monitor configuration");
  }

  return success;
}

bool WifiRequestManager::requestRanging(
    Nanoapp *nanoapp, const struct chreWifiRangingParams *params,
    const void *cookie) {
  CHRE_ASSERT(nanoapp);

  bool success = false;
  if (!mPendingRangingRequests.emplace()) {
    LOGE("Can't issue new RTT request; pending queue full");
  } else {
    PendingRangingRequest& req = mPendingRangingRequests.back();
    req.nanoappInstanceId = nanoapp->getInstanceId();
    req.cookie = cookie;

    if (mPendingRangingRequests.size() == 1) {
      // First in line; dispatch request immediately
      success = mPlatformWifi.requestRanging(params);
      if (!success) {
        LOGE("WiFi RTT request failed");
        mPendingRangingRequests.pop_back();
      } else {
        mRangingResponseTimeout = SystemTime::getMonotonicTime()
            + Nanoseconds(CHRE_WIFI_RANGING_RESULT_TIMEOUT_NS);
      }
    } else {
      // Dispatch request later, after prior requests finish
      // TODO(b/65331248): use a timer to ensure the platform is meeting its
      // contract
      CHRE_ASSERT_LOG(SystemTime::getMonotonicTime() <= mRangingResponseTimeout,
                      "WiFi platform didn't give callback in time");
      success = req.targetList.copy_array(params->targetList,
                                          params->targetListLen);
      if (!success) {
        LOG_OOM();
        mPendingRangingRequests.pop_back();
      }
    }
  }

  return success;
}

bool WifiRequestManager::requestScan(Nanoapp *nanoapp,
                                     const struct chreWifiScanParams *params,
                                     const void *cookie) {
  CHRE_ASSERT(nanoapp);

  // TODO(b/65331248): replace with a timer to actively check response timeout
  bool timedOut = (mScanRequestingNanoappInstanceId.has_value()
                   && mLastScanRequestTime
                       + Nanoseconds(CHRE_WIFI_SCAN_RESULT_TIMEOUT_NS)
                     < SystemTime::getMonotonicTime());
  if (timedOut) {
    LOGE("Scan request async response timed out");
    mScanRequestingNanoappInstanceId.reset();
  }

  // Handle compatibility with nanoapps compiled against API v1.1, which doesn't
  // include the radioChainPref parameter in chreWifiScanParams
  struct chreWifiScanParams paramsCompat;
  if (nanoapp->getTargetApiVersion() < CHRE_API_VERSION_1_2) {
    memcpy(&paramsCompat, params, offsetof(chreWifiScanParams, radioChainPref));
    paramsCompat.radioChainPref = CHRE_WIFI_RADIO_CHAIN_PREF_DEFAULT;
    params = &paramsCompat;
  }

  bool success = false;
  if (mScanRequestingNanoappInstanceId.has_value()) {
     LOGE("Active wifi scan request made while a request is in flight");
  } else {
    success = mPlatformWifi.requestScan(params);
    if (!success) {
      LOGE("Wifi scan request failed");
    } else {
      mScanRequestingNanoappInstanceId = nanoapp->getInstanceId();
      mScanRequestingNanoappCookie = cookie;
      mLastScanRequestTime = SystemTime::getMonotonicTime();
    }
  }

  return success;
}

void WifiRequestManager::handleScanMonitorStateChange(bool enabled,
                                                      uint8_t errorCode) {
  struct CallbackState {
    bool enabled;
    uint8_t errorCode;
  };

  auto *cbState = memoryAlloc<CallbackState>();
  if (cbState == nullptr) {
    LOG_OOM();
  } else {
    cbState->enabled = enabled;
    cbState->errorCode = errorCode;

    auto callback = [](uint16_t /* eventType */, void *eventData) {
      auto *state = static_cast<CallbackState *>(eventData);
      EventLoopManagerSingleton::get()->getWifiRequestManager()
          .handleScanMonitorStateChangeSync(state->enabled, state->errorCode);
      memoryFree(state);
    };

    EventLoopManagerSingleton::get()->deferCallback(
        SystemCallbackType::WifiScanMonitorStateChange, cbState, callback);
  }
}

void WifiRequestManager::handleScanResponse(bool pending,
                                            uint8_t errorCode) {
  struct CallbackState {
    bool pending;
    uint8_t errorCode;
  };

  auto *cbState = memoryAlloc<CallbackState>();
  if (cbState == nullptr) {
    LOG_OOM();
  } else {
    cbState->pending = pending;
    cbState->errorCode = errorCode;

    auto callback = [](uint16_t /* eventType */, void *eventData) {
      auto *state = static_cast<CallbackState *>(eventData);
      EventLoopManagerSingleton::get()->getWifiRequestManager()
          .handleScanResponseSync(state->pending, state->errorCode);
      memoryFree(state);
    };

    EventLoopManagerSingleton::get()->deferCallback(
        SystemCallbackType::WifiRequestScanResponse, cbState, callback);
  }
}

void WifiRequestManager::handleRangingEvent(
    uint8_t errorCode, struct chreWifiRangingEvent *event) {
  // Use two different callbacks to avoid needing a temporary allocation to
  // carry the error code into the event loop context
  if (errorCode != CHRE_ERROR_NONE) {
    // Enables passing the error code through the event data pointer to avoid
    // allocating memory
    union NestedErrorCode {
      void *eventData;
      uint8_t errorCode;
    };

    auto errorCb = [](uint16_t /* eventType */, void *eventData) {
      NestedErrorCode cbErrorCode;
      cbErrorCode.eventData = eventData;
      EventLoopManagerSingleton::get()->getWifiRequestManager()
          .handleRangingEventSync(cbErrorCode.errorCode, nullptr);
    };

    NestedErrorCode error = {};
    error.errorCode = errorCode;
    EventLoopManagerSingleton::get()->deferCallback(
        SystemCallbackType::WifiHandleFailedRanging, error.eventData, errorCb);
  } else {
    auto successCb = [](uint16_t /* eventType */, void *eventData) {
      auto *rttEvent = static_cast<struct chreWifiRangingEvent *>(eventData);
      EventLoopManagerSingleton::get()->getWifiRequestManager()
          .handleRangingEventSync(CHRE_ERROR_NONE, rttEvent);
    };

    EventLoopManagerSingleton::get()->deferCallback(
        SystemCallbackType::WifiHandleRangingEvent, event, successCb);
  }
}

void WifiRequestManager::handleScanEvent(chreWifiScanEvent *event) {
  auto callback = [](uint16_t eventType, void *eventData) {
    chreWifiScanEvent *scanEvent = static_cast<chreWifiScanEvent *>(eventData);
    EventLoopManagerSingleton::get()->getWifiRequestManager()
        .postScanEventFatal(scanEvent);
  };

  EventLoopManagerSingleton::get()->deferCallback(
      SystemCallbackType::WifiHandleScanEvent, event, callback);
}

void WifiRequestManager::logStateToBuffer(char *buffer, size_t *bufferPos,
                                          size_t bufferSize) const {
  debugDumpPrint(buffer, bufferPos, bufferSize,
                 "\nWifi: scan monitor %s\n",
                 scanMonitorIsEnabled() ? "enabled" : "disabled");
  debugDumpPrint(buffer, bufferPos, bufferSize,
                 " Wifi scan monitor enabled nanoapps:\n");
  for (const auto& instanceId : mScanMonitorNanoapps) {
    debugDumpPrint(buffer, bufferPos, bufferSize,
                   "  nanoappId=%" PRIu32 "\n", instanceId);
  }

  if (mScanRequestingNanoappInstanceId.has_value()) {
    debugDumpPrint(buffer, bufferPos, bufferSize,
                   " Wifi request pending nanoappId=%" PRIu32 "\n",
                   mScanRequestingNanoappInstanceId.value());
  }

  debugDumpPrint(buffer, bufferPos, bufferSize,
                 " Wifi transition queue:\n");
  for (const auto& transition : mPendingScanMonitorRequests) {
    debugDumpPrint(buffer, bufferPos, bufferSize,
                   "  enable=%s nanoappId=%" PRIu32 "\n",
                   transition.enable ? "true" : "false",
                   transition.nanoappInstanceId);
  }
}

bool WifiRequestManager::scanMonitorIsEnabled() const {
  return !mScanMonitorNanoapps.empty();
}

bool WifiRequestManager::nanoappHasScanMonitorRequest(
    uint32_t instanceId, size_t *nanoappIndex) const {
  size_t index = mScanMonitorNanoapps.find(instanceId);
  bool hasScanMonitorRequest = (index != mScanMonitorNanoapps.size());
  if (hasScanMonitorRequest && nanoappIndex != nullptr) {
    *nanoappIndex = index;
  }

  return hasScanMonitorRequest;
}

bool WifiRequestManager::scanMonitorIsInRequestedState(
    bool requestedState, bool nanoappHasRequest) const {
  return (requestedState == scanMonitorIsEnabled() || (!requestedState
      && (!nanoappHasRequest || mScanMonitorNanoapps.size() > 1)));
}

bool WifiRequestManager::scanMonitorStateTransitionIsRequired(
    bool requestedState, bool nanoappHasRequest) const {
  return ((requestedState && mScanMonitorNanoapps.empty())
      || (!requestedState && nanoappHasRequest
              && mScanMonitorNanoapps.size() == 1));
}

bool WifiRequestManager::addScanMonitorRequestToQueue(Nanoapp *nanoapp,
                                                      bool enable,
                                                      const void *cookie) {
  PendingScanMonitorRequest scanMonitorStateTransition;
  scanMonitorStateTransition.nanoappInstanceId = nanoapp->getInstanceId();
  scanMonitorStateTransition.cookie = cookie;
  scanMonitorStateTransition.enable = enable;

  bool success = mPendingScanMonitorRequests.push(scanMonitorStateTransition);
  if (!success) {
    LOGW("Too many scan monitor state transitions");
  }

  return success;
}

bool WifiRequestManager::updateNanoappScanMonitoringList(bool enable,
                                                         uint32_t instanceId) {
  bool success = true;
  Nanoapp *nanoapp = EventLoopManagerSingleton::get()->getEventLoop()
      .findNanoappByInstanceId(instanceId);
  if (nanoapp == nullptr) {
    LOGW("Failed to update scan monitoring list for non-existent nanoapp");
  } else {
    size_t nanoappIndex;
    bool hasExistingRequest = nanoappHasScanMonitorRequest(instanceId,
                                                           &nanoappIndex);
    if (enable) {
      if (!hasExistingRequest) {
        // The scan monitor was successfully enabled for this nanoapp and
        // there is no existing request. Add it to the list of scan monitoring
        // nanoapps.
        success = mScanMonitorNanoapps.push_back(instanceId);
        if (!success) {
          LOG_OOM();
        } else {
          nanoapp->registerForBroadcastEvent(CHRE_EVENT_WIFI_SCAN_RESULT);
        }
      }
    } else if (hasExistingRequest) {
      // The scan monitor was successfully disabled for a previously enabled
      // nanoapp. Remove it from the list of scan monitoring nanoapps.
      mScanMonitorNanoapps.erase(nanoappIndex);
      nanoapp->unregisterForBroadcastEvent(CHRE_EVENT_WIFI_SCAN_RESULT);
    } // else disabling an inactive request, treat as success per the CHRE API.
  }

  return success;
}

bool WifiRequestManager::postScanMonitorAsyncResultEvent(
    uint32_t nanoappInstanceId, bool success, bool enable, uint8_t errorCode,
    const void *cookie) {
  // Allocate and post an event to the nanoapp requesting wifi.
  bool eventPosted = false;
  if (!success || updateNanoappScanMonitoringList(enable, nanoappInstanceId)) {
    chreAsyncResult *event = memoryAlloc<chreAsyncResult>();
    if (event == nullptr) {
      LOG_OOM();
    } else {
      event->requestType = CHRE_WIFI_REQUEST_TYPE_CONFIGURE_SCAN_MONITOR;
      event->success = success;
      event->errorCode = errorCode;
      event->reserved = 0;
      event->cookie = cookie;

      // Post the event.
      eventPosted =
          EventLoopManagerSingleton::get()->getEventLoop().postEventOrDie(
              CHRE_EVENT_WIFI_ASYNC_RESULT, event, freeEventDataCallback,
              nanoappInstanceId);
      if (!eventPosted) {
        memoryFree(event);
      }
    }
  }

  return eventPosted;
}

void WifiRequestManager::postScanMonitorAsyncResultEventFatal(
    uint32_t nanoappInstanceId, bool success, bool enable, uint8_t errorCode,
    const void *cookie) {
  if (!postScanMonitorAsyncResultEvent(nanoappInstanceId, success, enable,
                                       errorCode, cookie)) {
    FATAL_ERROR("Failed to send WiFi scan monitor async result event");
  }
}

bool WifiRequestManager::postScanRequestAsyncResultEvent(
    uint32_t nanoappInstanceId, bool success, uint8_t errorCode,
    const void *cookie) {
  bool eventPosted = false;
  chreAsyncResult *event = memoryAlloc<chreAsyncResult>();
  if (event == nullptr) {
    LOG_OOM();
  } else {
    event->requestType = CHRE_WIFI_REQUEST_TYPE_REQUEST_SCAN;
    event->success = success;
    event->errorCode = errorCode;
    event->reserved = 0;
    event->cookie = cookie;

    // Post the event.
    eventPosted =
        EventLoopManagerSingleton::get()->getEventLoop().postEventOrDie(
            CHRE_EVENT_WIFI_ASYNC_RESULT, event, freeEventDataCallback,
            nanoappInstanceId);
  }

  return eventPosted;
}

void WifiRequestManager::postScanRequestAsyncResultEventFatal(
    uint32_t nanoappInstanceId, bool success, uint8_t errorCode,
    const void *cookie) {
  if (!postScanRequestAsyncResultEvent(nanoappInstanceId, success, errorCode,
                                       cookie)) {
    FATAL_ERROR("Failed to send WiFi scan request async result event");
  }
}

void WifiRequestManager::postScanEventFatal(chreWifiScanEvent *event) {
  EventLoopManagerSingleton::get()->getEventLoop().postEventOrDie(
      CHRE_EVENT_WIFI_SCAN_RESULT, event, freeWifiScanEventCallback);
}

void WifiRequestManager::handleScanMonitorStateChangeSync(bool enabled,
                                                          uint8_t errorCode) {
  // Success is defined as having no errors ... in life ༼ つ ◕_◕ ༽つ
  bool success = (errorCode == CHRE_ERROR_NONE);

  // TODO(b/62904616): re-enable this assertion
  //CHRE_ASSERT_LOG(!mScanMonitorStateTransitions.empty(),
  //                "handleScanMonitorStateChangeSync called with no transitions");
  if (mPendingScanMonitorRequests.empty()) {
    LOGE("WiFi PAL error: handleScanMonitorStateChangeSync called with no "
         "transitions (enabled %d errorCode %" PRIu8 ")", enabled, errorCode);
  }

  // Always check the front of the queue.
  if (!mPendingScanMonitorRequests.empty()) {
    const auto& stateTransition = mPendingScanMonitorRequests.front();
    success &= (stateTransition.enable == enabled);
    postScanMonitorAsyncResultEventFatal(stateTransition.nanoappInstanceId,
                                         success, stateTransition.enable,
                                         errorCode, stateTransition.cookie);
    mPendingScanMonitorRequests.pop();
  }

  while (!mPendingScanMonitorRequests.empty()) {
    const auto& stateTransition = mPendingScanMonitorRequests.front();
    bool hasScanMonitorRequest = nanoappHasScanMonitorRequest(
        stateTransition.nanoappInstanceId);
    if (scanMonitorIsInRequestedState(
        stateTransition.enable, hasScanMonitorRequest)) {
      // We are already in the target state so just post an event indicating
      // success
      postScanMonitorAsyncResultEventFatal(stateTransition.nanoappInstanceId,
                                           true /* success */,
                                           stateTransition.enable,
                                           CHRE_ERROR_NONE,
                                           stateTransition.cookie);
    } else if (scanMonitorStateTransitionIsRequired(
        stateTransition.enable, hasScanMonitorRequest)) {
      if (mPlatformWifi.configureScanMonitor(stateTransition.enable)) {
        break;
      } else {
        postScanMonitorAsyncResultEventFatal(stateTransition.nanoappInstanceId,
                                             false /* success */,
                                             stateTransition.enable, CHRE_ERROR,
                                             stateTransition.cookie);
      }
    } else {
      CHRE_ASSERT_LOG(false, "Invalid scan monitor state");
      break;
    }

    mPendingScanMonitorRequests.pop();
  }
}

void WifiRequestManager::handleScanResponseSync(bool pending,
                                                uint8_t errorCode) {
  // TODO(b/65206783): re-enable this assertion
  //CHRE_ASSERT_LOG(mScanRequestingNanoappInstanceId.has_value(),
  //                "handleScanResponseSync called with no outstanding request");
  if (!mScanRequestingNanoappInstanceId.has_value()) {
    LOGE("handleScanResponseSync called with no outstanding request");
  }

  // TODO: raise this to CHRE_ASSERT_LOG
  if (!pending && errorCode == CHRE_ERROR_NONE) {
    LOGE("Invalid wifi scan response");
    errorCode = CHRE_ERROR;
  }

  if (mScanRequestingNanoappInstanceId.has_value()) {
    bool success = (pending && errorCode == CHRE_ERROR_NONE);
    if (!success) {
      LOGW("Wifi scan request failed: pending %d, errorCode %" PRIu8,
           pending, errorCode);
    }
    postScanRequestAsyncResultEventFatal(*mScanRequestingNanoappInstanceId,
                                         success, errorCode,
                                         mScanRequestingNanoappCookie);

    // Set a flag to indicate that results may be pending.
    mScanRequestResultsArePending = pending;

    if (pending) {
      Nanoapp *nanoapp = EventLoopManagerSingleton::get()->getEventLoop()
          .findNanoappByInstanceId(*mScanRequestingNanoappInstanceId);
      if (nanoapp == nullptr) {
        LOGW("Received WiFi scan response for unknown nanoapp");
      } else {
        nanoapp->registerForBroadcastEvent(CHRE_EVENT_WIFI_SCAN_RESULT);
      }
    } else {
      // If the scan results are not pending, clear the nanoapp instance ID.
      // Otherwise, wait for the results to be delivered and then clear the
      // instance ID.
      mScanRequestingNanoappInstanceId.reset();
    }
  }
}

bool WifiRequestManager::postRangingAsyncResult(uint8_t errorCode) {
  bool eventPosted = false;

  if (mPendingRangingRequests.empty()) {
    LOGE("Unexpected ranging event callback");
  } else {
    auto *event = memoryAlloc<struct chreAsyncResult>();
    if (event == nullptr) {
      LOG_OOM();
    } else {
      const PendingRangingRequest& req = mPendingRangingRequests.front();

      event->requestType = CHRE_WIFI_REQUEST_TYPE_RANGING;
      event->success = (errorCode == CHRE_ERROR_NONE);
      event->errorCode = errorCode;
      event->reserved = 0;
      event->cookie = req.cookie;

      eventPosted =
          EventLoopManagerSingleton::get()->getEventLoop().postEventOrDie(
              CHRE_EVENT_WIFI_ASYNC_RESULT, event, freeEventDataCallback,
              req.nanoappInstanceId);
      if (!eventPosted) {
        memoryFree(event);
      }
    }
  }

  return eventPosted;
}

bool WifiRequestManager::dispatchQueuedRangingRequest() {
  const PendingRangingRequest& req = mPendingRangingRequests.front();
  struct chreWifiRangingParams params = {};
  params.targetListLen = static_cast<uint8_t>(req.targetList.size());
  params.targetList = req.targetList.data();

  bool success = mPlatformWifi.requestRanging(&params);
  if (!success) {
    LOGE("Failed to issue queued ranging result");
    postRangingAsyncResult(CHRE_ERROR);
    mPendingRangingRequests.pop();
  } else {
    mRangingResponseTimeout = SystemTime::getMonotonicTime()
        + Nanoseconds(CHRE_WIFI_RANGING_RESULT_TIMEOUT_NS);
  }

  return success;
}

void WifiRequestManager::handleRangingEventSync(
    uint8_t errorCode, struct chreWifiRangingEvent *event) {
  if (postRangingAsyncResult(errorCode)) {
    if (errorCode != CHRE_ERROR_NONE) {
      LOGW("RTT ranging failed with error %d", errorCode);
    } else {
      EventLoopManagerSingleton::get()->getEventLoop().postEventOrDie(
          CHRE_EVENT_WIFI_RANGING_RESULT, event, freeWifiRangingEventCallback,
          mPendingRangingRequests.front().nanoappInstanceId);
    }
    mPendingRangingRequests.pop();
  }

  // If we have any pending requests, try issuing them to the platform until the
  // first one succeeds
  while (!mPendingRangingRequests.empty() && !dispatchQueuedRangingRequest());
}

void WifiRequestManager::handleFreeWifiScanEvent(chreWifiScanEvent *scanEvent) {
  if (mScanRequestResultsArePending) {
    // Reset the event distribution logic once an entire scan event has been
    // received and processed by the nanoapp requesting the scan event.
    mScanEventResultCountAccumulator += scanEvent->resultCount;
    if (mScanEventResultCountAccumulator >= scanEvent->resultTotal) {
      mScanEventResultCountAccumulator = 0;
      mScanRequestResultsArePending = false;
    }

    if (!mScanRequestResultsArePending
        && mScanRequestingNanoappInstanceId.has_value()) {
      Nanoapp *nanoapp = EventLoopManagerSingleton::get()->getEventLoop()
          .findNanoappByInstanceId(*mScanRequestingNanoappInstanceId);
      if (nanoapp == nullptr) {
        LOGW("Attempted to unsubscribe unknown nanoapp from WiFi scan events");
      } else if (!nanoappHasScanMonitorRequest(
          *mScanRequestingNanoappInstanceId)) {
        nanoapp->unregisterForBroadcastEvent(CHRE_EVENT_WIFI_SCAN_RESULT);
      }

      mScanRequestingNanoappInstanceId.reset();
    }
  }

  mPlatformWifi.releaseScanEvent(scanEvent);
}

void WifiRequestManager::freeWifiScanEventCallback(uint16_t eventType,
                                                   void *eventData) {
  chreWifiScanEvent *scanEvent = static_cast<chreWifiScanEvent *>(eventData);
  EventLoopManagerSingleton::get()->getWifiRequestManager()
      .handleFreeWifiScanEvent(scanEvent);
}

void WifiRequestManager::freeWifiRangingEventCallback(uint16_t eventType,
                                                      void *eventData) {
  auto *event = static_cast<struct chreWifiRangingEvent *>(eventData);
  EventLoopManagerSingleton::get()->getWifiRequestManager()
      .mPlatformWifi.releaseRangingEvent(event);
}

}  // namespace chre
