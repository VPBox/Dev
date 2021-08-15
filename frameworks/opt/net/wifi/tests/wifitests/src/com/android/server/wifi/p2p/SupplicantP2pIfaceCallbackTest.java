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
package com.android.server.wifi.p2p;

import static org.junit.Assert.*;
import static org.mockito.Matchers.*;
import static org.mockito.Mockito.doAnswer;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.never;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;

import android.app.test.MockAnswerUtil.AnswerWithArguments;
import android.hardware.wifi.supplicant.V1_0.ISupplicantP2pIfaceCallback;
import android.hardware.wifi.supplicant.V1_0.ISupplicantP2pIfaceCallback.P2pStatusCode;
import android.hardware.wifi.supplicant.V1_0.WpsConfigMethods;
import android.net.wifi.WpsInfo;
import android.net.wifi.p2p.WifiP2pConfig;
import android.net.wifi.p2p.WifiP2pDevice;
import android.net.wifi.p2p.WifiP2pGroup;
import android.net.wifi.p2p.WifiP2pProvDiscEvent;
import android.net.wifi.p2p.nsd.WifiP2pServiceResponse;

import androidx.test.filters.SmallTest;

import com.android.server.wifi.p2p.WifiP2pServiceImpl.P2pStatus;
import com.android.server.wifi.util.NativeUtil;

import org.junit.Assert.*;
import org.junit.Before;
import org.junit.Test;
import org.mockito.ArgumentCaptor;
import org.mockito.MockitoAnnotations;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;

/**
 * Unit tests for SupplicantP2pIfaceCallback
 */
@SmallTest
public class SupplicantP2pIfaceCallbackTest {
    private static final String TAG = "SupplicantP2pIfaceCallbackTest";

    private String mIface = "test_p2p0";
    private String mGroupIface = "test_p2p-p2p0-3";
    private WifiP2pMonitor mMonitor;
    private SupplicantP2pIfaceCallback mDut;

