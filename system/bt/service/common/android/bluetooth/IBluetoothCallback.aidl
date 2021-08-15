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

import android.bluetooth.BluetoothRemoteDeviceProps;

oneway interface IBluetoothCallback {
    void OnBluetoothStateChange(int prev_state, int new_state);
    void OnScanEnableChanged(boolean scan_enabled);
    void OnDeviceConnectionStateChanged(
        String device_address,
        boolean connected);
    void OnSspRequest(
        String device_address,
        String device_name,
        int cod,
        int pairing_variant,
        int pass_key);
    void OnGetBondedDevices(int status, in String[] device_addresses);
    void OnBondStateChanged(int status, String device_address, int state);
    void OnGetRemoteDeviceProperties(
        int status,
        String device_address,
        in BluetoothRemoteDeviceProps props);
    void OnDeviceFound(
        in BluetoothRemoteDeviceProps props);
}
