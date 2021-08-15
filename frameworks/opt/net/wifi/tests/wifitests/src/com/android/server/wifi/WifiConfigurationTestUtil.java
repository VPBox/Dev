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

import android.net.IpConfiguration;
import android.net.LinkAddress;
import android.net.NetworkUtils;
import android.net.ProxyInfo;
import android.net.StaticIpConfiguration;
import android.net.wifi.WifiConfiguration;
import android.net.wifi.WifiConfiguration.NetworkSelectionStatus;
import android.net.wifi.WifiEnterpriseConfig;
import android.net.wifi.WifiSsid;
import android.text.TextUtils;

import java.net.InetAddress;
import java.security.cert.X509Certificate;
import java.util.Arrays;
import java.util.List;

/**
 * Helper for creating and populating WifiConfigurations in unit tests.
 */
public class WifiConfigurationTestUtil {
    /**
     * These values are used to describe AP's security setting. One AP can support multiple of them,
     * only if there is no conflict.
     */
    public static final int SECURITY_NONE = 0;
    public static final int SECURITY_WEP =  1 << 0;
    public static final int SECURITY_PSK =  1 << 1;
    public static final int SECURITY_EAP =  1 << 2;
    public static final int SECURITY_SAE =  1 << 3;
    public static final int SECURITY_OWE =  1 << 4;
    public static final int SECURITY_EAP_SUITE_B =  1 << 5;

    /**
     * These values are used to describe ip configuration parameters for a network.
     */
    public static final int STATIC_IP_ASSIGNMENT = 0;
    public static final int DHCP_IP_ASSIGNMENT = 1;
    public static final int STATIC_PROXY_SETTING = 0;
    public static final int PAC_PROXY_SETTING = 1;
    public static final int NONE_PROXY_SETTING = 2;

    /**
     * These are constants used to generate predefined WifiConfiguration objects.
     */
    public static final int TEST_NETWORK_ID = -1;
    public static final int TEST_UID = 5;
    public static final String TEST_SSID = "WifiConfigurationTestSSID";
    public static final String TEST_PSK = "\"WifiConfigurationTestUtilPsk\"";
    public static final String[] TEST_WEP_KEYS =
            {"\"WifiConfigurationTestUtilWep1\"", "\"WifiConfigurationTestUtilWep2\"",
                    "45342312ab", "45342312ab45342312ab34ac12"};
    public static final String TEST_EAP_PASSWORD = "WifiConfigurationTestUtilEapPassword";
    public static final int TEST_WEP_TX_KEY_INDEX = 1;
    public static final String TEST_FQDN = "WifiConfigurationTestUtilFQDN";
    public static final String TEST_PROVIDER_FRIENDLY_NAME =
            "WifiConfigurationTestUtilFriendlyName";
    public static final String TEST_STATIC_IP_LINK_ADDRESS = "192.168.48.2";
    public static final int TEST_STATIC_IP_LINK_PREFIX_LENGTH = 8;
    public static final String TEST_STATIC_IP_GATEWAY_ADDRESS = "192.168.48.1";
    public static final String[] TEST_STATIC_IP_DNS_SERVER_ADDRESSES =
            new String[]{"192.168.48.1", "192.168.48.10"};
    public static final String TEST_STATIC_PROXY_HOST = "192.168.48.1";
    public static final int TEST_STATIC_PROXY_PORT = 8000;
    public static final String TEST_STATIC_PROXY_EXCLUSION_LIST = "";
    public static final String TEST_PAC_PROXY_LOCATION = "http://";
    public static final String TEST_CA_CERT_ALIAS = "WifiConfigurationTestUtilCaCertAlias";

    private static final int MAX_SSID_LENGTH = 32;
    /**
     * Index used to assign unique SSIDs for the generation of predefined WifiConfiguration objects.
     */
    private static int sNetworkIndex = 0;

    /**
     * Construct a {@link android.net.wifi.WifiConfiguration}.
     * @param networkId the configuration's networkId
     * @param uid the configuration's creator uid
     * @param ssid the configuration's ssid
     * @param shared whether the configuration is shared with other users on the device
     * @param enabled whether the configuration is enabled
     * @param fqdn the configuration's FQDN (Hotspot 2.0 only)
     * @param providerFriendlyName the configuration's provider's friendly name (Hotspot 2.0 only)
     * @return the constructed {@link android.net.wifi.WifiConfiguration}
     */
    public static WifiConfiguration generateWifiConfig(int networkId, int uid, String ssid,
            boolean shared, boolean enabled, String fqdn, String providerFriendlyName) {
        final WifiConfiguration config = new WifiConfiguration();
        config.SSID = ssid;
        config.networkId = networkId;
        config.creatorUid = uid;
        config.shared = shared;
        config.status = enabled ? WifiConfiguration.Status.ENABLED
                : WifiConfiguration.Status.DISABLED;
        config.FQDN = fqdn;
        config.providerFriendlyName = providerFriendlyName;
        return config;
    }

