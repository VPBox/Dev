/*
 * Copyright (C) 2018 The Android Open Source Project
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
import static org.junit.Assert.assertNotEquals;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertNull;
import static org.junit.Assert.assertTrue;
import static org.mockito.Matchers.any;
import static org.mockito.Matchers.anyBoolean;
import static org.mockito.Matchers.anyInt;
import static org.mockito.Matchers.anyString;
import static org.mockito.Matchers.eq;
import static org.mockito.Matchers.isNull;
import static org.mockito.Mockito.atLeastOnce;
import static org.mockito.Mockito.doAnswer;
import static org.mockito.Mockito.doNothing;
import static org.mockito.Mockito.doThrow;
import static org.mockito.Mockito.never;
import static org.mockito.Mockito.reset;
import static org.mockito.Mockito.spy;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;

import android.app.test.MockAnswerUtil.AnswerWithArguments;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.pm.PackageManager;
import android.content.res.Resources;
import android.location.LocationManager;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.wifi.WifiManager;
import android.net.wifi.WpsInfo;
import android.net.wifi.p2p.WifiP2pConfig;
import android.net.wifi.p2p.WifiP2pDevice;
import android.net.wifi.p2p.WifiP2pDeviceList;
import android.net.wifi.p2p.WifiP2pGroup;
import android.net.wifi.p2p.WifiP2pGroupList;
import android.net.wifi.p2p.WifiP2pInfo;
import android.net.wifi.p2p.WifiP2pManager;
import android.net.wifi.p2p.WifiP2pWfdInfo;
import android.net.wifi.p2p.nsd.WifiP2pServiceInfo;
import android.net.wifi.p2p.nsd.WifiP2pServiceRequest;
import android.os.Binder;
import android.os.Bundle;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.INetworkManagementService;
import android.os.Message;
import android.os.Messenger;
import android.os.UserHandle;
import android.os.UserManager;
import android.os.test.TestLooper;
import android.provider.Settings;

import androidx.test.filters.SmallTest;

import com.android.internal.R;
import com.android.server.wifi.FakeWifiLog;
import com.android.server.wifi.FrameworkFacade;
import com.android.server.wifi.HalDeviceManager;
import com.android.server.wifi.WifiInjector;
import com.android.server.wifi.nano.WifiMetricsProto.P2pConnectionEvent;
import com.android.server.wifi.util.WifiPermissionsUtil;
import com.android.server.wifi.util.WifiPermissionsWrapper;

import org.junit.Before;
import org.junit.Test;
import org.mockito.ArgumentCaptor;
import org.mockito.Mock;
import org.mockito.MockitoAnnotations;
import org.mockito.Spy;

import java.util.Arrays;
import java.util.List;

/**
 * Unit test harness for WifiP2pServiceImpl.
 */
@SmallTest
public class WifiP2pServiceImplTest {
    private static final String TAG = "WifiP2pServiceImplTest";
    private static final String IFACE_NAME_P2P = "mockP2p0";
    private static final long STATE_CHANGE_WAITING_TIME = 1000;
    private static final String thisDeviceMac = "11:22:33:44:55:66";
    private static final String thisDeviceName = "thisDeviceName";
    private static final String ANONYMIZED_DEVICE_ADDRESS = "02:00:00:00:00:00";

    private ArgumentCaptor<HalDeviceManager.InterfaceAvailableForRequestListener>
            mAvailListenerCaptor = ArgumentCaptor.forClass(
            HalDeviceManager.InterfaceAvailableForRequestListener.class);
    private ArgumentCaptor<BroadcastReceiver> mBcastRxCaptor = ArgumentCaptor.forClass(
            BroadcastReceiver.class);
    private Binder mClient1;
    private Binder mClient2;

    private BroadcastReceiver mLocationModeReceiver;
    private BroadcastReceiver mWifiStateChangedReceiver;
    private Handler mClientHandler;
    private Messenger mP2pStateMachineMessenger;
    private Messenger mClientMessenger;
    private WifiP2pServiceImpl mWifiP2pServiceImpl;
    private TestLooper mClientHanderLooper;
    private TestLooper mLooper;
    private WifiP2pConfig mTestWifiP2pPeerConfig;
    private WifiP2pConfig mTestWifiP2pFastConnectionConfig;
    private WifiP2pGroup mTestWifiP2pNewPersistentGoGroup;
    private WifiP2pGroup mTestWifiP2pGroup;
    private WifiP2pDevice mTestWifiP2pDevice;
    private WifiP2pGroupList mGroups = new WifiP2pGroupList(null, null);
    private WifiP2pDevice mTestThisDevice;
    private ArgumentCaptor<Message> mMessageCaptor = ArgumentCaptor.forClass(Message.class);

    @Mock Context mContext;
    @Mock FrameworkFacade mFrameworkFacade;
    @Mock HandlerThread mHandlerThread;
    @Mock INetworkManagementService mNwService;
    @Mock PackageManager mPackageManager;
    @Mock Resources mResources;
    @Mock WifiInjector mWifiInjector;
    @Mock WifiManager mMockWifiManager;
    @Mock WifiPermissionsUtil mWifiPermissionsUtil;
    @Mock WifiPermissionsWrapper mWifiPermissionsWrapper;
    @Mock WifiP2pNative mWifiNative;
    @Mock WifiP2pServiceInfo mTestWifiP2pServiceInfo;
    @Mock WifiP2pServiceRequest mTestWifiP2pServiceRequest;
    @Mock UserManager mUserManager;
    @Mock WifiP2pMetrics mWifiP2pMetrics;
    @Spy FakeWifiLog mLog;
    @Spy MockWifiP2pMonitor mWifiMonitor;


    private void generatorTestData() {
        mTestWifiP2pGroup = new WifiP2pGroup();
        mTestWifiP2pGroup.setNetworkName("TestGroupName");
        mTestWifiP2pDevice = spy(new WifiP2pDevice());
        mTestWifiP2pDevice.deviceName = "TestDeviceName";
        mTestWifiP2pDevice.deviceAddress = "aa:bb:cc:dd:ee:ff";

        // for general connect command
        mTestWifiP2pPeerConfig = new WifiP2pConfig();
        mTestWifiP2pPeerConfig.deviceAddress = mTestWifiP2pDevice.deviceAddress;

        // for fast-connection connect command
        mTestWifiP2pFastConnectionConfig = new WifiP2pConfig.Builder()
                .setNetworkName("DIRECT-XY-HELLO")
                .setPassphrase("DEADBEEF")
                .build();

        // for general group started event
        mTestWifiP2pNewPersistentGoGroup = new WifiP2pGroup();
        mTestWifiP2pNewPersistentGoGroup.setNetworkId(WifiP2pGroup.PERSISTENT_NET_ID);
        mTestWifiP2pNewPersistentGoGroup.setNetworkName("DIRECT-xy-NEW");
        mTestWifiP2pNewPersistentGoGroup.setOwner(new WifiP2pDevice(thisDeviceMac));
        mTestWifiP2pNewPersistentGoGroup.setIsGroupOwner(true);

        mGroups.clear();
        WifiP2pGroup group1 = new WifiP2pGroup();
        group1.setNetworkId(0);
        group1.setNetworkName(mTestWifiP2pGroup.getNetworkName());
        group1.setOwner(mTestWifiP2pDevice);
        group1.setIsGroupOwner(false);
        mGroups.add(group1);

        WifiP2pGroup group2 = new WifiP2pGroup();
        group2.setNetworkId(1);
        group2.setNetworkName("DIRECT-ab-Hello");
        group2.setOwner(new WifiP2pDevice("12:34:56:78:90:ab"));
        group2.setIsGroupOwner(false);
        mGroups.add(group2);

        WifiP2pGroup group3 = new WifiP2pGroup();
        group3.setNetworkId(2);
        group3.setNetworkName("DIRECT-cd-OWNER");
        group3.setOwner(new WifiP2pDevice(thisDeviceMac));
        group3.setIsGroupOwner(true);
        mGroups.add(group3);

        mTestThisDevice = new WifiP2pDevice();
        mTestThisDevice.deviceName = thisDeviceName;
        mTestThisDevice.deviceAddress = thisDeviceMac;
        mTestThisDevice.primaryDeviceType = "10-0050F204-5";
    }

    /**
     * Simulate Location Mode change: Changes the location manager return values and dispatches a
     * broadcast.
     *
     * @param isLocationModeEnabled whether the location mode is enabled.,
     */
    private void simulateLocationModeChange(boolean isLocationModeEnabled) {
        when(mWifiPermissionsUtil.isLocationModeEnabled()).thenReturn(isLocationModeEnabled);

        Intent intent = new Intent(LocationManager.MODE_CHANGED_ACTION);
        mLocationModeReceiver.onReceive(mContext, intent);
    }

    /**
     * Simulate Wi-Fi state change: broadcast state change and modify the API return value.
     *
     * @param isWifiOn whether the wifi mode is enabled.
     */
    private void simulateWifiStateChange(boolean isWifiOn) {
        when(mMockWifiManager.getWifiState()).thenReturn(
                isWifiOn ? WifiManager.WIFI_STATE_ENABLED : WifiManager.WIFI_STATE_DISABLED);

        Intent intent = new Intent(WifiManager.WIFI_STATE_CHANGED_ACTION);
        intent.putExtra(WifiManager.EXTRA_WIFI_STATE,
                isWifiOn ? WifiManager.WIFI_STATE_ENABLED : WifiManager.WIFI_STATE_DISABLED);
        mWifiStateChangedReceiver.onReceive(mContext, intent);
    }

    /**
     * Mock send WifiP2pManager.UPDATE_CHANNEL_INFO
     *
     * @param pkgName package name used for p2p channel init
     * @param binder client binder used for p2p channel init
     * @param replyMessenger for checking replied message.
     */
    private void sendChannelInfoUpdateMsg(String pkgName, Binder binder,
            Messenger replyMessenger) throws Exception {
        Message msg = Message.obtain();
        msg.what = WifiP2pManager.UPDATE_CHANNEL_INFO;
        Bundle bundle = new Bundle();
        bundle.putString(WifiP2pManager.CALLING_PACKAGE, pkgName);
        bundle.putBinder(WifiP2pManager.CALLING_BINDER, binder);
        msg.obj = bundle;
        msg.replyTo = replyMessenger;
        mP2pStateMachineMessenger.send(Message.obtain(msg));
        mLooper.dispatchAll();
    }

    /**
     * Mock send WifiP2pManager.ADD_LOCAL_SERVICE with mTestWifiP2pServiceInfo
     *
     * @param replyMessenger for checking replied message.
     */
    private void sendAddLocalServiceMsg(Messenger replyMessenger) throws Exception {
        Message msg = Message.obtain();
        msg.what = WifiP2pManager.ADD_LOCAL_SERVICE;
        msg.obj = mTestWifiP2pServiceInfo;
        msg.replyTo = replyMessenger;
        mP2pStateMachineMessenger.send(Message.obtain(msg));
        mLooper.dispatchAll();
    }

    /**
     * Mock send WifiP2pManager.CONNECT with ConfigValidAsGroup
     *
     * @param replyMessenger for checking replied message.
     */
    private void sendConnectMsgWithConfigValidAsGroup(Messenger replyMessenger) throws Exception {
        sendConnectMsg(replyMessenger, mTestWifiP2pFastConnectionConfig);
    }

    /**
     * Mock send WifiP2pManager.CREATE_GROUP with ConfigValidAsGroup
     *
     * @param replyMessenger for checking replied message.
     */
    private void sendCreateGroupMsgWithConfigValidAsGroup(Messenger replyMessenger)
            throws Exception {
        sendCreateGroupMsg(replyMessenger, 0, mTestWifiP2pFastConnectionConfig);
    }

    /**
     * Mock send WifiP2pManager.DISCOVER_PEERS
     *
     * @param replyMessenger for checking replied message.
     */
    private void sendDiscoverPeersMsg(Messenger replyMessenger) throws Exception {
        Message msg = Message.obtain();
        msg.what = WifiP2pManager.DISCOVER_PEERS;
        msg.replyTo = replyMessenger;
        mP2pStateMachineMessenger.send(Message.obtain(msg));
        mLooper.dispatchAll();
    }

    /**
     * Mock send WifiP2pManager.ADD_SERVICE_REQUEST with mocked mTestWifiP2pServiceRequest
     *
     * @param replyMessenger for checking replied message.
     */
    private void sendAddServiceRequestMsg(Messenger replyMessenger) throws Exception {
        Message msg = Message.obtain();
        msg.what = WifiP2pManager.ADD_SERVICE_REQUEST;
        msg.replyTo = replyMessenger;
        msg.obj = mTestWifiP2pServiceRequest;
        mP2pStateMachineMessenger.send(Message.obtain(msg));
        mLooper.dispatchAll();
    }

    /**
     * Mock send WifiP2pManager.DISCOVER_SERVICES
     *
     * @param replyMessenger for checking replied message.
     */
    private void sendDiscoverServiceMsg(Messenger replyMessenger) throws Exception {
        Message msg = Message.obtain();
        msg.what = WifiP2pManager.DISCOVER_SERVICES;
        msg.replyTo = replyMessenger;
        mP2pStateMachineMessenger.send(Message.obtain(msg));
        mLooper.dispatchAll();
    }

    /**
     * Mock send WifiP2pManager.REQUEST_PEERS
     *
     * @param replyMessenger for checking replied message.
     */
    private void sendRequestPeersMsg(Messenger replyMessenger) throws Exception {
        Message msg = Message.obtain();
        msg.what = WifiP2pManager.REQUEST_PEERS;
        msg.replyTo = replyMessenger;
        mP2pStateMachineMessenger.send(Message.obtain(msg));
        mLooper.dispatchAll();
    }

    /**
     * Mock send WifiP2pManager.REQUEST_GROUP_INFO
     *
     * @param replyMessenger for checking replied message.
     */
    private void sendRequestGroupInfoMsg(Messenger replyMessenger) throws Exception {
        Message msg = Message.obtain();
        msg.what = WifiP2pManager.REQUEST_GROUP_INFO;
        msg.replyTo = replyMessenger;
        mP2pStateMachineMessenger.send(Message.obtain(msg));
        mLooper.dispatchAll();
    }

    /**
     * Mock send WifiP2pManager.DELETE_PERSISTENT_GROUP.
     *
     * @param replyMessenger for checking replied message.
     * @param netId is the network id of the p2p group.
     */
    private void sendDeletePersistentGroupMsg(Messenger replyMessenger,
            int netId) throws Exception {
        Message msg = Message.obtain();
        msg.what = WifiP2pManager.DELETE_PERSISTENT_GROUP;
        msg.arg1 = netId;
        msg.replyTo = replyMessenger;
        mP2pStateMachineMessenger.send(Message.obtain(msg));
        mLooper.dispatchAll();
    }

    /**
     * Send WifiP2pMonitor.P2P_GROUP_STARTED_EVENT.
     *
     * @param group the started group.
     */
    private void sendGroupStartedMsg(WifiP2pGroup group) throws Exception {
        if (group.getNetworkId() == WifiP2pGroup.PERSISTENT_NET_ID) {
            mGroups.add(group);
        }

        Message msg = Message.obtain();
        msg.what = WifiP2pMonitor.P2P_GROUP_STARTED_EVENT;
        msg.obj = group;
        mP2pStateMachineMessenger.send(Message.obtain(msg));
        mLooper.dispatchAll();
    }

    /**
     * Send WifiP2pMonitor.P2P_GROUP_REMOVED_EVENT.
     */
    private void sendGroupRemovedMsg() throws Exception {
        Message msg = Message.obtain();
        msg.what = WifiP2pMonitor.P2P_GROUP_REMOVED_EVENT;
        mP2pStateMachineMessenger.send(Message.obtain(msg));
        mLooper.dispatchAll();
    }

    /**
     * Send WifiP2pMonitor.P2P_DEVICE_FOUND_EVENT.
     *
     * @param device the found device.
     */
    private void sendDeviceFoundEventMsg(WifiP2pDevice device) throws Exception {
        Message msg = Message.obtain();
        msg.what = WifiP2pMonitor.P2P_DEVICE_FOUND_EVENT;
        msg.obj = device;
        mP2pStateMachineMessenger.send(Message.obtain(msg));
        mLooper.dispatchAll();
    }

    /**
     * Send WifiP2pMonitor.P2P_INVITATION_RESULT_EVENT.
     *
     * @param status invitation result.
     */
    private void sendInvitationResultMsg(
            WifiP2pServiceImpl.P2pStatus status) throws Exception {
        Message msg = Message.obtain();
        msg.what = WifiP2pMonitor.P2P_INVITATION_RESULT_EVENT;
        msg.obj = status;
        mP2pStateMachineMessenger.send(Message.obtain(msg));
        mLooper.dispatchAll();
    }

    /**
     * Mock send WifiP2pManager.SET_CHANNEL
     *
     * @param replyMessenger for checking replied message.
     * @param p2pChannels stores the listen and operating channels.
     */
    private void sendSetChannelMsg(Messenger replyMessenger,
            Bundle p2pChannels) throws Exception {
        Message msg = Message.obtain();
        msg.what = WifiP2pManager.SET_CHANNEL;
        msg.replyTo = replyMessenger;
        msg.obj = p2pChannels;
        mP2pStateMachineMessenger.send(Message.obtain(msg));
        mLooper.dispatchAll();
    }

    /**
     * Mock send WifiP2pManager.START_WPS
     *
     * @param replyMessenger for checking replied message.
     * @param wps is the WPS configuration.
     */
    private void sendStartWpsMsg(Messenger replyMessenger, WpsInfo wps) throws Exception {
        Message msg = Message.obtain();
        msg.what = WifiP2pManager.START_WPS;
        msg.replyTo = replyMessenger;
        msg.obj = wps;
        mP2pStateMachineMessenger.send(Message.obtain(msg));
        mLooper.dispatchAll();
    }

    /**
     * Mock send WifiP2pManager.SET_DEVICE_NAME
     *
     * @param replyMessenger for checking replied message.
     * @param dev is the P2p device configuration.
     */
    private void sendSetDeviceNameMsg(
            Messenger replyMessenger, WifiP2pDevice dev) throws Exception {
        Message msg = Message.obtain();
        msg.what = WifiP2pManager.SET_DEVICE_NAME;
        msg.replyTo = replyMessenger;
        msg.obj = dev;
        mP2pStateMachineMessenger.send(Message.obtain(msg));
        mLooper.dispatchAll();
    }

    /**
     * Mock send WifiP2pManager.SET_WFD_INFO
     *
     * @param replyMessenger for checking replied message.
     * @param wfdInfo is the P2p device's wfd information.
     */
    private void sendSetWfdInfoMsg(
            Messenger replyMessenger, WifiP2pWfdInfo wfdInfo) throws Exception {
        Message msg = Message.obtain();
        msg.what = WifiP2pManager.SET_WFD_INFO;
        msg.replyTo = replyMessenger;
        msg.obj = wfdInfo;
        mP2pStateMachineMessenger.send(Message.obtain(msg));
        mLooper.dispatchAll();
    }

    /**
     * Mock send WifiP2pManager.SET_ONGOING_PEER_CONFIG
     *
     * @param replyMessenger for checking replied message.
     * @param config used for change an ongoing peer connection.
     */
    private void sendSetOngoingPeerConfigMsg(
            Messenger replyMessenger, WifiP2pConfig config) throws Exception {
        Message msg = Message.obtain();
        msg.what = WifiP2pManager.SET_ONGOING_PEER_CONFIG;
        msg.replyTo = replyMessenger;
        msg.obj = config;
        mP2pStateMachineMessenger.send(Message.obtain(msg));
        mLooper.dispatchAll();
    }

    /**
     * Mock send WifiP2pManager.REMOVE_LOCAL_SERVICE.
     *
     * @param replyMessenger for checking replied message.
     * @param servInfo is the local service information.
     */
    private void sendRemoveLocalServiceMsg(Messenger replyMessenger,
            WifiP2pServiceInfo servInfo) throws Exception {
        Message msg = Message.obtain();
        msg.what = WifiP2pManager.REMOVE_LOCAL_SERVICE;
        msg.obj = servInfo;
        msg.replyTo = replyMessenger;
        mP2pStateMachineMessenger.send(Message.obtain(msg));
        mLooper.dispatchAll();
    }

    /**
     * Mock send WifiP2pManager.REMOVE_SERVICE_REQUEST.
     *
     * @param replyMessenger for checking replied message.
     * @param req is the service discovery request.
     */
    private void sendRemoveServiceRequestMsg(Messenger replyMessenger,
            WifiP2pServiceRequest req) throws Exception {
        Message msg = Message.obtain();
        msg.what = WifiP2pManager.REMOVE_SERVICE_REQUEST;
        msg.obj = req;
        msg.replyTo = replyMessenger;
        mP2pStateMachineMessenger.send(Message.obtain(msg));
        mLooper.dispatchAll();
    }

    /**
     * Send Connect API msg.
     *
     * @param replyMessenger for checking replied message.
     * @param config options as described in {@link WifiP2pConfig} class.
     */
    private void sendConnectMsg(Messenger replyMessenger,
            WifiP2pConfig config) throws Exception {
        Message msg = Message.obtain();
        msg.what = WifiP2pManager.CONNECT;
        msg.obj = config;
        msg.replyTo = replyMessenger;
        mP2pStateMachineMessenger.send(Message.obtain(msg));
        mLooper.dispatchAll();
    }

    /**
     * Send CreateGroup API msg.
     *
     * @param replyMessenger for checking replied message.
     * @param config options as described in {@link WifiP2pConfig} class.
     */
    private void sendCreateGroupMsg(Messenger replyMessenger,
            int netId,
            WifiP2pConfig config) throws Exception {
        Message msg = Message.obtain();
        msg.what = WifiP2pManager.CREATE_GROUP;
        msg.arg1 = netId;
        msg.obj = config;
        msg.replyTo = replyMessenger;
        mP2pStateMachineMessenger.send(Message.obtain(msg));
        mLooper.dispatchAll();
    }

    /**
     * Send simple API msg.
     *
     * Mock the API msg without arguments.
     *
     * @param replyMessenger for checking replied message.
     */
    private void sendSimpleMsg(Messenger replyMessenger,
            int what) throws Exception {
        Message msg = Message.obtain();
        msg.what = what;
        if (replyMessenger != null) {
            msg.replyTo = replyMessenger;
        }
        mP2pStateMachineMessenger.send(Message.obtain(msg));
        mLooper.dispatchAll();
    }

    /**
     * force P2p State enter InactiveState to start others unit test
     *
     * @param clientBinder client binder to use for p2p channel init
     */
    private void forceP2pEnabled(Binder clientBinder) throws Exception {
        simulateWifiStateChange(true);
        simulateLocationModeChange(true);
        checkIsP2pInitWhenClientConnected(true, clientBinder);
    }

    /**
     * Check is P2p init as expected when client connected
     *
     * @param expectInit set true if p2p init should succeed as expected, set false when
     *        expected init should not happen
     * @param clientBinder client binder to use for p2p channel init
     */
    private void checkIsP2pInitWhenClientConnected(boolean expectInit, Binder clientBinder)
            throws Exception {
        mWifiP2pServiceImpl.getMessenger(clientBinder);
        mLooper.dispatchAll();
        if (expectInit) {
            verify(mWifiNative).setupInterface(any(), any());
            verify(mNwService).setInterfaceUp(anyString());
            verify(mWifiMonitor, atLeastOnce()).registerHandler(anyString(), anyInt(), any());
        } else {
            verify(mWifiNative, never()).setupInterface(any(), any());
            verify(mNwService, never()).setInterfaceUp(anyString());
            verify(mWifiMonitor, never()).registerHandler(anyString(), anyInt(), any());
        }
    }

