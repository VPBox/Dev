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

package com.android.server.wifi.aware;

import static android.hardware.wifi.V1_0.NanDataPathChannelCfg.CHANNEL_NOT_REQUESTED;

import static org.hamcrest.core.IsEqual.equalTo;
import static org.junit.Assert.assertArrayEquals;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;
import static org.mockito.ArgumentMatchers.any;
import static org.mockito.ArgumentMatchers.anyBoolean;
import static org.mockito.ArgumentMatchers.anyInt;
import static org.mockito.ArgumentMatchers.anyLong;
import static org.mockito.ArgumentMatchers.anyShort;
import static org.mockito.ArgumentMatchers.anyString;
import static org.mockito.ArgumentMatchers.eq;
import static org.mockito.Mockito.atLeastOnce;
import static org.mockito.Mockito.inOrder;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.never;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.verifyNoMoreInteractions;
import static org.mockito.Mockito.when;

import android.Manifest;
import android.app.test.TestAlarmManager;
import android.content.Context;
import android.content.pm.PackageManager;
import android.hardware.wifi.V1_0.NanStatusType;
import android.net.ConnectivityManager;
import android.net.MacAddress;
import android.net.NetworkCapabilities;
import android.net.NetworkFactory;
import android.net.NetworkRequest;
import android.net.NetworkSpecifier;
import android.net.wifi.WifiManager;
import android.net.wifi.aware.AttachCallback;
import android.net.wifi.aware.ConfigRequest;
import android.net.wifi.aware.DiscoverySession;
import android.net.wifi.aware.DiscoverySessionCallback;
import android.net.wifi.aware.IWifiAwareDiscoverySessionCallback;
import android.net.wifi.aware.IWifiAwareEventCallback;
import android.net.wifi.aware.IWifiAwareManager;
import android.net.wifi.aware.PeerHandle;
import android.net.wifi.aware.PublishConfig;
import android.net.wifi.aware.PublishDiscoverySession;
import android.net.wifi.aware.SubscribeConfig;
import android.net.wifi.aware.SubscribeDiscoverySession;
import android.net.wifi.aware.TlvBufferUtils;
import android.net.wifi.aware.WifiAwareManager;
import android.net.wifi.aware.WifiAwareNetworkInfo;
import android.net.wifi.aware.WifiAwareNetworkSpecifier;
import android.net.wifi.aware.WifiAwareSession;
import android.os.Build;
import android.os.Handler;
import android.os.INetworkManagementService;
import android.os.IPowerManager;
import android.os.Message;
import android.os.Messenger;
import android.os.PowerManager;
import android.os.Process;
import android.os.test.TestLooper;
import android.util.Pair;

import androidx.test.filters.SmallTest;

import com.android.internal.util.AsyncChannel;
import com.android.server.wifi.Clock;
import com.android.server.wifi.util.WifiPermissionsUtil;
import com.android.server.wifi.util.WifiPermissionsWrapper;

import libcore.util.HexEncoding;

import org.junit.After;
import org.junit.Before;
import org.junit.Rule;
import org.junit.Test;
import org.junit.rules.ErrorCollector;
import org.mockito.ArgumentCaptor;
import org.mockito.InOrder;
import org.mockito.Mock;
import org.mockito.MockitoAnnotations;
import org.mockito.Spy;

import java.nio.ByteOrder;
import java.util.Arrays;
import java.util.HashSet;
import java.util.Set;


/**
 * Unit test harness for WifiAwareDataPathStateManager class.
 */
@SmallTest
public class WifiAwareDataPathStateManagerTest {
    private static final String sAwareInterfacePrefix = "aware_data";

    private TestLooper mMockLooper;
    private Handler mMockLooperHandler;
    private WifiAwareStateManager mDut;
    @Mock private Clock mClock;
    @Mock private WifiAwareNativeManager mMockNativeManager;
    @Spy private TestUtils.MonitoredWifiAwareNativeApi mMockNative =
            new TestUtils.MonitoredWifiAwareNativeApi();
    @Mock private Context mMockContext;
    @Mock private ConnectivityManager mMockCm;
    @Mock private INetworkManagementService mMockNwMgt;
    @Mock private WifiAwareDataPathStateManager.NetworkInterfaceWrapper mMockNetworkInterface;
    @Mock private IWifiAwareEventCallback mMockCallback;
    @Mock IWifiAwareDiscoverySessionCallback mMockSessionCallback;
    @Mock private WifiAwareMetrics mAwareMetricsMock;
    @Mock private WifiPermissionsUtil mWifiPermissionsUtil;
    @Mock private WifiPermissionsWrapper mPermissionsWrapperMock;
    @Mock private WifiManager mMockWifiManager;
    TestAlarmManager mAlarmManager;
    private PowerManager mMockPowerManager;

    @Rule
    public ErrorCollector collector = new ErrorCollector();

    /**
     * Initialize mocks.
     */
    @Before
    public void setUp() throws Exception {
        MockitoAnnotations.initMocks(this);

        mAlarmManager = new TestAlarmManager();
        when(mMockContext.getSystemService(Context.ALARM_SERVICE))
                .thenReturn(mAlarmManager.getAlarmManager());

        when(mMockContext.getSystemService(Context.WIFI_SERVICE)).thenReturn(mMockWifiManager);
        when(mMockWifiManager.getWifiState()).thenReturn(WifiManager.WIFI_STATE_ENABLED);

        mMockLooper = new TestLooper();
        mMockLooperHandler = new Handler(mMockLooper.getLooper());

        IPowerManager powerManagerService = mock(IPowerManager.class);
        mMockPowerManager = new PowerManager(mMockContext, powerManagerService,
                new Handler(mMockLooper.getLooper()));

        when(mMockContext.getSystemService(Context.CONNECTIVITY_SERVICE)).thenReturn(mMockCm);
        when(mMockContext.getSystemServiceName(PowerManager.class)).thenReturn(
                Context.POWER_SERVICE);
        when(mMockContext.getSystemService(PowerManager.class)).thenReturn(mMockPowerManager);

        // by default pretend to be an old API: i.e. allow Responders configured as *ANY*. This
        // allows older (more extrensive) tests to run.
        when(mWifiPermissionsUtil.isTargetSdkLessThan(anyString(), anyInt(), anyInt()))
            .thenReturn(true);
        when(mWifiPermissionsUtil.isLocationModeEnabled()).thenReturn(true);

        mDut = new WifiAwareStateManager();
        mDut.setNative(mMockNativeManager, mMockNative);
        mDut.start(mMockContext, mMockLooper.getLooper(), mAwareMetricsMock,
                mWifiPermissionsUtil, mPermissionsWrapperMock, mClock);
        mDut.startLate();
        mMockLooper.dispatchAll();

        when(mMockNetworkInterface.configureAgentProperties(any(), any(), anyInt(), any(), any(),
                any())).thenReturn(true);
        when(mMockNetworkInterface.isAddressUsable(any())).thenReturn(true);

        when(mMockPowerManager.isDeviceIdleMode()).thenReturn(false);
        when(mMockPowerManager.isInteractive()).thenReturn(true);

        when(mPermissionsWrapperMock.getUidPermission(eq(Manifest.permission.CONNECTIVITY_INTERNAL),
                anyInt())).thenReturn(PackageManager.PERMISSION_GRANTED);

        mDut.mDataPathMgr.mNwService = mMockNwMgt;
        mDut.mDataPathMgr.mNiWrapper = mMockNetworkInterface;
    }

    /**
     * Post-test validation.
     */
    @After
    public void tearDown() throws Exception {
        mMockNative.validateUniqueTransactionIds();
    }

    /**
     * Validates that creating and deleting all interfaces works based on capabilities.
     */
    @Test
    public void testCreateDeleteAllInterfaces() throws Exception {
        final int numNdis = 3;
        final int failCreateInterfaceIndex = 1;

        Capabilities capabilities = new Capabilities();
        capabilities.maxNdiInterfaces = numNdis;

        ArgumentCaptor<Short> transactionId = ArgumentCaptor.forClass(Short.class);
        ArgumentCaptor<String> interfaceName = ArgumentCaptor.forClass(String.class);
        InOrder inOrder = inOrder(mMockNative);

        // (1) get capabilities
        mDut.queryCapabilities();
        mMockLooper.dispatchAll();
        inOrder.verify(mMockNative).getCapabilities(transactionId.capture());
        mDut.onCapabilitiesUpdateResponse(transactionId.getValue(), capabilities);
        mMockLooper.dispatchAll();

        // (2) create all interfaces
        mDut.createAllDataPathInterfaces();
        mMockLooper.dispatchAll();
        for (int i = 0; i < numNdis; ++i) {
            inOrder.verify(mMockNative).createAwareNetworkInterface(transactionId.capture(),
                    interfaceName.capture());
            collector.checkThat("interface created -- " + i, sAwareInterfacePrefix + i,
                    equalTo(interfaceName.getValue()));
            mDut.onCreateDataPathInterfaceResponse(transactionId.getValue(), true, 0);
            mMockLooper.dispatchAll();
        }

        // (3) delete all interfaces [one unsuccessfully] - note that will not necessarily be
        // done sequentially
        boolean[] done = new boolean[numNdis];
        Arrays.fill(done, false);
        mDut.deleteAllDataPathInterfaces();
        mMockLooper.dispatchAll();
        for (int i = 0; i < numNdis; ++i) {
            inOrder.verify(mMockNative).deleteAwareNetworkInterface(transactionId.capture(),
                    interfaceName.capture());
            int interfaceIndex = Integer.valueOf(
                    interfaceName.getValue().substring(sAwareInterfacePrefix.length()));
            done[interfaceIndex] = true;
            if (interfaceIndex == failCreateInterfaceIndex) {
                mDut.onDeleteDataPathInterfaceResponse(transactionId.getValue(), false, 0);
            } else {
                mDut.onDeleteDataPathInterfaceResponse(transactionId.getValue(), true, 0);
            }
            mMockLooper.dispatchAll();
        }
        for (int i = 0; i < numNdis; ++i) {
            collector.checkThat("interface deleted -- " + i, done[i], equalTo(true));
        }

        // (4) create all interfaces (should get a delete for the one which couldn't delete earlier)
        mDut.createAllDataPathInterfaces();
        mMockLooper.dispatchAll();
        for (int i = 0; i < numNdis; ++i) {
            if (i == failCreateInterfaceIndex) {
                inOrder.verify(mMockNative).deleteAwareNetworkInterface(transactionId.capture(),
                        interfaceName.capture());
                collector.checkThat("interface delete pre-create -- " + i,
                        sAwareInterfacePrefix + i, equalTo(interfaceName.getValue()));
                mDut.onDeleteDataPathInterfaceResponse(transactionId.getValue(), true, 0);
                mMockLooper.dispatchAll();
            }
            inOrder.verify(mMockNative).createAwareNetworkInterface(transactionId.capture(),
                    interfaceName.capture());
            collector.checkThat("interface created -- " + i, sAwareInterfacePrefix + i,
                    equalTo(interfaceName.getValue()));
            mDut.onCreateDataPathInterfaceResponse(transactionId.getValue(), true, 0);
            mMockLooper.dispatchAll();
        }

        verifyNoMoreInteractions(mMockNative, mMockNwMgt);
    }