    /**
     * Construct a {@link android.net.wifi.WifiConfiguration}.
     * @param networkId the configuration's networkId
     * @param uid the configuration's creator uid
     * @param ssid the configuration's ssid
     * @param shared whether the configuration is shared with other users on the device
     * @param enabled whether the configuration is enabled
     * @param fqdn the configuration's FQDN (Hotspot 2.0 only)
     * @param providerFriendlyName the configuration's provider's friendly name (Hotspot 2.0 only)
     * @param security the configuration's security type
     * @return the constructed {@link android.net.wifi.WifiConfiguration}
     */
    public static WifiConfiguration generateWifiConfig(int networkId, int uid, String ssid,
            boolean shared, boolean enabled, String fqdn, String providerFriendlyName,
            int security) {
        WifiConfiguration config = generateWifiConfig(networkId, uid, ssid, shared, enabled, fqdn,
                providerFriendlyName);

        if ((security == SECURITY_NONE) || ((security & SECURITY_WEP) != 0)) {
            config.allowedKeyManagement.set(WifiConfiguration.KeyMgmt.NONE);
        } else {
            if ((security & SECURITY_PSK) != 0) {
                config.allowedKeyManagement.set(WifiConfiguration.KeyMgmt.WPA_PSK);
            }

            if ((security & SECURITY_SAE) != 0) {
                config.allowedKeyManagement.set(WifiConfiguration.KeyMgmt.SAE);
                config.requirePMF = true;
            }

            if ((security & SECURITY_OWE) != 0) {
                config.allowedKeyManagement.set(WifiConfiguration.KeyMgmt.OWE);
                config.requirePMF = true;
            }

            if ((security & SECURITY_EAP) != 0) {
                config.allowedKeyManagement.set(WifiConfiguration.KeyMgmt.WPA_EAP);
                config.allowedKeyManagement.set(WifiConfiguration.KeyMgmt.IEEE8021X);
                config.enterpriseConfig.setEapMethod(WifiEnterpriseConfig.Eap.TTLS);
            }

            if ((security & SECURITY_EAP_SUITE_B) != 0) {
                config.allowedKeyManagement.set(WifiConfiguration.KeyMgmt.SUITE_B_192);
                config.requirePMF = true;
            }

        }
        return config;
    }

    /**
     * Construct a {@link android.net.IpConfiguration }.
     * @param ipAssignmentType One of {@link #STATIC_IP_ASSIGNMENT} or {@link #DHCP_IP_ASSIGNMENT}.
     * @param proxySettingType One of {@link #STATIC_PROXY_SETTING} or {@link #PAC_PROXY_SETTING} or
     *                        {@link #NONE_PROXY_SETTING}.
     * @param linkAddress static ip address string.
     * @param linkPrefixLength static ip address prefix length.
     * @param gatewayAddress static gateway address.
     * @param dnsServerAddresses list of dns servers for static ip configuration.
     * @param proxyHost Static proxy server address.
     * @param proxyPort Static proxy server port.
     * @param proxyExclusionList Static proxy exclusion list.
     * @param pacProxyPath Pac proxy server path.
     * @return the constructed {@link android.net.IpConfiguration}
     */
    public static IpConfiguration generateIpConfig(
            int ipAssignmentType, int proxySettingType, String linkAddress, int linkPrefixLength,
            String gatewayAddress, String[] dnsServerAddresses, String proxyHost,
            int proxyPort, String proxyExclusionList, String pacProxyPath) {
        StaticIpConfiguration staticIpConfiguration = null;
        ProxyInfo proxyInfo = null;
        IpConfiguration.IpAssignment ipAssignment = IpConfiguration.IpAssignment.UNASSIGNED;
        IpConfiguration.ProxySettings proxySettings = IpConfiguration.ProxySettings.UNASSIGNED;

        if (ipAssignmentType == STATIC_IP_ASSIGNMENT) {
            staticIpConfiguration = new StaticIpConfiguration();
            if (!TextUtils.isEmpty(linkAddress)) {
                LinkAddress linkAddr =
                        new LinkAddress(
                                NetworkUtils.numericToInetAddress(linkAddress), linkPrefixLength);
                staticIpConfiguration.ipAddress = linkAddr;
            }

            if (!TextUtils.isEmpty(gatewayAddress)) {
                InetAddress gatewayAddr =
                        NetworkUtils.numericToInetAddress(gatewayAddress);
                staticIpConfiguration.gateway = gatewayAddr;
            }
            if (dnsServerAddresses != null) {
                for (String dnsServerAddress : dnsServerAddresses) {
                    if (!TextUtils.isEmpty(dnsServerAddress)) {
                        staticIpConfiguration.dnsServers.add(
                                NetworkUtils.numericToInetAddress(dnsServerAddress));
                    }

                }
            }
            ipAssignment = IpConfiguration.IpAssignment.STATIC;
        } else if (ipAssignmentType == DHCP_IP_ASSIGNMENT) {
            ipAssignment = IpConfiguration.IpAssignment.DHCP;
        }

        if (proxySettingType == STATIC_PROXY_SETTING) {
            proxyInfo = new ProxyInfo(proxyHost, proxyPort, proxyExclusionList);
            proxySettings = IpConfiguration.ProxySettings.STATIC;
        } else if (proxySettingType == PAC_PROXY_SETTING) {
            proxyInfo = new ProxyInfo(pacProxyPath);
            proxySettings = IpConfiguration.ProxySettings.PAC;
        } else if (proxySettingType == NONE_PROXY_SETTING) {
            proxySettings = IpConfiguration.ProxySettings.NONE;
        }
        return new IpConfiguration(ipAssignment, proxySettings, staticIpConfiguration, proxyInfo);
    }

