//
//  Copyright 2015 Google, Inc.
//
//  Licensed under the Apache License, Version 2.0 (the "License");
//  you may not use this file except in compliance with the License.
//  You may obtain a copy of the License at:
//
//  http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.
//
#include <base/bind.h>
#include <base/location.h>
#include <base/logging.h>
#include <base/rand_util.h>

#include <android/bluetooth/BnBluetoothLeAdvertiserCallback.h>
#include <android/bluetooth/IBluetoothLeAdvertiser.h>
#include <bluetooth/low_energy_constants.h>

#include "constants.h"
#include "heart_rate_server.h"

using android::binder::Status;
using android::String8;
using android::String16;

using android::bluetooth::IBluetoothLeAdvertiser;
using android::bluetooth::BluetoothGattService;

namespace heart_rate {

class CLIBluetoothLeAdvertiserCallback
    : public android::bluetooth::BnBluetoothLeAdvertiserCallback {
 public:
  explicit CLIBluetoothLeAdvertiserCallback(
      android::sp<android::bluetooth::IBluetooth> bt)
      : bt_(bt) {}

  // IBluetoothLeAdvertiserCallback overrides:
  Status OnAdvertiserRegistered(int status, int advertiser_id) {
    if (status != bluetooth::BLE_STATUS_SUCCESS) {
      LOG(ERROR)
          << "Failed to register BLE advertiser, will not start advertising";
      return Status::ok();
    }

    LOG(INFO) << "Registered BLE advertiser with ID: " << advertiser_id;

    String16 name_param;
    bt_->GetName(&name_param);
    std::string name(String8(name_param).string());

    /* Advertising data: 16-bit Service Uuid: Heart Rate Service, Tx power*/
    std::vector<uint8_t> data{0x03, bluetooth::kEIRTypeComplete16BitUuids,
                              0x0D, 0x18,
                              0x02, bluetooth::kEIRTypeTxPower,
                              0x00};
    data.push_back(name.length() + 1);
    data.push_back(bluetooth::kEIRTypeCompleteLocalName);
    data.insert(data.end(), name.c_str(), name.c_str() + name.length());

    base::TimeDelta timeout;

    bluetooth::AdvertiseSettings settings(
        bluetooth::AdvertiseSettings::MODE_LOW_POWER, timeout,
        bluetooth::AdvertiseSettings::TX_POWER_LEVEL_MEDIUM, true);

    bluetooth::AdvertiseData adv_data(data);
    bluetooth::AdvertiseData scan_rsp;

    android::sp<IBluetoothLeAdvertiser> ble;
    bt_->GetLeAdvertiserInterface(&ble);
    bool start_status;
    ble->StartMultiAdvertising(advertiser_id, adv_data, scan_rsp, settings,
                               &start_status);
    return Status::ok();
  }

  Status OnMultiAdvertiseCallback(
      int status, bool is_start,
      const android::bluetooth::AdvertiseSettings& /* settings */) {
    LOG(INFO) << "Advertising" << (is_start ? " started" : " stopped");
    return Status::ok();
  };

 private:
  android::sp<android::bluetooth::IBluetooth> bt_;
  DISALLOW_COPY_AND_ASSIGN(CLIBluetoothLeAdvertiserCallback);
};

HeartRateServer::HeartRateServer(
    android::sp<android::bluetooth::IBluetooth> bluetooth,
    scoped_refptr<base::SingleThreadTaskRunner> main_task_runner,
    bool advertise)
    : simulation_started_(false),
      bluetooth_(bluetooth),
      server_if_(-1),
      hr_notification_count_(0),
      energy_expended_(0),
      advertise_(advertise),
      main_task_runner_(main_task_runner),
      weak_ptr_factory_(this) {
  CHECK(bluetooth_.get());
}

HeartRateServer::~HeartRateServer() {
  std::lock_guard<std::mutex> lock(mutex_);
  if (!gatt_.get() || server_if_ == -1) return;

  if (!android::IInterface::asBinder(gatt_.get())->isBinderAlive()) return;

  // Manually unregister ourselves from the daemon. It's good practice to do
  // this, even though the daemon will automatically unregister us if this
  // process exits.
  gatt_->UnregisterServer(server_if_);
}

bool HeartRateServer::Run(const RunCallback& callback) {
  std::lock_guard<std::mutex> lock(mutex_);

  if (pending_run_cb_) {
    LOG(ERROR) << "Already started";
    return false;
  }

  // Grab the IBluetoothGattServer binder from the Bluetooth daemon.
  bluetooth_->GetGattServerInterface(&gatt_);
  if (!gatt_.get()) {
    LOG(ERROR) << "Failed to obtain handle to IBluetoothGattServer interface";
    return false;
  }

  // Register this instance as a GATT server. If this call succeeds, we will
  // asynchronously receive a server ID via the OnServerRegistered callback.
  bool status;
  gatt_->RegisterServer(this, &status);
  if (!status) {
    LOG(ERROR) << "Failed to register with the server interface";
    return false;
  }

  pending_run_cb_ = callback;

  return true;
}

void HeartRateServer::ScheduleNextMeasurement() {
  main_task_runner_->PostDelayedTask(
      FROM_HERE, base::Bind(&HeartRateServer::SendHeartRateMeasurement,
                            weak_ptr_factory_.GetWeakPtr()),
      base::TimeDelta::FromSeconds(1));
}

void HeartRateServer::SendHeartRateMeasurement() {
  std::lock_guard<std::mutex> lock(mutex_);

  // Send a notification or indication to all enabled devices.
  bool found = false;
  for (const auto& iter : device_ccc_map_) {
    uint8_t ccc_val = iter.second;

    if (!ccc_val) continue;

    found = true;

    // Don't send a notification if one is already pending for this device.
    if (pending_notification_map_[iter.first]) continue;

    std::vector<uint8_t> value;
    BuildHeartRateMeasurementValue(&value);

    bool status;
    gatt_->SendNotification(server_if_, String16(String8(iter.first.c_str())),
                            hr_measurement_handle_, false, value, &status);
    if (status) pending_notification_map_[iter.first] = true;
  }

  // Still enabled!
  if (found) {
    ScheduleNextMeasurement();
    return;
  }

  // All clients disabled notifications.
  simulation_started_ = false;

  // TODO(armansito): We should keep track of closed connections here so that we
  // don't send notifications to uninterested clients.
}

void HeartRateServer::BuildHeartRateMeasurementValue(
    std::vector<uint8_t>* out_value) {
  CHECK(out_value);  // Assert that |out_value| is not nullptr.

  // Default flags field. Here is what we put in there:
  //   Bit 0: 0 - 8-bit Heart Rate value
  //   Bits 1 & 2: 11 - Sensor contact feature supported and contact detected.
  uint8_t flags = kHRValueFormat8Bit | kHRSensorContactDetected;

  // Our demo's heart rate. Pick a value between 90 and 130.
  uint8_t heart_rate = base::RandInt(90, 130);

  // On every tenth beat we include the Energy Expended value.
  bool include_ee = false;
  if (!(hr_notification_count_ % 10)) {
    include_ee = true;
    flags |= kHREnergyExpendedPresent;
  }

  hr_notification_count_++;
  energy_expended_ = std::min(UINT16_MAX, (int)energy_expended_ + 1);

  // Add all the value bytes.
  out_value->push_back(flags);
  out_value->push_back(heart_rate);
  if (include_ee) {
    out_value->push_back(energy_expended_);
    out_value->push_back(energy_expended_ >> 8);
  }
}

Status HeartRateServer::OnServerRegistered(int status, int server_if) {
  std::lock_guard<std::mutex> lock(mutex_);

  if (status != bluetooth::BLE_STATUS_SUCCESS) {
    LOG(ERROR) << "Failed to register GATT server";
    pending_run_cb_(false);
    return Status::ok();
  }

  // Registration succeeded. Store our ID, as we need it for GATT server
  // operations.
  server_if_ = server_if;

  LOG(INFO) << "Heart Rate server registered - server_if: " << server_if_;

  bluetooth::Service hrService(0, true, kHRServiceUuid,
                               {{0,
                                 kHRMeasurementUuid,
                                 bluetooth::kCharacteristicPropertyNotify,
                                 0,
                                 {{0, kCCCDescriptorUuid,
                                   (bluetooth::kAttributePermissionRead |
                                    bluetooth::kAttributePermissionWrite)}}},
                                {0,
                                 kBodySensorLocationUuid,
                                 bluetooth::kCharacteristicPropertyRead,
                                 bluetooth::kAttributePermissionRead,
                                 {}},
                                {0,
                                 kHRControlPointUuid,
                                 bluetooth::kCharacteristicPropertyWrite,
                                 bluetooth::kAttributePermissionWrite,
                                 {}}},
                               {});

  bool op_status = true;

  Status stat = gatt_->AddService(server_if_, (BluetoothGattService)hrService,
                                  &op_status);
  if (!stat.isOk()) {
    LOG(ERROR) << "Failed to add service, status is: " /*<< stat*/;
    pending_run_cb_(false);
    return Status::ok();
  }

  if (!op_status) {
    LOG(ERROR) << "Failed to add service";
    pending_run_cb_(false);
    return Status::ok();
  }

  LOG(INFO) << "Initiated AddService request";
  return Status::ok();
}

Status HeartRateServer::OnServiceAdded(
    int status, const android::bluetooth::BluetoothGattService& service) {
  std::lock_guard<std::mutex> lock(mutex_);

  if (status != bluetooth::BLE_STATUS_SUCCESS) {
    LOG(ERROR) << "Failed to add Heart Rate service";
    pending_run_cb_(false);
    return Status::ok();
  }

  hr_service_handle_ = service.handle();
  hr_measurement_handle_ = service.characteristics()[0].handle();
  hr_measurement_cccd_handle_ =
      service.characteristics()[0].descriptors()[0].handle();
  body_sensor_loc_handle_ = service.characteristics()[1].handle();
  hr_control_point_handle_ = service.characteristics()[2].handle();

  LOG(INFO) << "Heart Rate service added";
  pending_run_cb_(true);

  if (advertise_) {
    android::sp<IBluetoothLeAdvertiser> ble;
    bluetooth_->GetLeAdvertiserInterface(&ble);
    bool status;
    ble->RegisterAdvertiser(new CLIBluetoothLeAdvertiserCallback(bluetooth_),
                            &status);
  }

  return Status::ok();
}

Status HeartRateServer::OnCharacteristicReadRequest(
    const String16& device_address, int request_id, int offset,
    bool /* is_long */, int handle) {
  std::lock_guard<std::mutex> lock(mutex_);

  // This is where we handle an incoming characteristic read. Only the body
  // sensor location characteristic is readable.
  CHECK(handle == body_sensor_loc_handle_);

  std::vector<uint8_t> value;
  bluetooth::GATTError error = bluetooth::GATT_ERROR_NONE;
  if (offset > 1)
    error = bluetooth::GATT_ERROR_INVALID_OFFSET;
  else if (offset == 0)
    value.push_back(kHRBodyLocationFoot);

  bool status;
  gatt_->SendResponse(server_if_, device_address, request_id, error, offset,
                      value, &status);
  return Status::ok();
}

Status HeartRateServer::OnDescriptorReadRequest(const String16& device_address,
                                                int request_id, int offset,
                                                bool /* is_long */,
                                                int handle) {
  std::lock_guard<std::mutex> lock(mutex_);

  // This is where we handle an incoming characteristic descriptor read. There
  // is only one descriptor.
  if (handle != hr_measurement_cccd_handle_) {
    std::vector<uint8_t> value;
    bool status;
    gatt_->SendResponse(server_if_, device_address, request_id,
                        bluetooth::GATT_ERROR_ATTRIBUTE_NOT_FOUND, offset,
                        value, &status);
    return Status::ok();
  }

  // 16-bit value encoded as little-endian.
  const uint8_t value_bytes[] = {
      device_ccc_map_[std::string(String8(device_address).string())], 0x00};

  std::vector<uint8_t> value;
  bluetooth::GATTError error = bluetooth::GATT_ERROR_NONE;
  if (offset > 2)
    error = bluetooth::GATT_ERROR_INVALID_OFFSET;
  else
    value.insert(value.begin(), value_bytes + offset, value_bytes + 2 - offset);

  bool status;
  gatt_->SendResponse(server_if_, device_address, request_id, error, offset,
                      value, &status);
  return Status::ok();
}

Status HeartRateServer::OnCharacteristicWriteRequest(
    const String16& device_address, int request_id, int offset,
    bool is_prepare_write, bool need_response,
    const std::vector<uint8_t>& value, int handle) {
  std::lock_guard<std::mutex> lock(mutex_);

  std::vector<uint8_t> dummy;

  // This is where we handle an incoming characteristic write. The Heart Rate
  // service doesn't really support prepared writes, so we just reject them to
  // keep things simple.
  if (is_prepare_write) {
    bool status;
    gatt_->SendResponse(server_if_, device_address, request_id,
                        bluetooth::GATT_ERROR_REQUEST_NOT_SUPPORTED, offset,
                        dummy, &status);
    return Status::ok();
  }

  // Heart Rate Control point is the only writable characteristic.
  CHECK(handle == hr_control_point_handle_);

  // Writes to the Heart Rate Control Point characteristic must contain a single
  // byte with the value 0x01.
  if (value.size() != 1 || value[0] != 0x01) {
    bool status;
    gatt_->SendResponse(server_if_, device_address, request_id,
                        bluetooth::GATT_ERROR_OUT_OF_RANGE, offset, dummy,
                        &status);
    return Status::ok();
  }

  LOG(INFO) << "Heart Rate Control Point written; Enery Expended reset!";
  energy_expended_ = 0;

  if (!need_response) return Status::ok();

  bool status;
  gatt_->SendResponse(server_if_, device_address, request_id,
                      bluetooth::GATT_ERROR_NONE, offset, dummy, &status);
  return Status::ok();
}

Status HeartRateServer::OnDescriptorWriteRequest(
    const String16& device_address, int request_id, int offset,
    bool is_prepare_write, bool need_response,
    const std::vector<uint8_t>& value, int handle) {
  std::lock_guard<std::mutex> lock(mutex_);

  std::vector<uint8_t> dummy;

  // This is where we handle an incoming characteristic write. The Heart Rate
  // service doesn't really support prepared writes, so we just reject them to
  // keep things simple.
  if (is_prepare_write) {
    bool status;
    gatt_->SendResponse(server_if_, device_address, request_id,
                        bluetooth::GATT_ERROR_REQUEST_NOT_SUPPORTED, offset,
                        dummy, &status);
    return Status::ok();
  }

  // CCC is the only descriptor we have.
  CHECK(handle == hr_measurement_cccd_handle_);

  // CCC must contain 2 bytes for a 16-bit value in little-endian. The only
  // allowed values here are 0x0000 and 0x0001.
  if (value.size() != 2 || value[1] != 0x00 || value[0] > 0x01) {
    bool status;
    gatt_->SendResponse(server_if_, device_address, request_id,
                        bluetooth::GATT_ERROR_CCCD_IMPROPERLY_CONFIGURED,
                        offset, dummy, &status);
    return Status::ok();
  }

  device_ccc_map_[std::string(String8(device_address).string())] = value[0];

  LOG(INFO) << "Heart Rate Measurement CCC written - device: " << device_address
            << " value: " << (int)value[0];

  // Start the simulation.
  if (!simulation_started_ && value[0]) {
    simulation_started_ = true;
    ScheduleNextMeasurement();
  }

  if (!need_response) return Status::ok();

  bool status;
  gatt_->SendResponse(server_if_, device_address, request_id,
                      bluetooth::GATT_ERROR_NONE, offset, dummy, &status);
  return Status::ok();
}

Status HeartRateServer::OnExecuteWriteRequest(const String16& device_address,
                                              int request_id,
                                              bool /* is_execute */) {
  // We don't support Prepared Writes so, simply return Not Supported error.
  std::vector<uint8_t> dummy;
  bool status;
  gatt_->SendResponse(server_if_, device_address, request_id,
                      bluetooth::GATT_ERROR_REQUEST_NOT_SUPPORTED, 0, dummy,
                      &status);

  return Status::ok();
}

Status HeartRateServer::OnNotificationSent(const String16& device_address,
                                           int status) {
  LOG(INFO) << "Notification was sent - device: " << device_address
            << " status: " << status;
  std::lock_guard<std::mutex> lock(mutex_);
  pending_notification_map_[std::string(String8(device_address).string())] =
      false;

  return Status::ok();
}

Status HeartRateServer::OnConnectionStateChanged(const String16& device_address,
                                                 bool connected) {
  LOG(INFO) << "Connection state changed - device: " << device_address
            << " connected: " << (connected ? "true" : "false");
  return Status::ok();
}
}  // namespace heart_rate
