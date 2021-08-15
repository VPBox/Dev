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

#include "service/ipc/binder/bluetooth_binder_server.h"

#include <base/logging.h>

#include "service/ipc/binder/bluetooth_a2dp_sink_binder_server.h"
#include "service/ipc/binder/bluetooth_a2dp_source_binder_server.h"
#include "service/ipc/binder/bluetooth_avrcp_control_binder_server.h"
#include "service/ipc/binder/bluetooth_avrcp_target_binder_server.h"
#include "service/ipc/binder/bluetooth_gatt_client_binder_server.h"
#include "service/ipc/binder/bluetooth_gatt_server_binder_server.h"
#include "service/ipc/binder/bluetooth_le_advertiser_binder_server.h"
#include "service/ipc/binder/bluetooth_le_scanner_binder_server.h"
#include "service/ipc/binder/bluetooth_low_energy_binder_server.h"

#include "service/hal/bluetooth_interface.h"

using android::sp;
using android::String8;
using android::String16;

using android::bluetooth::IBluetoothCallback;
using android::bluetooth::IBluetoothGattClient;
using android::bluetooth::IBluetoothGattServer;

namespace ipc {
namespace binder {

BluetoothBinderServer::BluetoothBinderServer(bluetooth::Adapter* adapter)
    : adapter_(adapter) {
  CHECK(adapter_);
  adapter_->AddObserver(this);
}

BluetoothBinderServer::~BluetoothBinderServer() {
  adapter_->RemoveObserver(this);
}

// binder::BnBluetooth overrides:
Status BluetoothBinderServer::IsEnabled(bool* _aidl_return) {
  VLOG(2) << __func__;
  *_aidl_return = adapter_->IsEnabled();
  return Status::ok();
}

Status BluetoothBinderServer::GetState(int32_t* _aidl_return) {
  VLOG(2) << __func__;
  *_aidl_return = adapter_->GetState();
  return Status::ok();
}

Status BluetoothBinderServer::Enable(bool* _aidl_return) {
  VLOG(2) << __func__;
  *_aidl_return = adapter_->Enable();
  return Status::ok();
}

Status BluetoothBinderServer::EnableNoAutoConnect(bool* _aidl_return) {
  VLOG(2) << __func__;
  // TODO(armansito): Implement.
  *_aidl_return = false;
  return Status::ok();
}

Status BluetoothBinderServer::Disable(bool* _aidl_return) {
  VLOG(2) << __func__;
  *_aidl_return = adapter_->Disable();
  return Status::ok();
}

Status BluetoothBinderServer::GetAddress(::android::String16* _aidl_return) {
  VLOG(2) << __func__;
  *_aidl_return = String16(String8(adapter_->GetAddress().c_str()));
  return Status::ok();
}

Status BluetoothBinderServer::GetUUIDs(
    ::std::vector<::android::bluetooth::UUID>* _aidl_return) {
  VLOG(2) << __func__;
  // TODO(armansito): Implement.
  *_aidl_return = std::vector<android::bluetooth::UUID>();
  return Status::ok();
}

Status BluetoothBinderServer::SetName(const ::android::String16& name,
                                      bool* _aidl_return) {
  VLOG(2) << __func__;
  *_aidl_return = adapter_->SetName(std::string(String8(name).string()));
  return Status::ok();
}

Status BluetoothBinderServer::GetName(::android::String16* _aidl_return) {
  VLOG(2) << __func__;
  *_aidl_return = String16(String8(adapter_->GetName().c_str()));
  return Status::ok();
}

Status BluetoothBinderServer::SetScanMode(int32_t scan_mode,
                                          bool* _aidl_return) {
  VLOG(2) << __func__;
  *_aidl_return = adapter_->SetScanMode(scan_mode);
  return Status::ok();
}

Status BluetoothBinderServer::SetScanEnable(bool scan_enable,
                                            bool* _aidl_return) {
  VLOG(2) << __func__;
  *_aidl_return = adapter_->SetScanEnable(scan_enable);
  return Status::ok();
}

Status BluetoothBinderServer::SspReply(
    const ::android::String16& device_address, int32_t variant, bool accept,
    int32_t passkey, bool* _aidl_return) {
  VLOG(2) << __func__;
  *_aidl_return = adapter_->SspReply(String8(device_address).string(), variant,
                                     accept, passkey);
  return Status::ok();
}

Status BluetoothBinderServer::CreateBond(
    const ::android::String16& device_address, int32_t transport,
    bool* _aidl_return) {
  VLOG(2) << __func__;
  *_aidl_return =
      adapter_->CreateBond(String8(device_address).string(), transport);
  return Status::ok();
}

Status BluetoothBinderServer::GetBondedDevices(bool* _aidl_return) {
  VLOG(2) << __func__;
  *_aidl_return = adapter_->GetBondedDevices();
  return Status::ok();
}

Status BluetoothBinderServer::RemoveBond(
    const ::android::String16& device_address, bool* _aidl_return) {
  VLOG(2) << __func__;
  *_aidl_return = adapter_->RemoveBond(String8(device_address).string());
  return Status::ok();
}

Status BluetoothBinderServer::GetRemoteDeviceProperties(
    const ::android::String16& device_address, bool* _aidl_return) {
  VLOG(2) << __func__;
  *_aidl_return =
      adapter_->GetRemoteDeviceProperties(String8(device_address).string());
  return Status::ok();
}

Status BluetoothBinderServer::RegisterCallback(
    const ::android::sp<IBluetoothCallback>& callback) {
  VLOG(2) << __func__;
  if (!callback.get()) {
    LOG(ERROR) << "RegisterCallback called with NULL binder. Ignoring.";
    return Status::ok();
  }
  callbacks_.Register(callback);
  return Status::ok();
}

Status BluetoothBinderServer::UnregisterCallback(
    const ::android::sp<IBluetoothCallback>& callback) {
  VLOG(2) << __func__;
  if (!callback.get()) {
    LOG(ERROR) << "UnregisterCallback called with NULL binder. Ignoring.";
    return Status::ok();
  }
  callbacks_.Unregister(callback);
  return Status::ok();
}

Status BluetoothBinderServer::IsMultiAdvertisementSupported(
    bool* _aidl_return) {
  VLOG(2) << __func__;
  *_aidl_return = adapter_->IsMultiAdvertisementSupported();
  return Status::ok();
}

Status BluetoothBinderServer::GetA2dpSinkInterface(
    ::android::sp<IBluetoothA2dpSink>* _aidl_return) {
  VLOG(2) << __func__;

  if (!adapter_->IsEnabled()) {
    LOG(ERROR) << "Cannot obtain IBluetoothA2dpSink interface while disabled";
    *_aidl_return = nullptr;
    return Status::ok();
  }

  if (!a2dp_sink_interface_.get())
    a2dp_sink_interface_ = new BluetoothA2dpSinkBinderServer(adapter_);

  if (a2dp_sink_interface_->HasInstance()) {
    LOG(ERROR) << "Only one A2dpSink interface allowed at a time";
    *_aidl_return = nullptr;
    return Status::ok();
  }

  *_aidl_return = a2dp_sink_interface_;
  return Status::ok();
}

Status BluetoothBinderServer::GetA2dpSourceInterface(
    ::android::sp<IBluetoothA2dpSource>* _aidl_return) {
  VLOG(2) << __func__;

  if (!adapter_->IsEnabled()) {
    LOG(ERROR) << "Cannot obtain IBluetoothA2dpSource interface while disabled";
    *_aidl_return = nullptr;
    return Status::ok();
  }

  if (!a2dp_source_interface_.get())
    a2dp_source_interface_ = new BluetoothA2dpSourceBinderServer(adapter_);

  if (a2dp_source_interface_->HasInstance()) {
    LOG(ERROR) << "Only one A2dpSource interface allowed at a time";
    *_aidl_return = nullptr;
    return Status::ok();
  }

  *_aidl_return = a2dp_source_interface_;
  return Status::ok();
}

Status BluetoothBinderServer::GetLowEnergyInterface(
    ::android::sp<IBluetoothLowEnergy>* _aidl_return) {
  VLOG(2) << __func__;

  if (!adapter_->IsEnabled()) {
    LOG(ERROR) << "Cannot obtain IBluetoothLowEnergy interface while disabled";
    *_aidl_return = NULL;
    return Status::ok();
  }

  if (!low_energy_interface_.get())
    low_energy_interface_ = new BluetoothLowEnergyBinderServer(adapter_);

  *_aidl_return = low_energy_interface_;
  return Status::ok();
}

Status BluetoothBinderServer::GetLeAdvertiserInterface(
    ::android::sp<IBluetoothLeAdvertiser>* _aidl_return) {
  VLOG(2) << __func__;

  if (!adapter_->IsEnabled()) {
    LOG(ERROR)
        << "Cannot obtain IBluetoothLeAdvertiser interface while disabled";
    *_aidl_return = NULL;
    return Status::ok();
  }

  if (!le_advertiser_interface_.get())
    le_advertiser_interface_ = new BluetoothLeAdvertiserBinderServer(adapter_);

  *_aidl_return = le_advertiser_interface_;
  return Status::ok();
}

Status BluetoothBinderServer::GetLeScannerInterface(
    ::android::sp<IBluetoothLeScanner>* _aidl_return) {
  VLOG(2) << __func__;

  if (!adapter_->IsEnabled()) {
    LOG(ERROR) << "Cannot obtain IBluetoothLeScanner interface while disabled";
    *_aidl_return = NULL;
    return Status::ok();
  }

  if (!le_scanner_interface_.get())
    le_scanner_interface_ = new BluetoothLeScannerBinderServer(adapter_);

  *_aidl_return = le_scanner_interface_;
  return Status::ok();
}

Status BluetoothBinderServer::GetGattClientInterface(
    ::android::sp<IBluetoothGattClient>* _aidl_return) {
  VLOG(2) << __func__;

  if (!adapter_->IsEnabled()) {
    LOG(ERROR) << "Cannot obtain IBluetoothGattClient interface while disabled";
    *_aidl_return = NULL;
    return Status::ok();
  }

  if (!gatt_client_interface_.get())
    gatt_client_interface_ = new BluetoothGattClientBinderServer(adapter_);

  *_aidl_return = gatt_client_interface_;
  return Status::ok();
}

Status BluetoothBinderServer::GetGattServerInterface(
    ::android::sp<IBluetoothGattServer>* _aidl_return) {
  VLOG(2) << __func__;

  if (!adapter_->IsEnabled()) {
    LOG(ERROR) << "Cannot obtain IBluetoothGattServer interface while disabled";
    *_aidl_return = NULL;
    return Status::ok();
  }

  if (!gatt_server_interface_.get())
    gatt_server_interface_ = new BluetoothGattServerBinderServer(adapter_);

  *_aidl_return = gatt_server_interface_;
  return Status::ok();
}

Status BluetoothBinderServer::GetAvrcpControlInterface(
    ::android::sp<IBluetoothAvrcpControl>* _aidl_return) {
  VLOG(2) << __func__;

  if (!adapter_->IsEnabled()) {
    LOG(ERROR)
        << "Cannot obtain IBluetoothAvrcpControl interface while disabled";
    *_aidl_return = NULL;
    return Status::ok();
  }

  if (!avrcp_control_interface_.get())
    avrcp_control_interface_ = new BluetoothAvrcpControlBinderServer(adapter_);

  *_aidl_return = avrcp_control_interface_;
  return Status::ok();
}

Status BluetoothBinderServer::GetAvrcpTargetInterface(
    ::android::sp<IBluetoothAvrcpTarget>* _aidl_return) {
  VLOG(2) << __func__;

  if (!adapter_->IsEnabled()) {
    LOG(ERROR)
        << "Cannot obtain IBluetoothAvrcpTarget interface while disabled";
    *_aidl_return = NULL;
    return Status::ok();
  }

  if (!avrcp_target_interface_.get())
    avrcp_target_interface_ = new BluetoothAvrcpTargetBinderServer(adapter_);

  if (avrcp_target_interface_->HasInstance()) {
    LOG(ERROR) << "Only one AVRCP target interface allowed at a time";
    *_aidl_return = nullptr;
    return Status::ok();
  }

  *_aidl_return = avrcp_target_interface_;
  return Status::ok();
}

android::status_t BluetoothBinderServer::dump(
    int fd, const android::Vector<android::String16>& args) {
  VLOG(2) << __func__ << " called with fd " << fd;
  if (args.size() > 0) {
    // TODO (jamuraa): Parse arguments and switch on --proto, --proto_text
    for (const auto& x : args) {
      VLOG(2) << __func__ << "argument: " << x.string();
    }
  }
  // TODO (jamuraa): enumerate profiles and dump profile information
  const bt_interface_t* iface =
      bluetooth::hal::BluetoothInterface::Get()->GetHALInterface();
  iface->dump(fd, NULL);
  return android::NO_ERROR;
}

void BluetoothBinderServer::OnAdapterStateChanged(
    bluetooth::Adapter* adapter, bluetooth::AdapterState prev_state,
    bluetooth::AdapterState new_state) {
  CHECK_EQ(adapter, adapter_);
  VLOG(2) << "Received adapter state update - prev: " << prev_state
          << " new: " << new_state;
  callbacks_.ForEach([prev_state, new_state](IBluetoothCallback* callback) {
    callback->OnBluetoothStateChange(prev_state, new_state);
  });
}

void BluetoothBinderServer::OnDeviceConnectionStateChanged(
    bluetooth::Adapter* adapter, const std::string& device_address,
    bool connected) {
  CHECK_EQ(adapter, adapter_);
  auto addr_s16 = String16(device_address.c_str(), device_address.size());
  callbacks_.ForEach([&addr_s16, connected](IBluetoothCallback* callback) {
    callback->OnDeviceConnectionStateChanged(addr_s16, connected);
  });
}

void BluetoothBinderServer::OnScanEnableChanged(bluetooth::Adapter* adapter,
                                                bool scan_enabled) {
  CHECK_EQ(adapter, adapter_);
  callbacks_.ForEach([scan_enabled](IBluetoothCallback* callback) {
    callback->OnScanEnableChanged(scan_enabled);
  });
}

void BluetoothBinderServer::OnSspRequest(bluetooth::Adapter* adapter,
                                         const std::string& device_address,
                                         const std::string& device_name,
                                         int cod, int pairing_variant,
                                         int pass_key) {
  CHECK_EQ(adapter, adapter_);
  VLOG(2) << "Received ssp request: device_address: " << device_address
          << ", device_name: " << device_name << ", cod: " << cod
          << ", pairing_variant: " << pairing_variant
          << ", pass_key: " << pass_key;

  android::String16 addr_s16(device_address.c_str());
  android::String16 name_s16(device_name.c_str());
  callbacks_.ForEach([&addr_s16, &name_s16, cod, pairing_variant,
                      pass_key](IBluetoothCallback* callback) {
    callback->OnSspRequest(addr_s16, name_s16, cod, pairing_variant, pass_key);
  });
}

void BluetoothBinderServer::OnBondStateChanged(
    bluetooth::Adapter* adapter, int status, const std::string& device_address,
    int state) {
  CHECK_EQ(adapter, adapter_);
  VLOG(2) << "Received " << __func__ << " "
          << "status: " << status << ", device_address: " << device_address
          << ", state: " << state;
  android::String16 addr_s16(device_address.c_str(), device_address.size());
  callbacks_.ForEach([status, &addr_s16, state](IBluetoothCallback* callback) {
    callback->OnBondStateChanged(status, addr_s16, state);
  });
}

void BluetoothBinderServer::OnGetBondedDevices(
    bluetooth::Adapter* adapter, int status,
    const std::vector<std::string>& bonded_devices) {
  CHECK_EQ(adapter, adapter_);
  VLOG(2) << "Received " << __func__;
  std::vector<android::String16> devices_s16;
  devices_s16.reserve(bonded_devices.size());
  for (const auto& device : bonded_devices)
    devices_s16.emplace_back(device.c_str(), device.size());

  callbacks_.ForEach([status, &devices_s16](IBluetoothCallback* callback) {
    callback->OnGetBondedDevices(status, devices_s16);
  });
}

void BluetoothBinderServer::OnGetRemoteDeviceProperties(
    bluetooth::Adapter* adapter, int status, const std::string& device_address,
    const bluetooth::RemoteDeviceProps& properties) {
  CHECK_EQ(adapter, adapter_);
  VLOG(2) << "Received " << __func__ << " "
          << "status: " << status << ", device_address: " << device_address;
  android::String16 addr_s16(device_address.c_str(), device_address.size());
  auto binder_props =
      android::bluetooth::BluetoothRemoteDeviceProps(properties);
  callbacks_.ForEach(
      [status, &addr_s16, &binder_props](IBluetoothCallback* callback) {
        callback->OnGetRemoteDeviceProperties(status, addr_s16, binder_props);
      });
}

void BluetoothBinderServer::OnDeviceFound(
    bluetooth::Adapter* adapter,
    const bluetooth::RemoteDeviceProps& properties) {
  CHECK_EQ(adapter, adapter_);
  VLOG(2) << "Received " << __func__ << " ";
  auto binder_props =
      android::bluetooth::BluetoothRemoteDeviceProps(properties);
  callbacks_.ForEach([&binder_props](IBluetoothCallback* callback) {
    callback->OnDeviceFound(binder_props);
  });
}

}  // namespace binder
}  // namespace ipc