    /**
     * Create a new SSID for the the network being created.
     */
    private static String createNewSSID() {
        String ssid = TEST_SSID + sNetworkIndex++;
        assertTrue(ssid.length() <= MAX_SSID_LENGTH);
        return "\"" + ssid + "\"";
    }

    /**
     * Helper methods to generate predefined WifiConfiguration objects of the required type. These
     * use a static index to avoid duplicate configurations.
     */
    public static WifiConfiguration createOweNetwork() {
        return createOweNetwork(createNewSSID());
    }

    public static WifiConfiguration createOweNetwork(String ssid) {
        return generateWifiConfig(TEST_NETWORK_ID, TEST_UID, ssid, true, true, null,
                null, SECURITY_OWE);
    }

    public static WifiConfiguration createOpenNetwork() {
        return createOpenNetwork(createNewSSID());
    }

    public static WifiConfiguration createOpenNetwork(String ssid) {
        return generateWifiConfig(TEST_NETWORK_ID, TEST_UID, ssid, true, true, null,
                null, SECURITY_NONE);
    }

    public static WifiConfiguration createEphemeralNetwork() {
        WifiConfiguration configuration = createOpenNetwork();
        configuration.ephemeral = true;
        return configuration;
    }

    public static WifiConfiguration createOpenHiddenNetwork() {
        WifiConfiguration configuration = createOpenNetwork();
        configuration.hiddenSSID = true;
        return configuration;
    }

    public static WifiConfiguration createSaeNetwork() {
        WifiConfiguration configuration =
                generateWifiConfig(TEST_NETWORK_ID, TEST_UID, createNewSSID(), true, true, null,
                        null, SECURITY_SAE);

        // SAE password uses the same member.
        configuration.preSharedKey = TEST_PSK;
        configuration.requirePMF = true;
        return configuration;
    }

    public static WifiConfiguration createPskNetwork() {
        WifiConfiguration configuration =
                generateWifiConfig(TEST_NETWORK_ID, TEST_UID, createNewSSID(), true, true, null,
                        null, SECURITY_PSK);
        configuration.preSharedKey = TEST_PSK;
        return configuration;
    }

    public static WifiConfiguration createPskNetwork(String ssid) {
        WifiConfiguration configuration =
                generateWifiConfig(TEST_NETWORK_ID, TEST_UID, ssid, true, true, null,
                        null, SECURITY_PSK);
        configuration.preSharedKey = TEST_PSK;
        return configuration;
    }

    public static WifiConfiguration createSaeNetwork(String ssid) {
        WifiConfiguration configuration =
                generateWifiConfig(TEST_NETWORK_ID, TEST_UID, ssid, true, true, null,
                        null, SECURITY_SAE);
        return configuration;
    }

    public static WifiConfiguration createPskHiddenNetwork() {
        WifiConfiguration configuration = createPskNetwork();
        configuration.hiddenSSID = true;
        return configuration;
    }

    public static WifiConfiguration createWepNetwork() {
        WifiConfiguration configuration =
                generateWifiConfig(TEST_NETWORK_ID, TEST_UID, createNewSSID(), true, true, null,
                        null, SECURITY_WEP);
        configuration.wepKeys = TEST_WEP_KEYS;
        configuration.wepTxKeyIndex = TEST_WEP_TX_KEY_INDEX;
        return configuration;
    }

    public static WifiConfiguration createWepHiddenNetwork() {
        WifiConfiguration configuration = createWepNetwork();
        configuration.hiddenSSID = true;
        return configuration;
    }

    public static WifiConfiguration createWepNetworkWithSingleKey() {
        WifiConfiguration configuration =
                generateWifiConfig(TEST_NETWORK_ID, TEST_UID, createNewSSID(), true, true, null,
                        null, SECURITY_WEP);
        configuration.wepKeys[0] = TEST_WEP_KEYS[0];
        configuration.wepTxKeyIndex = 0;
        return configuration;
    }


    public static WifiConfiguration createEapNetwork() {
        WifiConfiguration configuration =
                generateWifiConfig(TEST_NETWORK_ID, TEST_UID, createNewSSID(), true, true,
                        null, null, SECURITY_EAP);
        return configuration;
    }

    public static WifiConfiguration createEapNetwork(String ssid) {
        WifiConfiguration configuration =
                generateWifiConfig(TEST_NETWORK_ID, TEST_UID, ssid, true, true,
                        null, null, SECURITY_EAP);
        return configuration;
    }


    public static WifiConfiguration createEapNetwork(int eapMethod, int phase2Method) {
        WifiConfiguration configuration =
                generateWifiConfig(TEST_NETWORK_ID, TEST_UID, createNewSSID(), true, true,
                        null, null, SECURITY_EAP);
        configuration.enterpriseConfig.setEapMethod(eapMethod);
        configuration.enterpriseConfig.setPhase2Method(phase2Method);
        return configuration;
    }

