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

package android.net.wifi;

// A callback for receiving events related to soft AP.
interface IApInterfaceEventCallback {

  // Channel bandwidth type. Used in |onSoftApChannelSwitched|
  const int BANDWIDTH_INVALID = 0;
  const int BANDWIDTH_20_NOHT = 1;
  const int BANDWIDTH_20 = 2;
  const int BANDWIDTH_40 = 3;
  const int BANDWIDTH_80 = 4;
  const int BANDWIDTH_80P80 = 5;
  const int BANDWIDTH_160 = 6;

  // Signals that number of stations associated to this soft Ap has changed.
  //
  // @param numStations Number of associated stations after change
  oneway void onNumAssociatedStationsChanged(int numStations);

  // Signals a channel switch event for this soft Ap.
  //
  // @param frequency Represents the frequency of the channel in MHz
  // @param bandwidth Bandwidth of the channel, one of the values from |BANDWIDTH_*|
  oneway void onSoftApChannelSwitched(int frequency, int bandwidth);
}
