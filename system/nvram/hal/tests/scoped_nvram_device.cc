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

#include "nvram/hal/tests/scoped_nvram_device.h"

#include <android-base/logging.h>
#include <hardware/hardware.h>
#include <hardware/nvram.h>

namespace {

const uint8_t* StringToBytePtr(const std::string& s) {
  return s.empty() ? nullptr : reinterpret_cast<const uint8_t*>(s.data());
}

uint8_t* StringToMutableBytePtr(std::string* s) {
  return reinterpret_cast<uint8_t*>(&s->front());
}

}  // namespace

namespace nvram {

ScopedNvramDevice::ScopedNvramDevice() {
  const hw_module_t* module = nullptr;
  int result = hw_get_module(NVRAM_HARDWARE_MODULE_ID, &module);
  if (result) {
    LOG(ERROR) << "Failed to load NVRAM module: " << result;
    return;
  }
  result = nvram_open(module, &device_);
  if (result) {
    LOG(ERROR) << "Failed to open NVRAM device: " << result;
    device_ = nullptr;
    return;
  }
  if (device_->common.version != NVRAM_DEVICE_API_VERSION_1_1) {
    LOG(ERROR) << "Unsupported NVRAM HAL version.";
    nvram_close(device_);
    device_ = nullptr;
    return;
  }
}

ScopedNvramDevice::~ScopedNvramDevice() {
  if (device_) {
    int result = nvram_close(device_);
    if (result) {
      LOG(WARNING) << "Failed to close NVRAM device: " << result;
      return;
    }
  }
}

nvram_result_t ScopedNvramDevice::GetTotalSizeInBytes(uint64_t* total_size) {
  if (!device_) {
    return NV_RESULT_INTERNAL_ERROR;
  }
  return device_->get_total_size_in_bytes(device_, total_size);
}

nvram_result_t ScopedNvramDevice::GetAvailableSizeInBytes(
    uint64_t* available_size) {
  if (!device_) {
    return NV_RESULT_INTERNAL_ERROR;
  }
  return device_->get_available_size_in_bytes(device_, available_size);
}

nvram_result_t ScopedNvramDevice::GetMaxSpaceSizeInBytes(
    uint64_t* max_space_size) {
  if (!device_) {
    return NV_RESULT_INTERNAL_ERROR;
  }
  return device_->get_max_space_size_in_bytes(device_, max_space_size);
}

nvram_result_t ScopedNvramDevice::GetMaxSpaces(uint32_t* num_spaces) {
  if (!device_) {
    return NV_RESULT_INTERNAL_ERROR;
  }
  return device_->get_max_spaces(device_, num_spaces);
}

nvram_result_t ScopedNvramDevice::GetSpaceList(
    std::vector<uint32_t>* space_index_list) {
  if (!device_) {
    return NV_RESULT_INTERNAL_ERROR;
  }
  uint32_t max_spaces = 0;
  nvram_result_t result = device_->get_max_spaces(device_, &max_spaces);
  if (result) {
    return result;
  }
  space_index_list->resize(max_spaces);
  uint32_t list_size = 0;
  result = device_->get_space_list(device_, max_spaces,
                                   space_index_list->data(), &list_size);
  if (result) {
    return result;
  }
  space_index_list->resize(list_size);
  return NV_RESULT_SUCCESS;
}

nvram_result_t ScopedNvramDevice::GetSpaceSize(uint32_t index, uint64_t* size) {
  if (!device_) {
    return NV_RESULT_INTERNAL_ERROR;
  }
  return device_->get_space_size(device_, index, size);
}

nvram_result_t ScopedNvramDevice::GetSpaceControls(
    uint32_t index,
    std::vector<nvram_control_t>* control_list) {
  constexpr uint32_t kMaxControls = 16;
  if (!device_) {
    return NV_RESULT_INTERNAL_ERROR;
  }
  control_list->resize(kMaxControls);
  uint32_t list_size = 0;
  nvram_result_t result = device_->get_space_controls(
      device_, index, kMaxControls, control_list->data(), &list_size);
  if (result) {
    return result;
  }
  control_list->resize(list_size);
  return NV_RESULT_SUCCESS;
}

nvram_result_t ScopedNvramDevice::IsSpaceLocked(uint32_t index,
                                                int* write_lock_enabled,
                                                int* read_lock_enabled) {
  if (!device_) {
    return NV_RESULT_INTERNAL_ERROR;
  }
  return device_->is_space_locked(device_, index, write_lock_enabled,
                                  read_lock_enabled);
}

nvram_result_t ScopedNvramDevice::CreateSpace(
    uint32_t index,
    uint64_t size_in_bytes,
    const std::vector<nvram_control_t>& control_list,
    const std::string& authorization_value) {
  if (!device_) {
    return NV_RESULT_INTERNAL_ERROR;
  }
  return device_->create_space(
      device_, index, size_in_bytes, control_list.data(), control_list.size(),
      StringToBytePtr(authorization_value), authorization_value.size());
}

nvram_result_t ScopedNvramDevice::DeleteSpace(
    uint32_t index,
    const std::string& authorization_value) {
  return device_->delete_space(device_, index,
                               StringToBytePtr(authorization_value),
                               authorization_value.size());
}

nvram_result_t ScopedNvramDevice::DisableCreate() {
  if (!device_) {
    return NV_RESULT_INTERNAL_ERROR;
  }
  return device_->disable_create(device_);
}

nvram_result_t ScopedNvramDevice::WriteSpace(
    uint32_t index,
    const std::string& data,
    const std::string& authorization_value) {
  if (!device_) {
    return NV_RESULT_INTERNAL_ERROR;
  }
  return device_->write_space(device_, index, StringToBytePtr(data),
                              data.size(), StringToBytePtr(authorization_value),
                              authorization_value.size());
}

nvram_result_t ScopedNvramDevice::ReadSpace(
    uint32_t index,
    uint64_t num_bytes_to_read,
    const std::string& authorization_value,
    std::string* data) {
  if (!device_) {
    return NV_RESULT_INTERNAL_ERROR;
  }
  data->resize(num_bytes_to_read);
  uint64_t bytes_read = 0;
  nvram_result_t result = device_->read_space(
      device_, index, num_bytes_to_read, StringToBytePtr(authorization_value),
      authorization_value.size(), StringToMutableBytePtr(data), &bytes_read);
  if (result) {
    return result;
  }
  data->resize(bytes_read);
  return NV_RESULT_SUCCESS;
}

nvram_result_t ScopedNvramDevice::EnableWriteLock(
    uint32_t index,
    const std::string& authorization_value) {
  if (!device_) {
    return NV_RESULT_INTERNAL_ERROR;
  }
  return device_->enable_write_lock(device_, index,
                                    StringToBytePtr(authorization_value),
                                    authorization_value.size());
}

nvram_result_t ScopedNvramDevice::EnableReadLock(
    uint32_t index,
    const std::string& authorization_value) {
  if (!device_) {
    return NV_RESULT_INTERNAL_ERROR;
  }
  return device_->enable_read_lock(device_, index,
                                   StringToBytePtr(authorization_value),
                                   authorization_value.size());
}

}  // namespace nvram
