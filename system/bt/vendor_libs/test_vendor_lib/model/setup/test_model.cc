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

#define LOG_TAG "test_model"

#include "test_model.h"

// TODO: Remove when registration works
#include "model/devices/beacon.h"
#include "model/devices/beacon_swarm.h"
#include "model/devices/car_kit.h"
#include "model/devices/classic.h"
#include "model/devices/keyboard.h"
#include "model/devices/remote_loopback_device.h"
#include "model/devices/sniffer.h"

#include <memory>

#include <stdlib.h>

#include <base/logging.h>
#include "base/files/file_util.h"
#include "base/json/json_reader.h"
#include "base/values.h"

#include "osi/include/log.h"
#include "osi/include/osi.h"

#include "device_boutique.h"
#include "include/phy.h"
#include "model/devices/hci_socket_device.h"
#include "model/devices/link_layer_socket_device.h"

using std::vector;

namespace test_vendor_lib {

TestModel::TestModel(
    std::function<AsyncTaskId(std::chrono::milliseconds, const TaskCallback&)> event_scheduler,

    std::function<AsyncTaskId(std::chrono::milliseconds, std::chrono::milliseconds, const TaskCallback&)>
        periodic_event_scheduler,

    std::function<void(AsyncTaskId)> cancel, std::function<int(const std::string&, int)> connect_to_remote)
    : schedule_task_(event_scheduler), schedule_periodic_task_(periodic_event_scheduler), cancel_task_(cancel),
      connect_to_remote_(connect_to_remote) {
  // TODO: Remove when registration works!
  example_devices_.push_back(std::make_shared<Beacon>());
  example_devices_.push_back(std::make_shared<BeaconSwarm>());
  example_devices_.push_back(std::make_shared<Keyboard>());
  example_devices_.push_back(std::make_shared<CarKit>());
  example_devices_.push_back(std::make_shared<Classic>());
  example_devices_.push_back(std::make_shared<Sniffer>());
  example_devices_.push_back(std::make_shared<RemoteLoopbackDevice>());
}

void TestModel::SetTimerPeriod(std::chrono::milliseconds new_period) {
  timer_period_ = new_period;

  if (timer_tick_task_ == kInvalidTaskId) return;

  // Restart the timer with the new period
  StopTimer();
  StartTimer();
}

void TestModel::StartTimer() {
  LOG_INFO(LOG_TAG, "StartTimer()");
  timer_tick_task_ =
      schedule_periodic_task_(std::chrono::milliseconds(0), timer_period_, [this]() { TestModel::TimerTick(); });
}

void TestModel::StopTimer() {
  LOG_INFO(LOG_TAG, "StopTimer()");
  cancel_task_(timer_tick_task_);
  timer_tick_task_ = kInvalidTaskId;
}

size_t TestModel::Add(std::shared_ptr<Device> new_dev) {
  devices_.push_back(new_dev);
  return devices_.size() - 1;
}

void TestModel::Del(size_t dev_index) {
  if (dev_index >= devices_.size()) {
    LOG_WARN(LOG_TAG, "del: index out of range!");
    return;
  }
  devices_.erase(devices_.begin() + dev_index);
}

size_t TestModel::AddPhy(std::shared_ptr<PhyLayerFactory> new_phy) {
  phys_.push_back(new_phy);
  return phys_.size() - 1;
}

void TestModel::DelPhy(size_t phy_index) {
  if (phy_index >= phys_.size()) {
    LOG_WARN(LOG_TAG, "del_phy: index %d out of range: ", static_cast<int>(phy_index));
    return;
  }
}

void TestModel::AddDeviceToPhy(size_t dev_index, size_t phy_index) {
  if (dev_index >= devices_.size()) {
    LOG_WARN(LOG_TAG, "add_device_to_phy: device out of range: ");
    return;
  }
  if (phy_index >= phys_.size()) {
    LOG_WARN(LOG_TAG, "add_device_to_phy: phy out of range: ");
    return;
  }
  std::shared_ptr<Device> dev = devices_[dev_index];
  dev->RegisterPhyLayer(
      phys_[phy_index]->GetPhyLayer([dev](packets::LinkLayerPacketView packet) { dev->IncomingPacket(packet); }));
}

void TestModel::DelDeviceFromPhy(size_t dev_index, size_t phy_index) {
  if (dev_index >= devices_.size()) {
    LOG_WARN(LOG_TAG, "del_device_from_phy: device out of range: ");
    return;
  }
  if (phy_index >= phys_.size()) {
    LOG_WARN(LOG_TAG, "del_device_from_phy: phy out of range: ");
    return;
  }
}

void TestModel::AddLinkLayerConnection(int socket_fd, Phy::Type phy_type) {
  std::shared_ptr<Device> dev = LinkLayerSocketDevice::Create(socket_fd, phy_type);
  int index = Add(dev);
  for (size_t phy_index = 0; phy_index < phys_.size(); phy_index++) {
    if (phy_type == phys_[phy_index]->GetType()) {
      AddDeviceToPhy(index, phy_index);
    }
  }
}

void TestModel::IncomingLinkLayerConnection(int socket_fd) {
  // TODO: Handle other phys
  AddLinkLayerConnection(socket_fd, Phy::Type::BR_EDR);
}

void TestModel::AddRemote(const std::string& server, int port, Phy::Type phy_type) {
  int socket_fd = connect_to_remote_(server, port);
  if (socket_fd < 0) {
    return;
  }
  AddLinkLayerConnection(socket_fd, phy_type);
}

void TestModel::IncomingHciConnection(int socket_fd) {
  std::shared_ptr<HciSocketDevice> dev = HciSocketDevice::Create(socket_fd);
  // TODO: Auto-increment addresses?
  static int hci_devs = 0;
  int index = Add(std::static_pointer_cast<Device>(dev));
  std::string addr = "da:4c:10:de:17:0";  // Da HCI dev
  CHECK(hci_devs < 10) << "Why do you need more than 9?";
  addr += '0' + hci_devs++;
  dev->Initialize({"IgnoredTypeName", addr});
  // TODO: Add device to all phys?  For now, just the first two.
  for (size_t phy = 0; phy < 2 && phy < phys_.size(); phy++) {
    AddDeviceToPhy(index, phy);
  }
  dev->RegisterTaskScheduler(schedule_task_);
  dev->RegisterTaskCancel(cancel_task_);
}

const std::string& TestModel::List() {
  list_string_ = "";
  list_string_ += " Devices: \r\n";
  for (size_t dev = 0; dev < devices_.size(); dev++) {
    list_string_ += "  " + std::to_string(dev) + ":";
    list_string_ += devices_[dev]->ToString() + " \r\n";
  }
  list_string_ += " Phys: \r\n";
  for (size_t phy = 0; phy < phys_.size(); phy++) {
    list_string_ += "  " + std::to_string(phy) + ":";
    list_string_ += phys_[phy]->ToString() + " \r\n";
  }
  return list_string_;
}

void TestModel::TimerTick() {
  for (size_t dev = 0; dev < devices_.size(); dev++) {
    devices_[dev]->TimerTick();
  }
}

void TestModel::Reset() {
  StopTimer();
  devices_.clear();
  phys_.clear();
}

}  // namespace test_vendor_lib
