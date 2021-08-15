//
// Copyright (C) 2016 The Android Open Source Project
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

#ifndef NVRAM_HAL_TESTS_SCOPED_NVRAM_DEVICE_H_
#define NVRAM_HAL_TESTS_SCOPED_NVRAM_DEVICE_H_

#include <string>
#include <vector>

#include <android-base/logging.h>
#include <hardware/nvram.h>

namespace nvram {

using IndexList = std::vector<uint32_t>;
using ControlList = std::vector<nvram_control_t>;

// Wraps an nvram_device_t. The device will be opened on construction and closed
// on destruction. Failure to open the device will cause all methods to return
// NV_RESULT_INTERNAL_ERROR.
// Note: Currently this resides in hal/tests because that's where it is used. It
//       may be useful elsewhere but should be moved somewhere more common
//       first.
class ScopedNvramDevice {
 public:
  ScopedNvramDevice();
  virtual ~ScopedNvramDevice();

  // Convenience methods which trivially wrap the device functions.
  virtual nvram_result_t GetTotalSizeInBytes(uint64_t* total_size);
  virtual nvram_result_t GetAvailableSizeInBytes(uint64_t* available_size);
  virtual nvram_result_t GetMaxSpaceSizeInBytes(uint64_t* max_space_size);
  virtual nvram_result_t GetMaxSpaces(uint32_t* num_spaces);
  virtual nvram_result_t GetSpaceList(std::vector<uint32_t>* space_index_list);
  virtual nvram_result_t GetSpaceSize(uint32_t index, uint64_t* size);
  virtual nvram_result_t GetSpaceControls(
      uint32_t index,
      std::vector<nvram_control_t>* control_list);
  virtual nvram_result_t IsSpaceLocked(uint32_t index,
                                       int* write_lock_enabled,
                                       int* read_lock_enabled);
  virtual nvram_result_t CreateSpace(
      uint32_t index,
      uint64_t size_in_bytes,
      const std::vector<nvram_control_t>& control_list,
      const std::string& authorization_value);
  virtual nvram_result_t DeleteSpace(uint32_t index,
                                     const std::string& authorization_value);
  virtual nvram_result_t DisableCreate();
  virtual nvram_result_t WriteSpace(uint32_t index,
                                    const std::string& data,
                                    const std::string& authorization_value);
  virtual nvram_result_t ReadSpace(uint32_t index,
                                   uint64_t num_bytes_to_read,
                                   const std::string& authorization_value,
                                   std::string* data);
  virtual nvram_result_t EnableWriteLock(
      uint32_t index,
      const std::string& authorization_value);
  virtual nvram_result_t EnableReadLock(uint32_t index,
                                        const std::string& authorization_value);

 private:
  nvram_device_t* device_ = nullptr;
};

}  // namespace nvram

#endif  // NVRAM_HAL_TESTS_SCOPED_NVRAM_DEVICE_H_
