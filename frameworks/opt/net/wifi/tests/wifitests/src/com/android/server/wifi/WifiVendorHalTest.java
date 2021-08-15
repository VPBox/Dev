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

package com.android.server.wifi;

import static org.junit.Assert.assertArrayEquals;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertNotEquals;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertNull;
import static org.junit.Assert.assertTrue;
import static org.mockito.Mockito.any;
import static org.mockito.Mockito.anyBoolean;
import static org.mockito.Mockito.anyByte;
import static org.mockito.Mockito.anyInt;
import static org.mockito.Mockito.anyObject;
import static org.mockito.Mockito.anyShort;
import static org.mockito.Mockito.doAnswer;
import static org.mockito.Mockito.doNothing;
import static org.mockito.Mockito.doThrow;
import static org.mockito.Mockito.eq;
import static org.mockito.Mockito.inOrder;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.never;
import static org.mockito.Mockito.reset;
import static org.mockito.Mockito.spy;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.verifyNoMoreInteractions;
import static org.mockito.Mockito.when;

import android.app.test.MockAnswerUtil.AnswerWithArguments;
import android.hardware.wifi.V1_0.IWifiApIface;
import android.hardware.wifi.V1_0.IWifiChip;
import android.hardware.wifi.V1_0.IWifiChipEventCallback;
import android.hardware.wifi.V1_0.IWifiIface;
import android.hardware.wifi.V1_0.IWifiStaIface;
import android.hardware.wifi.V1_0.IWifiStaIfaceEventCallback;
import android.hardware.wifi.V1_0.IfaceType;
import android.hardware.wifi.V1_0.StaApfPacketFilterCapabilities;
import android.hardware.wifi.V1_0.StaBackgroundScanCapabilities;
import android.hardware.wifi.V1_0.StaBackgroundScanParameters;
import android.hardware.wifi.V1_0.StaLinkLayerIfacePacketStats;
import android.hardware.wifi.V1_0.StaLinkLayerIfaceStats;
import android.hardware.wifi.V1_0.StaLinkLayerRadioStats;
import android.hardware.wifi.V1_0.StaLinkLayerStats;
import android.hardware.wifi.V1_0.StaRoamingCapabilities;
import android.hardware.wifi.V1_0.StaRoamingState;
import android.hardware.wifi.V1_0.StaScanData;
import android.hardware.wifi.V1_0.StaScanDataFlagMask;
import android.hardware.wifi.V1_0.StaScanResult;
import android.hardware.wifi.V1_0.WifiDebugHostWakeReasonStats;
import android.hardware.wifi.V1_0.WifiDebugPacketFateFrameType;
import android.hardware.wifi.V1_0.WifiDebugRingBufferFlags;
import android.hardware.wifi.V1_0.WifiDebugRingBufferStatus;
import android.hardware.wifi.V1_0.WifiDebugRingBufferVerboseLevel;
import android.hardware.wifi.V1_0.WifiDebugRxPacketFate;
import android.hardware.wifi.V1_0.WifiDebugRxPacketFateReport;
import android.hardware.wifi.V1_0.WifiDebugTxPacketFate;
import android.hardware.wifi.V1_0.WifiDebugTxPacketFateReport;
import android.hardware.wifi.V1_0.WifiInformationElement;
import android.hardware.wifi.V1_0.WifiStatus;
import android.hardware.wifi.V1_0.WifiStatusCode;
import android.hardware.wifi.V1_2.IWifiChipEventCallback.IfaceInfo;
import android.hardware.wifi.V1_2.IWifiChipEventCallback.RadioModeInfo;
import android.hardware.wifi.V1_3.WifiChannelStats;
import android.net.KeepalivePacketData;
import android.net.MacAddress;
import android.net.NattKeepalivePacketData;
import android.net.apf.ApfCapabilities;
import android.net.wifi.ScanResult;
import android.net.wifi.WifiManager;
import android.net.wifi.WifiScanner;
import android.net.wifi.WifiSsid;
import android.os.Handler;
import android.os.Looper;
import android.os.RemoteException;
import android.os.test.TestLooper;
import android.system.OsConstants;
import android.util.Pair;

import androidx.test.filters.SmallTest;

import com.android.server.wifi.HalDeviceManager.InterfaceDestroyedListener;
import com.android.server.wifi.WifiLinkLayerStats.ChannelStats;
import com.android.server.wifi.util.NativeUtil;

import org.junit.Before;
import org.junit.Test;
import org.mockito.ArgumentCaptor;
import org.mockito.InOrder;
import org.mockito.Mock;
import org.mockito.MockitoAnnotations;
import org.mockito.stubbing.Answer;

import java.net.InetAddress;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashSet;
import java.util.List;
import java.util.Random;
import java.util.Set;

/**
 * Unit tests for {@link com.android.server.wifi.WifiVendorHal}.
 */
@SmallTest
public class WifiVendorHalTest {

    private static final String TEST_IFACE_NAME = "wlan0";
    private static final String TEST_IFACE_NAME_1 = "wlan1";
    private static final MacAddress TEST_MAC_ADDRESS = MacAddress.fromString("ee:33:a2:94:10:92");
    private static final int SAR_SENSOR_INVALID_STATE = -6;
    private static final int[] TEST_FREQUENCIES =
            {2412, 2417, 2422, 2427, 2432, 2437};

    WifiVendorHal mWifiVendorHal;
    private WifiStatus mWifiStatusSuccess;
    private WifiStatus mWifiStatusFailure;
    private WifiStatus mWifiStatusBusy;
    WifiLog mWifiLog;
    @Mock
    private HalDeviceManager mHalDeviceManager;
    @Mock
    private TestLooper mLooper;
    @Mock
    private WifiVendorHal.HalDeviceManagerStatusListener mHalDeviceManagerStatusCallbacks;
    @Mock
    private IWifiApIface mIWifiApIface;
    @Mock
    private IWifiChip mIWifiChip;
    @Mock
    private android.hardware.wifi.V1_1.IWifiChip mIWifiChipV11;
    @Mock
    private android.hardware.wifi.V1_2.IWifiChip mIWifiChipV12;
    @Mock
    private android.hardware.wifi.V1_3.IWifiChip mIWifiChipV13;
    @Mock
    private IWifiStaIface mIWifiStaIface;
    @Mock
    private android.hardware.wifi.V1_2.IWifiStaIface mIWifiStaIfaceV12;
    @Mock
    private android.hardware.wifi.V1_3.IWifiStaIface mIWifiStaIfaceV13;
    private IWifiStaIfaceEventCallback mIWifiStaIfaceEventCallback;
    private IWifiChipEventCallback mIWifiChipEventCallback;
    private android.hardware.wifi.V1_2.IWifiChipEventCallback mIWifiChipEventCallbackV12;
    @Mock
    private WifiNative.VendorHalDeathEventHandler mVendorHalDeathHandler;
    @Mock
    private WifiNative.VendorHalRadioModeChangeEventHandler mVendorHalRadioModeChangeHandler;

    /**
     * Spy used to return the V1_1 IWifiChip mock object to simulate the 1.1 HAL running on the
     * device.
     */
    private class WifiVendorHalSpyV1_1 extends WifiVendorHal {
        WifiVendorHalSpyV1_1(HalDeviceManager halDeviceManager, Looper looper) {
            super(halDeviceManager, looper);
        }

        @Override
        protected android.hardware.wifi.V1_1.IWifiChip getWifiChipForV1_1Mockable() {
            return mIWifiChipV11;
        }

        @Override
        protected android.hardware.wifi.V1_2.IWifiChip getWifiChipForV1_2Mockable() {
            return null;
        }

        @Override
        protected android.hardware.wifi.V1_3.IWifiChip getWifiChipForV1_3Mockable() {
            return null;
        }

        @Override
        protected android.hardware.wifi.V1_2.IWifiStaIface getWifiStaIfaceForV1_2Mockable(
                String ifaceName) {
            return null;
        }

        @Override
        protected android.hardware.wifi.V1_3.IWifiStaIface getWifiStaIfaceForV1_3Mockable(
                String ifaceName) {
            return null;
        }
    }

    /**
     * Spy used to return the V1_2 IWifiChip and IWifiStaIface mock objects to simulate
     * the 1.2 HAL running on the device.
     */
    private class WifiVendorHalSpyV1_2 extends WifiVendorHal {
        WifiVendorHalSpyV1_2(HalDeviceManager halDeviceManager, Looper looper) {
            super(halDeviceManager, looper);
        }

        @Override
        protected android.hardware.wifi.V1_1.IWifiChip getWifiChipForV1_1Mockable() {
            return null;
        }

        @Override
        protected android.hardware.wifi.V1_2.IWifiChip getWifiChipForV1_2Mockable() {
            return mIWifiChipV12;
        }

        @Override
        protected android.hardware.wifi.V1_3.IWifiChip getWifiChipForV1_3Mockable() {
            return null;
        }

        @Override
        protected android.hardware.wifi.V1_2.IWifiStaIface getWifiStaIfaceForV1_2Mockable(
                String ifaceName) {
            return mIWifiStaIfaceV12;
        }

        @Override
        protected android.hardware.wifi.V1_3.IWifiStaIface getWifiStaIfaceForV1_3Mockable(
                String ifaceName) {
            return null;
        }
    }

    /**
     * Spy used to return the V1_3 IWifiChip and V1_3 IWifiStaIface mock objects to simulate
     * the 1.3 HAL running on the device.
     */
    private class WifiVendorHalSpyV1_3 extends WifiVendorHal {
        WifiVendorHalSpyV1_3(HalDeviceManager halDeviceManager, Looper looper) {
            super(halDeviceManager, looper);
        }

        @Override
        protected android.hardware.wifi.V1_1.IWifiChip getWifiChipForV1_1Mockable() {
            return null;
        }

        @Override
        protected android.hardware.wifi.V1_2.IWifiChip getWifiChipForV1_2Mockable() {
            return null;
        }

        @Override
        protected android.hardware.wifi.V1_3.IWifiChip getWifiChipForV1_3Mockable() {
            return mIWifiChipV13;
        }

        @Override
        protected android.hardware.wifi.V1_2.IWifiStaIface getWifiStaIfaceForV1_2Mockable(
                String ifaceName) {
            return mIWifiStaIfaceV12;
        }

        @Override
        protected android.hardware.wifi.V1_3.IWifiStaIface getWifiStaIfaceForV1_3Mockable(
                String ifaceName) {
            return mIWifiStaIfaceV13;
        }
    }

    /**
     * Identity function to supply a type to its argument, which is a lambda
     */
    static Answer<WifiStatus> answerWifiStatus(Answer<WifiStatus> statusLambda) {
        return (statusLambda);
    }

    /**
     * Sets up for unit test
     */
    @Before
    public void setUp() throws Exception {
        MockitoAnnotations.initMocks(this);
        mWifiLog = new FakeWifiLog();
        mLooper = new TestLooper();
        mWifiStatusSuccess = new WifiStatus();
        mWifiStatusSuccess.code = WifiStatusCode.SUCCESS;
        mWifiStatusFailure = new WifiStatus();
        mWifiStatusFailure.code = WifiStatusCode.ERROR_UNKNOWN;
        mWifiStatusFailure.description = "I don't even know what a Mock Turtle is.";
        mWifiStatusBusy = new WifiStatus();
        mWifiStatusBusy.code = WifiStatusCode.ERROR_BUSY;
        mWifiStatusBusy.description = "Don't bother me, kid";
        when(mIWifiStaIface.enableLinkLayerStatsCollection(false)).thenReturn(mWifiStatusSuccess);

        // Setup the HalDeviceManager mock's start/stop behaviour. This can be overridden in
        // individual tests, if needed.
        doAnswer(new AnswerWithArguments() {
            public boolean answer() {
                when(mHalDeviceManager.isReady()).thenReturn(true);
                when(mHalDeviceManager.isStarted()).thenReturn(true);
                mHalDeviceManagerStatusCallbacks.onStatusChanged();
                mLooper.dispatchAll();
                return true;
            }
        }).when(mHalDeviceManager).start();

        doAnswer(new AnswerWithArguments() {
            public void answer() {
                when(mHalDeviceManager.isReady()).thenReturn(true);
                when(mHalDeviceManager.isStarted()).thenReturn(false);
                mHalDeviceManagerStatusCallbacks.onStatusChanged();
                mLooper.dispatchAll();
            }
        }).when(mHalDeviceManager).stop();
        when(mHalDeviceManager.createStaIface(anyBoolean(), any(), eq(null)))
                .thenReturn(mIWifiStaIface);
        when(mHalDeviceManager.createApIface(any(), eq(null)))
                .thenReturn(mIWifiApIface);
        when(mHalDeviceManager.removeIface(any())).thenReturn(true);
        when(mHalDeviceManager.getChip(any(IWifiIface.class)))
                .thenReturn(mIWifiChip);
        when(mIWifiChip.registerEventCallback(any(IWifiChipEventCallback.class)))
                .thenReturn(mWifiStatusSuccess);
        mIWifiStaIfaceEventCallback = null;
        when(mIWifiStaIface.registerEventCallback(any(IWifiStaIfaceEventCallback.class)))
                .thenAnswer(answerWifiStatus((invocation) -> {
                    Object[] args = invocation.getArguments();
                    mIWifiStaIfaceEventCallback = (IWifiStaIfaceEventCallback) args[0];
                    return (mWifiStatusSuccess);
                }));
        when(mIWifiChip.registerEventCallback(any(IWifiChipEventCallback.class)))
                .thenAnswer(answerWifiStatus((invocation) -> {
                    Object[] args = invocation.getArguments();
                    mIWifiChipEventCallback = (IWifiChipEventCallback) args[0];
                    return (mWifiStatusSuccess);
                }));
        when(mIWifiChipV12.registerEventCallback_1_2(
                any(android.hardware.wifi.V1_2.IWifiChipEventCallback.class)))
                .thenAnswer(answerWifiStatus((invocation) -> {
                    Object[] args = invocation.getArguments();
                    mIWifiChipEventCallbackV12 =
                            (android.hardware.wifi.V1_2.IWifiChipEventCallback) args[0];
                    return (mWifiStatusSuccess);
                }));

        doAnswer(new AnswerWithArguments() {
            public void answer(IWifiIface.getNameCallback cb)
                    throws RemoteException {
                cb.onValues(mWifiStatusSuccess, TEST_IFACE_NAME);
            }
        }).when(mIWifiStaIface).getName(any(IWifiIface.getNameCallback.class));
        doAnswer(new AnswerWithArguments() {
            public void answer(IWifiIface.getNameCallback cb)
                    throws RemoteException {
                cb.onValues(mWifiStatusSuccess, TEST_IFACE_NAME);
            }
        }).when(mIWifiApIface).getName(any(IWifiIface.getNameCallback.class));

        // Create the vendor HAL object under test.
        mWifiVendorHal = new WifiVendorHal(mHalDeviceManager, mLooper.getLooper());

        // Initialize the vendor HAL to capture the registered callback.
        mWifiVendorHal.initialize(mVendorHalDeathHandler);
        ArgumentCaptor<WifiVendorHal.HalDeviceManagerStatusListener> hdmCallbackCaptor =
                ArgumentCaptor.forClass(WifiVendorHal.HalDeviceManagerStatusListener.class);
        verify(mHalDeviceManager).registerStatusListener(
                hdmCallbackCaptor.capture(), any(Handler.class));
        mHalDeviceManagerStatusCallbacks = hdmCallbackCaptor.getValue();

    }

    /**
     * Tests the successful starting of HAL in STA mode using
     * {@link WifiVendorHal#startVendorHalSta()}.
     */
    @Test
    public void testStartHalSuccessInStaMode() throws  Exception {
        assertTrue(mWifiVendorHal.startVendorHalSta());
        assertTrue(mWifiVendorHal.isHalStarted());

        verify(mHalDeviceManager).start();
        verify(mHalDeviceManager).createStaIface(eq(false), any(), eq(null));
        verify(mHalDeviceManager).getChip(eq(mIWifiStaIface));
        verify(mHalDeviceManager).isReady();
        verify(mHalDeviceManager).isStarted();
        verify(mIWifiStaIface).registerEventCallback(any(IWifiStaIfaceEventCallback.class));
        verify(mIWifiChip).registerEventCallback(any(IWifiChipEventCallback.class));

        verify(mHalDeviceManager, never()).createApIface(any(), eq(null));
    }

    /**
     * Tests the successful starting of HAL in AP mode using
     * {@link WifiVendorHal#startVendorHalAp()}.
     */
    @Test
    public void testStartHalSuccessInApMode() throws Exception {
        assertTrue(mWifiVendorHal.startVendorHalAp());
        assertTrue(mWifiVendorHal.isHalStarted());

        verify(mHalDeviceManager).start();
        verify(mHalDeviceManager).createApIface(any(), eq(null));
        verify(mHalDeviceManager).getChip(eq(mIWifiApIface));
        verify(mHalDeviceManager).isReady();
        verify(mHalDeviceManager).isStarted();

        verify(mHalDeviceManager, never()).createStaIface(anyBoolean(), any(), eq(null));
    }

    /**
     * Tests the failure to start HAL in STA mode using
     * {@link WifiVendorHal#startVendorHalSta()}.
     */
    @Test
    public void testStartHalFailureInStaMode() throws Exception {
        // No callbacks are invoked in this case since the start itself failed. So, override
        // default AnswerWithArguments that we setup.
        doAnswer(new AnswerWithArguments() {
            public boolean answer() throws Exception {
                return false;
            }
        }).when(mHalDeviceManager).start();
        assertFalse(mWifiVendorHal.startVendorHalSta());
        assertFalse(mWifiVendorHal.isHalStarted());

        verify(mHalDeviceManager).start();

        verify(mHalDeviceManager, never()).createStaIface(anyBoolean(), any(), eq(null));
        verify(mHalDeviceManager, never()).createApIface(any(), eq(null));
        verify(mHalDeviceManager, never()).getChip(any(IWifiIface.class));
        verify(mIWifiStaIface, never())
                .registerEventCallback(any(IWifiStaIfaceEventCallback.class));
    }

    /**
     * Tests the failure to start HAL in STA mode using
     * {@link WifiVendorHal#startVendorHalSta()}.
     */
    @Test
    public void testStartHalFailureInIfaceCreationInStaMode() throws Exception {
        when(mHalDeviceManager.createStaIface(anyBoolean(), any(), eq(null))).thenReturn(null);
        assertFalse(mWifiVendorHal.startVendorHalSta());
        assertFalse(mWifiVendorHal.isHalStarted());

        verify(mHalDeviceManager).start();
        verify(mHalDeviceManager).createStaIface(eq(false), any(), eq(null));
        verify(mHalDeviceManager).stop();

        verify(mHalDeviceManager, never()).createApIface(any(), eq(null));
        verify(mHalDeviceManager, never()).getChip(any(IWifiIface.class));
        verify(mIWifiStaIface, never())
                .registerEventCallback(any(IWifiStaIfaceEventCallback.class));
    }

    /**
     * Tests the failure to start HAL in STA mode using
     * {@link WifiVendorHal#startVendorHalSta()}.
     */
    @Test
    public void testStartHalFailureInChipGetInStaMode() throws Exception {
        when(mHalDeviceManager.getChip(any(IWifiIface.class))).thenReturn(null);
        assertFalse(mWifiVendorHal.startVendorHalSta());
        assertFalse(mWifiVendorHal.isHalStarted());

        verify(mHalDeviceManager).start();
        verify(mHalDeviceManager).createStaIface(eq(false), any(), eq(null));
        verify(mHalDeviceManager).getChip(any(IWifiIface.class));
        verify(mHalDeviceManager).stop();
        verify(mIWifiStaIface).registerEventCallback(any(IWifiStaIfaceEventCallback.class));

        verify(mHalDeviceManager, never()).createApIface(any(), eq(null));
    }

