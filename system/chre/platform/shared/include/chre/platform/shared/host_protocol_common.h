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

#ifndef CHRE_PLATFORM_SHARED_HOST_PROTOCOL_COMMON_H_
#define CHRE_PLATFORM_SHARED_HOST_PROTOCOL_COMMON_H_

#include <stdint.h>

#include "flatbuffers/flatbuffers.h"

namespace chre {

namespace fbs {

// Forward declaration of the ChreMessage enum defined in the generated
// FlatBuffers header file
enum class ChreMessage : uint8_t;

}  // namespace fbs

//! On a message sent from CHRE, specifies that the host daemon should determine
//! which client to send the message to. Usually, this is all clients, but for a
//! message from a nanoapp, the host daemon can use the endpoint ID to determine
//! the destination client ID.
constexpr uint16_t kHostClientIdUnspecified = 0;

/**
 * Functions that are shared between the CHRE and host to assist with
 * communications between the two. Note that normally these functions are
 * accessed through a derived class like chre::HostProtocolChre (CHRE-side) or
 * android::chre:HostProtocolHost (host-side).
 */
class HostProtocolCommon {
 public:
  /**
   * Encodes a message to/from a nanoapp using the given FlatBufferBuilder and
   * supplied parameters.
   *
   * @param builder A newly constructed FlatBufferBuilder that will be used to
   *        encode the message. It will be finalized before returning from this
   *        function.
   */
  static void encodeNanoappMessage(
      flatbuffers::FlatBufferBuilder& builder, uint64_t appId,
      uint32_t messageType, uint16_t hostEndpoint, const void *messageData,
      size_t messageDataLen);

  /**
   * Adds a string to the provided builder as a byte vector.
   *
   * @param builder The builder to add the string to.
   * @param str The string to add.
   * @return The offset in the builder that the string is stored at.
   */
  static flatbuffers::Offset<flatbuffers::Vector<int8_t>>
      addStringAsByteVector(flatbuffers::FlatBufferBuilder& builder,
                            const char *str);

   /**
    * Constructs the message container and finalizes the FlatBufferBuilder
    *
    * @param builder The FlatBufferBuilder that was used to construct the
    *        message prior to adding the container
    * @param messageType Type of message that was constructed
    * @param message Offset of the message to include (normally the return value
    *        of flatbuffers::Offset::Union() on the message offset)
    * @param hostClientId The source/client ID of the host-side entity that
    *        sent/should receive this message. Leave unspecified (default 0)
    *        when constructing a message on the host, as this field will be
    *        set before the message is sent to CHRE.
    */
   static void finalize(
       flatbuffers::FlatBufferBuilder& builder, fbs::ChreMessage messageType,
       flatbuffers::Offset<void> message,
       uint16_t hostClientId = kHostClientIdUnspecified);

   /**
    * Verifies that the provided message contains a valid flatbuffers CHRE
    * protocol message,
    *
    * @param message The message to validate.
    * @param length The size of the message to validate.
    * @return true if the message is valid, false otherwise.
    */
   static bool verifyMessage(const void *message, size_t messageLen);
};

}  // namespace chre

#endif  // CHRE_PLATFORM_SHARED_HOST_PROTOCOL_COMMON_H_
