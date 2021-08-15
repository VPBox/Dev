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
import static org.mockito.Mockito.doThrow;
import static org.mockito.Mockito.inOrder;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.never;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.verifyNoMoreInteractions;
import static org.mockito.Mockito.when;

import android.app.test.MockAnswerUtil.AnswerWithArguments;
import android.hardware.wifi.supplicant.V1_0.ISupplicant;
import android.hardware.wifi.supplicant.V1_0.ISupplicantIface;
import android.hardware.wifi.supplicant.V1_0.ISupplicantNetwork;
import android.hardware.wifi.supplicant.V1_0.ISupplicantP2pIface;
import android.hardware.wifi.supplicant.V1_0.ISupplicantP2pIfaceCallback;
import android.hardware.wifi.supplicant.V1_0.ISupplicantP2pNetwork;
import android.hardware.wifi.supplicant.V1_0.IfaceType;
import android.hardware.wifi.supplicant.V1_0.SupplicantStatus;
import android.hardware.wifi.supplicant.V1_0.SupplicantStatusCode;
import android.hidl.manager.V1_0.IServiceManager;
import android.hidl.manager.V1_0.IServiceNotification;
import android.net.wifi.WpsInfo;
import android.net.wifi.p2p.WifiP2pConfig;
import android.net.wifi.p2p.WifiP2pDevice;
import android.net.wifi.p2p.WifiP2pGroup;
import android.net.wifi.p2p.WifiP2pGroupList;
import android.net.wifi.p2p.WifiP2pManager;
import android.net.wifi.p2p.nsd.WifiP2pServiceInfo;
import android.os.IHwBinder;
import android.os.RemoteException;
import android.text.TextUtils;

import androidx.test.filters.SmallTest;

import com.android.server.wifi.util.NativeUtil;

import org.junit.Assert.*;
import org.junit.Before;
import org.junit.Test;
import org.mockito.ArgumentCaptor;
import org.mockito.InOrder;
import org.mockito.Mock;
import org.mockito.MockitoAnnotations;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;

/**
 * Unit tests for SupplicantP2pIfaceHal
 */
@SmallTest
public class SupplicantP2pIfaceHalTest {
    private static final String TAG = "SupplicantP2pIfaceHalTest";
    private SupplicantP2pIfaceHal mDut;
    private @Mock IServiceManager mServiceManagerMock;
    private @Mock ISupplicant mISupplicantMock;
    private android.hardware.wifi.supplicant.V1_1.ISupplicant mISupplicantMockV1_1;
    private @Mock ISupplicantIface mISupplicantIfaceMock;
    private @Mock ISupplicantP2pIface mISupplicantP2pIfaceMock;
    private @Mock android.hardware.wifi.supplicant.V1_2.ISupplicantP2pIface
            mISupplicantP2pIfaceMockV12;
    private boolean mISupplicantV12Enabled;
    private @Mock ISupplicantP2pNetwork mISupplicantP2pNetworkMock;
    private @Mock WifiP2pMonitor mWifiMonitor;

    SupplicantStatus mStatusSuccess;
    SupplicantStatus mStatusFailure;
    RemoteException mRemoteException;
    ISupplicant.IfaceInfo mStaIface;
    ISupplicant.IfaceInfo mP2pIface;
    ArrayList<ISupplicant.IfaceInfo> mIfaceInfoList;

