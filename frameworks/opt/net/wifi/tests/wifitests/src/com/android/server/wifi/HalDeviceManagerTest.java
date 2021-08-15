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

import static com.android.server.wifi.HalDeviceManager.START_HAL_RETRY_TIMES;

import static junit.framework.Assert.assertEquals;

import static org.hamcrest.core.IsEqual.equalTo;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;
import static org.mockito.Matchers.any;
import static org.mockito.Matchers.anyInt;
import static org.mockito.Matchers.anyLong;
import static org.mockito.Matchers.anyString;
import static org.mockito.Matchers.eq;
import static org.mockito.Mockito.doAnswer;
import static org.mockito.Mockito.inOrder;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.verifyNoMoreInteractions;
import static org.mockito.Mockito.when;

import android.app.test.MockAnswerUtil;
import android.hardware.wifi.V1_0.IWifi;
import android.hardware.wifi.V1_0.IWifiApIface;
import android.hardware.wifi.V1_0.IWifiChip;
import android.hardware.wifi.V1_0.IWifiChipEventCallback;
import android.hardware.wifi.V1_0.IWifiEventCallback;
import android.hardware.wifi.V1_0.IWifiIface;
import android.hardware.wifi.V1_0.IWifiNanIface;
import android.hardware.wifi.V1_0.IWifiP2pIface;
import android.hardware.wifi.V1_0.IWifiRttController;
import android.hardware.wifi.V1_0.IWifiStaIface;
import android.hardware.wifi.V1_0.IfaceType;
import android.hardware.wifi.V1_0.WifiStatus;
import android.hardware.wifi.V1_0.WifiStatusCode;
import android.hidl.manager.V1_0.IServiceNotification;
import android.hidl.manager.V1_2.IServiceManager;
import android.os.Handler;
import android.os.IHwBinder;
import android.os.test.TestLooper;
import android.util.Log;

import androidx.test.filters.SmallTest;

import com.android.server.wifi.HalDeviceManager.InterfaceDestroyedListener;

import org.hamcrest.core.IsNull;
import org.junit.After;
import org.junit.Before;
import org.junit.Rule;
import org.junit.Test;
import org.junit.rules.ErrorCollector;
import org.mockito.ArgumentCaptor;
import org.mockito.InOrder;
import org.mockito.Mock;
import org.mockito.MockitoAnnotations;

import java.io.PrintWriter;
import java.io.StringWriter;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Map;
import java.util.Set;

/**
 * Unit test harness for HalDeviceManagerTest.
 */
@SmallTest
public class HalDeviceManagerTest {
    private HalDeviceManager mDut;
    @Mock IServiceManager mServiceManagerMock;
    @Mock IWifi mWifiMock;
    @Mock IWifiRttController mRttControllerMock;
    @Mock HalDeviceManager.ManagerStatusListener mManagerStatusListenerMock;
    @Mock private Clock mClock;
    private TestLooper mTestLooper;
    private Handler mHandler;
    private ArgumentCaptor<IHwBinder.DeathRecipient> mDeathRecipientCaptor =
            ArgumentCaptor.forClass(IHwBinder.DeathRecipient.class);
    private ArgumentCaptor<IServiceNotification.Stub> mServiceNotificationCaptor =
            ArgumentCaptor.forClass(IServiceNotification.Stub.class);
    private ArgumentCaptor<IWifiEventCallback> mWifiEventCallbackCaptor = ArgumentCaptor.forClass(
            IWifiEventCallback.class);
    private InOrder mInOrder;
    @Rule public ErrorCollector collector = new ErrorCollector();
    private WifiStatus mStatusOk;
    private WifiStatus mStatusFail;

    private class HalDeviceManagerSpy extends HalDeviceManager {
        HalDeviceManagerSpy() {
            super(mClock, mTestLooper.getLooper());
        }

        @Override
        protected IWifi getWifiServiceMockable() {
            return mWifiMock;
        }

        @Override
        protected IServiceManager getServiceManagerMockable() {
            return mServiceManagerMock;
        }
    }

    @Before
    public void before() throws Exception {
        MockitoAnnotations.initMocks(this);

        mTestLooper = new TestLooper();
        mHandler = new Handler(mTestLooper.getLooper());

        // initialize dummy status objects
        mStatusOk = getStatus(WifiStatusCode.SUCCESS);
        mStatusFail = getStatus(WifiStatusCode.ERROR_UNKNOWN);

        when(mServiceManagerMock.linkToDeath(any(IHwBinder.DeathRecipient.class),
                anyLong())).thenReturn(true);
        when(mServiceManagerMock.registerForNotifications(anyString(), anyString(),
                any(IServiceNotification.Stub.class))).thenReturn(true);
        when(mServiceManagerMock.listManifestByInterface(eq(IWifi.kInterfaceName)))
                .thenReturn(new ArrayList(Arrays.asList("default")));
        when(mWifiMock.linkToDeath(any(IHwBinder.DeathRecipient.class), anyLong())).thenReturn(
                true);
        when(mWifiMock.registerEventCallback(any(IWifiEventCallback.class))).thenReturn(mStatusOk);
        when(mWifiMock.start()).thenReturn(mStatusOk);
        when(mWifiMock.stop()).thenReturn(mStatusOk);
        when(mWifiMock.isStarted()).thenReturn(true);

        mDut = new HalDeviceManagerSpy();
    }

    /**
     * Print out the dump of the device manager after each test. Not used in test validation
     * (internal state) - but can help in debugging failed tests.
     */
    @After
    public void after() throws Exception {
        dumpDut("after: ");
    }

    //////////////////////////////////////////////////////////////////////////////////////
    // Chip Independent Tests
    //////////////////////////////////////////////////////////////////////////////////////

    /**
     * Test basic startup flow:
     * - IServiceManager registrations
     * - IWifi registrations
     * - IWifi startup delayed
     * - Start Wi-Fi -> onStart
     * - Stop Wi-Fi -> onStop
     */
    @Test
    public void testStartStopFlow() throws Exception {
        mInOrder = inOrder(mServiceManagerMock, mWifiMock, mManagerStatusListenerMock);
        executeAndValidateInitializationSequence();
        executeAndValidateStartupSequence();

        // act: stop Wi-Fi
        mDut.stop();
        mTestLooper.dispatchAll();

        // verify: onStop called
        mInOrder.verify(mWifiMock).stop();
        mInOrder.verify(mManagerStatusListenerMock).onStatusChanged();

        verifyNoMoreInteractions(mManagerStatusListenerMock);
    }

    /**
     * Test the service manager notification coming in after
     * {@link HalDeviceManager#initIWifiIfNecessary()} is already invoked as a part of
     * {@link HalDeviceManager#initialize()}.
     */
    @Test
    public void testServiceRegisterationAfterInitialize() throws Exception {
        mInOrder = inOrder(mServiceManagerMock, mWifiMock, mManagerStatusListenerMock);
        executeAndValidateInitializationSequence();

        // This should now be ignored since IWifi is already non-null.
        mServiceNotificationCaptor.getValue().onRegistration(IWifi.kInterfaceName, "", true);

        verifyNoMoreInteractions(mManagerStatusListenerMock, mWifiMock, mServiceManagerMock);
    }

    /**
     * Validate that multiple callback registrations are called and that duplicate ones are
     * only called once.
     */
    @Test
    public void testMultipleCallbackRegistrations() throws Exception {
        mInOrder = inOrder(mServiceManagerMock, mWifiMock, mManagerStatusListenerMock);
        executeAndValidateInitializationSequence();

        // register another 2 callbacks - one of them twice
        HalDeviceManager.ManagerStatusListener callback1 = mock(
                HalDeviceManager.ManagerStatusListener.class);
        HalDeviceManager.ManagerStatusListener callback2 = mock(
                HalDeviceManager.ManagerStatusListener.class);
        mDut.registerStatusListener(callback2, mHandler);
        mDut.registerStatusListener(callback1, mHandler);
        mDut.registerStatusListener(callback2, mHandler);

        // startup
        executeAndValidateStartupSequence();

        // verify
        verify(callback1).onStatusChanged();
        verify(callback2).onStatusChanged();

        verifyNoMoreInteractions(mManagerStatusListenerMock, callback1, callback2);
    }

    /**
     * Validate IWifi death listener and registration flow.
     */
    @Test
    public void testWifiDeathAndRegistration() throws Exception {
        mInOrder = inOrder(mServiceManagerMock, mWifiMock, mManagerStatusListenerMock);
        executeAndValidateInitializationSequence();
        executeAndValidateStartupSequence();

        // act: IWifi service death
        mDeathRecipientCaptor.getValue().serviceDied(0);
        mTestLooper.dispatchAll();

        // verify: getting onStop
        mInOrder.verify(mManagerStatusListenerMock).onStatusChanged();

        // act: service startup
        mServiceNotificationCaptor.getValue().onRegistration(IWifi.kInterfaceName, "", false);

        // verify: initialization of IWifi
        mInOrder.verify(mWifiMock).linkToDeath(mDeathRecipientCaptor.capture(), anyLong());
        mInOrder.verify(mWifiMock).registerEventCallback(mWifiEventCallbackCaptor.capture());

        // act: start
        collector.checkThat(mDut.start(), equalTo(true));
        mWifiEventCallbackCaptor.getValue().onStart();
        mTestLooper.dispatchAll();

        // verify: service and callback calls
        mInOrder.verify(mWifiMock).start();
        mInOrder.verify(mManagerStatusListenerMock, times(2)).onStatusChanged();

        verifyNoMoreInteractions(mManagerStatusListenerMock);
    }

    /**
     * Validate IWifi onFailure causes notification
     */
    @Test
    public void testWifiFail() throws Exception {
        mInOrder = inOrder(mServiceManagerMock, mWifiMock, mManagerStatusListenerMock);
        executeAndValidateInitializationSequence();
        executeAndValidateStartupSequence();

        // act: IWifi failure
        mWifiEventCallbackCaptor.getValue().onFailure(mStatusFail);
        mTestLooper.dispatchAll();

        // verify: getting onStop
        mInOrder.verify(mManagerStatusListenerMock).onStatusChanged();

        // act: start again
        collector.checkThat(mDut.start(), equalTo(true));
        mWifiEventCallbackCaptor.getValue().onStart();
        mTestLooper.dispatchAll();

        // verify: service and callback calls
        mInOrder.verify(mWifiMock).start();
        mInOrder.verify(mManagerStatusListenerMock).onStatusChanged();

        verifyNoMoreInteractions(mManagerStatusListenerMock);
    }

    /**
     * Validates that when (for some reason) the cache is out-of-sync with the actual chip status
     * then Wi-Fi is shut-down.
     *
     * Uses TestChipV1 - but nothing specific to its configuration. The test validates internal
     * HDM behavior.
     */
    @Test
    public void testCacheMismatchError() throws Exception {
        TestChipV1 chipMock = new TestChipV1();
        chipMock.initialize();
        mInOrder = inOrder(mServiceManagerMock, mWifiMock, chipMock.chip,
                mManagerStatusListenerMock);
        executeAndValidateInitializationSequence();
        executeAndValidateStartupSequence();

        InterfaceDestroyedListener staDestroyedListener = mock(
                InterfaceDestroyedListener.class);
        HalDeviceManager.InterfaceAvailableForRequestListener staAvailListener = mock(
                HalDeviceManager.InterfaceAvailableForRequestListener.class);

        InterfaceDestroyedListener nanDestroyedListener = mock(
                InterfaceDestroyedListener.class);
        HalDeviceManager.InterfaceAvailableForRequestListener nanAvailListener = mock(
                HalDeviceManager.InterfaceAvailableForRequestListener.class);

        InOrder availInOrder = inOrder(staAvailListener, nanAvailListener);

        // Request STA
        IWifiIface staIface = validateInterfaceSequence(chipMock,
                false, // chipModeValid
                -1000, // chipModeId (only used if chipModeValid is true)
                IfaceType.STA, // ifaceTypeToCreate
                "wlan0", // ifaceName
                TestChipV1.STA_CHIP_MODE_ID, // finalChipMode
                false, // high priority
                null, // tearDownList
                staDestroyedListener, // destroyedListener
                staAvailListener // availableListener
        );
        availInOrder.verify(staAvailListener).onAvailabilityChanged(false);

        // Request NAN
        IWifiIface nanIface = validateInterfaceSequence(chipMock,
                true, // chipModeValid
                TestChipV1.STA_CHIP_MODE_ID, // chipModeId
                IfaceType.NAN, // ifaceTypeToCreate
                "wlan0", // ifaceName
                TestChipV1.STA_CHIP_MODE_ID, // finalChipMode
                false, // high priority (but irrelevant)
                null, // tearDownList
                nanDestroyedListener, // destroyedListener
                nanAvailListener // availableListener
        );
        availInOrder.verify(nanAvailListener).onAvailabilityChanged(false);

        // fiddle with the "chip" by removing the STA
        chipMock.interfaceNames.get(IfaceType.STA).remove("wlan0");

        // now try to request another NAN
        IWifiIface nanIface2 = mDut.createNanIface(nanDestroyedListener, mHandler);
        collector.checkThat("NAN can't be created", nanIface2, IsNull.nullValue());

        mDut.registerInterfaceAvailableForRequestListener(IfaceType.NAN, nanAvailListener,
                mHandler);
        mTestLooper.dispatchAll();

        // extra (apparently duplicate) call since everything was cleaned-up once a cache mismatch
        // was detected - so this is a call on a new registration
        availInOrder.verify(nanAvailListener).onAvailabilityChanged(false);

        // verify that Wi-Fi is shut-down: should also get all onDestroyed messages that are
        // registered (even if they seem out-of-sync to chip)
        verify(mWifiMock, times(2)).stop();
        verify(mManagerStatusListenerMock, times(2)).onStatusChanged();
        verify(staDestroyedListener).onDestroyed(getName(staIface));
        verify(nanDestroyedListener).onDestroyed(getName(nanIface));

        verifyNoMoreInteractions(mManagerStatusListenerMock, staDestroyedListener, staAvailListener,
                nanDestroyedListener, nanAvailListener);
    }

    /**
     * Validates that a duplicate registration of the same InterfaceAvailableForRequestListener
     * listener will result in a single callback.
     *
     * Also validates that get an immediate call on registration if available.
     *
     * Uses TestChipV1 - but nothing specific to its configuration. The test validates internal
     * HDM behavior.
     */
    @Test
    public void testDuplicateAvailableRegistrations() throws Exception {
        TestChipV1 chipMock = new TestChipV1();
        chipMock.initialize();
        mInOrder = inOrder(mServiceManagerMock, mWifiMock, chipMock.chip,
                mManagerStatusListenerMock);
        executeAndValidateInitializationSequence();
        executeAndValidateStartupSequence();

        HalDeviceManager.InterfaceAvailableForRequestListener staAvailListener = mock(
                HalDeviceManager.InterfaceAvailableForRequestListener.class);

        // get STA interface
        IWifiIface staIface = validateInterfaceSequence(chipMock,
                false, // chipModeValid
                -1000, // chipModeId (only used if chipModeValid is true)
                IfaceType.STA, // ifaceTypeToCreate
                "wlan0", // ifaceName
                TestChipV1.STA_CHIP_MODE_ID, // finalChipMode
                false, // high priority
                null, // tearDownList
                null, // destroyedListener
                null // availableListener
        );
        collector.checkThat("STA created", staIface, IsNull.notNullValue());

        // act: register the same listener twice
        mDut.registerInterfaceAvailableForRequestListener(IfaceType.STA, staAvailListener,
                mHandler);
        mDut.registerInterfaceAvailableForRequestListener(IfaceType.STA, staAvailListener,
                mHandler);
        mTestLooper.dispatchAll();

        verify(staAvailListener).onAvailabilityChanged(false);

        // remove STA interface -> should trigger callbacks
        mDut.removeIface(staIface);
        mTestLooper.dispatchAll();

        // verify: only a single trigger
        verify(staAvailListener).onAvailabilityChanged(true);

        verifyNoMoreInteractions(staAvailListener);
    }

    /**
     * Validate that when no chip info is found an empty list is returned.
     */
    @Test
    public void testGetSupportedIfaceTypesError() throws Exception {
        // try API
        Set<Integer> results = mDut.getSupportedIfaceTypes();

        // verify results
        assertEquals(0, results.size());
    }

    /**
     * Test start HAL can retry upon failure.
     *
     * Uses TestChipV1 - but nothing specific to its configuration. The test validates internal
     * HDM behavior.
     */
    @Test
    public void testStartHalRetryUponNotAvailableFailure() throws Exception {
        // Override the stubbing for mWifiMock in before().
        when(mWifiMock.start())
                .thenReturn(getStatus(WifiStatusCode.ERROR_NOT_AVAILABLE))
                .thenReturn(mStatusOk);

        TestChipV1 chipMock = new TestChipV1();
        chipMock.initialize();
        mInOrder = inOrder(mServiceManagerMock, mWifiMock, chipMock.chip,
                mManagerStatusListenerMock);
        executeAndValidateInitializationSequence();
        executeAndValidateStartupSequence(2, true);
    }

