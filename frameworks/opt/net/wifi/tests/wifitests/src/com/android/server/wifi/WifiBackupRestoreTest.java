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
import static org.mockito.Mockito.*;

import android.net.IpConfiguration;
import android.net.wifi.WifiConfiguration;
import android.os.Process;

import androidx.test.filters.SmallTest;

import com.android.server.net.IpConfigStore;
import com.android.server.wifi.util.WifiPermissionsUtil;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.mockito.Mock;
import org.mockito.MockitoAnnotations;

import java.io.ByteArrayOutputStream;
import java.io.DataOutputStream;
import java.io.FileDescriptor;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.io.StringWriter;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Random;

/**
 * Unit tests for {@link com.android.server.wifi.WifiBackupRestore}.
 */
@SmallTest
public class WifiBackupRestoreTest {

    private static final String WIFI_BACKUP_DATA_WITH_UNSUPPORTED_TAG =
            "<?xml version='1.0' encoding='utf-8' standalone='yes' ?>"
            + "<WifiBackupData>"
            + "<int name=\"Version\" value=\"1\" />"
            + "<NetworkList>"
            + "<Network>"
            + "<WifiConfiguration>"
            + "<string name=\"ConfigKey\">&quot;GoogleGuest-Legacy&quot;NONE</string>"
            + "<string name=\"SSID\">&quot;GoogleGuest-Legacy&quot;</string>"
            + "<null name=\"BSSID\" />"
            + "<null name=\"PreSharedKey\" />"
            + "<null name=\"WEPKeys\" />"
            + "<int name=\"WEPTxKeyIndex\" value=\"0\" />"
            + "<boolean name=\"HiddenSSID\" value=\"false\" />"
            + "<boolean name=\"RequirePMF\" value=\"false\" />"
            + "<byte-array name=\"AllowedKeyMgmt\" num=\"1\">01</byte-array>"
            + "<byte-array name=\"AllowedProtocols\" num=\"1\">03</byte-array>"
            + "<byte-array name=\"AllowedAuthAlgos\" num=\"1\">01</byte-array>"
            + "<byte-array name=\"AllowedGroupCiphers\" num=\"1\">0f</byte-array>"
            + "<byte-array name=\"AllowedPairwiseCiphers\" num=\"1\">06</byte-array>"
            + "<boolean name=\"Shared\" value=\"true\" />"
            + "<null name=\"SimSlot\" />"
            + "</WifiConfiguration>"
            + "<IpConfiguration>"
            + "<string name=\"IpAssignment\">DHCP</string>"
            + "<string name=\"ProxySettings\">NONE</string>"
            + "</IpConfiguration>"
            + "</Network>"
            + "</NetworkList>"
            + "</WifiBackupData>";

    // |AllowedKeyMgmt|, |AllowedProtocols|, |AllowedAuthAlgorithms|, |AllowedGroupCiphers| and
    // |AllowedPairwiseCiphers| fields have invalid values in them.
    // NOTE: The byte values are encoded in little endian
    private static final String WIFI_BACKUP_DATA_WITH_UNSUPPORTED_VALUES_IN_BITSETS =
            "<?xml version='1.0' encoding='utf-8' standalone='yes' ?>"
                    + "<WifiBackupData>"
                    + "<int name=\"Version\" value=\"1\" />"
                    + "<NetworkList>"
                    + "<Network>"
                    + "<WifiConfiguration>"
                    + "<string name=\"ConfigKey\">&quot;GoogleGuest-Legacy&quot;NONE</string>"
                    + "<string name=\"SSID\">&quot;GoogleGuest-Legacy&quot;</string>"
                    + "<null name=\"BSSID\" />"
                    + "<null name=\"PreSharedKey\" />"
                    + "<null name=\"WEPKeys\" />"
                    + "<int name=\"WEPTxKeyIndex\" value=\"0\" />"
                    + "<boolean name=\"HiddenSSID\" value=\"false\" />"
                    + "<boolean name=\"RequirePMF\" value=\"false\" />"
                    // Valid Value: 01
                    + "<byte-array name=\"AllowedKeyMgmt\" num=\"2\">0180</byte-array>"
                    // Valid Value: 03
                    + "<byte-array name=\"AllowedProtocols\" num=\"1\">0b</byte-array>"
                    // Valid Value: 01
                    + "<byte-array name=\"AllowedAuthAlgos\" num=\"1\">09</byte-array>"
                    // Valid Value: 0f
                    + "<byte-array name=\"AllowedGroupCiphers\" num=\"1\">4f</byte-array>"
                    // Valid Value: 06
                    + "<byte-array name=\"AllowedPairwiseCiphers\" num=\"1\">26</byte-array>"
                    + "<boolean name=\"Shared\" value=\"true\" />"
                    + "<null name=\"SimSlot\" />"
                    + "</WifiConfiguration>"
                    + "<IpConfiguration>"
                    + "<string name=\"IpAssignment\">DHCP</string>"
                    + "<string name=\"ProxySettings\">NONE</string>"
                    + "</IpConfiguration>"
                    + "</Network>"
                    + "</NetworkList>"
                    + "</WifiBackupData>";

    private static final String WIFI_BACKUP_DATA_V1_0 =
            "<?xml version='1.0' encoding='utf-8' standalone='yes' ?>"
                    + "<WifiBackupData>"
                    + "<float name=\"Version\" value=\"1.0\" />"
                    + "<NetworkList>"
                    + "<Network>"
                    + "<WifiConfiguration>"
                    + "<string name=\"ConfigKey\">&quot;GoogleGuest-Legacy&quot;NONE</string>"
                    + "<string name=\"SSID\">&quot;GoogleGuest-Legacy&quot;</string>"
                    + "<null name=\"BSSID\" />"
                    + "<null name=\"PreSharedKey\" />"
                    + "<null name=\"WEPKeys\" />"
                    + "<int name=\"WEPTxKeyIndex\" value=\"0\" />"
                    + "<boolean name=\"HiddenSSID\" value=\"false\" />"
                    + "<boolean name=\"RequirePMF\" value=\"false\" />"
                    + "<byte-array name=\"AllowedKeyMgmt\" num=\"1\">01</byte-array>"
                    + "<byte-array name=\"AllowedProtocols\" num=\"1\">03</byte-array>"
                    + "<byte-array name=\"AllowedAuthAlgos\" num=\"1\">01</byte-array>"
                    + "<byte-array name=\"AllowedGroupCiphers\" num=\"1\">0f</byte-array>"
                    + "<byte-array name=\"AllowedPairwiseCiphers\" num=\"1\">06</byte-array>"
                    + "<boolean name=\"Shared\" value=\"true\" />"
                    + "</WifiConfiguration>"
                    + "<IpConfiguration>"
                    + "<string name=\"IpAssignment\">DHCP</string>"
                    + "<string name=\"ProxySettings\">NONE</string>"
                    + "</IpConfiguration>"
                    + "</Network>"
                    + "</NetworkList>"
                    + "</WifiBackupData>";

