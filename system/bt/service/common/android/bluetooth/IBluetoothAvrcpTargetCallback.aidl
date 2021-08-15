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

import android.bluetooth.BluetoothAvrcpIntValue;

oneway interface IBluetoothAvrcpTargetCallback {
  void OnRegistered(int status);
  void OnGetRemoteFeatures(String addr, int features);
  void OnGetPlayStatus(String addr);
  void OnListPlayerAppAttr(String addr);
  void OnListPlayerAppValues(String addr, int attr_id);
  void OnGetPlayerAppValue(String addr, in int[] attrs);
  void OnGetPlayerAppAttrsText(String addr, in int[] attrs);
  void OnGetPlayerAppValuesText(String addr, int attr_id, in int[] values);
  void OnSetPlayerAppValue(String addr, in BluetoothAvrcpIntValue[] values);
  void OnGetElementAttrs(String addr, in int[] attrs);
  void OnRegisterNotification(String addr, int event_id, int param);
  void OnVolumeChange(String addr, int volume, int ctype);
  void OnPassThroughCommand(String addr, int id, int key_state);
}
