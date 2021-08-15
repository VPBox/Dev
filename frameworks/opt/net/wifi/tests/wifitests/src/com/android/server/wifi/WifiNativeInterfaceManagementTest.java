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
 * limitations under the License
 */

package com.android.server.wifi;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertNull;
import static org.junit.Assert.assertTrue;
import static org.mockito.ArgumentMatchers.any;
import static org.mockito.ArgumentMatchers.anyBoolean;
import static org.mockito.Mockito.atLeastOnce;
import static org.mockito.Mockito.doAnswer;
import static org.mockito.Mockito.doNothing;
import static org.mockito.Mockito.doThrow;
import static org.mockito.Mockito.eq;
import static org.mockito.Mockito.inOrder;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.verifyNoMoreInteractions;
import static org.mockito.Mockito.when;

import android.app.test.MockAnswerUtil;
import android.net.InterfaceConfiguration;
import android.net.wifi.IApInterface;
import android.net.wifi.IClientInterface;
import android.net.wifi.WifiConfiguration;
import android.net.wifi.WifiScanner;
import android.os.Handler;
import android.os.INetworkManagementService;
import android.os.RemoteException;
import android.os.test.TestLooper;

import androidx.test.filters.SmallTest;

import com.android.server.net.BaseNetworkObserver;
import com.android.server.wifi.HalDeviceManager.InterfaceDestroyedListener;
import com.android.server.wifi.WifiNative.SupplicantDeathEventHandler;
import com.android.server.wifi.WifiNative.VendorHalDeathEventHandler;
import com.android.server.wifi.WifiNative.WificondDeathEventHandler;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.mockito.ArgumentCaptor;
import org.mockito.InOrder;
import org.mockito.Mock;
import org.mockito.MockitoAnnotations;

/**
 * Unit tests for the interface management operations in
 * {@link com.android.server.wifi.WifiNative}.
 */
@SmallTest
public class WifiNativeInterfaceManagementTest {
    private static final String IFACE_NAME_0 = "mockWlan0";
    private static final String IFACE_NAME_1 = "mockWlan1";

    @Mock private WifiVendorHal mWifiVendorHal;
    @Mock private WificondControl mWificondControl;
    @Mock private SupplicantStaIfaceHal mSupplicantStaIfaceHal;
    @Mock private HostapdHal mHostapdHal;
    @Mock private WifiMonitor mWifiMonitor;
    @Mock private INetworkManagementService mNwManagementService;
    @Mock private PropertyService mPropertyService;
    @Mock private WifiMetrics mWifiMetrics;

    @Mock private WifiNative.StatusListener mStatusListener;
    @Mock private WifiNative.InterfaceCallback mIfaceCallback0;
    @Mock private WifiNative.InterfaceCallback mIfaceCallback1;
    private TestLooper mLooper;
    private final InterfaceConfiguration mInterfaceConfiguration = new InterfaceConfiguration();

    private ArgumentCaptor<VendorHalDeathEventHandler> mWifiVendorHalDeathHandlerCaptor =
            ArgumentCaptor.forClass(VendorHalDeathEventHandler.class);
    private ArgumentCaptor<WificondDeathEventHandler> mWificondDeathHandlerCaptor =
            ArgumentCaptor.forClass(WificondDeathEventHandler.class);
    private ArgumentCaptor<WifiNative.VendorHalRadioModeChangeEventHandler>
            mWifiVendorHalRadioModeChangeHandlerCaptor =
            ArgumentCaptor.forClass(WifiNative.VendorHalRadioModeChangeEventHandler.class);
    private ArgumentCaptor<SupplicantDeathEventHandler> mSupplicantDeathHandlerCaptor =
            ArgumentCaptor.forClass(SupplicantDeathEventHandler.class);
    private ArgumentCaptor<WifiNative.HostapdDeathEventHandler> mHostapdDeathHandlerCaptor =
            ArgumentCaptor.forClass(WifiNative.HostapdDeathEventHandler.class);
    private ArgumentCaptor<BaseNetworkObserver> mNetworkObserverCaptor0 =
            ArgumentCaptor.forClass(BaseNetworkObserver.class);
    private ArgumentCaptor<BaseNetworkObserver> mNetworkObserverCaptor1 =
            ArgumentCaptor.forClass(BaseNetworkObserver.class);
    private ArgumentCaptor<InterfaceDestroyedListener> mIfaceDestroyedListenerCaptor0 =
            ArgumentCaptor.forClass(InterfaceDestroyedListener.class);
    private ArgumentCaptor<InterfaceDestroyedListener> mIfaceDestroyedListenerCaptor1 =
            ArgumentCaptor.forClass(InterfaceDestroyedListener.class);
    private InOrder mInOrder;

    private WifiNative mWifiNative;

    @Before
    public void setUp() throws Exception {
        MockitoAnnotations.initMocks(this);
        mLooper = new TestLooper();

        // Setup mocks for the positive single interface cases, individual tests can modify the
        // mocks for negative or multi-interface tests.
        when(mWifiVendorHal.initialize(mWifiVendorHalDeathHandlerCaptor.capture()))
            .thenReturn(true);
        doNothing().when(mWifiVendorHal).registerRadioModeChangeHandler(
                mWifiVendorHalRadioModeChangeHandlerCaptor.capture());
        when(mWifiVendorHal.isVendorHalSupported()).thenReturn(true);
        when(mWifiVendorHal.startVendorHal()).thenReturn(true);
        when(mWifiVendorHal.createStaIface(anyBoolean(), any())).thenReturn(IFACE_NAME_0);
        when(mWifiVendorHal.createApIface(any())).thenReturn(IFACE_NAME_0);
        when(mWifiVendorHal.removeStaIface(any())).thenReturn(true);
        when(mWifiVendorHal.removeApIface(any())).thenReturn(true);

        when(mWificondControl.initialize(mWificondDeathHandlerCaptor.capture()))
            .thenReturn(true);
        when(mWificondControl.setupInterfaceForClientMode(any()))
            .thenReturn(mock(IClientInterface.class));
        when(mWificondControl.setupInterfaceForSoftApMode(any()))
            .thenReturn(mock(IApInterface.class));
        when(mWificondControl.tearDownClientInterface(any())).thenReturn(true);
        when(mWificondControl.tearDownSoftApInterface(any())).thenReturn(true);
        when(mWificondControl.tearDownInterfaces()).thenReturn(true);
        when(mWificondControl.registerApListener(any(), any())).thenReturn(true);

        when(mSupplicantStaIfaceHal.registerDeathHandler(mSupplicantDeathHandlerCaptor.capture()))
            .thenReturn(true);
        when(mSupplicantStaIfaceHal.deregisterDeathHandler()).thenReturn(true);
        when(mSupplicantStaIfaceHal.initialize()).thenReturn(true);
        when(mSupplicantStaIfaceHal.isInitializationStarted()).thenReturn(false);
        when(mSupplicantStaIfaceHal.isInitializationComplete()).thenReturn(true);
        when(mSupplicantStaIfaceHal.startDaemon()).thenReturn(true);
        when(mSupplicantStaIfaceHal.setupIface(any())).thenReturn(true);
        when(mSupplicantStaIfaceHal.teardownIface(any())).thenReturn(true);

        when(mHostapdHal.registerDeathHandler(mHostapdDeathHandlerCaptor.capture()))
                .thenReturn(true);
        when(mHostapdHal.deregisterDeathHandler()).thenReturn(true);
        when(mHostapdHal.initialize()).thenReturn(true);
        when(mHostapdHal.isInitializationStarted()).thenReturn(false);
        when(mHostapdHal.isInitializationComplete()).thenReturn(true);
        when(mHostapdHal.startDaemon()).thenReturn(true);
        when(mHostapdHal.addAccessPoint(any(), any(), any())).thenReturn(true);
        when(mHostapdHal.removeAccessPoint(any())).thenReturn(true);

        when(mNwManagementService.getInterfaceConfig(IFACE_NAME_0))
                .thenReturn(mInterfaceConfiguration);

        mInOrder = inOrder(mWifiVendorHal, mWificondControl, mSupplicantStaIfaceHal, mHostapdHal,
                mWifiMonitor, mNwManagementService, mIfaceCallback0, mIfaceCallback1, mWifiMetrics);

        mWifiNative = new WifiNative(
                mWifiVendorHal, mSupplicantStaIfaceHal, mHostapdHal, mWificondControl,
                mWifiMonitor, mNwManagementService, mPropertyService, mWifiMetrics,
                new Handler(mLooper.getLooper()), null);
        mWifiNative.initialize();
        mWifiNative.registerStatusListener(mStatusListener);

        mInOrder.verify(mWifiVendorHal).initialize(any());
        mInOrder.verify(mWificondControl).initialize(any());
        mInOrder.verify(mWifiVendorHal).registerRadioModeChangeHandler(any());
    }

    @After
    public void tearDown() throws Exception {
        verifyNoMoreInteractions(mWifiVendorHal, mWificondControl, mSupplicantStaIfaceHal,
                mHostapdHal, mWifiMonitor, mNwManagementService, mIfaceCallback0, mIfaceCallback1,
                mWifiMetrics);
    }

    /**
     * Verifies the setup of a single client interface.
     */
    @Test
    public void testSetupClientInterface() throws Exception {
        executeAndValidateSetupClientInterface(
                false, false, IFACE_NAME_0, mIfaceCallback0, mIfaceDestroyedListenerCaptor0,
                mNetworkObserverCaptor0);
        assertEquals(IFACE_NAME_0, mWifiNative.getClientInterfaceName());
    }

    /**
     * Verifies the setup of a single client interface (for scan).
     */
    @Test
    public void testSetupClientInterfaceForScan() throws Exception {
        executeAndValidateSetupClientInterfaceForScan(
                false, false, IFACE_NAME_0, mIfaceCallback0, mIfaceDestroyedListenerCaptor0,
                mNetworkObserverCaptor0);
        assertEquals(IFACE_NAME_0, mWifiNative.getClientInterfaceName());
        verifyNoMoreInteractions(mWifiVendorHal, mWificondControl, mSupplicantStaIfaceHal,
                mHostapdHal, mNwManagementService, mIfaceCallback0, mIfaceCallback1, mWifiMetrics);
    }

