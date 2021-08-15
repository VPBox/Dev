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
#include <vector>

#include "model/controller/dual_mode_controller.h"
#include "model/devices/h4_packetizer.h"

namespace {
const std::string kHciSocketDevicePropertiesFile = "/etc/bluetooth/hci_socket_device_controller_properties.json";
}  // namespace

namespace test_vendor_lib {

class HciSocketDevice : public DualModeController {
 public:
  HciSocketDevice(int socket_fd);
  ~HciSocketDevice() = default;

  static std::shared_ptr<HciSocketDevice> Create(int socket_fd) {
    return std::make_shared<HciSocketDevice>(socket_fd);
  }

  virtual std::string GetTypeString() const override {
    return "hci_socket_device";
  }

  virtual void TimerTick() override;

  void SendHci(hci::PacketType packet_type, const std::shared_ptr<std::vector<uint8_t>> packet);

 private:
  int socket_file_descriptor_{-1};
  hci::H4Packetizer h4_{socket_file_descriptor_, [](const std::vector<uint8_t>&) {}, [](const std::vector<uint8_t>&) {},
                        [](const std::vector<uint8_t>&) {}, [](const std::vector<uint8_t>&) {}};
};

}  // namespace test_vendor_lib
