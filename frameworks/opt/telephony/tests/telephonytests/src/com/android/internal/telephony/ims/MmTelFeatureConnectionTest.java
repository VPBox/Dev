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

package com.android.internal.telephony.ims;

import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;
import static org.mockito.ArgumentMatchers.any;
import static org.mockito.ArgumentMatchers.anyBoolean;
import static org.mockito.Mockito.doReturn;
import static org.mockito.Mockito.never;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;

import android.content.Context;
import android.os.Binder;
import android.os.IBinder;
import android.os.IInterface;
import android.os.Looper;
import android.telephony.SubscriptionInfo;
import android.telephony.SubscriptionManager;
import android.test.suitebuilder.annotation.SmallTest;

import com.android.ims.MmTelFeatureConnection;
import com.android.internal.telephony.TelephonyTest;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import java.util.ArrayList;
import java.util.List;

public class MmTelFeatureConnectionTest extends TelephonyTest {

    private class TestCallback extends Binder implements IInterface {

        @Override
        public IBinder asBinder() {
            return this;
        }
    }

    private class CallbackManagerTest extends
            MmTelFeatureConnection.CallbackAdapterManager<TestCallback> {

        List<TestCallback> mCallbacks = new ArrayList<>();

        CallbackManagerTest(Context context, Object lock) {
            super(context, lock);
        }

        // A callback has been registered. Register that callback with the MmTelFeature.
        @Override
        public void registerCallback(TestCallback localCallback) {
            mCallbacks.add(localCallback);
        }

        // A callback has been removed, unregister that callback with the MmTelFeature.
        @Override
        public void unregisterCallback(TestCallback localCallback) {
            mCallbacks.remove(localCallback);
        }

        public boolean doesCallbackExist(TestCallback callback) {
            return mCallbacks.contains(callback);
        }
    }
    private CallbackManagerTest mCallbackManagerUT;

    @Before
    public void setUp() throws Exception {
        super.setUp("MmTelFeatureConnectionTest");
        if (Looper.myLooper() == null) {
            Looper.prepare();
        }
        mCallbackManagerUT = new CallbackManagerTest(mContext, this);
    }

    @After
    public void tearDown() throws Exception {
        mCallbackManagerUT = null;
        super.tearDown();
    }

    /**
     * Basic test of deprecated functionality, ensure that adding the callback directly triggers the
     * appropriate registerCallback and unregisterCallback calls.
     */
    @Test
    @SmallTest
    public void testCallbackAdapter_addAndRemoveCallback() throws Exception {
        TestCallback testCallback = new TestCallback();
        mCallbackManagerUT.addCallback(testCallback);
        assertTrue(mCallbackManagerUT.doesCallbackExist(testCallback));
        // The subscriptions changed listener should only be added for callbacks that are being
        // linked to a subscription.
        verify(mSubscriptionManager, never()).addOnSubscriptionsChangedListener(
                any(SubscriptionManager.OnSubscriptionsChangedListener.class));

        mCallbackManagerUT.removeCallback(testCallback);
        assertFalse(mCallbackManagerUT.doesCallbackExist(testCallback));
        // The subscriptions changed listener should only be removed for callbacks that are
        // linked to a subscription.
        verify(mSubscriptionManager, never()).removeOnSubscriptionsChangedListener(
                any(SubscriptionManager.OnSubscriptionsChangedListener.class));
    }

    /**
     * Ensure that adding the callback and linking subId triggers the appropriate registerCallback
     * and unregisterCallback calls as well as the subscriptionChanged listener.
     */
    @Test
    @SmallTest
    public void testCallbackAdapter_addAndRemoveCallbackForSub() throws Exception {
        TestCallback testCallback = new TestCallback();
        int testSub = 1;
        mCallbackManagerUT.addCallbackForSubscription(testCallback, testSub);
        assertTrue(mCallbackManagerUT.doesCallbackExist(testCallback));
        verify(mSubscriptionManager, times(1)).addOnSubscriptionsChangedListener(
                any(SubscriptionManager.OnSubscriptionsChangedListener.class));

        mCallbackManagerUT.removeCallbackForSubscription(testCallback, testSub);
        assertFalse(mCallbackManagerUT.doesCallbackExist(testCallback));
        verify(mSubscriptionManager, times(1)).removeOnSubscriptionsChangedListener(
                any(SubscriptionManager.OnSubscriptionsChangedListener.class));
    }

