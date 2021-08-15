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

package com.android.server.wifi.util;

import static org.junit.Assert.*;
import static org.mockito.Mockito.*;

import android.net.wifi.WifiConfiguration;
import android.net.wifi.WifiEnterpriseConfig;
import android.telephony.ImsiEncryptionInfo;
import android.telephony.TelephonyManager;
import android.util.Base64;
import android.util.Pair;

import androidx.test.filters.SmallTest;

import com.android.dx.mockito.inline.extended.ExtendedMockito;
import com.android.server.wifi.CarrierNetworkConfig;
import com.android.server.wifi.WifiConfigurationTestUtil;
import com.android.server.wifi.util.TelephonyUtil.SimAuthRequestData;
import com.android.server.wifi.util.TelephonyUtil.SimAuthResponseData;

import org.junit.Before;
import org.junit.Test;
import org.mockito.Mock;
import org.mockito.MockitoAnnotations;
import org.mockito.MockitoSession;

import java.security.PublicKey;

import javax.crypto.BadPaddingException;
import javax.crypto.Cipher;

/**
 * Unit tests for {@link com.android.server.wifi.util.TelephonyUtil}.
 */
@SmallTest
public class TelephonyUtilTest {
    private TelephonyUtil mTelephonyUtil;

    @Mock
    CarrierNetworkConfig mCarrierNetworkConfig;
    @Mock
    TelephonyManager mTelephonyManager;
    @Mock
    TelephonyManager mDataTelephonyManager;

    @Before
    public void setUp() throws Exception {
        MockitoAnnotations.initMocks(this);
        mTelephonyUtil = new TelephonyUtil();
        when(mTelephonyManager.createForSubscriptionId(anyInt())).thenReturn(mDataTelephonyManager);
    }

    @Test
    public void getSimIdentityEapSim() {
        final Pair<String, String> expectedIdentity = Pair.create(
                "13214561234567890@wlan.mnc456.mcc321.3gppnetwork.org", "");

        when(mDataTelephonyManager.getSubscriberId()).thenReturn("3214561234567890");
        when(mDataTelephonyManager.getSimState()).thenReturn(TelephonyManager.SIM_STATE_READY);
        when(mDataTelephonyManager.getSimOperator()).thenReturn("321456");
        when(mDataTelephonyManager.getCarrierInfoForImsiEncryption(anyInt())).thenReturn(null);

        assertEquals(expectedIdentity, TelephonyUtil.getSimIdentity(mTelephonyManager,
                mTelephonyUtil,
                WifiConfigurationTestUtil.createEapNetwork(WifiEnterpriseConfig.Eap.SIM,
                        WifiEnterpriseConfig.Phase2.NONE), mCarrierNetworkConfig));
        assertEquals(expectedIdentity,
                TelephonyUtil.getSimIdentity(mTelephonyManager, mTelephonyUtil,
                        WifiConfigurationTestUtil.createEapNetwork(WifiEnterpriseConfig.Eap.PEAP,
                                WifiEnterpriseConfig.Phase2.SIM), mCarrierNetworkConfig));
    }

    @Test
    public void getSimIdentityEapAka() {
        final Pair<String, String> expectedIdentity = Pair.create(
                "03214561234567890@wlan.mnc456.mcc321.3gppnetwork.org", "");
        when(mDataTelephonyManager.getSubscriberId()).thenReturn("3214561234567890");

        when(mDataTelephonyManager.getSimState()).thenReturn(TelephonyManager.SIM_STATE_READY);
        when(mDataTelephonyManager.getSimOperator()).thenReturn("321456");
        when(mDataTelephonyManager.getCarrierInfoForImsiEncryption(anyInt())).thenReturn(null);

        assertEquals(expectedIdentity,
                TelephonyUtil.getSimIdentity(mTelephonyManager, mTelephonyUtil,
                        WifiConfigurationTestUtil.createEapNetwork(WifiEnterpriseConfig.Eap.AKA,
                                WifiEnterpriseConfig.Phase2.NONE), mCarrierNetworkConfig));
        assertEquals(expectedIdentity,
                TelephonyUtil.getSimIdentity(mTelephonyManager, mTelephonyUtil,
                        WifiConfigurationTestUtil.createEapNetwork(WifiEnterpriseConfig.Eap.PEAP,
                                WifiEnterpriseConfig.Phase2.AKA), mCarrierNetworkConfig));
    }