    /**
     * Test start HAL fails after multiple retry failures.
     *
     * Uses TestChipV1 - but nothing specific to its configuration. The test validates internal
     * HDM behavior.
     */
    @Test
    public void testStartHalRetryFailUponMultipleNotAvailableFailures() throws Exception {
        // Override the stubbing for mWifiMock in before().
        when(mWifiMock.start()).thenReturn(getStatus(WifiStatusCode.ERROR_NOT_AVAILABLE));

        TestChipV1 chipMock = new TestChipV1();
        chipMock.initialize();
        mInOrder = inOrder(mServiceManagerMock, mWifiMock, chipMock.chip);
        executeAndValidateInitializationSequence();
        executeAndValidateStartupSequence(START_HAL_RETRY_TIMES + 1, false);
    }

    /**
     * Test start HAL fails after multiple retry failures.
     *
     * Uses TestChipV1 - but nothing specific to its configuration. The test validates internal
     * HDM behavior.
     */
    @Test
    public void testStartHalRetryFailUponTrueFailure() throws Exception {
        // Override the stubbing for mWifiMock in before().
        when(mWifiMock.start()).thenReturn(getStatus(WifiStatusCode.ERROR_UNKNOWN));

        TestChipV1 chipMock = new TestChipV1();
        chipMock.initialize();
        mInOrder = inOrder(mServiceManagerMock, mWifiMock, chipMock.chip);
        executeAndValidateInitializationSequence();
        executeAndValidateStartupSequence(1, false);
    }

    /**
     * Validate that isSupported() returns true when IServiceManager finds the vendor HAL daemon in
     * the VINTF.
     */
    @Test
    public void testIsSupportedTrue() throws Exception {
        mInOrder = inOrder(mServiceManagerMock, mWifiMock);
        executeAndValidateInitializationSequence();
        assertTrue(mDut.isSupported());
    }

    /**
     * Validate that isSupported() returns false when IServiceManager does not find the vendor HAL
     * daemon in the VINTF.
     */
    @Test
    public void testIsSupportedFalse() throws Exception {
        when(mServiceManagerMock.listManifestByInterface(eq(IWifi.kInterfaceName)))
                .thenReturn(new ArrayList());
        mInOrder = inOrder(mServiceManagerMock, mWifiMock);
        executeAndValidateInitializationSequence(false);
        assertFalse(mDut.isSupported());
    }

    /**
     * Validate RTT configuration when the callback is registered first and the chip is
     * configured later - i.e. RTT isn't available immediately.
     */
    @Test
    public void testAndTriggerRttLifecycleCallbacksRegBeforeChipConfig() throws Exception {
        HalDeviceManager.InterfaceRttControllerLifecycleCallback cb = mock(
                HalDeviceManager.InterfaceRttControllerLifecycleCallback.class);

        InOrder io = inOrder(cb);

        // initialize a test chip (V1 is fine since we're not testing any specifics of
        // concurrency in this test).
        ChipMockBase chipMock = new TestChipV1();
        chipMock.initialize();
        mInOrder = inOrder(mServiceManagerMock, mWifiMock, chipMock.chip,
                mManagerStatusListenerMock);
        executeAndValidateInitializationSequence();
        executeAndValidateStartupSequence();

        // register initial cb: don't expect RTT since chip isn't configured
        mDut.registerRttControllerLifecycleCallback(cb, mHandler);
        mTestLooper.dispatchAll();
        io.verify(cb, times(0)).onNewRttController(any());

        // create a STA - that will get the chip configured and get us an RTT controller
        validateInterfaceSequence(chipMock,
                false, // chipModeValid
                -1000, // chipModeId (only used if chipModeValid is true)
                IfaceType.STA,
                "wlan0",
                TestChipV1.STA_CHIP_MODE_ID,
                false, // high priority
                null, // tearDownList
                null, // destroyedListener
                null // availableListener
        );
        verify(chipMock.chip).createRttController(any(), any());
        io.verify(cb).onNewRttController(any());

        verifyNoMoreInteractions(cb);
    }

    /**
     * Validate the RTT Controller lifecycle using a multi-mode chip (i.e. a chip which can
     * switch modes, during which RTT is destroyed).
     *
     * 1. Validate that an RTT is created as soon as the callback is registered - if the chip
     * is already configured (i.e. it is possible to create the RTT controller).
     *
     * 2. Validate that only the registered callback is triggered, not previously registered ones
     * and not duplicate ones.
     *
     * 3. Validate that onDestroy callbacks are triggered on mode change.
     */
    @Test
    public void testAndTriggerRttLifecycleCallbacksMultiModeChip() throws Exception {
        HalDeviceManager.InterfaceRttControllerLifecycleCallback cb1 = mock(
                HalDeviceManager.InterfaceRttControllerLifecycleCallback.class);
        HalDeviceManager.InterfaceRttControllerLifecycleCallback cb2 = mock(
                HalDeviceManager.InterfaceRttControllerLifecycleCallback.class);

        InOrder io1 = inOrder(cb1);
        InOrder io2 = inOrder(cb2);

        // initialize a test chip (V1 is a must since we're testing a multi-mode chip) & create a
        // STA (which will configure the chip).
        ChipMockBase chipMock = new TestChipV1();
        chipMock.initialize();
        mInOrder = inOrder(mServiceManagerMock, mWifiMock, chipMock.chip,
                mManagerStatusListenerMock);
        executeAndValidateInitializationSequence();
        executeAndValidateStartupSequence();
        validateInterfaceSequence(chipMock,
                false, // chipModeValid
                -1000, // chipModeId (only used if chipModeValid is true)
                IfaceType.STA,
                "wlan0",
                TestChipV1.STA_CHIP_MODE_ID,
                false, // high priority
                null, // tearDownList
                null, // destroyedListener
                null // availableListener
        );
        mInOrder.verify(chipMock.chip, times(0)).createRttController(any(), any());

        // register initial cb - expect the cb right away
        mDut.registerRttControllerLifecycleCallback(cb1, mHandler);
        mTestLooper.dispatchAll();
        verify(chipMock.chip).createRttController(any(), any());
        io1.verify(cb1).onNewRttController(mRttControllerMock);

        // register a second callback and the first one again
        mDut.registerRttControllerLifecycleCallback(cb2, mHandler);
        mDut.registerRttControllerLifecycleCallback(cb1, mHandler);
        mTestLooper.dispatchAll();
        io2.verify(cb2).onNewRttController(mRttControllerMock);

        // change to AP mode (which for TestChipV1 doesn't allow RTT): trigger onDestroyed for all
        validateInterfaceSequence(chipMock,
                true, // chipModeValid
                TestChipV1.STA_CHIP_MODE_ID, // chipModeId (only used if chipModeValid is true)
                IfaceType.AP,
                "wlan0",
                TestChipV1.AP_CHIP_MODE_ID,
                false, // high priority
                null, // tearDownList
                null, // destroyedListener
                null // availableListener
        );
        mTestLooper.dispatchAll();
        verify(chipMock.chip, times(2)).createRttController(any(), any()); // but returns a null!
        io1.verify(cb1).onRttControllerDestroyed();
        io2.verify(cb2).onRttControllerDestroyed();

        // change back to STA mode (which for TestChipV1 will re-allow RTT): trigger onNew for all
        validateInterfaceSequence(chipMock,
                true, // chipModeValid
                TestChipV1.AP_CHIP_MODE_ID, // chipModeId (only used if chipModeValid is true)
                IfaceType.STA,
                "wlan0",
                TestChipV1.STA_CHIP_MODE_ID,
                false, // high priority
                null, // tearDownList
                null, // destroyedListener
                null // availableListener
        );
        mTestLooper.dispatchAll();
        verify(chipMock.chip, times(3)).createRttController(any(), any());
        io1.verify(cb1).onNewRttController(mRttControllerMock);
        io2.verify(cb2).onNewRttController(mRttControllerMock);

        verifyNoMoreInteractions(cb1, cb2);
    }

    /**
     * Validate the RTT Controller lifecycle using a single-mode chip. Specifically validate
     * that RTT isn't impacted during STA -> AP change.
     */
    @Test
    public void testAndTriggerRttLifecycleCallbacksSingleModeChip() throws Exception {
        HalDeviceManager.InterfaceRttControllerLifecycleCallback cb = mock(
                HalDeviceManager.InterfaceRttControllerLifecycleCallback.class);

        InOrder io = inOrder(cb);

        // initialize a test chip (V2 is a must since we need a single mode chip)
        // & create a STA (which will configure the chip).
        ChipMockBase chipMock = new TestChipV2();
        chipMock.initialize();
        mInOrder = inOrder(mServiceManagerMock, mWifiMock, chipMock.chip,
                mManagerStatusListenerMock);
        executeAndValidateInitializationSequence();
        executeAndValidateStartupSequence();
        validateInterfaceSequence(chipMock,
                false, // chipModeValid
                -1000, // chipModeId (only used if chipModeValid is true)
                IfaceType.STA,
                "wlan0",
                TestChipV2.CHIP_MODE_ID,
                false, // high priority
                null, // tearDownList
                null, // destroyedListener
                null // availableListener
        );
        mInOrder.verify(chipMock.chip, times(0)).createRttController(any(), any());

        // register initial cb - expect the cb right away
        mDut.registerRttControllerLifecycleCallback(cb, mHandler);
        mTestLooper.dispatchAll();
        verify(chipMock.chip).createRttController(any(), any());
        io.verify(cb).onNewRttController(mRttControllerMock);

        // create an AP: no mode change for TestChipV2 -> expect no impact on RTT
        validateInterfaceSequence(chipMock,
                true, // chipModeValid
                TestChipV2.CHIP_MODE_ID, // chipModeId (only used if chipModeValid is true)
                IfaceType.AP,
                "wlan0",
                TestChipV2.CHIP_MODE_ID,
                false, // high priority
                null, // tearDownList
                null, // destroyedListener
                null // availableListener
        );
        mTestLooper.dispatchAll();

        verifyNoMoreInteractions(cb);
    }

    //////////////////////////////////////////////////////////////////////////////////////
    // Chip Specific Tests - but should work on all chips!
    // (i.e. add copies for each test chip)
    //////////////////////////////////////////////////////////////////////////////////////

    // TestChipV1

    /**
     * Validate creation of STA interface from blank start-up. The remove interface.
     */
    @Test
    public void testCreateStaInterfaceNoInitModeTestChipV1() throws Exception {
        runCreateSingleXxxInterfaceNoInitMode(new TestChipV1(), IfaceType.STA, "wlan0",
                TestChipV1.STA_CHIP_MODE_ID, false);
    }

    /**
     * Validate creation of AP interface from blank start-up. The remove interface.
     */
    @Test
    public void testCreateApInterfaceNoInitModeTestChipV1() throws Exception {
        runCreateSingleXxxInterfaceNoInitMode(new TestChipV1(), IfaceType.AP, "wlan0",
                TestChipV1.AP_CHIP_MODE_ID, false);
    }

    /**
     * Validate creation of P2P interface from blank start-up. The remove interface.
     */
    @Test
    public void testCreateP2pInterfaceNoInitModeTestChipV1() throws Exception {
        runCreateSingleXxxInterfaceNoInitMode(new TestChipV1(), IfaceType.P2P, "p2p0",
                TestChipV1.STA_CHIP_MODE_ID, false);
    }

    /**
     * Validate creation of NAN interface from blank start-up. The remove interface.
     */
    @Test
    public void testCreateNanInterfaceNoInitModeTestChipV1() throws Exception {
        runCreateSingleXxxInterfaceNoInitMode(new TestChipV1(), IfaceType.NAN, "wlan0",
                TestChipV1.STA_CHIP_MODE_ID, false);
    }

    // TestChipV2

    /**
     * Validate creation of STA interface from blank start-up. The remove interface.
     */
    @Test
    public void testCreateStaInterfaceNoInitModeTestChipV2() throws Exception {
        // Note: we expected 2 available callbacks since we now have 2 STAs possible. So
        // we get callback 1 after creating the first STA (since we can create another STA),
        // and we get callback 2 after destroying the first STA (since we can create another STA -
        // as expected).
        runCreateSingleXxxInterfaceNoInitMode(new TestChipV2(), IfaceType.STA, "wlan0",
                TestChipV2.CHIP_MODE_ID, true);
    }

    /**
     * Validate creation of AP interface from blank start-up. The remove interface.
     */
    @Test
    public void testCreateApInterfaceNoInitModeTestChipV2() throws Exception {
        runCreateSingleXxxInterfaceNoInitMode(new TestChipV2(), IfaceType.AP, "wlan0",
                TestChipV2.CHIP_MODE_ID, false);
    }

    /**
     * Validate creation of P2P interface from blank start-up. The remove interface.
     */
    @Test
    public void testCreateP2pInterfaceNoInitModeTestChipV2() throws Exception {
        runCreateSingleXxxInterfaceNoInitMode(new TestChipV2(), IfaceType.P2P, "p2p0",
                TestChipV2.CHIP_MODE_ID, false);
    }

    /**
     * Validate creation of NAN interface from blank start-up. The remove interface.
     */
    @Test
    public void testCreateNanInterfaceNoInitModeTestChipV2() throws Exception {
        runCreateSingleXxxInterfaceNoInitMode(new TestChipV2(), IfaceType.NAN, "wlan0",
                TestChipV2.CHIP_MODE_ID, false);
    }

    // TestChipV3

    /**
     * Validate creation of STA interface from blank start-up. The remove interface.
     */
    @Test
    public void testCreateStaInterfaceNoInitModeTestChipV3() throws Exception {
        // Note: we expected 2 available callbacks since we now have 2 STAs possible. So
        // we get callback 1 after creating the first STA (since we can create another STA),
        // and we get callback 2 after destroying the first STA (since we can create another STA -
        // as expected).
        runCreateSingleXxxInterfaceNoInitMode(new TestChipV3(), IfaceType.STA, "wlan0",
                TestChipV3.CHIP_MODE_ID, true);
    }

    /**
     * Validate creation of AP interface from blank start-up. The remove interface.
     */
    @Test
    public void testCreateApInterfaceNoInitModeTestChipV3() throws Exception {
        runCreateSingleXxxInterfaceNoInitMode(new TestChipV3(), IfaceType.AP, "wlan0",
                TestChipV3.CHIP_MODE_ID, false);
    }

    /**
     * Validate creation of P2P interface from blank start-up. The remove interface.
     */
    @Test
    public void testCreateP2pInterfaceNoInitModeTestChipV3() throws Exception {
        runCreateSingleXxxInterfaceNoInitMode(new TestChipV3(), IfaceType.P2P, "p2p0",
                TestChipV3.CHIP_MODE_ID, false);
    }

    /**
     * Validate creation of NAN interface from blank start-up. The remove interface.
     */
    @Test
    public void testCreateNanInterfaceNoInitModeTestChipV3() throws Exception {
        runCreateSingleXxxInterfaceNoInitMode(new TestChipV3(), IfaceType.NAN, "wlan0",
                TestChipV3.CHIP_MODE_ID, false);
    }

    // TestChipV4

    /**
     * Validate creation of STA interface from blank start-up. The remove interface.
     */
    @Test
    public void testCreateStaInterfaceNoInitModeTestChipV4() throws Exception {
        runCreateSingleXxxInterfaceNoInitMode(new TestChipV4(), IfaceType.STA, "wlan0",
                TestChipV4.CHIP_MODE_ID, false);
    }

    /**
     * Validate creation of AP interface from blank start-up. The remove interface.
     */
    @Test
    public void testCreateApInterfaceNoInitModeTestChipV4() throws Exception {
        runCreateSingleXxxInterfaceNoInitMode(new TestChipV4(), IfaceType.AP, "wlan0",
                TestChipV4.CHIP_MODE_ID, false);
    }

    /**
     * Validate creation of P2P interface from blank start-up. The remove interface.
     */
    @Test
    public void testCreateP2pInterfaceNoInitModeTestChipV4() throws Exception {
        runCreateSingleXxxInterfaceNoInitMode(new TestChipV4(), IfaceType.P2P, "p2p0",
                TestChipV4.CHIP_MODE_ID, false);
    }

    /**
     * Validate creation of NAN interface from blank start-up. The remove interface.
     */
    @Test
    public void testCreateNanInterfaceNoInitModeTestChipV4() throws Exception {
        runCreateSingleXxxInterfaceNoInitMode(new TestChipV4(), IfaceType.NAN, "wlan0",
                TestChipV4.CHIP_MODE_ID, false);
    }

    //////////////////////////////////////////////////////////////////////////////////////
    // TestChipV1 Specific Tests
    //////////////////////////////////////////////////////////////////////////////////////