    /**
     * Verifies the setup of a single softAp interface.
     */
    @Test
    public void testSetupSoftApInterface() throws Exception {
        executeAndValidateSetupSoftApInterface(
                false, false, IFACE_NAME_0, mIfaceCallback0, mIfaceDestroyedListenerCaptor0,
                mNetworkObserverCaptor0);
        assertNull(mWifiNative.getClientInterfaceName());
    }

    /**
     * Verifies the setup & teardown of a single client interface.
     */
    @Test
    public void testSetupAndTeardownClientInterface() throws Exception {
        executeAndValidateSetupClientInterface(
                false, false, IFACE_NAME_0, mIfaceCallback0, mIfaceDestroyedListenerCaptor0,
                mNetworkObserverCaptor0);
        assertEquals(IFACE_NAME_0, mWifiNative.getClientInterfaceName());
        executeAndValidateTeardownClientInterface(false, false, IFACE_NAME_0, mIfaceCallback0,
                mIfaceDestroyedListenerCaptor0.getValue(), mNetworkObserverCaptor0.getValue());
    }

    /**
     * Verifies the setup & teardown of a single client interface (for scan).
     */
    @Test
    public void testSetupAndTeardownClientInterfaceForScan() throws Exception {
        executeAndValidateSetupClientInterfaceForScan(
                false, false, IFACE_NAME_0, mIfaceCallback0, mIfaceDestroyedListenerCaptor0,
                mNetworkObserverCaptor0);
        assertEquals(IFACE_NAME_0, mWifiNative.getClientInterfaceName());
        executeAndValidateTeardownClientInterfaceForScan(false, false, IFACE_NAME_0,
                mIfaceCallback0, mIfaceDestroyedListenerCaptor0.getValue(),
                mNetworkObserverCaptor0.getValue());
        verifyNoMoreInteractions(mWifiVendorHal, mWificondControl, mSupplicantStaIfaceHal,
                mHostapdHal, mNwManagementService, mIfaceCallback0, mIfaceCallback1, mWifiMetrics);
    }

    /**
     * Verifies the setup & teardown of a single softAp interface.
     */
    @Test
    public void testSetupAndTeardownSoftApInterface() throws Exception {
        executeAndValidateSetupSoftApInterface(
                false, false, IFACE_NAME_0, mIfaceCallback0, mIfaceDestroyedListenerCaptor0,
                mNetworkObserverCaptor0);
        assertNull(mWifiNative.getClientInterfaceName());
        executeAndValidateTeardownSoftApInterface(false, false, IFACE_NAME_0, mIfaceCallback0,
                mIfaceDestroyedListenerCaptor0.getValue(), mNetworkObserverCaptor0.getValue());
    }

    /**
     * Verifies the setup & teardown of a client & softAp interface.
     *
     * Sequence tested:
     * a) Setup client interface.
     * b) Setup softAp interface.
     * c) Teardown client interface.
     * d) Teardown softAp interface.
     */
    @Test
    public void testSetupAndTeardownClientAndSoftApInterface_Seq1() throws Exception {
        executeAndValidateSetupClientInterface(
                false, false, IFACE_NAME_0, mIfaceCallback0, mIfaceDestroyedListenerCaptor0,
                mNetworkObserverCaptor0);
        executeAndValidateSetupSoftApInterface(
                true, false, IFACE_NAME_1, mIfaceCallback1, mIfaceDestroyedListenerCaptor1,
                mNetworkObserverCaptor1);
        assertEquals(IFACE_NAME_0, mWifiNative.getClientInterfaceName());
        executeAndValidateTeardownClientInterface(false, true, IFACE_NAME_0, mIfaceCallback0,
                mIfaceDestroyedListenerCaptor0.getValue(), mNetworkObserverCaptor0.getValue());
        executeAndValidateTeardownSoftApInterface(false, false, IFACE_NAME_1, mIfaceCallback1,
                mIfaceDestroyedListenerCaptor1.getValue(), mNetworkObserverCaptor1.getValue());
    }

    /**
     * Verifies the setup & teardown of a client & softAp interface.
     *
     * Sequence tested:
     * a) Setup client interface.
     * b) Setup softAp interface.
     * c) Teardown softAp interface.
     * d) Teardown client interface.
     */
    @Test
    public void testSetupAndTeardownClientAndSoftApInterface_Seq2() throws Exception {
        executeAndValidateSetupClientInterface(
                false, false, IFACE_NAME_0, mIfaceCallback0, mIfaceDestroyedListenerCaptor0,
                mNetworkObserverCaptor0);
        executeAndValidateSetupSoftApInterface(
                true, false, IFACE_NAME_1, mIfaceCallback1, mIfaceDestroyedListenerCaptor1,
                mNetworkObserverCaptor1);
        assertEquals(IFACE_NAME_0, mWifiNative.getClientInterfaceName());
        executeAndValidateTeardownSoftApInterface(true, false, IFACE_NAME_1, mIfaceCallback1,
                mIfaceDestroyedListenerCaptor1.getValue(), mNetworkObserverCaptor1.getValue());
        executeAndValidateTeardownClientInterface(false, false, IFACE_NAME_0, mIfaceCallback0,
                mIfaceDestroyedListenerCaptor0.getValue(), mNetworkObserverCaptor0.getValue());
    }

    /**
     * Verifies the setup & teardown of a client & softAp interface.
     *
     * Sequence tested:
     * a) Setup softAp interface.
     * b) Setup client interface.
     * c) Teardown softAp interface.
     * d) Teardown client interface.
     */
    @Test
    public void testSetupAndTeardownClientAndSoftApInterface_Seq3() throws Exception {
        executeAndValidateSetupSoftApInterface(
                false, false, IFACE_NAME_0, mIfaceCallback0, mIfaceDestroyedListenerCaptor0,
                mNetworkObserverCaptor0);
        executeAndValidateSetupClientInterface(
                false, true, IFACE_NAME_1, mIfaceCallback1, mIfaceDestroyedListenerCaptor1,
                mNetworkObserverCaptor1);
        assertEquals(IFACE_NAME_1, mWifiNative.getClientInterfaceName());
        executeAndValidateTeardownSoftApInterface(true, false, IFACE_NAME_0, mIfaceCallback0,
                mIfaceDestroyedListenerCaptor0.getValue(), mNetworkObserverCaptor0.getValue());
        executeAndValidateTeardownClientInterface(false, false, IFACE_NAME_1, mIfaceCallback1,
                mIfaceDestroyedListenerCaptor1.getValue(), mNetworkObserverCaptor1.getValue());
    }

    /**
     * Verifies the setup & teardown of a client & softAp interface.
     *
     * Sequence tested:
     * a) Setup softAp interface.
     * b) Setup client interface.
     * c) Teardown client interface.
     * d) Teardown softAp interface.
     */
    @Test
    public void testSetupAndTeardownClientAndSoftApInterface_Seq4() throws Exception {
        executeAndValidateSetupSoftApInterface(
                false, false, IFACE_NAME_0, mIfaceCallback0, mIfaceDestroyedListenerCaptor0,
                mNetworkObserverCaptor0);
        executeAndValidateSetupClientInterface(
                false, true, IFACE_NAME_1, mIfaceCallback1, mIfaceDestroyedListenerCaptor1,
                mNetworkObserverCaptor1);
        assertEquals(IFACE_NAME_1, mWifiNative.getClientInterfaceName());
        executeAndValidateTeardownClientInterface(false, true, IFACE_NAME_1, mIfaceCallback1,
                mIfaceDestroyedListenerCaptor1.getValue(), mNetworkObserverCaptor1.getValue());
        executeAndValidateTeardownSoftApInterface(false, false, IFACE_NAME_0, mIfaceCallback0,
                mIfaceDestroyedListenerCaptor0.getValue(), mNetworkObserverCaptor0.getValue());
    }

    /**
     * Verifies the setup of a client & softAp interface & then initiate teardown of all active
     * interfaces.
     *
     * Sequence tested:
     * a) Setup softAp interface.
     * b) Setup client interface.
     * c) Teardown all active interfaces.
     */
    @Test
    public void testTeardownAllInterfaces() throws Exception {
        executeAndValidateSetupSoftApInterface(
                false, false, IFACE_NAME_0, mIfaceCallback0, mIfaceDestroyedListenerCaptor0,
                mNetworkObserverCaptor0);
        executeAndValidateSetupClientInterface(
                false, true, IFACE_NAME_1, mIfaceCallback1, mIfaceDestroyedListenerCaptor1,
                mNetworkObserverCaptor1);

        // Assert that a client & softap interface is present.
        assertNotNull(mWifiNative.getClientInterfaceName());
        assertNotNull(mWifiNative.getSoftApInterfaceName());

        mWifiNative.teardownAllInterfaces();

        // Note: This is not using InOrder because order of interface deletion cannot be
        // predetermined.

        // Verify STA removal
        verify(mWifiMonitor).stopMonitoring(IFACE_NAME_1);
        verify(mNwManagementService).unregisterObserver(mNetworkObserverCaptor1.getValue());
        verify(mSupplicantStaIfaceHal).teardownIface(IFACE_NAME_1);
        verify(mWificondControl).tearDownClientInterface(IFACE_NAME_1);
        verify(mSupplicantStaIfaceHal).deregisterDeathHandler();
        verify(mSupplicantStaIfaceHal).terminate();
        verify(mIfaceCallback1).onDestroyed(IFACE_NAME_1);

        // Verify AP removal
        verify(mNwManagementService).unregisterObserver(mNetworkObserverCaptor0.getValue());
        verify(mHostapdHal).removeAccessPoint(IFACE_NAME_0);
        verify(mWificondControl).tearDownSoftApInterface(IFACE_NAME_0);
        verify(mHostapdHal).deregisterDeathHandler();
        verify(mHostapdHal).terminate();

        // Verify we stopped HAL & wificond
        verify(mWificondControl).tearDownInterfaces();
        verify(mWifiVendorHal).stopVendorHal();
        verify(mIfaceCallback0).onDestroyed(IFACE_NAME_0);

        verify(mWifiVendorHal, atLeastOnce()).isVendorHalSupported();

        // Assert that the client & softap interface is no more there.
        assertNull(mWifiNative.getClientInterfaceName());
        assertNull(mWifiNative.getSoftApInterfaceName());
    }

