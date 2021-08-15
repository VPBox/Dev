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
package com.android.internal.telephony;

import static android.telephony.PhoneStateListener.LISTEN_ACTIVE_DATA_SUBSCRIPTION_ID_CHANGE;
import static android.telephony.PhoneStateListener.LISTEN_PHONE_CAPABILITY_CHANGE;
import static android.telephony.PhoneStateListener.LISTEN_SRVCC_STATE_CHANGED;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.fail;
import static org.mockito.ArgumentMatchers.anyInt;
import static org.mockito.Mockito.doReturn;
import static org.mockito.Mockito.when;

import android.os.HandlerThread;
import android.os.ServiceManager;
import android.telephony.PhoneCapability;
import android.telephony.PhoneStateListener;
import android.telephony.TelephonyManager;
import android.test.suitebuilder.annotation.SmallTest;

import com.android.server.TelephonyRegistry;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.mockito.Mock;

public class TelephonyRegistryTest extends TelephonyTest {
    @Mock
    private ISub.Stub mISubStub;
    private PhoneStateListener mPhoneStateListener;
    private TelephonyRegistry mTelephonyRegistry;
    private PhoneCapability mPhoneCapability;
    private int mActiveSubId;
    private int mSrvccState = -1;

    public class PhoneStateListenerWrapper extends PhoneStateListener {
        @Override
        public void onSrvccStateChanged(int srvccState) {
            mSrvccState = srvccState;
            setReady(true);
        }

        @Override
        public void onPhoneCapabilityChanged(PhoneCapability capability) {
            mPhoneCapability = capability;
            setReady(true);
        }
        @Override
        public void onActiveDataSubscriptionIdChanged(int activeSubId) {
            mActiveSubId = activeSubId;
            setReady(true);
        }
    }

    private void addTelephonyRegistryService() {
        mServiceManagerMockedServices.put("telephony.registry", mTelephonyRegistry.asBinder());
    }

    private HandlerThread mHandlerThread = new HandlerThread("ListenerThread") {
        @Override
        public void onLooperPrepared() {
            mTelephonyRegistry = new TelephonyRegistry(mContext);
            addTelephonyRegistryService();
            mPhoneStateListener = new PhoneStateListenerWrapper();
            setReady(true);
        }
    };

    @Before
    public void setUp() throws Exception {
        super.setUp("TelephonyRegistryTest");
        // ServiceManager.getService("isub") will return this stub for any call to
        // SubscriptionManager.
        mServiceManagerMockedServices.put("isub", mISubStub);
        mHandlerThread.start();
        waitUntilReady();
        assertEquals(mTelephonyRegistry.asBinder(),
                ServiceManager.getService("telephony.registry"));
    }

    @After
    public void tearDown() throws Exception {
        mTelephonyRegistry = null;
        mHandlerThread.quit();
        super.tearDown();
    }

    @Test @SmallTest
    public void testPhoneCapabilityChanged() {
        // mTelephonyRegistry.listen with notifyNow = true should trigger callback immediately.
        setReady(false);
        PhoneCapability phoneCapability = new PhoneCapability(1, 2, 3, null, false);
        mTelephonyRegistry.notifyPhoneCapabilityChanged(phoneCapability);
        mTelephonyRegistry.listen(mContext.getOpPackageName(),
                mPhoneStateListener.callback,
                LISTEN_PHONE_CAPABILITY_CHANGE, true);
        waitUntilReady();
        assertEquals(phoneCapability, mPhoneCapability);

        // notifyPhoneCapabilityChanged with a new capability. Callback should be triggered.
        setReady(false);
        phoneCapability = new PhoneCapability(3, 2, 2, null, false);
        mTelephonyRegistry.notifyPhoneCapabilityChanged(phoneCapability);
        waitUntilReady();
        assertEquals(phoneCapability, mPhoneCapability);
    }


    @Test @SmallTest
    public void testActiveDataSubChanged() {
        // mTelephonyRegistry.listen with notifyNow = true should trigger callback immediately.
        setReady(false);
        int[] activeSubs = {0, 1, 2};
        when(mSubscriptionManager.getActiveSubscriptionIdList()).thenReturn(activeSubs);
        int activeSubId = 0;
        mTelephonyRegistry.notifyActiveDataSubIdChanged(activeSubId);
        mTelephonyRegistry.listen(mContext.getOpPackageName(),
                mPhoneStateListener.callback,
                LISTEN_ACTIVE_DATA_SUBSCRIPTION_ID_CHANGE, true);
        waitUntilReady();
        assertEquals(activeSubId, mActiveSubId);

        // notifyPhoneCapabilityChanged with a new capability. Callback should be triggered.
        setReady(false);
        mActiveSubId = 1;
        mTelephonyRegistry.notifyActiveDataSubIdChanged(activeSubId);
        waitUntilReady();
        assertEquals(activeSubId, mActiveSubId);
    }

    /**
     * Test that we first receive a callback when listen(...) is called that contains the latest
     * notify(...) response and then that the callback is called correctly when notify(...) is
     * called.
     */
    @Test
    @SmallTest
    public void testSrvccStateChanged() throws Exception {
        // Return a phone ID of 0 for all sub ids given.
        doReturn(0/*phoneId*/).when(mISubStub).getPhoneId(anyInt());
        setReady(false);
        int srvccState = TelephonyManager.SRVCC_STATE_HANDOVER_STARTED;
        mTelephonyRegistry.notifySrvccStateChanged(0 /*subId*/, srvccState);
        // Should receive callback when listen is called that contains the latest notify result.
        mTelephonyRegistry.listenForSubscriber(0 /*subId*/, mContext.getOpPackageName(),
                mPhoneStateListener.callback,
                LISTEN_SRVCC_STATE_CHANGED, true);
        waitUntilReady();
        assertEquals(srvccState, mSrvccState);

        // trigger callback
        setReady(false);
        srvccState = TelephonyManager.SRVCC_STATE_HANDOVER_COMPLETED;
        mTelephonyRegistry.notifySrvccStateChanged(0 /*subId*/, srvccState);
        waitUntilReady();
        assertEquals(srvccState, mSrvccState);
    }

    /**
     * Test that a SecurityException is thrown when we try to listen to a SRVCC state change without
     * READ_PRIVILEGED_PHONE_STATE.
     */
    @Test
    @SmallTest
    public void testSrvccStateChangedNoPermission() {
        // Clear all permission grants for test.
        mContextFixture.addCallingOrSelfPermission("");
        int srvccState = TelephonyManager.SRVCC_STATE_HANDOVER_STARTED;
        mTelephonyRegistry.notifySrvccStateChanged(0 /*subId*/, srvccState);
        try {
            mTelephonyRegistry.listenForSubscriber(0 /*subId*/, mContext.getOpPackageName(),
                    mPhoneStateListener.callback,
                    LISTEN_SRVCC_STATE_CHANGED, true);
            fail();
        } catch (SecurityException e) {
            // pass test!
        }
    }
}