    private static final String WIFI_BACKUP_DATA_V1_1 =
            "<?xml version='1.0' encoding='utf-8' standalone='yes' ?>"
                    + "<WifiBackupData>"
                    + "<float name=\"Version\" value=\"1.1\" />"
                    + "<NetworkList>"
                    + "<Network>"
                    + "<WifiConfiguration>"
                    + "<string name=\"ConfigKey\">&quot;GoogleGuest-Legacy&quot;NONE</string>"
                    + "<string name=\"SSID\">&quot;GoogleGuest-Legacy&quot;</string>"
                    + "<null name=\"BSSID\" />"
                    + "<null name=\"PreSharedKey\" />"
                    + "<null name=\"WEPKeys\" />"
                    + "<int name=\"WEPTxKeyIndex\" value=\"0\" />"
                    + "<boolean name=\"HiddenSSID\" value=\"false\" />"
                    + "<boolean name=\"RequirePMF\" value=\"false\" />"
                    + "<byte-array name=\"AllowedKeyMgmt\" num=\"1\">01</byte-array>"
                    + "<byte-array name=\"AllowedProtocols\" num=\"1\">03</byte-array>"
                    + "<byte-array name=\"AllowedAuthAlgos\" num=\"1\">01</byte-array>"
                    + "<byte-array name=\"AllowedGroupCiphers\" num=\"1\">0f</byte-array>"
                    + "<byte-array name=\"AllowedPairwiseCiphers\" num=\"1\">06</byte-array>"
                    + "<boolean name=\"Shared\" value=\"true\" />"
                    + "<int name=\"MeteredOverride\" value=\"1\" />"
                    + "</WifiConfiguration>"
                    + "<IpConfiguration>"
                    + "<string name=\"IpAssignment\">DHCP</string>"
                    + "<string name=\"ProxySettings\">NONE</string>"
                    + "</IpConfiguration>"
                    + "</Network>"
                    + "</NetworkList>"
                    + "</WifiBackupData>";

    @Mock WifiPermissionsUtil mWifiPermissionsUtil;
    private WifiBackupRestore mWifiBackupRestore;
    private boolean mCheckDump = true;

    @Before
    public void setUp() throws Exception {
        MockitoAnnotations.initMocks(this);
        when(mWifiPermissionsUtil.checkConfigOverridePermission(anyInt())).thenReturn(true);
        mWifiBackupRestore = new WifiBackupRestore(mWifiPermissionsUtil);
        // Enable verbose logging before tests to check the backup data dumps.
        mWifiBackupRestore.enableVerboseLogging(1);
    }

    @After
    public void cleanUp() throws Exception {
        if (mCheckDump) {
            StringWriter stringWriter = new StringWriter();
            mWifiBackupRestore.dump(
                    new FileDescriptor(), new PrintWriter(stringWriter), new String[0]);
            String dumpString = stringWriter.toString();
            // Ensure that the SSID was dumped out.
            assertTrue("Dump: " + dumpString,
                    dumpString.contains(WifiConfigurationTestUtil.TEST_SSID));
            // Ensure that the password wasn't dumped out.
            assertFalse("Dump: " + dumpString,
                    dumpString.contains(WifiConfigurationTestUtil.TEST_PSK));
            assertFalse("Dump: " + dumpString,
                    dumpString.contains(WifiConfigurationTestUtil.TEST_WEP_KEYS[0]));
            assertFalse("Dump: " + dumpString,
                    dumpString.contains(WifiConfigurationTestUtil.TEST_WEP_KEYS[1]));
            assertFalse("Dump: " + dumpString,
                    dumpString.contains(WifiConfigurationTestUtil.TEST_WEP_KEYS[2]));
            assertFalse("Dump: " + dumpString,
                    dumpString.contains(WifiConfigurationTestUtil.TEST_WEP_KEYS[3]));
        }
    }

    /**
     * Verify that a null network list is serialized correctly.
     */
    @Test
    public void testNullNetworkListBackup() {
        byte[] backupData = mWifiBackupRestore.retrieveBackupDataFromConfigurations(null);
        assertTrue(backupData != null);
        assertEquals(backupData.length, 0);
        // No valid data to check in dump.
        mCheckDump = false;
    }

    /**
     * Verify that a single open network configuration is serialized & deserialized correctly.
     */
    @Test
    public void testSingleOpenNetworkBackupRestore() {
        List<WifiConfiguration> configurations = new ArrayList<>();
        configurations.add(WifiConfigurationTestUtil.createOpenNetwork());

        byte[] backupData = mWifiBackupRestore.retrieveBackupDataFromConfigurations(configurations);
        List<WifiConfiguration> retrievedConfigurations =
                mWifiBackupRestore.retrieveConfigurationsFromBackupData(backupData);
        WifiConfigurationTestUtil.assertConfigurationsEqualForBackup(
                configurations, retrievedConfigurations);
    }

    /**
     * Verify that a single open hidden network configuration is serialized & deserialized
     * correctly.
     */
    @Test
    public void testSingleOpenHiddenNetworkBackupRestore() {
        List<WifiConfiguration> configurations = new ArrayList<>();
        configurations.add(WifiConfigurationTestUtil.createOpenHiddenNetwork());

        byte[] backupData = mWifiBackupRestore.retrieveBackupDataFromConfigurations(configurations);
        List<WifiConfiguration> retrievedConfigurations =
                mWifiBackupRestore.retrieveConfigurationsFromBackupData(backupData);
        WifiConfigurationTestUtil.assertConfigurationsEqualForBackup(
                configurations, retrievedConfigurations);
    }

    /**
     * Verify that a single PSK network configuration is serialized & deserialized correctly.
     */
    @Test
    public void testSinglePskNetworkBackupRestore() {
        List<WifiConfiguration> configurations = new ArrayList<>();
        configurations.add(WifiConfigurationTestUtil.createPskNetwork());

        byte[] backupData = mWifiBackupRestore.retrieveBackupDataFromConfigurations(configurations);
        List<WifiConfiguration> retrievedConfigurations =
                mWifiBackupRestore.retrieveConfigurationsFromBackupData(backupData);
        WifiConfigurationTestUtil.assertConfigurationsEqualForBackup(
                configurations, retrievedConfigurations);
    }