    public static WifiConfiguration createEapSuiteBNetwork() {
        WifiConfiguration configuration =
                generateWifiConfig(TEST_NETWORK_ID, TEST_UID, createNewSSID(), true, true,
                        null, null, SECURITY_EAP_SUITE_B);

        configuration.requirePMF = true;
        configuration.enterpriseConfig.setEapMethod(WifiEnterpriseConfig.Eap.TLS);
        configuration.enterpriseConfig.setPhase2Method(WifiEnterpriseConfig.Phase2.NONE);
        return configuration;
    }

    public static WifiConfiguration createPasspointNetwork() {
        WifiConfiguration configuration =
                generateWifiConfig(TEST_NETWORK_ID, TEST_UID, createNewSSID(), true, true,
                        TEST_FQDN, TEST_PROVIDER_FRIENDLY_NAME, SECURITY_EAP);
        return configuration;
    }

    public static IpConfiguration createStaticIpConfigurationWithPacProxy() {
        return generateIpConfig(
                STATIC_IP_ASSIGNMENT, PAC_PROXY_SETTING,
                TEST_STATIC_IP_LINK_ADDRESS, TEST_STATIC_IP_LINK_PREFIX_LENGTH,
                TEST_STATIC_IP_GATEWAY_ADDRESS, TEST_STATIC_IP_DNS_SERVER_ADDRESSES,
                TEST_STATIC_PROXY_HOST, TEST_STATIC_PROXY_PORT, TEST_STATIC_PROXY_EXCLUSION_LIST,
                TEST_PAC_PROXY_LOCATION);
    }

    public static IpConfiguration createStaticIpConfigurationWithStaticProxy() {
        return generateIpConfig(
                STATIC_IP_ASSIGNMENT, STATIC_PROXY_SETTING,
                TEST_STATIC_IP_LINK_ADDRESS, TEST_STATIC_IP_LINK_PREFIX_LENGTH,
                TEST_STATIC_IP_GATEWAY_ADDRESS, TEST_STATIC_IP_DNS_SERVER_ADDRESSES,
                TEST_STATIC_PROXY_HOST, TEST_STATIC_PROXY_PORT, TEST_STATIC_PROXY_EXCLUSION_LIST,
                TEST_PAC_PROXY_LOCATION);
    }

    public static IpConfiguration createPartialStaticIpConfigurationWithPacProxy() {
        return generateIpConfig(
                STATIC_IP_ASSIGNMENT, PAC_PROXY_SETTING,
                TEST_STATIC_IP_LINK_ADDRESS, TEST_STATIC_IP_LINK_PREFIX_LENGTH,
                null, null,
                TEST_STATIC_PROXY_HOST, TEST_STATIC_PROXY_PORT, TEST_STATIC_PROXY_EXCLUSION_LIST,
                TEST_PAC_PROXY_LOCATION);
    }

    public static IpConfiguration createDHCPIpConfigurationWithPacProxy() {
        return generateIpConfig(
                DHCP_IP_ASSIGNMENT, PAC_PROXY_SETTING,
                TEST_STATIC_IP_LINK_ADDRESS, TEST_STATIC_IP_LINK_PREFIX_LENGTH,
                TEST_STATIC_IP_GATEWAY_ADDRESS, TEST_STATIC_IP_DNS_SERVER_ADDRESSES,
                TEST_STATIC_PROXY_HOST, TEST_STATIC_PROXY_PORT, TEST_STATIC_PROXY_EXCLUSION_LIST,
                TEST_PAC_PROXY_LOCATION);
    }

    public static IpConfiguration createDHCPIpConfigurationWithStaticProxy() {
        return generateIpConfig(
                DHCP_IP_ASSIGNMENT, STATIC_PROXY_SETTING,
                TEST_STATIC_IP_LINK_ADDRESS, TEST_STATIC_IP_LINK_PREFIX_LENGTH,
                TEST_STATIC_IP_GATEWAY_ADDRESS, TEST_STATIC_IP_DNS_SERVER_ADDRESSES,
                TEST_STATIC_PROXY_HOST, TEST_STATIC_PROXY_PORT, TEST_STATIC_PROXY_EXCLUSION_LIST,
                TEST_PAC_PROXY_LOCATION);
    }

    public static IpConfiguration createDHCPIpConfigurationWithNoProxy() {
        return generateIpConfig(
                DHCP_IP_ASSIGNMENT, NONE_PROXY_SETTING,
                TEST_STATIC_IP_LINK_ADDRESS, TEST_STATIC_IP_LINK_PREFIX_LENGTH,
                TEST_STATIC_IP_GATEWAY_ADDRESS, TEST_STATIC_IP_DNS_SERVER_ADDRESSES,
                TEST_STATIC_PROXY_HOST, TEST_STATIC_PROXY_PORT, TEST_STATIC_PROXY_EXCLUSION_LIST,
                TEST_PAC_PROXY_LOCATION);
    }

