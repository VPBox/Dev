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

import static org.junit.Assert.assertArrayEquals;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import android.net.wifi.ScanResult.InformationElement;

import androidx.test.filters.SmallTest;

import com.android.server.wifi.hotspot2.NetworkDetail;

import org.junit.Test;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.util.Arrays;
import java.util.BitSet;

/**
 * Unit tests for {@link com.android.server.wifi.util.InformationElementUtil}.
 */
@SmallTest
public class InformationElementUtilTest {

    // SSID Information Element tags
    private static final byte[] TEST_SSID_BYTES_TAG = new byte[] { (byte) 0x00, (byte) 0x0B };
    // SSID Information Element entry used for testing.
    private static final byte[] TEST_SSID_BYTES = "GoogleGuest".getBytes();
    // Valid zero length tag.
    private static final byte[] TEST_VALID_ZERO_LENGTH_TAG =
            new byte[] { (byte) 0x0B, (byte) 0x00 };
    // BSS_LOAD Information Element entry used for testing.
    private static final byte[] TEST_BSS_LOAD_BYTES_IE =
            new byte[] { (byte) 0x0B, (byte) 0x01, (byte) 0x08 };

    /*
     * Function to provide SSID Information Element (SSID = "GoogleGuest").
     *
     * @return byte[] Byte array representing the test SSID
     */
    private byte[] getTestSsidIEBytes() throws IOException {
        return concatenateByteArrays(TEST_SSID_BYTES_TAG, TEST_SSID_BYTES);
    }

    /*
     * Function used to set byte arrays used for testing.
     *
     * @param byteArrays variable number of byte arrays to concatenate
     * @return byte[] Byte array resulting from concatenating the arrays passed to the function
     */
    private static byte[] concatenateByteArrays(byte[]... byteArrays) throws IOException {
        final ByteArrayOutputStream baos = new ByteArrayOutputStream();
        for (byte[] b : byteArrays) {
            baos.write(b);
        }
        baos.flush();
        return baos.toByteArray();
    }

    /**
     * Test parseInformationElements with an empty byte array.
     * Expect parseInformationElement to return an empty InformationElement array.
     */
    @Test
    public void parseInformationElements_withEmptyByteArray() {
        byte[] emptyBytes = new byte[0];
        InformationElement[] results =
                InformationElementUtil.parseInformationElements(emptyBytes);
        assertEquals("parsed results should be empty", 0, results.length);
    }

    /**
     * Test parseInformationElements called with a null parameter.
     * Expect parseInfomrationElement to return an empty InformationElement array.
     */
    @Test
    public void parseInformationElements_withNullBytes() {
        byte[] nullBytes = null;
        InformationElement[] results =
                InformationElementUtil.parseInformationElements(nullBytes);
        assertEquals("parsed results should be empty", 0, results.length);
    }

    /*
     * Test parseInformationElements with a single element represented in the byte array.
     * Expect a single element to be returned in the InformationElements array.  The
     * length of this array should be 1 and the contents should be valid.
     *
     * @throws java.io.IOException
     */
    @Test
    public void parseInformationElements_withSingleElement() throws IOException {
        byte[] ssidBytes = getTestSsidIEBytes();

        InformationElement[] results =
                InformationElementUtil.parseInformationElements(ssidBytes);
        assertEquals("Parsed results should have 1 IE", 1, results.length);
        assertEquals("Parsed result should be a ssid", InformationElement.EID_SSID, results[0].id);
        assertArrayEquals("parsed SSID does not match input",
                TEST_SSID_BYTES, results[0].bytes);
    }

    /*
     * Test parseInformationElement with extra padding in the data to parse.
     * Expect the function to return the SSID information element.
     *
     * Note: Experience shows that APs often pad messages with 0x00.  This happens to be the tag for
     * EID_SSID.  This test checks if padding will be properly discarded.
     *
     * @throws java.io.IOException
     */
    @Test
    public void parseInformationElements_withExtraPadding() throws IOException {
        byte[] paddingBytes = new byte[10];
        Arrays.fill(paddingBytes, (byte) 0x00);
        byte[] ssidBytesWithPadding = concatenateByteArrays(getTestSsidIEBytes(), paddingBytes);

        InformationElement[] results =
                InformationElementUtil.parseInformationElements(ssidBytesWithPadding);
        assertEquals("Parsed results should have 1 IE", 1, results.length);
        assertEquals("Parsed result should be a ssid", InformationElement.EID_SSID, results[0].id);
        assertArrayEquals("parsed SSID does not match input",
                TEST_SSID_BYTES, results[0].bytes);
    }

    /*
     * Test parseInformationElement with two elements where the second element has an invalid
     * length.
     * Expect the function to return the first valid entry and skip the remaining information.
     *
     * Note:  This test partially exposes issues with blindly parsing the data.  A higher level
     * function to validate the parsed data may be added.
     *
     * @throws java.io.IOException
     * */
    @Test
    public void parseInformationElements_secondElementInvalidLength() throws IOException {
        byte[] invalidTag = new byte[] { (byte) 0x01, (byte) 0x08, (byte) 0x08 };
        byte[] twoTagsSecondInvalidBytes = concatenateByteArrays(getTestSsidIEBytes(), invalidTag);

        InformationElement[] results =
                InformationElementUtil.parseInformationElements(twoTagsSecondInvalidBytes);
        assertEquals("Parsed results should have 1 IE", 1, results.length);
        assertEquals("Parsed result should be a ssid.", InformationElement.EID_SSID, results[0].id);
        assertArrayEquals("parsed SSID does not match input",
                TEST_SSID_BYTES, results[0].bytes);
    }

