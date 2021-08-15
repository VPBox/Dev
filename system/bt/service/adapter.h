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

#include <memory>
#include <string>
#include <vector>

#include <base/macros.h>

#include "service/common/bluetooth/adapter_state.h"
#include "service/common/bluetooth/remote_device_props.h"

namespace bluetooth {

class A2dpSinkFactory;
class A2dpSourceFactory;
class AvrcpControlFactory;
class AvrcpTargetFactory;
class GattClientFactory;
class GattServerFactory;
class LowEnergyAdvertiserFactory;
class LowEnergyScannerFactory;
class LowEnergyClientFactory;

// Represents the local Bluetooth adapter.
class Adapter {
 public:
  // The default values returned before the Adapter is fully initialized and
  // powered. The complete values for these fields are obtained following a
  // successful call to "Enable".
  static const char kDefaultAddress[];
  static const char kDefaultName[];

  // Observer interface allows other classes to receive notifications from us.
  // All of the methods in this interface are declared as optional to allow
  // different layers to process only those events that they are interested in.
  //
  // All methods take in an |adapter| argument which points to the Adapter
  // object that the Observer instance was added to.
  class Observer {
   public:
    virtual ~Observer() = default;

    // Called when there is a change in the state of the local Bluetooth
    // |adapter| from |prev_state| to |new_state|.
    virtual void OnAdapterStateChanged(Adapter* adapter,
                                       AdapterState prev_state,
                                       AdapterState new_state);

    // Called when there is a change in the connection state between the local
    // |adapter| and a remote device with address |device_address|. If the ACL
    // state changes from disconnected to connected, then |connected| will be
    // true and vice versa.
    virtual void OnDeviceConnectionStateChanged(
        Adapter* adapter, const std::string& device_address, bool connected);

    // Called when scanning is enabled or disabled.
    virtual void OnScanEnableChanged(Adapter* adapter, bool scan_enabled);

    // Called when a SSP pairing request comes from a remote device.
    virtual void OnSspRequest(Adapter* adapter,
                              const std::string& device_address,
                              const std::string& device_name, int cod,
                              int pairing_variant, int pass_key);

    // Called when a remote device bond state changes.
    virtual void OnBondStateChanged(Adapter* adapter, int status,
                                    const std::string& device_address,
                                    int state);

    // Called in response to |GetBondedDevices|.
    virtual void OnGetBondedDevices(
        Adapter* adapter, int status,
        const std::vector<std::string>& bonded_devices);

    // Called in response to |GetRemoteDeviceProperties|.
    virtual void OnGetRemoteDeviceProperties(Adapter* adapter, int status,
                                             const std::string& device_address,
                                             const RemoteDeviceProps& props);

    // Called when a device is found through scanning.
    virtual void OnDeviceFound(Adapter* adapter,
                               const RemoteDeviceProps& props);
  };

  // Returns an Adapter implementation to be used in production. Don't use these
  // in tests; use MockAdapter instead.
  static std::unique_ptr<Adapter> Create();

  virtual ~Adapter() = default;

  // Add or remove an observer.
  virtual void AddObserver(Observer* observer) = 0;
  virtual void RemoveObserver(Observer* observer) = 0;

  // Returns the current Adapter state.
  virtual AdapterState GetState() const = 0;

  // Returns true, if the adapter radio is current powered.
  virtual bool IsEnabled() const = 0;

  // Enables Bluetooth. This method will send a request to the Bluetooth adapter
  // to power up its radio. Returns true, if the request was successfully sent
  // to the controller, otherwise returns false. A successful call to this
  // method only means that the enable request has been sent to the Bluetooth
  // controller and does not imply that the operation itself succeeded.
  virtual bool Enable() = 0;

  // Powers off the Bluetooth radio. Returns true, if the disable request was
  // successfully sent to the Bluetooth controller.
  virtual bool Disable() = 0;

  // Returns the name currently assigned to the local adapter.
  virtual std::string GetName() const = 0;

  // Sets the name assigned to the local Bluetooth adapter. This is the name
  // that the local controller will present to remote devices.
  virtual bool SetName(const std::string& name) = 0;

