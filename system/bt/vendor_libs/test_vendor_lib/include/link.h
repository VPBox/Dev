/*
 * Copyright 2018 The Android Open Source Project
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

#pragma once

namespace test_vendor_lib {
class Link {
 public:
  static constexpr size_t kSizeBytes = sizeof(uint32_t);
  static constexpr size_t kTypeBytes = sizeof(uint8_t);

  enum class PacketType : uint8_t {
    UNKNOWN,
    ACL,
    COMMAND,
    DISCONNECT,
    ENCRYPT_CONNECTION,
    ENCRYPT_CONNECTION_RESPONSE,
    EVENT,
    INQUIRY,
    INQUIRY_RESPONSE,
    IO_CAPABILITY_REQUEST,
    IO_CAPABILITY_RESPONSE,
    IO_CAPABILITY_NEGATIVE_RESPONSE,
    LE_ADVERTISEMENT,
    LE_SCAN,
    LE_SCAN_RESPONSE,
    PAGE,
    PAGE_RESPONSE,
    RESPONSE,
    SCO,
  };
};
}  // namespace test_vendor_lib