    /**
     * Verifies the setup of a client interface and then a SoftAp interface which would
     * destroy the Client interface. This is what would happen on older devices which do not
     * support concurrent interfaces.
     */
    @Test
    public void testSetupClientAndSoftApInterfaceCausesClientInterfaceTeardown() throws Exception {
        executeAndValidateSetupClientInterface(
                false, false, IFACE_NAME_0, mIfaceCallback0, mIfaceDestroyedListenerCaptor0,
                mNetworkObserverCaptor0);

        // Trigger the STA interface teardown when AP interface is created.
        // The iface name will remain the same.
        doAnswer(new MockAnswerUtil.AnswerWithArguments() {
            public String answer(InterfaceDestroyedListener destroyedListener) {
                mIfaceDestroyedListenerCaptor0.getValue().onDestroyed(IFACE_NAME_0);
                return IFACE_NAME_0;
            }
        }).when(mWifiVendorHal).createApIface(any());

        assertEquals(IFACE_NAME_0, mWifiNative.setupInterfaceForSoftApMode(mIfaceCallback1));

        mInOrder.verify(mHostapdHal).isInitializationStarted();
        mInOrder.verify(mHostapdHal).initialize();
        mInOrder.verify(mHostapdHal).startDaemon();
        mInOrder.verify(mHostapdHal).isInitializationComplete();
        mInOrder.verify(mHostapdHal).registerDeathHandler(any());
        mInOrder.verify(mWifiVendorHal).isVendorHalSupported();
        mInOrder.verify(mWifiVendorHal).createApIface(mIfaceDestroyedListenerCaptor1.capture());
        // Creation of AP interface should trigger the STA interface destroy
        validateOnDestroyedClientInterface(
                false, true, IFACE_NAME_0, mIfaceCallback0, mNetworkObserverCaptor0.getValue());
        // Now continue with rest of AP interface setup.
        mInOrder.verify(mWificondControl).setupInterfaceForSoftApMode(IFACE_NAME_0);
        mInOrder.verify(mNwManagementService).registerObserver(mNetworkObserverCaptor1.capture());
        mInOrder.verify(mNwManagementService).getInterfaceConfig(IFACE_NAME_0);
        mInOrder.verify(mSupplicantStaIfaceHal).getAdvancedKeyMgmtCapabilities(IFACE_NAME_0);
        mInOrder.verify(mWifiVendorHal).getSupportedFeatureSet(IFACE_NAME_0);

        // Execute a teardown of the interface to ensure that the new iface removal works.
        executeAndValidateTeardownSoftApInterface(false, false, IFACE_NAME_0, mIfaceCallback1,
                mIfaceDestroyedListenerCaptor1.getValue(), mNetworkObserverCaptor1.getValue());
    }

    /**
     * Verifies the setup of a client interface and then a SoftAp interface which would
     * destroy the Client interface. This is what would happen on older devices which do not
     * support concurrent interfaces.
     */
    @Test
    public void testSetupSoftApAndClientInterfaceCausesSoftApInterfaceTeardown() throws Exception {
        executeAndValidateSetupSoftApInterface(
                false, false, IFACE_NAME_0, mIfaceCallback0, mIfaceDestroyedListenerCaptor0,
                mNetworkObserverCaptor0);

        // Trigger the AP interface teardown when STA interface is created.
        // The iface name will remain the same.
        doAnswer(new MockAnswerUtil.AnswerWithArguments() {
            public String answer(boolean lowPrioritySta,
                    InterfaceDestroyedListener destroyedListener) {
                mIfaceDestroyedListenerCaptor0.getValue().onDestroyed(IFACE_NAME_0);
                return IFACE_NAME_0;
            }
        }).when(mWifiVendorHal).createStaIface(anyBoolean(), any());

        assertEquals(IFACE_NAME_0,
                mWifiNative.setupInterfaceForClientInConnectivityMode(mIfaceCallback1));

        mInOrder.verify(mSupplicantStaIfaceHal).isInitializationStarted();
        mInOrder.verify(mSupplicantStaIfaceHal).initialize();
        mInOrder.verify(mSupplicantStaIfaceHal).startDaemon();
        mInOrder.verify(mSupplicantStaIfaceHal).isInitializationComplete();
        mInOrder.verify(mSupplicantStaIfaceHal).registerDeathHandler(any());
        mInOrder.verify(mWifiVendorHal).isVendorHalSupported();
        mInOrder.verify(mWifiVendorHal).createStaIface(eq(false),
                mIfaceDestroyedListenerCaptor1.capture());
        // Creation of STA interface should trigger the AP interface destroy.
        validateOnDestroyedSoftApInterface(
                true, false, IFACE_NAME_0, mIfaceCallback0, mNetworkObserverCaptor0.getValue());
        // Now continue with rest of STA interface setup.
        mInOrder.verify(mWificondControl).setupInterfaceForClientMode(IFACE_NAME_0);
        mInOrder.verify(mSupplicantStaIfaceHal).setupIface(IFACE_NAME_0);
        mInOrder.verify(mNwManagementService).registerObserver(mNetworkObserverCaptor1.capture());
        mInOrder.verify(mWifiMonitor).startMonitoring(IFACE_NAME_0);
        mInOrder.verify(mNwManagementService).getInterfaceConfig(IFACE_NAME_0);
        mInOrder.verify(mNwManagementService).clearInterfaceAddresses(IFACE_NAME_0);
        mInOrder.verify(mNwManagementService).setInterfaceIpv6PrivacyExtensions(IFACE_NAME_0, true);
        mInOrder.verify(mNwManagementService).disableIpv6(IFACE_NAME_0);
        mInOrder.verify(mSupplicantStaIfaceHal).getAdvancedKeyMgmtCapabilities(IFACE_NAME_0);
        mInOrder.verify(mWifiVendorHal).getSupportedFeatureSet(IFACE_NAME_0);

        // Execute a teardown of the interface to ensure that the new iface removal works.
        executeAndValidateTeardownClientInterface(false, false, IFACE_NAME_0, mIfaceCallback1,
                mIfaceDestroyedListenerCaptor1.getValue(), mNetworkObserverCaptor1.getValue());
    }

    /**
     * Verifies the setup of a client interface and trigger an interface down event.
     * This should be ignored since interface is considered to be down before setup.
     */
    @Test
    public void testSetupClientInterfaceAndTriggerInterfaceDown() throws Exception {
        executeAndValidateSetupSoftApInterface(
                false, false, IFACE_NAME_0, mIfaceCallback0, mIfaceDestroyedListenerCaptor0,
                mNetworkObserverCaptor0);

        executeAndValidateInterfaceStateChange(
                IFACE_NAME_0, false, mNetworkObserverCaptor0.getValue());
    }

    /**
     * Verifies the setup of a client interface and trigger an interface up event.
     */
    @Test
    public void testSetupClientInterfaceAndTriggerInterfaceUp() throws Exception {
        executeAndValidateSetupSoftApInterface(
                false, false, IFACE_NAME_0, mIfaceCallback0, mIfaceDestroyedListenerCaptor0,
                mNetworkObserverCaptor0);

        executeAndValidateInterfaceStateChange(
                IFACE_NAME_0, true, mNetworkObserverCaptor0.getValue());
        mInOrder.verify(mIfaceCallback0).onUp(IFACE_NAME_0);
    }

    /**
     * Verifies the setup of a client interface and trigger an interface up event, followed by a
     * down event.
     */
    @Test
    public void testSetupClientInterfaceAndTriggerInterfaceUpFollowedByDown() throws Exception {
        executeAndValidateSetupClientInterface(
                false, false, IFACE_NAME_0, mIfaceCallback0, mIfaceDestroyedListenerCaptor0,
                mNetworkObserverCaptor0);

        executeAndValidateInterfaceStateChange(
                IFACE_NAME_0, true, mNetworkObserverCaptor0.getValue());
        mInOrder.verify(mIfaceCallback0).onUp(IFACE_NAME_0);

        executeAndValidateInterfaceStateChange(
                IFACE_NAME_0, false, mNetworkObserverCaptor0.getValue());
        mInOrder.verify(mIfaceCallback0).onDown(IFACE_NAME_0);
        mInOrder.verify(mWifiMetrics).incrementNumClientInterfaceDown();
    }

    /**
     * Verifies the setup of a softap interface and trigger an interface up event, followed by a
     * down event.
     */
    @Test
    public void testSetupSoftApInterfaceAndTriggerInterfaceUpFollowedByDown() throws Exception {
        executeAndValidateSetupSoftApInterface(
                false, false, IFACE_NAME_0, mIfaceCallback0, mIfaceDestroyedListenerCaptor0,
                mNetworkObserverCaptor0);

        executeAndValidateInterfaceStateChange(
                IFACE_NAME_0, true, mNetworkObserverCaptor0.getValue());
        mInOrder.verify(mIfaceCallback0).onUp(IFACE_NAME_0);

        executeAndValidateInterfaceStateChange(
                IFACE_NAME_0, false, mNetworkObserverCaptor0.getValue());
        mInOrder.verify(mIfaceCallback0).onDown(IFACE_NAME_0);
        mInOrder.verify(mWifiMetrics).incrementNumSoftApInterfaceDown();
    }