    /**
     * Ensure that adding the callback and linking multiple subIds trigger the appropriate
     * registerCallback and unregisterCallback calls as well as the subscriptionChanged listener.
     * When removing the callbacks, the subscriptionChanged listener shoud only be removed when all
     * callbacks have been removed.
     */
    @Test
    @SmallTest
    public void testCallbackAdapter_addAndRemoveCallbackForMultipleSubs() throws Exception {
        TestCallback testCallback1 = new TestCallback();
        TestCallback testCallback2 = new TestCallback();
        int testSub1 = 1;
        int testSub2 = 2;
        mCallbackManagerUT.addCallbackForSubscription(testCallback1, testSub1);
        assertTrue(mCallbackManagerUT.doesCallbackExist(testCallback1));
        mCallbackManagerUT.addCallbackForSubscription(testCallback2, testSub2);
        assertTrue(mCallbackManagerUT.doesCallbackExist(testCallback2));
        // This should only happen once.
        verify(mSubscriptionManager, times(1)).addOnSubscriptionsChangedListener(
                any(SubscriptionManager.OnSubscriptionsChangedListener.class));

        mCallbackManagerUT.removeCallbackForSubscription(testCallback1, testSub1);
        assertFalse(mCallbackManagerUT.doesCallbackExist(testCallback1));
        // removing the listener should not happen until the second callback is removed.
        verify(mSubscriptionManager, never()).removeOnSubscriptionsChangedListener(
                any(SubscriptionManager.OnSubscriptionsChangedListener.class));

        mCallbackManagerUT.removeCallbackForSubscription(testCallback2, testSub2);
        assertFalse(mCallbackManagerUT.doesCallbackExist(testCallback2));
        verify(mSubscriptionManager, times(1)).removeOnSubscriptionsChangedListener(
                any(SubscriptionManager.OnSubscriptionsChangedListener.class));
    }

    /**
     * The subscriptions have changed, ensure that the callbacks registered to the original
     * subscription testSub1 are removed, while keeping the callbacks for testSub2, since it was not
     * removed.
     */
    @Test
    @SmallTest
    public void testCallbackAdapter_onSubscriptionsChangedMultipleSubs() throws Exception {
        TestCallback testCallback1 = new TestCallback();
        TestCallback testCallback2 = new TestCallback();
        int testSub1 = 1;
        int testSub2 = 2;
        int testSub3 = 3;
        mCallbackManagerUT.addCallbackForSubscription(testCallback1, testSub1);
        assertTrue(mCallbackManagerUT.doesCallbackExist(testCallback1));
        mCallbackManagerUT.addCallbackForSubscription(testCallback2, testSub2);
        assertTrue(mCallbackManagerUT.doesCallbackExist(testCallback2));
        verify(mSubscriptionManager, times(1)).addOnSubscriptionsChangedListener(
                any(SubscriptionManager.OnSubscriptionsChangedListener.class));

        // Simulate subscriptions changed, where testSub1 is no longer active
        doReturn(createSubscriptionInfoList(new int[] {testSub2, testSub3}))
                .when(mSubscriptionManager).getActiveSubscriptionInfoList(anyBoolean());
        mCallbackManagerUT.mSubChangedListener.onSubscriptionsChanged();
        assertFalse(mCallbackManagerUT.doesCallbackExist(testCallback1));
        // verify that the subscription changed listener is not removed, since we still have a
        // callback on testSub2
        verify(mSubscriptionManager, never()).removeOnSubscriptionsChangedListener(
                any(SubscriptionManager.OnSubscriptionsChangedListener.class));
    }