    /**
     * Validate that trying to specify a PMK without permission results in failure.
     */
    @Test
    public void testDataPathPmkWithoutPermission() throws Exception {
        final int clientId = 123;
        final byte pubSubId = 55;
        final byte[] pmk = "01234567890123456789012345678901".getBytes();
        final int requestorId = 1341234;
        final byte[] peerDiscoveryMac = HexEncoding.decode("000102030405".toCharArray(), false);

        InOrder inOrder = inOrder(mMockNative, mMockCm, mMockCallback, mMockSessionCallback);
        InOrder inOrderM = inOrder(mAwareMetricsMock);

        when(mPermissionsWrapperMock.getUidPermission(eq(Manifest.permission.CONNECTIVITY_INTERNAL),
                anyInt())).thenReturn(PackageManager.PERMISSION_DENIED);

        // (0) initialize
        DataPathEndPointInfo res = initDataPathEndPoint(true, clientId, pubSubId, requestorId,
                peerDiscoveryMac, inOrder, inOrderM, false);

        // (1) request network
        NetworkRequest nr = getSessionNetworkRequest(clientId, res.mSessionId, res.mPeerHandle, pmk,
                null, false, 0);

        Message reqNetworkMsg = Message.obtain();
        reqNetworkMsg.what = NetworkFactory.CMD_REQUEST_NETWORK;
        reqNetworkMsg.obj = nr;
        reqNetworkMsg.arg1 = 0;
        res.mMessenger.send(reqNetworkMsg);
        mMockLooper.dispatchAll();

        // failure: no interactions with native manager
        verifyUnfullfillableDispatched(res.mReverseMessenger);
        verifyNoMoreInteractions(mMockNative, mMockCm, mAwareMetricsMock, mMockNwMgt);
    }

    /**
     * Validate that trying to specify port info on subscriber results in failure.
     */
    @Test
    public void testDataPathWithPortInfoOnPublisher() throws Exception {
        final int clientId = 123;
        final byte pubSubId = 55;
        final int requestorId = 1341234;
        final String passphrase = "SomeSecurePassword";
        final int ndpId = 1;
        final byte[] peerDiscoveryMac = HexEncoding.decode("000102030405".toCharArray(), false);

        InOrder inOrder = inOrder(mMockNative, mMockCm, mMockCallback, mMockSessionCallback);
        InOrder inOrderM = inOrder(mAwareMetricsMock);

        // (0) initialize
        DataPathEndPointInfo res = initDataPathEndPoint(true, clientId, pubSubId, requestorId,
                peerDiscoveryMac, inOrder, inOrderM, false);

        // (1) request network
        NetworkRequest nr = getSessionNetworkRequestMore(clientId, res.mSessionId, res.mPeerHandle,
                null, passphrase, true, 0, 5, 6);

        Message reqNetworkMsg = Message.obtain();
        reqNetworkMsg.what = NetworkFactory.CMD_REQUEST_NETWORK;
        reqNetworkMsg.obj = nr;
        reqNetworkMsg.arg1 = 0;
        res.mMessenger.send(reqNetworkMsg);
        mMockLooper.dispatchAll();

        // (2) provide a request
        mDut.onDataPathRequestNotification(pubSubId, peerDiscoveryMac, ndpId, null);
        mMockLooper.dispatchAll();

        // do not respond/create a data-path!
        verify(mMockNative, never()).respondToDataPathRequest(anyShort(), anyBoolean(), anyInt(),
                anyString(), any(), anyString(), any(), anyBoolean(), any());
    }

    /**
     * Validate that trying to specify invalid port info results in failure.
     */
    @Test
    public void testDataPathWithPortInfoInvalidPort() throws Exception {
        final int clientId = 123;
        final byte pubSubId = 55;
        final int requestorId = 1341234;
        final String passphrase = "SomeSecurePassword";
        final byte[] peerDiscoveryMac = HexEncoding.decode("000102030405".toCharArray(), false);

        InOrder inOrder = inOrder(mMockNative, mMockCm, mMockCallback, mMockSessionCallback);
        InOrder inOrderM = inOrder(mAwareMetricsMock);

        // (0) initialize
        DataPathEndPointInfo res = initDataPathEndPoint(true, clientId, pubSubId, requestorId,
                peerDiscoveryMac, inOrder, inOrderM, false);

        // (1) request network
        NetworkRequest nr = getSessionNetworkRequestMore(clientId, res.mSessionId, res.mPeerHandle,
                null, passphrase, false, 0, -3, 6);

        Message reqNetworkMsg = Message.obtain();
        reqNetworkMsg.what = NetworkFactory.CMD_REQUEST_NETWORK;
        reqNetworkMsg.obj = nr;
        reqNetworkMsg.arg1 = 0;
        res.mMessenger.send(reqNetworkMsg);
        mMockLooper.dispatchAll();

        // do not create a data-path!
        verify(mMockNative, never()).initiateDataPath(anyShort(), anyInt(), anyInt(), anyInt(),
                any(), anyString(), any(), anyString(), anyBoolean(), any(), any());
    }

    /**
     * Validate that trying to specify port info without security results in failure.
     */
    @Test
    public void testDataPathWithPortInfoButNoSecurityOnSubscriber() throws Exception {
        final int clientId = 123;
        final byte pubSubId = 55;
        final int requestorId = 1341234;
        final byte[] peerDiscoveryMac = HexEncoding.decode("000102030405".toCharArray(), false);

        InOrder inOrder = inOrder(mMockNative, mMockCm, mMockCallback, mMockSessionCallback);
        InOrder inOrderM = inOrder(mAwareMetricsMock);

        // (0) initialize
        DataPathEndPointInfo res = initDataPathEndPoint(true, clientId, pubSubId, requestorId,
                peerDiscoveryMac, inOrder, inOrderM, false);

        // (1) request network
        NetworkRequest nr = getSessionNetworkRequestMore(clientId, res.mSessionId, res.mPeerHandle,
                null, null, false, 0, 10, 6);

        Message reqNetworkMsg = Message.obtain();
        reqNetworkMsg.what = NetworkFactory.CMD_REQUEST_NETWORK;
        reqNetworkMsg.obj = nr;
        reqNetworkMsg.arg1 = 0;
        res.mMessenger.send(reqNetworkMsg);
        mMockLooper.dispatchAll();

        // do not create a data-path!
        verify(mMockNative, never()).initiateDataPath(anyShort(), anyInt(), anyInt(), anyInt(),
                any(), anyString(), any(), anyString(), anyBoolean(), any(), any());
    }

    /**
     * Validate that if the data-interfaces are deleted while a data-path is being created, the
     * process will terminate.
     */
    @Test
    public void testDestroyNdiDuringNdpSetupResponder() throws Exception {
        final int clientId = 123;
        final byte pubSubId = 55;
        final int requestorId = 1341234;
        final byte[] peerDiscoveryMac = HexEncoding.decode("000102030405".toCharArray(), false);
        final int ndpId = 3;

        InOrder inOrder = inOrder(mMockNative, mMockCm, mMockCallback, mMockSessionCallback);
        InOrder inOrderM = inOrder(mAwareMetricsMock);

        ArgumentCaptor<Short> transactionId = ArgumentCaptor.forClass(Short.class);

        // (0) initialize
        DataPathEndPointInfo res = initDataPathEndPoint(true, clientId, pubSubId, requestorId,
                peerDiscoveryMac, inOrder, inOrderM, true);

        // (1) request network
        NetworkRequest nr = getSessionNetworkRequest(clientId, res.mSessionId, res.mPeerHandle,
                null, null, true, 0);

        Message reqNetworkMsg = Message.obtain();
        reqNetworkMsg.what = NetworkFactory.CMD_REQUEST_NETWORK;
        reqNetworkMsg.obj = nr;
        reqNetworkMsg.arg1 = 0;
        res.mMessenger.send(reqNetworkMsg);
        mMockLooper.dispatchAll();

        // (2) delete interface(s)
        mDut.deleteAllDataPathInterfaces();
        mMockLooper.dispatchAll();
        inOrder.verify(mMockNative).deleteAwareNetworkInterface(transactionId.capture(),
                anyString());
        mDut.onDeleteDataPathInterfaceResponse(transactionId.getValue(), true, 0);
        mMockLooper.dispatchAll();

        // (3) have responder receive request
        mDut.onDataPathRequestNotification(pubSubId, peerDiscoveryMac, ndpId, null);
        mMockLooper.dispatchAll();

        // (4) verify that responder aborts (i.e. refuses request)
        inOrder.verify(mMockNative).respondToDataPathRequest(transactionId.capture(), eq(false),
                eq(ndpId), eq(""), eq(null), eq(null), eq(null), eq(false), any());
        mDut.onRespondToDataPathSetupRequestResponse(transactionId.getValue(), true, 0);
        mMockLooper.dispatchAll();

        verifyUnfullfillableDispatched(res.mReverseMessenger);

        // failure if there's further activity
        verifyNoMoreInteractions(mMockNative, mMockCm, mAwareMetricsMock, mMockNwMgt);
    }