    /**
     * Verifies the setup of a client interface and trigger an interface up event, followed by
     * link down/up events. The link state change events should be ignored since we only care for
     * interface state changes.
     */
    @Test
    public void testSetupClientInterfaceAndTriggerInterfaceUpFollowedByLinkDownAndUp()
            throws Exception {
        executeAndValidateSetupClientInterface(
                false, false, IFACE_NAME_0, mIfaceCallback0, mIfaceDestroyedListenerCaptor0,
                mNetworkObserverCaptor0);

        executeAndValidateInterfaceStateChange(
                IFACE_NAME_0, true, mNetworkObserverCaptor0.getValue());
        mInOrder.verify(mIfaceCallback0).onUp(IFACE_NAME_0);

        // Trigger a link down, with the interface still up.
        // Should not trigger the external iface callback.
        mNetworkObserverCaptor0.getValue().interfaceLinkStateChanged(IFACE_NAME_0, false);
        mLooper.dispatchAll();
        mInOrder.verify(mNwManagementService).getInterfaceConfig(IFACE_NAME_0);

        // Now trigger a link up, with the interface still up.
        // Should not trigger the external iface callback.
        mNetworkObserverCaptor0.getValue().interfaceLinkStateChanged(IFACE_NAME_0, true);
        mLooper.dispatchAll();
        mInOrder.verify(mNwManagementService).getInterfaceConfig(IFACE_NAME_0);
    }

    /**
     * Verifies the setup of a client interface and trigger an interface up event, followed by
     * link down/up events. The link state change events should be ignored since we only care for
     * interface state changes.
     */
    @Test
    public void testSetupSoftApInterfaceAndTriggerInterfaceUpFollowedByLinkDownAndUp()
            throws Exception {
        executeAndValidateSetupSoftApInterface(
                false, false, IFACE_NAME_0, mIfaceCallback0, mIfaceDestroyedListenerCaptor0,
                mNetworkObserverCaptor0);

        executeAndValidateInterfaceStateChange(
                IFACE_NAME_0, true, mNetworkObserverCaptor0.getValue());
        mInOrder.verify(mIfaceCallback0).onUp(IFACE_NAME_0);

        // Trigger a link down, with the interface still up.
        // Should not trigger the external iface callback.
        mNetworkObserverCaptor0.getValue().interfaceLinkStateChanged(IFACE_NAME_0, false);
        mLooper.dispatchAll();
        mInOrder.verify(mNwManagementService).getInterfaceConfig(IFACE_NAME_0);

        // Now trigger a link up, with the interface still up.
        // Should not trigger the external iface callback.
        mNetworkObserverCaptor0.getValue().interfaceLinkStateChanged(IFACE_NAME_0, true);
        mLooper.dispatchAll();
        mInOrder.verify(mNwManagementService).getInterfaceConfig(IFACE_NAME_0);
    }

    /**
     * Verifies the setup of a client interface and trigger an interface up event twice.
     * The second interface up event should be masked.
     */
    @Test
    public void testSetupClientInterfaceAndTriggerInterfaceUpTwice() throws Exception {
        executeAndValidateSetupSoftApInterface(
                false, false, IFACE_NAME_0, mIfaceCallback0, mIfaceDestroyedListenerCaptor0,
                mNetworkObserverCaptor0);

        executeAndValidateInterfaceStateChange(
                IFACE_NAME_0, true, mNetworkObserverCaptor0.getValue());
        mInOrder.verify(mIfaceCallback0).onUp(IFACE_NAME_0);

        executeAndValidateInterfaceStateChange(
                IFACE_NAME_0, true, mNetworkObserverCaptor0.getValue());
    }

    /**
     * Verifies the setup of a client interface and trigger an interface up event on a different
     * interface.
     */
    @Test
    public void testSetupClientInterfaceAndTriggerInterfaceUpOnAnInvalidIface() throws Exception {
        executeAndValidateSetupSoftApInterface(
                false, false, IFACE_NAME_0, mIfaceCallback0, mIfaceDestroyedListenerCaptor0,
                mNetworkObserverCaptor0);

        mNetworkObserverCaptor0.getValue().interfaceLinkStateChanged(IFACE_NAME_1, true);
        mLooper.dispatchAll();
    }

    /**
     * Verifies that interface down on a destroyed interface is ignored.
     * The test triggers
     * a) Setup of a client interface
     * b) Setup of a SoftAp interface which would destroy the Client interface.
     * This is what would happen on older devices which do not support concurrent interfaces.
     * c) Once the client interface is destroyed, trigger an interface up event on the old
     * network observer. This should be ignored.
     * d) Trigger an interface down event on the new network observer. This should trigger an
     * interface up event to external clients.
     * e) Remove the new SoftAp interface.
     */
    @Test
    public void testSetupClientInterfaceAndTriggerInterfaceUpOnDestroyedIface() throws Exception {
        // Step (a)
        executeAndValidateSetupClientInterface(
                false, false, IFACE_NAME_0, mIfaceCallback0, mIfaceDestroyedListenerCaptor0,
                mNetworkObserverCaptor0);

        // Step (b)
        // Trigger the STA interface teardown when AP interface is created.
        // The iface name will remain the same.
        doAnswer(new MockAnswerUtil.AnswerWithArguments() {
            public String answer(InterfaceDestroyedListener destroyedListener) {
                mIfaceDestroyedListenerCaptor0.getValue().onDestroyed(IFACE_NAME_0);
                return IFACE_NAME_0;
            }
        }).when(mWifiVendorHal).createApIface(any());

        assertEquals(IFACE_NAME_0, mWifiNative.setupInterfaceForSoftApMode(mIfaceCallback1));

        mInOrder.verify(mHostapdHal).isInitializationStarted();
        mInOrder.verify(mHostapdHal).initialize();
        mInOrder.verify(mHostapdHal).startDaemon();
        mInOrder.verify(mHostapdHal).isInitializationComplete();
        mInOrder.verify(mHostapdHal).registerDeathHandler(any());
        mInOrder.verify(mWifiVendorHal).isVendorHalSupported();
        mInOrder.verify(mWifiVendorHal).createApIface(mIfaceDestroyedListenerCaptor1.capture());
        // Creation of AP interface should trigger the STA interface destroy
        validateOnDestroyedClientInterface(
                false, true, IFACE_NAME_0, mIfaceCallback0, mNetworkObserverCaptor0.getValue());
        // Now continue with rest of AP interface setup.
        mInOrder.verify(mWificondControl).setupInterfaceForSoftApMode(IFACE_NAME_0);
        mInOrder.verify(mNwManagementService).registerObserver(mNetworkObserverCaptor1.capture());
        mInOrder.verify(mNwManagementService).getInterfaceConfig(IFACE_NAME_0);
        mInOrder.verify(mSupplicantStaIfaceHal).getAdvancedKeyMgmtCapabilities(IFACE_NAME_0);
        mInOrder.verify(mWifiVendorHal).getSupportedFeatureSet(IFACE_NAME_0);

        // Step (c) - Iface up on old iface, ignored!
        mNetworkObserverCaptor0.getValue().interfaceLinkStateChanged(IFACE_NAME_0, true);
        mLooper.dispatchAll();

        // Step (d) - Iface up on new iface, handled!
        executeAndValidateInterfaceStateChange(
                IFACE_NAME_0, true, mNetworkObserverCaptor1.getValue());
        mInOrder.verify(mIfaceCallback1).onUp(IFACE_NAME_0);

        // Execute a teardown of the softap interface to ensure that the new iface removal works.
        executeAndValidateTeardownSoftApInterface(false, false, IFACE_NAME_0, mIfaceCallback1,
                mIfaceDestroyedListenerCaptor1.getValue(), mNetworkObserverCaptor1.getValue());
    }

    /**
     * Verifies the setup of a client interface and wificond death handling.
     */
    @Test
    public void testSetupClientInterfaceAndWicondDied() throws Exception {
        executeAndValidateSetupClientInterface(
                false, false, IFACE_NAME_0, mIfaceCallback0, mIfaceDestroyedListenerCaptor0,
                mNetworkObserverCaptor0);
        // Trigger wificond death
        mWificondDeathHandlerCaptor.getValue().onDeath();

        mInOrder.verify(mWifiMetrics).incrementNumWificondCrashes();

        verify(mStatusListener).onStatusChanged(false);
        verify(mStatusListener).onStatusChanged(true);
    }

    /**
     * Verifies the setup of a soft ap interface and vendor HAL death handling.
     */
    @Test
    public void testSetupSoftApInterfaceAndVendorHalDied() throws Exception {
        executeAndValidateSetupSoftApInterface(
                false, false, IFACE_NAME_0, mIfaceCallback0, mIfaceDestroyedListenerCaptor0,
                mNetworkObserverCaptor0);

        // Trigger vendor HAL death
        mWifiVendorHalDeathHandlerCaptor.getValue().onDeath();

        mInOrder.verify(mWifiMetrics).incrementNumHalCrashes();

        verify(mStatusListener).onStatusChanged(false);
        verify(mStatusListener).onStatusChanged(true);
    }

    /**
     * Verifies the setup of a client interface and supplicant HAL death handling.
     */
    @Test
    public void testSetupClientInterfaceAndSupplicantDied() throws Exception {
        executeAndValidateSetupClientInterface(
                false, false, IFACE_NAME_0, mIfaceCallback0, mIfaceDestroyedListenerCaptor0,
                mNetworkObserverCaptor0);
        // Trigger wificond death
        mSupplicantDeathHandlerCaptor.getValue().onDeath();

        mInOrder.verify(mWifiMetrics).incrementNumSupplicantCrashes();

        verify(mStatusListener).onStatusChanged(false);
        verify(mStatusListener).onStatusChanged(true);
    }

    /**
     * Verifies the setup of a soft ap interface and hostapd death handling.
     */
    @Test
    public void testStartSoftApAndHostapdDied() throws Exception {
        executeAndValidateSetupSoftApInterface(
                false, false, IFACE_NAME_0, mIfaceCallback0, mIfaceDestroyedListenerCaptor0,
                mNetworkObserverCaptor0);

        // Start softap
        assertTrue(mWifiNative.startSoftAp(IFACE_NAME_0, new WifiConfiguration(),
                mock(WifiNative.SoftApListener.class)));

        mInOrder.verify(mWificondControl).registerApListener(any(), any());
        mInOrder.verify(mHostapdHal).addAccessPoint(any(), any(), any());

        // Trigger vendor HAL death
        mHostapdDeathHandlerCaptor.getValue().onDeath();

        mInOrder.verify(mWifiMetrics).incrementNumHostapdCrashes();

        verify(mStatusListener).onStatusChanged(false);
        verify(mStatusListener).onStatusChanged(true);
    }