    /**
     * Check is P2p teardown as expected when client disconnected
     *
     * @param expectTearDown set true if p2p teardown should succeed as expected,
     *        set false when expected teardown should not happen
     * @param clientBinder client binder to use for p2p channel init
     */
    private void checkIsP2pTearDownWhenClientDisconnected(
            boolean expectTearDown, Binder clientBinder) throws Exception {
        mWifiP2pServiceImpl.close(clientBinder);
        mLooper.dispatchAll();
        if (expectTearDown) {
            verify(mWifiNative).teardownInterface();
            verify(mWifiMonitor).stopMonitoring(anyString());
        } else {
            verify(mWifiNative, never()).teardownInterface();
            verify(mWifiMonitor, never()).stopMonitoring(anyString());
        }
    }

    /**
     * Check the broadcast of WIFI_P2P_THIS_DEVICE_CHANGED_ACTION is sent as expected.
     */
    private void checkSendThisDeviceChangedBroadcast() {
        ArgumentCaptor<Intent> intentCaptor = ArgumentCaptor.forClass(Intent.class);
        String[] permission_gold = new String[] {android.Manifest.permission.ACCESS_FINE_LOCATION,
                                                 android.Manifest.permission.ACCESS_WIFI_STATE};
        ArgumentCaptor<String []> permissionCaptor = ArgumentCaptor.forClass(String[].class);
        verify(mContext, atLeastOnce()).sendBroadcastAsUserMultiplePermissions(
                intentCaptor.capture(), eq(UserHandle.ALL), permissionCaptor.capture());
        String [] permission = permissionCaptor.getValue();
        Arrays.sort(permission);
        Arrays.sort(permission_gold);
        assertEquals(permission_gold, permission);
        Intent intent = intentCaptor.getValue();
        WifiP2pDevice device = intent.getParcelableExtra(WifiP2pManager.EXTRA_WIFI_P2P_DEVICE);
        assertEquals(WifiP2pManager.WIFI_P2P_THIS_DEVICE_CHANGED_ACTION, intent.getAction());
        assertEquals(Intent.FLAG_RECEIVER_REGISTERED_ONLY_BEFORE_BOOT, intent.getFlags());
        assertEquals(mTestThisDevice.deviceName, device.deviceName);
        assertEquals(ANONYMIZED_DEVICE_ADDRESS, device.deviceAddress);
        assertEquals(mTestThisDevice.primaryDeviceType, device.primaryDeviceType);
        assertEquals(mTestThisDevice.secondaryDeviceType, device.secondaryDeviceType);
        assertEquals(mTestThisDevice.wpsConfigMethodsSupported, device.wpsConfigMethodsSupported);
        assertEquals(mTestThisDevice.deviceCapability, device.deviceCapability);
        assertEquals(mTestThisDevice.groupCapability, device.groupCapability);
        assertEquals(mTestThisDevice.status, device.status);
        if (mTestThisDevice.wfdInfo != null) {
            assertEquals(mTestThisDevice.wfdInfo.isWfdEnabled(),
                    device.wfdInfo.isWfdEnabled());
            assertEquals(mTestThisDevice.wfdInfo.getDeviceInfoHex(),
                    device.wfdInfo.getDeviceInfoHex());
            assertEquals(mTestThisDevice.wfdInfo.getControlPort(),
                    device.wfdInfo.getControlPort());
            assertEquals(mTestThisDevice.wfdInfo.getMaxThroughput(),
                    device.wfdInfo.getMaxThroughput());
        } else {
            assertEquals(mTestThisDevice.wfdInfo, device.wfdInfo);
        }
    }

    /**
     * Check the broadcast of WIFI_P2P_PERSISTENT_GROUPS_CHANGED_ACTION is sent as expected.
     */
    private void checkSendP2pPersistentGroupsChangedBroadcast() {
        ArgumentCaptor<Intent> intentCaptor = ArgumentCaptor.forClass(Intent.class);
        verify(mContext, atLeastOnce()).sendStickyBroadcastAsUser(
                intentCaptor.capture(), eq(UserHandle.ALL));
        Intent intent = intentCaptor.getValue();
        assertEquals(WifiP2pManager.WIFI_P2P_PERSISTENT_GROUPS_CHANGED_ACTION, intent.getAction());
        assertEquals(Intent.FLAG_RECEIVER_REGISTERED_ONLY_BEFORE_BOOT, intent.getFlags());
    }

    /**
     * Set up the instance of WifiP2pServiceImpl for testing.
     *
     * @param supported defines the p2p is supported or not in this instance.
     */
    private void setUpWifiP2pServiceImpl(boolean supported) {
        reset(mContext, mFrameworkFacade, mHandlerThread, mPackageManager, mResources,
                mWifiInjector, mWifiNative);

        generatorTestData();
        mClientHanderLooper = new TestLooper();
        mClientHandler = spy(new Handler(mClientHanderLooper.getLooper()));
        mClientMessenger =  new Messenger(mClientHandler);
        mLooper = new TestLooper();

        when(mContext.getPackageManager()).thenReturn(mPackageManager);
        when(mContext.getResources()).thenReturn(mResources);
        when(mFrameworkFacade.getStringSetting(any(),
                eq(Settings.Global.WIFI_P2P_DEVICE_NAME))).thenReturn(thisDeviceName);
        when(mFrameworkFacade.getIntegerSetting(any(),
                eq(Settings.Global.WIFI_P2P_PENDING_FACTORY_RESET), eq(0))).thenReturn(0);
        when(mHandlerThread.getLooper()).thenReturn(mLooper.getLooper());
        if (supported) {
            when(mPackageManager.hasSystemFeature(eq(PackageManager.FEATURE_WIFI_DIRECT)))
                    .thenReturn(true);
        } else {
            when(mPackageManager.hasSystemFeature(eq(PackageManager.FEATURE_WIFI_DIRECT)))
                    .thenReturn(false);
        }
        when(mResources.getString(R.string.config_wifi_p2p_device_type))
                .thenReturn("10-0050F204-5");
        when(mWifiInjector.getFrameworkFacade()).thenReturn(mFrameworkFacade);
        when(mWifiInjector.getUserManager()).thenReturn(mUserManager);
        when(mWifiInjector.getWifiP2pMetrics()).thenReturn(mWifiP2pMetrics);
        when(mWifiInjector.getWifiP2pMonitor()).thenReturn(mWifiMonitor);
        when(mWifiInjector.getWifiP2pNative()).thenReturn(mWifiNative);
        when(mWifiInjector.getWifiP2pServiceHandlerThread()).thenReturn(mHandlerThread);
        when(mWifiInjector.getWifiPermissionsUtil()).thenReturn(mWifiPermissionsUtil);
        when(mWifiNative.setupInterface(any(), any())).thenReturn(IFACE_NAME_P2P);
        when(mWifiNative.p2pGetDeviceAddress()).thenReturn(thisDeviceMac);
        doAnswer(new AnswerWithArguments() {
            public boolean answer(WifiP2pGroupList groups) {
                groups.clear();
                for (WifiP2pGroup group : mGroups.getGroupList()) {
                    groups.add(group);
                }
                return true;
            }
        }).when(mWifiNative).p2pListNetworks(any(WifiP2pGroupList.class));
        doAnswer(new AnswerWithArguments() {
            public boolean answer(int netId) {
                mGroups.remove(netId);
                return true;
            }
        }).when(mWifiNative).removeP2pNetwork(anyInt());

        mWifiP2pServiceImpl = new WifiP2pServiceImpl(mContext, mWifiInjector);
        if (supported) {
            verify(mContext, times(2)).registerReceiver(mBcastRxCaptor.capture(),
                    any(IntentFilter.class));
            mWifiStateChangedReceiver = mBcastRxCaptor.getAllValues().get(0);
            mLocationModeReceiver = mBcastRxCaptor.getAllValues().get(1);
            verify(mWifiNative).registerInterfaceAvailableListener(
                    mAvailListenerCaptor.capture(), any(Handler.class));
            mAvailListenerCaptor.getValue().onAvailabilityChanged(true);
        }

        mWifiP2pServiceImpl.mNwService = mNwService;
        mP2pStateMachineMessenger = mWifiP2pServiceImpl.getP2pStateMachineMessenger();
        mWifiP2pServiceImpl.setWifiHandlerLogForTest(mLog);
        mWifiP2pServiceImpl.setWifiLogForReplyChannel(mLog);
    }

    @Before
    public void setUp() throws Exception {
        MockitoAnnotations.initMocks(this);
        setUpWifiP2pServiceImpl(true);
        mClient1 = new Binder();
        mClient2 = new Binder();
    }

    /**
     * Mock enter Disabled state.
     */
    private void mockEnterDisabledState() throws Exception {
        Message msg = Message.obtain();
        msg.what = WifiP2pMonitor.SUP_DISCONNECTION_EVENT;
        mP2pStateMachineMessenger.send(Message.obtain(msg));
        mLooper.dispatchAll();
    }

    /**
     * Mock enter GroupNegotiation state.
     */
    private void mockEnterGroupNegotiationState() throws Exception {
        sendCreateGroupMsg(mClientMessenger, WifiP2pGroup.TEMPORARY_NET_ID, null);
    }


    /**
     * Mock enter ProvisionDiscovery state.
     */
    private void mockEnterProvisionDiscoveryState() throws Exception {
        mockPeersList();
        sendConnectMsg(mClientMessenger, mTestWifiP2pPeerConfig);
    }

    /**
     * Mock WifiP2pServiceImpl.mPeers.
     */
    private void mockPeersList() throws Exception {
        sendDeviceFoundEventMsg(mTestWifiP2pDevice);
    }

    /**
     * Verify that p2p init / teardown whn a client connects / disconnects
     * with wifi enabled
     */
    @Test
    public void testP2pInitWhenClientConnectWithWifiEnabled() throws Exception {
        simulateWifiStateChange(true);
        checkIsP2pInitWhenClientConnected(true, mClient1);
        checkIsP2pTearDownWhenClientDisconnected(true, mClient1);
    }

    /**
     * Verify that p2p doesn't init when  a client connects / disconnects
     * with wifi disabled
     */
    @Test
    public void testP2pDoesntInitWhenClientConnectWithWifiDisabledEnabled()
            throws Exception {
        simulateWifiStateChange(false);
        checkIsP2pInitWhenClientConnected(false, mClient1);
        checkIsP2pTearDownWhenClientDisconnected(false, mClient1);
    }

    /**
     * Verify that p2p init / teardown when wifi off / on
     * with a client connected
     */
    @Test
    public void checkIsP2pInitForWifiChanges() throws Exception {
        forceP2pEnabled(mClient1);

        simulateWifiStateChange(false);
        mLooper.dispatchAll();
        verify(mWifiNative).teardownInterface();
        verify(mWifiMonitor).stopMonitoring(anyString());
        // Force to back disable state for next test
        mockEnterDisabledState();

        simulateWifiStateChange(true);
        mLooper.dispatchAll();
        verify(mWifiNative, times(2)).setupInterface(any(), any());
        verify(mNwService, times(2)).setInterfaceUp(anyString());
        verify(mWifiMonitor, atLeastOnce()).registerHandler(anyString(), anyInt(), any());
    }

    /**
     * Verify p2p init / teardown when two clients connect / disconnect
     */
    @Test
    public void checkIsP2pInitForTwoClientsConnection() throws Exception {
        forceP2pEnabled(mClient1);
        // P2pInit check count should keep in once, same as one client connected case.
        checkIsP2pInitWhenClientConnected(true, mClient2);
        checkIsP2pTearDownWhenClientDisconnected(false, mClient2);
        checkIsP2pTearDownWhenClientDisconnected(true, mClient1);
    }

    /**
     * Verify WifiP2pManager.ADD_LOCAL_SERVICE_FAILED is returned when a caller
     * uses abnormal way to send WifiP2pManager.ADD_LOCAL_SERVICE (i.e no channel info updated).
     */
    @Test
    public void testAddLocalServiceFailureWhenNoChannelUpdated() throws Exception {
        forceP2pEnabled(mClient1);
        sendAddLocalServiceMsg(mClientMessenger);
        verify(mWifiNative, never()).p2pServiceAdd(any());
        assertTrue(mClientHandler.hasMessages(WifiP2pManager.ADD_LOCAL_SERVICE_FAILED));
    }

    /**
     * Verify WifiP2pManager.ADD_LOCAL_SERVICE_FAILED is returned when a caller
     * uses wrong package name to initialize a channel.
     */
    @Test
    public void testAddLocalServiceFailureWhenChannelUpdateWrongPkgName() throws Exception {
        forceP2pEnabled(mClient1);
        doThrow(new SecurityException("P2p unit test"))
                .when(mWifiPermissionsUtil).checkPackage(anyInt(), anyString());
        sendChannelInfoUpdateMsg("testPkg1", mClient1, mClientMessenger);
        sendAddLocalServiceMsg(mClientMessenger);
        assertTrue(mClientHandler.hasMessages(WifiP2pManager.ADD_LOCAL_SERVICE_FAILED));
        verify(mWifiNative, never()).p2pServiceAdd(any());
    }

    /**
     * Verify WifiP2pManager.ADD_LOCAL_SERVICE_FAILED is returned when a caller
     * without proper permission attmepts to send WifiP2pManager.ADD_LOCAL_SERVICE.
     */
    @Test
    public void testAddLocalServiceFailureWhenCallerPermissionDenied() throws Exception {
        forceP2pEnabled(mClient1);
        doNothing().when(mWifiPermissionsUtil).checkPackage(anyInt(), anyString());
        when(mWifiPermissionsUtil.checkCanAccessWifiDirect(
                anyString(), anyInt(), anyBoolean())).thenReturn(false);
        sendChannelInfoUpdateMsg("testPkg1", mClient1, mClientMessenger);
        sendAddLocalServiceMsg(mClientMessenger);
        assertTrue(mClientHandler.hasMessages(WifiP2pManager.ADD_LOCAL_SERVICE_FAILED));
        verify(mWifiNative, never()).p2pServiceAdd(any());
        verify(mWifiPermissionsUtil)
                .checkCanAccessWifiDirect(eq("testPkg1"), anyInt(), eq(false));
    }

    /**
     * Verify the caller with proper permission sends WifiP2pManager.ADD_LOCAL_SERVICE.
     */
    @Test
    public void testAddLocalServiceSuccess() throws Exception {
        forceP2pEnabled(mClient1);
        sendChannelInfoUpdateMsg("testPkg1", mClient1, mClientMessenger);
        doNothing().when(mWifiPermissionsUtil).checkPackage(anyInt(), anyString());
        when(mWifiPermissionsUtil.checkCanAccessWifiDirect(anyString(), anyInt(), anyBoolean()))
                .thenReturn(true);
        when(mWifiNative.p2pServiceAdd(any())).thenReturn(true);
        sendAddLocalServiceMsg(mClientMessenger);
        verify(mWifiNative).p2pServiceAdd(any());
        verify(mWifiPermissionsUtil)
                .checkCanAccessWifiDirect(eq("testPkg1"), anyInt(), eq(false));
        assertTrue(mClientHandler.hasMessages(WifiP2pManager.ADD_LOCAL_SERVICE_SUCCEEDED));
    }

    /**
     * Verify WifiP2pManager.ADD_LOCAL_SERVICE_FAILED is returned when native call failure.
     */
    @Test
    public void testAddLocalServiceFailureWhenNativeCallFailure() throws Exception {
        forceP2pEnabled(mClient1);
        sendChannelInfoUpdateMsg("testPkg1", mClient1, mClientMessenger);
        doNothing().when(mWifiPermissionsUtil).checkPackage(anyInt(), anyString());
        when(mWifiPermissionsUtil.checkCanAccessWifiDirect(anyString(), anyInt(), anyBoolean()))
                .thenReturn(true);
        when(mWifiNative.p2pServiceAdd(any())).thenReturn(false);
        sendAddLocalServiceMsg(mClientMessenger);
        verify(mWifiNative).p2pServiceAdd(any());
        verify(mWifiPermissionsUtil)
                .checkCanAccessWifiDirect(eq("testPkg1"), anyInt(), eq(false));
        assertTrue(mClientHandler.hasMessages(WifiP2pManager.ADD_LOCAL_SERVICE_FAILED));
    }

    /**
     * Verify WifiP2pManager.CONNECT_FAILED is returned when a caller
     * uses abnormal way to send WifiP2pManager.CONNECT (i.e no channel info updated).
     */
    @Test
    public void testConnectWithConfigValidAsGroupFailureWhenNoChannelUpdated() throws Exception {
        forceP2pEnabled(mClient1);
        sendConnectMsgWithConfigValidAsGroup(mClientMessenger);
        assertTrue(mClientHandler.hasMessages(WifiP2pManager.CONNECT_FAILED));
        verify(mWifiNative, never()).p2pGroupAdd(any(), anyBoolean());
    }

    /**
     * Verify WifiP2pManager.CONNECT_FAILED is returned when a caller
     * uses wrong package name to initialize a channel.
     */
    @Test
    public void testConnectWithConfigValidAsGroupFailureWhenChannelUpdateWrongPkgName()
            throws Exception {
        forceP2pEnabled(mClient1);
        doThrow(new SecurityException("P2p unit test"))
                .when(mWifiPermissionsUtil).checkPackage(anyInt(), anyString());
        sendChannelInfoUpdateMsg("testPkg1", mClient1, mClientMessenger);
        sendConnectMsgWithConfigValidAsGroup(mClientMessenger);
        assertTrue(mClientHandler.hasMessages(WifiP2pManager.CONNECT_FAILED));
        verify(mWifiNative, never()).p2pGroupAdd(any(), anyBoolean());
    }

    /**
     * Verify WifiP2pManager.CONNECT_FAILED is returned when a caller
     * without proper permission attmepts to send WifiP2pManager.CONNECT.
     */
    @Test
    public void testConnectWithConfigValidAsGroupFailureWhenPermissionDenied() throws Exception {
        forceP2pEnabled(mClient1);
        doNothing().when(mWifiPermissionsUtil).checkPackage(anyInt(), anyString());
        when(mWifiPermissionsUtil.checkCanAccessWifiDirect(anyString(), anyInt(), anyBoolean()))
                .thenReturn(false);
        sendChannelInfoUpdateMsg("testPkg1", mClient1, mClientMessenger);
        sendConnectMsgWithConfigValidAsGroup(mClientMessenger);
        assertTrue(mClientHandler.hasMessages(WifiP2pManager.CONNECT_FAILED));
        verify(mWifiNative, never()).p2pGroupAdd(any(), anyBoolean());
        verify(mWifiPermissionsUtil)
                .checkCanAccessWifiDirect(eq("testPkg1"), anyInt(), eq(false));
    }

    /**
     * Verify the caller with proper permission sends WifiP2pManager.CONNECT.
     */
    @Test
    public void testConnectWithConfigValidAsGroupSuccess() throws Exception {
        forceP2pEnabled(mClient1);
        when(mWifiPermissionsUtil.checkCanAccessWifiDirect(anyString(), anyInt(), anyBoolean()))
                .thenReturn(true);
        sendChannelInfoUpdateMsg("testPkg1", mClient1, mClientMessenger);
        when(mWifiNative.p2pGroupAdd(any(), eq(true))).thenReturn(true);
        sendConnectMsgWithConfigValidAsGroup(mClientMessenger);
        verify(mWifiNative).p2pGroupAdd(any(), eq(true));
        verify(mWifiPermissionsUtil)
                .checkCanAccessWifiDirect(eq("testPkg1"), anyInt(), eq(false));
        assertTrue(mClientHandler.hasMessages(WifiP2pManager.CONNECT_SUCCEEDED));
    }

    /**
     * Verify WifiP2pManager.CONNECT_FAILED is returned when native call failure.
     */
    @Test
    public void testConnectWithConfigValidAsGroupFailureWhenNativeCallFailure() throws Exception {
        forceP2pEnabled(mClient1);
        when(mWifiPermissionsUtil.checkCanAccessWifiDirect(anyString(), anyInt(), anyBoolean()))
                .thenReturn(true);
        sendChannelInfoUpdateMsg("testPkg1", mClient1, mClientMessenger);
        when(mWifiNative.p2pGroupAdd(any(), eq(true))).thenReturn(false);
        sendConnectMsgWithConfigValidAsGroup(mClientMessenger);
        verify(mWifiNative).p2pGroupAdd(any(), eq(true));
        verify(mWifiPermissionsUtil)
                .checkCanAccessWifiDirect(eq("testPkg1"), anyInt(), eq(false));
        assertTrue(mClientHandler.hasMessages(WifiP2pManager.CONNECT_FAILED));
    }

    /**
     * Verify WifiP2pManager.CREATE_GROUP_FAILED is returned when a caller
     * uses abnormal way to send WifiP2pManager.CREATE_GROUP (i.e no channel info updated).
     */
    @Test
    public void testCreateGroupWithConfigValidAsGroupFailureWhenNoChannelUpdated()
            throws Exception {
        forceP2pEnabled(mClient1);
        sendCreateGroupMsgWithConfigValidAsGroup(mClientMessenger);
        assertTrue(mClientHandler.hasMessages(WifiP2pManager.CREATE_GROUP_FAILED));
        verify(mWifiNative, never()).p2pGroupAdd(anyBoolean());
        verify(mWifiNative, never()).p2pGroupAdd(any(), anyBoolean());
    }

    /**
     * Verify WifiP2pManager.CREATE_GROUP_FAILED is returned with null object when a caller
     * uses wrong package name to initialize a channel.
     */
    @Test
    public void testCreateGroupWithConfigValidAsGroupFailureWhenChannelUpdateWrongPkgName()
            throws Exception {
        forceP2pEnabled(mClient1);
        doThrow(new SecurityException("P2p unit test"))
                .when(mWifiPermissionsUtil).checkPackage(anyInt(), anyString());
        sendChannelInfoUpdateMsg("testPkg1", mClient1, mClientMessenger);
        sendCreateGroupMsgWithConfigValidAsGroup(mClientMessenger);
        assertTrue(mClientHandler.hasMessages(WifiP2pManager.CREATE_GROUP_FAILED));
        verify(mWifiNative, never()).p2pGroupAdd(anyBoolean());
        verify(mWifiNative, never()).p2pGroupAdd(any(), anyBoolean());
    }

    /**
     * Verify WifiP2pManager.CREATE_GROUP_FAILED is returned when a caller
     * without proper permission attmepts to send WifiP2pManager.CREATE_GROUP.
     */
    @Test
    public void testCreateGroupWithConfigValidAsGroupFailureWhenPermissionDenied()
            throws Exception {
        forceP2pEnabled(mClient1);
        doNothing().when(mWifiPermissionsUtil).checkPackage(anyInt(), anyString());
        when(mWifiPermissionsUtil.checkCanAccessWifiDirect(anyString(), anyInt(), anyBoolean()))
                .thenReturn(false);
        sendChannelInfoUpdateMsg("testPkg1", mClient1, mClientMessenger);
        sendCreateGroupMsgWithConfigValidAsGroup(mClientMessenger);
        assertTrue(mClientHandler.hasMessages(WifiP2pManager.CREATE_GROUP_FAILED));
        verify(mWifiNative, never()).p2pGroupAdd(anyBoolean());
        verify(mWifiNative, never()).p2pGroupAdd(any(), anyBoolean());
        verify(mWifiPermissionsUtil)
                .checkCanAccessWifiDirect(eq("testPkg1"), anyInt(), eq(false));
    }