    /**
     * Validate multiple NDPs created on a single NDI. Most importantly that the interface is
     * set up on first NDP and torn down on last NDP - and not when one or the other is created or
     * deleted.
     *
     * Procedure:
     * - create NDP 1, 2, and 3 (interface up only on first)
     * - delete NDP 2, 1, and 3 (interface down only on last)
     */
    @Test
    public void testMultipleNdpsOnSingleNdi() throws Exception {
        final int clientId = 123;
        final byte pubSubId = 58;
        final int requestorId = 1341234;
        final int ndpId = 2;
        final String interfaceName = sAwareInterfacePrefix + "0";
        final int port = 23;
        final byte transportProtocol = 6;

        final int[] startOrder = {0, 1, 2};
        final int[] endOrder = {1, 0, 2};
        int networkRequestId = 0;

        ArgumentCaptor<Messenger> messengerCaptor = ArgumentCaptor.forClass(Messenger.class);
        ArgumentCaptor<Short> transactionId = ArgumentCaptor.forClass(Short.class);
        ArgumentCaptor<NetworkCapabilities> netCapCaptor = ArgumentCaptor.forClass(
                NetworkCapabilities.class);
        InOrder inOrder = inOrder(mMockNative, mMockCm, mMockCallback, mMockSessionCallback,
                mMockNwMgt);
        InOrder inOrderM = inOrder(mAwareMetricsMock);

        NetworkRequest[] nrs = new NetworkRequest[3];
        DataPathEndPointInfo[] ress = new DataPathEndPointInfo[3];
        Messenger[] agentMessengers = new Messenger[3];
        Messenger messenger = null;
        boolean first = true;
        for (int i : startOrder) {
            networkRequestId += 1;
            byte[] peerDiscoveryMac = HexEncoding.decode("000102030405".toCharArray(), false);
            byte[] peerDataPathMac = HexEncoding.decode("0A0B0C0D0E0F".toCharArray(), false);
            peerDiscoveryMac[5] = (byte) (peerDiscoveryMac[5] + i);
            peerDataPathMac[5] = (byte) (peerDataPathMac[5] + i);

            // (0) initialize
            ress[i] = initDataPathEndPoint(first, clientId, (byte) (pubSubId + i),
                    requestorId + i, peerDiscoveryMac, inOrder, inOrderM, false);
            if (first) {
                messenger = ress[i].mMessenger;
            }

            // (1) request network
            nrs[i] = getSessionNetworkRequest(clientId, ress[i].mSessionId, ress[i].mPeerHandle,
                    null, null, false, networkRequestId);

            Message reqNetworkMsg = Message.obtain();
            reqNetworkMsg.what = NetworkFactory.CMD_REQUEST_NETWORK;
            reqNetworkMsg.obj = nrs[i];
            reqNetworkMsg.arg1 = 0;
            messenger.send(reqNetworkMsg);
            mMockLooper.dispatchAll();
            inOrder.verify(mMockNative).initiateDataPath(transactionId.capture(),
                    eq(requestorId + i),
                    eq(CHANNEL_NOT_REQUESTED), anyInt(), eq(peerDiscoveryMac),
                    eq(interfaceName), eq(null),
                    eq(null), eq(false), any(), any());

            mDut.onInitiateDataPathResponseSuccess(transactionId.getValue(), ndpId + i);
            mMockLooper.dispatchAll();

            // (2) get confirmation
            mDut.onDataPathConfirmNotification(ndpId + i, peerDataPathMac, true, 0,
                    buildTlv(port, transportProtocol, true), null);
            mMockLooper.dispatchAll();
            if (first) {
                inOrder.verify(mMockNwMgt).setInterfaceUp(anyString());
                inOrder.verify(mMockNwMgt).enableIpv6(anyString());

                first = false;
            }
            inOrder.verify(mMockCm).registerNetworkAgent(messengerCaptor.capture(), any(), any(),
                    netCapCaptor.capture(), anyInt(), any(), anyInt());
            agentMessengers[i] = messengerCaptor.getValue();
            inOrderM.verify(mAwareMetricsMock).recordNdpStatus(eq(NanStatusType.SUCCESS),
                    eq(false), anyLong());
            inOrderM.verify(mAwareMetricsMock).recordNdpCreation(anyInt(), any());
            WifiAwareNetworkInfo netInfo =
                    (WifiAwareNetworkInfo) netCapCaptor.getValue().getTransportInfo();
            assertArrayEquals(MacAddress.fromBytes(
                    peerDataPathMac).getLinkLocalIpv6FromEui48Mac().getAddress(),
                    netInfo.getPeerIpv6Addr().getAddress());
            assertEquals(port, netInfo.getPort());
            assertEquals(transportProtocol, netInfo.getTransportProtocol());
        }

        // (3) end data-path (unless didn't get confirmation)
        int index = 0;
        for (int i: endOrder) {
            Message endNetworkReqMsg = Message.obtain();
            endNetworkReqMsg.what = NetworkFactory.CMD_CANCEL_REQUEST;
            endNetworkReqMsg.obj = nrs[i];
            messenger.send(endNetworkReqMsg);

            Message endNetworkUsageMsg = Message.obtain();
            endNetworkUsageMsg.what = AsyncChannel.CMD_CHANNEL_DISCONNECTED;
            agentMessengers[i].send(endNetworkUsageMsg);
            mMockLooper.dispatchAll();

            inOrder.verify(mMockNative).endDataPath(transactionId.capture(), eq(ndpId + i));

            mDut.onEndDataPathResponse(transactionId.getValue(), true, 0);
            mDut.onDataPathEndNotification(ndpId + i);
            mMockLooper.dispatchAll();

            if (index++ == endOrder.length - 1) {
                inOrder.verify(mMockNwMgt).setInterfaceDown(anyString());
            }
            inOrderM.verify(mAwareMetricsMock).recordNdpSessionDuration(anyLong());
        }

        verifyNoMoreInteractions(mMockNative, mMockCm, mAwareMetricsMock, mMockNwMgt);
    }

    /**
     * Validate that multiple NDP requests which resolve to the same canonical request are treated
     * as one.
     */
    @Test
    public void testMultipleIdenticalRequests() throws Exception {
        final int numRequestsPre = 6;
        final int numRequestsPost = 5;
        final int clientId = 123;
        final int ndpId = 5;
        final int port = 0;
        final int transportProtocol = 6; // can't specify transport protocol without port
        final byte[] peerDiscoveryMac = HexEncoding.decode("000102030405".toCharArray(), false);
        final byte[] peerDataPathMac = HexEncoding.decode("0A0B0C0D0E0F".toCharArray(), false);
        final byte[] allZeros = HexEncoding.decode("000000000000".toCharArray(), false);
        NetworkRequest[] nrs = new NetworkRequest[numRequestsPre + numRequestsPost + 1];

        ArgumentCaptor<Short> transactionId = ArgumentCaptor.forClass(Short.class);
        ArgumentCaptor<Messenger> agentMessengerCaptor = ArgumentCaptor.forClass(Messenger.class);
        ArgumentCaptor<NetworkCapabilities> netCapCaptor = ArgumentCaptor.forClass(
                NetworkCapabilities.class);
        InOrder inOrder = inOrder(mMockNative, mMockCm, mMockCallback, mMockSessionCallback,
                mMockNwMgt);
        InOrder inOrderM = inOrder(mAwareMetricsMock);

        // (1) initialize all clients
        Messenger messenger = initOobDataPathEndPoint(true, 2, clientId, inOrder, inOrderM).first;
        for (int i = 1; i < numRequestsPre + numRequestsPost; ++i) {
            initOobDataPathEndPoint(false, 1, clientId + i, inOrder, inOrderM);
        }
        DataPathEndPointInfo ddepi = initDataPathEndPoint(false,
                clientId + numRequestsPre + numRequestsPost, (byte) 10, 11, peerDiscoveryMac,
                inOrder, inOrderM, false);

        // (2) make initial network requests (all identical under the hood)
        for (int i = 0; i < numRequestsPre; ++i) {
            nrs[i] = getDirectNetworkRequest(clientId + i,
                    WifiAwareManager.WIFI_AWARE_DATA_PATH_ROLE_INITIATOR, peerDiscoveryMac, null,
                    null, i);

            Message reqNetworkMsg = Message.obtain();
            reqNetworkMsg.what = NetworkFactory.CMD_REQUEST_NETWORK;
            reqNetworkMsg.obj = nrs[i];
            reqNetworkMsg.arg1 = 0;
            messenger.send(reqNetworkMsg);
        }
        mMockLooper.dispatchAll();

        // (3) verify the start NDP HAL request
        inOrder.verify(mMockNative).initiateDataPath(transactionId.capture(), eq(0),
                eq(CHANNEL_NOT_REQUESTED), anyInt(), eq(peerDiscoveryMac),
                eq(sAwareInterfacePrefix + "0"), eq(null), eq(null), eq(true), any(), any());

        // (4) unregister request #0 (the primary)
        Message endNetworkReqMsg = Message.obtain();
        endNetworkReqMsg.what = NetworkFactory.CMD_CANCEL_REQUEST;
        endNetworkReqMsg.obj = nrs[0];
        messenger.send(endNetworkReqMsg);
        mMockLooper.dispatchAll();

        // (5) respond to the registration request
        mDut.onInitiateDataPathResponseSuccess(transactionId.getValue(), ndpId);
        mMockLooper.dispatchAll();

        // (6) unregister request #1
        endNetworkReqMsg = Message.obtain();
        endNetworkReqMsg.what = NetworkFactory.CMD_CANCEL_REQUEST;
        endNetworkReqMsg.obj = nrs[1];
        messenger.send(endNetworkReqMsg);
        mMockLooper.dispatchAll();

        // (6.5) provide a (semi) bogus NDP Requst Indication - mostly bogus on Initiator but
        // may contain the peer's TLVs (in this case it does)
        mDut.onDataPathRequestNotification(0, allZeros, ndpId,
                buildTlv(port, transportProtocol, true));

        // (7) confirm the NDP creation
        mDut.onDataPathConfirmNotification(ndpId, peerDataPathMac, true, 0, null, null);
        mMockLooper.dispatchAll();

        inOrder.verify(mMockNwMgt).setInterfaceUp(anyString());
        inOrder.verify(mMockNwMgt).enableIpv6(anyString());
        inOrder.verify(mMockCm).registerNetworkAgent(agentMessengerCaptor.capture(), any(), any(),
                netCapCaptor.capture(), anyInt(), any(), anyInt());
        inOrderM.verify(mAwareMetricsMock).recordNdpStatus(eq(NanStatusType.SUCCESS),
                eq(true), anyLong());
        inOrderM.verify(mAwareMetricsMock).recordNdpCreation(anyInt(), any());
        WifiAwareNetworkInfo netInfo =
                (WifiAwareNetworkInfo) netCapCaptor.getValue().getTransportInfo();
        assertArrayEquals(MacAddress.fromBytes(
                peerDataPathMac).getLinkLocalIpv6FromEui48Mac().getAddress(),
                netInfo.getPeerIpv6Addr().getAddress());
        assertEquals(port, netInfo.getPort());
        assertEquals(transportProtocol, netInfo.getTransportProtocol());

        // (8) execute 'post' requests
        for (int i = numRequestsPre; i < numRequestsPre + numRequestsPost; ++i) {
            nrs[i] = getDirectNetworkRequest(clientId + i,
                    WifiAwareManager.WIFI_AWARE_DATA_PATH_ROLE_INITIATOR, peerDiscoveryMac, null,
                    null, i);

            Message reqNetworkMsg = Message.obtain();
            reqNetworkMsg.what = NetworkFactory.CMD_REQUEST_NETWORK;
            reqNetworkMsg.obj = nrs[i];
            reqNetworkMsg.arg1 = 0;
            messenger.send(reqNetworkMsg);
        }
        nrs[numRequestsPre + numRequestsPost] = getSessionNetworkRequest(
                clientId + numRequestsPre + numRequestsPost, ddepi.mSessionId, ddepi.mPeerHandle,
                null, null, false, 11);
        Message reqNetworkMsg = Message.obtain();
        reqNetworkMsg.what = NetworkFactory.CMD_REQUEST_NETWORK;
        reqNetworkMsg.obj = nrs[numRequestsPre + numRequestsPost];
        reqNetworkMsg.arg1 = 0;
        messenger.send(reqNetworkMsg);
        mMockLooper.dispatchAll();

        // (9) unregister all requests
        for (int i = 2; i < numRequestsPre + numRequestsPost + 1; ++i) {
            endNetworkReqMsg = Message.obtain();
            endNetworkReqMsg.what = NetworkFactory.CMD_CANCEL_REQUEST;
            endNetworkReqMsg.obj = nrs[i];
            messenger.send(endNetworkReqMsg);
            mMockLooper.dispatchAll();
        }

        Message endNetworkUsageMsg = Message.obtain();
        endNetworkUsageMsg.what = AsyncChannel.CMD_CHANNEL_DISCONNECTED;
        agentMessengerCaptor.getValue().send(endNetworkUsageMsg);
        mMockLooper.dispatchAll();

        // (10) verify that NDP torn down
        inOrder.verify(mMockNative).endDataPath(transactionId.capture(), eq(ndpId));

        mDut.onEndDataPathResponse(transactionId.getValue(), true, 0);
        mDut.onDataPathEndNotification(ndpId);
        mMockLooper.dispatchAll();

        inOrder.verify(mMockNwMgt).setInterfaceDown(anyString());
        inOrderM.verify(mAwareMetricsMock).recordNdpSessionDuration(anyLong());

        verifyNoMoreInteractions(mMockNative, mMockCm, mMockCallback, mMockSessionCallback,
                mAwareMetricsMock, mMockNwMgt);
    }

