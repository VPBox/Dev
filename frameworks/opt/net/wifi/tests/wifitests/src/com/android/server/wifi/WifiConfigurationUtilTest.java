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

package com.android.server.wifi;

import static org.junit.Assert.*;

import android.content.pm.UserInfo;
import android.net.IpConfiguration;
import android.net.MacAddress;
import android.net.wifi.WifiConfiguration;
import android.net.wifi.WifiEnterpriseConfig;
import android.net.wifi.WifiNetworkSpecifier;
import android.net.wifi.WifiScanner;
import android.os.PatternMatcher;
import android.os.UserHandle;
import android.util.Pair;

import androidx.test.filters.SmallTest;

import org.junit.Test;

import java.security.cert.X509Certificate;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.List;

/**
 * Unit tests for {@link com.android.server.wifi.WifiConfigurationUtil}.
 */
@SmallTest
public class WifiConfigurationUtilTest {
    static final int CURRENT_USER_ID = 0;
    static final int CURRENT_USER_MANAGED_PROFILE_USER_ID = 10;
    static final int OTHER_USER_ID = 11;
    static final int TEST_UID = 10000;
    static final String TEST_PACKAGE = "com.test";
    static final String TEST_SSID = "test_ssid";
    static final String TEST_SSID_1 = "test_ssid_1";
    static final String TEST_BSSID = "aa:aa:11:22:cc:dd";
    static final String TEST_BSSID_1 = "11:22:11:22:cc:dd";
    static final List<UserInfo> PROFILES = Arrays.asList(
            new UserInfo(CURRENT_USER_ID, "owner", 0),
            new UserInfo(CURRENT_USER_MANAGED_PROFILE_USER_ID, "managed profile", 0));

    /**
     * Test for {@link WifiConfigurationUtil.isVisibleToAnyProfile}.
     */
    @Test
    public void isVisibleToAnyProfile() {
        // Shared network configuration created by another user.
        final WifiConfiguration configuration = new WifiConfiguration();
        configuration.creatorUid = UserHandle.getUid(OTHER_USER_ID, 0);
        assertTrue(WifiConfigurationUtil.isVisibleToAnyProfile(configuration, PROFILES));

        // Private network configuration created by another user.
        configuration.shared = false;
        assertFalse(WifiConfigurationUtil.isVisibleToAnyProfile(configuration, PROFILES));

        // Private network configuration created by the current user.
        configuration.creatorUid = UserHandle.getUid(CURRENT_USER_ID, 0);
        assertTrue(WifiConfigurationUtil.isVisibleToAnyProfile(configuration, PROFILES));

        // Private network configuration created by the current user's managed profile.
        configuration.creatorUid = UserHandle.getUid(CURRENT_USER_MANAGED_PROFILE_USER_ID, 0);
        assertTrue(WifiConfigurationUtil.isVisibleToAnyProfile(configuration, PROFILES));
    }

    /**
     * Verify that new WifiEnterpriseConfig is detected.
     */
    @Test
    public void testEnterpriseConfigAdded() {
        EnterpriseConfig eapConfig = new EnterpriseConfig(WifiEnterpriseConfig.Eap.TTLS)
                .setPhase2(WifiEnterpriseConfig.Phase2.MSCHAPV2)
                .setIdentity("username", "password")
                .setCaCerts(new X509Certificate[]{FakeKeys.CA_CERT0});

        assertTrue(WifiConfigurationUtil.hasEnterpriseConfigChanged(
                null, eapConfig.enterpriseConfig));
    }

    /**
     * Verify WifiEnterpriseConfig eap change is detected.
     */
    @Test
    public void testEnterpriseConfigEapChangeDetected() {
        EnterpriseConfig eapConfig = new EnterpriseConfig(WifiEnterpriseConfig.Eap.TTLS);
        EnterpriseConfig peapConfig = new EnterpriseConfig(WifiEnterpriseConfig.Eap.PEAP);

        assertTrue(WifiConfigurationUtil.hasEnterpriseConfigChanged(eapConfig.enterpriseConfig,
                peapConfig.enterpriseConfig));
    }

    /**
     * Verify WifiEnterpriseConfig phase2 method change is detected.
     */
    @Test
    public void testEnterpriseConfigPhase2ChangeDetected() {
        EnterpriseConfig eapConfig =
                new EnterpriseConfig(WifiEnterpriseConfig.Eap.TTLS)
                        .setPhase2(WifiEnterpriseConfig.Phase2.MSCHAPV2);
        EnterpriseConfig papConfig =
                new EnterpriseConfig(WifiEnterpriseConfig.Eap.TTLS)
                        .setPhase2(WifiEnterpriseConfig.Phase2.PAP);

        assertTrue(WifiConfigurationUtil.hasEnterpriseConfigChanged(eapConfig.enterpriseConfig,
                papConfig.enterpriseConfig));
    }

    /**
     * Verify WifiEnterpriseConfig added Certificate is detected.
     */
    @Test
    public void testCaCertificateAddedDetected() {
        EnterpriseConfig eapConfigNoCerts = new EnterpriseConfig(WifiEnterpriseConfig.Eap.TTLS)
                .setPhase2(WifiEnterpriseConfig.Phase2.MSCHAPV2)
                .setIdentity("username", "password");

        EnterpriseConfig eapConfig1Cert = new EnterpriseConfig(WifiEnterpriseConfig.Eap.TTLS)
                .setPhase2(WifiEnterpriseConfig.Phase2.MSCHAPV2)
                .setIdentity("username", "password")
                .setCaCerts(new X509Certificate[]{FakeKeys.CA_CERT0});

        assertTrue(WifiConfigurationUtil.hasEnterpriseConfigChanged(
                eapConfigNoCerts.enterpriseConfig, eapConfig1Cert.enterpriseConfig));
    }

