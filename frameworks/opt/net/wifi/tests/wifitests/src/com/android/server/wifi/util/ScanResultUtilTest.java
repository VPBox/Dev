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

package com.android.server.wifi.util;

import static org.junit.Assert.*;

import android.net.wifi.ScanResult;
import android.net.wifi.ScanResult.InformationElement;
import android.net.wifi.WifiConfiguration;
import android.net.wifi.WifiSsid;

import androidx.test.filters.SmallTest;

import com.android.server.wifi.ScanDetail;

import org.junit.Test;

import java.nio.charset.StandardCharsets;
import java.util.Arrays;

/**
 * Unit tests for {@link com.android.server.wifi.util.ScanResultUtil}.
 */
@SmallTest
public class ScanResultUtilTest {

    @Test
    public void convertScanResult() {
        final String ssid = "SOME SsId";

        ScanResult input = new ScanResult(WifiSsid.createFromAsciiEncoded(ssid), ssid,
                "ab:cd:01:ef:45:89", 1245, 0, "", -78, 2450, 1025, 22, 33, 20, 0, 0, true);

        input.informationElements = new InformationElement[] {
            createIE(InformationElement.EID_SSID, ssid.getBytes(StandardCharsets.UTF_8))
        };

        ScanDetail output = ScanResultUtil.toScanDetail(input);

        validateScanDetail(input, output);
    }

    @Test
    public void convertScanResultWithAnqpLines() {
        final String ssid = "SOME SsId";

        ScanResult input = new ScanResult(WifiSsid.createFromAsciiEncoded(ssid), ssid,
                "ab:cd:01:ef:45:89", 1245, 0, "some caps", -78, 2450, 1025, 22, 33, 20, 0, 0, true);

        input.informationElements = new InformationElement[] {
            createIE(InformationElement.EID_SSID, ssid.getBytes(StandardCharsets.UTF_8))
        };
        input.anqpLines = Arrays.asList("LINE 1", "line 2", "Line 3");

        ScanDetail output = ScanResultUtil.toScanDetail(input);

        validateScanDetail(input, output);
    }

    @Test
    public void convertScanResultWithoutWifiSsid() {
        final String ssid = "Another SSid";
        ScanResult input = new ScanResult(ssid, "ab:cd:01:ef:45:89", 1245, 0, "other caps",
                -78, 2450, 1025, 22, 33, 20, 0, 0, true);
        input.informationElements = new InformationElement[] {
            createIE(InformationElement.EID_SSID, ssid.getBytes(StandardCharsets.UTF_8))
        };

        ScanDetail output = ScanResultUtil.toScanDetail(input);

        validateScanDetail(input, output);
    }

    @Test
    public void testNetworkCreationFromScanResult() {
        final String ssid = "Another SSid";
        ScanResult scanResult = new ScanResult(ssid, "ab:cd:01:ef:45:89", 1245, 0, "",
                -78, 2450, 1025, 22, 33, 20, 0, 0, true);
        WifiConfiguration config;

        scanResult.capabilities = "";
        config = ScanResultUtil.createNetworkFromScanResult(scanResult);
        assertEquals(config.SSID, ScanResultUtil.createQuotedSSID(ssid));
        assertTrue(config.allowedKeyManagement.get(WifiConfiguration.KeyMgmt.NONE));

        scanResult.capabilities = "WEP";
        config = ScanResultUtil.createNetworkFromScanResult(scanResult);
        assertEquals(config.SSID, ScanResultUtil.createQuotedSSID(ssid));
        assertTrue(config.allowedKeyManagement.get(WifiConfiguration.KeyMgmt.NONE));
        assertTrue(config.allowedAuthAlgorithms.get(WifiConfiguration.AuthAlgorithm.OPEN));
        assertTrue(config.allowedAuthAlgorithms.get(WifiConfiguration.AuthAlgorithm.SHARED));

        scanResult.capabilities = "PSK";
        config = ScanResultUtil.createNetworkFromScanResult(scanResult);
        assertEquals(config.SSID, ScanResultUtil.createQuotedSSID(ssid));
        assertTrue(config.allowedKeyManagement.get(WifiConfiguration.KeyMgmt.WPA_PSK));

        scanResult.capabilities = "EAP";
        config = ScanResultUtil.createNetworkFromScanResult(scanResult);
        assertEquals(config.SSID, ScanResultUtil.createQuotedSSID(ssid));
        assertTrue(config.allowedKeyManagement.get(WifiConfiguration.KeyMgmt.WPA_EAP));
        assertTrue(config.allowedKeyManagement.get(WifiConfiguration.KeyMgmt.IEEE8021X));
    }

