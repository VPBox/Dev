/*
 * Copyright (C) 2017 The Android Open Source Project
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


package com.android.server.wifi.rtt;

import static org.hamcrest.core.IsEqual.equalTo;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;
import static org.mockito.ArgumentMatchers.any;
import static org.mockito.ArgumentMatchers.anyInt;
import static org.mockito.ArgumentMatchers.eq;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.verifyNoMoreInteractions;
import static org.mockito.Mockito.when;

import android.hardware.wifi.V1_0.IWifiRttController;
import android.hardware.wifi.V1_0.RttBw;
import android.hardware.wifi.V1_0.RttCapabilities;
import android.hardware.wifi.V1_0.RttConfig;
import android.hardware.wifi.V1_0.RttPeerType;
import android.hardware.wifi.V1_0.RttPreamble;
import android.hardware.wifi.V1_0.RttResult;
import android.hardware.wifi.V1_0.RttStatus;
import android.hardware.wifi.V1_0.RttType;
import android.hardware.wifi.V1_0.WifiChannelWidthInMhz;
import android.hardware.wifi.V1_0.WifiStatus;
import android.hardware.wifi.V1_0.WifiStatusCode;
import android.net.MacAddress;
import android.net.wifi.rtt.RangingRequest;
import android.net.wifi.rtt.ResponderConfig;

import androidx.test.filters.SmallTest;

import com.android.server.wifi.HalDeviceManager;

import org.hamcrest.core.IsNull;
import org.junit.Before;
import org.junit.Rule;
import org.junit.Test;
import org.junit.rules.ErrorCollector;
import org.mockito.ArgumentCaptor;
import org.mockito.Mock;
import org.mockito.MockitoAnnotations;

import java.util.ArrayList;
import java.util.List;

/**
 * Unit test harness for the RttNative class.
 */
@SmallTest
public class RttNativeTest {
    private RttNative mDut;
    private WifiStatus mStatusSuccess;

    private ArgumentCaptor<ArrayList> mRttConfigCaptor = ArgumentCaptor.forClass(ArrayList.class);
    private ArgumentCaptor<List> mRttResultCaptor = ArgumentCaptor.forClass(List.class);
    private ArgumentCaptor<HalDeviceManager.ManagerStatusListener> mHdmStatusListener =
            ArgumentCaptor.forClass(HalDeviceManager.ManagerStatusListener.class);
    private ArgumentCaptor<HalDeviceManager.InterfaceRttControllerLifecycleCallback>
            mRttLifecycleCbCaptor = ArgumentCaptor.forClass(
            HalDeviceManager.InterfaceRttControllerLifecycleCallback.class);
    private ArgumentCaptor<IWifiRttController.getCapabilitiesCallback> mGetCapCbCatpr =
            ArgumentCaptor.forClass(IWifiRttController.getCapabilitiesCallback.class);

    @Rule
    public ErrorCollector collector = new ErrorCollector();

    @Mock
    public RttServiceImpl mockRttServiceImpl;

    @Mock
    public HalDeviceManager mockHalDeviceManager;

    @Mock
    public IWifiRttController mockRttController;

    @Before
    public void setUp() throws Exception {
        MockitoAnnotations.initMocks(this);

        when(mockHalDeviceManager.isStarted()).thenReturn(true);

        mStatusSuccess = new WifiStatus();
        mStatusSuccess.code = WifiStatusCode.SUCCESS;
        when(mockRttController.registerEventCallback(any())).thenReturn(mStatusSuccess);
        when(mockRttController.rangeRequest(anyInt(), any(ArrayList.class))).thenReturn(
                mStatusSuccess);
        when(mockRttController.rangeCancel(anyInt(), any(ArrayList.class))).thenReturn(
                mStatusSuccess);

        mDut = new RttNative(mockRttServiceImpl, mockHalDeviceManager);
        mDut.start(null);
        verify(mockHalDeviceManager).initialize();
        verify(mockHalDeviceManager).registerRttControllerLifecycleCallback(
                mRttLifecycleCbCaptor.capture(), any());
        mRttLifecycleCbCaptor.getValue().onNewRttController(mockRttController);
        verify(mockRttController).registerEventCallback(any());
        verify(mockRttServiceImpl).enableIfPossible();
        verify(mockRttController).getCapabilities(mGetCapCbCatpr.capture());
        // will override capabilities (just call cb again) for specific tests
        mGetCapCbCatpr.getValue().onValues(mStatusSuccess, getFullRttCapabilities());
        assertTrue(mDut.isReady());
    }

