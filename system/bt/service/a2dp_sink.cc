//
//  Copyright 2017 Google, Inc.
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

#include "service/a2dp_sink.h"

#include <base/logging.h>
#include <base/memory/ptr_util.h>

#include "service/logging_helpers.h"

using bluetooth::hal::BluetoothAvInterface;

namespace bluetooth {

// static
const int A2dpSink::kSingletonInstanceId = 0;

A2dpSink::A2dpSink(const Uuid& uuid) : app_identifier_(uuid) {
  hal::BluetoothAvInterface::Get()->AddA2dpSinkObserver(this);
}

A2dpSink::~A2dpSink() {
  hal::BluetoothAvInterface::Get()->RemoveA2dpSinkObserver(this);
}

const Uuid& A2dpSink::GetAppIdentifier() const { return app_identifier_; }

int A2dpSink::GetInstanceId() const { return kSingletonInstanceId; }

void A2dpSink::SetDelegate(Delegate* delegate) {
  std::lock_guard<std::mutex> lock(delegate_mutex_);
  delegate_ = delegate;
}

bool A2dpSink::Enable() {
  std::lock_guard<std::mutex> lock(mutex_);
  return hal::BluetoothAvInterface::Get()->A2dpSinkEnable();
}

void A2dpSink::Disable() {
  std::lock_guard<std::mutex> lock(mutex_);
  hal::BluetoothAvInterface::Get()->A2dpSinkDisable();
}

bool A2dpSink::Connect(const std::string& device_address) {
  std::lock_guard<std::mutex> lock(mutex_);
  RawAddress addr;
  if (!RawAddress::FromString(device_address, addr)) {
    LOG(ERROR) << "Invalid device address given: " << device_address;
    return false;
  }

  bt_status_t status =
      hal::BluetoothAvInterface::Get()->GetA2dpSinkHALInterface()->connect(
          addr);
  if (status != BT_STATUS_SUCCESS) {
    LOG(ERROR) << "Failed to connect";
    return false;
  }

  return true;
}

bool A2dpSink::Disconnect(const std::string& device_address) {
  std::lock_guard<std::mutex> lock(mutex_);
  RawAddress addr;
  if (!RawAddress::FromString(device_address, addr)) {
    LOG(ERROR) << "Invalid device address given: " << device_address;
    return false;
  }

  bt_status_t status =
      hal::BluetoothAvInterface::Get()->GetA2dpSinkHALInterface()->disconnect(
          addr);
  if (status != BT_STATUS_SUCCESS) {
    LOG(ERROR) << "Failed to disconnect";
    return false;
  }

  return true;
}

void A2dpSink::SetAudioFocusState(int focus_state) {
  std::lock_guard<std::mutex> lock(mutex_);
  hal::BluetoothAvInterface::Get()
      ->GetA2dpSinkHALInterface()
      ->set_audio_focus_state(focus_state);
}

void A2dpSink::SetAudioTrackGain(float gain) {
  std::lock_guard<std::mutex> lock(mutex_);
  hal::BluetoothAvInterface::Get()
      ->GetA2dpSinkHALInterface()
      ->set_audio_track_gain(gain);
}

void A2dpSink::ConnectionStateCallback(BluetoothAvInterface* iface,
                                       const RawAddress& bd_addr,
                                       btav_connection_state_t state) {
  std::string device_address = BtAddrString(&bd_addr);
  std::lock_guard<std::mutex> lock(delegate_mutex_);

  if (delegate_)
    delegate_->OnConnectionState(device_address, static_cast<int>(state));
}

void A2dpSink::AudioStateCallback(BluetoothAvInterface* iface,
                                  const RawAddress& bd_addr,
                                  btav_audio_state_t state) {
  std::string device_address = BtAddrString(&bd_addr);
  std::lock_guard<std::mutex> lock(delegate_mutex_);

  if (delegate_)
    delegate_->OnAudioState(device_address, static_cast<int>(state));
}

void A2dpSink::AudioConfigCallback(BluetoothAvInterface* iface,
                                   const RawAddress& bd_addr,
                                   uint32_t sample_rate,
                                   uint8_t channel_count) {
  std::string device_address = BtAddrString(&bd_addr);
  std::lock_guard<std::mutex> lock(delegate_mutex_);
  if (delegate_)
    delegate_->OnAudioConfig(device_address, sample_rate, channel_count);
}

// A2dpSinkFactory implementation
// ========================================================
A2dpSinkFactory::A2dpSinkFactory() = default;
A2dpSinkFactory::~A2dpSinkFactory() = default;

bool A2dpSinkFactory::RegisterInstance(const Uuid& uuid,
                                       const RegisterCallback& callback) {
  VLOG(1) << __func__ << " - Uuid: " << uuid.ToString();

  auto a2dp_sink = base::WrapUnique(new A2dpSink(uuid));
  callback(BLE_STATUS_SUCCESS, uuid, std::move(a2dp_sink));
  return true;
}

}  // namespace bluetooth
