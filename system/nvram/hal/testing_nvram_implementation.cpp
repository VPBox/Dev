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

#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <android-base/logging.h>
#include <cutils/sockets.h>

#include <nvram/hal/nvram_device_adapter.h>
#include <nvram/messages/nvram_messages.h>

namespace {

constexpr char kFakeNvramControlSocketName[] = "nvram";

// This instantiates an |NvramManager| with the storage interface wired up with
// an in-memory implementation. This *DOES NOT* meet the persistence and tamper
// evidence requirements of the HAL, but is useful for demonstration and running
// tests against the |NvramManager| implementation.
class TestingNvramImplementation : public nvram::NvramImplementation {
 public:
  ~TestingNvramImplementation() override;

  void Execute(const nvram::Request& request,
               nvram::Response* response) override;

 private:
  // Connects the fake NVRAM control socket it it is not open already. Returns
  // true if the channel is open, false on errors.
  bool Connect();

  // Sends a request to the fake NVRAM daemon. Returns true if successful, false
  // on any I/O errors.
  bool SendRequest(const nvram::Request& request, nvram::Response* response);

  // A file descriptor of the socket connected to the fake NVRAM daemon.
  int nvram_socket_fd_ = -1;

  // The command buffer, used for encoding request and decoding responses.
  uint8_t command_buffer_[4096];
};

TestingNvramImplementation::~TestingNvramImplementation() {
  if (nvram_socket_fd_ != -1) {
    // No need to handle EINTR specially here as bionic filters it out.
    if (close(nvram_socket_fd_)) {
      PLOG(ERROR) << "Failed to close NVRAM command socket";
    }
    nvram_socket_fd_ = -1;
  }
}

void TestingNvramImplementation::Execute(const nvram::Request& request,
                                         nvram::Response* response) {
  if (!SendRequest(request, response)) {
    response->result = NV_RESULT_INTERNAL_ERROR;
  }
}

bool TestingNvramImplementation::Connect() {
  if (nvram_socket_fd_ != -1) {
    return true;
  }

  int rc =
      socket_local_client(kFakeNvramControlSocketName,
                          ANDROID_SOCKET_NAMESPACE_RESERVED, SOCK_SEQPACKET);
  if (rc < 0) {
    PLOG(ERROR) << "Failed to connect fake NVRAM control socket";
    return false;
  }

  nvram_socket_fd_ = rc;
  return true;
}

bool TestingNvramImplementation::SendRequest(const nvram::Request& request,
                                             nvram::Response* response) {
  if (!Connect()) {
    return false;
  }

  size_t request_size = sizeof(command_buffer_);
  if (!nvram::Encode(request, command_buffer_, &request_size)) {
    LOG(ERROR) << "Failed to encode NVRAM request.";
    return false;
  }

  ssize_t rc = TEMP_FAILURE_RETRY(
      write(nvram_socket_fd_, command_buffer_, request_size));
  if (rc < 0) {
    PLOG(ERROR) << "Failed to send request on NVRAM control socket";
    return false;
  }

  rc = TEMP_FAILURE_RETRY(
      read(nvram_socket_fd_, command_buffer_, sizeof(command_buffer_)));
  if (rc < 0 || static_cast<size_t>(rc) > sizeof(command_buffer_)) {
    PLOG(ERROR) << "Failed to read NVRAM response";
    return false;
  }

  if (!nvram::Decode(command_buffer_, static_cast<size_t>(rc), response)) {
    LOG(ERROR) << "Failed to decode NVRAM response.";
    return false;
  }

  return true;
}

}  // namespace

extern "C" int testing_nvram_open(const hw_module_t* module,
                                  const char* device_id,
                                  hw_device_t** device_ptr) {
  if (strcmp(NVRAM_HARDWARE_DEVICE_ID, device_id) != 0) {
    return -EINVAL;
  }

  nvram::NvramDeviceAdapter* adapter =
      new nvram::NvramDeviceAdapter(module, new TestingNvramImplementation);
  *device_ptr = adapter->as_device();
  return 0;
}