    /**
     * Verifies failure handling in setup of a client interface.
     */
    @Test
    public void testSetupClientInterfaceFailureInStartHal() throws Exception {
        when(mWifiVendorHal.startVendorHal()).thenReturn(false);
        assertNull(mWifiNative.setupInterfaceForClientInConnectivityMode(mIfaceCallback0));

        mInOrder.verify(mWifiVendorHal).isVendorHalSupported();
        mInOrder.verify(mWifiVendorHal).startVendorHal();
        mInOrder.verify(mWifiMetrics).incrementNumSetupClientInterfaceFailureDueToHal();

        // To test if the failure is handled cleanly, invoke teardown and ensure that
        // none of the mocks are used because the iface does not exist in the internal
        // database.
        mWifiNative.teardownInterface(IFACE_NAME_0);
    }

    /**
     * Verifies failure handling in setup of a client interface.
     */
    @Test
    public void testSetupClientInterfaceFailureInStartSupplicant() throws Exception {
        when(mSupplicantStaIfaceHal.startDaemon()).thenReturn(false);
        assertNull(mWifiNative.setupInterfaceForClientInConnectivityMode(mIfaceCallback0));

        mInOrder.verify(mWifiVendorHal).isVendorHalSupported();
        mInOrder.verify(mWifiVendorHal).startVendorHal();
        mInOrder.verify(mSupplicantStaIfaceHal).isInitializationStarted();
        mInOrder.verify(mSupplicantStaIfaceHal).initialize();
        mInOrder.verify(mSupplicantStaIfaceHal).startDaemon();
        mInOrder.verify(mWifiMetrics).incrementNumSetupClientInterfaceFailureDueToSupplicant();

        // To test if the failure is handled cleanly, invoke teardown and ensure that
        // none of the mocks are used because the iface does not exist in the internal
        // database.
        mWifiNative.teardownInterface(IFACE_NAME_0);
    }

    /**
     * Verifies failure handling in setup of a client interface.
     */
    @Test
    public void testSetupClientInterfaceFailureInHalCreateStaIface() throws Exception {
        when(mWifiVendorHal.createStaIface(anyBoolean(), any())).thenReturn(null);
        assertNull(mWifiNative.setupInterfaceForClientInConnectivityMode(mIfaceCallback0));

        mInOrder.verify(mWifiVendorHal).isVendorHalSupported();
        mInOrder.verify(mWifiVendorHal).startVendorHal();
        mInOrder.verify(mSupplicantStaIfaceHal).isInitializationStarted();
        mInOrder.verify(mSupplicantStaIfaceHal).initialize();
        mInOrder.verify(mSupplicantStaIfaceHal).startDaemon();
        mInOrder.verify(mSupplicantStaIfaceHal).isInitializationComplete();
        mInOrder.verify(mSupplicantStaIfaceHal).registerDeathHandler(any());
        mInOrder.verify(mWifiVendorHal).isVendorHalSupported();
        mInOrder.verify(mWifiVendorHal).createStaIface(eq(false), any());
        mInOrder.verify(mWifiMetrics).incrementNumSetupClientInterfaceFailureDueToHal();

        // To test if the failure is handled cleanly, invoke teardown and ensure that
        // none of the mocks are used because the iface does not exist in the internal
        // database.
        mWifiNative.teardownInterface(IFACE_NAME_0);
    }

    /**
     * Verifies failure handling in setup of a client interface.
     */
    @Test
    public void testSetupClientInterfaceFailureInWificondSetupInterfaceForClientMode()
            throws Exception {
        when(mWificondControl.setupInterfaceForClientMode(any())).thenReturn(null);
        assertNull(mWifiNative.setupInterfaceForClientInConnectivityMode(mIfaceCallback0));

        mInOrder.verify(mWifiVendorHal).isVendorHalSupported();
        mInOrder.verify(mWifiVendorHal).startVendorHal();
        mInOrder.verify(mSupplicantStaIfaceHal).isInitializationStarted();
        mInOrder.verify(mSupplicantStaIfaceHal).initialize();
        mInOrder.verify(mSupplicantStaIfaceHal).startDaemon();
        mInOrder.verify(mSupplicantStaIfaceHal).isInitializationComplete();
        mInOrder.verify(mSupplicantStaIfaceHal).registerDeathHandler(any());
        mInOrder.verify(mWifiVendorHal).isVendorHalSupported();
        mInOrder.verify(mWifiVendorHal).createStaIface(eq(false),
                mIfaceDestroyedListenerCaptor0.capture());
        mInOrder.verify(mWificondControl).setupInterfaceForClientMode(any());
        mInOrder.verify(mWifiVendorHal).isVendorHalSupported();
        mInOrder.verify(mWifiVendorHal).removeStaIface(any());
        mInOrder.verify(mWifiMetrics).incrementNumSetupClientInterfaceFailureDueToWificond();

        // Trigger the HAL interface destroyed callback to verify the whole removal sequence.
        mIfaceDestroyedListenerCaptor0.getValue().onDestroyed(IFACE_NAME_0);
        validateOnDestroyedClientInterface(false, false, IFACE_NAME_0, mIfaceCallback0,
                null);

        // To test if the failure is handled cleanly, invoke teardown and ensure that
        // none of the mocks are used because the iface does not exist in the internal
        // database.
        mWifiNative.teardownInterface(IFACE_NAME_0);
    }

    /**
     * Verifies failure handling in setup of a client interface.
     */
    @Test
    public void testSetupClientInterfaceFailureInSupplicantSetupIface() throws Exception {
        when(mSupplicantStaIfaceHal.setupIface(any())).thenReturn(false);
        assertNull(mWifiNative.setupInterfaceForClientInConnectivityMode(mIfaceCallback0));

        mInOrder.verify(mWifiVendorHal).isVendorHalSupported();
        mInOrder.verify(mWifiVendorHal).startVendorHal();
        mInOrder.verify(mSupplicantStaIfaceHal).isInitializationStarted();
        mInOrder.verify(mSupplicantStaIfaceHal).initialize();
        mInOrder.verify(mSupplicantStaIfaceHal).startDaemon();
        mInOrder.verify(mSupplicantStaIfaceHal).isInitializationComplete();
        mInOrder.verify(mSupplicantStaIfaceHal).registerDeathHandler(any());
        mInOrder.verify(mWifiVendorHal).isVendorHalSupported();
        mInOrder.verify(mWifiVendorHal).createStaIface(eq(false),
                mIfaceDestroyedListenerCaptor0.capture());
        mInOrder.verify(mWificondControl).setupInterfaceForClientMode(any());
        mInOrder.verify(mSupplicantStaIfaceHal).setupIface(any());
        mInOrder.verify(mWifiVendorHal).isVendorHalSupported();
        mInOrder.verify(mWifiVendorHal).removeStaIface(any());
        mInOrder.verify(mWifiMetrics).incrementNumSetupClientInterfaceFailureDueToSupplicant();

        // Trigger the HAL interface destroyed callback to verify the whole removal sequence.
        mIfaceDestroyedListenerCaptor0.getValue().onDestroyed(IFACE_NAME_0);
        validateOnDestroyedClientInterface(false, false, IFACE_NAME_0, mIfaceCallback0,
                null);

        // To test if the failure is handled cleanly, invoke teardown and ensure that
        // none of the mocks are used because the iface does not exist in the internal
        // database.
        mWifiNative.teardownInterface(IFACE_NAME_0);
    }

    /**
     * Verifies failure handling in setup of a client interface.
     */
    @Test
    public void testSetupClientInterfaceFailureInNetworkObserverRegister() throws Exception {
        doThrow(new RemoteException()).when(mNwManagementService).registerObserver(any());
        assertNull(mWifiNative.setupInterfaceForClientInConnectivityMode(mIfaceCallback0));

        mInOrder.verify(mWifiVendorHal).isVendorHalSupported();
        mInOrder.verify(mWifiVendorHal).startVendorHal();
        mInOrder.verify(mSupplicantStaIfaceHal).isInitializationStarted();
        mInOrder.verify(mSupplicantStaIfaceHal).initialize();
        mInOrder.verify(mSupplicantStaIfaceHal).startDaemon();
        mInOrder.verify(mSupplicantStaIfaceHal).isInitializationComplete();
        mInOrder.verify(mSupplicantStaIfaceHal).registerDeathHandler(any());
        mInOrder.verify(mWifiVendorHal).isVendorHalSupported();
        mInOrder.verify(mWifiVendorHal).createStaIface(eq(false),
                mIfaceDestroyedListenerCaptor0.capture());
        mInOrder.verify(mWificondControl).setupInterfaceForClientMode(any());
        mInOrder.verify(mSupplicantStaIfaceHal).setupIface(any());
        mInOrder.verify(mNwManagementService).registerObserver(mNetworkObserverCaptor0.capture());
        mInOrder.verify(mWifiVendorHal).isVendorHalSupported();
        mInOrder.verify(mWifiVendorHal).removeStaIface(any());

        // Trigger the HAL interface destroyed callback to verify the whole removal sequence.
        mIfaceDestroyedListenerCaptor0.getValue().onDestroyed(IFACE_NAME_0);
        validateOnDestroyedClientInterface(false, false, IFACE_NAME_0, mIfaceCallback0,
                mNetworkObserverCaptor0.getValue());

        // To test if the failure is handled cleanly, invoke teardown and ensure that
        // none of the mocks are used because the iface does not exist in the internal
        // database.
        mWifiNative.teardownInterface(IFACE_NAME_0);
    }