    @Test
    public void getSimIdentityEapAkaPrime() {
        final Pair<String, String> expectedIdentity = Pair.create(
                "63214561234567890@wlan.mnc456.mcc321.3gppnetwork.org", "");

        when(mDataTelephonyManager.getSubscriberId()).thenReturn("3214561234567890");
        when(mDataTelephonyManager.getSimState()).thenReturn(TelephonyManager.SIM_STATE_READY);
        when(mDataTelephonyManager.getSimOperator()).thenReturn("321456");
        when(mDataTelephonyManager.getCarrierInfoForImsiEncryption(anyInt())).thenReturn(null);

        assertEquals(expectedIdentity,
                TelephonyUtil.getSimIdentity(mTelephonyManager, mTelephonyUtil,
                        WifiConfigurationTestUtil.createEapNetwork(
                                WifiEnterpriseConfig.Eap.AKA_PRIME,
                                WifiEnterpriseConfig.Phase2.NONE), mCarrierNetworkConfig));
        assertEquals(expectedIdentity,
                TelephonyUtil.getSimIdentity(mTelephonyManager, mTelephonyUtil,
                        WifiConfigurationTestUtil.createEapNetwork(WifiEnterpriseConfig.Eap.PEAP,
                                WifiEnterpriseConfig.Phase2.AKA_PRIME), mCarrierNetworkConfig));
    }

    /**
     * Verify that an expected identity is returned when using the encrypted identity
     * encoded by RFC4648.
     */
    @Test
    public void getEncryptedIdentity_WithRfc4648() throws Exception {
        Cipher cipher = mock(Cipher.class);
        PublicKey key = null;
        String imsi = "3214561234567890";
        String permanentIdentity = "03214561234567890@wlan.mnc456.mcc321.3gppnetwork.org";
        String encryptedImsi = Base64.encodeToString(permanentIdentity.getBytes(), 0,
                permanentIdentity.getBytes().length, Base64.NO_WRAP);
        String encryptedIdentity = "\0" + encryptedImsi;
        final Pair<String, String> expectedIdentity = Pair.create(permanentIdentity,
                encryptedIdentity);

        // static mocking
        MockitoSession session = ExtendedMockito.mockitoSession().mockStatic(
                Cipher.class).startMocking();
        try {
            when(Cipher.getInstance(anyString())).thenReturn(cipher);
            when(cipher.doFinal(any(byte[].class))).thenReturn(permanentIdentity.getBytes());
            when(mDataTelephonyManager.getSubscriberId()).thenReturn(imsi);
            when(mDataTelephonyManager.getSimState()).thenReturn(TelephonyManager.SIM_STATE_READY);
            when(mDataTelephonyManager.getSimOperator()).thenReturn("321456");
            ImsiEncryptionInfo info = new ImsiEncryptionInfo("321", "456",
                    TelephonyManager.KEY_TYPE_WLAN, null, key, null);
            when(mDataTelephonyManager.getCarrierInfoForImsiEncryption(
                    eq(TelephonyManager.KEY_TYPE_WLAN)))
                    .thenReturn(info);

            assertEquals(expectedIdentity,
                    TelephonyUtil.getSimIdentity(mTelephonyManager, mTelephonyUtil,
                            WifiConfigurationTestUtil.createEapNetwork(WifiEnterpriseConfig.Eap.AKA,
                                    WifiEnterpriseConfig.Phase2.NONE), mCarrierNetworkConfig));
        } finally {
            session.finishMocking();
        }
    }

