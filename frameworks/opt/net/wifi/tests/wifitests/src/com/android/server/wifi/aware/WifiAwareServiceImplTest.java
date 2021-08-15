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

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;
import static org.mockito.ArgumentMatchers.any;
import static org.mockito.ArgumentMatchers.anyBoolean;
import static org.mockito.ArgumentMatchers.anyInt;
import static org.mockito.ArgumentMatchers.anyString;
import static org.mockito.ArgumentMatchers.eq;
import static org.mockito.Mockito.doNothing;
import static org.mockito.Mockito.doThrow;
import static org.mockito.Mockito.inOrder;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;

import android.Manifest;
import android.app.AppOpsManager;
import android.content.Context;
import android.content.pm.PackageManager;
import android.net.wifi.aware.Characteristics;
import android.net.wifi.aware.ConfigRequest;
import android.net.wifi.aware.IWifiAwareDiscoverySessionCallback;
import android.net.wifi.aware.IWifiAwareEventCallback;
import android.net.wifi.aware.IWifiAwareMacAddressProvider;
import android.net.wifi.aware.PublishConfig;
import android.net.wifi.aware.SubscribeConfig;
import android.os.HandlerThread;
import android.os.IBinder;
import android.os.RemoteException;
import android.os.test.TestLooper;
import android.util.SparseArray;
import android.util.SparseIntArray;

import androidx.test.filters.SmallTest;

import com.android.server.wifi.FrameworkFacade;
import com.android.server.wifi.util.WifiPermissionsUtil;
import com.android.server.wifi.util.WifiPermissionsWrapper;

import org.junit.Before;
import org.junit.Test;
import org.mockito.ArgumentCaptor;
import org.mockito.InOrder;
import org.mockito.Mock;
import org.mockito.MockitoAnnotations;

import java.lang.reflect.Field;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;


/**
 * Unit test harness for WifiAwareStateManager.
 */
@SmallTest
public class WifiAwareServiceImplTest {
    private static final int MAX_LENGTH = 255;

    private WifiAwareServiceImplSpy mDut;
    private int mDefaultUid = 1500;
    private String mPackageName = "some.package";
    private TestLooper mMockLooper;

    @Mock
    private Context mContextMock;
    @Mock
    private HandlerThread mHandlerThreadMock;
    @Mock
    private PackageManager mPackageManagerMock;
    @Mock
    private WifiAwareStateManager mAwareStateManagerMock;
    @Mock
    private WifiAwareShellCommand mWifiAwareShellCommandMock;
    @Mock
    private IBinder mBinderMock;
    @Mock
    private IWifiAwareEventCallback mCallbackMock;
    @Mock
    private IWifiAwareDiscoverySessionCallback mSessionCallbackMock;
    @Mock private WifiAwareMetrics mAwareMetricsMock;
    @Mock private WifiPermissionsUtil mWifiPermissionsUtil;
    @Mock private WifiPermissionsWrapper mPermissionsWrapperMock;
    @Mock
    FrameworkFacade mFrameworkFacade;

    /**
     * Using instead of spy to avoid native crash failures - possibly due to
     * spy's copying of state.
     */
    private class WifiAwareServiceImplSpy extends WifiAwareServiceImpl {
        public int fakeUid;

        WifiAwareServiceImplSpy(Context context) {
            super(context);
        }

        /**
         * Return the fake UID instead of the real one: pseudo-spy
         * implementation.
         */
        @Override
        public int getMockableCallingUid() {
            return fakeUid;
        }
    }

