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

import static org.junit.Assert.assertEquals;

import android.net.wifi.WifiConfiguration;

import androidx.test.filters.SmallTest;

import org.junit.Before;
import org.junit.Test;
import org.mockito.MockitoAnnotations;

/**
 * Unit tests for {@link com.android.server.wifi.WifiScoreCardProto}.
 */
@SmallTest
public class WifiScoreCardProtoTest {

    /**
     * Sets up for unit test
     */
    @Before
    public void setUp() throws Exception {
        MockitoAnnotations.initMocks(this);
    }

    /**
     * Test a builder
     */
    @Test
    public void testBuilder() throws Exception {
        WifiScoreCardProto.Network network = WifiScoreCardProto.Network.newBuilder()
                .setSsid("FloogleGuest")
                .build();
        assertEquals("FloogleGuest", network.getSsid());
    }

    /**
     * Make sure that the numeric values of SecurityType proto match
     * the @IntDef versions.
     */
    @Test
    public void testSecurityTypeValues() throws Exception {
        assertEquals(WifiConfiguration.SECURITY_TYPE_OPEN,
                WifiScoreCardProto.SecurityType.OPEN.getNumber());
        assertEquals(WifiConfiguration.SECURITY_TYPE_WEP,
                WifiScoreCardProto.SecurityType.WEP.getNumber());
        assertEquals(WifiConfiguration.SECURITY_TYPE_PSK,
                WifiScoreCardProto.SecurityType.PSK.getNumber());
        assertEquals(WifiConfiguration.SECURITY_TYPE_EAP,
                WifiScoreCardProto.SecurityType.EAP.getNumber());
        assertEquals(WifiConfiguration.SECURITY_TYPE_SAE,
                WifiScoreCardProto.SecurityType.SAE.getNumber());
        assertEquals(WifiConfiguration.SECURITY_TYPE_EAP_SUITE_B,
                WifiScoreCardProto.SecurityType.EAP_SUITE_B.getNumber());
        assertEquals(WifiConfiguration.SECURITY_TYPE_OWE,
                WifiScoreCardProto.SecurityType.OWE.getNumber());
    }

}
