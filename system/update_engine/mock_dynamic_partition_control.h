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

#include <stdint.h>

#include <memory>
#include <string>

#include <gmock/gmock.h>

#include "update_engine/dynamic_partition_control_interface.h"

namespace chromeos_update_engine {

class MockDynamicPartitionControl : public DynamicPartitionControlInterface {
 public:
  MOCK_METHOD5(MapPartitionOnDeviceMapper,
               bool(const std::string&,
                    const std::string&,
                    uint32_t,
                    bool,
                    std::string*));
  MOCK_METHOD2(UnmapPartitionOnDeviceMapper, bool(const std::string&, bool));
  MOCK_METHOD0(Cleanup, void());
  MOCK_METHOD1(DeviceExists, bool(const std::string&));
  MOCK_METHOD1(GetState, ::android::dm::DmDeviceState(const std::string&));
  MOCK_METHOD2(GetDmDevicePathByName, bool(const std::string&, std::string*));
  MOCK_METHOD3(LoadMetadataBuilder,
               std::unique_ptr<::android::fs_mgr::MetadataBuilder>(
                   const std::string&, uint32_t, uint32_t));
  MOCK_METHOD3(StoreMetadata,
               bool(const std::string&,
                    android::fs_mgr::MetadataBuilder*,
                    uint32_t));
  MOCK_METHOD1(GetDeviceDir, bool(std::string*));
  MOCK_METHOD0(IsDynamicPartitionsEnabled, bool());
  MOCK_METHOD0(IsDynamicPartitionsRetrofit, bool());
};

}  // namespace chromeos_update_engine
