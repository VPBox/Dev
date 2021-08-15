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

package com.android.ike.ikev2;

import static org.junit.Assert.assertArrayEquals;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.fail;

import android.content.Context;
import android.net.IpSecManager;
import android.net.IpSecManager.UdpEncapsulationSocket;

import androidx.test.InstrumentationRegistry;

import libcore.net.InetAddressUtils;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import java.net.Inet4Address;

public final class IkeSessionOptionsTest {
    private static final Inet4Address IPV4_ADDRESS =
            (Inet4Address) (InetAddressUtils.parseNumericAddress("192.0.2.100"));

    private UdpEncapsulationSocket mUdpEncapSocket;

    @Before
    public void setUp() throws Exception {
        Context context = InstrumentationRegistry.getContext();
        IpSecManager ipSecManager = (IpSecManager) context.getSystemService(Context.IPSEC_SERVICE);
        mUdpEncapSocket = ipSecManager.openUdpEncapsulationSocket();
    }

    @After
    public void tearDown() throws Exception {
        mUdpEncapSocket.close();
    }

    @Test
    public void testBuild() throws Exception {
        SaProposal saProposal =
                SaProposal.Builder.newIkeSaProposalBuilder()
                        .addEncryptionAlgorithm(
                                SaProposal.ENCRYPTION_ALGORITHM_AES_GCM_8,
                                SaProposal.KEY_LEN_AES_128)
                        .addPseudorandomFunction(SaProposal.PSEUDORANDOM_FUNCTION_AES128_XCBC)
                        .addDhGroup(SaProposal.DH_GROUP_1024_BIT_MODP)
                        .build();

        IkeSessionOptions sessionOptions =
                new IkeSessionOptions.Builder(IPV4_ADDRESS, mUdpEncapSocket)
                        .addSaProposal(saProposal)
                        .build();

        assertEquals(IPV4_ADDRESS, sessionOptions.getServerAddress());
        assertEquals(mUdpEncapSocket, sessionOptions.getUdpEncapsulationSocket());
        assertArrayEquals(new SaProposal[] {saProposal}, sessionOptions.getSaProposals());
        assertFalse(sessionOptions.isIkeFragmentationSupported());
    }

    @Test
    public void testBuildWithoutSaProposal() throws Exception {
        try {
            new IkeSessionOptions.Builder(IPV4_ADDRESS, mUdpEncapSocket).build();
            fail("Expected to fail due to absence of SA proposal.");
        } catch (IllegalArgumentException expected) {
        }
    }

    @Test
    public void testBuildWithChildSaProposal() throws Exception {
        SaProposal saProposal =
                SaProposal.Builder.newChildSaProposalBuilder(true)
                        .addEncryptionAlgorithm(
                                SaProposal.ENCRYPTION_ALGORITHM_AES_GCM_8,
                                SaProposal.KEY_LEN_AES_128)
                        .build();
        try {
            new IkeSessionOptions.Builder(IPV4_ADDRESS, mUdpEncapSocket)
                    .addSaProposal(saProposal)
                    .build();
            fail("Expected to fail due to wrong type of SA proposal.");
        } catch (IllegalArgumentException expected) {
        }
    }
}
