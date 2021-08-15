
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

#ifndef CHRE_HOST_HOST_PROTOCOL_HOST_H_
#define CHRE_HOST_HOST_PROTOCOL_HOST_H_

#include <stdint.h>

#include "chre/platform/shared/host_protocol_common.h"
#include "chre_host/fragmented_load_transaction.h"
#include "chre_host/host_messages_generated.h"
#include "flatbuffers/flatbuffers.h"

#include <vector>

namespace android {
namespace chre {

/**
 * Checks that a string encapsulated as a byte vector is null-terminated, and
 * if it is, returns a pointer to the vector's data. Otherwise returns null.
 *
 * This is similar to getStringFromByteVector in host_protocol_chre.h. Ensure
 * that method's implementation is kept in sync with this.
 *
 * @param vec Target vector, can be null
 *
 * @return Pointer to the vector's data, or null
 */
const char *getStringFromByteVector(const std::vector<int8_t>& vec);

/**
 * Calling code should provide an implementation of this interface to handle
 * parsed results from decodeMessageFromChre().
 */
class IChreMessageHandlers {
 public:
  virtual ~IChreMessageHandlers() = default;

  virtual void handleNanoappMessage(
      const ::chre::fbs::NanoappMessageT& /*message*/) {};

  virtual void handleHubInfoResponse(
      const ::chre::fbs::HubInfoResponseT& /*response*/) {};

  virtual void handleNanoappListResponse(
      const ::chre::fbs::NanoappListResponseT& /*response*/) {};

  virtual void handleLoadNanoappResponse(
      const ::chre::fbs::LoadNanoappResponseT& /*response*/) {};

  virtual void handleUnloadNanoappResponse(
      const ::chre::fbs::UnloadNanoappResponseT& /*response*/) {};

  virtual void handleDebugDumpData(
      const ::chre::fbs::DebugDumpDataT& /*data*/) {};

  virtual void handleDebugDumpResponse(
      const ::chre::fbs::DebugDumpResponseT& /*response*/) {};
};

/**
 * A set of helper methods that simplify the encode/decode of FlatBuffers
 * messages used in communication with CHRE from the host.
 */
class HostProtocolHost : public ::chre::HostProtocolCommon {
 public:
  /**
   * Decodes a message sent from CHRE and invokes the appropriate handler
   * function in the provided interface implementation to handle the parsed
   * result.
   *
   * @param message Buffer containing a complete FlatBuffers CHRE message
   * @param messageLen Size of the message, in bytes
   * @param handlers Set of callbacks to handle the parsed message. If this
   *        function returns success, then exactly one of these functions was
   *        called.
   *
   * @return true if the message was parsed successfully and passed to a handler
   */
  static bool decodeMessageFromChre(const void *message, size_t messageLen,
                                    IChreMessageHandlers& handlers);

  /**
   * Encodes a message requesting hub information from CHRE
   *
   * @param builder A newly constructed FlatBufferBuilder that will be used to
   *        construct the message
   */
  static void encodeHubInfoRequest(flatbuffers::FlatBufferBuilder& builder);

  /**
   * Encodes a message requesting to load a nanoapp specified by the included
   * (possibly fragmented) binary payload and metadata.
   *
   * @param builder A newly constructed FlatBufferBuilder that will be used to
   *        construct the message
   * @param request The FragmentedLoadRequest object with the binary and the
   *        metadata
   */
  static void encodeFragmentedLoadNanoappRequest(
      flatbuffers::FlatBufferBuilder& builder,
      const FragmentedLoadRequest& request);

  /**
   * Encodes a message requesting the list of loaded nanoapps from CHRE
   *
   * @param builder A newly constructed FlatBufferBuilder that will be used to
   *        construct the message
   */
  static void encodeNanoappListRequest(flatbuffers::FlatBufferBuilder& builder);

  /**
   * Encodes a message requesting to unload a nanoapp specified by app ID.
   *
   * @param builder A newly constructed FlatBufferBuilder that will be used to
   *        construct the message
   * @param transactionId A transaction identifier to tie the subsequent
   *        response to this request
   * @param appId Identifier for the app to unload
   * @param allowSystemNanoappUnload Whether this request should be allowed to
   *        result in unloading a system nanoapp (e.g. requests from the context
   *        hub HAL should have set this to false, as system nanoapps are not
   *        expected to be managed through that HAL)
   */
  static void encodeUnloadNanoappRequest(
      flatbuffers::FlatBufferBuilder& builder, uint32_t transactionId,
      uint64_t appId, bool allowSystemNanoappUnload);

  /**
   * Encodes a message to send the AP timestamp to CHRE
   *
   * @param builder A newly constructed FlatBufferBuilder that will be used to
   *        construct the message
   * @param offset The AP to SLPI offset in nanoseconds
   */
  static void encodeTimeSyncMessage(flatbuffers::FlatBufferBuilder& builder,
                                    int64_t offset);

  /**
   * Encodes a message requesting debugging information from CHRE
   *
   * @param builder A newly constructed FlatBufferBuilder that will be used to
   *        construct the message
   */
  static void encodeDebugDumpRequest(flatbuffers::FlatBufferBuilder& builder);

  /**
   * Decodes the host client ID included in the message container
   *
   * @param message Buffer containing a complete FlatBuffers CHRE message
   * @param messageLen Size of the message, in bytes
   * @param hostClientId Output parameter that will be populated with the client
   *        ID included in the message on success
   *
   * @return true if the host client ID was successfully decoded from the
   *         message
   */
  static bool extractHostClientIdAndType(
      const void *message, size_t messageLen, uint16_t *hostClientId,
      ::chre::fbs::ChreMessage *messageType);

  /**
   * Update the host client ID field in the MessageContainer.
   *
   * @param message Buffer containing a complete FlatBuffers CHRE message
   * @param messageLen Size of the message, in bytes
   * @param hostClientId The value to set the host client ID to
   *
   * @return true if the message was verified successfully, and we were able to
   *         modify the host client ID field
   */
  static bool mutateHostClientId(void *message, size_t messageLen,
                                 uint16_t hostClientId);

  /**
   * Encodes a message requesting to load a nanoapp specified by the included
   * binary payload and metadata.
   *
   * @param builder A newly constructed FlatBufferBuilder that will be used to
   *        construct the message
   */
  static void encodeLoadNanoappRequestForBinary(
      flatbuffers::FlatBufferBuilder& builder, uint32_t transactionId,
      uint64_t appId, uint32_t appVersion, uint32_t targetApiVersion,
      const std::vector<uint8_t>& nanoappBinary, uint32_t fragmentId,
      size_t appTotalSizeBytes);

  /**
   * Encodes a message requesting to load a nanoapp specified by the included
   * binary filename and metadata.
   *
   * @param builder A newly constructed FlatBufferBuilder that will be used to
   *        construct the message
   */
  static void encodeLoadNanoappRequestForFile(
      flatbuffers::FlatBufferBuilder& builder, uint32_t transactionId,
      uint64_t appId, uint32_t appVersion, uint32_t targetApiVersion,
      const char *nanoappBinaryName);
};

}  // namespace chre
}  // namespace android

#endif  // CHRE_HOST_HOST_PROTOCOL_HOST_H_
