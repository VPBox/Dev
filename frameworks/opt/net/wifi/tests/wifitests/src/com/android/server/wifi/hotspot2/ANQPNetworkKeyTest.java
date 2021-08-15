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

package com.android.server.wifi.hotspot2;

import static org.junit.Assert.assertEquals;

import androidx.test.filters.SmallTest;

import org.junit.Test;

/**
 * Unit tests for {@link com.android.server.wifi.hotspot2.ANQPNetworkKey}.
 */
@SmallTest
public class ANQPNetworkKeyTest {
    private static final String SSID = "TestSSID";
    private static final long BSSID = 0x123456L;
    private static final long HESSID = 0x789012L;
    private static final int ANQP_DOMAIN_ID = 1;

    /**
     * Verify that building a SSID based key works as expected.
     *
     * @throws Exception
     */
    @Test
    public void buildStandardESSKey() throws Exception {
        ANQPNetworkKey expectedKey = new ANQPNetworkKey(SSID, 0, 0, ANQP_DOMAIN_ID);
        ANQPNetworkKey actualKey = ANQPNetworkKey.buildKey(SSID, BSSID, 0, ANQP_DOMAIN_ID);
        assertEquals(expectedKey, actualKey);
    }

    /**
     * Verify that building a HESSID based key works as expected.
     *
     * @throws Exception
     */
    @Test
    public void buildHessidKey() throws Exception {
        ANQPNetworkKey expectedKey = new ANQPNetworkKey(null, 0, HESSID, ANQP_DOMAIN_ID);
        ANQPNetworkKey actualKey = ANQPNetworkKey.buildKey(SSID, BSSID, HESSID, ANQP_DOMAIN_ID);
        assertEquals(expectedKey, actualKey);
    }

    /**
     * Verify that building a key based on an AP (SSID + BSSID) works as expected.
     *
     * @throws Exception
     */
    @Test
    public void buildAPKey() throws Exception {
        ANQPNetworkKey expectedKey = new ANQPNetworkKey(SSID, BSSID, 0, 0);
        ANQPNetworkKey actualKey = ANQPNetworkKey.buildKey(SSID, BSSID, HESSID, 0);
        assertEquals(expectedKey, actualKey);
    }
}