    /**
     * Verify WifiEnterpriseConfig Certificate change is detected.
     */
    @Test
    public void testDifferentCaCertificateDetected() {
        EnterpriseConfig eapConfig = new EnterpriseConfig(WifiEnterpriseConfig.Eap.TTLS)
                .setPhase2(WifiEnterpriseConfig.Phase2.MSCHAPV2)
                .setIdentity("username", "password")
                .setCaCerts(new X509Certificate[]{FakeKeys.CA_CERT0});

        EnterpriseConfig eapConfigNewCert = new EnterpriseConfig(WifiEnterpriseConfig.Eap.TTLS)
                .setPhase2(WifiEnterpriseConfig.Phase2.MSCHAPV2)
                .setIdentity("username", "password")
                .setCaCerts(new X509Certificate[]{FakeKeys.CA_CERT1});

        assertTrue(WifiConfigurationUtil.hasEnterpriseConfigChanged(eapConfig.enterpriseConfig,
                eapConfigNewCert.enterpriseConfig));
    }

    /**
     * Verify WifiEnterpriseConfig added Certificate changes are detected.
     */
    @Test
    public void testCaCertificateChangesDetected() {
        EnterpriseConfig eapConfig = new EnterpriseConfig(WifiEnterpriseConfig.Eap.TTLS)
                .setPhase2(WifiEnterpriseConfig.Phase2.MSCHAPV2)
                .setIdentity("username", "password")
                .setCaCerts(new X509Certificate[]{FakeKeys.CA_CERT0});

        EnterpriseConfig eapConfigAddedCert = new EnterpriseConfig(WifiEnterpriseConfig.Eap.TTLS)
                .setPhase2(WifiEnterpriseConfig.Phase2.MSCHAPV2)
                .setIdentity("username", "password")
                .setCaCerts(new X509Certificate[]{FakeKeys.CA_CERT0, FakeKeys.CA_CERT1});

        assertTrue(WifiConfigurationUtil.hasEnterpriseConfigChanged(eapConfig.enterpriseConfig,
                eapConfigAddedCert.enterpriseConfig));
    }

    /**
     * Verify that WifiEnterpriseConfig does not detect changes for identical configs.
     */
    @Test
    public void testWifiEnterpriseConfigNoChanges() {
        EnterpriseConfig eapConfig = new EnterpriseConfig(WifiEnterpriseConfig.Eap.TTLS)
                .setPhase2(WifiEnterpriseConfig.Phase2.MSCHAPV2)
                .setIdentity("username", "password")
                .setCaCerts(new X509Certificate[]{FakeKeys.CA_CERT0, FakeKeys.CA_CERT1});

        // Just to be clear that check is not against the same object
        EnterpriseConfig eapConfigSame = new EnterpriseConfig(WifiEnterpriseConfig.Eap.TTLS)
                .setPhase2(WifiEnterpriseConfig.Phase2.MSCHAPV2)
                .setIdentity("username", "password")
                .setCaCerts(new X509Certificate[]{FakeKeys.CA_CERT0, FakeKeys.CA_CERT1});

        assertFalse(WifiConfigurationUtil.hasEnterpriseConfigChanged(eapConfig.enterpriseConfig,
                eapConfigSame.enterpriseConfig));
    }

    /**
     * Verify that the validate method successfully validates good WifiConfigurations with ASCII
     * values.
     */
    @Test
    public void testValidatePositiveCases_Ascii() {
        assertTrue(WifiConfigurationUtil.validate(
                WifiConfigurationTestUtil.createOpenNetwork(),
                WifiConfigurationUtil.VALIDATE_FOR_ADD));
        assertTrue(WifiConfigurationUtil.validate(
                WifiConfigurationTestUtil.createPskNetwork(),
                WifiConfigurationUtil.VALIDATE_FOR_ADD));
        assertTrue(WifiConfigurationUtil.validate(
                WifiConfigurationTestUtil.createWepNetwork(),
                WifiConfigurationUtil.VALIDATE_FOR_ADD));
        assertTrue(WifiConfigurationUtil.validate(
                WifiConfigurationTestUtil.createEapNetwork(),
                WifiConfigurationUtil.VALIDATE_FOR_ADD));
        assertTrue(WifiConfigurationUtil.validate(
                WifiConfigurationTestUtil.createOweNetwork(),
                WifiConfigurationUtil.VALIDATE_FOR_ADD));
        assertTrue(WifiConfigurationUtil.validate(
                WifiConfigurationTestUtil.createSaeNetwork(),
                WifiConfigurationUtil.VALIDATE_FOR_ADD));
        assertTrue(WifiConfigurationUtil.validate(
                WifiConfigurationTestUtil.createEapSuiteBNetwork(),
                WifiConfigurationUtil.VALIDATE_FOR_ADD));
    }

    /**
     * Verify that the validate method successfully validates good WifiConfigurations with hex
     * values.
     */
    @Test
    public void testValidatePositiveCases_Hex() {
        WifiConfiguration config = WifiConfigurationTestUtil.createPskNetwork();
        config.SSID = "abcd1234555a";
        config.preSharedKey = "abcd123455151234556788990034556667332345667322344556676743233445";
        assertTrue(WifiConfigurationUtil.validate(config, WifiConfigurationUtil.VALIDATE_FOR_ADD));
    }

    /**
     * Verify that the validate method validates WifiConfiguration with masked psk string only for
     * an update.
     */
    @Test
    public void testValidatePositiveCases_MaskedPskString() {
        WifiConfiguration config = WifiConfigurationTestUtil.createPskNetwork();
        assertTrue(WifiConfigurationUtil.validate(config, WifiConfigurationUtil.VALIDATE_FOR_ADD));

        config.preSharedKey = WifiConfigurationUtil.PASSWORD_MASK;
        assertFalse(WifiConfigurationUtil.validate(config, WifiConfigurationUtil.VALIDATE_FOR_ADD));
        assertTrue(WifiConfigurationUtil.validate(
                config, WifiConfigurationUtil.VALIDATE_FOR_UPDATE));
    }

    /**
     * Verify that the validate method validates WifiConfiguration with null ssid only for an
     * update.
     */
    @Test
    public void testValidatePositiveCases_OnlyUpdateIgnoresNullSsid() {
        WifiConfiguration config = new WifiConfiguration();
        assertFalse(WifiConfigurationUtil.validate(config, WifiConfigurationUtil.VALIDATE_FOR_ADD));
        assertTrue(WifiConfigurationUtil.validate(
                config, WifiConfigurationUtil.VALIDATE_FOR_UPDATE));
    }

