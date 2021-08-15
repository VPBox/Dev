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

#include "device.h"
#include "packets/link_layer/link_layer_packet_view.h"

namespace test_vendor_lib {

class RemoteLoopbackDevice : public Device {
 public:
  RemoteLoopbackDevice();
  virtual ~RemoteLoopbackDevice() = default;

  static std::shared_ptr<Device> Create() {
    return std::make_shared<RemoteLoopbackDevice>();
  }

  virtual std::string GetTypeString() const override {
    return "remote_loopback_device";
  }

  virtual std::string ToString() const override;

  virtual void Initialize(const std::vector<std::string>& args) override;

  virtual void IncomingPacket(packets::LinkLayerPacketView packet) override;

 private:
  static bool registered_;
};

}  // namespace test_vendor_lib