    /**
     * Validate successful ranging flow.
     */
    @Test
    public void testRangeRequest() throws Exception {
        int cmdId = 55;
        RangingRequest request = RttTestUtils.getDummyRangingRequest((byte) 0);

        // (1) issue range request
        mDut.rangeRequest(cmdId, request, true);

        // (2) verify HAL call and parameters
        verify(mockRttController).rangeRequest(eq(cmdId), mRttConfigCaptor.capture());

        // verify contents of HAL request (hard codes knowledge from getDummyRangingRequest()).
        ArrayList<RttConfig> halRequest = mRttConfigCaptor.getValue();

        collector.checkThat("number of entries", halRequest.size(),
                equalTo(request.mRttPeers.size()));

        RttConfig rttConfig = halRequest.get(0);
        collector.checkThat("entry 0: MAC", rttConfig.addr,
                equalTo(MacAddress.fromString("00:01:02:03:04:00").toByteArray()));
        collector.checkThat("entry 0: rtt type", rttConfig.type, equalTo(RttType.TWO_SIDED));
        collector.checkThat("entry 0: peer type", rttConfig.peer, equalTo(RttPeerType.AP));
        collector.checkThat("entry 0: lci", rttConfig.mustRequestLci, equalTo(true));
        collector.checkThat("entry 0: lcr", rttConfig.mustRequestLcr, equalTo(true));

        rttConfig = halRequest.get(1);
        collector.checkThat("entry 1: MAC", rttConfig.addr,
                equalTo(MacAddress.fromString("0A:0B:0C:0D:0E:00").toByteArray()));
        collector.checkThat("entry 1: rtt type", rttConfig.type, equalTo(RttType.ONE_SIDED));
        collector.checkThat("entry 1: peer type", rttConfig.peer, equalTo(RttPeerType.AP));
        collector.checkThat("entry 1: lci", rttConfig.mustRequestLci, equalTo(true));
        collector.checkThat("entry 1: lcr", rttConfig.mustRequestLcr, equalTo(true));

        rttConfig = halRequest.get(2);
        collector.checkThat("entry 2: MAC", rttConfig.addr,
                equalTo(MacAddress.fromString("08:09:08:07:06:05").toByteArray()));
        collector.checkThat("entry 2: rtt type", rttConfig.type, equalTo(RttType.TWO_SIDED));
        collector.checkThat("entry 2: peer type", rttConfig.peer, equalTo(RttPeerType.NAN));
        collector.checkThat("entry 2: lci", rttConfig.mustRequestLci, equalTo(false));
        collector.checkThat("entry 2: lcr", rttConfig.mustRequestLcr, equalTo(false));

        verifyNoMoreInteractions(mockRttController, mockRttServiceImpl);
    }

    /**
     * Validate ranging request with a mix of Repsonders with and without IEEE 802.11mc support,
     * from a non- privileged context.
     */
    @Test
    public void testRangeRequestNotPrivilegedNo80211mcSupportMixed() throws Exception {
        int cmdId = 66;

        // the request has 3 responders: first AP support 802.11mc, second AP does not, third is
        // Aware (which supports 802.11mc by default)
        RangingRequest request = RttTestUtils.getDummyRangingRequest((byte) 0);

        // (1) issue range request
        mDut.rangeRequest(cmdId, request, false);

        // (2) verify HAL call and parameters
        verify(mockRttController).rangeRequest(eq(cmdId), mRttConfigCaptor.capture());

        // verify contents of HAL request (hard codes knowledge from getDummyRangingRequest()).
        ArrayList<RttConfig> halRequest = mRttConfigCaptor.getValue();

        collector.checkThat("number of entries", halRequest.size(), equalTo(2));

        RttConfig rttConfig = halRequest.get(0);
        collector.checkThat("entry 0: MAC", rttConfig.addr,
                equalTo(MacAddress.fromString("00:01:02:03:04:00").toByteArray()));
        collector.checkThat("entry 0: rtt type", rttConfig.type, equalTo(RttType.TWO_SIDED));
        collector.checkThat("entry 0: peer type", rttConfig.peer, equalTo(RttPeerType.AP));
        collector.checkThat("entry 0: lci", rttConfig.mustRequestLci, equalTo(true));
        collector.checkThat("entry 0: lcr", rttConfig.mustRequestLcr, equalTo(true));

        rttConfig = halRequest.get(1);
        collector.checkThat("entry 1: MAC", rttConfig.addr,
                equalTo(MacAddress.fromString("08:09:08:07:06:05").toByteArray()));
        collector.checkThat("entry 1: rtt type", rttConfig.type, equalTo(RttType.TWO_SIDED));
        collector.checkThat("entry 1: peer type", rttConfig.peer, equalTo(RttPeerType.NAN));
        collector.checkThat("entry 1: lci", rttConfig.mustRequestLci, equalTo(false));
        collector.checkThat("entry 1: lcr", rttConfig.mustRequestLcr, equalTo(false));

        verifyNoMoreInteractions(mockRttController, mockRttServiceImpl);
    }