    /**
     * Tests the failure to start HAL in STA mode using
     * {@link WifiVendorHal#startVendorHalSta()}.
     */
    @Test
    public void testStartHalFailureInStaIfaceCallbackRegistration() throws Exception {
        when(mIWifiStaIface.registerEventCallback(any(IWifiStaIfaceEventCallback.class)))
                .thenReturn(mWifiStatusFailure);
        assertFalse(mWifiVendorHal.startVendorHalSta());
        assertFalse(mWifiVendorHal.isHalStarted());

        verify(mHalDeviceManager).start();
        verify(mHalDeviceManager).createStaIface(eq(false), any(), eq(null));
        verify(mHalDeviceManager).stop();
        verify(mIWifiStaIface).registerEventCallback(any(IWifiStaIfaceEventCallback.class));

        verify(mHalDeviceManager, never()).getChip(any(IWifiIface.class));
        verify(mHalDeviceManager, never()).createApIface(any(), eq(null));
    }

    /**
     * Tests the failure to start HAL in STA mode using
     * {@link WifiVendorHal#startVendorHalSta()}.
     */
    @Test
    public void testStartHalFailureInChipCallbackRegistration() throws Exception {
        when(mIWifiChip.registerEventCallback(any(IWifiChipEventCallback.class)))
                .thenReturn(mWifiStatusFailure);
        assertFalse(mWifiVendorHal.startVendorHalSta());
        assertFalse(mWifiVendorHal.isHalStarted());

        verify(mHalDeviceManager).start();
        verify(mHalDeviceManager).createStaIface(eq(false), any(), eq(null));
        verify(mHalDeviceManager).getChip(any(IWifiIface.class));
        verify(mHalDeviceManager).stop();
        verify(mIWifiStaIface).registerEventCallback(any(IWifiStaIfaceEventCallback.class));
        verify(mIWifiChip).registerEventCallback(any(IWifiChipEventCallback.class));

        verify(mHalDeviceManager, never()).createApIface(any(), eq(null));
    }

    /**
     * Tests the failure to start HAL in AP mode using
     * {@link WifiVendorHal#startVendorHalAp()}.
     */
    @Test
    public void testStartHalFailureInApMode() throws Exception {
        when(mHalDeviceManager.createApIface(any(), eq(null))).thenReturn(null);
        assertFalse(mWifiVendorHal.startVendorHalAp());
        assertFalse(mWifiVendorHal.isHalStarted());

        verify(mHalDeviceManager).start();
        verify(mHalDeviceManager).createApIface(any(), eq(null));
        verify(mHalDeviceManager).stop();

        verify(mHalDeviceManager, never()).createStaIface(anyBoolean(), any(), eq(null));
        verify(mHalDeviceManager, never()).getChip(any(IWifiIface.class));
    }

    /**
     * Tests the stopping of HAL in STA mode using
     * {@link WifiVendorHal#stopVendorHal()}.
     */
    @Test
    public void testStopHalInStaMode() {
        assertTrue(mWifiVendorHal.startVendorHalSta());
        assertTrue(mWifiVendorHal.isHalStarted());

        mWifiVendorHal.stopVendorHal();
        assertFalse(mWifiVendorHal.isHalStarted());

        verify(mHalDeviceManager).start();
        verify(mHalDeviceManager).stop();
        verify(mHalDeviceManager).createStaIface(eq(false), any(), eq(null));
        verify(mHalDeviceManager).getChip(eq(mIWifiStaIface));
        verify(mHalDeviceManager, times(2)).isReady();
        verify(mHalDeviceManager, times(2)).isStarted();

        verify(mHalDeviceManager, never()).createApIface(any(), eq(null));
    }

    /**
     * Tests the stopping of HAL in AP mode using
     * {@link WifiVendorHal#stopVendorHal()}.
     */
    @Test
    public void testStopHalInApMode() {
        assertTrue(mWifiVendorHal.startVendorHalAp());
        assertTrue(mWifiVendorHal.isHalStarted());

        mWifiVendorHal.stopVendorHal();
        assertFalse(mWifiVendorHal.isHalStarted());

        verify(mHalDeviceManager).start();
        verify(mHalDeviceManager).stop();
        verify(mHalDeviceManager).createApIface(any(), eq(null));
        verify(mHalDeviceManager).getChip(eq(mIWifiApIface));
        verify(mHalDeviceManager, times(2)).isReady();
        verify(mHalDeviceManager, times(2)).isStarted();

        verify(mHalDeviceManager, never()).createStaIface(anyBoolean(), any(), eq(null));
    }

    /**
     * Tests the handling of interface destroyed callback from HalDeviceManager.
     */
    @Test
    public void testStaInterfaceDestroyedHandling() throws  Exception {
        ArgumentCaptor<InterfaceDestroyedListener> internalListenerCaptor =
                ArgumentCaptor.forClass(InterfaceDestroyedListener.class);
        InterfaceDestroyedListener externalLister = mock(InterfaceDestroyedListener.class);

        assertTrue(mWifiVendorHal.startVendorHal());
        assertNotNull(mWifiVendorHal.createStaIface(false, externalLister));
        assertTrue(mWifiVendorHal.isHalStarted());

        verify(mHalDeviceManager).start();
        verify(mHalDeviceManager).createStaIface(eq(false), internalListenerCaptor.capture(),
                eq(null));
        verify(mHalDeviceManager).getChip(eq(mIWifiStaIface));
        verify(mHalDeviceManager).isReady();
        verify(mHalDeviceManager).isStarted();
        verify(mIWifiStaIface).registerEventCallback(any(IWifiStaIfaceEventCallback.class));
        verify(mIWifiChip).registerEventCallback(any(IWifiChipEventCallback.class));

        // Now trigger the interface destroyed callback from HalDeviceManager and ensure the
        // external listener is invoked and iface removed from internal database.
        internalListenerCaptor.getValue().onDestroyed(TEST_IFACE_NAME);
        verify(externalLister).onDestroyed(TEST_IFACE_NAME);

        // This should fail now, since the interface was already destroyed.
        assertFalse(mWifiVendorHal.removeStaIface(TEST_IFACE_NAME));
        verify(mHalDeviceManager, never()).removeIface(any());
    }

    /**
     * Tests the handling of interface destroyed callback from HalDeviceManager.
     */
    @Test
    public void testApInterfaceDestroyedHandling() throws  Exception {
        ArgumentCaptor<InterfaceDestroyedListener> internalListenerCaptor =
                ArgumentCaptor.forClass(InterfaceDestroyedListener.class);
        InterfaceDestroyedListener externalLister = mock(InterfaceDestroyedListener.class);

        assertTrue(mWifiVendorHal.startVendorHal());
        assertNotNull(mWifiVendorHal.createApIface(externalLister));
        assertTrue(mWifiVendorHal.isHalStarted());

        verify(mHalDeviceManager).start();
        verify(mHalDeviceManager).createApIface(internalListenerCaptor.capture(), eq(null));
        verify(mHalDeviceManager).getChip(eq(mIWifiApIface));
        verify(mHalDeviceManager).isReady();
        verify(mHalDeviceManager).isStarted();
        verify(mIWifiChip).registerEventCallback(any(IWifiChipEventCallback.class));

        // Now trigger the interface destroyed callback from HalDeviceManager and ensure the
        // external listener is invoked and iface removed from internal database.
        internalListenerCaptor.getValue().onDestroyed(TEST_IFACE_NAME);
        verify(externalLister).onDestroyed(TEST_IFACE_NAME);

        // This should fail now, since the interface was already destroyed.
        assertFalse(mWifiVendorHal.removeApIface(TEST_IFACE_NAME));
        verify(mHalDeviceManager, never()).removeIface(any());
    }

    /**
     * Test that enter logs when verbose logging is enabled
     */
    @Test
    public void testEnterLogging() {
        mWifiVendorHal.mLog = spy(mWifiLog);
        mWifiVendorHal.enableVerboseLogging(true);
        mWifiVendorHal.installPacketFilter(TEST_IFACE_NAME, new byte[0]);
        verify(mWifiVendorHal.mLog).trace(eq("filter length %"), eq(1));
    }

    /**
     * Test that enter does not log when verbose logging is not enabled
     */
    @Test
    public void testEnterSilenceWhenNotEnabled() {
        mWifiVendorHal.mLog = spy(mWifiLog);
        mWifiVendorHal.installPacketFilter(TEST_IFACE_NAME, new byte[0]);
        mWifiVendorHal.enableVerboseLogging(true);
        mWifiVendorHal.enableVerboseLogging(false);
        mWifiVendorHal.installPacketFilter(TEST_IFACE_NAME, new byte[0]);
        verify(mWifiVendorHal.mLog, never()).trace(eq("filter length %"), anyInt());
    }

    /**
     * Test that boolResult logs a false result
     */
    @Test
    public void testBoolResultFalse() {
        mWifiLog = spy(mWifiLog);
        mWifiVendorHal.mLog = mWifiLog;
        mWifiVendorHal.mVerboseLog = mWifiLog;
        assertFalse(mWifiVendorHal.getBgScanCapabilities(
                TEST_IFACE_NAME, new WifiNative.ScanCapabilities()));
        verify(mWifiLog).err("% returns %");
    }

    /**
     * Test that getBgScanCapabilities is hooked up to the HAL correctly
     *
     * A call before the vendor HAL is started should return a non-null result with version 0
     *
     * A call after the HAL is started should return the mocked values.
     */
    @Test
    public void testGetBgScanCapabilities() throws Exception {
        StaBackgroundScanCapabilities capabilities = new StaBackgroundScanCapabilities();
        capabilities.maxCacheSize = 12;
        capabilities.maxBuckets = 34;
        capabilities.maxApCachePerScan = 56;
        capabilities.maxReportingThreshold = 78;

        doAnswer(new AnswerWithArguments() {
            public void answer(IWifiStaIface.getBackgroundScanCapabilitiesCallback cb)
                    throws RemoteException {
                cb.onValues(mWifiStatusSuccess, capabilities);
            }
        }).when(mIWifiStaIface).getBackgroundScanCapabilities(any(
                IWifiStaIface.getBackgroundScanCapabilitiesCallback.class));

        WifiNative.ScanCapabilities result = new WifiNative.ScanCapabilities();

        // should fail - not started
        assertFalse(mWifiVendorHal.getBgScanCapabilities(TEST_IFACE_NAME, result));
        // Start the vendor hal
        assertTrue(mWifiVendorHal.startVendorHalSta());
        // should succeed
        assertTrue(mWifiVendorHal.getBgScanCapabilities(TEST_IFACE_NAME, result));

        assertEquals(12, result.max_scan_cache_size);
        assertEquals(34, result.max_scan_buckets);
        assertEquals(56, result.max_ap_cache_per_scan);
        assertEquals(78, result.max_scan_reporting_threshold);
    }

    /**
     * Test translation to WifiManager.WIFI_FEATURE_*
     *
     * Just do a spot-check with a few feature bits here; since the code is table-
     * driven we don't have to work hard to exercise all of it.
     */
    @Test
    public void testStaIfaceFeatureMaskTranslation() {
        int caps = (
                IWifiStaIface.StaIfaceCapabilityMask.BACKGROUND_SCAN
                | IWifiStaIface.StaIfaceCapabilityMask.LINK_LAYER_STATS
            );
        long expected = (
                WifiManager.WIFI_FEATURE_SCANNER
                | WifiManager.WIFI_FEATURE_LINK_LAYER_STATS);
        assertEquals(expected, mWifiVendorHal.wifiFeatureMaskFromStaCapabilities(caps));
    }

    /**
     * Test translation to WifiManager.WIFI_FEATURE_*
     *
     * Just do a spot-check with a few feature bits here; since the code is table-
     * driven we don't have to work hard to exercise all of it.
     */
    @Test
    public void testChipFeatureMaskTranslation() {
        int caps = (
                android.hardware.wifi.V1_1.IWifiChip.ChipCapabilityMask.SET_TX_POWER_LIMIT
                        | android.hardware.wifi.V1_1.IWifiChip.ChipCapabilityMask.D2D_RTT
                        | android.hardware.wifi.V1_1.IWifiChip.ChipCapabilityMask.D2AP_RTT
        );
        long expected = (
                WifiManager.WIFI_FEATURE_TX_POWER_LIMIT
                        | WifiManager.WIFI_FEATURE_D2D_RTT
                        | WifiManager.WIFI_FEATURE_D2AP_RTT
        );
        assertEquals(expected, mWifiVendorHal.wifiFeatureMaskFromChipCapabilities(caps));
    }

    /**
     * Test translation to WifiManager.WIFI_FEATURE_* for V1.3
     *
     * Test the added features in V1.3
     */
    @Test
    public void testChipFeatureMaskTranslation_1_3() {
        int caps = (
                android.hardware.wifi.V1_3.IWifiChip.ChipCapabilityMask.SET_LATENCY_MODE
                        | android.hardware.wifi.V1_1.IWifiChip.ChipCapabilityMask.D2D_RTT
        );
        long expected = (
                WifiManager.WIFI_FEATURE_LOW_LATENCY
                        | WifiManager.WIFI_FEATURE_D2D_RTT
        );
        assertEquals(expected, mWifiVendorHal.wifiFeatureMaskFromChipCapabilities_1_3(caps));
    }

    /**
     * Test get supported features. Tests whether we coalesce information from different sources
     * (IWifiStaIface, IWifiChip and HalDeviceManager) into the bitmask of supported features
     * correctly.
     */
    @Test
    public void testGetSupportedFeatures() throws Exception {
        assertTrue(mWifiVendorHal.startVendorHalSta());

        int staIfaceHidlCaps = (
                IWifiStaIface.StaIfaceCapabilityMask.BACKGROUND_SCAN
                        | IWifiStaIface.StaIfaceCapabilityMask.LINK_LAYER_STATS
        );
        int chipHidlCaps =
                android.hardware.wifi.V1_1.IWifiChip.ChipCapabilityMask.SET_TX_POWER_LIMIT;
        Set<Integer>  halDeviceManagerSupportedIfaces = new HashSet<Integer>() {{
                add(IfaceType.STA);
                add(IfaceType.P2P);
            }};
        long expectedFeatureSet = (
                WifiManager.WIFI_FEATURE_SCANNER
                        | WifiManager.WIFI_FEATURE_LINK_LAYER_STATS
                        | WifiManager.WIFI_FEATURE_TX_POWER_LIMIT
                        | WifiManager.WIFI_FEATURE_INFRA
                        | WifiManager.WIFI_FEATURE_P2P
        );

        doAnswer(new AnswerWithArguments() {
            public void answer(IWifiStaIface.getCapabilitiesCallback cb) throws RemoteException {
                cb.onValues(mWifiStatusSuccess, staIfaceHidlCaps);
            }
        }).when(mIWifiStaIface).getCapabilities(any(IWifiStaIface.getCapabilitiesCallback.class));
        doAnswer(new AnswerWithArguments() {
            public void answer(IWifiChip.getCapabilitiesCallback cb) throws RemoteException {
                cb.onValues(mWifiStatusSuccess, chipHidlCaps);
            }
        }).when(mIWifiChip).getCapabilities(any(IWifiChip.getCapabilitiesCallback.class));
        when(mHalDeviceManager.getSupportedIfaceTypes())
                .thenReturn(halDeviceManagerSupportedIfaces);

        assertEquals(expectedFeatureSet, mWifiVendorHal.getSupportedFeatureSet(TEST_IFACE_NAME));
    }

    /**
     * Test |getFactoryMacAddress| gets called when the hal version is V1_3
     * @throws Exception
     */
    @Test
    public void testGetFactoryMacWithHalV1_3() throws Exception {
        doAnswer(new AnswerWithArguments() {
            public void answer(
                    android.hardware.wifi.V1_3.IWifiStaIface.getFactoryMacAddressCallback cb)
                    throws RemoteException {
                cb.onValues(mWifiStatusSuccess, MacAddress.BROADCAST_ADDRESS.toByteArray());
            }
        }).when(mIWifiStaIfaceV13).getFactoryMacAddress(any(
                android.hardware.wifi.V1_3.IWifiStaIface.getFactoryMacAddressCallback.class));
        mWifiVendorHal = new WifiVendorHalSpyV1_3(mHalDeviceManager, mLooper.getLooper());
        assertEquals(MacAddress.BROADCAST_ADDRESS.toString(),
                mWifiVendorHal.getFactoryMacAddress(TEST_IFACE_NAME).toString());
        verify(mIWifiStaIfaceV13).getFactoryMacAddress(any());
    }

    /**
     * Test enablement of link layer stats after startup
     *
     * Request link layer stats before HAL start
     * - should not make it to the HAL layer
     * Start the HAL in STA mode
     * Request link layer stats twice more
     * - enable request should make it to the HAL layer
     * - HAL layer should have been called to make the requests (i.e., two calls total)
     */
    @Test
    public void testLinkLayerStatsEnableAfterStartup() throws Exception {
        doNothing().when(mIWifiStaIface).getLinkLayerStats(any());

        assertNull(mWifiVendorHal.getWifiLinkLayerStats(TEST_IFACE_NAME));
        assertTrue(mWifiVendorHal.startVendorHalSta());
        assertTrue(mWifiVendorHal.isHalStarted());

        verify(mHalDeviceManager).start();
        mWifiVendorHal.getWifiLinkLayerStats(TEST_IFACE_NAME);
        mWifiVendorHal.getWifiLinkLayerStats(TEST_IFACE_NAME);
        verify(mIWifiStaIface).enableLinkLayerStatsCollection(false); // mLinkLayerStatsDebug
        verify(mIWifiStaIface, times(2)).getLinkLayerStats(any());
    }

    /**
     * Test getLinkLayerStats_1_3 gets called when the hal version is V1_3.
     */
    @Test
    public void testLinkLayerStatsCorrectVersionWithHalV1_3() throws Exception {
        mWifiVendorHal = new WifiVendorHalSpyV1_3(mHalDeviceManager, mLooper.getLooper());
        mWifiVendorHal.getWifiLinkLayerStats(TEST_IFACE_NAME);
        verify(mIWifiStaIfaceV13).getLinkLayerStats_1_3(any());
    }

    /**
     * Test that link layer stats are not enabled and harmless in AP mode
     *
     * Start the HAL in AP mode
     * - stats should not be enabled
     * Request link layer stats
     * - HAL layer should have been called to make the request
     */
    @Test
    public void testLinkLayerStatsNotEnabledAndHarmlessInApMode() throws Exception {
        doNothing().when(mIWifiStaIface).getLinkLayerStats(any());

        assertTrue(mWifiVendorHal.startVendorHalAp());
        assertTrue(mWifiVendorHal.isHalStarted());
        assertNull(mWifiVendorHal.getWifiLinkLayerStats(TEST_IFACE_NAME));

        verify(mHalDeviceManager).start();

        verify(mIWifiStaIface, never()).enableLinkLayerStatsCollection(false);
        verify(mIWifiStaIface, never()).getLinkLayerStats(any());
    }

    /**
     * Test that the link layer stats fields are populated correctly.
     *
     * This is done by filling Hal LinkLayerStats (V1_0) with random values, converting it to
     * WifiLinkLayerStats and then asserting the values in the original structure are equal to the
     * values in the converted structure.
     */
    @Test
    public void testLinkLayerStatsAssignment() throws Exception {
        Random r = new Random(1775968256);
        StaLinkLayerStats stats = new StaLinkLayerStats();
        randomizePacketStats(r, stats.iface.wmeBePktStats);
        randomizePacketStats(r, stats.iface.wmeBkPktStats);
        randomizePacketStats(r, stats.iface.wmeViPktStats);
        randomizePacketStats(r, stats.iface.wmeVoPktStats);
        randomizeRadioStats(r, stats.radios);
        stats.timeStampInMs = r.nextLong() & 0xFFFFFFFFFFL;

        WifiLinkLayerStats converted = WifiVendorHal.frameworkFromHalLinkLayerStats(stats);

        verifyIFaceStats(stats.iface, converted);
        verifyRadioStats(stats.radios, converted);
        assertEquals(stats.timeStampInMs, converted.timeStampInMs);
        assertEquals(WifiLinkLayerStats.V1_0, converted.version);
    }