    /**
     * Validate creation of AP interface when in STA mode - but with no interface created. Expect
     * a change in chip mode.
     */
    @Test
    public void testCreateApWithStaModeUpTestChipV1() throws Exception {
        final String name = "wlan0";

        TestChipV1 chipMock = new TestChipV1();
        chipMock.initialize();
        mInOrder = inOrder(mServiceManagerMock, mWifiMock, chipMock.chip,
                mManagerStatusListenerMock);
        executeAndValidateInitializationSequence();
        executeAndValidateStartupSequence();

        InterfaceDestroyedListener idl = mock(
                InterfaceDestroyedListener.class);
        HalDeviceManager.InterfaceAvailableForRequestListener iafrl = mock(
                HalDeviceManager.InterfaceAvailableForRequestListener.class);

        IWifiApIface iface = (IWifiApIface) validateInterfaceSequence(chipMock,
                true, // chipModeValid
                TestChipV1.STA_CHIP_MODE_ID, // chipModeId
                IfaceType.AP, // ifaceTypeToCreate
                name, // ifaceName
                TestChipV1.AP_CHIP_MODE_ID, // finalChipMode
                false, // high priority (but irrelevant)
                null, // tearDownList
                idl, // destroyedListener
                iafrl // availableListener
        );
        collector.checkThat("allocated interface", iface, IsNull.notNullValue());

        verify(iafrl).onAvailabilityChanged(false);

        // act: stop Wi-Fi
        mDut.stop();
        mTestLooper.dispatchAll();

        // verify: callback triggered
        verify(idl).onDestroyed(getName(iface));
        verify(mManagerStatusListenerMock, times(2)).onStatusChanged();

        verifyNoMoreInteractions(mManagerStatusListenerMock, idl, iafrl);
    }

    /**
     * Validate creation of AP interface when in STA mode with a single STA iface created.
     * Expect a change in chip mode.
     */
    @Test
    public void testCreateApWithStIfaceUpTestChipV1UsingNoHandlerListeners() throws Exception {
        TestChipV1 chipMock = new TestChipV1();
        chipMock.initialize();

        InterfaceDestroyedListener staIdl = mock(
                InterfaceDestroyedListener.class);
        HalDeviceManager.InterfaceAvailableForRequestListener staIafrl = mock(
                HalDeviceManager.InterfaceAvailableForRequestListener.class);
        InterfaceDestroyedListener apIdl = mock(
                InterfaceDestroyedListener.class);
        HalDeviceManager.InterfaceAvailableForRequestListener apIafrl = mock(
                HalDeviceManager.InterfaceAvailableForRequestListener.class);

        mInOrder = inOrder(mServiceManagerMock, mWifiMock, chipMock.chip,
                mManagerStatusListenerMock, staIdl, staIafrl, apIdl, apIafrl);
        executeAndValidateInitializationSequence();

        // Register listener & start Wi-Fi
        mDut.registerStatusListener(mManagerStatusListenerMock, null);
        assertTrue(mDut.start());
        mInOrder.verify(mManagerStatusListenerMock).onStatusChanged();

        mDut.registerInterfaceAvailableForRequestListener(IfaceType.STA, staIafrl, null);
        mDut.registerInterfaceAvailableForRequestListener(IfaceType.AP, apIafrl, null);

        mInOrder.verify(staIafrl).onAvailabilityChanged(true);
        mInOrder.verify(apIafrl).onAvailabilityChanged(true);

        // Create STA Iface first.
        IWifiStaIface staIface = mock(IWifiStaIface.class);
        doAnswer(new GetNameAnswer("wlan0")).when(staIface).getName(
                any(IWifiIface.getNameCallback.class));
        doAnswer(new GetTypeAnswer(IfaceType.STA)).when(staIface).getType(
                any(IWifiIface.getTypeCallback.class));
        doAnswer(new CreateXxxIfaceAnswer(chipMock, mStatusOk, staIface)).when(
                chipMock.chip).createStaIface(any(IWifiChip.createStaIfaceCallback.class));
        assertEquals(staIface, mDut.createStaIface(false, staIdl, null));

        mInOrder.verify(chipMock.chip).configureChip(TestChipV1.STA_CHIP_MODE_ID);
        mInOrder.verify(staIafrl).onAvailabilityChanged(false);

        // Now Create AP Iface.
        IWifiApIface apIface = mock(IWifiApIface.class);
        doAnswer(new GetNameAnswer("wlan0")).when(apIface).getName(
                any(IWifiIface.getNameCallback.class));
        doAnswer(new GetTypeAnswer(IfaceType.AP)).when(apIface).getType(
                any(IWifiIface.getTypeCallback.class));
        doAnswer(new CreateXxxIfaceAnswer(chipMock, mStatusOk, apIface)).when(
                chipMock.chip).createApIface(any(IWifiChip.createApIfaceCallback.class));
        assertEquals(apIface, mDut.createApIface(apIdl, null));

        mInOrder.verify(chipMock.chip).removeStaIface(getName(staIface));
        mInOrder.verify(staIdl).onDestroyed(getName(staIface));
        mInOrder.verify(chipMock.chip).configureChip(TestChipV1.AP_CHIP_MODE_ID);
        mInOrder.verify(apIafrl).onAvailabilityChanged(false);
        mInOrder.verify(staIafrl).onAvailabilityChanged(true);

        // Stop Wi-Fi
        mDut.stop();

        mInOrder.verify(mWifiMock).stop();
        mInOrder.verify(mManagerStatusListenerMock).onStatusChanged();
        mInOrder.verify(apIdl).onDestroyed(getName(apIface));

        verifyNoMoreInteractions(mManagerStatusListenerMock, staIdl, staIafrl, apIdl, apIafrl);
    }

    /**
     * Validate creation of AP interface when in AP mode - but with no interface created. Expect
     * no change in chip mode.
     */
    @Test
    public void testCreateApWithApModeUpTestChipV1() throws Exception {
        final String name = "wlan0";

        TestChipV1 chipMock = new TestChipV1();
        chipMock.initialize();
        mInOrder = inOrder(mServiceManagerMock, mWifiMock, chipMock.chip,
                mManagerStatusListenerMock);
        executeAndValidateInitializationSequence();
        executeAndValidateStartupSequence();

        InterfaceDestroyedListener idl = mock(
                InterfaceDestroyedListener.class);
        HalDeviceManager.InterfaceAvailableForRequestListener iafrl = mock(
                HalDeviceManager.InterfaceAvailableForRequestListener.class);

        IWifiApIface iface = (IWifiApIface) validateInterfaceSequence(chipMock,
                true, // chipModeValid
                TestChipV1.AP_CHIP_MODE_ID, // chipModeId
                IfaceType.AP, // ifaceTypeToCreate
                name, // ifaceName
                TestChipV1.AP_CHIP_MODE_ID, // finalChipMode
                false, // high priority (but irrelevant)
                null, // tearDownList
                idl, // destroyedListener
                iafrl // availableListener
        );
        collector.checkThat("allocated interface", iface, IsNull.notNullValue());

        verify(iafrl).onAvailabilityChanged(false);

        // act: stop Wi-Fi
        mDut.stop();
        mTestLooper.dispatchAll();

        // verify: callback triggered
        verify(idl).onDestroyed(getName(iface));
        verify(mManagerStatusListenerMock, times(2)).onStatusChanged();

        verifyNoMoreInteractions(mManagerStatusListenerMock, idl, iafrl);
    }

    /**
     * Validate AP up/down creation of AP interface when a STA already created. Expect:
     * - STA created
     * - P2P created
     * - When AP requested:
     *   - STA & P2P torn down
     *   - AP created
     * - P2P creation refused
     * - Request STA: will tear down AP
     * - When AP destroyed:
     *   - Get p2p available listener callback
     *   - Can create P2P when requested
     * - Create P2P
     * - Request NAN: will get refused
     * - Tear down P2P:
     *    - should get nan available listener callback
     *    - Can create NAN when requested
     */
    @Test
    public void testCreateSameAndDiffPrioritiesTestChipV1() throws Exception {
        TestChipV1 chipMock = new TestChipV1();
        chipMock.initialize();
        mInOrder = inOrder(mServiceManagerMock, mWifiMock, chipMock.chip,
                mManagerStatusListenerMock);
        executeAndValidateInitializationSequence();
        executeAndValidateStartupSequence();

        InterfaceDestroyedListener staDestroyedListener = mock(
                InterfaceDestroyedListener.class);
        HalDeviceManager.InterfaceAvailableForRequestListener staAvailListener = mock(
                HalDeviceManager.InterfaceAvailableForRequestListener.class);

        InterfaceDestroyedListener staDestroyedListener2 = mock(
                InterfaceDestroyedListener.class);

        InterfaceDestroyedListener apDestroyedListener = mock(
                InterfaceDestroyedListener.class);
        HalDeviceManager.InterfaceAvailableForRequestListener apAvailListener = mock(
                HalDeviceManager.InterfaceAvailableForRequestListener.class);

        InterfaceDestroyedListener p2pDestroyedListener = mock(
                InterfaceDestroyedListener.class);
        HalDeviceManager.InterfaceAvailableForRequestListener p2pAvailListener = mock(
                HalDeviceManager.InterfaceAvailableForRequestListener.class);

        InterfaceDestroyedListener p2pDestroyedListener2 = mock(
                InterfaceDestroyedListener.class);

        InterfaceDestroyedListener nanDestroyedListener = mock(
                InterfaceDestroyedListener.class);
        HalDeviceManager.InterfaceAvailableForRequestListener nanAvailListener = mock(
                HalDeviceManager.InterfaceAvailableForRequestListener.class);

        InOrder inOrderAvail = inOrder(staAvailListener, apAvailListener, p2pAvailListener,
                nanAvailListener);

        // register listeners for interface availability
        mDut.registerInterfaceAvailableForRequestListener(IfaceType.STA, staAvailListener,
                mHandler);
        mDut.registerInterfaceAvailableForRequestListener(IfaceType.AP, apAvailListener, mHandler);
        mDut.registerInterfaceAvailableForRequestListener(IfaceType.P2P, p2pAvailListener,
                mHandler);
        mDut.registerInterfaceAvailableForRequestListener(IfaceType.NAN, nanAvailListener,
                mHandler);
        mTestLooper.dispatchAll();

        inOrderAvail.verify(staAvailListener).onAvailabilityChanged(true);
        inOrderAvail.verify(apAvailListener).onAvailabilityChanged(true);
        inOrderAvail.verify(p2pAvailListener).onAvailabilityChanged(true);
        inOrderAvail.verify(nanAvailListener).onAvailabilityChanged(true);

        // Request STA
        IWifiIface staIface = validateInterfaceSequence(chipMock,
                false, // chipModeValid
                -1000, // chipModeId (only used if chipModeValid is true)
                IfaceType.STA, // ifaceTypeToCreate
                "wlan0", // ifaceName
                TestChipV1.STA_CHIP_MODE_ID, // finalChipMode
                false, // high priority
                null, // tearDownList
                staDestroyedListener, // destroyedListener
                null // availableListener
        );
        collector.checkThat("allocated STA interface", staIface, IsNull.notNullValue());

        inOrderAvail.verify(staAvailListener).onAvailabilityChanged(false);

        // request STA2: should fail
        IWifiIface staIface2 = mDut.createStaIface(false, null, null);
        collector.checkThat("STA2 should not be created", staIface2, IsNull.nullValue());

        // register additional InterfaceDestroyedListeners - including a duplicate (verify that
        // only called once!)
        mDut.registerDestroyedListener(staIface, staDestroyedListener2, mHandler);
        mDut.registerDestroyedListener(staIface, staDestroyedListener, mHandler);

        // Request P2P
        IWifiIface p2pIface = validateInterfaceSequence(chipMock,
                true, // chipModeValid
                TestChipV1.STA_CHIP_MODE_ID, // chipModeId
                IfaceType.P2P, // ifaceTypeToCreate
                "p2p0", // ifaceName
                TestChipV1.STA_CHIP_MODE_ID, // finalChipMode
                false, // high priority (but irrelevant)
                null, // tearDownList
                p2pDestroyedListener, // destroyedListener
                null // availableListener
        );
        collector.checkThat("allocated P2P interface", p2pIface, IsNull.notNullValue());

        inOrderAvail.verify(p2pAvailListener).onAvailabilityChanged(false);
        inOrderAvail.verify(nanAvailListener).onAvailabilityChanged(false);

        // Request AP
        IWifiIface apIface = validateInterfaceSequence(chipMock,
                true, // chipModeValid
                TestChipV1.STA_CHIP_MODE_ID, // chipModeId
                IfaceType.AP, // ifaceTypeToCreate
                "wlan0", // ifaceName
                TestChipV1.AP_CHIP_MODE_ID, // finalChipMode
                false, // high priority (but irrelevant)
                new IWifiIface[]{staIface, p2pIface}, // tearDownList
                apDestroyedListener, // destroyedListener
                null, // availableListener
                // destroyedInterfacesDestroyedListeners...
                new InterfaceDestroyedListenerWithIfaceName(
                        getName(staIface), staDestroyedListener),
                new InterfaceDestroyedListenerWithIfaceName(
                        getName(staIface), staDestroyedListener2),
                new InterfaceDestroyedListenerWithIfaceName(
                        getName(p2pIface), p2pDestroyedListener)
        );
        collector.checkThat("allocated AP interface", apIface, IsNull.notNullValue());

        inOrderAvail.verify(apAvailListener).onAvailabilityChanged(false);
        inOrderAvail.verify(staAvailListener).onAvailabilityChanged(true);

        // request AP2: should fail
        IWifiIface apIface2 = mDut.createApIface(null, null);
        collector.checkThat("AP2 should not be created", apIface2, IsNull.nullValue());

        // Request P2P: expect failure
        p2pIface = mDut.createP2pIface(p2pDestroyedListener, mHandler);
        collector.checkThat("P2P can't be created", p2pIface, IsNull.nullValue());

        // Request STA: expect success
        staIface = validateInterfaceSequence(chipMock,
                true, // chipModeValid
                TestChipV1.AP_CHIP_MODE_ID, // chipModeId
                IfaceType.STA, // ifaceTypeToCreate
                "wlan0", // ifaceName
                TestChipV1.STA_CHIP_MODE_ID, // finalChipMode
                false, // high priority
                null, // tearDownList
                staDestroyedListener, // destroyedListener
                null, // availableListener
                // destroyedInterfacesDestroyedListeners...
                new InterfaceDestroyedListenerWithIfaceName(
                        getName(apIface), apDestroyedListener)
        );
        collector.checkThat("allocated STA interface", staIface, IsNull.notNullValue());

        inOrderAvail.verify(apAvailListener).onAvailabilityChanged(true);
        inOrderAvail.verify(staAvailListener).onAvailabilityChanged(false);
        inOrderAvail.verify(p2pAvailListener).onAvailabilityChanged(true);
        inOrderAvail.verify(nanAvailListener).onAvailabilityChanged(true);

        mTestLooper.dispatchAll();
        verify(apDestroyedListener).onDestroyed(getName(apIface));

        // Request P2P: expect success now
        p2pIface = validateInterfaceSequence(chipMock,
                true, // chipModeValid
                TestChipV1.STA_CHIP_MODE_ID, // chipModeId
                IfaceType.P2P, // ifaceTypeToCreate
                "p2p0", // ifaceName
                TestChipV1.STA_CHIP_MODE_ID, // finalChipMode
                false, // high priority (but irrelevant)
                null, // tearDownList
                p2pDestroyedListener2, // destroyedListener
                null // availableListener
        );

        inOrderAvail.verify(p2pAvailListener).onAvailabilityChanged(false);
        inOrderAvail.verify(nanAvailListener).onAvailabilityChanged(false);

        // Request NAN: should fail
        IWifiIface nanIface = mDut.createNanIface(nanDestroyedListener, mHandler);
        mDut.registerInterfaceAvailableForRequestListener(IfaceType.NAN, nanAvailListener,
                mHandler);
        collector.checkThat("NAN can't be created", nanIface, IsNull.nullValue());

        // Tear down P2P
        mDut.removeIface(p2pIface);
        mTestLooper.dispatchAll();

        inOrderAvail.verify(p2pAvailListener).onAvailabilityChanged(true);
        inOrderAvail.verify(nanAvailListener).onAvailabilityChanged(true);
        verify(chipMock.chip, times(2)).removeP2pIface("p2p0");
        verify(p2pDestroyedListener2).onDestroyed(getName(p2pIface));

        // Should now be able to request and get NAN
        nanIface = validateInterfaceSequence(chipMock,
                true, // chipModeValid
                TestChipV1.STA_CHIP_MODE_ID, // chipModeId
                IfaceType.NAN, // ifaceTypeToCreate
                "wlan0", // ifaceName
                TestChipV1.STA_CHIP_MODE_ID, // finalChipMode
                false, // high priority (but irrelevant)
                null, // tearDownList
                nanDestroyedListener, // destroyedListener
                nanAvailListener // availableListener
        );
        collector.checkThat("allocated NAN interface", nanIface, IsNull.notNullValue());

        inOrderAvail.verify(nanAvailListener).onAvailabilityChanged(false);

        verifyNoMoreInteractions(mManagerStatusListenerMock, staDestroyedListener, staAvailListener,
                staDestroyedListener2, apDestroyedListener, apAvailListener, p2pDestroyedListener,
                nanDestroyedListener, nanAvailListener, p2pDestroyedListener2);
    }