    /**
     * Verify the caller with proper permission sends WifiP2pManager.CREATE_GROUP.
     */
    @Test
    public void testCreateGroupWithConfigValidAsGroupSuccess() throws Exception {
        forceP2pEnabled(mClient1);
        when(mWifiPermissionsUtil.checkCanAccessWifiDirect(anyString(), anyInt(), anyBoolean()))
                .thenReturn(true);
        sendChannelInfoUpdateMsg("testPkg1", mClient1, mClientMessenger);
        when(mWifiNative.p2pGroupAdd(any(), eq(false))).thenReturn(true);
        sendCreateGroupMsgWithConfigValidAsGroup(mClientMessenger);
        verify(mWifiNative).p2pGroupAdd(any(), eq(false));
        verify(mWifiPermissionsUtil)
                .checkCanAccessWifiDirect(eq("testPkg1"), anyInt(), eq(false));
        assertTrue(mClientHandler.hasMessages(WifiP2pManager.CREATE_GROUP_SUCCEEDED));
    }

    /**
     * Verify WifiP2pManager.CREATE_GROUP_FAILED is returned when native call failure.
     */
    @Test
    public void testCreateGroupWithConfigValidAsGroupFailureWhenNativeCallFailure()
            throws Exception {
        forceP2pEnabled(mClient1);
        when(mWifiPermissionsUtil.checkCanAccessWifiDirect(anyString(), anyInt(), anyBoolean()))
                .thenReturn(true);
        sendChannelInfoUpdateMsg("testPkg1", mClient1, mClientMessenger);
        when(mWifiNative.p2pGroupAdd(any(), eq(false))).thenReturn(false);
        sendCreateGroupMsgWithConfigValidAsGroup(mClientMessenger);
        verify(mWifiNative).p2pGroupAdd(any(), eq(false));
        verify(mWifiPermissionsUtil)
                .checkCanAccessWifiDirect(eq("testPkg1"), anyInt(), eq(false));
        assertTrue(mClientHandler.hasMessages(WifiP2pManager.CREATE_GROUP_FAILED));
    }

    /**
     * Verify WifiP2pManager.DISCOVER_PEERS_FAILED is returned when a caller
     * uses abnormal way to send WifiP2pManager.REQUEST_PEERS (i.e no channel info updated).
     */
    @Test
    public void testDiscoverPeersFailureWhenNoChannelUpdated() throws Exception {
        forceP2pEnabled(mClient1);
        sendDiscoverPeersMsg(mClientMessenger);
        verify(mWifiNative, never()).p2pFind(anyInt());
        assertTrue(mClientHandler.hasMessages(WifiP2pManager.DISCOVER_PEERS_FAILED));
    }

    /**
     * Verify WifiP2pManager.DISCOVER_PEERS_FAILED is returned when a caller
     * uses wrong package name to initialize a channel.
     */
    @Test
    public void testDiscoverPeersFailureWhenChannelUpdateWrongPkgName() throws Exception {
        forceP2pEnabled(mClient1);
        doThrow(new SecurityException("P2p unit test"))
                .when(mWifiPermissionsUtil).checkPackage(anyInt(), anyString());
        sendChannelInfoUpdateMsg("testPkg1", mClient1, mClientMessenger);
        sendDiscoverPeersMsg(mClientMessenger);
        verify(mWifiNative, never()).p2pFind(anyInt());
        assertTrue(mClientHandler.hasMessages(WifiP2pManager.DISCOVER_PEERS_FAILED));
    }

    /**
     * Verify WifiP2pManager.DISCOVER_PEERS_FAILED is returned with null object when a caller
     * without proper permission attmepts to send WifiP2pManager.DISCOVER_PEERS.
     */
    @Test
    public void testDiscoverPeersFailureWhenPermissionDenied() throws Exception {
        forceP2pEnabled(mClient1);
        doNothing().when(mWifiPermissionsUtil).checkPackage(anyInt(), anyString());
        when(mWifiPermissionsUtil.checkCanAccessWifiDirect(anyString(), anyInt(), anyBoolean()))
                .thenReturn(false);
        sendChannelInfoUpdateMsg("testPkg1", mClient1, mClientMessenger);
        sendDiscoverPeersMsg(mClientMessenger);
        verify(mWifiNative, never()).p2pFind(anyInt());
        verify(mWifiPermissionsUtil)
                .checkCanAccessWifiDirect(eq("testPkg1"), anyInt(), eq(true));
        assertTrue(mClientHandler.hasMessages(WifiP2pManager.DISCOVER_PEERS_FAILED));
    }

    /**
     * Verify WifiP2pManager.DISCOVER_PEERS_FAILED is returned with null object when a caller
     * attmepts to send WifiP2pManager.DISCOVER_PEERS and location mode is disabled.
     */
    @Test
    public void testDiscoverPeersFailureWhenLocationModeDisabled() throws Exception {
        forceP2pEnabled(mClient1);
        doNothing().when(mWifiPermissionsUtil).checkPackage(anyInt(), anyString());
        when(mWifiPermissionsUtil.checkCanAccessWifiDirect(anyString(), anyInt(), eq(false)))
                .thenReturn(true);
        when(mWifiPermissionsUtil.checkCanAccessWifiDirect(anyString(), anyInt(), eq(true)))
                .thenReturn(false);
        sendChannelInfoUpdateMsg("testPkg1", mClient1, mClientMessenger);
        sendDiscoverPeersMsg(mClientMessenger);
        verify(mWifiNative, never()).p2pFind(anyInt());
        verify(mWifiPermissionsUtil)
                .checkCanAccessWifiDirect(eq("testPkg1"), anyInt(), eq(true));
        assertTrue(mClientHandler.hasMessages(WifiP2pManager.DISCOVER_PEERS_FAILED));
    }

    /**
     * Verify the caller with proper permission sends WifiP2pManager.DISCOVER_PEERS.
     */
    @Test
    public void testDiscoverPeersSuccess() throws Exception {
        forceP2pEnabled(mClient1);
        when(mWifiPermissionsUtil.checkCanAccessWifiDirect(anyString(), anyInt(), anyBoolean()))
                .thenReturn(true);
        when(mWifiNative.p2pFind(anyInt())).thenReturn(true);
        sendChannelInfoUpdateMsg("testPkg1", mClient1, mClientMessenger);
        sendDiscoverPeersMsg(mClientMessenger);
        verify(mWifiNative).p2pFind(anyInt());
        verify(mWifiPermissionsUtil)
                .checkCanAccessWifiDirect(eq("testPkg1"), anyInt(), eq(true));
        assertTrue(mClientHandler.hasMessages(WifiP2pManager.DISCOVER_PEERS_SUCCEEDED));
    }

    /**
     * Verify WifiP2pManager.DISCOVER_PEERS_FAILED is returned when native call failure.
     */
    @Test
    public void testDiscoverPeersFailureWhenNativeCallFailure() throws Exception {
        forceP2pEnabled(mClient1);
        when(mWifiPermissionsUtil.checkCanAccessWifiDirect(anyString(), anyInt(), anyBoolean()))
                .thenReturn(true);
        when(mWifiNative.p2pFind(anyInt())).thenReturn(false);
        sendChannelInfoUpdateMsg("testPkg1", mClient1, mClientMessenger);
        sendDiscoverPeersMsg(mClientMessenger);
        verify(mWifiNative).p2pFind(anyInt());
        verify(mWifiPermissionsUtil)
                .checkCanAccessWifiDirect(eq("testPkg1"), anyInt(), eq(true));
        assertTrue(mClientHandler.hasMessages(WifiP2pManager.DISCOVER_PEERS_FAILED));
    }


    /**
     * Verify WifiP2pManager.DISCOVER_SERVICES_FAILED is returned when a caller
     * uses abnormal way to send WifiP2pManager.DISCOVER_SERVICES (i.e no channel info updated).
     */
    @Test
    public void testDiscoverServicesFailureWhenNoChannelUpdated() throws Exception {
        when(mWifiNative.p2pServDiscReq(anyString(), anyString())).thenReturn("mServiceDiscReqId");
        forceP2pEnabled(mClient1);
        sendAddServiceRequestMsg(mClientMessenger);
        sendDiscoverServiceMsg(mClientMessenger);
        verify(mWifiNative, never()).p2pServDiscReq(anyString(), anyString());
        verify(mWifiNative, never()).p2pFind(anyInt());
        assertTrue(mClientHandler.hasMessages(WifiP2pManager.DISCOVER_SERVICES_FAILED));
    }

    /**
     * Verify WifiP2pManager.DISCOVER_SERVICES_FAILED is returned when a caller
     * uses wrong package name to initialize a channel.
     */
    @Test
    public void testDiscoverServicesFailureWhenChannelUpdateWrongPkgName() throws Exception {
        when(mWifiNative.p2pServDiscReq(anyString(), anyString())).thenReturn("mServiceDiscReqId");
        forceP2pEnabled(mClient1);
        doThrow(new SecurityException("P2p unit test"))
                .when(mWifiPermissionsUtil).checkPackage(anyInt(), anyString());
        sendChannelInfoUpdateMsg("testPkg1", mClient1, mClientMessenger);
        sendAddServiceRequestMsg(mClientMessenger);
        sendDiscoverServiceMsg(mClientMessenger);
        verify(mWifiNative, never()).p2pServDiscReq(anyString(), anyString());
        verify(mWifiNative, never()).p2pFind(anyInt());
        assertTrue(mClientHandler.hasMessages(WifiP2pManager.DISCOVER_SERVICES_FAILED));
    }

    /**
     * Verify WifiP2pManager.DISCOVER_SERVICES_FAILED is returned when a caller
     * without proper permission attmepts to send WifiP2pManager.DISCOVER_SERVICES.
     */
    @Test
    public void testDiscoverServicesFailureWhenPermissionDenied() throws Exception {
        when(mWifiNative.p2pServDiscReq(anyString(), anyString()))
                .thenReturn("mServiceDiscReqId");
        forceP2pEnabled(mClient1);
        doNothing().when(mWifiPermissionsUtil).checkPackage(anyInt(), anyString());
        when(mWifiPermissionsUtil.checkCanAccessWifiDirect(anyString(), anyInt(), anyBoolean()))
                .thenReturn(false);
        sendChannelInfoUpdateMsg("testPkg1", mClient1, mClientMessenger);
        sendAddServiceRequestMsg(mClientMessenger);
        sendDiscoverServiceMsg(mClientMessenger);
        verify(mWifiNative, never()).p2pServDiscReq(anyString(), anyString());
        verify(mWifiNative, never()).p2pFind(anyInt());
        verify(mWifiPermissionsUtil)
                .checkCanAccessWifiDirect(eq("testPkg1"), anyInt(), eq(true));
        assertTrue(mClientHandler.hasMessages(WifiP2pManager.DISCOVER_SERVICES_FAILED));
    }

    /**
     * Verify WifiP2pManager.DISCOVER_SERVICES_FAILED is returned when a caller
     * attmepts to send WifiP2pManager.DISCOVER_SERVICES and location mode is disabled.
     */
    @Test
    public void testDiscoverServicesFailureWhenLocationModeDisabled() throws Exception {
        when(mWifiNative.p2pServDiscReq(anyString(), anyString()))
                .thenReturn("mServiceDiscReqId");
        forceP2pEnabled(mClient1);
        doNothing().when(mWifiPermissionsUtil).checkPackage(anyInt(), anyString());
        when(mWifiPermissionsUtil.checkCanAccessWifiDirect(anyString(), anyInt(), eq(false)))
                .thenReturn(true);
        when(mWifiPermissionsUtil.checkCanAccessWifiDirect(anyString(), anyInt(), eq(true)))
                .thenReturn(false);
        sendChannelInfoUpdateMsg("testPkg1", mClient1, mClientMessenger);
        sendAddServiceRequestMsg(mClientMessenger);
        sendDiscoverServiceMsg(mClientMessenger);
        verify(mWifiNative, never()).p2pServDiscReq(anyString(), anyString());
        verify(mWifiNative, never()).p2pFind(anyInt());
        verify(mWifiPermissionsUtil)
                .checkCanAccessWifiDirect(eq("testPkg1"), anyInt(), eq(true));
        assertTrue(mClientHandler.hasMessages(WifiP2pManager.DISCOVER_SERVICES_FAILED));
    }

    /**
     * Verify the caller with proper permission sends WifiP2pManager.DISCOVER_SERVICES.
     */
    @Test
    public void testDiscoverServicesSuccess() throws Exception {
        when(mWifiNative.p2pServDiscReq(anyString(), anyString()))
                .thenReturn("mServiceDiscReqId");
        when(mWifiNative.p2pFind(anyInt())).thenReturn(true);
        forceP2pEnabled(mClient1);
        when(mWifiPermissionsUtil.checkCanAccessWifiDirect(anyString(), anyInt(), anyBoolean()))
                .thenReturn(true);
        sendChannelInfoUpdateMsg("testPkg1", mClient1, mClientMessenger);
        sendAddServiceRequestMsg(mClientMessenger);
        sendDiscoverServiceMsg(mClientMessenger);
        verify(mWifiNative).p2pServDiscReq(anyString(), anyString());
        verify(mWifiNative).p2pFind(anyInt());
        verify(mWifiPermissionsUtil)
                .checkCanAccessWifiDirect(eq("testPkg1"), anyInt(), eq(true));
        assertTrue(mClientHandler.hasMessages(WifiP2pManager.DISCOVER_SERVICES_SUCCEEDED));
    }

    /**
     * Verify WifiP2pManager.DISCOVER_SERVICES_FAILED is returned when add service failure.
     */
    @Test
    public void testDiscoverServicesFailureWhenAddServiceRequestFailure() throws Exception {
        when(mWifiNative.p2pServDiscReq(anyString(), anyString())).thenReturn(null);
        forceP2pEnabled(mClient1);
        when(mWifiPermissionsUtil.checkCanAccessWifiDirect(anyString(), anyInt(), anyBoolean()))
                .thenReturn(true);
        sendChannelInfoUpdateMsg("testPkg1", mClient1, mClientMessenger);
        sendAddServiceRequestMsg(mClientMessenger);
        sendDiscoverServiceMsg(mClientMessenger);
        verify(mWifiNative).p2pServDiscReq(anyString(), anyString());
        verify(mWifiNative, never()).p2pFind(anyInt());
        verify(mWifiPermissionsUtil)
                .checkCanAccessWifiDirect(eq("testPkg1"), anyInt(), eq(true));
        assertTrue(mClientHandler.hasMessages(WifiP2pManager.DISCOVER_SERVICES_FAILED));
    }

    /**
     * Verify WifiP2pManager.DISCOVER_SERVICES_FAILED is returned when native call failure.
     */
    @Test
    public void testDiscoverServicesFailureWhenNativeCallFailure() throws Exception {
        when(mWifiNative.p2pServDiscReq(anyString(), anyString()))
                .thenReturn("mServiceDiscReqId");
        when(mWifiNative.p2pFind(anyInt())).thenReturn(false);
        forceP2pEnabled(mClient1);
        when(mWifiPermissionsUtil.checkCanAccessWifiDirect(anyString(), anyInt(), anyBoolean()))
                .thenReturn(true);
        sendChannelInfoUpdateMsg("testPkg1", mClient1, mClientMessenger);
        sendAddServiceRequestMsg(mClientMessenger);
        sendDiscoverServiceMsg(mClientMessenger);
        verify(mWifiNative).p2pServDiscReq(anyString(), anyString());
        verify(mWifiNative).p2pFind(anyInt());
        verify(mWifiPermissionsUtil)
                .checkCanAccessWifiDirect(eq("testPkg1"), anyInt(), eq(true));
        assertTrue(mClientHandler.hasMessages(WifiP2pManager.DISCOVER_SERVICES_FAILED));
    }

    /**
     * Verify WifiP2pManager.RESPONSE_PEERS is returned with null object when a caller
     * uses abnormal way to send WifiP2pManager.REQUEST_PEERS (i.e no channel info updated).
     */
    @Test
    public void testRequestPeersFailureWhenNoChannelUpdated() throws Exception {
        forceP2pEnabled(mClient1);
        mockPeersList();
        sendRequestPeersMsg(mClientMessenger);
        verify(mClientHandler).sendMessage(mMessageCaptor.capture());
        WifiP2pDeviceList peers = (WifiP2pDeviceList) mMessageCaptor.getValue().obj;
        assertEquals(WifiP2pManager.RESPONSE_PEERS, mMessageCaptor.getValue().what);
        assertNull(peers.get(mTestWifiP2pDevice.deviceAddress));

    }

    /**
     * Verify WifiP2pManager.RESPONSE_PEERS is returned with null object when a caller
     * uses wrong package name to initialize a channel.
     */
    @Test
    public void testRequestPeersFailureWhenChannelUpdateWrongPkgName() throws Exception {
        forceP2pEnabled(mClient1);
        mockPeersList();
        doThrow(new SecurityException("P2p unit test"))
                .when(mWifiPermissionsUtil).checkPackage(anyInt(), anyString());
        sendChannelInfoUpdateMsg("testPkg1", mClient1, mClientMessenger);
        sendRequestPeersMsg(mClientMessenger);
        verify(mClientHandler).sendMessage(mMessageCaptor.capture());
        WifiP2pDeviceList peers = (WifiP2pDeviceList) mMessageCaptor.getValue().obj;
        assertEquals(WifiP2pManager.RESPONSE_PEERS, mMessageCaptor.getValue().what);
        assertNull(peers.get(mTestWifiP2pDevice.deviceAddress));
    }

    /**
     * Verify WifiP2pManager.RESPONSE_PEERS is returned with null object when a caller
     * without proper permission attmepts to send WifiP2pManager.REQUEST_PEERS.
     */
    @Test
    public void testRequestPeersFailureWhenPermissionDenied() throws Exception {
        forceP2pEnabled(mClient1);
        mockPeersList();
        doNothing().when(mWifiPermissionsUtil).checkPackage(anyInt(), anyString());
        when(mWifiPermissionsUtil.checkCanAccessWifiDirect(anyString(), anyInt(), anyBoolean()))
                .thenReturn(false);
        sendChannelInfoUpdateMsg("testPkg1", mClient1, mClientMessenger);
        sendRequestPeersMsg(mClientMessenger);
        verify(mClientHandler).sendMessage(mMessageCaptor.capture());
        verify(mWifiPermissionsUtil)
                .checkCanAccessWifiDirect(eq("testPkg1"), anyInt(), eq(true));
        WifiP2pDeviceList peers = (WifiP2pDeviceList) mMessageCaptor.getValue().obj;
        assertEquals(WifiP2pManager.RESPONSE_PEERS, mMessageCaptor.getValue().what);
        assertNull(peers.get(mTestWifiP2pDevice.deviceAddress));

    }

    /**
     * Verify WifiP2pManager.RESPONSE_PEERS is returned with null object when a caller
     * attmepts to send WifiP2pManager.REQUEST_PEERS and location mode is disabled.
     */
    @Test
    public void testRequestPeersFailureWhenLocationModeDisabled() throws Exception {
        forceP2pEnabled(mClient1);
        mockPeersList();
        doNothing().when(mWifiPermissionsUtil).checkPackage(anyInt(), anyString());
        when(mWifiPermissionsUtil.checkCanAccessWifiDirect(anyString(), anyInt(), eq(false)))
                .thenReturn(true);
        when(mWifiPermissionsUtil.checkCanAccessWifiDirect(anyString(), anyInt(), eq(true)))
                .thenReturn(false);
        sendChannelInfoUpdateMsg("testPkg1", mClient1, mClientMessenger);
        sendRequestPeersMsg(mClientMessenger);
        verify(mClientHandler).sendMessage(mMessageCaptor.capture());
        verify(mWifiPermissionsUtil)
                .checkCanAccessWifiDirect(eq("testPkg1"), anyInt(), eq(true));
        WifiP2pDeviceList peers = (WifiP2pDeviceList) mMessageCaptor.getValue().obj;
        assertEquals(WifiP2pManager.RESPONSE_PEERS, mMessageCaptor.getValue().what);
        assertNull(peers.get(mTestWifiP2pDevice.deviceAddress));

    }

    /**
     * Verify WifiP2pManager.RESPONSE_PEERS is returned with expect object when a caller
     * with proper permission to send WifiP2pManager.REQUEST_PEERS.
     */
    @Test
    public void testRequestPeersSuccess() throws Exception {
        forceP2pEnabled(mClient1);
        mockPeersList();
        when(mWifiPermissionsUtil.checkCanAccessWifiDirect(anyString(), anyInt(), anyBoolean()))
                .thenReturn(true);
        sendChannelInfoUpdateMsg("testPkg1", mClient1, mClientMessenger);
        sendRequestPeersMsg(mClientMessenger);
        verify(mClientHandler).sendMessage(mMessageCaptor.capture());
        verify(mWifiPermissionsUtil)
                .checkCanAccessWifiDirect(eq("testPkg1"), anyInt(), eq(true));
        WifiP2pDeviceList peers = (WifiP2pDeviceList) mMessageCaptor.getValue().obj;
        assertEquals(WifiP2pManager.RESPONSE_PEERS, mMessageCaptor.getValue().what);
        assertNotEquals(null, peers.get(mTestWifiP2pDevice.deviceAddress));
    }

    /**
     * Verify WifiP2pManager.RESPONSE_GROUP_INFO is returned with null object when a caller
     * uses abnormal way to send WifiP2pManager.REQUEST_GROUP_INFO (i.e no channel info updated).
     */
    @Test
    public void testRequestGroupInfoFailureWhenNoChannelUpdated() throws Exception {
        forceP2pEnabled(mClient1);
        sendGroupStartedMsg(mTestWifiP2pGroup);
        sendRequestGroupInfoMsg(mClientMessenger);
        verify(mClientHandler).sendMessage(mMessageCaptor.capture());
        assertEquals(WifiP2pManager.RESPONSE_GROUP_INFO, mMessageCaptor.getValue().what);
        assertNull(mMessageCaptor.getValue().obj);
    }

    /**
     * Verify WifiP2pManager.RESPONSE_GROUP_INFO is returned with null object when a caller
     * uses wrong package name to initialize a channel.
     */
    @Test
    public void testRequestGroupInfoFailureWhenChannelUpdateWrongPkgName() throws Exception {
        forceP2pEnabled(mClient1);
        sendGroupStartedMsg(mTestWifiP2pGroup);
        doThrow(new SecurityException("P2p unit test"))
                .when(mWifiPermissionsUtil).checkPackage(anyInt(), anyString());
        sendChannelInfoUpdateMsg("testPkg1", mClient1, mClientMessenger);
        sendRequestGroupInfoMsg(mClientMessenger);
        verify(mClientHandler).sendMessage(mMessageCaptor.capture());
        assertEquals(WifiP2pManager.RESPONSE_GROUP_INFO, mMessageCaptor.getValue().what);
        assertNull(mMessageCaptor.getValue().obj);
    }

    /**
     * Verify WifiP2pManager.RESPONSE_GROUP_INFO is returned with null object when a caller
     * without proper permission attempts to send WifiP2pManager.REQUEST_GROUP_INFO.
     */
    @Test
    public void testRequestGroupInfoFailureWhenPermissionDenied() throws Exception {
        forceP2pEnabled(mClient1);
        sendGroupStartedMsg(mTestWifiP2pGroup);
        doNothing().when(mWifiPermissionsUtil).checkPackage(anyInt(), anyString());
        when(mWifiPermissionsUtil.checkCanAccessWifiDirect(anyString(), anyInt(), anyBoolean()))
                .thenReturn(false);
        sendChannelInfoUpdateMsg("testPkg1", mClient1, mClientMessenger);
        sendRequestGroupInfoMsg(mClientMessenger);
        verify(mClientHandler).sendMessage(mMessageCaptor.capture());
        verify(mWifiPermissionsUtil)
                .checkCanAccessWifiDirect(eq("testPkg1"), anyInt(), eq(false));
        assertEquals(WifiP2pManager.RESPONSE_GROUP_INFO, mMessageCaptor.getValue().what);
        assertNull(mMessageCaptor.getValue().obj);
    }