    /**
     * Validate successful ranging flow - with privileges access but with limited capabilities:
     * - No single-sided RTT
     * - No LCI/LCR
     * - Limited BW
     * - Limited Preamble
     */
    @Test
    public void testRangeRequestWithLimitedCapabilities() throws Exception {
        int cmdId = 55;
        RangingRequest request = RttTestUtils.getDummyRangingRequest((byte) 0);

        // update capabilities to a limited set
        RttCapabilities cap = getFullRttCapabilities();
        cap.rttOneSidedSupported = false;
        cap.lciSupported = false;
        cap.lcrSupported = false;
        cap.bwSupport = RttBw.BW_10MHZ | RttBw.BW_160MHZ;
        cap.preambleSupport = RttPreamble.LEGACY;
        mGetCapCbCatpr.getValue().onValues(mStatusSuccess, cap);

        // Note: request 1: BW = 40MHz --> 10MHz, Preamble = HT (since 40MHz) -> Legacy

        // (1) issue range request
        mDut.rangeRequest(cmdId, request, true);

        // (2) verify HAL call and parameters
        verify(mockRttController).rangeRequest(eq(cmdId), mRttConfigCaptor.capture());

        // verify contents of HAL request (hard codes knowledge from getDummyRangingRequest()).
        ArrayList<RttConfig> halRequest = mRttConfigCaptor.getValue();

        collector.checkThat("number of entries", halRequest.size(), equalTo(2));

        RttConfig rttConfig = halRequest.get(0);
        collector.checkThat("entry 0: MAC", rttConfig.addr,
                equalTo(MacAddress.fromString("00:01:02:03:04:00").toByteArray()));
        collector.checkThat("entry 0: rtt type", rttConfig.type, equalTo(RttType.TWO_SIDED));
        collector.checkThat("entry 0: peer type", rttConfig.peer, equalTo(RttPeerType.AP));
        collector.checkThat("entry 0: lci", rttConfig.mustRequestLci, equalTo(false));
        collector.checkThat("entry 0: lcr", rttConfig.mustRequestLcr, equalTo(false));
        collector.checkThat("entry 0: channel.width", rttConfig.channel.width, equalTo(
                WifiChannelWidthInMhz.WIDTH_40));
        collector.checkThat("entry 0: bw", rttConfig.bw, equalTo(RttBw.BW_10MHZ));
        collector.checkThat("entry 0: preamble", rttConfig.preamble, equalTo(RttPreamble.LEGACY));

        rttConfig = halRequest.get(1);
        collector.checkThat("entry 1: MAC", rttConfig.addr,
                equalTo(MacAddress.fromString("08:09:08:07:06:05").toByteArray()));
        collector.checkThat("entry 1: rtt type", rttConfig.type, equalTo(RttType.TWO_SIDED));
        collector.checkThat("entry 1: peer type", rttConfig.peer, equalTo(RttPeerType.NAN));
        collector.checkThat("entry 1: lci", rttConfig.mustRequestLci, equalTo(false));
        collector.checkThat("entry 1: lcr", rttConfig.mustRequestLcr, equalTo(false));

        verifyNoMoreInteractions(mockRttController, mockRttServiceImpl);
    }