    /**
     * Verifies failure handling in setup of a softAp interface.
     */
    @Test
    public void testSetupSoftApInterfaceFailureInStartHal() throws Exception {
        when(mWifiVendorHal.startVendorHal()).thenReturn(false);
        assertNull(mWifiNative.setupInterfaceForSoftApMode(mIfaceCallback0));

        mInOrder.verify(mWifiVendorHal).isVendorHalSupported();
        mInOrder.verify(mWifiVendorHal).startVendorHal();
        mInOrder.verify(mWifiMetrics).incrementNumSetupSoftApInterfaceFailureDueToHal();

        // To test if the failure is handled cleanly, invoke teardown and ensure that
        // none of the mocks are used because the iface does not exist in the internal
        // database.
        mWifiNative.teardownInterface(IFACE_NAME_0);
    }

    /**
     * Verifies failure handling in setup of a softAp interface.
     */
    @Test
    public void testSetupSoftApInterfaceFailureInStartHostapd() throws Exception {
        when(mHostapdHal.startDaemon()).thenReturn(false);
        assertNull(mWifiNative.setupInterfaceForSoftApMode(mIfaceCallback0));

        mInOrder.verify(mWifiVendorHal).isVendorHalSupported();
        mInOrder.verify(mWifiVendorHal).startVendorHal();
        mInOrder.verify(mHostapdHal).isInitializationStarted();
        mInOrder.verify(mHostapdHal).initialize();
        mInOrder.verify(mHostapdHal).startDaemon();
        mInOrder.verify(mWifiMetrics).incrementNumSetupSoftApInterfaceFailureDueToHostapd();

        // To test if the failure is handled cleanly, invoke teardown and ensure that
        // none of the mocks are used because the iface does not exist in the internal
        // database.
        mWifiNative.teardownInterface(IFACE_NAME_0);
    }

    /**
     * Verifies failure handling in setup of a softAp interface.
     */
    @Test
    public void testSetupSoftApInterfaceFailureInHalCreateApIface() throws Exception {
        when(mWifiVendorHal.createApIface(any())).thenReturn(null);
        assertNull(mWifiNative.setupInterfaceForSoftApMode(mIfaceCallback0));

        mInOrder.verify(mWifiVendorHal).isVendorHalSupported();
        mInOrder.verify(mWifiVendorHal).startVendorHal();
        mInOrder.verify(mHostapdHal).isInitializationStarted();
        mInOrder.verify(mHostapdHal).initialize();
        mInOrder.verify(mHostapdHal).startDaemon();
        mInOrder.verify(mHostapdHal).isInitializationComplete();
        mInOrder.verify(mHostapdHal).registerDeathHandler(any());
        mInOrder.verify(mWifiVendorHal).isVendorHalSupported();
        mInOrder.verify(mWifiVendorHal).createApIface(any());
        mInOrder.verify(mWifiMetrics).incrementNumSetupSoftApInterfaceFailureDueToHal();

        // To test if the failure is handled cleanly, invoke teardown and ensure that
        // none of the mocks are used because the iface does not exist in the internal
        // database.
        mWifiNative.teardownInterface(IFACE_NAME_0);
    }

    /**
     * Verifies failure handling in setup of a softAp interface.
     */
    @Test
    public void testSetupSoftApInterfaceFailureInWificondSetupInterfaceForSoftapMode()
            throws Exception {
        when(mWificondControl.setupInterfaceForSoftApMode(any())).thenReturn(null);
        assertNull(mWifiNative.setupInterfaceForSoftApMode(mIfaceCallback0));

        mInOrder.verify(mWifiVendorHal).isVendorHalSupported();
        mInOrder.verify(mWifiVendorHal).startVendorHal();
        mInOrder.verify(mHostapdHal).isInitializationStarted();
        mInOrder.verify(mHostapdHal).initialize();
        mInOrder.verify(mHostapdHal).startDaemon();
        mInOrder.verify(mHostapdHal).isInitializationComplete();
        mInOrder.verify(mHostapdHal).registerDeathHandler(any());
        mInOrder.verify(mWifiVendorHal).isVendorHalSupported();
        mInOrder.verify(mWifiVendorHal).createApIface(mIfaceDestroyedListenerCaptor0.capture());
        mInOrder.verify(mWificondControl).setupInterfaceForSoftApMode(any());
        mInOrder.verify(mWifiVendorHal).isVendorHalSupported();
        mInOrder.verify(mWifiVendorHal).removeApIface(any());
        mInOrder.verify(mWifiMetrics).incrementNumSetupSoftApInterfaceFailureDueToWificond();

        // Trigger the HAL interface destroyed callback to verify the whole removal sequence.
        mIfaceDestroyedListenerCaptor0.getValue().onDestroyed(IFACE_NAME_0);
        validateOnDestroyedSoftApInterface(false, false, IFACE_NAME_0, mIfaceCallback0,
                null);

        // To test if the failure is handled cleanly, invoke teardown and ensure that
        // none of the mocks are used because the iface does not exist in the internal
        // database.
        mWifiNative.teardownInterface(IFACE_NAME_0);
    }

    /**
     * Verifies the interface state query API.
     */
    @Test
    public void testIsInterfaceUp() throws Exception {
        executeAndValidateSetupClientInterface(
                false, false, IFACE_NAME_0, mIfaceCallback0, mIfaceDestroyedListenerCaptor0,
                mNetworkObserverCaptor0);

        mInterfaceConfiguration.setInterfaceUp();
        assertTrue(mWifiNative.isInterfaceUp(IFACE_NAME_0));

        mInterfaceConfiguration.setInterfaceDown();
        assertFalse(mWifiNative.isInterfaceUp(IFACE_NAME_0));

        when(mNwManagementService.getInterfaceConfig(IFACE_NAME_0)).thenReturn(null);
        assertFalse(mWifiNative.isInterfaceUp(IFACE_NAME_0));

        verify(mNwManagementService, times(4)).getInterfaceConfig(IFACE_NAME_0);
    }

    /**
     * Verifies that the interface name is null when there are no interfaces setup.
     */
    @Test
    public void testGetClientInterfaceNameWithNoInterfacesSetup() throws Exception {
        assertNull(mWifiNative.getClientInterfaceName());
    }

    /**
     * Verifies that the interface name is null when there are no client interfaces setup.
     */
    @Test
    public void testGetClientInterfaceNameWithNoClientInterfaceSetup() throws Exception {
        executeAndValidateSetupSoftApInterface(
                false, false, IFACE_NAME_0, mIfaceCallback0, mIfaceDestroyedListenerCaptor0,
                mNetworkObserverCaptor0);
        assertNull(mWifiNative.getClientInterfaceName());
    }

    /**
     * Verifies that the interface name is not null when there is one client interface setup.
     */
    @Test
    public void testGetClientInterfaceNameWithOneClientInterfaceSetup() throws Exception {
        executeAndValidateSetupClientInterface(
                false, false, IFACE_NAME_0, mIfaceCallback0, mIfaceDestroyedListenerCaptor0,
                mNetworkObserverCaptor0);
        assertEquals(IFACE_NAME_0, mWifiNative.getClientInterfaceName());
    }

    /**
     * Verifies that the interface name is not null when there are more than one client interfaces
     * setup.
     */
    @Test
    public void testGetClientInterfaceNameWithMoreThanOneClientInterfaceSetup() throws Exception {
        executeAndValidateSetupClientInterface(
                false, false, IFACE_NAME_0, mIfaceCallback0, mIfaceDestroyedListenerCaptor0,
                mNetworkObserverCaptor0);
        executeAndValidateSetupClientInterface(
                true, false, IFACE_NAME_1, mIfaceCallback1, mIfaceDestroyedListenerCaptor1,
                mNetworkObserverCaptor1);
        String interfaceName = mWifiNative.getClientInterfaceName();
        assertNotNull(interfaceName);
        assertTrue(interfaceName.equals(IFACE_NAME_0) || interfaceName.equals(IFACE_NAME_1));
    }

    /*
     * Verifies the setup of a client interface and then a SoftAp interface which would
     * destroy the Client interface. This is what would happen on older devices which do not
     * support the vendor HAL.
     */
    @Test
    public void testSetupClientAndSoftApInterfaceCausesClientInterfaceTeardownWithNoVendorHal()
            throws Exception {
        when(mWifiVendorHal.isVendorHalSupported()).thenReturn(false);
        when(mPropertyService.getString(any(), any())).thenReturn(IFACE_NAME_0);

        // First setup a STA interface and verify.
        assertEquals(IFACE_NAME_0,
                mWifiNative.setupInterfaceForClientInConnectivityMode(mIfaceCallback0));

        mInOrder.verify(mWifiVendorHal).isVendorHalSupported();
        mInOrder.verify(mSupplicantStaIfaceHal).isInitializationStarted();
        mInOrder.verify(mSupplicantStaIfaceHal).initialize();
        mInOrder.verify(mSupplicantStaIfaceHal).startDaemon();
        mInOrder.verify(mSupplicantStaIfaceHal).isInitializationComplete();
        mInOrder.verify(mSupplicantStaIfaceHal).registerDeathHandler(any());
        mInOrder.verify(mWifiVendorHal).isVendorHalSupported();
        mInOrder.verify(mWificondControl).setupInterfaceForClientMode(IFACE_NAME_0);
        mInOrder.verify(mSupplicantStaIfaceHal).setupIface(IFACE_NAME_0);
        mInOrder.verify(mNwManagementService).registerObserver(mNetworkObserverCaptor0.capture());
        mInOrder.verify(mWifiMonitor).startMonitoring(IFACE_NAME_0);
        mInOrder.verify(mNwManagementService).getInterfaceConfig(IFACE_NAME_0);
        mInOrder.verify(mNwManagementService).clearInterfaceAddresses(IFACE_NAME_0);
        mInOrder.verify(mNwManagementService).setInterfaceIpv6PrivacyExtensions(IFACE_NAME_0, true);
        mInOrder.verify(mNwManagementService).disableIpv6(IFACE_NAME_0);
        mInOrder.verify(mSupplicantStaIfaceHal).getAdvancedKeyMgmtCapabilities(IFACE_NAME_0);
        mInOrder.verify(mWifiVendorHal).getSupportedFeatureSet(IFACE_NAME_0);

        // Now setup an AP interface.
        assertEquals(IFACE_NAME_0, mWifiNative.setupInterfaceForSoftApMode(mIfaceCallback1));

        mInOrder.verify(mHostapdHal).isInitializationStarted();
        mInOrder.verify(mHostapdHal).initialize();
        mInOrder.verify(mHostapdHal).startDaemon();
        mInOrder.verify(mHostapdHal).isInitializationComplete();
        mInOrder.verify(mHostapdHal).registerDeathHandler(any());
        mInOrder.verify(mWifiVendorHal).isVendorHalSupported();
        // Creation of AP interface should trigger the STA interface destroy
        verify(mWifiMonitor).stopMonitoring(IFACE_NAME_0);
        mInOrder.verify(mNwManagementService).unregisterObserver(
                mNetworkObserverCaptor0.getValue());
        mInOrder.verify(mSupplicantStaIfaceHal).teardownIface(IFACE_NAME_0);
        mInOrder.verify(mWificondControl).tearDownClientInterface(IFACE_NAME_0);
        mInOrder.verify(mSupplicantStaIfaceHal).deregisterDeathHandler();
        mInOrder.verify(mSupplicantStaIfaceHal).terminate();
        mInOrder.verify(mIfaceCallback0).onDestroyed(IFACE_NAME_0);
        // Now continue with rest of AP interface setup.
        mInOrder.verify(mWificondControl).setupInterfaceForSoftApMode(IFACE_NAME_0);
        mInOrder.verify(mNwManagementService).registerObserver(mNetworkObserverCaptor1.capture());
        mInOrder.verify(mNwManagementService).getInterfaceConfig(IFACE_NAME_0);
        mInOrder.verify(mSupplicantStaIfaceHal).getAdvancedKeyMgmtCapabilities(IFACE_NAME_0);
        mInOrder.verify(mWifiVendorHal).getSupportedFeatureSet(IFACE_NAME_0);
    }