    /**
     * Verify WifiP2pManager.RESPONSE_GROUP_INFO is returned with expect object when a caller
     * with proper permission.
     */
    @Test
    public void testRequestGroupInfoSuccess() throws Exception {
        mTestWifiP2pGroup.setOwner(mTestThisDevice);
        forceP2pEnabled(mClient1);
        sendGroupStartedMsg(mTestWifiP2pGroup);
        when(mWifiPermissionsUtil.checkLocalMacAddressPermission(anyInt())).thenReturn(false);
        when(mWifiPermissionsUtil.checkCanAccessWifiDirect(anyString(), anyInt(), anyBoolean()))
                .thenReturn(true);
        sendChannelInfoUpdateMsg("testPkg1", mClient1, mClientMessenger);
        sendRequestGroupInfoMsg(mClientMessenger);
        verify(mClientHandler).sendMessage(mMessageCaptor.capture());
        verify(mWifiPermissionsUtil)
                .checkCanAccessWifiDirect(eq("testPkg1"), anyInt(), eq(false));
        assertEquals(WifiP2pManager.RESPONSE_GROUP_INFO, mMessageCaptor.getValue().what);
        WifiP2pGroup wifiP2pGroup = (WifiP2pGroup) mMessageCaptor.getValue().obj;
        assertEquals(mTestWifiP2pGroup.getNetworkName(), wifiP2pGroup.getNetworkName());
        // Ensure that our own MAC address is anonymized if we're the group owner.
        assertEquals(ANONYMIZED_DEVICE_ADDRESS, wifiP2pGroup.getOwner().deviceAddress);
    }

    /**
     * Verify WifiP2pManager.RESPONSE_GROUP_INFO does not anonymize this device's MAC address when
     * requested by an app with the LOCAL_MAC_ADDRESS permission.
     */
    @Test
    public void testRequestGroupInfoIncludesMacForNetworkSettingsApp() throws Exception {
        mTestWifiP2pGroup.setOwner(mTestThisDevice);
        forceP2pEnabled(mClient1);
        sendGroupStartedMsg(mTestWifiP2pGroup);
        when(mWifiPermissionsUtil.checkLocalMacAddressPermission(anyInt())).thenReturn(true);
        when(mWifiPermissionsUtil.checkCanAccessWifiDirect(anyString(), anyInt(), anyBoolean()))
                .thenReturn(true);
        sendChannelInfoUpdateMsg("testPkg1", mClient1, mClientMessenger);
        sendRequestGroupInfoMsg(mClientMessenger);
        verify(mClientHandler).sendMessage(mMessageCaptor.capture());
        verify(mWifiPermissionsUtil)
                .checkCanAccessWifiDirect(eq("testPkg1"), anyInt(), eq(false));
        assertEquals(WifiP2pManager.RESPONSE_GROUP_INFO, mMessageCaptor.getValue().what);
        WifiP2pGroup wifiP2pGroup = (WifiP2pGroup) mMessageCaptor.getValue().obj;
        assertEquals(thisDeviceMac, wifiP2pGroup.getOwner().deviceAddress);
    }

    /**
     * Verify WifiP2pManager.START_LISTEN_FAILED is returned when a caller
     * without proper permission attempts to send WifiP2pManager.START_LISTEN.
     */
    @Test
    public void testStartListenFailureWhenPermissionDenied() throws Exception {
        when(mWifiPermissionsUtil.checkNetworkSettingsPermission(anyInt())).thenReturn(false);
        forceP2pEnabled(mClient1);
        sendSimpleMsg(mClientMessenger, WifiP2pManager.START_LISTEN);
        assertTrue(mClientHandler.hasMessages(WifiP2pManager.START_LISTEN_FAILED));
        // p2pFlush should be invoked once in forceP2pEnabled.
        verify(mWifiNative).p2pFlush();
        verify(mWifiNative, never()).p2pExtListen(anyBoolean(), anyInt(), anyInt());
    }

    /**
     * Verify WifiP2pManager.START_LISTEN_FAILED is returned when native call failure.
     */
    @Test
    public void testStartListenFailureWhenNativeCallFailure() throws Exception {
        when(mWifiPermissionsUtil.checkNetworkSettingsPermission(anyInt())).thenReturn(true);
        when(mWifiNative.p2pExtListen(eq(true), anyInt(), anyInt())).thenReturn(false);
        forceP2pEnabled(mClient1);
        sendSimpleMsg(mClientMessenger, WifiP2pManager.START_LISTEN);
        // p2pFlush would be invoked in forceP2pEnabled and startListen both.
        verify(mWifiNative, times(2)).p2pFlush();
        verify(mWifiNative).p2pExtListen(eq(true), anyInt(), anyInt());
        assertTrue(mClientHandler.hasMessages(WifiP2pManager.START_LISTEN_FAILED));
    }

    /**
     * Verify the caller with proper permission sends WifiP2pManager.START_LISTEN.
     */
    @Test
    public void testStartListenSuccess() throws Exception {
        when(mWifiNative.p2pExtListen(eq(true), anyInt(), anyInt())).thenReturn(true);
        when(mWifiPermissionsUtil.checkNetworkSettingsPermission(anyInt())).thenReturn(true);
        forceP2pEnabled(mClient1);
        sendSimpleMsg(mClientMessenger, WifiP2pManager.START_LISTEN);
        // p2pFlush would be invoked in forceP2pEnabled and startListen both.
        verify(mWifiNative, times(2)).p2pFlush();
        verify(mWifiNative).p2pExtListen(eq(true), anyInt(), anyInt());
        assertTrue(mClientHandler.hasMessages(WifiP2pManager.START_LISTEN_SUCCEEDED));
    }

    /**
     * Verify WifiP2pManager.STOP_LISTEN_FAILED is returned when a caller
     * without proper permission attempts to sends WifiP2pManager.STOP_LISTEN.
     */
    @Test
    public void testStopListenFailureWhenPermissionDenied() throws Exception {
        when(mWifiPermissionsUtil.checkNetworkSettingsPermission(anyInt())).thenReturn(false);
        forceP2pEnabled(mClient1);
        sendSimpleMsg(mClientMessenger, WifiP2pManager.STOP_LISTEN);
        assertTrue(mClientHandler.hasMessages(WifiP2pManager.STOP_LISTEN_FAILED));
        verify(mWifiNative, never()).p2pExtListen(anyBoolean(), anyInt(), anyInt());
    }

    /**
     * Verify WifiP2pManager.STOP_LISTEN_FAILED is returned when native call failure.
     */
    @Test
    public void testStopListenFailureWhenNativeCallFailure() throws Exception {
        when(mWifiPermissionsUtil.checkNetworkSettingsPermission(anyInt())).thenReturn(true);
        when(mWifiNative.p2pExtListen(eq(false), anyInt(), anyInt())).thenReturn(false);
        forceP2pEnabled(mClient1);
        sendSimpleMsg(mClientMessenger, WifiP2pManager.STOP_LISTEN);
        verify(mWifiNative).p2pExtListen(eq(false), anyInt(), anyInt());
        assertTrue(mClientHandler.hasMessages(WifiP2pManager.STOP_LISTEN_FAILED));
    }

    /**
     * Verify the caller with proper permission sends WifiP2pManager.STOP_LISTEN.
     */
    @Test
    public void testStopListenSuccess() throws Exception {
        when(mWifiPermissionsUtil.checkNetworkSettingsPermission(anyInt())).thenReturn(true);
        when(mWifiNative.p2pExtListen(eq(false), anyInt(), anyInt())).thenReturn(true);
        forceP2pEnabled(mClient1);
        sendSimpleMsg(mClientMessenger, WifiP2pManager.STOP_LISTEN);
        verify(mWifiNative).p2pExtListen(eq(false), anyInt(), anyInt());
        assertTrue(mClientHandler.hasMessages(WifiP2pManager.STOP_LISTEN_SUCCEEDED));
    }

    /** Verify the p2p randomized MAC feature is enabled if OEM supports it. */
    @Test
    public void testP2pRandomMacWithOemSupport() throws Exception {
        when(mResources.getBoolean(R.bool.config_wifi_p2p_mac_randomization_supported))
                .thenReturn(true);
        forceP2pEnabled(mClient1);
        verify(mWifiNative, never()).setMacRandomization(eq(false));
        verify(mWifiNative).setMacRandomization(eq(true));
    }

    /** Verify the p2p randomized MAC feature is disabled if OEM does not support it. */
    @Test
    public void testP2pRandomMacWithoutOemSupport() throws Exception {
        when(mResources.getBoolean(R.bool.config_wifi_p2p_mac_randomization_supported))
                .thenReturn(false);
        forceP2pEnabled(mClient1);
        verify(mWifiNative, never()).setMacRandomization(eq(true));
        verify(mWifiNative).setMacRandomization(eq(false));
    }

    /**
     * Verify the caller sends WifiP2pManager.DELETE_PERSISTENT_GROUP.
     */
    @Test
    public void testDeletePersistentGroupSuccess() throws Exception {
        // Move to enabled state
        forceP2pEnabled(mClient1);

        sendDeletePersistentGroupMsg(mClientMessenger, WifiP2pGroup.PERSISTENT_NET_ID);
        verify(mClientHandler).sendMessage(mMessageCaptor.capture());
        Message message = mMessageCaptor.getValue();
        assertEquals(WifiP2pManager.DELETE_PERSISTENT_GROUP_SUCCEEDED, message.what);
    }

    /**
     * Verify that respond with DELETE_PERSISTENT_GROUP_FAILED
     * when caller sends DELETE_PERSISTENT_GROUP and p2p is disabled.
     */
    @Test
    public void testDeletePersistentGroupFailureWhenP2pDisabled() throws Exception {
        sendDeletePersistentGroupMsg(mClientMessenger, WifiP2pGroup.PERSISTENT_NET_ID);
        verify(mClientHandler).sendMessage(mMessageCaptor.capture());
        Message message = mMessageCaptor.getValue();
        assertEquals(WifiP2pManager.DELETE_PERSISTENT_GROUP_FAILED, message.what);
        assertEquals(WifiP2pManager.BUSY, message.arg1);
    }

    /**
     * Verify that respond with DELETE_PERSISTENT_GROUP_FAILED
     * when caller sends DELETE_PERSISTENT_GROUP and p2p is unsupported.
     */
    @Test
    public void testDeletePersistentGroupFailureWhenP2pUnsupported() throws Exception {
        setUpWifiP2pServiceImpl(false);
        sendDeletePersistentGroupMsg(mClientMessenger, WifiP2pGroup.PERSISTENT_NET_ID);
        verify(mClientHandler).sendMessage(mMessageCaptor.capture());
        Message message = mMessageCaptor.getValue();
        assertEquals(WifiP2pManager.DELETE_PERSISTENT_GROUP_FAILED, message.what);
        assertEquals(WifiP2pManager.P2P_UNSUPPORTED, message.arg1);
    }

    /**
     * Verify the peer scan counter is increased while receiving WifiP2pManager.DISCOVER_PEERS at
     * P2pEnabledState.
     */
    @Test
    public void testPeerScanMetricWhenSendDiscoverPeers() throws Exception {
        forceP2pEnabled(mClient1);
        when(mWifiPermissionsUtil.checkCanAccessWifiDirect(anyString(), anyInt(), anyBoolean()))
                .thenReturn(true);
        when(mWifiNative.p2pFind(anyInt())).thenReturn(true);
        sendChannelInfoUpdateMsg("testPkg1", mClient1, mClientMessenger);
        sendDiscoverPeersMsg(mClientMessenger);
        verify(mWifiP2pMetrics).incrementPeerScans();
        verify(mWifiPermissionsUtil)
                .checkCanAccessWifiDirect(eq("testPkg1"), anyInt(), eq(true));
    }

    /**
     * Verify the service scan counter is increased while receiving
     * WifiP2pManager.DISCOVER_SERVICES at P2pEnabledState.
     */
    @Test
    public void testServiceScanMetricWhenSendDiscoverServices() throws Exception {
        when(mWifiNative.p2pServDiscReq(anyString(), anyString()))
                .thenReturn("mServiceDiscReqId");
        when(mWifiNative.p2pFind(anyInt())).thenReturn(true);
        forceP2pEnabled(mClient1);
        when(mWifiPermissionsUtil.checkCanAccessWifiDirect(anyString(), anyInt(), anyBoolean()))
                .thenReturn(true);
        sendChannelInfoUpdateMsg("testPkg1", mClient1, mClientMessenger);
        sendAddServiceRequestMsg(mClientMessenger);
        sendDiscoverServiceMsg(mClientMessenger);
        verify(mWifiP2pMetrics).incrementServiceScans();
        verify(mWifiPermissionsUtil)
                .checkCanAccessWifiDirect(eq("testPkg1"), anyInt(), eq(true));
    }

    /**
     * Verify the persistent group counter is updated while receiving
     * WifiP2pManager.FACTORY_RESET.
     */
    @Test
    public void testPersistentGroupMetricWhenSendFactoryReset() throws Exception {
        forceP2pEnabled(mClient1);
        sendChannelInfoUpdateMsg("testPkg1", mClient1, mClientMessenger);

        // permissions for factory reset
        when(mWifiPermissionsUtil.checkNetworkSettingsPermission(anyInt()))
                .thenReturn(true);
        when(mUserManager.hasUserRestriction(eq(UserManager.DISALLOW_NETWORK_RESET)))
                .thenReturn(false);
        when(mUserManager.hasUserRestriction(eq(UserManager.DISALLOW_CONFIG_WIFI)))
                .thenReturn(false);

        ArgumentCaptor<WifiP2pGroupList> groupsCaptor =
                ArgumentCaptor.forClass(WifiP2pGroupList.class);
        verify(mWifiP2pMetrics).updatePersistentGroup(groupsCaptor.capture());
        assertEquals(3, groupsCaptor.getValue().getGroupList().size());

        sendSimpleMsg(mClientMessenger, WifiP2pManager.FACTORY_RESET);

        verify(mWifiP2pMetrics, times(2)).updatePersistentGroup(groupsCaptor.capture());
        // the captured object is the same object, just get the latest one is ok.
        assertEquals(0, groupsCaptor.getValue().getGroupList().size());
    }

    /**
     * Verify the persistent group counter is updated while receiving
     * WifiP2pMonitor.P2P_GROUP_STARTED_EVENT.
     */
    @Test
    public void testPersistentGroupMetricWhenSendP2pGroupStartedEvent() throws Exception {
        forceP2pEnabled(mClient1);
        sendChannelInfoUpdateMsg("testPkg1", mClient1, mClientMessenger);

        ArgumentCaptor<WifiP2pGroupList> groupsCaptor =
                ArgumentCaptor.forClass(WifiP2pGroupList.class);
        verify(mWifiP2pMetrics).updatePersistentGroup(groupsCaptor.capture());
        assertEquals(3, groupsCaptor.getValue().getGroupList().size());

        sendGroupStartedMsg(mTestWifiP2pNewPersistentGoGroup);

        verify(mWifiP2pMetrics, times(2)).updatePersistentGroup(groupsCaptor.capture());
        // the captured object is the same object, just get the latest one is ok.
        assertEquals(4, groupsCaptor.getValue().getGroupList().size());
    }

    /**
     * Verify the persistent group counter is updated while receiving
     * WifiP2pManager.DELETE_PERSISTENT_GROUP.
     */
    @Test
    public void testPersistentGroupMetricWhenSendDeletePersistentGroup() throws Exception {
        forceP2pEnabled(mClient1);
        sendChannelInfoUpdateMsg("testPkg1", mClient1, mClientMessenger);

        ArgumentCaptor<WifiP2pGroupList> groupsCaptor =
                ArgumentCaptor.forClass(WifiP2pGroupList.class);
        verify(mWifiP2pMetrics).updatePersistentGroup(groupsCaptor.capture());
        assertEquals(3, groupsCaptor.getValue().getGroupList().size());

        sendDeletePersistentGroupMsg(mClientMessenger, 0);

        verify(mWifiP2pMetrics, times(2)).updatePersistentGroup(groupsCaptor.capture());
        // the captured object is the same object, just get the latest one is ok.
        assertEquals(2, groupsCaptor.getValue().getGroupList().size());
    }

    /**
     * Verify the group event.
     */
    @Test
    public void testGroupEventMetric() throws Exception {
        forceP2pEnabled(mClient1);
        sendChannelInfoUpdateMsg("testPkg1", mClient1, mClientMessenger);

        sendGroupStartedMsg(mTestWifiP2pNewPersistentGoGroup);

        ArgumentCaptor<WifiP2pGroup> groupCaptor =
                ArgumentCaptor.forClass(WifiP2pGroup.class);
        verify(mWifiP2pMetrics).startGroupEvent(groupCaptor.capture());
        WifiP2pGroup groupCaptured = groupCaptor.getValue();
        assertEquals(mTestWifiP2pNewPersistentGoGroup.toString(), groupCaptured.toString());

        sendGroupRemovedMsg();
        verify(mWifiP2pMetrics).endGroupEvent();
    }

    /**
     * Verify the connection event for a fresh connection.
     */
    @Test
    public void testStartFreshConnectionEventWhenSendConnect() throws Exception {
        forceP2pEnabled(mClient1);
        when(mWifiPermissionsUtil.checkCanAccessWifiDirect(anyString(), anyInt(), anyBoolean()))
                .thenReturn(true);
        sendChannelInfoUpdateMsg("testPkg1", mClient1, mClientMessenger);

        mockPeersList();
        sendConnectMsg(mClientMessenger, mTestWifiP2pPeerConfig);
        verify(mWifiPermissionsUtil)
                .checkCanAccessWifiDirect(eq("testPkg1"), anyInt(), eq(false));

        ArgumentCaptor<WifiP2pConfig> configCaptor =
                ArgumentCaptor.forClass(WifiP2pConfig.class);
        verify(mWifiP2pMetrics).startConnectionEvent(
                eq(P2pConnectionEvent.CONNECTION_FRESH),
                configCaptor.capture());
        assertEquals(mTestWifiP2pPeerConfig.toString(), configCaptor.getValue().toString());
    }

    /**
     * Verify the connection event for a reinvoked connection.
     */
    @Test
    public void testStartReinvokeConnectionEventWhenSendConnect() throws Exception {
        forceP2pEnabled(mClient1);
        when(mWifiPermissionsUtil.checkCanAccessWifiDirect(anyString(), anyInt(), anyBoolean()))
                .thenReturn(true);
        when(mWifiNative.p2pGroupAdd(anyInt()))
                .thenReturn(true);
        when(mTestWifiP2pDevice.isGroupOwner()).thenReturn(true);
        when(mWifiNative.p2pGetSsid(eq(mTestWifiP2pDevice.deviceAddress)))
                .thenReturn(mTestWifiP2pGroup.getNetworkName());
        sendChannelInfoUpdateMsg("testPkg1", mClient1, mClientMessenger);

        mockPeersList();
        sendConnectMsg(mClientMessenger, mTestWifiP2pPeerConfig);
        verify(mWifiPermissionsUtil)
                .checkCanAccessWifiDirect(eq("testPkg1"), anyInt(), eq(false));

        ArgumentCaptor<WifiP2pConfig> configCaptor =
                ArgumentCaptor.forClass(WifiP2pConfig.class);
        verify(mWifiP2pMetrics).startConnectionEvent(
                eq(P2pConnectionEvent.CONNECTION_REINVOKE),
                configCaptor.capture());
        assertEquals(mTestWifiP2pPeerConfig.toString(), configCaptor.getValue().toString());
    }

    /**
     * Verify the connection event for a reinvoked connection via
     * createGroup API.
     *
     * If there is a persistent group whose owner is this deivce, this would be
     * a reinvoked group.
     */
    @Test
    public void testStartReinvokeConnectionEventWhenCreateGroup()
            throws Exception {
        forceP2pEnabled(mClient1);
        when(mWifiPermissionsUtil.checkCanAccessWifiDirect(anyString(), anyInt(), anyBoolean()))
                .thenReturn(true);
        sendChannelInfoUpdateMsg("testPkg1", mClient1, mClientMessenger);

        sendCreateGroupMsg(mClientMessenger, WifiP2pGroup.PERSISTENT_NET_ID, null);
        verify(mWifiPermissionsUtil)
                .checkCanAccessWifiDirect(eq("testPkg1"), anyInt(), eq(false));

        verify(mWifiP2pMetrics).startConnectionEvent(
                eq(P2pConnectionEvent.CONNECTION_REINVOKE),
                eq(null));
    }

    /**
     * Verify the connection event for a local connection while setting
     * netId to {@link WifiP2pGroup#PERSISTENT_NET_ID}.
     */
    @Test
    public void testStartLocalConnectionWhenCreateGroup() throws Exception {
        forceP2pEnabled(mClient1);
        when(mWifiPermissionsUtil.checkCanAccessWifiDirect(anyString(), anyInt(), anyBoolean()))
                .thenReturn(true);
        sendChannelInfoUpdateMsg("testPkg1", mClient1, mClientMessenger);

        // permissions for factory reset
        when(mWifiPermissionsUtil.checkNetworkSettingsPermission(anyInt()))
                .thenReturn(true);
        when(mUserManager.hasUserRestriction(eq(UserManager.DISALLOW_NETWORK_RESET)))
                .thenReturn(false);
        when(mUserManager.hasUserRestriction(eq(UserManager.DISALLOW_CONFIG_WIFI)))
                .thenReturn(false);

        // There is one group hosted by this device in mGroups.
        // clear all groups to avoid re-invoking a group.
        sendSimpleMsg(mClientMessenger, WifiP2pManager.FACTORY_RESET);

        sendCreateGroupMsg(mClientMessenger, WifiP2pGroup.PERSISTENT_NET_ID, null);
        verify(mWifiPermissionsUtil)
                .checkCanAccessWifiDirect(eq("testPkg1"), anyInt(), eq(false));

        verify(mWifiP2pMetrics).startConnectionEvent(
                eq(P2pConnectionEvent.CONNECTION_LOCAL),
                eq(null));
    }

    /**
     * Verify the connection event for a local connection while setting the
     * netId to {@link WifiP2pGroup#TEMPORARY_NET_ID}.
     */
    @Test
    public void testStartLocalConnectionEventWhenCreateTemporaryGroup() throws Exception {
        forceP2pEnabled(mClient1);
        when(mWifiPermissionsUtil.checkCanAccessWifiDirect(anyString(), anyInt(), anyBoolean()))
                .thenReturn(true);
        sendChannelInfoUpdateMsg("testPkg1", mClient1, mClientMessenger);

        sendCreateGroupMsg(mClientMessenger, WifiP2pGroup.TEMPORARY_NET_ID, null);
        verify(mWifiPermissionsUtil)
                .checkCanAccessWifiDirect(eq("testPkg1"), anyInt(), eq(false));

        verify(mWifiP2pMetrics).startConnectionEvent(
                eq(P2pConnectionEvent.CONNECTION_LOCAL),
                eq(null));
    }

    /**
     * Verify the connection event for a fast connection via
     * connect with config.
     */
    @Test
    public void testStartFastConnectionEventWhenSendConnectWithConfig()
            throws Exception {
        forceP2pEnabled(mClient1);
        when(mWifiPermissionsUtil.checkCanAccessWifiDirect(anyString(), anyInt(), anyBoolean()))
                .thenReturn(true);
        when(mWifiNative.p2pGroupAdd(any(), eq(true))).thenReturn(true);
        sendChannelInfoUpdateMsg("testPkg1", mClient1, mClientMessenger);

        sendConnectMsg(mClientMessenger, mTestWifiP2pFastConnectionConfig);
        verify(mWifiPermissionsUtil)
                .checkCanAccessWifiDirect(eq("testPkg1"), anyInt(), eq(false));

        ArgumentCaptor<WifiP2pConfig> configCaptor =
                ArgumentCaptor.forClass(WifiP2pConfig.class);
        verify(mWifiP2pMetrics).startConnectionEvent(
                eq(P2pConnectionEvent.CONNECTION_FAST),
                configCaptor.capture());
        assertEquals(mTestWifiP2pFastConnectionConfig.toString(),
                configCaptor.getValue().toString());
    }

