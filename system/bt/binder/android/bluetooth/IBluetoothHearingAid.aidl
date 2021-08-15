/*
 * Copyright 2018 The Android Open Source Project
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

import android.bluetooth.BluetoothDevice;

/**
 * APIs for Bluetooth Hearing Aid service
 *
 * @hide
 */
interface IBluetoothHearingAid {
    // Public API
    boolean connect(in BluetoothDevice device);
    boolean disconnect(in BluetoothDevice device);
    List<BluetoothDevice> getConnectedDevices();
    List<BluetoothDevice> getDevicesMatchingConnectionStates(in int[] states);
    int getConnectionState(in BluetoothDevice device);
    boolean setActiveDevice(in BluetoothDevice device);
    List<BluetoothDevice> getActiveDevices();
    boolean setPriority(in BluetoothDevice device, int priority);
    int getPriority(in BluetoothDevice device);
    void adjustVolume(int direction);
    void setVolume(int volume);
    int getVolume();

    const int HI_SYNC_ID_INVALID = 0;
    long getHiSyncId(in BluetoothDevice device);

    const int SIDE_LEFT = 0;
    const int SIDE_RIGHT = 1;
    int getDeviceSide(in BluetoothDevice device);

    const int MODE_MONAURAL = 0;
    const int MODE_BINAURAL = 1;
    int getDeviceMode(in BluetoothDevice device);
}