    /**
     * Validate successful ranging flow - with privileges access but with limited capabilities:
     * - Very limited BW
     * - Very limited Preamble
     */
    @Test
    public void testRangeRequestWithLimitedCapabilitiesNoOverlap() throws Exception {
        int cmdId = 55;
        RangingRequest request = RttTestUtils.getDummyRangingRequest((byte) 0);

        // update capabilities to a limited set
        RttCapabilities cap = getFullRttCapabilities();
        cap.bwSupport = RttBw.BW_80MHZ;
        cap.preambleSupport = RttPreamble.VHT;
        mGetCapCbCatpr.getValue().onValues(mStatusSuccess, cap);

        // Note: request 1: BW = 40MHz --> no overlap -> dropped
        // Note: request 2: BW = 160MHz --> 160MHz, preamble = VHT (since 160MHz) -> no overlap,
        //                                                                           dropped

        // (1) issue range request
        mDut.rangeRequest(cmdId, request, true);

        // (2) verify HAL call and parameters
        verify(mockRttController).rangeRequest(eq(cmdId), mRttConfigCaptor.capture());

        // verify contents of HAL request (hard codes knowledge from getDummyRangingRequest()).
        ArrayList<RttConfig> halRequest = mRttConfigCaptor.getValue();

        collector.checkThat("number of entries", halRequest.size(), equalTo(1));

        RttConfig rttConfig = halRequest.get(0);
        collector.checkThat("entry 0: MAC", rttConfig.addr,
                equalTo(MacAddress.fromString("08:09:08:07:06:05").toByteArray()));
        collector.checkThat("entry 0: rtt type", rttConfig.type, equalTo(RttType.TWO_SIDED));
        collector.checkThat("entry 0: peer type", rttConfig.peer, equalTo(RttPeerType.NAN));
        collector.checkThat("entry 0: lci", rttConfig.mustRequestLci, equalTo(false));
        collector.checkThat("entry 0: lcr", rttConfig.mustRequestLcr, equalTo(false));

        verifyNoMoreInteractions(mockRttController, mockRttServiceImpl);
    }

    /**
     * Validate ranging request with all Repsonders without IEEE 802.11mc support, from a non-
     * privileged context.
     */
    @Test
    public void testRangeRequestNotPrivilegedNo80211mcSupportForAny() throws Exception {
        int cmdId = 77;
        RangingRequest request = RttTestUtils.getDummyRangingRequestNo80211mcSupport((byte) 0);

        // (1) issue range request
        mDut.rangeRequest(cmdId, request, false);

        // (2) verify immediate result callback (empty result set)
        verify(mockRttServiceImpl).onRangingResults(eq(cmdId), mRttResultCaptor.capture());

        collector.checkThat("Result set", mRttResultCaptor.getValue().size(), equalTo(0));

        verifyNoMoreInteractions(mockRttController, mockRttServiceImpl);
    }

    /**
     * Validate no range request when Wi-Fi is down
     */
    @Test
    public void testWifiDown() throws Exception {
        int cmdId = 55;
        RangingRequest request = RttTestUtils.getDummyRangingRequest((byte) 0);

        // (1) simulate Wi-Fi down and send a status change indication
        mRttLifecycleCbCaptor.getValue().onRttControllerDestroyed();
        verify(mockRttServiceImpl).disable();
        assertFalse(mDut.isReady());

        // (2) issue range request
        mDut.rangeRequest(cmdId, request, true);

        verifyNoMoreInteractions(mockRttServiceImpl, mockRttController);
    }

    /**
     * Validate that we react correctly (i.e. enable/disable global RTT availability) when
     * notified that the RTT controller has disappear and appeared.
     */
    @Test
    public void testRttControllerLifecycle() throws Exception {
        // RTT controller disappears
        mRttLifecycleCbCaptor.getValue().onRttControllerDestroyed();
        verify(mockRttServiceImpl).disable();
        assertFalse(mDut.isReady());

        // RTT controller re-appears (verification is x2 since 1st time is in setup())
        mRttLifecycleCbCaptor.getValue().onNewRttController(mockRttController);
        verify(mockRttController, times(2)).registerEventCallback(any());
        verify(mockRttServiceImpl, times(2)).enableIfPossible();
        verify(mockRttController, times(2)).getCapabilities(mGetCapCbCatpr.capture());
        assertTrue(mDut.isReady());

        verifyNoMoreInteractions(mockRttServiceImpl, mockRttController);
    }

    /**
     * Validate ranging cancel flow.
     */
    @Test
    public void testRangeCancel() throws Exception {
        int cmdId = 66;
        ArrayList<byte[]> macAddresses = new ArrayList<>();
        byte[] mac1 = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05};
        byte[] mac2 = {0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};
        macAddresses.add(mac1);
        macAddresses.add(mac2);

        // (1) issue cancel request
        mDut.rangeCancel(cmdId, macAddresses);

        // (2) verify HAL call and parameters
        verify(mockRttController).rangeCancel(cmdId, macAddresses);

