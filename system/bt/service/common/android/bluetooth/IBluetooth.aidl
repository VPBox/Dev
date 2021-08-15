/*
 * Copyright (C) 2016 The Android Open Source Project
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

package android.bluetooth;

import android.bluetooth.IBluetoothCallback;
import android.bluetooth.IBluetoothLowEnergy;
import android.bluetooth.IBluetoothLeAdvertiser;
import android.bluetooth.IBluetoothLeScanner;
import android.bluetooth.IBluetoothGattClient;
import android.bluetooth.IBluetoothGattServer;
import android.bluetooth.IBluetoothA2dpSink;
import android.bluetooth.IBluetoothA2dpSource;
import android.bluetooth.IBluetoothAvrcpControl;
import android.bluetooth.IBluetoothAvrcpTarget;

import android.bluetooth.UUID;

interface IBluetooth {
  boolean IsEnabled();
  int GetState();
  boolean Enable();
  boolean EnableNoAutoConnect();
  boolean Disable();

  String GetAddress();
  UUID[] GetUUIDs();
  boolean SetName(String name);
  String GetName();
  boolean SetScanMode(int scan_mode);
  boolean SetScanEnable(boolean scan_enable);
  boolean SspReply(
      String device_address,
      int variant,
      boolean accept,
      int passkey);
  boolean CreateBond(String device_address, int transport);
  boolean GetBondedDevices();
  boolean RemoveBond(String device_address);
  boolean GetRemoteDeviceProperties(String device_address);

  void RegisterCallback(IBluetoothCallback callback);
  void UnregisterCallback(IBluetoothCallback callback);

  boolean IsMultiAdvertisementSupported();

  IBluetoothLowEnergy GetLowEnergyInterface();
  IBluetoothLeAdvertiser GetLeAdvertiserInterface();
  IBluetoothLeScanner GetLeScannerInterface();
  IBluetoothGattClient GetGattClientInterface();
  IBluetoothGattServer GetGattServerInterface();
  IBluetoothA2dpSink GetA2dpSinkInterface();
  IBluetoothA2dpSource GetA2dpSourceInterface();
  IBluetoothAvrcpControl GetAvrcpControlInterface();
  IBluetoothAvrcpTarget GetAvrcpTargetInterface();
}
