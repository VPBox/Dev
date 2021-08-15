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

package com.android.server.wifi.hotspot2.anqp;

import static org.junit.Assert.assertEquals;

import android.net.wifi.WifiSsid;

import androidx.test.filters.SmallTest;

import org.junit.Test;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.net.ProtocolException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Locale;

/**
 * Unit tests for {@link com.android.server.wifi.hotspot2.anqp.ANQPParser}.
 */
@SmallTest
public class ANQPParserTest {
    /**
     * Helper function for generating payload for a Venue Name ANQP element.
     *
     * @param language Array of languages
     * @param text Array of text
     * @return byte[]
     * @throws IOException
     */
    private static byte[] getVenueNamePayload(String[] language, String[] text)
            throws IOException {
        ByteArrayOutputStream stream = new ByteArrayOutputStream();
        stream.write(new byte[VenueNameElement.VENUE_INFO_LENGTH]);
        stream.write(getI18NameListPayload(language, text));
        return stream.toByteArray();
    }

    /**
     * Helper function for generating payload for a Domain Name ANQP element.
     *
     * @param names Array of domain names
     * @return byte[]
     * @throws IOException
     */
    private static byte[] getDomainNamePayload(String[] names) throws IOException {
        ByteArrayOutputStream stream = new ByteArrayOutputStream();
        for (String name : names) {
            byte[] nameBytes = name.getBytes(StandardCharsets.ISO_8859_1);
            stream.write((byte) nameBytes.length);
            stream.write(nameBytes);
        }
        return stream.toByteArray();
    }

    /**
     * Helper function for generating payload for a Roaming Consortium ANQP element.
     *
     * @param ois Array of OIs
     * @param oisLength Array of length of each corresponding OI
     * @return byte[]
     * @throws IOException
     */
    private static byte[] getRoamingConsortiumPayload(Long[] ois, int[] oisLength)
            throws IOException {
        ByteArrayOutputStream stream = new ByteArrayOutputStream();
        for (int i = 0; i < ois.length; i++) {
            stream.write((byte) oisLength[i]);
            // Write the OI data in big-endian.
            for (int l = oisLength[i] - 1; l >= 0; l--) {
                stream.write((byte) ((ois[i].longValue() >> l * Byte.SIZE) & 0xFF));
            }
        }
        return stream.toByteArray();
    }

    /**
     * Helper function for generating payload for a NAI Realm ANQP element.
     *
     * @param realmDataList Array of realm data.
     * @return byte[]
     * @throws IOException
     */
    private static byte[] getNAIRealmPayload(byte[][] realmDataList) throws IOException {
        ByteArrayOutputStream stream = new ByteArrayOutputStream();
        // Data count in little-endian
        stream.write((byte) (realmDataList.length & 0xFF));
        stream.write((byte) ((realmDataList.length >> 8) & 0xFF));
        for (byte[] realmData : realmDataList) {
            stream.write(realmData);
        }
        return stream.toByteArray();
    }

    /**
     * Helper function for generating payload for 3GPP Network ANQP element.
     *
     * @param ieiList Array of IEI data
     * @return byte[]
     * @throws IOException
     */
    private static byte[] getThreeGPPNetworkPayload(byte[][] ieiList) throws IOException {
        ByteArrayOutputStream stream = new ByteArrayOutputStream();
        int totalIeiSize = CellularNetworkTestUtil.getDataSize(ieiList);
        stream.write((byte) ThreeGPPNetworkElement.GUD_VERSION_1);
        stream.write((byte) totalIeiSize);
        for (byte[] iei : ieiList) {
            stream.write(iei);
        }
        return stream.toByteArray();
    }