    /**
     * Verify that {@code null} will be returned when IMSI encryption failed.
     *
     * @throws Exception
     */
    @Test
    public void getEncryptedIdentityFailed() throws Exception {
        Cipher cipher = mock(Cipher.class);
        String keyIdentifier = "key=testKey";
        String imsi = "3214561234567890";
        // static mocking
        MockitoSession session = ExtendedMockito.mockitoSession().mockStatic(
                Cipher.class).startMocking();
        try {
            when(Cipher.getInstance(anyString())).thenReturn(cipher);
            when(cipher.doFinal(any(byte[].class))).thenThrow(BadPaddingException.class);
            when(mDataTelephonyManager.getSubscriberId()).thenReturn(imsi);
            when(mDataTelephonyManager.getSimState()).thenReturn(TelephonyManager.SIM_STATE_READY);
            when(mDataTelephonyManager.getSimOperator()).thenReturn("321456");
            ImsiEncryptionInfo info = new ImsiEncryptionInfo("321", "456",
                    TelephonyManager.KEY_TYPE_WLAN, keyIdentifier, (PublicKey) null, null);
            when(mDataTelephonyManager.getCarrierInfoForImsiEncryption(
                    eq(TelephonyManager.KEY_TYPE_WLAN)))
                    .thenReturn(info);

            assertNull(TelephonyUtil.getSimIdentity(mTelephonyManager, mTelephonyUtil,
                    WifiConfigurationTestUtil.createEapNetwork(WifiEnterpriseConfig.Eap.AKA,
                            WifiEnterpriseConfig.Phase2.NONE), mCarrierNetworkConfig));
        } finally {
            session.finishMocking();
        }
    }

    @Test
    public void getSimIdentity2DigitMnc() {
        final Pair<String, String> expectedIdentity = Pair.create(
                "1321560123456789@wlan.mnc056.mcc321.3gppnetwork.org", "");

        when(mDataTelephonyManager.getSubscriberId()).thenReturn("321560123456789");
        when(mDataTelephonyManager.getSimState()).thenReturn(TelephonyManager.SIM_STATE_READY);
        when(mDataTelephonyManager.getSimOperator()).thenReturn("32156");
        when(mDataTelephonyManager.getCarrierInfoForImsiEncryption(anyInt())).thenReturn(null);

        assertEquals(expectedIdentity,
                TelephonyUtil.getSimIdentity(mTelephonyManager, mTelephonyUtil,
                        WifiConfigurationTestUtil.createEapNetwork(WifiEnterpriseConfig.Eap.SIM,
                                WifiEnterpriseConfig.Phase2.NONE), mCarrierNetworkConfig));
    }

    @Test
    public void getSimIdentityUnknownMccMnc() {
        final Pair<String, String> expectedIdentity = Pair.create(
                "13214560123456789@wlan.mnc456.mcc321.3gppnetwork.org", "");

        when(mDataTelephonyManager.getSubscriberId()).thenReturn("3214560123456789");
        when(mDataTelephonyManager.getSimState()).thenReturn(TelephonyManager.SIM_STATE_UNKNOWN);
        when(mDataTelephonyManager.getSimOperator()).thenReturn(null);
        when(mDataTelephonyManager.getCarrierInfoForImsiEncryption(anyInt())).thenReturn(null);

        assertEquals(expectedIdentity,
                TelephonyUtil.getSimIdentity(mTelephonyManager, mTelephonyUtil,
                        WifiConfigurationTestUtil.createEapNetwork(WifiEnterpriseConfig.Eap.SIM,
                                WifiEnterpriseConfig.Phase2.NONE), mCarrierNetworkConfig));
    }

    @Test
    public void getSimIdentityWithNoTelephonyManager() {
        assertEquals(null, TelephonyUtil.getSimIdentity(null, null,
                WifiConfigurationTestUtil.createEapNetwork(
                        WifiEnterpriseConfig.Eap.SIM, WifiEnterpriseConfig.Phase2.NONE),
                mCarrierNetworkConfig));
    }

    @Test
    public void getSimIdentityNonTelephonyConfig() {
        when(mDataTelephonyManager.getSubscriberId()).thenReturn("321560123456789");
        when(mDataTelephonyManager.getSimState()).thenReturn(TelephonyManager.SIM_STATE_READY);
        when(mDataTelephonyManager.getSimOperator()).thenReturn("32156");
        assertEquals(null, TelephonyUtil.getSimIdentity(mTelephonyManager, mTelephonyUtil,
                WifiConfigurationTestUtil.createEapNetwork(
                        WifiEnterpriseConfig.Eap.TTLS, WifiEnterpriseConfig.Phase2.SIM),
                mCarrierNetworkConfig));
        assertEquals(null, TelephonyUtil.getSimIdentity(mTelephonyManager, mTelephonyUtil,
                WifiConfigurationTestUtil.createEapNetwork(
                        WifiEnterpriseConfig.Eap.PEAP, WifiEnterpriseConfig.Phase2.MSCHAPV2),
                mCarrierNetworkConfig));
        assertEquals(null, TelephonyUtil.getSimIdentity(mTelephonyManager, mTelephonyUtil,
                WifiConfigurationTestUtil.createEapNetwork(
                        WifiEnterpriseConfig.Eap.TLS, WifiEnterpriseConfig.Phase2.NONE),
                mCarrierNetworkConfig));
        assertEquals(null, TelephonyUtil.getSimIdentity(
                mTelephonyManager, mTelephonyUtil, new WifiConfiguration(), mCarrierNetworkConfig));
    }

