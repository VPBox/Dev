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

import static org.junit.Assert.assertEquals;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.when;

import android.net.wifi.ScanResult;

import androidx.test.filters.SmallTest;

import com.android.server.wifi.ScanDetail;
import com.android.server.wifi.hotspot2.anqp.ANQPElement;
import com.android.server.wifi.hotspot2.anqp.Constants;
import com.android.server.wifi.hotspot2.anqp.Constants.ANQPElementType;
import com.android.server.wifi.hotspot2.anqp.HSWanMetricsElement;
import com.android.server.wifi.hotspot2.anqp.IPAddressTypeAvailabilityElement;

import org.junit.Test;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * Unit tests for {@link com.android.server.wifi.hotspot2.PasspointNetworkScore}.
 */
@SmallTest
public class PasspointNetworkScoreTest {
    private static class TestData {
        public final boolean isHomeProvider;
        public final boolean isActiveNetwork;
        public final int rssiLevel;
        public final boolean internetAccess;
        public final NetworkDetail.Ant networkType;
        public final Map<ANQPElementType, ANQPElement> anqpElements;
        public int expectedScore;

        TestData(boolean homeProvider, boolean activeNetwork, int rssi, boolean internet,
                NetworkDetail.Ant type, Map<ANQPElementType, ANQPElement> elements,
                int score) {
            isHomeProvider = homeProvider;
            isActiveNetwork = activeNetwork;
            rssiLevel = rssi;
            internetAccess = internet;
            networkType = type;
            anqpElements = elements;
            expectedScore = score;
        }
    }

    private static final HSWanMetricsElement WAN_PORT_DOWN_ELEMENT = new HSWanMetricsElement(
            HSWanMetricsElement.LINK_STATUS_DOWN /* status */, true /* symmetric */,
            false /* capped */, 1233 /* downlinkSpeed */, 1233 /* uplinkSpeed */,
            10 /* downlinkLoad */, 10 /* uplinkLoad */, 12 /* lmd */);

    private static final HSWanMetricsElement WAN_PORT_UP_ELEMENT = new HSWanMetricsElement(
            HSWanMetricsElement.LINK_STATUS_UP /* status */, true /* symmetric */,
            false /* capped */, 1233 /* downlinkSpeed */, 1233 /* uplinkSpeed */,
            10 /* downlinkLoad */, 10 /* uplinkLoad */, 12 /* lmd */);

    private static final HSWanMetricsElement WAN_PORT_CAPPED_ELEMENT = new HSWanMetricsElement(
            HSWanMetricsElement.LINK_STATUS_UP /* status */, true /* symmetric */,
            true /* capped */, 1233 /* downlinkSpeed */, 1233 /* uplinkSpeed */,
            10 /* downlinkLoad */, 10 /* uplinkLoad */, 12 /* lmd */);

    private static final IPAddressTypeAvailabilityElement UNRESTRICTED_IP_ADDRESS_ELEMENT =
            new IPAddressTypeAvailabilityElement(IPAddressTypeAvailabilityElement.IPV4_PUBLIC,
                    IPAddressTypeAvailabilityElement.IPV6_AVAILABLE);

    private static final IPAddressTypeAvailabilityElement UNAVAILABLE_IP_ADDRESS_ELEMENT =
            new IPAddressTypeAvailabilityElement(
                    IPAddressTypeAvailabilityElement.IPV4_NOT_AVAILABLE,
                    IPAddressTypeAvailabilityElement.IPV6_NOT_AVAILABLE);

    private static final IPAddressTypeAvailabilityElement UNKNOWN_IP_ADDRESS_ELEMENT =
            new IPAddressTypeAvailabilityElement(
                    IPAddressTypeAvailabilityElement.IPV4_UNKNOWN,
                    IPAddressTypeAvailabilityElement.IPV6_UNKNOWN);

    private static final Map<ANQPElementType, ANQPElement> TEST_ANQP_WITH_WAN_PORT_DOWN =
            new HashMap<>();

    private static final Map<ANQPElementType, ANQPElement> TEST_ANQP_WITH_WAN_PORT_UP =
            new HashMap<>();