    /**
     * Helper function for generating payload for Vendor Specific ANQP element.
     *
     * @param oi The OI of the vendor
     * @param type The type of the element
     * @param subtype The subtype of the element
     * @param payload The vendor specific data
     * @return byte[]
     * @throws IOException
     */
    private static byte[] getVendorSpecificPayload(int oi, int type, int subtype, byte[] payload)
            throws IOException {
        ByteArrayOutputStream stream = new ByteArrayOutputStream();
        stream.write((byte) ((oi >> 16) & 0xFF));
        stream.write((byte) ((oi >> 8) & 0xFF));
        stream.write((byte) (oi & 0xFF));
        stream.write((byte) type);
        stream.write((byte) subtype);
        stream.write((byte) 0);    // Reserved
        stream.write(payload);
        return stream.toByteArray();
    }

    /**
     * Helper function for generating payload for a Hotspot 2.0 Operator Friendly Name ANQP element.
     *
     * @param language Array of language
     * @param text Array of text
     * @return byte[]
     * @throws IOException
     */
    private static byte[] getHSFriendlyNamePayload(String[] language, String[] text)
            throws IOException {
        return getI18NameListPayload(language, text);
    }

    /**
     * Helper function for generating payload for a Hotspot 2.0 WAN Metrics ANQP element.
     *
     * @param status Link status
     * @param symmetric Flag indicating symmetric link speed
     * @param capped Flag indicating link operating at max capacity
     * @param downlinkSpeed Downlink speed
     * @param uplinkSpeed Uplink speed
     * @param downlinkLoad Downlink load
     * @param uplinkLoad Uplink load
     * @param lmd Load measurement duration
     * @return byte[]
     */
    private static byte[] getHSWanMetricsPayload(int status, boolean symmetric, boolean capped,
            long downlinkSpeed, long uplinkSpeed, int downlinkLoad, int uplinkLoad, int lmd) {
        ByteBuffer buffer = ByteBuffer.allocate(HSWanMetricsElement.EXPECTED_BUFFER_SIZE)
                .order(ByteOrder.LITTLE_ENDIAN);
        int wanInfo = status & HSWanMetricsElement.LINK_STATUS_MASK;
        if (symmetric) wanInfo |= HSWanMetricsElement.SYMMETRIC_LINK_MASK;
        if (capped) wanInfo |= HSWanMetricsElement.AT_CAPACITY_MASK;
        buffer.put((byte) wanInfo);
        buffer.putInt((int) (downlinkSpeed & 0xFFFFFFFFL));
        buffer.putInt((int) (uplinkSpeed & 0xFFFFFFFFL));
        buffer.put((byte) (downlinkLoad & 0xFF));
        buffer.put((byte) (uplinkLoad & 0xFF));
        buffer.putShort((short) (lmd & 0xFFFF));
        buffer.position(0);
        byte[] data = new byte[HSWanMetricsElement.EXPECTED_BUFFER_SIZE];
        buffer.get(data);
        return data;
    }

    /**
     * Helper function for generating payload for a Hotspot 2.0 Connection Capability ANQP
     * element.
     *
     * @param protocol Network protocol
     * @param port Network port
     * @param status Status of the port
     * @return byte[]
     */
    private static byte[] getHSConnectionCapabilityPayload(int protocol, int port, int status) {
        ByteArrayOutputStream stream = new ByteArrayOutputStream();
        stream.write((byte) protocol);
        // Write 2-byte port in little-endian.
        stream.write((byte) (port & 0xFF));
        stream.write((byte) ((port >> 8) & 0xFF));
        stream.write((byte) status);
        return stream.toByteArray();
    }

    /**
     * Helper function for generating payload for a Hotspot 2.0 OSU Providers List ANQP
     * element.
     *
     * @param osuSsidBytes Bytes of OSU SSID
     * @return byte[]
     */
    private static byte[] getHSOsuProvidersPayload(byte[] osuSsidBytes) throws IOException {
        ByteArrayOutputStream out = new ByteArrayOutputStream();
        out.write((byte) osuSsidBytes.length);
        out.write(osuSsidBytes);
        out.write((byte) 1);
        out.write(OsuProviderInfoTestUtil.TEST_OSU_PROVIDER_INFO_RAW_BYTES);
        return out.toByteArray();
    }