    /**
     * Initializes mocks.
     */
    @Before
    public void setup() throws Exception {
        MockitoAnnotations.initMocks(this);
        mMockLooper = new TestLooper();

        when(mHandlerThreadMock.getLooper()).thenReturn(mMockLooper.getLooper());
        doNothing().when(mFrameworkFacade).registerContentObserver(eq(mContextMock), any(),
                anyBoolean(), any());

        AppOpsManager appOpsMock = mock(AppOpsManager.class);
        when(mContextMock.getSystemService(Context.APP_OPS_SERVICE)).thenReturn(appOpsMock);

        when(mContextMock.getApplicationContext()).thenReturn(mContextMock);
        when(mContextMock.getPackageManager()).thenReturn(mPackageManagerMock);
        when(mPackageManagerMock.hasSystemFeature(PackageManager.FEATURE_WIFI_AWARE))
                .thenReturn(true);
        when(mPackageManagerMock.hasSystemFeature(PackageManager.FEATURE_WIFI_RTT))
                .thenReturn(true);
        when(mAwareStateManagerMock.getCharacteristics()).thenReturn(getCharacteristics());

        mDut = new WifiAwareServiceImplSpy(mContextMock);
        mDut.fakeUid = mDefaultUid;
        mDut.start(mHandlerThreadMock, mAwareStateManagerMock, mWifiAwareShellCommandMock,
                mAwareMetricsMock, mWifiPermissionsUtil, mPermissionsWrapperMock, mFrameworkFacade,
                mock(WifiAwareNativeManager.class), mock(WifiAwareNativeApi.class),
                mock(WifiAwareNativeCallback.class));
        verify(mAwareStateManagerMock).start(eq(mContextMock), any(), eq(mAwareMetricsMock),
                eq(mWifiPermissionsUtil), eq(mPermissionsWrapperMock), any());
    }

    /**
     * Validate isUsageEnabled() function
     */
    @Test
    public void testIsUsageEnabled() {
        mDut.isUsageEnabled();

        verify(mAwareStateManagerMock).isUsageEnabled();
    }


    /**
     * Validate connect() - returns and uses a client ID.
     */
    @Test
    public void testConnect() {
        doConnect();
    }

    /**
     * Validate connect() when a non-null config is passed.
     */
    @Test
    public void testConnectWithConfig() {
        ConfigRequest configRequest = new ConfigRequest.Builder().setMasterPreference(55).build();
        String callingPackage = "com.google.somePackage";

        mDut.connect(mBinderMock, callingPackage, mCallbackMock,
                configRequest, false);

        verify(mAwareStateManagerMock).connect(anyInt(), anyInt(), anyInt(),
                eq(callingPackage), eq(mCallbackMock), eq(configRequest), eq(false));
    }

    /**
     * Validate disconnect() - correct pass-through args.
     *
     * @throws Exception
     */
    @Test
    public void testDisconnect() throws Exception {
        int clientId = doConnect();

        mDut.disconnect(clientId, mBinderMock);

        verify(mAwareStateManagerMock).disconnect(clientId);
        validateInternalStateCleanedUp(clientId);
    }

    /**
     * Validate that security exception thrown when attempting operation using
     * an invalid client ID.
     */
    @Test(expected = SecurityException.class)
    public void testFailOnInvalidClientId() {
        mDut.disconnect(-1, mBinderMock);
    }

    /**
     * Validate that security exception thrown when attempting operation using a
     * client ID which was already cleared-up.
     */
    @Test(expected = SecurityException.class)
    public void testFailOnClearedUpClientId() throws Exception {
        int clientId = doConnect();

        mDut.disconnect(clientId, mBinderMock);

        verify(mAwareStateManagerMock).disconnect(clientId);
        validateInternalStateCleanedUp(clientId);

        mDut.disconnect(clientId, mBinderMock);
    }

    /**
     * Validate that trying to use a client ID from a UID which is different
     * from the one that created it fails - and that the internal state is not
     * modified so that a valid call (from the correct UID) will subsequently
     * succeed.
     */
    @Test
    public void testFailOnAccessClientIdFromWrongUid() throws Exception {
        int clientId = doConnect();

        mDut.fakeUid = mDefaultUid + 1;

        /*
         * Not using thrown.expect(...) since want to test that subsequent
         * access works.
         */
        boolean failsAsExpected = false;
        try {
            mDut.disconnect(clientId, mBinderMock);
        } catch (SecurityException e) {
            failsAsExpected = true;
        }

        mDut.fakeUid = mDefaultUid;

        PublishConfig publishConfig = new PublishConfig.Builder().setServiceName("valid.value")
                .build();
        mDut.publish(mPackageName, clientId, publishConfig, mSessionCallbackMock);

        verify(mAwareStateManagerMock).publish(clientId, publishConfig, mSessionCallbackMock);
        assertTrue("SecurityException for invalid access from wrong UID thrown", failsAsExpected);
    }

