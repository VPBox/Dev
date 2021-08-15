//
// Copyright (C) 2018 The Android Open Source Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#include <android/hardware/boot/1.0/IBootControl.h>
#include <stdint.h>

#include <gmock/gmock.h>

namespace chromeos_update_engine {

class MockBootControlHal
    : public ::android::hardware::boot::V1_0::IBootControl {
 public:
  MOCK_METHOD0(getNumberSlots, ::android::hardware::Return<uint32_t>());
  MOCK_METHOD0(getCurrentSlot, ::android::hardware::Return<uint32_t>());
  MOCK_METHOD1(markBootSuccessful,
               ::android::hardware::Return<void>(markBootSuccessful_cb));
  MOCK_METHOD2(setActiveBootSlot,
               ::android::hardware::Return<void>(uint32_t,
                                                 setActiveBootSlot_cb));
  MOCK_METHOD2(setSlotAsUnbootable,
               ::android::hardware::Return<void>(uint32_t,
                                                 setSlotAsUnbootable_cb));
  MOCK_METHOD1(
      isSlotBootable,
      ::android::hardware::Return<::android::hardware::boot::V1_0::BoolResult>(
          uint32_t));
  MOCK_METHOD1(
      isSlotMarkedSuccessful,
      ::android::hardware::Return<::android::hardware::boot::V1_0::BoolResult>(
          uint32_t));
  MOCK_METHOD2(getSuffix,
               ::android::hardware::Return<void>(uint32_t, getSuffix_cb));
};

}  // namespace chromeos_update_engine
