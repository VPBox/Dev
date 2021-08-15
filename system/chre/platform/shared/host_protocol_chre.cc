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

#include "chre/platform/shared/host_protocol_chre.h"

#include <inttypes.h>
#include <string.h>

#include "chre/platform/log.h"
#include "chre/platform/shared/host_messages_generated.h"

using flatbuffers::FlatBufferBuilder;
using flatbuffers::Offset;
using flatbuffers::Vector;

namespace chre {

// This is similar to getStringFromByteVector in host_protocol_host.h. Ensure
// that method's implementation is kept in sync with this.
const char *getStringFromByteVector(const flatbuffers::Vector<int8_t> *vec) {
  constexpr int8_t kNullChar = static_cast<int8_t>('\0');
  const char *str = nullptr;

  // Check that the vector is present, non-empty, and null-terminated
  if (vec != nullptr && vec->size() > 0 && (*vec)[vec->size() - 1] == kNullChar) {
    str = reinterpret_cast<const char *>(vec->Data());
  }

  return str;
}

bool HostProtocolChre::decodeMessageFromHost(const void *message,
                                             size_t messageLen) {
  bool success = verifyMessage(message, messageLen);
  if (!success) {
    LOGE("Dropping invalid/corrupted message from host (length %zu)",
         messageLen);
  } else {
    const fbs::MessageContainer *container = fbs::GetMessageContainer(message);
    uint16_t hostClientId = container->host_addr()->client_id();

    switch (container->message_type()) {
      case fbs::ChreMessage::NanoappMessage: {
        const auto *nanoappMsg = static_cast<const fbs::NanoappMessage *>(
            container->message());
        // Required field; verifier ensures that this is not null (though it
        // may be empty)
        const flatbuffers::Vector<uint8_t> *msgData = nanoappMsg->message();
        HostMessageHandlers::handleNanoappMessage(
            nanoappMsg->app_id(), nanoappMsg->message_type(),
            nanoappMsg->host_endpoint(), msgData->data(), msgData->size());
        break;
      }

      case fbs::ChreMessage::HubInfoRequest:
        HostMessageHandlers::handleHubInfoRequest(hostClientId);
        break;

      case fbs::ChreMessage::NanoappListRequest:
        HostMessageHandlers::handleNanoappListRequest(hostClientId);
        break;

      case fbs::ChreMessage::LoadNanoappRequest: {
        const auto *request = static_cast<const fbs::LoadNanoappRequest *>(
            container->message());
        const flatbuffers::Vector<uint8_t> *appBinary = request->app_binary();
        const char *appBinaryFilename = getStringFromByteVector(
            request->app_binary_file_name());
        HostMessageHandlers::handleLoadNanoappRequest(
            hostClientId, request->transaction_id(), request->app_id(),
            request->app_version(), request->target_api_version(),
            appBinary->data(), appBinary->size(), appBinaryFilename,
            request->fragment_id(), request->total_app_size());
        break;
      }

      case fbs::ChreMessage::UnloadNanoappRequest: {
        const auto *request = static_cast<const fbs::UnloadNanoappRequest *>(
            container->message());
        HostMessageHandlers::handleUnloadNanoappRequest(
            hostClientId, request->transaction_id(), request->app_id(),
            request->allow_system_nanoapp_unload());
        break;
      }

      case fbs::ChreMessage::TimeSyncMessage: {
        const auto *request = static_cast<const fbs::TimeSyncMessage *>(
            container->message());
        HostMessageHandlers::handleTimeSyncMessage(request->offset());
        break;
      }

      case fbs::ChreMessage::DebugDumpRequest:
        HostMessageHandlers::handleDebugDumpRequest(hostClientId);
        break;

      default:
        LOGW("Got invalid/unexpected message type %" PRIu8,
             static_cast<uint8_t>(container->message_type()));
        success = false;
    }
  }

  return success;
}

void HostProtocolChre::encodeHubInfoResponse(
    FlatBufferBuilder& builder, const char *name, const char *vendor,
    const char *toolchain, uint32_t legacyPlatformVersion,
    uint32_t legacyToolchainVersion, float peakMips, float stoppedPower,
    float sleepPower, float peakPower, uint32_t maxMessageLen,
    uint64_t platformId, uint32_t version, uint16_t hostClientId) {
  auto nameOffset = addStringAsByteVector(builder, name);
  auto vendorOffset = addStringAsByteVector(builder, vendor);
  auto toolchainOffset = addStringAsByteVector(builder, toolchain);

  auto response = fbs::CreateHubInfoResponse(
      builder, nameOffset, vendorOffset, toolchainOffset, legacyPlatformVersion,
      legacyToolchainVersion, peakMips, stoppedPower, sleepPower, peakPower,
      maxMessageLen, platformId, version);
  finalize(builder, fbs::ChreMessage::HubInfoResponse, response.Union(),
           hostClientId);
}

void HostProtocolChre::addNanoappListEntry(
    FlatBufferBuilder& builder,
    DynamicVector<Offset<fbs::NanoappListEntry>>& offsetVector,
    uint64_t appId, uint32_t appVersion, bool enabled, bool isSystemNanoapp) {
  auto offset = fbs::CreateNanoappListEntry(
      builder, appId, appVersion, enabled, isSystemNanoapp);
  if (!offsetVector.push_back(offset)) {
    LOGE("Couldn't push nanoapp list entry offset!");
  }
}

void HostProtocolChre::finishNanoappListResponse(
    FlatBufferBuilder& builder,
    DynamicVector<Offset<fbs::NanoappListEntry>>& offsetVector,
    uint16_t hostClientId) {
  auto vectorOffset = builder.CreateVector<Offset<fbs::NanoappListEntry>>(
      offsetVector);
  auto response = fbs::CreateNanoappListResponse(builder, vectorOffset);
  finalize(builder, fbs::ChreMessage::NanoappListResponse, response.Union(),
           hostClientId);
}

void HostProtocolChre::encodeLoadNanoappResponse(
    flatbuffers::FlatBufferBuilder& builder, uint16_t hostClientId,
    uint32_t transactionId, bool success, uint32_t fragmentId) {
  auto response = fbs::CreateLoadNanoappResponse(builder, transactionId,
                                                 success, fragmentId);
  finalize(builder, fbs::ChreMessage::LoadNanoappResponse, response.Union(),
           hostClientId);
}

void HostProtocolChre::encodeUnloadNanoappResponse(
    flatbuffers::FlatBufferBuilder& builder, uint16_t hostClientId,
    uint32_t transactionId, bool success) {
  auto response = fbs::CreateUnloadNanoappResponse(builder, transactionId,
                                                   success);
  finalize(builder, fbs::ChreMessage::UnloadNanoappResponse, response.Union(),
           hostClientId);
}

void HostProtocolChre::encodeLogMessages(
    flatbuffers::FlatBufferBuilder& builder, const char *logBuffer,
    size_t bufferSize) {
  auto logBufferOffset = builder.CreateVector(
      reinterpret_cast<const int8_t *>(logBuffer), bufferSize);
  auto message = fbs::CreateLogMessage(builder, logBufferOffset);
  finalize(builder, fbs::ChreMessage::LogMessage, message.Union());
}

void HostProtocolChre::encodeDebugDumpData(
    flatbuffers::FlatBufferBuilder& builder, uint16_t hostClientId,
    const char *debugStr, size_t debugStrSize) {
  auto debugStrOffset = builder.CreateVector(
      reinterpret_cast<const int8_t *>(debugStr), debugStrSize);
  auto message = fbs::CreateDebugDumpData(builder, debugStrOffset);
  finalize(builder, fbs::ChreMessage::DebugDumpData, message.Union(),
           hostClientId);
}

void HostProtocolChre::encodeDebugDumpResponse(
      flatbuffers::FlatBufferBuilder& builder, uint16_t hostClientId,
      bool success, uint32_t dataCount) {
  auto response = fbs::CreateDebugDumpResponse(builder, success, dataCount);
  finalize(builder, fbs::ChreMessage::DebugDumpResponse, response.Union(),
           hostClientId);
}

void HostProtocolChre::encodeTimeSyncRequest(
    flatbuffers::FlatBufferBuilder& builder) {
  auto request = fbs::CreateTimeSyncRequest(builder);
  finalize(builder, fbs::ChreMessage::TimeSyncRequest, request.Union());
}

void HostProtocolChre::encodeLowPowerMicAccessRequest(
    flatbuffers::FlatBufferBuilder& builder) {
  auto request = fbs::CreateLowPowerMicAccessRequest(builder);
  finalize(builder, fbs::ChreMessage::LowPowerMicAccessRequest,
           request.Union());
}

void HostProtocolChre::encodeLowPowerMicAccessRelease(
    flatbuffers::FlatBufferBuilder& builder) {
  auto request = fbs::CreateLowPowerMicAccessRelease(builder);
  finalize(builder, fbs::ChreMessage::LowPowerMicAccessRelease,
           request.Union());
}

}  // namespace chre
