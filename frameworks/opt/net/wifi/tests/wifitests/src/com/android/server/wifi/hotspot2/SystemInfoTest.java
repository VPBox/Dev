/*
 * Copyright (C) 2018 The Android Open Source Project
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

package com.android.server.wifi.hotspot2;

import static org.junit.Assert.assertEquals;
import static org.mockito.ArgumentMatchers.any;
import static org.mockito.ArgumentMatchers.anyInt;
import static org.mockito.Mockito.when;
import static org.mockito.MockitoAnnotations.initMocks;

import android.content.Context;
import android.telephony.TelephonyManager;

import androidx.test.filters.SmallTest;

import com.android.server.wifi.WifiNative;

import org.junit.Before;
import org.junit.Test;
import org.mockito.Mock;

/**
 * Unit tests for {@link SystemInfo}.
 */
@SmallTest
public class SystemInfoTest {
    @Mock Context mContext;
    @Mock TelephonyManager mTelephonyManager;
    @Mock TelephonyManager mDataTelephonyManager;
    @Mock WifiNative mWifiNative;

    SystemInfo mSystemInfo;
    private static final String TEST_MAC = "11:22:33:44:55:66";
    private static final String TEST_IFACE = "wlan0";

    @Before
    public void setUp() throws Exception {
        initMocks(this);
        when(mContext.getSystemService(Context.TELEPHONY_SERVICE)).thenReturn(mTelephonyManager);
        when(mTelephonyManager.createForSubscriptionId(anyInt())).thenReturn(mDataTelephonyManager);
        mSystemInfo = new SystemInfo(mContext, mWifiNative);
    }

    /**
     * Verify that IMEI is returned as device ID when it is provided by {@link TelephonyManager}.
     */
    @Test
    public void getDeviceIdWithImei() {
        String imei = "123456";
        when(mDataTelephonyManager.getImei()).thenReturn(imei);
        assertEquals(imei, mSystemInfo.getDeviceId());
    }

    /**
     * Verify that MEID is returned as device ID when it is provided by {@link TelephonyManager}.
     */
    @Test
    public void getDeviceIdWithMeid() {
        String meid = "098763";
        when(mDataTelephonyManager.getImei()).thenReturn(null);
        when(mDataTelephonyManager.getMeid()).thenReturn(meid);
        assertEquals(meid, mSystemInfo.getDeviceId());
    }

    /**
     * Verify that {@link SystemInfo#UNKNOWN_INFO} is returned as device ID when both IMEI and
     * MEID are not provided by {@link TelephonyManager}.
     */
    @Test
    public void getDeviceIdWithoutSim() {
        when(mDataTelephonyManager.getImei()).thenReturn(null);
        when(mDataTelephonyManager.getMeid()).thenReturn(null);
        assertEquals(SystemInfo.UNKNOWN_INFO, mSystemInfo.getDeviceId());
    }

    /**
     * Verify that mac address is returned successfully.
     */
    @Test
    public void getWifiMacAddress() {
        when(mWifiNative.getMacAddress(any(String.class))).thenReturn(TEST_MAC);
        assertEquals(TEST_MAC, mSystemInfo.getMacAddress(TEST_IFACE));
    }
}