    /**
     * Verify that the validate method fails to validate WifiConfiguration with bad ssid length.
     */
    @Test
    public void testValidateNegativeCases_BadAsciiSsidLength() {
        WifiConfiguration config = WifiConfigurationTestUtil.createOpenNetwork();
        assertTrue(WifiConfigurationUtil.validate(config, WifiConfigurationUtil.VALIDATE_FOR_ADD));

        config.SSID = "\"abcdfefeeretretyetretetetetetrertertrsreqwrwe\"";
        assertFalse(WifiConfigurationUtil.validate(config, WifiConfigurationUtil.VALIDATE_FOR_ADD));
        config.SSID = "\"\"";
        assertFalse(WifiConfigurationUtil.validate(config, WifiConfigurationUtil.VALIDATE_FOR_ADD));
    }

    /**
     * Verify that the validate method fails to validate WifiConfiguration with bad ssid length.
     */
    @Test
    public void testValidateNegativeCases_BadUtf8SsidLength() {
        WifiConfiguration config = WifiConfigurationTestUtil.createOpenNetwork();
        assertTrue(WifiConfigurationUtil.validate(config, WifiConfigurationUtil.VALIDATE_FOR_ADD));

        config.SSID = "\"가하아너너ㅓ저저ㅓ어아아다자저ㅓ더타아어어러두어\"";
        assertFalse(WifiConfigurationUtil.validate(config, WifiConfigurationUtil.VALIDATE_FOR_ADD));
        config.SSID = "\"\"";
        assertFalse(WifiConfigurationUtil.validate(config, WifiConfigurationUtil.VALIDATE_FOR_ADD));
    }

    /**
     * Verify that the validate method fails to validate WifiConfiguration with malformed ssid
     * string.
     */
    @Test
    public void testValidateNegativeCases_MalformedAsciiSsidString() {
        WifiConfiguration config = WifiConfigurationTestUtil.createOpenNetwork();
        assertTrue(WifiConfigurationUtil.validate(config, WifiConfigurationUtil.VALIDATE_FOR_ADD));

        config.SSID = "\"ab";
        assertFalse(WifiConfigurationUtil.validate(config, WifiConfigurationUtil.VALIDATE_FOR_ADD));
    }

    /**
     * Verify that the validate method fails to validate WifiConfiguration with bad ssid length.
     */
    @Test
    public void testValidateNegativeCases_BadHexSsidLength() {
        WifiConfiguration config = WifiConfigurationTestUtil.createOpenNetwork();
        assertTrue(WifiConfigurationUtil.validate(config, WifiConfigurationUtil.VALIDATE_FOR_ADD));

        config.SSID = "abcdfe012345632423343543453456464545656464545646454ace34534545634535";
        assertFalse(WifiConfigurationUtil.validate(config, WifiConfigurationUtil.VALIDATE_FOR_ADD));
        config.SSID = "";
        assertFalse(WifiConfigurationUtil.validate(config, WifiConfigurationUtil.VALIDATE_FOR_ADD));
    }

    /**
     * Verify that the validate method fails to validate WifiConfiguration with malformed ssid
     * string.
     */
    @Test
    public void testValidateNegativeCases_MalformedHexSsidString() {
        WifiConfiguration config = WifiConfigurationTestUtil.createOpenNetwork();
        assertTrue(WifiConfigurationUtil.validate(config, WifiConfigurationUtil.VALIDATE_FOR_ADD));

        config.SSID = "hello";
        assertFalse(WifiConfigurationUtil.validate(config, WifiConfigurationUtil.VALIDATE_FOR_ADD));
    }

    /**
     * Verify that the validate method fails to validate WifiConfiguration with bad psk length.
     */
    @Test
    public void testValidateNegativeCases_BadAsciiPskLength() {
        WifiConfiguration config = WifiConfigurationTestUtil.createPskNetwork();
        assertTrue(WifiConfigurationUtil.validate(config, WifiConfigurationUtil.VALIDATE_FOR_ADD));

        config.preSharedKey = "\"abcdffeeretretyetreteteteabe34tetrertertrsraaaaaaaaaaa345eqwrweewq"
                + "weqe\"";
        assertFalse(WifiConfigurationUtil.validate(config, WifiConfigurationUtil.VALIDATE_FOR_ADD));
        config.preSharedKey = "\"454\"";
        assertFalse(WifiConfigurationUtil.validate(config, WifiConfigurationUtil.VALIDATE_FOR_ADD));
    }

    /**
     * Verify that the validate method fails to validate WifiConfiguration with bad sae length.
     */
    @Test
    public void testValidateNegativeCases_BadAsciiSaeLength() {
        WifiConfiguration config = WifiConfigurationTestUtil.createSaeNetwork();
        assertTrue(WifiConfigurationUtil.validate(config, WifiConfigurationUtil.VALIDATE_FOR_ADD));

        config.preSharedKey = "\"abcdffeeretretyetreteteteabe34tetrertertrsraaaaaaaaaaa345eqwrweewq"
                + "weqe\"";
        assertFalse(WifiConfigurationUtil.validate(config, WifiConfigurationUtil.VALIDATE_FOR_ADD));
        config.preSharedKey = "\"\"";
        assertFalse(WifiConfigurationUtil.validate(config, WifiConfigurationUtil.VALIDATE_FOR_ADD));
    }

    /**
     * Verify that the validate method fails to validate WifiConfiguration with malformed psk
     * string.
     */
    @Test
    public void testValidateNegativeCases_MalformedAsciiPskString() {
        WifiConfiguration config = WifiConfigurationTestUtil.createPskNetwork();
        assertTrue(WifiConfigurationUtil.validate(config, WifiConfigurationUtil.VALIDATE_FOR_ADD));

        config.preSharedKey = "\"abcdfefeeretrety";
        assertFalse(WifiConfigurationUtil.validate(config, WifiConfigurationUtil.VALIDATE_FOR_ADD));
    }

    /**
     * Verify that the validate method fails to validate WifiConfiguration with malformed sae
     * string.
     */
    @Test
    public void testValidateNegativeCases_MalformedAsciiSaeString() {
        WifiConfiguration config = WifiConfigurationTestUtil.createSaeNetwork();
        assertTrue(WifiConfigurationUtil.validate(config, WifiConfigurationUtil.VALIDATE_FOR_ADD));

        config.preSharedKey = "\"abcdfefeeretrety";
        assertFalse(WifiConfigurationUtil.validate(config, WifiConfigurationUtil.VALIDATE_FOR_ADD));
    }