    /**
     * Validate that multiple NDP requests to the same peer target different NDIs.
     */
    @Test
    public void testMultipleNdi() throws Exception {
        final int numNdis = 5;
        final int clientId = 123;
        final int ndpId = 5;
        final byte[] peerDiscoveryMac = HexEncoding.decode("000102030405".toCharArray(), false);
        final byte[] peerDataPathMac = HexEncoding.decode("0A0B0C0D0E0F".toCharArray(), false);

        ArgumentCaptor<Short> transactionId = ArgumentCaptor.forClass(Short.class);
        ArgumentCaptor<String> ifNameCaptor = ArgumentCaptor.forClass(String.class);
        ArgumentCaptor<NetworkCapabilities> netCapCaptor = ArgumentCaptor.forClass(
                NetworkCapabilities.class);

        InOrder inOrder = inOrder(mMockNative, mMockCm, mMockCallback, mMockSessionCallback,
                mMockNwMgt);
        InOrder inOrderM = inOrder(mAwareMetricsMock);

        // (1) initialize all clients
        Pair<Messenger, Messenger> res = initOobDataPathEndPoint(true, numNdis, clientId, inOrder,
                inOrderM);
        for (int i = 1; i < numNdis + 3; ++i) {
            initOobDataPathEndPoint(false, numNdis, clientId + i, inOrder, inOrderM);
        }

        // (2) make N network requests: each unique
        Set<String> interfaces = new HashSet<>();
        for (int i = 0; i < numNdis + 1; ++i) {
            byte[] pmk = new byte[32];
            pmk[0] = (byte) i;

            NetworkRequest nr = getDirectNetworkRequest(clientId + i,
                    WifiAwareManager.WIFI_AWARE_DATA_PATH_ROLE_INITIATOR, peerDiscoveryMac, pmk,
                    null, i);

            Message reqNetworkMsg = Message.obtain();
            reqNetworkMsg.what = NetworkFactory.CMD_REQUEST_NETWORK;
            reqNetworkMsg.obj = nr;
            reqNetworkMsg.arg1 = 0;
            res.first.send(reqNetworkMsg);
            mMockLooper.dispatchAll();

            if (i < numNdis) {
                inOrder.verify(mMockNative).initiateDataPath(transactionId.capture(), eq(0),
                        eq(CHANNEL_NOT_REQUESTED), anyInt(), eq(peerDiscoveryMac),
                        ifNameCaptor.capture(), eq(pmk), eq(null), eq(true), any(), any());
                interfaces.add(ifNameCaptor.getValue());

                mDut.onInitiateDataPathResponseSuccess(transactionId.getValue(), ndpId + i);
                mDut.onDataPathConfirmNotification(ndpId + i, peerDataPathMac, true, 0, null, null);
                mMockLooper.dispatchAll();

                inOrder.verify(mMockNwMgt).setInterfaceUp(anyString());
                inOrder.verify(mMockNwMgt).enableIpv6(anyString());
                inOrder.verify(mMockCm).registerNetworkAgent(any(), any(), any(),
                        netCapCaptor.capture(), anyInt(), any(), anyInt());
                inOrderM.verify(mAwareMetricsMock).recordNdpStatus(eq(NanStatusType.SUCCESS),
                        eq(true), anyLong());
                inOrderM.verify(mAwareMetricsMock).recordNdpCreation(anyInt(), any());
                WifiAwareNetworkInfo netInfo =
                        (WifiAwareNetworkInfo) netCapCaptor.getValue().getTransportInfo();
                assertArrayEquals(MacAddress.fromBytes(
                        peerDataPathMac).getLinkLocalIpv6FromEui48Mac().getAddress(),
                        netInfo.getPeerIpv6Addr().getAddress());
                assertEquals(0, netInfo.getPort()); // uninitialized -> 0
                assertEquals(-1, netInfo.getTransportProtocol()); // uninitialized -> -1
            } else {
                verifyUnfullfillableDispatched(res.second);
            }
        }

        // verify that each interface name is unique
        assertEquals("Number of unique interface names", numNdis, interfaces.size());

        verifyNoMoreInteractions(mMockNative, mMockCm, mMockCallback, mMockSessionCallback,
                mAwareMetricsMock, mMockNwMgt);
    }

    /*
     * Initiator tests
     */

    /**
     * Validate the success flow of the Initiator: using session network specifier with a non-null
     * PMK.
     */
    @Test
    public void testDataPathInitiatorMacPmkSuccess() throws Exception {
        testDataPathInitiatorUtility(false, true, true, false, true, false);
    }

    /**
     * Validate the success flow of the Initiator: using a direct network specifier with a non-null
     * peer mac and non-null PMK.
     */
    @Test
    public void testDataPathInitiatorDirectMacPmkSuccess() throws Exception {
        testDataPathInitiatorUtility(true, true, true, false, true, false);
    }


    /**
     * Validate the fail flow of the Initiator: use a session network specifier with a non-null
     * PMK, but don't get a confirmation.
     */
    @Test
    public void testDataPathInitiatorNoConfirmationTimeoutFail() throws Exception {
        testDataPathInitiatorUtility(false, true, true, false, false, false);
    }

    /**
     * Validate the fail flow of the Initiator: use a session network specifier with a non-null
     * Passphrase, but get an immediate failure
     */
    @Test
    public void testDataPathInitiatorNoConfirmationHalFail() throws Exception {
        testDataPathInitiatorUtility(false, true, false, true, true, true);
    }

    /**
     * Verify that an TLV configuration with large port/transport-protocol work correctly.
     */
    @Test
    public void testDataPathInitiatorNetInfoLargeValuesExp1() throws Exception {
        final byte[] peerDataPathMac = HexEncoding.decode("0A0B0C0D0E0F".toCharArray(), false);
        String linkLocalIpv6Address = MacAddress.fromBytes(
                peerDataPathMac).getLinkLocalIpv6FromEui48Mac().getHostAddress();

        testDataPathInitiatorUtilityMore(false, true, true, false, true, false, peerDataPathMac,
                buildTlv((1 << 16) - 1, (1 << 8) - 1, true), (1 << 16) - 1, (1 << 8) - 1,
                linkLocalIpv6Address, 0);
    }

    /**
     * Verify that an TLV configuration with large port/transport-protocol work correctly.
     */
    @Test
    public void testDataPathInitiatorNetInfoLargeValuesExp2() throws Exception {
        final byte[] peerDataPathMac = HexEncoding.decode("0A0B0C0D0E0F".toCharArray(), false);
        String linkLocalIpv6Address = MacAddress.fromBytes(
                peerDataPathMac).getLinkLocalIpv6FromEui48Mac().getHostAddress();

        testDataPathInitiatorUtilityMore(false, true, true, false, true, false, peerDataPathMac,
                buildTlv(1 << 15, 1 << 7, true), 1 << 15, 1 << 7, linkLocalIpv6Address, 0);
    }

    /**
     * Verify that an TLV configuration with large port/transport-protocol work correctly.
     */
    @Test
    public void testDataPathInitiatorNetInfoLargeValuesExp3() throws Exception {
        final byte[] peerDataPathMac = HexEncoding.decode("0A0B0C0D0E0F".toCharArray(), false);
        String linkLocalIpv6Address = MacAddress.fromBytes(
                peerDataPathMac).getLinkLocalIpv6FromEui48Mac().getHostAddress();

        testDataPathInitiatorUtilityMore(false, true, true, false, true, false, peerDataPathMac,
                buildTlv((1 << 15) - 1, (1 << 7) - 1, true), (1 << 15) - 1, (1 << 7) - 1,
                linkLocalIpv6Address, 0);
    }

    /**
     * Verify that an TLV configuration with an IPv6 override works correctly.
     */
    @Test
    public void testDataPathInitiatorNetInfoIpv6Override() throws Exception {
        final byte[] peerDataPathMac = HexEncoding.decode("0A0B0C0D0E0F".toCharArray(), false);
        final byte[] testVector =
                new byte[]{0x00, 0x08, 0x00, 0x00, (byte) 0xb3, (byte) 0xe1, (byte) 0xff,
                        (byte) 0xfe, 0x7a, 0x2f, (byte) 0xa2};

        testDataPathInitiatorUtilityMore(false, true, true, false, true, false, peerDataPathMac,
                testVector, 0, -1, "fe80::b3:e1ff:fe7a:2fa2", 0);
    }

    /**
     * Verify that retrying address validation a 'small' number of times results in successful
     * NDP setup.
     */
    @Test
    public void testDataPathInitiatorAddressValidationRetrySuccess() throws Exception {
        final byte[] peerDataPathMac = HexEncoding.decode("0A0B0C0D0E0F".toCharArray(), false);
        String linkLocalIpv6Address = MacAddress.fromBytes(
                peerDataPathMac).getLinkLocalIpv6FromEui48Mac().getHostAddress();

        testDataPathInitiatorUtilityMore(false, true, true, false, true, false, peerDataPathMac,
                null, 0, -1, linkLocalIpv6Address,
                WifiAwareDataPathStateManager.ADDRESS_VALIDATION_TIMEOUT_MS
                        / WifiAwareDataPathStateManager.ADDRESS_VALIDATION_RETRY_INTERVAL_MS - 1);
    }

    /**
     * Verify that retrying address validation a 'large' number of times results in failure.
     */
    @Test
    public void testDataPathInitiatorAddressValidationRetryFail() throws Exception {
        final byte[] peerDataPathMac = HexEncoding.decode("0A0B0C0D0E0F".toCharArray(), false);
        String linkLocalIpv6Address = MacAddress.fromBytes(
                peerDataPathMac).getLinkLocalIpv6FromEui48Mac().getHostAddress();

        testDataPathInitiatorUtilityMore(false, true, true, false, true, false, peerDataPathMac,
                null, 0, -1, linkLocalIpv6Address,
                WifiAwareDataPathStateManager.ADDRESS_VALIDATION_TIMEOUT_MS
                        / WifiAwareDataPathStateManager.ADDRESS_VALIDATION_RETRY_INTERVAL_MS + 10);
    }

    /**
     * Validate the fail flow of a mis-configured request: Publisher as Initiator
     */
    @Test
    public void testDataPathInitiatorOnPublisherError() throws Exception {
        testDataPathInitiatorResponderMismatchUtility(true);
    }

    /**
     * Validate the fail flow of an Initiator (subscriber) with its UID unset
     */
    @Test
    public void testDataPathInitiatorUidUnsetError() throws Exception {
        testDataPathInitiatorResponderInvalidUidUtility(false, false);
    }

    /**
     * Validate the fail flow of an Initiator (subscriber) with its UID set as a malicious
     * attacker (i.e. mismatched to its requested client's UID).
     */
    @Test
    public void testDataPathInitiatorUidSetIncorrectlyError() throws Exception {
        testDataPathInitiatorResponderInvalidUidUtility(false, true);
    }

    /*
     * Responder tests
     */

    /**
     * Validate the success flow of the Responder: using session network specifier with a
     * PMK.
     */
    @Test
    public void testDataPathResonderMacPmkSuccess() throws Exception {
        testDataPathResponderUtility(false, true, true, false, true);
    }

    /**
     * Validate the success flow of the Responder: using session network specifier with a
     * Passphrase.
     */
    @Test
    public void testDataPathResonderMacPassphraseSuccess() throws Exception {
        testDataPathResponderUtility(false, true, false, false, true);
    }

    /**
     * Validate the success flow of the Responder: using session network specifier with a
     * Passphrase and no peer ID (i.e. 0).
     */
    @Test
    public void testDataPathResonderMacPassphraseNoPeerIdSuccess() throws Exception {
        testDataPathResponderUtility(false, false, false, true, true);
    }