        verifyNoMoreInteractions(mockRttController);
    }

    /**
     * Validate correct result conversion from HAL to framework.
     */
    @Test
    public void testRangeResults() throws Exception {
        int cmdId = 55;
        ArrayList<RttResult> results = new ArrayList<>();
        RttResult res = new RttResult();
        res.addr[0] = 5;
        res.addr[1] = 6;
        res.addr[2] = 7;
        res.addr[3] = 8;
        res.addr[4] = 9;
        res.addr[5] = 10;
        res.status = RttStatus.SUCCESS;
        res.distanceInMm = 1500;
        res.timeStampInUs = 666;
        results.add(res);

        // (1) have HAL call native with results
        mDut.onResults(cmdId, results);

        // (2) verify call to framework
        verify(mockRttServiceImpl).onRangingResults(eq(cmdId), mRttResultCaptor.capture());

        // verify contents of the framework results
        List<RttResult> rttR = mRttResultCaptor.getValue();

        collector.checkThat("number of entries", rttR.size(), equalTo(1));

        RttResult rttResult = rttR.get(0);
        collector.checkThat("status", rttResult.status,
                equalTo(RttStatus.SUCCESS));
        collector.checkThat("mac", rttResult.addr,
                equalTo(MacAddress.fromString("05:06:07:08:09:0A").toByteArray()));
        collector.checkThat("distanceCm", rttResult.distanceInMm, equalTo(1500));
        collector.checkThat("timestamp", rttResult.timeStampInUs, equalTo(666L));

        verifyNoMoreInteractions(mockRttController, mockRttServiceImpl);
    }

    /**
     * Validate correct cleanup when a null array of results is provided by HAL.
     */
    @Test
    public void testRangeResultsNullArray() {
        int cmdId = 66;

        mDut.onResults(cmdId, null);
        verify(mockRttServiceImpl).onRangingResults(eq(cmdId), mRttResultCaptor.capture());

        collector.checkThat("number of entries", mRttResultCaptor.getValue().size(), equalTo(0));
    }

    /**
     * Validate correct cleanup when an array of results containing null entries is provided by HAL.
     */
    @Test
    public void testRangeResultsSomeNulls() {
        int cmdId = 77;

        ArrayList<RttResult> results = new ArrayList<>();
        results.add(null);
        results.add(new RttResult());
        results.add(null);
        results.add(null);
        results.add(new RttResult());
        results.add(null);

        mDut.onResults(cmdId, results);
        verify(mockRttServiceImpl).onRangingResults(eq(cmdId), mRttResultCaptor.capture());

        List<RttResult> rttR = mRttResultCaptor.getValue();
        collector.checkThat("number of entries", rttR.size(), equalTo(2));
        for (int i = 0; i < rttR.size(); ++i) {
            collector.checkThat("entry", rttR.get(i), IsNull.notNullValue());
        }
    }

    /**
     * Validation ranging with invalid bw and preamble combination will be ignored.
     */
    @Test
    public void testRangingWithInvalidParameterCombination() throws Exception {
        int cmdId = 88;
        RangingRequest request = new RangingRequest.Builder().build();
        ResponderConfig invalidConfig = new ResponderConfig(
                MacAddress.fromString("08:09:08:07:06:88"), ResponderConfig.RESPONDER_AP, true,
                ResponderConfig.CHANNEL_WIDTH_80MHZ, 0, 0, 0, ResponderConfig.PREAMBLE_HT);
        ResponderConfig config = new ResponderConfig(MacAddress.fromString("08:09:08:07:06:89"),
                ResponderConfig.RESPONDER_AP, true,
                ResponderConfig.CHANNEL_WIDTH_80MHZ, 0, 0, 0, ResponderConfig.PREAMBLE_VHT);

        // Add a ResponderConfig with invalid parameter, should be ignored.
        request.mRttPeers.add(invalidConfig);
        request.mRttPeers.add(config);
        mDut.rangeRequest(cmdId, request, true);
        verify(mockRttController).rangeRequest(eq(cmdId), mRttConfigCaptor.capture());
        assertEquals(request.mRttPeers.size() - 1, mRttConfigCaptor.getValue().size());
    }

    // Utilities

    /**
     * Return an RttCapabilities structure with all features enabled and support for all
     * preambles and bandwidths. The purpose is to enable any request. The returned structure can
     * then be modified to disable specific features.
     */
    RttCapabilities getFullRttCapabilities() {
        RttCapabilities cap = new RttCapabilities();

        cap.rttOneSidedSupported = true;
        cap.rttFtmSupported = true;
        cap.lciSupported = true;
        cap.lcrSupported = true;
        cap.responderSupported = true; // unused
        cap.preambleSupport = RttPreamble.LEGACY | RttPreamble.HT | RttPreamble.VHT;
        cap.bwSupport =
                RttBw.BW_5MHZ | RttBw.BW_10MHZ | RttBw.BW_20MHZ | RttBw.BW_40MHZ | RttBw.BW_80MHZ
                        | RttBw.BW_160MHZ;
        cap.mcVersion = 1; // unused

        return cap;
    }
}