    /**
     * Test that the link layer stats V1_3 fields are populated correctly.
     *
     * This is done by filling Hal LinkLayerStats (V1_3) with random values, converting it to
     * WifiLinkLayerStats and then asserting the values in the original structure are equal to the
     * values in the converted structure.
     */
    @Test
    public void testLinkLayerStatsAssignment_1_3() throws Exception {
        Random r = new Random(1775968256);
        android.hardware.wifi.V1_3.StaLinkLayerStats stats =
                new android.hardware.wifi.V1_3.StaLinkLayerStats();
        randomizePacketStats(r, stats.iface.wmeBePktStats);
        randomizePacketStats(r, stats.iface.wmeBkPktStats);
        randomizePacketStats(r, stats.iface.wmeViPktStats);
        randomizePacketStats(r, stats.iface.wmeVoPktStats);
        randomizeRadioStats_1_3(r, stats.radios);
        stats.timeStampInMs = r.nextLong() & 0xFFFFFFFFFFL;

        WifiLinkLayerStats converted = WifiVendorHal.frameworkFromHalLinkLayerStats_1_3(stats);

        verifyIFaceStats(stats.iface, converted);
        verifyRadioStats_1_3(stats.radios, converted);
        assertEquals(stats.timeStampInMs, converted.timeStampInMs);
        assertEquals(WifiLinkLayerStats.V1_3, converted.version);
    }

    private void verifyIFaceStats(StaLinkLayerIfaceStats iface,
            WifiLinkLayerStats wifiLinkLayerStats) {
        assertEquals(iface.beaconRx, wifiLinkLayerStats.beacon_rx);
        assertEquals(iface.avgRssiMgmt, wifiLinkLayerStats.rssi_mgmt);

        assertEquals(iface.wmeBePktStats.rxMpdu, wifiLinkLayerStats.rxmpdu_be);
        assertEquals(iface.wmeBePktStats.txMpdu, wifiLinkLayerStats.txmpdu_be);
        assertEquals(iface.wmeBePktStats.lostMpdu, wifiLinkLayerStats.lostmpdu_be);
        assertEquals(iface.wmeBePktStats.retries, wifiLinkLayerStats.retries_be);

        assertEquals(iface.wmeBkPktStats.rxMpdu, wifiLinkLayerStats.rxmpdu_bk);
        assertEquals(iface.wmeBkPktStats.txMpdu, wifiLinkLayerStats.txmpdu_bk);
        assertEquals(iface.wmeBkPktStats.lostMpdu, wifiLinkLayerStats.lostmpdu_bk);
        assertEquals(iface.wmeBkPktStats.retries, wifiLinkLayerStats.retries_bk);

        assertEquals(iface.wmeViPktStats.rxMpdu, wifiLinkLayerStats.rxmpdu_vi);
        assertEquals(iface.wmeViPktStats.txMpdu, wifiLinkLayerStats.txmpdu_vi);
        assertEquals(iface.wmeViPktStats.lostMpdu, wifiLinkLayerStats.lostmpdu_vi);
        assertEquals(iface.wmeViPktStats.retries, wifiLinkLayerStats.retries_vi);

        assertEquals(iface.wmeVoPktStats.rxMpdu, wifiLinkLayerStats.rxmpdu_vo);
        assertEquals(iface.wmeVoPktStats.txMpdu, wifiLinkLayerStats.txmpdu_vo);
        assertEquals(iface.wmeVoPktStats.lostMpdu, wifiLinkLayerStats.lostmpdu_vo);
        assertEquals(iface.wmeVoPktStats.retries, wifiLinkLayerStats.retries_vo);
    }

    private void verifyRadioStats(List<StaLinkLayerRadioStats> radios,
            WifiLinkLayerStats wifiLinkLayerStats) {
        StaLinkLayerRadioStats radio = radios.get(0);
        assertEquals(radio.onTimeInMs, wifiLinkLayerStats.on_time);
        assertEquals(radio.txTimeInMs, wifiLinkLayerStats.tx_time);
        assertEquals(radio.rxTimeInMs, wifiLinkLayerStats.rx_time);
        assertEquals(radio.onTimeInMsForScan, wifiLinkLayerStats.on_time_scan);
        assertEquals(radio.txTimeInMsPerLevel.size(),
                wifiLinkLayerStats.tx_time_per_level.length);
        for (int i = 0; i < radio.txTimeInMsPerLevel.size(); i++) {
            assertEquals((int) radio.txTimeInMsPerLevel.get(i),
                    wifiLinkLayerStats.tx_time_per_level[i]);
        }
    }

    private void verifyRadioStats_1_3(
            List<android.hardware.wifi.V1_3.StaLinkLayerRadioStats> radios,
            WifiLinkLayerStats wifiLinkLayerStats) {
        android.hardware.wifi.V1_3.StaLinkLayerRadioStats radio = radios.get(0);
        assertEquals(radio.V1_0.onTimeInMs, wifiLinkLayerStats.on_time);
        assertEquals(radio.V1_0.txTimeInMs, wifiLinkLayerStats.tx_time);
        assertEquals(radio.V1_0.rxTimeInMs, wifiLinkLayerStats.rx_time);
        assertEquals(radio.V1_0.onTimeInMsForScan, wifiLinkLayerStats.on_time_scan);
        assertEquals(radio.V1_0.txTimeInMsPerLevel.size(),
                wifiLinkLayerStats.tx_time_per_level.length);
        for (int i = 0; i < radio.V1_0.txTimeInMsPerLevel.size(); i++) {
            assertEquals((int) radio.V1_0.txTimeInMsPerLevel.get(i),
                    wifiLinkLayerStats.tx_time_per_level[i]);
        }
        assertEquals(radio.onTimeInMsForNanScan, wifiLinkLayerStats.on_time_nan_scan);
        assertEquals(radio.onTimeInMsForBgScan, wifiLinkLayerStats.on_time_background_scan);
        assertEquals(radio.onTimeInMsForRoamScan, wifiLinkLayerStats.on_time_roam_scan);
        assertEquals(radio.onTimeInMsForPnoScan, wifiLinkLayerStats.on_time_pno_scan);
        assertEquals(radio.onTimeInMsForHs20Scan, wifiLinkLayerStats.on_time_hs20_scan);
        assertEquals(radio.channelStats.size(),
                wifiLinkLayerStats.channelStatsMap.size());
        for (int j = 0; j < radio.channelStats.size(); j++) {
            WifiChannelStats channelStats = radio.channelStats.get(j);
            ChannelStats retrievedChannelStats =
                    wifiLinkLayerStats.channelStatsMap.get(channelStats.channel.centerFreq);
            assertNotNull(retrievedChannelStats);
            assertEquals(channelStats.channel.centerFreq, retrievedChannelStats.frequency);
            assertEquals(channelStats.onTimeInMs, retrievedChannelStats.radioOnTimeMs);
            assertEquals(channelStats.ccaBusyTimeInMs, retrievedChannelStats.ccaBusyTimeMs);
        }
    }


    /**
     * Populate packet stats with non-negative random values
     */
    private static void randomizePacketStats(Random r, StaLinkLayerIfacePacketStats pstats) {
        pstats.rxMpdu = r.nextLong() & 0xFFFFFFFFFFL; // more than 32 bits
        pstats.txMpdu = r.nextLong() & 0xFFFFFFFFFFL;
        pstats.lostMpdu = r.nextLong() & 0xFFFFFFFFFFL;
        pstats.retries = r.nextLong() & 0xFFFFFFFFFFL;
    }

    /**
     * Populate radio stats with non-negative random values
     */
    private static void randomizeRadioStats(Random r, ArrayList<StaLinkLayerRadioStats> rstats) {
        StaLinkLayerRadioStats rstat = new StaLinkLayerRadioStats();
        rstat.onTimeInMs = r.nextInt() & 0xFFFFFF;
        rstat.txTimeInMs = r.nextInt() & 0xFFFFFF;
        for (int i = 0; i < 4; i++) {
            Integer v = r.nextInt() & 0xFFFFFF;
            rstat.txTimeInMsPerLevel.add(v);
        }
        rstat.rxTimeInMs = r.nextInt() & 0xFFFFFF;
        rstat.onTimeInMsForScan = r.nextInt() & 0xFFFFFF;
        rstats.add(rstat);
    }

    /**
     * Populate radio stats with non-negative random values
     */
    private static void randomizeRadioStats_1_3(Random r,
            ArrayList<android.hardware.wifi.V1_3.StaLinkLayerRadioStats> rstats) {
        android.hardware.wifi.V1_3.StaLinkLayerRadioStats rstat =
                new android.hardware.wifi.V1_3.StaLinkLayerRadioStats();
        rstat.V1_0.onTimeInMs = r.nextInt() & 0xFFFFFF;
        rstat.V1_0.txTimeInMs = r.nextInt() & 0xFFFFFF;
        for (int i = 0; i < 4; i++) {
            Integer v = r.nextInt() & 0xFFFFFF;
            rstat.V1_0.txTimeInMsPerLevel.add(v);
        }
        rstat.V1_0.rxTimeInMs = r.nextInt() & 0xFFFFFF;
        rstat.V1_0.onTimeInMsForScan = r.nextInt() & 0xFFFFFF;
        rstat.onTimeInMsForNanScan = r.nextInt() & 0xFFFFFF;
        rstat.onTimeInMsForBgScan = r.nextInt() & 0xFFFFFF;
        rstat.onTimeInMsForRoamScan = r.nextInt() & 0xFFFFFF;
        rstat.onTimeInMsForPnoScan = r.nextInt() & 0xFFFFFF;
        rstat.onTimeInMsForHs20Scan = r.nextInt() & 0xFFFFFF;
        for (int j = 0; j < TEST_FREQUENCIES.length; j++) {
            WifiChannelStats channelStats = new WifiChannelStats();
            channelStats.channel.centerFreq = TEST_FREQUENCIES[j];
            channelStats.onTimeInMs = r.nextInt() & 0xFFFFFF;
            channelStats.ccaBusyTimeInMs = r.nextInt() & 0xFFFFFF;
            rstat.channelStats.add(channelStats);
        }
        rstats.add(rstat);
    }

    /**
     * Test that getFirmwareVersion() and getDriverVersion() work
     *
     * Calls before the STA is started are expected to return null.
     */
    @Test
    public void testVersionGetters() throws Exception {
        String firmwareVersion = "fuzzy";
        String driverVersion = "dizzy";
        IWifiChip.ChipDebugInfo chipDebugInfo = new IWifiChip.ChipDebugInfo();
        chipDebugInfo.firmwareDescription = firmwareVersion;
        chipDebugInfo.driverDescription = driverVersion;

        doAnswer(new AnswerWithArguments() {
            public void answer(IWifiChip.requestChipDebugInfoCallback cb) throws RemoteException {
                cb.onValues(mWifiStatusSuccess, chipDebugInfo);
            }
        }).when(mIWifiChip).requestChipDebugInfo(any(IWifiChip.requestChipDebugInfoCallback.class));

        assertNull(mWifiVendorHal.getFirmwareVersion());
        assertNull(mWifiVendorHal.getDriverVersion());

        assertTrue(mWifiVendorHal.startVendorHalSta());

        assertEquals(firmwareVersion, mWifiVendorHal.getFirmwareVersion());
        assertEquals(driverVersion, mWifiVendorHal.getDriverVersion());
    }

    /**
     * For checkRoundTripIntTranslation lambdas
     */
    interface IntForInt {
        int translate(int value);
    }

    /**
     * Checks that translation from x to y and back again is the identity function
     *
     * @param xFromY reverse translator
     * @param yFromX forward translator
     * @param xLimit non-inclusive upper bound on x (lower bound is zero)
     */
    private void checkRoundTripIntTranslation(
            IntForInt xFromY, IntForInt yFromX, int xFirst, int xLimit) throws Exception {
        int ex = 0;
        for (int i = xFirst; i < xLimit; i++) {
            assertEquals(i, xFromY.translate(yFromX.translate(i)));
        }
        try {
            yFromX.translate(xLimit);
            assertTrue("expected an exception here", false);
        } catch (IllegalArgumentException e) {
            ex++;
        }
        try {
            xFromY.translate(yFromX.translate(xLimit - 1) + 1);
            assertTrue("expected an exception here", false);
        } catch (IllegalArgumentException e) {
            ex++;
        }
        assertEquals(2, ex);
    }

    /**
     * Test that setScanningMacOui is hooked up to the HAL correctly
     */
    @Test
    public void testSetScanningMacOui() throws Exception {
        byte[] oui = NativeUtil.macAddressOuiToByteArray("DA:A1:19");
        byte[] zzz = NativeUtil.macAddressOuiToByteArray("00:00:00");

        when(mIWifiStaIface.setScanningMacOui(any())).thenReturn(mWifiStatusSuccess);

        // expect fail - STA not started
        assertFalse(mWifiVendorHal.setScanningMacOui(TEST_IFACE_NAME, oui));
        assertTrue(mWifiVendorHal.startVendorHalSta());
        // expect fail - null
        assertFalse(mWifiVendorHal.setScanningMacOui(TEST_IFACE_NAME, null));
        // expect fail - len
        assertFalse(mWifiVendorHal.setScanningMacOui(TEST_IFACE_NAME, new byte[]{(byte) 1}));
        assertTrue(mWifiVendorHal.setScanningMacOui(TEST_IFACE_NAME, oui));
        assertTrue(mWifiVendorHal.setScanningMacOui(TEST_IFACE_NAME, zzz));

        verify(mIWifiStaIface).setScanningMacOui(eq(oui));
        verify(mIWifiStaIface).setScanningMacOui(eq(zzz));
    }

    @Test
    public void testStartSendingOffloadedPacket() throws Exception {
        byte[] srcMac = NativeUtil.macAddressToByteArray("4007b2088c81");
        byte[] dstMac = NativeUtil.macAddressToByteArray("4007b8675309");
        InetAddress src = InetAddress.parseNumericAddress("192.168.13.13");
        InetAddress dst = InetAddress.parseNumericAddress("93.184.216.34");
        int slot = 13;
        int millis = 16000;

        KeepalivePacketData kap =
                NattKeepalivePacketData.nattKeepalivePacket(src, 63000, dst, 4500);

        when(mIWifiStaIface.startSendingKeepAlivePackets(
                anyInt(), any(), anyShort(), any(), any(), anyInt()
        )).thenReturn(mWifiStatusSuccess);

        assertTrue(mWifiVendorHal.startVendorHalSta());
        assertTrue(0 == mWifiVendorHal.startSendingOffloadedPacket(
                TEST_IFACE_NAME, slot, srcMac, dstMac, kap.getPacket(),
                OsConstants.ETH_P_IPV6, millis));

        verify(mIWifiStaIface).startSendingKeepAlivePackets(
                eq(slot), any(), anyShort(), any(), any(), eq(millis));
    }

    @Test
    public void testStopSendingOffloadedPacket() throws Exception {
        int slot = 13;

        when(mIWifiStaIface.stopSendingKeepAlivePackets(anyInt())).thenReturn(mWifiStatusSuccess);

        assertTrue(mWifiVendorHal.startVendorHalSta());
        assertTrue(0 == mWifiVendorHal.stopSendingOffloadedPacket(
                TEST_IFACE_NAME, slot));

        verify(mIWifiStaIface).stopSendingKeepAlivePackets(eq(slot));
    }

    /**
     * Test the setup, invocation, and removal of a RSSI event handler
     *
     */
    @Test
    public void testRssiMonitoring() throws Exception {
        when(mIWifiStaIface.startRssiMonitoring(anyInt(), anyInt(), anyInt()))
                .thenReturn(mWifiStatusSuccess);
        when(mIWifiStaIface.stopRssiMonitoring(anyInt()))
                .thenReturn(mWifiStatusSuccess);

        ArrayList<Byte> breach = new ArrayList<>(10);
        byte hi = -21;
        byte med = -42;
        byte lo = -84;
        Byte lower = -88;
        WifiNative.WifiRssiEventHandler handler;
        handler = ((cur) -> {
            breach.add(cur);
        });
        // not started
        assertEquals(-1, mWifiVendorHal.startRssiMonitoring(TEST_IFACE_NAME, hi, lo, handler));
        // not started
        assertEquals(-1, mWifiVendorHal.stopRssiMonitoring(TEST_IFACE_NAME));
        assertTrue(mWifiVendorHal.startVendorHalSta());
        assertEquals(0, mWifiVendorHal.startRssiMonitoring(TEST_IFACE_NAME, hi, lo, handler));
        int theCmdId = mWifiVendorHal.sRssiMonCmdId;
        breach.clear();
        mIWifiStaIfaceEventCallback.onRssiThresholdBreached(theCmdId, new byte[6], lower);
        assertEquals(breach.get(0), lower);
        assertEquals(0, mWifiVendorHal.stopRssiMonitoring(TEST_IFACE_NAME));
        assertEquals(0, mWifiVendorHal.startRssiMonitoring(TEST_IFACE_NAME, hi, lo, handler));
        // replacing works
        assertEquals(0, mWifiVendorHal.startRssiMonitoring(TEST_IFACE_NAME, med, lo, handler));
        // null handler fails
        assertEquals(-1, mWifiVendorHal.startRssiMonitoring(
                TEST_IFACE_NAME, hi, lo, null));
        assertEquals(0, mWifiVendorHal.startRssiMonitoring(TEST_IFACE_NAME, hi, lo, handler));
        // empty range
        assertEquals(-1, mWifiVendorHal.startRssiMonitoring(TEST_IFACE_NAME, lo, hi, handler));
    }

    /**
     * Test that getApfCapabilities is hooked up to the HAL correctly
     *
     * A call before the vendor HAL is started should return a non-null result with version 0
     *
     * A call after the HAL is started should return the mocked values.
     */
    @Test
    public void testApfCapabilities() throws Exception {
        int myVersion = 33;
        int myMaxSize = 1234;

        StaApfPacketFilterCapabilities capabilities = new StaApfPacketFilterCapabilities();
        capabilities.version = myVersion;
        capabilities.maxLength = myMaxSize;

        doAnswer(new AnswerWithArguments() {
            public void answer(IWifiStaIface.getApfPacketFilterCapabilitiesCallback cb)
                    throws RemoteException {
                cb.onValues(mWifiStatusSuccess, capabilities);
            }
        }).when(mIWifiStaIface).getApfPacketFilterCapabilities(any(
                IWifiStaIface.getApfPacketFilterCapabilitiesCallback.class));


        assertEquals(0, mWifiVendorHal.getApfCapabilities(TEST_IFACE_NAME)
                .apfVersionSupported);

        assertTrue(mWifiVendorHal.startVendorHalSta());

        ApfCapabilities actual = mWifiVendorHal.getApfCapabilities(TEST_IFACE_NAME);

        assertEquals(myVersion, actual.apfVersionSupported);
        assertEquals(myMaxSize, actual.maximumApfProgramSize);
        assertEquals(android.system.OsConstants.ARPHRD_ETHER, actual.apfPacketFormat);
        assertNotEquals(0, actual.apfPacketFormat);
    }

    /**
     * Test that an APF program can be installed.
     */
    @Test
    public void testInstallApf() throws Exception {
        byte[] filter = new byte[] {19, 53, 10};

        ArrayList<Byte> expected = new ArrayList<>(3);
        for (byte b : filter) expected.add(b);

        when(mIWifiStaIface.installApfPacketFilter(anyInt(), any(ArrayList.class)))
                .thenReturn(mWifiStatusSuccess);

        assertTrue(mWifiVendorHal.startVendorHalSta());
        assertTrue(mWifiVendorHal.installPacketFilter(TEST_IFACE_NAME, filter));

        verify(mIWifiStaIface).installApfPacketFilter(eq(0), eq(expected));
    }

