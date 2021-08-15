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

import static org.junit.Assert.*;
import static org.mockito.ArgumentMatchers.eq;
import static org.mockito.Mockito.any;
import static org.mockito.Mockito.anyString;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;

import androidx.test.filters.SmallTest;

import org.junit.Before;
import org.junit.Test;
import org.mockito.Mock;
import org.mockito.MockitoAnnotations;

import java.io.PrintWriter;
import java.io.StringWriter;
import java.util.Locale;

/**
 * Unit tests for {@link com.android.server.wifi.WifiCountryCode}.
 */
@SmallTest
public class WifiCountryCodeTest {

    private static final String TAG = "WifiCountryCodeTest";
    private String mDefaultCountryCode = "US";
    private String mTelephonyCountryCode = "JP";
    private boolean mRevertCountryCodeOnCellularLoss = true;
    @Mock WifiNative mWifiNative;
    private WifiCountryCode mWifiCountryCode;

    /**
     * Setup test.
     */
    @Before
    public void setUp() throws Exception {
        MockitoAnnotations.initMocks(this);

        when(mWifiNative.setCountryCode(any(), anyString())).thenReturn(true);

        mWifiCountryCode = new WifiCountryCode(
                mWifiNative,
                mDefaultCountryCode,
                mRevertCountryCodeOnCellularLoss);
    }

    /**
     * Test if we do not receive country code from Telephony.
     * @throws Exception
     */
    @Test
    public void useDefaultCountryCode() throws Exception {
        // Supplicant started.
        mWifiCountryCode.setReadyForChange(true);
        // Wifi get L2 connected.
        mWifiCountryCode.setReadyForChange(false);
        verify(mWifiNative).setCountryCode(any(), anyString());
        assertEquals(mDefaultCountryCode, mWifiCountryCode.getCountryCodeSentToDriver());
    }

    /**
     * Test if we receive country code from Telephony before supplicant starts.
     * @throws Exception
     */
    @Test
    public void useTelephonyCountryCode() throws Exception {
        mWifiCountryCode.setCountryCode(mTelephonyCountryCode);
        assertEquals(null, mWifiCountryCode.getCountryCodeSentToDriver());
        // Supplicant started.
        mWifiCountryCode.setReadyForChange(true);
        // Wifi get L2 connected.
        mWifiCountryCode.setReadyForChange(false);
        verify(mWifiNative).setCountryCode(any(), anyString());
        assertEquals(mTelephonyCountryCode, mWifiCountryCode.getCountryCodeSentToDriver());
    }

    /**
     * Test if we receive country code from Telephony after supplicant starts.
     * @throws Exception
     */
    @Test
    public void setTelephonyCountryCodeAfterSupplicantStarts() throws Exception {
        // Supplicant starts.
        mWifiCountryCode.setReadyForChange(true);
        assertEquals(mDefaultCountryCode, mWifiCountryCode.getCountryCodeSentToDriver());
        // Telephony country code arrives.
        mWifiCountryCode.setCountryCode(mTelephonyCountryCode);
        // Wifi get L2 connected.
        mWifiCountryCode.setReadyForChange(false);
        verify(mWifiNative, times(2)).setCountryCode(any(), anyString());
        assertEquals(mTelephonyCountryCode, mWifiCountryCode.getCountryCodeSentToDriver());
    }

    /**
     * Test if we receive country code from Telephony after we get L2 connected.
     * @throws Exception
     */
    @Test
    public void setTelephonyCountryCodeAfterL2Connected() throws Exception {
        // Supplicant starts.
        mWifiCountryCode.setReadyForChange(true);
        // Wifi get L2 connected.
        mWifiCountryCode.setReadyForChange(false);
        // Telephony country code arrives.
        mWifiCountryCode.setCountryCode(mTelephonyCountryCode);
        // Telephony coutry code won't be applied at this time.
        assertEquals(mDefaultCountryCode, mWifiCountryCode.getCountryCodeSentToDriver());
        mWifiCountryCode.setReadyForChange(true);
        // Telephony coutry is applied after supplicant is ready.
        verify(mWifiNative, times(2)).setCountryCode(any(), anyString());
        assertEquals(mTelephonyCountryCode, mWifiCountryCode.getCountryCodeSentToDriver());
    }