    @Test
    public void isSimConfig() {
        assertFalse(TelephonyUtil.isSimConfig(null));
        assertFalse(TelephonyUtil.isSimConfig(new WifiConfiguration()));
        assertFalse(TelephonyUtil.isSimConfig(WifiConfigurationTestUtil.createOpenNetwork()));
        assertFalse(TelephonyUtil.isSimConfig(WifiConfigurationTestUtil.createWepNetwork()));
        assertFalse(TelephonyUtil.isSimConfig(WifiConfigurationTestUtil.createPskNetwork()));
        assertFalse(TelephonyUtil.isSimConfig(WifiConfigurationTestUtil.createEapNetwork(
                WifiEnterpriseConfig.Eap.TTLS, WifiEnterpriseConfig.Phase2.SIM)));
        assertFalse(TelephonyUtil.isSimConfig(WifiConfigurationTestUtil.createEapNetwork(
                WifiEnterpriseConfig.Eap.TLS, WifiEnterpriseConfig.Phase2.NONE)));
        assertFalse(TelephonyUtil.isSimConfig(WifiConfigurationTestUtil.createEapNetwork(
                WifiEnterpriseConfig.Eap.PEAP, WifiEnterpriseConfig.Phase2.MSCHAPV2)));
        assertTrue(TelephonyUtil.isSimConfig(WifiConfigurationTestUtil.createEapNetwork(
                WifiEnterpriseConfig.Eap.SIM, WifiEnterpriseConfig.Phase2.NONE)));
        assertTrue(TelephonyUtil.isSimConfig(WifiConfigurationTestUtil.createEapNetwork(
                WifiEnterpriseConfig.Eap.AKA, WifiEnterpriseConfig.Phase2.NONE)));
        assertTrue(TelephonyUtil.isSimConfig(WifiConfigurationTestUtil.createEapNetwork(
                WifiEnterpriseConfig.Eap.AKA_PRIME, WifiEnterpriseConfig.Phase2.NONE)));
        assertTrue(TelephonyUtil.isSimConfig(WifiConfigurationTestUtil.createEapNetwork(
                WifiEnterpriseConfig.Eap.PEAP, WifiEnterpriseConfig.Phase2.SIM)));
        assertTrue(TelephonyUtil.isSimConfig(WifiConfigurationTestUtil.createEapNetwork(
                WifiEnterpriseConfig.Eap.PEAP, WifiEnterpriseConfig.Phase2.AKA)));
        assertTrue(TelephonyUtil.isSimConfig(WifiConfigurationTestUtil.createEapNetwork(
                WifiEnterpriseConfig.Eap.PEAP, WifiEnterpriseConfig.Phase2.AKA_PRIME)));
    }

    /**
     * Produce a base64 encoded length byte + data.
     */
    private static String createSimChallengeRequest(byte[] challengeValue) {
        byte[] challengeLengthAndValue = new byte[challengeValue.length + 1];
        challengeLengthAndValue[0] = (byte) challengeValue.length;
        for (int i = 0; i < challengeValue.length; ++i) {
            challengeLengthAndValue[i + 1] = challengeValue[i];
        }
        return Base64.encodeToString(challengeLengthAndValue, android.util.Base64.NO_WRAP);
    }

    /**
     * Produce a base64 encoded data without length.
     */
    private static String create2gUsimChallengeRequest(byte[] challengeValue) {
        return Base64.encodeToString(challengeValue, android.util.Base64.NO_WRAP);
    }

    /**
     * Produce a base64 encoded sres length byte + sres + kc length byte + kc.
     */
    private static String createGsmSimAuthResponse(byte[] sresValue, byte[] kcValue) {
        int overallLength = sresValue.length + kcValue.length + 2;
        byte[] result = new byte[sresValue.length + kcValue.length + 2];
        int idx = 0;
        result[idx++] = (byte) sresValue.length;
        for (int i = 0; i < sresValue.length; ++i) {
            result[idx++] = sresValue[i];
        }
        result[idx++] = (byte) kcValue.length;
        for (int i = 0; i < kcValue.length; ++i) {
            result[idx++] = kcValue[i];
        }
        return Base64.encodeToString(result, Base64.NO_WRAP);
    }