    /**
     * Test that an APF program and data buffer can be read back.
     */
    @Test
    public void testReadApf() throws Exception {
        // Expose the 1.2 IWifiStaIface.
        mWifiVendorHal = new WifiVendorHalSpyV1_2(mHalDeviceManager, mLooper.getLooper());

        byte[] program = new byte[] {65, 66, 67};
        ArrayList<Byte> expected = new ArrayList<>(3);
        for (byte b : program) expected.add(b);

        doAnswer(new AnswerWithArguments() {
            public void answer(
                    android.hardware.wifi.V1_2.IWifiStaIface.readApfPacketFilterDataCallback cb)
                    throws RemoteException {
                cb.onValues(mWifiStatusSuccess, expected);
            }
        }).when(mIWifiStaIfaceV12).readApfPacketFilterData(any(
                android.hardware.wifi.V1_2.IWifiStaIface.readApfPacketFilterDataCallback.class));

        assertTrue(mWifiVendorHal.startVendorHalSta());
        assertArrayEquals(program, mWifiVendorHal.readPacketFilter(TEST_IFACE_NAME));
    }

    /**
     * Test that the country code is set in AP mode (when it should be).
     */
    @Test
    public void testSetCountryCodeHal() throws Exception {
        byte[] expected = new byte[]{(byte) 'C', (byte) 'A'};

        when(mIWifiApIface.setCountryCode(any()))
                .thenReturn(mWifiStatusSuccess);

        assertTrue(mWifiVendorHal.startVendorHalAp());

        assertFalse(mWifiVendorHal.setCountryCodeHal(TEST_IFACE_NAME, null));
        assertFalse(mWifiVendorHal.setCountryCodeHal(TEST_IFACE_NAME, ""));
        assertFalse(mWifiVendorHal.setCountryCodeHal(TEST_IFACE_NAME, "A"));
        // Only one expected to succeed
        assertTrue(mWifiVendorHal.setCountryCodeHal(TEST_IFACE_NAME, "CA"));
        assertFalse(mWifiVendorHal.setCountryCodeHal(TEST_IFACE_NAME, "ZZZ"));

        verify(mIWifiApIface).setCountryCode(eq(expected));
    }

    /**
     * Test that RemoteException is caught and logged.
     */
    @Test
    public void testRemoteExceptionIsHandled() throws Exception {
        mWifiLog = spy(mWifiLog);
        mWifiVendorHal.mVerboseLog = mWifiLog;
        when(mIWifiApIface.setCountryCode(any()))
                .thenThrow(new RemoteException("oops"));
        assertTrue(mWifiVendorHal.startVendorHalAp());
        assertFalse(mWifiVendorHal.setCountryCodeHal(TEST_IFACE_NAME, "CA"));
        assertFalse(mWifiVendorHal.isHalStarted());
        verify(mWifiLog).err("% RemoteException in HIDL call %");
    }

    /**
     * Test that startLoggingToDebugRingBuffer is plumbed to chip
     *
     * A call before the vendor hal is started should just return false.
     * After starting in STA mode, the call should succeed, and pass ther right things down.
     */
    @Test
    public void testStartLoggingRingBuffer() throws Exception {
        when(mIWifiChip.startLoggingToDebugRingBuffer(
                any(String.class), anyInt(), anyInt(), anyInt()
        )).thenReturn(mWifiStatusSuccess);

        assertFalse(mWifiVendorHal.startLoggingRingBuffer(1, 0x42, 0, 0, "One"));
        assertTrue(mWifiVendorHal.startVendorHalSta());
        assertTrue(mWifiVendorHal.startLoggingRingBuffer(1, 0x42, 11, 3000, "One"));

        verify(mIWifiChip).startLoggingToDebugRingBuffer("One", 1, 11, 3000);
    }

    /**
     * Same test as testStartLoggingRingBuffer, but in AP mode rather than STA.
     */
    @Test
    public void testStartLoggingRingBufferOnAp() throws Exception {
        when(mIWifiChip.startLoggingToDebugRingBuffer(
                any(String.class), anyInt(), anyInt(), anyInt()
        )).thenReturn(mWifiStatusSuccess);

        assertFalse(mWifiVendorHal.startLoggingRingBuffer(1, 0x42, 0, 0, "One"));
        assertTrue(mWifiVendorHal.startVendorHalAp());
        assertTrue(mWifiVendorHal.startLoggingRingBuffer(1, 0x42, 11, 3000, "One"));

        verify(mIWifiChip).startLoggingToDebugRingBuffer("One", 1, 11, 3000);
    }

    /**
     * Test that getRingBufferStatus gets and translates its stuff correctly
     */
    @Test
    public void testRingBufferStatus() throws Exception {
        WifiDebugRingBufferStatus one = new WifiDebugRingBufferStatus();
        one.ringName = "One";
        one.flags = WifiDebugRingBufferFlags.HAS_BINARY_ENTRIES;
        one.ringId = 5607371;
        one.sizeInBytes = 54321;
        one.freeSizeInBytes = 42;
        one.verboseLevel = WifiDebugRingBufferVerboseLevel.VERBOSE;
        String oneExpect = "name: One flag: 1 ringBufferId: 5607371 ringBufferByteSize: 54321"
                + " verboseLevel: 2 writtenBytes: 0 readBytes: 0 writtenRecords: 0";

        WifiDebugRingBufferStatus two = new WifiDebugRingBufferStatus();
        two.ringName = "Two";
        two.flags = WifiDebugRingBufferFlags.HAS_ASCII_ENTRIES
                | WifiDebugRingBufferFlags.HAS_PER_PACKET_ENTRIES;
        two.ringId = 4512470;
        two.sizeInBytes = 300;
        two.freeSizeInBytes = 42;
        two.verboseLevel = WifiDebugRingBufferVerboseLevel.DEFAULT;

        ArrayList<WifiDebugRingBufferStatus> halBufferStatus = new ArrayList<>(2);
        halBufferStatus.add(one);
        halBufferStatus.add(two);

        WifiNative.RingBufferStatus[] actual;

        doAnswer(new AnswerWithArguments() {
            public void answer(IWifiChip.getDebugRingBuffersStatusCallback cb)
                    throws RemoteException {
                cb.onValues(mWifiStatusSuccess, halBufferStatus);
            }
        }).when(mIWifiChip).getDebugRingBuffersStatus(any(
                IWifiChip.getDebugRingBuffersStatusCallback.class));

        assertTrue(mWifiVendorHal.startVendorHalSta());
        actual = mWifiVendorHal.getRingBufferStatus();

        assertEquals(halBufferStatus.size(), actual.length);
        assertEquals(oneExpect, actual[0].toString());
        assertEquals(two.ringId, actual[1].ringBufferId);
    }

    /**
     * Test that getRingBufferData calls forceDumpToDebugRingBuffer
     *
     * Try once before hal start, and twice after (one success, one failure).
     */
    @Test
    public void testForceRingBufferDump() throws Exception {
        when(mIWifiChip.forceDumpToDebugRingBuffer(eq("Gunk"))).thenReturn(mWifiStatusSuccess);
        when(mIWifiChip.forceDumpToDebugRingBuffer(eq("Glop"))).thenReturn(mWifiStatusFailure);

        assertFalse(mWifiVendorHal.getRingBufferData("Gunk")); // hal not started

        assertTrue(mWifiVendorHal.startVendorHalSta());

        assertTrue(mWifiVendorHal.getRingBufferData("Gunk")); // mocked call succeeds
        assertFalse(mWifiVendorHal.getRingBufferData("Glop")); // mocked call fails

        verify(mIWifiChip).forceDumpToDebugRingBuffer("Gunk");
        verify(mIWifiChip).forceDumpToDebugRingBuffer("Glop");
    }

    /**
     * Test flush ring buffer to files.
     *
     * Try once before hal start, and once after.
     */
    @Test
    public void testFlushRingBufferToFile() throws Exception {
        mWifiVendorHal = new WifiVendorHalSpyV1_3(mHalDeviceManager, mLooper.getLooper());
        when(mIWifiChipV13.flushRingBufferToFile()).thenReturn(mWifiStatusSuccess);

        assertFalse(mWifiVendorHal.flushRingBufferData());

        assertTrue(mWifiVendorHal.startVendorHalSta());
        assertTrue(mWifiVendorHal.flushRingBufferData());
        verify(mIWifiChipV13).flushRingBufferToFile();
    }

    /**
     * Tests the start of packet fate monitoring.
     *
     * Try once before hal start, and once after (one success, one failure).
     */
    @Test
    public void testStartPktFateMonitoring() throws Exception {
        when(mIWifiStaIface.startDebugPacketFateMonitoring()).thenReturn(mWifiStatusSuccess);

        assertFalse(mWifiVendorHal.startPktFateMonitoring(TEST_IFACE_NAME));
        verify(mIWifiStaIface, never()).startDebugPacketFateMonitoring();

        assertTrue(mWifiVendorHal.startVendorHalSta());
        assertTrue(mWifiVendorHal.startPktFateMonitoring(TEST_IFACE_NAME));
        verify(mIWifiStaIface).startDebugPacketFateMonitoring();
    }

    /**
     * Tests the retrieval of tx packet fates.
     *
     * Try once before hal start, and once after.
     */
    @Test
    public void testGetTxPktFates() throws Exception {
        byte[] frameContentBytes = new byte[30];
        new Random().nextBytes(frameContentBytes);
        WifiDebugTxPacketFateReport fateReport = new WifiDebugTxPacketFateReport();
        fateReport.fate = WifiDebugTxPacketFate.DRV_QUEUED;
        fateReport.frameInfo.driverTimestampUsec = new Random().nextLong();
        fateReport.frameInfo.frameType = WifiDebugPacketFateFrameType.ETHERNET_II;
        fateReport.frameInfo.frameContent.addAll(
                NativeUtil.byteArrayToArrayList(frameContentBytes));

        doAnswer(new AnswerWithArguments() {
            public void answer(IWifiStaIface.getDebugTxPacketFatesCallback cb) {
                cb.onValues(mWifiStatusSuccess,
                        new ArrayList<WifiDebugTxPacketFateReport>(Arrays.asList(fateReport)));
            }
        }).when(mIWifiStaIface)
                .getDebugTxPacketFates(any(IWifiStaIface.getDebugTxPacketFatesCallback.class));

        WifiNative.TxFateReport[] retrievedFates = new WifiNative.TxFateReport[1];
        assertFalse(mWifiVendorHal.getTxPktFates(TEST_IFACE_NAME, retrievedFates));
        verify(mIWifiStaIface, never())
                .getDebugTxPacketFates(any(IWifiStaIface.getDebugTxPacketFatesCallback.class));

        assertTrue(mWifiVendorHal.startVendorHalSta());

        assertTrue(mWifiVendorHal.getTxPktFates(TEST_IFACE_NAME, retrievedFates));
        verify(mIWifiStaIface)
                .getDebugTxPacketFates(any(IWifiStaIface.getDebugTxPacketFatesCallback.class));
        assertEquals(WifiLoggerHal.TX_PKT_FATE_DRV_QUEUED, retrievedFates[0].mFate);
        assertEquals(fateReport.frameInfo.driverTimestampUsec,
                retrievedFates[0].mDriverTimestampUSec);
        assertEquals(WifiLoggerHal.FRAME_TYPE_ETHERNET_II, retrievedFates[0].mFrameType);
        assertArrayEquals(frameContentBytes, retrievedFates[0].mFrameBytes);
    }

    /**
     * Tests the retrieval of tx packet fates when the number of fates retrieved exceeds the
     * input array.
     *
     * Try once before hal start, and once after.
     */
    @Test
    public void testGetTxPktFatesExceedsInputArrayLength() throws Exception {
        byte[] frameContentBytes = new byte[30];
        new Random().nextBytes(frameContentBytes);
        WifiDebugTxPacketFateReport fateReport = new WifiDebugTxPacketFateReport();
        fateReport.fate = WifiDebugTxPacketFate.FW_DROP_OTHER;
        fateReport.frameInfo.driverTimestampUsec = new Random().nextLong();
        fateReport.frameInfo.frameType = WifiDebugPacketFateFrameType.MGMT_80211;
        fateReport.frameInfo.frameContent.addAll(
                NativeUtil.byteArrayToArrayList(frameContentBytes));

        doAnswer(new AnswerWithArguments() {
            public void answer(IWifiStaIface.getDebugTxPacketFatesCallback cb) {
                cb.onValues(mWifiStatusSuccess,
                        new ArrayList<WifiDebugTxPacketFateReport>(Arrays.asList(
                                fateReport, fateReport)));
            }
        }).when(mIWifiStaIface)
                .getDebugTxPacketFates(any(IWifiStaIface.getDebugTxPacketFatesCallback.class));

        WifiNative.TxFateReport[] retrievedFates = new WifiNative.TxFateReport[1];
        assertFalse(mWifiVendorHal.getTxPktFates(TEST_IFACE_NAME, retrievedFates));
        verify(mIWifiStaIface, never())
                .getDebugTxPacketFates(any(IWifiStaIface.getDebugTxPacketFatesCallback.class));

        assertTrue(mWifiVendorHal.startVendorHalSta());

        assertTrue(mWifiVendorHal.getTxPktFates(TEST_IFACE_NAME, retrievedFates));
        verify(mIWifiStaIface)
                .getDebugTxPacketFates(any(IWifiStaIface.getDebugTxPacketFatesCallback.class));
        assertEquals(WifiLoggerHal.TX_PKT_FATE_FW_DROP_OTHER, retrievedFates[0].mFate);
        assertEquals(fateReport.frameInfo.driverTimestampUsec,
                retrievedFates[0].mDriverTimestampUSec);
        assertEquals(WifiLoggerHal.FRAME_TYPE_80211_MGMT, retrievedFates[0].mFrameType);
        assertArrayEquals(frameContentBytes, retrievedFates[0].mFrameBytes);
    }

    /**
     * Tests the retrieval of rx packet fates.
     *
     * Try once before hal start, and once after.
     */
    @Test
    public void testGetRxPktFates() throws Exception {
        byte[] frameContentBytes = new byte[30];
        new Random().nextBytes(frameContentBytes);
        WifiDebugRxPacketFateReport fateReport = new WifiDebugRxPacketFateReport();
        fateReport.fate = WifiDebugRxPacketFate.SUCCESS;
        fateReport.frameInfo.driverTimestampUsec = new Random().nextLong();
        fateReport.frameInfo.frameType = WifiDebugPacketFateFrameType.ETHERNET_II;
        fateReport.frameInfo.frameContent.addAll(
                NativeUtil.byteArrayToArrayList(frameContentBytes));

        doAnswer(new AnswerWithArguments() {
            public void answer(IWifiStaIface.getDebugRxPacketFatesCallback cb) {
                cb.onValues(mWifiStatusSuccess,
                        new ArrayList<WifiDebugRxPacketFateReport>(Arrays.asList(fateReport)));
            }
        }).when(mIWifiStaIface)
                .getDebugRxPacketFates(any(IWifiStaIface.getDebugRxPacketFatesCallback.class));

        WifiNative.RxFateReport[] retrievedFates = new WifiNative.RxFateReport[1];
        assertFalse(mWifiVendorHal.getRxPktFates(TEST_IFACE_NAME, retrievedFates));
        verify(mIWifiStaIface, never())
                .getDebugRxPacketFates(any(IWifiStaIface.getDebugRxPacketFatesCallback.class));

        assertTrue(mWifiVendorHal.startVendorHalSta());

        assertTrue(mWifiVendorHal.getRxPktFates(TEST_IFACE_NAME, retrievedFates));
        verify(mIWifiStaIface)
                .getDebugRxPacketFates(any(IWifiStaIface.getDebugRxPacketFatesCallback.class));
        assertEquals(WifiLoggerHal.RX_PKT_FATE_SUCCESS, retrievedFates[0].mFate);
        assertEquals(fateReport.frameInfo.driverTimestampUsec,
                retrievedFates[0].mDriverTimestampUSec);
        assertEquals(WifiLoggerHal.FRAME_TYPE_ETHERNET_II, retrievedFates[0].mFrameType);
        assertArrayEquals(frameContentBytes, retrievedFates[0].mFrameBytes);
    }

    /**
     * Tests the retrieval of rx packet fates when the number of fates retrieved exceeds the
     * input array.
     *
     * Try once before hal start, and once after.
     */
    @Test
    public void testGetRxPktFatesExceedsInputArrayLength() throws Exception {
        byte[] frameContentBytes = new byte[30];
        new Random().nextBytes(frameContentBytes);
        WifiDebugRxPacketFateReport fateReport = new WifiDebugRxPacketFateReport();
        fateReport.fate = WifiDebugRxPacketFate.FW_DROP_FILTER;
        fateReport.frameInfo.driverTimestampUsec = new Random().nextLong();
        fateReport.frameInfo.frameType = WifiDebugPacketFateFrameType.MGMT_80211;
        fateReport.frameInfo.frameContent.addAll(
                NativeUtil.byteArrayToArrayList(frameContentBytes));

        doAnswer(new AnswerWithArguments() {
            public void answer(IWifiStaIface.getDebugRxPacketFatesCallback cb) {
                cb.onValues(mWifiStatusSuccess,
                        new ArrayList<WifiDebugRxPacketFateReport>(Arrays.asList(
                                fateReport, fateReport)));
            }
        }).when(mIWifiStaIface)
                .getDebugRxPacketFates(any(IWifiStaIface.getDebugRxPacketFatesCallback.class));

        WifiNative.RxFateReport[] retrievedFates = new WifiNative.RxFateReport[1];
        assertFalse(mWifiVendorHal.getRxPktFates(TEST_IFACE_NAME, retrievedFates));
        verify(mIWifiStaIface, never())
                .getDebugRxPacketFates(any(IWifiStaIface.getDebugRxPacketFatesCallback.class));

        assertTrue(mWifiVendorHal.startVendorHalSta());

        assertTrue(mWifiVendorHal.getRxPktFates(TEST_IFACE_NAME, retrievedFates));
        verify(mIWifiStaIface)
                .getDebugRxPacketFates(any(IWifiStaIface.getDebugRxPacketFatesCallback.class));
        assertEquals(WifiLoggerHal.RX_PKT_FATE_FW_DROP_FILTER, retrievedFates[0].mFate);
        assertEquals(fateReport.frameInfo.driverTimestampUsec,
                retrievedFates[0].mDriverTimestampUSec);
        assertEquals(WifiLoggerHal.FRAME_TYPE_80211_MGMT, retrievedFates[0].mFrameType);
        assertArrayEquals(frameContentBytes, retrievedFates[0].mFrameBytes);
    }

    /**
     * Tests the failure to retrieve tx packet fates when the input array is empty.
     */
    @Test
    public void testGetTxPktFatesEmptyInputArray() throws Exception {
        assertTrue(mWifiVendorHal.startVendorHalSta());
        assertFalse(mWifiVendorHal.getTxPktFates(TEST_IFACE_NAME, new WifiNative.TxFateReport[0]));
        verify(mIWifiStaIface, never())
                .getDebugTxPacketFates(any(IWifiStaIface.getDebugTxPacketFatesCallback.class));
    }

    /**
     * Tests the failure to retrieve rx packet fates when the input array is empty.
     */
    @Test
    public void testGetRxPktFatesEmptyInputArray() throws Exception {
        assertTrue(mWifiVendorHal.startVendorHalSta());
        assertFalse(mWifiVendorHal.getRxPktFates(TEST_IFACE_NAME, new WifiNative.RxFateReport[0]));
        verify(mIWifiStaIface, never())
                .getDebugRxPacketFates(any(IWifiStaIface.getDebugRxPacketFatesCallback.class));
    }