    /**
     * Validate the success flow of the Responder: using session network specifier with a null
     * PMK/Passphrase and no peer ID (i.e. 0).
     */
    @Test
    public void testDataPathResonderMacOpenNoPeerIdNoPmkPassphraseSuccess() throws Exception {
        testDataPathResponderUtility(false, false, false, false, true);
    }

    /**
     * Validate the failure flow of the Responder: using session network specifier with a
     * Passphrase and no peer ID (i.e. 0) on a NON-LEGACY device.
     */
    @Test
    public void testDataPathResonderMacPassphraseNoPeerIdSuccessNonLegacy() throws Exception {
        when(mWifiPermissionsUtil.isTargetSdkLessThan(anyString(), anyInt(), anyInt()))
            .thenReturn(false);
        testDataPathResponderUtility(false, false, false, true, true);
    }

    /**
     * Validate the failure flow of the Responder: using session network specifier with a null
     * PMK/Passphrase and no peer ID (i.e. 0) on a NON-LEGACY device.
     */
    @Test
    public void testDataPathResonderMacOpenNoPeerIdNoPmkPassphraseSuccessNonLegacy()
            throws Exception {
        when(mWifiPermissionsUtil.isTargetSdkLessThan(anyString(), anyInt(), anyInt()))
            .thenReturn(false);
        testDataPathResponderUtility(false, false, false, false, true);
    }

    /**
     * Validate the success flow of the Responder: using a direct network specifier with a non-null
     * peer mac and non-null PMK.
     */
    @Test
    public void testDataPathResonderDirectMacPmkSuccess() throws Exception {
        testDataPathResponderUtility(true, true, true, false, true);
    }

    /**
     * Validate the success flow of the Responder: using a direct network specifier with a non-null
     * peer mac and null PMK/Passphrase.
     */
    @Test
    public void testDataPathResonderDirectMacNoPmkPassphraseSuccess() throws Exception {
        testDataPathResponderUtility(true, true, false, false, true);
    }

    /**
     * Validate the success flow of the Responder: using a direct network specifier with a null peer
     * mac and non-null Passphrase.
     */
    @Test
    public void testDataPathResonderDirectNoMacPassphraseSuccess() throws Exception {
        testDataPathResponderUtility(true, false, false, true, true);
    }

    /**
     * Validate the success flow of the Responder: using a direct network specifier with a null peer
     * mac and null Pmk/Passphrase.
     */
    @Test
    public void testDataPathResonderDirectNoMacNoPmkPassphraseSuccess() throws Exception {
        testDataPathResponderUtility(true, false, false, false, true);
    }

    /**
     * Validate the failure flow of the Responder: using a direct network specifier with a null peer
     * mac and non-null Passphrase on a NON-LEGACY device.
     */
    @Test
    public void testDataPathResonderDirectNoMacPassphraseSuccessNonLegacy() throws Exception {
        when(mWifiPermissionsUtil.isTargetSdkLessThan(anyString(), anyInt(), anyInt()))
            .thenReturn(false);
        testDataPathResponderUtility(true, false, false, true, true);
    }

    /**
     * Validate the failure flow of the Responder: using a direct network specifier with a null peer
     * mac and null Pmk/Passphrase on a NON-LEGACY device.
     */
    @Test
    public void testDataPathResonderDirectNoMacNoPmkPassphraseSuccessNonLegacy() throws Exception {
        when(mWifiPermissionsUtil.isTargetSdkLessThan(anyString(), anyInt(), anyInt()))
            .thenReturn(false);
        testDataPathResponderUtility(true, false, false, false, true);
    }

    /**
     * Validate the fail flow of the Responder: use a session network specifier with a non-null
     * PMK, but don't get a confirmation.
     */
    @Test
    public void testDataPathResponderNoConfirmationTimeoutFail() throws Exception {
        testDataPathResponderUtility(false, true, true, false, false);
    }

    /**
     * Validate the fail flow of a mis-configured request: Subscriber as Responder
     */
    @Test
    public void testDataPathResponderOnSubscriberError() throws Exception {
        testDataPathInitiatorResponderMismatchUtility(false);
    }

    /**
     * Validate the fail flow of an Initiator (subscriber) with its UID unset
     */
    @Test
    public void testDataPathResponderUidUnsetError() throws Exception {
        testDataPathInitiatorResponderInvalidUidUtility(true, false);
    }

    /**
     * Validate the fail flow of an Initiator (subscriber) with its UID set as a malicious
     * attacker (i.e. mismatched to its requested client's UID).
     */
    @Test
    public void testDataPathResponderUidSetIncorrectlyError() throws Exception {
        testDataPathInitiatorResponderInvalidUidUtility(true, true);
    }

    /**
     * Validate the TLV generation based on a test vector manually generated from spec.
     */
    @Test
    public void testTlvGenerationTestVectorPortTransportProtocol() {
        int port = 7000;
        int transportProtocol = 6;

        byte[] tlvData = WifiAwareDataPathStateManager.NetworkInformationData.buildTlv(port,
                transportProtocol);
        byte[] testVector =
                new byte[]{0x01, 0x0d, 0x00, 0x50, 0x6f, (byte) 0x9a, 0x02, 0x00, 0x02, 0x00, 0x58,
                        0x1b, 0x01, 0x01, 0x00, 0x06};

        assertArrayEquals(testVector, tlvData);
    }

    /**
     * Validate the TLV parsing based on a test vector of the port + transport protocol manually
     * generated from spec.
     */
    @Test
    public void testTlvParsingTestVectorPortTransportProtocol() {
        int port = 7000;
        int transportProtocol = 6;

        byte[] testVector =
                new byte[]{0x01, 0x0d, 0x00, 0x50, 0x6f, (byte) 0x9a, 0x02, 0x00, 0x02, 0x00, 0x58,
                        0x1b, 0x01, 0x01, 0x00, 0x06};

        WifiAwareDataPathStateManager.NetworkInformationData.ParsedResults parsed =
                WifiAwareDataPathStateManager.NetworkInformationData.parseTlv(testVector);
        assertEquals(port, (int) parsed.port);
        assertEquals(transportProtocol, (int) parsed.transportProtocol);
    }

    /*
     * Utilities
     */

    private void testDataPathInitiatorResponderMismatchUtility(boolean doPublish) throws Exception {
        final int clientId = 123;
        final byte pubSubId = 55;
        final int ndpId = 2;
        final byte[] pmk = "01234567890123456789012345678901".getBytes();
        final int requestorId = 1341234;
        final byte[] peerDiscoveryMac = HexEncoding.decode("000102030405".toCharArray(), false);

        InOrder inOrder = inOrder(mMockNative, mMockCm, mMockCallback, mMockSessionCallback);
        InOrder inOrderM = inOrder(mAwareMetricsMock);

        // (0) initialize
        DataPathEndPointInfo res = initDataPathEndPoint(true, clientId, pubSubId, requestorId,
                peerDiscoveryMac, inOrder, inOrderM, doPublish);

        // (1) request network
        NetworkRequest nr = getSessionNetworkRequest(clientId, res.mSessionId, res.mPeerHandle, pmk,
                null, doPublish, 0);

        // corrupt the network specifier: reverse the role (so it's mis-matched)
        WifiAwareNetworkSpecifier ns =
                (WifiAwareNetworkSpecifier) nr.networkCapabilities.getNetworkSpecifier();
        ns = new WifiAwareNetworkSpecifier(
                ns.type,
                1 - ns.role, // corruption hack
                ns.clientId,
                ns.sessionId,
                ns.peerId,
                ns.peerMac,
                ns.pmk,
                ns.passphrase,
                0,
                0,
                ns.requestorUid);
        nr.networkCapabilities.setNetworkSpecifier(ns);

        Message reqNetworkMsg = Message.obtain();
        reqNetworkMsg.what = NetworkFactory.CMD_REQUEST_NETWORK;
        reqNetworkMsg.obj = nr;
        reqNetworkMsg.arg1 = 0;
        res.mMessenger.send(reqNetworkMsg);
        mMockLooper.dispatchAll();

        // consequences of failure:
        //   Responder (publisher): responds with a rejection to any data-path requests
        //   Initiator (subscribe): doesn't initiate (i.e. no HAL requests)
        verifyUnfullfillableDispatched(res.mReverseMessenger);
        if (doPublish) {
            // (2) get request & respond
            mDut.onDataPathRequestNotification(pubSubId, peerDiscoveryMac, ndpId, null);
            mMockLooper.dispatchAll();
            inOrder.verify(mMockNative).respondToDataPathRequest(anyShort(), eq(false),
                    eq(ndpId), eq(""), eq(null), eq(null), eq(null), anyBoolean(), any());
        }

        verifyNoMoreInteractions(mMockNative, mMockCm, mAwareMetricsMock, mMockNwMgt);
    }

    private void testDataPathInitiatorResponderInvalidUidUtility(boolean doPublish,
            boolean isUidSet) throws Exception {
        final int clientId = 123;
        final byte pubSubId = 56;
        final int ndpId = 2;
        final byte[] pmk = "01234567890123456789012345678901".getBytes();
        final int requestorId = 1341234;
        final byte[] peerDiscoveryMac = HexEncoding.decode("000102030405".toCharArray(), false);

        InOrder inOrder = inOrder(mMockNative, mMockCm, mMockCallback, mMockSessionCallback);
        InOrder inOrderM = inOrder(mAwareMetricsMock);

        // (0) initialize
        DataPathEndPointInfo res = initDataPathEndPoint(true, clientId, pubSubId, requestorId,
                peerDiscoveryMac, inOrder, inOrderM, doPublish);

        // (1) create network request
        NetworkRequest nr = getSessionNetworkRequest(clientId, res.mSessionId, res.mPeerHandle, pmk,
                null, doPublish, 0);

        // (2) corrupt request's UID
        WifiAwareNetworkSpecifier ns =
                (WifiAwareNetworkSpecifier) nr.networkCapabilities.getNetworkSpecifier();
        ns = new WifiAwareNetworkSpecifier(
                ns.type,
                ns.role,
                ns.clientId,
                ns.sessionId,
                ns.peerId,
                ns.peerMac,
                ns.pmk,
                ns.passphrase,
                0,
                0,
                ns.requestorUid + 1); // corruption hack
        nr.networkCapabilities.setNetworkSpecifier(ns);

        // (3) request network
        Message reqNetworkMsg = Message.obtain();
        reqNetworkMsg.what = NetworkFactory.CMD_REQUEST_NETWORK;
        reqNetworkMsg.obj = nr;
        reqNetworkMsg.arg1 = 0;
        res.mMessenger.send(reqNetworkMsg);
        mMockLooper.dispatchAll();

        // consequences of failure:
        //   Responder (publisher): responds with a rejection to any data-path requests
        //   Initiator (subscribe): doesn't initiate (i.e. no HAL requests)
        verifyUnfullfillableDispatched(res.mReverseMessenger);
        if (doPublish) {
            // (2) get request & respond
            mDut.onDataPathRequestNotification(pubSubId, peerDiscoveryMac, ndpId, null);
            mMockLooper.dispatchAll();
            inOrder.verify(mMockNative).respondToDataPathRequest(anyShort(), eq(false),
                    eq(ndpId), eq(""), eq(null), eq(null), eq(null), anyBoolean(), any());
        }

        verifyNoMoreInteractions(mMockNative, mMockCm, mAwareMetricsMock, mMockNwMgt);
    }