    /**
     * Test that a PSK-SHA256+SAE network is detected as transition mode
     */
    @Test
    public void testPskSha256SaeTransitionModeCheck() {
        final String ssid = "WPA3-Transition";
        String caps = "[WPA2-FT/PSK-CCMP][RSN-FT/PSK+PSK-SHA256+SAE+FT/SAE-CCMP][ESS][WPS]";

        ScanResult input = new ScanResult(WifiSsid.createFromAsciiEncoded(ssid), ssid,
                "ab:cd:01:ef:45:89", 1245, 0, caps, -78, 2450, 1025, 22, 33, 20, 0,
                0, true);

        input.informationElements = new InformationElement[] {
                createIE(InformationElement.EID_SSID, ssid.getBytes(StandardCharsets.UTF_8))
        };

        assertTrue(ScanResultUtil.isScanResultForPskSaeTransitionNetwork(input));
    }

    /**
     * Test that a PSK+SAE network is detected as transition mode
     */
    @Test
    public void testPskSaeTransitionModeCheck() {
        final String ssid = "WPA3-Transition";
        String caps = "[WPA2-FT/PSK+PSK+SAE+FT/SAE-CCMP][ESS][WPS]";

        ScanResult input = new ScanResult(WifiSsid.createFromAsciiEncoded(ssid), ssid,
                "ab:cd:01:ef:45:89", 1245, 0, caps, -78, 2450, 1025, 22, 33, 20, 0,
                0, true);

        input.informationElements = new InformationElement[] {
                createIE(InformationElement.EID_SSID, ssid.getBytes(StandardCharsets.UTF_8))
        };

        assertTrue(ScanResultUtil.isScanResultForPskSaeTransitionNetwork(input));
    }

    /**
     * Test that a PSK network is not detected as transition mode
     */
    @Test
    public void testPskNotInTransitionModeCheck() {
        final String ssid = "WPA2-Network";
        String caps = "[WPA2-FT/PSK+PSK][ESS][WPS]";

        ScanResult input = new ScanResult(WifiSsid.createFromAsciiEncoded(ssid), ssid,
                "ab:cd:01:ef:45:89", 1245, 0, caps, -78, 2450, 1025, 22, 33, 20, 0,
                0, true);

        input.informationElements = new InformationElement[] {
                createIE(InformationElement.EID_SSID, ssid.getBytes(StandardCharsets.UTF_8))
        };

        assertFalse(ScanResultUtil.isScanResultForPskSaeTransitionNetwork(input));
    }

    /**
     * Test that an SAE network is not detected as transition mode
     */
    @Test
    public void testSaeNotInTransitionModeCheck() {
        final String ssid = "WPA3-Network";
        String caps = "[WPA2-FT/SAE+SAE][ESS][WPS]";

        ScanResult input = new ScanResult(WifiSsid.createFromAsciiEncoded(ssid), ssid,
                "ab:cd:01:ef:45:89", 1245, 0, caps, -78, 2450, 1025, 22, 33, 20, 0,
                0, true);

        input.informationElements = new InformationElement[] {
                createIE(InformationElement.EID_SSID, ssid.getBytes(StandardCharsets.UTF_8))
        };

        assertFalse(ScanResultUtil.isScanResultForPskSaeTransitionNetwork(input));
    }

    private static InformationElement createIE(int id, byte[] bytes) {
        InformationElement ie = new InformationElement();
        ie.id = id;
        ie.bytes = bytes;
        return ie;
    }

    private static void validateScanDetail(ScanResult input, ScanDetail output) {
        assertNotNull("NetworkDetail was null", output.getNetworkDetail());
        assertNotNull("ScanResult was null", output.getScanResult());
        assertEquals("NetworkDetail SSID", input.SSID,
                output.getNetworkDetail().getSSID());
        assertEquals("ScanResult SSID", input.SSID,
                output.getScanResult().SSID);
        assertEquals("ScanResult wifiSsid", input.wifiSsid,
                output.getScanResult().wifiSsid);
        assertEquals("getSSID", input.SSID, output.getSSID());
        assertEquals("NetworkDetail BSSID", input.BSSID,
                output.getNetworkDetail().getBSSIDString());
        assertEquals("getBSSIDString", input.BSSID, output.getBSSIDString());
        assertEquals("ScanResult frequency", input.frequency,
                output.getScanResult().frequency);
        assertEquals("ScanResult level", input.level,
                output.getScanResult().level);
        assertEquals("ScanResult capabilities", input.capabilities,
                output.getScanResult().capabilities);
        assertEquals("ScanResult timestamp", input.timestamp,
                output.getScanResult().timestamp);
        assertArrayEquals("ScanResult information elements", input.informationElements,
                output.getScanResult().informationElements);
        assertEquals("ScanResult anqp lines", input.anqpLines,
                output.getScanResult().anqpLines);
    }

}
