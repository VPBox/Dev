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

#ifndef CHRE_PLATFORM_SHARED_HOST_PROTOCOL_CHRE_H_
#define CHRE_PLATFORM_SHARED_HOST_PROTOCOL_CHRE_H_

#include <stdint.h>

#include "chre/platform/shared/host_messages_generated.h"
#include "chre/platform/shared/host_protocol_common.h"
#include "flatbuffers/flatbuffers.h"

namespace chre {

typedef flatbuffers::Offset<fbs::NanoappListEntry> NanoappListEntryOffset;

/**
 * Checks that a string encapsulated as a byte vector is null-terminated, and
 * if it is, returns a pointer to the vector's data. Otherwise returns null.
 *
 * This is similar to getStringFromByteVector in host_protocol_host.h. Ensure
 * that method's implementation is kept in sync with this.
 *
 * @param vec Target vector, can be null
 *
 * @return Pointer to the vector's data, or null
 */
const char *getStringFromByteVector(const flatbuffers::Vector<int8_t> *vec);

/**
 * These methods are called from decodeMessageFromHost() and must be implemented
 * by the code that calls it to handle parsed messages.
 */
class HostMessageHandlers {
 public:
  static void handleNanoappMessage(
    uint64_t appId, uint32_t messageType, uint16_t hostEndpoint,
    const void *messageData, size_t messageDataLen);

  static void handleHubInfoRequest(uint16_t hostClientId);

  static void handleNanoappListRequest(uint16_t hostClientId);

  static void handleLoadNanoappRequest(
      uint16_t hostClientId, uint32_t transactionId, uint64_t appId,
      uint32_t appVersion, uint32_t targetApiVersion, const void *buffer,
      size_t bufferLen, const char *appFileName, uint32_t fragmentId,
      size_t appBinaryLen);

  static void handleUnloadNanoappRequest(
      uint16_t hostClientId, uint32_t transactionId, uint64_t appId,
      bool allowSystemNanoappUnload);

  static void handleTimeSyncMessage(int64_t offset);

  static void handleDebugDumpRequest(uint16_t hostClientId);
};

/**
 * A set of helper methods that simplify the encode/decode of FlatBuffers
 * messages used in communications with the host from CHRE.
 */
class HostProtocolChre : public HostProtocolCommon {
 public:
  /**
   * Verifies and decodes a FlatBuffers-encoded CHRE message.
   *
   * @param message Buffer containing message
   * @param messageLen Size of the message, in bytes
   * @param handlers Contains callbacks to process a decoded message
   *
   * @return bool true if the message was successfully decoded, false if it was
   *         corrupted/invalid/unrecognized
   */
  static bool decodeMessageFromHost(const void *message, size_t messageLen);

  /**
   * Refer to the context hub HAL definition for a details of these parameters.
   *
   * @param builder A newly constructed FlatBufferBuilder that will be used to
   *        encode the message
   */
  static void encodeHubInfoResponse(
      flatbuffers::FlatBufferBuilder& builder, const char *name,
      const char *vendor, const char *toolchain, uint32_t legacyPlatformVersion,
      uint32_t legacyToolchainVersion, float peakMips, float stoppedPower,
      float sleepPower, float peakPower, uint32_t maxMessageLen,
      uint64_t platformId, uint32_t version, uint16_t hostClientId);

  /**
   * Supports construction of a NanoappListResponse by adding a single
   * NanoappListEntry to the response. The offset for the newly added entry is
   * maintained in the given vector until finishNanoappListResponse() is called.
   * Example usage:
   *
   *   FlatBufferBuilder builder;
   *   DynamicVector<NanoappListEntryOffset> vector;
   *   for (auto app : appList) {
   *     HostProtocolChre::addNanoppListEntry(builder, vector, ...);
   *   }
   *   HostProtocolChre::finishNanoappListResponse(builder, vector);
   *
   * @param builder A FlatBufferBuilder to use for encoding the message
   * @param offsetVector A vector to track the offset to the newly added
   *        NanoappListEntry, which be passed to finishNanoappListResponse()
   *        once all entries are added
   */
  static void addNanoappListEntry(
      flatbuffers::FlatBufferBuilder& builder,
      DynamicVector<NanoappListEntryOffset>& offsetVector,
      uint64_t appId, uint32_t appVersion, bool enabled, bool isSystemNanoapp);

  /**
   * Finishes encoding a NanoappListResponse message after all NanoappListEntry
   * elements have already been added to the builder.
   *
   * @param builder The FlatBufferBuilder used with addNanoappListEntry()
   * @param offsetVector The vector used with addNanoappListEntry()
   * @param hostClientId
   *
   * @see addNanoappListEntry()
   */
  static void finishNanoappListResponse(
      flatbuffers::FlatBufferBuilder& builder,
      DynamicVector<NanoappListEntryOffset>& offsetVector,
      uint16_t hostClientId);

  /**
   * Encodes a response to the host communicating the result of dynamically
   * loading a nanoapp.
   */
  static void encodeLoadNanoappResponse(
      flatbuffers::FlatBufferBuilder& builder, uint16_t hostClientId,
      uint32_t transactionId, bool success, uint32_t fragmentId);

  /**
   * Encodes a response to the host communicating the result of dynamically
   * unloading a nanoapp.
   */
  static void encodeUnloadNanoappResponse(
      flatbuffers::FlatBufferBuilder& builder, uint16_t hostClientId,
      uint32_t transactionId, bool success);

  /**
   * Encodes a buffer of log messages to the host.
   */
  static void encodeLogMessages(
      flatbuffers::FlatBufferBuilder& builder, const char *logBuffer,
      size_t bufferSize);

  /**
   * Encodes a string into a DebugDumpData message.
   *
   * @param debugStr Null-terminated ASCII string containing debug information
   * @param debugStrSize Size of the debugStr buffer, including null termination
   */
  static void encodeDebugDumpData(
      flatbuffers::FlatBufferBuilder& builder, uint16_t hostClientId,
      const char *debugStr, size_t debugStrSize);

  /**
   * Encodes the final response to a debug dump request.
   */
  static void encodeDebugDumpResponse(
      flatbuffers::FlatBufferBuilder& builder, uint16_t hostClientId,
      bool success, uint32_t dataCount);

  /**
   * Encodes a message requesting time sync from host.
   */
  static void encodeTimeSyncRequest(flatbuffers::FlatBufferBuilder& builder);

  /**
   * Encodes a message notifying the host that audio has been requested by a
   * nanoapp, so the low-power microphone needs to be powered on.
   */
  static void encodeLowPowerMicAccessRequest(
      flatbuffers::FlatBufferBuilder& builder);

  /**
   * Encodes a message notifying the host that no nanoapps are requesting audio
   * anymore, so the low-power microphone may be powered off.
   */
  static void encodeLowPowerMicAccessRelease(
      flatbuffers::FlatBufferBuilder& builder);
};

}  // namespace chre

#endif  // CHRE_PLATFORM_SHARED_HOST_PROTOCOL_CHRE_H_