  // Returns the local adapter addess in string form (XX:XX:XX:XX:XX:XX).
  virtual std::string GetAddress() const = 0;

  // Set discoverability mode.
  virtual bool SetScanMode(int scan_mode) = 0;

  // Enable or disable discoverability.
  virtual bool SetScanEnable(bool scan_enable) = 0;

  // Reply to an SSP request received in |OnSspRequest|.
  virtual bool SspReply(const std::string& device_address, int variant,
                        bool accept, int32_t pass_key) = 0;

  // Create a bond with device specified by |device_address|.
  virtual bool CreateBond(const std::string& device_address, int transport) = 0;

  // Returns true if the local adapter supports the Low-Energy
  // multi-advertisement feature.
  virtual bool IsMultiAdvertisementSupported() = 0;

  // Returns true if the remote device with address |device_address| is
  // currently connected. This is not a const method as it modifies the state of
  // the associated internal mutex.
  virtual bool IsDeviceConnected(const std::string& device_address) = 0;

  // Returns the total number of trackable advertisements as supported by the
  // underlying hardware.
  virtual int GetTotalNumberOfTrackableAdvertisements() = 0;

  // Returns true if hardware-backed scan filtering is supported.
  virtual bool IsOffloadedFilteringSupported() = 0;

  // Returns true if hardware-backed batch scanning is supported.
  virtual bool IsOffloadedScanBatchingSupported() = 0;

  // When the stack call completes, |OnGetBondedDevices| will be called.
  virtual bool GetBondedDevices() = 0;

  // When the stack call completets, |OnBondStateChanged| will be called.
  virtual bool RemoveBond(const std::string& device_address) = 0;

  // When the stack call completets, |OnGetRemoteDeviceProperties| will be
  // called.
  virtual bool GetRemoteDeviceProperties(const std::string& device_address) = 0;

  // Returns a pointer to the A2dpSinkFactory. This can be used to
  // register per-application A2dpSinkClient instances to perform A2DP sink
  // operations.
  virtual A2dpSinkFactory* GetA2dpSinkFactory() const = 0;

  // Returns a pointer to the A2dpSourceFactory. This can be used to
  // register per-application A2dpSourceClient instances to perform A2DP source
  // operations.
  virtual A2dpSourceFactory* GetA2dpSourceFactory() const = 0;

  // Returns a pointer to the AvrcpControlFactory. This can be used to register
  // per-application AvrcpControlClient instances to perform AVRCP control
  // operations.
  virtual AvrcpControlFactory* GetAvrcpControlFactory() const = 0;

  // Returns a pointer to the AvrcpTargetFactory. This can be used to register
  // per-application AvrcpTargetClient instances to perform AVRCP target
  // operations.
  virtual AvrcpTargetFactory* GetAvrcpTargetFactory() const = 0;

  // Returns a pointer to the LowEnergyClientFactory. This can be used to
  // register per-application LowEnergyClient instances to perform BLE GAP
  // operations.
  virtual LowEnergyClientFactory* GetLowEnergyClientFactory() const = 0;

  // Returns a pointer to the LowEnergyScannerFactory. This can be used to
  // register per-application LowEnergyScanner instances to perform scanning.
  virtual LowEnergyScannerFactory* GetLeScannerFactory() const = 0;

  // Returns a pointer to the LowEnergyAdvertiserFactory. This can be used to
  // register per-application LowEnergyAdvertiser instances to perform
  // advertising.
  virtual LowEnergyAdvertiserFactory* GetLeAdvertiserFactory() const = 0;

  // Returns a pointer to the GattClientFactory. This can be used to register
  // per-application GATT server instances.
  virtual GattClientFactory* GetGattClientFactory() const = 0;

  // Returns a pointer to the GattServerFactory. This can be used to register
  // per-application GATT server instances.
  virtual GattServerFactory* GetGattServerFactory() const = 0;

 protected:
  Adapter() = default;

 private:
  DISALLOW_COPY_AND_ASSIGN(Adapter);
};

}  // namespace bluetooth