    /*
     * Test parseInformationElements with two valid Information Element entries.
     * Expect the function to return an InformationElement array with two entries containing valid
     * data.
     *
     * @throws java.io.IOException
     */
    @Test
    public void parseInformationElements_twoElements() throws IOException {
        byte[] twoValidTagsBytes =
                concatenateByteArrays(getTestSsidIEBytes(), TEST_BSS_LOAD_BYTES_IE);

        InformationElement[] results =
                InformationElementUtil.parseInformationElements(twoValidTagsBytes);
        assertEquals("parsed results should have 2 elements", 2, results.length);
        assertEquals("First parsed element should be a ssid",
                InformationElement.EID_SSID, results[0].id);
        assertArrayEquals("parsed SSID does not match input",
                TEST_SSID_BYTES, results[0].bytes);
        assertEquals("second element should be a BSS_LOAD tag",
                InformationElement.EID_BSS_LOAD, results[1].id);
        assertEquals("second element should have data of length 1", 1, results[1].bytes.length);
        assertEquals("second element data was not parsed correctly.",
                (byte) 0x08, results[1].bytes[0]);
    }

    /*
     * Test parseInformationElements with two elements where the first information element has a
     * length of zero.
     * Expect the function to return an InformationElement array with two entries containing valid
     * data.
     *
     * @throws java.io.IOException
     */
    @Test
    public void parseInformationElements_firstElementZeroLength() throws IOException {
        byte[] zeroLengthTagWithSSIDBytes =
                concatenateByteArrays(TEST_VALID_ZERO_LENGTH_TAG, getTestSsidIEBytes());

        InformationElement[] results =
                InformationElementUtil.parseInformationElements(zeroLengthTagWithSSIDBytes);
        assertEquals("Parsed results should have 2 elements.", 2, results.length);
        assertEquals("First element tag should be EID_BSS_LOAD",
                InformationElement.EID_BSS_LOAD, results[0].id);
        assertEquals("First element should be length 0", 0, results[0].bytes.length);

        assertEquals("Second element should be a ssid", InformationElement.EID_SSID, results[1].id);
        assertArrayEquals("parsed SSID does not match input",
                TEST_SSID_BYTES, results[1].bytes);
    }

    /*
     * Test parseInformationElements with two elements where the first element has an invalid
     * length.  The invalid length in the first element causes us to miss the start of the second
     * Infomation Element.  This results in a single element in the returned array.
     * Expect the function to return a single entry in an InformationElement array. This returned
     * entry is not validated at this time and does not contain valid data (since the incorrect
     * length was used).
     * TODO: attempt to validate the data and recover as much as possible.  When the follow-on CL
     * is in development, this test will be updated to reflect the change.
     *
     * @throws java.io.IOException
     */
    @Test
    public void parseInformationElements_firstElementWrongLength() throws IOException {
        byte[] invalidLengthTag = new byte[] {(byte) 0x0B, (byte) 0x01 };
        byte[] invalidLengthTagWithSSIDBytes =
                concatenateByteArrays(invalidLengthTag, getTestSsidIEBytes());

        InformationElement[] results =
                InformationElementUtil.parseInformationElements(invalidLengthTagWithSSIDBytes);
        assertEquals("Parsed results should have 1 element", 1, results.length);
        assertEquals("First result should be a EID_BSS_LOAD tag.",
                InformationElement.EID_BSS_LOAD, results[0].id);
        assertEquals("First result should have data of 1 byte", 1, results[0].bytes.length);
        assertEquals("First result should have data set to 0x00",
                invalidLengthTagWithSSIDBytes[2], results[0].bytes[0]);
    }

    /**
     * Test Capabilities.generateCapabilitiesString() with a RSN IE.
     * Expect the function to return a string with the proper security information.
     */
    @Test
    public void buildCapabilities_rsnElement() {
        InformationElement ie = new InformationElement();
        ie.id = InformationElement.EID_RSN;
        ie.bytes = new byte[] { (byte) 0x01, (byte) 0x00, (byte) 0x00, (byte) 0x0F,
                                (byte) 0xAC, (byte) 0x02, (byte) 0x02, (byte) 0x00,
                                (byte) 0x00, (byte) 0x0F, (byte) 0xAC, (byte) 0x04,
                                (byte) 0x00, (byte) 0x0F, (byte) 0xAC, (byte) 0x02,
                                (byte) 0x01, (byte) 0x00, (byte) 0x00, (byte) 0x0F,
                                (byte) 0xAC, (byte) 0x02, (byte) 0x00, (byte) 0x00 };

        InformationElement[] ies = new InformationElement[] { ie };

        BitSet beaconCap = new BitSet(16);
        beaconCap.set(4);

        InformationElementUtil.Capabilities capabilities =
                new InformationElementUtil.Capabilities();
        capabilities.from(ies, beaconCap, false);
        String result = capabilities.generateCapabilitiesString();

        assertEquals("[WPA2-PSK-CCMP+TKIP][RSN-PSK-CCMP+TKIP]", result);
    }