    /**
     * Validate P2P and NAN interactions. Expect:
     * - STA created
     * - NAN created
     * - When P2P requested:
     *   - NAN torn down
     *   - P2P created
     * - NAN creation refused
     * - When P2P destroyed:
     *   - get nan available listener
     *   - Can create NAN when requested
     */
    @Test
    public void testP2pAndNanInteractionsTestChipV1() throws Exception {
        runP2pAndNanExclusiveInteractionsTestChip(new TestChipV1(), TestChipV1.STA_CHIP_MODE_ID);
    }

    /**
     * Validates that trying to allocate a STA and then another STA fails. Only one STA at a time
     * is permitted (by TestChipV1 chip).
     */
    @Test
    public void testDuplicateStaRequestsTestChipV1() throws Exception {
        TestChipV1 chipMock = new TestChipV1();
        chipMock.initialize();
        mInOrder = inOrder(mServiceManagerMock, mWifiMock, chipMock.chip,
                mManagerStatusListenerMock);
        executeAndValidateInitializationSequence();
        executeAndValidateStartupSequence();

        InterfaceDestroyedListener staDestroyedListener1 = mock(
                InterfaceDestroyedListener.class);
        HalDeviceManager.InterfaceAvailableForRequestListener staAvailListener1 = mock(
                HalDeviceManager.InterfaceAvailableForRequestListener.class);

        InterfaceDestroyedListener staDestroyedListener2 = mock(
                InterfaceDestroyedListener.class);

        // get STA interface
        IWifiIface staIface1 = validateInterfaceSequence(chipMock,
                false, // chipModeValid
                -1000, // chipModeId (only used if chipModeValid is true)
                IfaceType.STA, // ifaceTypeToCreate
                "wlan0", // ifaceName
                TestChipV1.STA_CHIP_MODE_ID, // finalChipMode
                false, // high priority
                null, // tearDownList
                staDestroyedListener1, // destroyedListener
                staAvailListener1 // availableListener
        );
        collector.checkThat("STA created", staIface1, IsNull.notNullValue());

        verify(staAvailListener1).onAvailabilityChanged(false);

        // get STA interface again
        IWifiIface staIface2 = mDut.createStaIface(false, staDestroyedListener2, mHandler);
        collector.checkThat("STA created", staIface2, IsNull.nullValue());

        verifyNoMoreInteractions(mManagerStatusListenerMock, staDestroyedListener1,
                staAvailListener1, staDestroyedListener2);
    }

    /**
     * Validate that the getSupportedIfaceTypes API works when requesting for all chips.
     */
    @Test
    public void testGetSupportedIfaceTypesAllTestChipV1() throws Exception {
        TestChipV1 chipMock = new TestChipV1();
        chipMock.initialize();
        mInOrder = inOrder(mServiceManagerMock, mWifiMock, chipMock.chip,
                mManagerStatusListenerMock);
        executeAndValidateInitializationSequence();
        executeAndValidateStartupSequence();

        // try API
        Set<Integer> results = mDut.getSupportedIfaceTypes();

        // verify results
        Set<Integer> correctResults = new HashSet<>();
        correctResults.add(IfaceType.AP);
        correctResults.add(IfaceType.STA);
        correctResults.add(IfaceType.P2P);
        correctResults.add(IfaceType.NAN);

        assertEquals(correctResults, results);
    }

    /**
     * Validate that the getSupportedIfaceTypes API works when requesting for a specific chip.
     */
    @Test
    public void testGetSupportedIfaceTypesOneChipTestChipV1() throws Exception {
        TestChipV1 chipMock = new TestChipV1();
        chipMock.initialize();
        mInOrder = inOrder(mServiceManagerMock, mWifiMock, chipMock.chip,
                mManagerStatusListenerMock);
        executeAndValidateInitializationSequence();
        executeAndValidateStartupSequence();

        // try API
        Set<Integer> results = mDut.getSupportedIfaceTypes(chipMock.chip);

        // verify results
        Set<Integer> correctResults = new HashSet<>();
        correctResults.add(IfaceType.AP);
        correctResults.add(IfaceType.STA);
        correctResults.add(IfaceType.P2P);
        correctResults.add(IfaceType.NAN);

        assertEquals(correctResults, results);
    }

    //////////////////////////////////////////////////////////////////////////////////////
    // TestChipV2 Specific Tests
    //////////////////////////////////////////////////////////////////////////////////////

    /**
     * Validate a flow sequence for test chip 2:
     * - create STA
     * - create P2P
     * - request NAN: failure
     * - create AP
     * - create STA: will get refused
     * - create AP: will get refused
     * - tear down AP
     * - create STA
     * - create STA: will get refused
     * - create AP: should get created and the last created STA should get destroyed
     * - tear down P2P
     * - create NAN
     */
    @Test
    public void testInterfaceCreationFlowTestChipV2() throws Exception {
        TestChipV2 chipMock = new TestChipV2();
        chipMock.initialize();
        mInOrder = inOrder(mServiceManagerMock, mWifiMock, chipMock.chip,
                mManagerStatusListenerMock);
        executeAndValidateInitializationSequence();
        executeAndValidateStartupSequence();

        InterfaceDestroyedListener staDestroyedListener = mock(
                InterfaceDestroyedListener.class);
        InterfaceDestroyedListener staDestroyedListener2 = mock(
                InterfaceDestroyedListener.class);
        HalDeviceManager.InterfaceAvailableForRequestListener staAvailListener = mock(
                HalDeviceManager.InterfaceAvailableForRequestListener.class);

        InterfaceDestroyedListener apDestroyedListener = mock(
                InterfaceDestroyedListener.class);
        HalDeviceManager.InterfaceAvailableForRequestListener apAvailListener = mock(
                HalDeviceManager.InterfaceAvailableForRequestListener.class);

        InterfaceDestroyedListener p2pDestroyedListener = mock(
                InterfaceDestroyedListener.class);
        HalDeviceManager.InterfaceAvailableForRequestListener p2pAvailListener = mock(
                HalDeviceManager.InterfaceAvailableForRequestListener.class);

        InterfaceDestroyedListener nanDestroyedListener = mock(
                InterfaceDestroyedListener.class);
        HalDeviceManager.InterfaceAvailableForRequestListener nanAvailListener = mock(
                HalDeviceManager.InterfaceAvailableForRequestListener.class);

        InOrder inOrderAvail = inOrder(staAvailListener, apAvailListener, p2pAvailListener,
                nanAvailListener);

        // register listeners for interface availability
        mDut.registerInterfaceAvailableForRequestListener(IfaceType.STA, staAvailListener,
                mHandler);
        mDut.registerInterfaceAvailableForRequestListener(IfaceType.AP, apAvailListener, mHandler);
        mDut.registerInterfaceAvailableForRequestListener(IfaceType.P2P, p2pAvailListener,
                mHandler);
        mDut.registerInterfaceAvailableForRequestListener(IfaceType.NAN, nanAvailListener,
                mHandler);
        mTestLooper.dispatchAll();

        inOrderAvail.verify(staAvailListener).onAvailabilityChanged(true);
        inOrderAvail.verify(apAvailListener).onAvailabilityChanged(true);
        inOrderAvail.verify(p2pAvailListener).onAvailabilityChanged(true);
        inOrderAvail.verify(nanAvailListener).onAvailabilityChanged(true);

        // create STA
        when(mClock.getUptimeSinceBootMillis()).thenReturn(15L);
        IWifiIface staIface = validateInterfaceSequence(chipMock,
                false, // chipModeValid
                -1000, // chipModeId (only used if chipModeValid is true)
                IfaceType.STA, // ifaceTypeToCreate
                "wlan0", // ifaceName
                TestChipV2.CHIP_MODE_ID, // finalChipMode
                false, // high priority
                null, // tearDownList
                staDestroyedListener, // destroyedListener
                null // availableListener (already registered)
        );
        collector.checkThat("STA interface wasn't created", staIface, IsNull.notNullValue());

        // create P2P
        IWifiIface p2pIface = validateInterfaceSequence(chipMock,
                true, // chipModeValid
                TestChipV2.CHIP_MODE_ID, // chipModeId
                IfaceType.P2P, // ifaceTypeToCreate
                "p2p0", // ifaceName
                TestChipV2.CHIP_MODE_ID, // finalChipMode
                false, // high priority (but irrelevant)
                null, // tearDownList
                p2pDestroyedListener, // destroyedListener
                null // availableListener (already registered)
        );
        collector.checkThat("P2P interface wasn't created", p2pIface, IsNull.notNullValue());

        inOrderAvail.verify(p2pAvailListener).onAvailabilityChanged(false);
        inOrderAvail.verify(nanAvailListener).onAvailabilityChanged(false);

        // request NAN: should fail
        IWifiIface nanIface = mDut.createNanIface(null, null);
        collector.checkThat("NAN should not be created", nanIface, IsNull.nullValue());

        // create AP
        IWifiIface apIface = validateInterfaceSequence(chipMock,
                true, // chipModeValid
                TestChipV2.CHIP_MODE_ID, // chipModeId
                IfaceType.AP, // ifaceTypeToCreate
                "wlan1", // ifaceName
                TestChipV2.CHIP_MODE_ID, // finalChipMode
                false, // high priority (but irrelevant)
                null, // tearDownList
                apDestroyedListener, // destroyedListener
                null // availableListener (already registered)
        );
        collector.checkThat("AP interface wasn't created", apIface, IsNull.notNullValue());

        inOrderAvail.verify(apAvailListener).onAvailabilityChanged(false);
        inOrderAvail.verify(staAvailListener).onAvailabilityChanged(false);

        // request STA2: should fail
        IWifiIface staIface2 = mDut.createStaIface(false, null, null);
        collector.checkThat("STA2 should not be created", staIface2, IsNull.nullValue());

        // request AP2: should fail
        IWifiIface apIface2 = mDut.createApIface(null, null);
        collector.checkThat("AP2 should not be created", apIface2, IsNull.nullValue());

        // tear down AP
        mDut.removeIface(apIface);
        mTestLooper.dispatchAll();

        inOrderAvail.verify(apAvailListener).onAvailabilityChanged(true);
        inOrderAvail.verify(staAvailListener).onAvailabilityChanged(true);
        verify(chipMock.chip).removeApIface("wlan1");
        verify(apDestroyedListener).onDestroyed(getName(apIface));

        // create STA2: using a later clock
        when(mClock.getUptimeSinceBootMillis()).thenReturn(20L);
        staIface2 = validateInterfaceSequence(chipMock,
                true, // chipModeValid
                TestChipV2.CHIP_MODE_ID, // chipModeId
                IfaceType.STA, // ifaceTypeToCreate
                "wlan1", // ifaceName
                TestChipV2.CHIP_MODE_ID, // finalChipMode
                false, // high priority
                null, // tearDownList
                staDestroyedListener2, // destroyedListener
                null // availableListener (already registered)
        );
        collector.checkThat("STA 2 interface wasn't created", staIface2, IsNull.notNullValue());

        inOrderAvail.verify(staAvailListener).onAvailabilityChanged(false);

        // request STA3: should fail
        IWifiIface staIface3 = mDut.createStaIface(false, null, null);
        collector.checkThat("STA3 should not be created", staIface3, IsNull.nullValue());

        // create AP - this will destroy the last STA created, i.e. STA2
        apIface = validateInterfaceSequence(chipMock,
                true, // chipModeValid
                TestChipV2.CHIP_MODE_ID, // chipModeId
                IfaceType.AP, // ifaceTypeToCreate
                "wlan1", // ifaceName
                TestChipV2.CHIP_MODE_ID, // finalChipMode
                false, // high priority (but irrelevant)
                null, // tearDownList
                apDestroyedListener, // destroyedListener
                null, // availableListener (already registered),
                // destroyedInterfacesDestroyedListeners...
                new InterfaceDestroyedListenerWithIfaceName(
                        getName(staIface2), staDestroyedListener2)
        );
        collector.checkThat("AP interface wasn't created", apIface, IsNull.notNullValue());

        inOrderAvail.verify(apAvailListener).onAvailabilityChanged(false);

        // tear down P2P
        mDut.removeIface(p2pIface);
        mTestLooper.dispatchAll();

        inOrderAvail.verify(p2pAvailListener).onAvailabilityChanged(true);
        inOrderAvail.verify(nanAvailListener).onAvailabilityChanged(true);
        verify(chipMock.chip).removeP2pIface("p2p0");
        verify(p2pDestroyedListener).onDestroyed(getName(p2pIface));

        // create NAN
        nanIface = validateInterfaceSequence(chipMock,
                true, // chipModeValid
                TestChipV2.CHIP_MODE_ID, // chipModeId
                IfaceType.NAN, // ifaceTypeToCreate
                "wlan0", // ifaceName
                TestChipV2.CHIP_MODE_ID, // finalChipMode
                false, // high priority (but irrelevant)
                null, // tearDownList
                nanDestroyedListener, // destroyedListener
                null // availableListener (already registered)
        );
        collector.checkThat("NAN interface wasn't created", nanIface, IsNull.notNullValue());

        inOrderAvail.verify(nanAvailListener).onAvailabilityChanged(false);

        verifyNoMoreInteractions(mManagerStatusListenerMock, staDestroyedListener,
                staDestroyedListener2, apDestroyedListener, p2pDestroyedListener,
                nanDestroyedListener, staAvailListener, apAvailListener, p2pAvailListener,
                nanAvailListener, staAvailListener, apAvailListener, p2pAvailListener,
                nanAvailListener);
    }

    /**
     * Validate P2P and NAN interactions. Expect:
     * - STA created
     * - NAN created
     * - When P2P requested:
     *   - NAN torn down
     *   - P2P created
     * - NAN creation refused
     * - When P2P destroyed:
     *   - get nan available listener
     *   - Can create NAN when requested
     */
    @Test
    public void testP2pAndNanInteractionsTestChipV2() throws Exception {
        runP2pAndNanExclusiveInteractionsTestChip(new TestChipV2(), TestChipV2.CHIP_MODE_ID);
    }

    /**
     * Validate that the getSupportedIfaceTypes API works when requesting for all chips.
     */
    @Test
    public void testGetSupportedIfaceTypesAllTestChipV2() throws Exception {
        TestChipV2 chipMock = new TestChipV2();
        chipMock.initialize();
        mInOrder = inOrder(mServiceManagerMock, mWifiMock, chipMock.chip,
                mManagerStatusListenerMock);
        executeAndValidateInitializationSequence();
        executeAndValidateStartupSequence();

        // try API
        Set<Integer> results = mDut.getSupportedIfaceTypes();

        // verify results
        Set<Integer> correctResults = new HashSet<>();
        correctResults.add(IfaceType.AP);
        correctResults.add(IfaceType.STA);
        correctResults.add(IfaceType.P2P);
        correctResults.add(IfaceType.NAN);

        assertEquals(correctResults, results);
    }

    /**
     * Validate that the getSupportedIfaceTypes API works when requesting for a specific chip.
     */
    @Test
    public void testGetSupportedIfaceTypesOneChipTestChipV2() throws Exception {
        TestChipV2 chipMock = new TestChipV2();
        chipMock.initialize();
        mInOrder = inOrder(mServiceManagerMock, mWifiMock, chipMock.chip,
                mManagerStatusListenerMock);
        executeAndValidateInitializationSequence();
        executeAndValidateStartupSequence();

        // try API
        Set<Integer> results = mDut.getSupportedIfaceTypes(chipMock.chip);

        // verify results
        Set<Integer> correctResults = new HashSet<>();
        correctResults.add(IfaceType.AP);
        correctResults.add(IfaceType.STA);
        correctResults.add(IfaceType.P2P);
        correctResults.add(IfaceType.NAN);

        assertEquals(correctResults, results);
    }

    //////////////////////////////////////////////////////////////////////////////////////
    // TestChipV3 Specific Tests
    //////////////////////////////////////////////////////////////////////////////////////