    /**
     * Verify that a single PSK hidden network configuration is serialized & deserialized correctly.
     */
    @Test
    public void testSinglePskHiddenNetworkBackupRestore() {
        List<WifiConfiguration> configurations = new ArrayList<>();
        configurations.add(WifiConfigurationTestUtil.createPskHiddenNetwork());

        byte[] backupData = mWifiBackupRestore.retrieveBackupDataFromConfigurations(configurations);
        List<WifiConfiguration> retrievedConfigurations =
                mWifiBackupRestore.retrieveConfigurationsFromBackupData(backupData);
        WifiConfigurationTestUtil.assertConfigurationsEqualForBackup(
                configurations, retrievedConfigurations);
    }

    /**
     * Verify that a single WEP network configuration is serialized & deserialized correctly.
     */
    @Test
    public void testSingleWepNetworkBackupRestore() {
        List<WifiConfiguration> configurations = new ArrayList<>();
        configurations.add(WifiConfigurationTestUtil.createWepNetwork());

        byte[] backupData = mWifiBackupRestore.retrieveBackupDataFromConfigurations(configurations);
        List<WifiConfiguration> retrievedConfigurations =
                mWifiBackupRestore.retrieveConfigurationsFromBackupData(backupData);
        WifiConfigurationTestUtil.assertConfigurationsEqualForBackup(
                configurations, retrievedConfigurations);
    }

    /**
     * Verify that restoring of configuration that contains unsupported tags works correctly
     * (unsupported tags are ignored).
     */
    @Test
    public void testConfigurationWithUnsupportedTagsRestore() {
        List<WifiConfiguration> configurations = new ArrayList<>();
        configurations.add(createNetworkForConfigurationWithUnsupportedTag());

        byte[] backupData = WIFI_BACKUP_DATA_WITH_UNSUPPORTED_TAG.getBytes();
        List<WifiConfiguration> retrievedConfigurations =
                mWifiBackupRestore.retrieveConfigurationsFromBackupData(backupData);
        WifiConfigurationTestUtil.assertConfigurationsEqualForBackup(
                configurations, retrievedConfigurations);

        // No valid data to check in dump.
        mCheckDump = false;
    }