    /**
     * Verify the connection event for a fast connection via
     * createGroup API with config.
     */
    @Test
    public void testStartFastConnectionEventWhenCreateGroupWithConfig()
            throws Exception {
        forceP2pEnabled(mClient1);
        when(mWifiPermissionsUtil.checkCanAccessWifiDirect(anyString(), anyInt(), anyBoolean()))
                .thenReturn(true);
        sendChannelInfoUpdateMsg("testPkg1", mClient1, mClientMessenger);

        sendCreateGroupMsg(mClientMessenger, 0, mTestWifiP2pFastConnectionConfig);
        verify(mWifiPermissionsUtil)
                .checkCanAccessWifiDirect(eq("testPkg1"), anyInt(), eq(false));

        ArgumentCaptor<WifiP2pConfig> configCaptor =
                ArgumentCaptor.forClass(WifiP2pConfig.class);
        verify(mWifiP2pMetrics).startConnectionEvent(
                eq(P2pConnectionEvent.CONNECTION_FAST),
                configCaptor.capture());
        assertEquals(mTestWifiP2pFastConnectionConfig.toString(),
                configCaptor.getValue().toString());
    }

    /**
     * Verify the connection event ends while the group is formed.
     */
    @Test
    public void testEndConnectionEventWhenGroupFormed() throws Exception {
        forceP2pEnabled(mClient1);
        sendChannelInfoUpdateMsg("testPkg1", mClient1, mClientMessenger);

        WifiP2pGroup group = new WifiP2pGroup();
        group.setNetworkId(WifiP2pGroup.PERSISTENT_NET_ID);
        group.setNetworkName("DIRECT-xy-NEW");
        group.setOwner(new WifiP2pDevice("thisDeviceMac"));
        group.setIsGroupOwner(true);
        sendGroupStartedMsg(group);
        verify(mWifiP2pMetrics).endConnectionEvent(
                eq(P2pConnectionEvent.CLF_NONE));
    }

    /**
     * Verify the connection event ends due to timeout.
     */
    @Test
    public void testEndConnectionEventWhenTimeout() throws Exception {
        forceP2pEnabled(mClient1);
        when(mWifiPermissionsUtil.checkCanAccessWifiDirect(anyString(), anyInt(), anyBoolean()))
                .thenReturn(true);
        when(mWifiNative.p2pGroupAdd(anyBoolean())).thenReturn(true);
        sendChannelInfoUpdateMsg("testPkg1", mClient1, mClientMessenger);

        mockEnterGroupNegotiationState();
        verify(mWifiPermissionsUtil)
                .checkCanAccessWifiDirect(eq("testPkg1"), anyInt(), eq(false));

        mLooper.moveTimeForward(120 * 1000 * 2);
        mLooper.dispatchAll();

        verify(mWifiP2pMetrics).endConnectionEvent(
                eq(P2pConnectionEvent.CLF_TIMEOUT));
    }

    /**
     * Verify the connection event ends due to the cancellation.
     */
    @Test
    public void testEndConnectionEventWhenCancel() throws Exception {
        forceP2pEnabled(mClient1);
        when(mWifiPermissionsUtil.checkCanAccessWifiDirect(anyString(), anyInt(), anyBoolean()))
                .thenReturn(true);
        when(mWifiNative.p2pGroupAdd(anyBoolean())).thenReturn(true);
        sendChannelInfoUpdateMsg("testPkg1", mClient1, mClientMessenger);

        mockEnterGroupNegotiationState();
        verify(mWifiPermissionsUtil)
                .checkCanAccessWifiDirect(eq("testPkg1"), anyInt(), eq(false));

        sendSimpleMsg(mClientMessenger, WifiP2pManager.CANCEL_CONNECT);

        verify(mWifiP2pMetrics).endConnectionEvent(
                eq(P2pConnectionEvent.CLF_CANCEL));
    }

    /**
     * Verify the connection event ends due to the provision discovery failure.
     */
    @Test
    public void testEndConnectionEventWhenProvDiscFailure() throws Exception {
        forceP2pEnabled(mClient1);
        when(mWifiPermissionsUtil.checkCanAccessWifiDirect(anyString(), anyInt(), anyBoolean()))
                .thenReturn(true);
        when(mWifiNative.p2pGroupAdd(anyBoolean())).thenReturn(true);
        sendChannelInfoUpdateMsg("testPkg1", mClient1, mClientMessenger);

        mockEnterProvisionDiscoveryState();
        verify(mWifiPermissionsUtil)
                .checkCanAccessWifiDirect(eq("testPkg1"), anyInt(), eq(false));

        sendSimpleMsg(null, WifiP2pMonitor.P2P_PROV_DISC_FAILURE_EVENT);

        verify(mWifiP2pMetrics).endConnectionEvent(
                eq(P2pConnectionEvent.CLF_PROV_DISC_FAIL));
    }

    /**
     * Verify the connection event ends due to the group removal.
     */
    @Test
    public void testEndConnectionEventWhenGroupRemoval() throws Exception {
        forceP2pEnabled(mClient1);
        when(mWifiPermissionsUtil.checkCanAccessWifiDirect(anyString(), anyInt(), anyBoolean()))
                .thenReturn(true);
        when(mWifiNative.p2pGroupAdd(anyBoolean())).thenReturn(true);
        sendChannelInfoUpdateMsg("testPkg1", mClient1, mClientMessenger);

        mockEnterGroupNegotiationState();
        verify(mWifiPermissionsUtil)
                .checkCanAccessWifiDirect(eq("testPkg1"), anyInt(), eq(false));

        sendSimpleMsg(null, WifiP2pMonitor.P2P_GROUP_REMOVED_EVENT);

        verify(mWifiP2pMetrics).endConnectionEvent(
                eq(P2pConnectionEvent.CLF_UNKNOWN));
    }

    /**
     * Verify the connection event ends due to the invitation failure.
     */
    @Test
    public void testEndConnectionEventWhenInvitationFailure() throws Exception {
        forceP2pEnabled(mClient1);
        when(mWifiPermissionsUtil.checkCanAccessWifiDirect(anyString(), anyInt(), anyBoolean()))
                .thenReturn(true);
        when(mWifiNative.p2pGroupAdd(anyBoolean())).thenReturn(true);
        sendChannelInfoUpdateMsg("testPkg1", mClient1, mClientMessenger);

        mockEnterGroupNegotiationState();
        verify(mWifiPermissionsUtil)
                .checkCanAccessWifiDirect(eq("testPkg1"), anyInt(), eq(false));

        sendInvitationResultMsg(WifiP2pServiceImpl.P2pStatus.UNKNOWN);

        verify(mWifiP2pMetrics).endConnectionEvent(
                eq(P2pConnectionEvent.CLF_INVITATION_FAIL));
    }

    /**
     * Verify WifiP2pManager.RESPONSE_DEVICE_INFO is returned with null object when a caller
     * without proper permission attempts.
     */
    @Test
    public void testRequestDeviceInfoFailureWhenPermissionDenied() throws Exception {
        forceP2pEnabled(mClient1);
        doNothing().when(mWifiPermissionsUtil).checkPackage(anyInt(), anyString());
        when(mWifiPermissionsUtil.checkCanAccessWifiDirect(anyString(), anyInt(), anyBoolean()))
                .thenReturn(false);
        sendChannelInfoUpdateMsg("testPkg1", mClient1, mClientMessenger);
        sendSimpleMsg(mClientMessenger, WifiP2pManager.REQUEST_DEVICE_INFO);
        verify(mWifiPermissionsUtil)
                .checkCanAccessWifiDirect(eq("testPkg1"), anyInt(), eq(false));
        verify(mClientHandler).sendMessage(mMessageCaptor.capture());
        assertEquals(WifiP2pManager.RESPONSE_DEVICE_INFO, mMessageCaptor.getValue().what);
        assertNull(mMessageCaptor.getValue().obj);
    }

    /**
     * Verify WifiP2pManager.RESPONSE_DEVICE_INFO is returned with expect object when a caller
     * with proper permission attempts in p2p enabled state.
     */
    @Test
    public void testRequestDeviceInfoSuccessWhenP2pEnabled() throws Exception {
        forceP2pEnabled(mClient1);
        when(mWifiPermissionsUtil.checkCanAccessWifiDirect(anyString(), anyInt(), anyBoolean()))
                .thenReturn(true);
        sendChannelInfoUpdateMsg("testPkg1", mClient1, mClientMessenger);
        sendSimpleMsg(mClientMessenger, WifiP2pManager.REQUEST_DEVICE_INFO);
        verify(mWifiPermissionsUtil)
                .checkCanAccessWifiDirect(eq("testPkg1"), anyInt(), eq(false));
        verify(mClientHandler).sendMessage(mMessageCaptor.capture());
        assertEquals(WifiP2pManager.RESPONSE_DEVICE_INFO, mMessageCaptor.getValue().what);
        WifiP2pDevice wifiP2pDevice = (WifiP2pDevice) mMessageCaptor.getValue().obj;
        assertEquals(ANONYMIZED_DEVICE_ADDRESS, wifiP2pDevice.deviceAddress);
        assertEquals(thisDeviceName, wifiP2pDevice.deviceName);
    }

    /**
     * Verify WifiP2pManager.RESPONSE_DEVICE_INFO is returned with empty object when a caller
     * with proper permission attempts in p2p disabled state.
     */
    @Test
    public void testRequestDeviceInfoReturnEmptyWifiP2pDeviceWhenP2pDisabled() throws Exception {
        when(mWifiPermissionsUtil.checkCanAccessWifiDirect(anyString(), anyInt(), anyBoolean()))
                .thenReturn(true);
        sendChannelInfoUpdateMsg("testPkg1", mClient1, mClientMessenger);
        sendSimpleMsg(mClientMessenger, WifiP2pManager.REQUEST_DEVICE_INFO);
        verify(mWifiPermissionsUtil)
                .checkCanAccessWifiDirect(eq("testPkg1"), anyInt(), eq(false));
        verify(mClientHandler).sendMessage(mMessageCaptor.capture());
        assertEquals(WifiP2pManager.RESPONSE_DEVICE_INFO, mMessageCaptor.getValue().what);
        WifiP2pDevice wifiP2pDevice = (WifiP2pDevice) mMessageCaptor.getValue().obj;
        assertEquals("", wifiP2pDevice.deviceAddress);
        assertEquals("", wifiP2pDevice.deviceName);
    }

    /**
     * Verify WifiP2pManager.RESPONSE_DEVICE_INFO returns an object with the actual device MAC when
     * the caller holds the LOCAL_MAC_ADDRESS permission.
     */
    @Test
    public void testRequestDeviceInfoReturnsActualMacForNetworkSettingsApp() throws Exception {
        forceP2pEnabled(mClient1);
        when(mWifiPermissionsUtil.checkLocalMacAddressPermission(anyInt())).thenReturn(true);
        when(mWifiPermissionsUtil.checkCanAccessWifiDirect(anyString(), anyInt(), anyBoolean()))
                .thenReturn(true);
        sendChannelInfoUpdateMsg("testPkg1", mClient1, mClientMessenger);
        sendSimpleMsg(mClientMessenger, WifiP2pManager.REQUEST_DEVICE_INFO);
        verify(mWifiPermissionsUtil)
                .checkCanAccessWifiDirect(eq("testPkg1"), anyInt(), eq(false));
        verify(mClientHandler).sendMessage(mMessageCaptor.capture());
        assertEquals(WifiP2pManager.RESPONSE_DEVICE_INFO, mMessageCaptor.getValue().what);
        WifiP2pDevice wifiP2pDevice = (WifiP2pDevice) mMessageCaptor.getValue().obj;
        assertEquals(thisDeviceMac, wifiP2pDevice.deviceAddress);
        assertEquals(thisDeviceName, wifiP2pDevice.deviceName);
    }

    /**
     * Verify the caller sends WifiP2pManager.STOP_DISCOVERY.
     */
    @Test
    public void testStopDiscoverySuccess() throws Exception {
        when(mWifiNative.p2pStopFind()).thenReturn(true);
        forceP2pEnabled(mClient1);
        sendSimpleMsg(mClientMessenger, WifiP2pManager.STOP_DISCOVERY);
        verify(mWifiNative).p2pStopFind();
        verify(mClientHandler).sendMessage(mMessageCaptor.capture());
        Message message = mMessageCaptor.getValue();
        assertEquals(WifiP2pManager.STOP_DISCOVERY_SUCCEEDED, message.what);
    }

    /**
     * Verify WifiP2pManager.STOP_DISCOVERY_FAILED is returned when native call failure.
     */
    @Test
    public void testStopDiscoveryFailureWhenNativeCallFailure() throws Exception {
        when(mWifiNative.p2pStopFind()).thenReturn(false);
        forceP2pEnabled(mClient1);
        sendSimpleMsg(mClientMessenger, WifiP2pManager.STOP_DISCOVERY);
        verify(mWifiNative).p2pStopFind();
        verify(mClientHandler).sendMessage(mMessageCaptor.capture());
        Message message = mMessageCaptor.getValue();
        assertEquals(WifiP2pManager.STOP_DISCOVERY_FAILED, message.what);
        assertEquals(WifiP2pManager.ERROR, message.arg1);
    }

    /**
     * Verify WifiP2pManager.STOP_DISCOVERY_FAILED is returned when p2p is disabled.
     */
    @Test
    public void testStopDiscoveryFailureWhenP2pDisabled() throws Exception {
        sendSimpleMsg(mClientMessenger, WifiP2pManager.STOP_DISCOVERY);
        verify(mClientHandler).sendMessage(mMessageCaptor.capture());
        Message message = mMessageCaptor.getValue();
        assertEquals(WifiP2pManager.STOP_DISCOVERY_FAILED, message.what);
        assertEquals(WifiP2pManager.BUSY, message.arg1);
    }

    /**
     * Verify WifiP2pManager.STOP_DISCOVERY_FAILED is returned when p2p is unsupported.
     */
    @Test
    public void testStopDiscoveryFailureWhenP2pUnsupported() throws Exception {
        setUpWifiP2pServiceImpl(false);
        sendSimpleMsg(mClientMessenger, WifiP2pManager.STOP_DISCOVERY);
        verify(mClientHandler).sendMessage(mMessageCaptor.capture());
        Message message = mMessageCaptor.getValue();
        assertEquals(WifiP2pManager.STOP_DISCOVERY_FAILED, message.what);
        assertEquals(WifiP2pManager.P2P_UNSUPPORTED, message.arg1);
    }

    /**
     * Verify the caller sends WifiP2pManager.CANCEL_CONNECT.
     */
    @Test
    public void testCancelConnectSuccess() throws Exception {
        // Move to group creating state
        testConnectWithConfigValidAsGroupSuccess();

        sendSimpleMsg(mClientMessenger, WifiP2pManager.CANCEL_CONNECT);
        verify(mClientHandler, atLeastOnce()).sendMessage(mMessageCaptor.capture());
        Message message = mMessageCaptor.getValue();
        assertEquals(WifiP2pManager.CANCEL_CONNECT_SUCCEEDED, message.what);
    }

    /**
     * Verify WifiP2pManager.CANCEL_CONNECT_FAILED is returned when p2p is inactive.
     */
    @Test
    public void testCancelConnectFailureWhenP2pInactive() throws Exception {
        // Move to inactive state
        forceP2pEnabled(mClient1);

        sendSimpleMsg(mClientMessenger, WifiP2pManager.CANCEL_CONNECT);
        verify(mClientHandler).sendMessage(mMessageCaptor.capture());
        Message message = mMessageCaptor.getValue();
        assertEquals(WifiP2pManager.CANCEL_CONNECT_FAILED, message.what);
        assertEquals(WifiP2pManager.BUSY, message.arg1);
    }

    /**
     * Verify WifiP2pManager.CANCEL_CONNECT_FAILED is returned when p2p is disabled.
     */
    @Test
    public void testCancelConnectFailureWhenP2pDisabled() throws Exception {
        sendSimpleMsg(mClientMessenger, WifiP2pManager.CANCEL_CONNECT);
        verify(mClientHandler).sendMessage(mMessageCaptor.capture());
        Message message = mMessageCaptor.getValue();
        assertEquals(WifiP2pManager.CANCEL_CONNECT_FAILED, message.what);
        assertEquals(WifiP2pManager.BUSY, message.arg1);
    }

    /**
     * Verify WifiP2pManager.CANCEL_CONNECT_FAILED is returned when p2p is unsupported.
     */
    @Test
    public void testCancelConnectFailureWhenP2pUnsupported() throws Exception {
        setUpWifiP2pServiceImpl(false);
        sendSimpleMsg(mClientMessenger, WifiP2pManager.CANCEL_CONNECT);
        verify(mClientHandler).sendMessage(mMessageCaptor.capture());
        Message message = mMessageCaptor.getValue();
        assertEquals(WifiP2pManager.CANCEL_CONNECT_FAILED, message.what);
        assertEquals(WifiP2pManager.P2P_UNSUPPORTED, message.arg1);
    }

    /**
     * Verify the caller sends WifiP2pManager.REMOVE_GROUP.
     */
    @Test
    public void testRemoveGroupSuccess() throws Exception {
        // Move to group created state
        forceP2pEnabled(mClient1);
        WifiP2pGroup group = new WifiP2pGroup();
        group.setNetworkId(WifiP2pGroup.PERSISTENT_NET_ID);
        group.setNetworkName("DIRECT-xy-NEW");
        group.setOwner(new WifiP2pDevice("thisDeviceMac"));
        group.setIsGroupOwner(true);
        group.setInterface(IFACE_NAME_P2P);
        sendGroupStartedMsg(group);

        when(mWifiNative.p2pGroupRemove(eq(IFACE_NAME_P2P))).thenReturn(true);
        sendSimpleMsg(mClientMessenger, WifiP2pManager.REMOVE_GROUP);
        verify(mWifiNative).p2pGroupRemove(eq(IFACE_NAME_P2P));
        verify(mClientHandler).sendMessage(mMessageCaptor.capture());
        Message message = mMessageCaptor.getValue();
        assertEquals(WifiP2pManager.REMOVE_GROUP_SUCCEEDED, message.what);
    }

    /**
     *  Verify WifiP2pManager.REMOVE_GROUP_FAILED is returned when native call failure.
     */
    @Test
    public void testRemoveGroupFailureWhenNativeCallFailure() throws Exception {
        // Move to group created state
        forceP2pEnabled(mClient1);
        WifiP2pGroup group = new WifiP2pGroup();
        group.setNetworkId(WifiP2pGroup.PERSISTENT_NET_ID);
        group.setNetworkName("DIRECT-xy-NEW");
        group.setOwner(new WifiP2pDevice("thisDeviceMac"));
        group.setIsGroupOwner(true);
        group.setInterface(IFACE_NAME_P2P);
        sendGroupStartedMsg(group);

        when(mWifiNative.p2pGroupRemove(eq(IFACE_NAME_P2P))).thenReturn(false);
        sendSimpleMsg(mClientMessenger, WifiP2pManager.REMOVE_GROUP);
        verify(mWifiNative).p2pGroupRemove(eq(IFACE_NAME_P2P));
        verify(mClientHandler).sendMessage(mMessageCaptor.capture());
        Message message = mMessageCaptor.getValue();
        assertEquals(WifiP2pManager.REMOVE_GROUP_FAILED, message.what);
        assertEquals(WifiP2pManager.ERROR, message.arg1);
    }

    /**
     *  Verify WifiP2pManager.REMOVE_GROUP_FAILED is returned when p2p is creating group.
     */
    @Test
    public void testRemoveGroupFailureWhenP2pCreatingGroup() throws Exception {
        // Move to group creating state
        testConnectWithConfigValidAsGroupSuccess();

        sendSimpleMsg(mClientMessenger, WifiP2pManager.REMOVE_GROUP);
        verify(mClientHandler, atLeastOnce()).sendMessage(mMessageCaptor.capture());
        Message message = mMessageCaptor.getValue();
        assertEquals(WifiP2pManager.REMOVE_GROUP_FAILED, message.what);
        assertEquals(WifiP2pManager.BUSY, message.arg1);
    }

    /**
     *  Verify WifiP2pManager.REMOVE_GROUP_FAILED is returned when p2p is inactive.
     */
    @Test
    public void testRemoveGroupFailureWhenP2pInactive() throws Exception {
        // Move to inactive state
        forceP2pEnabled(mClient1);

        sendSimpleMsg(mClientMessenger, WifiP2pManager.REMOVE_GROUP);
        verify(mClientHandler).sendMessage(mMessageCaptor.capture());
        Message message = mMessageCaptor.getValue();
        assertEquals(WifiP2pManager.REMOVE_GROUP_FAILED, message.what);
        assertEquals(WifiP2pManager.BUSY, message.arg1);
    }

    /**
     *  Verify WifiP2pManager.REMOVE_GROUP_FAILED is returned when p2p is disabled.
     */
    @Test
    public void testRemoveGroupFailureWhenP2pDisabled() throws Exception {
        sendSimpleMsg(mClientMessenger, WifiP2pManager.REMOVE_GROUP);
        verify(mClientHandler).sendMessage(mMessageCaptor.capture());
        Message message = mMessageCaptor.getValue();
        assertEquals(WifiP2pManager.REMOVE_GROUP_FAILED, message.what);
        assertEquals(WifiP2pManager.BUSY, message.arg1);
    }

    /**
     *  Verify WifiP2pManager.REMOVE_GROUP_FAILED is returned when p2p is unsupported.
     */
    @Test
    public void testRemoveGroupFailureWhenP2pUnsupported() throws Exception {
        setUpWifiP2pServiceImpl(false);
        sendSimpleMsg(mClientMessenger, WifiP2pManager.REMOVE_GROUP);
        verify(mClientHandler).sendMessage(mMessageCaptor.capture());
        Message message = mMessageCaptor.getValue();
        assertEquals(WifiP2pManager.REMOVE_GROUP_FAILED, message.what);
        assertEquals(WifiP2pManager.P2P_UNSUPPORTED, message.arg1);
    }

    /**
     * Verify the caller sends WifiP2pManager.SET_CHANNEL.
     */
    @Test
    public void testSetChannelSuccess() throws Exception {
        // Move to enabled state
        forceP2pEnabled(mClient1);

        Bundle p2pChannels = new Bundle();
        p2pChannels.putInt("lc", 1);
        p2pChannels.putInt("oc", 2);
        when(mWifiNative.p2pSetChannel(anyInt(), anyInt())).thenReturn(true);
        sendSetChannelMsg(mClientMessenger, p2pChannels);
        verify(mWifiNative).p2pSetChannel(eq(1), eq(2));
        verify(mClientHandler).sendMessage(mMessageCaptor.capture());
        Message message = mMessageCaptor.getValue();
        assertEquals(WifiP2pManager.SET_CHANNEL_SUCCEEDED, message.what);
    }

    /**
     *  Verify WifiP2pManager.SET_CHANNEL_FAILED is returned when native call failure.
     */
    @Test
    public void testSetChannelFailureWhenNativeCallFailure() throws Exception {
        // Move to enabled state
        forceP2pEnabled(mClient1);

        Bundle p2pChannels = new Bundle();
        p2pChannels.putInt("lc", 1);
        p2pChannels.putInt("oc", 2);
        when(mWifiNative.p2pSetChannel(anyInt(), anyInt())).thenReturn(false);
        sendSetChannelMsg(mClientMessenger, p2pChannels);
        verify(mWifiNative).p2pSetChannel(eq(1), eq(2));
        verify(mClientHandler).sendMessage(mMessageCaptor.capture());
        Message message = mMessageCaptor.getValue();
        assertEquals(WifiP2pManager.SET_CHANNEL_FAILED, message.what);
    }

