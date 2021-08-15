/*
 * Copyright (C) 2019 The Android Open Source Project
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

package com.android.server.wifi.p2p;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;
import static org.mockito.ArgumentMatchers.any;
import static org.mockito.ArgumentMatchers.anyBoolean;
import static org.mockito.ArgumentMatchers.anyInt;
import static org.mockito.ArgumentMatchers.anyString;
import static org.mockito.ArgumentMatchers.eq;
import static org.mockito.Mockito.doAnswer;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;

import android.app.test.MockAnswerUtil.AnswerWithArguments;
import android.net.wifi.WifiManager;
import android.net.wifi.p2p.WifiP2pConfig;
import android.net.wifi.p2p.WifiP2pDevice;
import android.net.wifi.p2p.WifiP2pGroup;
import android.net.wifi.p2p.WifiP2pGroupList;
import android.net.wifi.p2p.WifiP2pManager;
import android.net.wifi.p2p.nsd.WifiP2pDnsSdServiceInfo;
import android.net.wifi.p2p.nsd.WifiP2pServiceInfo;
import android.os.Handler;

import androidx.test.filters.SmallTest;

import com.android.server.wifi.HalDeviceManager;
import com.android.server.wifi.PropertyService;
import com.android.server.wifi.WifiVendorHal;

import org.junit.Before;
import org.junit.Test;
import org.mockito.ArgumentCaptor;
import org.mockito.Mock;
import org.mockito.MockitoAnnotations;

/**
 * Unit tests for {@link com.android.server.wifi.WifiP2pMonitor}.
 */
@SmallTest
public class WifiP2pNativeTest {

    private static final String TEST_DEVICE_NAME = "Android_HelloWorld";
    private static final String TEST_IFACE = "p2p-p2p0-1";
    private static final String TEST_BSSID = "de:ad:be:ef:01:02";
    private static final String TEST_PIN = "12345678";
    private static final String TEST_DEVICE_TYPE = "12-1234abcd-08";
    private static final String TEST_WPS_CONFIG = "usba label display push_button keypad";
    private static final String TEST_SSID_POSTFIX = "NiceBoat";
    private static final int TEST_IDLE_TIME = 10;
    private static final String TEST_NETWORK_NAME = "DIRECT-xy-NiceBoat";
    private static final String TEST_PASSPHRASE = "DeadEnd!";
    private static final int TEST_GROUP_FREQ = 5400;
    private static final String TEST_WFD_DEVICE_INFO = "deadbeef";
    private static final int TEST_P2P_FIND_TIMEOUT = 120;
    private static final String TEST_SERVICE_DISCOVERY_IDENTIFIER = "identifier";
    private static final String TEST_SERVICE_DISCOVERY_QUERY = "query";
    private static final String TEST_NFC_REQUEST_MSG = "request";
    private static final String TEST_NFC_SELECT_MSG = "select";
    private static final String TEST_CLIENT_LIST = "aa:bb:cc:dd:ee:ff 11:22:33:44:55:66";

    @Mock private WifiVendorHal mWifiVendorHalMock;
    @Mock private SupplicantP2pIfaceHal mSupplicantP2pIfaceHalMock;
    @Mock private HalDeviceManager mHalDeviceManagerMock;
    @Mock private PropertyService mPropertyServiceMock;
    @Mock private Handler mHandler;

    private WifiP2pNative mWifiP2pNative;
    private WifiP2pGroupList mWifiP2pGroupList = new WifiP2pGroupList();

    private WifiP2pGroup createP2pGroup(
            int networkId, String networkName, String passphrase, boolean isGo, String goAddr) {
        WifiP2pGroup group = new WifiP2pGroup();
        group.setNetworkId(networkId);
        group.setNetworkName(networkName);
        group.setPassphrase(passphrase);
        group.setIsGroupOwner(isGo);
        group.setOwner(new WifiP2pDevice(goAddr));
        return group;
    }