    private byte[] mDeviceAddressInvalid1 = { 0x00 };
    private byte[] mDeviceAddressInvalid2 = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66 };
    private byte[] mDeviceAddress1Bytes = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55 };
    private String mDeviceAddress1String = "00:11:22:33:44:55";
    private byte[] mDeviceAddress2Bytes = { 0x01, 0x12, 0x23, 0x34, 0x45, 0x56 };
    private String mDeviceAddress2String = "01:12:23:34:45:56";
    private byte[] mDeviceInfoBytes = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05 };
    private static final byte[] DEVICE_ADDRESS = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05 };
    private static final int TEST_NETWORK_ID = 9;
    private static final int TEST_GROUP_FREQUENCY = 5400;

    private class SupplicantP2pIfaceCallbackSpy extends SupplicantP2pIfaceCallback {
        SupplicantP2pIfaceCallbackSpy(String iface, WifiP2pMonitor monitor) {
            super(iface, monitor);
        }
    }

    @Before
    public void setUp() throws Exception {
        MockitoAnnotations.initMocks(this);
        mMonitor = mock(WifiP2pMonitor.class);
        mDut = new SupplicantP2pIfaceCallbackSpy(mIface, mMonitor);
    }

    /**
     * Sunny day scenario for onDeviceFound call.
     */
    @Test
    public void testOnDeviceFound_success() throws Exception {
        byte[] fakePrimaryDeviceTypeBytes = { 0x00, 0x01, 0x02, -1, 0x04, 0x05, 0x06, 0x07 };
        String fakePrimaryDeviceTypeString = "1-02FF0405-1543";
        String fakeDeviceName = "test device name";
        short fakeConfigMethods = 0x1234;
        byte fakeCapabilities = 123;
        int fakeGroupCapabilities = 456;

        doAnswer(new AnswerWithArguments() {
            public void answer(String iface, WifiP2pDevice device) {
                // NOTE: mDeviceAddress1Bytes seems to be ignored by
                // legacy implementation of WifiP2pDevice.
                assertEquals(iface, mIface);
                assertEquals(device.deviceName, fakeDeviceName);
                assertEquals(device.primaryDeviceType, fakePrimaryDeviceTypeString);
                assertEquals(device.deviceCapability, fakeCapabilities);
                assertEquals(device.groupCapability, fakeGroupCapabilities);
                assertEquals(device.wpsConfigMethodsSupported, fakeConfigMethods);
                assertEquals(device.deviceAddress, mDeviceAddress2String);
                assertEquals(device.status, WifiP2pDevice.AVAILABLE);
            }
        })
        .when(mMonitor).broadcastP2pDeviceFound(
                anyString(), any(WifiP2pDevice.class));

        mDut.onDeviceFound(
                mDeviceAddress1Bytes, mDeviceAddress2Bytes,
                fakePrimaryDeviceTypeBytes,
                fakeDeviceName, fakeConfigMethods,
                fakeCapabilities, fakeGroupCapabilities,
                mDeviceInfoBytes);

        mDut.onDeviceFound(
                mDeviceAddress1Bytes, mDeviceAddress2Bytes,
                fakePrimaryDeviceTypeBytes,
                fakeDeviceName, fakeConfigMethods,
                fakeCapabilities, fakeGroupCapabilities,
                null);

        // Make sure we issued a broadcast each time.
        verify(mMonitor, times(2)).broadcastP2pDeviceFound(
                anyString(), any(WifiP2pDevice.class));
    }

    /**
     * Sunny day scenario for onDeviceFound call with sign bit set in bytes.
     */
    @Test
    public void testOnDeviceFoundWithSignBitInDeviceInfoBytesSuccess() throws Exception {
        byte[] fakePrimaryDeviceTypeBytes = { 0x00, 0x01, 0x02, -1, 0x04, 0x05, 0x06, 0x07 };
        String fakePrimaryDeviceTypeString = "1-02FF0405-1543";
        String fakeDeviceName = "test device name";
        short fakeConfigMethods = 0x1234;
        byte fakeCapabilities = 123;
        int fakeGroupCapabilities = 456;
        byte[] fakeDevInfoBytes = { (byte) 0x80, 0x01, (byte) 0xC0, 0x03, (byte) 0xFF, 0x05 };

        mDut.onDeviceFound(
                mDeviceAddress1Bytes, mDeviceAddress2Bytes,
                fakePrimaryDeviceTypeBytes,
                fakeDeviceName, fakeConfigMethods,
                fakeCapabilities, fakeGroupCapabilities,
                fakeDevInfoBytes);

        ArgumentCaptor<WifiP2pDevice> deviceCaptor = ArgumentCaptor.forClass(WifiP2pDevice.class);
        verify(mMonitor).broadcastP2pDeviceFound(eq(mIface), deviceCaptor.capture());

        WifiP2pDevice device = deviceCaptor.getValue();
        assertEquals(fakeDeviceName, device.deviceName);
        assertEquals(fakePrimaryDeviceTypeString, device.primaryDeviceType);
        assertEquals(fakeCapabilities, device.deviceCapability);
        assertEquals(fakeGroupCapabilities, device.groupCapability);
        assertEquals(fakeConfigMethods, device.wpsConfigMethodsSupported);
        assertEquals(mDeviceAddress2String, device.deviceAddress);
        assertEquals(WifiP2pDevice.AVAILABLE, device.status);

        assertNotNull(device.wfdInfo);
        // WifiP2pWfdInfo.mDeviceInfo won't be returned as the raw value, skip it.
        assertEquals(((fakeDevInfoBytes[2] & 0xFF) << 8) + fakeDevInfoBytes[3],
                device.wfdInfo.getControlPort());
        assertEquals(((fakeDevInfoBytes[4] & 0xFF) << 8) + fakeDevInfoBytes[5],
                device.wfdInfo.getMaxThroughput());

        // Make sure we issued a broadcast each time.
        verify(mMonitor).broadcastP2pDeviceFound(
                anyString(), any(WifiP2pDevice.class));
    }

    /**
     * Failing scenarios for onDeviceFound call.
     */
    @Test
    public void testOnDeviceFound_invalidArguments() throws Exception {
        byte[] fakePrimaryDeviceTypeBytes = { 0x0, 0x01, 0x02, -1, 0x04, 0x05, 0x06, 0x07 };
        String fakePrimaryDeviceTypeString = "1-02FF0405-1543";
        String fakeDeviceName = "test device name";
        short fakeConfigMethods = 0x1234;
        byte fakeCapabilities = 123;
        int fakeGroupCapabilities = 456;

        mDut.onDeviceFound(
                mDeviceAddress2Bytes, null,
                fakePrimaryDeviceTypeBytes,
                fakeDeviceName, fakeConfigMethods,
                fakeCapabilities, fakeGroupCapabilities,
                mDeviceInfoBytes);
        verify(mMonitor, never()).broadcastP2pDeviceFound(
                anyString(), any(WifiP2pDevice.class));


        mDut.onDeviceFound(
                mDeviceAddress1Bytes, mDeviceAddress2Bytes,
                null,
                fakeDeviceName, fakeConfigMethods,
                fakeCapabilities, fakeGroupCapabilities,
                mDeviceInfoBytes);
        verify(mMonitor, never()).broadcastP2pDeviceFound(
                anyString(), any(WifiP2pDevice.class));


        mDut.onDeviceFound(
                mDeviceAddress1Bytes, mDeviceAddress2Bytes,
                fakePrimaryDeviceTypeBytes,
                null, fakeConfigMethods,
                fakeCapabilities, fakeGroupCapabilities,
                mDeviceInfoBytes);
        verify(mMonitor, never()).broadcastP2pDeviceFound(
                anyString(), any(WifiP2pDevice.class));


        mDut.onDeviceFound(
                mDeviceAddress1Bytes, mDeviceAddressInvalid1,
                fakePrimaryDeviceTypeBytes,
                null, fakeConfigMethods,
                fakeCapabilities, fakeGroupCapabilities,
                mDeviceInfoBytes);
        verify(mMonitor, never()).broadcastP2pDeviceFound(
                anyString(), any(WifiP2pDevice.class));


        mDut.onDeviceFound(
                mDeviceAddress1Bytes, mDeviceAddressInvalid2,
                fakePrimaryDeviceTypeBytes,
                null, fakeConfigMethods,
                fakeCapabilities, fakeGroupCapabilities,
                mDeviceInfoBytes);
        verify(mMonitor, never()).broadcastP2pDeviceFound(
                anyString(), any(WifiP2pDevice.class));
    }

    /**
     * Sunny day scenario for onDeviceLost call.
     */
    @Test
    public void testOnDeviceLost_success() throws Exception {
        doAnswer(new AnswerWithArguments() {
            public void answer(String iface, WifiP2pDevice device) {
                assertEquals(iface, mIface);
                assertEquals(device.deviceAddress, mDeviceAddress1String);
                assertEquals(device.status, WifiP2pDevice.UNAVAILABLE);
            }
        })
        .when(mMonitor).broadcastP2pDeviceLost(
                anyString(), any(WifiP2pDevice.class));

        mDut.onDeviceLost(mDeviceAddress1Bytes);

        // Make sure we issued a broadcast each time.
        verify(mMonitor, times(1)).broadcastP2pDeviceLost(
                anyString(), any(WifiP2pDevice.class));
    }

    /**
     * Failing scenarios for onDeviceLost call.
     */
    @Test
    public void testOnDeviceLost_invalidArguments() throws Exception {
        mDut.onDeviceLost(null);
        verify(mMonitor, never()).broadcastP2pDeviceLost(
                anyString(), any(WifiP2pDevice.class));

        mDut.onDeviceLost(mDeviceAddressInvalid1);
        verify(mMonitor, never()).broadcastP2pDeviceLost(
                anyString(), any(WifiP2pDevice.class));

        mDut.onDeviceLost(mDeviceAddressInvalid2);
        verify(mMonitor, never()).broadcastP2pDeviceLost(
                anyString(), any(WifiP2pDevice.class));
    }

    /**
     * Sunny day scenario for onGoNegotiationRequest call.
     */
    @Test
    public void testOnGoNegotiationRequest_success() throws Exception {
        HashSet<Integer> setups = new HashSet<Integer>();

        doAnswer(new AnswerWithArguments() {
            public void answer(String iface, WifiP2pConfig config) {
                assertEquals(iface, mIface);
                assertNotNull(config.wps);
                setups.add(config.wps.setup);
                assertEquals(config.deviceAddress, mDeviceAddress1String);
            }
        })
        .when(mMonitor).broadcastP2pGoNegotiationRequest(
                anyString(), any(WifiP2pConfig.class));

        mDut.onGoNegotiationRequest(mDeviceAddress1Bytes,
                (short)ISupplicantP2pIfaceCallback.WpsDevPasswordId.USER_SPECIFIED);
        assertTrue(setups.contains(WpsInfo.DISPLAY));

        mDut.onGoNegotiationRequest(mDeviceAddress1Bytes,
                (short)ISupplicantP2pIfaceCallback.WpsDevPasswordId.PUSHBUTTON);
        assertTrue(setups.contains(WpsInfo.PBC));

        mDut.onGoNegotiationRequest(mDeviceAddress1Bytes,
                (short)ISupplicantP2pIfaceCallback.WpsDevPasswordId.REGISTRAR_SPECIFIED);
        assertTrue(setups.contains(WpsInfo.KEYPAD));

        // Invalid should default to PBC
        setups.clear();
        mDut.onGoNegotiationRequest(mDeviceAddress1Bytes, (short)0xffff);
        assertTrue(setups.contains(WpsInfo.PBC));
    }

    /**
     * Failing scenarios for onGoNegotiationRequest call.
     */
    @Test
    public void testOnGoNegotiationRequest_invalidArguments() throws Exception {
        mDut.onGoNegotiationRequest(null, (short)0);
        verify(mMonitor, never()).broadcastP2pDeviceLost(
                anyString(), any(WifiP2pDevice.class));

        mDut.onGoNegotiationRequest(mDeviceAddressInvalid1, (short)0);
        verify(mMonitor, never()).broadcastP2pDeviceLost(
                anyString(), any(WifiP2pDevice.class));

        mDut.onGoNegotiationRequest(mDeviceAddressInvalid2, (short)0);
        verify(mMonitor, never()).broadcastP2pDeviceLost(
                anyString(), any(WifiP2pDevice.class));
    }

    /**
     * Sunny day scenario for onGroupStarted call.
     */
    @Test
    public void testOnGroupStarted_success() throws Exception {
        String fakeName = "group name";
        String fakePassphrase = "secret";
        ArrayList<Byte> fakeSsidBytesList = new ArrayList<Byte>() {{
            add((byte)0x30);
            add((byte)0x31);
            add((byte)0x32);
            add((byte)0x33);
        }};
        String fakeSsidString = "0123";
        HashSet<String> passwords = new HashSet<String>();

        doAnswer(new AnswerWithArguments() {
            public void answer(String iface, WifiP2pGroup group) {
                assertEquals(iface, mIface);
                assertNotNull(group.getOwner());
                assertEquals(group.getOwner().deviceAddress, mDeviceAddress1String);
                assertEquals(group.getNetworkId(), WifiP2pGroup.PERSISTENT_NET_ID);
                passwords.add(group.getPassphrase());
                assertEquals(group.getInterface(), fakeName);
                assertEquals(group.getNetworkName(), fakeSsidString);
            }
        })
        .when(mMonitor).broadcastP2pGroupStarted(
                anyString(), any(WifiP2pGroup.class));

        mDut.onGroupStarted(
                fakeName, true, fakeSsidBytesList, 1, null, fakePassphrase,
                mDeviceAddress1Bytes, true);
        assertTrue(passwords.contains(fakePassphrase));

        mDut.onGroupStarted(
                fakeName, true, fakeSsidBytesList, 1, null, null,
                mDeviceAddress1Bytes, true);
        assertTrue(passwords.contains(null));

        verify(mMonitor, times(2)).broadcastP2pGroupStarted(
                anyString(), any(WifiP2pGroup.class));
    }

    /**
     * Failing scenarios for onGroupStarted call.
     */
    @Test
    public void testOnGroupStarted_invalidArguments() throws Exception {
        String fakeName = "group name";
        String fakePassphrase = "secret";
        ArrayList<Byte> fakeSsidBytesList = new ArrayList<Byte>() {{
            add((byte)0x30);
            add((byte)0x31);
            add((byte)0x32);
            add((byte)0x33);
        }};
        String fakeSsidString = "0123";

        mDut.onGroupStarted(
                null, true, fakeSsidBytesList, 1, null, fakePassphrase,
                mDeviceAddress1Bytes, true);
        verify(mMonitor, never()).broadcastP2pGroupStarted(
                anyString(), any(WifiP2pGroup.class));

        mDut.onGroupStarted(
                fakeName, true, null, 1, null, fakePassphrase,
                mDeviceAddress1Bytes, true);
        verify(mMonitor, never()).broadcastP2pGroupStarted(
                anyString(), any(WifiP2pGroup.class));

        mDut.onGroupStarted(
                fakeName, true, fakeSsidBytesList, 1, null, fakePassphrase,
                null, true);
        verify(mMonitor, never()).broadcastP2pGroupStarted(
                anyString(), any(WifiP2pGroup.class));
    }

    /**
     * Test provision discovery callback.
     */
    @Test
    public void testOnProvisionDiscoveryCompleted() throws Exception {
        byte[] p2pDeviceAddr = DEVICE_ADDRESS;
        boolean isRequest = false;
        byte status = ISupplicantP2pIfaceCallback.P2pProvDiscStatusCode.SUCCESS;
        short configMethods = WpsConfigMethods.DISPLAY;
        String generatedPin = "12345678";

        ArgumentCaptor<WifiP2pProvDiscEvent> discEventCaptor =
                ArgumentCaptor.forClass(WifiP2pProvDiscEvent.class);
        mDut.onProvisionDiscoveryCompleted(
                p2pDeviceAddr, isRequest, status, configMethods, generatedPin);
        verify(mMonitor).broadcastP2pProvisionDiscoveryEnterPin(
                anyString(), discEventCaptor.capture());
        assertEquals(WifiP2pProvDiscEvent.ENTER_PIN, discEventCaptor.getValue().event);

        configMethods = WpsConfigMethods.KEYPAD;
        mDut.onProvisionDiscoveryCompleted(
                p2pDeviceAddr, isRequest, status, configMethods, generatedPin);
        verify(mMonitor).broadcastP2pProvisionDiscoveryShowPin(
                anyString(), discEventCaptor.capture());
        assertEquals(WifiP2pProvDiscEvent.SHOW_PIN, discEventCaptor.getValue().event);
        assertEquals(generatedPin, discEventCaptor.getValue().pin);

        isRequest = true;
        configMethods = WpsConfigMethods.KEYPAD;
        mDut.onProvisionDiscoveryCompleted(
                p2pDeviceAddr, isRequest, status, configMethods, generatedPin);
        verify(mMonitor, times(2)).broadcastP2pProvisionDiscoveryEnterPin(
                anyString(), discEventCaptor.capture());
        assertEquals(WifiP2pProvDiscEvent.ENTER_PIN, discEventCaptor.getValue().event);

        configMethods = WpsConfigMethods.DISPLAY;
        mDut.onProvisionDiscoveryCompleted(
                p2pDeviceAddr, isRequest, status, configMethods, generatedPin);
        verify(mMonitor, times(2)).broadcastP2pProvisionDiscoveryShowPin(
                anyString(), discEventCaptor.capture());
        assertEquals(WifiP2pProvDiscEvent.SHOW_PIN, discEventCaptor.getValue().event);
        assertEquals(generatedPin, discEventCaptor.getValue().pin);

        isRequest = false;
        configMethods = WpsConfigMethods.PUSHBUTTON;
        mDut.onProvisionDiscoveryCompleted(
                p2pDeviceAddr, isRequest, status, configMethods, generatedPin);
        verify(mMonitor).broadcastP2pProvisionDiscoveryPbcResponse(
                anyString(), discEventCaptor.capture());
        assertEquals(WifiP2pProvDiscEvent.PBC_RSP, discEventCaptor.getValue().event);

        isRequest = true;
        mDut.onProvisionDiscoveryCompleted(
                p2pDeviceAddr, isRequest, status, configMethods, generatedPin);
        verify(mMonitor).broadcastP2pProvisionDiscoveryPbcRequest(
                anyString(), discEventCaptor.capture());
        assertEquals(WifiP2pProvDiscEvent.PBC_REQ, discEventCaptor.getValue().event);
    }

    /**
     * Test staAuth with device address, should trigger ApStaConnected broadcast
     */
    @Test
    public void testStaAuth_success() {
        // Trigger onStaAuthorized callback, ensure wifimonitor broadcast is sent with WifiP2pDevice
        // using the p2pDeviceAddress
        ArgumentCaptor<WifiP2pDevice> p2pDeviceCaptor =
                ArgumentCaptor.forClass(WifiP2pDevice.class);
        mDut.onStaAuthorized(mDeviceAddress1Bytes, mDeviceAddress2Bytes);
        verify(mMonitor).broadcastP2pApStaConnected(any(String.class), p2pDeviceCaptor.capture());
        assertEquals(mDeviceAddress2String, p2pDeviceCaptor.getValue().deviceAddress);
    }

    /**
     * Test staAuth without device address, should trigger ApStaConnected broadcast using srcAddress
     */
    @Test
    public void testStaAuth_noDeviceAddress_success() {
        // Trigger onStaAuthorized callback, using a zero'd p2pDeviceAddress, ensure wifimonitor
        // broadcast is sent with WifiP2pDevice using the srcAddress
        ArgumentCaptor<WifiP2pDevice> p2pDeviceCaptor =
                ArgumentCaptor.forClass(WifiP2pDevice.class);
        mDut.onStaAuthorized(mDeviceAddress1Bytes, NativeUtil.ANY_MAC_BYTES);
        verify(mMonitor).broadcastP2pApStaConnected(any(String.class), p2pDeviceCaptor.capture());
        assertEquals(mDeviceAddress1String, p2pDeviceCaptor.getValue().deviceAddress);
    }

    // TLVS hex data encoded as a hex string.
    // Taken directly from an observed supplicant service response event
    private static final String SERV_DISC_RESP_TLVS = "1d00010100076578616d706c650b5f6166706f766572"
            + "746370c00c001001001e000101000b5f6166706f766572746370c00c000c01074578616d706c65c0273c"
            + "00010100096d797072696e746572045f697070c00c00100109747874766572733d311a70646c3d617070"
            + "6c69636174696f6e2f706f73747363726970741900010100045f697070c00c000c01094d795072696e74"
            + "6572c0275f000201000a757569643a36383539646564652d383537342d353961622d393333322d313233"
            + "3435363738393031313a3a75726e3a736368656d61732d75706e702d6f72673a736572766963653a436f"
            + "6e6e656374696f6e4d616e616765723a3159000201000a757569643a36383539646564652d383537342d"
            + "353961622d393333322d3132333435363738393031313a3a75726e3a736368656d61732d75706e702d6f"
            + "72673a736572766963653a41565472616e73706f72743a315a000201000a757569643a36383539646564"
            + "652d383537342d353961622d393333322d3132333435363738393031313a3a75726e3a736368656d6173"
            + "2d75706e702d6f72673a6465766963653a4d6564696152656e64657265723a313e000201000a75756964"
            + "3a36383539646564652d383537342d353961622d393333322d3132333435363738393031313a3a75706e"
            + "703a726f6f746465766963652d000201000a757569643a36383539646564652d383537342d353961622d"
            + "393333322d313233343536373839303131";

    /**
     * Pretty basic onServiceDiscoveryResponse callback test.
     * Mocks the callback event, passing some observed real data to it, and ensures that it returns
     * a non-null WifiP2pServiceResponse list.
     */
    @Test
    public void testOnServiceDiscoveryResponseCompleted_success() throws Exception {
        ArrayList<Byte> tlvs = NativeUtil.byteArrayToArrayList(hexStr2Bin(SERV_DISC_RESP_TLVS));
        ArgumentCaptor<List<WifiP2pServiceResponse>> respListCaptor =
                ArgumentCaptor.forClass(List.class);
        mDut.onServiceDiscoveryResponse(
                mDeviceAddress1Bytes,
                (short) 10 /* unused updateIndicator value */,
                tlvs);
        verify(mMonitor).broadcastP2pServiceDiscoveryResponse(anyString(),
                respListCaptor.capture());
        assertNotNull(respListCaptor.getValue());
    }

    /**
     * Test onFindStopped event should trigger P2pFindStopped broadcast.
     */
    @Test
    public void testOnFindStopped() {
        mDut.onFindStopped();
        verify(mMonitor).broadcastP2pFindStopped(eq(mIface));
    }

    /**
     * Test onGoNegotiationCompleted sunny case.
     */
    @Test
    public void testOnGoNegotiationCompletedSuccess() {
        mDut.onGoNegotiationCompleted(P2pStatusCode.SUCCESS);
        verify(mMonitor).broadcastP2pGoNegotiationSuccess(eq(mIface));
    }

    /**
     * Test onGoNegotiationCompleted failure cases.
     */
    @Test
    public void testOnGoNegotiationCompletedFailureUnavailable() {
        mDut.onGoNegotiationCompleted(P2pStatusCode.FAIL_INFO_CURRENTLY_UNAVAILABLE);
        verify(mMonitor).broadcastP2pGoNegotiationFailure(
                eq(mIface), eq(P2pStatus.INFORMATION_IS_CURRENTLY_UNAVAILABLE));
    }

    /**
     * Test onGroupFormationSuccess should trigger P2pGroupFormationSuccess broadcast.
     */
    @Test
    public void testOnGroupFormationSuccess() {
        mDut.onGroupFormationSuccess();
        verify(mMonitor).broadcastP2pGroupFormationSuccess(eq(mIface));
    }

    /**
     * Test onGroupFormationFailure should trigger P2pGroupFormationFailure broadcast.
     */
    @Test
    public void testOnGroupFormationFailure() {
        mDut.onGroupFormationFailure("failure-reason");
        verify(mMonitor).broadcastP2pGroupFormationFailure(eq(mIface), eq("failure-reason"));
    }

    /**
     * Test onGroupRemoved should trigger P2pGroupRemoved broadcast for Group Owner.
     */
    @Test
    public void testOnGroupRemovedForGroupOwner() {
        mDut.onGroupRemoved(mGroupIface, true);
        ArgumentCaptor<WifiP2pGroup> groupCaptor = ArgumentCaptor.forClass(WifiP2pGroup.class);
        verify(mMonitor).broadcastP2pGroupRemoved(eq(mIface), groupCaptor.capture());
        assertEquals(mGroupIface, groupCaptor.getValue().getInterface());
        assertEquals(true, groupCaptor.getValue().isGroupOwner());
    }

    /**
     * Test onGroupRemoved should trigger P2pGroupRemoved broadcast for Group Client.
     */
    @Test
    public void testOnGroupRemovedForGroupClient() {
        mDut.onGroupRemoved(mGroupIface, false);

        ArgumentCaptor<WifiP2pGroup> groupCaptor = ArgumentCaptor.forClass(WifiP2pGroup.class);
        verify(mMonitor).broadcastP2pGroupRemoved(eq(mIface), groupCaptor.capture());
        assertEquals(mGroupIface, groupCaptor.getValue().getInterface());
        assertEquals(false, groupCaptor.getValue().isGroupOwner());
    }

    /**
     * Test onInvitationReceived should trigger P2pInvitationReceived broadcast.
     */
    @Test
    public void testOnInvitationReceived() {
        mDut.onInvitationReceived(
                mDeviceAddress1Bytes,
                mDeviceAddress2Bytes,
                DEVICE_ADDRESS,
                TEST_NETWORK_ID,
                TEST_GROUP_FREQUENCY);

        ArgumentCaptor<WifiP2pGroup> groupCaptor = ArgumentCaptor.forClass(WifiP2pGroup.class);
        verify(mMonitor).broadcastP2pInvitationReceived(eq(mIface), groupCaptor.capture());

        WifiP2pGroup group = groupCaptor.getValue();
        assertEquals(TEST_NETWORK_ID, group.getNetworkId());
        assertEquals(mDeviceAddress2String, group.getOwner().deviceAddress);
    }

    /**
     * Test onInvitationResult should trigger P2pInvitationResult broadcast.
     */
    @Test
    public void testOnInvitationResult() {
        mDut.onInvitationResult(mDeviceAddress1Bytes, P2pStatusCode.SUCCESS);
        verify(mMonitor).broadcastP2pInvitationResult(eq(mIface), eq(P2pStatus.SUCCESS));
    }

    /**
     * Test onStaDeauthorized should trigger P2pApStaDisconnected broadcast.
     */
    @Test
    public void testOnStaDeauthorized() {
        mDut.onStaDeauthorized(mDeviceAddress1Bytes, mDeviceAddress2Bytes);

        ArgumentCaptor<WifiP2pDevice> p2pDeviceCaptor =
                ArgumentCaptor.forClass(WifiP2pDevice.class);
        verify(mMonitor).broadcastP2pApStaDisconnected(eq(mIface), p2pDeviceCaptor.capture());
        assertEquals(mDeviceAddress2String, p2pDeviceCaptor.getValue().deviceAddress);
    }
    /**
     * Converts hex string to byte array.
     *
     * @param hex hex string. if invalid, return null.
     * @return binary data.
     */
    private static byte[] hexStr2Bin(String hex) {
        int sz = hex.length() / 2;
        byte[] b = new byte[hex.length() / 2];
        for (int i = 0; i < sz; i++) {
            try {
                b[i] = (byte) Integer.parseInt(hex.substring(i * 2, i * 2 + 2), 16);
            } catch (Exception e) {
                return null;
            }
        }
        return b;
    }
}