    private static final Map<ANQPElementType, ANQPElement> TEST_ANQP_WITH_WAN_PORT_CAPPED =
            new HashMap<>();

    private static final Map<ANQPElementType, ANQPElement> TEST_ANQP_WITH_UNRESTRICTED_IP =
            new HashMap<>();

    private static final Map<ANQPElementType, ANQPElement> TEST_ANQP_WITH_UNAVAILABLE_IP =
            new HashMap<>();

    private static final Map<ANQPElementType, ANQPElement> TEST_ANQP_WITH_UNKNOWN_IP =
            new HashMap<>();

    // List of test data.
    private static final List<TestData> TEST_DATA_LIST = new ArrayList<>();
    static {
        // Setup ANQP elements map for testing.
        TEST_ANQP_WITH_WAN_PORT_DOWN.put(Constants.ANQPElementType.HSWANMetrics,
                WAN_PORT_DOWN_ELEMENT);
        TEST_ANQP_WITH_WAN_PORT_UP.put(Constants.ANQPElementType.HSWANMetrics,
                WAN_PORT_UP_ELEMENT);
        TEST_ANQP_WITH_WAN_PORT_CAPPED.put(Constants.ANQPElementType.HSWANMetrics,
                WAN_PORT_CAPPED_ELEMENT);
        TEST_ANQP_WITH_UNRESTRICTED_IP.put(Constants.ANQPElementType.ANQPIPAddrAvailability,
                UNRESTRICTED_IP_ADDRESS_ELEMENT);
        TEST_ANQP_WITH_UNAVAILABLE_IP.put(Constants.ANQPElementType.ANQPIPAddrAvailability,
                UNAVAILABLE_IP_ADDRESS_ELEMENT);
        TEST_ANQP_WITH_UNKNOWN_IP.put(Constants.ANQPElementType.ANQPIPAddrAvailability,
                UNKNOWN_IP_ADDRESS_ELEMENT);

        // Home provider public network with Internet access that's not the current
        // active network.
        TEST_DATA_LIST.add(new TestData(true /* isHomeProvider */, false /* isActiveNetwork */,
                -60 /* rssiLevel */, true /* internetAccess */,
                NetworkDetail.Ant.FreePublic /* networkType */, null /* anqpElements */,
                /* expectedScore */
                PasspointNetworkScore.HOME_PROVIDER_AWARD
                + PasspointNetworkScore.INTERNET_ACCESS_AWARD
                + PasspointNetworkScore.PUBLIC_OR_PRIVATE_NETWORK_AWARDS
                + PasspointNetworkScore.RSSI_SCORE.lookupScore(-60, false)));

        // Home provider public network with Internet access that's the current active network.
        TEST_DATA_LIST.add(new TestData(true /* isHomeProvider */, true /* isActiveNetwork */,
                -60 /* rssiLevel */, true /* internetAccess */,
                NetworkDetail.Ant.FreePublic /* networkType */, null /* anqpElements */,
                /* expectedScore */
                PasspointNetworkScore.HOME_PROVIDER_AWARD
                + PasspointNetworkScore.INTERNET_ACCESS_AWARD
                + PasspointNetworkScore.PUBLIC_OR_PRIVATE_NETWORK_AWARDS
                + PasspointNetworkScore.RSSI_SCORE.lookupScore(-60, true)));

        // Home provider public network without Internet access that's not the current
        // active network.
        TEST_DATA_LIST.add(new TestData(true /* isHomeProvider */, false /* isActiveNetwork */,
                -60 /* rssiLevel */, false /* internetAccess */,
                NetworkDetail.Ant.FreePublic /* networkType */, null /* anqpElements */,
                /* expectedScore */
                PasspointNetworkScore.HOME_PROVIDER_AWARD
                - PasspointNetworkScore.INTERNET_ACCESS_AWARD
                + PasspointNetworkScore.PUBLIC_OR_PRIVATE_NETWORK_AWARDS
                + PasspointNetworkScore.RSSI_SCORE.lookupScore(-60, false)));

        // Home provider personal network with Internet access that's not the current active
        // network.
        TEST_DATA_LIST.add(new TestData(true /* isHomeProvider */, false /* isActiveNetwork */,
                -60 /* rssiLevel */, true /* internetAccess */,
                NetworkDetail.Ant.Personal /* networkType */, null /* anqpElements */,
                /* expectedScore */
                PasspointNetworkScore.HOME_PROVIDER_AWARD
                + PasspointNetworkScore.INTERNET_ACCESS_AWARD
                + PasspointNetworkScore.PERSONAL_OR_EMERGENCY_NETWORK_AWARDS
                + PasspointNetworkScore.RSSI_SCORE.lookupScore(-60, false)));

        // Home provider public network with Internet access that's not the current
        // active network, and ANPQ element indicating WAN port is up.
        TEST_DATA_LIST.add(new TestData(true /* isHomeProvider */, false /* isActiveNetwork */,
                -60 /* rssiLevel */, true /* internetAccess */,
                NetworkDetail.Ant.FreePublic /* networkType */,
                TEST_ANQP_WITH_WAN_PORT_UP /* anqpElements */,
                /* expectedScore */
                PasspointNetworkScore.HOME_PROVIDER_AWARD
                + PasspointNetworkScore.INTERNET_ACCESS_AWARD
                + PasspointNetworkScore.PUBLIC_OR_PRIVATE_NETWORK_AWARDS
                + PasspointNetworkScore.RSSI_SCORE.lookupScore(-60, false)));

        // Home provider public network with Internet access that's not the current
        // active network, and ANPQ element indicating WAN port is down.
        TEST_DATA_LIST.add(new TestData(true /* isHomeProvider */, false /* isActiveNetwork */,
                -60 /* rssiLevel */, true /* internetAccess */,
                NetworkDetail.Ant.FreePublic /* networkType */,
                TEST_ANQP_WITH_WAN_PORT_DOWN /* anqpElements */,
                /* expectedScore */
                PasspointNetworkScore.HOME_PROVIDER_AWARD
                + PasspointNetworkScore.INTERNET_ACCESS_AWARD
                + PasspointNetworkScore.PUBLIC_OR_PRIVATE_NETWORK_AWARDS
                + PasspointNetworkScore.RSSI_SCORE.lookupScore(-60, false)
                - PasspointNetworkScore.WAN_PORT_DOWN_OR_CAPPED_PENALTY));

        // Home provider public network with Internet access that's not the current
        // active network, and ANPQ element indicating WAN port is capped (max load reached).
        TEST_DATA_LIST.add(new TestData(true /* isHomeProvider */, false /* isActiveNetwork */,
                -60 /* rssiLevel */, true /* internetAccess */,
                NetworkDetail.Ant.FreePublic /* networkType */,
                TEST_ANQP_WITH_WAN_PORT_CAPPED /* anqpElements */,
                /* expectedScore */
                PasspointNetworkScore.HOME_PROVIDER_AWARD
                + PasspointNetworkScore.INTERNET_ACCESS_AWARD
                + PasspointNetworkScore.PUBLIC_OR_PRIVATE_NETWORK_AWARDS
                + PasspointNetworkScore.RSSI_SCORE.lookupScore(-60, false)
                - PasspointNetworkScore.WAN_PORT_DOWN_OR_CAPPED_PENALTY));

        // Home provider public network with Internet access that's not the current
        // active network, and ANPQ element indicating both IPv4 and IPv6 addresses are available.
        TEST_DATA_LIST.add(new TestData(true /* isHomeProvider */, false /* isActiveNetwork */,
                -60 /* rssiLevel */, true /* internetAccess */,
                NetworkDetail.Ant.FreePublic /* networkType */,
                TEST_ANQP_WITH_UNRESTRICTED_IP /* anqpElements */,
                /* expectedScore */
                PasspointNetworkScore.HOME_PROVIDER_AWARD
                + PasspointNetworkScore.INTERNET_ACCESS_AWARD
                + PasspointNetworkScore.PUBLIC_OR_PRIVATE_NETWORK_AWARDS
                + PasspointNetworkScore.RSSI_SCORE.lookupScore(-60, false)
                + PasspointNetworkScore.UNRESTRICTED_IP_AWARDS * 2 /* one for IPv4 and IPv6 */));

        // Home provider public network with Internet access that's not the current
        // active network, and ANPQ element indicating both IPv4 and IPv6 addresses are available.
        TEST_DATA_LIST.add(new TestData(true /* isHomeProvider */, false /* isActiveNetwork */,
                -60 /* rssiLevel */, true /* internetAccess */,
                NetworkDetail.Ant.FreePublic /* networkType */,
                TEST_ANQP_WITH_UNRESTRICTED_IP /* anqpElements */,
                /* expectedScore */
                PasspointNetworkScore.HOME_PROVIDER_AWARD
                + PasspointNetworkScore.INTERNET_ACCESS_AWARD
                + PasspointNetworkScore.PUBLIC_OR_PRIVATE_NETWORK_AWARDS
                + PasspointNetworkScore.RSSI_SCORE.lookupScore(-60, false)
                /* one each for IPv4 and IPv6. */
                + PasspointNetworkScore.UNRESTRICTED_IP_AWARDS * 2));

        // Home provider public network with Internet access that's not the current
        // active network, and ANPQ element indicating both IPv4 and IPv6 addresses are
        // unavailable.
        TEST_DATA_LIST.add(new TestData(true /* isHomeProvider */, false /* isActiveNetwork */,
                -60 /* rssiLevel */, true /* internetAccess */,
                NetworkDetail.Ant.FreePublic /* networkType */,
                TEST_ANQP_WITH_UNAVAILABLE_IP /* anqpElements */,
                /* expectedScore */
                PasspointNetworkScore.HOME_PROVIDER_AWARD
                + PasspointNetworkScore.INTERNET_ACCESS_AWARD
                + PasspointNetworkScore.PUBLIC_OR_PRIVATE_NETWORK_AWARDS
                + PasspointNetworkScore.RSSI_SCORE.lookupScore(-60, false)));

        // Home provider public network with Internet access that's not the current
        // active network, and ANPQ element indicating both IPv4 and IPv6 addresses are unknown.
        TEST_DATA_LIST.add(new TestData(true /* isHomeProvider */, false /* isActiveNetwork */,
                -60 /* rssiLevel */, true /* internetAccess */,
                NetworkDetail.Ant.FreePublic /* networkType */,
                TEST_ANQP_WITH_UNKNOWN_IP /* anqpElements */,
                /* expectedScore */
                PasspointNetworkScore.HOME_PROVIDER_AWARD
                + PasspointNetworkScore.INTERNET_ACCESS_AWARD
                + PasspointNetworkScore.PUBLIC_OR_PRIVATE_NETWORK_AWARDS
                + PasspointNetworkScore.RSSI_SCORE.lookupScore(-60, false)
                /* one each for IPv4 and IPv6. */
                + PasspointNetworkScore.RESTRICTED_OR_UNKNOWN_IP_AWARDS * 2));

        // Roaming provider public network with Internet access that's not the current active
        // network.
        TEST_DATA_LIST.add(new TestData(false /* isHomeProvider */, false /* isActiveNetwork */,
                -60 /* rssiLevel */, true /* internetAccess */,
                NetworkDetail.Ant.FreePublic /* networkType */, null /* anqpElements */,
                /* expectedScore */
                PasspointNetworkScore.INTERNET_ACCESS_AWARD
                + PasspointNetworkScore.PUBLIC_OR_PRIVATE_NETWORK_AWARDS
                + PasspointNetworkScore.RSSI_SCORE.lookupScore(-60, false)));

        // Roaming provider public network with Internet access that's the current active network.
        TEST_DATA_LIST.add(new TestData(false /* isHomeProvider */, true /* isActiveNetwork */,
                -60 /* rssiLevel */, true /* internetAccess */,
                NetworkDetail.Ant.FreePublic /* networkType */, null /* anqpElements */,
                /* expectedScore */
                PasspointNetworkScore.INTERNET_ACCESS_AWARD
                + PasspointNetworkScore.PUBLIC_OR_PRIVATE_NETWORK_AWARDS
                + PasspointNetworkScore.RSSI_SCORE.lookupScore(-60, true)));

        // Roaming provider public network without Internet access that's not the current active
        // network.
        TEST_DATA_LIST.add(new TestData(false /* isHomeProvider */, false /* isActiveNetwork */,
                -60 /* rssiLevel */, false /* internetAccess */,
                NetworkDetail.Ant.FreePublic /* networkType */, null /* anqpElements */,
                /* expectedScore */
                PasspointNetworkScore.PUBLIC_OR_PRIVATE_NETWORK_AWARDS
                - PasspointNetworkScore.INTERNET_ACCESS_AWARD
                + PasspointNetworkScore.RSSI_SCORE.lookupScore(-60, false)));

        // Roaming provider personal network with Internet access that's not the current active
        // network.
        TEST_DATA_LIST.add(new TestData(false /* isHomeProvider */, false /* isActiveNetwork */,
                -60 /* rssiLevel */, true /* internetAccess */,
                NetworkDetail.Ant.Personal /* networkType */, null /* anqpElements */,
                /* expectedScore */
                PasspointNetworkScore.INTERNET_ACCESS_AWARD
                + PasspointNetworkScore.PERSONAL_OR_EMERGENCY_NETWORK_AWARDS
                + PasspointNetworkScore.RSSI_SCORE.lookupScore(-60, false)));
    }