    @Before
    public void setUp() throws Exception {
        MockitoAnnotations.initMocks(this);
        mWifiP2pNative = new WifiP2pNative(
                mWifiVendorHalMock,
                mSupplicantP2pIfaceHalMock,
                mHalDeviceManagerMock,
                mPropertyServiceMock);

        mWifiP2pGroupList.add(
                createP2pGroup(1, "testGroup1", "passphrase", true, "aa:bb:cc:dd:ee:f0"));
        mWifiP2pGroupList.add(
                createP2pGroup(2, "testGroup2", "passphrase", false, "aa:bb:cc:dd:ee:f0"));
        mWifiP2pGroupList.add(
                createP2pGroup(3, "testGroup3", "passphrase", true, "aa:bb:cc:dd:ee:aa"));
        mWifiP2pGroupList.add(
                createP2pGroup(4, "testGroup4", "passphrase", true, "aa:bb:cc:dd:ee:bb"));

        // setup default mock behaviors
        when(mHalDeviceManagerMock.isSupported()).thenReturn(true);

        doAnswer(new AnswerWithArguments() {
                public boolean answer(WifiP2pGroupList groupList) {
                    for (WifiP2pGroup g : mWifiP2pGroupList.getGroupList()) {
                        groupList.add(g);
                    }
                    return true;
                }
        }).when(mSupplicantP2pIfaceHalMock).loadGroups(any());

    }

    /**
     * Verifies that isHalInterfaceSupported returns correct values.
     */
    @Test
    public void testIsHalInterfaceSupported() {
        assertTrue(mWifiP2pNative.isHalInterfaceSupported());

        when(mHalDeviceManagerMock.isSupported()).thenReturn(false);
        assertFalse(mWifiP2pNative.isHalInterfaceSupported());
    }

    /**
     * Verifies that the device name can be set.
     */
    @Test
    public void testSetDeviceName() {
        when(mSupplicantP2pIfaceHalMock.setWpsDeviceName(anyString())).thenReturn(true);
        assertTrue(mWifiP2pNative.setDeviceName(TEST_DEVICE_NAME));
        verify(mSupplicantP2pIfaceHalMock).setWpsDeviceName(eq(TEST_DEVICE_NAME));
    }

    /**
     * Verifies that the device name can be set.
     */
    @Test
    public void testSetP2pDeviceName() {
        when(mSupplicantP2pIfaceHalMock.setWpsDeviceName(anyString())).thenReturn(true);
        assertTrue(mWifiP2pNative.setP2pDeviceName(TEST_DEVICE_NAME));
        verify(mSupplicantP2pIfaceHalMock).setWpsDeviceName(eq(TEST_DEVICE_NAME));
    }

    /**
     * Verifies that networks could be listed.
     */
    @Test
    public void testP2pListNetworks() {
        WifiP2pGroupList groupList = new WifiP2pGroupList();
        assertTrue(mWifiP2pNative.p2pListNetworks(groupList));

        verify(mSupplicantP2pIfaceHalMock).loadGroups(any(WifiP2pGroupList.class));
        assertEquals(mWifiP2pGroupList.toString(), groupList.toString());
    }

    /**
     * Verifies that WPS PBC starts without errors.
     */
    @Test
    public void testStartWpsPbc() {
        when(mSupplicantP2pIfaceHalMock.startWpsPbc(anyString(), anyString())).thenReturn(true);
        assertTrue(mWifiP2pNative.startWpsPbc(TEST_IFACE, TEST_BSSID));
        verify(mSupplicantP2pIfaceHalMock).startWpsPbc(eq(TEST_IFACE), eq(TEST_BSSID));
    }

    /**
     * Verifies that WPS Pin/Keypad starts without errors.
     */
    @Test
    public void testStartWpsPinKeypad() {
        when(mSupplicantP2pIfaceHalMock.startWpsPinKeypad(anyString(), anyString()))
                .thenReturn(true);
        assertTrue(mWifiP2pNative.startWpsPinKeypad(TEST_IFACE, TEST_PIN));
        verify(mSupplicantP2pIfaceHalMock).startWpsPinKeypad(eq(TEST_IFACE), eq(TEST_PIN));
    }

