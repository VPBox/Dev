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

import android.net.wifi.IPnoScanEvent;
import android.net.wifi.IScanEvent;
import com.android.server.wifi.wificond.NativeScanResult;
import com.android.server.wifi.wificond.PnoSettings;
import com.android.server.wifi.wificond.SingleScanSettings;

interface IWifiScannerImpl {
  // Type of scan request. This is used in |SingleScanSettings.scan_type|.
  const int SCAN_TYPE_LOW_SPAN = 0;
  const int SCAN_TYPE_LOW_POWER = 1;
  const int SCAN_TYPE_HIGH_ACCURACY = 2;
  // Scan type used internally if the device does not support
  // the type specified in |SingleScanSettings.scan_type|.
  // Scan requests from framework with this type will be rejected.
  const int SCAN_TYPE_DEFAULT = -1;

  // Get the latest single scan results from kernel.
  NativeScanResult[] getScanResults();

  // Get the latest pno scan results from the interface which has most recently
  // completed disconnected mode PNO scans
  NativeScanResult[] getPnoScanResults();

  // Request a single scan using a SingleScanSettings parcelable object.
  boolean scan(in SingleScanSettings scanSettings);

  // Subscribe single scanning events.
  // Scanner assumes there is only one subscriber.
  // This call will replace any existing |handler|.
  oneway void subscribeScanEvents(IScanEvent handler);

  // Unsubscribe single scanning events .
  oneway void unsubscribeScanEvents();

  // Subscribe Pno scanning events.
  // Scanner assumes there is only one subscriber.
  // This call will replace any existing |handler|.
  oneway void subscribePnoScanEvents(IPnoScanEvent handler);

  // Unsubscribe Pno scanning events .
  oneway void unsubscribePnoScanEvents();

  // Request a scheduled scan.
  boolean startPnoScan(in PnoSettings pnoSettings);

  // Stop any existing scheduled scan.
  // Returns true on success.
  // Returns false on failure or there is no existing scheduled scan.
  boolean stopPnoScan();

  // Abort ongoing scan.
  void abortScan();

  // TODO(nywang) add more interfaces.
}
