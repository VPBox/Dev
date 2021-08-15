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

package com.android.internal.telephony;


import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;
import static org.mockito.ArgumentMatchers.any;
import static org.mockito.Matchers.eq;
import static org.mockito.Mockito.doReturn;
import static org.mockito.Mockito.verify;

import android.net.Network;
import android.net.NetworkCapabilities;
import android.net.NetworkRequest;
import android.os.HandlerThread;
import android.telephony.PhoneCapability;
import android.telephony.SubscriptionManager;
import android.test.suitebuilder.annotation.SmallTest;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

public class CellularNetworkValidatorTest extends TelephonyTest {
    private boolean mValidated = false;
    private CellularNetworkValidator mValidatorUT;
    private int mValidatedSubId = SubscriptionManager.INVALID_SUBSCRIPTION_ID;
    private static final PhoneCapability CAPABILITY_WITH_VALIDATION_SUPPORTED =
            new PhoneCapability(1, 1, 0, null, true);
    private static final PhoneCapability CAPABILITY_WITHOUT_VALIDATION_SUPPORTED =
            new PhoneCapability(1, 1, 0, null, false);
    private HandlerThread mHandlerThread;

    CellularNetworkValidator.ValidationCallback mCallback = (validated, subId) -> {
        mValidated = validated;
        mValidatedSubId = subId;
    };

