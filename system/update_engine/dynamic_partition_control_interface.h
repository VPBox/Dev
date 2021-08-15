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

#ifndef UPDATE_ENGINE_DYNAMIC_PARTITION_CONTROL_INTERFACE_H_
#define UPDATE_ENGINE_DYNAMIC_PARTITION_CONTROL_INTERFACE_H_

#include <stdint.h>

#include <memory>
#include <string>

#include <base/files/file_util.h>
#include <libdm/dm.h>
#include <liblp/builder.h>

namespace chromeos_update_engine {

class DynamicPartitionControlInterface {
 public:
  virtual ~DynamicPartitionControlInterface() = default;

  // Return true iff dynamic partitions is enabled on this device.
  virtual bool IsDynamicPartitionsEnabled() = 0;

  // Return true iff dynamic partitions is retrofitted on this device.
  virtual bool IsDynamicPartitionsRetrofit() = 0;

  // Map logical partition on device-mapper.
  // |super_device| is the device path of the physical partition ("super").
  // |target_partition_name| is the identifier used in metadata; for example,
  // "vendor_a"
  // |slot| is the selected slot to mount; for example, 0 for "_a".
  // Returns true if mapped successfully; if so, |path| is set to the device
  // path of the mapped logical partition.
  virtual bool MapPartitionOnDeviceMapper(
      const std::string& super_device,
      const std::string& target_partition_name,
      uint32_t slot,
      bool force_writable,
      std::string* path) = 0;

  // Unmap logical partition on device mapper. This is the reverse operation
  // of MapPartitionOnDeviceMapper.
  // If |wait| is set, wait until the device is unmapped.
  // Returns true if unmapped successfully.
  virtual bool UnmapPartitionOnDeviceMapper(
      const std::string& target_partition_name, bool wait) = 0;

  // Do necessary cleanups before destroying the object.
  virtual void Cleanup() = 0;

  // Return true if a static partition exists at device path |path|.
  virtual bool DeviceExists(const std::string& path) = 0;

  // Returns the current state of the underlying device mapper device
  // with given name.
  // One of INVALID, SUSPENDED or ACTIVE.
  virtual android::dm::DmDeviceState GetState(const std::string& name) = 0;

  // Returns the path to the device mapper device node in '/dev' corresponding
  // to 'name'. If the device does not exist, false is returned, and the path
  // parameter is not set.
  virtual bool GetDmDevicePathByName(const std::string& name,
                                     std::string* path) = 0;

  // Retrieve metadata from |super_device| at slot |source_slot|.
  // On retrofit devices, if |target_slot| != kInvalidSlot, the returned
  // metadata automatically includes block devices at |target_slot|.
  virtual std::unique_ptr<android::fs_mgr::MetadataBuilder> LoadMetadataBuilder(
      const std::string& super_device,
      uint32_t source_slot,
      uint32_t target_slot) = 0;

  // Write metadata |builder| to |super_device| at slot |target_slot|.
  virtual bool StoreMetadata(const std::string& super_device,
                             android::fs_mgr::MetadataBuilder* builder,
                             uint32_t target_slot) = 0;

  // Return a possible location for devices listed by name.
  virtual bool GetDeviceDir(std::string* path) = 0;
};

}  // namespace chromeos_update_engine

#endif  // UPDATE_ENGINE_DYNAMIC_PARTITION_CONTROL_INTERFACE_H_
