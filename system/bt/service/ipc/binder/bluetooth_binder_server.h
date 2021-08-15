//
//  Copyright (C) 2015 Google, Inc.
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

#pragma once

#include <string>
#include <vector>

#include <base/macros.h>
#include <utils/String16.h>
#include <utils/Vector.h>

#include <android/bluetooth/BnBluetooth.h>
#include <android/bluetooth/IBluetoothA2dpSink.h>
#include <android/bluetooth/IBluetoothA2dpSource.h>
#include <android/bluetooth/IBluetoothAvrcpControl.h>
#include <android/bluetooth/IBluetoothAvrcpTarget.h>
#include <android/bluetooth/IBluetoothCallback.h>
#include <android/bluetooth/IBluetoothGattClient.h>
#include <android/bluetooth/IBluetoothGattServer.h>
#include <android/bluetooth/IBluetoothLeAdvertiser.h>
#include <android/bluetooth/IBluetoothLeScanner.h>
#include <android/bluetooth/IBluetoothLowEnergy.h>

#include "bluetooth/uuid.h"
#include "service/adapter.h"
#include "service/ipc/binder/remote_callback_list.h"

using android::String16;
using android::binder::Status;

using android::bluetooth::BnBluetooth;
using android::bluetooth::IBluetoothA2dpSink;
using android::bluetooth::IBluetoothA2dpSource;
using android::bluetooth::IBluetoothAvrcpControl;
using android::bluetooth::IBluetoothAvrcpTarget;
using android::bluetooth::IBluetoothCallback;
using android::bluetooth::IBluetoothGattClient;
using android::bluetooth::IBluetoothGattServer;
using android::bluetooth::IBluetoothLeAdvertiser;
using android::bluetooth::IBluetoothLeScanner;
using android::bluetooth::IBluetoothLowEnergy;