    /**
     * Validate that the RTT feature support is checked when attempting a Publish with ranging.
     */
    @Test(expected = IllegalArgumentException.class)
    public void testFailOnPublishRangingWithoutRttFeature() throws Exception {
        when(mPackageManagerMock.hasSystemFeature(PackageManager.FEATURE_WIFI_RTT)).thenReturn(
                false);

        PublishConfig publishConfig = new PublishConfig.Builder().setServiceName("something.valid")
                .setRangingEnabled(true).build();
        int clientId = doConnect();
        IWifiAwareDiscoverySessionCallback mockCallback = mock(
                IWifiAwareDiscoverySessionCallback.class);

        mDut.publish(mPackageName, clientId, publishConfig, mockCallback);
    }

    /**
     * Validate that the RTT feature support is checked when attempting a Subscribe with ranging.
     */
    @Test(expected = IllegalArgumentException.class)
    public void testFailOnSubscribeRangingWithoutRttFeature() throws Exception {
        when(mPackageManagerMock.hasSystemFeature(PackageManager.FEATURE_WIFI_RTT)).thenReturn(
                false);

        SubscribeConfig subscribeConfig = new SubscribeConfig.Builder().setServiceName(
                "something.valid").setMaxDistanceMm(100).build();
        int clientId = doConnect();
        IWifiAwareDiscoverySessionCallback mockCallback = mock(
                IWifiAwareDiscoverySessionCallback.class);

        mDut.subscribe(mPackageName, clientId, subscribeConfig, mockCallback);
    }


    /**
     * Validates that on binder death we get a disconnect().
     */
    @Test
    public void testBinderDeath() throws Exception {
        ArgumentCaptor<IBinder.DeathRecipient> deathRecipient = ArgumentCaptor
                .forClass(IBinder.DeathRecipient.class);

        int clientId = doConnect();

        verify(mBinderMock).linkToDeath(deathRecipient.capture(), eq(0));
        deathRecipient.getValue().binderDied();
        verify(mAwareStateManagerMock).disconnect(clientId);
        validateInternalStateCleanedUp(clientId);
    }

    /**
     * Validates that sequential connect() calls return increasing client IDs.
     */
    @Test
    public void testClientIdIncrementing() {
        int loopCount = 100;

        InOrder inOrder = inOrder(mAwareStateManagerMock);
        ArgumentCaptor<Integer> clientIdCaptor = ArgumentCaptor.forClass(Integer.class);

        int prevId = 0;
        for (int i = 0; i < loopCount; ++i) {
            mDut.connect(mBinderMock, "", mCallbackMock, null, false);
            inOrder.verify(mAwareStateManagerMock).connect(clientIdCaptor.capture(), anyInt(),
                    anyInt(), any(), eq(mCallbackMock), any(), eq(false));
            int id = clientIdCaptor.getValue();
            if (i != 0) {
                assertTrue("Client ID incrementing", id > prevId);
            }
            prevId = id;
        }
    }

    /**
     * Validate terminateSession() - correct pass-through args.
     */
    @Test
    public void testTerminateSession() {
        int sessionId = 1024;
        int clientId = doConnect();

        mDut.terminateSession(clientId, sessionId);

        verify(mAwareStateManagerMock).terminateSession(clientId, sessionId);
    }

    /**
     * Validate publish() - correct pass-through args.
     */
    @Test
    public void testPublish() {
        PublishConfig publishConfig = new PublishConfig.Builder().setServiceName("something.valid")
                .setRangingEnabled(true).build();
        int clientId = doConnect();
        IWifiAwareDiscoverySessionCallback mockCallback = mock(
                IWifiAwareDiscoverySessionCallback.class);

        mDut.publish(mPackageName, clientId, publishConfig, mockCallback);

        verify(mAwareStateManagerMock).publish(clientId, publishConfig, mockCallback);
    }

    /**
     * Validate that publish() verifies the input PublishConfig and fails on an invalid service
     * name.
     */
    @Test(expected = IllegalArgumentException.class)
    public void testPublishInvalidServiceName() {
        doBadPublishConfiguration("Including invalid characters - spaces", null, null);
    }