    /**
     * Helper function for generating a {@link ScanDetail} for testing.
     *
     * @param rssiLevel RSSI level of the network
     * @param internetAccess Flag indicating if the network provides Internet access
     * @param networkType The type of the network
     * @return {@link ScanDetail}
     */
    private static ScanDetail generateScanDetail(int rssiLevel, boolean internetAccess,
            NetworkDetail.Ant networkType) {
        // Setup ScanResult.
        ScanResult scanResult = new ScanResult();
        scanResult.level = -60;

        // Setup NetworkDetail.
        NetworkDetail networkDetail = mock(NetworkDetail.class);
        when(networkDetail.isInternet()).thenReturn(internetAccess);
        when(networkDetail.getAnt()).thenReturn(networkType);

        // Setup ScanDetail.
        ScanDetail scanDetail = mock(ScanDetail.class);
        when(scanDetail.getScanResult()).thenReturn(scanResult);
        when(scanDetail.getNetworkDetail()).thenReturn(networkDetail);

        return scanDetail;
    }

    /**
     * Go through the list of the test data {@link #TEST_DATA_LIST} and verify the score for each.
     *
     * @throws Exception
     */
    @Test
    public void calculateScore() throws Exception {
        for (TestData data : TEST_DATA_LIST) {
            ScanDetail scanDetail = generateScanDetail(data.rssiLevel, data.internetAccess,
                    data.networkType);
            assertEquals(data.expectedScore, PasspointNetworkScore.calculateScore(
                    data.isHomeProvider, scanDetail, data.anqpElements, data.isActiveNetwork));
        }
    }

    /**
     * It should not throw {@link NullPointerException} when unsupported network type is provided.
     *
     * @throws Exception
     */
    @Test
    public void calculatorScoreWithUnSupportedNetworkType() throws Exception {
        TestData data = new TestData(false /* isHomeProvider */, false /* isActiveNetwork */,
                -60 /* rssiLevel */, true /* internetAccess */,
                NetworkDetail.Ant.Resvd6 /* reserved */, null /* anqpElements */,
                /* expectedScore */
                PasspointNetworkScore.INTERNET_ACCESS_AWARD
                        + PasspointNetworkScore.RSSI_SCORE.lookupScore(-60, false));

        ScanDetail scanDetail = generateScanDetail(data.rssiLevel, data.internetAccess,
                data.networkType);

        assertEquals(data.expectedScore, PasspointNetworkScore.calculateScore(
                    data.isHomeProvider, scanDetail, data.anqpElements, data.isActiveNetwork));
    }
}