    /**
     * Tests the nd offload enable/disable.
     */
    @Test
    public void testEnableDisableNdOffload() throws Exception {
        when(mIWifiStaIface.enableNdOffload(anyBoolean())).thenReturn(mWifiStatusSuccess);

        assertFalse(mWifiVendorHal.configureNeighborDiscoveryOffload(TEST_IFACE_NAME, true));
        verify(mIWifiStaIface, never()).enableNdOffload(anyBoolean());

        assertTrue(mWifiVendorHal.startVendorHalSta());

        assertTrue(mWifiVendorHal.configureNeighborDiscoveryOffload(TEST_IFACE_NAME, true));
        verify(mIWifiStaIface).enableNdOffload(eq(true));
        assertTrue(mWifiVendorHal.configureNeighborDiscoveryOffload(TEST_IFACE_NAME, false));
        verify(mIWifiStaIface).enableNdOffload(eq(false));
    }

    /**
     * Tests the nd offload enable failure.
     */
    @Test
    public void testEnableNdOffloadFailure() throws Exception {
        when(mIWifiStaIface.enableNdOffload(eq(true))).thenReturn(mWifiStatusFailure);

        assertTrue(mWifiVendorHal.startVendorHalSta());

        assertFalse(mWifiVendorHal.configureNeighborDiscoveryOffload(TEST_IFACE_NAME, true));
        verify(mIWifiStaIface).enableNdOffload(eq(true));
    }

    /**
     * Helper class for mocking getRoamingCapabilities callback
     */
    private class GetRoamingCapabilitiesAnswer extends AnswerWithArguments {
        private final WifiStatus mStatus;
        private final StaRoamingCapabilities mCaps;

        GetRoamingCapabilitiesAnswer(WifiStatus status, StaRoamingCapabilities caps) {
            mStatus = status;
            mCaps = caps;
        }

        public void answer(IWifiStaIface.getRoamingCapabilitiesCallback cb) {
            cb.onValues(mStatus, mCaps);
        }
    }

    /**
     * Tests retrieval of firmware roaming capabilities
     */
    @Test
    public void testFirmwareRoamingCapabilityRetrieval() throws Exception {
        WifiNative.RoamingCapabilities roamingCapabilities = new WifiNative.RoamingCapabilities();
        assertTrue(mWifiVendorHal.startVendorHalSta());
        for (int i = 0; i < 4; i++) {
            int blacklistSize = i + 10;
            int whitelistSize = i * 3;
            StaRoamingCapabilities caps = new StaRoamingCapabilities();
            caps.maxBlacklistSize = blacklistSize;
            caps.maxWhitelistSize = whitelistSize;
            doAnswer(new GetRoamingCapabilitiesAnswer(mWifiStatusSuccess, caps))
                    .when(mIWifiStaIface).getRoamingCapabilities(
                            any(IWifiStaIface.getRoamingCapabilitiesCallback.class));
            assertTrue(mWifiVendorHal.getRoamingCapabilities(TEST_IFACE_NAME, roamingCapabilities));
            assertEquals(blacklistSize, roamingCapabilities.maxBlacklistSize);
            assertEquals(whitelistSize, roamingCapabilities.maxWhitelistSize);
        }
    }

    /**
     * Tests unsuccessful retrieval of firmware roaming capabilities
     */
    @Test
    public void testUnsuccessfulFirmwareRoamingCapabilityRetrieval() throws Exception {
        assertTrue(mWifiVendorHal.startVendorHalSta());
        int blacklistSize = 42;
        int whitelistSize = 17;
        WifiNative.RoamingCapabilities roamingCapabilities = new WifiNative.RoamingCapabilities();
        roamingCapabilities.maxBlacklistSize = blacklistSize;
        roamingCapabilities.maxWhitelistSize = whitelistSize;
        StaRoamingCapabilities caps = new StaRoamingCapabilities();
        caps.maxBlacklistSize = blacklistSize + 1; // different value here
        caps.maxWhitelistSize = whitelistSize + 1;

        // hal returns a failure status
        doAnswer(new GetRoamingCapabilitiesAnswer(mWifiStatusFailure, null))
                .when(mIWifiStaIface).getRoamingCapabilities(
                        any(IWifiStaIface.getRoamingCapabilitiesCallback.class));
        assertFalse(mWifiVendorHal.getRoamingCapabilities(TEST_IFACE_NAME, roamingCapabilities));
        // in failure cases, result container should not be changed
        assertEquals(blacklistSize, roamingCapabilities.maxBlacklistSize);
        assertEquals(whitelistSize, roamingCapabilities.maxWhitelistSize);

        // hal returns failure status, but supplies caps anyway
        doAnswer(new GetRoamingCapabilitiesAnswer(mWifiStatusFailure, caps))
                .when(mIWifiStaIface).getRoamingCapabilities(
                        any(IWifiStaIface.getRoamingCapabilitiesCallback.class));
        assertFalse(mWifiVendorHal.getRoamingCapabilities(TEST_IFACE_NAME, roamingCapabilities));
        // in failure cases, result container should not be changed
        assertEquals(blacklistSize, roamingCapabilities.maxBlacklistSize);
        assertEquals(whitelistSize, roamingCapabilities.maxWhitelistSize);

        // lost connection
        doThrow(new RemoteException())
                .when(mIWifiStaIface).getRoamingCapabilities(
                        any(IWifiStaIface.getRoamingCapabilitiesCallback.class));
        assertFalse(mWifiVendorHal.getRoamingCapabilities(TEST_IFACE_NAME, roamingCapabilities));
        // in failure cases, result container should not be changed
        assertEquals(blacklistSize, roamingCapabilities.maxBlacklistSize);
        assertEquals(whitelistSize, roamingCapabilities.maxWhitelistSize);
    }

    /**
     * Tests enableFirmwareRoaming successful enable
     */
    @Test
    public void testEnableFirmwareRoamingSuccess() throws Exception {
        assertTrue(mWifiVendorHal.startVendorHalSta());
        when(mIWifiStaIface.setRoamingState(eq(StaRoamingState.ENABLED)))
                .thenReturn(mWifiStatusSuccess);
        assertEquals(WifiNative.SET_FIRMWARE_ROAMING_SUCCESS,
                mWifiVendorHal.enableFirmwareRoaming(TEST_IFACE_NAME,
                                                     WifiNative.ENABLE_FIRMWARE_ROAMING));
    }

    /**
     * Tests enableFirmwareRoaming successful disable
     */
    @Test
    public void testDisbleFirmwareRoamingSuccess() throws Exception {
        assertTrue(mWifiVendorHal.startVendorHalSta());
        when(mIWifiStaIface.setRoamingState(eq(StaRoamingState.DISABLED)))
                .thenReturn(mWifiStatusSuccess);
        assertEquals(WifiNative.SET_FIRMWARE_ROAMING_SUCCESS,
                mWifiVendorHal.enableFirmwareRoaming(TEST_IFACE_NAME,
                                                     WifiNative.DISABLE_FIRMWARE_ROAMING));
    }

    /**
     * Tests enableFirmwareRoaming failure case - invalid argument
     */
    @Test
    public void testEnableFirmwareRoamingFailureInvalidArgument() throws Exception {
        final int badState = WifiNative.DISABLE_FIRMWARE_ROAMING
                + WifiNative.ENABLE_FIRMWARE_ROAMING + 1;
        assertTrue(mWifiVendorHal.startVendorHalSta());
        assertEquals(WifiNative.SET_FIRMWARE_ROAMING_FAILURE,
                mWifiVendorHal.enableFirmwareRoaming(TEST_IFACE_NAME, badState));
        verify(mIWifiStaIface, never()).setRoamingState(anyByte());
    }

    /**
     * Tests enableFirmwareRoaming failure case - busy
     */
    @Test
    public void testEnableFirmwareRoamingFailureBusy() throws Exception {
        assertTrue(mWifiVendorHal.startVendorHalSta());
        when(mIWifiStaIface.setRoamingState(anyByte()))
                .thenReturn(mWifiStatusBusy);
        assertEquals(WifiNative.SET_FIRMWARE_ROAMING_BUSY,
                mWifiVendorHal.enableFirmwareRoaming(TEST_IFACE_NAME,
                                                     WifiNative.ENABLE_FIRMWARE_ROAMING));
    }

    /**
     * Tests enableFirmwareRoaming generic failure case
     */
    @Test
    public void testEnableFirmwareRoamingFailure() throws Exception {
        assertTrue(mWifiVendorHal.startVendorHalSta());
        when(mIWifiStaIface.setRoamingState(anyByte()))
                .thenReturn(mWifiStatusFailure);
        assertEquals(WifiNative.SET_FIRMWARE_ROAMING_FAILURE,
                mWifiVendorHal.enableFirmwareRoaming(TEST_IFACE_NAME,
                                                     WifiNative.ENABLE_FIRMWARE_ROAMING));
    }

    /**
     * Tests enableFirmwareRoaming remote exception failure case
     */
    @Test
    public void testEnableFirmwareRoamingException() throws Exception {
        assertTrue(mWifiVendorHal.startVendorHalSta());
        doThrow(new RemoteException()).when(mIWifiStaIface).setRoamingState(anyByte());
        assertEquals(WifiNative.SET_FIRMWARE_ROAMING_FAILURE,
                mWifiVendorHal.enableFirmwareRoaming(TEST_IFACE_NAME,
                                                     WifiNative.ENABLE_FIRMWARE_ROAMING));
    }

    /**
     * Tests configureRoaming success
     */
    @Test
    public void testConfigureRoamingSuccess() throws Exception {
        assertTrue(mWifiVendorHal.startVendorHalSta());
        WifiNative.RoamingConfig roamingConfig = new WifiNative.RoamingConfig();
        roamingConfig.blacklistBssids = new ArrayList();
        roamingConfig.blacklistBssids.add("12:34:56:78:ca:fe");
        roamingConfig.whitelistSsids = new ArrayList();
        roamingConfig.whitelistSsids.add("\"xyzzy\"");
        roamingConfig.whitelistSsids.add("\"\u0F00 \u05D0\"");
        when(mIWifiStaIface.configureRoaming(any())).thenReturn(mWifiStatusSuccess);
        assertTrue(mWifiVendorHal.configureRoaming(TEST_IFACE_NAME, roamingConfig));
        verify(mIWifiStaIface).configureRoaming(any());
    }

    /**
     * Tests configureRoaming success with null lists
     */
    @Test
    public void testConfigureRoamingResetSuccess() throws Exception {
        assertTrue(mWifiVendorHal.startVendorHalSta());
        WifiNative.RoamingConfig roamingConfig = new WifiNative.RoamingConfig();
        when(mIWifiStaIface.configureRoaming(any())).thenReturn(mWifiStatusSuccess);
        assertTrue(mWifiVendorHal.configureRoaming(TEST_IFACE_NAME, roamingConfig));
        verify(mIWifiStaIface).configureRoaming(any());
    }

    /**
     * Tests configureRoaming failure when hal returns failure
     */
    @Test
    public void testConfigureRoamingFailure() throws Exception {
        assertTrue(mWifiVendorHal.startVendorHalSta());
        WifiNative.RoamingConfig roamingConfig = new WifiNative.RoamingConfig();
        when(mIWifiStaIface.configureRoaming(any())).thenReturn(mWifiStatusFailure);
        assertFalse(mWifiVendorHal.configureRoaming(TEST_IFACE_NAME, roamingConfig));
        verify(mIWifiStaIface).configureRoaming(any());
    }

    /**
     * Tests configureRoaming failure due to remote exception
     */
    @Test
    public void testConfigureRoamingRemoteException() throws Exception {
        assertTrue(mWifiVendorHal.startVendorHalSta());
        WifiNative.RoamingConfig roamingConfig = new WifiNative.RoamingConfig();
        doThrow(new RemoteException()).when(mIWifiStaIface).configureRoaming(any());
        assertFalse(mWifiVendorHal.configureRoaming(TEST_IFACE_NAME, roamingConfig));
        verify(mIWifiStaIface).configureRoaming(any());
    }

    /**
     * Tests configureRoaming failure due to invalid bssid
     */
    @Test
    public void testConfigureRoamingBadBssid() throws Exception {
        assertTrue(mWifiVendorHal.startVendorHalSta());
        WifiNative.RoamingConfig roamingConfig = new WifiNative.RoamingConfig();
        roamingConfig.blacklistBssids = new ArrayList();
        roamingConfig.blacklistBssids.add("12:34:56:78:zz:zz");
        when(mIWifiStaIface.configureRoaming(any())).thenReturn(mWifiStatusSuccess);
        assertFalse(mWifiVendorHal.configureRoaming(TEST_IFACE_NAME, roamingConfig));
        verify(mIWifiStaIface, never()).configureRoaming(any());
    }

    /**
     * Tests configureRoaming failure due to invalid ssid
     */
    @Test
    public void testConfigureRoamingBadSsid() throws Exception {
        assertTrue(mWifiVendorHal.startVendorHalSta());
        WifiNative.RoamingConfig roamingConfig = new WifiNative.RoamingConfig();
        roamingConfig.whitelistSsids = new ArrayList();
        // Add an SSID that is too long (> 32 bytes) due to the multi-byte utf-8 characters
        roamingConfig.whitelistSsids.add("\"123456789012345678901234567890\u0F00\u05D0\"");
        when(mIWifiStaIface.configureRoaming(any())).thenReturn(mWifiStatusSuccess);
        assertFalse(mWifiVendorHal.configureRoaming(TEST_IFACE_NAME, roamingConfig));
        verify(mIWifiStaIface, never()).configureRoaming(any());
    }

    /**
     * Tests the retrieval of wlan wake reason stats.
     */
    @Test
    public void testGetWlanWakeReasonCount() throws Exception {
        WifiDebugHostWakeReasonStats stats = new WifiDebugHostWakeReasonStats();
        Random rand = new Random();
        stats.totalCmdEventWakeCnt = rand.nextInt();
        stats.totalDriverFwLocalWakeCnt = rand.nextInt();
        stats.totalRxPacketWakeCnt = rand.nextInt();
        stats.rxPktWakeDetails.rxUnicastCnt = rand.nextInt();
        stats.rxPktWakeDetails.rxMulticastCnt = rand.nextInt();
        stats.rxIcmpPkWakeDetails.icmpPkt = rand.nextInt();
        stats.rxIcmpPkWakeDetails.icmp6Pkt = rand.nextInt();
        stats.rxMulticastPkWakeDetails.ipv4RxMulticastAddrCnt = rand.nextInt();
        stats.rxMulticastPkWakeDetails.ipv6RxMulticastAddrCnt = rand.nextInt();

        doAnswer(new AnswerWithArguments() {
            public void answer(IWifiChip.getDebugHostWakeReasonStatsCallback cb) {
                cb.onValues(mWifiStatusSuccess, stats);
            }
        }).when(mIWifiChip).getDebugHostWakeReasonStats(
                any(IWifiChip.getDebugHostWakeReasonStatsCallback.class));

        assertNull(mWifiVendorHal.getWlanWakeReasonCount());
        verify(mIWifiChip, never())
                .getDebugHostWakeReasonStats(
                        any(IWifiChip.getDebugHostWakeReasonStatsCallback.class));

        assertTrue(mWifiVendorHal.startVendorHalSta());

        WlanWakeReasonAndCounts retrievedStats = mWifiVendorHal.getWlanWakeReasonCount();
        verify(mIWifiChip).getDebugHostWakeReasonStats(
                any(IWifiChip.getDebugHostWakeReasonStatsCallback.class));
        assertNotNull(retrievedStats);
        assertEquals(stats.totalCmdEventWakeCnt, retrievedStats.totalCmdEventWake);
        assertEquals(stats.totalDriverFwLocalWakeCnt, retrievedStats.totalDriverFwLocalWake);
        assertEquals(stats.totalRxPacketWakeCnt, retrievedStats.totalRxDataWake);
        assertEquals(stats.rxPktWakeDetails.rxUnicastCnt, retrievedStats.rxUnicast);
        assertEquals(stats.rxPktWakeDetails.rxMulticastCnt, retrievedStats.rxMulticast);
        assertEquals(stats.rxIcmpPkWakeDetails.icmpPkt, retrievedStats.icmp);
        assertEquals(stats.rxIcmpPkWakeDetails.icmp6Pkt, retrievedStats.icmp6);
        assertEquals(stats.rxMulticastPkWakeDetails.ipv4RxMulticastAddrCnt,
                retrievedStats.ipv4RxMulticast);
        assertEquals(stats.rxMulticastPkWakeDetails.ipv6RxMulticastAddrCnt,
                retrievedStats.ipv6Multicast);
    }

    /**
     * Tests the failure in retrieval of wlan wake reason stats.
     */
    @Test
    public void testGetWlanWakeReasonCountFailure() throws Exception {
        doAnswer(new AnswerWithArguments() {
            public void answer(IWifiChip.getDebugHostWakeReasonStatsCallback cb) {
                cb.onValues(mWifiStatusFailure, new WifiDebugHostWakeReasonStats());
            }
        }).when(mIWifiChip).getDebugHostWakeReasonStats(
                any(IWifiChip.getDebugHostWakeReasonStatsCallback.class));

        // This should work in both AP & STA mode.
        assertTrue(mWifiVendorHal.startVendorHalAp());

        assertNull(mWifiVendorHal.getWlanWakeReasonCount());
        verify(mIWifiChip).getDebugHostWakeReasonStats(
                any(IWifiChip.getDebugHostWakeReasonStatsCallback.class));
    }

    /**
     * Test that getFwMemoryDump is properly plumbed
     */
    @Test
    public void testGetFwMemoryDump() throws Exception {
        byte [] sample = NativeUtil.hexStringToByteArray("268c7a3fbfa4661c0bdd6a36");
        ArrayList<Byte> halBlob = NativeUtil.byteArrayToArrayList(sample);

        doAnswer(new AnswerWithArguments() {
            public void answer(IWifiChip.requestFirmwareDebugDumpCallback cb)
                    throws RemoteException {
                cb.onValues(mWifiStatusSuccess, halBlob);
            }
        }).when(mIWifiChip).requestFirmwareDebugDump(any(
                IWifiChip.requestFirmwareDebugDumpCallback.class));

        assertTrue(mWifiVendorHal.startVendorHalSta());
        assertArrayEquals(sample, mWifiVendorHal.getFwMemoryDump());
    }

    /**
     * Test that getDriverStateDump is properly plumbed
     *
     * Just for variety, use AP mode here.
     */
    @Test
    public void testGetDriverStateDump() throws Exception {
        byte [] sample = NativeUtil.hexStringToByteArray("e83ff543cf80083e6459d20f");
        ArrayList<Byte> halBlob = NativeUtil.byteArrayToArrayList(sample);

        doAnswer(new AnswerWithArguments() {
            public void answer(IWifiChip.requestDriverDebugDumpCallback cb)
                    throws RemoteException {
                cb.onValues(mWifiStatusSuccess, halBlob);
            }
        }).when(mIWifiChip).requestDriverDebugDump(any(
                IWifiChip.requestDriverDebugDumpCallback.class));

        assertTrue(mWifiVendorHal.startVendorHalAp());
        assertArrayEquals(sample, mWifiVendorHal.getDriverStateDump());
    }

    /**
     * Test that background scan failure is handled correctly.
     */
    @Test
    public void testBgScanFailureCallback() throws Exception {
        assertTrue(mWifiVendorHal.startVendorHalSta());
        assertNotNull(mIWifiStaIfaceEventCallback);

        WifiNative.ScanEventHandler eventHandler = mock(WifiNative.ScanEventHandler.class);
        startBgScan(eventHandler);

        mIWifiStaIfaceEventCallback.onBackgroundScanFailure(mWifiVendorHal.mScan.cmdId);
        verify(eventHandler).onScanStatus(WifiNative.WIFI_SCAN_FAILED);
    }

    /**
     * Test that background scan failure with wrong id is not reported.
     */
    @Test
    public void testBgScanFailureCallbackWithInvalidCmdId() throws Exception {
        assertTrue(mWifiVendorHal.startVendorHalSta());
        assertNotNull(mIWifiStaIfaceEventCallback);

        WifiNative.ScanEventHandler eventHandler = mock(WifiNative.ScanEventHandler.class);
        startBgScan(eventHandler);

        mIWifiStaIfaceEventCallback.onBackgroundScanFailure(mWifiVendorHal.mScan.cmdId + 1);
        verify(eventHandler, never()).onScanStatus(WifiNative.WIFI_SCAN_FAILED);
    }

