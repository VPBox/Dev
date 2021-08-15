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

#include <cinttypes>
#include <type_traits>

#include "chre/core/event_loop_manager.h"
#include "chre/core/host_comms_manager.h"
#include "chre/platform/assert.h"
#include "chre/platform/host_link.h"

namespace chre {

constexpr uint32_t kMessageToHostReservedFieldValue = UINT32_MAX;

void HostCommsManager::flushMessagesSentByNanoapp(uint64_t appId) {
  mHostLink.flushMessagesSentByNanoapp(appId);
}

bool HostCommsManager::sendMessageToHostFromNanoapp(
    Nanoapp *nanoapp, void *messageData, size_t messageSize,
    uint32_t messageType, uint16_t hostEndpoint,
    chreMessageFreeFunction *freeCallback) {
  bool success = false;
  if (messageSize > 0 && messageData == nullptr) {
    LOGW("Rejecting malformed message (null data but non-zero size)");
  } else if (messageSize > CHRE_MESSAGE_TO_HOST_MAX_SIZE) {
    LOGW("Rejecting message of size %zu bytes (max %d)",
         messageSize, CHRE_MESSAGE_TO_HOST_MAX_SIZE);
  } else if (hostEndpoint == kHostEndpointUnspecified) {
    LOGW("Rejecting message to invalid host endpoint");
  } else {
    MessageToHost *msgToHost = mMessagePool.allocate();

    if (msgToHost == nullptr) {
      LOG_OOM();
    } else {
      msgToHost->appId = nanoapp->getAppId();
      msgToHost->message.wrap(static_cast<uint8_t *>(messageData), messageSize);
      msgToHost->toHostData.hostEndpoint = hostEndpoint;
      msgToHost->toHostData.messageType = messageType;
      msgToHost->toHostData.nanoappFreeFunction = freeCallback;

      // Populate a special value to help disambiguate message direction when
      // debugging
      msgToHost->toHostData.reserved = kMessageToHostReservedFieldValue;

      success = mHostLink.sendMessage(msgToHost);
      if (!success) {
        mMessagePool.deallocate(msgToHost);
      }
    }
  }

  return success;
}

void HostCommsManager::deliverNanoappMessageFromHost(
    uint64_t appId, uint16_t hostEndpoint, uint32_t messageType,
    const void *messageData, uint32_t messageSize, uint32_t targetInstanceId) {
  bool success = false;

  MessageFromHost *msgFromHost = mMessagePool.allocate();
  if (msgFromHost == nullptr) {
    LOG_OOM();
  } else if (!msgFromHost->message.copy_array(
      static_cast<const uint8_t *>(messageData), messageSize)) {
    LOGE("Couldn't allocate %" PRIu32 " bytes for message data from host "
             "(endpoint 0x%" PRIx16 " type %" PRIu32 ")", messageSize,
         hostEndpoint, messageType);
  } else {
    msgFromHost->appId = appId;
    msgFromHost->fromHostData.messageType = messageType;
    msgFromHost->fromHostData.messageSize = static_cast<uint32_t>(
        messageSize);
    msgFromHost->fromHostData.message = msgFromHost->message.data();
    msgFromHost->fromHostData.hostEndpoint = hostEndpoint;

    success = EventLoopManagerSingleton::get()->getEventLoop().postEventOrDie(
        CHRE_EVENT_MESSAGE_FROM_HOST, &msgFromHost->fromHostData,
        freeMessageFromHostCallback, targetInstanceId);
  }

  if (!success && msgFromHost != nullptr) {
    mMessagePool.deallocate(msgFromHost);
  }
}

void HostCommsManager::sendMessageToNanoappFromHost(
    uint64_t appId, uint32_t messageType, uint16_t hostEndpoint,
    const void *messageData, size_t messageSize) {
  const EventLoop& eventLoop = EventLoopManagerSingleton::get()
      ->getEventLoop();
  uint32_t targetInstanceId;

  if (hostEndpoint == kHostEndpointBroadcast) {
    LOGE("Received invalid message from host from broadcast endpoint");
  } else if (messageSize > ((UINT32_MAX))) {
    // The current CHRE API uses uint32_t to represent the message size in
    // struct chreMessageFromHostData. We don't expect to ever need to exceed
    // this, but the check ensures we're on the up and up.
    LOGE("Rejecting message of size %zu (too big)", messageSize);
  } else if (!eventLoop.findNanoappInstanceIdByAppId(appId,
                                                     &targetInstanceId)) {
    LOGE("Dropping message; destination app ID 0x%016" PRIx64 " not found",
         appId);
  } else {
    deliverNanoappMessageFromHost(appId, hostEndpoint, messageType, messageData,
                                  static_cast<uint32_t>(messageSize),
                                  targetInstanceId);
  }
}

void HostCommsManager::onMessageToHostComplete(const MessageToHost *message) {
  // Removing const on message since we own the memory and will deallocate it;
  // the caller (HostLink) only gets a const pointer
  auto *msgToHost = const_cast<MessageToHost *>(message);

  // If there's no free callback, we can free the message right away as the
  // message pool is thread-safe; otherwise, we need to do it from within the
  // EventLoop context.
  if (msgToHost->toHostData.nanoappFreeFunction == nullptr) {
    mMessagePool.deallocate(msgToHost);
  } else {
    auto freeMsgCallback = [](uint16_t /*type*/, void *data) {
      EventLoopManagerSingleton::get()->getHostCommsManager().freeMessageToHost(
          static_cast<MessageToHost *>(data));
    };

    EventLoopManagerSingleton::get()->deferCallback(
        SystemCallbackType::MessageToHostComplete, msgToHost, freeMsgCallback);
  }
}

void HostCommsManager::freeMessageToHost(MessageToHost *msgToHost) {
  if (msgToHost->toHostData.nanoappFreeFunction != nullptr) {
    EventLoopManagerSingleton::get()->getEventLoop().invokeMessageFreeFunction(
        msgToHost->appId, msgToHost->toHostData.nanoappFreeFunction,
        msgToHost->message.data(), msgToHost->message.size());
  }
  mMessagePool.deallocate(msgToHost);
}

void HostCommsManager::freeMessageFromHostCallback(uint16_t /*type*/,
                                                   void *data) {
  // We pass the chreMessageFromHostData structure to the nanoapp as the event's
  // data pointer, but we need to return to the enclosing HostMessage pointer.
  // As long as HostMessage is standard-layout, and fromHostData is the first
  // field, we can convert between these two pointers via reinterpret_cast.
  // These static assertions ensure this assumption is held.
  static_assert(std::is_standard_layout<HostMessage>::value,
                "HostMessage* is derived from HostMessage::fromHostData*, "
                "therefore it must be standard layout");
  static_assert(offsetof(MessageFromHost, fromHostData) == 0,
                "fromHostData must be the first field in HostMessage");

  auto *eventData = static_cast<chreMessageFromHostData *>(data);
  auto *msgFromHost = reinterpret_cast<MessageFromHost *>(eventData);
  auto& hostCommsMgr = EventLoopManagerSingleton::get()->getHostCommsManager();
  hostCommsMgr.mMessagePool.deallocate(msgFromHost);
}


}  // namespace chre