    /**
     * Verifies that WPS Pin/Display starts without errors.
     */
    @Test
    public void testStartWpsPinDisplay() {
        when(mSupplicantP2pIfaceHalMock.startWpsPinDisplay(anyString(), anyString()))
                .thenReturn(TEST_PIN);
        assertEquals(TEST_PIN, mWifiP2pNative.startWpsPinDisplay(TEST_IFACE, TEST_BSSID));
        verify(mSupplicantP2pIfaceHalMock).startWpsPinDisplay(eq(TEST_IFACE), eq(TEST_BSSID));
    }

    /**
     * Verifies removing a network.
     */
    @Test
    public void testP2pRemoveNetwork() {
        when(mSupplicantP2pIfaceHalMock.removeNetwork(anyInt())).thenReturn(true);
        assertTrue(mWifiP2pNative.removeP2pNetwork(1));
        verify(mSupplicantP2pIfaceHalMock).removeNetwork(eq(1));
    }

    /**
     * Verifies setting the device type.
     */
    @Test
    public void testSetP2pDeviceType() {
        when(mSupplicantP2pIfaceHalMock.setWpsDeviceType(anyString())).thenReturn(true);
        assertTrue(mWifiP2pNative.setP2pDeviceType(TEST_DEVICE_TYPE));
        verify(mSupplicantP2pIfaceHalMock).setWpsDeviceType(eq(TEST_DEVICE_TYPE));
    }

    /**
     * Verifies setting WPS config method.
     */
    @Test
    public void testSetConfigMethods() {
        when(mSupplicantP2pIfaceHalMock.setWpsConfigMethods(anyString())).thenReturn(true);
        assertTrue(mWifiP2pNative.setConfigMethods(TEST_WPS_CONFIG));
        verify(mSupplicantP2pIfaceHalMock).setWpsConfigMethods(eq(TEST_WPS_CONFIG));
    }

    /**
     * Verifies setting SSID postfix.
     */
    @Test
    public void testSetP2pSsidPostfix() {
        when(mSupplicantP2pIfaceHalMock.setSsidPostfix(anyString())).thenReturn(true);
        assertTrue(mWifiP2pNative.setP2pSsidPostfix(TEST_SSID_POSTFIX));
        verify(mSupplicantP2pIfaceHalMock).setSsidPostfix(eq(TEST_SSID_POSTFIX));
    }

    /**
     * Verifies setting group idle time.
     */
    @Test
    public void testSetP2pGroupIdle() {
        when(mSupplicantP2pIfaceHalMock.setGroupIdle(anyString(), anyInt())).thenReturn(true);
        assertTrue(mWifiP2pNative.setP2pGroupIdle(TEST_IFACE, TEST_IDLE_TIME));
        verify(mSupplicantP2pIfaceHalMock).setGroupIdle(eq(TEST_IFACE), eq(TEST_IDLE_TIME));
    }

    /**
     * Verifies setting power save mode.
     */
    @Test
    public void testSetP2pPowerSave() {
        when(mSupplicantP2pIfaceHalMock.setPowerSave(anyString(), anyBoolean())).thenReturn(true);
        assertTrue(mWifiP2pNative.setP2pPowerSave(TEST_IFACE, true));
        verify(mSupplicantP2pIfaceHalMock).setPowerSave(eq(TEST_IFACE), eq(true));
    }

    /**
     * Verifies enabling Wifi Display.
     */
    @Test
    public void testSetWfdEnable() {
        when(mSupplicantP2pIfaceHalMock.enableWfd(anyBoolean())).thenReturn(true);
        assertTrue(mWifiP2pNative.setWfdEnable(true));
        verify(mSupplicantP2pIfaceHalMock).enableWfd(eq(true));
    }

    /**
     * Verifies setting WFD info.
     */
    @Test
    public void testSetWfdDeviceInfo() {
        when(mSupplicantP2pIfaceHalMock.setWfdDeviceInfo(anyString())).thenReturn(true);
        assertTrue(mWifiP2pNative.setWfdDeviceInfo(TEST_WFD_DEVICE_INFO));
        verify(mSupplicantP2pIfaceHalMock).setWfdDeviceInfo(eq(TEST_WFD_DEVICE_INFO));
    }

    /**
     * Verifies initiating a P2P service discovery indefinitely.
     */
    @Test
    public void testP2pFindIndefinitely() {
        when(mSupplicantP2pIfaceHalMock.find(anyInt())).thenReturn(true);
        assertTrue(mWifiP2pNative.p2pFind());
        verify(mSupplicantP2pIfaceHalMock).find(eq(0));
    }

