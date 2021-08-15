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

import static org.junit.Assert.assertEquals;

import androidx.test.filters.SmallTest;

import com.android.server.wifi.WifiLoggerHal;

import org.junit.Test;

/**
 * Unit tests for {@link com.android.server.wifi.util.FrameParser}.
 */
@SmallTest
public class FrameParserTest {

    private static final byte[] TEST_EAPOL_1_OF_4_FRAME_BYTES = new byte[] {
            (byte) 0x7C, (byte) 0x7D, (byte) 0x3D, (byte) 0x51, (byte) 0x10, (byte) 0xDC,
            (byte) 0x08, (byte) 0x96, (byte) 0xD7, (byte) 0x8B, (byte) 0xE3, (byte) 0xFB,
            (byte) 0x88, (byte) 0x8E, (byte) 0x02, (byte) 0x03, (byte) 0x00, (byte) 0x5F,
            (byte) 0x02, (byte) 0x00, (byte) 0x8A, (byte) 0x00, (byte) 0x10, (byte) 0x00,
            (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00,
            (byte) 0x01, (byte) 0xF2, (byte) 0x0D, (byte) 0xFA, (byte) 0x35, (byte) 0x89,
            (byte) 0x9C, (byte) 0xA8, (byte) 0x8C, (byte) 0x14, (byte) 0xD9, (byte) 0x3F,
            (byte) 0xC9, (byte) 0x62, (byte) 0x11, (byte) 0x39, (byte) 0xC3, (byte) 0x34,
            (byte) 0xE1, (byte) 0x00, (byte) 0x09, (byte) 0xE3, (byte) 0x9C, (byte) 0x0C,
            (byte) 0x32, (byte) 0xFE, (byte) 0x7F, (byte) 0x79, (byte) 0x29, (byte) 0x3E,
            (byte) 0x6C, (byte) 0xF2, (byte) 0x57, (byte) 0x00, (byte) 0x00, (byte) 0x00,
            (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00,
            (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00,
            (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00,
            (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00,
            (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00,
            (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00,
            (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00,
            (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00};
    private static final byte TEST_EAPOL_1_OF_4_FRAME_TYPE =
            WifiLoggerHal.FRAME_TYPE_ETHERNET_II;
    private static final String TEST_EAPOL_1_OF_4_FRAME_PROTOCOL_STRING = "EAPOL";
    private static final String TEST_EAPOL_1_OF_4_FRAME_TYPE_STRING = "Pairwise Key message 1/4";

    private static final byte[] TEST_EAPOL_2_OF_4_FRAME_BYTES = new byte[] {
            (byte) 0x08, (byte) 0x96, (byte) 0xD7, (byte) 0x8B, (byte) 0xE3, (byte) 0xFB,
            (byte) 0x7C, (byte) 0x7D, (byte) 0x3D, (byte) 0x51, (byte) 0x10, (byte) 0xDC,
            (byte) 0x88, (byte) 0x8E, (byte) 0x01, (byte) 0x03, (byte) 0x00, (byte) 0x75,
            (byte) 0x02, (byte) 0x01, (byte) 0x0A, (byte) 0x00, (byte) 0x00, (byte) 0x00,
            (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00,
            (byte) 0x01, (byte) 0x42, (byte) 0xAE, (byte) 0x33, (byte) 0x61, (byte) 0xF8,
            (byte) 0x38, (byte) 0x28, (byte) 0x81, (byte) 0x70, (byte) 0xD8, (byte) 0xA5,
            (byte) 0xDD, (byte) 0x90, (byte) 0xB1, (byte) 0x8E, (byte) 0xEB, (byte) 0x58,
            (byte) 0xF1, (byte) 0x0A, (byte) 0xE4, (byte) 0xA1, (byte) 0x93, (byte) 0x34,
            (byte) 0xE1, (byte) 0x4F, (byte) 0x90, (byte) 0x85, (byte) 0xA0, (byte) 0x21,
            (byte) 0x95, (byte) 0xC8, (byte) 0xD8, (byte) 0x00, (byte) 0x00, (byte) 0x00,
            (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00,
            (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00,
            (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00,
            (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00,
            (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x00, (byte) 0x53,
            (byte) 0x12, (byte) 0xE4, (byte) 0x97, (byte) 0xBC, (byte) 0xFF, (byte) 0x15,
            (byte) 0x45, (byte) 0xCF, (byte) 0x4C, (byte) 0xC0, (byte) 0xB8, (byte) 0xBA,
            (byte) 0x30, (byte) 0x5F, (byte) 0x6C, (byte) 0x00, (byte) 0x16, (byte) 0x30,
            (byte) 0x14, (byte) 0x01, (byte) 0x00, (byte) 0x00, (byte) 0x0F, (byte) 0xAC,
            (byte) 0x04, (byte) 0x01, (byte) 0x00, (byte) 0x00, (byte) 0x0F, (byte) 0xAC,
            (byte) 0x04, (byte) 0x01, (byte) 0x00, (byte) 0x00, (byte) 0x0F, (byte) 0xAC,
            (byte) 0x02, (byte) 0x80, (byte) 0x00};
    private static final byte TEST_EAPOL_2_OF_4_FRAME_TYPE =
            WifiLoggerHal.FRAME_TYPE_ETHERNET_II;
    private static final String TEST_EAPOL_2_OF_4_FRAME_PROTOCOL_STRING = "EAPOL";
    private static final String TEST_EAPOL_2_OF_4_FRAME_TYPE_STRING = "Pairwise Key message 2/4";

    private static final byte[] TEST_PROBE_RESPONSE_FRAME_BYTES = new byte[] {
            (byte) 0x50, (byte) 0x08, (byte) 0x3a, (byte) 0x01, (byte) 0x54, (byte) 0x27,
            (byte) 0x1e, (byte) 0xf2, (byte) 0xcd, (byte) 0x0f, (byte) 0x10, (byte) 0x6f,
            (byte) 0x3f, (byte) 0xf6, (byte) 0x89, (byte) 0x0e, (byte) 0x10, (byte) 0x6f,
            (byte) 0x3f, (byte) 0xf6, (byte) 0x89, (byte) 0x0e, (byte) 0x60, (byte) 0xc4,
            (byte) 0x4a, (byte) 0xaa, (byte) 0x2a, (byte) 0x0d, (byte) 0x00, (byte) 0x00,
            (byte) 0x00, (byte) 0x00, (byte) 0x64, (byte) 0x00, (byte) 0x11, (byte) 0x04,
            (byte) 0x00, (byte) 0x16, (byte) 0x77, (byte) 0x7a, (byte) 0x72, (byte) 0x5f,
            (byte) 0x68, (byte) 0x70, (byte) 0x5f, (byte) 0x67, (byte) 0x34, (byte) 0x35,
            (byte) 0x30, (byte) 0x68, (byte) 0x5f, (byte) 0x67, (byte) 0x5f, (byte) 0x63,
            (byte) 0x68, (byte) 0x35, (byte) 0x5f, (byte) 0x77, (byte) 0x70, (byte) 0x61,
            (byte) 0x01, (byte) 0x08, (byte) 0x82, (byte) 0x84, (byte) 0x8b, (byte) 0x96,
            (byte) 0x0c, (byte) 0x12, (byte) 0x18, (byte) 0x24, (byte) 0x03, (byte) 0x01,
            (byte) 0x05, (byte) 0x2a, (byte) 0x01, (byte) 0x04, (byte) 0x32, (byte) 0x04,
            (byte) 0x30, (byte) 0x48, (byte) 0x60, (byte) 0x6c, (byte) 0x30, (byte) 0x18,
            (byte) 0x01, (byte) 0x00, (byte) 0x00, (byte) 0x0f, (byte) 0xac, (byte) 0x02,
            (byte) 0x02, (byte) 0x00, (byte) 0x00, (byte) 0x0f, (byte) 0xac, (byte) 0x04,
            (byte) 0x00, (byte) 0x0f, (byte) 0xac, (byte) 0x02, (byte) 0x01, (byte) 0x00,
            (byte) 0x00, (byte) 0x0f, (byte) 0xac, (byte) 0x02, (byte) 0x0c, (byte) 0x00,
            (byte) 0xdd, (byte) 0x18, (byte) 0x00, (byte) 0x50, (byte) 0xf2, (byte) 0x02,
            (byte) 0x01, (byte) 0x01, (byte) 0x00, (byte) 0x00, (byte) 0x03, (byte) 0xa4,
            (byte) 0x00, (byte) 0x00, (byte) 0x27, (byte) 0xa4, (byte) 0x00, (byte) 0x00,
            (byte) 0x42, (byte) 0x43, (byte) 0x5e, (byte) 0x00, (byte) 0x62, (byte) 0x32,
            (byte) 0x2f, (byte) 0x00, (byte) 0xdd, (byte) 0x70, (byte) 0x00, (byte) 0x50,
            (byte) 0xf2, (byte) 0x04, (byte) 0x10, (byte) 0x4a, (byte) 0x00, (byte) 0x01,
            (byte) 0x10, (byte) 0x10, (byte) 0x44, (byte) 0x00, (byte) 0x01, (byte) 0x02,
            (byte) 0x10, (byte) 0x3b, (byte) 0x00, (byte) 0x01, (byte) 0x03, (byte) 0x10,
            (byte) 0x47, (byte) 0x00, (byte) 0x10, (byte) 0xf3, (byte) 0x1b, (byte) 0x31,
            (byte) 0x7f, (byte) 0x8c, (byte) 0x25, (byte) 0x56, (byte) 0x46, (byte) 0xb5,
            (byte) 0xc9, (byte) 0x5f, (byte) 0x2f, (byte) 0x0b, (byte) 0xcf, (byte) 0x6a,
            (byte) 0x14, (byte) 0x10, (byte) 0x21, (byte) 0x00, (byte) 0x06, (byte) 0x44,
            (byte) 0x44, (byte) 0x2d, (byte) 0x57, (byte) 0x52, (byte) 0x54, (byte) 0x10,
            (byte) 0x23, (byte) 0x00, (byte) 0x0c, (byte) 0x57, (byte) 0x5a, (byte) 0x52,
            (byte) 0x2d, (byte) 0x48, (byte) 0x50, (byte) 0x2d, (byte) 0x47, (byte) 0x34,
            (byte) 0x35, (byte) 0x30, (byte) 0x48, (byte) 0x10, (byte) 0x24, (byte) 0x00,
            (byte) 0x01, (byte) 0x30, (byte) 0x10, (byte) 0x42, (byte) 0x00, (byte) 0x05,
            (byte) 0x31, (byte) 0x32, (byte) 0x33, (byte) 0x34, (byte) 0x35, (byte) 0x10,
            (byte) 0x54, (byte) 0x00, (byte) 0x08, (byte) 0x00, (byte) 0x06, (byte) 0x00,
            (byte) 0x50, (byte) 0xf2, (byte) 0x04, (byte) 0x00, (byte) 0x01, (byte) 0x10,
            (byte) 0x11, (byte) 0x00, (byte) 0x06, (byte) 0x44, (byte) 0x44, (byte) 0x2d,
            (byte) 0x57, (byte) 0x52, (byte) 0x54, (byte) 0x10, (byte) 0x08, (byte) 0x00,
            (byte) 0x02, (byte) 0x01, (byte) 0x04, (byte) 0x10, (byte) 0x3c, (byte) 0x00,
            (byte) 0x01, (byte) 0x01, (byte) 0x8a, (byte) 0x0e, (byte) 0x06, (byte) 0xcf};
    private static final byte TEST_PROBE_RESPONSE_FRAME_TYPE = WifiLoggerHal.FRAME_TYPE_80211_MGMT;
    private static final String TEST_PROBE_RESPONSE_FRAME_PROTOCOL_STRING = "802.11 Mgmt";
    private static final String TEST_PROBE_RESPONSE_FRAME_TYPE_STRING = "Probe Response";

    /**
     * Test that a probe response frame is parsed correctly.
     */
    @Test
    public void parseProbeResponseFrame() {
        FrameParser parser = new FrameParser(
                TEST_PROBE_RESPONSE_FRAME_TYPE, TEST_PROBE_RESPONSE_FRAME_BYTES);
        assertEquals(TEST_PROBE_RESPONSE_FRAME_PROTOCOL_STRING, parser.mMostSpecificProtocolString);
        assertEquals(TEST_PROBE_RESPONSE_FRAME_TYPE_STRING, parser.mTypeString);
    }

    /**
     * Test that pairwise EAPOL 1/4 and 2/4 frames are parsed correctly.
     */
    @Test
    public void parseEapolFrames() {
        FrameParser parser1 = new FrameParser(
                TEST_EAPOL_1_OF_4_FRAME_TYPE, TEST_EAPOL_1_OF_4_FRAME_BYTES);
        assertEquals(TEST_EAPOL_1_OF_4_FRAME_PROTOCOL_STRING, parser1.mMostSpecificProtocolString);
        assertEquals(TEST_EAPOL_1_OF_4_FRAME_TYPE_STRING, parser1.mTypeString);

        FrameParser parser2 = new FrameParser(
                TEST_EAPOL_2_OF_4_FRAME_TYPE, TEST_EAPOL_2_OF_4_FRAME_BYTES);
        assertEquals(TEST_EAPOL_2_OF_4_FRAME_PROTOCOL_STRING, parser2.mMostSpecificProtocolString);
        assertEquals(TEST_EAPOL_2_OF_4_FRAME_TYPE_STRING, parser2.mTypeString);
    }

    /** Test that we parse the status code out of a non-HT authentication reply. */
    @Test
    public void canParseStatusCodeOutOfNonHtOpenAuthenticationReply() {
        FrameParser parser = new FrameParser(
                WifiLoggerHal.FRAME_TYPE_80211_MGMT,
                new byte[] {
                        (byte) 0xb0,  // type + subtype
                        0x00,  // flags
                        0x3c, 0x00,  // duration
                        0x00, 0x01, 0x02, 0x03, 0x04, 0x05,  // addr1 -- RA
                        0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b,  // addr2 -- TA
                        0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b,  // addr3 -- BSSID
                        (byte) 0xa0, 0x3b,  // sequence + control
                        0x00, 0x00,  // auth algorithm
                        0x02, 0x00,  // auth sequence num
                        0x11, 0x00  // status code
                });
        assertEquals("802.11 Mgmt", parser.mMostSpecificProtocolString);
        assertEquals("Authentication", parser.mTypeString);
        assertEquals("17: Association denied; too many STAs", parser.mResultString);
    }

    /** Test that we parse the status code out of a non-HT association response. */
    @Test
    public void canParseStatusCodeOutOfNonHtAssociationResponse() {
        FrameParser parser = new FrameParser(
                WifiLoggerHal.FRAME_TYPE_80211_MGMT,
                new byte[] {
                        0x10,  // type + subtype
                        0x00,  // flags
                        0x3c, 0x00,  // duration
                        0x00, 0x01, 0x02, 0x03, 0x04, 0x05,  // addr1 (RA)
                        0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b,  // addr2 (TA)
                        0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b,  // addr3 (BSSID)
                        (byte) 0xb0, (byte) 0xbc,   // sequence + control
                        0x01, 0x01,  // capabilities
                        0x00, 0x00,  // status code
                        0x01, (byte) 0xc0, 0x01, 0x08, (byte) 0x8c, 0x12, (byte) 0x98, 0x24,
                        (byte) 0xb0, 0x48, 0x60, 0x6c, 0x2d, 0x1a, 0x6e, 0x00, 0x1b, (byte) 0xff,
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01,
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3d, 0x16,
                        0x24, 0x05, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, (byte) 0xbf,
                        0x0c, 0x00, 0x00, 0x00, 0x00, (byte) 0xfe, (byte) 0xff, 0x00, 0x00,
                        (byte) 0xfe, (byte) 0xff, 0x00, 0x00, (byte) 0xc0, 0x05, 0x00, 0x00, 0x00,
                        (byte) 0xfc, (byte) 0xff, 0x7f, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                        0x00, 0x40, (byte) 0xdd, 0x18, 0x00, 0x50, (byte) 0xf2, 0x02, 0x01, 0x01,
                        0x00, 0x00, 0x03, (byte) 0xa4, 0x00, 0x00, 0x27, (byte) 0xa4, 0x00, 0x00,
                        0x42, 0x43, 0x5e, 0x00, 0x62, 0x32, 0x2f, 0x00
                });
        assertEquals("802.11 Mgmt", parser.mMostSpecificProtocolString);
        assertEquals("Association Response", parser.mTypeString);
        assertEquals("0: Success", parser.mResultString);
    }

    /** Test that we do not parse the status code out of a non-HT authentication request. */
    @Test
    public void doesNotParseStatusCodeOutOfNonHtOpenAuthenticationRequest() {
        FrameParser parser = new FrameParser(
                WifiLoggerHal.FRAME_TYPE_80211_MGMT,
                new byte[] {
                        (byte) 0xb0,  // type + subtype
                        0x00,  // flags
                        0x00, 0x00,  // duration (from host; probably to be filled by firmware)
                        0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b,  // addr1 (RA)
                        0x00, 0x01, 0x02, 0x03, 0x04, 0x05,  // addr2 (TA)
                        0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b,  // addr3 (BSSID)
                        0x10, (byte) 0x80,  // sequence + control
                        0x00, 0x00,  // auth algorithm
                        0x01, 0x00,  // auth sequence num
                        0x00, 0x00,  // status code; present but meaningless for first auth frame

                });
        assertEquals("802.11 Mgmt", parser.mMostSpecificProtocolString);
        assertEquals("Authentication", parser.mTypeString);
        assertEquals("N/A", parser.mResultString);
    }

    /** Test that we do not parse the status code out of an association request. */
    @Test
    public void doesNotParseStatusCodeOutOfNonHtOpenAssociationRequest() {
        FrameParser parser = new FrameParser(
                WifiLoggerHal.FRAME_TYPE_80211_MGMT,
                new byte[] {
                        0x00,  // type + subtype
                        0x00,  // flags
                        0x00, 0x00,  // duration (from host; probably to be filled by firmware)
                        0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b,  // addr1 (RA)
                        0x00, 0x01, 0x02, 0x03, 0x04, 0x05,  // addr2 (TA)
                        0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b,  // addr3 (BSSID)
                        0x20, (byte) 0x80,  // sequence + control
                        0x01, 0x11, 0x01, 0x00, 0x00, 0x0b, 0x71, 0x75,
                        0x69, 0x63, 0x68, 0x65, 0x2d, 0x74, 0x65, 0x73, 0x74, 0x01, 0x08,
                        (byte) 0x8c, 0x12, (byte) 0x98, 0x24, (byte) 0xb0, 0x48, 0x60, 0x6c, 0x21,
                        0x02, 0x08, 0x18, 0x24, 0x32, 0x24, 0x01, 0x28, 0x01, 0x2c, 0x01, 0x30,
                        0x01, 0x34, 0x01, 0x38, 0x01, 0x3c, 0x01, 0x40, 0x01, 0x64, 0x01, 0x68,
                        0x01, 0x6c, 0x01, 0x70, 0x01, 0x74, 0x01, 0x78, 0x01, 0x7c, 0x01,
                        (byte) 0x80, 0x01, (byte) 0x84, 0x01, (byte) 0x88, 0x01, (byte) 0x8c, 0x01,
                        (byte) 0x90, 0x01, (byte) 0x95, 0x01, (byte) 0x99, 0x01, (byte) 0x9d, 0x01,
                        (byte) 0xa1, 0x01, (byte) 0xa5, 0x01, 0x2d, 0x1a, (byte) 0xef, 0x01, 0x1f,
                        (byte) 0xff, (byte) 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                        0x00, (byte) 0xdd, 0x07, 0x00, 0x50, (byte) 0xf2, 0x02, 0x00, 0x01, 0x00,
                        (byte) 0xbf, 0x0c, (byte) 0x92, 0x71, (byte) 0x90, 0x33, (byte) 0xfa,
                        (byte) 0xff, 0x0c, 0x03, (byte) 0xfa, (byte) 0xff, 0x0c, 0x03, 0x7f, 0x09,
                        0x04, 0x00, 0x0a, 0x02, 0x01, 0x00, 0x00, 0x40, (byte) 0x80,
                });
        assertEquals("802.11 Mgmt", parser.mMostSpecificProtocolString);
        assertEquals("Association Request", parser.mTypeString);
        assertEquals("N/A", parser.mResultString);
    }

    /** Test that we parse the result code of a deauthentication frame */
    @Test
    public void parseDeauthenticationResultCode() {
        FrameParser parser = new FrameParser(
                WifiLoggerHal.FRAME_TYPE_80211_MGMT,
                new byte[]{
                        (byte) 0xc0,  // type + subtype
                        0x00,  // flags
                        0x00, 0x00,  // duration (from host; probably to be filled by firmware)
                        // addr1 (RA):
                        (byte) 0xa0, 0x63, (byte) 0x91, (byte) 0xa9, (byte) 0xed, (byte) 0xa1,
                        // addr2 (TA):
                        (byte) 0xf4, (byte) 0xf5, (byte) 0xe8, 0x51, (byte) 0x9e, 0x09,
                        // addr3 (BSSID):
                        (byte) 0xa0, 0x63, (byte) 0x91, (byte) 0xa9, (byte) 0xed, (byte) 0xa1,
                        0x70, (byte) 0x80,  // sequence + control
                        0x03, 0x00,  // reason code
                });
        assertEquals("802.11 Mgmt", parser.mMostSpecificProtocolString);
        assertEquals("Deauthentication", parser.mTypeString);
        assertEquals(
                "3: Deauthenticated because sending STA is leaving (or has left) IBSS or ESS",
                parser.mResultString);
    }

    /** Test that we parse the result code of a disassociation frame */
    @Test
    public void parseDisassociationResultCode() {
        FrameParser parser = new FrameParser(
                WifiLoggerHal.FRAME_TYPE_80211_MGMT,
                new byte[]{
                        (byte) 0xa0,  // type + subtype
                        0x00,  // flags
                        0x00, 0x00,  // duration (from host; probably to be filled by firmware)
                        // addr1 (RA):
                        (byte) 0xa0, 0x63, (byte) 0x91, (byte) 0xa9, (byte) 0xed, (byte) 0xa1,
                        // addr2 (TA):
                        (byte) 0xf4, (byte) 0xf5, (byte) 0xe8, 0x51, (byte) 0x9e, 0x09,
                        // addr3 (BSSID):
                        (byte) 0xa0, 0x63, (byte) 0x91, (byte) 0xa9, (byte) 0xed, (byte) 0xa1,
                        0x70, (byte) 0x80,  // sequence + control
                        0x04, 0x00,  // reason code
                });
        assertEquals("802.11 Mgmt", parser.mMostSpecificProtocolString);
        assertEquals("Disassociation", parser.mTypeString);
        assertEquals("4: Disassociated due to inactivity", parser.mResultString);
    }

    /** Test that we parse the subtype of an Action No Ack frame */
    @Test
    public void parseActionNoAckSubtype() {
        FrameParser parser = new FrameParser(
                WifiLoggerHal.FRAME_TYPE_80211_MGMT,
                new byte[]{
                        (byte) 0xe0,  // type + subtype
                        0x00,  // flags
                        0x00, 0x00,  // duration (from host; probably to be filled by firmware)
                        // addr1 (RA):
                        (byte) 0xa0, 0x63, (byte) 0x91, (byte) 0xa9, (byte) 0xed, (byte) 0xa1,
                        // addr2 (TA):
                        (byte) 0xf4, (byte) 0xf5, (byte) 0xe8, 0x51, (byte) 0x9e, 0x09,
                        // addr3 (BSSID):
                        (byte) 0xa0, 0x63, (byte) 0x91, (byte) 0xa9, (byte) 0xed, (byte) 0xa1,
                        0x70, (byte) 0x80,  // sequence + control
                        0x00, 0x00,  // action
                });
        assertEquals("802.11 Mgmt", parser.mMostSpecificProtocolString);
        assertEquals("Action No Ack", parser.mTypeString);
        assertEquals("N/A", parser.mResultString);
    }
}