    /**
     * Test Capabilities.generateCapabilitiesString() with a RSN IE which is malformed.
     * Expect the function to return a string with empty key management & pairswise cipher security
     * information.
     */
    @Test
    public void buildCapabilities_malformedRsnElement() {
        InformationElement ie = new InformationElement();
        ie.id = InformationElement.EID_RSN;
        ie.bytes = new byte[] { (byte) 0x01, (byte) 0x00, (byte) 0x00, (byte) 0x0F,
                (byte) 0xAC, (byte) 0x02, (byte) 0x02, (byte) 0x00,
                (byte) 0x00, (byte) 0x0F, (byte) 0xAC };

        InformationElement[] ies = new InformationElement[] { ie };

        BitSet beaconCap = new BitSet(16);
        beaconCap.set(4);

        InformationElementUtil.Capabilities capabilities =
                new InformationElementUtil.Capabilities();
        capabilities.from(ies, beaconCap, false);
        String result = capabilities.generateCapabilitiesString();

        assertEquals("[RSN]", result);
    }

    /**
     * Test Capabilities.generateCapabilitiesString() with a WPA type 1 IE.
     * Expect the function to return a string with the proper security information.
     */
    @Test
    public void buildCapabilities_wpa1Element() {
        InformationElement ie = new InformationElement();
        ie.id = InformationElement.EID_VSA;
        ie.bytes = new byte[] { (byte) 0x00, (byte) 0x50, (byte) 0xF2, (byte) 0x01,
                                (byte) 0x01, (byte) 0x00, (byte) 0x00, (byte) 0x50,
                                (byte) 0xF2, (byte) 0x02, (byte) 0x02, (byte) 0x00,
                                (byte) 0x00, (byte) 0x50, (byte) 0xF2, (byte) 0x04,
                                (byte) 0x00, (byte) 0x50, (byte) 0xF2, (byte) 0x02,
                                (byte) 0x01, (byte) 0x00, (byte) 0x00, (byte) 0x50,
                                (byte) 0xF2, (byte) 0x02, (byte) 0x00, (byte) 0x00 };

        InformationElement[] ies = new InformationElement[] { ie };

        BitSet beaconCap = new BitSet(16);
        beaconCap.set(4);
        InformationElementUtil.Capabilities capabilities =
                new InformationElementUtil.Capabilities();
        capabilities.from(ies, beaconCap, false);
        String result = capabilities.generateCapabilitiesString();

        assertEquals("[WPA-PSK-CCMP+TKIP]", result);
    }

    /**
     * Test Capabilities.generateCapabilitiesString() with a WPA type 1 IE which is malformed.
     * Expect the function to return a string with empty key management & pairswise cipher security
     * information.
     */
    @Test
    public void buildCapabilities_malformedWpa1Element() {
        InformationElement ie = new InformationElement();
        ie.id = InformationElement.EID_VSA;
        ie.bytes = new byte[] { (byte) 0x00, (byte) 0x50, (byte) 0xF2, (byte) 0x01,
                (byte) 0x01, (byte) 0x00 };

        InformationElement[] ies = new InformationElement[] { ie };

        BitSet beaconCap = new BitSet(16);
        beaconCap.set(4);
        InformationElementUtil.Capabilities capabilities =
                new InformationElementUtil.Capabilities();
        capabilities.from(ies, beaconCap, false);
        String result = capabilities.generateCapabilitiesString();

        assertEquals("[WPA]", result);
    }

    /**
     * Test Capabilities.generateCapabilitiesString() with both RSN and WPA1 IE.
     * Expect the function to return a string with the proper security information.
     */
    @Test
    public void buildCapabilities_rsnAndWpaElement() {
        InformationElement ieRsn = new InformationElement();
        ieRsn.id = InformationElement.EID_RSN;
        ieRsn.bytes = new byte[] { (byte) 0x01, (byte) 0x00, (byte) 0x00, (byte) 0x0F,
                                   (byte) 0xAC, (byte) 0x02, (byte) 0x02, (byte) 0x00,
                                   (byte) 0x00, (byte) 0x0F, (byte) 0xAC, (byte) 0x04,
                                   (byte) 0x00, (byte) 0x0F, (byte) 0xAC, (byte) 0x02,
                                   (byte) 0x01, (byte) 0x00, (byte) 0x00, (byte) 0x0F,
                                   (byte) 0xAC, (byte) 0x02, (byte) 0x00, (byte) 0x00 };

        InformationElement ieWpa = new InformationElement();
        ieWpa.id = InformationElement.EID_VSA;
        ieWpa.bytes = new byte[] { (byte) 0x00, (byte) 0x50, (byte) 0xF2, (byte) 0x01,
                                   (byte) 0x01, (byte) 0x00, (byte) 0x00, (byte) 0x50,
                                   (byte) 0xF2, (byte) 0x02, (byte) 0x02, (byte) 0x00,
                                   (byte) 0x00, (byte) 0x50, (byte) 0xF2, (byte) 0x04,
                                   (byte) 0x00, (byte) 0x50, (byte) 0xF2, (byte) 0x02,
                                   (byte) 0x01, (byte) 0x00, (byte) 0x00, (byte) 0x50,
                                   (byte) 0xF2, (byte) 0x02, (byte) 0x00, (byte) 0x00 };

        InformationElement[] ies = new InformationElement[] { ieWpa, ieRsn };

        BitSet beaconCap = new BitSet(16);
        beaconCap.set(4);

        InformationElementUtil.Capabilities capabilities =
                new InformationElementUtil.Capabilities();
        capabilities.from(ies, beaconCap, false);
        String result = capabilities.generateCapabilitiesString();

        assertEquals("[WPA-PSK-CCMP+TKIP][WPA2-PSK-CCMP+TKIP][RSN-PSK-CCMP+TKIP]", result);
    }