    /**
     * Produce a base64 encoded sres + kc without length.
     */
    private static String create2gUsimAuthResponse(byte[] sresValue, byte[] kcValue) {
        int overallLength = sresValue.length + kcValue.length;
        byte[] result = new byte[sresValue.length + kcValue.length];
        int idx = 0;
        for (int i = 0; i < sresValue.length; ++i) {
            result[idx++] = sresValue[i];
        }
        for (int i = 0; i < kcValue.length; ++i) {
            result[idx++] = kcValue[i];
        }
        return Base64.encodeToString(result, Base64.NO_WRAP);
    }

    @Test
    public void getGsmSimAuthResponseInvalidRequest() {
        final String[] invalidRequests = { null, "", "XXXX" };
        assertEquals("", TelephonyUtil.getGsmSimAuthResponse(invalidRequests, mTelephonyManager));
    }

    @Test
    public void getGsmSimAuthResponseFailedSimResponse() {
        final String[] failedRequests = { "5E5F" };
        when(mDataTelephonyManager.getIccAuthentication(anyInt(), anyInt(),
                eq(createSimChallengeRequest(new byte[] { 0x5e, 0x5f })))).thenReturn(null);

        assertEquals(null, TelephonyUtil.getGsmSimAuthResponse(failedRequests, mTelephonyManager));
    }

    @Test
    public void getGsmSimAuthResponseUsim() {
        when(mDataTelephonyManager.getIccAuthentication(TelephonyManager.APPTYPE_USIM,
                        TelephonyManager.AUTHTYPE_EAP_SIM,
                        createSimChallengeRequest(new byte[] { 0x1b, 0x2b })))
                .thenReturn(createGsmSimAuthResponse(new byte[] { 0x1D, 0x2C },
                                new byte[] { 0x3B, 0x4A }));
        when(mDataTelephonyManager.getIccAuthentication(TelephonyManager.APPTYPE_USIM,
                        TelephonyManager.AUTHTYPE_EAP_SIM,
                        createSimChallengeRequest(new byte[] { 0x01, 0x22 })))
                .thenReturn(createGsmSimAuthResponse(new byte[] { 0x11, 0x11 },
                                new byte[] { 0x12, 0x34 }));

        assertEquals(":3b4a:1d2c:1234:1111", TelephonyUtil.getGsmSimAuthResponse(
                        new String[] { "1B2B", "0122" }, mTelephonyManager));
    }

    @Test
    public void getGsmSimpleSimAuthResponseInvalidRequest() {
        final String[] invalidRequests = { null, "", "XXXX" };
        assertEquals("",
                TelephonyUtil.getGsmSimpleSimAuthResponse(invalidRequests, mTelephonyManager));
    }

    @Test
    public void getGsmSimpleSimAuthResponseFailedSimResponse() {
        final String[] failedRequests = { "5E5F" };
        when(mDataTelephonyManager.getIccAuthentication(anyInt(), anyInt(),
                eq(createSimChallengeRequest(new byte[] { 0x5e, 0x5f })))).thenReturn(null);

        assertEquals(null,
                TelephonyUtil.getGsmSimpleSimAuthResponse(failedRequests, mTelephonyManager));
    }

    @Test
    public void getGsmSimpleSimAuthResponse() {
        when(mDataTelephonyManager.getIccAuthentication(TelephonyManager.APPTYPE_SIM,
                        TelephonyManager.AUTHTYPE_EAP_SIM,
                        createSimChallengeRequest(new byte[] { 0x1a, 0x2b })))
                .thenReturn(createGsmSimAuthResponse(new byte[] { 0x1D, 0x2C },
                                new byte[] { 0x3B, 0x4A }));
        when(mDataTelephonyManager.getIccAuthentication(TelephonyManager.APPTYPE_SIM,
                        TelephonyManager.AUTHTYPE_EAP_SIM,
                        createSimChallengeRequest(new byte[] { 0x01, 0x23 })))
                .thenReturn(createGsmSimAuthResponse(new byte[] { 0x33, 0x22 },
                                new byte[] { 0x11, 0x00 }));

        assertEquals(":3b4a:1d2c:1100:3322", TelephonyUtil.getGsmSimpleSimAuthResponse(
                        new String[] { "1A2B", "0123" }, mTelephonyManager));
    }