    /**
     * Verifies initiating a P2P service discovery with timeout.
     */
    @Test
    public void testP2pFindWithTimeout() {
        when(mSupplicantP2pIfaceHalMock.find(anyInt())).thenReturn(true);
        assertTrue(mWifiP2pNative.p2pFind(TEST_P2P_FIND_TIMEOUT));
        verify(mSupplicantP2pIfaceHalMock).find(eq(TEST_P2P_FIND_TIMEOUT));
    }

    /**
     * Verifies stopping a P2P service discovery.
     */
    @Test
    public void testP2pStopFind() {
        when(mSupplicantP2pIfaceHalMock.stopFind()).thenReturn(true);
        assertTrue(mWifiP2pNative.p2pStopFind());
        verify(mSupplicantP2pIfaceHalMock).stopFind();
    }

    /**
     * Verifies configuring extended listen timing.
     */
    @Test
    public void testP2pExtListen() {
        when(mSupplicantP2pIfaceHalMock.configureExtListen(anyBoolean(), anyInt(), anyInt()))
                .thenReturn(true);
        assertTrue(mWifiP2pNative.p2pExtListen(true, 10000, 20000));
        verify(mSupplicantP2pIfaceHalMock).configureExtListen(eq(true), eq(10000), eq(20000));
    }

    /**
     * Verifies setting p2p listen channel.
     */
    @Test
    public void testP2pSetChannel() {
        when(mSupplicantP2pIfaceHalMock.setListenChannel(anyInt(), anyInt()))
                .thenReturn(true);
        assertTrue(mWifiP2pNative.p2pSetChannel(1, 81));
        verify(mSupplicantP2pIfaceHalMock).setListenChannel(eq(1), eq(81));
    }

    /**
     * Verifies flushing P2P peer table and state.
     */
    @Test
    public void testP2pFlush() {
        when(mSupplicantP2pIfaceHalMock.flush()).thenReturn(true);
        assertTrue(mWifiP2pNative.p2pFlush());
        verify(mSupplicantP2pIfaceHalMock).flush();
    }

    /**
     * Verifies starting p2p group formation.
     */
    @Test
    public void testP2pConnect() {
        WifiP2pConfig config = new WifiP2pConfig();
        config.deviceAddress = TEST_BSSID;
        mWifiP2pNative.p2pConnect(config, true);
        ArgumentCaptor<WifiP2pConfig> configCaptor = ArgumentCaptor.forClass(WifiP2pConfig.class);
        verify(mSupplicantP2pIfaceHalMock).connect(configCaptor.capture(), eq(true));
        // there is no equals operator for WifiP2pConfig.
        assertEquals(config.toString(), configCaptor.getValue().toString());
    }

    /**
     * Verifies cancelling an ongoing P2P group formation and joining-a-group related operation.
     */
    @Test
    public void testP2pCancelConnect() {
        when(mSupplicantP2pIfaceHalMock.cancelConnect()).thenReturn(true);
        assertTrue(mWifiP2pNative.p2pCancelConnect());
        verify(mSupplicantP2pIfaceHalMock).cancelConnect();
    }

    /**
     * Verifies sending P2P provision discovery request to the specified peer.
     */
    @Test
    public void testP2pProvisionDiscovery() {
        when(mSupplicantP2pIfaceHalMock.provisionDiscovery(any(WifiP2pConfig.class)))
                .thenReturn(true);
        WifiP2pConfig config = new WifiP2pConfig();
        config.deviceAddress = TEST_BSSID;
        assertTrue(mWifiP2pNative.p2pProvisionDiscovery(config));
        ArgumentCaptor<WifiP2pConfig> configCaptor =
                ArgumentCaptor.forClass(WifiP2pConfig.class);
        verify(mSupplicantP2pIfaceHalMock).provisionDiscovery(configCaptor.capture());
        // there is no equals operator for WifiP2pConfig.
        assertEquals(config.toString(), configCaptor.getValue().toString());
    }