    /**
     * Test Capabilities.generateCapabilitiesString() with RSN IE, CCMP and PSK+SAE transition mode.
     * Expect the function to return a string with the proper security information.
     */
    @Test
    public void buildCapabilities_rsnPskSaeTransitionElement() {
        InformationElement ieRsn = new InformationElement();
        ieRsn.id = InformationElement.EID_RSN;
        ieRsn.bytes = new byte[] {
                // RSNE Version (0x0001)
                (byte) 0x01, (byte) 0x00,
                // Group cipher suite: CCMP
                (byte) 0x00, (byte) 0x0F, (byte) 0xAC, (byte) 0x04,
                // Number of cipher suites (1)
                (byte) 0x01, (byte) 0x00,
                // Cipher suite: CCMP
                (byte) 0x00, (byte) 0x0F, (byte) 0xAC, (byte) 0x04,
                // Number of AKMs (2)
                (byte) 0x02, (byte) 0x00,
                // PSK AKM
                (byte) 0x00, (byte) 0x0F, (byte) 0xAC, (byte) 0x02,
                // SAE AKM
                (byte) 0x00, (byte) 0x0F, (byte) 0xAC, (byte) 0x08,
                // Padding
                (byte) 0x00, (byte) 0x00 };

        InformationElement[] ies = new InformationElement[] { ieRsn };

        BitSet beaconCap = new BitSet(16);
        beaconCap.set(4);

        InformationElementUtil.Capabilities capabilities =
                new InformationElementUtil.Capabilities();
        capabilities.from(ies, beaconCap, true);
        String result = capabilities.generateCapabilitiesString();

        assertEquals("[WPA2-PSK-CCMP][RSN-PSK+SAE-CCMP]", result);
    }

    /**
     * Test Capabilities.generateCapabilitiesString() with RSN IE, CCMP and SAE+FT/SAE.
     * Expect the function to return a string with the proper security information.
     */
    @Test
    public void buildCapabilities_rsnSaeFtSaeElement() {
        InformationElement ieRsn = new InformationElement();
        ieRsn.id = InformationElement.EID_RSN;
        ieRsn.bytes = new byte[] {
                // RSNE Version (0x0001)
                (byte) 0x01, (byte) 0x00,
                // Group cipher suite: CCMP
                (byte) 0x00, (byte) 0x0F, (byte) 0xAC, (byte) 0x04,
                // Number of cipher suites (1)
                (byte) 0x01, (byte) 0x00,
                // Cipher suite: CCMP
                (byte) 0x00, (byte) 0x0F, (byte) 0xAC, (byte) 0x04,
                // Number of AKMs (2)
                (byte) 0x02, (byte) 0x00,
                // SAE AKM
                (byte) 0x00, (byte) 0x0F, (byte) 0xAC, (byte) 0x08,
                // FT/SAE AKM
                (byte) 0x00, (byte) 0x0F, (byte) 0xAC, (byte) 0x09,
                // Padding
                (byte) 0x00, (byte) 0x00 };

        InformationElement[] ies = new InformationElement[] { ieRsn };

        BitSet beaconCap = new BitSet(16);
        beaconCap.set(4);

        InformationElementUtil.Capabilities capabilities =
                new InformationElementUtil.Capabilities();
        capabilities.from(ies, beaconCap, true);
        String result = capabilities.generateCapabilitiesString();

        assertEquals("[RSN-SAE+FT/SAE-CCMP]", result);
    }

    /**
     * Test Capabilities.generateCapabilitiesString() with RSN IE, CCMP and OWE.
     * Expect the function to return a string with the proper security information.
     */
    @Test
    public void buildCapabilities_rsnOweElement() {
        InformationElement ieRsn = new InformationElement();
        ieRsn.id = InformationElement.EID_RSN;
        ieRsn.bytes = new byte[] {
                // RSNE Version (0x0001)
                (byte) 0x01, (byte) 0x00,
                // Group cipher suite: CCMP
                (byte) 0x00, (byte) 0x0F, (byte) 0xAC, (byte) 0x04,
                // Number of cipher suites (1)
                (byte) 0x01, (byte) 0x00,
                // Cipher suite: CCMP
                (byte) 0x00, (byte) 0x0F, (byte) 0xAC, (byte) 0x04,
                // Number of AKMs (1)
                (byte) 0x01, (byte) 0x00,
                // OWE AKM
                (byte) 0x00, (byte) 0x0F, (byte) 0xAC, (byte) 0x12,
                // Padding
                (byte) 0x00, (byte) 0x00 };

        InformationElement[] ies = new InformationElement[] { ieRsn };

        BitSet beaconCap = new BitSet(16);
        beaconCap.set(4);

        InformationElementUtil.Capabilities capabilities =
                new InformationElementUtil.Capabilities();
        capabilities.from(ies, beaconCap, true);
        String result = capabilities.generateCapabilitiesString();

        assertEquals("[RSN-OWE-CCMP]", result);
    }

