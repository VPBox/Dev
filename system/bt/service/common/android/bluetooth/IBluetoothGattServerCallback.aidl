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

import android.bluetooth.BluetoothGattService;

oneway interface IBluetoothGattServerCallback {
  void OnServerRegistered(int status, int server_id);

  void OnServiceAdded(int status, in BluetoothGattService service);

  void OnCharacteristicReadRequest(String device_address,
    int request_id, int offset, boolean is_long, int handle);

  void OnDescriptorReadRequest(String device_address,
    int request_id, int offset, boolean is_long, int handle);

  void OnCharacteristicWriteRequest(String device_address,
    int request_id, int offset, boolean is_prepare_write, boolean need_response,
    in byte[] value, int handle);

  void OnDescriptorWriteRequest(String device_address,
    int request_id, int offset, boolean is_prepare_write, boolean need_response,
    in byte[] value, int handle);

  void OnExecuteWriteRequest(String device_address,
    int request_id, boolean is_execute);

  void OnNotificationSent(String device_address,
    int status);

  void OnConnectionStateChanged(String device_address, boolean connected);
  }
