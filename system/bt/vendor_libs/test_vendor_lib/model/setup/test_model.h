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

#include "async_manager.h"
#include "model/devices/device.h"
#include "phy_layer_factory.h"
#include "test_channel_transport.h"

namespace test_vendor_lib {

class TestModel {
 public:
  TestModel(std::function<AsyncTaskId(std::chrono::milliseconds, const TaskCallback&)> evtScheduler,
            std::function<AsyncTaskId(std::chrono::milliseconds, std::chrono::milliseconds, const TaskCallback&)>
                periodicEvtScheduler,
            std::function<void(AsyncTaskId)> cancel, std::function<int(const std::string&, int)> connect_to_remote);
  ~TestModel() = default;

  // Commands:

  // Add a device, return its index
  size_t Add(std::shared_ptr<Device> device);

  // Remove devices by index
  void Del(size_t device_index);

  // Add phy, return its index
  size_t AddPhy(std::shared_ptr<PhyLayerFactory> phy);

  // Remove phy by index
  void DelPhy(size_t phy_index);

  // Add device to phy
  void AddDeviceToPhy(size_t device_index, size_t phy_index);

  // Remove device from phy
  void DelDeviceFromPhy(size_t device_index, size_t phy_index);

  // Handle incoming remote connections
  void AddLinkLayerConnection(int socket_fd, Phy::Type phy_type);
  void IncomingLinkLayerConnection(int socket_fd);
  void IncomingHciConnection(int socket_fd);

  // Connect to a remote device
  void AddRemote(const std::string& server, int port, Phy::Type phy_type);

  // Let devices know about the passage of time
  void TimerTick();
  void StartTimer();
  void StopTimer();
  void SetTimerPeriod(std::chrono::milliseconds new_period);

  // List the devices that the test knows about
  const std::string& List();

  // Clear all devices and phys.
  void Reset();

 private:
  std::vector<std::shared_ptr<PhyLayerFactory>> phys_;
  std::vector<std::shared_ptr<Device>> devices_;
  std::string list_string_;

  // Callbacks to schedule tasks.
  std::function<AsyncTaskId(std::chrono::milliseconds, const TaskCallback&)> schedule_task_;
  std::function<AsyncTaskId(std::chrono::milliseconds, std::chrono::milliseconds, const TaskCallback&)>
      schedule_periodic_task_;
  std::function<void(AsyncTaskId)> cancel_task_;
  std::function<int(const std::string&, int)> connect_to_remote_;

  AsyncTaskId timer_tick_task_{kInvalidTaskId};
  std::chrono::milliseconds timer_period_;

  TestModel(TestModel& model) = delete;
  TestModel& operator=(const TestModel& model) = delete;

  std::vector<std::shared_ptr<Device>> example_devices_;
};

}  // namespace test_vendor_lib