    /**
     * Verify that the validate method fails to validate WifiConfiguration with bad psk length.
     */
    @Test
    public void testValidateNegativeCases_BadHexPskLength() {
        WifiConfiguration config = WifiConfigurationTestUtil.createPskNetwork();
        assertTrue(WifiConfigurationUtil.validate(config, WifiConfigurationUtil.VALIDATE_FOR_ADD));

        config.preSharedKey = "abcd123456788990013453445345465465476546";
        assertFalse(WifiConfigurationUtil.validate(config, WifiConfigurationUtil.VALIDATE_FOR_ADD));
        config.preSharedKey = "";
        assertFalse(WifiConfigurationUtil.validate(config, WifiConfigurationUtil.VALIDATE_FOR_ADD));
    }

    /**
     * Verify that the validate method fails to validate WifiConfiguration with malformed psk
     * string.
     */
    @Test
    public void testValidateNegativeCases_MalformedHexPskString() {
        WifiConfiguration config = WifiConfigurationTestUtil.createPskNetwork();
        assertTrue(WifiConfigurationUtil.validate(config, WifiConfigurationUtil.VALIDATE_FOR_ADD));

        config.preSharedKey = "adbdfgretrtyrtyrty";
        assertFalse(WifiConfigurationUtil.validate(config, WifiConfigurationUtil.VALIDATE_FOR_ADD));
    }

    /**
     * Verify that the validate method fails to validate WifiConfiguration with malformed sae
     * string.
     */
    @Test
    public void testValidateNegativeCases_MalformedHexSaeString() {
        WifiConfiguration config = WifiConfigurationTestUtil.createSaeNetwork();
        assertTrue(WifiConfigurationUtil.validate(config, WifiConfigurationUtil.VALIDATE_FOR_ADD));

        config.preSharedKey = "adbdfgretrtyrtyrty";
        assertFalse(WifiConfigurationUtil.validate(config, WifiConfigurationUtil.VALIDATE_FOR_ADD));
    }

    /**
     * Verify that the validate method fails to validate WifiConfiguration with bad key mgmt values.
     */
    @Test
    public void testValidateNegativeCases_BadKeyMgmtPskEap() {
        WifiConfiguration config = WifiConfigurationTestUtil.createPskNetwork();
        assertTrue(WifiConfigurationUtil.validate(config, WifiConfigurationUtil.VALIDATE_FOR_ADD));

        config.allowedKeyManagement.set(WifiConfiguration.KeyMgmt.IEEE8021X);
        assertFalse(WifiConfigurationUtil.validate(config, WifiConfigurationUtil.VALIDATE_FOR_ADD));
    }

    /**
     * Verify that the validate method fails to validate WifiConfiguration with bad key mgmt values.
     */
    @Test
    public void testValidateNegativeCases_BadKeyMgmtOpenPsk() {
        WifiConfiguration config = WifiConfigurationTestUtil.createOpenNetwork();
        assertTrue(WifiConfigurationUtil.validate(config, WifiConfigurationUtil.VALIDATE_FOR_ADD));

        config.allowedKeyManagement.set(WifiConfiguration.KeyMgmt.WPA_PSK);
        assertFalse(WifiConfigurationUtil.validate(config, WifiConfigurationUtil.VALIDATE_FOR_ADD));
    }

    /**
     * Verify that the validate method fails to validate WifiConfiguration with bad key mgmt values.
     */
    @Test
    public void testValidateNegativeCases_BadKeyMgmt() {
        WifiConfiguration config = WifiConfigurationTestUtil.createPskNetwork();
        assertTrue(WifiConfigurationUtil.validate(config, WifiConfigurationUtil.VALIDATE_FOR_ADD));

        config.allowedKeyManagement.set(WifiConfiguration.KeyMgmt.IEEE8021X);
        assertFalse(WifiConfigurationUtil.validate(config, WifiConfigurationUtil.VALIDATE_FOR_ADD));
    }

    /**
     * Verify that the validate method fails to validate WifiConfiguration with bad ipconfiguration
     * values.
     */
    @Test
    public void testValidateNegativeCases_BadIpconfiguration() {
        WifiConfiguration config = WifiConfigurationTestUtil.createPskNetwork();
        IpConfiguration ipConfig =
                WifiConfigurationTestUtil.createStaticIpConfigurationWithPacProxy();
        config.setIpConfiguration(ipConfig);
        assertTrue(WifiConfigurationUtil.validate(config, WifiConfigurationUtil.VALIDATE_FOR_ADD));

        ipConfig.setStaticIpConfiguration(null);
        config.setIpConfiguration(ipConfig);
        assertFalse(WifiConfigurationUtil.validate(config, WifiConfigurationUtil.VALIDATE_FOR_ADD));
    }

    /**
     * Verify that the validate method fails to validate WifiConfiguration with bad KeyMgmt value.
     */
    @Test
    public void testValidateNegativeCases_InvalidKeyMgmt() {
        WifiConfiguration config = WifiConfigurationTestUtil.createOpenNetwork();
        assertTrue(WifiConfigurationUtil.validate(config, WifiConfigurationUtil.VALIDATE_FOR_ADD));

        config.allowedKeyManagement.set(WifiConfiguration.KeyMgmt.FT_EAP + 1);
        assertFalse(WifiConfigurationUtil.validate(config, WifiConfigurationUtil.VALIDATE_FOR_ADD));
    }

    /**
     * Verify that the validate method fails to validate WifiConfiguration with bad Protocol value.
     */
    @Test
    public void testValidateNegativeCases_InvalidProtocol() {
        WifiConfiguration config = WifiConfigurationTestUtil.createOpenNetwork();
        assertTrue(WifiConfigurationUtil.validate(config, WifiConfigurationUtil.VALIDATE_FOR_ADD));

        config.allowedProtocols.set(3);
        assertFalse(WifiConfigurationUtil.validate(config, WifiConfigurationUtil.VALIDATE_FOR_ADD));
    }

