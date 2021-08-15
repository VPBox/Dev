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

#pragma once

#include <atomic>
#include <functional>
#include <map>
#include <mutex>

#include <base/macros.h>
#include <bluetooth/uuid.h>

#include "service/bluetooth_instance.h"
#include "service/common/bluetooth/low_energy_constants.h"
#include "service/common/bluetooth/scan_filter.h"
#include "service/common/bluetooth/scan_result.h"
#include "service/common/bluetooth/scan_settings.h"
#include "service/hal/bluetooth_gatt_interface.h"

namespace bluetooth {

struct ConnComparator {
  bool operator()(const RawAddress& a, const RawAddress& b) const {
    return memcmp(a.address, b.address, RawAddress::kLength) < 0;
  }
};

class Adapter;

// A LowEnergyClient represents an application's handle to perform various
// Bluetooth Low Energy GAP operations. Instances cannot be created directly and
// should be obtained through the factory.
class LowEnergyClient : private hal::BluetoothGattInterface::ClientObserver,
                        public BluetoothInstance {
 public:
  // The Delegate interface is used to notify asynchronous events related to BLE
  // GAP operations.
  class Delegate {
   public:
    Delegate() = default;
    virtual ~Delegate() = default;

    // Called asynchronously to notify the delegate of connection state change
    virtual void OnConnectionState(LowEnergyClient* client, int status,
                                   const char* address, bool connected) = 0;

    // Called asynchronously to notify the delegate of mtu change
    virtual void OnMtuChanged(LowEnergyClient* client, int status,
                              const char* address, int mtu) = 0;

   private:
    DISALLOW_COPY_AND_ASSIGN(Delegate);
  };

  // The destructor automatically unregisters this client instance from the
  // stack.
  ~LowEnergyClient() override;

  // Assigns a delegate to this instance. |delegate| must out-live this
  // LowEnergyClient instance.
  void SetDelegate(Delegate* delegate);

  // Callback type used to return the result of asynchronous operations below.
  using StatusCallback = std::function<void(BLEStatus)>;

  // Initiates a BLE connection do device with address |address|. If
  // |is_direct| is set, use direct connect procedure. Return true on success
  //, false otherwise.
  bool Connect(const std::string& address, bool is_direct);

  // Disconnect from previously connected BLE device with address |address|.
  // Return true on success, false otherwise.
  bool Disconnect(const std::string& address);

  // Sends request to set MTU to |mtu| for device with address |address|.
  // Return true on success, false otherwise.
  bool SetMtu(const std::string& address, int mtu);

  // BluetoothClientInstace overrides:
  const Uuid& GetAppIdentifier() const override;
  int GetInstanceId() const override;

 private:
  friend class LowEnergyClientFactory;

  // Constructor shouldn't be called directly as instances are meant to be
  // obtained from the factory.
  LowEnergyClient(Adapter& adapter, const Uuid& uuid, int client_id);

  // BluetoothGattInterface::ClientObserver overrides:
  void ConnectCallback(hal::BluetoothGattInterface* gatt_iface, int conn_id,
                       int status, int client_id,
                       const RawAddress& bda) override;
  void DisconnectCallback(hal::BluetoothGattInterface* gatt_iface, int conn_id,
                          int status, int client_id,
                          const RawAddress& bda) override;
  void MtuChangedCallback(hal::BluetoothGattInterface* gatt_iface, int conn_id,
                          int status, int mtu) override;

  // Calls and clears the pending callbacks.
  void InvokeAndClearStartCallback(BLEStatus status);
  void InvokeAndClearStopCallback(BLEStatus status);

  // Raw pointer to the Bluetooth Adapter.
  Adapter& adapter_;

  // See getters above for documentation.
  Uuid app_identifier_;
  int client_id_;

  // Raw handle to the Delegate, which must outlive this LowEnergyClient
  // instance.
  std::mutex delegate_mutex_;
  Delegate* delegate_;

  // Protects device connection related members below.
  std::mutex connection_fields_lock_;

  // Maps bluetooth address to connection id
  // TODO(jpawlowski): change type to bimap
  std::map<const RawAddress, int, ConnComparator> connection_ids_;

  DISALLOW_COPY_AND_ASSIGN(LowEnergyClient);
};

// LowEnergyClientFactory is used to register and obtain a per-application
// LowEnergyClient instance. Users should call RegisterInstance to obtain their
// own unique LowEnergyClient instance that has been registered with the
// Bluetooth stack.
class LowEnergyClientFactory
    : private hal::BluetoothGattInterface::ClientObserver,
      public BluetoothInstanceFactory {
 public:
  // Don't construct/destruct directly except in tests. Instead, obtain a handle
  // from an Adapter instance.
  explicit LowEnergyClientFactory(Adapter& adapter);
  ~LowEnergyClientFactory() override;

  // BluetoothInstanceFactory override:
  bool RegisterInstance(const Uuid& uuid,
                        const RegisterCallback& callback) override;

 private:
  friend class LowEnergyClient;

  // BluetoothGattInterface::ClientObserver overrides:
  void RegisterClientCallback(hal::BluetoothGattInterface* gatt_iface,
                              int status, int client_id,
                              const bluetooth::Uuid& app_uuid) override;

  // Map of pending calls to register.
  std::mutex pending_calls_lock_;
  std::map<Uuid, RegisterCallback> pending_calls_;

  // Raw pointer to the Adapter that owns this factory.
  Adapter& adapter_;

  DISALLOW_COPY_AND_ASSIGN(LowEnergyClientFactory);
};

}  // namespace bluetooth