    /**
     * Test Capabilities.generateCapabilitiesString() with OWE IE.
     * Expect the function to return a string with the proper security information.
     */
    @Test
    public void buildCapabilities_oweVsElementOweSupported() {
        InformationElement ieOwe = new InformationElement();
        ieOwe.id = InformationElement.EID_VSA;
        ieOwe.bytes = new byte[] {
                // OWE vendor specific
                (byte) 0x50, (byte) 0x6F, (byte) 0x9A, (byte) 0x1C,
                // OWE IE contains BSSID, SSID and channel of other BSS, but we don't parse it.
                (byte) 0x00, (byte) 0x000, (byte) 0x00, (byte) 0x00 };

        InformationElement[] ies = new InformationElement[] { ieOwe };

        BitSet beaconCap = new BitSet(16);
        beaconCap.set(0);

        InformationElementUtil.Capabilities capabilities =
                new InformationElementUtil.Capabilities();
        capabilities.from(ies, beaconCap, true);
        String result = capabilities.generateCapabilitiesString();

        assertEquals("[RSN-OWE_TRANSITION-CCMP][ESS]", result);
    }

    /**
     * Test Capabilities.generateCapabilitiesString() with OWE IE.
     * Expect the function to return a string with the proper security information.
     */
    @Test
    public void buildCapabilities_oweVsElementOweNotSupported() {
        InformationElement ieOwe = new InformationElement();
        ieOwe.id = InformationElement.EID_VSA;
        ieOwe.bytes = new byte[] {
                // OWE vendor specific
                (byte) 0x50, (byte) 0x6F, (byte) 0x9A, (byte) 0x1C,
                // OWE IE contains BSSID, SSID and channel of other BSS, but we don't parse it.
                (byte) 0x00, (byte) 0x000, (byte) 0x00, (byte) 0x00 };

        InformationElement[] ies = new InformationElement[] { ieOwe };

        BitSet beaconCap = new BitSet(16);
        beaconCap.set(0);

        InformationElementUtil.Capabilities capabilities =
                new InformationElementUtil.Capabilities();
        capabilities.from(ies, beaconCap, false);
        String result = capabilities.generateCapabilitiesString();

        assertEquals("[ESS]", result);
    }

    /**
     * Test Capabilities.generateCapabilitiesString() with RSN IE, GCMP-256 and SUITE_B_192.
     * Expect the function to return a string with the proper security information.
     */
    @Test
    public void buildCapabilities_rsnSuiteB192Element() {
        InformationElement ieRsn = new InformationElement();
        ieRsn.id = InformationElement.EID_RSN;
        ieRsn.bytes = new byte[] {
                // RSNE Version (0x0001)
                (byte) 0x01, (byte) 0x00,
                // Group cipher suite: GCMP-256
                (byte) 0x00, (byte) 0x0F, (byte) 0xAC, (byte) 0x09,
                // Number of cipher suites (1)
                (byte) 0x01, (byte) 0x00,
                // Cipher suite: GCMP-256
                (byte) 0x00, (byte) 0x0F, (byte) 0xAC, (byte) 0x09,
                // Number of AKMs (1)
                (byte) 0x01, (byte) 0x00,
                // SUITE_B_192 AKM
                (byte) 0x00, (byte) 0x0F, (byte) 0xAC, (byte) 0x0C,
                // Padding
                (byte) 0x00, (byte) 0x00 };

        InformationElement[] ies = new InformationElement[] { ieRsn };

        BitSet beaconCap = new BitSet(16);
        beaconCap.set(4);

        InformationElementUtil.Capabilities capabilities =
                new InformationElementUtil.Capabilities();
        capabilities.from(ies, beaconCap, false);
        String result = capabilities.generateCapabilitiesString();

        assertEquals("[RSN-EAP_SUITE_B_192-GCMP-256]", result);
    }

    /**
     * Test Capabilities.generateCapabilitiesString() with both RSN and WPA1 IE which are malformed.
     * Expect the function to return a string with empty key management & pairswise cipher security
     * information.
     */
    @Test
    public void buildCapabilities_malformedRsnAndWpaElement() {
        InformationElement ieRsn = new InformationElement();
        ieRsn.id = InformationElement.EID_RSN;
        ieRsn.bytes = new byte[] { (byte) 0x01, (byte) 0x00, (byte) 0x00, (byte) 0x0F,
                (byte) 0xAC, (byte) 0x02, (byte) 0x02 };

        InformationElement ieWpa = new InformationElement();
        ieWpa.id = InformationElement.EID_VSA;
        ieWpa.bytes = new byte[] { (byte) 0x00, (byte) 0x50, (byte) 0xF2, (byte) 0x01,
                (byte) 0x01, (byte) 0x00, (byte) 0x00, (byte) 0x50,
                (byte) 0xF2, (byte) 0x02, (byte) 0x02, (byte) 0x00,
                (byte) 0x00, (byte) 0x50 };

        InformationElement[] ies = new InformationElement[] { ieWpa, ieRsn };

        BitSet beaconCap = new BitSet(16);
        beaconCap.set(4);

        InformationElementUtil.Capabilities capabilities =
                new InformationElementUtil.Capabilities();
        capabilities.from(ies, beaconCap, false);
        String result = capabilities.generateCapabilitiesString();

        assertEquals("[WPA][RSN]", result);
    }

