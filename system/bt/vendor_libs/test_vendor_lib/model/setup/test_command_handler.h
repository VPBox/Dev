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

#include <unistd.h>
#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "model/devices/device.h"
#include "phy_layer_factory.h"
#include "test_channel_transport.h"
#include "test_model.h"

namespace test_vendor_lib {

class TestCommandHandler {
 public:
  // Sets all of the methods to be used as callbacks in the HciHandler.
  TestCommandHandler(TestModel& test_model);

  ~TestCommandHandler() = default;

  // Dispatches the action corresponding to the command specified by |name|.
  void HandleCommand(const std::string& name, const std::vector<std::string>& args);

  // Dispatches the action corresponding to the command specified by |name|.
  void RegisterSendResponse(const std::function<void(const std::string&)> callback);

  // Commands:

  // Add a device
  void Add(const std::vector<std::string>& args);

  // Add a remote device
  void AddRemote(const std::vector<std::string>& args);

  // Remove devices by index
  void Del(const std::vector<std::string>& args);

  // Add phy
  void AddPhy(const std::vector<std::string>& args);

  // Remove phy by name
  void DelPhy(const std::vector<std::string>& args);

  // Add device to phy
  void AddDeviceToPhy(const std::vector<std::string>& args);

  // Remove device from phy
  void DelDeviceFromPhy(const std::vector<std::string>& args);

  // List the devices that the test knows about
  void List(const std::vector<std::string>& args);

  // Timer management functions
  void SetTimerPeriod(const std::vector<std::string>& args);

  void StartTimer(const std::vector<std::string>& args);

  void StopTimer(const std::vector<std::string>& args);

  // For manual testing
  void AddDefaults();

 private:
  TestModel& model_;

  std::string response_string_;

  std::unordered_map<std::string, std::function<void(const std::vector<std::string>&)>> active_commands_;

  std::function<void(const std::string&)> send_response_;

  TestCommandHandler(const TestCommandHandler& cmdPckt) = delete;
  TestCommandHandler& operator=(const TestCommandHandler& cmdPckt) = delete;
};

}  // namespace test_vendor_lib
