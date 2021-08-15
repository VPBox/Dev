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

class Inquiry {
 public:
  enum class InquiryState : uint8_t {
    STANDBY = 0x00,
    INQUIRY = 0x01,
  };
  enum class InquiryType : uint8_t {
    STANDARD = 0x00,
    RSSI = 0x01,
    EXTENDED = 0x02,
  };
};
}  // namespace test_vendor_lib