    /**
     * Verifies the setup of a client interface and then a SoftAp interface which would
     * destroy the Client interface. This is what would happen on older devices which do not
     * support the vendor HAL.
     */
    @Test
    public void testSetupSoftApAndClientInterfaceCausesSoftApInterfaceTeardownWithNoVendorHal()
            throws Exception {
        when(mWifiVendorHal.isVendorHalSupported()).thenReturn(false);
        when(mPropertyService.getString(any(), any())).thenReturn(IFACE_NAME_0);

        // First setup an AP interface and verify.
        assertEquals(IFACE_NAME_0, mWifiNative.setupInterfaceForSoftApMode(mIfaceCallback0));

        mInOrder.verify(mWifiVendorHal).isVendorHalSupported();
        mInOrder.verify(mHostapdHal).isInitializationStarted();
        mInOrder.verify(mHostapdHal).initialize();
        mInOrder.verify(mHostapdHal).startDaemon();
        mInOrder.verify(mHostapdHal).isInitializationComplete();
        mInOrder.verify(mHostapdHal).registerDeathHandler(any());
        mInOrder.verify(mWifiVendorHal).isVendorHalSupported();
        mInOrder.verify(mWificondControl).setupInterfaceForSoftApMode(IFACE_NAME_0);
        mInOrder.verify(mNwManagementService).registerObserver(mNetworkObserverCaptor0.capture());
        mInOrder.verify(mNwManagementService).getInterfaceConfig(IFACE_NAME_0);
        mInOrder.verify(mSupplicantStaIfaceHal).getAdvancedKeyMgmtCapabilities(IFACE_NAME_0);
        mInOrder.verify(mWifiVendorHal).getSupportedFeatureSet(IFACE_NAME_0);

        // Now setup a STA interface.
        assertEquals(IFACE_NAME_0,
                mWifiNative.setupInterfaceForClientInConnectivityMode(mIfaceCallback1));

        mInOrder.verify(mSupplicantStaIfaceHal).isInitializationStarted();
        mInOrder.verify(mSupplicantStaIfaceHal).initialize();
        mInOrder.verify(mSupplicantStaIfaceHal).startDaemon();
        mInOrder.verify(mSupplicantStaIfaceHal).isInitializationComplete();
        mInOrder.verify(mSupplicantStaIfaceHal).registerDeathHandler(any());
        mInOrder.verify(mWifiVendorHal).isVendorHalSupported();
        // Creation of STA interface should trigger the AP interface destroy.
        mInOrder.verify(mNwManagementService).unregisterObserver(
                mNetworkObserverCaptor0.getValue());
        mInOrder.verify(mHostapdHal).removeAccessPoint(IFACE_NAME_0);
        mInOrder.verify(mWificondControl).tearDownSoftApInterface(IFACE_NAME_0);
        mInOrder.verify(mHostapdHal).deregisterDeathHandler();
        mInOrder.verify(mHostapdHal).terminate();
        mInOrder.verify(mIfaceCallback0).onDestroyed(IFACE_NAME_0);
        // Now continue with rest of STA interface setup.
        mInOrder.verify(mWificondControl).setupInterfaceForClientMode(IFACE_NAME_0);
        mInOrder.verify(mSupplicantStaIfaceHal).setupIface(IFACE_NAME_0);
        mInOrder.verify(mNwManagementService).registerObserver(mNetworkObserverCaptor1.capture());
        mInOrder.verify(mWifiMonitor).startMonitoring(IFACE_NAME_0);
        mInOrder.verify(mNwManagementService).getInterfaceConfig(IFACE_NAME_0);
        mInOrder.verify(mNwManagementService).clearInterfaceAddresses(IFACE_NAME_0);
        mInOrder.verify(mNwManagementService).setInterfaceIpv6PrivacyExtensions(IFACE_NAME_0, true);
        mInOrder.verify(mNwManagementService).disableIpv6(IFACE_NAME_0);
        mInOrder.verify(mSupplicantStaIfaceHal).getAdvancedKeyMgmtCapabilities(IFACE_NAME_0);
        mInOrder.verify(mWifiVendorHal).getSupportedFeatureSet(IFACE_NAME_0);
    }

    /**
     * Verifies the handling of radio mode change callbacks.
     */
    @Test
    public void testRadioModeChangeCallback() {
        WifiNative.VendorHalRadioModeChangeEventHandler handler =
                mWifiVendorHalRadioModeChangeHandlerCaptor.getValue();

        handler.onMcc(WifiScanner.WIFI_BAND_5_GHZ);
        mInOrder.verify(mWifiMetrics).incrementNumRadioModeChangeToMcc();

        handler.onScc(WifiScanner.WIFI_BAND_24_GHZ);
        mInOrder.verify(mWifiMetrics).incrementNumRadioModeChangeToScc();

        handler.onSbs(WifiScanner.WIFI_BAND_24_GHZ);
        mInOrder.verify(mWifiMetrics).incrementNumRadioModeChangeToSbs();

        handler.onDbs();
        mInOrder.verify(mWifiMetrics).incrementNumRadioModeChangeToDbs();
    }

    private void executeAndValidateSetupClientInterface(
            boolean existingStaIface, boolean existingApIface,
            String ifaceName, @Mock WifiNative.InterfaceCallback callback,
            ArgumentCaptor<InterfaceDestroyedListener> destroyedListenerCaptor,
            ArgumentCaptor<BaseNetworkObserver> networkObserverCaptor) throws Exception {
        when(mWifiVendorHal.createStaIface(anyBoolean(), any())).thenReturn(ifaceName);
        assertEquals(ifaceName, mWifiNative.setupInterfaceForClientInConnectivityMode(callback));

        validateSetupClientInterface(
                existingStaIface, existingApIface, ifaceName, destroyedListenerCaptor,
                networkObserverCaptor);
    }

    private void validateSetupClientInterface(
            boolean existingStaIface, boolean existingApIface,
            String ifaceName, ArgumentCaptor<InterfaceDestroyedListener> destroyedListenerCaptor,
            ArgumentCaptor<BaseNetworkObserver> networkObserverCaptor) throws Exception {
        if (!existingStaIface && !existingApIface) {
            mInOrder.verify(mWifiVendorHal).isVendorHalSupported();
            mInOrder.verify(mWifiVendorHal).startVendorHal();
        }
        if (!existingStaIface) {
            mInOrder.verify(mSupplicantStaIfaceHal).isInitializationStarted();
            mInOrder.verify(mSupplicantStaIfaceHal).initialize();
            mInOrder.verify(mSupplicantStaIfaceHal).startDaemon();
            mInOrder.verify(mSupplicantStaIfaceHal).isInitializationComplete();
            mInOrder.verify(mSupplicantStaIfaceHal).registerDeathHandler(any());
        }
        mInOrder.verify(mWifiVendorHal).isVendorHalSupported();
        mInOrder.verify(mWifiVendorHal).createStaIface(eq(false),
                destroyedListenerCaptor.capture());
        mInOrder.verify(mWificondControl).setupInterfaceForClientMode(ifaceName);
        mInOrder.verify(mSupplicantStaIfaceHal).setupIface(ifaceName);
        mInOrder.verify(mNwManagementService).registerObserver(networkObserverCaptor.capture());
        mInOrder.verify(mWifiMonitor).startMonitoring(ifaceName);
        mInOrder.verify(mNwManagementService).getInterfaceConfig(ifaceName);
        mInOrder.verify(mNwManagementService).clearInterfaceAddresses(ifaceName);
        mInOrder.verify(mNwManagementService).setInterfaceIpv6PrivacyExtensions(ifaceName, true);
        mInOrder.verify(mNwManagementService).disableIpv6(ifaceName);
        mInOrder.verify(mSupplicantStaIfaceHal).getAdvancedKeyMgmtCapabilities(ifaceName);
        mInOrder.verify(mWifiVendorHal).getSupportedFeatureSet(ifaceName);
    }

