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

package com.android.server.wifi;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.wifi.WifiInfo;
import android.net.wifi.WifiManager;
import android.os.PowerManager;

import java.util.ArrayList;

/**
 * Utils for wifi tests.
 */
public class TestUtil {
    /**
     * Send {@link WifiManager#NETWORK_STATE_CHANGED_ACTION} broadcast.
     */
    public static void sendNetworkStateChanged(BroadcastReceiver broadcastReceiver,
            Context context, NetworkInfo.DetailedState detailedState) {
        Intent intent = new Intent(WifiManager.NETWORK_STATE_CHANGED_ACTION);
        NetworkInfo networkInfo = new NetworkInfo(0, 0, "", "");
        networkInfo.setDetailedState(detailedState, "", "");
        intent.putExtra(WifiManager.EXTRA_NETWORK_INFO, networkInfo);
        broadcastReceiver.onReceive(context, intent);
    }

    /**
     * Send {@link WifiManager#NETWORK_STATE_CHANGED_ACTION} broadcast.
     */
    public static void sendNetworkStateChanged(BroadcastReceiver broadcastReceiver,
            Context context, NetworkInfo nwInfo, WifiInfo wifiInfo) {
        Intent intent = new Intent(WifiManager.NETWORK_STATE_CHANGED_ACTION);
        intent.putExtra(WifiManager.EXTRA_NETWORK_INFO, nwInfo);
        intent.putExtra(WifiManager.EXTRA_WIFI_INFO, wifiInfo);
        broadcastReceiver.onReceive(context, intent);
    }

    /**
     * Send {@link WifiManager#SCAN_RESULTS_AVAILABLE_ACTION} broadcast.
     */
    public static void sendScanResultsAvailable(BroadcastReceiver broadcastReceiver,
            Context context) {
        Intent intent = new Intent(WifiManager.SCAN_RESULTS_AVAILABLE_ACTION);
        broadcastReceiver.onReceive(context, intent);
    }

    /**
     * Send {@link WifiManager#WIFI_SCAN_AVAILABLE} broadcast.
     */
    public static void sendWifiScanAvailable(BroadcastReceiver broadcastReceiver,
            Context context, int scanAvailable) {
        Intent intent = new Intent(WifiManager.WIFI_SCAN_AVAILABLE);
        intent.putExtra(WifiManager.EXTRA_SCAN_AVAILABLE, scanAvailable);
        broadcastReceiver.onReceive(context, intent);
    }

    /**
     * Send {@link WifiManager#WIFI_STATE_CHANGED} broadcast.
     */
    public static void sendWifiStateChanged(BroadcastReceiver broadcastReceiver,
            Context context, int wifiState) {
        Intent intent = new Intent(WifiManager.WIFI_STATE_CHANGED_ACTION);
        intent.putExtra(WifiManager.EXTRA_WIFI_STATE, wifiState);
        broadcastReceiver.onReceive(context, intent);
    }

    /**
     * Send {@link WifiManager#WIFI_AP_STATE_CHANGED} broadcast.
     */
    public static void sendWifiApStateChanged(BroadcastReceiver broadcastReceiver,
            Context context, int apState, int previousState, int error, String ifaceName,
            int mode) {
        Intent intent = new Intent(WifiManager.WIFI_AP_STATE_CHANGED_ACTION);
        intent.putExtra(WifiManager.EXTRA_WIFI_AP_STATE, apState);
        intent.putExtra(WifiManager.EXTRA_PREVIOUS_WIFI_AP_STATE, previousState);
        if (apState == WifiManager.WIFI_AP_STATE_FAILED) {
            // only set reason number when softAP start failed
            intent.putExtra(WifiManager.EXTRA_WIFI_AP_FAILURE_REASON, error);
        }
        intent.putExtra(WifiManager.EXTRA_WIFI_AP_INTERFACE_NAME, ifaceName);
        intent.putExtra(WifiManager.EXTRA_WIFI_AP_MODE, mode);
        broadcastReceiver.onReceive(context, intent);
    }

    /**
     * Send {@link ConnectivityManager#ACTION_TETHER_STATE_CHANGED} broadcast.
     */
    public static void sendTetherStateChanged(BroadcastReceiver broadcastReceiver,
            Context context, ArrayList<String> available, ArrayList<String> active) {
        Intent intent = new Intent(ConnectivityManager.ACTION_TETHER_STATE_CHANGED);
        intent.putExtra(ConnectivityManager.EXTRA_AVAILABLE_TETHER, available);
        intent.putExtra(ConnectivityManager.EXTRA_ACTIVE_TETHER, active);
        broadcastReceiver.onReceive(context, intent);
    }

    public static void sendIdleModeChanged(BroadcastReceiver broadcastReceiver, Context context) {
        Intent intent = new Intent(PowerManager.ACTION_DEVICE_IDLE_MODE_CHANGED);
        broadcastReceiver.onReceive(context, intent);
    }
}
