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

#ifndef CHRE_CORE_HOST_COMMS_MANAGER_H_
#define CHRE_CORE_HOST_COMMS_MANAGER_H_

#include <cstddef>

#include "chre_api/chre/event.h"
#include "chre/core/event_loop.h"
#include "chre/platform/host_link.h"
#include "chre/util/buffer.h"
#include "chre/util/non_copyable.h"
#include "chre/util/synchronized_memory_pool.h"

namespace chre {

//! Only valid for messages from host to CHRE - indicates that the sender of the
//! message is not specified.
constexpr uint16_t kHostEndpointUnspecified = CHRE_HOST_ENDPOINT_UNSPECIFIED;

//! Only valid for messages from CHRE to host - delivers the message to all
//! registered clients of the Context Hub HAL, which is the default behavior.
constexpr uint16_t kHostEndpointBroadcast = CHRE_HOST_ENDPOINT_BROADCAST;

/**
 * Data associated with a message either to or from the host.
 */
struct HostMessage : public NonCopyable {
  // This union must be first, as this structure is aliased with
  // chreMessageFromHostData
  union {
    // Fields use when the message was received from the host
    struct chreMessageFromHostData fromHostData;

    // Fields used when the messsage is directed to the host
    struct {
      //! Application-specific message ID
      uint32_t messageType;

      //! Padding used to align this structure with chreMessageFromHostData
      uint32_t reserved;

      //! Message free callback supplied by the nanoapp. Must only be invoked from
      //! the EventLoop where the nanoapp runs.
      chreMessageFreeFunction *nanoappFreeFunction;

      //! Identifier for the host-side entity that should receive this message, or
      //! that which sent it
      uint16_t hostEndpoint;
    } toHostData;
  };

  //! Source/destination nanoapp ID
  uint64_t appId;

  //! Application-defined message data
  Buffer<uint8_t> message;
};

typedef HostMessage MessageFromHost;
typedef HostMessage MessageToHost;

/**
 * Manages bi-directional communications with the host. There must only be one
 * instance of this class per CHRE instance, as the HostLink is not multiplexed
 * per-EventLoop.
 */
class HostCommsManager : public NonCopyable {
 public:
  /**
   * @see HostLink::flushMessagesSentByNanoapp
   */
  void flushMessagesSentByNanoapp(uint64_t appId);

  /**
   * Formulates a MessageToHost using the supplied message contents and passes
   * it to HostLink for transmission to the host.
   *
   * @param nanoapp The sender of this message
   * @param messageData Pointer to message payload. Can be null if messageSize
   *        is 0. This buffer must remain valid until freeCallback is invoked.
   * @param messageSize Size of the message to send, in bytes
   * @param messageType Application-defined identifier for the message
   * @param hostEndpoint Identifier for the entity on the host that should
   *        receive this message
   * @param freeCallback Optional callback to invoke when the messageData is no
   *        longer needed (the message has been sent or an error occurred)
   *
   * @return true if the message was accepted into the outbound message queue.
   *         If this function returns false, it does *not* invoke freeCallback.
   *         If it returns true, freeCallback will be invoked (if non-null) on
   *         either success or failure.
   *
   * @see chreSendMessageToHost
   */
  bool sendMessageToHostFromNanoapp(
      Nanoapp *nanoapp, void *messageData, size_t messageSize,
      uint32_t messageType, uint16_t hostEndpoint,
      chreMessageFreeFunction *freeCallback);

  /**
   * Makes a copy of the supplied message data and posts it to the queue for
   * later delivery to the addressed nanoapp.
   *
   * This function is safe to call from any thread.
   *
   * @param appId Identifier for the destination nanoapp
   * @param messageType Application-defined message identifier
   * @param hostEndpoint Identifier for the entity on the host that sent this
   *        message
   * @param messageData Buffer containing application-specific message data; can
   *        be null if messageSize is 0
   * @param messageSize Size of messageData, in bytes
   */
  void sendMessageToNanoappFromHost(
      uint64_t appId, uint32_t messageType, uint16_t hostEndpoint,
      const void *messageData, size_t messageSize);

  /**
   * Invoked by the HostLink platform layer when it is done with a message to
   * the host: either it successfully sent it, or encountered an error.
   *
   * This function is thread-safe.
   *
   * @param message A message pointer previously given to HostLink::sendMessage
   */
  void onMessageToHostComplete(const MessageToHost *msgToHost);

 private:
  //! The maximum number of messages we can have outstanding at any given time
  static constexpr size_t kMaxOutstandingMessages = 32;

  //! Memory pool used to allocate message metadata (but not the contents of the
  //! messages themselves). Must be synchronized as the same HostCommsManager
  //! handles communications for all EventLoops, and also to support freeing
  //! messages directly in onMessageToHostComplete.
  SynchronizedMemoryPool<HostMessage, kMaxOutstandingMessages> mMessagePool;

  //! The platform-specific link to the host that we manage
  HostLink mHostLink;

  /**
   * Allocates and populates the event structure used to notify a nanoapp of an
   * incoming message from the host, and posts an event to the nanoapp for
   * processing. Used to implement sendMessageToNanoappFromHost() - see that
   * function for parameter documentation.
   *
   * All parameters must be sanitized before invoking this function.
   *
   * @param targetInstanceId Instance ID of the destination nanoapp
   *
   * @see sendMessageToNanoappFromHost
   */
  void deliverNanoappMessageFromHost(
      uint64_t appId, uint16_t hostEndpoint, uint32_t messageType,
      const void *messageData, uint32_t messageSize, uint32_t targetInstanceId);

  /**
   * Releases memory associated with a message to the host, including invoking
   * the Nanoapp's free callback (if given). Must be called from within the
   * context of the EventLoop that contains the sending Nanoapp.
   *
   * @param msgToHost The message to free
   */
  void freeMessageToHost(MessageToHost *msgToHost);

  /**
   * Event free callback used to release memory allocated to deliver a message
   * to a nanoapp from the host.
   *
   * @param type Event type
   * @param data Event data
   */
  static void freeMessageFromHostCallback(uint16_t type, void *data);
};

} // namespace chre

#endif  // CHRE_CORE_HOST_COMMS_MANAGER_H_