    /**
     * Creates correct WiFiConfiguration that should be parsed out of
     * {@link #WIFI_BACKUP_DATA_WITH_UNSUPPORTED_TAG} configuration which contains unsupported tag.
     */
    private static WifiConfiguration createNetworkForConfigurationWithUnsupportedTag() {
        final WifiConfiguration config = new WifiConfiguration();
        config.SSID = "\"GoogleGuest-Legacy\"";
        config.wepTxKeyIndex = 0;
        config.hiddenSSID = false;
        config.requirePMF = false;
        config.allowedKeyManagement.set(WifiConfiguration.KeyMgmt.NONE);
        config.allowedProtocols.set(WifiConfiguration.Protocol.WPA);
        config.allowedProtocols.set(WifiConfiguration.Protocol.RSN);
        config.allowedAuthAlgorithms.set(WifiConfiguration.AuthAlgorithm.OPEN);
        config.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.WEP40);
        config.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.WEP104);
        config.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.TKIP);
        config.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.CCMP);
        config.allowedPairwiseCiphers.set(WifiConfiguration.PairwiseCipher.TKIP);
        config.allowedPairwiseCiphers.set(WifiConfiguration.PairwiseCipher.CCMP);
        config.shared = true;

        IpConfiguration ipConfiguration = new IpConfiguration();
        ipConfiguration.setIpAssignment(IpConfiguration.IpAssignment.DHCP);
        ipConfiguration.setProxySettings(IpConfiguration.ProxySettings.NONE);
        config.setIpConfiguration(ipConfiguration);

        return config;
    }

    /**
     * Verify that restoring of configuration that contains unsupported values in bitsets works
     * correctly (unsupported values are ignored).
     */
    @Test
    public void testConfigurationWithUnsupportedValuesInBitsetsRestore() {
        List<WifiConfiguration> configurations = new ArrayList<>();
        configurations.add(createNetworkForConfigurationWithUnsupportedValuesInBitsetsInRestore());

        byte[] backupData = WIFI_BACKUP_DATA_WITH_UNSUPPORTED_VALUES_IN_BITSETS.getBytes();
        List<WifiConfiguration> retrievedConfigurations =
                mWifiBackupRestore.retrieveConfigurationsFromBackupData(backupData);
        WifiConfigurationTestUtil.assertConfigurationsEqualForBackup(
                configurations, retrievedConfigurations);

        // No valid data to check in dump.
        mCheckDump = false;
    }

    /**
     * Creates correct WiFiConfiguration that should be parsed out of
     * {@link #WIFI_BACKUP_DATA_WITH_UNSUPPORTED_VALUES_IN_BITSETS} configuration which contains
     * unsupported values.
     * |AllowedKeyMgmt|, |AllowedProtocols|, |AllowedAuthAlgorithms|, |AllowedGroupCiphers| and
     * |AllowedPairwiseCiphers| fields have invalid values in them.
     */
    private static WifiConfiguration
            createNetworkForConfigurationWithUnsupportedValuesInBitsetsInRestore() {
        final WifiConfiguration config = new WifiConfiguration();
        config.SSID = "\"GoogleGuest-Legacy\"";
        config.wepTxKeyIndex = 0;
        config.hiddenSSID = false;
        config.requirePMF = false;
        config.allowedKeyManagement.set(WifiConfiguration.KeyMgmt.NONE);
        config.allowedProtocols.set(WifiConfiguration.Protocol.WPA);
        config.allowedProtocols.set(WifiConfiguration.Protocol.RSN);
        config.allowedAuthAlgorithms.set(WifiConfiguration.AuthAlgorithm.OPEN);
        config.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.WEP40);
        config.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.WEP104);
        config.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.TKIP);
        config.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.CCMP);
        config.allowedPairwiseCiphers.set(WifiConfiguration.PairwiseCipher.TKIP);
        config.allowedPairwiseCiphers.set(WifiConfiguration.PairwiseCipher.CCMP);
        config.shared = true;

        IpConfiguration ipConfiguration = new IpConfiguration();
        ipConfiguration.setIpAssignment(IpConfiguration.IpAssignment.DHCP);
        ipConfiguration.setProxySettings(IpConfiguration.ProxySettings.NONE);
        config.setIpConfiguration(ipConfiguration);

        return config;
    }

    /**
     * Verify that a single WEP network configuration with only 1 key is serialized & deserialized
     * correctly.
     */
    @Test
    public void testSingleWepNetworkWithSingleKeyBackupRestore() {
        List<WifiConfiguration> configurations = new ArrayList<>();
        configurations.add(WifiConfigurationTestUtil.createWepNetworkWithSingleKey());

        byte[] backupData = mWifiBackupRestore.retrieveBackupDataFromConfigurations(configurations);
        List<WifiConfiguration> retrievedConfigurations =
                mWifiBackupRestore.retrieveConfigurationsFromBackupData(backupData);
        WifiConfigurationTestUtil.assertConfigurationsEqualForBackup(
                configurations, retrievedConfigurations);
    }

    /**
     * Verify that a single enterprise network configuration is not serialized.
     */
    @Test
    public void testSingleEnterpriseNetworkNotBackupRestore() {
        List<WifiConfiguration> configurations = new ArrayList<>();
        configurations.add(WifiConfigurationTestUtil.createEapNetwork());
        configurations.add(WifiConfigurationTestUtil.createEapSuiteBNetwork());

        byte[] backupData = mWifiBackupRestore.retrieveBackupDataFromConfigurations(configurations);
        List<WifiConfiguration> retrievedConfigurations =
                mWifiBackupRestore.retrieveConfigurationsFromBackupData(backupData);
        assertTrue(retrievedConfigurations.isEmpty());
        // No valid data to check in dump.
        mCheckDump = false;
    }

    /**
     * Verify that a single PSK network configuration with static ip/proxy settings is serialized &
     * deserialized correctly.
     */
    @Test
    public void testSinglePskNetworkWithStaticIpAndStaticProxyBackupRestore() {
        List<WifiConfiguration> configurations = new ArrayList<>();
        WifiConfiguration pskNetwork = WifiConfigurationTestUtil.createPskNetwork();
        pskNetwork.setIpConfiguration(
                WifiConfigurationTestUtil.createStaticIpConfigurationWithStaticProxy());
        configurations.add(pskNetwork);

        byte[] backupData = mWifiBackupRestore.retrieveBackupDataFromConfigurations(configurations);
        List<WifiConfiguration> retrievedConfigurations =
                mWifiBackupRestore.retrieveConfigurationsFromBackupData(backupData);
        WifiConfigurationTestUtil.assertConfigurationsEqualForBackup(
                configurations, retrievedConfigurations);
    }

    /**
     * Verify that a single PSK network configuration with static ip & PAC proxy settings is
     * serialized & deserialized correctly.
     */
    @Test
    public void testSinglePskNetworkWithStaticIpAndPACProxyBackupRestore() {
        List<WifiConfiguration> configurations = new ArrayList<>();
        WifiConfiguration pskNetwork = WifiConfigurationTestUtil.createPskNetwork();
        pskNetwork.setIpConfiguration(
                WifiConfigurationTestUtil.createStaticIpConfigurationWithPacProxy());
        configurations.add(pskNetwork);

        byte[] backupData = mWifiBackupRestore.retrieveBackupDataFromConfigurations(configurations);
        List<WifiConfiguration> retrievedConfigurations =
                mWifiBackupRestore.retrieveConfigurationsFromBackupData(backupData);
        WifiConfigurationTestUtil.assertConfigurationsEqualForBackup(
                configurations, retrievedConfigurations);
    }

    /**
     * Verify that a single PSK network configuration with DHCP ip & PAC proxy settings is
     * serialized & deserialized correctly.
     */
    @Test
    public void testSinglePskNetworkWithDHCPIpAndPACProxyBackupRestore() {
        List<WifiConfiguration> configurations = new ArrayList<>();
        WifiConfiguration pskNetwork = WifiConfigurationTestUtil.createPskNetwork();
        pskNetwork.setIpConfiguration(
                WifiConfigurationTestUtil.createDHCPIpConfigurationWithPacProxy());
        configurations.add(pskNetwork);

        byte[] backupData = mWifiBackupRestore.retrieveBackupDataFromConfigurations(configurations);
        List<WifiConfiguration> retrievedConfigurations =
                mWifiBackupRestore.retrieveConfigurationsFromBackupData(backupData);
        WifiConfigurationTestUtil.assertConfigurationsEqualForBackup(
                configurations, retrievedConfigurations);
    }

    /**
     * Verify that a single PSK network configuration with partial static ip settings is serialized
     * & deserialized correctly.
     */
    @Test
    public void testSinglePskNetworkWithPartialStaticIpBackupRestore() {
        List<WifiConfiguration> configurations = new ArrayList<>();
        WifiConfiguration pskNetwork = WifiConfigurationTestUtil.createPskNetwork();
        pskNetwork.setIpConfiguration(
                WifiConfigurationTestUtil.createPartialStaticIpConfigurationWithPacProxy());
        configurations.add(pskNetwork);

        byte[] backupData = mWifiBackupRestore.retrieveBackupDataFromConfigurations(configurations);
        List<WifiConfiguration> retrievedConfigurations =
                mWifiBackupRestore.retrieveConfigurationsFromBackupData(backupData);
        WifiConfigurationTestUtil.assertConfigurationsEqualForBackup(
                configurations, retrievedConfigurations);
    }

    /**
     * Verify that multiple networks of different types are serialized and deserialized correctly.
     */
    @Test
    public void testMultipleNetworksAllBackupRestore() {
        List<WifiConfiguration> configurations = new ArrayList<>();
        configurations.add(WifiConfigurationTestUtil.createWepNetwork());
        configurations.add(WifiConfigurationTestUtil.createWepNetwork());
        configurations.add(WifiConfigurationTestUtil.createPskNetwork());
        configurations.add(WifiConfigurationTestUtil.createOpenNetwork());
        configurations.add(WifiConfigurationTestUtil.createOweNetwork());
        configurations.add(WifiConfigurationTestUtil.createSaeNetwork());

        byte[] backupData = mWifiBackupRestore.retrieveBackupDataFromConfigurations(configurations);
        List<WifiConfiguration> retrievedConfigurations =
                mWifiBackupRestore.retrieveConfigurationsFromBackupData(backupData);
        WifiConfigurationTestUtil.assertConfigurationsEqualForBackup(
                configurations, retrievedConfigurations);
    }

    /**
     * Verify that multiple networks of different types except enterprise ones are serialized and
     * deserialized correctly
     */
    @Test
    public void testMultipleNetworksNonEnterpriseBackupRestore() {
        List<WifiConfiguration> configurations = new ArrayList<>();
        List<WifiConfiguration> expectedConfigurations = new ArrayList<>();

        WifiConfiguration wepNetwork = WifiConfigurationTestUtil.createWepNetwork();
        configurations.add(wepNetwork);
        expectedConfigurations.add(wepNetwork);

        configurations.add(WifiConfigurationTestUtil.createEapNetwork());
        configurations.add(WifiConfigurationTestUtil.createEapSuiteBNetwork());

        WifiConfiguration pskNetwork = WifiConfigurationTestUtil.createPskNetwork();
        configurations.add(pskNetwork);
        expectedConfigurations.add(pskNetwork);

        WifiConfiguration openNetwork = WifiConfigurationTestUtil.createOpenNetwork();
        configurations.add(openNetwork);
        expectedConfigurations.add(openNetwork);

        WifiConfiguration saeNetwork = WifiConfigurationTestUtil.createSaeNetwork();
        configurations.add(saeNetwork);
        expectedConfigurations.add(saeNetwork);

        WifiConfiguration oweNetwork = WifiConfigurationTestUtil.createOweNetwork();
        configurations.add(oweNetwork);
        expectedConfigurations.add(oweNetwork);

        byte[] backupData = mWifiBackupRestore.retrieveBackupDataFromConfigurations(configurations);
        List<WifiConfiguration> retrievedConfigurations =
                mWifiBackupRestore.retrieveConfigurationsFromBackupData(backupData);
        WifiConfigurationTestUtil.assertConfigurationsEqualForBackup(
                expectedConfigurations, retrievedConfigurations);
    }

    /**
     * Verify that multiple networks with different credential types and IpConfiguration types are
     * serialized and deserialized correctly.
     */
    @Test
    public void testMultipleNetworksWithDifferentIpConfigurationsAllBackupRestore() {
        List<WifiConfiguration> configurations = new ArrayList<>();

        WifiConfiguration wepNetwork = WifiConfigurationTestUtil.createWepNetwork();
        wepNetwork.setIpConfiguration(
                WifiConfigurationTestUtil.createDHCPIpConfigurationWithPacProxy());
        configurations.add(wepNetwork);

        WifiConfiguration pskNetwork = WifiConfigurationTestUtil.createPskNetwork();
        pskNetwork.setIpConfiguration(
                WifiConfigurationTestUtil.createStaticIpConfigurationWithPacProxy());
        configurations.add(pskNetwork);

        WifiConfiguration openNetwork = WifiConfigurationTestUtil.createOpenNetwork();
        openNetwork.setIpConfiguration(
                WifiConfigurationTestUtil.createStaticIpConfigurationWithStaticProxy());
        configurations.add(openNetwork);

        byte[] backupData = mWifiBackupRestore.retrieveBackupDataFromConfigurations(configurations);
        List<WifiConfiguration> retrievedConfigurations =
                mWifiBackupRestore.retrieveConfigurationsFromBackupData(backupData);
        WifiConfigurationTestUtil.assertConfigurationsEqualForBackup(
                configurations, retrievedConfigurations);
    }

    /**
     * Verify that multiple networks of different types except the non system app created ones are
     * serialized and deserialized correctly.
     */
    @Test
    public void testMultipleNetworksSystemAppBackupRestore() {
        int systemAppUid = Process.SYSTEM_UID;
        int nonSystemAppUid = Process.FIRST_APPLICATION_UID + 556;
        when(mWifiPermissionsUtil.checkConfigOverridePermission(eq(systemAppUid)))
                .thenReturn(true);
        when(mWifiPermissionsUtil.checkConfigOverridePermission(eq(nonSystemAppUid)))
                .thenReturn(false);

        List<WifiConfiguration> configurations = new ArrayList<>();
        List<WifiConfiguration> expectedConfigurations = new ArrayList<>();

        WifiConfiguration wepNetwork = WifiConfigurationTestUtil.createWepNetwork();
        wepNetwork.creatorUid = systemAppUid;
        configurations.add(wepNetwork);
        expectedConfigurations.add(wepNetwork);

        // These should not be in |expectedConfigurations|.
        WifiConfiguration nonSystemAppWepNetwork = WifiConfigurationTestUtil.createWepNetwork();
        nonSystemAppWepNetwork.creatorUid = nonSystemAppUid;
        configurations.add(nonSystemAppWepNetwork);

        WifiConfiguration pskNetwork = WifiConfigurationTestUtil.createPskNetwork();
        pskNetwork.creatorUid = systemAppUid;
        configurations.add(pskNetwork);
        expectedConfigurations.add(pskNetwork);

        // These should not be in |expectedConfigurations|.
        WifiConfiguration nonSystemAppPskNetwork = WifiConfigurationTestUtil.createPskNetwork();
        nonSystemAppPskNetwork.creatorUid = nonSystemAppUid;
        configurations.add(nonSystemAppPskNetwork);

        WifiConfiguration openNetwork = WifiConfigurationTestUtil.createOpenNetwork();
        configurations.add(openNetwork);
        expectedConfigurations.add(openNetwork);

        byte[] backupData = mWifiBackupRestore.retrieveBackupDataFromConfigurations(configurations);
        List<WifiConfiguration> retrievedConfigurations =
                mWifiBackupRestore.retrieveConfigurationsFromBackupData(backupData);
        WifiConfigurationTestUtil.assertConfigurationsEqualForBackup(
                expectedConfigurations, retrievedConfigurations);
    }

    /**
     * Verify that a single open network configuration is serialized & deserialized correctly from
     * old backups.
     */
    @Test
    public void testSingleOpenNetworkSupplicantBackupRestore() {
        List<WifiConfiguration> configurations = new ArrayList<>();
        configurations.add(WifiConfigurationTestUtil.createOpenNetwork());

        byte[] supplicantData = createWpaSupplicantConfBackupData(configurations);
        byte[] ipConfigData = createIpConfBackupData(configurations);
        List<WifiConfiguration> retrievedConfigurations =
                mWifiBackupRestore.retrieveConfigurationsFromSupplicantBackupData(
                        supplicantData, ipConfigData);
        WifiConfigurationTestUtil.assertConfigurationsEqualForBackup(
                configurations, retrievedConfigurations);
    }

    /**
     * Verify that a single open hidden network configuration is serialized & deserialized
     * correctly from old backups.
     */
    @Test
    public void testSingleOpenHiddenNetworkSupplicantBackupRestore() {
        List<WifiConfiguration> configurations = new ArrayList<>();
        configurations.add(WifiConfigurationTestUtil.createOpenHiddenNetwork());

        byte[] supplicantData = createWpaSupplicantConfBackupData(configurations);
        byte[] ipConfigData = createIpConfBackupData(configurations);
        List<WifiConfiguration> retrievedConfigurations =
                mWifiBackupRestore.retrieveConfigurationsFromSupplicantBackupData(
                        supplicantData, ipConfigData);
        WifiConfigurationTestUtil.assertConfigurationsEqualForBackup(
                configurations, retrievedConfigurations);
    }

    /**
     * Verify that a single PSK network configuration is serialized & deserialized correctly from
     * old backups.
     */
    @Test
    public void testSinglePskNetworkSupplicantBackupRestore() {
        List<WifiConfiguration> configurations = new ArrayList<>();
        configurations.add(WifiConfigurationTestUtil.createPskNetwork());

        byte[] supplicantData = createWpaSupplicantConfBackupData(configurations);
        byte[] ipConfigData = createIpConfBackupData(configurations);
        List<WifiConfiguration> retrievedConfigurations =
                mWifiBackupRestore.retrieveConfigurationsFromSupplicantBackupData(
                        supplicantData, ipConfigData);
        WifiConfigurationTestUtil.assertConfigurationsEqualForBackup(
                configurations, retrievedConfigurations);
    }

    /**
     * Verify that a single PSK hidden network configuration is serialized & deserialized correctly
     * from old backups.
     */
    @Test
    public void testSinglePskHiddenNetworkSupplicantBackupRestore() {
        List<WifiConfiguration> configurations = new ArrayList<>();
        configurations.add(WifiConfigurationTestUtil.createPskHiddenNetwork());

        byte[] supplicantData = createWpaSupplicantConfBackupData(configurations);
        byte[] ipConfigData = createIpConfBackupData(configurations);
        List<WifiConfiguration> retrievedConfigurations =
                mWifiBackupRestore.retrieveConfigurationsFromSupplicantBackupData(
                        supplicantData, ipConfigData);
        WifiConfigurationTestUtil.assertConfigurationsEqualForBackup(
                configurations, retrievedConfigurations);
    }

    /**
     * Verify that a single WEP network configuration is serialized & deserialized correctly from
     * old backups.
     */
    @Test
    public void testSingleWepNetworkSupplicantBackupRestore() {
        List<WifiConfiguration> configurations = new ArrayList<>();
        configurations.add(WifiConfigurationTestUtil.createWepNetwork());

        byte[] supplicantData = createWpaSupplicantConfBackupData(configurations);
        byte[] ipConfigData = createIpConfBackupData(configurations);
        List<WifiConfiguration> retrievedConfigurations =
                mWifiBackupRestore.retrieveConfigurationsFromSupplicantBackupData(
                        supplicantData, ipConfigData);
        WifiConfigurationTestUtil.assertConfigurationsEqualForBackup(
                configurations, retrievedConfigurations);
    }

    /**
     * Verify that a single WEP network configuration with only 1 key is serialized & deserialized
     * correctly from old backups.
     */
    @Test
    public void testSingleWepNetworkWithSingleKeySupplicantBackupRestore() {
        List<WifiConfiguration> configurations = new ArrayList<>();
        configurations.add(WifiConfigurationTestUtil.createWepNetworkWithSingleKey());

        byte[] supplicantData = createWpaSupplicantConfBackupData(configurations);
        byte[] ipConfigData = createIpConfBackupData(configurations);
        List<WifiConfiguration> retrievedConfigurations =
                mWifiBackupRestore.retrieveConfigurationsFromSupplicantBackupData(
                        supplicantData, ipConfigData);
        WifiConfigurationTestUtil.assertConfigurationsEqualForBackup(
                configurations, retrievedConfigurations);
    }

    /**
     * Verify that a single enterprise network configuration is not serialized from old backups.
     */
    @Test
    public void testSingleEnterpriseNetworkNotSupplicantBackupRestore() {
        List<WifiConfiguration> configurations = new ArrayList<>();
        configurations.add(WifiConfigurationTestUtil.createEapNetwork());

        byte[] supplicantData = createWpaSupplicantConfBackupData(configurations);
        byte[] ipConfigData = createIpConfBackupData(configurations);
        List<WifiConfiguration> retrievedConfigurations =
                mWifiBackupRestore.retrieveConfigurationsFromSupplicantBackupData(
                        supplicantData, ipConfigData);
        assertTrue(retrievedConfigurations.isEmpty());
    }

    /**
     * Verify that multiple networks with different credential types and IpConfiguration types are
     * serialized and deserialized correctly from old backups
     */
    @Test
    public void testMultipleNetworksWithDifferentIpConfigurationsAllSupplicantBackupRestore() {
        List<WifiConfiguration> configurations = new ArrayList<>();

        WifiConfiguration wepNetwork = WifiConfigurationTestUtil.createWepNetwork();
        wepNetwork.setIpConfiguration(
                WifiConfigurationTestUtil.createDHCPIpConfigurationWithPacProxy());
        configurations.add(wepNetwork);

        WifiConfiguration pskNetwork = WifiConfigurationTestUtil.createPskNetwork();
        pskNetwork.setIpConfiguration(
                WifiConfigurationTestUtil.createStaticIpConfigurationWithPacProxy());
        configurations.add(pskNetwork);

        WifiConfiguration openNetwork = WifiConfigurationTestUtil.createOpenNetwork();
        openNetwork.setIpConfiguration(
                WifiConfigurationTestUtil.createStaticIpConfigurationWithStaticProxy());
        configurations.add(openNetwork);

        byte[] supplicantData = createWpaSupplicantConfBackupData(configurations);
        byte[] ipConfigData = createIpConfBackupData(configurations);
        List<WifiConfiguration> retrievedConfigurations =
                mWifiBackupRestore.retrieveConfigurationsFromSupplicantBackupData(
                        supplicantData, ipConfigData);
        WifiConfigurationTestUtil.assertConfigurationsEqualForBackup(
                configurations, retrievedConfigurations);
    }

    /**
     * Verify that a single open network configuration is serialized & deserialized correctly from
     * old backups with no ipconfig data.
     */
    @Test
    public void testSingleOpenNetworkSupplicantBackupRestoreWithNoIpConfigData() {
        List<WifiConfiguration> configurations = new ArrayList<>();
        configurations.add(WifiConfigurationTestUtil.createOpenNetwork());

        byte[] supplicantData = createWpaSupplicantConfBackupData(configurations);
        List<WifiConfiguration> retrievedConfigurations =
                mWifiBackupRestore.retrieveConfigurationsFromSupplicantBackupData(
                        supplicantData, null);
        WifiConfigurationTestUtil.assertConfigurationsEqualForBackup(
                configurations, retrievedConfigurations);
    }

    /**
     * Verify that multiple networks with different credential types are serialized and
     * deserialized correctly from old backups with no ipconfig data.
     */
    @Test
    public void testMultipleNetworksAllSupplicantBackupRestoreWithNoIpConfigData() {
        List<WifiConfiguration> configurations = new ArrayList<>();

        WifiConfiguration wepNetwork = WifiConfigurationTestUtil.createWepNetwork();
        configurations.add(wepNetwork);

        WifiConfiguration pskNetwork = WifiConfigurationTestUtil.createPskNetwork();
        configurations.add(pskNetwork);

        WifiConfiguration openNetwork = WifiConfigurationTestUtil.createOpenNetwork();
        configurations.add(openNetwork);

        byte[] supplicantData = createWpaSupplicantConfBackupData(configurations);
        List<WifiConfiguration> retrievedConfigurations =
                mWifiBackupRestore.retrieveConfigurationsFromSupplicantBackupData(
                        supplicantData, null);
        WifiConfigurationTestUtil.assertConfigurationsEqualForBackup(
                configurations, retrievedConfigurations);
    }

    /**
     * Verify that multiple networks of different types except the non system app created ones are
     * serialized and deserialized correctly from old backups.
     */
    @Test
    public void testMultipleNetworksSystemAppSupplicantBackupRestore() {
        List<WifiConfiguration> configurations = new ArrayList<>();
        List<WifiConfiguration> expectedConfigurations = new ArrayList<>();

        WifiConfiguration wepNetwork = WifiConfigurationTestUtil.createWepNetwork();
        configurations.add(wepNetwork);
        expectedConfigurations.add(wepNetwork);

        // These should not be in |expectedConfigurations|.
        WifiConfiguration nonSystemAppWepNetwork = WifiConfigurationTestUtil.createWepNetwork();
        nonSystemAppWepNetwork.creatorUid = Process.FIRST_APPLICATION_UID;
        configurations.add(nonSystemAppWepNetwork);

        WifiConfiguration pskNetwork = WifiConfigurationTestUtil.createPskNetwork();
        configurations.add(pskNetwork);
        expectedConfigurations.add(pskNetwork);

        // These should not be in |expectedConfigurations|.
        WifiConfiguration nonSystemAppPskNetwork = WifiConfigurationTestUtil.createPskNetwork();
        nonSystemAppPskNetwork.creatorUid = Process.FIRST_APPLICATION_UID + 1;
        configurations.add(nonSystemAppPskNetwork);

        WifiConfiguration openNetwork = WifiConfigurationTestUtil.createOpenNetwork();
        configurations.add(openNetwork);
        expectedConfigurations.add(openNetwork);

        byte[] supplicantData = createWpaSupplicantConfBackupData(configurations);
        byte[] ipConfigData = createIpConfBackupData(configurations);
        List<WifiConfiguration> retrievedConfigurations =
                mWifiBackupRestore.retrieveConfigurationsFromSupplicantBackupData(
                        supplicantData, ipConfigData);
        WifiConfigurationTestUtil.assertConfigurationsEqualForBackup(
                expectedConfigurations, retrievedConfigurations);
    }

    /**
     * Verifying that backup data containing some unknown keys is properly restored.
     * The backup data used here is a PII masked version of a backup data seen in a reported bug.
     */
    @Test
    public void testSingleNetworkSupplicantBackupRestoreWithUnknownEAPKey() {
        String backupSupplicantConfNetworkBlock = "network={\n"
                + "ssid=" + WifiConfigurationTestUtil.TEST_SSID + "\n"
                + "psk=" + WifiConfigurationTestUtil.TEST_PSK + "\n"
                + "key_mgmt=WPA-PSK WPA-PSK-SHA256\n"
                + "priority=18\n"
                + "id_str=\"%7B%22creatorUid%22%3A%221000%22%2C%22configKey"
                + "%22%3A%22%5C%22BLAH%5C%22WPA_PSK%22%7D\"\n"
                + "eapRetryCount=6\n";
        byte[] supplicantData = backupSupplicantConfNetworkBlock.getBytes();
        List<WifiConfiguration> retrievedConfigurations =
                mWifiBackupRestore.retrieveConfigurationsFromSupplicantBackupData(
                        supplicantData, null);

        final WifiConfiguration expectedConfiguration = new WifiConfiguration();
        expectedConfiguration.SSID = WifiConfigurationTestUtil.TEST_SSID;
        expectedConfiguration.preSharedKey = WifiConfigurationTestUtil.TEST_PSK;
        expectedConfiguration.allowedKeyManagement.set(WifiConfiguration.KeyMgmt.WPA_PSK);

        ArrayList<WifiConfiguration> expectedConfigurations = new ArrayList<WifiConfiguration>() {{
                add(expectedConfiguration);
            }};
        WifiConfigurationTestUtil.assertConfigurationsEqualForBackup(
                expectedConfigurations, retrievedConfigurations);
    }

    /**
     * Verify that any corrupted data provided by Backup/Restore is ignored correctly.
     */
    @Test
    public void testCorruptBackupRestore() {
        Random random = new Random();
        byte[] backupData = new byte[100];
        random.nextBytes(backupData);

        List<WifiConfiguration> retrievedConfigurations =
                mWifiBackupRestore.retrieveConfigurationsFromBackupData(backupData);
        assertNull(retrievedConfigurations);
        // No valid data to check in dump.
        mCheckDump = false;
    }

    /**
     * Verify that restoring of configuration from a 1.0 version backup data.
     */
    @Test
    public void testRestoreFromV1_0BackupData() {
        List<WifiConfiguration> configurations = new ArrayList<>();
        configurations.add(createNetworkForConfigurationWithV1_0Data());

        byte[] backupData = WIFI_BACKUP_DATA_V1_0.getBytes();
        List<WifiConfiguration> retrievedConfigurations =
                mWifiBackupRestore.retrieveConfigurationsFromBackupData(backupData);
        WifiConfigurationTestUtil.assertConfigurationsEqualForBackup(
                configurations, retrievedConfigurations);

        // No valid data to check in dump.
        mCheckDump = false;
    }

    /**
     * Creates correct WiFiConfiguration that should be parsed out of
     * {@link #WIFI_BACKUP_DATA_V1_0} configuration which contains 1.0 version backup.
     */
    private static WifiConfiguration createNetworkForConfigurationWithV1_0Data() {
        final WifiConfiguration config = new WifiConfiguration();
        config.SSID = "\"GoogleGuest-Legacy\"";
        config.wepTxKeyIndex = 0;
        config.hiddenSSID = false;
        config.requirePMF = false;
        config.allowedKeyManagement.set(WifiConfiguration.KeyMgmt.NONE);
        config.allowedProtocols.set(WifiConfiguration.Protocol.WPA);
        config.allowedProtocols.set(WifiConfiguration.Protocol.RSN);
        config.allowedAuthAlgorithms.set(WifiConfiguration.AuthAlgorithm.OPEN);
        config.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.WEP40);
        config.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.WEP104);
        config.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.TKIP);
        config.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.CCMP);
        config.allowedPairwiseCiphers.set(WifiConfiguration.PairwiseCipher.TKIP);
        config.allowedPairwiseCiphers.set(WifiConfiguration.PairwiseCipher.CCMP);
        config.shared = true;

        IpConfiguration ipConfiguration = new IpConfiguration();
        ipConfiguration.setIpAssignment(IpConfiguration.IpAssignment.DHCP);
        ipConfiguration.setProxySettings(IpConfiguration.ProxySettings.NONE);
        config.setIpConfiguration(ipConfiguration);

        return config;
    }

    /**
     * Verify that restoring of configuration from a 1.1 version backup data.
     */
    @Test
    public void testRestoreFromV1_1BackupData() {
        List<WifiConfiguration> configurations = new ArrayList<>();
        configurations.add(createNetworkForConfigurationWithV1_1Data());

        byte[] backupData = WIFI_BACKUP_DATA_V1_1.getBytes();
        List<WifiConfiguration> retrievedConfigurations =
                mWifiBackupRestore.retrieveConfigurationsFromBackupData(backupData);
        WifiConfigurationTestUtil.assertConfigurationsEqualForBackup(
                configurations, retrievedConfigurations);

        // No valid data to check in dump.
        mCheckDump = false;
    }

    /**
     * Creates correct WiFiConfiguration that should be parsed out of
     * {@link #WIFI_BACKUP_DATA_V1_1} configuration which contains 1.1 version backup.
     */
    private static WifiConfiguration createNetworkForConfigurationWithV1_1Data() {
        final WifiConfiguration config = createNetworkForConfigurationWithV1_0Data();
        config.meteredOverride = WifiConfiguration.METERED_OVERRIDE_METERED;

        return config;
    }


    /**
     * Helper method to write a list of networks in wpa_supplicant.conf format to the output stream.
     */
    private byte[] createWpaSupplicantConfBackupData(List<WifiConfiguration> configurations) {
        ByteArrayOutputStream bos = new ByteArrayOutputStream();
        OutputStreamWriter out = new OutputStreamWriter(bos);
        try {
            for (WifiConfiguration configuration : configurations) {
                writeConfigurationToWpaSupplicantConf(out, configuration);
            }
            out.flush();
            return bos.toByteArray();
        } catch (IOException e) {
            return null;
        }
    }

    /**
     * Helper method to write a network in wpa_supplicant.conf format to the output stream.
     * This was created using a sample wpa_supplicant.conf file. Using the raw key strings here
     * (instead of consts in WifiBackupRestore).
     */
    private void writeConfigurationToWpaSupplicantConf(
            OutputStreamWriter out, WifiConfiguration configuration)
            throws IOException {
        out.write("network={\n");
        out.write("        " + "ssid=" + configuration.SSID + "\n");
        String allowedKeyManagement = "";
        if (configuration.hiddenSSID) {
            out.write("        " + "scan_ssid=1" + "\n");
        }
        if (configuration.allowedKeyManagement.get(WifiConfiguration.KeyMgmt.NONE)) {
            allowedKeyManagement += "NONE";
        }
        if (configuration.allowedKeyManagement.get(WifiConfiguration.KeyMgmt.WPA_PSK)) {
            allowedKeyManagement += "WPA-PSK ";
        }
        if (configuration.allowedKeyManagement.get(WifiConfiguration.KeyMgmt.WPA_EAP)) {
            allowedKeyManagement += "WPA-EAP ";
        }
        if (configuration.allowedKeyManagement.get(WifiConfiguration.KeyMgmt.IEEE8021X)) {
            allowedKeyManagement += "IEEE8021X ";
        }
        out.write("        " + "key_mgmt=" + allowedKeyManagement + "\n");
        if (configuration.preSharedKey != null) {
            out.write("        " + "psk=" + configuration.preSharedKey + "\n");
        }
        if (configuration.wepKeys[0] != null) {
            out.write("        " + "wep_key0=" + configuration.wepKeys[0] + "\n");
        }
        if (configuration.wepKeys[1] != null) {
            out.write("        " + "wep_key1=" + configuration.wepKeys[1] + "\n");
        }
        if (configuration.wepKeys[2] != null) {
            out.write("        " + "wep_key2=" + configuration.wepKeys[2] + "\n");
        }
        if (configuration.wepKeys[3] != null) {
            out.write("        " + "wep_key3=" + configuration.wepKeys[3] + "\n");
        }
        if (configuration.wepKeys[0] != null || configuration.wepKeys[1] != null
                || configuration.wepKeys[2] != null || configuration.wepKeys[3] != null) {
            out.write("        " + "wep_tx_keyidx=" + configuration.wepTxKeyIndex + "\n");
        }
        Map<String, String> extras = new HashMap<>();
        extras.put(SupplicantStaNetworkHal.ID_STRING_KEY_CONFIG_KEY, configuration.configKey());
        extras.put(SupplicantStaNetworkHal.ID_STRING_KEY_CREATOR_UID,
                Integer.toString(configuration.creatorUid));
        String idString = "\"" + SupplicantStaNetworkHal.createNetworkExtra(extras) + "\"";
        if (idString != null) {
            out.write("        " + "id_str=" + idString + "\n");
        }
        out.write("}\n");
        out.write("\n");
    }

    /**
     * Helper method to write a list of networks in ipconfig.txt format to the output stream.
     */
    private byte[] createIpConfBackupData(List<WifiConfiguration> configurations) {
        ByteArrayOutputStream bos = new ByteArrayOutputStream();
        DataOutputStream out = new DataOutputStream(bos);
        final int configStoreVersion = 2;
        try {
            // write version first.
            out.writeInt(configStoreVersion);
            for (WifiConfiguration configuration : configurations) {
                // TODO: store configKey as a string instead of calculating its hash
                IpConfigStore.writeConfig(out, String.valueOf(configuration.configKey().hashCode()),
                        configuration.getIpConfiguration(), configStoreVersion);
            }
            out.flush();
            return bos.toByteArray();
        } catch (IOException e) {
            return null;
        }
    }
}