    /**
     * Verifies joining p2p group.
     */
    @Test
    public void testJoinGroup() {
        when(mSupplicantP2pIfaceHalMock.groupAdd(anyBoolean())).thenReturn(true);
        assertTrue(mWifiP2pNative.p2pGroupAdd(true));
        verify(mSupplicantP2pIfaceHalMock).groupAdd(eq(true));
    }

    /**
     * Verifies joining p2p group with network id.
     */
    @Test
    public void testJoinGroupWithNetworkId() {
        when(mSupplicantP2pIfaceHalMock.groupAdd(anyInt(), anyBoolean())).thenReturn(true);
        assertTrue(mWifiP2pNative.p2pGroupAdd(5));
        verify(mSupplicantP2pIfaceHalMock).groupAdd(eq(5), eq(true));
    }

    /**
     * Verifies joining p2p group with config.
     */
    @Test
    public void testJoinGroupWithConfig() {
        when(mSupplicantP2pIfaceHalMock.groupAdd(
                anyString(), anyString(), anyBoolean(),
                anyInt(), anyString(), anyBoolean())).thenReturn(true);
        WifiP2pConfig config = new WifiP2pConfig.Builder()
                .setNetworkName(TEST_NETWORK_NAME)
                .setPassphrase(TEST_PASSPHRASE)
                .enablePersistentMode(true)
                .setGroupOperatingFrequency(TEST_GROUP_FREQ)
                .build();
        assertTrue(mWifiP2pNative.p2pGroupAdd(config, true));

        verify(mSupplicantP2pIfaceHalMock).groupAdd(
                eq(TEST_NETWORK_NAME),
                eq(TEST_PASSPHRASE),
                eq(true),
                eq(TEST_GROUP_FREQ),
                eq(config.deviceAddress),
                eq(true));
    }

    /**
     * Verifies removing p2p group.
     */
    @Test
    public void testP2pGroupRemove() {
        when(mSupplicantP2pIfaceHalMock.groupRemove(anyString())).thenReturn(true);
        assertTrue(mWifiP2pNative.p2pGroupRemove(TEST_IFACE));
        verify(mSupplicantP2pIfaceHalMock).groupRemove(eq(TEST_IFACE));
    }

    /**
     * Verifies rejecting a connection attemp.
     */
    @Test
    public void testP2pReject() {
        when(mSupplicantP2pIfaceHalMock.reject(anyString())).thenReturn(true);
        assertTrue(mWifiP2pNative.p2pReject(TEST_BSSID));
        verify(mSupplicantP2pIfaceHalMock).reject(eq(TEST_BSSID));
    }

    /**
     * Verifies inviting a peer to a group.
     */
    @Test
    public void testP2pInvite() {
        when(mSupplicantP2pIfaceHalMock.invite(any(WifiP2pGroup.class), anyString()))
                .thenReturn(true);
        WifiP2pGroup group = new WifiP2pGroup();
        assertTrue(mWifiP2pNative.p2pInvite(group, TEST_BSSID));
        ArgumentCaptor<WifiP2pGroup> groupCaptor = ArgumentCaptor.forClass(WifiP2pGroup.class);
        verify(mSupplicantP2pIfaceHalMock).invite(groupCaptor.capture(), eq(TEST_BSSID));
        // no equals operator for WifiP2pGroup.
        assertEquals(group.toString(), groupCaptor.getValue().toString());
    }

    /**
     * Verifies reinvoking a device from a persiste group..
     */
    @Test
    public void testP2pReinvoke() {
        when(mSupplicantP2pIfaceHalMock.reinvoke(anyInt(), anyString())).thenReturn(true);
        assertTrue(mWifiP2pNative.p2pReinvoke(5, TEST_BSSID));
        verify(mSupplicantP2pIfaceHalMock).reinvoke(eq(5), eq(TEST_BSSID));
    }

    /**
     * Verifies getting the operational SSID of the device.
     */
    @Test
    public void testP2pGetSsid() {
        when(mSupplicantP2pIfaceHalMock.getSsid(anyString())).thenReturn(TEST_NETWORK_NAME);
        assertEquals(TEST_NETWORK_NAME, mWifiP2pNative.p2pGetSsid(TEST_BSSID));
        verify(mSupplicantP2pIfaceHalMock).getSsid(eq(TEST_BSSID));
    }