    private void testDataPathInitiatorUtility(boolean useDirect, boolean provideMac,
            boolean providePmk, boolean providePassphrase, boolean getConfirmation,
            boolean immediateHalFailure) throws Exception {
        final byte[] peerDataPathMac = HexEncoding.decode("0A0B0C0D0E0F".toCharArray(), false);
        String linkLocalIpv6Address = MacAddress.fromBytes(
                peerDataPathMac).getLinkLocalIpv6FromEui48Mac().getHostAddress();

        testDataPathInitiatorUtilityMore(useDirect, provideMac, providePmk, providePassphrase,
                getConfirmation, immediateHalFailure, peerDataPathMac, null, 0, -1,
                linkLocalIpv6Address, 0);
    }

    private void testDataPathInitiatorUtilityMore(boolean useDirect, boolean provideMac,
            boolean providePmk, boolean providePassphrase, boolean getConfirmation,
            boolean immediateHalFailure, byte[] peerDataPathMac, byte[] peerToken, int port,
            int transportProtocol, String ipv6Address, int numAddrValidationRetries)
            throws Exception {
        final int clientId = 123;
        final byte pubSubId = 58;
        final int requestorId = 1341234;
        final int ndpId = 2;
        final byte[] pmk = "01234567890123456789012345678901".getBytes();
        final String passphrase = "some passphrase";
        final byte[] peerDiscoveryMac = HexEncoding.decode("000102030405".toCharArray(), false);

        ArgumentCaptor<Messenger> messengerCaptor = ArgumentCaptor.forClass(Messenger.class);
        ArgumentCaptor<Short> transactionId = ArgumentCaptor.forClass(Short.class);
        ArgumentCaptor<NetworkCapabilities> netCapCaptor = ArgumentCaptor.forClass(
                NetworkCapabilities.class);
        InOrder inOrder = inOrder(mMockNative, mMockCm, mMockCallback, mMockSessionCallback,
                mMockNwMgt, mMockNetworkInterface);
        InOrder inOrderM = inOrder(mAwareMetricsMock);

        if (!providePmk) {
            when(mPermissionsWrapperMock.getUidPermission(
                    eq(Manifest.permission.CONNECTIVITY_INTERNAL), anyInt())).thenReturn(
                    PackageManager.PERMISSION_DENIED);
        }

        if (immediateHalFailure) {
            when(mMockNative.initiateDataPath(anyShort(), anyInt(), anyInt(), anyInt(), any(),
                    any(), any(), any(), anyBoolean(), any(), any())).thenReturn(false);

        }

        // (0) initialize
        DataPathEndPointInfo res = initDataPathEndPoint(true, clientId, pubSubId, requestorId,
                peerDiscoveryMac, inOrder, inOrderM, false);

        // (1) request network
        NetworkRequest nr;
        if (useDirect) {
            nr = getDirectNetworkRequest(clientId,
                    WifiAwareManager.WIFI_AWARE_DATA_PATH_ROLE_INITIATOR,
                    provideMac ? peerDiscoveryMac : null, providePmk ? pmk : null,
                    providePassphrase ? passphrase : null, 0);
        } else {
            nr = getSessionNetworkRequest(clientId, res.mSessionId,
                    provideMac ? res.mPeerHandle : null, providePmk ? pmk : null,
                    providePassphrase ? passphrase : null, false, 0);
        }

        Message reqNetworkMsg = Message.obtain();
        reqNetworkMsg.what = NetworkFactory.CMD_REQUEST_NETWORK;
        reqNetworkMsg.obj = nr;
        reqNetworkMsg.arg1 = 0;
        res.mMessenger.send(reqNetworkMsg);
        mMockLooper.dispatchAll();
        inOrder.verify(mMockNative).initiateDataPath(transactionId.capture(),
                eq(useDirect ? 0 : requestorId),
                eq(CHANNEL_NOT_REQUESTED), anyInt(), eq(peerDiscoveryMac),
                eq(sAwareInterfacePrefix + "0"), eq(providePmk ? pmk : null),
                eq(providePassphrase ? passphrase : null), eq(useDirect), any(), any());
        if (immediateHalFailure) {
            // short-circuit the rest of this test
            inOrderM.verify(mAwareMetricsMock).recordNdpStatus(eq(NanStatusType.INTERNAL_FAILURE),
                    eq(useDirect), anyLong());
            verifyUnfullfillableDispatched(res.mReverseMessenger);
            verifyNoMoreInteractions(mMockNative, mMockCm, mAwareMetricsMock);
            return;
        }

        mDut.onInitiateDataPathResponseSuccess(transactionId.getValue(), ndpId);
        mMockLooper.dispatchAll();

        // (2) get confirmation OR timeout
        if (getConfirmation) {
            if (numAddrValidationRetries > 0) {
                when(mMockNetworkInterface.isAddressUsable(any())).thenReturn(false);
            }
            when(mClock.getElapsedSinceBootMillis()).thenReturn(0L);

            mDut.onDataPathConfirmNotification(ndpId, peerDataPathMac, true, 0, peerToken, null);
            mMockLooper.dispatchAll();
            inOrder.verify(mMockNwMgt).setInterfaceUp(anyString());
            inOrder.verify(mMockNwMgt).enableIpv6(anyString());
            inOrder.verify(mMockNetworkInterface).configureAgentProperties(any(), any(), anyInt(),
                    any(), any(), any());
            inOrder.verify(mMockCm).registerNetworkAgent(messengerCaptor.capture(), any(), any(),
                    netCapCaptor.capture(), anyInt(), any(), anyInt());

            inOrder.verify(mMockNetworkInterface).isAddressUsable(any());
            boolean timedout = false;
            for (int i = 0; i < numAddrValidationRetries; ++i) {
                if (i == numAddrValidationRetries - 1) {
                    when(mMockNetworkInterface.isAddressUsable(any())).thenReturn(true);
                }

                long currentTime = (i + 1L)
                        * WifiAwareDataPathStateManager.ADDRESS_VALIDATION_RETRY_INTERVAL_MS;
                when(mClock.getElapsedSinceBootMillis()).thenReturn(currentTime);
                mMockLooper.moveTimeForward(
                        WifiAwareDataPathStateManager.ADDRESS_VALIDATION_RETRY_INTERVAL_MS + 1);
                mMockLooper.dispatchAll();
                inOrder.verify(mMockNetworkInterface).isAddressUsable(any());

                if (currentTime > WifiAwareDataPathStateManager.ADDRESS_VALIDATION_TIMEOUT_MS) {
                    timedout = true;
                    break;
                }
            }

            if (timedout) {
                verifyUnfullfillableDispatched(res.mReverseMessenger);
                inOrder.verify(mMockNative).endDataPath(transactionId.capture(), eq(ndpId));
                mDut.onEndDataPathResponse(transactionId.getValue(), true, 0);
            } else {
                inOrder.verify(mMockNetworkInterface).sendAgentNetworkInfo(any(), any());
                inOrderM.verify(mAwareMetricsMock).recordNdpStatus(eq(NanStatusType.SUCCESS),
                        eq(useDirect), anyLong());
                inOrderM.verify(mAwareMetricsMock).recordNdpCreation(anyInt(), any());
                WifiAwareNetworkInfo netInfo =
                        (WifiAwareNetworkInfo) netCapCaptor.getValue().getTransportInfo();
                assertEquals(ipv6Address, netInfo.getPeerIpv6Addr().getHostAddress());
                assertEquals(port, netInfo.getPort());
                assertEquals(transportProtocol, netInfo.getTransportProtocol());
            }
        } else {
            assertTrue(mAlarmManager.dispatch(
                    WifiAwareStateManager.HAL_DATA_PATH_CONFIRM_TIMEOUT_TAG));
            mMockLooper.dispatchAll();
            verifyUnfullfillableDispatched(res.mReverseMessenger);
            inOrder.verify(mMockNative).endDataPath(transactionId.capture(), eq(ndpId));
            mDut.onEndDataPathResponse(transactionId.getValue(), true, 0);
            mMockLooper.dispatchAll();
            inOrderM.verify(mAwareMetricsMock).recordNdpStatus(eq(NanStatusType.INTERNAL_FAILURE),
                    eq(useDirect), anyLong());
        }

        // (3) end data-path (unless didn't get confirmation)
        if (getConfirmation) {
            Message endNetworkReqMsg = Message.obtain();
            endNetworkReqMsg.what = NetworkFactory.CMD_CANCEL_REQUEST;
            endNetworkReqMsg.obj = nr;
            res.mMessenger.send(endNetworkReqMsg);

            Message endNetworkUsageMsg = Message.obtain();
            endNetworkUsageMsg.what = AsyncChannel.CMD_CHANNEL_DISCONNECTED;
            messengerCaptor.getValue().send(endNetworkUsageMsg);
            mDut.onEndDataPathResponse(transactionId.getValue(), true, 0);
            mDut.onDataPathEndNotification(ndpId);
            mMockLooper.dispatchAll();

            inOrder.verify(mMockNwMgt).setInterfaceDown(anyString());
            inOrder.verify(mMockNative).endDataPath(transactionId.capture(), eq(ndpId));
            inOrderM.verify(mAwareMetricsMock).recordNdpSessionDuration(anyLong());
        }

        verifyNoMoreInteractions(mMockNative, mMockCm, mAwareMetricsMock, mMockNwMgt,
                mMockNetworkInterface);
    }

