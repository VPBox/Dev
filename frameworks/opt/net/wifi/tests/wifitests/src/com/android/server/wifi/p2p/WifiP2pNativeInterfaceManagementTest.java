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
import static org.mockito.ArgumentMatchers.any;
import static org.mockito.Mockito.any;
import static org.mockito.Mockito.doAnswer;
import static org.mockito.Mockito.eq;
import static org.mockito.Mockito.never;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;

import android.app.test.MockAnswerUtil;
import android.hardware.wifi.V1_0.IWifiIface;
import android.hardware.wifi.V1_0.IWifiP2pIface;
import android.hardware.wifi.V1_0.IfaceType;
import android.hardware.wifi.V1_0.WifiStatus;
import android.hardware.wifi.V1_0.WifiStatusCode;
import android.os.Handler;
import android.os.RemoteException;

import androidx.test.filters.SmallTest;

import com.android.server.wifi.HalDeviceManager;
import com.android.server.wifi.HalDeviceManager.InterfaceAvailableForRequestListener;
import com.android.server.wifi.HalDeviceManager.InterfaceDestroyedListener;
import com.android.server.wifi.HalDeviceManager.ManagerStatusListener;
import com.android.server.wifi.PropertyService;
import com.android.server.wifi.WifiVendorHal;

import org.junit.Before;
import org.junit.Test;
import org.mockito.ArgumentCaptor;
import org.mockito.Mock;
import org.mockito.MockitoAnnotations;

/**
 * Unit tests for the interface management operations in
 * {@link com.android.server.wifi.WifiP2pNative}.
 */
@SmallTest
public class WifiP2pNativeInterfaceManagementTest {
    private static final String P2P_IFACE_NAME = "p2p0";
    private static final String P2P_INTERFACE_PROPERTY = "wifi.direct.interface";

    @Mock private SupplicantP2pIfaceHal mSupplicantP2pIfaceHal;
    @Mock private HalDeviceManager mHalDeviceManager;
    @Mock private PropertyService mPropertyService;
    @Mock private Handler mHandler;
    @Mock private InterfaceAvailableForRequestListener mInterfaceRequestListener;
    @Mock private InterfaceDestroyedListener mHalDeviceInterfaceDestroyedListener;
    @Mock private IWifiP2pIface mIWifiP2pIface;
    @Mock private IWifiIface mIWifiIface;
    @Mock private WifiVendorHal mWifiVendorHal;
    private WifiP2pNative mWifiP2pNative;
    private WifiStatus mWifiStatusSuccess;
    private ManagerStatusListener mManagerStatusListener;

    /**
     * Sets up for unit test
     */
    @Before
    public void setUp() throws Exception {
        MockitoAnnotations.initMocks(this);
        mWifiStatusSuccess = new WifiStatus();
        mWifiStatusSuccess.code = WifiStatusCode.SUCCESS;

        when(mHalDeviceManager.isSupported()).thenReturn(true);
        when(mHalDeviceManager.createP2pIface(any(InterfaceDestroyedListener.class),
                any(Handler.class))).thenReturn(mIWifiP2pIface);
        doAnswer(new MockAnswerUtil.AnswerWithArguments() {
            public void answer(IWifiIface.getNameCallback cb)
                    throws RemoteException {
                cb.onValues(mWifiStatusSuccess, P2P_IFACE_NAME);
            }
        }).when(mIWifiP2pIface).getName(any(IWifiIface.getNameCallback.class));

        when(mSupplicantP2pIfaceHal.isInitializationStarted()).thenReturn(true);
        when(mSupplicantP2pIfaceHal.initialize()).thenReturn(true);
        when(mSupplicantP2pIfaceHal.isInitializationComplete()).thenReturn(true);
        when(mSupplicantP2pIfaceHal.setupIface(P2P_IFACE_NAME)).thenReturn(true);
        when(mPropertyService.getString(P2P_INTERFACE_PROPERTY, P2P_IFACE_NAME))
              .thenReturn(P2P_IFACE_NAME);

        mWifiP2pNative = new WifiP2pNative(
                              mWifiVendorHal, mSupplicantP2pIfaceHal, mHalDeviceManager,
                              mPropertyService);
    }

    /**
     * Verifies the HAL (HIDL) interface listener.
     */
    @Test
    public void testRegisterInterfaceAvailableListener() throws Exception {
        when(mHalDeviceManager.isStarted()).thenReturn(false);

        mWifiP2pNative.registerInterfaceAvailableListener(mInterfaceRequestListener, mHandler);
        when(mHalDeviceManager.isStarted()).thenReturn(true);

        ArgumentCaptor<ManagerStatusListener> hdmCallbackCaptor =
                ArgumentCaptor.forClass(ManagerStatusListener.class);
        verify(mHalDeviceManager).registerStatusListener(hdmCallbackCaptor.capture(), eq(mHandler));
        // Simulate to call status callback from device hal manager
        hdmCallbackCaptor.getValue().onStatusChanged();

        verify(mHalDeviceManager).registerInterfaceAvailableForRequestListener(eq(IfaceType.P2P),
                any(InterfaceAvailableForRequestListener.class), eq(mHandler));
    }

    /**
     * Verifies the setup of a p2p interface.
     */
    @Test
    public void testSetUpInterface() throws Exception {
        assertEquals(P2P_IFACE_NAME,
                mWifiP2pNative.setupInterface(mHalDeviceInterfaceDestroyedListener, mHandler));

        verify(mHalDeviceManager).createP2pIface(any(InterfaceDestroyedListener.class),
                eq(mHandler));
        verify(mSupplicantP2pIfaceHal).setupIface(eq(P2P_IFACE_NAME));
    }

    /**
     * Verifies the setup of a p2p interface with no HAL (HIDL) support.
     */
    @Test
    public void testSetUpInterfaceWithNoVendorHal() throws Exception {
        when(mHalDeviceManager.isSupported()).thenReturn(false);

        assertEquals(P2P_IFACE_NAME, mWifiP2pNative.setupInterface(
                mHalDeviceInterfaceDestroyedListener, mHandler));

        verify(mHalDeviceManager, never())
                .createP2pIface(any(InterfaceDestroyedListener.class), any(Handler.class));
        verify(mSupplicantP2pIfaceHal).setupIface(eq(P2P_IFACE_NAME));
    }

    /**
     * Verifies the teardown of a p2p interface.
     */
    @Test
    public void testTeardownInterface() throws Exception {
        assertEquals(P2P_IFACE_NAME,
                mWifiP2pNative.setupInterface(mHalDeviceInterfaceDestroyedListener,
                    mHandler));

        mWifiP2pNative.teardownInterface();

        verify(mHalDeviceManager).removeIface(any(IWifiIface.class));
        verify(mSupplicantP2pIfaceHal).teardownIface(eq(P2P_IFACE_NAME));
    }

    /**
     * Verifies the teardown of a p2p interface with no HAL (HIDL) support.
     */
    @Test
    public void testTeardownInterfaceWithNoVendorHal() throws Exception {
        when(mHalDeviceManager.isSupported()).thenReturn(false);

        assertEquals(P2P_IFACE_NAME, mWifiP2pNative.setupInterface(
                mHalDeviceInterfaceDestroyedListener, mHandler));

        mWifiP2pNative.teardownInterface();

        verify(mHalDeviceManager, never()).removeIface(any(IWifiIface.class));
        verify(mSupplicantP2pIfaceHal).teardownIface(eq(P2P_IFACE_NAME));
    }
}