    /**
     * Verifies getting the MAC address of the device.
     */
    @Test
    public void testP2pGetDeviceAddress() {
        when(mSupplicantP2pIfaceHalMock.getDeviceAddress()).thenReturn(TEST_BSSID);
        assertEquals(TEST_BSSID, mWifiP2pNative.p2pGetDeviceAddress());
        verify(mSupplicantP2pIfaceHalMock).getDeviceAddress();
    }

    /**
     * Verifies getting the group capabilities.
     */
    @Test
    public void testGetGroupCapability() {
        when(mSupplicantP2pIfaceHalMock.getGroupCapability(anyString())).thenReturn(0x156);
        assertEquals(0x156, mWifiP2pNative.getGroupCapability(TEST_BSSID));
        verify(mSupplicantP2pIfaceHalMock).getGroupCapability(eq(TEST_BSSID));
    }

    /**
     * Verifies adding a service..
     */
    @Test
    public void testP2pServiceAdd() {
        when(mSupplicantP2pIfaceHalMock.serviceAdd(any(WifiP2pServiceInfo.class)))
                .thenReturn(true);
        WifiP2pServiceInfo info =
                WifiP2pDnsSdServiceInfo.newInstance("MyPrinter", "_ipp._tcp", null);
        assertTrue(mWifiP2pNative.p2pServiceAdd(info));
        verify(mSupplicantP2pIfaceHalMock).serviceAdd(eq(info));
    }

    /**
     * Verifies deleting a service..
     */
    @Test
    public void testP2pServiceDel() {
        when(mSupplicantP2pIfaceHalMock.serviceRemove(any(WifiP2pServiceInfo.class)))
                .thenReturn(true);
        WifiP2pServiceInfo info =
                WifiP2pDnsSdServiceInfo.newInstance("MyPrinter", "_ipp._tcp", null);
        assertTrue(mWifiP2pNative.p2pServiceDel(info));
        verify(mSupplicantP2pIfaceHalMock).serviceRemove(eq(info));
    }

    /**
     * Verifies flushing p2p services in this device.
     */
    @Test
    public void testP2pServiceFlush() {
        when(mSupplicantP2pIfaceHalMock.serviceFlush()).thenReturn(true);
        assertTrue(mWifiP2pNative.p2pServiceFlush());
        verify(mSupplicantP2pIfaceHalMock).serviceFlush();
    }

    /**
     * Verifies scheduling a P2P service discovery request.
     */
    @Test
    public void testP2pServDiscReq() {
        when(mSupplicantP2pIfaceHalMock.requestServiceDiscovery(anyString(), anyString()))
                .thenReturn(TEST_SERVICE_DISCOVERY_IDENTIFIER);
        assertEquals(TEST_SERVICE_DISCOVERY_IDENTIFIER,
                mWifiP2pNative.p2pServDiscReq(TEST_BSSID, TEST_SERVICE_DISCOVERY_QUERY));
        verify(mSupplicantP2pIfaceHalMock)
                .requestServiceDiscovery(eq(TEST_BSSID), eq(TEST_SERVICE_DISCOVERY_QUERY));
    }

    /**
     * Verifies canceling a p2p service discovery request.
     */
    @Test
    public void testP2pServDiscCancelReq() {
        when(mSupplicantP2pIfaceHalMock.cancelServiceDiscovery(anyString())).thenReturn(true);
        assertTrue(mWifiP2pNative.p2pServDiscCancelReq(
                TEST_SERVICE_DISCOVERY_IDENTIFIER));
        verify(mSupplicantP2pIfaceHalMock).cancelServiceDiscovery(
                TEST_SERVICE_DISCOVERY_IDENTIFIER);
    }

    /**
     * Verifies setting miracast mode.
     */
    @Test
    public void testSetMiracastMode() {
        mWifiP2pNative.setMiracastMode(WifiP2pManager.MIRACAST_SOURCE);
        verify(mSupplicantP2pIfaceHalMock).setMiracastMode(eq(WifiP2pManager.MIRACAST_SOURCE));
    }

