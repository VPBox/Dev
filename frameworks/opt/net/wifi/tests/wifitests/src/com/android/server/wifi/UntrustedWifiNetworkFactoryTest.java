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

package com.android.server.wifi;

import static org.junit.Assert.*;
import static org.mockito.Mockito.*;

import android.content.Context;
import android.net.NetworkCapabilities;
import android.net.NetworkRequest;
import android.os.test.TestLooper;
import android.test.suitebuilder.annotation.SmallTest;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.mockito.Mock;
import org.mockito.MockitoAnnotations;

/**
 * Unit tests for {@link com.android.server.wifi.UntrustedWifiNetworkFactory}.
 */
@SmallTest
public class UntrustedWifiNetworkFactoryTest {
    @Mock WifiConnectivityManager mWifiConnectivityManager;
    @Mock Context mContext;
    NetworkCapabilities mNetworkCapabilities;
    TestLooper mLooper;
    NetworkRequest mNetworkRequest;

    private UntrustedWifiNetworkFactory mUntrustedWifiNetworkFactory;

    /**
     * Setup the mocks.
     */
    @Before
    public void setUp() throws Exception {
        MockitoAnnotations.initMocks(this);

        mLooper = new TestLooper();
        mNetworkCapabilities = new NetworkCapabilities();
        mNetworkCapabilities.addTransportType(NetworkCapabilities.TRANSPORT_WIFI);
        mNetworkCapabilities.removeCapability(NetworkCapabilities.NET_CAPABILITY_TRUSTED);

        mUntrustedWifiNetworkFactory = new UntrustedWifiNetworkFactory(
                mLooper.getLooper(), mContext,
                mNetworkCapabilities, mWifiConnectivityManager);

        mNetworkRequest = new NetworkRequest.Builder()
                .setCapabilities(mNetworkCapabilities)
                .build();
    }

    /**
     * Called after each test
     */
    @After
    public void cleanup() {
        validateMockitoUsage();
    }

    /**
     * Validates handling of needNetworkFor.
     */
    @Test
    public void testHandleNetworkRequest() {
        assertFalse(mUntrustedWifiNetworkFactory.hasConnectionRequests());
        mUntrustedWifiNetworkFactory.needNetworkFor(mNetworkRequest, 0);

        // First network request should turn on auto-join.
        verify(mWifiConnectivityManager).setUntrustedConnectionAllowed(true);
        assertTrue(mUntrustedWifiNetworkFactory.hasConnectionRequests());

        // Subsequent ones should do nothing.
        mUntrustedWifiNetworkFactory.needNetworkFor(mNetworkRequest, 0);
        verifyNoMoreInteractions(mWifiConnectivityManager);
    }

    /**
     * Validates handling of releaseNetwork.
     */
    @Test
    public void testHandleNetworkRelease() {
        // Release network without a corresponding request should be ignored.
        mUntrustedWifiNetworkFactory.releaseNetworkFor(mNetworkRequest);
        assertFalse(mUntrustedWifiNetworkFactory.hasConnectionRequests());

        // Now request & then release the network request
        mUntrustedWifiNetworkFactory.needNetworkFor(mNetworkRequest, 0);
        assertTrue(mUntrustedWifiNetworkFactory.hasConnectionRequests());
        verify(mWifiConnectivityManager).setUntrustedConnectionAllowed(true);

        mUntrustedWifiNetworkFactory.releaseNetworkFor(mNetworkRequest);
        assertFalse(mUntrustedWifiNetworkFactory.hasConnectionRequests());
        verify(mWifiConnectivityManager).setUntrustedConnectionAllowed(false);
    }
}