namespace ipc {
namespace binder {

class BluetoothA2dpSinkBinderServer;
class BluetoothA2dpSourceBinderServer;
class BluetoothAvrcpTargetBinderServer;

// Implements the server side of the IBluetooth Binder interface.
class BluetoothBinderServer : public BnBluetooth,
                              public bluetooth::Adapter::Observer {
 public:
  explicit BluetoothBinderServer(bluetooth::Adapter* adapter);
  ~BluetoothBinderServer() override;

  // IBluetooth overrides:
  Status IsEnabled(bool* _aidl_return) override;
  Status GetState(int32_t* _aidl_return) override;
  Status Enable(bool* _aidl_return) override;
  Status EnableNoAutoConnect(bool* _aidl_return) override;
  Status Disable(bool* _aidl_return) override;

  Status GetAddress(::android::String16* _aidl_return) override;
  Status GetUUIDs(
      ::std::vector<::android::bluetooth::UUID>* _aidl_return) override;
  Status SetName(const ::android::String16& name, bool* _aidl_return) override;
  Status GetName(::android::String16* _aidl_return) override;
  Status SetScanMode(int32_t scan_mode, bool* _aidl_return) override;
  Status SetScanEnable(bool scan_enable, bool* _aidl_return) override;
  Status SspReply(const ::android::String16& device_address, int32_t variant,
                  bool accept, int32_t passkey, bool* _aidl_return) override;
  Status CreateBond(const ::android::String16& device_address,
                    int32_t transport, bool* _aidl_return) override;
  Status GetBondedDevices(bool* _aidl_return) override;
  Status RemoveBond(const ::android::String16& device_address,
                    bool* _aidl_return) override;
  Status GetRemoteDeviceProperties(const ::android::String16& device_address,
                                   bool* _aidl_return) override;

  Status RegisterCallback(
      const ::android::sp<IBluetoothCallback>& callback) override;
  Status UnregisterCallback(
      const ::android::sp<IBluetoothCallback>& callback) override;
  Status IsMultiAdvertisementSupported(bool* _aidl_return) override;
  Status GetA2dpSinkInterface(
      ::android::sp<IBluetoothA2dpSink>* _aidl_return) override;
  Status GetA2dpSourceInterface(
      ::android::sp<IBluetoothA2dpSource>* _aidl_return) override;
  Status GetLowEnergyInterface(
      ::android::sp<IBluetoothLowEnergy>* _aidl_return) override;
  Status GetLeAdvertiserInterface(
      ::android::sp<IBluetoothLeAdvertiser>* _aidl_return) override;
  Status GetLeScannerInterface(
      ::android::sp<IBluetoothLeScanner>* _aidl_return) override;
  Status GetGattClientInterface(
      ::android::sp<IBluetoothGattClient>* _aidl_return) override;
  Status GetGattServerInterface(
      ::android::sp<IBluetoothGattServer>* _aidl_return) override;
  Status GetAvrcpControlInterface(
      ::android::sp<IBluetoothAvrcpControl>* _aidl_return) override;
  Status GetAvrcpTargetInterface(
      ::android::sp<IBluetoothAvrcpTarget>* _aidl_return) override;

  android::status_t dump(
      int fd, const android::Vector<android::String16>& args) override;

  // bluetooth::Adapter::Observer overrides:
  void OnAdapterStateChanged(bluetooth::Adapter* adapter,
                             bluetooth::AdapterState prev_state,
                             bluetooth::AdapterState new_state) override;

  void OnDeviceConnectionStateChanged(bluetooth::Adapter* adapter,
                                      const std::string& device_address,
                                      bool connected) override;

  void OnScanEnableChanged(bluetooth::Adapter* adapter,
                           bool scan_enabled) override;

  void OnSspRequest(bluetooth::Adapter* adapter,
                    const std::string& device_address,
                    const std::string& device_name, int cod,
                    int pairing_variant, int pass_key) override;

  void OnBondStateChanged(bluetooth::Adapter* adapter, int status,
                          const std::string& device_address,
                          int state) override;
  void OnGetBondedDevices(
      bluetooth::Adapter* adapter, int status,
      const std::vector<std::string>& bonded_devices) override;
  void OnGetRemoteDeviceProperties(
      bluetooth::Adapter* adapter, int status,
      const std::string& device_address,
      const bluetooth::RemoteDeviceProps& properties) override;
  void OnDeviceFound(bluetooth::Adapter* adapter,
                     const bluetooth::RemoteDeviceProps& properties) override;

 private:
  bluetooth::Adapter* adapter_;  // weak
  RemoteCallbackList<IBluetoothCallback> callbacks_;

  // The IBluetoothA2dpSink interface handle. This is lazily initialized on the
  // first call to GetA2dpSinkInterface().
  android::sp<BluetoothA2dpSinkBinderServer> a2dp_sink_interface_;

  // The IBluetoothA2dpSource interface handle. This is lazily initialized on
  // the first call to GetA2dpSourceInterface().
  android::sp<BluetoothA2dpSourceBinderServer> a2dp_source_interface_;

  // The IBluetoothLowEnergy interface handle. This is lazily initialized on the
  // first call to GetLowEnergyInterface().
  android::sp<IBluetoothLowEnergy> low_energy_interface_;

  // The IBluetoothLeAdvertiser interface handle. This is lazily initialized on
  // the first call to GetLeAdvertiserInterface().
  android::sp<IBluetoothLeAdvertiser> le_advertiser_interface_;

  // The IBluetoothLeScanner interface handle. This is lazily initialized on the
  // first call to GetLeScannerInterface().
  android::sp<IBluetoothLeScanner> le_scanner_interface_;

  // The IBluetoothGattClient interface handle. This is lazily initialized on
  // the first call to GetGattClientInterface().
  android::sp<IBluetoothGattClient> gatt_client_interface_;

  // The IBluetoothGattServer interface handle. This is lazily initialized on
  // the first call to GetGattServerInterface().
  android::sp<IBluetoothGattServer> gatt_server_interface_;

  // The IBluetoothAvrcpControl interface handle. This is lazily initialized on
  // the first call to GetAvrcpControlInterface().
  android::sp<IBluetoothAvrcpControl> avrcp_control_interface_;

  // The IBluetoothAvrcpTarget interface handle. This is lazily initialized on
  // the first call to GetAvrcpTargetInterface().
  android::sp<BluetoothAvrcpTargetBinderServer> avrcp_target_interface_;

  DISALLOW_COPY_AND_ASSIGN(BluetoothBinderServer);
};

}  // namespace binder
}  // namespace ipc