    /**
     * Validate that publish() verifies the input PublishConfig and fails on a "very long"
     * service name.
     */
    @Test(expected = IllegalArgumentException.class)
    public void testPublishServiceNameTooLong() {
        byte[] byteArray = new byte[MAX_LENGTH + 1];
        for (int i = 0; i < MAX_LENGTH + 1; ++i) {
            byteArray[i] = 'a';
        }
        doBadPublishConfiguration(new String(byteArray), null, null);
    }

    /**
     * Validate that publish() verifies the input PublishConfig and fails on a "very long" ssi.
     */
    @Test(expected = IllegalArgumentException.class)
    public void testPublishSsiTooLong() {
        doBadPublishConfiguration("correctservicename", new byte[MAX_LENGTH + 1], null);
    }

    /**
     * Validate that publish() verifies the input PublishConfig and fails on a "very long" match
     * filter.
     */
    @Test(expected = IllegalArgumentException.class)
    public void testPublishMatchFilterTooLong() {
        doBadPublishConfiguration("correctservicename", null, new byte[MAX_LENGTH + 1]);
    }

    /**
     * Validate that publish() verifies the input PublishConfig and fails on a bad match filter -
     * invalid LV.
     */
    @Test(expected = IllegalArgumentException.class)
    public void testPublishMatchFilterBadLv() {
        byte[] badLv = { 0, 1, 127, 2, 126, 125, 3 };
        doBadPublishConfiguration("correctservicename", null, badLv);
    }

    /**
     * Validate updatePublish() - correct pass-through args.
     */
    @Test
    public void testUpdatePublish() {
        int sessionId = 1232;
        PublishConfig publishConfig = new PublishConfig.Builder().setServiceName("something.valid")
                .build();
        int clientId = doConnect();

        mDut.updatePublish(clientId, sessionId, publishConfig);

        verify(mAwareStateManagerMock).updatePublish(clientId, sessionId, publishConfig);
    }

    /**
     * Validate updatePublish() error checking.
     */
    @Test(expected = IllegalArgumentException.class)
    public void testUpdatePublishInvalid() {
        int sessionId = 1232;
        PublishConfig publishConfig = new PublishConfig.Builder()
                .setServiceName("something with spaces").build();
        int clientId = doConnect();

        mDut.updatePublish(clientId, sessionId, publishConfig);

        verify(mAwareStateManagerMock).updatePublish(clientId, sessionId, publishConfig);
    }

    /**
     * Validate subscribe() - correct pass-through args.
     */
    @Test
    public void testSubscribe() {
        SubscribeConfig subscribeConfig = new SubscribeConfig.Builder()
                .setServiceName("something.valid").setMaxDistanceMm(100).build();
        int clientId = doConnect();
        IWifiAwareDiscoverySessionCallback mockCallback = mock(
                IWifiAwareDiscoverySessionCallback.class);

        mDut.subscribe(mPackageName, clientId, subscribeConfig, mockCallback);

        verify(mAwareStateManagerMock).subscribe(clientId, subscribeConfig, mockCallback);
    }

    /**
     * Validate that subscribe() verifies the input SubscribeConfig and fails on an invalid service
     * name.
     */
    @Test(expected = IllegalArgumentException.class)
    public void testSubscribeInvalidServiceName() {
        doBadSubscribeConfiguration("Including invalid characters - spaces", null, null);
    }

    /**
     * Validate that subscribe() verifies the input SubscribeConfig and fails on a "very long"
     * service name.
     */
    @Test(expected = IllegalArgumentException.class)
    public void testSubscribeServiceNameTooLong() {
        byte[] byteArray = new byte[MAX_LENGTH + 1];
        for (int i = 0; i < MAX_LENGTH + 1; ++i) {
            byteArray[i] = 'a';
        }
        doBadSubscribeConfiguration(new String(byteArray), null, null);
    }

    /**
     * Validate that subscribe() verifies the input SubscribeConfig and fails on a "very long" ssi.
     */
    @Test(expected = IllegalArgumentException.class)
    public void testSubscribeSsiTooLong() {
        doBadSubscribeConfiguration("correctservicename", new byte[MAX_LENGTH + 1], null);
    }

