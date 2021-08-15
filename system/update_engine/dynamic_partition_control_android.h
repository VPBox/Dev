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

#ifndef UPDATE_ENGINE_DYNAMIC_PARTITION_CONTROL_ANDROID_H_
#define UPDATE_ENGINE_DYNAMIC_PARTITION_CONTROL_ANDROID_H_

#include "update_engine/dynamic_partition_control_interface.h"

#include <memory>
#include <set>
#include <string>

namespace chromeos_update_engine {

class DynamicPartitionControlAndroid : public DynamicPartitionControlInterface {
 public:
  DynamicPartitionControlAndroid() = default;
  ~DynamicPartitionControlAndroid();
  bool IsDynamicPartitionsEnabled() override;
  bool IsDynamicPartitionsRetrofit() override;
  bool MapPartitionOnDeviceMapper(const std::string& super_device,
                                  const std::string& target_partition_name,
                                  uint32_t slot,
                                  bool force_writable,
                                  std::string* path) override;
  bool UnmapPartitionOnDeviceMapper(const std::string& target_partition_name,
                                    bool wait) override;
  void Cleanup() override;
  bool DeviceExists(const std::string& path) override;
  android::dm::DmDeviceState GetState(const std::string& name) override;
  bool GetDmDevicePathByName(const std::string& name,
                             std::string* path) override;
  std::unique_ptr<android::fs_mgr::MetadataBuilder> LoadMetadataBuilder(
      const std::string& super_device,
      uint32_t source_slot,
      uint32_t target_slot) override;
  bool StoreMetadata(const std::string& super_device,
                     android::fs_mgr::MetadataBuilder* builder,
                     uint32_t target_slot) override;
  bool GetDeviceDir(std::string* path) override;

 private:
  std::set<std::string> mapped_devices_;

  void CleanupInternal(bool wait);
  bool MapPartitionInternal(const std::string& super_device,
                            const std::string& target_partition_name,
                            uint32_t slot,
                            bool force_writable,
                            std::string* path);

  DISALLOW_COPY_AND_ASSIGN(DynamicPartitionControlAndroid);
};

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_DYNAMIC_PARTITION_CONTROL_ANDROID_H_