    private void testDataPathResponderUtility(boolean useDirect, boolean provideMac,
            boolean providePmk, boolean providePassphrase, boolean getConfirmation)
            throws Exception {
        final int clientId = 123;
        final byte pubSubId = 60;
        final int requestorId = 1341234;
        final int ndpId = 2;
        final byte[] pmk = "01234567890123456789012345678901".getBytes();
        final String passphrase = "some passphrase";
        final byte[] peerDiscoveryMac = HexEncoding.decode("000102030405".toCharArray(), false);
        final byte[] peerDataPathMac = HexEncoding.decode("0A0B0C0D0E0F".toCharArray(), false);

        ArgumentCaptor<Messenger> messengerCaptor = ArgumentCaptor.forClass(Messenger.class);
        ArgumentCaptor<Short> transactionId = ArgumentCaptor.forClass(Short.class);
        ArgumentCaptor<NetworkCapabilities> netCapCaptor = ArgumentCaptor.forClass(
                NetworkCapabilities.class);
        InOrder inOrder = inOrder(mMockNative, mMockCm, mMockCallback, mMockSessionCallback,
                mMockNwMgt);
        InOrder inOrderM = inOrder(mAwareMetricsMock);

        boolean isLegacy = mWifiPermissionsUtil.isTargetSdkLessThan("anything",
                Build.VERSION_CODES.P, 0);

        if (providePmk) {
            when(mPermissionsWrapperMock.getUidPermission(
                    eq(Manifest.permission.CONNECTIVITY_INTERNAL), anyInt())).thenReturn(
                    PackageManager.PERMISSION_GRANTED);
        }

        // (0) initialize
        DataPathEndPointInfo res = initDataPathEndPoint(true, clientId, pubSubId, requestorId,
                peerDiscoveryMac, inOrder, inOrderM, true);

        // (1) request network
        NetworkRequest nr;
        if (useDirect) {
            nr = getDirectNetworkRequest(clientId,
                    WifiAwareManager.WIFI_AWARE_DATA_PATH_ROLE_RESPONDER,
                    provideMac ? peerDiscoveryMac : null, providePmk ? pmk : null,
                    providePassphrase ? passphrase : null, 0);
        } else {
            nr = getSessionNetworkRequest(clientId, res.mSessionId,
                    provideMac ? res.mPeerHandle : null, providePmk ? pmk : null,
                    providePassphrase ? passphrase : null, true, 0);
        }

        Message reqNetworkMsg = Message.obtain();
        reqNetworkMsg.what = NetworkFactory.CMD_REQUEST_NETWORK;
        reqNetworkMsg.obj = nr;
        reqNetworkMsg.arg1 = 0;
        res.mMessenger.send(reqNetworkMsg);
        mMockLooper.dispatchAll();

        // (2) get request & respond (if legacy)
        mDut.onDataPathRequestNotification(pubSubId, peerDiscoveryMac, ndpId, null);
        mMockLooper.dispatchAll();
        if (isLegacy) {
            inOrder.verify(mMockNative).respondToDataPathRequest(transactionId.capture(), eq(true),
                    eq(ndpId), eq(sAwareInterfacePrefix + "0"), eq(providePmk ? pmk : null),
                    eq(providePassphrase ? passphrase : null), eq(null), eq(useDirect), any());
            mDut.onRespondToDataPathSetupRequestResponse(transactionId.getValue(), true, 0);
            mMockLooper.dispatchAll();

            // (3) get confirmation OR timeout
            if (getConfirmation) {
                mDut.onDataPathConfirmNotification(ndpId, peerDataPathMac, true, 0, null, null);
                mMockLooper.dispatchAll();
                inOrder.verify(mMockNwMgt).setInterfaceUp(anyString());
                inOrder.verify(mMockNwMgt).enableIpv6(anyString());
                inOrder.verify(mMockCm).registerNetworkAgent(messengerCaptor.capture(), any(),
                        any(), netCapCaptor.capture(), anyInt(), any(), anyInt());
                inOrderM.verify(mAwareMetricsMock).recordNdpStatus(eq(NanStatusType.SUCCESS),
                        eq(useDirect), anyLong());
                inOrderM.verify(mAwareMetricsMock).recordNdpCreation(anyInt(), any());
                WifiAwareNetworkInfo netInfo =
                        (WifiAwareNetworkInfo) netCapCaptor.getValue().getTransportInfo();
                assertArrayEquals(MacAddress.fromBytes(
                        peerDataPathMac).getLinkLocalIpv6FromEui48Mac().getAddress(),
                        netInfo.getPeerIpv6Addr().getAddress());
                assertEquals(0, netInfo.getPort());
                assertEquals(-1, netInfo.getTransportProtocol());
            } else {
                assertTrue(mAlarmManager.dispatch(
                        WifiAwareStateManager.HAL_DATA_PATH_CONFIRM_TIMEOUT_TAG));
                mMockLooper.dispatchAll();
                verifyUnfullfillableDispatched(res.mReverseMessenger);
                inOrder.verify(mMockNative).endDataPath(transactionId.capture(), eq(ndpId));
                mDut.onEndDataPathResponse(transactionId.getValue(), true, 0);
                mMockLooper.dispatchAll();
                inOrderM.verify(mAwareMetricsMock).recordNdpStatus(
                        eq(NanStatusType.INTERNAL_FAILURE), eq(useDirect), anyLong());
            }

            // (4) end data-path (unless didn't get confirmation)
            if (getConfirmation) {
                Message endNetworkMsg = Message.obtain();
                endNetworkMsg.what = NetworkFactory.CMD_CANCEL_REQUEST;
                endNetworkMsg.obj = nr;
                res.mMessenger.send(endNetworkMsg);

                Message endNetworkUsageMsg = Message.obtain();
                endNetworkUsageMsg.what = AsyncChannel.CMD_CHANNEL_DISCONNECTED;
                messengerCaptor.getValue().send(endNetworkUsageMsg);

                mDut.onEndDataPathResponse(transactionId.getValue(), true, 0);
                mDut.onDataPathEndNotification(ndpId);
                mMockLooper.dispatchAll();

                inOrder.verify(mMockNwMgt).setInterfaceDown(anyString());
                inOrder.verify(mMockNative).endDataPath(transactionId.capture(), eq(ndpId));
                inOrderM.verify(mAwareMetricsMock).recordNdpSessionDuration(anyLong());
            }
        } else {
            verifyUnfullfillableDispatched(res.mReverseMessenger);
            inOrder.verify(mMockNative).respondToDataPathRequest(transactionId.capture(), eq(false),
                    eq(ndpId), eq(""), eq(null), eq(null), eq(null), eq(false), any());
            mDut.onRespondToDataPathSetupRequestResponse(transactionId.getValue(), true, 0);
            mMockLooper.dispatchAll();
        }

        verifyNoMoreInteractions(mMockNative, mMockCm, mAwareMetricsMock, mMockNwMgt);
    }

    private NetworkRequest getSessionNetworkRequest(int clientId, int sessionId,
            PeerHandle peerHandle, byte[] pmk, String passphrase, boolean doPublish, int requestId)
            throws Exception {
        return getSessionNetworkRequestMore(clientId, sessionId, peerHandle, pmk, passphrase,
                doPublish, requestId, 0, -1);
    }

    private NetworkRequest getSessionNetworkRequestMore(int clientId, int sessionId,
            PeerHandle peerHandle, byte[] pmk, String passphrase, boolean doPublish, int requestId,
            int port, int transportProtocol)
            throws Exception {
        final IWifiAwareManager mockAwareService = mock(IWifiAwareManager.class);
        final WifiAwareManager mgr = new WifiAwareManager(mMockContext, mockAwareService);
        final ConfigRequest configRequest = new ConfigRequest.Builder().build();
        final PublishConfig publishConfig = new PublishConfig.Builder().build();
        final SubscribeConfig subscribeConfig = new SubscribeConfig.Builder().build();

        ArgumentCaptor<WifiAwareSession> sessionCaptor = ArgumentCaptor.forClass(
                WifiAwareSession.class);
        ArgumentCaptor<IWifiAwareEventCallback> clientProxyCallback = ArgumentCaptor
                .forClass(IWifiAwareEventCallback.class);
        ArgumentCaptor<IWifiAwareDiscoverySessionCallback> sessionProxyCallback = ArgumentCaptor
                .forClass(IWifiAwareDiscoverySessionCallback.class);
        ArgumentCaptor<DiscoverySession> discoverySession = ArgumentCaptor
                .forClass(DiscoverySession.class);

        AttachCallback mockCallback = mock(AttachCallback.class);
        DiscoverySessionCallback mockSessionCallback = mock(
                DiscoverySessionCallback.class);

        InOrder inOrderS = inOrder(mockAwareService, mockCallback, mockSessionCallback);

        mgr.attach(mMockLooperHandler, configRequest, mockCallback, null);
        inOrderS.verify(mockAwareService).connect(any(), any(),
                clientProxyCallback.capture(), eq(configRequest), eq(false));
        IWifiAwareEventCallback iwaec = clientProxyCallback.getValue();
        iwaec.onConnectSuccess(clientId);
        mMockLooper.dispatchAll();
        inOrderS.verify(mockCallback).onAttached(sessionCaptor.capture());
        if (doPublish) {
            sessionCaptor.getValue().publish(publishConfig, mockSessionCallback,
                    mMockLooperHandler);
            inOrderS.verify(mockAwareService).publish(any(), eq(clientId), eq(publishConfig),
                    sessionProxyCallback.capture());
        } else {
            sessionCaptor.getValue().subscribe(subscribeConfig, mockSessionCallback,
                    mMockLooperHandler);
            inOrderS.verify(mockAwareService).subscribe(any(), eq(clientId), eq(subscribeConfig),
                    sessionProxyCallback.capture());
        }
        sessionProxyCallback.getValue().onSessionStarted(sessionId);
        mMockLooper.dispatchAll();
        if (doPublish) {
            inOrderS.verify(mockSessionCallback).onPublishStarted(
                    (PublishDiscoverySession) discoverySession.capture());
        } else {
            inOrderS.verify(mockSessionCallback).onSubscribeStarted(
                    (SubscribeDiscoverySession) discoverySession.capture());
        }

        NetworkSpecifier ns;
        if (pmk == null && passphrase == null) {
            ns = createNetworkSpecifier(clientId,
                    doPublish ? WifiAwareManager.WIFI_AWARE_DATA_PATH_ROLE_RESPONDER
                            : WifiAwareManager.WIFI_AWARE_DATA_PATH_ROLE_INITIATOR, sessionId,
                    peerHandle, null, null, port, transportProtocol);
        } else if (passphrase == null) {
            ns = createNetworkSpecifier(clientId,
                    doPublish ? WifiAwareManager.WIFI_AWARE_DATA_PATH_ROLE_RESPONDER
                            : WifiAwareManager.WIFI_AWARE_DATA_PATH_ROLE_INITIATOR, sessionId,
                    peerHandle, pmk, null, port, transportProtocol);
        } else {
            ns = createNetworkSpecifier(clientId,
                    doPublish ? WifiAwareManager.WIFI_AWARE_DATA_PATH_ROLE_RESPONDER
                            : WifiAwareManager.WIFI_AWARE_DATA_PATH_ROLE_INITIATOR, sessionId,
                    peerHandle, null, passphrase, port, transportProtocol);
        }

        NetworkCapabilities nc = new NetworkCapabilities();
        nc.clearAll();
        nc.addTransportType(NetworkCapabilities.TRANSPORT_WIFI_AWARE);
        nc.addCapability(NetworkCapabilities.NET_CAPABILITY_NOT_VPN).addCapability(
                NetworkCapabilities.NET_CAPABILITY_NOT_METERED);
        nc.setNetworkSpecifier(ns);
        nc.setLinkUpstreamBandwidthKbps(1);
        nc.setLinkDownstreamBandwidthKbps(1);
        nc.setSignalStrength(1);

        return new NetworkRequest(nc, 0, requestId, NetworkRequest.Type.REQUEST);
    }

    private NetworkRequest getDirectNetworkRequest(int clientId, int role, byte[] peer,
            byte[] pmk, String passphrase, int requestId) throws Exception {
        return getDirectNetworkRequestMore(clientId, role, peer, pmk, passphrase, requestId, 0, -1);
    }