    /**
     *  Verify p2pSetChannel doesn't been called when message contain null object.
     */
    @Test
    public void testSetChannelFailureWhenObjectIsNull() throws Exception {
        // Move to enabled state
        forceP2pEnabled(mClient1);

        when(mWifiNative.p2pSetChannel(anyInt(), anyInt())).thenReturn(false);
        sendSetChannelMsg(mClientMessenger, null);
        verify(mWifiNative, never()).p2pSetChannel(anyInt(), anyInt());
    }

    /**
     * Verify the caller sends WifiP2pManager.START_WPS with push button configuration.
     */
    @Test
    public void testStartWpsWithPbcSuccess() throws Exception {
        // Move to group created state
        forceP2pEnabled(mClient1);
        WifiP2pGroup group = new WifiP2pGroup();
        group.setNetworkId(WifiP2pGroup.PERSISTENT_NET_ID);
        group.setNetworkName("DIRECT-xy-NEW");
        group.setOwner(new WifiP2pDevice("thisDeviceMac"));
        group.setIsGroupOwner(true);
        group.setInterface(IFACE_NAME_P2P);
        sendGroupStartedMsg(group);

        when(mWifiNative.startWpsPbc(anyString(), any())).thenReturn(true);
        WpsInfo wps = new WpsInfo();
        wps.setup = WpsInfo.PBC;
        sendStartWpsMsg(mClientMessenger, wps);
        verify(mWifiNative).startWpsPbc(eq(IFACE_NAME_P2P), isNull());
        verify(mClientHandler).sendMessage(mMessageCaptor.capture());
        Message message = mMessageCaptor.getValue();
        assertEquals(WifiP2pManager.START_WPS_SUCCEEDED, message.what);
    }

    /**
     * Verify the caller sends WifiP2pManager.START_WPS with pin display.
     */
    @Test
    public void testStartWpsWithPinDisplaySuccess() throws Exception {
        // TODO(hsiuchangchen): This test item is related to UI.
    }

    /**
     * Verify the caller sends WifiP2pManager.START_WPS with pin keypad.
     */
    @Test
    public void testStartWpsWithPinKeypadSuccess() throws Exception {
        // Move to group created state
        forceP2pEnabled(mClient1);
        WifiP2pGroup group = new WifiP2pGroup();
        group.setNetworkId(WifiP2pGroup.PERSISTENT_NET_ID);
        group.setNetworkName("DIRECT-xy-NEW");
        group.setOwner(new WifiP2pDevice("thisDeviceMac"));
        group.setIsGroupOwner(true);
        group.setInterface(IFACE_NAME_P2P);
        sendGroupStartedMsg(group);

        when(mWifiNative.startWpsPinKeypad(anyString(), anyString())).thenReturn(true);
        WpsInfo wps = new WpsInfo();
        wps.setup = WpsInfo.KEYPAD;
        wps.pin = "1234";
        sendStartWpsMsg(mClientMessenger, wps);
        verify(mWifiNative).startWpsPinKeypad(eq(IFACE_NAME_P2P), eq("1234"));
        verify(mClientHandler).sendMessage(mMessageCaptor.capture());
        Message message = mMessageCaptor.getValue();
        assertEquals(WifiP2pManager.START_WPS_SUCCEEDED, message.what);
    }

    /**
     * Verify WifiP2pManager.START_WPS_FAILED is returned when message contain null object.
     */
    @Test
    public void testStartWpsFailureWhenObjectIsNull() throws Exception {
        // Move to group created state
        forceP2pEnabled(mClient1);
        WifiP2pGroup group = new WifiP2pGroup();
        group.setNetworkId(WifiP2pGroup.PERSISTENT_NET_ID);
        group.setNetworkName("DIRECT-xy-NEW");
        group.setOwner(new WifiP2pDevice("thisDeviceMac"));
        group.setIsGroupOwner(true);
        group.setInterface(IFACE_NAME_P2P);
        sendGroupStartedMsg(group);

        WpsInfo wps = null;
        sendStartWpsMsg(mClientMessenger, wps);
        verify(mClientHandler).sendMessage(mMessageCaptor.capture());
        Message message = mMessageCaptor.getValue();
        assertEquals(WifiP2pManager.START_WPS_FAILED, message.what);
    }

    /**
     * Verify WifiP2pManager.START_WPS_FAILED is returned when native call failure with
     * push button configuration.
     */
    @Test
    public void testStartWpsWithPbcFailureWhenNativeCallFailure() throws Exception {
        // Move to group created state
        forceP2pEnabled(mClient1);
        WifiP2pGroup group = new WifiP2pGroup();
        group.setNetworkId(WifiP2pGroup.PERSISTENT_NET_ID);
        group.setNetworkName("DIRECT-xy-NEW");
        group.setOwner(new WifiP2pDevice("thisDeviceMac"));
        group.setIsGroupOwner(true);
        group.setInterface(IFACE_NAME_P2P);
        sendGroupStartedMsg(group);
        when(mWifiNative.startWpsPbc(anyString(), any())).thenReturn(false);
        WpsInfo wps = new WpsInfo();
        wps.setup = WpsInfo.PBC;
        sendStartWpsMsg(mClientMessenger, wps);
        verify(mWifiNative).startWpsPbc(eq(IFACE_NAME_P2P), isNull());
        verify(mClientHandler).sendMessage(mMessageCaptor.capture());
        Message message = mMessageCaptor.getValue();
        assertEquals(WifiP2pManager.START_WPS_FAILED, message.what);
    }

    /**
     * Verify WifiP2pManager.START_WPS_FAILED is returned when native call failure with
     * pin display.
     */
    @Test
    public void testStartWpsWithPinDisplayFailureWhenNativeCallFailure() throws Exception {
        // Move to group created state
        forceP2pEnabled(mClient1);
        WifiP2pGroup group = new WifiP2pGroup();
        group.setNetworkId(WifiP2pGroup.PERSISTENT_NET_ID);
        group.setNetworkName("DIRECT-xy-NEW");
        group.setOwner(new WifiP2pDevice("thisDeviceMac"));
        group.setIsGroupOwner(true);
        group.setInterface(IFACE_NAME_P2P);
        sendGroupStartedMsg(group);

        when(mWifiNative.startWpsPinDisplay(anyString(), any())).thenReturn("abcd");
        WpsInfo wps = new WpsInfo();
        wps.setup = WpsInfo.DISPLAY;
        sendStartWpsMsg(mClientMessenger, wps);
        verify(mWifiNative).startWpsPinDisplay(eq(IFACE_NAME_P2P), isNull());
        verify(mClientHandler).sendMessage(mMessageCaptor.capture());
        Message message = mMessageCaptor.getValue();
        assertEquals(WifiP2pManager.START_WPS_FAILED, message.what);
    }

    /**
     * Verify WifiP2pManager.START_WPS_FAILED is returned when native call failure with
     * pin keypad.
     */
    @Test
    public void testStartWpsWithPinKeypadFailureWhenNativeCallFailure() throws Exception {
        // Move to group created state
        forceP2pEnabled(mClient1);
        WifiP2pGroup group = new WifiP2pGroup();
        group.setNetworkId(WifiP2pGroup.PERSISTENT_NET_ID);
        group.setNetworkName("DIRECT-xy-NEW");
        group.setOwner(new WifiP2pDevice("thisDeviceMac"));
        group.setIsGroupOwner(true);
        group.setInterface(IFACE_NAME_P2P);
        sendGroupStartedMsg(group);

        when(mWifiNative.startWpsPinKeypad(anyString(), anyString())).thenReturn(false);
        WpsInfo wps = new WpsInfo();
        wps.setup = WpsInfo.KEYPAD;
        wps.pin = "1234";
        sendStartWpsMsg(mClientMessenger, wps);
        verify(mWifiNative).startWpsPinKeypad(eq(IFACE_NAME_P2P), eq("1234"));
        verify(mClientHandler).sendMessage(mMessageCaptor.capture());
        Message message = mMessageCaptor.getValue();
        assertEquals(WifiP2pManager.START_WPS_FAILED, message.what);
    }

    /**
     *  Verify WifiP2pManager.START_WPS_FAILED is returned when p2p is inactive.
     */
    @Test
    public void testStartWpsFailureWhenP2pInactive() throws Exception {
        // Move to inactive state
        forceP2pEnabled(mClient1);

        WpsInfo wps = new WpsInfo();
        sendStartWpsMsg(mClientMessenger, wps);
        verify(mClientHandler).sendMessage(mMessageCaptor.capture());
        Message message = mMessageCaptor.getValue();
        assertEquals(WifiP2pManager.START_WPS_FAILED, message.what);
        assertEquals(WifiP2pManager.BUSY, message.arg1);
    }

    /**
     *  Verify WifiP2pManager.START_WPS_FAILED is returned when p2p is disabled.
     */
    @Test
    public void testStartWpsFailureWhenP2pDisabled() throws Exception {
        WpsInfo wps = new WpsInfo();
        sendStartWpsMsg(mClientMessenger, wps);
        verify(mClientHandler).sendMessage(mMessageCaptor.capture());
        Message message = mMessageCaptor.getValue();
        assertEquals(WifiP2pManager.START_WPS_FAILED, message.what);
        assertEquals(WifiP2pManager.BUSY, message.arg1);
    }

    /**
     *  Verify WifiP2pManager.START_WPS_FAILED is returned when p2p is unsupported.
     */
    @Test
    public void testStartWpsFailureWhenP2pUnsupported() throws Exception {
        setUpWifiP2pServiceImpl(false);
        WpsInfo wps = new WpsInfo();
        sendStartWpsMsg(mClientMessenger, wps);
        verify(mClientHandler).sendMessage(mMessageCaptor.capture());
        Message message = mMessageCaptor.getValue();
        assertEquals(WifiP2pManager.START_WPS_FAILED, message.what);
        assertEquals(WifiP2pManager.P2P_UNSUPPORTED, message.arg1);
    }

    /**
     * Verify the caller sends WifiP2pManager.SET_DEVICE_NAME.
     */
    @Test
    public void testSetDeviceNameSuccess() throws Exception {
        // Move to enabled state
        forceP2pEnabled(mClient1);
        mTestThisDevice.status = mTestThisDevice.AVAILABLE;

        mTestThisDevice.deviceName = "another-name";
        when(mWifiNative.setDeviceName(anyString())).thenReturn(true);
        when(mFrameworkFacade.setStringSetting(
                any(), eq(Settings.Global.WIFI_P2P_DEVICE_NAME), anyString())).thenReturn(true);
        sendSetDeviceNameMsg(mClientMessenger, mTestThisDevice);
        verify(mWifiNative).setDeviceName(eq(mTestThisDevice.deviceName));
        verify(mWifiNative).setP2pSsidPostfix(eq("-" + mTestThisDevice.deviceName));
        verify(mFrameworkFacade).setStringSetting(eq(mContext),
                eq(Settings.Global.WIFI_P2P_DEVICE_NAME), eq(mTestThisDevice.deviceName));
        checkSendThisDeviceChangedBroadcast();
        verify(mClientHandler).sendMessage(mMessageCaptor.capture());
        Message message = mMessageCaptor.getValue();
        assertEquals(WifiP2pManager.SET_DEVICE_NAME_SUCCEEDED, message.what);
    }

    /**
     * Verify WifiP2pManager.SET_DEVICE_NAME_FAILED is returned when p2p device is null.
     */
    @Test
    public void testSetDeviceNameFailureWhenNativeCallFailure() throws Exception {
        // Move to enabled state
        forceP2pEnabled(mClient1);

        when(mWifiNative.setDeviceName(anyString())).thenReturn(false);
        sendSetDeviceNameMsg(mClientMessenger, mTestThisDevice);
        verify(mWifiNative).setDeviceName(eq(mTestThisDevice.deviceName));
        verify(mClientHandler).sendMessage(mMessageCaptor.capture());
        Message message = mMessageCaptor.getValue();
        assertEquals(WifiP2pManager.SET_DEVICE_NAME_FAILED, message.what);
        assertEquals(WifiP2pManager.ERROR, message.arg1);
    }

    /**
     * Verify WifiP2pManager.SET_DEVICE_NAME_FAILED is returned when p2p device is null.
     */
    @Test
    public void testSetDeviceNameFailureWhenDeviceIsNull() throws Exception {
        // Move to enabled state
        forceP2pEnabled(mClient1);

        sendSetDeviceNameMsg(mClientMessenger, null);
        verify(mClientHandler).sendMessage(mMessageCaptor.capture());
        Message message = mMessageCaptor.getValue();
        assertEquals(WifiP2pManager.SET_DEVICE_NAME_FAILED, message.what);
        assertEquals(WifiP2pManager.ERROR, message.arg1);
    }

    /**
     * Verify WifiP2pManager.SET_DEVICE_NAME_FAILED is returned when p2p device's name is null.
     */
    @Test
    public void testSetDeviceNameFailureWhenDeviceNameIsNull() throws Exception {
        // Move to enabled state
        forceP2pEnabled(mClient1);

        mTestThisDevice.deviceName = null;
        sendSetDeviceNameMsg(mClientMessenger, mTestThisDevice);
        verify(mClientHandler).sendMessage(mMessageCaptor.capture());
        Message message = mMessageCaptor.getValue();
        assertEquals(WifiP2pManager.SET_DEVICE_NAME_FAILED, message.what);
        assertEquals(WifiP2pManager.ERROR, message.arg1);
    }

    /**
     * Verify WifiP2pManager.SET_DEVICE_NAME_FAILED is returned when p2p is disabled.
     */
    @Test
    public void testSetDeviceNameFailureWhenP2pDisabled() throws Exception {
        sendSetDeviceNameMsg(mClientMessenger, mTestThisDevice);
        verify(mClientHandler).sendMessage(mMessageCaptor.capture());
        Message message = mMessageCaptor.getValue();
        assertEquals(WifiP2pManager.SET_DEVICE_NAME_FAILED, message.what);
        assertEquals(WifiP2pManager.BUSY, message.arg1);
    }

    /**
     * Verify WifiP2pManager.SET_DEVICE_NAME_FAILED is returned when p2p is unsupported.
     */
    @Test
    public void testSetDeviceNameFailureWhenP2pUnsupported() throws Exception {
        setUpWifiP2pServiceImpl(false);
        sendSetDeviceNameMsg(mClientMessenger, mTestThisDevice);
        verify(mClientHandler).sendMessage(mMessageCaptor.capture());
        Message message = mMessageCaptor.getValue();
        assertEquals(WifiP2pManager.SET_DEVICE_NAME_FAILED, message.what);
        assertEquals(WifiP2pManager.P2P_UNSUPPORTED, message.arg1);
    }

    /**
     * Verify the caller sends WifiP2pManager.SET_WFD_INFO with wfd enabled.
     */
    @Test
    public void testSetWfdInfoSuccessWithWfdEnabled() throws Exception {
        // Move to enabled state
        forceP2pEnabled(mClient1);
        mTestThisDevice.status = mTestThisDevice.AVAILABLE;

        mTestThisDevice.wfdInfo = new WifiP2pWfdInfo();
        mTestThisDevice.wfdInfo.setWfdEnabled(true);
        when(mWifiInjector.getWifiPermissionsWrapper()).thenReturn(mWifiPermissionsWrapper);
        when(mWifiPermissionsWrapper.getUidPermission(anyString(), anyInt()))
                .thenReturn(PackageManager.PERMISSION_GRANTED);
        when(mWifiNative.setWfdEnable(anyBoolean())).thenReturn(true);
        when(mWifiNative.setWfdDeviceInfo(anyString())).thenReturn(true);
        sendSetWfdInfoMsg(mClientMessenger, mTestThisDevice.wfdInfo);

        verify(mWifiInjector).getWifiPermissionsWrapper();
        verify(mWifiPermissionsWrapper).getUidPermission(
                eq(android.Manifest.permission.CONFIGURE_WIFI_DISPLAY), anyInt());
        verify(mWifiNative).setWfdEnable(eq(true));
        verify(mWifiNative).setWfdDeviceInfo(eq(mTestThisDevice.wfdInfo.getDeviceInfoHex()));
        checkSendThisDeviceChangedBroadcast();
        verify(mClientHandler).sendMessage(mMessageCaptor.capture());
        Message message = mMessageCaptor.getValue();
        assertEquals(WifiP2pManager.SET_WFD_INFO_SUCCEEDED, message.what);
    }

    /**
     * Verify the caller sends WifiP2pManager.SET_WFD_INFO with wfd is disabled.
     */
    @Test
    public void testSetWfdInfoSuccessWithWfdDisabled() throws Exception {
        // Move to enabled state
        forceP2pEnabled(mClient1);
        mTestThisDevice.status = mTestThisDevice.AVAILABLE;

        mTestThisDevice.wfdInfo = new WifiP2pWfdInfo();
        mTestThisDevice.wfdInfo.setWfdEnabled(false);
        when(mWifiInjector.getWifiPermissionsWrapper()).thenReturn(mWifiPermissionsWrapper);
        when(mWifiPermissionsWrapper.getUidPermission(anyString(), anyInt()))
                .thenReturn(PackageManager.PERMISSION_GRANTED);
        when(mWifiNative.setWfdEnable(anyBoolean())).thenReturn(true);
        sendSetWfdInfoMsg(mClientMessenger, mTestThisDevice.wfdInfo);

        verify(mWifiInjector).getWifiPermissionsWrapper();
        verify(mWifiPermissionsWrapper).getUidPermission(
                eq(android.Manifest.permission.CONFIGURE_WIFI_DISPLAY), anyInt());
        verify(mWifiNative).setWfdEnable(eq(false));
        checkSendThisDeviceChangedBroadcast();
        verify(mClientHandler).sendMessage(mMessageCaptor.capture());
        Message message = mMessageCaptor.getValue();
        assertEquals(WifiP2pManager.SET_WFD_INFO_SUCCEEDED, message.what);
    }

    /**
     * Verify WifiP2pManager.SET_WFD_INFO_FAILED is returned when wfd permission denied.
     */
    @Test
    public void testSetWfdInfoFailureWhenWfdPermissionDenied() throws Exception {
        // Move to enabled state
        forceP2pEnabled(mClient1);
        mTestThisDevice.status = mTestThisDevice.AVAILABLE;

        mTestThisDevice.wfdInfo = new WifiP2pWfdInfo();
        when(mWifiInjector.getWifiPermissionsWrapper()).thenReturn(mWifiPermissionsWrapper);
        when(mWifiPermissionsWrapper.getUidPermission(anyString(), anyInt()))
                .thenReturn(PackageManager.PERMISSION_DENIED);
        sendSetWfdInfoMsg(mClientMessenger, mTestThisDevice.wfdInfo);
        verify(mWifiInjector).getWifiPermissionsWrapper();
        verify(mWifiPermissionsWrapper).getUidPermission(
                eq(android.Manifest.permission.CONFIGURE_WIFI_DISPLAY), anyInt());
        verify(mClientHandler).sendMessage(mMessageCaptor.capture());
        Message message = mMessageCaptor.getValue();
        assertEquals(WifiP2pManager.SET_WFD_INFO_FAILED, message.what);
        assertEquals(WifiP2pManager.ERROR, message.arg1);
    }

    /**
     * Verify WifiP2pManager.SET_WFD_INFO_FAILED is returned when wfdInfo is null.
     */
    @Test
    public void testSetWfdInfoFailureWhenWfdInfoIsNull() throws Exception {
        // Move to enabled state
        forceP2pEnabled(mClient1);
        mTestThisDevice.status = mTestThisDevice.AVAILABLE;

        mTestThisDevice.wfdInfo = null;
        when(mWifiInjector.getWifiPermissionsWrapper()).thenReturn(mWifiPermissionsWrapper);
        when(mWifiPermissionsWrapper.getUidPermission(anyString(), anyInt()))
                .thenReturn(PackageManager.PERMISSION_GRANTED);
        sendSetWfdInfoMsg(mClientMessenger, mTestThisDevice.wfdInfo);
        verify(mWifiInjector).getWifiPermissionsWrapper();
        verify(mWifiPermissionsWrapper).getUidPermission(
                eq(android.Manifest.permission.CONFIGURE_WIFI_DISPLAY), anyInt());
        verify(mClientHandler).sendMessage(mMessageCaptor.capture());
        Message message = mMessageCaptor.getValue();
        assertEquals(WifiP2pManager.SET_WFD_INFO_FAILED, message.what);
        assertEquals(WifiP2pManager.ERROR, message.arg1);
    }

    /**
     * Verify WifiP2pManager.SET_WFD_INFO_FAILED is returned when wfd is enabled
     * and native call "setWfdEnable" failure.
     */
    @Test
    public void testSetWfdInfoFailureWithWfdEnabledWhenNativeCallFailure1() throws Exception {
        // Move to enabled state
        forceP2pEnabled(mClient1);
        mTestThisDevice.status = mTestThisDevice.AVAILABLE;

        mTestThisDevice.wfdInfo = new WifiP2pWfdInfo();
        mTestThisDevice.wfdInfo.setWfdEnabled(true);
        when(mWifiInjector.getWifiPermissionsWrapper()).thenReturn(mWifiPermissionsWrapper);
        when(mWifiPermissionsWrapper.getUidPermission(anyString(), anyInt()))
                .thenReturn(PackageManager.PERMISSION_GRANTED);
        when(mWifiNative.setWfdEnable(anyBoolean())).thenReturn(false);
        sendSetWfdInfoMsg(mClientMessenger, mTestThisDevice.wfdInfo);

        verify(mWifiInjector).getWifiPermissionsWrapper();
        verify(mWifiPermissionsWrapper).getUidPermission(
                eq(android.Manifest.permission.CONFIGURE_WIFI_DISPLAY), anyInt());
        verify(mWifiNative).setWfdEnable(eq(true));
        verify(mClientHandler).sendMessage(mMessageCaptor.capture());
        Message message = mMessageCaptor.getValue();
        assertEquals(WifiP2pManager.SET_WFD_INFO_FAILED, message.what);
        assertEquals(WifiP2pManager.ERROR, message.arg1);
    }

    /**
     * Verify WifiP2pManager.SET_WFD_INFO_FAILED is returned when wfd is enabled
     * and native call "setWfdDeviceInfo" failure.
     */
    @Test
    public void testSetWfdInfoFailureWithWfdEnabledWhenNativeCallFailure2() throws Exception {
        // Move to enabled state
        forceP2pEnabled(mClient1);
        mTestThisDevice.status = mTestThisDevice.AVAILABLE;

        mTestThisDevice.wfdInfo = new WifiP2pWfdInfo();
        mTestThisDevice.wfdInfo.setWfdEnabled(true);
        when(mWifiInjector.getWifiPermissionsWrapper()).thenReturn(mWifiPermissionsWrapper);
        when(mWifiPermissionsWrapper.getUidPermission(anyString(), anyInt()))
                .thenReturn(PackageManager.PERMISSION_GRANTED);
        when(mWifiNative.setWfdEnable(anyBoolean())).thenReturn(true);
        when(mWifiNative.setWfdDeviceInfo(anyString())).thenReturn(false);
        sendSetWfdInfoMsg(mClientMessenger, mTestThisDevice.wfdInfo);

        verify(mWifiInjector).getWifiPermissionsWrapper();
        verify(mWifiPermissionsWrapper).getUidPermission(
                eq(android.Manifest.permission.CONFIGURE_WIFI_DISPLAY), anyInt());
        verify(mWifiNative).setWfdEnable(eq(true));
        verify(mWifiNative).setWfdDeviceInfo(eq(mTestThisDevice.wfdInfo.getDeviceInfoHex()));
        verify(mClientHandler).sendMessage(mMessageCaptor.capture());
        Message message = mMessageCaptor.getValue();
        assertEquals(WifiP2pManager.SET_WFD_INFO_FAILED, message.what);
        assertEquals(WifiP2pManager.ERROR, message.arg1);
    }

