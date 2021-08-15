/*
 * Copyright (C) 2017 The Android Open Source Project
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

import android.bluetooth.IBluetoothAvrcpControlCallback;

interface IBluetoothAvrcpControl {
  boolean Register(in IBluetoothAvrcpControlCallback callback);
  void Unregister(int id);
  void UnregisterAll();

  boolean Enable(int id);
  boolean Disable(int id);

  boolean SendPassThroughCommand(int id, String device_address,
      int key_code, boolean key_pressed);

  boolean SetAbsVolumeResponse(int id, String device_address, int abs_vol,
      int label);

  boolean RegisterForAbsVolumeCallbackResponse(int id, String device_address,
      int response_type, int abs_vol, int label);
}