    /**
     * Test Capabilities.generateCapabilitiesString() with both WPS and WPA1 IE.
     * Expect the function to return a string with the proper security information.
     */
    @Test
    public void buildCapabilities_wpaAndWpsElement() {
        InformationElement ieWpa = new InformationElement();
        ieWpa.id = InformationElement.EID_VSA;
        ieWpa.bytes = new byte[] { (byte) 0x00, (byte) 0x50, (byte) 0xF2, (byte) 0x01,
                                   (byte) 0x01, (byte) 0x00, (byte) 0x00, (byte) 0x50,
                                   (byte) 0xF2, (byte) 0x02, (byte) 0x02, (byte) 0x00,
                                   (byte) 0x00, (byte) 0x50, (byte) 0xF2, (byte) 0x04,
                                   (byte) 0x00, (byte) 0x50, (byte) 0xF2, (byte) 0x02,
                                   (byte) 0x01, (byte) 0x00, (byte) 0x00, (byte) 0x50,
                                   (byte) 0xF2, (byte) 0x02, (byte) 0x00, (byte) 0x00 };

        InformationElement ieWps = new InformationElement();
        ieWps.id = InformationElement.EID_VSA;
        ieWps.bytes = new byte[] { (byte) 0x00, (byte) 0x50, (byte) 0xF2, (byte) 0x04 };

        InformationElement[] ies = new InformationElement[] { ieWpa, ieWps };

        BitSet beaconCap = new BitSet(16);
        beaconCap.set(4);


        InformationElementUtil.Capabilities capabilities =
                 new InformationElementUtil.Capabilities();
        capabilities.from(ies, beaconCap, false);
        String result = capabilities.generateCapabilitiesString();

        assertEquals("[WPA-PSK-CCMP+TKIP][WPS]", result);
    }