    /**
     * Validate a flow sequence for test chip 3:
     * - create STA
     * - create P2P
     * - request NAN: failure
     * - create AP: should tear down P2P first
     * - create STA: will get refused
     * - create AP: will get refused
     * - request P2P: failure
     * - tear down AP
     * - create STA
     * - create STA: will get refused
     * - create NAN: should tear down last created STA
     * - create STA: will get refused
     */
    @Test
    public void testInterfaceCreationFlowTestChipV3() throws Exception {
        TestChipV3 chipMock = new TestChipV3();
        chipMock.initialize();
        mInOrder = inOrder(mServiceManagerMock, mWifiMock, chipMock.chip,
                mManagerStatusListenerMock);
        executeAndValidateInitializationSequence();
        executeAndValidateStartupSequence();

        InterfaceDestroyedListener staDestroyedListener = mock(
                InterfaceDestroyedListener.class);
        InterfaceDestroyedListener staDestroyedListener2 = mock(
                InterfaceDestroyedListener.class);
        HalDeviceManager.InterfaceAvailableForRequestListener staAvailListener = mock(
                HalDeviceManager.InterfaceAvailableForRequestListener.class);

        InterfaceDestroyedListener apDestroyedListener = mock(
                InterfaceDestroyedListener.class);
        HalDeviceManager.InterfaceAvailableForRequestListener apAvailListener = mock(
                HalDeviceManager.InterfaceAvailableForRequestListener.class);

        InterfaceDestroyedListener p2pDestroyedListener = mock(
                InterfaceDestroyedListener.class);
        HalDeviceManager.InterfaceAvailableForRequestListener p2pAvailListener = mock(
                HalDeviceManager.InterfaceAvailableForRequestListener.class);

        InterfaceDestroyedListener nanDestroyedListener = mock(
                InterfaceDestroyedListener.class);
        HalDeviceManager.InterfaceAvailableForRequestListener nanAvailListener = mock(
                HalDeviceManager.InterfaceAvailableForRequestListener.class);

        InOrder inOrderAvail = inOrder(staAvailListener, apAvailListener, p2pAvailListener,
                nanAvailListener);

        // register listeners for interface availability
        mDut.registerInterfaceAvailableForRequestListener(IfaceType.STA, staAvailListener,
                mHandler);
        mDut.registerInterfaceAvailableForRequestListener(IfaceType.AP, apAvailListener, mHandler);
        mDut.registerInterfaceAvailableForRequestListener(IfaceType.P2P, p2pAvailListener,
                mHandler);
        mDut.registerInterfaceAvailableForRequestListener(IfaceType.NAN, nanAvailListener,
                mHandler);
        mTestLooper.dispatchAll();

        inOrderAvail.verify(staAvailListener).onAvailabilityChanged(true);
        inOrderAvail.verify(apAvailListener).onAvailabilityChanged(true);
        inOrderAvail.verify(p2pAvailListener).onAvailabilityChanged(true);
        inOrderAvail.verify(nanAvailListener).onAvailabilityChanged(true);

        // create STA
        when(mClock.getUptimeSinceBootMillis()).thenReturn(15L);
        IWifiIface staIface = validateInterfaceSequence(chipMock,
                false, // chipModeValid
                -1000, // chipModeId (only used if chipModeValid is true)
                IfaceType.STA, // ifaceTypeToCreate
                "wlan0", // ifaceName
                TestChipV3.CHIP_MODE_ID, // finalChipMode
                false, // high priority
                null, // tearDownList
                staDestroyedListener, // destroyedListener
                null // availableListener (already registered)
        );
        collector.checkThat("STA interface wasn't created", staIface, IsNull.notNullValue());

        // create P2P
        IWifiIface p2pIface = validateInterfaceSequence(chipMock,
                true, // chipModeValid
                TestChipV3.CHIP_MODE_ID, // chipModeId
                IfaceType.P2P, // ifaceTypeToCreate
                "p2p0", // ifaceName
                TestChipV3.CHIP_MODE_ID, // finalChipMode
                false, // high priority (but irrelevant)
                null, // tearDownList
                p2pDestroyedListener, // destroyedListener
                null // availableListener (already registered)
        );
        collector.checkThat("P2P interface wasn't created", p2pIface, IsNull.notNullValue());

        inOrderAvail.verify(staAvailListener).onAvailabilityChanged(false);
        inOrderAvail.verify(p2pAvailListener).onAvailabilityChanged(false);
        inOrderAvail.verify(nanAvailListener).onAvailabilityChanged(false);

        // request NAN: should fail
        IWifiIface nanIface = mDut.createNanIface(null, null);
        collector.checkThat("NAN should not be created", nanIface, IsNull.nullValue());

        // create AP: will destroy P2P
        IWifiIface apIface = validateInterfaceSequence(chipMock,
                true, // chipModeValid
                TestChipV3.CHIP_MODE_ID, // chipModeId
                IfaceType.AP, // ifaceTypeToCreate
                "wlan1", // ifaceName
                TestChipV3.CHIP_MODE_ID, // finalChipMode
                false, // high priority (but irrelevant)
                null, // tearDownList
                apDestroyedListener, // destroyedListener
                null, // availableListener (already registered)
                new InterfaceDestroyedListenerWithIfaceName("p2p0", p2pDestroyedListener)
        );
        collector.checkThat("AP interface wasn't created", apIface, IsNull.notNullValue());
        verify(chipMock.chip).removeP2pIface("p2p0");

        inOrderAvail.verify(apAvailListener).onAvailabilityChanged(false);

        // request STA2: should fail
        IWifiIface staIface2 = mDut.createStaIface(false, null, null);
        collector.checkThat("STA2 should not be created", staIface2, IsNull.nullValue());

        // request AP2: should fail
        IWifiIface apIface2 = mDut.createApIface(null, null);
        collector.checkThat("AP2 should not be created", apIface2, IsNull.nullValue());

        // request P2P: should fail
        p2pIface = mDut.createP2pIface(null, null);
        collector.checkThat("P2P should not be created", p2pIface, IsNull.nullValue());

        // tear down AP
        mDut.removeIface(apIface);
        mTestLooper.dispatchAll();

        inOrderAvail.verify(apAvailListener).onAvailabilityChanged(true);
        inOrderAvail.verify(staAvailListener).onAvailabilityChanged(true);
        inOrderAvail.verify(p2pAvailListener).onAvailabilityChanged(true);
        inOrderAvail.verify(nanAvailListener).onAvailabilityChanged(true);
        verify(chipMock.chip).removeApIface("wlan1");
        verify(apDestroyedListener).onDestroyed(getName(apIface));

        // create STA2: using a later clock
        when(mClock.getUptimeSinceBootMillis()).thenReturn(20L);
        staIface2 = validateInterfaceSequence(chipMock,
                true, // chipModeValid
                TestChipV3.CHIP_MODE_ID, // chipModeId
                IfaceType.STA, // ifaceTypeToCreate
                "wlan1", // ifaceName
                TestChipV3.CHIP_MODE_ID, // finalChipMode
                false, // high priority
                null, // tearDownList
                staDestroyedListener2, // destroyedListener
                null // availableListener (already registered)
        );
        collector.checkThat("STA 2 interface wasn't created", staIface2, IsNull.notNullValue());

        inOrderAvail.verify(staAvailListener).onAvailabilityChanged(false);

        // request STA3: should fail
        IWifiIface staIface3 = mDut.createStaIface(false, null, null);
        collector.checkThat("STA3 should not be created", staIface3, IsNull.nullValue());

        // create NAN: should destroy the last created STA (STA2)
        nanIface = validateInterfaceSequence(chipMock,
                true, // chipModeValid
                TestChipV3.CHIP_MODE_ID, // chipModeId
                IfaceType.NAN, // ifaceTypeToCreate
                "wlan0", // ifaceName
                TestChipV3.CHIP_MODE_ID, // finalChipMode
                false, // high priority (but irrelevant)
                null, // tearDownList
                nanDestroyedListener, // destroyedListener
                null, // availableListener (already registered)
                new InterfaceDestroyedListenerWithIfaceName(
                        getName(staIface2), staDestroyedListener2)
        );
        collector.checkThat("NAN interface wasn't created", nanIface, IsNull.notNullValue());

        inOrderAvail.verify(nanAvailListener).onAvailabilityChanged(false);
        verify(chipMock.chip).removeStaIface("wlan1");
        verify(staDestroyedListener2).onDestroyed(getName(staIface2));

        // request STA2: should fail
        staIface2 = mDut.createStaIface(false, null, null);
        collector.checkThat("STA2 should not be created", staIface2, IsNull.nullValue());

        verifyNoMoreInteractions(mManagerStatusListenerMock, staDestroyedListener,
                staDestroyedListener2, apDestroyedListener, p2pDestroyedListener,
                nanDestroyedListener, staAvailListener, apAvailListener, p2pAvailListener,
                nanAvailListener, staAvailListener, apAvailListener, p2pAvailListener,
                nanAvailListener);
    }

    /**
     * Validate P2P and NAN interactions. Expect:
     * - STA created
     * - NAN created
     * - When P2P requested:
     *   - NAN torn down
     *   - P2P created
     * - NAN creation refused
     * - When P2P destroyed:
     *   - get nan available listener
     *   - Can create NAN when requested
     */
    @Test
    public void testP2pAndNanInteractionsTestChipV3() throws Exception {
        runP2pAndNanExclusiveInteractionsTestChip(new TestChipV3(), TestChipV3.CHIP_MODE_ID);
    }

    /**
     * Validate that the getSupportedIfaceTypes API works when requesting for all chips.
     */
    @Test
    public void testGetSupportedIfaceTypesAllTestChipV3() throws Exception {
        TestChipV3 chipMock = new TestChipV3();
        chipMock.initialize();
        mInOrder = inOrder(mServiceManagerMock, mWifiMock, chipMock.chip,
                mManagerStatusListenerMock);
        executeAndValidateInitializationSequence();
        executeAndValidateStartupSequence();

        // try API
        Set<Integer> results = mDut.getSupportedIfaceTypes();

        // verify results
        Set<Integer> correctResults = new HashSet<>();
        correctResults.add(IfaceType.AP);
        correctResults.add(IfaceType.STA);
        correctResults.add(IfaceType.P2P);
        correctResults.add(IfaceType.NAN);

        assertEquals(correctResults, results);
    }

    /**
     * Validate that the getSupportedIfaceTypes API works when requesting for a specific chip.
     */
    @Test
    public void testGetSupportedIfaceTypesOneChipTestChipV3() throws Exception {
        TestChipV3 chipMock = new TestChipV3();
        chipMock.initialize();
        mInOrder = inOrder(mServiceManagerMock, mWifiMock, chipMock.chip,
                mManagerStatusListenerMock);
        executeAndValidateInitializationSequence();
        executeAndValidateStartupSequence();

        // try API
        Set<Integer> results = mDut.getSupportedIfaceTypes(chipMock.chip);

        // verify results
        Set<Integer> correctResults = new HashSet<>();
        correctResults.add(IfaceType.AP);
        correctResults.add(IfaceType.STA);
        correctResults.add(IfaceType.P2P);
        correctResults.add(IfaceType.NAN);

        assertEquals(correctResults, results);
    }

    //////////////////////////////////////////////////////////////////////////////////////
    // TestChipV4 Specific Tests
    //////////////////////////////////////////////////////////////////////////////////////

    /**
     * Validate a flow sequence for test chip 4:
     * - create STA
     * - create P2P
     * - request NAN: failure
     * - create AP: should tear down P2P first
     * - create STA: will get refused
     * - create AP: will get refused
     * - request P2P: failure
     * - tear down AP
     * - create STA: will get refused
     * - create NAN
     * - create STA: will get refused
     */
    @Test
    public void testInterfaceCreationFlowTestChipV4() throws Exception {
        TestChipV4 chipMock = new TestChipV4();
        chipMock.initialize();
        mInOrder = inOrder(mServiceManagerMock, mWifiMock, chipMock.chip,
                mManagerStatusListenerMock);
        executeAndValidateInitializationSequence();
        executeAndValidateStartupSequence();

        InterfaceDestroyedListener staDestroyedListener = mock(
                InterfaceDestroyedListener.class);
        InterfaceDestroyedListener staDestroyedListener2 = mock(
                InterfaceDestroyedListener.class);
        HalDeviceManager.InterfaceAvailableForRequestListener staAvailListener = mock(
                HalDeviceManager.InterfaceAvailableForRequestListener.class);

        InterfaceDestroyedListener apDestroyedListener = mock(
                InterfaceDestroyedListener.class);
        HalDeviceManager.InterfaceAvailableForRequestListener apAvailListener = mock(
                HalDeviceManager.InterfaceAvailableForRequestListener.class);

        InterfaceDestroyedListener p2pDestroyedListener = mock(
                InterfaceDestroyedListener.class);
        HalDeviceManager.InterfaceAvailableForRequestListener p2pAvailListener = mock(
                HalDeviceManager.InterfaceAvailableForRequestListener.class);

        InterfaceDestroyedListener nanDestroyedListener = mock(
                InterfaceDestroyedListener.class);
        HalDeviceManager.InterfaceAvailableForRequestListener nanAvailListener = mock(
                HalDeviceManager.InterfaceAvailableForRequestListener.class);

        InOrder inOrderAvail = inOrder(staAvailListener, apAvailListener, p2pAvailListener,
                nanAvailListener);

        // register listeners for interface availability
        mDut.registerInterfaceAvailableForRequestListener(IfaceType.STA, staAvailListener,
                mHandler);
        mDut.registerInterfaceAvailableForRequestListener(IfaceType.AP, apAvailListener, mHandler);
        mDut.registerInterfaceAvailableForRequestListener(IfaceType.P2P, p2pAvailListener,
                mHandler);
        mDut.registerInterfaceAvailableForRequestListener(IfaceType.NAN, nanAvailListener,
                mHandler);
        mTestLooper.dispatchAll();

        inOrderAvail.verify(staAvailListener).onAvailabilityChanged(true);
        inOrderAvail.verify(apAvailListener).onAvailabilityChanged(true);
        inOrderAvail.verify(p2pAvailListener).onAvailabilityChanged(true);
        inOrderAvail.verify(nanAvailListener).onAvailabilityChanged(true);

        // create STA
        when(mClock.getUptimeSinceBootMillis()).thenReturn(15L);
        IWifiIface staIface = validateInterfaceSequence(chipMock,
                false, // chipModeValid
                -1000, // chipModeId (only used if chipModeValid is true)
                IfaceType.STA, // ifaceTypeToCreate
                "wlan0", // ifaceName
                TestChipV4.CHIP_MODE_ID, // finalChipMode
                false, // high priority
                null, // tearDownList
                staDestroyedListener, // destroyedListener
                null // availableListener (already registered)
        );
        collector.checkThat("STA interface wasn't created", staIface, IsNull.notNullValue());
        inOrderAvail.verify(staAvailListener).onAvailabilityChanged(false);

        // create P2P
        IWifiIface p2pIface = validateInterfaceSequence(chipMock,
                true, // chipModeValid
                TestChipV4.CHIP_MODE_ID, // chipModeId
                IfaceType.P2P, // ifaceTypeToCreate
                "p2p0", // ifaceName
                TestChipV4.CHIP_MODE_ID, // finalChipMode
                false, // high priority (but irrelevant)
                null, // tearDownList
                p2pDestroyedListener, // destroyedListener
                null // availableListener (already registered)
        );
        collector.checkThat("P2P interface wasn't created", p2pIface, IsNull.notNullValue());

        inOrderAvail.verify(p2pAvailListener).onAvailabilityChanged(false);
        inOrderAvail.verify(nanAvailListener).onAvailabilityChanged(false);

        // request NAN: should fail
        IWifiIface nanIface = mDut.createNanIface(null, null);
        collector.checkThat("NAN should not be created", nanIface, IsNull.nullValue());

        // create AP: will destroy P2P
        IWifiIface apIface = validateInterfaceSequence(chipMock,
                true, // chipModeValid
                TestChipV4.CHIP_MODE_ID, // chipModeId
                IfaceType.AP, // ifaceTypeToCreate
                "wlan1", // ifaceName
                TestChipV4.CHIP_MODE_ID, // finalChipMode
                false, // high priority (but irrelevant)
                null, // tearDownList
                apDestroyedListener, // destroyedListener
                null, // availableListener (already registered)
                new InterfaceDestroyedListenerWithIfaceName("p2p0", p2pDestroyedListener)
        );
        collector.checkThat("AP interface wasn't created", apIface, IsNull.notNullValue());
        verify(chipMock.chip).removeP2pIface("p2p0");

        inOrderAvail.verify(apAvailListener).onAvailabilityChanged(false);

        // request STA2: should fail
        IWifiIface staIface2 = mDut.createStaIface(false, null, null);
        collector.checkThat("STA2 should not be created", staIface2, IsNull.nullValue());

        // request AP2: should fail
        IWifiIface apIface2 = mDut.createApIface(null, null);
        collector.checkThat("AP2 should not be created", apIface2, IsNull.nullValue());

        // request P2P: should fail
        p2pIface = mDut.createP2pIface(null, null);
        collector.checkThat("P2P should not be created", p2pIface, IsNull.nullValue());

        // tear down AP
        mDut.removeIface(apIface);
        mTestLooper.dispatchAll();

        inOrderAvail.verify(apAvailListener).onAvailabilityChanged(true);
        inOrderAvail.verify(p2pAvailListener).onAvailabilityChanged(true);
        inOrderAvail.verify(nanAvailListener).onAvailabilityChanged(true);
        verify(chipMock.chip).removeApIface("wlan1");
        verify(apDestroyedListener).onDestroyed(getName(apIface));

        // request STA2: should fail
        staIface2 = mDut.createStaIface(false, null, null);
        collector.checkThat("STA2 should not be created", staIface2, IsNull.nullValue());

        // create NAN
        nanIface = validateInterfaceSequence(chipMock,
                true, // chipModeValid
                TestChipV4.CHIP_MODE_ID, // chipModeId
                IfaceType.NAN, // ifaceTypeToCreate
                "wlan0", // ifaceName
                TestChipV4.CHIP_MODE_ID, // finalChipMode
                false, // high priority (but irrelevant)
                null, // tearDownList
                nanDestroyedListener, // destroyedListener
                null // availableListener (already registered)
        );
        collector.checkThat("NAN interface wasn't created", nanIface, IsNull.notNullValue());

        inOrderAvail.verify(nanAvailListener).onAvailabilityChanged(false);

        // request STA2: should fail
        staIface2 = mDut.createStaIface(false, null, null);
        collector.checkThat("STA2 should not be created", staIface2, IsNull.nullValue());

        // tear down STA
        mDut.removeIface(staIface);
        mTestLooper.dispatchAll();

        inOrderAvail.verify(staAvailListener).onAvailabilityChanged(true);
        verify(chipMock.chip).removeStaIface("wlan0");
        verify(staDestroyedListener).onDestroyed(getName(staIface));

        verifyNoMoreInteractions(mManagerStatusListenerMock, staDestroyedListener,
                staDestroyedListener2, apDestroyedListener, p2pDestroyedListener,
                nanDestroyedListener, staAvailListener, apAvailListener, p2pAvailListener,
                nanAvailListener, staAvailListener, apAvailListener, p2pAvailListener,
                nanAvailListener);
    }