    /**
     * Test that background scan full results are handled correctly.
     */
    @Test
    public void testBgScanFullScanResults() throws Exception {
        assertTrue(mWifiVendorHal.startVendorHalSta());
        assertNotNull(mIWifiStaIfaceEventCallback);

        WifiNative.ScanEventHandler eventHandler = mock(WifiNative.ScanEventHandler.class);
        startBgScan(eventHandler);

        Pair<StaScanResult, ScanResult> result = createHidlAndFrameworkBgScanResult();
        mIWifiStaIfaceEventCallback.onBackgroundFullScanResult(
                mWifiVendorHal.mScan.cmdId, 5, result.first);

        ArgumentCaptor<ScanResult> scanResultCaptor = ArgumentCaptor.forClass(ScanResult.class);
        verify(eventHandler).onFullScanResult(scanResultCaptor.capture(), eq(5));

        assertScanResultEqual(result.second, scanResultCaptor.getValue());
    }

    /**
     * Test that background scan results are handled correctly.
     */
    @Test
    public void testBgScanScanResults() throws Exception {
        assertTrue(mWifiVendorHal.startVendorHalSta());
        assertNotNull(mIWifiStaIfaceEventCallback);

        WifiNative.ScanEventHandler eventHandler = mock(WifiNative.ScanEventHandler.class);
        startBgScan(eventHandler);

        Pair<ArrayList<StaScanData>, ArrayList<WifiScanner.ScanData>> data =
                createHidlAndFrameworkBgScanDatas();
        mIWifiStaIfaceEventCallback.onBackgroundScanResults(
                mWifiVendorHal.mScan.cmdId, data.first);

        verify(eventHandler).onScanStatus(WifiNative.WIFI_SCAN_RESULTS_AVAILABLE);
        assertScanDatasEqual(
                data.second, Arrays.asList(mWifiVendorHal.mScan.latestScanResults));
    }

    /**
     * Test that starting a new background scan when one is active will stop the previous one.
     */
    @Test
    public void testBgScanReplacement() throws Exception {
        when(mIWifiStaIface.stopBackgroundScan(anyInt())).thenReturn(mWifiStatusSuccess);
        assertTrue(mWifiVendorHal.startVendorHalSta());
        assertNotNull(mIWifiStaIfaceEventCallback);
        WifiNative.ScanEventHandler eventHandler = mock(WifiNative.ScanEventHandler.class);
        startBgScan(eventHandler);
        int cmdId1 = mWifiVendorHal.mScan.cmdId;
        startBgScan(eventHandler);
        assertNotEquals(mWifiVendorHal.mScan.cmdId, cmdId1);
        verify(mIWifiStaIface, times(2)).startBackgroundScan(anyInt(), any());
        verify(mIWifiStaIface).stopBackgroundScan(cmdId1);
    }

    /**
     * Test stopping a background scan.
     */
    @Test
    public void testBgScanStop() throws Exception {
        when(mIWifiStaIface.stopBackgroundScan(anyInt())).thenReturn(mWifiStatusSuccess);
        assertTrue(mWifiVendorHal.startVendorHalSta());
        assertNotNull(mIWifiStaIfaceEventCallback);
        WifiNative.ScanEventHandler eventHandler = mock(WifiNative.ScanEventHandler.class);
        startBgScan(eventHandler);

        int cmdId = mWifiVendorHal.mScan.cmdId;

        mWifiVendorHal.stopBgScan(TEST_IFACE_NAME);
        mWifiVendorHal.stopBgScan(TEST_IFACE_NAME); // second call should not do anything
        verify(mIWifiStaIface).stopBackgroundScan(cmdId); // Should be called just once
    }

    /**
     * Test pausing and restarting a background scan.
     */
    @Test
    public void testBgScanPauseAndRestart() throws Exception {
        when(mIWifiStaIface.stopBackgroundScan(anyInt())).thenReturn(mWifiStatusSuccess);
        assertTrue(mWifiVendorHal.startVendorHalSta());
        assertNotNull(mIWifiStaIfaceEventCallback);
        WifiNative.ScanEventHandler eventHandler = mock(WifiNative.ScanEventHandler.class);
        startBgScan(eventHandler);

        int cmdId = mWifiVendorHal.mScan.cmdId;

        mWifiVendorHal.pauseBgScan(TEST_IFACE_NAME);
        mWifiVendorHal.restartBgScan(TEST_IFACE_NAME);
        verify(mIWifiStaIface).stopBackgroundScan(cmdId); // Should be called just once
        verify(mIWifiStaIface, times(2)).startBackgroundScan(eq(cmdId), any());
    }

    /**
     * Test the handling of log handler set.
     */
    @Test
    public void testSetLogHandler() throws Exception {
        when(mIWifiChip.enableDebugErrorAlerts(anyBoolean())).thenReturn(mWifiStatusSuccess);

        WifiNative.WifiLoggerEventHandler eventHandler =
                mock(WifiNative.WifiLoggerEventHandler.class);

        assertFalse(mWifiVendorHal.setLoggingEventHandler(eventHandler));
        verify(mIWifiChip, never()).enableDebugErrorAlerts(anyBoolean());

        assertTrue(mWifiVendorHal.startVendorHalSta());

        assertTrue(mWifiVendorHal.setLoggingEventHandler(eventHandler));
        verify(mIWifiChip).enableDebugErrorAlerts(eq(true));
        reset(mIWifiChip);

        // Second call should fail.
        assertFalse(mWifiVendorHal.setLoggingEventHandler(eventHandler));
        verify(mIWifiChip, never()).enableDebugErrorAlerts(anyBoolean());
    }

    /**
     * Test the handling of log handler reset.
     */
    @Test
    public void testResetLogHandler() throws Exception {
        when(mIWifiChip.enableDebugErrorAlerts(anyBoolean())).thenReturn(mWifiStatusSuccess);
        when(mIWifiChip.stopLoggingToDebugRingBuffer()).thenReturn(mWifiStatusSuccess);

        assertFalse(mWifiVendorHal.resetLogHandler());
        verify(mIWifiChip, never()).enableDebugErrorAlerts(anyBoolean());
        verify(mIWifiChip, never()).stopLoggingToDebugRingBuffer();

        assertTrue(mWifiVendorHal.startVendorHalSta());

        // Now set and then reset.
        assertTrue(mWifiVendorHal.setLoggingEventHandler(
                mock(WifiNative.WifiLoggerEventHandler.class)));
        assertTrue(mWifiVendorHal.resetLogHandler());
        verify(mIWifiChip).enableDebugErrorAlerts(eq(false));
        verify(mIWifiChip).stopLoggingToDebugRingBuffer();
        reset(mIWifiChip);
    }

    /**
     * Test the handling of log handler reset.
     */
    @Test
    public void testResetLogHandlerAfterHalStop() throws Exception {
        when(mIWifiChip.enableDebugErrorAlerts(anyBoolean())).thenReturn(mWifiStatusSuccess);
        when(mIWifiChip.stopLoggingToDebugRingBuffer()).thenReturn(mWifiStatusSuccess);

        // Start in STA mode.
        assertTrue(mWifiVendorHal.startVendorHalSta());

        // Now set the log handler, succeeds.
        assertTrue(mWifiVendorHal.setLoggingEventHandler(
                mock(WifiNative.WifiLoggerEventHandler.class)));
        verify(mIWifiChip).enableDebugErrorAlerts(eq(true));

        // Stop
        mWifiVendorHal.stopVendorHal();

        // Reset the log handler after stop, not HAL methods invoked.
        assertFalse(mWifiVendorHal.resetLogHandler());
        verify(mIWifiChip, never()).enableDebugErrorAlerts(eq(false));
        verify(mIWifiChip, never()).stopLoggingToDebugRingBuffer();

        // Start in STA mode again.
        assertTrue(mWifiVendorHal.startVendorHalSta());

        // Now set the log handler again, should succeed.
        assertTrue(mWifiVendorHal.setLoggingEventHandler(
                mock(WifiNative.WifiLoggerEventHandler.class)));
        verify(mIWifiChip, times(2)).enableDebugErrorAlerts(eq(true));
    }

    /**
     * Test the handling of alert callback.
     */
    @Test
    public void testAlertCallback() throws Exception {
        assertTrue(mWifiVendorHal.startVendorHalSta());
        assertNotNull(mIWifiChipEventCallback);

        testAlertCallbackUsingProvidedCallback(mIWifiChipEventCallback);
    }

    /**
     * Test the handling of ring buffer callback.
     */
    @Test
    public void testRingBufferDataCallback() throws Exception {
        when(mIWifiChip.enableDebugErrorAlerts(anyBoolean())).thenReturn(mWifiStatusSuccess);
        when(mIWifiChip.stopLoggingToDebugRingBuffer()).thenReturn(mWifiStatusSuccess);

        assertTrue(mWifiVendorHal.startVendorHalSta());
        assertNotNull(mIWifiChipEventCallback);

        byte[] errorData = new byte[45];
        new Random().nextBytes(errorData);

        // Randomly raise the HIDL callback before we register for the log callback.
        // This should be safely ignored. (Not trigger NPE.)
        mIWifiChipEventCallback.onDebugRingBufferDataAvailable(
                new WifiDebugRingBufferStatus(), NativeUtil.byteArrayToArrayList(errorData));
        mLooper.dispatchAll();

        WifiNative.WifiLoggerEventHandler eventHandler =
                mock(WifiNative.WifiLoggerEventHandler.class);
        assertTrue(mWifiVendorHal.setLoggingEventHandler(eventHandler));
        verify(mIWifiChip).enableDebugErrorAlerts(eq(true));

        // Now raise the HIDL callback, this should be properly handled.
        mIWifiChipEventCallback.onDebugRingBufferDataAvailable(
                new WifiDebugRingBufferStatus(), NativeUtil.byteArrayToArrayList(errorData));
        mLooper.dispatchAll();
        verify(eventHandler).onRingBufferData(
                any(WifiNative.RingBufferStatus.class), eq(errorData));

        // Now stop the logging and invoke the callback. This should be ignored.
        reset(eventHandler);
        assertTrue(mWifiVendorHal.resetLogHandler());
        mIWifiChipEventCallback.onDebugRingBufferDataAvailable(
                new WifiDebugRingBufferStatus(), NativeUtil.byteArrayToArrayList(errorData));
        mLooper.dispatchAll();
        verify(eventHandler, never()).onRingBufferData(anyObject(), anyObject());
    }

    /**
     * Test the handling of Vendor HAL death.
     */
    @Test
    public void testVendorHalDeath() {
        // Invoke the HAL device manager status callback with ready set to false to indicate the
        // death of the HAL.
        when(mHalDeviceManager.isReady()).thenReturn(false);
        mHalDeviceManagerStatusCallbacks.onStatusChanged();
        mLooper.dispatchAll();

        verify(mVendorHalDeathHandler).onDeath();
    }

    /**
     * Test the selectTxPowerScenario HIDL method invocation for 1.0 interface.
     * This should return failure since SAR is not supported for this interface version.
     */
    @Test
    public void testSelectTxPowerScenario_1_0() throws RemoteException {
        // Create a SAR info record (no sensor support)
        SarInfo sarInfo = new SarInfo();
        sarInfo.isVoiceCall = true;

        assertTrue(mWifiVendorHal.startVendorHalSta());
        // Should fail because we exposed the 1.0 IWifiChip.
        assertFalse(mWifiVendorHal.selectTxPowerScenario(sarInfo));
        verify(mIWifiChipV11, never()).selectTxPowerScenario(anyInt());
        mWifiVendorHal.stopVendorHal();
    }

    /**
     * Test the selectTxPowerScenario HIDL method invocation for 1.1 interface.
     * This should return success.
     */
    @Test
    public void testSelectTxPowerScenario_1_1() throws RemoteException {
        // Create a SAR info record (no sensor support)
        SarInfo sarInfo = new SarInfo();
        sarInfo.sarVoiceCallSupported = true;
        sarInfo.sarSapSupported = false;
        sarInfo.sarSensorSupported = false;

        sarInfo.isVoiceCall = true;

        // Now expose the 1.1 IWifiChip.
        mWifiVendorHal = new WifiVendorHalSpyV1_1(mHalDeviceManager, mLooper.getLooper());
        when(mIWifiChipV11.selectTxPowerScenario(anyInt())).thenReturn(mWifiStatusSuccess);

        assertTrue(mWifiVendorHal.startVendorHalSta());
        assertTrue(mWifiVendorHal.selectTxPowerScenario(sarInfo));
        verify(mIWifiChipV11).selectTxPowerScenario(
                eq(android.hardware.wifi.V1_1.IWifiChip.TxPowerScenario.VOICE_CALL));
        verify(mIWifiChipV11, never()).resetTxPowerScenario();
        mWifiVendorHal.stopVendorHal();
    }

   /**
     * Test the selectTxPowerScenario HIDL method invocation for 1.2 interface.
     * This should return success.
     */
    @Test
    public void testSelectTxPowerScenario_1_2() throws RemoteException {
        // Create a SAR info record (no sensor support)
        SarInfo sarInfo = new SarInfo();
        sarInfo.sarVoiceCallSupported = true;
        sarInfo.sarSapSupported = false;
        sarInfo.sarSensorSupported = false;

        sarInfo.isVoiceCall = true;

        // Now expose the 1.2 IWifiChip
        mWifiVendorHal = new WifiVendorHalSpyV1_2(mHalDeviceManager, mLooper.getLooper());
        when(mIWifiChipV12.selectTxPowerScenario_1_2(anyInt())).thenReturn(mWifiStatusSuccess);

        assertTrue(mWifiVendorHal.startVendorHalSta());
        assertTrue(mWifiVendorHal.selectTxPowerScenario(sarInfo));
        verify(mIWifiChipV12).selectTxPowerScenario_1_2(
                eq(android.hardware.wifi.V1_2.IWifiChip.TxPowerScenario.VOICE_CALL));
        verify(mIWifiChipV12, never()).resetTxPowerScenario();
        mWifiVendorHal.stopVendorHal();
    }

    /**
     * Test the resetTxPowerScenario HIDL method invocation for 1.0 interface.
     * This should return failure since it does not supprt SAR.
     */
    @Test
    public void testResetTxPowerScenario_1_0() throws RemoteException {
        // Create a SAR info record (no sensor support)
        SarInfo sarInfo = new SarInfo();

        assertTrue(mWifiVendorHal.startVendorHalSta());
        // Should fail because we exposed the 1.0 IWifiChip.
        assertFalse(mWifiVendorHal.selectTxPowerScenario(sarInfo));
        verify(mIWifiChipV11, never()).resetTxPowerScenario();
        mWifiVendorHal.stopVendorHal();
    }

    /**
     * Test the resetTxPowerScenario HIDL method invocation for 1.1 interface.
     * This should return success.
     */
    @Test
    public void testResetTxPowerScenario_1_1() throws RemoteException {
        // Create a SAR info record (no sensor support)
        SarInfo sarInfo = new SarInfo();
        sarInfo.sarVoiceCallSupported = true;
        sarInfo.sarSapSupported = false;
        sarInfo.sarSensorSupported = false;

        // Now expose the 1.1 IWifiChip.
        mWifiVendorHal = new WifiVendorHalSpyV1_1(mHalDeviceManager, mLooper.getLooper());
        when(mIWifiChipV11.resetTxPowerScenario()).thenReturn(mWifiStatusSuccess);

        assertTrue(mWifiVendorHal.startVendorHalSta());
        assertTrue(mWifiVendorHal.selectTxPowerScenario(sarInfo));
        verify(mIWifiChipV11).resetTxPowerScenario();
        verify(mIWifiChipV11, never()).selectTxPowerScenario(anyInt());
        mWifiVendorHal.stopVendorHal();
    }

    /**
     * Test resetting SAR scenario when not needed, should return true without invoking
     * the HAL method.
     * This is using HAL 1.1 interface.
     */
    @Test
    public void testResetTxPowerScenario_not_needed_1_1() throws RemoteException {
        InOrder inOrder = inOrder(mIWifiChipV11);

        // Create a SAR info record (no sensor or SAP support)
        SarInfo sarInfo = new SarInfo();
        sarInfo.sarVoiceCallSupported = true;
        sarInfo.sarSapSupported = false;
        sarInfo.sarSensorSupported = false;

        // Now expose the 1.1 IWifiChip.
        mWifiVendorHal = new WifiVendorHalSpyV1_1(mHalDeviceManager, mLooper.getLooper());
        when(mIWifiChipV11.resetTxPowerScenario()).thenReturn(mWifiStatusSuccess);

        assertTrue(mWifiVendorHal.startVendorHalSta());

        /* Calling reset once */
        assertTrue(mWifiVendorHal.selectTxPowerScenario(sarInfo));
        inOrder.verify(mIWifiChipV11).resetTxPowerScenario();
        inOrder.verify(mIWifiChipV11, never()).selectTxPowerScenario(anyInt());
        sarInfo.reportingSuccessful();

        /* Calling reset second time */
        assertTrue(mWifiVendorHal.selectTxPowerScenario(sarInfo));
        inOrder.verify(mIWifiChipV11, never()).resetTxPowerScenario();
        inOrder.verify(mIWifiChipV11, never()).selectTxPowerScenario(anyInt());

        mWifiVendorHal.stopVendorHal();
    }

    /**
     * Test the new resetTxPowerScenario HIDL method invocation for 1.2 interface.
     * This should return success.
     */
    @Test
    public void testResetTxPowerScenario_1_2() throws RemoteException {
        // Create a SAR info record (no sensor or SAP support)
        SarInfo sarInfo = new SarInfo();
        sarInfo.sarVoiceCallSupported = true;
        sarInfo.sarSapSupported = false;
        sarInfo.sarSensorSupported = false;

        // Now expose the 1.2 IWifiChip.
        mWifiVendorHal = new WifiVendorHalSpyV1_2(mHalDeviceManager, mLooper.getLooper());
        when(mIWifiChipV12.resetTxPowerScenario()).thenReturn(mWifiStatusSuccess);

        assertTrue(mWifiVendorHal.startVendorHalSta());
        assertTrue(mWifiVendorHal.selectTxPowerScenario(sarInfo));
        verify(mIWifiChipV12).resetTxPowerScenario();
        verify(mIWifiChipV12, never()).selectTxPowerScenario_1_2(anyInt());
        mWifiVendorHal.stopVendorHal();
    }

    /**
     * Test resetting SAR scenario when not needed, should return true without invoking
     * the HAL method.
     * This is using HAL 1.2 interface.
     */
    @Test
    public void testResetTxPowerScenario_not_needed_1_2() throws RemoteException {
        InOrder inOrder = inOrder(mIWifiChipV12);

        // Create a SAR info record (no sensor or SAP support)
        SarInfo sarInfo = new SarInfo();
        sarInfo.sarVoiceCallSupported = true;
        sarInfo.sarSapSupported = false;
        sarInfo.sarSensorSupported = false;

        // Now expose the 1.2 IWifiChip.
        mWifiVendorHal = new WifiVendorHalSpyV1_2(mHalDeviceManager, mLooper.getLooper());
        when(mIWifiChipV12.resetTxPowerScenario()).thenReturn(mWifiStatusSuccess);

        assertTrue(mWifiVendorHal.startVendorHalSta());

        /* Calling reset once */
        assertTrue(mWifiVendorHal.selectTxPowerScenario(sarInfo));
        inOrder.verify(mIWifiChipV12).resetTxPowerScenario();
        inOrder.verify(mIWifiChipV12, never()).selectTxPowerScenario(anyInt());
        sarInfo.reportingSuccessful();

        /* Calling reset second time */
        assertTrue(mWifiVendorHal.selectTxPowerScenario(sarInfo));
        inOrder.verify(mIWifiChipV12, never()).resetTxPowerScenario();
        inOrder.verify(mIWifiChipV12, never()).selectTxPowerScenario(anyInt());

        mWifiVendorHal.stopVendorHal();
    }