    @Test
    public void getGsmSimpleSimNoLengthAuthResponseInvalidRequest() {
        final String[] invalidRequests = { null, "", "XXXX" };
        assertEquals("", TelephonyUtil.getGsmSimpleSimNoLengthAuthResponse(invalidRequests,
                mTelephonyManager));
    }

    @Test
    public void getGsmSimpleSimNoLengthAuthResponseFailedSimResponse() {
        final String[] failedRequests = { "5E5F" };
        when(mDataTelephonyManager.getIccAuthentication(anyInt(), anyInt(),
                eq(create2gUsimChallengeRequest(new byte[] { 0x5e, 0x5f })))).thenReturn(null);

        assertEquals(null, TelephonyUtil.getGsmSimpleSimNoLengthAuthResponse(failedRequests,
                mTelephonyManager));
    }

    @Test
    public void getGsmSimpleSimNoLengthAuthResponse() {
        when(mDataTelephonyManager.getIccAuthentication(TelephonyManager.APPTYPE_SIM,
                        TelephonyManager.AUTHTYPE_EAP_SIM,
                        create2gUsimChallengeRequest(new byte[] { 0x1a, 0x2b })))
                .thenReturn(create2gUsimAuthResponse(new byte[] { 0x1a, 0x2b, 0x3c, 0x4d },
                                new byte[] { 0x1a, 0x2b, 0x3c, 0x4d, 0x5e, 0x6f, 0x7a, 0x1a }));
        when(mDataTelephonyManager.getIccAuthentication(TelephonyManager.APPTYPE_SIM,
                        TelephonyManager.AUTHTYPE_EAP_SIM,
                        create2gUsimChallengeRequest(new byte[] { 0x01, 0x23 })))
                .thenReturn(create2gUsimAuthResponse(new byte[] { 0x12, 0x34, 0x56, 0x78 },
                                new byte[] { 0x12, 0x34, 0x56, 0x78, 0x12, 0x34, 0x56, 0x78 }));

        assertEquals(":1a2b3c4d5e6f7a1a:1a2b3c4d:1234567812345678:12345678",
                TelephonyUtil.getGsmSimpleSimNoLengthAuthResponse(
                        new String[] { "1A2B", "0123" }, mTelephonyManager));
    }

    /**
     * Produce a base64 encoded tag + res length byte + res + ck length byte + ck + ik length byte +
     * ik.
     */
    private static String create3GSimAuthUmtsAuthResponse(byte[] res, byte[] ck, byte[] ik) {
        byte[] result = new byte[res.length + ck.length + ik.length + 4];
        int idx = 0;
        result[idx++] = (byte) 0xdb;
        result[idx++] = (byte) res.length;
        for (int i = 0; i < res.length; ++i) {
            result[idx++] = res[i];
        }
        result[idx++] = (byte) ck.length;
        for (int i = 0; i < ck.length; ++i) {
            result[idx++] = ck[i];
        }
        result[idx++] = (byte) ik.length;
        for (int i = 0; i < ik.length; ++i) {
            result[idx++] = ik[i];
        }
        return Base64.encodeToString(result, Base64.NO_WRAP);
    }

    private static String create3GSimAuthUmtsAutsResponse(byte[] auts) {
        byte[] result = new byte[auts.length + 2];
        int idx = 0;
        result[idx++] = (byte) 0xdc;
        result[idx++] = (byte) auts.length;
        for (int i = 0; i < auts.length; ++i) {
            result[idx++] = auts[i];
        }
        return Base64.encodeToString(result, Base64.NO_WRAP);
    }

    @Test
    public void get3GAuthResponseInvalidRequest() {
        assertEquals(null, TelephonyUtil.get3GAuthResponse(
                new SimAuthRequestData(0, 0, "SSID", new String[]{"0123"}), mTelephonyManager));
        assertEquals(null, TelephonyUtil.get3GAuthResponse(
                new SimAuthRequestData(0, 0, "SSID", new String[]{"xyz2", "1234"}),
                mTelephonyManager));
        verifyNoMoreInteractions(mDataTelephonyManager);
    }