    private void executeAndValidateTeardownClientInterface(
            boolean anyOtherStaIface, boolean anyOtherApIface,
            String ifaceName, @Mock WifiNative.InterfaceCallback callback,
            InterfaceDestroyedListener destroyedListener,
            BaseNetworkObserver networkObserver) throws Exception {
        mWifiNative.teardownInterface(ifaceName);

        mInOrder.verify(mWifiVendorHal).isVendorHalSupported();
        mInOrder.verify(mWifiVendorHal).removeStaIface(ifaceName);

        // Now trigger the HalDeviceManager destroy callback to initiate the rest of the teardown.
        destroyedListener.onDestroyed(ifaceName);

        validateOnDestroyedClientInterface(
                anyOtherStaIface, anyOtherApIface, ifaceName, callback, networkObserver);
    }

    private void validateOnDestroyedClientInterface(
            boolean anyOtherStaIface, boolean anyOtherApIface,
            String ifaceName, @Mock WifiNative.InterfaceCallback callback,
            BaseNetworkObserver networkObserver) throws Exception {
        mInOrder.verify(mWifiMonitor).stopMonitoring(ifaceName);
        if (networkObserver != null) {
            mInOrder.verify(mNwManagementService).unregisterObserver(networkObserver);
        }
        mInOrder.verify(mSupplicantStaIfaceHal).teardownIface(ifaceName);
        mInOrder.verify(mWificondControl).tearDownClientInterface(ifaceName);

        if (!anyOtherStaIface) {
            mInOrder.verify(mSupplicantStaIfaceHal).deregisterDeathHandler();
            mInOrder.verify(mSupplicantStaIfaceHal).terminate();
        }
        if (!anyOtherStaIface && !anyOtherApIface) {
            mInOrder.verify(mWificondControl).tearDownInterfaces();
            mInOrder.verify(mWifiVendorHal).isVendorHalSupported();
            mInOrder.verify(mWifiVendorHal).stopVendorHal();
        }
        mInOrder.verify(callback).onDestroyed(ifaceName);
    }

    private void executeAndValidateSetupClientInterfaceForScan(
            boolean existingStaIface, boolean existingApIface,
            String ifaceName, @Mock WifiNative.InterfaceCallback callback,
            ArgumentCaptor<InterfaceDestroyedListener> destroyedListenerCaptor,
            ArgumentCaptor<BaseNetworkObserver> networkObserverCaptor) throws Exception {
        when(mWifiVendorHal.createStaIface(anyBoolean(), any())).thenReturn(ifaceName);
        assertEquals(ifaceName, mWifiNative.setupInterfaceForClientInScanMode(callback));

        validateSetupClientInterfaceForScan(
                existingStaIface, existingApIface, ifaceName, destroyedListenerCaptor,
                networkObserverCaptor);
    }

    private void validateSetupClientInterfaceForScan(
            boolean existingStaIface, boolean existingApIface,
            String ifaceName, ArgumentCaptor<InterfaceDestroyedListener> destroyedListenerCaptor,
            ArgumentCaptor<BaseNetworkObserver> networkObserverCaptor) throws Exception {
        if (!existingStaIface && !existingApIface) {
            mInOrder.verify(mWifiVendorHal).isVendorHalSupported();
            mInOrder.verify(mWifiVendorHal).startVendorHal();
        }
        mInOrder.verify(mWifiVendorHal).isVendorHalSupported();
        mInOrder.verify(mWifiVendorHal).createStaIface(eq(true),
                destroyedListenerCaptor.capture());
        mInOrder.verify(mWificondControl).setupInterfaceForClientMode(ifaceName);
        mInOrder.verify(mNwManagementService).registerObserver(networkObserverCaptor.capture());
        mInOrder.verify(mWifiMonitor).startMonitoring(ifaceName);
        mInOrder.verify(mNwManagementService).getInterfaceConfig(ifaceName);
        mInOrder.verify(mSupplicantStaIfaceHal).getAdvancedKeyMgmtCapabilities(ifaceName);
        mInOrder.verify(mWifiVendorHal).getSupportedFeatureSet(ifaceName);
    }

    private void executeAndValidateTeardownClientInterfaceForScan(
            boolean anyOtherStaIface, boolean anyOtherApIface,
            String ifaceName, @Mock WifiNative.InterfaceCallback callback,
            InterfaceDestroyedListener destroyedListener,
            BaseNetworkObserver networkObserver) throws Exception {
        mWifiNative.teardownInterface(ifaceName);

        mInOrder.verify(mWifiVendorHal).isVendorHalSupported();
        mInOrder.verify(mWifiVendorHal).removeStaIface(ifaceName);

        // Now trigger the HalDeviceManager destroy callback to initiate the rest of the teardown.
        destroyedListener.onDestroyed(ifaceName);

        validateOnDestroyedClientInterfaceForScan(
                anyOtherStaIface, anyOtherApIface, ifaceName, callback, networkObserver);
    }

    private void validateOnDestroyedClientInterfaceForScan(
            boolean anyOtherStaIface, boolean anyOtherApIface,
            String ifaceName, @Mock WifiNative.InterfaceCallback callback,
            BaseNetworkObserver networkObserver) throws Exception {
        mInOrder.verify(mWifiMonitor).stopMonitoring(ifaceName);
        if (networkObserver != null) {
            mInOrder.verify(mNwManagementService).unregisterObserver(networkObserver);
        }
        mInOrder.verify(mWificondControl).tearDownClientInterface(ifaceName);

        if (!anyOtherStaIface && !anyOtherApIface) {
            mInOrder.verify(mWificondControl).tearDownInterfaces();
            mInOrder.verify(mWifiVendorHal).isVendorHalSupported();
            mInOrder.verify(mWifiVendorHal).stopVendorHal();
        }
        mInOrder.verify(callback).onDestroyed(ifaceName);
    }

    private void executeAndValidateSetupSoftApInterface(
            boolean existingStaIface, boolean existingApIface,
            String ifaceName, @Mock WifiNative.InterfaceCallback callback,
            ArgumentCaptor<InterfaceDestroyedListener> destroyedListenerCaptor,
            ArgumentCaptor<BaseNetworkObserver> networkObserverCaptor) throws Exception {
        when(mWifiVendorHal.createApIface(any())).thenReturn(ifaceName);
        assertEquals(ifaceName, mWifiNative.setupInterfaceForSoftApMode(callback));

        validateSetupSoftApInterface(
                existingStaIface, existingApIface, ifaceName, destroyedListenerCaptor,
                networkObserverCaptor);
    }

    private void validateSetupSoftApInterface(
            boolean existingStaIface, boolean existingApIface,
            String ifaceName, ArgumentCaptor<InterfaceDestroyedListener> destroyedListenerCaptor,
            ArgumentCaptor<BaseNetworkObserver> networkObserverCaptor) throws Exception {
        if (!existingStaIface && !existingApIface) {
            mInOrder.verify(mWifiVendorHal).isVendorHalSupported();
            mInOrder.verify(mWifiVendorHal).startVendorHal();
        }
        if (!existingApIface) {
            mInOrder.verify(mHostapdHal).isInitializationStarted();
            mInOrder.verify(mHostapdHal).initialize();
            mInOrder.verify(mHostapdHal).startDaemon();
            mInOrder.verify(mHostapdHal).isInitializationComplete();
            mInOrder.verify(mHostapdHal).registerDeathHandler(any());
        }
        mInOrder.verify(mWifiVendorHal).isVendorHalSupported();
        mInOrder.verify(mWifiVendorHal).createApIface(destroyedListenerCaptor.capture());
        mInOrder.verify(mWificondControl).setupInterfaceForSoftApMode(ifaceName);
        mInOrder.verify(mNwManagementService).registerObserver(networkObserverCaptor.capture());
        mInOrder.verify(mNwManagementService).getInterfaceConfig(ifaceName);
        mInOrder.verify(mSupplicantStaIfaceHal).getAdvancedKeyMgmtCapabilities(ifaceName);
        mInOrder.verify(mWifiVendorHal).getSupportedFeatureSet(ifaceName);
    }

    private void executeAndValidateTeardownSoftApInterface(
            boolean anyOtherStaIface, boolean anyOtherApIface,
            String ifaceName, @Mock WifiNative.InterfaceCallback callback,
            InterfaceDestroyedListener destroyedListener,
            BaseNetworkObserver networkObserver) throws Exception {
        mWifiNative.teardownInterface(ifaceName);

        mInOrder.verify(mWifiVendorHal).isVendorHalSupported();
        mInOrder.verify(mWifiVendorHal).removeApIface(ifaceName);

        // Now trigger the HalDeviceManager destroy callback to initiate the rest of the teardown.
        destroyedListener.onDestroyed(ifaceName);

        validateOnDestroyedSoftApInterface(
                anyOtherStaIface, anyOtherApIface, ifaceName, callback, networkObserver);
    }

    private void validateOnDestroyedSoftApInterface(
            boolean anyOtherStaIface, boolean anyOtherApIface,
            String ifaceName, @Mock WifiNative.InterfaceCallback callback,
            BaseNetworkObserver networkObserver) throws Exception {
        if (networkObserver != null) {
            mInOrder.verify(mNwManagementService).unregisterObserver(networkObserver);
        }
        mInOrder.verify(mHostapdHal).removeAccessPoint(ifaceName);
        mInOrder.verify(mWificondControl).tearDownSoftApInterface(ifaceName);

        if (!anyOtherApIface) {
            mInOrder.verify(mHostapdHal).deregisterDeathHandler();
            mInOrder.verify(mHostapdHal).terminate();
        }
        if (!anyOtherStaIface && !anyOtherApIface) {
            mInOrder.verify(mWificondControl).tearDownInterfaces();
            mInOrder.verify(mWifiVendorHal).isVendorHalSupported();
            mInOrder.verify(mWifiVendorHal).stopVendorHal();
        }
        mInOrder.verify(callback).onDestroyed(ifaceName);
    }

    private void executeAndValidateInterfaceStateChange(
            String ifaceName, boolean up, BaseNetworkObserver networkObserver) throws Exception {
        if (up) {
            mInterfaceConfiguration.setInterfaceUp();
        } else {
            mInterfaceConfiguration.setInterfaceDown();
        }
        networkObserver.interfaceLinkStateChanged(ifaceName, up);
        mLooper.dispatchAll();
        mInOrder.verify(mNwManagementService).getInterfaceConfig(ifaceName);
    }
}