    /**
     * Validate P2P and NAN interactions. Expect:
     * - STA created
     * - NAN created
     * - When P2P requested:
     *   - NAN torn down
     *   - P2P created
     * - NAN creation refused
     * - When P2P destroyed:
     *   - get nan available listener
     *   - Can create NAN when requested
     */
    @Test
    public void testP2pAndNanInteractionsTestChipV4() throws Exception {
        runP2pAndNanExclusiveInteractionsTestChip(new TestChipV4(), TestChipV4.CHIP_MODE_ID);
    }

    /**
     * Validate that the getSupportedIfaceTypes API works when requesting for all chips.
     */
    @Test
    public void testGetSupportedIfaceTypesAllTestChipV4() throws Exception {
        TestChipV4 chipMock = new TestChipV4();
        chipMock.initialize();
        mInOrder = inOrder(mServiceManagerMock, mWifiMock, chipMock.chip,
                mManagerStatusListenerMock);
        executeAndValidateInitializationSequence();
        executeAndValidateStartupSequence();

        // try API
        Set<Integer> results = mDut.getSupportedIfaceTypes();

        // verify results
        Set<Integer> correctResults = new HashSet<>();
        correctResults.add(IfaceType.AP);
        correctResults.add(IfaceType.STA);
        correctResults.add(IfaceType.P2P);
        correctResults.add(IfaceType.NAN);

        assertEquals(correctResults, results);
    }

    /**
     * Validate that the getSupportedIfaceTypes API works when requesting for a specific chip.
     */
    @Test
    public void testGetSupportedIfaceTypesOneChipTestChipV4() throws Exception {
        TestChipV4 chipMock = new TestChipV4();
        chipMock.initialize();
        mInOrder = inOrder(mServiceManagerMock, mWifiMock, chipMock.chip,
                mManagerStatusListenerMock);
        executeAndValidateInitializationSequence();
        executeAndValidateStartupSequence();

        // try API
        Set<Integer> results = mDut.getSupportedIfaceTypes(chipMock.chip);

        // verify results
        Set<Integer> correctResults = new HashSet<>();
        correctResults.add(IfaceType.AP);
        correctResults.add(IfaceType.STA);
        correctResults.add(IfaceType.P2P);
        correctResults.add(IfaceType.NAN);

        assertEquals(correctResults, results);
    }

    ///////////////////////////////////////////////////////////////////////////////////////
    // Tests targeting low priority STA creation
    ///////////////////////////////////////////////////////////////////////////////////////

    /**
     * Validate low priority STA management on Test Chip V1 (which has single STA capability).
     * Procedure:
     * - Create STA
     * - Create STA(low priority): expect failure
     * - Create AP: expect STA to be deleted
     * - Create STA(low priority): expect failure
     * - Delete AP
     * - Create STA(low priority): success!
     * - Create STA: expect STA(low priority) to be deleted
     */
    @Test
    public void testLowPriorityStaTestChipV1() throws Exception {
        TestChipV1 chipMock = new TestChipV1();
        chipMock.initialize();
        mInOrder = inOrder(mServiceManagerMock, mWifiMock, chipMock.chip,
                mManagerStatusListenerMock);
        executeAndValidateInitializationSequence();
        executeAndValidateStartupSequence();

        InterfaceDestroyedListener staDestroyedListener = mock(
                InterfaceDestroyedListener.class);
        InterfaceDestroyedListener staLpDestroyedListener = mock(
                InterfaceDestroyedListener.class);
        InterfaceDestroyedListener apDestroyedListener = mock(
                InterfaceDestroyedListener.class);

        // create STA
        IWifiIface staIface = validateInterfaceSequence(chipMock,
                false, // chipModeValid
                -1000, // chipModeId (only used if chipModeValid is true)
                IfaceType.STA, // ifaceTypeToCreate
                "wlan0", // ifaceName
                TestChipV1.STA_CHIP_MODE_ID, // finalChipMode
                false, // high priority
                null, // tearDownList
                staDestroyedListener, // destroyedListener
                null // availableListener (already registered)
        );
        collector.checkThat("STA interface wasn't created", staIface, IsNull.notNullValue());

        // request STA(low priority): should fail
        IWifiIface staLowIface = mDut.createStaIface(true, null, null);
        collector.checkThat("STA(low priority) should not be created", staLowIface,
                IsNull.nullValue());

        // create AP: will destroy STA
        IWifiIface apIface = validateInterfaceSequence(chipMock,
                true, // chipModeValid
                TestChipV1.STA_CHIP_MODE_ID, // chipModeId
                IfaceType.AP, // ifaceTypeToCreate
                "ap0", // ifaceName
                TestChipV1.AP_CHIP_MODE_ID, // finalChipMode
                false, // high priority (but irrelevant)
                null, // tearDownList
                apDestroyedListener, // destroyedListener
                null, // availableListener (already registered)
                new InterfaceDestroyedListenerWithIfaceName("wlan0", staDestroyedListener)
        );
        collector.checkThat("AP interface wasn't created", apIface, IsNull.notNullValue());

        // request STA(low priority): should fail
        staLowIface = mDut.createStaIface(true, null, null);
        collector.checkThat("STA(low priority) should not be created", staLowIface,
                IsNull.nullValue());

        // tear down AP
        mDut.removeIface(apIface);
        mTestLooper.dispatchAll();
        verify(chipMock.chip).removeApIface("ap0");
        verify(apDestroyedListener).onDestroyed(getName(apIface));

        // create STA(low priority)
        staLowIface = validateInterfaceSequence(chipMock,
                true, // chipModeValid
                TestChipV1.AP_CHIP_MODE_ID, // chipModeId (only used if chipModeValid is true)
                IfaceType.STA, // ifaceTypeToCreate
                "wlan1", // ifaceName
                TestChipV1.STA_CHIP_MODE_ID, // finalChipMode
                true, // low priority
                null, // tearDownList
                staLpDestroyedListener, // destroyedListener
                null // availableListener (already registered)
        );
        collector.checkThat("STA(low priority) interface wasn't created", staIface,
                IsNull.notNullValue());

        // create STA: should destroy the low priority STA
        staIface = validateInterfaceSequence(chipMock,
                true, // chipModeValid
                TestChipV1.STA_CHIP_MODE_ID, // chipModeId (only used if chipModeValid is true)
                IfaceType.STA, // ifaceTypeToCreate
                "wlan0", // ifaceName
                TestChipV1.STA_CHIP_MODE_ID, // finalChipMode
                false, // high priority
                null, // tearDownList
                staDestroyedListener, // destroyedListener
                null, // availableListener (already registered)
                new InterfaceDestroyedListenerWithIfaceName("wlan1", staLpDestroyedListener)
        );
        collector.checkThat("STA interface wasn't created", staIface,
                IsNull.notNullValue());

        verifyNoMoreInteractions(mManagerStatusListenerMock, staDestroyedListener,
                apDestroyedListener, staLpDestroyedListener);
    }

    /**
     * Validate low priority STA management on Test Chip V4 (which has single STA+AP capability).
     * Procedure:
     * - Create STA
     * - Create STA(low priority): expect failure
     * - Create AP
     * - Create STA: expect failure
     * - Destroy STA
     * - Create STA(low priority): success!
     * - Create STA: expect STA(low priority) to be deleted
     */
    @Test
    public void testLowPriorityStaTestChipV4() throws Exception {
        TestChipV4 chipMock = new TestChipV4();
        chipMock.initialize();
        mInOrder = inOrder(mServiceManagerMock, mWifiMock, chipMock.chip,
                mManagerStatusListenerMock);
        executeAndValidateInitializationSequence();
        executeAndValidateStartupSequence();

        InterfaceDestroyedListener staDestroyedListener = mock(
                InterfaceDestroyedListener.class);
        InterfaceDestroyedListener staLpDestroyedListener = mock(
                InterfaceDestroyedListener.class);
        InterfaceDestroyedListener apDestroyedListener = mock(
                InterfaceDestroyedListener.class);

        // create STA
        IWifiIface staIface = validateInterfaceSequence(chipMock,
                false, // chipModeValid
                -1000, // chipModeId (only used if chipModeValid is true)
                IfaceType.STA, // ifaceTypeToCreate
                "wlan0", // ifaceName
                TestChipV4.CHIP_MODE_ID, // finalChipMode
                false, // high priority
                null, // tearDownList
                staDestroyedListener, // destroyedListener
                null // availableListener (already registered)
        );
        collector.checkThat("STA interface wasn't created", staIface, IsNull.notNullValue());

        // request STA(low priority): should fail
        IWifiIface staLowIface = mDut.createStaIface(true, null, null);
        collector.checkThat("STA(low priority) should not be created", staLowIface,
                IsNull.nullValue());

        // create AP
        IWifiIface apIface = validateInterfaceSequence(chipMock,
                true, // chipModeValid
                TestChipV4.CHIP_MODE_ID, // chipModeId
                IfaceType.AP, // ifaceTypeToCreate
                "ap0", // ifaceName
                TestChipV4.CHIP_MODE_ID, // finalChipMode
                false, // high priority (but irrelevant)
                null, // tearDownList
                apDestroyedListener, // destroyedListener
                null // availableListener (already registered)
        );
        collector.checkThat("AP interface wasn't created", apIface, IsNull.notNullValue());

        // request STA2: should fail
        IWifiIface sta2Iface = mDut.createStaIface(true, null, null);
        collector.checkThat("STA2 should not be created", sta2Iface,
                IsNull.nullValue());

        // tear down STA
        mDut.removeIface(staIface);
        mTestLooper.dispatchAll();
        verify(chipMock.chip).removeStaIface("wlan0");
        verify(staDestroyedListener).onDestroyed(getName(staIface));

        // create STA(low priority)
        staLowIface = validateInterfaceSequence(chipMock,
                true, // chipModeValid
                TestChipV4.CHIP_MODE_ID, // chipModeId (only used if chipModeValid is true)
                IfaceType.STA, // ifaceTypeToCreate
                "wlan1", // ifaceName
                TestChipV4.CHIP_MODE_ID, // finalChipMode
                true, // low priority
                null, // tearDownList
                staLpDestroyedListener, // destroyedListener
                null // availableListener (already registered)
        );
        collector.checkThat("STA(low priority) interface wasn't created", staIface,
                IsNull.notNullValue());

        // create STA: should destroy the low priority STA
        staIface = validateInterfaceSequence(chipMock,
                true, // chipModeValid
                TestChipV4.CHIP_MODE_ID, // chipModeId (only used if chipModeValid is true)
                IfaceType.STA, // ifaceTypeToCreate
                "wlan0", // ifaceName
                TestChipV4.CHIP_MODE_ID, // finalChipMode
                false, // high priority
                null, // tearDownList
                staDestroyedListener, // destroyedListener
                null, // availableListener (already registered)
                new InterfaceDestroyedListenerWithIfaceName("wlan1", staLpDestroyedListener)
        );
        collector.checkThat("STA interface wasn't created", staIface,
                IsNull.notNullValue());

        verifyNoMoreInteractions(mManagerStatusListenerMock, staDestroyedListener,
                apDestroyedListener, staLpDestroyedListener);
    }

    ///////////////////////////////////////////////////////////////////////////////////////
    // utilities
    ///////////////////////////////////////////////////////////////////////////////////////

    private void dumpDut(String prefix) {
        StringWriter sw = new StringWriter();
        mDut.dump(null, new PrintWriter(sw), null);
        Log.e("HalDeviceManager", prefix + sw.toString());
    }

    private void executeAndValidateInitializationSequence() throws Exception {
        executeAndValidateInitializationSequence(true);
    }

    private void executeAndValidateInitializationSequence(boolean isSupported) throws Exception {
        // act:
        mDut.initialize();

        // verify: service manager initialization sequence
        mInOrder.verify(mServiceManagerMock).linkToDeath(any(IHwBinder.DeathRecipient.class),
                anyLong());
        mInOrder.verify(mServiceManagerMock).registerForNotifications(eq(IWifi.kInterfaceName),
                eq(""), mServiceNotificationCaptor.capture());

        // If not using the lazy version of the IWifi service, the process should already be up at
        // this point.
        mInOrder.verify(mServiceManagerMock).listManifestByInterface(eq(IWifi.kInterfaceName));

        // verify: wifi initialization sequence if vendor HAL is supported.
        if (isSupported) {
            mInOrder.verify(mWifiMock).linkToDeath(mDeathRecipientCaptor.capture(), anyLong());
            mInOrder.verify(mWifiMock).registerEventCallback(mWifiEventCallbackCaptor.capture());
            // verify: onStop called as a part of initialize.
            mInOrder.verify(mWifiMock).stop();
            collector.checkThat("isReady is true", mDut.isReady(), equalTo(true));
        } else {
            collector.checkThat("isReady is false", mDut.isReady(), equalTo(false));
        }
    }

    private void executeAndValidateStartupSequence() throws Exception {
        executeAndValidateStartupSequence(1, true);
    }

    private void executeAndValidateStartupSequence(int numAttempts, boolean success)
            throws Exception {
        // act: register listener & start Wi-Fi
        mDut.registerStatusListener(mManagerStatusListenerMock, mHandler);
        collector.checkThat(mDut.start(), equalTo(success));

        // verify
        mInOrder.verify(mWifiMock, times(numAttempts)).start();

        if (success) {
            // act: trigger onStart callback of IWifiEventCallback
            mWifiEventCallbackCaptor.getValue().onStart();
            mTestLooper.dispatchAll();

            // verify: onStart called on registered listener
            mInOrder.verify(mManagerStatusListenerMock).onStatusChanged();
        }
    }

    private void runCreateSingleXxxInterfaceNoInitMode(ChipMockBase chipMock, int ifaceTypeToCreate,
            String ifaceName, int finalChipMode, boolean multipleIfaceSupport) throws Exception {
        chipMock.initialize();
        mInOrder = inOrder(mServiceManagerMock, mWifiMock, chipMock.chip,
                mManagerStatusListenerMock);
        executeAndValidateInitializationSequence();
        executeAndValidateStartupSequence();

        InterfaceDestroyedListener idl = mock(
                InterfaceDestroyedListener.class);
        HalDeviceManager.InterfaceAvailableForRequestListener iafrl = mock(
                HalDeviceManager.InterfaceAvailableForRequestListener.class);

        InOrder availInOrder = inOrder(iafrl);

        IWifiIface iface = validateInterfaceSequence(chipMock,
                false, // chipModeValid
                -1000, // chipModeId (only used if chipModeValid is true)
                ifaceTypeToCreate,
                ifaceName,
                finalChipMode,
                false, // high priority
                null, // tearDownList
                idl, // destroyedListener
                iafrl // availableListener
        );
        collector.checkThat("allocated interface", iface, IsNull.notNullValue());
        availInOrder.verify(iafrl).onAvailabilityChanged(multipleIfaceSupport);

        // act: remove interface
        mDut.removeIface(iface);
        mTestLooper.dispatchAll();

        // verify: callback triggered
        switch (ifaceTypeToCreate) {
            case IfaceType.STA:
                mInOrder.verify(chipMock.chip).removeStaIface(ifaceName);
                break;
            case IfaceType.AP:
                mInOrder.verify(chipMock.chip).removeApIface(ifaceName);
                break;
            case IfaceType.P2P:
                mInOrder.verify(chipMock.chip).removeP2pIface(ifaceName);
                break;
            case IfaceType.NAN:
                mInOrder.verify(chipMock.chip).removeNanIface(ifaceName);
                break;
        }

        verify(idl).onDestroyed(ifaceName);
        if (!multipleIfaceSupport) {
            availInOrder.verify(iafrl).onAvailabilityChanged(true);
        }

        verifyNoMoreInteractions(mManagerStatusListenerMock, idl, iafrl);
    }