    /**
     * Verifies getting NFC handover request message.
     */
    @Test
    public void testGetNfcHandoverRequest() {
        when(mSupplicantP2pIfaceHalMock.getNfcHandoverRequest())
                .thenReturn(TEST_NFC_REQUEST_MSG);
        assertEquals(TEST_NFC_REQUEST_MSG, mWifiP2pNative.getNfcHandoverRequest());
        verify(mSupplicantP2pIfaceHalMock).getNfcHandoverRequest();
    }

    /**
     * Verifies getting NFC handover select message.
     */
    @Test
    public void testGetNfcHandoverSelect() {
        when(mSupplicantP2pIfaceHalMock.getNfcHandoverSelect())
                .thenReturn(TEST_NFC_SELECT_MSG);
        assertEquals(TEST_NFC_SELECT_MSG, mWifiP2pNative.getNfcHandoverSelect());
        verify(mSupplicantP2pIfaceHalMock).getNfcHandoverSelect();
    }

    /**
     * Verifies reporting NFC handover select message.
     */
    @Test
    public void testInitiatorReportNfcHandover() {
        when(mSupplicantP2pIfaceHalMock.initiatorReportNfcHandover(anyString()))
                .thenReturn(true);
        assertTrue(mWifiP2pNative.initiatorReportNfcHandover(TEST_NFC_SELECT_MSG));
        verify(mSupplicantP2pIfaceHalMock).initiatorReportNfcHandover(eq(TEST_NFC_SELECT_MSG));
    }

    /**
     * Verifies reporting NFC handover request message.
     */
    @Test
    public void testResponderReportNfcHandover() {
        when(mSupplicantP2pIfaceHalMock.responderReportNfcHandover(anyString()))
                .thenReturn(true);
        assertTrue(mWifiP2pNative.responderReportNfcHandover(TEST_NFC_REQUEST_MSG));
        verify(mSupplicantP2pIfaceHalMock).responderReportNfcHandover(eq(TEST_NFC_REQUEST_MSG));
    }

    /**
     * Verifies getting client list.
     */
    @Test
    public void testGetP2pClientList() {
        when(mSupplicantP2pIfaceHalMock.getClientList(anyInt()))
                .thenReturn(TEST_CLIENT_LIST);
        assertEquals(TEST_CLIENT_LIST, mWifiP2pNative.getP2pClientList(5));
        verify(mSupplicantP2pIfaceHalMock).getClientList(eq(5));
    }

    /**
     * Verifies setting client list.
     */
    @Test
    public void testSetP2pClientList() {
        when(mSupplicantP2pIfaceHalMock.setClientList(anyInt(), anyString()))
                .thenReturn(true);
        assertTrue(mWifiP2pNative.setP2pClientList(5, TEST_CLIENT_LIST));
        verify(mSupplicantP2pIfaceHalMock).setClientList(eq(5), eq(TEST_CLIENT_LIST));
    }

    /**
     * Verifies saving p2p config.
     */
    @Test
    public void testSaveConfig() {
        when(mSupplicantP2pIfaceHalMock.saveConfig())
                .thenReturn(true);
        assertTrue(mWifiP2pNative.saveConfig());
        verify(mSupplicantP2pIfaceHalMock).saveConfig();
    }

    /**
     * Verifies enabling MAC randomization.
     */
    @Test
    public void testSetMacRandomization() {
        when(mSupplicantP2pIfaceHalMock.setMacRandomization(anyBoolean()))
                .thenReturn(true);
        assertTrue(mWifiP2pNative.setMacRandomization(true));
        verify(mSupplicantP2pIfaceHalMock).setMacRandomization(eq(true));
    }

    /**
     * Verifies getting supported feature set.
     */
    @Test
    public void testGetSupportedFeatureSet() {
        when(mWifiVendorHalMock.getSupportedFeatureSet(anyString()))
                .thenReturn(WifiManager.WIFI_FEATURE_P2P_RAND_MAC);
        assertEquals(WifiManager.WIFI_FEATURE_P2P_RAND_MAC,
                mWifiP2pNative.getSupportedFeatureSet(TEST_IFACE));
        verify(mWifiVendorHalMock).getSupportedFeatureSet(eq(TEST_IFACE));
    }
}