    /**
     * Verify WifiP2pManager.SET_WFD_INFO_FAILED is returned when wfd is disabled
     * and native call failure.
     */
    @Test
    public void testSetWfdInfoFailureWithWfdDisabledWhenNativeCallFailure() throws Exception {
        // Move to enabled state
        forceP2pEnabled(mClient1);
        mTestThisDevice.status = mTestThisDevice.AVAILABLE;

        mTestThisDevice.wfdInfo = new WifiP2pWfdInfo();
        mTestThisDevice.wfdInfo.setWfdEnabled(false);
        when(mWifiInjector.getWifiPermissionsWrapper()).thenReturn(mWifiPermissionsWrapper);
        when(mWifiPermissionsWrapper.getUidPermission(anyString(), anyInt()))
                .thenReturn(PackageManager.PERMISSION_GRANTED);
        when(mWifiNative.setWfdEnable(anyBoolean())).thenReturn(false);
        sendSetWfdInfoMsg(mClientMessenger, mTestThisDevice.wfdInfo);

        verify(mWifiInjector).getWifiPermissionsWrapper();
        verify(mWifiPermissionsWrapper).getUidPermission(
                eq(android.Manifest.permission.CONFIGURE_WIFI_DISPLAY), anyInt());
        verify(mWifiNative).setWfdEnable(eq(false));
        verify(mClientHandler).sendMessage(mMessageCaptor.capture());
        Message message = mMessageCaptor.getValue();
        assertEquals(WifiP2pManager.SET_WFD_INFO_FAILED, message.what);
        assertEquals(WifiP2pManager.ERROR, message.arg1);
    }

    /**
     * Verify WifiP2pManager.SET_WFD_INFO_FAILED is returned when p2p is disabled.
     */
    @Test
    public void testSetWfdInfoFailureWhenP2pDisabled() throws Exception {
        mTestThisDevice.wfdInfo = new WifiP2pWfdInfo();
        when(mWifiInjector.getWifiPermissionsWrapper()).thenReturn(mWifiPermissionsWrapper);
        when(mWifiPermissionsWrapper.getUidPermission(anyString(), anyInt()))
                .thenReturn(PackageManager.PERMISSION_GRANTED);
        sendSetWfdInfoMsg(mClientMessenger, mTestThisDevice.wfdInfo);
        verify(mWifiInjector).getWifiPermissionsWrapper();
        verify(mWifiPermissionsWrapper).getUidPermission(
                eq(android.Manifest.permission.CONFIGURE_WIFI_DISPLAY), anyInt());
        verify(mClientHandler).sendMessage(mMessageCaptor.capture());
        Message message = mMessageCaptor.getValue();
        assertEquals(WifiP2pManager.SET_WFD_INFO_FAILED, message.what);
        assertEquals(WifiP2pManager.BUSY, message.arg1);
    }

    /**
     * Verify WifiP2pManager.SET_WFD_INFO_FAILED is returned when wfd permission denied
     * and p2p is disabled.
     */
    @Test
    public void testSetWfdInfoFailureWhenWfdPermissionDeniedAndP2pDisabled() throws Exception {
        mTestThisDevice.wfdInfo = new WifiP2pWfdInfo();
        when(mWifiInjector.getWifiPermissionsWrapper()).thenReturn(mWifiPermissionsWrapper);
        when(mWifiPermissionsWrapper.getUidPermission(anyString(), anyInt()))
                .thenReturn(PackageManager.PERMISSION_DENIED);
        sendSetWfdInfoMsg(mClientMessenger, mTestThisDevice.wfdInfo);
        verify(mWifiInjector).getWifiPermissionsWrapper();
        verify(mWifiPermissionsWrapper).getUidPermission(
                eq(android.Manifest.permission.CONFIGURE_WIFI_DISPLAY), anyInt());
        verify(mClientHandler).sendMessage(mMessageCaptor.capture());
        Message message = mMessageCaptor.getValue();
        assertEquals(WifiP2pManager.SET_WFD_INFO_FAILED, message.what);
        assertEquals(WifiP2pManager.ERROR, message.arg1);
    }

    /**
     * Verify WifiP2pManager.SET_WFD_INFO_FAILED is returned when p2p is unsupported.
     */
    @Test
    public void testSetWfdInfoFailureWhenP2pUnsupported() throws Exception {
        setUpWifiP2pServiceImpl(false);
        mTestThisDevice.wfdInfo = new WifiP2pWfdInfo();
        when(mWifiInjector.getWifiPermissionsWrapper()).thenReturn(mWifiPermissionsWrapper);
        when(mWifiPermissionsWrapper.getUidPermission(anyString(), anyInt()))
                .thenReturn(PackageManager.PERMISSION_GRANTED);
        sendSetWfdInfoMsg(mClientMessenger, mTestThisDevice.wfdInfo);
        verify(mWifiInjector).getWifiPermissionsWrapper();
        verify(mWifiPermissionsWrapper).getUidPermission(
                eq(android.Manifest.permission.CONFIGURE_WIFI_DISPLAY), anyInt());
        verify(mClientHandler).sendMessage(mMessageCaptor.capture());
        Message message = mMessageCaptor.getValue();
        assertEquals(WifiP2pManager.SET_WFD_INFO_FAILED, message.what);
        assertEquals(WifiP2pManager.P2P_UNSUPPORTED, message.arg1);
    }

    /**
     * Verify WifiP2pManager.SET_WFD_INFO_FAILED is returned when wfd permission denied
     * and p2p is unsupported.
     */
    @Test
    public void testSetWfdInfoFailureWhenWfdPermissionDeniedAndP2pUnsupported() throws Exception {
        setUpWifiP2pServiceImpl(false);
        mTestThisDevice.wfdInfo = new WifiP2pWfdInfo();
        when(mWifiInjector.getWifiPermissionsWrapper()).thenReturn(mWifiPermissionsWrapper);
        when(mWifiPermissionsWrapper.getUidPermission(anyString(), anyInt()))
                .thenReturn(PackageManager.PERMISSION_DENIED);
        sendSetWfdInfoMsg(mClientMessenger, mTestThisDevice.wfdInfo);
        verify(mWifiInjector).getWifiPermissionsWrapper();
        verify(mWifiPermissionsWrapper).getUidPermission(
                eq(android.Manifest.permission.CONFIGURE_WIFI_DISPLAY), anyInt());
        verify(mClientHandler).sendMessage(mMessageCaptor.capture());
        Message message = mMessageCaptor.getValue();
        assertEquals(WifiP2pManager.SET_WFD_INFO_FAILED, message.what);
        assertEquals(WifiP2pManager.ERROR, message.arg1);
    }

    /**
     * Verify the call setMiracastMode when p2p is enabled.
     */
    @Test
    public void testSetMiracastModeWhenP2pEnabled() throws Exception {
        // Move to enabled state
        forceP2pEnabled(mClient1);

        when(mWifiInjector.getWifiPermissionsWrapper()).thenReturn(mWifiPermissionsWrapper);
        when(mWifiPermissionsWrapper.getUidPermission(anyString(), anyInt()))
                .thenReturn(PackageManager.PERMISSION_GRANTED);
        mWifiP2pServiceImpl.setMiracastMode(0);
        mLooper.dispatchAll();
        verify(mContext).enforceCallingOrSelfPermission(
                eq(android.Manifest.permission.CONNECTIVITY_INTERNAL), eq("WifiP2pService"));
        verify(mWifiInjector).getWifiPermissionsWrapper();
        verify(mWifiPermissionsWrapper).getUidPermission(
                eq(android.Manifest.permission.CONFIGURE_WIFI_DISPLAY), anyInt());
        verify(mWifiNative).setMiracastMode(eq(0));
    }

    /**
     * Verify the call setMiracastMode when p2p is disable.
     */
    @Test
    public void testSetMiracastModeWhenP2pDisabled() throws Exception {
        when(mWifiInjector.getWifiPermissionsWrapper()).thenReturn(mWifiPermissionsWrapper);
        when(mWifiPermissionsWrapper.getUidPermission(anyString(), anyInt()))
                .thenReturn(PackageManager.PERMISSION_GRANTED);
        mWifiP2pServiceImpl.setMiracastMode(0);
        mLooper.dispatchAll();
        verify(mContext).enforceCallingOrSelfPermission(
                eq(android.Manifest.permission.CONNECTIVITY_INTERNAL), eq("WifiP2pService"));
        verify(mWifiInjector).getWifiPermissionsWrapper();
        verify(mWifiPermissionsWrapper).getUidPermission(
                eq(android.Manifest.permission.CONFIGURE_WIFI_DISPLAY), anyInt());
        verify(mWifiNative, never()).setMiracastMode(anyInt());
    }

    /**
     * Verify the call setMiracastMode when CONNECTIVITY_INTERNAL permission denied.
     */
    @Test(expected = SecurityException.class)
    public void testSetMiracastModeWhenPermissionDeined1() throws Exception {
        doThrow(SecurityException.class).when(mContext)
                .enforceCallingOrSelfPermission(anyString(), anyString());
        mWifiP2pServiceImpl.setMiracastMode(0);
        verify(mContext).enforceCallingOrSelfPermission(
                eq(android.Manifest.permission.CONNECTIVITY_INTERNAL), eq("WifiP2pService"));
        verify(mWifiNative, never()).setMiracastMode(anyInt());
    }

    /**
     * Verify the call setMiracastMode when CONFIGURE_WIFI_DISPLAY permission denied.
     */
    @Test(expected = SecurityException.class)
    public void testSetMiracastModeWhenPermissionDeined2() throws Exception {
        when(mWifiInjector.getWifiPermissionsWrapper()).thenReturn(mWifiPermissionsWrapper);
        when(mWifiPermissionsWrapper.getUidPermission(anyString(), anyInt()))
                .thenReturn(PackageManager.PERMISSION_DENIED);
        mWifiP2pServiceImpl.setMiracastMode(0);
        verify(mContext).enforceCallingOrSelfPermission(
                eq(android.Manifest.permission.CONNECTIVITY_INTERNAL), eq("WifiP2pService"));
        verify(mWifiInjector).getWifiPermissionsWrapper();
        verify(mWifiPermissionsWrapper).getUidPermission(
                eq(android.Manifest.permission.CONFIGURE_WIFI_DISPLAY), anyInt());
        verify(mWifiNative, never()).setMiracastMode(anyInt());
    }

    /**
     * Verify the caller sends WifiP2pManager.FACTORY_RESET when p2p is enabled.
     */
    @Test
    public void testFactoryResetSuccessWhenP2pEnabled() throws Exception {
        // Move to enabled state
        forceP2pEnabled(mClient1);

        when(mWifiInjector.getUserManager()).thenReturn(mUserManager);
        when(mPackageManager.getNameForUid(anyInt())).thenReturn("testPkg");
        when(mWifiPermissionsUtil.checkNetworkSettingsPermission(anyInt())).thenReturn(true);
        when(mUserManager.hasUserRestriction(UserManager.DISALLOW_NETWORK_RESET)).thenReturn(false);
        when(mUserManager.hasUserRestriction(UserManager.DISALLOW_CONFIG_WIFI)).thenReturn(false);
        when(mWifiNative.p2pListNetworks(any())).thenReturn(true);
        sendSimpleMsg(mClientMessenger, WifiP2pManager.FACTORY_RESET);
        checkSendP2pPersistentGroupsChangedBroadcast();
        verify(mWifiInjector).getUserManager();
        verify(mPackageManager).getNameForUid(anyInt());
        verify(mWifiPermissionsUtil).checkNetworkSettingsPermission(anyInt());
        verify(mUserManager).hasUserRestriction(eq(UserManager.DISALLOW_NETWORK_RESET));
        verify(mUserManager).hasUserRestriction(eq(UserManager.DISALLOW_CONFIG_WIFI));
        verify(mWifiNative, atLeastOnce()).p2pListNetworks(any());
        verify(mFrameworkFacade).setIntegerSetting(eq(mContext),
                eq(Settings.Global.WIFI_P2P_PENDING_FACTORY_RESET), eq(0));
        verify(mClientHandler).sendMessage(mMessageCaptor.capture());
        Message message = mMessageCaptor.getValue();
        assertEquals(WifiP2pManager.FACTORY_RESET_SUCCEEDED, message.what);
    }

    /**
     * Verify the caller sends WifiP2pManager.FACTORY_RESET when p2p is disabled at first
     * and changes to enabled.
     */
    @Test
    public void testFactoryResetSuccessWhenP2pFromDisabledToEnabled() throws Exception {
        when(mWifiInjector.getUserManager()).thenReturn(mUserManager);
        when(mPackageManager.getNameForUid(anyInt())).thenReturn("testPkg");
        when(mWifiPermissionsUtil.checkNetworkSettingsPermission(anyInt())).thenReturn(true);
        when(mUserManager.hasUserRestriction(UserManager.DISALLOW_NETWORK_RESET)).thenReturn(false);
        when(mUserManager.hasUserRestriction(UserManager.DISALLOW_CONFIG_WIFI)).thenReturn(false);
        when(mWifiNative.p2pListNetworks(any())).thenReturn(true);
        sendSimpleMsg(mClientMessenger, WifiP2pManager.FACTORY_RESET);
        verify(mWifiInjector).getUserManager();
        verify(mPackageManager).getNameForUid(anyInt());
        verify(mWifiPermissionsUtil).checkNetworkSettingsPermission(anyInt());
        verify(mUserManager).hasUserRestriction(eq(UserManager.DISALLOW_NETWORK_RESET));
        verify(mUserManager).hasUserRestriction(eq(UserManager.DISALLOW_CONFIG_WIFI));
        verify(mWifiNative, never()).p2pListNetworks(any());
        verify(mFrameworkFacade).setIntegerSetting(eq(mContext),
                eq(Settings.Global.WIFI_P2P_PENDING_FACTORY_RESET), eq(1));
        verify(mClientHandler).sendMessage(mMessageCaptor.capture());
        Message message = mMessageCaptor.getValue();
        assertEquals(WifiP2pManager.FACTORY_RESET_SUCCEEDED, message.what);

        // Move to enabled state
        when(mFrameworkFacade.getIntegerSetting(eq(mContext),
                eq(Settings.Global.WIFI_P2P_PENDING_FACTORY_RESET), anyInt())).thenReturn(1);
        forceP2pEnabled(mClient1);
        verify(mWifiInjector, times(2)).getUserManager();
        verify(mPackageManager, times(2)).getNameForUid(anyInt());
        verify(mWifiPermissionsUtil, times(2)).checkNetworkSettingsPermission(anyInt());
        verify(mUserManager, times(2)).hasUserRestriction(eq(UserManager.DISALLOW_NETWORK_RESET));
        verify(mUserManager, times(2)).hasUserRestriction(eq(UserManager.DISALLOW_CONFIG_WIFI));
        verify(mWifiNative, atLeastOnce()).p2pListNetworks(any());
        verify(mFrameworkFacade).getIntegerSetting(eq(mContext),
                eq(Settings.Global.WIFI_P2P_PENDING_FACTORY_RESET), eq(0));
        verify(mFrameworkFacade).setIntegerSetting(eq(mContext),
                eq(Settings.Global.WIFI_P2P_PENDING_FACTORY_RESET), eq(0));
        checkSendP2pPersistentGroupsChangedBroadcast();
    }

    /**
     * Verify WifiP2pManager.FACTORY_RESET_FAILED is returned without network setting permission.
     */
    @Test
    public void testFactoryResetFailureWithoutNetworkSettingPermission() throws Exception {
        // Move to enabled state
        forceP2pEnabled(mClient1);

        when(mWifiInjector.getUserManager()).thenReturn(mUserManager);
        when(mPackageManager.getNameForUid(anyInt())).thenReturn("testPkg");
        when(mWifiPermissionsUtil.checkNetworkSettingsPermission(anyInt())).thenReturn(false);
        sendSimpleMsg(mClientMessenger, WifiP2pManager.FACTORY_RESET);
        verify(mWifiInjector).getUserManager();
        verify(mPackageManager).getNameForUid(anyInt());
        verify(mWifiPermissionsUtil).checkNetworkSettingsPermission(anyInt());
        verify(mClientHandler).sendMessage(mMessageCaptor.capture());
        Message message = mMessageCaptor.getValue();
        assertEquals(WifiP2pManager.FACTORY_RESET_FAILED, message.what);
        assertEquals(WifiP2pManager.ERROR, message.arg1);
    }

    /**
     * Verify WifiP2pManager.FACTORY_RESET_FAILED is returned when network reset disallow.
     */
    @Test
    public void testFactoryResetFailureWhenNetworkResetDisallow() throws Exception {
        // Move to enabled state
        forceP2pEnabled(mClient1);

        when(mWifiInjector.getUserManager()).thenReturn(mUserManager);
        when(mPackageManager.getNameForUid(anyInt())).thenReturn("testPkg");
        when(mWifiPermissionsUtil.checkNetworkSettingsPermission(anyInt())).thenReturn(true);
        when(mUserManager.hasUserRestriction(UserManager.DISALLOW_NETWORK_RESET)).thenReturn(true);
        sendSimpleMsg(mClientMessenger, WifiP2pManager.FACTORY_RESET);
        verify(mWifiInjector).getUserManager();
        verify(mPackageManager).getNameForUid(anyInt());
        verify(mWifiPermissionsUtil).checkNetworkSettingsPermission(anyInt());
        verify(mUserManager).hasUserRestriction(eq(UserManager.DISALLOW_NETWORK_RESET));
        verify(mClientHandler).sendMessage(mMessageCaptor.capture());
        Message message = mMessageCaptor.getValue();
        assertEquals(WifiP2pManager.FACTORY_RESET_FAILED, message.what);
        assertEquals(WifiP2pManager.ERROR, message.arg1);
    }

    /**
     * Verify WifiP2pManager.FACTORY_RESET_FAILED is returned when config wifi disallow.
     */
    @Test
    public void testFactoryResetFailureWhenConfigWifiDisallow() throws Exception {
        // Move to enabled state
        forceP2pEnabled(mClient1);

        when(mWifiInjector.getUserManager()).thenReturn(mUserManager);
        when(mPackageManager.getNameForUid(anyInt())).thenReturn("testPkg");
        when(mWifiPermissionsUtil.checkNetworkSettingsPermission(anyInt())).thenReturn(true);
        when(mUserManager.hasUserRestriction(UserManager.DISALLOW_NETWORK_RESET)).thenReturn(false);
        when(mUserManager.hasUserRestriction(UserManager.DISALLOW_CONFIG_WIFI)).thenReturn(true);
        sendSimpleMsg(mClientMessenger, WifiP2pManager.FACTORY_RESET);
        verify(mWifiInjector).getUserManager();
        verify(mPackageManager).getNameForUid(anyInt());
        verify(mWifiPermissionsUtil).checkNetworkSettingsPermission(anyInt());
        verify(mUserManager).hasUserRestriction(eq(UserManager.DISALLOW_NETWORK_RESET));
        verify(mUserManager).hasUserRestriction(eq(UserManager.DISALLOW_CONFIG_WIFI));
        verify(mClientHandler).sendMessage(mMessageCaptor.capture());
        Message message = mMessageCaptor.getValue();
        assertEquals(WifiP2pManager.FACTORY_RESET_FAILED, message.what);
        assertEquals(WifiP2pManager.ERROR, message.arg1);
    }

    /**
     * Verify WifiP2pManager.FACTORY_RESET_FAILED is returned when p2p is unsupported.
     */
    @Test
    public void testFactoryResetFailureWhenP2pUnsupported() throws Exception {
        setUpWifiP2pServiceImpl(false);

        sendSimpleMsg(mClientMessenger, WifiP2pManager.FACTORY_RESET);
        verify(mClientHandler).sendMessage(mMessageCaptor.capture());
        Message message = mMessageCaptor.getValue();
        assertEquals(WifiP2pManager.FACTORY_RESET_FAILED, message.what);
        assertEquals(WifiP2pManager.P2P_UNSUPPORTED, message.arg1);
    }

    /**
     * Verify the caller sends WifiP2pManager.SET_ONGOING_PEER_CONFIG.
     */
    @Test
    public void testSetOngingPeerConfigSuccess() throws Exception {
        forceP2pEnabled(mClient1);
        mockPeersList();
        WifiP2pConfig config = new WifiP2pConfig();
        config.deviceAddress = mTestWifiP2pDevice.deviceAddress;

        when(mWifiPermissionsUtil.checkNetworkStackPermission(anyInt())).thenReturn(true);
        sendSetOngoingPeerConfigMsg(mClientMessenger, config);

        verify(mClientHandler).sendMessage(mMessageCaptor.capture());
        Message message = mMessageCaptor.getValue();
        assertEquals(WifiP2pManager.SET_ONGOING_PEER_CONFIG_SUCCEEDED, message.what);
    }

    /**
     * Verify WifiP2pManager.SET_ONGOING_PEER_CONFIG_FAILED is returned without NETWORK_STACK
     * permission.
     */
    @Test
    public void testSetOngingPeerConfigFailureWithoutPermission() throws Exception {
        forceP2pEnabled(mClient1);
        mockPeersList();
        WifiP2pConfig config = new WifiP2pConfig();

        when(mWifiPermissionsUtil.checkNetworkStackPermission(anyInt())).thenReturn(false);
        sendSetOngoingPeerConfigMsg(mClientMessenger, config);

        verify(mClientHandler).sendMessage(mMessageCaptor.capture());
        Message message = mMessageCaptor.getValue();
        assertEquals(WifiP2pManager.SET_ONGOING_PEER_CONFIG_FAILED, message.what);
    }

    /**
     * Verify WifiP2pManager.SET_ONGOING_PEER_CONFIG_FAILED is returned with invalid peer config.
     */
    @Test
    public void testSetOngoingPeerConfigFailureWithInvalidPeerConfig() throws Exception {
        forceP2pEnabled(mClient1);
        mockPeersList();
        WifiP2pConfig config = new WifiP2pConfig();

        when(mWifiPermissionsUtil.checkNetworkStackPermission(anyInt())).thenReturn(true);
        sendSetOngoingPeerConfigMsg(mClientMessenger, config);

        verify(mClientHandler).sendMessage(mMessageCaptor.capture());
        Message message = mMessageCaptor.getValue();
        assertEquals(WifiP2pManager.SET_ONGOING_PEER_CONFIG_FAILED, message.what);
    }

    /**
     * Verify that respond with RESPONSE_ONGOING_PEER_CONFIG
     * when caller sends REQUEST_ONGOING_PEER_CONFIG and permission is granted.
     */
    @Test
    public void testRequestOngoingPeerConfigSuccess() throws Exception {
        forceP2pEnabled(mClient1);

        when(mWifiPermissionsUtil.checkNetworkStackPermission(anyInt())).thenReturn(true);
        sendSimpleMsg(mClientMessenger, WifiP2pManager.REQUEST_ONGOING_PEER_CONFIG);

        verify(mClientHandler).sendMessage(mMessageCaptor.capture());
        Message message = mMessageCaptor.getValue();
        WifiP2pConfig config = (WifiP2pConfig) message.obj;
        assertEquals(WifiP2pManager.RESPONSE_ONGOING_PEER_CONFIG, message.what);
        assertNotNull(config);
    }