    /**
     * Creates an IP configuration with specific parameters.
     * @param proxySetting Must be one of {@link WifiConfigurationTestUtil#STATIC_PROXY_SETTING},
     * {@link WifiConfigurationTestUtil#PAC_PROXY_SETTING},
     * {@link WifiConfigurationTestUtil#NONE_PROXY_SETTING}
     */
    public static IpConfiguration createDHCPIpConfigurationWithSpecificProxy(
            int proxySetting,
            String staticProxyHost,
            int staticProxyPort,
            String staticProxyExclusionList,
            String pacProxyLocation) {
        return generateIpConfig(
                DHCP_IP_ASSIGNMENT, proxySetting,
                TEST_STATIC_IP_LINK_ADDRESS, TEST_STATIC_IP_LINK_PREFIX_LENGTH,
                TEST_STATIC_IP_GATEWAY_ADDRESS, TEST_STATIC_IP_DNS_SERVER_ADDRESSES,
                staticProxyHost, staticProxyPort, staticProxyExclusionList,
                pacProxyLocation);
    }

    // TODO: These enterprise configurations may need more parameters set.
    public static WifiEnterpriseConfig createPEAPWifiEnterpriseConfigWithGTCPhase2() {
        WifiEnterpriseConfig config = new WifiEnterpriseConfig();
        config.setEapMethod(WifiEnterpriseConfig.Eap.PEAP);
        config.setPhase2Method(WifiEnterpriseConfig.Phase2.GTC);
        config.setCaCertificateAliases(new String[] {TEST_CA_CERT_ALIAS + "PEAP"});
        config.setCaCertificates(new X509Certificate[] {FakeKeys.CA_CERT0, FakeKeys.CA_CERT1});
        return config;
    }

    public static WifiEnterpriseConfig createTLSWifiEnterpriseConfigWithNonePhase2() {
        WifiEnterpriseConfig config = new WifiEnterpriseConfig();
        config.setEapMethod(WifiEnterpriseConfig.Eap.TLS);
        config.setPhase2Method(WifiEnterpriseConfig.Phase2.NONE);
        config.setCaCertificateAliases(new String[] {TEST_CA_CERT_ALIAS + "TLS"});
        config.setCaCertificates(new X509Certificate[] {FakeKeys.CA_CERT0, FakeKeys.CA_CERT1});
        return config;
    }

    public static WifiEnterpriseConfig createTLSWifiEnterpriseConfigWithAkaPhase2() {
        WifiEnterpriseConfig config = new WifiEnterpriseConfig();
        config.setEapMethod(WifiEnterpriseConfig.Eap.TLS);
        config.setPhase2Method(WifiEnterpriseConfig.Phase2.AKA);
        return config;
    }

    /**
     * Gets scan result capabilities for a particular network configuration.
     */
    public static String getScanResultCapsForNetwork(WifiConfiguration configuration) {
        String caps;
        if (configuration.allowedKeyManagement.get(WifiConfiguration.KeyMgmt.WPA_PSK)) {
            caps = "[RSN-PSK-CCMP]";
        } else if (configuration.allowedKeyManagement.get(WifiConfiguration.KeyMgmt.WPA_EAP)
                || configuration.allowedKeyManagement.get(WifiConfiguration.KeyMgmt.IEEE8021X)) {
            caps = "[RSN-EAP-CCMP]";
        } else if (configuration.allowedKeyManagement.get(WifiConfiguration.KeyMgmt.NONE)
                && WifiConfigurationUtil.hasAnyValidWepKey(configuration.wepKeys)) {
            caps = "[WEP]";
        } else if (configuration.allowedKeyManagement.get(WifiConfiguration.KeyMgmt.SAE)) {
            caps = "[RSN-SAE-CCMP]";
        } else if (configuration.allowedKeyManagement.get(WifiConfiguration.KeyMgmt.OWE)) {
            caps = "[RSN-OWE-CCMP]";
        } else if (configuration.allowedKeyManagement.get(WifiConfiguration.KeyMgmt.SUITE_B_192)) {
            caps = "[RSN-SUITE-B-192-CCMP]";
        } else {
            caps = "[]";
        }
        return caps;
    }

    /**
     * Gets scan result capabilities for a WPA2/WPA3-Transition mode network configuration
     */
    private static String
            getScanResultCapsForWpa2Wpa3TransitionNetwork(WifiConfiguration configuration) {
        String caps = "[RSN-PSK+SAE-CCMP]";
        return caps;
    }

    /**
     * Creates a scan detail corresponding to the provided network and given BSSID, etc.
     */
    public static ScanDetail createScanDetailForNetwork(
            WifiConfiguration configuration, String bssid, int level, int frequency,
            long tsf, long seen) {
        String caps = getScanResultCapsForNetwork(configuration);
        WifiSsid ssid = WifiSsid.createFromAsciiEncoded(configuration.getPrintableSsid());
        return new ScanDetail(ssid, bssid, caps, level, frequency, tsf, seen);
    }

    /**
     * Creates a scan detail corresponding to the provided network and given BSSID, but sets
     * the capabilities to WPA2/WPA3-Transition mode network.
     */
    public static ScanDetail createScanDetailForWpa2Wpa3TransitionModeNetwork(
            WifiConfiguration configuration, String bssid, int level, int frequency,
            long tsf, long seen) {
        String caps = getScanResultCapsForWpa2Wpa3TransitionNetwork(configuration);
        WifiSsid ssid = WifiSsid.createFromAsciiEncoded(configuration.getPrintableSsid());
        return new ScanDetail(ssid, bssid, caps, level, frequency, tsf, seen);
    }