    /**
     * Validate that subscribe() verifies the input SubscribeConfig and fails on a "very long" match
     * filter.
     */
    @Test(expected = IllegalArgumentException.class)
    public void testSubscribeMatchFilterTooLong() {
        doBadSubscribeConfiguration("correctservicename", null, new byte[MAX_LENGTH + 1]);
    }

    /**
     * Validate that subscribe() verifies the input SubscribeConfig and fails on a bad match filter
     * - invalid LV.
     */
    @Test(expected = IllegalArgumentException.class)
    public void testSubscribeMatchFilterBadLv() {
        byte[] badLv = { 0, 1, 127, 2, 126, 125, 3 };
        doBadSubscribeConfiguration("correctservicename", null, badLv);
    }

    /**
     * Validate updateSubscribe() error checking.
     */
    @Test(expected = IllegalArgumentException.class)
    public void testUpdateSubscribeInvalid() {
        int sessionId = 1232;
        SubscribeConfig subscribeConfig = new SubscribeConfig.Builder()
                .setServiceName("something.valid")
                .setServiceSpecificInfo(new byte[MAX_LENGTH + 1]).build();
        int clientId = doConnect();

        mDut.updateSubscribe(clientId, sessionId, subscribeConfig);

        verify(mAwareStateManagerMock).updateSubscribe(clientId, sessionId, subscribeConfig);
    }

    /**
     * Validate updateSubscribe() validates configuration.
     */
    @Test
    public void testUpdateSubscribe() {
        int sessionId = 1232;
        SubscribeConfig subscribeConfig = new SubscribeConfig.Builder()
                .setServiceName("something.valid").build();
        int clientId = doConnect();

        mDut.updateSubscribe(clientId, sessionId, subscribeConfig);

        verify(mAwareStateManagerMock).updateSubscribe(clientId, sessionId, subscribeConfig);
    }

    /**
     * Validate sendMessage() - correct pass-through args.
     */
    @Test
    public void testSendMessage() {
        int sessionId = 2394;
        int peerId = 2032;
        byte[] message = new byte[MAX_LENGTH];
        int messageId = 2043;
        int clientId = doConnect();

        mDut.sendMessage(clientId, sessionId, peerId, message, messageId, 0);

        verify(mAwareStateManagerMock).sendMessage(clientId, sessionId, peerId, message, messageId,
                0);
    }

    /**
     * Validate sendMessage() validates that message length is correct.
     */
    @Test(expected = IllegalArgumentException.class)
    public void testSendMessageTooLong() {
        int sessionId = 2394;
        int peerId = 2032;
        byte[] message = new byte[MAX_LENGTH + 1];
        int messageId = 2043;
        int clientId = doConnect();

        mDut.sendMessage(clientId, sessionId, peerId, message, messageId, 0);

        verify(mAwareStateManagerMock).sendMessage(clientId, sessionId, peerId, message, messageId,
                0);
    }

    @Test
    public void testRequestMacAddress() {
        int uid = 1005;
        List<Integer> list = new ArrayList<>();
        IWifiAwareMacAddressProvider callback = new IWifiAwareMacAddressProvider() { // dummy
            @Override
            public void macAddress(Map peerIdToMacMap) throws RemoteException {
                // empty
            }

            @Override
            public IBinder asBinder() {
                return null;
            }
        };

        mDut.requestMacAddresses(uid, list, callback);

        verify(mAwareStateManagerMock).requestMacAddresses(uid, list, callback);
    }

    @Test(expected = SecurityException.class)
    public void testRequestMacAddressWithoutPermission() {
        doThrow(new SecurityException()).when(mContextMock).enforceCallingOrSelfPermission(
                eq(Manifest.permission.NETWORK_STACK), anyString());

        mDut.requestMacAddresses(1005, new ArrayList<>(), new IWifiAwareMacAddressProvider() {
            @Override
            public void macAddress(Map peerIdToMacMap) throws RemoteException {
            }

            @Override
            public IBinder asBinder() {
                return null;
            }
        });
    }

    /*
     * Utilities
     */