    /**
     * Helper function for generating payload for a list of I18Name.
     *
     * @param language Array of language
     * @param text Array of text
     * @return byte[]
     * @throws IOException
     */
    private static byte[] getI18NameListPayload(String[] language, String[] text)
            throws IOException {
        ByteArrayOutputStream stream = new ByteArrayOutputStream();
        for (int i = 0; i < language.length; i++) {
            byte[] textBytes = text[i].getBytes(StandardCharsets.UTF_8);
            int length = I18Name.LANGUAGE_CODE_LENGTH + text[i].length();
            stream.write((byte) length);
            stream.write(language[i].getBytes(StandardCharsets.US_ASCII));
            // Add padding for two-character language code.
            if (language[i].getBytes(StandardCharsets.US_ASCII).length
                    < I18Name.LANGUAGE_CODE_LENGTH) {
                stream.write(new byte[]{(byte) 0x0});
            }
            stream.write(textBytes);
        }
        return stream.toByteArray();
    }

    /**
     * Verify that an expected VenueNameElement will be returned when parsing a buffer that
     * contained a Venue Name ANQP element.
     *
     * @throws Exception
     */
    @Test
    public void parseVenueNameElement() throws Exception {
        // Test data.
        String[] language = new String[] {"en"};
        String[] text = new String[] {"test"};

        // Setup expectation.
        List<I18Name> nameList = new ArrayList<>();
        nameList.add(new I18Name(language[0], Locale.forLanguageTag(language[0]), text[0]));
        VenueNameElement expected = new VenueNameElement(nameList);

        ByteBuffer buffer = ByteBuffer.wrap(getVenueNamePayload(language, text));
        assertEquals(expected,
                ANQPParser.parseElement(Constants.ANQPElementType.ANQPVenueName, buffer));
    }

    /**
     * Verify that an expected IPAddressTypeAvailabilityElement will be returned when parsing a
     * buffer that contained an IP Address Type Availability ANQP element.
     *
     * @throws Exception
     */
    @Test
    public void parseIPAddressTypeAvailabilityElement() throws Exception {
        // Test data.
        int ipAddressAvailability = IPAddressTypeAvailabilityElement.IPV4_PUBLIC << 2
                | IPAddressTypeAvailabilityElement.IPV6_AVAILABLE;

        // Setup expectation.
        IPAddressTypeAvailabilityElement expected = new IPAddressTypeAvailabilityElement(
                IPAddressTypeAvailabilityElement.IPV4_PUBLIC,
                IPAddressTypeAvailabilityElement.IPV6_AVAILABLE);

        ByteBuffer buffer = ByteBuffer.wrap(new byte[] {(byte) ipAddressAvailability});
        assertEquals(expected,
                ANQPParser.parseElement(Constants.ANQPElementType.ANQPIPAddrAvailability, buffer));
    }

    /**
     * Verify that an expected DomainNameElement will be returned when parsing a buffer that
     * contained a Domain Name ANQP element.
     *
     * @throws Exception
     */
    @Test
    public void parseDomainNameElement() throws Exception {
        String[] testNames = new String[] {"test.com", "abc.com"};
        DomainNameElement expected = new DomainNameElement(Arrays.asList(testNames));

        ByteBuffer buffer = ByteBuffer.wrap(getDomainNamePayload(testNames));
        assertEquals(expected,
                ANQPParser.parseElement(Constants.ANQPElementType.ANQPDomName, buffer));
    }

    /**
     * Verify that an expected RoamingConsortiumElement will be returned when parsing a buffer that
     * contained a Roaming Consortium ANQP element.
     *
     * @throws Exception
     */
    @Test
    public void parseRoamingConsortium() throws Exception {
        Long[] ois = new Long[] {0x12345678L, 0x5678L};
        int[] oisLength = new int[] {4, 2};
        RoamingConsortiumElement expected = new RoamingConsortiumElement(Arrays.asList(ois));

        ByteBuffer buffer = ByteBuffer.wrap(getRoamingConsortiumPayload(ois, oisLength));
        assertEquals(expected,
                ANQPParser.parseElement(Constants.ANQPElementType.ANQPRoamingConsortium, buffer));
    }