    /**
     * Test Capabilities.generateCapabilitiesString() with a vendor specific element which
     * is not WPA type 1. Beacon Capability Information field has the Privacy
     * bit set.
     *
     * Expect the function to return a string with the proper security information.
     */
    @Test
    public void buildCapabilities_nonRsnWpa1Element_privacySet() {
        InformationElement ie = new InformationElement();
        ie.id = InformationElement.EID_VSA;
        ie.bytes = new byte[] { (byte) 0x00, (byte) 0x04, (byte) 0x0E, (byte) 0x01,
                                (byte) 0x01, (byte) 0x02, (byte) 0x01, (byte) 0x00,
                                (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00 };

        InformationElement[] ies = new InformationElement[] { ie };

        BitSet beaconCap = new BitSet(16);
        beaconCap.set(4);

        InformationElementUtil.Capabilities capabilities =
                new InformationElementUtil.Capabilities();
        capabilities.from(ies, beaconCap, false);
        String result = capabilities.generateCapabilitiesString();


        assertEquals("[WEP]", result);
    }

    /**
     * Test Capabilities.generateCapabilitiesString() with a vendor specific element which
     * is not WPA type 1. Beacon Capability Information field doesn't have the
     * Privacy bit set.
     *
     * Expect the function to return an empty string.
     */
    @Test
    public void buildCapabilities_nonRsnWpa1Element_privacyClear() {
        InformationElement ie = new InformationElement();
        ie.id = InformationElement.EID_VSA;
        ie.bytes = new byte[] { (byte) 0x00, (byte) 0x04, (byte) 0x0E, (byte) 0x01,
                                (byte) 0x01, (byte) 0x02, (byte) 0x01, (byte) 0x00,
                                (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00 };

        InformationElement[] ies = new InformationElement[] { ie };

        BitSet beaconCap = new BitSet(16);
        beaconCap.clear(4);

        InformationElementUtil.Capabilities capabilities =
                new InformationElementUtil.Capabilities();
        capabilities.from(ies, beaconCap, false);
        String result = capabilities.generateCapabilitiesString();


        assertEquals("", result);
    }

    /**
     * Test Capabilities.generateCapabilitiesString() with a vendor specific element which
     * is not WPA type 1. Beacon Capability Information field has the ESS bit set.
     *
     * Expect the function to return a string with [ESS] there.
     */
    @Test
    public void buildCapabilities_nonRsnWpa1Element_essSet() {
        InformationElement ie = new InformationElement();
        ie.id = InformationElement.EID_VSA;
        ie.bytes = new byte[] { (byte) 0x00, (byte) 0x04, (byte) 0x0E, (byte) 0x01,
                                (byte) 0x01, (byte) 0x02, (byte) 0x01, (byte) 0x00,
                                (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00 };

        InformationElement[] ies = new InformationElement[] { ie };

        BitSet beaconCap = new BitSet(16);
        beaconCap.set(0);

        InformationElementUtil.Capabilities capabilities =
                new InformationElementUtil.Capabilities();
        capabilities.from(ies, beaconCap, false);
        String result = capabilities.generateCapabilitiesString();


        assertEquals("[ESS]", result);
    }

    /**
     * Test Capabilities.generateCapabilitiesString() with a vendor specific element which
     * is not WPA type 1. Beacon Capability Information field doesn't have the
     * ESS bit set.
     *
     * Expect the function to return an empty string.
     */
    @Test
    public void buildCapabilities_nonRsnWpa1Element_essClear() {
        InformationElement ie = new InformationElement();
        ie.id = InformationElement.EID_VSA;
        ie.bytes = new byte[] { (byte) 0x00, (byte) 0x04, (byte) 0x0E, (byte) 0x01,
                                (byte) 0x01, (byte) 0x02, (byte) 0x01, (byte) 0x00,
                                (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00 };

        InformationElement[] ies = new InformationElement[] { ie };

        BitSet beaconCap = new BitSet(16);
        beaconCap.clear(0);

        InformationElementUtil.Capabilities capabilities =
                new InformationElementUtil.Capabilities();
        capabilities.from(ies, beaconCap, false);
        String result = capabilities.generateCapabilitiesString();


        assertEquals("", result);
    }

    /**
     * Verify the expectations when building an ExtendedCapabilites IE from data with no bits set.
     * Both ExtendedCapabilities#isStrictUtf8() and ExtendedCapabilites#is80211McRTTResponder()
     * should return false.
     */
    @Test
    public void buildExtendedCapabilities_emptyBitSet() {
        InformationElement ie = new InformationElement();
        ie.id = InformationElement.EID_EXTENDED_CAPS;
        ie.bytes = new byte[8];

        InformationElementUtil.ExtendedCapabilities extendedCap =
                new InformationElementUtil.ExtendedCapabilities();
        extendedCap.from(ie);
        assertFalse(extendedCap.isStrictUtf8());
        assertFalse(extendedCap.is80211McRTTResponder());
    }

    /**
     * Verify the expectations when building an ExtendedCapabilites IE from data with UTF-8 SSID
     * bit set (bit 48).  ExtendedCapabilities#isStrictUtf8() should return true.
     */
    @Test
    public void buildExtendedCapabilites_strictUtf8() {
        InformationElement ie = new InformationElement();
        ie.id = InformationElement.EID_EXTENDED_CAPS;
        ie.bytes = new byte[] { (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00,
                                (byte) 0x00, (byte) 0x00, (byte) 0x01, (byte) 0x00 };

        InformationElementUtil.ExtendedCapabilities extendedCap =
                new InformationElementUtil.ExtendedCapabilities();
        extendedCap.from(ie);
        assertTrue(extendedCap.isStrictUtf8());
        assertFalse(extendedCap.is80211McRTTResponder());
    }

    /**
     * Verify the expectations when building an ExtendedCapabilites IE from data with RTT Response
     * Enable bit set (bit 70).  ExtendedCapabilities#is80211McRTTResponder() should return true.
     */
    @Test
    public void buildExtendedCapabilites_80211McRTTResponder() {
        InformationElement ie = new InformationElement();
        ie.id = InformationElement.EID_EXTENDED_CAPS;
        ie.bytes = new byte[] { (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00,
                                (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00,
                                (byte) 0x40 };

        InformationElementUtil.ExtendedCapabilities extendedCap =
                new InformationElementUtil.ExtendedCapabilities();
        extendedCap.from(ie);
        assertFalse(extendedCap.isStrictUtf8());
        assertTrue(extendedCap.is80211McRTTResponder());
    }

    /**
     * Test a that a correctly formed TIM Information Element is decoded into a valid TIM element,
     * and the values are captured
     */
    @Test
    public void parseTrafficIndicationMapInformationElementValid() {
        InformationElement ie = new InformationElement();
        ie.id = InformationElement.EID_TIM;
        ie.bytes = new byte[] { (byte) 0x03, (byte) 0x05, (byte) 0x00, (byte) 0x00};
        InformationElementUtil.TrafficIndicationMap trafficIndicationMap =
                new InformationElementUtil.TrafficIndicationMap();
        trafficIndicationMap.from(ie);
        assertEquals(trafficIndicationMap.mLength, 4);
        assertEquals(trafficIndicationMap.mDtimCount, 3);
        assertEquals(trafficIndicationMap.mDtimPeriod, 5);
        assertEquals(trafficIndicationMap.mBitmapControl, 0);
        assertEquals(trafficIndicationMap.isValid(), true);
    }

    /**
     * Test that a short invalid Information Element is marked as being an invalid TIM element when
     * parsed as Traffic Indication Map.
     */
    @Test
    public void parseTrafficIndicationMapInformationElementInvalidTooShort() {
        InformationElement ie = new InformationElement();
        ie.id = InformationElement.EID_TIM;
        ie.bytes = new byte[] { (byte) 0x01, (byte) 0x07 };
        InformationElementUtil.TrafficIndicationMap trafficIndicationMap =
                new InformationElementUtil.TrafficIndicationMap();
        trafficIndicationMap.from(ie);
        assertEquals(trafficIndicationMap.isValid(), false);
    }

    /**
     * Test that a too-large invalid Information Element is marked as an invalid TIM element when
     * parsed as Traffic Indication Map.
     */
    @Test
    public void parseTrafficIndicationMapInformationElementInvalidTooLong() {
        InformationElement ie = new InformationElement();
        ie.id = InformationElement.EID_TIM;
        ie.bytes = new byte[255]; // bytes length of upto 254 is valid for TIM
        Arrays.fill(ie.bytes, (byte) 7);
        InformationElementUtil.TrafficIndicationMap trafficIndicationMap =
                new InformationElementUtil.TrafficIndicationMap();
        trafficIndicationMap.from(ie);
        assertEquals(trafficIndicationMap.isValid(), false);
    }

    /**
     * Verify that the expected Roaming Consortium information element is parsed and retrieved
     * from the list of IEs.
     *
     * @throws Exception
     */
    @Test
    public void getRoamingConsortiumIE() throws Exception {
        InformationElement ie = new InformationElement();
        ie.id = InformationElement.EID_ROAMING_CONSORTIUM;
        /**
         * Roaming Consortium Format;
         * | Number of OIs | OI#1 and OI#2 Lengths | OI #1 | OI #2 (optional) | OI #3 (optional) |
         *        1                  1              variable     variable           variable
         */
        ie.bytes = new byte[] { (byte) 0x01 /* number of OIs */, (byte) 0x03 /* OI Length */,
                                (byte) 0x11, (byte) 0x22, (byte) 0x33};
        InformationElementUtil.RoamingConsortium roamingConsortium =
                InformationElementUtil.getRoamingConsortiumIE(new InformationElement[] {ie});
        assertEquals(1, roamingConsortium.anqpOICount);
        assertEquals(1, roamingConsortium.getRoamingConsortiums().length);
        assertEquals(0x112233, roamingConsortium.getRoamingConsortiums()[0]);
    }

    /**
     * Verify that the expected Hotspot 2.0 Vendor Specific information element is parsed and
     * retrieved from the list of IEs.
     *
     * @throws Exception
     */
    @Test
    public void getHS2VendorSpecificIE() throws Exception {
        InformationElement ie = new InformationElement();
        ie.id = InformationElement.EID_VSA;
        /**
         * Vendor Specific OI Format:
         * | OI | Type | Hotspot Configuration | PPS MO ID (optional) | ANQP Domain ID (optional)
         *    3    1              1                    2                        2
         *
         * With OI=0x506F9A and Type=0x10 for Hotspot 2.0
         *
         * The Format of Hotspot Configuration:
         *        B0               B1                   B2             B3    B4              B7
         * | DGAF Disabled | PPS MO ID Flag | ANQP Domain ID Flag | reserved | Release Number |
         */
        ie.bytes = new byte[] { (byte) 0x50, (byte) 0x6F, (byte) 0x9A, (byte) 0x10,
                                (byte) 0x14 /* Hotspot Configuration */, (byte) 0x11, (byte) 0x22};
        InformationElementUtil.Vsa vsa =
                InformationElementUtil.getHS2VendorSpecificIE(new InformationElement[] {ie});
        assertEquals(NetworkDetail.HSRelease.R2, vsa.hsRelease);
        assertEquals(0x2211, vsa.anqpDomainID);
    }

    /**
     * Verify that the expected Interworking information element is parsed and retrieved from the
     * list of IEs. Uses an IE w/o the optional Venue Info.
     *
     * @throws Exception
     */
    @Test
    public void getInterworkingElementNoVenueIE() throws Exception {
        InformationElement ie = new InformationElement();
        ie.id = InformationElement.EID_INTERWORKING;
        /**
         * Interworking Format:
         * | Access Network Option | Venue Info (optional) | HESSID (optional) |
         *           1                       2                     6
         *
         * Access Network Option Format:
         *
         * B0                   B3    B4       B5    B6     B7
         * | Access Network Type | Internet | ASRA | ESR | UESA |
         */
        ie.bytes = new byte[] { (byte) 0x10, (byte) 0x11, (byte) 0x22, (byte) 0x33, (byte) 0x44,
                                (byte) 0x55, (byte) 0x66 };
        InformationElementUtil.Interworking interworking =
                InformationElementUtil.getInterworkingIE(new InformationElement[] {ie});
        assertTrue(interworking.internet);
        assertEquals(NetworkDetail.Ant.Private, interworking.ant);
        assertEquals(0x112233445566L, interworking.hessid);
    }

    /**
     * Verify that the expected Interworking information element is parsed and retrieved from the
     * list of IEs. Uses an IE with the optional Venue Info.
     *
     * @throws Exception
     */
    @Test
    public void getInterworkingElementWithVenueIE() throws Exception {
        InformationElement ie = new InformationElement();
        ie.id = InformationElement.EID_INTERWORKING;
        /**
         * Interworking Format:
         * | Access Network Option | Venue Info (optional) | HESSID (optional) |
         *           1                       2                     6
         *
         * Access Network Option Format:
         *
         * B0                   B3    B4       B5    B6     B7
         * | Access Network Type | Internet | ASRA | ESR | UESA |
         */
        ie.bytes = new byte[]{(byte) 0x10, (byte) 0xAA, (byte) 0xBB, (byte) 0x11, (byte) 0x22,
                (byte) 0x33, (byte) 0x44, (byte) 0x55, (byte) 0x66};
        InformationElementUtil.Interworking interworking =
                InformationElementUtil.getInterworkingIE(new InformationElement[] {ie});
        assertTrue(interworking.internet);
        assertEquals(NetworkDetail.Ant.Private, interworking.ant);
        assertEquals(0x112233445566L, interworking.hessid);
    }

    // TODO: SAE, OWN, SUITE_B
}
