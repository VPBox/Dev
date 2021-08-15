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

import android.net.wifi.ISendMgmtFrameEvent;
import android.net.wifi.IWifiScannerImpl;

// IClientInterface represents a network interface that can be used to connect
// to access points and obtain internet connectivity.
interface IClientInterface {
  // Get packet counters for this interface.
  // First element in array is the number of successfully transmitted packets.
  // Second element in array is the number of tramsmission failure.
  // This call is valid only when interface is associated with an AP, otherwise
  // it returns an empty array.
  int[] getPacketCounters();

  // Do signal poll for this interface.
  // First element in array is the RSSI value in dBM.
  // Second element in array is the transmission bit rate in Mbps.
  // Third element in array is the association frequency in MHz.
  // Fourth element in array is the last received packet bit rate in Mbps.
  // This call is valid only when interface is associated with an AP, otherwise
  // it returns an empty array.
  int[] signalPoll();

  // Get the MAC address of this interface.
  byte[] getMacAddress();

  // Retrieve the name of the network interface corresponding to this
  // IClientInterface instance (e.g. "wlan0")
  @utf8InCpp
  String getInterfaceName();

  // Get a WifiScanner interface associated with this interface.
  // Returns null when the underlying interface object is destroyed.
  @nullable IWifiScannerImpl getWifiScannerImpl();

  // Set the MAC address of this interface
  // Returns true if the set was successful
  boolean setMacAddress(in byte[] mac);

  // Sends an arbitrary 802.11 management frame on the current channel.
  // @param frame Bytes of the 802.11 management frame to be sent, including the
  //     header, but not including the frame check sequence (FCS).
  // @param Callback triggered when the transmitted frame is ACKed or the
  //     transmission fails.
  // @param mcs MCS rate which the management frame will be sent at. If mcs < 0,
  //     the driver will select the rate automatically. If the device does not
  //     support sending the frame at a specified MCS rate, the transmission
  //     will be aborted and ISendMgmtFrameEvent.OnFailure() will be called with
  //     reason ISendMgmtFrameEvent.SEND_MGMT_FRAME_ERROR_MCS_UNSUPPORTED.
  oneway void SendMgmtFrame(
      in byte[] frame, in ISendMgmtFrameEvent callback, int mcs);
}
