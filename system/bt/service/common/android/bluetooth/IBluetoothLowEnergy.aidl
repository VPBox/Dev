/*
 * Copyright 2016 The Android Open Source Project
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

import android.bluetooth.IBluetoothLowEnergyCallback;

import android.bluetooth.AdvertiseData;
import android.bluetooth.AdvertiseSettings;

interface IBluetoothLowEnergy {
  boolean RegisterClient(in IBluetoothLowEnergyCallback callback);
  void UnregisterClient(int client_if);
  void UnregisterAll();

  boolean Connect(int client_id, String address, boolean is_direct);
  boolean Disconnect(int client_id, String address);

  boolean SetMtu(int client_id, String address, int mtu);
}