    /**
     * Asserts that the 2 WifiConfigurations are equal in the elements saved for both backup/restore
     * and config store.
     */
    private static void assertCommonConfigurationElementsEqual(
            WifiConfiguration expected, WifiConfiguration actual) {
        assertNotNull(expected);
        assertNotNull(actual);
        assertEquals(expected.SSID, actual.SSID);
        assertEquals(expected.BSSID, actual.BSSID);
        assertEquals(expected.preSharedKey, actual.preSharedKey);
        assertEquals(expected.wepKeys, actual.wepKeys);
        assertEquals(expected.wepTxKeyIndex, actual.wepTxKeyIndex);
        assertEquals(expected.hiddenSSID, actual.hiddenSSID);
        assertEquals(expected.requirePMF, actual.requirePMF);
        assertEquals(expected.allowedKeyManagement, actual.allowedKeyManagement);
        assertEquals(expected.allowedProtocols, actual.allowedProtocols);
        assertEquals(expected.allowedAuthAlgorithms, actual.allowedAuthAlgorithms);
        assertEquals(expected.allowedGroupCiphers, actual.allowedGroupCiphers);
        assertEquals(expected.allowedPairwiseCiphers, actual.allowedPairwiseCiphers);
        assertEquals(expected.shared, actual.shared);
        assertEquals(expected.getIpConfiguration(), actual.getIpConfiguration());
    }

    /**
     * Asserts that the 2 WifiConfigurations are equal. This only compares the elements saved
     * for backup/restore.
     */
    public static void assertConfigurationEqualForBackup(
            WifiConfiguration expected, WifiConfiguration actual) {
        assertCommonConfigurationElementsEqual(expected, actual);
        assertEquals(expected.meteredOverride, actual.meteredOverride);
    }

    /**
     * Asserts that the 2 WifiConfigurations are equal. This compares all the elements saved for
     * config store.
     */
    public static void assertConfigurationEqualForConfigStore(
            WifiConfiguration expected, WifiConfiguration actual) {
        assertCommonConfigurationElementsEqual(expected, actual);
        assertEquals(expected.status, actual.status);
        assertEquals(expected.FQDN, actual.FQDN);
        assertEquals(expected.providerFriendlyName, actual.providerFriendlyName);
        assertTrue(Arrays.equals(expected.roamingConsortiumIds, actual.roamingConsortiumIds));
        assertEquals(expected.linkedConfigurations, actual.linkedConfigurations);
        assertEquals(expected.defaultGwMacAddress, actual.defaultGwMacAddress);
        assertEquals(expected.validatedInternetAccess, actual.validatedInternetAccess);
        assertEquals(expected.noInternetAccessExpected, actual.noInternetAccessExpected);
        assertEquals(expected.userApproved, actual.userApproved);
        assertEquals(expected.meteredHint, actual.meteredHint);
        assertEquals(expected.meteredOverride, actual.meteredOverride);
        assertEquals(expected.useExternalScores, actual.useExternalScores);
        assertEquals(expected.numAssociation, actual.numAssociation);
        assertEquals(expected.creatorUid, actual.creatorUid);
        assertEquals(expected.creatorName, actual.creatorName);
        assertEquals(expected.creationTime, actual.creationTime);
        assertEquals(expected.lastUpdateUid, actual.lastUpdateUid);
        assertEquals(expected.lastUpdateName, actual.lastUpdateName);
        assertEquals(expected.lastConnectUid, actual.lastConnectUid);
        assertEquals(expected.updateTime, actual.updateTime);
        assertEquals(expected.isLegacyPasspointConfig, actual.isLegacyPasspointConfig);
        assertEquals(expected.getRandomizedMacAddress(), actual.getRandomizedMacAddress());
        assertEquals(expected.macRandomizationSetting, actual.macRandomizationSetting);
        assertNetworkSelectionStatusEqualForConfigStore(
                expected.getNetworkSelectionStatus(), actual.getNetworkSelectionStatus());
        assertWifiEnterpriseConfigEqualForConfigStore(
                expected.enterpriseConfig, actual.enterpriseConfig);
    }

    /**
     * Asserts that the 2 WifiConfigurations are equal. This compares all the elements that are
     * saved into internal database by WifiConfigurationManager for network additions/updates.
     */
    public static void assertConfigurationEqualForConfigManagerAddOrUpdate(
            WifiConfiguration expected, WifiConfiguration actual) {
        assertCommonConfigurationElementsEqual(expected, actual);
        assertEquals(expected.FQDN, actual.FQDN);
        assertEquals(expected.providerFriendlyName, actual.providerFriendlyName);
        assertEquals(expected.noInternetAccessExpected, actual.noInternetAccessExpected);
        assertEquals(expected.meteredHint, actual.meteredHint);
        assertEquals(expected.meteredOverride, actual.meteredOverride);
        assertEquals(expected.useExternalScores, actual.useExternalScores);
        assertEquals(expected.ephemeral, actual.ephemeral);
        assertEquals(expected.osu, actual.osu);
        assertEquals(expected.trusted, actual.trusted);
        assertEquals(expected.fromWifiNetworkSuggestion, actual.fromWifiNetworkSuggestion);
        assertEquals(expected.fromWifiNetworkSpecifier, actual.fromWifiNetworkSpecifier);
        assertEquals(expected.creatorUid, actual.creatorUid);
        assertEquals(expected.creatorName, actual.creatorName);
        assertEquals(expected.creationTime, actual.creationTime);
        assertEquals(expected.lastUpdateUid, actual.lastUpdateUid);
        assertEquals(expected.lastUpdateName, actual.lastUpdateName);
        assertEquals(expected.updateTime, actual.updateTime);
        assertNetworkSelectionStatusEqualForConfigStore(
                expected.getNetworkSelectionStatus(), actual.getNetworkSelectionStatus());
        assertWifiEnterpriseConfigEqualForConfigStore(
                expected.enterpriseConfig, actual.enterpriseConfig);
    }

