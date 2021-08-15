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

#ifndef _GTS_NANOAPPS_SHARED_SEND_MESSAGE_H_
#define _GTS_NANOAPPS_SHARED_SEND_MESSAGE_H_

#include <cstddef>
#include <cstdint>

#include <shared/abort.h>

/**
 * NOTE: The MessageType values are manually synced in the GTS Java's
 *     ContextHubTestConstants.java.  If you make a change here, be sure
 *     to update ContextHubTestContants.java as well.
 */

namespace nanoapp_testing {

/**
 * Messages types which are sent between Nanoapps and the Java Host testing
 * code.
 */
enum class MessageType : uint32_t {
  /**
   * Value which should never be used.
   *
   * This value starts at CONTEXT_HUB_TYPE_PRIVATE_MSG_BASE.
   *
   * This type should never be sent by Host or Nanoapp code.
   */
  kInvalidMessageType = 0x0400,

  /**
   * Test has completed in success.
   *
   * This type should only be sent by the Nanoapp code.
   */
  kSuccess = 0x0401,

  /**
   * Indicates a failure in the CHRE implementation.
   *
   * This should be followed by null-terminated string
   * giving details of failure.
   *
   * This type should only be sent by the Nanoapp code.
   */
  kFailure = 0x0402,

  /**
   * Indicate a failure within the testing infrastructure.
   *
   * This should be followed by null-terminated string
   * giving details of failure.
   *
   * This type should only be sent by the Nanoapp code.
   */
  kInternalFailure = 0x0403,

  /**
   * Indicate a test is being skipped.
   *
   * This should be followed by null-terminated string
   * giving an explanation of why this test was skipped.
   *
   * This type should only be sent by the Nanoapp code.
   */
  kSkipped = 0x0404,

  /**
   * A generic message indicating that the test should continue.
   *
   * The meaning of this generic message depends on the specific test.
   * In general, it means something along the lines of "The test is
   * successful thus far, please proceed to the next stage."
   *
   * This type can be sent by the Host or Nanoapp code.
   */
  kContinue = 0x0405,

  // Tests wanting to add custom message types for their protocols should
  // add them below.  Remember to update ContextHubTestConstants.java as
  // well (see NOTE at the top of this header).
};

/**
 * Sends a message to the host with the given data.
 *
 * This method will make a copy of 'data', so there's no need for the
 * caller to keep that alive after this call.
 *
 * Note it may often be more convenient to use one the other methods
 * when sending a text string.
 *
 * @param messageType  The type of the message.
 * @param data  The data to send.  This can be nullptr, but then 'dataSize'
 *     must be 0.
 * @param dataSize  The number of bytes of 'data' to send.  If 'data' is
 *     not 'nullptr', then this must be non-zero.
 */
void sendMessageToHost(MessageType messageType, const void *data = nullptr,
                       size_t dataSize = 0);

/**
 * Sends a message to the host, optionally with the 'value' appended as in
 * hex.
 *
 * This method will make a copy of 'message' and 'value', so there's no
 * need for the caller to keep those alive after this call.
 *
 * Note it may often be more convenient to use one of the other methods
 * below.
 *
 * @param messageType  The type of the message.
 * @param message  The text of the message.  This cannot be nullptr.
 * @param value  Optional, defaults to nullptr.  If non-null, this value will
 *     be output as hexadecimal at the end of the message to the host.
 */
void sendStringToHost(MessageType messageType, const char *message,
                      const uint32_t *value = nullptr);

/**
 * Same as sendStringToHost(), but using MessageType::kFailure for the 'status'.
 */
inline void sendFailureToHost(const char *message,
                              const uint32_t *value = nullptr) {
  sendStringToHost(MessageType::kFailure, message, value);
}

/**
 * Same as sendFailureToHost(), but aborts the test with the given 'reason',
 * and never returns.
 */
void sendFatalFailureToHost(const char *message,
                            const uint32_t *value = nullptr,
                            AbortBlame reason = AbortBlame::kChre);

/**
 * Helper function to invoke sendFatalFailureToHost() with uint8_t type.
 * It is needed since sendFatalFailureToHost() only accepts uint32_t type.
 *
 * TODO: Deprecate this function and redesign sendFatalFailureToHost()
 * so that a generic string message is accepted.
 *
 * @param message a text message to be sent to host.
 * @param value a value output into the message.
 */
void sendFatalFailureToHostUint8(const char *message, const uint8_t value);

/**
 * Same as sendStringToHost(), but uses MessageType::kInternalFailure for the
 * 'status', and aborts the test with the given 'reason' and never returns.
 */
void sendInternalFailureToHost(const char *message,
                               const uint32_t *value = nullptr,
                               AbortBlame reason = AbortBlame::kTestFramework);

/**
 * Invoke sendMessageToHost with MessageType::kSuccess and no other information.
 */
inline void sendSuccessToHost() {
  sendMessageToHost(MessageType::kSuccess);
}


}  // namespace nanoapp_testing


#endif  // _GTS_NANOAPPS_SHARED_SEND_MESSAGE_H_