    @Before
    public void setUp() throws Exception {
        super.setUp(getClass().getSimpleName());

        doReturn(CAPABILITY_WITH_VALIDATION_SUPPORTED).when(mPhoneConfigurationManager)
                .getCurrentPhoneCapability();

        mHandlerThread = new HandlerThread("PhoneSwitcherTestThread") {
            @Override
            public void onLooperPrepared() {
                mValidatorUT = new CellularNetworkValidator(mContext);
            }
        };

        mHandlerThread.start();
        waitABit();

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
    public void testValidationSupported() throws Exception {
        doReturn(CAPABILITY_WITH_VALIDATION_SUPPORTED).when(mPhoneConfigurationManager)
                .getCurrentPhoneCapability();
        assertTrue(mValidatorUT.isValidationFeatureSupported());

        doReturn(CAPABILITY_WITHOUT_VALIDATION_SUPPORTED).when(mPhoneConfigurationManager)
                .getCurrentPhoneCapability();
        assertFalse(mValidatorUT.isValidationFeatureSupported());
    }

    /**
     * Test that a single phone case results in our phone being active and the RIL called
     */
    @Test
    @SmallTest
    public void testValidateSuccess() throws Exception {
        int subId = 1;
        int timeout = 1000;
        NetworkRequest expectedRequest = new NetworkRequest.Builder()
                .addCapability(NetworkCapabilities.NET_CAPABILITY_INTERNET)
                .addTransportType(NetworkCapabilities.TRANSPORT_CELLULAR)
                .setNetworkSpecifier(String.valueOf(subId))
                .build();

        mValidatorUT.validate(subId, timeout, true, mCallback);

        assertTrue(mValidatorUT.isValidating());
        assertEquals(subId, mValidatorUT.getSubIdInValidation());
        verify(mConnectivityManager).requestNetwork(
                eq(expectedRequest), eq(mValidatorUT.mNetworkCallback), any());

        mValidatorUT.mNetworkCallback.onCapabilitiesChanged(null, new NetworkCapabilities()
                .addCapability(NetworkCapabilities.NET_CAPABILITY_VALIDATED));

        assertTrue(mValidated);
        assertEquals(subId, mValidatedSubId);
        verify(mConnectivityManager).unregisterNetworkCallback(eq(mValidatorUT.mNetworkCallback));
        assertFalse(mValidatorUT.mHandler.hasCallbacks(mValidatorUT.mTimeoutCallback));
        assertFalse(mValidatorUT.isValidating());
        assertEquals(SubscriptionManager.INVALID_SUBSCRIPTION_ID,
                mValidatorUT.getSubIdInValidation());
    }

    /**
     * Test that a single phone case results in our phone being active and the RIL called
     */
    @Test
    @SmallTest
    public void testValidateTimeout() throws Exception {
        int subId = 1;
        int timeout = 100;
        NetworkRequest expectedRequest = new NetworkRequest.Builder()
                .addCapability(NetworkCapabilities.NET_CAPABILITY_INTERNET)
                .addTransportType(NetworkCapabilities.TRANSPORT_CELLULAR)
                .setNetworkSpecifier(String.valueOf(subId))
                .build();

        mValidatorUT.validate(subId, timeout, true, mCallback);

        assertTrue(mValidatorUT.isValidating());
        assertEquals(subId, mValidatorUT.getSubIdInValidation());
        verify(mConnectivityManager).requestNetwork(
                eq(expectedRequest), eq(mValidatorUT.mNetworkCallback), any());

        // Wait for timeout.
        waitABit();

        assertFalse(mValidated);
        assertEquals(subId, mValidatedSubId);
        verify(mConnectivityManager).unregisterNetworkCallback(eq(mValidatorUT.mNetworkCallback));
        assertFalse(mValidatorUT.mHandler.hasCallbacks(mValidatorUT.mTimeoutCallback));
        assertFalse(mValidatorUT.isValidating());
        assertEquals(SubscriptionManager.INVALID_SUBSCRIPTION_ID,
                mValidatorUT.getSubIdInValidation());
    }

    /**
     * Test that a single phone case results in our phone being active and the RIL called
     */
    @Test
    @SmallTest
    public void testValidateFailure() throws Exception {
        int subId = 1;
        int timeout = 100;
        NetworkRequest expectedRequest = new NetworkRequest.Builder()
                .addCapability(NetworkCapabilities.NET_CAPABILITY_INTERNET)
                .addTransportType(NetworkCapabilities.TRANSPORT_CELLULAR)
                .setNetworkSpecifier(String.valueOf(subId))
                .build();

        mValidatorUT.validate(subId, timeout, true, mCallback);

        assertTrue(mValidatorUT.isValidating());
        assertEquals(subId, mValidatorUT.getSubIdInValidation());
        verify(mConnectivityManager).requestNetwork(
                eq(expectedRequest), eq(mValidatorUT.mNetworkCallback), any());

        mValidatorUT.mNetworkCallback.onUnavailable();

        assertFalse(mValidated);
        assertEquals(subId, mValidatedSubId);
        verify(mConnectivityManager).unregisterNetworkCallback(eq(mValidatorUT.mNetworkCallback));
        assertFalse(mValidatorUT.mHandler.hasCallbacks(mValidatorUT.mTimeoutCallback));
        assertFalse(mValidatorUT.isValidating());
        assertEquals(SubscriptionManager.INVALID_SUBSCRIPTION_ID,
                mValidatorUT.getSubIdInValidation());
    }

    /**
     * Test that a single phone case results in our phone being active and the RIL called
     */
    @Test
    @SmallTest
    public void testNetworkAvailableNotValidated() throws Exception {
        int subId = 1;
        int timeout = 100;
        NetworkRequest expectedRequest = new NetworkRequest.Builder()
                .addCapability(NetworkCapabilities.NET_CAPABILITY_INTERNET)
                .addTransportType(NetworkCapabilities.TRANSPORT_CELLULAR)
                .setNetworkSpecifier(String.valueOf(subId))
                .build();

        mValidatorUT.validate(subId, timeout, true, mCallback);

        assertTrue(mValidatorUT.isValidating());
        assertEquals(subId, mValidatorUT.getSubIdInValidation());
        verify(mConnectivityManager).requestNetwork(
                eq(expectedRequest), eq(mValidatorUT.mNetworkCallback), any());

        mValidatorUT.mNetworkCallback.onAvailable(new Network(100));
        // Wait for timeout.
        waitABit();

        assertFalse(mValidated);
        assertEquals(subId, mValidatedSubId);
        verify(mConnectivityManager).unregisterNetworkCallback(eq(mValidatorUT.mNetworkCallback));
        assertFalse(mValidatorUT.mHandler.hasCallbacks(mValidatorUT.mTimeoutCallback));
        assertFalse(mValidatorUT.isValidating());
        assertEquals(SubscriptionManager.INVALID_SUBSCRIPTION_ID,
                mValidatorUT.getSubIdInValidation());
    }

    private void waitABit() {
        try {
            Thread.sleep(250);
        } catch (Exception e) {
        }
    }
}
