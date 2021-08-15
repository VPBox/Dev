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

#include <shared/send_message.h>

#include <inttypes.h>

#include <shared/abort.h>
#include <shared/dumb_allocator.h>
#include <shared/nano_endian.h>
#include <shared/nano_string.h>

#include <chre.h>

namespace nanoapp_testing {

constexpr size_t kAllocSize = 128;

static DumbAllocator<kAllocSize, 4> gDumbAlloc;

static void freeDumbAllocMessage(void *message, size_t messageSize) {
  if (messageSize > kAllocSize) {
    uint32_t localSize = uint32_t(messageSize);
    sendFatalFailureToHost("freeDumbAllocMessage given oversized message:",
                           &localSize);
  }
  if (!gDumbAlloc.free(message)) {
    uint32_t localPtr =
        reinterpret_cast<size_t>(message) & UINT32_C(0xFFFFFFFF);
    sendFatalFailureToHost("freeDumbAllocMessage given bad pointer:",
                           &localPtr);
  }
}

static void freeHeapMessage(void *message, size_t /* messageSize */) {
  if (gDumbAlloc.contains(message)) {
    uint32_t localPtr =
        reinterpret_cast<size_t>(message) & UINT32_C(0xFFFFFFFF);
    sendFatalFailureToHost("freeHeapMessage given DumbAlloc pointer:",
                           &localPtr);
  }
  chreHeapFree(message);
}

static void fatalError() {
  // Attempt to send a context-less failure message, in the hopes that
  // might get through.
  chreSendMessageToHost(nullptr, 0,
                        static_cast<uint32_t>(MessageType::kFailure),
                        nullptr);
  // Whether or not that made it through, unambigiously fail this test
  // by aborting.
  nanoapp_testing::abort();
}

// TODO(b/32114261): Remove this method.
static bool needToPrependMessageType() {
  // TODO: When we have a new API that properly send the messageType,
  //     this method should get the API version and return appropriately.
  //     Eventually we should remove this hacky method.
  return true;
}

static void *getMessageMemory(size_t *size, bool *dumbAlloc) {
  if (needToPrependMessageType()) {
    *size += sizeof(uint32_t);
  }
  void *ret = gDumbAlloc.alloc(*size);
  if (ret != nullptr) {
    *dumbAlloc = true;
  } else {
    // Not expected, but possible if the CHRE is lagging in freeing
    // these messages, or if we're sending a huge message.
    *dumbAlloc = false;
    ret = chreHeapAlloc(static_cast<uint32_t>(*size));
    if (ret == nullptr) {
      fatalError();
    }
  }
  return ret;
}

// TODO(b/32114261): Remove this method.
static void *prependMessageType(MessageType messageType, void *memory) {
  if (!needToPrependMessageType()) {
    return memory;
  }
  uint32_t type = nanoapp_testing::hostToLittleEndian(
      static_cast<uint32_t>(messageType));
  memcpy(memory, &type, sizeof(type));
  uint8_t *ptr = static_cast<uint8_t*>(memory);
  ptr += sizeof(type);
  return ptr;
}

static void internalSendMessage(MessageType messageType, void *data,
                                size_t dataSize, bool dumbAlloc) {
  // Note that if the CHRE implementation occasionally drops a message
  // here, then tests will become flaky.  For now, we consider that to
  // be a flaky CHRE implementation which should fail testing.
  if (!chreSendMessageToHostEndpoint(data, dataSize,
                                     static_cast<uint32_t>(messageType),
                                     CHRE_HOST_ENDPOINT_BROADCAST,
                                     dumbAlloc ? freeDumbAllocMessage :
                                     freeHeapMessage)) {
    fatalError();
  }
}

void sendMessageToHost(MessageType messageType, const void *data,
                       size_t dataSize) {
  if ((dataSize == 0) && (data != nullptr)) {
    sendInternalFailureToHost("Bad sendMessageToHost args");
  }
  bool dumbAlloc = true;
  size_t fullMessageSize = dataSize;
  void *myMessageBase = getMessageMemory(&fullMessageSize, &dumbAlloc);
  void *ptr = prependMessageType(messageType, myMessageBase);
  memcpy(ptr, data, dataSize);
  internalSendMessage(messageType, myMessageBase, fullMessageSize, dumbAlloc);
}

void sendStringToHost(MessageType messageType, const char *message,
                      const uint32_t *value) {
  if (message == nullptr) {
    sendInternalFailureToHost("sendStringToHost 'message' is NULL");
  }
  bool dumbAlloc = true;
  const size_t messageStrlen = strlen(message);
  size_t myMessageLen = messageStrlen;
  if (value != nullptr) {
    myMessageLen += kUint32ToHexAsciiBufferMinLen;
  }
  // Add null terminator
  myMessageLen++;

  size_t fullMessageLen = myMessageLen;
  char *fullMessage =
      static_cast<char*>(getMessageMemory(&fullMessageLen, &dumbAlloc));
  char *ptr = static_cast<char*>(prependMessageType(messageType,
                                                    fullMessage));
  memcpy(ptr, message, messageStrlen);
  ptr += messageStrlen;
  if (value != nullptr) {
    uint32ToHexAscii(
        ptr, fullMessageLen - static_cast<size_t>(ptr - fullMessage), *value);
  }
  // Add the terminator.
  fullMessage[fullMessageLen - 1] = '\0';

  internalSendMessage(messageType, fullMessage, fullMessageLen, dumbAlloc);
}

// Before we abort the nanoapp, we also put this message in the chreLog().
// We have no assurance our message will make it to the Host (not required
// for CHRE implementations), but this will at least make sure our message
// hits the log.
static void logFatalMessage(const char *message, const uint32_t *value) {
  if (value != nullptr) {
    chreLog(CHRE_LOG_ERROR, "TEST ABORT: %s0x%08" PRIX32, message, *value);
  } else {
    chreLog(CHRE_LOG_ERROR, "TEST ABORT: %s", message);
  }
}

void sendFatalFailureToHost(const char *message, const uint32_t *value,
                            AbortBlame reason) {
  sendFailureToHost(message, value);
  logFatalMessage(message, value);
  nanoapp_testing::abort(reason);
}

void sendFatalFailureToHostUint8(const char *message, uint8_t value) {
  uint32_t val = value;
  sendFatalFailureToHost(message, &val);
}

void sendInternalFailureToHost(const char *message, const uint32_t *value,
                               AbortBlame reason) {
  sendStringToHost(MessageType::kInternalFailure, message, value);
  logFatalMessage(message, value);
  nanoapp_testing::abort(reason);
}

}  // namespace nanoapp_testing
