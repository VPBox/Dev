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

#include "chre/platform/shared/host_protocol_common.h"

#include <string.h>

#include "chre/platform/shared/host_messages_generated.h"

using flatbuffers::FlatBufferBuilder;
using flatbuffers::Offset;
using flatbuffers::Vector;

namespace chre {

void HostProtocolCommon::encodeNanoappMessage(
    FlatBufferBuilder& builder, uint64_t appId, uint32_t messageType,
    uint16_t hostEndpoint, const void *messageData, size_t messageDataLen) {
  auto messageDataOffset = builder.CreateVector(
      static_cast<const uint8_t *>(messageData), messageDataLen);

  auto nanoappMessage = fbs::CreateNanoappMessage(
      builder, appId, messageType, hostEndpoint, messageDataOffset);
  finalize(builder, fbs::ChreMessage::NanoappMessage, nanoappMessage.Union());
}

Offset<Vector<int8_t>> HostProtocolCommon::addStringAsByteVector(
    FlatBufferBuilder& builder, const char *str) {
  return builder.CreateVector(reinterpret_cast<const int8_t *>(str),
                              strlen(str) + 1);
}

void HostProtocolCommon::finalize(
    FlatBufferBuilder& builder, fbs::ChreMessage messageType,
    flatbuffers::Offset<void> message, uint16_t hostClientId) {
  fbs::HostAddress hostAddr(hostClientId);
  auto container = fbs::CreateMessageContainer(
      builder, messageType, message, &hostAddr);
  builder.Finish(container);
}

bool HostProtocolCommon::verifyMessage(const void *message, size_t messageLen) {
  bool valid = false;

  if (message != nullptr) {
    flatbuffers::Verifier verifier(static_cast<const uint8_t *>(message),
                                   messageLen);

    valid = fbs::VerifyMessageContainerBuffer(verifier);
  }

  return valid;
}


}  // namespace chre