    /**
     * Asserts that the 2 WifiConfigurations are equal. This compares all the elements that are
     * saved into wpa_supplicant by SupplicantStaNetwork.
     */
    public static void assertConfigurationEqualForSupplicant(
            WifiConfiguration expected, WifiConfiguration actual) {
        assertNotNull(expected);
        assertNotNull(actual);
        assertEquals(expected.SSID, actual.SSID);
        assertEquals(expected.getNetworkSelectionStatus().getNetworkSelectionBSSID(),
                actual.getNetworkSelectionStatus().getNetworkSelectionBSSID());
        assertEquals(expected.preSharedKey, actual.preSharedKey);
        assertEquals(expected.wepKeys, actual.wepKeys);
        assertEquals(expected.wepTxKeyIndex, actual.wepTxKeyIndex);
        assertEquals(expected.hiddenSSID, actual.hiddenSSID);
        assertEquals(expected.requirePMF, actual.requirePMF);
        assertEquals(expected.allowedKeyManagement, actual.allowedKeyManagement);
        assertEquals(expected.allowedProtocols, actual.allowedProtocols);
        assertEquals(expected.allowedAuthAlgorithms, actual.allowedAuthAlgorithms);
        assertEquals(expected.allowedGroupCiphers, actual.allowedGroupCiphers);
        assertEquals(expected.allowedPairwiseCiphers, actual.allowedPairwiseCiphers);
        assertWifiEnterpriseConfigEqualForConfigStore(
                expected.enterpriseConfig, actual.enterpriseConfig);
    }

    /**
     * Asserts that the 2 WifiConfigurations are equal. This is a generic version of the comparator
     * which is used in QNS tests for comparing the network selections.
     * This importantly checks that the networkId's of the 2 configs are equal.
     */
    public static void assertConfigurationEqual(
            WifiConfiguration expected, WifiConfiguration actual) {
        assertCommonConfigurationElementsEqual(expected, actual);
        assertEquals(expected.networkId, actual.networkId);
        assertEquals(expected.ephemeral, actual.ephemeral);
        assertEquals(expected.fromWifiNetworkSuggestion, actual.fromWifiNetworkSuggestion);
        assertEquals(expected.fromWifiNetworkSpecifier, actual.fromWifiNetworkSpecifier);
        assertEquals(expected.trusted, actual.trusted);
    }

    /**
     * Assert that the 2 NetworkSelectionStatus's are equal. This compares all the elements saved
     * for config store.
     */
    public static void assertNetworkSelectionStatusEqualForConfigStore(
            NetworkSelectionStatus expected, NetworkSelectionStatus actual) {
        if (expected.isNetworkTemporaryDisabled()) {
            // Temporarily disabled networks are enabled when persisted.
            assertEquals(
                    NetworkSelectionStatus.NETWORK_SELECTION_ENABLED,
                    actual.getNetworkSelectionStatus());
            assertEquals(
                    NetworkSelectionStatus.NETWORK_SELECTION_ENABLE,
                    actual.getNetworkSelectionDisableReason());
        } else {
            assertEquals(expected.getNetworkSelectionStatus(), actual.getNetworkSelectionStatus());
            assertEquals(
                    expected.getNetworkSelectionDisableReason(),
                    actual.getNetworkSelectionDisableReason());
        }
        assertEquals(expected.getConnectChoice(), actual.getConnectChoice());
        assertEquals(expected.getConnectChoiceTimestamp(), actual.getConnectChoiceTimestamp());
        assertEquals(expected.getHasEverConnected(), actual.getHasEverConnected());
    }