    /**
     * Verify that respond with RESPONSE_ONGOING_PEER_CONFIG
     * when caller sends REQUEST_ONGOING_PEER_CONFIG and has no NETWORK_STACK permission.
     */
    @Test
    public void testRequestOngoingPeerConfigFailureWithoutPermission() throws Exception {
        forceP2pEnabled(mClient1);

        when(mWifiPermissionsUtil.checkNetworkStackPermission(anyInt())).thenReturn(false);
        sendSimpleMsg(mClientMessenger, WifiP2pManager.REQUEST_ONGOING_PEER_CONFIG);

        verify(mClientHandler).sendMessage(mMessageCaptor.capture());
        Message message = mMessageCaptor.getValue();
        WifiP2pConfig config = (WifiP2pConfig) message.obj;
        assertEquals(WifiP2pManager.RESPONSE_ONGOING_PEER_CONFIG, message.what);
        assertNull(config);
    }

    /**
     * Verify that respond with RESPONSE_PERSISTENT_GROUP_INFO
     * when caller sends REQUEST_PERSISTENT_GROUP_INFO.
     */
    @Test
    public void testRequestPersistentGroupInfoSuccess() throws Exception {
        // Ensure our own MAC address is not anonymized in the result
        when(mWifiPermissionsUtil.checkLocalMacAddressPermission(anyInt())).thenReturn(true);
        forceP2pEnabled(mClient1);

        sendSimpleMsg(mClientMessenger, WifiP2pManager.REQUEST_PERSISTENT_GROUP_INFO);

        verify(mClientHandler).sendMessage(mMessageCaptor.capture());
        Message message = mMessageCaptor.getValue();
        WifiP2pGroupList groups = (WifiP2pGroupList) message.obj;
        assertEquals(WifiP2pManager.RESPONSE_PERSISTENT_GROUP_INFO, message.what);
        // WifiP2pGroupList does not implement equals operator,
        // use toString to compare two lists.
        assertEquals(mGroups.toString(), groups.toString());
    }

    /**
     * Verify that respond with RESPONSE_CONNECTION_INFO
     * when caller sends REQUEST_CONNECTION_INFO.
     */
    @Test
    public void testRequestConnectionInfoSuccess() throws Exception {
        forceP2pEnabled(mClient1);

        sendSimpleMsg(mClientMessenger, WifiP2pManager.REQUEST_CONNECTION_INFO);

        verify(mClientHandler).sendMessage(mMessageCaptor.capture());
        Message message = mMessageCaptor.getValue();
        WifiP2pInfo info = (WifiP2pInfo) message.obj;
        assertEquals(WifiP2pManager.RESPONSE_CONNECTION_INFO, message.what);
        // WifiP2pInfo does not implement equals operator,
        // use toString to compare two objects.
        assertEquals((new WifiP2pInfo()).toString(), info.toString());
    }

    /**
     * Verify that respond with RESPONSE_P2P_STATE
     * when caller sends REQUEST_P2P_STATE and p2p is enabled.
     */
    @Test
    public void testRequestP2pStateEnabled() throws Exception {
        forceP2pEnabled(mClient1);

        sendSimpleMsg(mClientMessenger, WifiP2pManager.REQUEST_P2P_STATE);

        verify(mClientHandler).sendMessage(mMessageCaptor.capture());
        Message message = mMessageCaptor.getValue();
        assertEquals(WifiP2pManager.RESPONSE_P2P_STATE, message.what);
        assertEquals(WifiP2pManager.WIFI_P2P_STATE_ENABLED, message.arg1);
    }

    /**
     * Verify that respond with RESPONSE_P2P_STATE
     * when caller sends REQUEST_P2P_STATE and p2p is disabled.
     */
    @Test
    public void testRequestP2pStateDisabled() throws Exception {
        sendSimpleMsg(mClientMessenger, WifiP2pManager.REQUEST_P2P_STATE);

        verify(mClientHandler).sendMessage(mMessageCaptor.capture());
        Message message = mMessageCaptor.getValue();
        assertEquals(WifiP2pManager.RESPONSE_P2P_STATE, message.what);
        assertEquals(WifiP2pManager.WIFI_P2P_STATE_DISABLED, message.arg1);
    }

    /**
     * Verify that respond with RESPONSE_DISCOVERY_STATE
     * when caller sends REQUEST_DISCOVERY_STATE and discovery is started.
     */
    @Test
    public void testRequestDiscoveryStateWhenStarted() throws Exception {
        forceP2pEnabled(mClient1);
        when(mWifiPermissionsUtil.checkCanAccessWifiDirect(eq("testPkg1"), anyInt(), anyBoolean()))
                .thenReturn(true);
        when(mWifiNative.p2pFind(anyInt())).thenReturn(true);
        sendChannelInfoUpdateMsg("testPkg1", mClient1, mClientMessenger);
        sendDiscoverPeersMsg(mClientMessenger);
        verify(mWifiNative).p2pFind(anyInt());
        verify(mWifiPermissionsUtil)
                .checkCanAccessWifiDirect(eq("testPkg1"), anyInt(), eq(true));

        sendSimpleMsg(mClientMessenger, WifiP2pManager.REQUEST_DISCOVERY_STATE);

        // there are 2 responses:
        // * WifiP2pManager.DISCOVER_PEERS_SUCCEEDED
        // * WifiP2pManager.RESPONSE_DISCOVERY_STATE
        verify(mClientHandler, times(2)).sendMessage(mMessageCaptor.capture());
        List<Message> messages = mMessageCaptor.getAllValues();
        assertEquals(WifiP2pManager.DISCOVER_PEERS_SUCCEEDED, messages.get(0).what);
        assertEquals(WifiP2pManager.RESPONSE_DISCOVERY_STATE, messages.get(1).what);
        assertEquals(WifiP2pManager.WIFI_P2P_DISCOVERY_STARTED, messages.get(1).arg1);
    }

    /**
     * Verify that respond with RESPONSE_DISCOVERY_STATE
     * when caller sends REQUEST_DISCOVERY_STATE and discovery is stopped.
     */
    @Test
    public void testRequestDiscoveryStateWhenStopped() throws Exception {
        forceP2pEnabled(mClient1);

        sendSimpleMsg(mClientMessenger, WifiP2pManager.REQUEST_DISCOVERY_STATE);

        verify(mClientHandler).sendMessage(mMessageCaptor.capture());
        Message message = mMessageCaptor.getValue();
        assertEquals(WifiP2pManager.RESPONSE_DISCOVERY_STATE, message.what);
        assertEquals(WifiP2pManager.WIFI_P2P_DISCOVERY_STOPPED, message.arg1);
    }

    /**
     * Verify that respond with RESPONSE_NETWORK_INFO
     * when caller sends REQUEST_NETWORK_INFO.
     */
    @Test
    public void testRequestNetworkInfoSuccess() throws Exception {
        NetworkInfo info_gold =
                new NetworkInfo(ConnectivityManager.TYPE_WIFI_P2P, 0, "WIFI_P2P", "");

        sendSimpleMsg(mClientMessenger, WifiP2pManager.REQUEST_NETWORK_INFO);

        verify(mClientHandler).sendMessage(mMessageCaptor.capture());
        Message message = mMessageCaptor.getValue();
        NetworkInfo info = (NetworkInfo) message.obj;
        assertEquals(WifiP2pManager.RESPONSE_NETWORK_INFO, message.what);
        assertEquals(info_gold.toString(), info.toString());
    }

    /**
     * Verify the caller sends WifiP2pManager.REMOVE_LOCAL_SERVICE.
     */
    @Test
    public void testRemoveLocalServiceSuccess() throws Exception {
        forceP2pEnabled(mClient1);
        testAddLocalServiceSuccess();

        sendRemoveLocalServiceMsg(mClientMessenger, mTestWifiP2pServiceInfo);
        verify(mWifiNative).p2pServiceDel(any(WifiP2pServiceInfo.class));

        assertTrue(mClientHandler.hasMessages(WifiP2pManager.REMOVE_LOCAL_SERVICE_SUCCEEDED));
    }

    /**
     * Verify the caller sends WifiP2pManager.REMOVE_LOCAL_SERVICE without client info.
     */
    @Test
    public void testRemoveLocalServiceSuccessWithoutClientInfo() throws Exception {
        forceP2pEnabled(mClient1);

        sendRemoveLocalServiceMsg(mClientMessenger, mTestWifiP2pServiceInfo);
        verify(mWifiNative, never()).p2pServiceDel(any(WifiP2pServiceInfo.class));

        assertTrue(mClientHandler.hasMessages(WifiP2pManager.REMOVE_LOCAL_SERVICE_SUCCEEDED));
    }

    /**
     * Verify the caller sends WifiP2pManager.REMOVE_LOCAL_SERVICE when service info is null.
     */
    @Test
    public void testRemoveLocalServiceSuccessWithNullServiceInfo() throws Exception {
        forceP2pEnabled(mClient1);
        testAddLocalServiceSuccess();

        sendRemoveLocalServiceMsg(mClientMessenger, null);
        verify(mWifiNative, never()).p2pServiceDel(any(WifiP2pServiceInfo.class));

        assertTrue(mClientHandler.hasMessages(WifiP2pManager.REMOVE_LOCAL_SERVICE_SUCCEEDED));
    }

    /**
     * Verify that respond with REMOVE_LOCAL_SERVICE_FAILED
     * when caller sends REMOVE_LOCAL_SERVICE and p2p is disabled.
     */
    @Test
    public void testRemoveLocalServiceFailureWhenP2pDisabled() throws Exception {
        sendRemoveLocalServiceMsg(mClientMessenger, null);
        verify(mWifiNative, never()).p2pServiceDel(any(WifiP2pServiceInfo.class));

        verify(mClientHandler).sendMessage(mMessageCaptor.capture());
        Message message = mMessageCaptor.getValue();
        assertEquals(WifiP2pManager.REMOVE_LOCAL_SERVICE_FAILED, message.what);
        assertEquals(WifiP2pManager.BUSY, message.arg1);
    }

    /**
     * Verify that respond with REMOVE_LOCAL_SERVICE_FAILED
     * when caller sends REMOVE_LOCAL_SERVICE and p2p is unsupported.
     */
    @Test
    public void testRemoveLocalServiceFailureWhenP2pUnsupported() throws Exception {
        setUpWifiP2pServiceImpl(false);

        sendRemoveLocalServiceMsg(mClientMessenger, null);
        verify(mWifiNative, never()).p2pServiceDel(any(WifiP2pServiceInfo.class));

        verify(mClientHandler).sendMessage(mMessageCaptor.capture());
        Message message = mMessageCaptor.getValue();
        assertEquals(WifiP2pManager.REMOVE_LOCAL_SERVICE_FAILED, message.what);
        assertEquals(WifiP2pManager.P2P_UNSUPPORTED, message.arg1);
    }

    /**
     * Verify the caller sends WifiP2pManager.CLEAR_LOCAL_SERVICES.
     */
    @Test
    public void testClearLocalServiceSuccess() throws Exception {
        forceP2pEnabled(mClient1);
        testAddLocalServiceSuccess();

        sendSimpleMsg(mClientMessenger, WifiP2pManager.CLEAR_LOCAL_SERVICES);
        verify(mWifiNative, atLeastOnce()).p2pServiceDel(any(WifiP2pServiceInfo.class));

        assertTrue(mClientHandler.hasMessages(WifiP2pManager.CLEAR_LOCAL_SERVICES_SUCCEEDED));
    }

    /**
     * Verify the caller sends WifiP2pManager.CLEAR_LOCAL_SERVICES without client info.
     */
    @Test
    public void testClearLocalServiceSuccessWithoutClientInfo() throws Exception {
        forceP2pEnabled(mClient1);

        sendSimpleMsg(mClientMessenger, WifiP2pManager.CLEAR_LOCAL_SERVICES);
        verify(mWifiNative, never()).p2pServiceDel(any(WifiP2pServiceInfo.class));

        assertTrue(mClientHandler.hasMessages(WifiP2pManager.CLEAR_LOCAL_SERVICES_SUCCEEDED));
    }

    /**
     * Verify that respond with CLEAR_LOCAL_SERVICES_FAILED
     * when caller sends CLEAR_LOCAL_SERVICES and p2p is disabled.
     */
    @Test
    public void testClearLocalServiceFailureWhenP2pDisabled() throws Exception {
        sendSimpleMsg(mClientMessenger, WifiP2pManager.CLEAR_LOCAL_SERVICES);
        verify(mWifiNative, never()).p2pServiceDel(any(WifiP2pServiceInfo.class));

        verify(mClientHandler).sendMessage(mMessageCaptor.capture());
        Message message = mMessageCaptor.getValue();
        assertEquals(WifiP2pManager.CLEAR_LOCAL_SERVICES_FAILED, message.what);
        assertEquals(WifiP2pManager.BUSY, message.arg1);
    }

    /**
     * Verify that respond with CLEAR_LOCAL_SERVICES_FAILED
     * when caller sends CLEAR_LOCAL_SERVICES and p2p is unsupported.
     */
    @Test
    public void testClearLocalServiceFailureWhenP2pUnsupported() throws Exception {
        setUpWifiP2pServiceImpl(false);

        sendSimpleMsg(mClientMessenger, WifiP2pManager.CLEAR_LOCAL_SERVICES);
        verify(mWifiNative, never()).p2pServiceDel(any(WifiP2pServiceInfo.class));

        verify(mClientHandler).sendMessage(mMessageCaptor.capture());
        Message message = mMessageCaptor.getValue();
        assertEquals(WifiP2pManager.CLEAR_LOCAL_SERVICES_FAILED, message.what);
        assertEquals(WifiP2pManager.P2P_UNSUPPORTED, message.arg1);
    }

    /**
     * Verify the caller sends WifiP2pManager.ADD_SERVICE_REQUEST without services discover.
     */
    @Test
    public void testAddServiceRequestSuccessWithoutServiceDiscover() throws Exception {
        forceP2pEnabled(mClient1);
        sendChannelInfoUpdateMsg("testPkg1", mClient1, mClientMessenger);

        sendAddServiceRequestMsg(mClientMessenger);

        assertTrue(mClientHandler.hasMessages(WifiP2pManager.ADD_SERVICE_REQUEST_SUCCEEDED));
    }

    /**
     * Verify the caller sends WifiP2pManager.ADD_SERVICE_REQUEST with services discover.
     */
    @Test
    public void testAddServiceRequestSuccessWithServiceDiscover() throws Exception {
        testDiscoverServicesSuccess();

        sendAddServiceRequestMsg(mClientMessenger);
        verify(mWifiNative, atLeastOnce()).p2pServDiscReq(eq("00:00:00:00:00:00"), anyString());

        assertTrue(mClientHandler.hasMessages(WifiP2pManager.ADD_SERVICE_REQUEST_SUCCEEDED));
    }

    /**
     * Verify WifiP2pManager.ADD_SERVICE_REQUEST_FAILED is returned with null request.
     */
    @Test
    public void testAddServiceRequestFailureWithNullRequest() throws Exception {
        forceP2pEnabled(mClient1);

        sendSimpleMsg(mClientMessenger, WifiP2pManager.ADD_SERVICE_REQUEST);

        assertTrue(mClientHandler.hasMessages(WifiP2pManager.ADD_SERVICE_REQUEST_FAILED));
    }

    /**
     * Verify WifiP2pManager.ADD_SERVICE_REQUEST_FAILED is returned without client info.
     */
    @Test
    public void testAddServiceRequestFailureWithoutClientInfo() throws Exception {
        forceP2pEnabled(mClient1);

        sendAddServiceRequestMsg(mClientMessenger);

        assertTrue(mClientHandler.hasMessages(WifiP2pManager.ADD_SERVICE_REQUEST_FAILED));
    }

    /**
     * Verify that respond with ADD_SERVICE_REQUEST_FAILED
     * when caller sends ADD_SERVICE_REQUEST and p2p is disabled.
     */
    @Test
    public void testAddServiceRequestFailureWhenP2pDisabled() throws Exception {
        sendAddServiceRequestMsg(mClientMessenger);

        verify(mClientHandler).sendMessage(mMessageCaptor.capture());
        Message message = mMessageCaptor.getValue();
        assertEquals(WifiP2pManager.ADD_SERVICE_REQUEST_FAILED, message.what);
        assertEquals(WifiP2pManager.BUSY, message.arg1);
    }

    /**
     * Verify that respond with ADD_SERVICE_REQUEST_FAILED
     * when caller sends ADD_SERVICE_REQUEST and p2p is unsupported.
     */
    @Test
    public void testAddServiceRequestFailureWhenP2pUnsupported() throws Exception {
        setUpWifiP2pServiceImpl(false);

        sendAddServiceRequestMsg(mClientMessenger);

        verify(mClientHandler).sendMessage(mMessageCaptor.capture());
        Message message = mMessageCaptor.getValue();
        assertEquals(WifiP2pManager.ADD_SERVICE_REQUEST_FAILED, message.what);
        assertEquals(WifiP2pManager.P2P_UNSUPPORTED, message.arg1);
    }

    /**
     * Verify the caller sends WifiP2pManager.REMOVE_SERVICE_REQUEST.
     */
    @Test
    public void testRemoveServiceRequestSuccess() throws Exception {
        forceP2pEnabled(mClient1);
        testAddServiceRequestSuccessWithoutServiceDiscover();

        sendRemoveServiceRequestMsg(mClientMessenger, mTestWifiP2pServiceRequest);

        assertTrue(mClientHandler.hasMessages(WifiP2pManager.REMOVE_SERVICE_REQUEST_SUCCEEDED));
    }

    /**
     * Verify the caller sends WifiP2pManager.REMOVE_SERVICE_REQUEST without client info.
     */
    @Test
    public void testRemoveServiceRequestSuccessWithoutClientInfo() throws Exception {
        forceP2pEnabled(mClient1);

        sendRemoveServiceRequestMsg(mClientMessenger, mTestWifiP2pServiceRequest);

        assertTrue(mClientHandler.hasMessages(WifiP2pManager.REMOVE_SERVICE_REQUEST_SUCCEEDED));
    }

    /**
     * Verify the caller sends WifiP2pManager.REMOVE_SERVICE_REQUEST when service info is null.
     */
    @Test
    public void testRemoveServiceRequestSuccessWithNullServiceInfo() throws Exception {
        forceP2pEnabled(mClient1);
        testAddLocalServiceSuccess();

        sendRemoveServiceRequestMsg(mClientMessenger, null);

        assertTrue(mClientHandler.hasMessages(WifiP2pManager.REMOVE_SERVICE_REQUEST_SUCCEEDED));
    }

    /**
     * Verify that respond with REMOVE_SERVICE_REQUEST_FAILED
     * when caller sends REMOVE_SERVICE_REQUEST and p2p is disabled.
     */
    @Test
    public void testRemoveServiceRequestFailureWhenP2pDisabled() throws Exception {
        sendRemoveServiceRequestMsg(mClientMessenger, null);

        verify(mClientHandler).sendMessage(mMessageCaptor.capture());
        Message message = mMessageCaptor.getValue();
        assertEquals(WifiP2pManager.REMOVE_SERVICE_REQUEST_FAILED, message.what);
        assertEquals(WifiP2pManager.BUSY, message.arg1);
    }

    /**
     * Verify that respond with REMOVE_SERVICE_REQUEST_FAILED
     * when caller sends REMOVE_SERVICE_REQUEST and p2p is unsupported.
     */
    @Test
    public void testRemoveServiceRequestFailureWhenP2pUnsupported() throws Exception {
        setUpWifiP2pServiceImpl(false);

        sendRemoveServiceRequestMsg(mClientMessenger, null);

        verify(mClientHandler).sendMessage(mMessageCaptor.capture());
        Message message = mMessageCaptor.getValue();
        assertEquals(WifiP2pManager.REMOVE_SERVICE_REQUEST_FAILED, message.what);
        assertEquals(WifiP2pManager.P2P_UNSUPPORTED, message.arg1);
    }

    /**
     * Verify the caller sends WifiP2pManager.CLEAR_SERVICE_REQUESTS.
     */
    @Test
    public void testClearServiceRequestsSuccess() throws Exception {
        forceP2pEnabled(mClient1);
        testAddServiceRequestSuccessWithoutServiceDiscover();

        sendSimpleMsg(mClientMessenger, WifiP2pManager.CLEAR_SERVICE_REQUESTS);

        assertTrue(mClientHandler.hasMessages(WifiP2pManager.CLEAR_SERVICE_REQUESTS_SUCCEEDED));
    }

    /**
     * Verify the caller sends WifiP2pManager.CLEAR_SERVICE_REQUESTS without client info.
     */
    @Test
    public void testClearServiceRequestsSuccessWithoutClientInfo() throws Exception {
        forceP2pEnabled(mClient1);

        sendSimpleMsg(mClientMessenger, WifiP2pManager.CLEAR_SERVICE_REQUESTS);

        assertTrue(mClientHandler.hasMessages(WifiP2pManager.CLEAR_SERVICE_REQUESTS_SUCCEEDED));
    }

    /**
     * Verify that respond with CLEAR_SERVICE_REQUESTS_FAILED
     * when caller sends CLEAR_SERVICE_REQUEST and p2p is disabled.
     */
    @Test
    public void testClearServiceRequestsFailureWhenP2pDisabled() throws Exception {
        sendSimpleMsg(mClientMessenger, WifiP2pManager.CLEAR_SERVICE_REQUESTS);

        verify(mClientHandler).sendMessage(mMessageCaptor.capture());
        Message message = mMessageCaptor.getValue();
        assertEquals(WifiP2pManager.CLEAR_SERVICE_REQUESTS_FAILED, message.what);
        assertEquals(WifiP2pManager.BUSY, message.arg1);
    }

    /**
     * Verify that respond with CLEAR_SERVICE_REQUESTS_FAILED
     * when caller sends CLEAR_SERVICE_REQUEST and p2p is unsupported.
     */
    @Test
    public void testClearServiceRequestsFailureWhenP2pUnsupported() throws Exception {
        setUpWifiP2pServiceImpl(false);

        sendSimpleMsg(mClientMessenger, WifiP2pManager.CLEAR_SERVICE_REQUESTS);

        verify(mClientHandler).sendMessage(mMessageCaptor.capture());
        Message message = mMessageCaptor.getValue();
        assertEquals(WifiP2pManager.CLEAR_SERVICE_REQUESTS_FAILED, message.what);
        assertEquals(WifiP2pManager.P2P_UNSUPPORTED, message.arg1);
    }

    /**
     * Verify stopping discovery is executed when location mode is turned off.
     */
    @Test
    public void testStopDiscoveryWhenLocationModeIsDisabled() throws Exception {
        forceP2pEnabled(mClient1);
        simulateLocationModeChange(false);
        mLooper.dispatchAll();
        verify(mWifiNative).p2pStopFind();
    }

    /**
     * Verify a network name which is too long is rejected.
     */
    @Test
    public void testSendConnectMsgWithTooLongNetworkName() throws Exception {
        mTestWifiP2pFastConnectionConfig.networkName = "DIRECT-xy-abcdefghijklmnopqrstuvw";
        sendConnectMsg(mClientMessenger, mTestWifiP2pFastConnectionConfig);
        verify(mClientHandler).sendMessage(mMessageCaptor.capture());
        Message message = mMessageCaptor.getValue();
        assertEquals(WifiP2pManager.CONNECT_FAILED, message.what);
    }

    /**
     * Verify a network name which is too short is rejected.
     */
    @Test
    public void testSendConnectMsgWithTooShortNetworkName() throws Exception {
        mTestWifiP2pFastConnectionConfig.networkName = "DIRECT-x";
        sendConnectMsg(mClientMessenger, mTestWifiP2pFastConnectionConfig);
        verify(mClientHandler).sendMessage(mMessageCaptor.capture());
        Message message = mMessageCaptor.getValue();
        assertEquals(WifiP2pManager.CONNECT_FAILED, message.what);
    }
}