     /**
     * Test the selectTxPowerScenario HIDL method invocation with no sensor support, but with
     * SAP and voice call support.
     * When SAP is enabled, should result in SAP with near body scenario
     * Using IWifiChip 1.2 interface
     */
    @Test
    public void testSapScenarios_SelectTxPowerV1_2() throws RemoteException {
        // Create a SAR info record (with sensor and SAP support)
        SarInfo sarInfo = new SarInfo();
        sarInfo.sarVoiceCallSupported = true;
        sarInfo.sarSapSupported = true;
        sarInfo.sarSensorSupported = false;

        sarInfo.isWifiSapEnabled = true;

        // Expose the 1.2 IWifiChip.
        mWifiVendorHal = new WifiVendorHalSpyV1_2(mHalDeviceManager, mLooper.getLooper());
        when(mIWifiChipV12.selectTxPowerScenario_1_2(anyInt())).thenReturn(mWifiStatusSuccess);

        // ON_BODY_CELL_ON
        assertTrue(mWifiVendorHal.startVendorHalSta());
        assertTrue(mWifiVendorHal.selectTxPowerScenario(sarInfo));
        verify(mIWifiChipV12).selectTxPowerScenario_1_2(
                eq(android.hardware.wifi.V1_2.IWifiChip.TxPowerScenario.ON_BODY_CELL_ON));
        verify(mIWifiChipV12, never()).resetTxPowerScenario();
        mWifiVendorHal.stopVendorHal();
    }

    /**
     * Test the selectTxPowerScenario HIDL method invocation with no sensor support, but with
     * SAP and voice call support.
     * When a voice call is ongoing, should result in cell with near head scenario
     * Using IWifiChip 1.2 interface
     */
    @Test
    public void testVoiceCallScenarios_SelectTxPowerV1_2() throws RemoteException {
        // Create a SAR info record (with sensor and SAP support)
        SarInfo sarInfo = new SarInfo();
        sarInfo.sarVoiceCallSupported = true;
        sarInfo.sarSapSupported = true;
        sarInfo.sarSensorSupported = false;

        sarInfo.isVoiceCall = true;

        // Expose the 1.2 IWifiChip.
        mWifiVendorHal = new WifiVendorHalSpyV1_2(mHalDeviceManager, mLooper.getLooper());
        when(mIWifiChipV12.selectTxPowerScenario_1_2(anyInt())).thenReturn(mWifiStatusSuccess);

        // ON_HEAD_CELL_ON
        assertTrue(mWifiVendorHal.startVendorHalSta());
        assertTrue(mWifiVendorHal.selectTxPowerScenario(sarInfo));
        verify(mIWifiChipV12).selectTxPowerScenario_1_2(
                eq(android.hardware.wifi.V1_2.IWifiChip.TxPowerScenario.ON_HEAD_CELL_ON));
        verify(mIWifiChipV12, never()).resetTxPowerScenario();
        mWifiVendorHal.stopVendorHal();
    }

    /**
     * Test the selectTxPowerScenario HIDL method invocation with no sensor support, but with
     * SAP and voice call support.
     * When earpiece is active, should result in cell with near head scenario
     * Using IWifiChip 1.2 interface
     */
    @Test
    public void testEarPieceScenarios_SelectTxPowerV1_2() throws RemoteException {
        // Create a SAR info record (with sensor and SAP support)
        SarInfo sarInfo = new SarInfo();
        sarInfo.sarVoiceCallSupported = true;
        sarInfo.sarSapSupported = true;
        sarInfo.sarSensorSupported = false;

        sarInfo.isEarPieceActive = true;

        // Expose the 1.2 IWifiChip.
        mWifiVendorHal = new WifiVendorHalSpyV1_2(mHalDeviceManager, mLooper.getLooper());
        when(mIWifiChipV12.selectTxPowerScenario_1_2(anyInt())).thenReturn(mWifiStatusSuccess);

        // ON_HEAD_CELL_ON
        assertTrue(mWifiVendorHal.startVendorHalSta());
        assertTrue(mWifiVendorHal.selectTxPowerScenario(sarInfo));
        verify(mIWifiChipV12).selectTxPowerScenario_1_2(
                eq(android.hardware.wifi.V1_2.IWifiChip.TxPowerScenario.ON_HEAD_CELL_ON));
        verify(mIWifiChipV12, never()).resetTxPowerScenario();
        mWifiVendorHal.stopVendorHal();
    }

    /**
     * Test the selectTxPowerScenario HIDL method invocation with sensor related scenarios
     * to IWifiChip 1.2 interface
     */
    @Test
    public void testHeadSensorScenarios_SelectTxPowerV1_2() throws RemoteException {
        // Create a SAR info record (with sensor and SAP support)
        SarInfo sarInfo = new SarInfo();
        sarInfo.sarVoiceCallSupported = true;
        sarInfo.sarSapSupported = true;
        sarInfo.sarSensorSupported = true;

        sarInfo.sensorState = SarInfo.SAR_SENSOR_NEAR_HEAD;

        // Expose the 1.2 IWifiChip.
        mWifiVendorHal = new WifiVendorHalSpyV1_2(mHalDeviceManager, mLooper.getLooper());
        when(mIWifiChipV12.selectTxPowerScenario_1_2(anyInt())).thenReturn(mWifiStatusSuccess);

        // ON_HEAD_CELL_OFF
        assertTrue(mWifiVendorHal.startVendorHalSta());
        assertTrue(mWifiVendorHal.selectTxPowerScenario(sarInfo));
        verify(mIWifiChipV12).selectTxPowerScenario_1_2(
                eq(android.hardware.wifi.V1_2.IWifiChip.TxPowerScenario.ON_HEAD_CELL_OFF));
        verify(mIWifiChipV12, never()).resetTxPowerScenario();
        mWifiVendorHal.stopVendorHal();
    }

    /**
     * Test setting SAR scenario when not needed, should return true without invoking
     * the HAL method.
     * This is using HAL 1.2 interface.
     */
    @Test
    public void testSetTxPowerScenario_not_needed_1_2() throws RemoteException {
        InOrder inOrder = inOrder(mIWifiChipV12);

        // Create a SAR info record (no sensor and SAP support)
        SarInfo sarInfo = new SarInfo();
        sarInfo.sarVoiceCallSupported = true;
        sarInfo.sarSapSupported = true;
        sarInfo.sarSensorSupported = true;

        sarInfo.sensorState = SarInfo.SAR_SENSOR_NEAR_HEAD;

        // Now expose the 1.2 IWifiChip.
        mWifiVendorHal = new WifiVendorHalSpyV1_2(mHalDeviceManager, mLooper.getLooper());
        when(mIWifiChipV12.selectTxPowerScenario_1_2(anyInt())).thenReturn(mWifiStatusSuccess);

        assertTrue(mWifiVendorHal.startVendorHalSta());

        /* Calling set once */
        assertTrue(mWifiVendorHal.selectTxPowerScenario(sarInfo));
        inOrder.verify(mIWifiChipV12).selectTxPowerScenario_1_2(
                eq(android.hardware.wifi.V1_2.IWifiChip.TxPowerScenario.ON_HEAD_CELL_OFF));
        inOrder.verify(mIWifiChipV12, never()).resetTxPowerScenario();
        sarInfo.reportingSuccessful();

        /* Calling set second time */
        assertTrue(mWifiVendorHal.selectTxPowerScenario(sarInfo));
        inOrder.verify(mIWifiChipV12, never()).resetTxPowerScenario();
        inOrder.verify(mIWifiChipV12, never()).selectTxPowerScenario(anyInt());

        mWifiVendorHal.stopVendorHal();
    }

    /**
     * Test the selectTxPowerScenairo HIDL method invocation with sensor events for
     * IWifiChip 1.2 interface (Near hand event) along with a voice call.
     * This should be reverted to BODY events (First with CELL_OFF followed by CELL_ON).
     */
    @Test
    public void testHandSensorScenarios_SelectTxPowerV1_2() throws RemoteException {
        // Create a SAR info record (with sensor and SAR support)
        SarInfo sarInfo = new SarInfo();
        sarInfo.sarVoiceCallSupported = true;
        sarInfo.sarSapSupported = true;
        sarInfo.sarSensorSupported = true;

        sarInfo.sensorState = SarInfo.SAR_SENSOR_NEAR_HAND;

        // Expose the 1.2 IWifiChip.
        mWifiVendorHal = new WifiVendorHalSpyV1_2(mHalDeviceManager, mLooper.getLooper());
        when(mIWifiChipV12.selectTxPowerScenario_1_2(anyInt())).thenReturn(mWifiStatusSuccess);

        assertTrue(mWifiVendorHal.startVendorHalSta());

        // First select a scenario with cell off
        assertTrue(mWifiVendorHal.selectTxPowerScenario(sarInfo));
        verify(mIWifiChipV12).selectTxPowerScenario_1_2(
                eq(android.hardware.wifi.V1_2.IWifiChip.TxPowerScenario.ON_BODY_CELL_OFF));

        // Then select a scenario with cell on
        sarInfo.isVoiceCall = true;
        assertTrue(mWifiVendorHal.selectTxPowerScenario(sarInfo));
        verify(mIWifiChipV12).selectTxPowerScenario_1_2(
                eq(android.hardware.wifi.V1_2.IWifiChip.TxPowerScenario.ON_BODY_CELL_ON));

        mWifiVendorHal.stopVendorHal();
    }

    /**
     * Test the selectTxPowerScenario HIDL method invocation with a sensor info to IWifiChip
     * 1.1 interface.
     * Sensor mode should be ignored, and act only based on Cell on/off.
     */
    @Test
    public void testOnHeadCellOffOn_SelectTxPowerScenarioV1_1() throws RemoteException {
        // Create a SAR info record (with sensor and SAP support)
        SarInfo sarInfo = new SarInfo();
        sarInfo.sarVoiceCallSupported = true;
        sarInfo.sarSapSupported = true;
        sarInfo.sarSensorSupported = true;

        sarInfo.sensorState = SarInfo.SAR_SENSOR_NEAR_HEAD;

        // Expose the 1.1 IWifiChip.
        mWifiVendorHal = new WifiVendorHalSpyV1_1(mHalDeviceManager, mLooper.getLooper());
        when(mIWifiChipV11.selectTxPowerScenario(anyInt())).thenReturn(mWifiStatusSuccess);
        when(mIWifiChipV11.resetTxPowerScenario()).thenReturn(mWifiStatusSuccess);

        assertTrue(mWifiVendorHal.startVendorHalSta());

        // First select a scenario with cell off
        assertTrue(mWifiVendorHal.selectTxPowerScenario(sarInfo));
        verify(mIWifiChipV11).resetTxPowerScenario();

        // Then select a scenario with cell on
        sarInfo.isVoiceCall = true;
        assertTrue(mWifiVendorHal.selectTxPowerScenario(sarInfo));
        verify(mIWifiChipV11).selectTxPowerScenario(
                eq(android.hardware.wifi.V1_1.IWifiChip.TxPowerScenario.VOICE_CALL));

        mWifiVendorHal.stopVendorHal();
    }

    /**
     * Test the new selectTxPowerScenario HIDL method invocation with a bad input.
     * This should not result into any calls to the HAL.
     * Use IWifiChip 1.2 interface
     */
    @Test
    public void testInvalidSelectTxPowerScenario_1_2() throws RemoteException {
        // Create a SAR info record (with sensor and SAP support)
        SarInfo sarInfo = new SarInfo();
        sarInfo.sarVoiceCallSupported = true;
        sarInfo.sarSapSupported = true;
        sarInfo.sarSensorSupported = true;

        sarInfo.sensorState = SAR_SENSOR_INVALID_STATE;

        // Expose the 1.2 IWifiChip.
        mWifiVendorHal = new WifiVendorHalSpyV1_2(mHalDeviceManager, mLooper.getLooper());

        assertTrue(mWifiVendorHal.startVendorHalSta());
        assertFalse(mWifiVendorHal.selectTxPowerScenario(sarInfo));
        verify(mIWifiChipV12, never()).selectTxPowerScenario(anyInt());
        verify(mIWifiChipV12, never()).resetTxPowerScenario();
        mWifiVendorHal.stopVendorHal();
    }

    /**
     * Test the selectTxPowerScenario HIDL method invocation with IWifiChip 1.2 interface.
     * The following inputs:
     *   - Sensor support is enabled
     *   - Sensor state is NEAR_HEAD
     *   - SAP is enabled
     *   - No voice call
     */
    @Test
    public void testSelectTxPowerScenario_1_2_head_sap() throws RemoteException {
        // Create a SAR info record (with sensor and SAP support)
        SarInfo sarInfo = new SarInfo();
        sarInfo.sarVoiceCallSupported = true;
        sarInfo.sarSapSupported = true;
        sarInfo.sarSensorSupported = true;

        sarInfo.sensorState = SarInfo.SAR_SENSOR_NEAR_HEAD;
        sarInfo.isWifiSapEnabled = true;
        sarInfo.isVoiceCall = false;

        // Expose the 1.2 IWifiChip.
        mWifiVendorHal = new WifiVendorHalSpyV1_2(mHalDeviceManager, mLooper.getLooper());
        when(mIWifiChipV12.selectTxPowerScenario_1_2(anyInt())).thenReturn(mWifiStatusSuccess);

        assertTrue(mWifiVendorHal.startVendorHalSta());
        assertTrue(mWifiVendorHal.selectTxPowerScenario(sarInfo));
        verify(mIWifiChipV12).selectTxPowerScenario_1_2(
                eq(android.hardware.wifi.V1_2.IWifiChip.TxPowerScenario.ON_HEAD_CELL_ON));

        mWifiVendorHal.stopVendorHal();
    }

    /**
     * Test the selectTxPowerScenario HIDL method invocation with IWifiChip 1.2 interface.
     * The following inputs:
     *   - Sensor support is enabled
     *   - Sensor state is NEAR_HEAD
     *   - SAP is enabled
     *   - voice call is enabled
     */
    @Test
    public void testSelectTxPowerScenario_1_2_head_sap_call() throws RemoteException {
        // Create a SAR info record (with sensor and SAP support)
        SarInfo sarInfo = new SarInfo();
        sarInfo.sarVoiceCallSupported = true;
        sarInfo.sarSapSupported = true;
        sarInfo.sarSensorSupported = true;

        sarInfo.sensorState = SarInfo.SAR_SENSOR_NEAR_HEAD;
        sarInfo.isWifiSapEnabled = true;
        sarInfo.isVoiceCall = true;

        // Expose the 1.2 IWifiChip.
        mWifiVendorHal = new WifiVendorHalSpyV1_2(mHalDeviceManager, mLooper.getLooper());
        when(mIWifiChipV12.selectTxPowerScenario_1_2(anyInt())).thenReturn(mWifiStatusSuccess);

        assertTrue(mWifiVendorHal.startVendorHalSta());
        assertTrue(mWifiVendorHal.selectTxPowerScenario(sarInfo));
        verify(mIWifiChipV12).selectTxPowerScenario_1_2(
                eq(android.hardware.wifi.V1_2.IWifiChip.TxPowerScenario.ON_HEAD_CELL_ON));

        mWifiVendorHal.stopVendorHal();
    }

    /**
     * Test the selectTxPowerScenario HIDL method invocation with IWifiChip 1.2 interface.
     * The following inputs:
     *   - Sensor support is enabled
     *   - Sensor state is FREE_SPACE
     *   - SAP is enabled
     *   - No voice call
     */
    @Test
    public void testSelectTxPowerScenario_1_2_freespace_sap() throws RemoteException {
        // Create a SAR info record (with sensor and SAP support)
        SarInfo sarInfo = new SarInfo();
        sarInfo.sarVoiceCallSupported = true;
        sarInfo.sarSapSupported = true;
        sarInfo.sarSensorSupported = true;

        sarInfo.sensorState = SarInfo.SAR_SENSOR_FREE_SPACE;
        sarInfo.isWifiSapEnabled = true;
        sarInfo.isVoiceCall = false;

        // Expose the 1.2 IWifiChip.
        mWifiVendorHal = new WifiVendorHalSpyV1_2(mHalDeviceManager, mLooper.getLooper());
        when(mIWifiChipV12.resetTxPowerScenario()).thenReturn(mWifiStatusSuccess);

        assertTrue(mWifiVendorHal.startVendorHalSta());
        assertTrue(mWifiVendorHal.selectTxPowerScenario(sarInfo));

        verify(mIWifiChipV12).resetTxPowerScenario();
        verify(mIWifiChipV12, never()).selectTxPowerScenario_1_2(anyInt());
        mWifiVendorHal.stopVendorHal();
    }

    /**
     * Test the selectTxPowerScenario HIDL method invocation with IWifiChip 1.2 interface.
     * The following inputs:
     *   - Sensor support is disabled
     *   - SAP is enabled
     *   - No voice call
     */
    @Test
    public void testSelectTxPowerScenario_1_2_no_sensors_sap() throws RemoteException {
        // Create a SAR info record (with no sensor support)
        SarInfo sarInfo = new SarInfo();
        sarInfo.sarVoiceCallSupported = true;
        sarInfo.sarSapSupported = true;
        sarInfo.sarSensorSupported = true;

        sarInfo.isWifiSapEnabled = true;
        sarInfo.isVoiceCall = false;

        // Expose the 1.2 IWifiChip.
        mWifiVendorHal = new WifiVendorHalSpyV1_2(mHalDeviceManager, mLooper.getLooper());
        when(mIWifiChipV12.resetTxPowerScenario()).thenReturn(mWifiStatusSuccess);

        assertTrue(mWifiVendorHal.startVendorHalSta());
        assertTrue(mWifiVendorHal.selectTxPowerScenario(sarInfo));

        verify(mIWifiChipV12).resetTxPowerScenario();
        verify(mIWifiChipV12, never()).selectTxPowerScenario_1_2(anyInt());
        mWifiVendorHal.stopVendorHal();
    }

    /**
     * Test the setLowLatencyMode HIDL method invocation with IWifiChip 1.2 interface.
     * Function should return false
     */
    @Test
    public void testSetLowLatencyMode_1_2() throws RemoteException {
        // Expose the 1.2 IWifiChip.
        mWifiVendorHal = new WifiVendorHalSpyV1_2(mHalDeviceManager, mLooper.getLooper());
        assertFalse(mWifiVendorHal.setLowLatencyMode(true));
        assertFalse(mWifiVendorHal.setLowLatencyMode(false));
    }

    /**
     * Test the setLowLatencyMode HIDL method invocation with IWifiChip 1.3 interface
     */
    @Test
    public void testSetLowLatencyMode_1_3_enabled() throws RemoteException {
        int mode = android.hardware.wifi.V1_3.IWifiChip.LatencyMode.LOW;

        // Expose the 1.3 IWifiChip.
        mWifiVendorHal = new WifiVendorHalSpyV1_3(mHalDeviceManager, mLooper.getLooper());
        when(mIWifiChipV13.setLatencyMode(anyInt())).thenReturn(mWifiStatusSuccess);
        assertTrue(mWifiVendorHal.setLowLatencyMode(true));
        verify(mIWifiChipV13).setLatencyMode(eq(mode));
    }

    /**
     * Test the setLowLatencyMode HIDL method invocation with IWifiChip 1.3 interface
     */
    @Test
    public void testSetLowLatencyMode_1_3_disabled() throws RemoteException {
        int mode = android.hardware.wifi.V1_3.IWifiChip.LatencyMode.NORMAL;

        // Expose the 1.3 IWifiChip.
        mWifiVendorHal = new WifiVendorHalSpyV1_3(mHalDeviceManager, mLooper.getLooper());
        when(mIWifiChipV13.setLatencyMode(anyInt())).thenReturn(mWifiStatusSuccess);
        assertTrue(mWifiVendorHal.setLowLatencyMode(false));
        verify(mIWifiChipV13).setLatencyMode(eq(mode));
    }