    /**
     * Verify that an expected NAIRealmElement will be returned when parsing a buffer that
     * contained a NAI Realm ANQP element.
     *
     * @throws Exception
     */
    @Test
    public void parseNAIRealmElement() throws Exception {
        byte[][] testBytes = new byte[][] {NAIRealmDataTestUtil.TEST_REAML_WITH_UTF8_DATA_BYTES};
        NAIRealmData[] realmDataList = new NAIRealmData[] {NAIRealmDataTestUtil.TEST_REALM_DATA};
        NAIRealmElement expected = new NAIRealmElement(Arrays.asList(realmDataList));

        ByteBuffer buffer = ByteBuffer.wrap(getNAIRealmPayload(testBytes));
        assertEquals(expected,
                ANQPParser.parseElement(Constants.ANQPElementType.ANQPNAIRealm, buffer));
    }

    /**
     * Verify that an expected ThreeGPPNetworkElement will be returned when parsing a buffer that
     * contained a 3GPP Network ANQP element.
     *
     * @throws Exception
     */
    @Test
    public void parseThreeGPPNetworkElement() throws Exception {
        byte[][] plmnBytes = new byte[][] {new byte[] {(byte) 0x87, 0x29, 0x10}};
        String[] plmnList = new String[] {"789012"};

        List<CellularNetwork> networkList = new ArrayList<>();
        networkList.add(new CellularNetwork(Arrays.asList(plmnList)));
        ThreeGPPNetworkElement expected = new ThreeGPPNetworkElement(networkList);

        ByteBuffer buffer = ByteBuffer.wrap(getThreeGPPNetworkPayload(
                new byte[][] {CellularNetworkTestUtil.formatPLMNListIEI(plmnBytes)}));
        assertEquals(expected,
                ANQPParser.parseElement(Constants.ANQPElementType.ANQP3GPPNetwork, buffer));
    }

    /**
     * Verify that ProtocolException will be thrown when parsing a buffer that contained a
     * vendor specific element that contained a non-Hotspot 2.0 ANQP-element.
     *
     * @throws Exception
     */
    @Test(expected = ProtocolException.class)
    public void parseNonHS20VendorSpecificElement() throws Exception {
        ByteBuffer buffer = ByteBuffer.wrap(
                getVendorSpecificPayload(0x123456, 0x12, 1, new byte[0]));
        ANQPParser.parseElement(Constants.ANQPElementType.ANQPVendorSpec, buffer);
    }

    /**
     * Verify that an expected HSFriendlyNameElement will be returned when parsing a buffer that
     * contained a vendor specific element that contained a Hotspot 2.0 friendly name
     * ANQP element.
     *
     * @throws Exception
     */
    @Test
    public void parseVendorSpecificElementWithHSFriendlyName() throws Exception {
        String[] language = new String[] {"en"};
        String[] text = new String[] {"test"};

        // Setup expectation.
        List<I18Name> nameList = new ArrayList<>();
        nameList.add(new I18Name(language[0], Locale.forLanguageTag(language[0]), text[0]));
        HSFriendlyNameElement expected = new HSFriendlyNameElement(nameList);

        byte[] hsFriendlyNameBytes = getHSFriendlyNamePayload(language, text);
        byte[] data = getVendorSpecificPayload(
                ANQPParser.VENDOR_SPECIFIC_HS20_OI, ANQPParser.VENDOR_SPECIFIC_HS20_TYPE,
                Constants.HS_FRIENDLY_NAME, hsFriendlyNameBytes);
        assertEquals(expected, ANQPParser.parseElement(
                Constants.ANQPElementType.ANQPVendorSpec, ByteBuffer.wrap(data)));
    }