    /**
     * Assert that the 2 WifiEnterpriseConfig's are equal. This compares all the elements saved
     * for config store.
     */
    public static void assertWifiEnterpriseConfigEqualForConfigStore(
            WifiEnterpriseConfig expected, WifiEnterpriseConfig actual) {
        assertEquals(expected.getFieldValue(WifiEnterpriseConfig.IDENTITY_KEY),
                actual.getFieldValue(WifiEnterpriseConfig.IDENTITY_KEY));
        assertEquals(expected.getFieldValue(WifiEnterpriseConfig.ANON_IDENTITY_KEY),
                actual.getFieldValue(WifiEnterpriseConfig.ANON_IDENTITY_KEY));
        assertEquals(expected.getFieldValue(WifiEnterpriseConfig.PASSWORD_KEY),
                actual.getFieldValue(WifiEnterpriseConfig.PASSWORD_KEY));
        assertEquals(expected.getFieldValue(WifiEnterpriseConfig.CLIENT_CERT_KEY),
                actual.getFieldValue(WifiEnterpriseConfig.CLIENT_CERT_KEY));
        assertEquals(expected.getFieldValue(WifiEnterpriseConfig.CA_CERT_KEY),
                actual.getFieldValue(WifiEnterpriseConfig.CA_CERT_KEY));
        assertEquals(expected.getFieldValue(WifiEnterpriseConfig.SUBJECT_MATCH_KEY),
                actual.getFieldValue(WifiEnterpriseConfig.SUBJECT_MATCH_KEY));
        assertEquals(expected.getFieldValue(WifiEnterpriseConfig.ENGINE_KEY),
                actual.getFieldValue(WifiEnterpriseConfig.ENGINE_KEY));
        assertEquals(expected.getFieldValue(WifiEnterpriseConfig.ENGINE_ID_KEY),
                actual.getFieldValue(WifiEnterpriseConfig.ENGINE_ID_KEY));
        assertEquals(expected.getFieldValue(WifiEnterpriseConfig.PRIVATE_KEY_ID_KEY),
                actual.getFieldValue(WifiEnterpriseConfig.PRIVATE_KEY_ID_KEY));
        assertEquals(expected.getFieldValue(WifiEnterpriseConfig.ALTSUBJECT_MATCH_KEY),
                actual.getFieldValue(WifiEnterpriseConfig.ALTSUBJECT_MATCH_KEY));
        assertEquals(expected.getFieldValue(WifiEnterpriseConfig.DOM_SUFFIX_MATCH_KEY),
                actual.getFieldValue(WifiEnterpriseConfig.DOM_SUFFIX_MATCH_KEY));
        assertEquals(expected.getFieldValue(WifiEnterpriseConfig.CA_PATH_KEY),
                actual.getFieldValue(WifiEnterpriseConfig.CA_PATH_KEY));
        assertEquals(expected.getFieldValue(WifiEnterpriseConfig.REALM_KEY),
                actual.getFieldValue(WifiEnterpriseConfig.REALM_KEY));
        assertEquals(expected.getFieldValue(WifiEnterpriseConfig.PLMN_KEY),
                actual.getFieldValue(WifiEnterpriseConfig.PLMN_KEY));
        assertEquals(expected.getEapMethod(), actual.getEapMethod());
        assertEquals(expected.getPhase2Method(), actual.getPhase2Method());
    }

    /**
     * Asserts that the 2 lists of WifiConfigurations are equal. This compares all the elements
     * saved for backup/restore.
     */
    public static void assertConfigurationsEqualForBackup(
            List<WifiConfiguration> expected, List<WifiConfiguration> actual) {
        assertEquals(expected.size(), actual.size());
        for (WifiConfiguration expectedConfiguration : expected) {
            String expectedConfigKey = expectedConfiguration.configKey();
            boolean didCompare = false;
            for (WifiConfiguration actualConfiguration : actual) {
                String actualConfigKey = actualConfiguration.configKey();
                if (actualConfigKey.equals(expectedConfigKey)) {
                    assertConfigurationEqualForBackup(
                            expectedConfiguration, actualConfiguration);
                    didCompare = true;
                }
            }
            assertTrue(didCompare);
        }
    }

    /**
     * Asserts that the 2 lists of WifiConfigurations are equal. This compares all the elements
     * that are saved into internal database by WifiConfigurationManager for network
     * additions/updates.
     */
    public static void assertConfigurationsEqualForConfigManagerAddOrUpdate(
            List<WifiConfiguration> expected, List<WifiConfiguration> actual) {
        assertEquals(expected.size(), actual.size());
        for (WifiConfiguration expectedConfiguration : expected) {
            String expectedConfigKey = expectedConfiguration.configKey();
            boolean didCompare = false;
            for (WifiConfiguration actualConfiguration : actual) {
                String actualConfigKey = actualConfiguration.configKey();
                if (actualConfigKey.equals(expectedConfigKey)) {
                    assertConfigurationEqualForConfigManagerAddOrUpdate(
                            expectedConfiguration, actualConfiguration);
                    didCompare = true;
                }
            }
            assertTrue(didCompare);
        }
    }

    /**
     * Asserts that the 2 lists of WifiConfigurations are equal. This compares all the elements
     * saved for config store.
     */
    public static void assertConfigurationsEqualForConfigStore(
            List<WifiConfiguration> expected, List<WifiConfiguration> actual) {
        assertEquals(expected.size(), actual.size());
        for (WifiConfiguration expectedConfiguration : expected) {
            String expectedConfigKey = expectedConfiguration.configKey();
            boolean didCompare = false;
            for (WifiConfiguration actualConfiguration : actual) {
                String actualConfigKey = actualConfiguration.configKey();
                if (actualConfigKey.equals(expectedConfigKey)) {
                    assertConfigurationEqualForConfigStore(
                            expectedConfiguration, actualConfiguration);
                    didCompare = true;
                }
            }
            assertTrue(didCompare);
        }
    }
}