    /**
     * Test if we can reset to the default country code when phone is out of service, when
     * |config_wifi_revert_country_code_on_cellular_loss| is set to true;
     * Telephony service calls |setCountryCode| with an empty string when phone is out of service.
     * In this case we should fall back to the default country code.
     * @throws Exception
     */
    @Test
    public void resetCountryCodeWhenOutOfService() throws Exception {
        assertEquals(mDefaultCountryCode, mWifiCountryCode.getCountryCode());
        mWifiCountryCode.setCountryCode(mTelephonyCountryCode);
        assertEquals(mTelephonyCountryCode, mWifiCountryCode.getCountryCode());
        // Out of service.
        mWifiCountryCode.setCountryCode("");
        assertEquals(mDefaultCountryCode, mWifiCountryCode.getCountryCode());
    }

    /**
     * Test if we can keep using the last known country code when phone is out of service, when
     * |config_wifi_revert_country_code_on_cellular_loss| is set to false;
     * Telephony service calls |setCountryCode| with an empty string when phone is out of service.
     * In this case we should keep using the last known country code.
     * @throws Exception
     */
    @Test
    public void doNotResetCountryCodeWhenOutOfService() throws Exception {
        // Refresh mWifiCountryCode with |config_wifi_revert_country_code_on_cellular_loss|
        // setting to false.
        mWifiCountryCode = new WifiCountryCode(
                mWifiNative,
                mDefaultCountryCode,
                false /* config_wifi_revert_country_code_on_cellular_loss */);

        assertEquals(mDefaultCountryCode, mWifiCountryCode.getCountryCode());
        mWifiCountryCode.setCountryCode(mTelephonyCountryCode);
        assertEquals(mTelephonyCountryCode, mWifiCountryCode.getCountryCode());
        // Out of service.
        mWifiCountryCode.setCountryCode("");
        assertEquals(mTelephonyCountryCode, mWifiCountryCode.getCountryCode());
    }

    /**
     * Tests that we always use the US locale for converting the provided country code regardless
     * of the system locale set.
     */
    @Test
    public void useUSLocaleForConversionToUpperCase() {
        String oemCountryCodeLower = "us";
        String oemCountryCodeUpper = "US";
        String telephonyCountryCodeLower = "il";
        String telephonyCountryCodeUpper = "IL";

        mWifiCountryCode = new WifiCountryCode(
                mWifiNative,
                oemCountryCodeLower,
                mRevertCountryCodeOnCellularLoss);

        // Set the default locale to "tr" (Non US).
        Locale.setDefault(new Locale("tr"));

        // Trigger a country code change using the OEM country code.
        mWifiCountryCode.setReadyForChange(true);
        verify(mWifiNative).setCountryCode(any(), eq(oemCountryCodeUpper));

        // Now trigger a country code change using the telephony country code.
        mWifiCountryCode.setCountryCode(telephonyCountryCodeLower);
        verify(mWifiNative).setCountryCode(any(), eq(telephonyCountryCodeUpper));
    }
    /**
     * Verifies that dump() does not fail
     */
    @Test
    public void dumpDoesNotFail() {
        StringWriter sw = new StringWriter();
        PrintWriter pw = new PrintWriter(sw);

        mWifiCountryCode = new WifiCountryCode(
                null,
                null,
                false /* config_wifi_revert_country_code_on_cellular_loss */);

        mWifiCountryCode.dump(null, pw, null);
        String dumpCountryCodeStr = sw.toString();

        assertTrue(dumpCountryCodeStr.contains("mDriverCountryCode"));
        assertTrue(dumpCountryCodeStr.contains("mTelephonyCountryCode"));
        assertTrue(dumpCountryCodeStr.contains("mDefaultCountryCode"));
        assertTrue(dumpCountryCodeStr.contains("mTelephonyCountryTimestamp"));
        assertTrue(dumpCountryCodeStr.contains("mDriverCountryTimestamp"));
        assertTrue(dumpCountryCodeStr.contains("mReadyTimestamp"));
        assertTrue(dumpCountryCodeStr.contains("mReady"));
    }
}