    /**
     * Verify that an expected HSFriendlyNameElement will be returned when parsing a buffer that
     * contained a Hotspot 2.0 Friendly Name ANQP element.
     *
     * @throws Exception
     */
    @Test
    public void parseHSFrendlyNameElement() throws Exception {
        // Test data.
        String[] language = new String[] {"en"};
        String[] text = new String[] {"test"};

        // Setup expectation.
        List<I18Name> nameList = new ArrayList<>();
        nameList.add(new I18Name(language[0], Locale.forLanguageTag(language[0]), text[0]));
        HSFriendlyNameElement expected = new HSFriendlyNameElement(nameList);

        ByteBuffer buffer = ByteBuffer.wrap(getHSFriendlyNamePayload(language, text));
        assertEquals(expected,
                ANQPParser.parseHS20Element(Constants.ANQPElementType.HSFriendlyName, buffer));
    }

    /**
     * Verify that an expected HSWanMetricsElement will be returned when parsing a buffer that
     * contained a Hotspot 2.0 WAN Metrics ANQP element.
     *
     * @throws Exception
     */
    @Test
    public void parseHSWANMetricsElement() throws Exception {
        int status = HSWanMetricsElement.LINK_STATUS_UP;
        boolean symmetric = false;
        boolean capped = true;
        long downlinkSpeed = 0x12453L;
        long uplinkSpeed = 0x12423L;
        int downlinkLoad = 0x12;
        int uplinkLoad = 0x23;
        int lmd = 0x2321;

        HSWanMetricsElement expected = new HSWanMetricsElement(status, symmetric, capped,
                downlinkSpeed, uplinkSpeed, downlinkLoad, uplinkLoad, lmd);

        byte[] data = getHSWanMetricsPayload(status, symmetric, capped, downlinkSpeed,
                uplinkSpeed, downlinkLoad, uplinkLoad, lmd);
        ByteBuffer buffer = ByteBuffer.wrap(data);
        assertEquals(expected,
                ANQPParser.parseHS20Element(Constants.ANQPElementType.HSWANMetrics, buffer));
    }

    /**
     * Verify that an expected HSConnectionCapabilityElement will be returned when parsing a
     * buffer that contained a Hotspot 2.0 Connection Capability ANQP element.
     *
     * @throws Exception
     */
    @Test
    public void parseHSConnectionCapabilityElement() throws Exception {
        int protocol = 12;
        int port = 23;
        int status = ProtocolPortTuple.PROTO_STATUS_OPEN;

        List<ProtocolPortTuple> statusList = new ArrayList<>();
        statusList.add(new ProtocolPortTuple(protocol, port, status));
        HSConnectionCapabilityElement expected = new HSConnectionCapabilityElement(statusList);

        ByteBuffer buffer = ByteBuffer.wrap(
                getHSConnectionCapabilityPayload(protocol, port, status));
        assertEquals(expected,
                ANQPParser.parseHS20Element(Constants.ANQPElementType.HSConnCapability, buffer));
    }

    /**
     * Verify that an expected RawByteElement will be returned when parsing a buffer that
     * contained a Hotspot 2.0 OSU Providers element.
     *
     * @throws Exception
     */
    @Test
    public void parseHSOUSProvidersElement() throws Exception {
        byte[] osuSsidBytes = "Test SSID".getBytes(StandardCharsets.UTF_8);
        byte[] data = getHSOsuProvidersPayload(osuSsidBytes);

        HSOsuProvidersElement expected = new HSOsuProvidersElement(
                WifiSsid.createFromByteArray(osuSsidBytes),
                Arrays.asList(OsuProviderInfoTestUtil.TEST_OSU_PROVIDER_INFO));

        ByteBuffer buffer = ByteBuffer.wrap(data);
        assertEquals(expected,
                ANQPParser.parseHS20Element(Constants.ANQPElementType.HSOSUProviders, buffer));
    }
}