    /*
     * Tests of internal state of WifiAwareServiceImpl: very limited (not usually
     * a good idea). However, these test that the internal state is cleaned-up
     * appropriately. Alternatively would cause issues with memory leaks or
     * information leak between sessions.
     */
    private void validateInternalStateCleanedUp(int clientId) throws Exception {
        int uidEntry = getInternalStateUid(clientId);
        assertEquals(-1, uidEntry);

        IBinder.DeathRecipient dr = getInternalStateDeathRecipient(clientId);
        assertEquals(null, dr);
    }

    private void doBadPublishConfiguration(String serviceName, byte[] ssi, byte[] matchFilter)
            throws IllegalArgumentException {
        // using the hidden constructor since may be passing invalid parameters which would be
        // caught by the Builder. Want to test whether service side will catch invalidly
        // constructed configs.
        PublishConfig publishConfig = new PublishConfig(serviceName.getBytes(), ssi, matchFilter,
                PublishConfig.PUBLISH_TYPE_UNSOLICITED, 0, true, false);
        int clientId = doConnect();
        IWifiAwareDiscoverySessionCallback mockCallback = mock(
                IWifiAwareDiscoverySessionCallback.class);

        mDut.publish(mPackageName, clientId, publishConfig, mockCallback);

        verify(mAwareStateManagerMock).publish(clientId, publishConfig, mockCallback);
    }

    private void doBadSubscribeConfiguration(String serviceName, byte[] ssi, byte[] matchFilter)
            throws IllegalArgumentException {
        // using the hidden constructor since may be passing invalid parameters which would be
        // caught by the Builder. Want to test whether service side will catch invalidly
        // constructed configs.
        SubscribeConfig subscribeConfig = new SubscribeConfig(serviceName.getBytes(), ssi,
                matchFilter, SubscribeConfig.SUBSCRIBE_TYPE_PASSIVE, 0, true, false, 0, false, 0);
        int clientId = doConnect();
        IWifiAwareDiscoverySessionCallback mockCallback = mock(
                IWifiAwareDiscoverySessionCallback.class);

        mDut.subscribe(mPackageName, clientId, subscribeConfig, mockCallback);

        verify(mAwareStateManagerMock).subscribe(clientId, subscribeConfig, mockCallback);
    }

    private int doConnect() {
        String callingPackage = "com.google.somePackage";

        mDut.connect(mBinderMock, callingPackage, mCallbackMock, null, false);

        ArgumentCaptor<Integer> clientId = ArgumentCaptor.forClass(Integer.class);
        verify(mAwareStateManagerMock).connect(clientId.capture(), anyInt(), anyInt(),
                eq(callingPackage), eq(mCallbackMock), eq(new ConfigRequest.Builder().build()),
                eq(false));

        return clientId.getValue();
    }

    private static Characteristics getCharacteristics() {
        Capabilities cap = new Capabilities();
        cap.maxConcurrentAwareClusters = 1;
        cap.maxPublishes = 2;
        cap.maxSubscribes = 2;
        cap.maxServiceNameLen = MAX_LENGTH;
        cap.maxMatchFilterLen = MAX_LENGTH;
        cap.maxTotalMatchFilterLen = 255;
        cap.maxServiceSpecificInfoLen = MAX_LENGTH;
        cap.maxExtendedServiceSpecificInfoLen = MAX_LENGTH;
        cap.maxNdiInterfaces = 1;
        cap.maxNdpSessions = 1;
        cap.maxAppInfoLen = 255;
        cap.maxQueuedTransmitMessages = 6;
        return cap.toPublicCharacteristics();
    }

    private int getInternalStateUid(int clientId) throws Exception {
        Field field = WifiAwareServiceImpl.class.getDeclaredField("mUidByClientId");
        field.setAccessible(true);
        @SuppressWarnings("unchecked")
        SparseIntArray uidByClientId = (SparseIntArray) field.get(mDut);

        return uidByClientId.get(clientId, -1);
    }

    private IBinder.DeathRecipient getInternalStateDeathRecipient(int clientId) throws Exception {
        Field field = WifiAwareServiceImpl.class.getDeclaredField("mDeathRecipientsByClientId");
        field.setAccessible(true);
        @SuppressWarnings("unchecked")
        SparseArray<IBinder.DeathRecipient> deathRecipientsByClientId =
                            (SparseArray<IBinder.DeathRecipient>) field.get(mDut);

        return deathRecipientsByClientId.get(clientId);
    }
}