    /**
     * The active subscription has changed, ensure that the callback registered to the original
     * subscription testSub1 are removed as well as the subscription changed listener, since
     * there are mo more active callbacks.
     */
    @Test
    @SmallTest
    public void testCallbackAdapter_onSubscriptionsChangedOneSub() throws Exception {
        TestCallback testCallback1 = new TestCallback();
        int testSub1 = 1;
        int testSub2 = 2;
        mCallbackManagerUT.addCallbackForSubscription(testCallback1, testSub1);
        assertTrue(mCallbackManagerUT.doesCallbackExist(testCallback1));
        verify(mSubscriptionManager, times(1)).addOnSubscriptionsChangedListener(
                any(SubscriptionManager.OnSubscriptionsChangedListener.class));

        // Simulate subscriptions changed, where testSub1 is no longer active
        doReturn(createSubscriptionInfoList(new int[] {testSub2}))
                .when(mSubscriptionManager).getActiveSubscriptionInfoList(anyBoolean());
        mCallbackManagerUT.mSubChangedListener.onSubscriptionsChanged();
        assertFalse(mCallbackManagerUT.doesCallbackExist(testCallback1));
        // verify that the subscription listener is removed, since the only active callback has been
        // removed.
        verify(mSubscriptionManager, times(1)).removeOnSubscriptionsChangedListener(
                any(SubscriptionManager.OnSubscriptionsChangedListener.class));
    }

    /**
     * The close() method has been called, so al callbacks should be cleaned up and notified
     * that they have been removed. The subscriptions changed listener should also be removed.
     */
    @Test
    @SmallTest
    public void testCallbackAdapter_closeMultipleSubs() throws Exception {
        TestCallback testCallback1 = new TestCallback();
        TestCallback testCallback2 = new TestCallback();
        int testSub1 = 1;
        int testSub2 = 2;
        mCallbackManagerUT.addCallbackForSubscription(testCallback1, testSub1);
        assertTrue(mCallbackManagerUT.doesCallbackExist(testCallback1));
        mCallbackManagerUT.addCallbackForSubscription(testCallback2, testSub2);
        assertTrue(mCallbackManagerUT.doesCallbackExist(testCallback2));
        verify(mSubscriptionManager, times(1)).addOnSubscriptionsChangedListener(
                any(SubscriptionManager.OnSubscriptionsChangedListener.class));

        // Close the manager, ensure all subscription callbacks are removed
        mCallbackManagerUT.close();
        assertFalse(mCallbackManagerUT.doesCallbackExist(testCallback1));
        assertFalse(mCallbackManagerUT.doesCallbackExist(testCallback2));
        // verify that the subscription changed listener is removed.
        verify(mSubscriptionManager, times(1)).removeOnSubscriptionsChangedListener(
                any(SubscriptionManager.OnSubscriptionsChangedListener.class));
    }

    /**
     * The close() method has been called, so all callbacks should be cleaned up. Since they are
     * not associated with any subscriptions, no subscription based logic should be called.
     */
    @Test
    @SmallTest
    public void testCallbackAdapter_closeSlotBasedCallbacks() throws Exception {
        TestCallback testCallback1 = new TestCallback();
        TestCallback testCallback2 = new TestCallback();
        mCallbackManagerUT.addCallback(testCallback1);
        assertTrue(mCallbackManagerUT.doesCallbackExist(testCallback1));
        mCallbackManagerUT.addCallback(testCallback2);
        assertTrue(mCallbackManagerUT.doesCallbackExist(testCallback2));
        // verify that the subscription changed listener is never called for these callbacks
        // because they are not associated with any subscriptions.
        verify(mSubscriptionManager, never()).addOnSubscriptionsChangedListener(
                any(SubscriptionManager.OnSubscriptionsChangedListener.class));

        // Close the manager, ensure all subscription callbacks are removed
        mCallbackManagerUT.close();
        assertFalse(mCallbackManagerUT.doesCallbackExist(testCallback1));
        assertFalse(mCallbackManagerUT.doesCallbackExist(testCallback2));
        // verify that the subscription changed removed method is never called
        verify(mSubscriptionManager, never()).removeOnSubscriptionsChangedListener(
                any(SubscriptionManager.OnSubscriptionsChangedListener.class));
    }

    private List<SubscriptionInfo> createSubscriptionInfoList(int[] subIds) {
        List<SubscriptionInfo> infos = new ArrayList<>();
        for (int i = 0; i < subIds.length; i++) {
            SubscriptionInfo info = new SubscriptionInfo(subIds[i], null, -1, null, null, -1, -1,
                    null, -1, null, null, null, null, false, null, null);
            infos.add(info);
        }
        return infos;
    }
}
