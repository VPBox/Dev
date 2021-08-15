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

// A callback for receiving pno scanning events.
interface IPnoScanEvent {
  const int PNO_SCAN_OVER_OFFLOAD_BINDER_FAILURE = 0;
  const int PNO_SCAN_OVER_OFFLOAD_REMOTE_FAILURE = 1;

  oneway void OnPnoNetworkFound();
  oneway void OnPnoScanFailed();
  // Callback notifying the framework that PNO scan is started over Offload HAL
  // interface, this is meant for metrics collection only
  oneway void OnPnoScanOverOffloadStarted();
  // Callback notifying the framework that a PNO scan running over Offload HAL
  // has failed, this is meant for metrics collection only
  oneway void OnPnoScanOverOffloadFailed(int reason);
}