    final String mIfaceName = "virtual_interface_name";
    final String mSsid = "\"SSID\"";
    final ArrayList<Byte> mSsidBytes = new ArrayList<Byte>() {{
        add((byte)'S'); add((byte)'S'); add((byte)'I'); add((byte)'D');
    }};
    final String mPeerMacAddress = "00:11:22:33:44:55";
    final byte mPeerMacAddressBytes[] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55 };
    final String mGroupOwnerMacAddress = "01:12:23:34:45:56";
    final byte mGroupOwnerMacAddressBytes[] = { 0x01, 0x12, 0x23, 0x34, 0x45, 0x56 };
    final String mInvalidMacAddress1 = "00:11:22:33:44";
    final String mInvalidMacAddress2 = ":::::";
    final String mInvalidMacAddress3 = "invalid";
    final byte mInvalidMacAddressBytes1[] = null;
    final byte mInvalidMacAddressBytes2[] = {};
    final byte mInvalidMacAddressBytes3[] = { 0x00, 0x01, 0x02, 0x03, 0x04 };
    final byte mInvalidMacAddressBytes4[] = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06 };
    HashSet<String> mInvalidMacAddresses = new HashSet<String>(Arrays.asList(
            mInvalidMacAddress1, mInvalidMacAddress2,
            mInvalidMacAddress3));
    HashSet<byte[]> mInvalidMacAddressesBytes = new HashSet<byte[]>(Arrays.asList(
            mInvalidMacAddressBytes1, mInvalidMacAddressBytes2,
            mInvalidMacAddressBytes3, mInvalidMacAddressBytes4));

    final String mInvalidService1 = null;
    final String mInvalidService2 = "service";
    final String mValidServiceRequestString = "30313233";
    final byte[] mValidServiceRequestBytes = { 0x30, 0x31, 0x32, 0x33 };
    final String mInvalidServiceRequestString = "not a hex string";
    final String mInvalidUpnpService1 = "upnp";
    final String mInvalidUpnpService2 = "upnp 1";
    final String mInvalidUpnpService3 = "upnp invalid_number name";
    final String mInvalidBonjourService1 = "bonjour";
    final String mInvalidBonjourService2 = "bonjour 123456";
    final String mInvalidBonjourService3 = "bonjour invalid_hex 123456";
    final String mInvalidBonjourService4 = "bonjour 123456 invalid_hex";
    final String mValidUpnpService = "upnp 10 serviceName";
    final int mValidUpnpServiceVersion = 16;
    final String mValidUpnpServiceName = "serviceName";
    final String mValidBonjourService = "bonjour 30313233 34353637";
    final ArrayList<Byte> mValidBonjourServiceRequest = new ArrayList<Byte>() {{
        add((byte)'0'); add((byte)'1'); add((byte)'2'); add((byte)'3');
    }};
    final ArrayList<Byte> mValidBonjourServiceResponse = new ArrayList<Byte>() {{
        add((byte)'4'); add((byte)'5'); add((byte)'6'); add((byte)'7');
    }};

    // variables for groupAdd with config
    final String mNetworkName = "DIRECT-xy-Hello";
    final String mPassphrase = "12345678";
    final int mGroupOwnerBand = WifiP2pConfig.GROUP_OWNER_BAND_5GHZ;
    final boolean mIsPersistent = false;

    private ArgumentCaptor<IHwBinder.DeathRecipient> mDeathRecipientCaptor =
            ArgumentCaptor.forClass(IHwBinder.DeathRecipient.class);
    private ArgumentCaptor<IServiceNotification.Stub> mServiceNotificationCaptor =
            ArgumentCaptor.forClass(IServiceNotification.Stub.class);
    private InOrder mInOrder;

    private class SupplicantP2pIfaceHalSpy extends SupplicantP2pIfaceHal {
        SupplicantP2pIfaceHalSpy() {
            super(mWifiMonitor);
        }

        @Override
        protected IServiceManager getServiceManagerMockable() throws RemoteException {
            return mServiceManagerMock;
        }

        @Override
        protected ISupplicant getSupplicantMockable() throws RemoteException {
            return mISupplicantMock;
        }

        @Override
        protected android.hardware.wifi.supplicant.V1_1.ISupplicant getSupplicantMockableV1_1()
                throws RemoteException {
            return mISupplicantMockV1_1;
        }

        @Override
        protected ISupplicantP2pIface getP2pIfaceMockable(ISupplicantIface iface) {
            return mISupplicantP2pIfaceMock;
        }

        @Override
        protected android.hardware.wifi.supplicant.V1_2.ISupplicantP2pIface
                getP2pIfaceMockableV1_2() {
            return mISupplicantV12Enabled ? mISupplicantP2pIfaceMockV12 : null;
        }

        @Override
        protected ISupplicantP2pNetwork getP2pNetworkMockable(ISupplicantNetwork network) {
            return mISupplicantP2pNetworkMock;
        }
    }

    @Before
    public void setUp() throws Exception {
        MockitoAnnotations.initMocks(this);
        mStatusSuccess = createSupplicantStatus(SupplicantStatusCode.SUCCESS);
        mStatusFailure = createSupplicantStatus(SupplicantStatusCode.FAILURE_UNKNOWN);
        mRemoteException = new RemoteException("Test Remote Exception");
        mStaIface = createIfaceInfo(IfaceType.STA, "wlan0");
        mP2pIface = createIfaceInfo(IfaceType.P2P, mIfaceName);

        mIfaceInfoList = new ArrayList<ISupplicant.IfaceInfo>();
        mIfaceInfoList.add(mStaIface);
        mIfaceInfoList.add(mP2pIface);

        mISupplicantV12Enabled = false;

        when(mServiceManagerMock.linkToDeath(any(IHwBinder.DeathRecipient.class),
                anyLong())).thenReturn(true);
        when(mServiceManagerMock.registerForNotifications(anyString(), anyString(),
                any(IServiceNotification.Stub.class))).thenReturn(true);
        when(mISupplicantMock.linkToDeath(any(IHwBinder.DeathRecipient.class),
                anyLong())).thenReturn(true);
        when(mISupplicantP2pIfaceMock.linkToDeath(any(IHwBinder.DeathRecipient.class),
                anyLong())).thenReturn(true);
        when(mISupplicantP2pIfaceMock.registerCallback(any(ISupplicantP2pIfaceCallback.class)))
                .thenReturn(mStatusSuccess);
        mDut = new SupplicantP2pIfaceHalSpy();
    }

    /**
     * Sunny day scenario for SupplicantP2pIfaceHal initialization
     * Asserts successful initialization
     */
    @Test
    public void testInitialize_success() throws Exception {
        executeAndValidateInitializationSequence(false, false, false);
    }

    /**
     * Tests the initialization flow, with a RemoteException occurring when 'getInterface' is called
     * Ensures initialization fails.
     */
    @Test
    public void testInitialize_remoteExceptionFailure() throws Exception {
        executeAndValidateInitializationSequence(true, false, false);
    }

    /**
     * Tests the initialization flow, with listInterfaces returning 0 interfaces.
     * Ensures failure
     */
    @Test
    public void testInitialize_zeroInterfacesFailure() throws Exception {
        executeAndValidateInitializationSequence(false, true, false);
    }

    /**
     * Tests the initialization flow, with a null interface being returned by getInterface.
     * Ensures initialization fails.
     */
    @Test
    public void testInitialize_nullInterfaceFailure() throws Exception {
        executeAndValidateInitializationSequence(false, false, true);
    }

    /**
     * Sunny day scenario for SupplicantStaIfaceHal initialization
     * Asserts successful initialization
     */
    @Test
    public void testInitialize_successV1_1() throws Exception {
        mISupplicantMockV1_1 = mock(android.hardware.wifi.supplicant.V1_1.ISupplicant.class);
        executeAndValidateInitializationSequenceV1_1(false, false);
    }

    /**
     * Tests the initialization flow, with a RemoteException occurring when 'getInterface' is called
     * Ensures initialization fails.
     */
    @Test
    public void testInitialize_remoteExceptionFailureV1_1() throws Exception {
        mISupplicantMockV1_1 = mock(android.hardware.wifi.supplicant.V1_1.ISupplicant.class);
        executeAndValidateInitializationSequenceV1_1(true, false);
    }

    /**
     * Tests the initialization flow, with a null interface being returned by getInterface.
     * Ensures initialization fails.
     */
    @Test
    public void testInitialize_nullInterfaceFailureV1_1() throws Exception {
        mISupplicantMockV1_1 = mock(android.hardware.wifi.supplicant.V1_1.ISupplicant.class);
        executeAndValidateInitializationSequenceV1_1(false, true);
    }

    /**
     * Ensures that reject addition of an existing iface.
     */
    @Test
    public void testDuplicateSetupIfaceV1_1_Fails() throws Exception {
        mISupplicantMockV1_1 = mock(android.hardware.wifi.supplicant.V1_1.ISupplicant.class);
        executeAndValidateInitializationSequenceV1_1(false, false);

        // Trying setting up the p2p0 interface again & ensure it fails.
        assertFalse(mDut.setupIface(mIfaceName));
        verifyNoMoreInteractions(mISupplicantMockV1_1);
    }

    /**
     * Sunny day scenario for SupplicantStaIfaceHal teardown.
     * Asserts successful teardown.
     * Note: Only applicable for 1.1 supplicant HAL.
     */
    @Test
    public void testTeardown_successV1_1() throws Exception {
        mISupplicantMockV1_1 = mock(android.hardware.wifi.supplicant.V1_1.ISupplicant.class);
        executeAndValidateInitializationSequenceV1_1(false, false);

        when(mISupplicantMockV1_1.removeInterface(any(ISupplicant.IfaceInfo.class)))
                .thenReturn(mStatusSuccess);
        assertTrue(mDut.teardownIface(mIfaceName));
        verify(mISupplicantMockV1_1).removeInterface(any());
    }

    /**
     * Ensures that we reject removal of an invalid iface.
     */
    @Test
    public void testInvalidTeardownInterfaceV1_1_Fails() throws Exception {
        assertFalse(mDut.teardownIface(mIfaceName));
        verifyNoMoreInteractions(mISupplicantMock);
    }

    /**
     * Sunny day scenario for getName()
     */
    @Test
    public void testGetName_success() throws Exception {

        doAnswer(new AnswerWithArguments() {
            public void answer(ISupplicantIface.getNameCallback cb) throws RemoteException {
                cb.onValues(mStatusSuccess, mIfaceName);
            }
        })
        .when(mISupplicantP2pIfaceMock).getName(any(ISupplicantIface.getNameCallback.class));

        // Default value when service is not initialized.
        assertNull(mDut.getName());
        executeAndValidateInitializationSequence(false, false, false);
        assertEquals(mIfaceName, mDut.getName());
    }

    /**
     * Verify that getName returns null, if status is not SUCCESS.
     */
    @Test
    public void testGetName_failure() throws Exception {
        executeAndValidateInitializationSequence(false, false, false);
        doAnswer(new AnswerWithArguments() {
            public void answer(ISupplicantIface.getNameCallback cb) throws RemoteException {
                cb.onValues(mStatusFailure, "none");
            }
        })
        .when(mISupplicantP2pIfaceMock).getName(any(ISupplicantIface.getNameCallback.class));
        assertNull(mDut.getName());
        // Check that service is still alive.
        assertTrue(mDut.isInitializationComplete());
    }

    /**
     * Verify that getName disconnects and returns null, if HAL throws exception.
     */
    @Test
    public void testGetName_exception() throws Exception {
        executeAndValidateInitializationSequence(false, false, false);
        doAnswer(new AnswerWithArguments() {
            public void answer(ISupplicantIface.getNameCallback cb) throws RemoteException {
                throw new RemoteException("Test");
            }
        })
        .when(mISupplicantP2pIfaceMock).getName(any(ISupplicantIface.getNameCallback.class));
        assertNull(mDut.getName());
        // Check service is dead.
        assertFalse(mDut.isInitializationComplete());
    }


    /**
     * Sunny day scenario for find()
     */
    @Test
    public void testFind_success() throws Exception {
        when(mISupplicantP2pIfaceMock.find(anyInt())).thenReturn(mStatusSuccess);
        // Default value when service is not yet initialized.
        assertFalse(mDut.find(1));

        executeAndValidateInitializationSequence(false, false, false);
        assertTrue(mDut.find(1));
        assertFalse(mDut.find(-1));
    }

    /**
     * Verify that find returns false, if status is not SUCCESS.
     */
    @Test
    public void testFind_failure() throws Exception {
        executeAndValidateInitializationSequence(false, false, false);
        when(mISupplicantP2pIfaceMock.find(anyInt())).thenReturn(mStatusFailure);
        assertFalse(mDut.find(1));
        // Check that service is still alive.
        assertTrue(mDut.isInitializationComplete());
    }

    /**
     * Verify that find disconnects and returns false, if HAL throws exception.
     */
    @Test
    public void testFind_exception() throws Exception {
        executeAndValidateInitializationSequence(false, false, false);
        when(mISupplicantP2pIfaceMock.find(anyInt())).thenThrow(mRemoteException);
        assertFalse(mDut.find(0));
        // Check service is dead.
        assertFalse(mDut.isInitializationComplete());
    }


    /**
     * Sunny day scenario for stopFind()
     */
    @Test
    public void testStopFind_success() throws Exception {
        when(mISupplicantP2pIfaceMock.stopFind()).thenReturn(mStatusSuccess);
        // Default value when service is not yet initialized.
        assertFalse(mDut.stopFind());
        executeAndValidateInitializationSequence(false, false, false);
        assertTrue(mDut.stopFind());
    }

    /**
     * Verify that stopFind returns false, if status is not SUCCESS.
     */
    @Test
    public void testStopFind_failure() throws Exception {
        executeAndValidateInitializationSequence(false, false, false);
        when(mISupplicantP2pIfaceMock.stopFind()).thenReturn(mStatusFailure);
        assertFalse(mDut.stopFind());
        // Check that service is still alive.
        assertTrue(mDut.isInitializationComplete());
    }

    /**
     * Verify that stopFind disconnects and returns false, if HAL throws exception.
     */
    @Test
    public void testStopFind_exception() throws Exception {
        executeAndValidateInitializationSequence(false, false, false);
        when(mISupplicantP2pIfaceMock.stopFind()).thenThrow(mRemoteException);
        assertFalse(mDut.stopFind());
        // Check service is dead.
        assertFalse(mDut.isInitializationComplete());
    }


    /**
     * Sunny day scenario for flush()
     */
    @Test
    public void testFlush_success() throws Exception {
        when(mISupplicantP2pIfaceMock.flush()).thenReturn(mStatusSuccess);
        // Default value when service is not yet initialized.
        assertFalse(mDut.flush());
        executeAndValidateInitializationSequence(false, false, false);
        assertTrue(mDut.flush());
    }

    /**
     * Verify that flush returns false, if status is not SUCCESS.
     */
    @Test
    public void testFlush_failure() throws Exception {
        executeAndValidateInitializationSequence(false, false, false);
        when(mISupplicantP2pIfaceMock.flush()).thenReturn(mStatusFailure);
        assertFalse(mDut.flush());
        // Check that service is still alive.
        assertTrue(mDut.isInitializationComplete());
    }

    /**
     * Verify that flush disconnects and returns false, if HAL throws exception.
     */
    @Test
    public void testFlush_exception() throws Exception {
        executeAndValidateInitializationSequence(false, false, false);
        when(mISupplicantP2pIfaceMock.flush()).thenThrow(mRemoteException);
        assertFalse(mDut.flush());
        // Check service is dead.
        assertFalse(mDut.isInitializationComplete());
    }


    /**
     * Sunny day scenario for serviceFlush()
     */
    @Test
    public void testServiceFlush_success() throws Exception {
        when(mISupplicantP2pIfaceMock.flushServices()).thenReturn(mStatusSuccess);
        // Default value when service is not initialized.
        assertFalse(mDut.serviceFlush());
        executeAndValidateInitializationSequence(false, false, false);
        assertTrue(mDut.serviceFlush());
    }

    /**
     * Verify that serviceFlush returns false, if status is not SUCCESS.
     */
    @Test
    public void testServiceFlush_failure() throws Exception {
        executeAndValidateInitializationSequence(false, false, false);
        when(mISupplicantP2pIfaceMock.flushServices()).thenReturn(mStatusFailure);
        assertFalse(mDut.serviceFlush());
        // Check that service is still alive.
        assertTrue(mDut.isInitializationComplete());
    }

    /**
     * Verify that serviceFlush disconnects and returns false, if HAL throws exception.
     */
    @Test
    public void testServiceFlush_exception() throws Exception {
        executeAndValidateInitializationSequence(false, false, false);
        when(mISupplicantP2pIfaceMock.flushServices()).thenThrow(mRemoteException);
        assertFalse(mDut.serviceFlush());
        // Check service is dead.
        assertFalse(mDut.isInitializationComplete());
    }


    /**
     * Sunny day scenario for setPowerSave()
     */
    @Test
    public void testSetPowerSave_success() throws Exception {
        when(mISupplicantP2pIfaceMock.setPowerSave(eq(mIfaceName), anyBoolean()))
                .thenReturn(mStatusSuccess);
        // Default value when service is not initialized.
        assertFalse(mDut.setPowerSave(mIfaceName, true));
        executeAndValidateInitializationSequence(false, false, false);
        assertTrue(mDut.setPowerSave(mIfaceName, true));
    }

    /**
     * Verify that setPowerSave returns false, if status is not SUCCESS.
     */
    @Test
    public void testSetPowerSave_failure() throws Exception {
        executeAndValidateInitializationSequence(false, false, false);
        when(mISupplicantP2pIfaceMock.setPowerSave(eq(mIfaceName), anyBoolean()))
                .thenReturn(mStatusFailure);
        assertFalse(mDut.setPowerSave(mIfaceName, true));
        // Check that service is still alive.
        assertTrue(mDut.isInitializationComplete());
    }

    /**
     * Verify that setPowerSave disconnects and returns false, if HAL throws exception.
     */
    @Test
    public void testSetPowerSave_exception() throws Exception {
        executeAndValidateInitializationSequence(false, false, false);
        when(mISupplicantP2pIfaceMock.setPowerSave(eq(mIfaceName), anyBoolean()))
                .thenThrow(mRemoteException);
        assertFalse(mDut.setPowerSave(mIfaceName, true));
        // Check service is dead.
        assertFalse(mDut.isInitializationComplete());
    }


    /**
     * Sunny day scenario for setGroupIdle()
     */
    @Test
    public void testSetGroupIdle_success() throws Exception {
        when(mISupplicantP2pIfaceMock.setGroupIdle(eq(mIfaceName), anyInt()))
                .thenReturn(mStatusSuccess);
        // Default value when service is not initialized.
        assertFalse(mDut.setGroupIdle(mIfaceName, 1));
        executeAndValidateInitializationSequence(false, false, false);
        assertTrue(mDut.setGroupIdle(mIfaceName, 1));
        assertFalse(mDut.setGroupIdle(mIfaceName, -1));
    }

    /**
     * Verify that setGroupIdle returns false, if status is not SUCCESS.
     */
    @Test
    public void testSetGroupIdle_failure() throws Exception {
        executeAndValidateInitializationSequence(false, false, false);
        when(mISupplicantP2pIfaceMock.setGroupIdle(eq(mIfaceName), anyInt()))
                .thenReturn(mStatusFailure);
        assertFalse(mDut.setGroupIdle(mIfaceName, 1));
        // Check that service is still alive.
        assertTrue(mDut.isInitializationComplete());
    }

    /**
     * Verify that setGroupIdle disconnects and returns false, if HAL throws exception.
     */
    @Test
    public void testSetGroupIdle_exception() throws Exception {
        executeAndValidateInitializationSequence(false, false, false);
        when(mISupplicantP2pIfaceMock.setGroupIdle(eq(mIfaceName), anyInt()))
                .thenThrow(mRemoteException);
        assertFalse(mDut.setGroupIdle(mIfaceName, 1));
        // Check service is dead.
        assertFalse(mDut.isInitializationComplete());
    }


    /**
     * Sunny day scenario for setSsidPostfix()
     */
    @Test
    public void testSetSsidPostfix_success() throws Exception {
        String ssid = "SSID POSTFIX";
        when(mISupplicantP2pIfaceMock.setSsidPostfix(eq(NativeUtil.decodeSsid("\"" + ssid + "\""))))
                .thenReturn(mStatusSuccess);
        // Default value when service is not initialized.
        assertFalse(mDut.setSsidPostfix(ssid));
        executeAndValidateInitializationSequence(false, false, false);
        assertTrue(mDut.setSsidPostfix(ssid));
        assertFalse(mDut.setSsidPostfix(null));
    }

    /**
     * Verify that setSsidPostfix returns false, if status is not SUCCESS.
     */
    @Test
    public void testSetSsidPostfix_failure() throws Exception {
        String ssid = "SSID POSTFIX";
        executeAndValidateInitializationSequence(false, false, false);
        when(mISupplicantP2pIfaceMock.setSsidPostfix(eq(NativeUtil.decodeSsid("\"" + ssid + "\""))))
                .thenReturn(mStatusFailure);
        assertFalse(mDut.setSsidPostfix(ssid));
        // Check that service is still alive.
        assertTrue(mDut.isInitializationComplete());
    }

    /**
     * Verify that setSsidPostfix disconnects and returns false, if HAL throws exception.
     */
    @Test
    public void testSetSsidPostfix_exception() throws Exception {
        String ssid = "SSID POSTFIX";
        executeAndValidateInitializationSequence(false, false, false);
        when(mISupplicantP2pIfaceMock.setSsidPostfix(eq(NativeUtil.decodeSsid("\"" + ssid + "\""))))
                .thenThrow(mRemoteException);
        assertFalse(mDut.setSsidPostfix(ssid));
        // Check service is dead.
        assertFalse(mDut.isInitializationComplete());
    }


    /**
     * Sunny day scenario for connect()
     */
    @Test
    public void testConnect_success() throws Exception {
        final String configPin = "12345";
        final HashSet<Integer> methods = new HashSet<>();

        doAnswer(new AnswerWithArguments() {
            public void answer(byte[] peer, int method, String pin, boolean joinExisting,
                    boolean persistent, int goIntent,
                    ISupplicantP2pIface.connectCallback cb) throws RemoteException {
                methods.add(method);

                if (method == ISupplicantP2pIface.WpsProvisionMethod.DISPLAY
                        && TextUtils.isEmpty(pin)) {
                    // Return the configPin for DISPLAY method if the pin was not provided.
                    cb.onValues(mStatusSuccess, configPin);
                } else {
                    if (method != ISupplicantP2pIface.WpsProvisionMethod.PBC) {
                        // PIN is only required for PIN methods.
                        assertEquals(pin, configPin);
                    }
                    // For all the other cases, there is no generated pin.
                    cb.onValues(mStatusSuccess, "");
                }
            }
        })
        .when(mISupplicantP2pIfaceMock).connect(
                eq(mPeerMacAddressBytes), anyInt(), anyString(), anyBoolean(), anyBoolean(),
                anyInt(), any(ISupplicantP2pIface.connectCallback.class));

        WifiP2pConfig config = createDummyP2pConfig(mPeerMacAddress, WpsInfo.DISPLAY, "");

        // Default value when service is not initialized.
        assertNull(mDut.connect(config, false));

        executeAndValidateInitializationSequence(false, false, false);

        assertEquals(configPin, mDut.connect(config, false));
        assertTrue(methods.contains(ISupplicantP2pIface.WpsProvisionMethod.DISPLAY));
        methods.clear();

        config = createDummyP2pConfig(mPeerMacAddress, WpsInfo.DISPLAY, configPin);
        assertTrue(mDut.connect(config, false).isEmpty());
        assertTrue(methods.contains(ISupplicantP2pIface.WpsProvisionMethod.DISPLAY));
        methods.clear();

        config = createDummyP2pConfig(mPeerMacAddress, WpsInfo.PBC, "");
        assertTrue(mDut.connect(config, false).isEmpty());
        assertTrue(methods.contains(ISupplicantP2pIface.WpsProvisionMethod.PBC));
        methods.clear();

        config = createDummyP2pConfig(mPeerMacAddress, WpsInfo.KEYPAD, configPin);
        assertTrue(mDut.connect(config, false).isEmpty());
        assertTrue(methods.contains(ISupplicantP2pIface.WpsProvisionMethod.KEYPAD));
    }

    /**
     * Test connect with invalid arguments.
     */
    @Test
    public void testConnect_invalidArguments() throws Exception {
        executeAndValidateInitializationSequence(false, false, false);
        doAnswer(new AnswerWithArguments() {
            public void answer(byte[] peer, int method, String pin, boolean joinExisting,
                    boolean persistent, int goIntent,
                    ISupplicantP2pIface.connectCallback cb) throws RemoteException {
                cb.onValues(mStatusSuccess, pin);
            }
        })
        .when(mISupplicantP2pIfaceMock).connect(
                any(byte[].class), anyInt(), anyString(), anyBoolean(), anyBoolean(),
                anyInt(), any(ISupplicantP2pIface.connectCallback.class));

        WifiP2pConfig config = createDummyP2pConfig(mPeerMacAddress, WpsInfo.DISPLAY, "");

        // unsupported.
        config.wps.setup = -1;
        assertNull(mDut.connect(config, false));

        // Invalid peer address.
        config.wps.setup = WpsInfo.DISPLAY;
        for (String address : mInvalidMacAddresses) {
            config.deviceAddress = address;
            assertNull(mDut.connect(config, false));
        }

        // null pin not valid.
        config.wps.setup = WpsInfo.DISPLAY;
        config.wps.pin = null;
        assertNull(mDut.connect(config, false));

        // Pin should be empty for PBC.
        config.wps.setup = WpsInfo.PBC;
        config.wps.pin = "03455323";
        assertNull(mDut.connect(config, false));
    }

    /**
     * Verify that connect returns null, if status is not SUCCESS.
     */
    @Test
    public void testConnect_failure() throws Exception {
        final String configPin = "12345";
        WifiP2pConfig config = createDummyP2pConfig(mPeerMacAddress, WpsInfo.DISPLAY, configPin);

        executeAndValidateInitializationSequence(false, false, false);
        doAnswer(new AnswerWithArguments() {
            public void answer(byte[] peer, int method, String pin, boolean joinExisting,
                    boolean persistent, int goIntent,
                    ISupplicantP2pIface.connectCallback cb) throws RemoteException {
                cb.onValues(mStatusFailure, null);
            }
        })
        .when(mISupplicantP2pIfaceMock).connect(
                eq(mPeerMacAddressBytes), anyInt(), anyString(), anyBoolean(), anyBoolean(),
                anyInt(), any(ISupplicantP2pIface.connectCallback.class));

        assertNull(mDut.connect(config, false));
        // Check that service is still alive.
        assertTrue(mDut.isInitializationComplete());
    }

    /**
     * Verify that connect disconnects and returns null, if HAL throws exception.
     */
    @Test
    public void testConnect_exception() throws Exception {
        final String configPin = "12345";
        WifiP2pConfig config = createDummyP2pConfig(mPeerMacAddress, WpsInfo.DISPLAY, configPin);

        doThrow(mRemoteException)
        .when(mISupplicantP2pIfaceMock).connect(
                eq(mPeerMacAddressBytes), anyInt(), anyString(), anyBoolean(), anyBoolean(),
                anyInt(), any(ISupplicantP2pIface.connectCallback.class));

        assertNull(mDut.connect(config, false));
        // Check service is dead.
        assertFalse(mDut.isInitializationComplete());
    }


    /**
     * Sunny day scenario for cancelConnect()
     */
    @Test
    public void testCancelConnect_success() throws Exception {
        when(mISupplicantP2pIfaceMock.cancelConnect())
                .thenReturn(mStatusSuccess);
        // Default value when service is not initialized.
        assertFalse(mDut.cancelConnect());
        executeAndValidateInitializationSequence(false, false, false);
        assertTrue(mDut.cancelConnect());
    }

    /**
     * Verify that cancelConnect returns false, if status is not SUCCESS.
     */
    @Test
    public void testCancelConnect_failure() throws Exception {
        executeAndValidateInitializationSequence(false, false, false);
        when(mISupplicantP2pIfaceMock.cancelConnect())
                .thenReturn(mStatusFailure);
        assertFalse(mDut.cancelConnect());
        // Check that service is still alive.
        assertTrue(mDut.isInitializationComplete());
    }

    /**
     * Verify that cancelConnect disconnects and returns false, if HAL throws exception.
     */
    @Test
    public void testCancelConnect_exception() throws Exception {
        String ssid = "\"SSID POSTFIX\"";
        executeAndValidateInitializationSequence(false, false, false);
        when(mISupplicantP2pIfaceMock.cancelConnect())
                .thenThrow(mRemoteException);
        assertFalse(mDut.cancelConnect());
        // Check service is dead.
        assertFalse(mDut.isInitializationComplete());
    }


    /**
     * Sunny day scenario for provisionDiscovery()
     */
    @Test
    public void testProvisionDiscovery_success() throws Exception {
        WifiP2pConfig config = createDummyP2pConfig(mPeerMacAddress, WpsInfo.PBC, "");

        when(mISupplicantP2pIfaceMock.provisionDiscovery(
                eq(mPeerMacAddressBytes), anyInt()))
                .thenReturn(mStatusSuccess);
        // Default value when service is not initialized.
        assertFalse(mDut.provisionDiscovery(config));
        executeAndValidateInitializationSequence(false, false, false);
        assertTrue(mDut.provisionDiscovery(config));
    }

    /**
     * Test provisionDiscovery with invalid arguments.
     */
    @Test
    public void testProvisionDiscovery_invalidArguments() throws Exception {
        when(mISupplicantP2pIfaceMock.provisionDiscovery(
                eq(mPeerMacAddressBytes), anyInt()))
                .thenReturn(mStatusSuccess);
        executeAndValidateInitializationSequence(false, false, false);

        WifiP2pConfig config = createDummyP2pConfig(mPeerMacAddress, WpsInfo.PBC, "");

        // Unsupported method.
        config.wps.setup = -1;
        assertFalse(mDut.provisionDiscovery(config));

        config.wps.setup = WpsInfo.PBC;
        for (String address : mInvalidMacAddresses) {
            config.deviceAddress = address;
            assertFalse(mDut.provisionDiscovery(config));
        }
    }

    /**
     * Verify that provisionDiscovery returns false, if status is not SUCCESS.
     */
    @Test
    public void testProvisionDiscovery_failure() throws Exception {
        WifiP2pConfig config = createDummyP2pConfig(mPeerMacAddress, WpsInfo.PBC, "");

        executeAndValidateInitializationSequence(false, false, false);
        when(mISupplicantP2pIfaceMock.provisionDiscovery(
                eq(mPeerMacAddressBytes), anyInt()))
                .thenReturn(mStatusFailure);
        assertFalse(mDut.provisionDiscovery(config));
        // Check that service is still alive.
        assertTrue(mDut.isInitializationComplete());
    }

    /**
     * Verify that provisionDiscovery disconnects and returns false, if HAL throws exception.
     */
    @Test
    public void testProvisionDiscovery_exception() throws Exception {
        WifiP2pConfig config = createDummyP2pConfig(mPeerMacAddress, WpsInfo.PBC, "");

        executeAndValidateInitializationSequence(false, false, false);
        when(mISupplicantP2pIfaceMock.provisionDiscovery(
                eq(mPeerMacAddressBytes), anyInt()))
                .thenThrow(mRemoteException);
        assertFalse(mDut.provisionDiscovery(config));
        // Check service is dead.
        assertFalse(mDut.isInitializationComplete());
    }


    /**
     * Sunny day scenario for invite()
     */
    @Test
    public void testInvite_success() throws Exception {
        WifiP2pGroup group = createDummyP2pGroup();

        when(mISupplicantP2pIfaceMock.invite(
                eq(mIfaceName), eq(mGroupOwnerMacAddressBytes), eq(mPeerMacAddressBytes)))
                .thenReturn(mStatusSuccess);
        // Default value when service is not initialized.
        assertFalse(mDut.invite(group, mPeerMacAddress));
        executeAndValidateInitializationSequence(false, false, false);
        assertTrue(mDut.invite(group, mPeerMacAddress));
    }

    /**
     * Invite with invalid arguments.
     */
    @Test
    public void testInvite_invalidArguments() throws Exception {
        WifiP2pGroup group = createDummyP2pGroup();

        executeAndValidateInitializationSequence(false, false, false);
        when(mISupplicantP2pIfaceMock.invite(
                anyString(), any(byte[].class), any(byte[].class)))
                .thenReturn(mStatusSuccess);

        for (String address : mInvalidMacAddresses) {
            assertFalse(mDut.invite(group, address));
        }

        for (String address : mInvalidMacAddresses) {
            group.getOwner().deviceAddress = address;
            assertFalse(mDut.invite(group, mPeerMacAddress));
        }

        group.setOwner(null);
        assertFalse(mDut.invite(group, mPeerMacAddress));
        assertFalse(mDut.invite(null, mPeerMacAddress));
    }

    /**
     * Verify that invite returns false, if status is not SUCCESS.
     */
    @Test
    public void testInvite_failure() throws Exception {
        WifiP2pGroup group = createDummyP2pGroup();

        executeAndValidateInitializationSequence(false, false, false);
        when(mISupplicantP2pIfaceMock.invite(
                anyString(), any(byte[].class), any(byte[].class)))
                .thenReturn(mStatusFailure);
        assertFalse(mDut.invite(group, mPeerMacAddress));
        // Check that service is still alive.
        assertTrue(mDut.isInitializationComplete());
    }

    /**
     * Verify that invite disconnects and returns false, if HAL throws exception.
     */
    @Test
    public void testInvite_exception() throws Exception {
        WifiP2pGroup group = createDummyP2pGroup();

        executeAndValidateInitializationSequence(false, false, false);
        when(mISupplicantP2pIfaceMock.invite(
                anyString(), any(byte[].class), any(byte[].class)))
                .thenThrow(mRemoteException);
        assertFalse(mDut.invite(group, mPeerMacAddress));
        // Check service is dead.
        assertFalse(mDut.isInitializationComplete());
    }


    /**
     * Sunny day scenario for reject()
     */
    @Test
    public void testReject_success() throws Exception {
        when(mISupplicantP2pIfaceMock.reject(eq(mPeerMacAddressBytes)))
                .thenReturn(mStatusSuccess);
        // Default value when service is not initialized.
        assertFalse(mDut.reject(mPeerMacAddress));
        executeAndValidateInitializationSequence(false, false, false);
        assertTrue(mDut.reject(mPeerMacAddress));
    }

    /**
     * Reject with invalid arguments.
     */
    @Test
    public void testReject_invalidArguments() throws Exception {
        executeAndValidateInitializationSequence(false, false, false);
        when(mISupplicantP2pIfaceMock.reject(any(byte[].class)))
                .thenReturn(mStatusSuccess);

        for (String address : mInvalidMacAddresses) {
            assertFalse(mDut.reject(address));
        }
    }

    /**
     * Verify that reject returns false, if status is not SUCCESS.
     */
    @Test
    public void testReject_failure() throws Exception {
        executeAndValidateInitializationSequence(false, false, false);
        when(mISupplicantP2pIfaceMock.reject(any(byte[].class)))
                .thenReturn(mStatusFailure);
        assertFalse(mDut.reject(mPeerMacAddress));
        // Check that service is still alive.
        assertTrue(mDut.isInitializationComplete());
    }

    /**
     * Verify that reject disconnects and returns false, if HAL throws exception.
     */
    @Test
    public void testReject_exception() throws Exception {
        executeAndValidateInitializationSequence(false, false, false);
        when(mISupplicantP2pIfaceMock.reject(any(byte[].class)))
                .thenThrow(mRemoteException);
        assertFalse(mDut.reject(mPeerMacAddress));
        // Check service is dead.
        assertFalse(mDut.isInitializationComplete());
    }


    /**
     * Sunny day scenario for getDeviceAddress()
     */
    @Test
    public void testGetDeviceAddress_success() throws Exception {
        doAnswer(new AnswerWithArguments() {
            public void answer(ISupplicantP2pIface.getDeviceAddressCallback cb) {
                cb.onValues(mStatusSuccess, mPeerMacAddressBytes);
            }
        })
        .when(mISupplicantP2pIfaceMock).getDeviceAddress(
                any(ISupplicantP2pIface.getDeviceAddressCallback.class));

        // Default value when service is not initialized.
        assertNull(mDut.getDeviceAddress());
        executeAndValidateInitializationSequence(false, false, false);
        assertEquals(mPeerMacAddress, mDut.getDeviceAddress());
    }

    /**
     * Test getDeviceAddress() when invalid mac address is being reported.
     */
    @Test
    public void testGetDeviceAddress_invalidResult() throws Exception {
        executeAndValidateInitializationSequence(false, false, false);
        HashSet<byte[]> addresses = new HashSet<byte[]>(Arrays.asList(
                mInvalidMacAddressBytes1, mInvalidMacAddressBytes2,
                mInvalidMacAddressBytes3, mInvalidMacAddressBytes4));

        doAnswer(new AnswerWithArguments() {
            public void answer(ISupplicantP2pIface.getDeviceAddressCallback cb) {
                byte[] address = addresses.iterator().next();
                cb.onValues(mStatusSuccess, address);
                addresses.remove(address);
            }
        })
        .when(mISupplicantP2pIfaceMock).getDeviceAddress(
                any(ISupplicantP2pIface.getDeviceAddressCallback.class));

        // Default value when service is not initialized.
        while (!addresses.isEmpty()) {
            assertNull(mDut.getDeviceAddress());
        }
    }

    /**
     * Verify that getDeviceAddress returns false, if status is not SUCCESS.
     */
    @Test
    public void testGetDeviceAddress_failure() throws Exception {
        executeAndValidateInitializationSequence(false, false, false);
        doAnswer(new AnswerWithArguments() {
            public void answer(ISupplicantP2pIface.getDeviceAddressCallback cb) {
                cb.onValues(mStatusFailure, null);
            }
        })
        .when(mISupplicantP2pIfaceMock).getDeviceAddress(
                any(ISupplicantP2pIface.getDeviceAddressCallback.class));

        assertNull(mDut.getDeviceAddress());
        // Check that service is still alive.
        assertTrue(mDut.isInitializationComplete());
    }

    /**
     * Verify that getDeviceAddress disconnects and returns false, if HAL throws exception.
     */
    @Test
    public void testGetDeviceAddress_exception() throws Exception {
        executeAndValidateInitializationSequence(false, false, false);
        doThrow(mRemoteException).when(mISupplicantP2pIfaceMock).getDeviceAddress(
                any(ISupplicantP2pIface.getDeviceAddressCallback.class));

        assertNull(mDut.getDeviceAddress());
        // Check service is dead.
        assertFalse(mDut.isInitializationComplete());
    }


    /**
     * Sunny day scenario for getSsid()
     */
    @Test
    public void testGetSsid_success() throws Exception {
        doAnswer(new AnswerWithArguments() {
            public void answer(byte[] address, ISupplicantP2pIface.getSsidCallback cb) {
                cb.onValues(mStatusSuccess, mSsidBytes);
            }
        })
        .when(mISupplicantP2pIfaceMock).getSsid(
                eq(mPeerMacAddressBytes),
                any(ISupplicantP2pIface.getSsidCallback.class));

        // Default value when service is not initialized.
        assertNull(mDut.getSsid(mPeerMacAddress));
        executeAndValidateInitializationSequence(false, false, false);
        assertEquals(NativeUtil.removeEnclosingQuotes(mSsid), mDut.getSsid(mPeerMacAddress));
    }

    /**
     * Test getSsid() with invalid argument and response.
     */
    @Test
    public void testGetSsid_invalidArguments() throws Exception {
        executeAndValidateInitializationSequence(false, false, false);

        doAnswer(new AnswerWithArguments() {
            public void answer(byte[] address, ISupplicantP2pIface.getSsidCallback cb) {
                cb.onValues(mStatusSuccess, mSsidBytes);
            }
        })
        .when(mISupplicantP2pIfaceMock).getSsid(
                any(byte[].class), any(ISupplicantP2pIface.getSsidCallback.class));

        for (String address : mInvalidMacAddresses) {
            assertNull(mDut.getSsid(address));
        }

        // Simulate null response from HAL.
        doAnswer(new AnswerWithArguments() {
            public void answer(byte[] address, ISupplicantP2pIface.getSsidCallback cb) {
                cb.onValues(mStatusSuccess, null);
            }
        })
        .when(mISupplicantP2pIfaceMock).getSsid(
                any(byte[].class), any(ISupplicantP2pIface.getSsidCallback.class));

        assertNull(mDut.getSsid(mPeerMacAddress));
    }

    /**
     * Verify that getSsid returns false, if status is not SUCCESS.
     */
    @Test
    public void testGetSsid_failure() throws Exception {
        executeAndValidateInitializationSequence(false, false, false);

        doAnswer(new AnswerWithArguments() {
            public void answer(byte[] address, ISupplicantP2pIface.getSsidCallback cb) {
                cb.onValues(mStatusFailure, null);
            }
        })
        .when(mISupplicantP2pIfaceMock).getSsid(
                any(byte[].class), any(ISupplicantP2pIface.getSsidCallback.class));

        assertNull(mDut.getSsid(mPeerMacAddress));
        // Check that service is still alive.
        assertTrue(mDut.isInitializationComplete());
    }

    /**
     * Verify that getSsid disconnects and returns false, if HAL throws exception.
     */
    @Test
    public void testGetSsid_exception() throws Exception {
        executeAndValidateInitializationSequence(false, false, false);
        doThrow(mRemoteException)
        .when(mISupplicantP2pIfaceMock).getSsid(
                any(byte[].class), any(ISupplicantP2pIface.getSsidCallback.class));

        assertNull(mDut.getSsid(mPeerMacAddress));
        // Check service is dead.
        assertFalse(mDut.isInitializationComplete());
    }


    /**
     * Sunny day scenario for reinvoke()
     */
    @Test
    public void testReinvoke_success() throws Exception {
        when(mISupplicantP2pIfaceMock.reinvoke(anyInt(), eq(mPeerMacAddressBytes)))
                .thenReturn(mStatusSuccess);
        // Default value when service is not initialized.
        assertFalse(mDut.reinvoke(0, mPeerMacAddress));
        executeAndValidateInitializationSequence(false, false, false);
        assertTrue(mDut.reinvoke(0, mPeerMacAddress));
    }

    /**
     * Reinvoke with invalid arguments.
     */
    @Test
    public void testReinvoke_invalidArguments() throws Exception {
        executeAndValidateInitializationSequence(false, false, false);
        when(mISupplicantP2pIfaceMock.reinvoke(anyInt(), any(byte[].class)))
                .thenReturn(mStatusSuccess);

        for (String address : mInvalidMacAddresses) {
            assertFalse(mDut.reinvoke(0, address));
        }
    }

    /**
     * Verify that reinvoke returns false, if status is not SUCCESS.
     */
    @Test
    public void testReinvoke_failure() throws Exception {
        executeAndValidateInitializationSequence(false, false, false);
        when(mISupplicantP2pIfaceMock.reinvoke(anyInt(), any(byte[].class)))
                .thenReturn(mStatusFailure);
        assertFalse(mDut.reinvoke(0, mPeerMacAddress));
        // Check that service is still alive.
        assertTrue(mDut.isInitializationComplete());
    }

    /**
     * Verify that reinvoke disconnects and returns false, if HAL throws exception.
     */
    @Test
    public void testReinvoke_exception() throws Exception {
        executeAndValidateInitializationSequence(false, false, false);
        when(mISupplicantP2pIfaceMock.reinvoke(anyInt(), any(byte[].class)))
                .thenThrow(mRemoteException);
        assertFalse(mDut.reinvoke(0, mPeerMacAddress));
        // Check service is dead.
        assertFalse(mDut.isInitializationComplete());
    }


    /**
     * Sunny day scenario for groupAdd()
     */
    @Test
    public void testGroupAdd_success() throws Exception {
        when(mISupplicantP2pIfaceMock.addGroup(eq(true), eq(3)))
                .thenReturn(mStatusSuccess);
        // Default value when service is not initialized.
        assertFalse(mDut.groupAdd(3, true));
        executeAndValidateInitializationSequence(false, false, false);
        assertTrue(mDut.groupAdd(3, true));
    }

    /**
     * Verify that groupAdd returns false, if status is not SUCCESS.
     */
    @Test
    public void testGroupAdd_failure() throws Exception {
        executeAndValidateInitializationSequence(false, false, false);
        when(mISupplicantP2pIfaceMock.addGroup(anyBoolean(), anyInt()))
                .thenReturn(mStatusFailure);
        assertFalse(mDut.groupAdd(0, true));
        // Check that service is still alive.
        assertTrue(mDut.isInitializationComplete());
    }

    /**
     * Verify that groupAdd disconnects and returns false, if HAL throws exception.
     */
    @Test
    public void testGroupAdd_exception() throws Exception {
        executeAndValidateInitializationSequence(false, false, false);
        when(mISupplicantP2pIfaceMock.addGroup(anyBoolean(), anyInt()))
                .thenThrow(mRemoteException);
        assertFalse(mDut.groupAdd(0, true));
        // Check service is dead.
        assertFalse(mDut.isInitializationComplete());
    }


    /**
     * Sunny day scenario for groupAdd() with config
     */
    @Test
    public void testGroupAddWithConfigSuccess() throws Exception {
        when(mISupplicantP2pIfaceMockV12.addGroup_1_2(
                eq(NativeUtil.decodeSsid("\"" + mNetworkName + "\"")),
                eq(mPassphrase),
                eq(mIsPersistent),
                eq(mGroupOwnerBand),
                eq(mPeerMacAddressBytes),
                anyBoolean()))
                .thenReturn(mStatusSuccess);
        // Default value when service is not initialized.
        assertFalse(mDut.groupAdd(mNetworkName, mPassphrase, mIsPersistent,
                mGroupOwnerBand, mPeerMacAddress, true));
        verify(mISupplicantP2pIfaceMockV12, never()).addGroup_1_2(
                any(ArrayList.class), anyString(),
                anyBoolean(), anyInt(),
                any(byte[].class), anyBoolean());

        executeAndValidateInitializationSequence(false, false, false);
        assertTrue(mDut.groupAdd(mNetworkName, mPassphrase, mIsPersistent,
                mGroupOwnerBand, mPeerMacAddress, true));
        verify(mISupplicantP2pIfaceMockV12).addGroup_1_2(
                eq(NativeUtil.decodeSsid("\"" + mNetworkName + "\"")),
                eq(mPassphrase),
                eq(mIsPersistent),
                eq(mGroupOwnerBand),
                eq(mPeerMacAddressBytes),
                eq(true));
    }

    /**
     * Verify that groupAdd with config returns false, if status is not SUCCESS.
     */
    @Test
    public void testGroupAddWithConfigFailure() throws Exception {
        executeAndValidateInitializationSequence(false, false, false);
        when(mISupplicantP2pIfaceMockV12.addGroup_1_2(
                any(ArrayList.class), anyString(),
                anyBoolean(), anyInt(),
                any(byte[].class), anyBoolean()))
                .thenReturn(mStatusFailure);
        assertFalse(mDut.groupAdd(mNetworkName, mPassphrase, mIsPersistent,
                mGroupOwnerBand, mPeerMacAddress, true));
        verify(mISupplicantP2pIfaceMockV12).addGroup_1_2(
                eq(NativeUtil.decodeSsid("\"" + mNetworkName + "\"")),
                eq(mPassphrase),
                eq(mIsPersistent),
                eq(mGroupOwnerBand),
                eq(mPeerMacAddressBytes),
                eq(true));
        // Check that service is still alive.
        assertTrue(mDut.isInitializationComplete());
    }

    /**
     * Verify that groupAdd with config returns false, if HIDL revision older than v1.2.
     */
    @Test
    public void testGroupAddWithConfigFailureV1_0() throws Exception {
        when(mISupplicantP2pIfaceMockV12.addGroup_1_2(
                any(ArrayList.class), anyString(),
                anyBoolean(), anyInt(),
                any(byte[].class), anyBoolean()))
                .thenReturn(mStatusSuccess);
        executeAndValidateInitializationSequence(false, false, false);
        // disable 1.2 interface to simulator since older revision cannot be casted to v1.2
        mISupplicantV12Enabled = false;

        assertFalse(mDut.groupAdd(mNetworkName, mPassphrase, mIsPersistent,
                mGroupOwnerBand, mPeerMacAddress, true));
        verify(mISupplicantP2pIfaceMockV12, never()).addGroup_1_2(
                any(ArrayList.class), anyString(),
                anyBoolean(), anyInt(),
                any(byte[].class), anyBoolean());
        // Check that service is still alive.
        assertTrue(mDut.isInitializationComplete());
    }


    /**
     * Verify that groupAdd with config disconnects and returns false, if HAL throws exception.
     */
    @Test
    public void testGroupAddWithConfigException() throws Exception {
        executeAndValidateInitializationSequence(false, false, false);
        when(mISupplicantP2pIfaceMockV12.addGroup_1_2(
                any(ArrayList.class), anyString(),
                anyBoolean(), anyInt(),
                any(byte[].class), anyBoolean()))
                .thenThrow(mRemoteException);
        assertFalse(mDut.groupAdd(mNetworkName, mPassphrase, mIsPersistent,
                mGroupOwnerBand, mPeerMacAddress, true));
        verify(mISupplicantP2pIfaceMockV12).addGroup_1_2(
                eq(NativeUtil.decodeSsid("\"" + mNetworkName + "\"")),
                eq(mPassphrase),
                eq(mIsPersistent),
                eq(mGroupOwnerBand),
                eq(mPeerMacAddressBytes),
                eq(true));
        // Check service is dead.
        assertFalse(mDut.isInitializationComplete());
    }

    /**
     * Sunny day scenario for groupRemove()
     */
    @Test
    public void testGroupRemove_success() throws Exception {
        when(mISupplicantP2pIfaceMock.removeGroup(eq(mIfaceName)))
                .thenReturn(mStatusSuccess);
        // Default value when service is not initialized.
        assertFalse(mDut.groupRemove(mIfaceName));
        executeAndValidateInitializationSequence(false, false, false);
        assertTrue(mDut.groupRemove(mIfaceName));
    }

    /**
     * Verify that groupRemove returns false, if status is not SUCCESS.
     */
    @Test
    public void testGroupRemove_failure() throws Exception {
        executeAndValidateInitializationSequence(false, false, false);
        when(mISupplicantP2pIfaceMock.removeGroup(anyString()))
                .thenReturn(mStatusFailure);
        assertFalse(mDut.groupRemove(mIfaceName));
        // Check that service is still alive.
        assertTrue(mDut.isInitializationComplete());
    }

    /**
     * Verify that groupRemove disconnects and returns false, if HAL throws exception.
     */
    @Test
    public void testGroupRemove_exception() throws Exception {
        executeAndValidateInitializationSequence(false, false, false);
        when(mISupplicantP2pIfaceMock.removeGroup(anyString()))
                .thenThrow(mRemoteException);
        assertFalse(mDut.groupRemove(mIfaceName));
        // Check service is dead.
        assertFalse(mDut.isInitializationComplete());
    }


    /**
     * Sunny day scenario for getGroupCapability()
     */
    @Test
    public void testGetGroupCapability_success() throws Exception {
        final int caps = 123;

        doAnswer(new AnswerWithArguments() {
            public void answer(byte[] address, ISupplicantP2pIface.getGroupCapabilityCallback cb) {
                cb.onValues(mStatusSuccess, caps);
            }
        })
        .when(mISupplicantP2pIfaceMock)
                .getGroupCapability(
                        eq(mPeerMacAddressBytes),
                        any(ISupplicantP2pIface.getGroupCapabilityCallback.class));

        // Default value when service is not initialized.
        assertEquals(-1, mDut.getGroupCapability(mPeerMacAddress));
        executeAndValidateInitializationSequence(false, false, false);
        assertEquals(caps, mDut.getGroupCapability(mPeerMacAddress));
    }

    /**
     * GetGroupCapability with invalid arguments.
     */
    @Test
    public void testGetGroupCapability_invalidArguments() throws Exception {
        executeAndValidateInitializationSequence(false, false, false);

        doAnswer(new AnswerWithArguments() {
            public void answer(byte[] address, ISupplicantP2pIface.getGroupCapabilityCallback cb) {
                cb.onValues(mStatusSuccess, 0);
            }
        })
        .when(mISupplicantP2pIfaceMock)
                .getGroupCapability(
                        eq(mPeerMacAddressBytes),
                        any(ISupplicantP2pIface.getGroupCapabilityCallback.class));

        for (String address : mInvalidMacAddresses) {
            assertEquals(-1, mDut.getGroupCapability(address));
        }
    }

    /**
     * Verify that getGroupCapability returns false, if status is not SUCCESS.
     */
    @Test
    public void testGetGroupCapability_failure() throws Exception {
        executeAndValidateInitializationSequence(false, false, false);

        doAnswer(new AnswerWithArguments() {
            public void answer(byte[] address, ISupplicantP2pIface.getGroupCapabilityCallback cb) {
                cb.onValues(mStatusFailure, 0);
            }
        })
        .when(mISupplicantP2pIfaceMock)
                .getGroupCapability(
                        eq(mPeerMacAddressBytes),
                        any(ISupplicantP2pIface.getGroupCapabilityCallback.class));

        assertEquals(-1, mDut.getGroupCapability(mPeerMacAddress));
        // Check that service is still alive.
        assertTrue(mDut.isInitializationComplete());
    }

    /**
     * Verify that getGroupCapability disconnects and returns false, if HAL throws exception.
     */
    @Test
    public void testGetGroupCapability_exception() throws Exception {
        executeAndValidateInitializationSequence(false, false, false);
        doThrow(mRemoteException)
                .when(mISupplicantP2pIfaceMock)
                .getGroupCapability(
                        eq(mPeerMacAddressBytes),
                        any(ISupplicantP2pIface.getGroupCapabilityCallback.class));
        assertEquals(-1, mDut.getGroupCapability(mPeerMacAddress));
        // Check service is dead.
        assertFalse(mDut.isInitializationComplete());
    }


    /**
     * Sunny day scenario for configureExtListen()
     */
    @Test
    public void testConfigureExtListen_success() throws Exception {
        when(mISupplicantP2pIfaceMock.configureExtListen(eq(123), eq(456)))
                .thenReturn(mStatusSuccess);
        when(mISupplicantP2pIfaceMock.configureExtListen(eq(0), eq(0)))
                .thenReturn(mStatusSuccess);
        // Default value when service is not initialized.
        assertFalse(mDut.configureExtListen(true, 123, 456));
        executeAndValidateInitializationSequence(false, false, false);
        assertTrue(mDut.configureExtListen(true, 123, 456));
        // Turning listening off should reset intervals to 0s.
        assertTrue(mDut.configureExtListen(false, 999, 999));
        // Disable listening.
        assertTrue(mDut.configureExtListen(false, -1, -1));
    }

    /**
     * Test configureExtListen with invalid parameters.
     */
    @Test
    public void testConfigureExtListen_invalidArguments() throws Exception {
        executeAndValidateInitializationSequence(false, false, false);
        when(mISupplicantP2pIfaceMock.configureExtListen(anyInt(), anyInt()))
                .thenReturn(mStatusFailure);
        assertFalse(mDut.configureExtListen(true, -1, 1));
        assertFalse(mDut.configureExtListen(true, 1, -1));
    }

    /**
     * Verify that configureExtListen returns false, if status is not SUCCESS.
     */
    @Test
    public void testConfigureExtListen_failure() throws Exception {
        executeAndValidateInitializationSequence(false, false, false);
        when(mISupplicantP2pIfaceMock.configureExtListen(anyInt(), anyInt()))
                .thenReturn(mStatusFailure);
        assertFalse(mDut.configureExtListen(true, 1, 1));
        // Check that service is still alive.
        assertTrue(mDut.isInitializationComplete());
    }

    /**
     * Verify that configureExtListen disconnects and returns false, if HAL throws exception.
     */
    @Test
    public void testConfigureExtListen_exception() throws Exception {
        executeAndValidateInitializationSequence(false, false, false);
        when(mISupplicantP2pIfaceMock.configureExtListen(anyInt(), anyInt()))
                .thenThrow(mRemoteException);
        assertFalse(mDut.configureExtListen(true, 1, 1));
        // Check service is dead.
        assertFalse(mDut.isInitializationComplete());
    }


    /**
     * Sunny day scenario for setListenChannel()
     */
    @Test
    public void testSetListenChannel_success() throws Exception {
        int lc = 4;
        int oc = 163;
        ISupplicantP2pIface.FreqRange range1 = new ISupplicantP2pIface.FreqRange();
        range1.min = 1000;
        range1.max = 5810;
        ISupplicantP2pIface.FreqRange range2 = new ISupplicantP2pIface.FreqRange();
        range2.min = 5820;
        range2.max = 6000;
        ArrayList<ISupplicantP2pIface.FreqRange> ranges = new ArrayList<>();
        ranges.add(range1);
        ranges.add(range2);

        when(mISupplicantP2pIfaceMock.setListenChannel(eq(lc),  anyInt()))
                .thenReturn(mStatusSuccess);
        when(mISupplicantP2pIfaceMock.setDisallowedFrequencies(eq(ranges)))
                .thenReturn(mStatusSuccess);
        // Default value when service is not initialized.
        assertFalse(mDut.setListenChannel(lc, oc));
        executeAndValidateInitializationSequence(false, false, false);
        assertTrue(mDut.setListenChannel(lc, oc));
    }

    /**
     * Sunny day scenario for setListenChannel()
     */
    @Test
    public void testSetListenChannel_successResetDisallowedFreq() throws Exception {
        int lc = 2;
        int oc = 0;
        ArrayList<ISupplicantP2pIface.FreqRange> ranges = new ArrayList<>();

        when(mISupplicantP2pIfaceMock.setListenChannel(eq(lc),  anyInt()))
                .thenReturn(mStatusSuccess);
        when(mISupplicantP2pIfaceMock.setDisallowedFrequencies(eq(ranges)))
                .thenReturn(mStatusSuccess);
        // Default value when service is not initialized.
        assertFalse(mDut.setListenChannel(lc, oc));
        executeAndValidateInitializationSequence(false, false, false);
        assertTrue(mDut.setListenChannel(lc, oc));
    }

    /**
     * Test setListenChannel with invalid parameters.
     */
    @Test
    public void testSetListenChannel_invalidArguments() throws Exception {
        executeAndValidateInitializationSequence(false, false, false);
        when(mISupplicantP2pIfaceMock.setListenChannel(anyInt(), anyInt()))
                .thenReturn(mStatusSuccess);
        when(mISupplicantP2pIfaceMock.setDisallowedFrequencies(any(ArrayList.class)))
                .thenReturn(mStatusSuccess);
        assertFalse(mDut.setListenChannel(-1, 1));
        assertFalse(mDut.setListenChannel(1, -1));
    }

    /**
     * Verify that setListenChannel returns false, if status is not SUCCESS.
     */
    @Test
    public void testSetListenChannel_failure() throws Exception {
        executeAndValidateInitializationSequence(false, false, false);
        when(mISupplicantP2pIfaceMock.setListenChannel(anyInt(), anyInt()))
                .thenReturn(mStatusFailure);
        when(mISupplicantP2pIfaceMock.setDisallowedFrequencies(any(ArrayList.class)))
                .thenReturn(mStatusSuccess);
        assertFalse(mDut.setListenChannel(1, 1));
        // Check that service is still alive.
        assertTrue(mDut.isInitializationComplete());
    }

    /**
     * Verify that setListenChannel disconnects and returns false, if HAL throws exception.
     */
    @Test
    public void testSetListenChannel_exception() throws Exception {
        executeAndValidateInitializationSequence(false, false, false);
        when(mISupplicantP2pIfaceMock.setListenChannel(anyInt(), anyInt()))
                .thenThrow(mRemoteException);
        assertFalse(mDut.setListenChannel(1, 1));
        // Check service is dead.
        assertFalse(mDut.isInitializationComplete());
    }


    /**
     * Sunny day scenario for serviceAdd()
     */
    @Test
    public void testServiceAdd_success() throws Exception {
        WifiP2pServiceInfo info = createDummyP2pServiceInfo(
                mValidUpnpService, mValidBonjourService);
        final HashSet<String> services = new HashSet<String>();

        doAnswer(new AnswerWithArguments() {
            public SupplicantStatus answer(int version, String name) {
                services.add("upnp");
                assertEquals(mValidUpnpServiceVersion, version);
                assertEquals(mValidUpnpServiceName, name);
                return mStatusSuccess;
            }
        })
        .when(mISupplicantP2pIfaceMock).addUpnpService(anyInt(), anyString());

        doAnswer(new AnswerWithArguments() {
            public SupplicantStatus answer(ArrayList<Byte> request, ArrayList<Byte> response) {
                services.add("bonjour");
                assertEquals(mValidBonjourServiceRequest, request);
                assertEquals(mValidBonjourServiceResponse, response);
                return mStatusSuccess;
            }
        })
        .when(mISupplicantP2pIfaceMock).addBonjourService(
                any(ArrayList.class), any(ArrayList.class));

        // Default value when service is not initialized.
        assertFalse(mDut.serviceAdd(info));
        executeAndValidateInitializationSequence(false, false, false);
        assertTrue(mDut.serviceAdd(info));
        // Confirm that both services have been added.
        assertTrue(services.contains("upnp"));
        assertTrue(services.contains("bonjour"));

        // Empty services should cause no trouble.
        assertTrue(mDut.serviceAdd(createDummyP2pServiceInfo()));
    }

    /**
     * Test serviceAdd with invalid parameters.
     */
    @Test
    public void testServiceAdd_invalidArguments() throws Exception {
        executeAndValidateInitializationSequence(false, false, false);

        when(mISupplicantP2pIfaceMock.addUpnpService(anyInt(), anyString()))
                .thenReturn(mStatusSuccess);
        when(mISupplicantP2pIfaceMock.addBonjourService(
                any(ArrayList.class), any(ArrayList.class)))
                .thenReturn(mStatusSuccess);

        assertFalse(mDut.serviceAdd(null));
        assertFalse(mDut.serviceAdd(createDummyP2pServiceInfo(mInvalidService1)));
        assertFalse(mDut.serviceAdd(createDummyP2pServiceInfo(mInvalidService2)));
        assertFalse(mDut.serviceAdd(createDummyP2pServiceInfo(mInvalidUpnpService1)));
        assertFalse(mDut.serviceAdd(createDummyP2pServiceInfo(mInvalidUpnpService2)));
        assertFalse(mDut.serviceAdd(createDummyP2pServiceInfo(mInvalidUpnpService3)));
        assertFalse(mDut.serviceAdd(createDummyP2pServiceInfo(mInvalidBonjourService1)));
        assertFalse(mDut.serviceAdd(createDummyP2pServiceInfo(mInvalidBonjourService2)));
        assertFalse(mDut.serviceAdd(createDummyP2pServiceInfo(mInvalidBonjourService3)));
        assertFalse(mDut.serviceAdd(createDummyP2pServiceInfo(mInvalidBonjourService4)));
    }

    /**
     * Verify that serviceAdd returns false, if status is not SUCCESS.
     */
    @Test
    public void testServiceAdd_failure() throws Exception {
        executeAndValidateInitializationSequence(false, false, false);

        when(mISupplicantP2pIfaceMock.addUpnpService(anyInt(), anyString()))
                .thenReturn(mStatusFailure);
        when(mISupplicantP2pIfaceMock.addBonjourService(
                any(ArrayList.class), any(ArrayList.class)))
                .thenReturn(mStatusFailure);

        assertFalse(mDut.serviceAdd(createDummyP2pServiceInfo(mValidUpnpService)));
        assertFalse(mDut.serviceAdd(createDummyP2pServiceInfo(mValidBonjourService)));

        // Check that service is still alive.
        assertTrue(mDut.isInitializationComplete());
    }

    /**
     * Verify that serviceAdd disconnects and returns false, if HAL throws exception.
     */
    @Test
    public void testServiceAdd_exception() throws Exception {
        executeAndValidateInitializationSequence(false, false, false);

        when(mISupplicantP2pIfaceMock.addUpnpService(anyInt(), anyString()))
                .thenThrow(mRemoteException);
        assertFalse(mDut.serviceAdd(createDummyP2pServiceInfo(mValidUpnpService)));
        // Check service is dead.
        assertFalse(mDut.isInitializationComplete());

        executeAndValidateInitializationSequence(false, false, false);
        when(mISupplicantP2pIfaceMock.addBonjourService(
                any(ArrayList.class), any(ArrayList.class)))
                .thenThrow(mRemoteException);
        assertFalse(mDut.serviceAdd(createDummyP2pServiceInfo(mValidBonjourService)));
        // Check service is dead.
        assertFalse(mDut.isInitializationComplete());
    }


    /**
     * Sunny day scenario for serviceRemove()
     */
    @Test
    public void testServiceRemove_success() throws Exception {
        WifiP2pServiceInfo info = createDummyP2pServiceInfo(
                mValidUpnpService, mValidBonjourService);
        final HashSet<String> services = new HashSet<String>();

        doAnswer(new AnswerWithArguments() {
            public SupplicantStatus answer(int version, String name) {
                services.add("upnp");
                assertEquals(mValidUpnpServiceVersion, version);
                assertEquals(mValidUpnpServiceName, name);
                return mStatusSuccess;
            }
        })
        .when(mISupplicantP2pIfaceMock).removeUpnpService(anyInt(), anyString());

        doAnswer(new AnswerWithArguments() {
            public SupplicantStatus answer(ArrayList<Byte> request) {
                services.add("bonjour");
                assertEquals(mValidBonjourServiceRequest, request);
                return mStatusSuccess;
            }
        })
        .when(mISupplicantP2pIfaceMock).removeBonjourService(any(ArrayList.class));

        // Default value when service is not initialized.
        assertFalse(mDut.serviceRemove(info));
        executeAndValidateInitializationSequence(false, false, false);
        assertTrue(mDut.serviceRemove(info));
        // Confirm that both services have been removed.
        assertTrue(services.contains("upnp"));
        assertTrue(services.contains("bonjour"));

        // Empty services should cause no trouble.
        assertTrue(mDut.serviceRemove(createDummyP2pServiceInfo()));
    }

    /**
     * Test serviceRemove with invalid parameters.
     */
    @Test
    public void testServiceRemove_invalidArguments() throws Exception {
        executeAndValidateInitializationSequence(false, false, false);

        when(mISupplicantP2pIfaceMock.removeUpnpService(anyInt(), anyString()))
                .thenReturn(mStatusSuccess);
        when(mISupplicantP2pIfaceMock.removeBonjourService(any(ArrayList.class)))
                .thenReturn(mStatusSuccess);

        assertFalse(mDut.serviceRemove(null));
        assertFalse(mDut.serviceRemove(createDummyP2pServiceInfo(mInvalidService1)));
        assertFalse(mDut.serviceRemove(createDummyP2pServiceInfo(mInvalidService2)));
        assertFalse(mDut.serviceRemove(createDummyP2pServiceInfo(mInvalidUpnpService1)));
        assertFalse(mDut.serviceRemove(createDummyP2pServiceInfo(mInvalidUpnpService2)));
        assertFalse(mDut.serviceRemove(createDummyP2pServiceInfo(mInvalidUpnpService3)));
        assertFalse(mDut.serviceRemove(createDummyP2pServiceInfo(mInvalidBonjourService1)));
        assertFalse(mDut.serviceRemove(createDummyP2pServiceInfo(mInvalidBonjourService2)));
        assertFalse(mDut.serviceRemove(createDummyP2pServiceInfo(mInvalidBonjourService3)));
        // Response parameter is ignored by serviceRemove call, hence the following would pass.
        // The production code would need to parse otherwise redundant parameter to fail on this
        // one.
        //
        // assertFalse(mDut.serviceRemove(createDummyP2pServiceInfo(mInvalidBonjourService4)));
    }

    /**
     * Verify that serviceRemove returns false, if status is not SUCCESS.
     */
    @Test
    public void testServiceRemove_failure() throws Exception {
        executeAndValidateInitializationSequence(false, false, false);

        when(mISupplicantP2pIfaceMock.removeUpnpService(anyInt(), anyString()))
                .thenReturn(mStatusFailure);
        when(mISupplicantP2pIfaceMock.removeBonjourService(any(ArrayList.class)))
                .thenReturn(mStatusFailure);

        assertFalse(mDut.serviceRemove(createDummyP2pServiceInfo(mValidUpnpService)));
        assertFalse(mDut.serviceRemove(createDummyP2pServiceInfo(mValidBonjourService)));

        // Check that service is still alive.
        assertTrue(mDut.isInitializationComplete());
    }

    /**
     * Verify that serviceRemove disconnects and returns false, if HAL throws exception.
     */
    @Test
    public void testServiceRemove_exception() throws Exception {
        executeAndValidateInitializationSequence(false, false, false);

        when(mISupplicantP2pIfaceMock.removeUpnpService(anyInt(), anyString()))
                .thenThrow(mRemoteException);
        assertFalse(mDut.serviceRemove(createDummyP2pServiceInfo(mValidUpnpService)));
        // Check service is dead.
        assertFalse(mDut.isInitializationComplete());

        executeAndValidateInitializationSequence(false, false, false);
        when(mISupplicantP2pIfaceMock.removeBonjourService(any(ArrayList.class)))
                .thenThrow(mRemoteException);
        assertFalse(mDut.serviceRemove(createDummyP2pServiceInfo(mValidBonjourService)));
        // Check service is dead.
        assertFalse(mDut.isInitializationComplete());
    }


    /**
     * Sunny day scenario for requestServiceDiscovery()
     */
    @Test
    public void testRequestServiceDiscovery_success() throws Exception {
        final int caps = 123;

        doAnswer(new AnswerWithArguments() {
            public void answer(byte[] address, ArrayList<Byte> query,
                    ISupplicantP2pIface.requestServiceDiscoveryCallback cb) {
                cb.onValues(mStatusSuccess, 1234);
            }
        })
        .when(mISupplicantP2pIfaceMock)
                .requestServiceDiscovery(
                        eq(mPeerMacAddressBytes),
                        eq(mValidBonjourServiceRequest),
                        any(ISupplicantP2pIface.requestServiceDiscoveryCallback.class));

        // Default value when service is not initialized.
        assertNull(mDut.requestServiceDiscovery(mPeerMacAddress, mValidServiceRequestString));

        executeAndValidateInitializationSequence(false, false, false);
        assertEquals("1234", mDut.requestServiceDiscovery(
                mPeerMacAddress, mValidServiceRequestString));
    }

    /**
     * RequestServiceDiscovery with invalid arguments.
     */
    @Test
    public void testRequestServiceDiscovery_invalidArguments() throws Exception {
        executeAndValidateInitializationSequence(false, false, false);

        doAnswer(new AnswerWithArguments() {
            public void answer(byte[] address, ArrayList<Byte> query,
                    ISupplicantP2pIface.requestServiceDiscoveryCallback cb) {
                cb.onValues(mStatusSuccess, 0);
            }
        })
        .when(mISupplicantP2pIfaceMock)
                .requestServiceDiscovery(
                        any(byte[].class), any(ArrayList.class),
                        any(ISupplicantP2pIface.requestServiceDiscoveryCallback.class));

        for (String address : mInvalidMacAddresses) {
            assertNull(mDut.requestServiceDiscovery(
                    address, mValidServiceRequestString));
        }
        assertNull(mDut.requestServiceDiscovery(mPeerMacAddress, null));
        assertNull(mDut.requestServiceDiscovery(mPeerMacAddress, mInvalidServiceRequestString));
    }

    /**
     * Verify that requestServiceDiscovery returns false, if status is not SUCCESS.
     */
    @Test
    public void testRequestServiceDiscovery_failure() throws Exception {
        executeAndValidateInitializationSequence(false, false, false);

        doAnswer(new AnswerWithArguments() {
            public void answer(
                    byte[] address, ArrayList<Byte> query,
                    ISupplicantP2pIface.requestServiceDiscoveryCallback cb) {
                cb.onValues(mStatusFailure, 0);
            }
        })
        .when(mISupplicantP2pIfaceMock)
                .requestServiceDiscovery(
                        any(byte[].class), any(ArrayList.class),
                        any(ISupplicantP2pIface.requestServiceDiscoveryCallback.class));

        assertNull(mDut.requestServiceDiscovery(mPeerMacAddress, mValidServiceRequestString));
        // Check that service is still alive.
        assertTrue(mDut.isInitializationComplete());
    }

    /**
     * Verify that requestServiceDiscovery disconnects and returns false, if HAL throws exception.
     */
    @Test
    public void testRequestServiceDiscovery_exception() throws Exception {
        executeAndValidateInitializationSequence(false, false, false);
        doThrow(mRemoteException)
                .when(mISupplicantP2pIfaceMock)
                .requestServiceDiscovery(
                        any(byte[].class), any(ArrayList.class),
                        any(ISupplicantP2pIface.requestServiceDiscoveryCallback.class));
        assertNull(mDut.requestServiceDiscovery(mPeerMacAddress, mValidServiceRequestString));
        // Check service is dead.
        assertFalse(mDut.isInitializationComplete());
    }

    // Test constant used in cancelServiceDiscovery tests
    static final String SERVICE_IDENTIFIER_STR = "521918410304";
    static final long SERVICE_IDENTIFIER_LONG = 521918410304L;

    /**
     * Sunny day scenario for cancelServiceDiscovery()
     */
    @Test
    public void testCancelServiceDiscovery_success() throws Exception {
        when(mISupplicantP2pIfaceMock.cancelServiceDiscovery(SERVICE_IDENTIFIER_LONG))
                .thenReturn(mStatusSuccess);
        // Default value when service is not initialized.
        assertFalse(mDut.cancelServiceDiscovery(SERVICE_IDENTIFIER_STR));
        executeAndValidateInitializationSequence(false, false, false);
        assertTrue(mDut.cancelServiceDiscovery(SERVICE_IDENTIFIER_STR));
    }

    /**
     * Test cancelServiceDiscovery with invalid parameters.
     */
    @Test
    public void testCancelServiceDiscovery_invalidArguments() throws Exception {
        executeAndValidateInitializationSequence(false, false, false);
        when(mISupplicantP2pIfaceMock.cancelServiceDiscovery(anyLong()))
                .thenReturn(mStatusFailure);
        assertFalse(mDut.cancelServiceDiscovery(null));
        assertFalse(mDut.cancelServiceDiscovery("not a number"));
    }

    /**
     * Verify that cancelServiceDiscovery returns false, if status is not SUCCESS.
     */
    @Test
    public void testCancelServiceDiscovery_failure() throws Exception {
        executeAndValidateInitializationSequence(false, false, false);
        when(mISupplicantP2pIfaceMock.cancelServiceDiscovery(anyLong()))
                .thenReturn(mStatusFailure);
        assertFalse(mDut.cancelServiceDiscovery(SERVICE_IDENTIFIER_STR));
        // Check that service is still alive.
        assertTrue(mDut.isInitializationComplete());
    }

    /**
     * Verify that cancelServiceDiscovery disconnects and returns false, if HAL throws exception.
     */
    @Test
    public void testCancelServiceDiscovery_exception() throws Exception {
        executeAndValidateInitializationSequence(false, false, false);
        when(mISupplicantP2pIfaceMock.cancelServiceDiscovery(anyLong()))
                .thenThrow(mRemoteException);
        assertFalse(mDut.cancelServiceDiscovery(SERVICE_IDENTIFIER_STR));
        // Check service is dead.
        assertFalse(mDut.isInitializationComplete());
    }


    /**
     * Sunny day scenario for setMiracastMode()
     */
    @Test
    public void testSetMiracastMode_success() throws Exception {
        HashSet<Byte> modes = new HashSet<Byte>();

        when(mISupplicantP2pIfaceMock.setMiracastMode(anyByte()))
                .thenAnswer(new AnswerWithArguments() {
                    public SupplicantStatus answer(byte mode) {
                        modes.add(mode);
                        return mStatusSuccess;
                    }
                });
        // Default value when service is not initialized.
        assertFalse(mDut.setMiracastMode(WifiP2pManager.MIRACAST_SOURCE));
        executeAndValidateInitializationSequence(false, false, false);
        assertTrue(mDut.setMiracastMode(WifiP2pManager.MIRACAST_SOURCE));
        assertTrue(modes.contains(ISupplicantP2pIface.MiracastMode.SOURCE));

        assertTrue(mDut.setMiracastMode(WifiP2pManager.MIRACAST_SINK));
        assertTrue(modes.contains(ISupplicantP2pIface.MiracastMode.SINK));

        // Any invalid number yields disabled miracast mode.
        assertTrue(mDut.setMiracastMode(-1));
        assertTrue(modes.contains(ISupplicantP2pIface.MiracastMode.DISABLED));
    }

    /**
     * Verify that setMiracastMode returns false, if status is not SUCCESS.
     */
    @Test
    public void testSetMiracastMode_failure() throws Exception {
        executeAndValidateInitializationSequence(false, false, false);
        when(mISupplicantP2pIfaceMock.setMiracastMode(anyByte()))
                .thenReturn(mStatusFailure);
        assertFalse(mDut.setMiracastMode(WifiP2pManager.MIRACAST_SOURCE));
        // Check that service is still alive.
        assertTrue(mDut.isInitializationComplete());
    }

    /**
     * Verify that setMiracastMode disconnects and returns false, if HAL throws exception.
     */
    @Test
    public void testSetMiracastMode_exception() throws Exception {
        executeAndValidateInitializationSequence(false, false, false);
        when(mISupplicantP2pIfaceMock.setMiracastMode(anyByte()))
                .thenThrow(mRemoteException);
        assertFalse(mDut.setMiracastMode(WifiP2pManager.MIRACAST_SOURCE));
        // Check service is dead.
        assertFalse(mDut.isInitializationComplete());
    }


    /**
     * Sunny day scenario for startWpsPbc()
     */
    @Test
    public void testStartWpsPbc_success() throws Exception {
        when(mISupplicantP2pIfaceMock.startWpsPbc(eq(mIfaceName), eq(mPeerMacAddressBytes)))
                .thenReturn(mStatusSuccess);
        // Default value when service is not initialized.
        assertFalse(mDut.startWpsPbc(mIfaceName, mPeerMacAddress));
        executeAndValidateInitializationSequence(false, false, false);
        assertTrue(mDut.startWpsPbc(mIfaceName, mPeerMacAddress));
    }

    /**
     * StartWpsPbc with invalid arguments.
     */
    @Test
    public void testStartWpsPbc_invalidArguments() throws Exception {
        executeAndValidateInitializationSequence(false, false, false);
        when(mISupplicantP2pIfaceMock.startWpsPbc(anyString(), any(byte[].class)))
                .thenReturn(mStatusSuccess);

        for (String address : mInvalidMacAddresses) {
            assertFalse(mDut.startWpsPbc(mIfaceName, address));
        }

        assertFalse(mDut.startWpsPbc(null, mPeerMacAddress));
    }

    /**
     * Verify that startWpsPbc returns false, if status is not SUCCESS.
     */
    @Test
    public void testStartWpsPbc_failure() throws Exception {
        executeAndValidateInitializationSequence(false, false, false);
        when(mISupplicantP2pIfaceMock.startWpsPbc(anyString(), any(byte[].class)))
                .thenReturn(mStatusFailure);
        assertFalse(mDut.startWpsPbc(mIfaceName, mPeerMacAddress));
        // Check that service is still alive.
        assertTrue(mDut.isInitializationComplete());
    }

    /**
     * Verify that startWpsPbc disconnects and returns false, if HAL throws exception.
     */
    @Test
    public void testStartWpsPbc_exception() throws Exception {
        executeAndValidateInitializationSequence(false, false, false);
        when(mISupplicantP2pIfaceMock.startWpsPbc(anyString(), any(byte[].class)))
                .thenThrow(mRemoteException);
        assertFalse(mDut.startWpsPbc(mIfaceName, mPeerMacAddress));
        // Check service is dead.
        assertFalse(mDut.isInitializationComplete());
    }


    /**
     * Sunny day scenario for startWpsPinKeypad()
     */
    @Test
    public void testStartWpsPinKeypad_success() throws Exception {
        when(mISupplicantP2pIfaceMock.startWpsPinKeypad(eq(mIfaceName), eq("1234")))
                .thenReturn(mStatusSuccess);
        // Default value when service is not initialized.
        assertFalse(mDut.startWpsPinKeypad(mIfaceName, "1234"));
        executeAndValidateInitializationSequence(false, false, false);
        assertTrue(mDut.startWpsPinKeypad(mIfaceName, "1234"));
    }

    /**
     * StartWpsPinKeypad with invalid arguments.
     */
    @Test
    public void testStartWpsPinKeypad_invalidArguments() throws Exception {
        executeAndValidateInitializationSequence(false, false, false);
        when(mISupplicantP2pIfaceMock.startWpsPinKeypad(anyString(), anyString()))
                .thenReturn(mStatusSuccess);

        assertFalse(mDut.startWpsPinKeypad(null, "1234"));
        assertFalse(mDut.startWpsPinKeypad(mIfaceName, null));
        // StartWpsPinPinKeypad does not validate, that PIN indeed holds an integer encoded in a
        // string. This code would be redundant, as HAL requires string to be passed.
    }

    /**
     * Verify that startWpsPinKeypad returns false, if status is not SUCCESS.
     */
    @Test
    public void testStartWpsPinKeypad_failure() throws Exception {
        executeAndValidateInitializationSequence(false, false, false);
        when(mISupplicantP2pIfaceMock.startWpsPinKeypad(anyString(), anyString()))
                .thenReturn(mStatusFailure);
        assertFalse(mDut.startWpsPinKeypad(mIfaceName, "1234"));
        // Check that service is still alive.
        assertTrue(mDut.isInitializationComplete());
    }

    /**
     * Verify that startWpsPinKeypad disconnects and returns false, if HAL throws exception.
     */
    @Test
    public void testStartWpsPinKeypad_exception() throws Exception {
        executeAndValidateInitializationSequence(false, false, false);
        when(mISupplicantP2pIfaceMock.startWpsPinKeypad(anyString(), anyString()))
                .thenThrow(mRemoteException);
        assertFalse(mDut.startWpsPinKeypad(mIfaceName, "1234"));
        // Check service is dead.
        assertFalse(mDut.isInitializationComplete());
    }


    /**
     * Sunny day scenario for startWpsPinDisplay()
     */
    @Test
    public void testStartWpsPinDisplay_success() throws Exception {
        doAnswer(new AnswerWithArguments() {
            public void answer(String ifName, byte[] bssid,
                    ISupplicantP2pIface.startWpsPinDisplayCallback cb) {
                cb.onValues(mStatusSuccess, "1234");
            }
        })
        .when(mISupplicantP2pIfaceMock).startWpsPinDisplay(
                eq(mIfaceName), eq(mPeerMacAddressBytes),
                any(ISupplicantP2pIface.startWpsPinDisplayCallback.class));

        // Default value when service is not initialized.
        assertNull(mDut.startWpsPinDisplay(mIfaceName, mPeerMacAddress));
        executeAndValidateInitializationSequence(false, false, false);
        assertEquals("1234", mDut.startWpsPinDisplay(mIfaceName, mPeerMacAddress));
    }

    /**
     * StartWpsPinDisplay with invalid arguments.
     */
    @Test
    public void testStartWpsPinDisplay_invalidArguments() throws Exception {
        executeAndValidateInitializationSequence(false, false, false);
        doAnswer(new AnswerWithArguments() {
            public void answer(String ifName, byte[] bssid,
                    ISupplicantP2pIface.startWpsPinDisplayCallback cb) {
                cb.onValues(mStatusSuccess, "1234");
            }
        })
        .when(mISupplicantP2pIfaceMock).startWpsPinDisplay(
                anyString(), any(byte[].class),
                any(ISupplicantP2pIface.startWpsPinDisplayCallback.class));

        for (String address : mInvalidMacAddresses) {
            assertNull(mDut.startWpsPinDisplay(mIfaceName, address));
        }

        assertNull(mDut.startWpsPinDisplay(null, mPeerMacAddress));
    }

    /**
     * Verify that startWpsPinDisplay returns false, if status is not SUCCESS.
     */
    @Test
    public void testStartWpsPinDisplay_failure() throws Exception {
        executeAndValidateInitializationSequence(false, false, false);
        doAnswer(new AnswerWithArguments() {
            public void answer(String ifName, byte[] bssid,
                    ISupplicantP2pIface.startWpsPinDisplayCallback cb) {
                cb.onValues(mStatusFailure, "1234");
            }
        })
        .when(mISupplicantP2pIfaceMock).startWpsPinDisplay(
                anyString(), any(byte[].class),
                any(ISupplicantP2pIface.startWpsPinDisplayCallback.class));

        assertNull(mDut.startWpsPinDisplay(mIfaceName, mPeerMacAddress));
        // Check that service is still alive.
        assertTrue(mDut.isInitializationComplete());
    }

    /**
     * Verify that startWpsPinDisplay disconnects and returns false, if HAL throws exception.
     */
    @Test
    public void testStartWpsPinDisplay_exception() throws Exception {
        executeAndValidateInitializationSequence(false, false, false);
        doThrow(mRemoteException)
                .when(mISupplicantP2pIfaceMock).startWpsPinDisplay(
                        anyString(), any(byte[].class),
                        any(ISupplicantP2pIface.startWpsPinDisplayCallback.class));
        assertNull(mDut.startWpsPinDisplay(mIfaceName, mPeerMacAddress));
        // Check service is dead.
        assertFalse(mDut.isInitializationComplete());
    }


    /**
     * Sunny day scenario for cancelWps()
     */
    @Test
    public void testCancelWps_success() throws Exception {
        when(mISupplicantP2pIfaceMock.cancelWps(eq(mIfaceName)))
                .thenReturn(mStatusSuccess);
        // Default value when service is not initialized.
        assertFalse(mDut.cancelWps(mIfaceName));
        executeAndValidateInitializationSequence(false, false, false);
        assertTrue(mDut.cancelWps(mIfaceName));
    }

    /**
     * CancelWps with invalid arguments.
     */
    @Test
    public void testCancelWps_invalidArguments() throws Exception {
        executeAndValidateInitializationSequence(false, false, false);
        when(mISupplicantP2pIfaceMock.cancelWps(anyString()))
                .thenReturn(mStatusSuccess);

        assertFalse(mDut.cancelWps(null));
    }

    /**
     * Verify that cancelWps returns false, if status is not SUCCESS.
     */
    @Test
    public void testCancelWps_failure() throws Exception {
        executeAndValidateInitializationSequence(false, false, false);
        when(mISupplicantP2pIfaceMock.cancelWps(anyString()))
                .thenReturn(mStatusFailure);
        assertFalse(mDut.cancelWps(mIfaceName));
        // Check that service is still alive.
        assertTrue(mDut.isInitializationComplete());
    }

    /**
     * Verify that cancelWps disconnects and returns false, if HAL throws exception.
     */
    @Test
    public void testCancelWps_exception() throws Exception {
        executeAndValidateInitializationSequence(false, false, false);
        when(mISupplicantP2pIfaceMock.cancelWps(anyString()))
                .thenThrow(mRemoteException);
        assertFalse(mDut.cancelWps(mIfaceName));
        // Check service is dead.
        assertFalse(mDut.isInitializationComplete());
    }


    /**
     * Sunny day scenario for enableWfd()
     */
    @Test
    public void testEnableWfd_success() throws Exception {
        when(mISupplicantP2pIfaceMock.enableWfd(eq(true)))
                .thenReturn(mStatusSuccess);
        // Default value when service is not initialized.
        assertFalse(mDut.enableWfd(true));
        executeAndValidateInitializationSequence(false, false, false);
        assertTrue(mDut.enableWfd(true));
    }

    /**
     * Verify that enableWfd returns false, if status is not SUCCESS.
     */
    @Test
    public void testEnableWfd_failure() throws Exception {
        executeAndValidateInitializationSequence(false, false, false);
        when(mISupplicantP2pIfaceMock.enableWfd(anyBoolean()))
                .thenReturn(mStatusFailure);
        assertFalse(mDut.enableWfd(true));
        // Check that service is still alive.
        assertTrue(mDut.isInitializationComplete());
    }

    /**
     * Verify that enableWfd disconnects and returns false, if HAL throws exception.
     */
    @Test
    public void testEnableWfd_exception() throws Exception {
        executeAndValidateInitializationSequence(false, false, false);
        when(mISupplicantP2pIfaceMock.enableWfd(anyBoolean()))
                .thenThrow(mRemoteException);
        assertFalse(mDut.enableWfd(false));
        // Check service is dead.
        assertFalse(mDut.isInitializationComplete());
    }


    /**
     * Sunny day scenario for setWfdDeviceInfo()
     */
    @Test
    public void testSetWfdDeviceInfo_success() throws Exception {
        when(mISupplicantP2pIfaceMock.setWfdDeviceInfo(eq(mValidServiceRequestBytes)))
                .thenReturn(mStatusSuccess);
        // Default value when service is not initialized.
        assertFalse(mDut.setWfdDeviceInfo(mValidServiceRequestString));
        executeAndValidateInitializationSequence(false, false, false);
        assertTrue(mDut.setWfdDeviceInfo(mValidServiceRequestString));
    }

    /**
     * SetWfdDeviceInfo with invalid arguments.
     */
    @Test
    public void testSetWfdDeviceInfo_invalidArguments() throws Exception {
        executeAndValidateInitializationSequence(false, false, false);
        when(mISupplicantP2pIfaceMock.setWfdDeviceInfo(any(byte[].class)))
                .thenReturn(mStatusSuccess);

        assertFalse(mDut.setWfdDeviceInfo(null));
        assertFalse(mDut.setWfdDeviceInfo(mInvalidServiceRequestString));
    }

    /**
     * Verify that setWfdDeviceInfo returns false, if status is not SUCCESS.
     */
    @Test
    public void testSetWfdDeviceInfo_failure() throws Exception {
        executeAndValidateInitializationSequence(false, false, false);
        when(mISupplicantP2pIfaceMock.setWfdDeviceInfo(any(byte[].class)))
                .thenReturn(mStatusFailure);
        assertFalse(mDut.setWfdDeviceInfo(mValidServiceRequestString));
        // Check that service is still alive.
        assertTrue(mDut.isInitializationComplete());
    }

    /**
     * Verify that setWfdDeviceInfo disconnects and returns false, if HAL throws exception.
     */
    @Test
    public void testSetWfdDeviceInfo_exception() throws Exception {
        executeAndValidateInitializationSequence(false, false, false);
        when(mISupplicantP2pIfaceMock.setWfdDeviceInfo(any(byte[].class)))
                .thenThrow(mRemoteException);
        assertFalse(mDut.setWfdDeviceInfo(mValidServiceRequestString));
        // Check service is dead.
        assertFalse(mDut.isInitializationComplete());
    }

    /**
     * Verify the loading of group info.
     * Specifically, all groups returned by listNetworks are added as a persistent group, so long as
     * they are NOT current.
     */
    @Test
    public void testLoadGroups() throws Exception {
        executeAndValidateInitializationSequence(false, false, false);

        // Class to hold the P2p group info returned from the HIDL interface.
        class P2pGroupInfo {
            public String ssid;
            public byte[] bssid;
            public boolean isGo;
            public boolean isCurrent;
            P2pGroupInfo(String ssid, byte[] bssid, boolean isGo, boolean isCurrent) {
                this.ssid = ssid;
                this.bssid = bssid;
                this.isGo = isGo;
                this.isCurrent = isCurrent;
            }
        }

        Map<Integer, P2pGroupInfo> groups = new HashMap<>();
        groups.put(0, new P2pGroupInfo(
                "test_34",
                NativeUtil.macAddressToByteArray("56:34:ab:12:12:34"),
                false, false));
        groups.put(1, new P2pGroupInfo(
                "test_1234",
                NativeUtil.macAddressToByteArray("16:ed:ab:12:45:34"),
                true, false));
        groups.put(2, new P2pGroupInfo(
                "test_4545",
                NativeUtil.macAddressToByteArray("32:89:23:56:45:34"),
                true, false));
        groups.put(3, new P2pGroupInfo(
                "iShouldntBeHere",
                NativeUtil.macAddressToByteArray("aa:bb:cc:56:45:34"),
                true, true));

        doAnswer(new AnswerWithArguments() {
            public void answer(ISupplicantP2pIface.listNetworksCallback cb) {
                cb.onValues(mStatusSuccess, new ArrayList<Integer>(groups.keySet()));
            }
        }).when(mISupplicantP2pIfaceMock)
                .listNetworks(any(ISupplicantP2pIface.listNetworksCallback.class));

        doAnswer(new AnswerWithArguments() {
            public void answer(final int networkId, ISupplicantP2pIface.getNetworkCallback cb) {
                try {
                    doAnswer(new AnswerWithArguments() {
                        public void answer(ISupplicantP2pNetwork.getSsidCallback cb) {
                            cb.onValues(mStatusSuccess,
                                    NativeUtil.stringToByteArrayList(groups.get(networkId).ssid));
                            return;
                        }
                    }).when(mISupplicantP2pNetworkMock)
                            .getSsid(any(ISupplicantP2pNetwork.getSsidCallback.class));
                    doAnswer(new AnswerWithArguments() {
                        public void answer(ISupplicantP2pNetwork.getBssidCallback cb) {
                            cb.onValues(mStatusSuccess, groups.get(networkId).bssid);
                            return;
                        }
                    }).when(mISupplicantP2pNetworkMock)
                            .getBssid(any(ISupplicantP2pNetwork.getBssidCallback.class));
                    doAnswer(new AnswerWithArguments() {
                        public void answer(ISupplicantP2pNetwork.isCurrentCallback cb) {
                            cb.onValues(mStatusSuccess, groups.get(networkId).isCurrent);
                            return;
                        }
                    }).when(mISupplicantP2pNetworkMock)
                            .isCurrent(any(ISupplicantP2pNetwork.isCurrentCallback.class));
                    doAnswer(new AnswerWithArguments() {
                        public void answer(ISupplicantP2pNetwork.isGoCallback cb) {
                            cb.onValues(mStatusSuccess, groups.get(networkId).isGo);
                            return;
                        }
                    }).when(mISupplicantP2pNetworkMock)
                            .isGo(any(ISupplicantP2pNetwork.isGoCallback.class));
                } catch (RemoteException e) {
                }
                cb.onValues(mStatusSuccess, mISupplicantP2pNetworkMock);
                return;
            }
        }).when(mISupplicantP2pIfaceMock)
                .getNetwork(anyInt(), any(ISupplicantP2pIface.getNetworkCallback.class));

        WifiP2pGroupList p2pGroups = new WifiP2pGroupList();
        assertTrue(mDut.loadGroups(p2pGroups));

        assertEquals(3, p2pGroups.getGroupList().size());
        for (WifiP2pGroup group : p2pGroups.getGroupList()) {
            int networkId = group.getNetworkId();
            assertEquals(groups.get(networkId).ssid, group.getNetworkName());
            assertEquals(
                    NativeUtil.macAddressFromByteArray(groups.get(networkId).bssid),
                    group.getOwner().deviceAddress);
            assertEquals(groups.get(networkId).isGo, group.isGroupOwner());
        }
    }

    /**
     * Sunny day scenario for setClientList()
     */
    @Test
    public void testSetClientList() throws Exception {
        int testNetworkId = 5;
        final String client1 = mGroupOwnerMacAddress;
        final String client2 = mPeerMacAddress;

        executeAndValidateInitializationSequence(false, false, false);
        doAnswer(new AnswerWithArguments() {
            public void answer(final int networkId, ISupplicantP2pIface.getNetworkCallback cb) {
                if (networkId == testNetworkId) {
                    cb.onValues(mStatusSuccess, mISupplicantP2pNetworkMock);
                } else {
                    cb.onValues(mStatusFailure, null);
                }
                return;
            }
        }).when(mISupplicantP2pIfaceMock)
                .getNetwork(anyInt(), any(ISupplicantP2pIface.getNetworkCallback.class));
        when(mISupplicantP2pNetworkMock.setClientList(any(ArrayList.class)))
                .thenReturn(mStatusSuccess);

        String clientList = client1 + " " + client2;
        assertTrue(mDut.setClientList(testNetworkId, clientList));
        verify(mISupplicantP2pIfaceMock)
                .getNetwork(anyInt(), any(ISupplicantP2pIface.getNetworkCallback.class));
        ArgumentCaptor<ArrayList> capturedClients = ArgumentCaptor.forClass(ArrayList.class);
        verify(mISupplicantP2pNetworkMock).setClientList(capturedClients.capture());

        // Convert these to long to help with comparisons.
        ArrayList<byte[]> clients = capturedClients.getValue();
        ArrayList<Long> expectedClients = new ArrayList<Long>() {{
                add(NativeUtil.macAddressToLong(mGroupOwnerMacAddressBytes));
                add(NativeUtil.macAddressToLong(mPeerMacAddressBytes));
            }};
        ArrayList<Long> receivedClients = new ArrayList<Long>();
        for (byte[] client : clients) {
            receivedClients.add(NativeUtil.macAddressToLong(client));
        }
        assertEquals(expectedClients, receivedClients);
    }

    /**
     * Failure scenario for setClientList() when getNetwork returns null.
     */
    @Test
    public void testSetClientListFailureDueToGetNetwork() throws Exception {
        int testNetworkId = 5;
        final String client1 = mGroupOwnerMacAddress;
        final String client2 = mPeerMacAddress;

        executeAndValidateInitializationSequence(false, false, false);
        doAnswer(new AnswerWithArguments() {
            public void answer(final int networkId, ISupplicantP2pIface.getNetworkCallback cb) {
                cb.onValues(mStatusFailure, null);
                return;
            }
        }).when(mISupplicantP2pIfaceMock)
                .getNetwork(anyInt(), any(ISupplicantP2pIface.getNetworkCallback.class));
        when(mISupplicantP2pNetworkMock.setClientList(any(ArrayList.class)))
                .thenReturn(mStatusSuccess);

        String clientList = client1 + " " + client2;
        assertFalse(mDut.setClientList(testNetworkId, clientList));
        verify(mISupplicantP2pIfaceMock)
                .getNetwork(anyInt(), any(ISupplicantP2pIface.getNetworkCallback.class));
        verify(mISupplicantP2pNetworkMock, never()).setClientList(any(ArrayList.class));
    }

    /**
     * Sunny day scenario for getClientList()
     */
    @Test
    public void testGetClientList() throws Exception {
        int testNetworkId = 5;
        final String client1 = mGroupOwnerMacAddress;
        final String client2 = mPeerMacAddress;

        executeAndValidateInitializationSequence(false, false, false);
        doAnswer(new AnswerWithArguments() {
            public void answer(final int networkId, ISupplicantP2pIface.getNetworkCallback cb) {
                if (networkId == testNetworkId) {
                    cb.onValues(mStatusSuccess, mISupplicantP2pNetworkMock);
                } else {
                    cb.onValues(mStatusFailure, null);
                }
                return;
            }
        }).when(mISupplicantP2pIfaceMock)
                .getNetwork(anyInt(), any(ISupplicantP2pIface.getNetworkCallback.class));
        doAnswer(new AnswerWithArguments() {
            public void answer(ISupplicantP2pNetwork.getClientListCallback cb) {
                ArrayList<byte[]> clients = new ArrayList<byte[]>() {{
                        add(mGroupOwnerMacAddressBytes);
                        add(mPeerMacAddressBytes);
                    }};
                cb.onValues(mStatusSuccess, clients);
                return;
            }
        }).when(mISupplicantP2pNetworkMock)
                .getClientList(any(ISupplicantP2pNetwork.getClientListCallback.class));

        String clientList = client1 + " " + client2;
        assertEquals(clientList, mDut.getClientList(testNetworkId));
        verify(mISupplicantP2pIfaceMock)
                .getNetwork(anyInt(), any(ISupplicantP2pIface.getNetworkCallback.class));
        verify(mISupplicantP2pNetworkMock)
                .getClientList(any(ISupplicantP2pNetwork.getClientListCallback.class));
    }

    /**
     * Failure scenario for getClientList() when getNetwork returns null.
     */
    @Test
    public void testGetClientListFailureDueToGetNetwork() throws Exception {
        int testNetworkId = 5;
        final String client1 = mGroupOwnerMacAddress;
        final String client2 = mPeerMacAddress;

        executeAndValidateInitializationSequence(false, false, false);
        doAnswer(new AnswerWithArguments() {
            public void answer(final int networkId, ISupplicantP2pIface.getNetworkCallback cb) {
                cb.onValues(mStatusFailure, null);
                return;
            }
        }).when(mISupplicantP2pIfaceMock)
                .getNetwork(anyInt(), any(ISupplicantP2pIface.getNetworkCallback.class));
        doAnswer(new AnswerWithArguments() {
            public void answer(ISupplicantP2pNetwork.getClientListCallback cb) {
                ArrayList<byte[]> clients = new ArrayList<byte[]>() {{
                        add(mGroupOwnerMacAddressBytes);
                        add(mPeerMacAddressBytes);
                    }};
                cb.onValues(mStatusSuccess, clients);
                return;
            }
        }).when(mISupplicantP2pNetworkMock)
                .getClientList(any(ISupplicantP2pNetwork.getClientListCallback.class));

        assertEquals(null, mDut.getClientList(testNetworkId));
        verify(mISupplicantP2pIfaceMock)
                .getNetwork(anyInt(), any(ISupplicantP2pIface.getNetworkCallback.class));
        verify(mISupplicantP2pNetworkMock, never())
                .getClientList(any(ISupplicantP2pNetwork.getClientListCallback.class));
    }

    /**
     * Sunny day scenario for saveConfig()
     */
    @Test
    public void testSaveConfig() throws Exception {
        when(mISupplicantP2pIfaceMock.saveConfig()).thenReturn(mStatusSuccess);

        // Should fail before initialization.
        assertFalse(mDut.saveConfig());
        executeAndValidateInitializationSequence(false, false, false);
        assertTrue(mDut.saveConfig());
        verify(mISupplicantP2pIfaceMock).saveConfig();
    }

    /**
     * Sunny day scenario for setMacRandomization()
     */
    @Test
    public void testEnableMacRandomization() throws Exception {
        when(mISupplicantP2pIfaceMockV12.setMacRandomization(anyBoolean()))
                .thenReturn(mStatusSuccess);

        // Should fail before initialization.
        assertFalse(mDut.setMacRandomization(true));
        executeAndValidateInitializationSequence(false, false, false);
        assertTrue(mDut.setMacRandomization(true));
        verify(mISupplicantP2pIfaceMockV12).setMacRandomization(eq(true));
    }

    /**
     * Calls.initialize(), mocking various call back answers and verifying flow, asserting for the
     * expected result. Verifies if ISupplicantP2pIface manager is initialized or reset.
     * Each of the arguments will cause a different failure mode when set true.
     */
    private void executeAndValidateInitializationSequence(boolean causeRemoteException,
            boolean getZeroInterfaces, boolean getNullInterface) throws Exception {
        boolean shouldSucceed = !causeRemoteException && !getZeroInterfaces && !getNullInterface;
        // Setup callback mock answers
        ArrayList<ISupplicant.IfaceInfo> interfaces;
        if (getZeroInterfaces) {
            interfaces = new ArrayList<ISupplicant.IfaceInfo>();
        } else {
            interfaces = mIfaceInfoList;
        }

        doAnswer(new AnswerWithArguments() {
            public void answer(ISupplicant.listInterfacesCallback cb) throws RemoteException {
                cb.onValues(mStatusSuccess, interfaces);
            }
        })
        .when(mISupplicantMock).listInterfaces(any(ISupplicant.listInterfacesCallback.class));

        if (causeRemoteException) {
            doThrow(new RemoteException("Some error!!!"))
                    .when(mISupplicantMock).getInterface(any(ISupplicant.IfaceInfo.class),
                    any(ISupplicant.getInterfaceCallback.class));
        } else {
            doAnswer(new GetGetInterfaceAnswer(getNullInterface))
                    .when(mISupplicantMock).getInterface(any(ISupplicant.IfaceInfo.class),
                    any(ISupplicant.getInterfaceCallback.class));
        }

        mInOrder = inOrder(mServiceManagerMock, mISupplicantMock, mISupplicantP2pIfaceMock);
        // Initialize SupplicantP2pIfaceHal, should call serviceManager.registerForNotifications
        assertTrue(mDut.initialize());
        // verify: service manager initialization sequence
        mInOrder.verify(mServiceManagerMock).linkToDeath(
                any(IHwBinder.DeathRecipient.class), anyLong());
        mInOrder.verify(mServiceManagerMock).registerForNotifications(
                eq(ISupplicant.kInterfaceName), eq(""), mServiceNotificationCaptor.capture());
        // act: cause the onRegistration(...) callback to execute
        mServiceNotificationCaptor.getValue().onRegistration(ISupplicant.kInterfaceName, "", true);
        mInOrder.verify(mISupplicantMock).linkToDeath(
                any(IHwBinder.DeathRecipient.class), anyLong());
        assertEquals(true, mDut.isInitializationComplete());

        // Now setup the iface.
        assertTrue(mDut.setupIface(mIfaceName) == shouldSucceed);

        // verify: listInterfaces is called
        mInOrder.verify(mISupplicantMock).listInterfaces(
                any(ISupplicant.listInterfacesCallback.class));
        if (!causeRemoteException && !getNullInterface && !getZeroInterfaces) {
            mInOrder.verify(mISupplicantMock)
                    .getInterface(any(ISupplicant.IfaceInfo.class),
                    any(ISupplicant.getInterfaceCallback.class));
            mInOrder.verify(mISupplicantP2pIfaceMock).linkToDeath(
                    any(IHwBinder.DeathRecipient.class), anyLong());
            mInOrder.verify(mISupplicantP2pIfaceMock).registerCallback(
                    any(ISupplicantP2pIfaceCallback.class));
        }

        // if no errors, V1_2 interface is on as well
        mISupplicantV12Enabled = true;
    }

    /**
     * Calls.initialize(), mocking various call back answers and verifying flow, asserting for the
     * expected result. Verifies if ISupplicantP2pIface manager is initialized or reset.
     * Each of the arguments will cause a different failure mode when set true.
     */
    private void executeAndValidateInitializationSequenceV1_1(boolean causeRemoteException,
                                                               boolean getNullInterface)
            throws Exception {
        boolean shouldSucceed = !causeRemoteException && !getNullInterface;
        // Setup callback mock answers
        if (causeRemoteException) {
            doThrow(new RemoteException("Some error!!!"))
                    .when(mISupplicantMockV1_1).addInterface(any(ISupplicant.IfaceInfo.class),
                    any(android.hardware.wifi.supplicant.V1_1.ISupplicant
                            .addInterfaceCallback.class));
        } else {
            doAnswer(new GetAddInterfaceAnswer(getNullInterface))
                    .when(mISupplicantMockV1_1).addInterface(any(ISupplicant.IfaceInfo.class),
                    any(android.hardware.wifi.supplicant.V1_1.ISupplicant
                            .addInterfaceCallback.class));
        }

        mInOrder = inOrder(mServiceManagerMock, mISupplicantMock, mISupplicantMockV1_1,
                mISupplicantP2pIfaceMock);
        // Initialize SupplicantStaIfaceHal, should call serviceManager.registerForNotifications
        assertTrue(mDut.initialize());
        // verify: service manager initialization sequence
        mInOrder.verify(mServiceManagerMock).linkToDeath(
                any(IHwBinder.DeathRecipient.class), anyLong());
        mInOrder.verify(mServiceManagerMock).registerForNotifications(
                eq(ISupplicant.kInterfaceName), eq(""), mServiceNotificationCaptor.capture());
        // act: cause the onRegistration(...) callback to execute
        mServiceNotificationCaptor.getValue().onRegistration(ISupplicant.kInterfaceName, "", true);
        mInOrder.verify(mISupplicantMock).linkToDeath(
                any(IHwBinder.DeathRecipient.class), anyLong());
        assertEquals(true, mDut.isInitializationComplete());

        // Now setup the iface.
        assertTrue(mDut.setupIface(mIfaceName) == shouldSucceed);

        // verify: addInterface is called
        mInOrder.verify(mISupplicantMockV1_1)
                .addInterface(any(ISupplicant.IfaceInfo.class),
                        any(android.hardware.wifi.supplicant.V1_1.ISupplicant
                                .addInterfaceCallback.class));
        if (!causeRemoteException && !getNullInterface) {
            mInOrder.verify(mISupplicantP2pIfaceMock).linkToDeath(
                    any(IHwBinder.DeathRecipient.class), anyLong());
            mInOrder.verify(mISupplicantP2pIfaceMock).registerCallback(
                    any(ISupplicantP2pIfaceCallback.class));
        }

        // Ensure we don't try to use the listInterfaces method from 1.0 version.
        verify(mISupplicantMock, never()).listInterfaces(
                any(ISupplicant.listInterfacesCallback.class));
        verify(mISupplicantMock, never()).getInterface(any(ISupplicant.IfaceInfo.class),
                any(ISupplicant.getInterfaceCallback.class));
    }

    private SupplicantStatus createSupplicantStatus(int code) {
        SupplicantStatus status = new SupplicantStatus();
        status.code = code;
        return status;
    }

    /**
     * Create an IfaceInfo with given type and name
     */
    private ISupplicant.IfaceInfo createIfaceInfo(int type, String name) {
        ISupplicant.IfaceInfo info = new ISupplicant.IfaceInfo();
        info.type = type;
        info.name = name;
        return info;
    }

    /**
     * Create new dummy WifiP2pConfig instance.
     */
    private WifiP2pConfig createDummyP2pConfig(String peerAddress, int wpsProvMethod, String pin) {
        WifiP2pConfig config = new WifiP2pConfig();
        config.wps = new WpsInfo();
        config.deviceAddress = peerAddress;

        config.wps.setup = wpsProvMethod;
        config.wps.pin = pin;

        return config;
    }

    /**
     * Create new dummy WifiP2pGroup instance.
     */
    private WifiP2pGroup createDummyP2pGroup() {
        WifiP2pGroup group = new WifiP2pGroup();
        group.setInterface(mIfaceName);

        WifiP2pDevice owner = new WifiP2pDevice();
        owner.deviceAddress = mGroupOwnerMacAddress;
        group.setOwner(owner);

        return group;
    }

    /**
     * Create new dummy WifiP2pServiceInfo instance.
     */
    private WifiP2pServiceInfo createDummyP2pServiceInfo(String... services) {
        class TestP2pServiceInfo extends WifiP2pServiceInfo {
            TestP2pServiceInfo(String[] services) {
                super(Arrays.asList(services));
            }
        }
        return new TestP2pServiceInfo(services);
    }

    private class GetGetInterfaceAnswer extends AnswerWithArguments {
        boolean mGetNullInterface;

        GetGetInterfaceAnswer(boolean getNullInterface) {
            mGetNullInterface = getNullInterface;
        }

        public void answer(ISupplicant.IfaceInfo iface, ISupplicant.getInterfaceCallback cb) {
            if (mGetNullInterface) {
                cb.onValues(mStatusSuccess, null);
            } else {
                cb.onValues(mStatusSuccess, mISupplicantIfaceMock);
            }
        }
    }

    private class GetAddInterfaceAnswer extends AnswerWithArguments {
        boolean mGetNullInterface;

        GetAddInterfaceAnswer(boolean getNullInterface) {
            mGetNullInterface = getNullInterface;
        }

        public void answer(ISupplicant.IfaceInfo iface,
                           android.hardware.wifi.supplicant.V1_1.ISupplicant
                                   .addInterfaceCallback cb) {
            if (mGetNullInterface) {
                cb.onValues(mStatusSuccess, null);
            } else {
                cb.onValues(mStatusSuccess, mISupplicantIfaceMock);
            }
        }
    }
}