    /**
     * Validate P2P and NAN interactions. Expect:
     * - STA created
     * - NAN created
     * - When P2P requested:
     *   - NAN torn down
     *   - P2P created
     * - NAN creation refused
     * - When P2P destroyed:
     *   - get nan available listener
     *   - Can create NAN when requested
     *
     * Relevant for any chip which supports STA + NAN || P2P (or a richer combination - but bottom
     * line of NAN and P2P being exclusive).
     */
    public void runP2pAndNanExclusiveInteractionsTestChip(ChipMockBase chipMock,
            int onlyChipMode) throws Exception {
        chipMock.initialize();
        mInOrder = inOrder(mServiceManagerMock, mWifiMock, chipMock.chip,
                mManagerStatusListenerMock);
        executeAndValidateInitializationSequence();
        executeAndValidateStartupSequence();

        InterfaceDestroyedListener staDestroyedListener = mock(
                InterfaceDestroyedListener.class);
        HalDeviceManager.InterfaceAvailableForRequestListener staAvailListener = mock(
                HalDeviceManager.InterfaceAvailableForRequestListener.class);

        InterfaceDestroyedListener nanDestroyedListener = mock(
                InterfaceDestroyedListener.class);
        HalDeviceManager.InterfaceAvailableForRequestListener nanAvailListener = mock(
                HalDeviceManager.InterfaceAvailableForRequestListener.class);

        InterfaceDestroyedListener p2pDestroyedListener = mock(
                InterfaceDestroyedListener.class);

        InOrder availInOrder = inOrder(staAvailListener, nanAvailListener);

        // Request STA
        IWifiIface staIface = validateInterfaceSequence(chipMock,
                false, // chipModeValid
                -1000, // chipModeId (only used if chipModeValid is true)
                IfaceType.STA, // ifaceTypeToCreate
                "wlan0", // ifaceName
                onlyChipMode, // finalChipMode
                false, // high priority
                null, // tearDownList
                staDestroyedListener, // destroyedListener
                staAvailListener // availableListener
        );
        availInOrder.verify(staAvailListener).onAvailabilityChanged(
                chipMock.chipMockId == CHIP_MOCK_V2 || chipMock.chipMockId == CHIP_MOCK_V3);

        // Request NAN
        IWifiIface nanIface = validateInterfaceSequence(chipMock,
                true, // chipModeValid
                onlyChipMode, // chipModeId
                IfaceType.NAN, // ifaceTypeToCreate
                "wlan0", // ifaceName
                onlyChipMode, // finalChipMode
                false, // high priority (but irrelevant)
                null, // tearDownList
                nanDestroyedListener, // destroyedListener
                nanAvailListener // availableListener
        );
        if (chipMock.chipMockId == CHIP_MOCK_V3) {
            availInOrder.verify(staAvailListener).onAvailabilityChanged(false);
        }
        availInOrder.verify(nanAvailListener).onAvailabilityChanged(false);

        // Request P2P
        IWifiIface p2pIface = validateInterfaceSequence(chipMock,
                true, // chipModeValid
                onlyChipMode, // chipModeId
                IfaceType.P2P, // ifaceTypeToCreate
                "p2p0", // ifaceName
                onlyChipMode, // finalChipMode
                false, // high priority (but irrelevant)
                new IWifiIface[]{nanIface}, // tearDownList
                p2pDestroyedListener, // destroyedListener
                null, // availableListener
                // destroyedInterfacesDestroyedListeners...
                new InterfaceDestroyedListenerWithIfaceName(
                        getName(nanIface), nanDestroyedListener)
        );

        // Request NAN: expect failure
        nanIface = mDut.createNanIface(nanDestroyedListener, mHandler);
        mDut.registerInterfaceAvailableForRequestListener(IfaceType.NAN, nanAvailListener,
                mHandler);
        collector.checkThat("NAN can't be created", nanIface, IsNull.nullValue());

        // Destroy P2P interface
        boolean status = mDut.removeIface(p2pIface);
        mInOrder.verify(chipMock.chip).removeP2pIface("p2p0");
        collector.checkThat("P2P removal success", status, equalTo(true));

        mTestLooper.dispatchAll();
        verify(p2pDestroyedListener).onDestroyed(getName(p2pIface));
        if (chipMock.chipMockId == CHIP_MOCK_V3) {
            availInOrder.verify(staAvailListener).onAvailabilityChanged(true);
        }
        availInOrder.verify(nanAvailListener).onAvailabilityChanged(true);

        // Request NAN: expect success now
        nanIface = validateInterfaceSequence(chipMock,
                true, // chipModeValid
                onlyChipMode, // chipModeId
                IfaceType.NAN, // ifaceTypeToCreate
                "wlan0", // ifaceName
                onlyChipMode, // finalChipMode
                false, // high priority (but irrelevant)
                null, // tearDownList
                nanDestroyedListener, // destroyedListener
                nanAvailListener // availableListener
        );
        if (chipMock.chipMockId == CHIP_MOCK_V3) {
            availInOrder.verify(staAvailListener).onAvailabilityChanged(false);
        }
        availInOrder.verify(nanAvailListener).onAvailabilityChanged(false);

        verifyNoMoreInteractions(mManagerStatusListenerMock, staDestroyedListener, staAvailListener,
                nanDestroyedListener, nanAvailListener, p2pDestroyedListener);
    }

    private IWifiIface validateInterfaceSequence(ChipMockBase chipMock,
            boolean chipModeValid, int chipModeId,
            int ifaceTypeToCreate, String ifaceName, int finalChipMode, boolean lowPriority,
            IWifiIface[] tearDownList,
            InterfaceDestroyedListener destroyedListener,
            HalDeviceManager.InterfaceAvailableForRequestListener availableListener,
            InterfaceDestroyedListenerWithIfaceName...destroyedInterfacesDestroyedListeners)
            throws Exception {
        // configure chip mode response
        chipMock.chipModeValid = chipModeValid;
        chipMock.chipModeId = chipModeId;

        IWifiIface iface = null;

        // configure: interface to be created
        // act: request the interface
        switch (ifaceTypeToCreate) {
            case IfaceType.STA:
                iface = mock(IWifiStaIface.class);
                doAnswer(new GetNameAnswer(ifaceName)).when(iface).getName(
                        any(IWifiIface.getNameCallback.class));
                doAnswer(new GetTypeAnswer(IfaceType.STA)).when(iface).getType(
                        any(IWifiIface.getTypeCallback.class));
                doAnswer(new CreateXxxIfaceAnswer(chipMock, mStatusOk, iface)).when(
                        chipMock.chip).createStaIface(any(IWifiChip.createStaIfaceCallback.class));

                mDut.createStaIface(lowPriority, destroyedListener, mHandler);
                break;
            case IfaceType.AP:
                iface = mock(IWifiApIface.class);
                doAnswer(new GetNameAnswer(ifaceName)).when(iface).getName(
                        any(IWifiIface.getNameCallback.class));
                doAnswer(new GetTypeAnswer(IfaceType.AP)).when(iface).getType(
                        any(IWifiIface.getTypeCallback.class));
                doAnswer(new CreateXxxIfaceAnswer(chipMock, mStatusOk, iface)).when(
                        chipMock.chip).createApIface(any(IWifiChip.createApIfaceCallback.class));

                mDut.createApIface(destroyedListener, mHandler);
                break;
            case IfaceType.P2P:
                iface = mock(IWifiP2pIface.class);
                doAnswer(new GetNameAnswer(ifaceName)).when(iface).getName(
                        any(IWifiIface.getNameCallback.class));
                doAnswer(new GetTypeAnswer(IfaceType.P2P)).when(iface).getType(
                        any(IWifiIface.getTypeCallback.class));
                doAnswer(new CreateXxxIfaceAnswer(chipMock, mStatusOk, iface)).when(
                        chipMock.chip).createP2pIface(any(IWifiChip.createP2pIfaceCallback.class));

                mDut.createP2pIface(destroyedListener, mHandler);
                break;
            case IfaceType.NAN:
                iface = mock(IWifiNanIface.class);
                doAnswer(new GetNameAnswer(ifaceName)).when(iface).getName(
                        any(IWifiIface.getNameCallback.class));
                doAnswer(new GetTypeAnswer(IfaceType.NAN)).when(iface).getType(
                        any(IWifiIface.getTypeCallback.class));
                doAnswer(new CreateXxxIfaceAnswer(chipMock, mStatusOk, iface)).when(
                        chipMock.chip).createNanIface(any(IWifiChip.createNanIfaceCallback.class));

                mDut.createNanIface(destroyedListener, mHandler);
                break;
        }
        if (availableListener != null) {
            mDut.registerInterfaceAvailableForRequestListener(ifaceTypeToCreate, availableListener,
                    mHandler);
        }

        // validate: optional tear down of interfaces
        if (tearDownList != null) {
            for (IWifiIface tearDownIface: tearDownList) {
                switch (getType(tearDownIface)) {
                    case IfaceType.STA:
                        mInOrder.verify(chipMock.chip).removeStaIface(getName(tearDownIface));
                        break;
                    case IfaceType.AP:
                        mInOrder.verify(chipMock.chip).removeApIface(getName(tearDownIface));
                        break;
                    case IfaceType.P2P:
                        mInOrder.verify(chipMock.chip).removeP2pIface(getName(tearDownIface));
                        break;
                    case IfaceType.NAN:
                        mInOrder.verify(chipMock.chip).removeNanIface(getName(tearDownIface));
                        break;
                }
            }
        }

        // validate: optional switch to the requested mode
        if (!chipModeValid || chipModeId != finalChipMode) {
            mInOrder.verify(chipMock.chip).configureChip(finalChipMode);
        } else {
            mInOrder.verify(chipMock.chip, times(0)).configureChip(anyInt());
        }

        // validate: create interface
        switch (ifaceTypeToCreate) {
            case IfaceType.STA:
                mInOrder.verify(chipMock.chip).createStaIface(
                        any(IWifiChip.createStaIfaceCallback.class));
                break;
            case IfaceType.AP:
                mInOrder.verify(chipMock.chip).createApIface(
                        any(IWifiChip.createApIfaceCallback.class));
                break;
            case IfaceType.P2P:
                mInOrder.verify(chipMock.chip).createP2pIface(
                        any(IWifiChip.createP2pIfaceCallback.class));
                break;
            case IfaceType.NAN:
                mInOrder.verify(chipMock.chip).createNanIface(
                        any(IWifiChip.createNanIfaceCallback.class));
                break;
        }

        // verify: callbacks on deleted interfaces
        mTestLooper.dispatchAll();
        for (int i = 0; i < destroyedInterfacesDestroyedListeners.length; ++i) {
            destroyedInterfacesDestroyedListeners[i].validate();
        }
        return iface;
    }

    private int getType(IWifiIface iface) throws Exception {
        Mutable<Integer> typeResp = new Mutable<>();
        iface.getType((WifiStatus status, int type) -> {
            typeResp.value = type;
        });
        return typeResp.value;
    }

    private String getName(IWifiIface iface) throws Exception {
        Mutable<String> nameResp = new Mutable<>();
        iface.getName((WifiStatus status, String name) -> {
            nameResp.value = name;
        });
        return nameResp.value;
    }

    private WifiStatus getStatus(int code) {
        WifiStatus status = new WifiStatus();
        status.code = code;
        return status;
    }

    private static class InterfaceDestroyedListenerWithIfaceName {
        private final String mIfaceName;
        @Mock private final InterfaceDestroyedListener mListener;

        InterfaceDestroyedListenerWithIfaceName(
                String ifaceName, InterfaceDestroyedListener listener) {
            mIfaceName = ifaceName;
            mListener = listener;
        }

        public void validate() {
            verify(mListener).onDestroyed(mIfaceName);
        }
    }

    private static class Mutable<E> {
        public E value;

        Mutable() {
            value = null;
        }

        Mutable(E value) {
            this.value = value;
        }
    }

    // Answer objects
    private class GetChipIdsAnswer extends MockAnswerUtil.AnswerWithArguments {
        private WifiStatus mStatus;
        private ArrayList<Integer> mChipIds;

        GetChipIdsAnswer(WifiStatus status, ArrayList<Integer> chipIds) {
            mStatus = status;
            mChipIds = chipIds;
        }

        public void answer(IWifi.getChipIdsCallback cb) {
            cb.onValues(mStatus, mChipIds);
        }
    }

    private class GetChipAnswer extends MockAnswerUtil.AnswerWithArguments {
        private WifiStatus mStatus;
        private IWifiChip mChip;

        GetChipAnswer(WifiStatus status, IWifiChip chip) {
            mStatus = status;
            mChip = chip;
        }

        public void answer(int chipId, IWifi.getChipCallback cb) {
            cb.onValues(mStatus, mChip);
        }
    }

    private class GetIdAnswer extends MockAnswerUtil.AnswerWithArguments {
        private ChipMockBase mChipMockBase;

        GetIdAnswer(ChipMockBase chipMockBase) {
            mChipMockBase = chipMockBase;
        }

        public void answer(IWifiChip.getIdCallback cb) {
            cb.onValues(mStatusOk, mChipMockBase.chipId);
        }
    }

    private class GetAvailableModesAnswer extends MockAnswerUtil.AnswerWithArguments {
        private ChipMockBase mChipMockBase;

        GetAvailableModesAnswer(ChipMockBase chipMockBase) {
            mChipMockBase = chipMockBase;
        }

        public void answer(IWifiChip.getAvailableModesCallback cb) {
            cb.onValues(mStatusOk, mChipMockBase.availableModes);
        }
    }

    private class GetModeAnswer extends MockAnswerUtil.AnswerWithArguments {
        private ChipMockBase mChipMockBase;

        GetModeAnswer(ChipMockBase chipMockBase) {
            mChipMockBase = chipMockBase;
        }

        public void answer(IWifiChip.getModeCallback cb) {
            cb.onValues(mChipMockBase.chipModeValid ? mStatusOk
                    : getStatus(WifiStatusCode.ERROR_NOT_AVAILABLE), mChipMockBase.chipModeId);
        }
    }

    private class ConfigureChipAnswer extends MockAnswerUtil.AnswerWithArguments {
        private ChipMockBase mChipMockBase;

        ConfigureChipAnswer(ChipMockBase chipMockBase) {
            mChipMockBase = chipMockBase;
        }

        public WifiStatus answer(int chipMode) {
            mChipMockBase.chipModeValid = true;
            mChipMockBase.chipModeId = chipMode;
            return mStatusOk;
        }
    }

    private class GetXxxIfaceNamesAnswer extends MockAnswerUtil.AnswerWithArguments {
        private ChipMockBase mChipMockBase;

        GetXxxIfaceNamesAnswer(ChipMockBase chipMockBase) {
            mChipMockBase = chipMockBase;
        }

        public void answer(IWifiChip.getStaIfaceNamesCallback cb) {
            cb.onValues(mStatusOk, mChipMockBase.interfaceNames.get(IfaceType.STA));
        }

        public void answer(IWifiChip.getApIfaceNamesCallback cb) {
            cb.onValues(mStatusOk, mChipMockBase.interfaceNames.get(IfaceType.AP));
        }

        public void answer(IWifiChip.getP2pIfaceNamesCallback cb) {
            cb.onValues(mStatusOk, mChipMockBase.interfaceNames.get(IfaceType.P2P));
        }

        public void answer(IWifiChip.getNanIfaceNamesCallback cb) {
            cb.onValues(mStatusOk, mChipMockBase.interfaceNames.get(IfaceType.NAN));
        }
    }

    private class GetXxxIfaceAnswer extends MockAnswerUtil.AnswerWithArguments {
        private ChipMockBase mChipMockBase;

        GetXxxIfaceAnswer(ChipMockBase chipMockBase) {
            mChipMockBase = chipMockBase;
        }

        public void answer(String name, IWifiChip.getStaIfaceCallback cb) {
            IWifiIface iface = mChipMockBase.interfacesByName.get(IfaceType.STA).get(name);
            cb.onValues(iface != null ? mStatusOk : mStatusFail, (IWifiStaIface) iface);
        }

        public void answer(String name, IWifiChip.getApIfaceCallback cb) {
            IWifiIface iface = mChipMockBase.interfacesByName.get(IfaceType.AP).get(name);
            cb.onValues(iface != null ? mStatusOk : mStatusFail, (IWifiApIface) iface);
        }

        public void answer(String name, IWifiChip.getP2pIfaceCallback cb) {
            IWifiIface iface = mChipMockBase.interfacesByName.get(IfaceType.P2P).get(name);
            cb.onValues(iface != null ? mStatusOk : mStatusFail, (IWifiP2pIface) iface);
        }

        public void answer(String name, IWifiChip.getNanIfaceCallback cb) {
            IWifiIface iface = mChipMockBase.interfacesByName.get(IfaceType.NAN).get(name);
            cb.onValues(iface != null ? mStatusOk : mStatusFail, (IWifiNanIface) iface);
        }
    }

    private class CreateXxxIfaceAnswer extends MockAnswerUtil.AnswerWithArguments {
        private ChipMockBase mChipMockBase;
        private WifiStatus mStatus;
        private IWifiIface mWifiIface;

        CreateXxxIfaceAnswer(ChipMockBase chipMockBase, WifiStatus status, IWifiIface wifiIface) {
            mChipMockBase = chipMockBase;
            mStatus = status;
            mWifiIface = wifiIface;
        }

        private void addInterfaceInfo(int type) {
            if (mStatus.code == WifiStatusCode.SUCCESS) {
                try {
                    mChipMockBase.interfaceNames.get(type).add(getName(mWifiIface));
                    mChipMockBase.interfacesByName.get(type).put(getName(mWifiIface), mWifiIface);
                } catch (Exception e) {
                    // do nothing
                }
            }
        }

