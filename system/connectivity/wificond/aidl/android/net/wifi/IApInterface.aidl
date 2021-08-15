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

package android.net.wifi;

import android.net.wifi.IApInterfaceEventCallback;

// IApInterface represents a network interface configured to act as a
// WiFi access point.
interface IApInterface {

  const int ENCRYPTION_TYPE_NONE = 0;
  const int ENCRYPTION_TYPE_WPA = 1;
  const int ENCRYPTION_TYPE_WPA2 = 2;

  // Register a callback object for this interface.
  //
  // @param callback Object to add a set of event callbacks.
  // @return true on success.
  boolean registerCallback(IApInterfaceEventCallback callback);

  // Retrieve the name of the network interface corresponding to this
  // IApInterface instance (e.g. "wlan0")
  @utf8InCpp
  String getInterfaceName();

  // @return Returns the number of associated devices to this hotspot.
  // Returns -1 on failure.
  int getNumberOfAssociatedStations();
}
