/*
 * Copyright (C) 2006 The Android Open Source Project
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

package com.android.internal.telephony;

import static android.telephony.TelephonyManager.SET_OPPORTUNISTIC_SUB_INACTIVE_SUBSCRIPTION;
import static android.telephony.TelephonyManager.SET_OPPORTUNISTIC_SUB_SUCCESS;
import static android.telephony.TelephonyManager.SET_OPPORTUNISTIC_SUB_VALIDATION_FAILED;

import static com.android.internal.telephony.PhoneSwitcher.EVENT_DATA_ENABLED_CHANGED;
import static com.android.internal.telephony.PhoneSwitcher.EVENT_PRECISE_CALL_STATE_CHANGED;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertTrue;
import static org.mockito.ArgumentMatchers.any;
import static org.mockito.ArgumentMatchers.anyBoolean;
import static org.mockito.ArgumentMatchers.anyInt;
import static org.mockito.ArgumentMatchers.anyLong;
import static org.mockito.Matchers.eq;
import static org.mockito.Mockito.clearInvocations;
import static org.mockito.Mockito.doAnswer;
import static org.mockito.Mockito.doReturn;
import static org.mockito.Mockito.never;
import static org.mockito.Mockito.timeout;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;

import android.content.Context;
import android.content.Intent;
import android.net.ConnectivityManager;
import android.net.NetworkCapabilities;
import android.net.NetworkRequest;
import android.net.StringNetworkSpecifier;
import android.os.AsyncResult;
import android.os.Handler;
import android.os.HandlerThread;
import android.os.Message;
import android.os.Messenger;
import android.telephony.PhoneCapability;
import android.telephony.SubscriptionManager;
import android.test.suitebuilder.annotation.SmallTest;

import androidx.test.runner.AndroidJUnit4;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.ArgumentCaptor;
import org.mockito.Mock;

import java.util.concurrent.CompletableFuture;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.TimeUnit;

@RunWith(AndroidJUnit4.class)
public class PhoneSwitcherTest extends TelephonyTest {
    private static final String[] sNetworkAttributes = new String[] {
            "mobile,0,0,0,-1,true", "mobile_mms,2,0,2,60000,true",
            "mobile_supl,3,0,2,60000,true", "mobile_dun,4,0,2,60000,true",
            "mobile_hipri,5,0,3,60000,true", "mobile_fota,10,0,2,60000,true",
            "mobile_ims,11,0,2,60000,true", "mobile_cbs,12,0,2,60000,true",
            "mobile_ia,14,0,2,-1,true", "mobile_emergency,15,0,2,-1,true"};

    private static final int ACTIVE_PHONE_SWITCH = 1;

    @Mock
    private ITelephonyRegistry.Stub mTelRegistryMock;
    @Mock
    private ITelephonyRegistry mTelRegistryInterfaceMock;
    @Mock
    private CommandsInterface mCommandsInterface0;
    @Mock
    private CommandsInterface mCommandsInterface1;
    @Mock
    private Phone mPhone2; // mPhone as phone 1 is already defined in TelephonyTest.
    @Mock
    private Handler mActivePhoneSwitchHandler;
    @Mock
    private GsmCdmaCall mActiveCall;
    @Mock
    private GsmCdmaCall mHoldingCall;
    @Mock
    private GsmCdmaCall mInactiveCall;
    @Mock
    private ISetOpportunisticDataCallback mSetOpptDataCallback1;
    @Mock
    private ISetOpportunisticDataCallback mSetOpptDataCallback2;

    // The thread that mPhoneSwitcher will handle events in.
    private HandlerThread mHandlerThread;
    private PhoneSwitcher mPhoneSwitcher;
    private IOnSubscriptionsChangedListener mSubChangedListener;
    private ConnectivityManager mConnectivityManager;
    // The messenger of PhoneSwitcher used to receive network requests.
    private Messenger mNetworkFactoryMessenger = null;
    private int mDefaultDataSub = SubscriptionManager.INVALID_SUBSCRIPTION_ID;
    private CommandsInterface[] mCommandsInterfaces;
    private int[][] mSlotIndexToSubId;
    private boolean[] mDataAllowed;

    @Before
    public void setUp() throws Exception {
        super.setUp(getClass().getSimpleName());

        PhoneCapability phoneCapability = new PhoneCapability(1, 1, 0, null, false);
        doReturn(phoneCapability).when(mPhoneConfigurationManager).getCurrentPhoneCapability();
        mServiceManagerMockedServices.put("telephony.registry", mTelRegistryMock);
        doReturn(mTelRegistryInterfaceMock).when(mTelRegistryMock).queryLocalInterface(any());

        doReturn(Call.State.ACTIVE).when(mActiveCall).getState();
        doReturn(Call.State.IDLE).when(mInactiveCall).getState();
        doReturn(Call.State.HOLDING).when(mHoldingCall).getState();
    }

    @After
    public void tearDown() throws Exception {
        super.tearDown();
    }

    /**
     * Test that a single phone case results in our phone being active and the RIL called
     */
    @Test
    @SmallTest
    public void testRegister() throws Exception {
        final int numPhones = 2;
        final int maxActivePhones = 1;
        initialize(numPhones, maxActivePhones);

        // verify nothing has been done while there are no inputs
        assertFalse("data allowed initially", mDataAllowed[0]);
        assertFalse("data allowed initially", mDataAllowed[1]);

        NetworkRequest internetNetworkRequest = addInternetNetworkRequest(null, 50);
        waitABit();

        assertFalse("phone active after request", mPhoneSwitcher
                .shouldApplyNetworkRequest(internetNetworkRequest, 0));

        // not registered yet - shouldn't inc
        verify(mActivePhoneSwitchHandler, never()).sendMessageAtTime(any(), anyLong());

        mPhoneSwitcher.registerForActivePhoneSwitch(mActivePhoneSwitchHandler,
                ACTIVE_PHONE_SWITCH, null);

        verify(mActivePhoneSwitchHandler, times(1)).sendMessageAtTime(any(), anyLong());
        clearInvocations(mActivePhoneSwitchHandler);

        setDefaultDataSubId(0);

        verify(mActivePhoneSwitchHandler, never()).sendMessageAtTime(any(), anyLong());

        setSlotIndexToSubId(0, 0);
        mSubChangedListener.onSubscriptionsChanged();
        waitABit();

        verify(mActivePhoneSwitchHandler, times(1)).sendMessageAtTime(any(), anyLong());
        clearInvocations(mActivePhoneSwitchHandler);
        assertTrue("data not allowed", mDataAllowed[0]);

        // now try various things that should cause the active phone to switch:
        // 1 lose default via default sub change
        // 2 gain default via default sub change
        // 3 lose default via sub->phone change
        // 4 gain default via sub->phone change
        // 5 lose default network request
        // 6 gain subscription-specific request
        // 7 lose via sub->phone change
        // 8 gain via sub->phone change
        // 9 lose subscription-specific request
        // 10 don't switch phones when in emergency mode

        // 1 lose default via default sub change
        setDefaultDataSubId(1);
        waitABit();

        verify(mActivePhoneSwitchHandler, times(1)).sendMessageAtTime(any(), anyLong());
        clearInvocations(mActivePhoneSwitchHandler);
        assertFalse("data allowed", mDataAllowed[0]);

        setSlotIndexToSubId(1, 1);
        mSubChangedListener.onSubscriptionsChanged();
        waitABit();

        verify(mActivePhoneSwitchHandler, times(1)).sendMessageAtTime(any(), anyLong());
        clearInvocations(mActivePhoneSwitchHandler);
        assertFalse("data allowed", mDataAllowed[0]);
        assertTrue("data not allowed", mDataAllowed[1]);

        // 2 gain default via default sub change
        setDefaultDataSubId(0);
        waitABit();

        verify(mActivePhoneSwitchHandler, times(1)).sendMessageAtTime(any(), anyLong());
        clearInvocations(mActivePhoneSwitchHandler);
        assertFalse("data allowed", mDataAllowed[1]);
        assertTrue("data not allowed", mDataAllowed[0]);

        // 3 lose default via sub->phone change
        setSlotIndexToSubId(0, 2);
        mSubChangedListener.onSubscriptionsChanged();
        waitABit();

        verify(mActivePhoneSwitchHandler, times(1)).sendMessageAtTime(any(), anyLong());
        clearInvocations(mActivePhoneSwitchHandler);
        assertFalse("data allowed", mDataAllowed[0]);
        assertFalse("data allowed", mDataAllowed[1]);

        // 4 gain default via sub->phone change
        setSlotIndexToSubId(0, 0);
        mSubChangedListener.onSubscriptionsChanged();
        waitABit();

        verify(mActivePhoneSwitchHandler, times(1)).sendMessageAtTime(any(), anyLong());
        clearInvocations(mActivePhoneSwitchHandler);
        assertTrue("data not allowed", mDataAllowed[0]);
        assertFalse("data allowed", mDataAllowed[1]);

        // 5 lose default network request
        releaseNetworkRequest(internetNetworkRequest);
        waitABit();

        verify(mActivePhoneSwitchHandler, times(1)).sendMessageAtTime(any(), anyLong());
        clearInvocations(mActivePhoneSwitchHandler);
        assertFalse("data allowed", mDataAllowed[0]);
        assertFalse("data allowed", mDataAllowed[1]);

        // 6 gain subscription-specific request
        NetworkRequest specificInternetRequest = addInternetNetworkRequest(0, 50);
        waitABit();

        verify(mActivePhoneSwitchHandler, times(1)).sendMessageAtTime(any(), anyLong());
        clearInvocations(mActivePhoneSwitchHandler);
        assertTrue("data not allowed", mDataAllowed[0]);
        assertFalse("data allowed", mDataAllowed[1]);

        // 7 lose via sub->phone change
        setSlotIndexToSubId(0, 1);
        mSubChangedListener.onSubscriptionsChanged();
        waitABit();

        verify(mActivePhoneSwitchHandler, times(1)).sendMessageAtTime(any(), anyLong());
        clearInvocations(mActivePhoneSwitchHandler);
        assertFalse("data allowed", mDataAllowed[0]);
        assertFalse("data allowed", mDataAllowed[1]);

        // 8 gain via sub->phone change
        setSlotIndexToSubId(0, 0);
        mSubChangedListener.onSubscriptionsChanged();
        waitABit();

        verify(mActivePhoneSwitchHandler, times(1)).sendMessageAtTime(any(), anyLong());
        clearInvocations(mActivePhoneSwitchHandler);
        assertTrue("data not allowed", mDataAllowed[0]);
        assertFalse("data allowed", mDataAllowed[1]);

        // 9 lose subscription-specific request
        releaseNetworkRequest(specificInternetRequest);
        waitABit();

        verify(mActivePhoneSwitchHandler, times(1)).sendMessageAtTime(any(), anyLong());
        clearInvocations(mActivePhoneSwitchHandler);
        assertFalse("data allowed", mDataAllowed[0]);
        assertFalse("data allowed", mDataAllowed[1]);

        // 10 don't switch phones when in emergency mode
        // not ready yet - Phone turns out to be hard to stub out
//        phones[0].setInEmergencyCall(true);
//        connectivityServiceMock.addDefaultRequest();
//        waitABit();
//        if (testHandler.getActivePhoneSwitchCount() != 11) {
//            fail("after release of request, ActivePhoneSwitchCount not 11!");
//        }
//        if (commandsInterfaces[0].isDataAllowed()) fail("data allowed");
//        if (commandsInterfaces[1].isDataAllowed()) fail("data allowed");
//
//        phones[0].setInEmergencyCall(false);
//        connectivityServiceMock.addDefaultRequest();
//        waitABit();
//        if (testHandler.getActivePhoneSwitchCount() != 12) {
//            fail("after release of request, ActivePhoneSwitchCount not 11!");
//        }
//        if (commandsInterfaces[0].isDataAllowed()) fail("data allowed");
//        if (commandsInterfaces[1].isDataAllowed()) fail("data allowed");

        mHandlerThread.quit();
    }

    /**
     * Test a multi-sim case with limited active phones:
     * - lose default via default sub change
     * - lose default via sub->phone change
     * - gain default via sub->phone change
     * - gain default via default sub change
     * - lose default network request
     * - gain subscription-specific request
     * - lose via sub->phone change
     * - gain via sub->phone change
     * - lose subscription-specific request
     * - tear down low priority phone when new request comes in
     * - tear down low priority phone when sub change causes split
     * - bring up low priority phone when sub change causes join
     * - don't switch phones when in emergency mode
     */
    @Test
    @SmallTest
    public void testPrioritization() throws Exception {
        final int numPhones = 2;
        final int maxActivePhones = 1;
        initialize(numPhones, maxActivePhones);

        addInternetNetworkRequest(null, 50);
        setSlotIndexToSubId(0, 0);
        setSlotIndexToSubId(1, 1);
        setDefaultDataSubId(0);
        waitABit();
        mPhoneSwitcher.registerForActivePhoneSwitch(mActivePhoneSwitchHandler,
                ACTIVE_PHONE_SWITCH, null);
        waitABit();
        // verify initial conditions
        verify(mActivePhoneSwitchHandler, times(1)).sendMessageAtTime(any(), anyLong());

        assertTrue("data not allowed", mDataAllowed[0]);
        assertFalse("data allowed", mDataAllowed[1]);

        // now start a higher priority conneciton on the other sub
        addMmsNetworkRequest(1);
        waitABit();

        // After gain of network request, mActivePhoneSwitchHandler should be notified 2 times.
        verify(mActivePhoneSwitchHandler, times(2)).sendMessageAtTime(any(), anyLong());
        assertFalse("data allowed", mDataAllowed[0]);
        assertTrue("data not allowed", mDataAllowed[1]);

        mHandlerThread.quit();
    }

    /**
     * Verify we don't send spurious DATA_ALLOWED calls when another NetworkFactory
     * wins (ie, switch to wifi).
     */
    @Test
    @SmallTest
    public void testHigherPriorityDefault() throws Exception {
        final int numPhones = 2;
        final int maxActivePhones = 1;
        initialize(numPhones, maxActivePhones);

        addInternetNetworkRequest(null, 50);
        waitABit();

        setSlotIndexToSubId(0, 0);
        setSlotIndexToSubId(1, 1);
        setDefaultDataSubId(0);
        waitABit();

        // Phone 0 should be active
        assertTrue("data not allowed", mDataAllowed[0]);
        assertFalse("data allowed", mDataAllowed[1]);

        addInternetNetworkRequest(null, 100);
        waitABit();

        // should be no change
        assertTrue("data not allowed", mDataAllowed[0]);
        assertFalse("data allowed", mDataAllowed[1]);

        addInternetNetworkRequest(null, 0);
        waitABit();

        // should be no change
        assertTrue("data not allowed", mDataAllowed[0]);
        assertFalse("data allowed", mDataAllowed[1]);

        mHandlerThread.quit();
    }

    /**
     * Verify testSetPreferredData.
     * When preferredData is set, it overwrites defaultData sub to be active sub in single
     * active phone mode. If it's unset (to DEFAULT_SUBSCRIPTION_ID), defaultData sub becomes
     * active one.
     */
    @Test
    @SmallTest
    public void testSetPreferredData() throws Exception {
        final int numPhones = 2;
        final int maxActivePhones = 1;
        initialize(numPhones, maxActivePhones);

        // Phone 0 has sub 1, phone 1 has sub 2.
        // Sub 1 is default data sub.
        // Both are active subscriptions are active sub, as they are in both active slots.
        setSlotIndexToSubId(0, 1);
        setSlotIndexToSubId(1, 2);
        setDefaultDataSubId(1);

        // Notify phoneSwitcher about default data sub and default network request.
        addInternetNetworkRequest(null, 50);
        waitABit();
        // Phone 0 (sub 1) should be activated as it has default data sub.
        assertTrue(mDataAllowed[0]);

        // Set sub 2 as preferred sub should make phone 1 activated and phone 0 deactivated.
        mPhoneSwitcher.trySetOpportunisticDataSubscription(2, false, null);
        waitABit();
        assertFalse(mDataAllowed[0]);
        assertTrue(mDataAllowed[1]);

        // Unset preferred sub should make default data sub (phone 0 / sub 1) activated again.
        mPhoneSwitcher.trySetOpportunisticDataSubscription(
                SubscriptionManager.DEFAULT_SUBSCRIPTION_ID, false, null);
        waitABit();
        assertTrue(mDataAllowed[0]);
        assertFalse(mDataAllowed[1]);

        mHandlerThread.quit();
    }

    @Test
    @SmallTest
    public void testSetPreferredDataModemCommand() throws Exception {
        final int numPhones = 2;
        final int maxActivePhones = 1;
        doReturn(true).when(mMockRadioConfig).isSetPreferredDataCommandSupported();
        initialize(numPhones, maxActivePhones);
        mPhoneSwitcher.registerForActivePhoneSwitch(mActivePhoneSwitchHandler,
                ACTIVE_PHONE_SWITCH, null);
        mPhoneSwitcher.registerForActivePhoneSwitch(mActivePhoneSwitchHandler,
                ACTIVE_PHONE_SWITCH, null);
        verify(mActivePhoneSwitchHandler, times(2)).sendMessageAtTime(any(), anyLong());
        clearInvocations(mMockRadioConfig);
        clearInvocations(mActivePhoneSwitchHandler);

        // Phone 0 has sub 1, phone 1 has sub 2.
        // Sub 1 is default data sub.
        // Both are active subscriptions are active sub, as they are in both active slots.
        setSlotIndexToSubId(0, 1);
        setSlotIndexToSubId(1, 2);
        setDefaultDataSubId(1);
        waitABit();
        // Phone 0 (sub 1) should be preferred data phone as it has default data sub.
        verify(mMockRadioConfig).setPreferredDataModem(eq(0), any());
        verify(mActivePhoneSwitchHandler, times(2)).sendMessageAtTime(any(), anyLong());

        clearInvocations(mMockRadioConfig);
        clearInvocations(mActivePhoneSwitchHandler);

        // Notify phoneSwitcher about default data sub and default network request.
        // It shouldn't change anything.
        NetworkRequest internetRequest = addInternetNetworkRequest(null, 50);
        NetworkRequest mmsRequest = addMmsNetworkRequest(2);
        waitABit();
        verify(mMockRadioConfig, never()).setPreferredDataModem(anyInt(), any());
        verify(mActivePhoneSwitchHandler, never()).sendMessageAtTime(any(), anyLong());
        assertTrue(mPhoneSwitcher.shouldApplyNetworkRequest(internetRequest, 0));
        assertFalse(mPhoneSwitcher.shouldApplyNetworkRequest(mmsRequest, 0));
        assertFalse(mPhoneSwitcher.shouldApplyNetworkRequest(internetRequest, 1));
        assertTrue(mPhoneSwitcher.shouldApplyNetworkRequest(mmsRequest, 1));

        // Set sub 2 as preferred sub should make phone 1 preferredDataModem
        mPhoneSwitcher.trySetOpportunisticDataSubscription(2, false, null);
        waitABit();
        verify(mMockRadioConfig).setPreferredDataModem(eq(1), any());
        verify(mActivePhoneSwitchHandler, times(2)).sendMessageAtTime(any(), anyLong());
        assertFalse(mPhoneSwitcher.shouldApplyNetworkRequest(internetRequest, 0));
        assertFalse(mPhoneSwitcher.shouldApplyNetworkRequest(mmsRequest, 0));
        assertTrue(mPhoneSwitcher.shouldApplyNetworkRequest(internetRequest, 1));
        assertTrue(mPhoneSwitcher.shouldApplyNetworkRequest(mmsRequest, 1));

        clearInvocations(mMockRadioConfig);
        clearInvocations(mActivePhoneSwitchHandler);

        // Unset preferred sub should make phone0 preferredDataModem again.
        mPhoneSwitcher.trySetOpportunisticDataSubscription(
                SubscriptionManager.DEFAULT_SUBSCRIPTION_ID, false, null);
        waitABit();
        verify(mMockRadioConfig).setPreferredDataModem(eq(0), any());
        verify(mActivePhoneSwitchHandler, times(2)).sendMessageAtTime(any(), anyLong());
        assertTrue(mPhoneSwitcher.shouldApplyNetworkRequest(internetRequest, 0));
        assertFalse(mPhoneSwitcher.shouldApplyNetworkRequest(mmsRequest, 0));
        assertFalse(mPhoneSwitcher.shouldApplyNetworkRequest(internetRequest, 1));
        assertTrue(mPhoneSwitcher.shouldApplyNetworkRequest(mmsRequest, 1));

        // SetDataAllowed should never be triggered.
        verify(mCommandsInterface0, never()).setDataAllowed(anyBoolean(), any());
        verify(mCommandsInterface1, never()).setDataAllowed(anyBoolean(), any());

        mHandlerThread.quit();

    }

    @Test
    @SmallTest
    public void testSetPreferredDataWithValidation() throws Exception {
        final int numPhones = 2;
        final int maxActivePhones = 1;
        doReturn(true).when(mMockRadioConfig).isSetPreferredDataCommandSupported();
        initialize(numPhones, maxActivePhones);

        // Phone 0 has sub 1, phone 1 has sub 2.
        // Sub 1 is default data sub.
        // Both are active subscriptions are active sub, as they are in both active slots.
        setSlotIndexToSubId(0, 1);
        setSlotIndexToSubId(1, 2);
        setDefaultDataSubId(1);
        // Mark sub 2 as opportunistic.
        doReturn(true).when(mSubscriptionController).isOpportunistic(2);

        // Phone 0 (sub 1) should be activated as it has default data sub.
        assertEquals(0, mPhoneSwitcher.getPreferredDataPhoneId());

        // Set sub 2 as preferred sub should make phone 1 activated and phone 0 deactivated.
        mPhoneSwitcher.trySetOpportunisticDataSubscription(2, true, null);
        waitABit();
        verify(mCellularNetworkValidator).validate(eq(2), anyInt(), eq(false),
                eq(mPhoneSwitcher.mValidationCallback));
        // Validation failed. Preferred data sub should remain 1, data phone should remain 0.
        mPhoneSwitcher.mValidationCallback.onValidationResult(false, 2);
        waitABit();
        assertEquals(0, mPhoneSwitcher.getPreferredDataPhoneId());

        // Validation succeeds. Preferred data sub changes to 2, data phone changes to 1.
        mPhoneSwitcher.trySetOpportunisticDataSubscription(2, true, null);
        waitABit();
        mPhoneSwitcher.mValidationCallback.onValidationResult(true, 2);
        waitABit();
        assertEquals(1, mPhoneSwitcher.getPreferredDataPhoneId());

        // Switching data back to primary (subId 1).
        mPhoneSwitcher.trySetOpportunisticDataSubscription(
                SubscriptionManager.DEFAULT_SUBSCRIPTION_ID, true, null);
        waitABit();
        verify(mCellularNetworkValidator).validate(eq(1), anyInt(), eq(false),
                eq(mPhoneSwitcher.mValidationCallback));
        mPhoneSwitcher.mValidationCallback.onValidationResult(true, 1);
        waitABit();
        assertEquals(0, mPhoneSwitcher.getPreferredDataPhoneId());

        mHandlerThread.quit();
    }

    @Test
    @SmallTest
    public void testNonDefaultDataPhoneInCall() throws Exception {
        final int numPhones = 2;
        final int maxActivePhones = 1;
        doReturn(true).when(mMockRadioConfig).isSetPreferredDataCommandSupported();
        initialize(numPhones, maxActivePhones);
        // Phone 0 has sub 1, phone 1 has sub 2.
        // Sub 1 is default data sub.
        // Both are active subscriptions are active sub, as they are in both active slots.
        setSlotIndexToSubId(0, 1);
        setSlotIndexToSubId(1, 2);
        setDefaultDataSubId(1);
        waitABit();
        NetworkRequest internetRequest = addInternetNetworkRequest(null, 50);
        waitABit();
        assertTrue(mPhoneSwitcher.shouldApplyNetworkRequest(internetRequest, 0));
        assertFalse(mPhoneSwitcher.shouldApplyNetworkRequest(internetRequest, 1));
        clearInvocations(mMockRadioConfig);
        setAllPhonesInactive();
        // Initialization done.

        // Phone2 has active call, but data is turned off. So no data switching should happen.
        notifyDataEnabled(false);
        notifyPhoneAsInCall(mPhone2);
        verify(mMockRadioConfig, never()).setPreferredDataModem(anyInt(), any());
        assertTrue(mPhoneSwitcher.shouldApplyNetworkRequest(internetRequest, 0));
        assertFalse(mPhoneSwitcher.shouldApplyNetworkRequest(internetRequest, 1));

        // Phone2 has active call. So data switch to it.
        notifyDataEnabled(true);
        verify(mMockRadioConfig).setPreferredDataModem(eq(1), any());
        assertTrue(mPhoneSwitcher.shouldApplyNetworkRequest(internetRequest, 1));
        assertFalse(mPhoneSwitcher.shouldApplyNetworkRequest(internetRequest, 0));
        clearInvocations(mMockRadioConfig);

        // Phone2 call ended. So data switch back to default data sub.
        notifyPhoneAsInactive(mPhone2);
        verify(mMockRadioConfig).setPreferredDataModem(eq(0), any());
        assertTrue(mPhoneSwitcher.shouldApplyNetworkRequest(internetRequest, 0));
        assertFalse(mPhoneSwitcher.shouldApplyNetworkRequest(internetRequest, 1));
        clearInvocations(mMockRadioConfig);

        // Phone2 has holding call, but data is turned off. So no data switching should happen.
        notifyPhoneAsInHoldingCall(mPhone2);
        verify(mMockRadioConfig).setPreferredDataModem(eq(1), any());
        assertTrue(mPhoneSwitcher.shouldApplyNetworkRequest(internetRequest, 1));
        assertFalse(mPhoneSwitcher.shouldApplyNetworkRequest(internetRequest, 0));

        mHandlerThread.quit();
    }


    @Test
    @SmallTest
    public void testNetworkRequestOnNonDefaultData() throws Exception {
        final int numPhones = 2;
        final int maxActivePhones = 1;
        doReturn(true).when(mMockRadioConfig).isSetPreferredDataCommandSupported();
        initialize(numPhones, maxActivePhones);
        // Phone 0 has sub 1, phone 1 has sub 2.
        // Sub 1 is default data sub.
        // Both are active subscriptions are active sub, as they are in both active slots.
        setSlotIndexToSubId(0, 1);
        setSlotIndexToSubId(1, 2);
        setDefaultDataSubId(1);
        waitABit();
        NetworkRequest internetRequest = addInternetNetworkRequest(2, 50);
        waitABit();
        assertFalse(mPhoneSwitcher.shouldApplyNetworkRequest(internetRequest, 0));
        assertFalse(mPhoneSwitcher.shouldApplyNetworkRequest(internetRequest, 1));

        // Restricted network request will should be applied.
        internetRequest = addInternetNetworkRequest(2, 50, true);
        waitABit();
        assertFalse(mPhoneSwitcher.shouldApplyNetworkRequest(internetRequest, 0));
        assertTrue(mPhoneSwitcher.shouldApplyNetworkRequest(internetRequest, 1));

        mHandlerThread.quit();
    }

    @Test
    @SmallTest
    public void testEmergencyOverrideSuccessBeforeCallStarts() throws Exception {
        final int numPhones = 2;
        final int maxActivePhones = 1;
        doReturn(true).when(mMockRadioConfig).isSetPreferredDataCommandSupported();
        initialize(numPhones, maxActivePhones);
        // Phone 0 has sub 1, phone 1 has sub 2.
        // Sub 1 is default data sub.
        // Both are active subscriptions are active sub, as they are in both active slots.
        setMsimDefaultDataSubId(numPhones, 1);
        clearInvocations(mMockRadioConfig);

        // override the phone ID in prep for emergency call
        CountDownLatch latch = new CountDownLatch(1);
        CompletableFuture<Boolean> futurePhone = new CompletableFuture<>();
        futurePhone.whenComplete((result, error) -> {
            assertTrue(result);
            latch.countDown();
        });
        mPhoneSwitcher.overrideDefaultDataForEmergency(1, 1, futurePhone);
        sendPreferredDataSuccessResult(1);
        assertTrue(latch.await(2, TimeUnit.SECONDS));
        // Make sure the correct broadcast is sent out for the overridden phone ID
        verify(mTelRegistryInterfaceMock).notifyActiveDataSubIdChanged(eq(2));

        mHandlerThread.quit();
    }

    @Test
    @SmallTest
    public void testEmergencyOverrideNoDdsChange() throws Exception {
        final int numPhones = 2;
        final int maxActivePhones = 1;
        doReturn(true).when(mMockRadioConfig).isSetPreferredDataCommandSupported();
        initialize(numPhones, maxActivePhones);
        // Phone 0 has sub 1, phone 1 has sub 2.
        // Sub 1 is default data sub.
        // Both are active subscriptions are active sub, as they are in both active slots.
        setMsimDefaultDataSubId(numPhones, 1);
        clearInvocations(mMockRadioConfig);

        // override the phone ID in prep for emergency call
        CountDownLatch latch = new CountDownLatch(1);
        CompletableFuture<Boolean> futurePhone = new CompletableFuture<>();
        futurePhone.whenComplete((result, error) -> {
            assertTrue(result);
            latch.countDown();
        });
        mPhoneSwitcher.overrideDefaultDataForEmergency(0, 1, futurePhone);
        waitABit();
        // The radio command should never be called because the DDS hasn't changed.
        verify(mMockRadioConfig, never()).setPreferredDataModem(eq(0), any());
        assertTrue(latch.await(2, TimeUnit.SECONDS));

        mHandlerThread.quit();
    }

    @Test
    @SmallTest
    public void testEmergencyOverrideEndSuccess() throws Exception {
        PhoneSwitcher.ECBM_DEFAULT_DATA_SWITCH_BASE_TIME_MS = 500;
        final int numPhones = 2;
        final int maxActivePhones = 1;
        doReturn(true).when(mMockRadioConfig).isSetPreferredDataCommandSupported();
        initialize(numPhones, maxActivePhones);
        // Phone 0 has sub 1, phone 1 has sub 2.
        // Sub 1 is default data sub.
        // Both are active subscriptions are active sub, as they are in both active slots.
        setMsimDefaultDataSubId(numPhones, 1);
        setAllPhonesInactive();
        clearInvocations(mMockRadioConfig);
        clearInvocations(mTelRegistryInterfaceMock);

        // override the phone ID in prep for emergency call
        CountDownLatch latch = new CountDownLatch(1);
        CompletableFuture<Boolean> futurePhone = new CompletableFuture<>();
        futurePhone.whenComplete((result, error) -> {
            assertTrue(result);
            latch.countDown();
        });
        mPhoneSwitcher.overrideDefaultDataForEmergency(1, 1, futurePhone);
        sendPreferredDataSuccessResult(1);
        assertTrue(latch.await(1, TimeUnit.SECONDS));

        // Start and end the emergency call, which will start override timer
        notifyPhoneAsInCall(mPhone2);
        notifyPhoneAsInactive(mPhone2);

        // Verify that the DDS is successfully switched back after 1 second + base ECBM timeout
        verify(mMockRadioConfig,
                timeout(PhoneSwitcher.ECBM_DEFAULT_DATA_SWITCH_BASE_TIME_MS + 2000))
                .setPreferredDataModem(eq(0), any());
        // Make sure the correct broadcast is sent out for the phone ID
        verify(mTelRegistryInterfaceMock).notifyActiveDataSubIdChanged(eq(1));

        mHandlerThread.quit();
    }

    @Test
    @SmallTest
    public void testEmergencyOverrideEcbmStartEnd() throws Exception {
        PhoneSwitcher.ECBM_DEFAULT_DATA_SWITCH_BASE_TIME_MS = 500;
        final int numPhones = 2;
        final int maxActivePhones = 1;
        doReturn(true).when(mMockRadioConfig).isSetPreferredDataCommandSupported();
        initialize(numPhones, maxActivePhones);
        // Phone 0 has sub 1, phone 1 has sub 2.
        // Sub 1 is default data sub.
        // Both are active subscriptions are active sub, as they are in both active slots.
        setMsimDefaultDataSubId(numPhones, 1);
        setAllPhonesInactive();
        clearInvocations(mMockRadioConfig);
        clearInvocations(mTelRegistryInterfaceMock);

        // override the phone ID in prep for emergency call
        CountDownLatch latch = new CountDownLatch(1);
        CompletableFuture<Boolean> futurePhone = new CompletableFuture<>();
        futurePhone.whenComplete((result, error) -> {
            assertTrue(result);
            latch.countDown();
        });
        mPhoneSwitcher.overrideDefaultDataForEmergency(1, 1, futurePhone);
        sendPreferredDataSuccessResult(1);
        assertTrue(latch.await(1, TimeUnit.SECONDS));

        // Start and end the emergency call, which will start override timer
        notifyPhoneAsInCall(mPhone2);
        notifyPhoneAsInactive(mPhone2);
        // Start ECBM
        Message ecbmMessage = getEcbmRegistration(mPhone2);
        notifyEcbmStart(mPhone2, ecbmMessage);

        // DDS should not be switched back until ECBM ends, make sure there is no further
        // interaction.
        Thread.sleep(PhoneSwitcher.ECBM_DEFAULT_DATA_SWITCH_BASE_TIME_MS + 2000);
        verify(mMockRadioConfig, never()).setPreferredDataModem(eq(0), any());
        // Make sure the correct broadcast is sent out for the phone ID
        verify(mTelRegistryInterfaceMock).notifyActiveDataSubIdChanged(eq(2));

        // End ECBM
        ecbmMessage = getEcbmRegistration(mPhone2);
        notifyEcbmEnd(mPhone2, ecbmMessage);
        // Verify that the DDS is successfully switched back after 1 second.
        verify(mMockRadioConfig, timeout(2000)).setPreferredDataModem(eq(0), any());
        // Make sure the correct broadcast is sent out for the phone ID
        verify(mTelRegistryInterfaceMock).notifyActiveDataSubIdChanged(eq(1));


        mHandlerThread.quit();
    }

    @Test
    @SmallTest
    public void testEmergencyOverrideNoCallStart() throws Exception {
        PhoneSwitcher.DEFAULT_DATA_OVERRIDE_TIMEOUT_MS = 500;
        final int numPhones = 2;
        final int maxActivePhones = 1;
        doReturn(true).when(mMockRadioConfig).isSetPreferredDataCommandSupported();
        initialize(numPhones, maxActivePhones);
        // Phone 0 has sub 1, phone 1 has sub 2.
        // Sub 1 is default data sub.
        // Both are active subscriptions are active sub, as they are in both active slots.
        setMsimDefaultDataSubId(numPhones, 1);
        setAllPhonesInactive();
        clearInvocations(mMockRadioConfig);
        clearInvocations(mTelRegistryInterfaceMock);

        // override the phone ID in prep for emergency call
        CountDownLatch latch = new CountDownLatch(1);
        CompletableFuture<Boolean> futurePhone = new CompletableFuture<>();
        futurePhone.whenComplete((result, error) -> {
            assertTrue(result);
            latch.countDown();
        });
        mPhoneSwitcher.overrideDefaultDataForEmergency(1, 1, futurePhone);
        sendPreferredDataSuccessResult(1);
        assertTrue(latch.await(1, TimeUnit.SECONDS));

        // Do not start the call and make sure the override is removed once the timeout expires
        verify(mMockRadioConfig,
                timeout(PhoneSwitcher.DEFAULT_DATA_OVERRIDE_TIMEOUT_MS + 1000))
                .setPreferredDataModem(eq(0), any());
        // Make sure the correct broadcast is sent out for the phone ID
        verify(mTelRegistryInterfaceMock).notifyActiveDataSubIdChanged(eq(1));


        mHandlerThread.quit();
    }

    @Test
    @SmallTest
    public void testEmergencyOverrideMultipleOverrideRequests() throws Exception {
        PhoneSwitcher.ECBM_DEFAULT_DATA_SWITCH_BASE_TIME_MS = 500;
        final int numPhones = 2;
        final int maxActivePhones = 1;
        doReturn(true).when(mMockRadioConfig).isSetPreferredDataCommandSupported();
        initialize(numPhones, maxActivePhones);
        // Phone 0 has sub 1, phone 1 has sub 2.
        // Sub 1 is default data sub.
        // Both are active subscriptions are active sub, as they are in both active slots.
        setMsimDefaultDataSubId(numPhones, 1);
        setAllPhonesInactive();
        clearInvocations(mMockRadioConfig);
        clearInvocations(mTelRegistryInterfaceMock);

        // override the phone ID in prep for emergency call
        LinkedBlockingQueue<Boolean> queue = new LinkedBlockingQueue<>();
        CompletableFuture<Boolean> futurePhone = new CompletableFuture<>();
        futurePhone.whenComplete((r, error) -> queue.offer(r));
        mPhoneSwitcher.overrideDefaultDataForEmergency(1, 1, futurePhone);
        sendPreferredDataSuccessResult(1);
        Boolean result = queue.poll(1, TimeUnit.SECONDS);
        assertNotNull(result);
        assertTrue(result);

        // try override the phone ID again while there is an existing override for a different phone
        futurePhone = new CompletableFuture<>();
        futurePhone.whenComplete((r, error) -> queue.offer(r));
        mPhoneSwitcher.overrideDefaultDataForEmergency(0, 1, futurePhone);
        result = queue.poll(1, TimeUnit.SECONDS);
        assertNotNull(result);
        assertFalse(result);
        verify(mMockRadioConfig, never()).setPreferredDataModem(eq(0), any());

        // Start and end the emergency call, which will start override timer
        notifyPhoneAsInCall(mPhone2);
        notifyPhoneAsInactive(mPhone2);

        // Verify that the DDS is successfully switched back after 1 second + base ECBM timeout
        verify(mMockRadioConfig,
                timeout(PhoneSwitcher.ECBM_DEFAULT_DATA_SWITCH_BASE_TIME_MS + 2000))
                .setPreferredDataModem(eq(0), any());
        // Make sure the correct broadcast is sent out for the phone ID
        verify(mTelRegistryInterfaceMock).notifyActiveDataSubIdChanged(eq(1));


        mHandlerThread.quit();
    }

    @Test
    @SmallTest
    public void testSetPreferredDataCallback() throws Exception {
        final int numPhones = 2;
        final int maxActivePhones = 1;
        doReturn(true).when(mMockRadioConfig).isSetPreferredDataCommandSupported();
        initialize(numPhones, maxActivePhones);

        // Mark sub 2 as opportunistic.
        doReturn(true).when(mSubscriptionController).isOpportunistic(2);
        // Phone 0 has sub 1, phone 1 has sub 2.
        // Sub 1 is default data sub.
        // Both are active subscriptions are active sub, as they are in both active slots.
        setSlotIndexToSubId(0, 1);
        setSlotIndexToSubId(1, 2);
        setDefaultDataSubId(1);
        waitABit();

        // Validating on sub 10 which is inactive.
        mPhoneSwitcher.trySetOpportunisticDataSubscription(10, true, mSetOpptDataCallback1);
        waitABit();
        verify(mSetOpptDataCallback1).onComplete(SET_OPPORTUNISTIC_SUB_INACTIVE_SUBSCRIPTION);

        // Switch to active subId without validating. Should always succeed.
        mPhoneSwitcher.trySetOpportunisticDataSubscription(2, false, mSetOpptDataCallback1);
        waitABit();
        verify(mSetOpptDataCallback1).onComplete(SET_OPPORTUNISTIC_SUB_SUCCESS);

        // Validating on sub 1 and fails.
        clearInvocations(mSetOpptDataCallback1);
        mPhoneSwitcher.trySetOpportunisticDataSubscription(1, true, mSetOpptDataCallback1);
        waitABit();
        mPhoneSwitcher.mValidationCallback.onValidationResult(false, 1);
        waitABit();
        verify(mSetOpptDataCallback1).onComplete(SET_OPPORTUNISTIC_SUB_VALIDATION_FAILED);

        // Validating on sub 2 and succeeds.
        mPhoneSwitcher.trySetOpportunisticDataSubscription(2, true, mSetOpptDataCallback2);
        waitABit();
        mPhoneSwitcher.mValidationCallback.onValidationResult(true, 2);
        waitABit();
        verify(mSetOpptDataCallback2).onComplete(SET_OPPORTUNISTIC_SUB_SUCCESS);

        // Switching data back to primary and validation fails.
        clearInvocations(mSetOpptDataCallback2);
        mPhoneSwitcher.trySetOpportunisticDataSubscription(
                SubscriptionManager.DEFAULT_SUBSCRIPTION_ID, true, mSetOpptDataCallback2);
        waitABit();
        mPhoneSwitcher.mValidationCallback.onValidationResult(false, 1);
        waitABit();
        verify(mSetOpptDataCallback1).onComplete(SET_OPPORTUNISTIC_SUB_VALIDATION_FAILED);

        // Switching data back to primary and succeeds.
        clearInvocations(mSetOpptDataCallback2);
        mPhoneSwitcher.trySetOpportunisticDataSubscription(
                SubscriptionManager.DEFAULT_SUBSCRIPTION_ID, true, mSetOpptDataCallback2);
        waitABit();
        mPhoneSwitcher.mValidationCallback.onValidationResult(true, 1);
        waitABit();
        verify(mSetOpptDataCallback2).onComplete(SET_OPPORTUNISTIC_SUB_SUCCESS);

        // Back to back call on same subId.
        clearInvocations(mSetOpptDataCallback1);
        clearInvocations(mSetOpptDataCallback2);
        mPhoneSwitcher.trySetOpportunisticDataSubscription(2, true, mSetOpptDataCallback1);
        waitABit();
        verify(mCellularNetworkValidator).validate(eq(2), anyInt(), eq(false),
                eq(mPhoneSwitcher.mValidationCallback));
        doReturn(true).when(mCellularNetworkValidator).isValidating();
        mPhoneSwitcher.trySetOpportunisticDataSubscription(2, true, mSetOpptDataCallback2);
        waitABit();
        verify(mSetOpptDataCallback1).onComplete(SET_OPPORTUNISTIC_SUB_VALIDATION_FAILED);
        verify(mSetOpptDataCallback2, never()).onComplete(anyInt());
        // Validation succeeds.
        doReturn(false).when(mCellularNetworkValidator).isValidating();
        mPhoneSwitcher.mValidationCallback.onValidationResult(true, 2);
        waitABit();
        verify(mSetOpptDataCallback2).onComplete(SET_OPPORTUNISTIC_SUB_SUCCESS);

        mPhoneSwitcher.trySetOpportunisticDataSubscription(
                SubscriptionManager.DEFAULT_SUBSCRIPTION_ID, false, null);
        waitABit();
        clearInvocations(mSetOpptDataCallback1);
        clearInvocations(mSetOpptDataCallback2);
        clearInvocations(mCellularNetworkValidator);
        // Back to back call, call 1 to switch to subId 2, call 2 to switch back.
        mPhoneSwitcher.trySetOpportunisticDataSubscription(2, true, mSetOpptDataCallback1);
        waitABit();
        verify(mCellularNetworkValidator).validate(eq(2), anyInt(), eq(false),
                eq(mPhoneSwitcher.mValidationCallback));
        doReturn(true).when(mCellularNetworkValidator).isValidating();
        mPhoneSwitcher.trySetOpportunisticDataSubscription(
                SubscriptionManager.DEFAULT_SUBSCRIPTION_ID, true, mSetOpptDataCallback2);
        waitABit();
        // Call 1 should be cancelled and failed. Call 2 return success immediately as there's no
        // change.
        verify(mSetOpptDataCallback1).onComplete(SET_OPPORTUNISTIC_SUB_VALIDATION_FAILED);
        verify(mSetOpptDataCallback2).onComplete(SET_OPPORTUNISTIC_SUB_SUCCESS);
        mHandlerThread.quit();
    }

    /* Private utility methods start here */

    private void setAllPhonesInactive() {
        doReturn(mInactiveCall).when(mPhone).getForegroundCall();
        doReturn(mInactiveCall).when(mPhone).getBackgroundCall();
        doReturn(mInactiveCall).when(mPhone).getRingingCall();
        doReturn(mInactiveCall).when(mPhone2).getForegroundCall();
        doReturn(mInactiveCall).when(mPhone2).getBackgroundCall();
        doReturn(mInactiveCall).when(mPhone2).getRingingCall();
    }

    private void notifyPhoneAsInCall(Phone phone) {
        doReturn(mActiveCall).when(phone).getForegroundCall();
        mPhoneSwitcher.sendEmptyMessage(EVENT_PRECISE_CALL_STATE_CHANGED);
        waitABit();
    }

    private void notifyPhoneAsInHoldingCall(Phone phone) {
        doReturn(mHoldingCall).when(phone).getBackgroundCall();
        mPhoneSwitcher.sendEmptyMessage(EVENT_PRECISE_CALL_STATE_CHANGED);
        waitABit();
    }

    private void notifyPhoneAsInactive(Phone phone) {
        doReturn(mInactiveCall).when(phone).getForegroundCall();
        mPhoneSwitcher.sendEmptyMessage(EVENT_PRECISE_CALL_STATE_CHANGED);
        waitABit();
    }

    private void notifyDataEnabled(boolean dataEnabled) {
        doReturn(dataEnabled).when(mDataEnabledSettings).isDataEnabled(anyInt());
        mPhoneSwitcher.sendEmptyMessage(EVENT_DATA_ENABLED_CHANGED);
        waitABit();
    }

    private Message getEcbmRegistration(Phone phone) {
        ArgumentCaptor<Handler> handlerCaptor = ArgumentCaptor.forClass(Handler.class);
        ArgumentCaptor<Integer> intCaptor = ArgumentCaptor.forClass(Integer.class);

        verify(phone).registerForEmergencyCallToggle(handlerCaptor.capture(), intCaptor.capture(),
                any());
        assertNotNull(handlerCaptor.getValue());
        assertNotNull(intCaptor.getValue());
        Message message = Message.obtain(handlerCaptor.getValue(), intCaptor.getValue());
        return message;
    }

    private void notifyEcbmStart(Phone phone, Message ecmMessage) {
        doReturn(mInactiveCall).when(phone).getForegroundCall();
        doReturn(true).when(phone).isInEcm();
        ecmMessage.sendToTarget();
        waitABit();
    }

    private void notifyEcbmEnd(Phone phone, Message ecmMessage) {
        doReturn(false).when(phone).isInEcm();
        ecmMessage.sendToTarget();
        waitABit();
    }

    private void sendPreferredDataSuccessResult(int phoneId) {
        // make sure the radio command is called and then send a success result
        ArgumentCaptor<Message> msgCaptor = ArgumentCaptor.forClass(Message.class);
        verify(mMockRadioConfig, timeout(500)).setPreferredDataModem(eq(phoneId),
                msgCaptor.capture());
        assertNotNull(msgCaptor.getValue());
        // Send back successful result
        AsyncResult.forMessage(msgCaptor.getValue(), null, null);
        msgCaptor.getValue().sendToTarget();
        waitABit();
    }

    private void setMsimDefaultDataSubId(int numPhones, int defaultDataSub) throws Exception {
        for (int i = 0; i < numPhones; i++) {
            setSlotIndexToSubId(i, i + 1);
        }
        setDefaultDataSubId(defaultDataSub);
        waitABit();
        NetworkRequest internetRequest = addInternetNetworkRequest(null, 50);
        waitABit();
        for (int i = 0; i < numPhones; i++) {
            if (defaultDataSub == (i + 1)) {
                // sub id is always phoneId+1 for testing
                assertTrue(mPhoneSwitcher.shouldApplyNetworkRequest(internetRequest, i));
            } else {
                assertFalse(mPhoneSwitcher.shouldApplyNetworkRequest(internetRequest, i));
            }
        }
    }

    private void sendDefaultDataSubChanged() {
        final Intent intent = new Intent(TelephonyIntents.ACTION_DEFAULT_DATA_SUBSCRIPTION_CHANGED);
        mContext.sendBroadcast(intent);
    }

    private void initialize(int numPhones, int maxActivePhones) throws Exception {
        mContextFixture.putStringArrayResource(com.android.internal.R.array.networkAttributes,
                sNetworkAttributes);

        setNumPhones(numPhones);

        initializeSubControllerMock();
        initializeCommandInterfacesMock(numPhones);
        initializeTelRegistryMock();
        initializeConnManagerMock();

        mHandlerThread = new HandlerThread("PhoneSwitcherTestThread") {
            @Override
            public void onLooperPrepared() {
                mPhoneSwitcher = new PhoneSwitcher(maxActivePhones, numPhones,
                        mContext, mSubscriptionController, this.getLooper(),
                        mTelRegistryMock, mCommandsInterfaces, mPhones);
            }
        };

        mHandlerThread.start();
        waitABit();

        verify(mTelRegistryMock).addOnSubscriptionsChangedListener(
                eq(mContext.getOpPackageName()), any());
    }

    /**
     * Certain variables needs initialized depending on number of phones.
     */
    private void setNumPhones(int numPhones) {
        mDataAllowed = new boolean[numPhones];
        mSlotIndexToSubId = new int[numPhones][];
        doReturn(0).when(mPhone).getPhoneId();
        doReturn(1).when(mPhone2).getPhoneId();
        doReturn(true).when(mPhone2).isUserDataEnabled();
        doReturn(mDataEnabledSettings).when(mPhone2).getDataEnabledSettings();
        for (int i = 0; i < numPhones; i++) {
            mSlotIndexToSubId[i] = new int[1];
            mSlotIndexToSubId[i][0] = SubscriptionManager.INVALID_SUBSCRIPTION_ID;
        }

        doReturn(numPhones).when(mTelephonyManager).getPhoneCount();
        if (numPhones == 1) {
            mCommandsInterfaces = new CommandsInterface[] {mCommandsInterface0};
            mPhones = new Phone[] {mPhone};
        } else if (numPhones == 2) {
            mCommandsInterfaces =
                    new CommandsInterface[] {mCommandsInterface0, mCommandsInterface1};
            mPhones = new Phone[] {mPhone, mPhone2};
        }
    }

    private void initializeCommandInterfacesMock(int numPhones) {
        // Tell PhoneSwitcher that radio is on.
        doAnswer(invocation -> {
            Handler handler = (Handler) invocation.getArguments()[0];
            int message = (int) invocation.getArguments()[1];
            Object obj = invocation.getArguments()[2];
            handler.obtainMessage(message, obj).sendToTarget();
            return null;
        }).when(mCommandsInterface0).registerForAvailable(any(), anyInt(), any());

        // Store values of dataAllowed in mDataAllowed[] for easier checking.
        doAnswer(invocation -> {
            mDataAllowed[0] = (boolean) invocation.getArguments()[0];
            return null;
        }).when(mCommandsInterface0).setDataAllowed(anyBoolean(), any());

        if (numPhones == 2) {
            doAnswer(invocation -> {
                mDataAllowed[1] = (boolean) invocation.getArguments()[0];
                return null;
            }).when(mCommandsInterface1).setDataAllowed(anyBoolean(), any());
        }
    }

    /**
     * Store subChangedListener of PhoneSwitcher so that testing can notify
     * PhoneSwitcher of sub change.
     */
    private void initializeTelRegistryMock() throws Exception {
        doAnswer(invocation -> {
            IOnSubscriptionsChangedListener subChangedListener =
                    (IOnSubscriptionsChangedListener) invocation.getArguments()[1];
            mSubChangedListener = subChangedListener;
            mSubChangedListener.onSubscriptionsChanged();
            return null;
        }).when(mTelRegistryMock).addOnSubscriptionsChangedListener(any(), any());
    }

    /**
     * Capture mNetworkFactoryMessenger so that testing can request or release
     * network requests on PhoneSwitcher.
     */
    private void initializeConnManagerMock() {
        mConnectivityManager = (ConnectivityManager)
                mContext.getSystemService(Context.CONNECTIVITY_SERVICE);

        doAnswer(invocation -> {
            mNetworkFactoryMessenger = invocation.getArgument(0);
            return null;
        }).when(mConnectivityManager).registerNetworkFactory(any(), any());
    }

    /**
     * Capture mNetworkFactoryMessenger so that testing can request or release
     * network requests on PhoneSwitcher.
     */
    private void initializeSubControllerMock() {
        doReturn(mDefaultDataSub).when(mSubscriptionController).getDefaultDataSubId();
        doAnswer(invocation -> {
            int phoneId = (int) invocation.getArguments()[0];
            if (phoneId == SubscriptionManager.INVALID_PHONE_INDEX) {
                return SubscriptionManager.INVALID_SUBSCRIPTION_ID;
            } else if (phoneId == SubscriptionManager.DEFAULT_PHONE_INDEX) {
                return mSlotIndexToSubId[0][0];
            } else {
                return mSlotIndexToSubId[phoneId][0];
            }
        }).when(mSubscriptionController).getSubIdUsingPhoneId(anyInt());

        doAnswer(invocation -> {
            int subId = (int) invocation.getArguments()[0];

            if (!SubscriptionManager.isUsableSubIdValue(subId)) return false;

            for (int i = 0; i < mSlotIndexToSubId.length; i++) {
                if (mSlotIndexToSubId[i][0] == subId) return true;
            }
            return false;
        }).when(mSubscriptionController).isActiveSubId(anyInt());
    }

    private void setDefaultDataSubId(int defaultDataSub) {
        mDefaultDataSub = defaultDataSub;
        doReturn(mDefaultDataSub).when(mSubscriptionController).getDefaultDataSubId();
        sendDefaultDataSubChanged();
    }

    private void setSlotIndexToSubId(int slotId, int subId) {
        mSlotIndexToSubId[slotId][0] = subId;
    }

    /**
     * Create an internet PDN network request and send it to PhoneSwitcher.
     */
    private NetworkRequest addInternetNetworkRequest(Integer subId, int score) throws Exception {
        return addInternetNetworkRequest(subId, score, false);
    }

    private NetworkRequest addInternetNetworkRequest(Integer subId, int score, boolean restricted)
            throws Exception {
        NetworkCapabilities netCap = (new NetworkCapabilities())
                .addCapability(NetworkCapabilities.NET_CAPABILITY_INTERNET)
                .addTransportType(NetworkCapabilities.TRANSPORT_CELLULAR);
        if (restricted) {
            netCap.removeCapability(NetworkCapabilities.NET_CAPABILITY_NOT_RESTRICTED);
        }

        if (subId != null) {
            netCap.setNetworkSpecifier(new StringNetworkSpecifier(Integer.toString(subId)));
        }
        NetworkRequest networkRequest = new NetworkRequest(netCap, ConnectivityManager.TYPE_NONE,
                0, NetworkRequest.Type.REQUEST);

        Message message = Message.obtain();
        message.what = android.net.NetworkFactory.CMD_REQUEST_NETWORK;
        message.arg1 = score;
        message.obj = networkRequest;
        mNetworkFactoryMessenger.send(message);

        return networkRequest;
    }

    /**
     * Create a mms PDN network request and send it to PhoneSwitcher.
     */
    private NetworkRequest addMmsNetworkRequest(Integer subId) throws Exception {
        NetworkCapabilities netCap = (new NetworkCapabilities())
                .addCapability(NetworkCapabilities.NET_CAPABILITY_MMS)
                .addCapability(NetworkCapabilities.NET_CAPABILITY_NOT_RESTRICTED)
                .addTransportType(NetworkCapabilities.TRANSPORT_CELLULAR);
        netCap.setNetworkSpecifier(new StringNetworkSpecifier(Integer.toString(subId)));
        if (subId != null) {
            netCap.setNetworkSpecifier(new StringNetworkSpecifier(Integer.toString(subId)));
        }
        NetworkRequest networkRequest = new NetworkRequest(netCap, ConnectivityManager.TYPE_NONE,
                1, NetworkRequest.Type.REQUEST);

        Message message = Message.obtain();
        message.what = android.net.NetworkFactory.CMD_REQUEST_NETWORK;
        message.arg1 = 50; // Score
        message.obj = networkRequest;
        mNetworkFactoryMessenger.send(message);

        return networkRequest;
    }

    /**
     * Tell PhoneSwitcher to release a network request.
     */
    private void releaseNetworkRequest(NetworkRequest networkRequest) throws Exception {
        Message message = Message.obtain();
        message.what = android.net.NetworkFactory.CMD_CANCEL_REQUEST;
        message.obj = networkRequest;
        mNetworkFactoryMessenger.send(message);
    }

    private void waitABit() {
        try {
            Thread.sleep(250);
        } catch (Exception e) {
        }
    }
}