    @Test
    public void get3GAuthResponseNullIccAuthentication() {
        when(mDataTelephonyManager.getIccAuthentication(TelephonyManager.APPTYPE_USIM,
                        TelephonyManager.AUTHTYPE_EAP_AKA, "AgEjAkVn")).thenReturn(null);

        SimAuthResponseData response = TelephonyUtil.get3GAuthResponse(
                new SimAuthRequestData(0, 0, "SSID", new String[]{"0123", "4567"}),
                mTelephonyManager);
        assertNull(response);
    }

    @Test
    public void get3GAuthResponseIccAuthenticationTooShort() {
        when(mDataTelephonyManager.getIccAuthentication(TelephonyManager.APPTYPE_USIM,
                        TelephonyManager.AUTHTYPE_EAP_AKA, "AgEjAkVn"))
                .thenReturn(Base64.encodeToString(new byte[] {(byte) 0xdc}, Base64.NO_WRAP));

        SimAuthResponseData response = TelephonyUtil.get3GAuthResponse(
                new SimAuthRequestData(0, 0, "SSID", new String[]{"0123", "4567"}),
                mTelephonyManager);
        assertNull(response);
    }

    @Test
    public void get3GAuthResponseBadTag() {
        when(mDataTelephonyManager.getIccAuthentication(TelephonyManager.APPTYPE_USIM,
                        TelephonyManager.AUTHTYPE_EAP_AKA, "AgEjAkVn"))
                .thenReturn(Base64.encodeToString(new byte[] {0x31, 0x1, 0x2, 0x3, 0x4},
                                Base64.NO_WRAP));

        SimAuthResponseData response = TelephonyUtil.get3GAuthResponse(
                new SimAuthRequestData(0, 0, "SSID", new String[]{"0123", "4567"}),
                mTelephonyManager);
        assertNull(response);
    }

    @Test
    public void get3GAuthResponseUmtsAuth() {
        when(mDataTelephonyManager.getIccAuthentication(TelephonyManager.APPTYPE_USIM,
                        TelephonyManager.AUTHTYPE_EAP_AKA, "AgEjAkVn"))
                .thenReturn(create3GSimAuthUmtsAuthResponse(new byte[] {0x11, 0x12},
                                new byte[] {0x21, 0x22, 0x23}, new byte[] {0x31}));

        SimAuthResponseData response = TelephonyUtil.get3GAuthResponse(
                new SimAuthRequestData(0, 0, "SSID", new String[]{"0123", "4567"}),
                mTelephonyManager);
        assertNotNull(response);
        assertEquals("UMTS-AUTH", response.type);
        assertEquals(":31:212223:1112", response.response);
    }

    @Test
    public void get3GAuthResponseUmtsAuts() {
        when(mDataTelephonyManager.getIccAuthentication(TelephonyManager.APPTYPE_USIM,
                        TelephonyManager.AUTHTYPE_EAP_AKA, "AgEjAkVn"))
                .thenReturn(create3GSimAuthUmtsAutsResponse(new byte[] {0x22, 0x33}));

        SimAuthResponseData response = TelephonyUtil.get3GAuthResponse(
                new SimAuthRequestData(0, 0, "SSID", new String[]{"0123", "4567"}),
                mTelephonyManager);
        assertNotNull(response);
        assertEquals("UMTS-AUTS", response.type);
        assertEquals(":2233", response.response);
    }

    /**
     * Verify that anonymous identity should be a valid format based on MCC/MNC of current SIM.
     */
    @Test
    public void getAnonymousIdentityWithSim() {
        String mccmnc = "123456";
        String expectedIdentity = "anonymous@wlan.mnc456.mcc123.3gppnetwork.org";
        when(mDataTelephonyManager.getSimState()).thenReturn(TelephonyManager.SIM_STATE_READY);
        when(mDataTelephonyManager.getSimOperator()).thenReturn(mccmnc);

        assertEquals(expectedIdentity,
                TelephonyUtil.getAnonymousIdentityWith3GppRealm(mTelephonyManager));
    }

    /**
     * Verify that anonymous identity should be {@code null} when SIM is absent.
     */
    @Test
    public void getAnonymousIdentityWithoutSim() {
        when(mDataTelephonyManager.getSimState()).thenReturn(TelephonyManager.SIM_STATE_ABSENT);

        assertNull(TelephonyUtil.getAnonymousIdentityWith3GppRealm(mTelephonyManager));
    }
}
