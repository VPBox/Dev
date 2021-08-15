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

package com.android.server.wifi.hotspot2;

import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;
import static org.mockito.ArgumentMatchers.any;
import static org.mockito.Matchers.eq;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;
import static org.mockito.MockitoAnnotations.initMocks;

import androidx.test.filters.SmallTest;

import com.android.server.wifi.WifiNative;
import com.android.server.wifi.hotspot2.anqp.Constants;

import org.junit.Before;
import org.junit.Test;
import org.mockito.Mock;

import java.util.Arrays;
import java.util.HashSet;
import java.util.List;

/**
 * Unit tests for {@link com.android.server.wifi.hotspot2.PasspointEventHandler}.
 * TODO(zqiu): add more test when switch over to use wificond.
 */
@SmallTest
public class PasspointEventHandlerTest {

    private static final String TAG = "PasspointEventHandlerTest";

    private static final long BSSID = 0x112233445566L;
    private static final String BSSID_STR = "11:22:33:44:55:66";
    private static final String ICON_FILENAME = "icon.test";

    @Mock WifiNative mWifiNative;
    @Mock PasspointEventHandler.Callbacks mCallbacks;
    PasspointEventHandler mHandler;

    /** Sets up test. */
    @Before
    public void setUp() throws Exception {
        initMocks(this);
        mHandler = new PasspointEventHandler(mWifiNative, mCallbacks);
    }

    /**
     * Test for requesting Hotspot 2.0 R1 ANQP element.
     */
    @Test
    public void requestR1AnqpElement() {
        List<Constants.ANQPElementType> elementToRequest =
                Arrays.asList(Constants.ANQPElementType.ANQPRoamingConsortium);
        HashSet<Integer> expAnqpIds =
                new HashSet<>(Arrays.asList(Constants.getANQPElementID(
                        Constants.ANQPElementType.ANQPRoamingConsortium)));
        HashSet<Integer> expHs20Subtypes = new HashSet<>();

        // wpa_supplicant succeeded the request.
        when(mWifiNative.requestAnqp(any(), eq(BSSID_STR), eq(expAnqpIds), eq(expHs20Subtypes)))
                .thenReturn(true);
        assertTrue(mHandler.requestANQP(BSSID, elementToRequest));

        // wpa_supplicant failed the request.
        when(mWifiNative.requestAnqp(any(), eq(BSSID_STR), eq(expAnqpIds), eq(expHs20Subtypes)))
                .thenReturn(false);
        assertFalse(mHandler.requestANQP(BSSID, elementToRequest));
    }

    /**
     * Test for requesting Hotspot 2.0 R2 ANQP element.
     */
    @Test
    public void requestR2AnqpElement() {
        List<Constants.ANQPElementType> elementToRequest =
                Arrays.asList(Constants.ANQPElementType.HSFriendlyName);
        HashSet<Integer> expAnqpIds = new HashSet<>();
        HashSet<Integer> expHs20Subtypes =
                new HashSet<>(Arrays.asList(Constants.getHS20ElementID(
                        Constants.ANQPElementType.HSFriendlyName)));

        // wpa_supplicant succeeded the request.
        when(mWifiNative.requestAnqp(any(), eq(BSSID_STR), eq(expAnqpIds), eq(expHs20Subtypes)))
                .thenReturn(true);
        assertTrue(mHandler.requestANQP(BSSID, elementToRequest));

        // wpa_supplicant failed the request.
        when(mWifiNative.requestAnqp(any(), eq(BSSID_STR), eq(expAnqpIds), eq(expHs20Subtypes)))
                .thenReturn(false);
        assertFalse(mHandler.requestANQP(BSSID, elementToRequest));
    }

    /**
     * Test for requesting both Hotspot 2.0 R1 and R2 ANQP elements.
     */
    @Test
    public void requestMixAnqpElements() {
        List<Constants.ANQPElementType> elementToRequest =
                Arrays.asList(Constants.ANQPElementType.ANQPRoamingConsortium,
                              Constants.ANQPElementType.HSFriendlyName);
        HashSet<Integer> expAnqpIds =
                new HashSet<>(Arrays.asList(Constants.getANQPElementID(
                        Constants.ANQPElementType.ANQPRoamingConsortium)));
        HashSet<Integer> expHs20Subtypes =
                new HashSet<>(Arrays.asList(Constants.getHS20ElementID(
                        Constants.ANQPElementType.HSFriendlyName)));

        // wpa_supplicant succeeded the request.
        when(mWifiNative.requestAnqp(any(), eq(BSSID_STR), eq(expAnqpIds), eq(expHs20Subtypes)))
                .thenReturn(true);
        assertTrue(mHandler.requestANQP(BSSID, elementToRequest));

        // wpa_supplicant failed the request.
        when(mWifiNative.requestAnqp(any(), eq(BSSID_STR), eq(expAnqpIds), eq(expHs20Subtypes)))
                .thenReturn(false);
        assertFalse(mHandler.requestANQP(BSSID, elementToRequest));
    }

    /**
     * Test for requesting both Hotspot 2.0 R2 icon file.
     */
    @Test
    public void requestIconFile() {
        // wpa_supplicant succeeded the request.
        when(mWifiNative.requestIcon(any(), eq(BSSID_STR), eq(ICON_FILENAME))).thenReturn(true);
        assertTrue(mHandler.requestIcon(BSSID, ICON_FILENAME));

        // wpa_supplicant failed the request.
        when(mWifiNative.requestIcon(any(), eq(BSSID_STR), eq(ICON_FILENAME))).thenReturn(false);
        assertFalse(mHandler.requestIcon(BSSID, ICON_FILENAME));
    }

    /**
     * Test for ANQP request completed with error.
     */
    @Test
    public void anqpRequestCompletedWithError() {
        mHandler.notifyANQPDone(new AnqpEvent(BSSID, null));
        verify(mCallbacks).onANQPResponse(BSSID, null);
    }
}