        public void answer(IWifiChip.createStaIfaceCallback cb) {
            cb.onValues(mStatus, (IWifiStaIface) mWifiIface);
            addInterfaceInfo(IfaceType.STA);
        }

        public void answer(IWifiChip.createApIfaceCallback cb) {
            cb.onValues(mStatus, (IWifiApIface) mWifiIface);
            addInterfaceInfo(IfaceType.AP);
        }

        public void answer(IWifiChip.createP2pIfaceCallback cb) {
            cb.onValues(mStatus, (IWifiP2pIface) mWifiIface);
            addInterfaceInfo(IfaceType.P2P);
        }

        public void answer(IWifiChip.createNanIfaceCallback cb) {
            cb.onValues(mStatus, (IWifiNanIface) mWifiIface);
            addInterfaceInfo(IfaceType.NAN);
        }
    }

    private class CreateRttControllerAnswer extends MockAnswerUtil.AnswerWithArguments {
        private final ChipMockBase mChipMockBase;
        private final IWifiRttController mRttController;

        CreateRttControllerAnswer(ChipMockBase chipMockBase, IWifiRttController rttController) {
            mChipMockBase = chipMockBase;
            mRttController = rttController;
        }

        public void answer(IWifiIface boundIface, IWifiChip.createRttControllerCallback cb) {
            if (mChipMockBase.chipModeIdValidForRtt == mChipMockBase.chipModeId) {
                cb.onValues(mStatusOk, mRttController);
            } else {
                cb.onValues(mStatusFail, null);
            }
        }
    }

    private class RemoveXxxIfaceAnswer extends MockAnswerUtil.AnswerWithArguments {
        private ChipMockBase mChipMockBase;
        private int mType;

        RemoveXxxIfaceAnswer(ChipMockBase chipMockBase, int type) {
            mChipMockBase = chipMockBase;
            mType = type;
        }

        private WifiStatus removeIface(int type, String ifname) {
            try {
                if (!mChipMockBase.interfaceNames.get(type).remove(ifname)) {
                    return mStatusFail;
                }
                if (mChipMockBase.interfacesByName.get(type).remove(ifname) == null) {
                    return mStatusFail;
                }
            } catch (Exception e) {
                return mStatusFail;
            }
            return mStatusOk;
        }

        public WifiStatus answer(String ifname) {
            return removeIface(mType, ifname);
        }
    }

    private class GetNameAnswer extends MockAnswerUtil.AnswerWithArguments {
        private String mName;

        GetNameAnswer(String name) {
            mName = name;
        }

        public void answer(IWifiIface.getNameCallback cb) {
            cb.onValues(mStatusOk, mName);
        }
    }

    private class GetTypeAnswer extends MockAnswerUtil.AnswerWithArguments {
        private int mType;

        GetTypeAnswer(int type) {
            mType = type;
        }

        public void answer(IWifiIface.getTypeCallback cb) {
            cb.onValues(mStatusOk, mType);
        }
    }

    // chip configuration

    private static final int CHIP_MOCK_V1 = 0;
    private static final int CHIP_MOCK_V2 = 1;
    private static final int CHIP_MOCK_V3 = 2;
    private static final int CHIP_MOCK_V4 = 3;

    private class ChipMockBase {
        public int chipMockId;

        public IWifiChip chip;
        public int chipId;
        public boolean chipModeValid = false;
        public int chipModeId = -1000;
        public int chipModeIdValidForRtt = -1; // single chip mode ID where RTT can be created
        public Map<Integer, ArrayList<String>> interfaceNames = new HashMap<>();
        public Map<Integer, Map<String, IWifiIface>> interfacesByName = new HashMap<>();

        public ArrayList<IWifiChip.ChipMode> availableModes;

        void initialize() throws Exception {
            chip = mock(IWifiChip.class);

            interfaceNames.put(IfaceType.STA, new ArrayList<>());
            interfaceNames.put(IfaceType.AP, new ArrayList<>());
            interfaceNames.put(IfaceType.P2P, new ArrayList<>());
            interfaceNames.put(IfaceType.NAN, new ArrayList<>());

            interfacesByName.put(IfaceType.STA, new HashMap<>());
            interfacesByName.put(IfaceType.AP, new HashMap<>());
            interfacesByName.put(IfaceType.P2P, new HashMap<>());
            interfacesByName.put(IfaceType.NAN, new HashMap<>());

            when(chip.registerEventCallback(any(IWifiChipEventCallback.class))).thenReturn(
                    mStatusOk);
            when(chip.configureChip(anyInt())).thenAnswer(new ConfigureChipAnswer(this));
            doAnswer(new GetIdAnswer(this)).when(chip).getId(any(IWifiChip.getIdCallback.class));
            doAnswer(new GetModeAnswer(this)).when(chip).getMode(
                    any(IWifiChip.getModeCallback.class));
            GetXxxIfaceNamesAnswer getXxxIfaceNamesAnswer = new GetXxxIfaceNamesAnswer(this);
            doAnswer(getXxxIfaceNamesAnswer).when(chip).getStaIfaceNames(
                    any(IWifiChip.getStaIfaceNamesCallback.class));
            doAnswer(getXxxIfaceNamesAnswer).when(chip).getApIfaceNames(
                    any(IWifiChip.getApIfaceNamesCallback.class));
            doAnswer(getXxxIfaceNamesAnswer).when(chip).getP2pIfaceNames(
                    any(IWifiChip.getP2pIfaceNamesCallback.class));
            doAnswer(getXxxIfaceNamesAnswer).when(chip).getNanIfaceNames(
                    any(IWifiChip.getNanIfaceNamesCallback.class));
            GetXxxIfaceAnswer getXxxIfaceAnswer = new GetXxxIfaceAnswer(this);
            doAnswer(getXxxIfaceAnswer).when(chip).getStaIface(anyString(),
                    any(IWifiChip.getStaIfaceCallback.class));
            doAnswer(getXxxIfaceAnswer).when(chip).getApIface(anyString(),
                    any(IWifiChip.getApIfaceCallback.class));
            doAnswer(getXxxIfaceAnswer).when(chip).getP2pIface(anyString(),
                    any(IWifiChip.getP2pIfaceCallback.class));
            doAnswer(getXxxIfaceAnswer).when(chip).getNanIface(anyString(),
                    any(IWifiChip.getNanIfaceCallback.class));
            doAnswer(new RemoveXxxIfaceAnswer(this, IfaceType.STA)).when(chip).removeStaIface(
                    anyString());
            doAnswer(new RemoveXxxIfaceAnswer(this, IfaceType.AP)).when(chip).removeApIface(
                    anyString());
            doAnswer(new RemoveXxxIfaceAnswer(this, IfaceType.P2P)).when(chip).removeP2pIface(
                    anyString());
            doAnswer(new RemoveXxxIfaceAnswer(this, IfaceType.NAN)).when(chip).removeNanIface(
                    anyString());

            doAnswer(new CreateRttControllerAnswer(this, mRttControllerMock)).when(
                    chip).createRttController(any(), any());
        }
    }

    // test chip configuration V1:
    // mode: STA + (NAN || P2P)
    // mode: AP
    private class TestChipV1 extends ChipMockBase {
        static final int STA_CHIP_MODE_ID = 0;
        static final int AP_CHIP_MODE_ID = 1;

        void initialize() throws Exception {
            super.initialize();

            chipMockId = CHIP_MOCK_V1;

            // chip Id configuration
            ArrayList<Integer> chipIds;
            chipId = 10;
            chipIds = new ArrayList<>();
            chipIds.add(chipId);
            doAnswer(new GetChipIdsAnswer(mStatusOk, chipIds)).when(mWifiMock).getChipIds(
                    any(IWifi.getChipIdsCallback.class));

            doAnswer(new GetChipAnswer(mStatusOk, chip)).when(mWifiMock).getChip(eq(10),
                    any(IWifi.getChipCallback.class));

            // initialize dummy chip modes
            IWifiChip.ChipMode cm;
            IWifiChip.ChipIfaceCombination cic;
            IWifiChip.ChipIfaceCombinationLimit cicl;

            //   Mode 0: 1xSTA + 1x{P2P,NAN}
            //   Mode 1: 1xAP
            availableModes = new ArrayList<>();
            cm = new IWifiChip.ChipMode();
            cm.id = STA_CHIP_MODE_ID;

            cic = new IWifiChip.ChipIfaceCombination();

            cicl = new IWifiChip.ChipIfaceCombinationLimit();
            cicl.maxIfaces = 1;
            cicl.types.add(IfaceType.STA);
            cic.limits.add(cicl);

            cicl = new IWifiChip.ChipIfaceCombinationLimit();
            cicl.maxIfaces = 1;
            cicl.types.add(IfaceType.P2P);
            cicl.types.add(IfaceType.NAN);
            cic.limits.add(cicl);
            cm.availableCombinations.add(cic);
            availableModes.add(cm);

            cm = new IWifiChip.ChipMode();
            cm.id = AP_CHIP_MODE_ID;
            cic = new IWifiChip.ChipIfaceCombination();
            cicl = new IWifiChip.ChipIfaceCombinationLimit();
            cicl.maxIfaces = 1;
            cicl.types.add(IfaceType.AP);
            cic.limits.add(cicl);
            cm.availableCombinations.add(cic);
            availableModes.add(cm);

            chipModeIdValidForRtt = STA_CHIP_MODE_ID;

            doAnswer(new GetAvailableModesAnswer(this)).when(chip)
                    .getAvailableModes(any(IWifiChip.getAvailableModesCallback.class));
        }
    }

    // test chip configuration V2:
    // mode: STA + (STA || AP) + (NAN || P2P)
    private class TestChipV2 extends ChipMockBase {
        // only mode (different number from any in TestChipV1 so can catch test errors)
        static final int CHIP_MODE_ID = 5;

        void initialize() throws Exception {
            super.initialize();

            chipMockId = CHIP_MOCK_V2;

            // chip Id configuration
            ArrayList<Integer> chipIds;
            chipId = 12;
            chipIds = new ArrayList<>();
            chipIds.add(chipId);
            doAnswer(new GetChipIdsAnswer(mStatusOk, chipIds)).when(mWifiMock).getChipIds(
                    any(IWifi.getChipIdsCallback.class));

            doAnswer(new GetChipAnswer(mStatusOk, chip)).when(mWifiMock).getChip(eq(12),
                    any(IWifi.getChipCallback.class));

            // initialize dummy chip modes
            IWifiChip.ChipMode cm;
            IWifiChip.ChipIfaceCombination cic;
            IWifiChip.ChipIfaceCombinationLimit cicl;

            //   Mode 0 (only one): 1xSTA + 1x{STA,AP} + 1x{P2P,NAN}
            availableModes = new ArrayList<>();
            cm = new IWifiChip.ChipMode();
            cm.id = CHIP_MODE_ID;

            cic = new IWifiChip.ChipIfaceCombination();

            cicl = new IWifiChip.ChipIfaceCombinationLimit();
            cicl.maxIfaces = 1;
            cicl.types.add(IfaceType.STA);
            cic.limits.add(cicl);

            cicl = new IWifiChip.ChipIfaceCombinationLimit();
            cicl.maxIfaces = 1;
            cicl.types.add(IfaceType.STA);
            cicl.types.add(IfaceType.AP);
            cic.limits.add(cicl);

            cicl = new IWifiChip.ChipIfaceCombinationLimit();
            cicl.maxIfaces = 1;
            cicl.types.add(IfaceType.P2P);
            cicl.types.add(IfaceType.NAN);
            cic.limits.add(cicl);
            cm.availableCombinations.add(cic);
            availableModes.add(cm);

            chipModeIdValidForRtt = CHIP_MODE_ID;

            doAnswer(new GetAvailableModesAnswer(this)).when(chip)
                    .getAvailableModes(any(IWifiChip.getAvailableModesCallback.class));
        }
    }

    // test chip configuration V3:
    // mode:
    //    STA + (STA || AP)
    //    STA + (NAN || P2P)
    private class TestChipV3 extends ChipMockBase {
        // only mode (different number from any in other TestChips so can catch test errors)
        static final int CHIP_MODE_ID = 7;

        void initialize() throws Exception {
            super.initialize();

            chipMockId = CHIP_MOCK_V3;

            // chip Id configuration
            ArrayList<Integer> chipIds;
            chipId = 15;
            chipIds = new ArrayList<>();
            chipIds.add(chipId);
            doAnswer(new GetChipIdsAnswer(mStatusOk, chipIds)).when(mWifiMock).getChipIds(
                    any(IWifi.getChipIdsCallback.class));

            doAnswer(new GetChipAnswer(mStatusOk, chip)).when(mWifiMock).getChip(eq(15),
                    any(IWifi.getChipCallback.class));

            // initialize dummy chip modes
            IWifiChip.ChipMode cm;
            IWifiChip.ChipIfaceCombination cic;
            IWifiChip.ChipIfaceCombinationLimit cicl;

            //   Mode 0 (only one): 1xSTA + 1x{STA,AP}, 1xSTA + 1x{P2P,NAN}
            availableModes = new ArrayList<>();
            cm = new IWifiChip.ChipMode();
            cm.id = CHIP_MODE_ID;

            cic = new IWifiChip.ChipIfaceCombination();

            cicl = new IWifiChip.ChipIfaceCombinationLimit();
            cicl.maxIfaces = 1;
            cicl.types.add(IfaceType.STA);
            cic.limits.add(cicl);

            cicl = new IWifiChip.ChipIfaceCombinationLimit();
            cicl.maxIfaces = 1;
            cicl.types.add(IfaceType.STA);
            cicl.types.add(IfaceType.AP);
            cic.limits.add(cicl);

            cm.availableCombinations.add(cic);

            cic = new IWifiChip.ChipIfaceCombination();

            cicl = new IWifiChip.ChipIfaceCombinationLimit();
            cicl.maxIfaces = 1;
            cicl.types.add(IfaceType.STA);
            cic.limits.add(cicl);

            cicl = new IWifiChip.ChipIfaceCombinationLimit();
            cicl.maxIfaces = 1;
            cicl.types.add(IfaceType.P2P);
            cicl.types.add(IfaceType.NAN);
            cic.limits.add(cicl);

            cm.availableCombinations.add(cic);
            availableModes.add(cm);

            chipModeIdValidForRtt = CHIP_MODE_ID;

            doAnswer(new GetAvailableModesAnswer(this)).when(chip)
                    .getAvailableModes(any(IWifiChip.getAvailableModesCallback.class));
        }
    }

    // test chip configuration V4:
    // mode:
    //    STA + AP
    //    STA + (NAN || P2P)
    private class TestChipV4 extends ChipMockBase {
        // only mode (different number from any in other TestChips so can catch test errors)
        static final int CHIP_MODE_ID = 15;

        void initialize() throws Exception {
            super.initialize();

            chipMockId = CHIP_MOCK_V4;

            // chip Id configuration
            ArrayList<Integer> chipIds;
            chipId = 23;
            chipIds = new ArrayList<>();
            chipIds.add(chipId);
            doAnswer(new GetChipIdsAnswer(mStatusOk, chipIds)).when(mWifiMock).getChipIds(
                    any(IWifi.getChipIdsCallback.class));

            doAnswer(new GetChipAnswer(mStatusOk, chip)).when(mWifiMock).getChip(eq(23),
                    any(IWifi.getChipCallback.class));

            // initialize dummy chip modes
            IWifiChip.ChipMode cm;
            IWifiChip.ChipIfaceCombination cic;
            IWifiChip.ChipIfaceCombinationLimit cicl;

            //   Mode 0 (only one): 1xSTA + 1xAP, 1xSTA + 1x{P2P,NAN}
            availableModes = new ArrayList<>();
            cm = new IWifiChip.ChipMode();
            cm.id = CHIP_MODE_ID;

            cic = new IWifiChip.ChipIfaceCombination();

            cicl = new IWifiChip.ChipIfaceCombinationLimit();
            cicl.maxIfaces = 1;
            cicl.types.add(IfaceType.STA);
            cic.limits.add(cicl);

            cicl = new IWifiChip.ChipIfaceCombinationLimit();
            cicl.maxIfaces = 1;
            cicl.types.add(IfaceType.AP);
            cic.limits.add(cicl);

            cm.availableCombinations.add(cic);

            cic = new IWifiChip.ChipIfaceCombination();

            cicl = new IWifiChip.ChipIfaceCombinationLimit();
            cicl.maxIfaces = 1;
            cicl.types.add(IfaceType.STA);
            cic.limits.add(cicl);

            cicl = new IWifiChip.ChipIfaceCombinationLimit();
            cicl.maxIfaces = 1;
            cicl.types.add(IfaceType.P2P);
            cicl.types.add(IfaceType.NAN);
            cic.limits.add(cicl);

            cm.availableCombinations.add(cic);
            availableModes.add(cm);

            chipModeIdValidForRtt = CHIP_MODE_ID;

            doAnswer(new GetAvailableModesAnswer(this)).when(chip)
                    .getAvailableModes(any(IWifiChip.getAvailableModesCallback.class));
        }
    }
}