    /**
     * Verify that the validate method fails to validate WifiConfiguration with bad AuthAlgorithm
     * value.
     */
    @Test
    public void testValidateNegativeCases_InvalidAuthAlgorithm() {
        WifiConfiguration config = WifiConfigurationTestUtil.createOpenNetwork();
        assertTrue(WifiConfigurationUtil.validate(config, WifiConfigurationUtil.VALIDATE_FOR_ADD));

        config.allowedAuthAlgorithms.set(WifiConfiguration.AuthAlgorithm.LEAP + 3);
        assertFalse(WifiConfigurationUtil.validate(config, WifiConfigurationUtil.VALIDATE_FOR_ADD));
    }

    /**
     * Verify that the validate method fails to validate WifiConfiguration with bad GroupCipher
     * value.
     */
    @Test
    public void testValidateNegativeCases_InvalidGroupCipher() {
        WifiConfiguration config = WifiConfigurationTestUtil.createOpenNetwork();
        assertTrue(WifiConfigurationUtil.validate(config, WifiConfigurationUtil.VALIDATE_FOR_ADD));

        config.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.GTK_NOT_USED + 2);
        assertFalse(WifiConfigurationUtil.validate(config, WifiConfigurationUtil.VALIDATE_FOR_ADD));
    }

    /**
     * Verify that the validate method fails to validate WifiConfiguration with bad PairwiseCipher
     * value.
     */
    @Test
    public void testValidateNegativeCases_InvalidPairwiseCipher() {
        WifiConfiguration config = WifiConfigurationTestUtil.createOpenNetwork();
        assertTrue(WifiConfigurationUtil.validate(config, WifiConfigurationUtil.VALIDATE_FOR_ADD));

        config.allowedPairwiseCiphers.set(WifiConfiguration.PairwiseCipher.CCMP + 2);
        assertFalse(WifiConfigurationUtil.validate(config, WifiConfigurationUtil.VALIDATE_FOR_ADD));
    }

    /**
     * Verify that the validate method fails to validate WifiConfiguration with malformed sae
     * string.
     */
    @Test
    public void testValidateNegativeCases_SaeMissingPmf() {
        WifiConfiguration config = WifiConfigurationTestUtil.createSaeNetwork();
        assertTrue(WifiConfigurationUtil.validate(config, WifiConfigurationUtil.VALIDATE_FOR_ADD));

        config.requirePMF = false;
        assertFalse(WifiConfigurationUtil.validate(config, WifiConfigurationUtil.VALIDATE_FOR_ADD));
    }

    /**
     * Verify that the validate method fails to validate WifiConfiguration with malformed owe
     * string.
     */
    @Test
    public void testValidateNegativeCases_OweMissingPmf() {
        WifiConfiguration config = WifiConfigurationTestUtil.createOweNetwork();
        assertTrue(WifiConfigurationUtil.validate(config, WifiConfigurationUtil.VALIDATE_FOR_ADD));

        config.requirePMF = false;
        assertFalse(WifiConfigurationUtil.validate(config, WifiConfigurationUtil.VALIDATE_FOR_ADD));
    }

    /**
     * Verify that the validate method fails to validate WifiConfiguration with malformed suiteb
     * string.
     */
    @Test
    public void testValidateNegativeCases_SuitebMissingPmf() {
        WifiConfiguration config = WifiConfigurationTestUtil.createEapSuiteBNetwork();
        assertTrue(WifiConfigurationUtil.validate(config, WifiConfigurationUtil.VALIDATE_FOR_ADD));

        config.requirePMF = false;
        assertFalse(WifiConfigurationUtil.validate(config, WifiConfigurationUtil.VALIDATE_FOR_ADD));
    }

    /**
     * Verify that the validate method successfully validates good WifiNetworkSpecifier with
     * only ssid pattern set.
     */
    @Test
    public void testValidateNetworkSpecifierPositiveCases_SsidPattern() {
        WifiNetworkSpecifier specifier = new WifiNetworkSpecifier(
                new PatternMatcher(TEST_SSID, PatternMatcher.PATTERN_LITERAL),
                Pair.create(MacAddress.ALL_ZEROS_ADDRESS, MacAddress.ALL_ZEROS_ADDRESS),
                WifiConfigurationTestUtil.createOpenNetwork(),
                TEST_UID, TEST_PACKAGE);
        assertTrue(WifiConfigurationUtil.validateNetworkSpecifier(specifier));
    }

    /**
     * Verify that the validate method successfully validates good WifiNetworkSpecifier with
     * only bssid pattern set.
     */
    @Test
    public void testValidateNetworkSpecifierPositiveCases_BssidPattern() {
        WifiNetworkSpecifier specifier = new WifiNetworkSpecifier(
                new PatternMatcher(".*", PatternMatcher.PATTERN_SIMPLE_GLOB),
                Pair.create(MacAddress.fromString(TEST_BSSID), MacAddress.BROADCAST_ADDRESS),
                WifiConfigurationTestUtil.createOpenNetwork(),
                TEST_UID, TEST_PACKAGE);
        assertTrue(WifiConfigurationUtil.validateNetworkSpecifier(specifier));
    }

    /**
     * Verify that the validate method successfully validates good WifiNetworkSpecifier with
     * both ssid & bssid patterns set.
     */
    @Test
    public void testValidateNetworkSpecifierPositiveCases_BothSsidPatternAndBssidPattern() {
        WifiNetworkSpecifier specifier = new WifiNetworkSpecifier(
                new PatternMatcher(TEST_SSID, PatternMatcher.PATTERN_LITERAL),
                Pair.create(MacAddress.fromString(TEST_BSSID), MacAddress.BROADCAST_ADDRESS),
                WifiConfigurationTestUtil.createOpenNetwork(),
                TEST_UID, TEST_PACKAGE);
        assertTrue(WifiConfigurationUtil.validateNetworkSpecifier(specifier));
    }

    /**
     * Verify that the validate method fails to validate WifiNetworkSpecifier with no
     * ssid/bssid info.
     */
    @Test
    public void testValidateNetworkSpecifierNegativeCases_NoSsidBssid() {
        WifiNetworkSpecifier specifier = new WifiNetworkSpecifier(
                new PatternMatcher(".*", PatternMatcher.PATTERN_SIMPLE_GLOB),
                Pair.create(MacAddress.ALL_ZEROS_ADDRESS, MacAddress.ALL_ZEROS_ADDRESS),
                WifiConfigurationTestUtil.createOpenNetwork(),
                TEST_UID, TEST_PACKAGE);
        assertFalse(WifiConfigurationUtil.validateNetworkSpecifier(specifier));
    }

    /**
     * Verify that the validate method fails to validate WifiNetworkSpecifier with invalid SSID
     * match pattern.
     */
    @Test
    public void testValidateNetworkSpecifierNegativeCases_MatchNoneSsidPattern() {
        WifiNetworkSpecifier specifier = new WifiNetworkSpecifier(
                new PatternMatcher("", PatternMatcher.PATTERN_LITERAL),
                Pair.create(MacAddress.ALL_ZEROS_ADDRESS, MacAddress.ALL_ZEROS_ADDRESS),
                WifiConfigurationTestUtil.createOpenNetwork(),
                TEST_UID, TEST_PACKAGE);
        assertFalse(WifiConfigurationUtil.validateNetworkSpecifier(specifier));
    }

    /**
     * Verify that the validate method fails to validate WifiNetworkSpecifier with illegal
     * pattern.
     */
    @Test
    public void testValidateNetworkSpecifierNegativeCases_MatchNoneBssidPattern() {
        WifiNetworkSpecifier specifier = new WifiNetworkSpecifier(
                new PatternMatcher(TEST_SSID, PatternMatcher.PATTERN_LITERAL),
                Pair.create(MacAddress.BROADCAST_ADDRESS, MacAddress.BROADCAST_ADDRESS),
                WifiConfigurationTestUtil.createOpenNetwork(),
                TEST_UID, TEST_PACKAGE);
        assertFalse(WifiConfigurationUtil.validateNetworkSpecifier(specifier));
    }

    /**
     * Verify that the validate method fails to validate WifiNetworkSpecifier with illegal
     * pattern.
     */
    @Test
    public void testValidateNetworkSpecifierNegativeCases_InvalidBssidPattern() {
        WifiNetworkSpecifier specifier = new WifiNetworkSpecifier(
                new PatternMatcher(TEST_SSID, PatternMatcher.PATTERN_LITERAL),
                Pair.create(MacAddress.fromString(TEST_BSSID), MacAddress.ALL_ZEROS_ADDRESS),
                WifiConfigurationTestUtil.createOpenNetwork(),
                TEST_UID, TEST_PACKAGE);
        assertFalse(WifiConfigurationUtil.validateNetworkSpecifier(specifier));
    }

    /**
     * Verify that the validate method fails to validate WifiNetworkSpecifier with SSID pattern
     * for hidden network.
     */
    @Test
    public void testValidateNetworkSpecifierNegativeCases_NoSsidPatternForHiddenNetwork() {
        WifiNetworkSpecifier specifier = new WifiNetworkSpecifier(
                new PatternMatcher(TEST_SSID, PatternMatcher.PATTERN_PREFIX),
                Pair.create(MacAddress.ALL_ZEROS_ADDRESS, MacAddress.ALL_ZEROS_ADDRESS),
                WifiConfigurationTestUtil.createOpenHiddenNetwork(),
                TEST_UID, TEST_PACKAGE);
        assertFalse(WifiConfigurationUtil.validateNetworkSpecifier(specifier));
    }

    /**
     * Verify the instance of {@link android.net.wifi.WifiScanner.PnoSettings.PnoNetwork} created
     * for an open network using {@link WifiConfigurationUtil#createPnoNetwork(
     * WifiConfiguration)}.
     */
    @Test
    public void testCreatePnoNetworkWithOpenNetwork() {
        WifiConfiguration network = WifiConfigurationTestUtil.createOpenNetwork();
        WifiScanner.PnoSettings.PnoNetwork pnoNetwork =
                WifiConfigurationUtil.createPnoNetwork(network);
        assertEquals(network.SSID, pnoNetwork.ssid);
        assertEquals(
                WifiScanner.PnoSettings.PnoNetwork.FLAG_A_BAND
                        | WifiScanner.PnoSettings.PnoNetwork.FLAG_G_BAND, pnoNetwork.flags);
        assertEquals(WifiScanner.PnoSettings.PnoNetwork.AUTH_CODE_OPEN, pnoNetwork.authBitField);
    }

    /**
     * Verify the instance of {@link android.net.wifi.WifiScanner.PnoSettings.PnoNetwork} created
     * for an open hidden network using {@link WifiConfigurationUtil#createPnoNetwork(
     * WifiConfiguration)}.
     */
    @Test
    public void testCreatePnoNetworkWithOpenHiddenNetwork() {
        WifiConfiguration network = WifiConfigurationTestUtil.createOpenHiddenNetwork();
        WifiScanner.PnoSettings.PnoNetwork pnoNetwork =
                WifiConfigurationUtil.createPnoNetwork(network);
        assertEquals(network.SSID, pnoNetwork.ssid);
        assertEquals(
                WifiScanner.PnoSettings.PnoNetwork.FLAG_A_BAND
                        | WifiScanner.PnoSettings.PnoNetwork.FLAG_G_BAND
                        | WifiScanner.PnoSettings.PnoNetwork.FLAG_DIRECTED_SCAN, pnoNetwork.flags);
        assertEquals(WifiScanner.PnoSettings.PnoNetwork.AUTH_CODE_OPEN, pnoNetwork.authBitField);
    }

    /**
     * Verify the instance of {@link android.net.wifi.WifiScanner.PnoSettings.PnoNetwork} created
     * for a PSK network using {@link WifiConfigurationUtil#createPnoNetwork(WifiConfiguration)
     * }.
     */
    @Test
    public void testCreatePnoNetworkWithPskNetwork() {
        WifiConfiguration network = WifiConfigurationTestUtil.createPskNetwork();
        WifiScanner.PnoSettings.PnoNetwork pnoNetwork =
                WifiConfigurationUtil.createPnoNetwork(network);
        assertEquals(network.SSID, pnoNetwork.ssid);
        assertEquals(
                WifiScanner.PnoSettings.PnoNetwork.FLAG_A_BAND
                        | WifiScanner.PnoSettings.PnoNetwork.FLAG_G_BAND, pnoNetwork.flags);
        assertEquals(WifiScanner.PnoSettings.PnoNetwork.AUTH_CODE_PSK, pnoNetwork.authBitField);
    }

    /**
     * Verify that WifiConfigurationUtil.isSameNetwork returns true when two WifiConfiguration
     * objects have the same parameters.
     */
    @Test
    public void testIsSameNetworkReturnsTrueOnSameNetwork() {
        WifiConfiguration network = WifiConfigurationTestUtil.createPskNetwork(TEST_SSID);
        WifiConfiguration network1 = WifiConfigurationTestUtil.createPskNetwork(TEST_SSID);
        assertTrue(WifiConfigurationUtil.isSameNetwork(network, network1));
    }

    /**
     * Verify that WifiConfigurationUtil.isSameNetwork returns true when two WifiConfiguration
     * objects have the same parameters but different network selection BSSID's.
     */
    @Test
    public void testIsSameNetworkReturnsTrueOnSameNetworkWithDifferentBSSID() {
        WifiConfiguration network = WifiConfigurationTestUtil.createPskNetwork(TEST_SSID);
        network.getNetworkSelectionStatus().setNetworkSelectionBSSID(TEST_BSSID);
        WifiConfiguration network1 = WifiConfigurationTestUtil.createPskNetwork(TEST_SSID);
        network1.getNetworkSelectionStatus().setNetworkSelectionBSSID(TEST_BSSID_1);
        assertTrue(WifiConfigurationUtil.isSameNetwork(network, network1));
    }

    /**
     * Verify that WifiConfigurationUtil.isSameNetwork returns false when two WifiConfiguration
     * objects have the different SSIDs.
     */
    @Test
    public void testIsSameNetworkReturnsFalseOnDifferentSSID() {
        WifiConfiguration network = WifiConfigurationTestUtil.createPskNetwork(TEST_SSID);
        WifiConfiguration network1 = WifiConfigurationTestUtil.createPskNetwork(TEST_SSID_1);
        assertFalse(WifiConfigurationUtil.isSameNetwork(network, network1));
    }

    /**
     * Verify that WifiConfigurationUtil.isSameNetwork returns false when two WifiConfiguration
     * objects have the different security type.
     */
    @Test
    public void testIsSameNetworkReturnsFalseOnDifferentSecurityType() {
        WifiConfiguration network = WifiConfigurationTestUtil.createPskNetwork(TEST_SSID);
        WifiConfiguration network1 = WifiConfigurationTestUtil.createEapNetwork(TEST_SSID);
        assertFalse(WifiConfigurationUtil.isSameNetwork(network, network1));
    }

    /**
     * Verify that WifiConfigurationUtil.isSameNetwork returns false when two WifiConfiguration
     * objects have the different EAP identity.
     */
    @Test
    public void testIsSameNetworkReturnsFalseOnDifferentEapIdentity() {
        WifiConfiguration network1 = WifiConfigurationTestUtil.createEapNetwork(TEST_SSID);
        WifiConfiguration network2 = WifiConfigurationTestUtil.createEapNetwork(TEST_SSID);
        network1.enterpriseConfig.setIdentity("Identity1");
        network2.enterpriseConfig.setIdentity("Identity2");
        assertFalse(WifiConfigurationUtil.isSameNetwork(network1, network2));
    }

    /**
     * Verify that WifiConfigurationUtil.isSameNetwork returns false when two WifiConfiguration
     * objects have the different EAP anonymous identity.
     */
    @Test
    public void testIsSameNetworkReturnsFalseOnDifferentEapAnonymousIdentity() {
        WifiConfiguration network1 = WifiConfigurationTestUtil.createEapNetwork(TEST_SSID);
        WifiConfiguration network2 = WifiConfigurationTestUtil.createEapNetwork(TEST_SSID);
        network1.enterpriseConfig.setAnonymousIdentity("Identity1");
        network2.enterpriseConfig.setAnonymousIdentity("Identity2");
        assertFalse(WifiConfigurationUtil.isSameNetwork(network1, network2));
    }

    /**
     * Verify that WifiConfigurationUtil.isSameNetwork returns true when two WifiConfiguration
     * objects have the different EAP anonymous(pseudonym) identity in EAP-SIM.
     */
    @Test
    public void testIsSameNetworkReturnsTrueOnDifferentEapAnonymousIdentityInEapSim() {
        WifiConfiguration network1 = WifiConfigurationTestUtil.createEapNetwork(TEST_SSID);
        WifiConfiguration network2 = WifiConfigurationTestUtil.createEapNetwork(TEST_SSID);
        network1.enterpriseConfig.setEapMethod(WifiEnterpriseConfig.Eap.SIM);
        network2.enterpriseConfig.setEapMethod(WifiEnterpriseConfig.Eap.SIM);
        network1.enterpriseConfig.setAnonymousIdentity("Identity1");
        network2.enterpriseConfig.setAnonymousIdentity("Identity2");
        assertTrue(WifiConfigurationUtil.isSameNetwork(network1, network2));
    }

    /**
     * Verify the instance of {@link android.net.wifi.WifiScanner.PnoSettings.PnoNetwork} created
     * for a EAP network using {@link WifiConfigurationUtil#createPnoNetwork(WifiConfiguration)
     * }.
     */
    @Test
    public void testCreatePnoNetworkWithEapNetwork() {
        WifiConfiguration network = WifiConfigurationTestUtil.createEapNetwork();
        WifiScanner.PnoSettings.PnoNetwork pnoNetwork =
                WifiConfigurationUtil.createPnoNetwork(network);
        assertEquals(network.SSID, pnoNetwork.ssid);
        assertEquals(
                WifiScanner.PnoSettings.PnoNetwork.FLAG_A_BAND
                        | WifiScanner.PnoSettings.PnoNetwork.FLAG_G_BAND, pnoNetwork.flags);
        assertEquals(WifiScanner.PnoSettings.PnoNetwork.AUTH_CODE_EAPOL, pnoNetwork.authBitField);
    }

    /**
     * Verify that the generalized
     * {@link com.android.server.wifi.WifiConfigurationUtil.WifiConfigurationComparator}
     * can be used to sort a List given a 'compareNetworkWithSameStatus' method.
     */
    @Test
    public void testPnoListComparator() {
        List<WifiConfiguration> networks = new ArrayList<>();
        final WifiConfiguration enabledNetwork1 = WifiConfigurationTestUtil.createEapNetwork();
        enabledNetwork1.getNetworkSelectionStatus().setNetworkSelectionStatus(
                WifiConfiguration.NetworkSelectionStatus.NETWORK_SELECTION_ENABLED);
        final WifiConfiguration enabledNetwork2 = WifiConfigurationTestUtil.createEapNetwork();
        enabledNetwork2.getNetworkSelectionStatus().setNetworkSelectionStatus(
                WifiConfiguration.NetworkSelectionStatus.NETWORK_SELECTION_ENABLED);
        final WifiConfiguration tempDisabledNetwork1 = WifiConfigurationTestUtil.createEapNetwork();
        tempDisabledNetwork1.getNetworkSelectionStatus().setNetworkSelectionStatus(
                WifiConfiguration.NetworkSelectionStatus.NETWORK_SELECTION_TEMPORARY_DISABLED);
        final WifiConfiguration tempDisabledNetwork2 = WifiConfigurationTestUtil.createEapNetwork();
        tempDisabledNetwork2.getNetworkSelectionStatus().setNetworkSelectionStatus(
                WifiConfiguration.NetworkSelectionStatus.NETWORK_SELECTION_TEMPORARY_DISABLED);
        WifiConfiguration permDisabledNetwork = WifiConfigurationTestUtil.createEapNetwork();
        permDisabledNetwork.getNetworkSelectionStatus().setNetworkSelectionStatus(
                WifiConfiguration.NetworkSelectionStatus.NETWORK_SELECTION_PERMANENTLY_DISABLED);

        // Add all the networks to the list.
        networks.add(tempDisabledNetwork1);
        networks.add(enabledNetwork1);
        networks.add(permDisabledNetwork);
        networks.add(tempDisabledNetwork2);
        networks.add(enabledNetwork2);

        // Prefer |enabledNetwork1| over |enabledNetwork2| and |tempDisabledNetwork1| over
        // |tempDisabledNetwork2|.
        WifiConfigurationUtil.WifiConfigurationComparator comparator =
                new WifiConfigurationUtil.WifiConfigurationComparator() {
                    @Override
                    public int compareNetworksWithSameStatus(
                            WifiConfiguration a, WifiConfiguration b) {
                        if (a == enabledNetwork1 && b == enabledNetwork2) {
                            return -1;
                        } else if (b == enabledNetwork1 && a == enabledNetwork2) {
                            return 1;
                        } else if (a == tempDisabledNetwork1 && b == tempDisabledNetwork2) {
                            return -1;
                        } else if (b == tempDisabledNetwork1 && a == tempDisabledNetwork2) {
                            return 1;
                        }
                        return 0;
                    }
                };
        Collections.sort(networks, comparator);

        // Now ensure that the networks were sorted correctly.
        assertEquals(enabledNetwork1, networks.get(0));
        assertEquals(enabledNetwork2, networks.get(1));
        assertEquals(tempDisabledNetwork1, networks.get(2));
        assertEquals(tempDisabledNetwork2, networks.get(3));
        assertEquals(permDisabledNetwork, networks.get(4));
    }

    /**
     * Verifies that when the existing configuration is null and macRandomizationSetting in the
     * newConfig is the default value, then hasMacRandomizationSettingsChanged returns false.
     */
    @Test
    public void testHasMacRandomizationSettingsChangedNullExistingConfigDefaultNewConfig() {
        WifiConfiguration newConfig = new WifiConfiguration();
        assertFalse(WifiConfigurationUtil.hasMacRandomizationSettingsChanged(null, newConfig));
    }

    /**
     * Verifies that when the existing configuration is null and macRandomizationSetting in the
     * newConfig is not the default value, then hasMacRandomizationSettingsChanged returns true.
     */
    @Test
    public void testHasMacRandomizationSettingsChangedNullExistingConfigModifiedNewConfig() {
        WifiConfiguration newConfig = new WifiConfiguration();
        newConfig.macRandomizationSetting = WifiConfiguration.RANDOMIZATION_NONE;
        assertTrue(WifiConfigurationUtil.hasMacRandomizationSettingsChanged(null, newConfig));
    }

    /**
     * Verifies that when macRandomizationSetting in the newConfig is different from existingConfig
     * hasMacRandomizationSettingsChanged returns true.
     */
    @Test
    public void testHasMacRandomizationSettingsChangedFieldsDifferent() {
        WifiConfiguration existingConfig = new WifiConfiguration();
        WifiConfiguration newConfig = new WifiConfiguration();
        newConfig.macRandomizationSetting = WifiConfiguration.RANDOMIZATION_NONE;
        assertTrue(WifiConfigurationUtil.hasMacRandomizationSettingsChanged(
                existingConfig, newConfig));
    }

    /**
     * Verifies that when macRandomizationSetting in the newConfig is the same as existingConfig
     * hasMacRandomizationSettingsChanged returns false.
     */
    @Test
    public void testHasMacRandomizationSettingsChangedFieldsSame() {
        WifiConfiguration existingConfig = new WifiConfiguration();
        existingConfig.macRandomizationSetting = WifiConfiguration.RANDOMIZATION_NONE;
        WifiConfiguration newConfig = new WifiConfiguration();
        newConfig.macRandomizationSetting = WifiConfiguration.RANDOMIZATION_NONE;
        assertFalse(WifiConfigurationUtil.hasMacRandomizationSettingsChanged(
                existingConfig, newConfig));
    }

    private static class EnterpriseConfig {
        public String eap;
        public String phase2;
        public String identity;
        public String password;
        public X509Certificate[] caCerts;
        public WifiEnterpriseConfig enterpriseConfig;

        EnterpriseConfig(int eapMethod) {
            enterpriseConfig = new WifiEnterpriseConfig();
            enterpriseConfig.setEapMethod(eapMethod);
            eap = WifiEnterpriseConfig.Eap.strings[eapMethod];
        }

        public EnterpriseConfig setPhase2(int phase2Method) {
            enterpriseConfig.setPhase2Method(phase2Method);
            phase2 = "auth=" + WifiEnterpriseConfig.Phase2.strings[phase2Method];
            return this;
        }

        public EnterpriseConfig setIdentity(String identity, String password) {
            enterpriseConfig.setIdentity(identity);
            enterpriseConfig.setPassword(password);
            this.identity = identity;
            this.password = password;
            return this;
        }

        public EnterpriseConfig setCaCerts(X509Certificate[] certs) {
            enterpriseConfig.setCaCertificates(certs);
            caCerts = certs;
            return this;
        }
    }
}