    private NetworkRequest getDirectNetworkRequestMore(int clientId, int role, byte[] peer,
            byte[] pmk, String passphrase, int requestId, int port, int transportProtocol)
            throws Exception {
        final IWifiAwareManager mockAwareService = mock(IWifiAwareManager.class);
        final ConfigRequest configRequest = new ConfigRequest.Builder().build();
        final WifiAwareManager mgr = new WifiAwareManager(mMockContext, mockAwareService);

        ArgumentCaptor<WifiAwareSession> sessionCaptor = ArgumentCaptor.forClass(
                WifiAwareSession.class);
        ArgumentCaptor<IWifiAwareEventCallback> clientProxyCallback = ArgumentCaptor
                .forClass(IWifiAwareEventCallback.class);

        AttachCallback mockCallback = mock(AttachCallback.class);

        mgr.attach(mMockLooperHandler, configRequest, mockCallback, null);
        verify(mockAwareService).connect(any(), any(),
                clientProxyCallback.capture(), eq(configRequest), eq(false));
        clientProxyCallback.getValue().onConnectSuccess(clientId);
        mMockLooper.dispatchAll();
        verify(mockCallback).onAttached(sessionCaptor.capture());

        NetworkSpecifier ns;
        if (pmk == null && passphrase == null) {
            ns = createNetworkSpecifier(clientId, role, peer, null, null, port, transportProtocol);
        } else if (passphrase == null) {
            ns = createNetworkSpecifier(clientId, role, peer, pmk, null, port, transportProtocol);
        } else {
            ns = createNetworkSpecifier(clientId, role, peer, null, passphrase, port,
                    transportProtocol);
        }
        NetworkCapabilities nc = new NetworkCapabilities();
        nc.clearAll();
        nc.addTransportType(NetworkCapabilities.TRANSPORT_WIFI_AWARE);
        nc.addCapability(NetworkCapabilities.NET_CAPABILITY_NOT_VPN)
            .addCapability(NetworkCapabilities.NET_CAPABILITY_NOT_METERED)
            .addCapability(NetworkCapabilities.NET_CAPABILITY_NOT_ROAMING)
            .addCapability(NetworkCapabilities.NET_CAPABILITY_NOT_CONGESTED);
        nc.setNetworkSpecifier(ns);
        nc.setLinkUpstreamBandwidthKbps(1);
        nc.setLinkDownstreamBandwidthKbps(1);
        nc.setSignalStrength(1);

        return new NetworkRequest(nc, 0, requestId, NetworkRequest.Type.REQUEST);
    }

    private DataPathEndPointInfo initDataPathEndPoint(boolean isFirstIteration, int clientId,
            byte pubSubId, int requestorId, byte[] peerDiscoveryMac, InOrder inOrder,
            InOrder inOrderM, boolean doPublish)
            throws Exception {
        final String someMsg = "some arbitrary message from peer";
        final PublishConfig publishConfig = new PublishConfig.Builder().build();
        final SubscribeConfig subscribeConfig = new SubscribeConfig.Builder().build();

        ArgumentCaptor<Short> transactionId = ArgumentCaptor.forClass(Short.class);
        ArgumentCaptor<Integer> sessionId = ArgumentCaptor.forClass(Integer.class);
        ArgumentCaptor<Integer> peerIdCaptor = ArgumentCaptor.forClass(Integer.class);

        Pair<Messenger, Messenger> res = initOobDataPathEndPoint(isFirstIteration, 1, clientId,
                inOrder, inOrderM);

        if (doPublish) {
            mDut.publish(clientId, publishConfig, mMockSessionCallback);
        } else {
            mDut.subscribe(clientId, subscribeConfig, mMockSessionCallback);
        }
        mMockLooper.dispatchAll();
        if (doPublish) {
            inOrder.verify(mMockNative).publish(transactionId.capture(), eq((byte) 0),
                    eq(publishConfig));
        } else {
            inOrder.verify(mMockNative).subscribe(transactionId.capture(), eq((byte) 0),
                    eq(subscribeConfig));
        }
        mDut.onSessionConfigSuccessResponse(transactionId.getValue(), doPublish, pubSubId);
        mMockLooper.dispatchAll();
        inOrder.verify(mMockSessionCallback).onSessionStarted(sessionId.capture());
        inOrderM.verify(mAwareMetricsMock).recordDiscoverySession(eq(Process.myUid()), any());
        inOrderM.verify(mAwareMetricsMock).recordDiscoveryStatus(Process.myUid(),
                NanStatusType.SUCCESS, doPublish);

        mDut.onMessageReceivedNotification(pubSubId, requestorId, peerDiscoveryMac,
                someMsg.getBytes());
        mMockLooper.dispatchAll();
        inOrder.verify(mMockSessionCallback).onMessageReceived(peerIdCaptor.capture(),
                eq(someMsg.getBytes()));

        return new DataPathEndPointInfo(sessionId.getValue(), peerIdCaptor.getValue(),
                isFirstIteration ? res.first : null, isFirstIteration ? res.second : null);
    }

    private Pair<Messenger, Messenger> initOobDataPathEndPoint(boolean startUpSequence,
            int maxNdiInterfaces, int clientId, InOrder inOrder, InOrder inOrderM)
            throws Exception {
        final int pid = 2000;
        final String callingPackage = "com.android.somePackage";
        final ConfigRequest configRequest = new ConfigRequest.Builder().build();

        ArgumentCaptor<Short> transactionId = ArgumentCaptor.forClass(Short.class);
        ArgumentCaptor<Messenger> messengerCaptor = ArgumentCaptor.forClass(Messenger.class);
        ArgumentCaptor<String> strCaptor = ArgumentCaptor.forClass(String.class);

        Capabilities capabilities = new Capabilities();
        capabilities.maxNdiInterfaces = maxNdiInterfaces;

        if (startUpSequence) {
            // (0) start/registrations
            inOrder.verify(mMockCm).registerNetworkFactory(messengerCaptor.capture(),
                    strCaptor.capture());
            collector.checkThat("factory name", "WIFI_AWARE_FACTORY",
                    equalTo(strCaptor.getValue()));

            // (1) get capabilities
            mDut.queryCapabilities();
            mMockLooper.dispatchAll();
            inOrder.verify(mMockNative).getCapabilities(transactionId.capture());
            mDut.onCapabilitiesUpdateResponse(transactionId.getValue(), capabilities);
            mMockLooper.dispatchAll();

            // (2) enable usage
            mDut.enableUsage();
            mMockLooper.dispatchAll();
            inOrderM.verify(mAwareMetricsMock).recordEnableUsage();
        }

        // (3) create client
        mDut.connect(clientId, Process.myUid(), pid, callingPackage, mMockCallback,
                configRequest,
                false);
        mMockLooper.dispatchAll();

        if (startUpSequence) {
            inOrder.verify(mMockNative).enableAndConfigure(transactionId.capture(),
                    eq(configRequest), eq(false), eq(true), eq(true), eq(false));
            mDut.onConfigSuccessResponse(transactionId.getValue());
            mMockLooper.dispatchAll();
        }

        inOrder.verify(mMockCallback).onConnectSuccess(clientId);
        inOrderM.verify(mAwareMetricsMock).recordAttachSession(eq(Process.myUid()), eq(false),
                any());

        if (startUpSequence) {
            for (int i = 0; i < maxNdiInterfaces; ++i) {
                inOrder.verify(mMockNative).createAwareNetworkInterface(transactionId.capture(),
                        strCaptor.capture());
                collector.checkThat("interface created -- " + i, sAwareInterfacePrefix + i,
                        equalTo(strCaptor.getValue()));
                mDut.onCreateDataPathInterfaceResponse(transactionId.getValue(), true, 0);
                mMockLooper.dispatchAll();
            }
            Messenger messenger = messengerCaptor.getValue();

            Messenger reverseMessenger = null;
            if (messenger != null) {
                reverseMessenger = mock(Messenger.class);

                Message fullConnectionMessage = Message.obtain();
                fullConnectionMessage.what = AsyncChannel.CMD_CHANNEL_FULL_CONNECTION;
                fullConnectionMessage.replyTo = reverseMessenger;
                messenger.send(fullConnectionMessage);
                mMockLooper.dispatchAll();
            }

            return Pair.create(messenger, reverseMessenger);
        }

        return null;
    }

    /**
     * Copy of DiscoverySession.createNetworkSpecifier - but without any checks! Allows creating
     * network requests which may not be valid (e.g. for the API level).
     */
    public NetworkSpecifier createNetworkSpecifier(int clientId, int role, int sessionId,
            PeerHandle peerHandle, byte[] pmk, String passphrase, int port, int transportProtocol) {
        return new WifiAwareNetworkSpecifier(
                (peerHandle == null) ? WifiAwareNetworkSpecifier.NETWORK_SPECIFIER_TYPE_IB_ANY_PEER
                        : WifiAwareNetworkSpecifier.NETWORK_SPECIFIER_TYPE_IB,
                role,
                clientId,
                sessionId,
                peerHandle != null ? peerHandle.peerId : 0, // 0 is an invalid peer ID
                null, // peerMac (not used in this method)
                pmk,
                passphrase,
                port,
                transportProtocol,
                Process.myUid());
    }

    /**
     * Copy of WifiAwareSession.createNetworkSpecifier - but without any checks! Allows creating
     * network requests which may not be valid (e.g. for the API level).
     */
    private NetworkSpecifier createNetworkSpecifier(int clientId, int role, byte[] peer, byte[] pmk,
            String passphrase, int port, int transportProtocol) {
        return new WifiAwareNetworkSpecifier(
                (peer == null) ? WifiAwareNetworkSpecifier.NETWORK_SPECIFIER_TYPE_OOB_ANY_PEER
                        : WifiAwareNetworkSpecifier.NETWORK_SPECIFIER_TYPE_OOB,
                role,
                clientId,
                0, // 0 is an invalid session ID
                0, // 0 is an invalid peer ID
                peer,
                pmk,
                passphrase,
                port,
                transportProtocol,
                Process.myUid());
    }

    private static class DataPathEndPointInfo {
        int mSessionId;
        PeerHandle mPeerHandle;
        Messenger mMessenger;
        Messenger mReverseMessenger;

        DataPathEndPointInfo(int sessionId, int peerId, Messenger messenger,
                Messenger reverseMessenger) {
            mSessionId = sessionId;
            mPeerHandle = new PeerHandle(peerId);
            mMessenger = messenger;
            mReverseMessenger = reverseMessenger;
        }
    }

    /**
     * Verify that an EVENT_UNFULFILLABLE_REQUEST was dispatched on the (mock) messenger.
     */
    private void verifyUnfullfillableDispatched(Messenger messenger) throws Exception {
        ArgumentCaptor<Message> messageCaptor = ArgumentCaptor.forClass(Message.class);

        verify(messenger, atLeastOnce()).send(messageCaptor.capture());
        assertEquals(NetworkFactory.EVENT_UNFULFILLABLE_REQUEST, messageCaptor.getValue().what);
    }

    // copy of the method in WifiAwareDataPathStateManager - but without error checking (so we can
    // construct invalid TLVs for testing).
    private static byte[] buildTlv(int port, int transportProtocol, boolean includeGarbageTlv) {
        if (port == 0 && transportProtocol == -1) {
            return null;
        }

        TlvBufferUtils.TlvConstructor tlvc = new TlvBufferUtils.TlvConstructor(1, 2);
        tlvc.setByteOrder(ByteOrder.LITTLE_ENDIAN);
        tlvc.allocate(30); // safe size for now

        tlvc.putRawByteArray(WifiAwareDataPathStateManager.NetworkInformationData.WFA_OUI);
        tlvc.putRawByte((byte) WifiAwareDataPathStateManager.NetworkInformationData
                .GENERIC_SERVICE_PROTOCOL_TYPE);

        if (port != 0) {
            tlvc.putShort(WifiAwareDataPathStateManager.NetworkInformationData.SUB_TYPE_PORT,
                    (short) port);
        }
        if (transportProtocol != -1) {
            tlvc.putByte(WifiAwareDataPathStateManager.NetworkInformationData
                    .SUB_TYPE_TRANSPORT_PROTOCOL, (byte) transportProtocol);
        }
        if (includeGarbageTlv) {
            tlvc.putShort(55, (short) -1298);
        }

        byte[] subTypes = tlvc.getArray();

        tlvc.allocate(30);
        tlvc.putByteArray(WifiAwareDataPathStateManager.NetworkInformationData.SERVICE_INFO_TYPE,
                subTypes);
        if (includeGarbageTlv) {
            tlvc.putInt(78, 44);
        }

        return tlvc.getArray();
    }
}