    /**
     * Test the STA Iface creation failure due to iface name retrieval failure.
     */
    @Test
    public void testCreateStaIfaceFailureInIfaceName() throws RemoteException {
        doAnswer(new AnswerWithArguments() {
            public void answer(IWifiIface.getNameCallback cb)
                    throws RemoteException {
                cb.onValues(mWifiStatusFailure, "wlan0");
            }
        }).when(mIWifiStaIface).getName(any(IWifiIface.getNameCallback.class));

        assertTrue(mWifiVendorHal.startVendorHal());
        assertNull(mWifiVendorHal.createStaIface(true, null));
        verify(mHalDeviceManager).createStaIface(eq(true), any(), eq(null));
    }

    /**
     * Test the STA Iface creation failure due to iface name retrieval failure.
     */
    @Test
    public void testCreateApIfaceFailureInIfaceName() throws RemoteException {
        doAnswer(new AnswerWithArguments() {
            public void answer(IWifiIface.getNameCallback cb)
                    throws RemoteException {
                cb.onValues(mWifiStatusFailure, "wlan0");
            }
        }).when(mIWifiApIface).getName(any(IWifiIface.getNameCallback.class));

        assertTrue(mWifiVendorHal.startVendorHal());
        assertNull(mWifiVendorHal.createApIface(null));
        verify(mHalDeviceManager).createApIface(any(), eq(null));
    }

    /**
     * Test the creation and removal of STA Iface.
     */
    @Test
    public void testCreateRemoveStaIface() throws RemoteException {
        assertTrue(mWifiVendorHal.startVendorHal());
        String ifaceName = mWifiVendorHal.createStaIface(false, null);
        verify(mHalDeviceManager).createStaIface(eq(false), any(), eq(null));
        assertEquals(TEST_IFACE_NAME, ifaceName);
        assertTrue(mWifiVendorHal.removeStaIface(ifaceName));
        verify(mHalDeviceManager).removeIface(eq(mIWifiStaIface));
    }

    /**
     * Test the creation and removal of Ap Iface.
     */
    @Test
    public void testCreateRemoveApIface() throws RemoteException {
        assertTrue(mWifiVendorHal.startVendorHal());
        String ifaceName = mWifiVendorHal.createApIface(null);
        verify(mHalDeviceManager).createApIface(any(), eq(null));
        assertEquals(TEST_IFACE_NAME, ifaceName);
        assertTrue(mWifiVendorHal.removeApIface(ifaceName));
        verify(mHalDeviceManager).removeIface(eq(mIWifiApIface));
    }

    /**
     * Test the callback handling for the 1.2 HAL.
     */
    @Test
    public void testAlertCallbackUsing_1_2_EventCallback() throws Exception {
        // Expose the 1.2 IWifiChip.
        mWifiVendorHal = new WifiVendorHalSpyV1_2(mHalDeviceManager, mLooper.getLooper());

        assertTrue(mWifiVendorHal.startVendorHalSta());
        assertNotNull(mIWifiChipEventCallbackV12);

        testAlertCallbackUsingProvidedCallback(mIWifiChipEventCallbackV12);
    }

    /**
     * Verifies setMacAddress() success.
     */
    @Test
    public void testSetMacAddressSuccess() throws Exception {
        // Expose the 1.2 IWifiStaIface.
        mWifiVendorHal = new WifiVendorHalSpyV1_2(mHalDeviceManager, mLooper.getLooper());
        byte[] macByteArray = TEST_MAC_ADDRESS.toByteArray();
        when(mIWifiStaIfaceV12.setMacAddress(macByteArray)).thenReturn(mWifiStatusSuccess);

        assertTrue(mWifiVendorHal.setMacAddress(TEST_IFACE_NAME, TEST_MAC_ADDRESS));
        verify(mIWifiStaIfaceV12).setMacAddress(macByteArray);
    }

    /**
     * Verifies setMacAddress() can handle failure status.
     */
    @Test
    public void testSetMacAddressFailDueToStatusFailure() throws Exception {
        // Expose the 1.2 IWifiStaIface.
        mWifiVendorHal = new WifiVendorHalSpyV1_2(mHalDeviceManager, mLooper.getLooper());
        byte[] macByteArray = TEST_MAC_ADDRESS.toByteArray();
        when(mIWifiStaIfaceV12.setMacAddress(macByteArray)).thenReturn(mWifiStatusFailure);

        assertFalse(mWifiVendorHal.setMacAddress(TEST_IFACE_NAME, TEST_MAC_ADDRESS));
        verify(mIWifiStaIfaceV12).setMacAddress(macByteArray);
    }

    /**
     * Verifies setMacAddress() can handle RemoteException.
     */
    @Test
    public void testSetMacAddressFailDueToRemoteException() throws Exception {
        // Expose the 1.2 IWifiStaIface.
        mWifiVendorHal = new WifiVendorHalSpyV1_2(mHalDeviceManager, mLooper.getLooper());
        byte[] macByteArray = TEST_MAC_ADDRESS.toByteArray();
        doThrow(new RemoteException()).when(mIWifiStaIfaceV12).setMacAddress(macByteArray);

        assertFalse(mWifiVendorHal.setMacAddress(TEST_IFACE_NAME, TEST_MAC_ADDRESS));
        verify(mIWifiStaIfaceV12).setMacAddress(macByteArray);
    }

    /**
     * Verifies setMacAddress() does not crash with older HALs.
     */
    @Test
    public void testSetMacAddressDoesNotCrashOnOlderHal() throws Exception {
        byte[] macByteArray = TEST_MAC_ADDRESS.toByteArray();
        assertFalse(mWifiVendorHal.setMacAddress(TEST_IFACE_NAME, TEST_MAC_ADDRESS));
    }

    /**
     * Verifies radio mode change callback to indicate DBS mode.
     */
    @Test
    public void testRadioModeChangeCallbackToDbsMode() throws Exception {
        startHalInStaModeAndRegisterRadioModeChangeCallback();

        RadioModeInfo radioModeInfo0 = new RadioModeInfo();
        radioModeInfo0.bandInfo = WifiScanner.WIFI_BAND_5_GHZ;
        RadioModeInfo radioModeInfo1 = new RadioModeInfo();
        radioModeInfo1.bandInfo = WifiScanner.WIFI_BAND_24_GHZ;

        IfaceInfo ifaceInfo0 = new IfaceInfo();
        ifaceInfo0.name = TEST_IFACE_NAME;
        ifaceInfo0.channel = 34;
        IfaceInfo ifaceInfo1 = new IfaceInfo();
        ifaceInfo1.name = TEST_IFACE_NAME_1;
        ifaceInfo1.channel = 1;

        radioModeInfo0.ifaceInfos.add(ifaceInfo0);
        radioModeInfo1.ifaceInfos.add(ifaceInfo1);

        ArrayList<RadioModeInfo> radioModeInfos = new ArrayList<>();
        radioModeInfos.add(radioModeInfo0);
        radioModeInfos.add(radioModeInfo1);

        mIWifiChipEventCallbackV12.onRadioModeChange(radioModeInfos);
        mLooper.dispatchAll();
        verify(mVendorHalRadioModeChangeHandler).onDbs();

        verifyNoMoreInteractions(mVendorHalRadioModeChangeHandler);
    }

    /**
     * Verifies radio mode change callback to indicate SBS mode.
     */
    @Test
    public void testRadioModeChangeCallbackToSbsMode() throws Exception {
        startHalInStaModeAndRegisterRadioModeChangeCallback();

        RadioModeInfo radioModeInfo0 = new RadioModeInfo();
        radioModeInfo0.bandInfo = WifiScanner.WIFI_BAND_5_GHZ;
        RadioModeInfo radioModeInfo1 = new RadioModeInfo();
        radioModeInfo1.bandInfo = WifiScanner.WIFI_BAND_5_GHZ;

        IfaceInfo ifaceInfo0 = new IfaceInfo();
        ifaceInfo0.name = TEST_IFACE_NAME;
        ifaceInfo0.channel = 34;
        IfaceInfo ifaceInfo1 = new IfaceInfo();
        ifaceInfo1.name = TEST_IFACE_NAME_1;
        ifaceInfo1.channel = 36;

        radioModeInfo0.ifaceInfos.add(ifaceInfo0);
        radioModeInfo1.ifaceInfos.add(ifaceInfo1);

        ArrayList<RadioModeInfo> radioModeInfos = new ArrayList<>();
        radioModeInfos.add(radioModeInfo0);
        radioModeInfos.add(radioModeInfo1);

        mIWifiChipEventCallbackV12.onRadioModeChange(radioModeInfos);
        mLooper.dispatchAll();
        verify(mVendorHalRadioModeChangeHandler).onSbs(WifiScanner.WIFI_BAND_5_GHZ);

        verifyNoMoreInteractions(mVendorHalRadioModeChangeHandler);
    }

    /**
     * Verifies radio mode change callback to indicate SCC mode.
     */
    @Test
    public void testRadioModeChangeCallbackToSccMode() throws Exception {
        startHalInStaModeAndRegisterRadioModeChangeCallback();

        RadioModeInfo radioModeInfo0 = new RadioModeInfo();
        radioModeInfo0.bandInfo = WifiScanner.WIFI_BAND_5_GHZ;

        IfaceInfo ifaceInfo0 = new IfaceInfo();
        ifaceInfo0.name = TEST_IFACE_NAME;
        ifaceInfo0.channel = 34;
        IfaceInfo ifaceInfo1 = new IfaceInfo();
        ifaceInfo1.name = TEST_IFACE_NAME_1;
        ifaceInfo1.channel = 34;

        radioModeInfo0.ifaceInfos.add(ifaceInfo0);
        radioModeInfo0.ifaceInfos.add(ifaceInfo1);

        ArrayList<RadioModeInfo> radioModeInfos = new ArrayList<>();
        radioModeInfos.add(radioModeInfo0);

        mIWifiChipEventCallbackV12.onRadioModeChange(radioModeInfos);
        mLooper.dispatchAll();
        verify(mVendorHalRadioModeChangeHandler).onScc(WifiScanner.WIFI_BAND_5_GHZ);

        verifyNoMoreInteractions(mVendorHalRadioModeChangeHandler);
    }

    /**
     * Verifies radio mode change callback to indicate MCC mode.
     */
    @Test
    public void testRadioModeChangeCallbackToMccMode() throws Exception {
        startHalInStaModeAndRegisterRadioModeChangeCallback();

        RadioModeInfo radioModeInfo0 = new RadioModeInfo();
        radioModeInfo0.bandInfo = WifiScanner.WIFI_BAND_BOTH;

        IfaceInfo ifaceInfo0 = new IfaceInfo();
        ifaceInfo0.name = TEST_IFACE_NAME;
        ifaceInfo0.channel = 1;
        IfaceInfo ifaceInfo1 = new IfaceInfo();
        ifaceInfo1.name = TEST_IFACE_NAME_1;
        ifaceInfo1.channel = 36;

        radioModeInfo0.ifaceInfos.add(ifaceInfo0);
        radioModeInfo0.ifaceInfos.add(ifaceInfo1);

        ArrayList<RadioModeInfo> radioModeInfos = new ArrayList<>();
        radioModeInfos.add(radioModeInfo0);

        mIWifiChipEventCallbackV12.onRadioModeChange(radioModeInfos);
        mLooper.dispatchAll();
        verify(mVendorHalRadioModeChangeHandler).onMcc(WifiScanner.WIFI_BAND_BOTH);

        verifyNoMoreInteractions(mVendorHalRadioModeChangeHandler);
    }

    /**
     * Verifies radio mode change callback error cases.
     */
    @Test
    public void testRadioModeChangeCallbackErrorSimultaneousWithSameIfaceOnBothRadios()
            throws Exception {
        startHalInStaModeAndRegisterRadioModeChangeCallback();

        RadioModeInfo radioModeInfo0 = new RadioModeInfo();
        radioModeInfo0.bandInfo = WifiScanner.WIFI_BAND_24_GHZ;
        RadioModeInfo radioModeInfo1 = new RadioModeInfo();
        radioModeInfo1.bandInfo = WifiScanner.WIFI_BAND_5_GHZ;

        IfaceInfo ifaceInfo0 = new IfaceInfo();
        ifaceInfo0.name = TEST_IFACE_NAME;
        ifaceInfo0.channel = 34;

        radioModeInfo0.ifaceInfos.add(ifaceInfo0);
        radioModeInfo1.ifaceInfos.add(ifaceInfo0);

        ArrayList<RadioModeInfo> radioModeInfos = new ArrayList<>();
        radioModeInfos.add(radioModeInfo0);
        radioModeInfos.add(radioModeInfo1);

        mIWifiChipEventCallbackV12.onRadioModeChange(radioModeInfos);
        mLooper.dispatchAll();
        // Ignored....

        verifyNoMoreInteractions(mVendorHalRadioModeChangeHandler);
    }

    private void startHalInStaModeAndRegisterRadioModeChangeCallback() {
        // Expose the 1.2 IWifiChip.
        mWifiVendorHal = new WifiVendorHalSpyV1_2(mHalDeviceManager, mLooper.getLooper());
        mWifiVendorHal.registerRadioModeChangeHandler(mVendorHalRadioModeChangeHandler);
        assertTrue(mWifiVendorHal.startVendorHalSta());
        assertNotNull(mIWifiChipEventCallbackV12);
    }

    private void testAlertCallbackUsingProvidedCallback(IWifiChipEventCallback chipCallback)
            throws Exception {
        when(mIWifiChip.enableDebugErrorAlerts(anyBoolean())).thenReturn(mWifiStatusSuccess);
        when(mIWifiChip.stopLoggingToDebugRingBuffer()).thenReturn(mWifiStatusSuccess);

        int errorCode = 5;
        byte[] errorData = new byte[45];
        new Random().nextBytes(errorData);

        // Randomly raise the HIDL callback before we register for the log callback.
        // This should be safely ignored. (Not trigger NPE.)
        chipCallback.onDebugErrorAlert(
                errorCode, NativeUtil.byteArrayToArrayList(errorData));
        mLooper.dispatchAll();

        WifiNative.WifiLoggerEventHandler eventHandler =
                mock(WifiNative.WifiLoggerEventHandler.class);
        assertTrue(mWifiVendorHal.setLoggingEventHandler(eventHandler));
        verify(mIWifiChip).enableDebugErrorAlerts(eq(true));

        // Now raise the HIDL callback, this should be properly handled.
        chipCallback.onDebugErrorAlert(
                errorCode, NativeUtil.byteArrayToArrayList(errorData));
        mLooper.dispatchAll();
        verify(eventHandler).onWifiAlert(eq(errorCode), eq(errorData));

        // Now stop the logging and invoke the callback. This should be ignored.
        reset(eventHandler);
        assertTrue(mWifiVendorHal.resetLogHandler());
        chipCallback.onDebugErrorAlert(
                errorCode, NativeUtil.byteArrayToArrayList(errorData));
        mLooper.dispatchAll();
        verify(eventHandler, never()).onWifiAlert(anyInt(), anyObject());
    }

    private void startBgScan(WifiNative.ScanEventHandler eventHandler) throws Exception {
        when(mIWifiStaIface.startBackgroundScan(
                anyInt(), any(StaBackgroundScanParameters.class))).thenReturn(mWifiStatusSuccess);
        WifiNative.ScanSettings settings = new WifiNative.ScanSettings();
        settings.num_buckets = 1;
        WifiNative.BucketSettings bucketSettings = new WifiNative.BucketSettings();
        bucketSettings.bucket = 0;
        bucketSettings.period_ms = 16000;
        bucketSettings.report_events = WifiScanner.REPORT_EVENT_AFTER_EACH_SCAN;
        settings.buckets = new WifiNative.BucketSettings[] {bucketSettings};
        assertTrue(mWifiVendorHal.startBgScan(TEST_IFACE_NAME, settings, eventHandler));
    }

    // Create a pair of HIDL scan result and its corresponding framework scan result for
    // comparison.
    private Pair<StaScanResult, ScanResult> createHidlAndFrameworkBgScanResult() {
        StaScanResult staScanResult = new StaScanResult();
        Random random = new Random();
        byte[] ssid = new byte[8];
        random.nextBytes(ssid);
        staScanResult.ssid.addAll(NativeUtil.byteArrayToArrayList(ssid));
        random.nextBytes(staScanResult.bssid);
        staScanResult.frequency = 2432;
        staScanResult.rssi = -45;
        staScanResult.timeStampInUs = 5;
        WifiInformationElement ie1 = new WifiInformationElement();
        byte[] ie1_data = new byte[56];
        random.nextBytes(ie1_data);
        ie1.id = 1;
        ie1.data.addAll(NativeUtil.byteArrayToArrayList(ie1_data));
        staScanResult.informationElements.add(ie1);

        // Now create the corresponding Scan result structure.
        ScanResult scanResult = new ScanResult();
        scanResult.SSID = NativeUtil.encodeSsid(staScanResult.ssid);
        scanResult.BSSID = NativeUtil.macAddressFromByteArray(staScanResult.bssid);
        scanResult.wifiSsid = WifiSsid.createFromByteArray(ssid);
        scanResult.frequency = staScanResult.frequency;
        scanResult.level = staScanResult.rssi;
        scanResult.timestamp = staScanResult.timeStampInUs;

        return Pair.create(staScanResult, scanResult);
    }

    // Create a pair of HIDL scan datas and its corresponding framework scan datas for
    // comparison.
    private Pair<ArrayList<StaScanData>, ArrayList<WifiScanner.ScanData>>
            createHidlAndFrameworkBgScanDatas() {
        ArrayList<StaScanData> staScanDatas = new ArrayList<>();
        StaScanData staScanData = new StaScanData();

        Pair<StaScanResult, ScanResult> result = createHidlAndFrameworkBgScanResult();
        staScanData.results.add(result.first);
        staScanData.bucketsScanned = 5;
        staScanData.flags = StaScanDataFlagMask.INTERRUPTED;
        staScanDatas.add(staScanData);

        ArrayList<WifiScanner.ScanData> scanDatas = new ArrayList<>();
        ScanResult[] scanResults = new ScanResult[1];
        scanResults[0] = result.second;
        WifiScanner.ScanData scanData =
                new WifiScanner.ScanData(mWifiVendorHal.mScan.cmdId, 1,
                        staScanData.bucketsScanned, WifiScanner.WIFI_BAND_UNSPECIFIED, scanResults);
        scanDatas.add(scanData);
        return Pair.create(staScanDatas, scanDatas);
    }

    private void assertScanResultEqual(ScanResult expected, ScanResult actual) {
        assertEquals(expected.SSID, actual.SSID);
        assertEquals(expected.wifiSsid.getHexString(), actual.wifiSsid.getHexString());
        assertEquals(expected.BSSID, actual.BSSID);
        assertEquals(expected.frequency, actual.frequency);
        assertEquals(expected.level, actual.level);
        assertEquals(expected.timestamp, actual.timestamp);
    }

    private void assertScanResultsEqual(ScanResult[] expected, ScanResult[] actual) {
        assertEquals(expected.length, actual.length);
        for (int i = 0; i < expected.length; i++) {
            assertScanResultEqual(expected[i], actual[i]);
        }
    }

    private void assertScanDataEqual(WifiScanner.ScanData expected, WifiScanner.ScanData actual) {
        assertEquals(expected.getId(), actual.getId());
        assertEquals(expected.getFlags(), actual.getFlags());
        assertEquals(expected.getBucketsScanned(), actual.getBucketsScanned());
        assertScanResultsEqual(expected.getResults(), actual.getResults());
    }

    private void assertScanDatasEqual(
            List<WifiScanner.ScanData> expected, List<WifiScanner.ScanData> actual) {
        assertEquals(expected.size(), actual.size());
        for (int i = 0; i < expected.size(); i++) {
            assertScanDataEqual(expected.get(i), actual.get(i));
        }
    }
}
