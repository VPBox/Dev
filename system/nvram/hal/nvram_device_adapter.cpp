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

#include <nvram/hal/nvram_device_adapter.h>

#include <string.h>

#include <algorithm>
#include <type_traits>
#include <utility>

namespace nvram {
namespace {

// Executes an operation on the |NvramDeviceAdapter| corresponding to |device|.
// |command| identifies the type of operation, |request_payload| provides the
// input parameters. Output parameters are stored in |response_payload|, and the
// the nvram operation result code is returned.
template <nvram::Command command,
          typename RequestPayload,
          typename ResponsePayload>
nvram_result_t Execute(const nvram_device_t* device,
                       RequestPayload&& request_payload,
                       ResponsePayload* response_payload) {
  NvramDeviceAdapter* adapter = reinterpret_cast<NvramDeviceAdapter*>(
      const_cast<nvram_device_t*>(device));

  nvram::Request request;
  request.payload.Activate<command>() = std::move(request_payload);
  nvram::Response response;
  adapter->nvram_implementation()->Execute(request, &response);
  if (response.result != NV_RESULT_SUCCESS) {
    return response.result;
  }

  ResponsePayload* response_payload_ptr = response.payload.get<command>();
  if (!response_payload_ptr) {
    return NV_RESULT_INTERNAL_ERROR;
  }
  *response_payload = std::move(*response_payload_ptr);

  return NV_RESULT_SUCCESS;
}

// All the HAL methods need to be callable from C code.
extern "C" {

nvram_result_t device_get_total_size_in_bytes(const nvram_device_t* device,
                                              uint64_t* total_size) {
  nvram::GetInfoRequest get_info_request;
  nvram::GetInfoResponse get_info_response;
  nvram_result_t result = Execute<nvram::COMMAND_GET_INFO>(
      device, std::move(get_info_request), &get_info_response);
  *total_size = get_info_response.total_size;
  return result;
}

nvram_result_t device_get_available_size_in_bytes(const nvram_device_t* device,
                                                  uint64_t* available_size) {
  nvram::GetInfoRequest get_info_request;
  nvram::GetInfoResponse get_info_response;
  nvram_result_t result = Execute<nvram::COMMAND_GET_INFO>(
      device, std::move(get_info_request), &get_info_response);
  *available_size = get_info_response.available_size;
  return result;
}

nvram_result_t device_get_max_space_size_in_bytes(const nvram_device_t* device,
                                                  uint64_t* max_space_size) {
  nvram::GetInfoRequest get_info_request;
  nvram::GetInfoResponse get_info_response;
  nvram_result_t result = Execute<nvram::COMMAND_GET_INFO>(
      device, std::move(get_info_request), &get_info_response);
  *max_space_size = get_info_response.max_space_size;
  return result;
}

nvram_result_t device_get_max_spaces(const nvram_device_t* device,
                                     uint32_t* num_spaces) {
  nvram::GetInfoRequest get_info_request;
  nvram::GetInfoResponse get_info_response;
  nvram_result_t result = Execute<nvram::COMMAND_GET_INFO>(
      device, std::move(get_info_request), &get_info_response);
  *num_spaces = get_info_response.max_spaces;
  return result;
}

nvram_result_t device_get_space_list(const nvram_device_t* device,
                                     uint32_t max_list_size,
                                     uint32_t* space_index_list,
                                     uint32_t* list_size) {
  nvram::GetInfoRequest get_info_request;
  nvram::GetInfoResponse get_info_response;
  nvram_result_t result = Execute<nvram::COMMAND_GET_INFO>(
      device, std::move(get_info_request), &get_info_response);

  if (space_index_list) {
    *list_size = std::min(get_info_response.space_list.size(),
                          static_cast<size_t>(max_list_size));
    for (size_t i = 0; i < *list_size; ++i) {
      space_index_list[i] = get_info_response.space_list[i];
    }
  } else {
    *list_size = get_info_response.space_list.size();
  }

  return result;
}

nvram_result_t device_get_space_size(const nvram_device_t* device,
                                     uint32_t index,
                                     uint64_t* size) {
  nvram::GetSpaceInfoRequest get_space_info_request;
  get_space_info_request.index = index;
  nvram::GetSpaceInfoResponse get_space_info_response;
  nvram_result_t result = Execute<nvram::COMMAND_GET_SPACE_INFO>(
      device, std::move(get_space_info_request), &get_space_info_response);
  *size = get_space_info_response.size;
  return result;
}

nvram_result_t device_get_space_controls(const nvram_device_t* device,
                                         uint32_t index,
                                         uint32_t max_list_size,
                                         nvram_control_t* control_list,
                                         uint32_t* list_size) {
  nvram::GetSpaceInfoRequest get_space_info_request;
  get_space_info_request.index = index;
  nvram::GetSpaceInfoResponse get_space_info_response;
  nvram_result_t result = Execute<nvram::COMMAND_GET_SPACE_INFO>(
      device, std::move(get_space_info_request), &get_space_info_response);

  if (control_list) {
    *list_size = std::min(get_space_info_response.controls.size(),
                          static_cast<size_t>(max_list_size));
    for (size_t i = 0; i < *list_size; ++i) {
      control_list[i] = get_space_info_response.controls[i];
    }
  } else {
    *list_size = get_space_info_response.controls.size();
  }

  return result;
}

nvram_result_t device_is_space_locked(const nvram_device_t* device,
                                      uint32_t index,
                                      int* write_lock_enabled,
                                      int* read_lock_enabled) {
  nvram::GetSpaceInfoRequest get_space_info_request;
  get_space_info_request.index = index;
  nvram::GetSpaceInfoResponse get_space_info_response;
  nvram_result_t result = Execute<nvram::COMMAND_GET_SPACE_INFO>(
      device, std::move(get_space_info_request), &get_space_info_response);
  *write_lock_enabled = get_space_info_response.write_locked;
  *read_lock_enabled = get_space_info_response.read_locked;
  return result;
}

nvram_result_t device_create_space(const nvram_device_t* device,
                                   uint32_t index,
                                   uint64_t size_in_bytes,
                                   const nvram_control_t* control_list,
                                   uint32_t list_size,
                                   const uint8_t* authorization_value,
                                   uint32_t authorization_value_size) {
  nvram::CreateSpaceRequest create_space_request;
  create_space_request.index = index;
  create_space_request.size = size_in_bytes;
  if (!create_space_request.controls.Resize(list_size)) {
    return NV_RESULT_INTERNAL_ERROR;
  }
  for (size_t i = 0; i < list_size; ++i) {
    create_space_request.controls[i] = control_list[i];
  }
  if (!create_space_request.authorization_value.Assign(
          authorization_value, authorization_value_size)) {
    return NV_RESULT_INTERNAL_ERROR;
  }
  nvram::CreateSpaceResponse create_space_response;
  return Execute<nvram::COMMAND_CREATE_SPACE>(
      device, std::move(create_space_request), &create_space_response);
}

nvram_result_t device_delete_space(const nvram_device_t* device,
                                   uint32_t index,
                                   const uint8_t* authorization_value,
                                   uint32_t authorization_value_size) {
  nvram::DeleteSpaceRequest delete_space_request;
  delete_space_request.index = index;
  if (!delete_space_request.authorization_value.Assign(
          authorization_value, authorization_value_size)) {
    return NV_RESULT_INTERNAL_ERROR;
  }
  nvram::DeleteSpaceResponse delete_space_response;
  return Execute<nvram::COMMAND_DELETE_SPACE>(
      device, std::move(delete_space_request), &delete_space_response);
}

nvram_result_t device_disable_create(const nvram_device_t* device) {
  nvram::DisableCreateRequest disable_create_request;
  nvram::DisableCreateResponse disable_create_response;
  return Execute<nvram::COMMAND_DISABLE_CREATE>(
      device, std::move(disable_create_request), &disable_create_response);
}

nvram_result_t device_write_space(const nvram_device_t* device,
                                  uint32_t index,
                                  const uint8_t* buffer,
                                  uint64_t buffer_size,
                                  const uint8_t* authorization_value,
                                  uint32_t authorization_value_size) {
  nvram::WriteSpaceRequest write_space_request;
  write_space_request.index = index;
  if (!write_space_request.buffer.Assign(buffer, buffer_size) ||
      !write_space_request.authorization_value.Assign(
          authorization_value, authorization_value_size)) {
    return NV_RESULT_INTERNAL_ERROR;
  }
  nvram::WriteSpaceResponse write_space_response;
  return Execute<nvram::COMMAND_WRITE_SPACE>(
      device, std::move(write_space_request), &write_space_response);
}

nvram_result_t device_read_space(const nvram_device_t* device,
                                 uint32_t index,
                                 uint64_t num_bytes_to_read,
                                 const uint8_t* authorization_value,
                                 uint32_t authorization_value_size,
                                 uint8_t* buffer,
                                 uint64_t* bytes_read) {
  nvram::ReadSpaceRequest read_space_request;
  read_space_request.index = index;
  if (!read_space_request.authorization_value.Assign(
          authorization_value, authorization_value_size)) {
    return NV_RESULT_INTERNAL_ERROR;
  }
  nvram::ReadSpaceResponse read_space_response;
  nvram_result_t result = Execute<nvram::COMMAND_READ_SPACE>(
      device, std::move(read_space_request), &read_space_response);
  *bytes_read = std::min(static_cast<size_t>(num_bytes_to_read),
                         read_space_response.buffer.size());
  memcpy(buffer, read_space_response.buffer.data(), *bytes_read);
  return result;
}

nvram_result_t device_enable_write_lock(const nvram_device_t* device,
                                        uint32_t index,
                                        const uint8_t* authorization_value,
                                        uint32_t authorization_value_size) {
  nvram::LockSpaceWriteRequest lock_space_write_request;
  lock_space_write_request.index = index;
  if (!lock_space_write_request.authorization_value.Assign(
          authorization_value, authorization_value_size)) {
    return NV_RESULT_INTERNAL_ERROR;
  }
  nvram::LockSpaceWriteResponse lock_space_write_response;
  return Execute<nvram::COMMAND_LOCK_SPACE_WRITE>(
      device, std::move(lock_space_write_request), &lock_space_write_response);
}

nvram_result_t device_enable_read_lock(const nvram_device_t* device,
                                       uint32_t index,
                                       const uint8_t* authorization_value,
                                       uint32_t authorization_value_size) {
  nvram::LockSpaceReadRequest lock_space_read_request;
  lock_space_read_request.index = index;
  if (!lock_space_read_request.authorization_value.Assign(
          authorization_value, authorization_value_size)) {
    return NV_RESULT_INTERNAL_ERROR;
  }
  nvram::LockSpaceReadResponse lock_space_read_response;
  return Execute<nvram::COMMAND_LOCK_SPACE_READ>(
      device, std::move(lock_space_read_request), &lock_space_read_response);
}

int device_nvram_device_close(struct hw_device_t* device) {
  delete reinterpret_cast<NvramDeviceAdapter*>(
      reinterpret_cast<nvram_device_t*>(device));
  return 0;
}

}  // extern "C"
}  // namespace

NvramDeviceAdapter::NvramDeviceAdapter(const hw_module_t* module,
                                       NvramImplementation* implementation)
    : implementation_(implementation) {
  memset(&device_, 0, sizeof(nvram_device_t));

  device_.common.tag = HARDWARE_DEVICE_TAG;
  device_.common.version = NVRAM_DEVICE_API_VERSION_1_1;
  device_.common.module = const_cast<hw_module_t *>(module);
  device_.common.close = device_nvram_device_close;

  device_.get_total_size_in_bytes = device_get_total_size_in_bytes;
  device_.get_available_size_in_bytes = device_get_available_size_in_bytes;
  device_.get_max_space_size_in_bytes = device_get_max_space_size_in_bytes;
  device_.get_max_spaces = device_get_max_spaces;
  device_.get_space_list = device_get_space_list;
  device_.get_space_size = device_get_space_size;
  device_.get_space_controls = device_get_space_controls;
  device_.is_space_locked = device_is_space_locked;
  device_.create_space = device_create_space;
  device_.delete_space = device_delete_space;
  device_.disable_create = device_disable_create;
  device_.write_space = device_write_space;
  device_.read_space = device_read_space;
  device_.enable_write_lock = device_enable_write_lock;
  device_.enable_read_lock = device_enable_read_lock;
}

}  // namespace nvram
