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
import android.bluetooth.IBluetoothGattServerCallback;
import android.bluetooth.UUID;

interface IBluetoothGattServer {

  boolean RegisterServer(in IBluetoothGattServerCallback callback);
  void UnregisterServer(int server_id);
  void UnregisterAll();

  boolean AddService(int server_id, in BluetoothGattService service);

  boolean SendResponse(
      int server_id,
      String device_address,
      int request_id,
      int status, int offset,
      in byte[] value);

  boolean SendNotification(
      int server_id,
      String device_address,
      int handle,
      boolean confirm,
      in byte[] value);
}
