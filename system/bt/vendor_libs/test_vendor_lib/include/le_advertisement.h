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

#include <cstdint>

namespace test_vendor_lib {

class LeAdvertisement {
 public:
  enum class AdvertisementType : uint8_t {
    ADV_IND = 0,          // Connectable and scannable
    ADV_DIRECT_IND = 1,   // Connectable directed
    ADV_SCAN_IND = 2,     // Scannable undirected
    ADV_NONCONN_IND = 3,  // Non connectable undirected
    SCAN_RESPONSE = 4,
  };
  enum class AddressType : uint8_t {
    PUBLIC = 0,
    RANDOM = 1,
    PUBLIC_IDENTITY = 2,
    RANDOM_IDENTITY = 3,
  };
};

}  // namespace test_vendor_lib
