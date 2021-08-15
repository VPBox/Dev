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

import android.net.IpConfiguration;
import android.net.wifi.WifiConfiguration;
import android.net.wifi.WifiConfiguration.NetworkSelectionStatus;
import android.net.wifi.WifiEnterpriseConfig;
import android.util.Pair;
import android.util.Xml;

import androidx.test.filters.SmallTest;

import com.android.internal.util.FastXmlSerializer;
import com.android.server.wifi.WifiConfigurationTestUtil;
import com.android.server.wifi.util.XmlUtil.IpConfigurationXmlUtil;
import com.android.server.wifi.util.XmlUtil.NetworkSelectionStatusXmlUtil;
import com.android.server.wifi.util.XmlUtil.WifiConfigurationXmlUtil;
import com.android.server.wifi.util.XmlUtil.WifiEnterpriseConfigXmlUtil;

import org.junit.Before;
import org.junit.Test;
import org.mockito.MockitoAnnotations;
import org.xmlpull.v1.XmlPullParser;
import org.xmlpull.v1.XmlPullParserException;
import org.xmlpull.v1.XmlSerializer;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.util.HashMap;

/**
 * Unit tests for {@link com.android.server.wifi.util.XmlUtil}.
 */
@SmallTest
public class XmlUtilTest {
    public static final String XML_STRING_EAP_METHOD_REPLACE_FORMAT =
            "<int name=\"EapMethod\" value=\"%d\" />";

    private static final String TEST_PACKAGE_NAME = "XmlUtilPackage";
    private static final String TEST_STATIC_IP_GATEWAY_ADDRESS = "192.168.48.1";
    private static final String TEST_DUMMY_CONFIG_KEY = "XmlUtilDummyConfigKey";
    private static final String TEST_IDENTITY = "XmlUtilTestIdentity";
    private static final String TEST_ANON_IDENTITY = "XmlUtilTestAnonIdentity";
    private static final String TEST_PASSWORD = "XmlUtilTestPassword";
    private static final String TEST_CLIENT_CERT = "XmlUtilTestClientCert";
    private static final String TEST_CA_CERT = "XmlUtilTestCaCert";
    private static final String TEST_SUBJECT_MATCH = "XmlUtilTestSubjectMatch";
    private static final String TEST_ENGINE = "XmlUtilTestEngine";
    private static final String TEST_ENGINE_ID = "XmlUtilTestEngineId";
    private static final String TEST_PRIVATE_KEY_ID = "XmlUtilTestPrivateKeyId";
    private static final String TEST_ALTSUBJECT_MATCH = "XmlUtilTestAltSubjectMatch";
    private static final String TEST_DOM_SUFFIX_MATCH = "XmlUtilTestDomSuffixMatch";
    private static final String TEST_CA_PATH = "XmlUtilTestCaPath";
    private static final int TEST_EAP_METHOD = WifiEnterpriseConfig.Eap.PEAP;
    private static final int TEST_PHASE2_METHOD = WifiEnterpriseConfig.Phase2.MSCHAPV2;
    private final String mXmlDocHeader = "XmlUtilTest";

    private WifiConfigStoreEncryptionUtil mWifiConfigStoreEncryptionUtil = null;

    @Before
    public void setUp() throws Exception {
        MockitoAnnotations.initMocks(this);
    }

    /**
     * Verify that a open WifiConfiguration is serialized & deserialized correctly.
     */
    @Test
    public void testOpenWifiConfigurationSerializeDeserialize()
            throws IOException, XmlPullParserException {
        serializeDeserializeWifiConfiguration(WifiConfigurationTestUtil.createOpenNetwork());
    }

    /**
     * Verify that a open hidden WifiConfiguration is serialized & deserialized correctly.
     */
    @Test
    public void testOpenHiddenWifiConfigurationSerializeDeserialize()
            throws IOException, XmlPullParserException {
        serializeDeserializeWifiConfiguration(WifiConfigurationTestUtil.createOpenHiddenNetwork());
    }

    /**
     * Verify that a psk WifiConfiguration is serialized & deserialized correctly.
     */
    @Test
    public void testPskWifiConfigurationSerializeDeserialize()
            throws IOException, XmlPullParserException {
        serializeDeserializeWifiConfiguration(WifiConfigurationTestUtil.createPskNetwork());
    }

    /**
     * Verify that a psk WifiConfiguration is serialized & deserialized correctly.
     */
    @Test
    public void testPskWifiConfigurationSerializeDeserializeWithEncryption()
            throws IOException, XmlPullParserException {
        mWifiConfigStoreEncryptionUtil = mock(WifiConfigStoreEncryptionUtil.class);
        WifiConfiguration pskNetwork = WifiConfigurationTestUtil.createPskNetwork();
        EncryptedData encryptedData = new EncryptedData(new byte[0], new byte[0]);
        when(mWifiConfigStoreEncryptionUtil.encrypt(pskNetwork.preSharedKey.getBytes()))
                .thenReturn(encryptedData);
        when(mWifiConfigStoreEncryptionUtil.decrypt(encryptedData))
                .thenReturn(pskNetwork.preSharedKey.getBytes());
        serializeDeserializeWifiConfiguration(pskNetwork);
    }

    /**
     * Verify that a psk hidden WifiConfiguration is serialized & deserialized correctly.
     */
    @Test
    public void testPskHiddenWifiConfigurationSerializeDeserialize()
            throws IOException, XmlPullParserException {
        serializeDeserializeWifiConfiguration(WifiConfigurationTestUtil.createPskHiddenNetwork());
    }

    /**
     * Verify that a WEP WifiConfiguration is serialized & deserialized correctly.
     */
    @Test
    public void testWepWifiConfigurationSerializeDeserialize()
            throws IOException, XmlPullParserException {
        serializeDeserializeWifiConfiguration(WifiConfigurationTestUtil.createWepNetwork());
    }

    /**
     * Verify that a EAP WifiConfiguration is serialized & deserialized correctly only for
     * ConfigStore.
     */
    @Test
    public void testEapWifiConfigurationSerializeDeserialize()
            throws IOException, XmlPullParserException {
        serializeDeserializeWifiConfigurationForConfigStore(
                WifiConfigurationTestUtil.createEapNetwork());
    }

    /**
     * Verify that a static IpConfiguration with PAC proxy is serialized & deserialized correctly.
     */
    @Test
    public void testStaticIpConfigurationWithPacProxySerializeDeserialize()
            throws IOException, XmlPullParserException {
        serializeDeserializeIpConfiguration(
                WifiConfigurationTestUtil.createStaticIpConfigurationWithPacProxy());
    }

    /**
     * Verify that a static IpConfiguration with static proxy is serialized & deserialized correctly.
     */
    @Test
    public void testStaticIpConfigurationWithStaticProxySerializeDeserialize()
            throws IOException, XmlPullParserException {
        serializeDeserializeIpConfiguration(
                WifiConfigurationTestUtil.createStaticIpConfigurationWithStaticProxy());
    }

    /**
     * Verify that a partial static IpConfiguration with PAC proxy is serialized & deserialized
     * correctly.
     */
    @Test
    public void testPartialStaticIpConfigurationWithPacProxySerializeDeserialize()
            throws IOException, XmlPullParserException {
        serializeDeserializeIpConfiguration(
                WifiConfigurationTestUtil.createPartialStaticIpConfigurationWithPacProxy());
    }

    /**
     * Verify that a DHCP IpConfiguration with PAC proxy is serialized & deserialized
     * correctly.
     */
    @Test
    public void testDHCPIpConfigurationWithPacProxySerializeDeserialize()
            throws IOException, XmlPullParserException {
        serializeDeserializeIpConfiguration(
                WifiConfigurationTestUtil.createDHCPIpConfigurationWithPacProxy());
    }

    /**
     * Verify that a DHCP IpConfiguration with Static proxy is serialized & deserialized
     * correctly.
     */
    @Test
    public void testDHCPIpConfigurationWithStaticProxySerializeDeserialize()
            throws IOException, XmlPullParserException {
        serializeDeserializeIpConfiguration(
                WifiConfigurationTestUtil.createDHCPIpConfigurationWithStaticProxy());
    }

    /**
     * Verify that a EAP WifiConfiguration is serialized & deserialized correctly for config store.
     * This basically exercises all the elements being serialized in config store.
     */
    @Test
    public void testEapWifiConfigurationSerializeDeserializeForConfigStore()
            throws IOException, XmlPullParserException {
        WifiConfiguration configuration = WifiConfigurationTestUtil.createEapNetwork();
        configuration.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.WEP104);
        configuration.allowedGroupCiphers.set(WifiConfiguration.GroupCipher.WEP40);
        configuration.allowedPairwiseCiphers.set(WifiConfiguration.PairwiseCipher.CCMP);
        configuration.allowedPairwiseCiphers.set(WifiConfiguration.PairwiseCipher.TKIP);
        configuration.status = WifiConfiguration.Status.DISABLED;
        configuration.linkedConfigurations = new HashMap<>();
        configuration.linkedConfigurations.put(TEST_DUMMY_CONFIG_KEY, Integer.valueOf(1));
        configuration.defaultGwMacAddress = TEST_STATIC_IP_GATEWAY_ADDRESS;
        configuration.requirePMF = true;
        configuration.validatedInternetAccess = true;
        configuration.noInternetAccessExpected = true;
        configuration.userApproved = WifiConfiguration.USER_UNSPECIFIED;
        configuration.meteredHint = true;
        configuration.useExternalScores = true;
        configuration.numAssociation = 5;
        configuration.lastUpdateUid = configuration.lastConnectUid = configuration.creatorUid;
        configuration.creatorName = configuration.lastUpdateName = TEST_PACKAGE_NAME;
        configuration.creationTime = "04-04-2016";
        configuration.getOrCreateRandomizedMacAddress();
        configuration.macRandomizationSetting = WifiConfiguration.RANDOMIZATION_PERSISTENT;

        serializeDeserializeWifiConfigurationForConfigStore(configuration);
    }

    /**
     * Verify that a WifiConfiguration with status as CURRENT when serializing
     * is deserialized as ENABLED.
     */
    @Test
    public void testCurrentStatusConfigurationSerializeDeserializeForConfigStore()
            throws IOException, XmlPullParserException {
        WifiConfiguration configuration = WifiConfigurationTestUtil.createEapNetwork();
        configuration.status = WifiConfiguration.Status.CURRENT;
        byte[] xmlData = serializeWifiConfigurationForConfigStore(configuration);
        Pair<String, WifiConfiguration> deserializedConfiguration =
                deserializeWifiConfiguration(xmlData);
        assertEquals(WifiConfiguration.Status.ENABLED, deserializedConfiguration.second.status);
    }

    /**
     * Verify that an enabled network selection status object is serialized & deserialized
     * correctly.
     */
    @Test
    public void testEnabledNetworkSelectionStatusSerializeDeserialize()
            throws IOException, XmlPullParserException {
        NetworkSelectionStatus status = new NetworkSelectionStatus();
        status.setNetworkSelectionStatus(NetworkSelectionStatus.NETWORK_SELECTION_ENABLED);
        status.setNetworkSelectionDisableReason(NetworkSelectionStatus.NETWORK_SELECTION_ENABLE);
        status.setConnectChoice(TEST_DUMMY_CONFIG_KEY);
        status.setConnectChoiceTimestamp(867889);
        status.setHasEverConnected(true);
        serializeDeserializeNetworkSelectionStatus(status);
    }

    /**
     * Verify that a temporarily disabled network selection status object is serialized &
     * deserialized correctly.
     */
    @Test
    public void testTemporarilyDisabledNetworkSelectionStatusSerializeDeserialize()
            throws IOException, XmlPullParserException {
        NetworkSelectionStatus status = new NetworkSelectionStatus();
        status.setNetworkSelectionStatus(
                NetworkSelectionStatus.NETWORK_SELECTION_TEMPORARY_DISABLED);
        status.setNetworkSelectionDisableReason(
                NetworkSelectionStatus.DISABLED_ASSOCIATION_REJECTION);
        serializeDeserializeNetworkSelectionStatus(status);
    }

    /**
     * Verify that a network selection status deprecation is handled correctly during restore
     * of data after upgrade.
     * This test tries to simulate the scenario where we have a
     * {@link NetworkSelectionStatus#getNetworkStatusString()} string stored
     * in the XML file from a previous release which has now been deprecated. The network should
     * be restored as enabled.
     */
    @Test
    public void testDeprecatedNetworkSelectionStatusDeserialize()
            throws IOException, XmlPullParserException {
        // Create a dummy network selection status.
        NetworkSelectionStatus status = new NetworkSelectionStatus();
        status.setNetworkSelectionStatus(
                NetworkSelectionStatus.NETWORK_SELECTION_TEMPORARY_DISABLED);
        status.setNetworkSelectionDisableReason(
                NetworkSelectionStatus.DISABLED_DHCP_FAILURE);
        status.setConnectChoice(TEST_DUMMY_CONFIG_KEY);
        status.setConnectChoiceTimestamp(867889);
        status.setHasEverConnected(true);

        // Serialize this to XML string.
        byte[] xmlData = serializeNetworkSelectionStatus(status);

        // Now modify the status string with some invalid string in XML data..
        String xmlString = new String(xmlData);
        String deprecatedXmlString =
                xmlString.replaceAll(
                        status.getNetworkStatusString(), "NETWORK_SELECTION_DEPRECATED");
        // Ensure that the modification did take effect.
        assertFalse(xmlString.equals(deprecatedXmlString));

        // Now Deserialize the modified XML data.
        byte[] deprecatedXmlData = xmlString.getBytes();
        NetworkSelectionStatus retrievedStatus =
                deserializeNetworkSelectionStatus(deprecatedXmlData);

        // The status retrieved should have reset both the |Status| & |DisableReason| fields after
        // deserialization, but should have restored all the other fields correctly.
        NetworkSelectionStatus expectedStatus = new NetworkSelectionStatus();
        expectedStatus.copy(status);
        expectedStatus.setNetworkSelectionStatus(NetworkSelectionStatus.NETWORK_SELECTION_ENABLED);
        expectedStatus.setNetworkSelectionDisableReason(
                NetworkSelectionStatus.NETWORK_SELECTION_ENABLE);

        WifiConfigurationTestUtil.assertNetworkSelectionStatusEqualForConfigStore(
                expectedStatus, retrievedStatus);
    }

    /**
     * Verify that a network selection disable reason deprecation is handled correctly during
     * restore of data after upgrade.
     * This test tries to simulate the scenario where we have a
     * {@link NetworkSelectionStatus#getNetworkDisableReasonString()} ()} string stored
     * in the XML file from a previous release which has now been deprecated. The network should
     * be restored as enabled.
     */
    @Test
    public void testDeprecatedNetworkSelectionDisableReasonDeserialize()
            throws IOException, XmlPullParserException {
        // Create a dummy network selection status.
        NetworkSelectionStatus status = new NetworkSelectionStatus();
        status.setNetworkSelectionStatus(
                NetworkSelectionStatus.NETWORK_SELECTION_TEMPORARY_DISABLED);
        status.setNetworkSelectionDisableReason(
                NetworkSelectionStatus.DISABLED_DHCP_FAILURE);
        status.setConnectChoice(TEST_DUMMY_CONFIG_KEY);
        status.setConnectChoiceTimestamp(867889);
        status.setHasEverConnected(true);

        // Serialize this to XML string.
        byte[] xmlData = serializeNetworkSelectionStatus(status);

        // Now modify the disable reason string with some invalid string in XML data.
        String xmlString = new String(xmlData);
        String deprecatedXmlString =
                xmlString.replaceAll(status.getNetworkDisableReasonString(), "DISABLED_DEPRECATED");
        // Ensure that the modification did take effect.
        assertFalse(xmlString.equals(deprecatedXmlString));

        // Now Deserialize the modified XML data.
        byte[] deprecatedXmlData = xmlString.getBytes();
        NetworkSelectionStatus retrievedStatus =
                deserializeNetworkSelectionStatus(deprecatedXmlData);

        // The status retrieved should have reset both the |Status| & |DisableReason| fields after
        // deserialization, but should have restored all the other fields correctly.
        NetworkSelectionStatus expectedStatus = new NetworkSelectionStatus();
        expectedStatus.copy(status);
        expectedStatus.setNetworkSelectionStatus(NetworkSelectionStatus.NETWORK_SELECTION_ENABLED);
        expectedStatus.setNetworkSelectionDisableReason(
                NetworkSelectionStatus.NETWORK_SELECTION_ENABLE);

        WifiConfigurationTestUtil.assertNetworkSelectionStatusEqualForConfigStore(
                expectedStatus, retrievedStatus);
    }

    /**
     * Verify that a WifiEnterpriseConfig object is serialized & deserialized correctly.
     */
    @Test
    public void testWifiEnterpriseConfigSerializeDeserialize()
            throws IOException, XmlPullParserException {
        WifiEnterpriseConfig config = new WifiEnterpriseConfig();
        config.setFieldValue(WifiEnterpriseConfig.IDENTITY_KEY, TEST_IDENTITY);
        config.setFieldValue(WifiEnterpriseConfig.ANON_IDENTITY_KEY, TEST_ANON_IDENTITY);
        config.setFieldValue(WifiEnterpriseConfig.PASSWORD_KEY, TEST_PASSWORD);
        config.setFieldValue(WifiEnterpriseConfig.CLIENT_CERT_KEY, TEST_CLIENT_CERT);
        config.setFieldValue(WifiEnterpriseConfig.CA_CERT_KEY, TEST_CA_CERT);
        config.setFieldValue(WifiEnterpriseConfig.SUBJECT_MATCH_KEY, TEST_SUBJECT_MATCH);
        config.setFieldValue(WifiEnterpriseConfig.ENGINE_KEY, TEST_ENGINE);
        config.setFieldValue(WifiEnterpriseConfig.ENGINE_ID_KEY, TEST_ENGINE_ID);
        config.setFieldValue(WifiEnterpriseConfig.PRIVATE_KEY_ID_KEY, TEST_PRIVATE_KEY_ID);
        config.setFieldValue(WifiEnterpriseConfig.ALTSUBJECT_MATCH_KEY, TEST_ALTSUBJECT_MATCH);
        config.setFieldValue(WifiEnterpriseConfig.DOM_SUFFIX_MATCH_KEY, TEST_DOM_SUFFIX_MATCH);
        config.setFieldValue(WifiEnterpriseConfig.CA_PATH_KEY, TEST_CA_PATH);
        config.setEapMethod(TEST_EAP_METHOD);
        config.setPhase2Method(TEST_PHASE2_METHOD);
        serializeDeserializeWifiEnterpriseConfig(config);
    }

    /**
     * Verify that a WifiEnterpriseConfig object is serialized & deserialized correctly.
     */
    @Test
    public void testWifiEnterpriseConfigSerializeDeserializeWithEncryption()
            throws IOException, XmlPullParserException {
        WifiEnterpriseConfig config = new WifiEnterpriseConfig();
        config.setFieldValue(WifiEnterpriseConfig.IDENTITY_KEY, TEST_IDENTITY);
        config.setFieldValue(WifiEnterpriseConfig.ANON_IDENTITY_KEY, TEST_ANON_IDENTITY);
        config.setFieldValue(WifiEnterpriseConfig.PASSWORD_KEY, TEST_PASSWORD);
        config.setFieldValue(WifiEnterpriseConfig.CLIENT_CERT_KEY, TEST_CLIENT_CERT);
        config.setFieldValue(WifiEnterpriseConfig.CA_CERT_KEY, TEST_CA_CERT);
        config.setFieldValue(WifiEnterpriseConfig.SUBJECT_MATCH_KEY, TEST_SUBJECT_MATCH);
        config.setFieldValue(WifiEnterpriseConfig.ENGINE_KEY, TEST_ENGINE);
        config.setFieldValue(WifiEnterpriseConfig.ENGINE_ID_KEY, TEST_ENGINE_ID);
        config.setFieldValue(WifiEnterpriseConfig.PRIVATE_KEY_ID_KEY, TEST_PRIVATE_KEY_ID);
        config.setFieldValue(WifiEnterpriseConfig.ALTSUBJECT_MATCH_KEY, TEST_ALTSUBJECT_MATCH);
        config.setFieldValue(WifiEnterpriseConfig.DOM_SUFFIX_MATCH_KEY, TEST_DOM_SUFFIX_MATCH);
        config.setFieldValue(WifiEnterpriseConfig.CA_PATH_KEY, TEST_CA_PATH);
        config.setEapMethod(TEST_EAP_METHOD);
        config.setPhase2Method(TEST_PHASE2_METHOD);

        mWifiConfigStoreEncryptionUtil = mock(WifiConfigStoreEncryptionUtil.class);
        EncryptedData encryptedData = new EncryptedData(new byte[0], new byte[0]);
        when(mWifiConfigStoreEncryptionUtil.encrypt(TEST_PASSWORD.getBytes()))
                .thenReturn(encryptedData);
        when(mWifiConfigStoreEncryptionUtil.decrypt(encryptedData))
                .thenReturn(TEST_PASSWORD.getBytes());
        serializeDeserializeWifiEnterpriseConfig(config);
    }

    /**
     * Verify that an illegal argument exception is thrown when trying to parse out a corrupted
     * WifiEnterpriseConfig.
     *
     * @throws Exception
     */
    @Test(expected = IllegalArgumentException.class)
    public void testWifiEnterpriseConfigSerializeDeserializeThrowsIllegalArgException()
            throws Exception {
        WifiEnterpriseConfig config = new WifiEnterpriseConfig();
        config.setFieldValue(WifiEnterpriseConfig.IDENTITY_KEY, TEST_IDENTITY);
        config.setFieldValue(WifiEnterpriseConfig.ANON_IDENTITY_KEY, TEST_ANON_IDENTITY);
        config.setFieldValue(WifiEnterpriseConfig.PASSWORD_KEY, TEST_PASSWORD);
        config.setFieldValue(WifiEnterpriseConfig.CLIENT_CERT_KEY, TEST_CLIENT_CERT);
        config.setFieldValue(WifiEnterpriseConfig.CA_CERT_KEY, TEST_CA_CERT);
        config.setFieldValue(WifiEnterpriseConfig.SUBJECT_MATCH_KEY, TEST_SUBJECT_MATCH);
        config.setFieldValue(WifiEnterpriseConfig.ENGINE_KEY, TEST_ENGINE);
        config.setFieldValue(WifiEnterpriseConfig.ENGINE_ID_KEY, TEST_ENGINE_ID);
        config.setFieldValue(WifiEnterpriseConfig.PRIVATE_KEY_ID_KEY, TEST_PRIVATE_KEY_ID);
        config.setFieldValue(WifiEnterpriseConfig.ALTSUBJECT_MATCH_KEY, TEST_ALTSUBJECT_MATCH);
        config.setFieldValue(WifiEnterpriseConfig.DOM_SUFFIX_MATCH_KEY, TEST_DOM_SUFFIX_MATCH);
        config.setFieldValue(WifiEnterpriseConfig.CA_PATH_KEY, TEST_CA_PATH);
        config.setEapMethod(TEST_EAP_METHOD);
        config.setPhase2Method(TEST_PHASE2_METHOD);
        String xmlString = new String(serializeWifiEnterpriseConfig(config));
        // Manipulate the XML data to set the EAP method to None, this should raise an Illegal
        // argument exception in WifiEnterpriseConfig.setEapMethod().
        xmlString = xmlString.replaceAll(
                String.format(XML_STRING_EAP_METHOD_REPLACE_FORMAT, TEST_EAP_METHOD),
                String.format(XML_STRING_EAP_METHOD_REPLACE_FORMAT, WifiEnterpriseConfig.Eap.NONE));
        deserializeWifiEnterpriseConfig(xmlString.getBytes(StandardCharsets.UTF_8));
    }

    /**
     * Verify that WifiConfiguration representation of a legacy Passpoint configuration is
     * serialized & deserialized correctly.
     *
     *@throws Exception
     */
    @Test
    public void testLegacyPasspointConfigSerializeDeserialize() throws Exception {
        WifiConfiguration config = WifiConfigurationTestUtil.createPasspointNetwork();
        config.isLegacyPasspointConfig = true;
        config.roamingConsortiumIds = new long[] {0x12345678};
        config.enterpriseConfig.setPlmn("1234");
        config.enterpriseConfig.setRealm("test.com");
        serializeDeserializeWifiConfigurationForConfigStore(config);
    }

    /**
     * Verify that when the macRandomizationSetting field is not found in the XML file,
     * macRandomizationSetting is defaulted to RANDOMIZATION_NONE.
     * @throws IOException
     * @throws XmlPullParserException
     */
    @Test
    public void testMacRandomizationSettingDefaultToRandomizationNone()
            throws IOException, XmlPullParserException {
        // First generate XML data that only has the header filled in
        final XmlSerializer out = new FastXmlSerializer();
        final ByteArrayOutputStream outputStream = new ByteArrayOutputStream();
        out.setOutput(outputStream, StandardCharsets.UTF_8.name());
        XmlUtil.writeDocumentStart(out, mXmlDocHeader);
        XmlUtil.writeDocumentEnd(out, mXmlDocHeader);

        // Deserialize the data
        Pair<String, WifiConfiguration> retrieved =
                deserializeWifiConfiguration(outputStream.toByteArray());

        // Verify that macRandomizationSetting is set to |RANDOMIZATION_NONE|
        assertEquals(WifiConfiguration.RANDOMIZATION_NONE,
                retrieved.second.macRandomizationSetting);
    }

    private byte[] serializeWifiConfigurationForBackup(WifiConfiguration configuration)
            throws IOException, XmlPullParserException {
        final XmlSerializer out = new FastXmlSerializer();
        final ByteArrayOutputStream outputStream = new ByteArrayOutputStream();
        out.setOutput(outputStream, StandardCharsets.UTF_8.name());
        XmlUtil.writeDocumentStart(out, mXmlDocHeader);
        WifiConfigurationXmlUtil.writeToXmlForBackup(out, configuration);
        XmlUtil.writeDocumentEnd(out, mXmlDocHeader);
        return outputStream.toByteArray();
    }

    private byte[] serializeWifiConfigurationForConfigStore(
            WifiConfiguration configuration)
            throws IOException, XmlPullParserException {
        final XmlSerializer out = new FastXmlSerializer();
        final ByteArrayOutputStream outputStream = new ByteArrayOutputStream();
        out.setOutput(outputStream, StandardCharsets.UTF_8.name());
        XmlUtil.writeDocumentStart(out, mXmlDocHeader);
        WifiConfigurationXmlUtil.writeToXmlForConfigStore(
                out, configuration, mWifiConfigStoreEncryptionUtil);
        XmlUtil.writeDocumentEnd(out, mXmlDocHeader);
        return outputStream.toByteArray();
    }

    private Pair<String, WifiConfiguration> deserializeWifiConfiguration(byte[] data)
            throws IOException, XmlPullParserException {
        // Deserialize the configuration object.
        final XmlPullParser in = Xml.newPullParser();
        ByteArrayInputStream inputStream = new ByteArrayInputStream(data);
        in.setInput(inputStream, StandardCharsets.UTF_8.name());
        XmlUtil.gotoDocumentStart(in, mXmlDocHeader);
        return WifiConfigurationXmlUtil.parseFromXml(
                in, in.getDepth(),
                mWifiConfigStoreEncryptionUtil != null,
                mWifiConfigStoreEncryptionUtil);
    }

    /**
     * This helper method tests the serialization for backup/restore.
     */
    private void serializeDeserializeWifiConfigurationForBackupRestore(
            WifiConfiguration configuration)
            throws IOException, XmlPullParserException {
        Pair<String, WifiConfiguration> retrieved;
        // Test serialization/deserialization for config store.
        retrieved =
                deserializeWifiConfiguration(
                        serializeWifiConfigurationForBackup(configuration));
        assertEquals(retrieved.first, retrieved.second.configKey());
        WifiConfigurationTestUtil.assertConfigurationEqualForBackup(
                configuration, retrieved.second);
    }

    /**
     * This helper method tests the serialization for config store.
     */
    private void serializeDeserializeWifiConfigurationForConfigStore(
            WifiConfiguration configuration)
            throws IOException, XmlPullParserException {
        // Reset enterprise config because this needs to be serialized/deserialized separately.
        configuration.enterpriseConfig = new WifiEnterpriseConfig();
        Pair<String, WifiConfiguration> retrieved;
        // Test serialization/deserialization for config store.
        retrieved =
                deserializeWifiConfiguration(
                        serializeWifiConfigurationForConfigStore(configuration));
        assertEquals(retrieved.first, retrieved.second.configKey());
        WifiConfigurationTestUtil.assertConfigurationEqualForConfigStore(
                configuration, retrieved.second);
    }

    /**
     * This helper method tests both the serialization for backup/restore and config store.
     */
    private void serializeDeserializeWifiConfiguration(WifiConfiguration configuration)
            throws IOException, XmlPullParserException {
        Pair<String, WifiConfiguration> retrieved;
        // Test serialization/deserialization for backup first.
        serializeDeserializeWifiConfigurationForBackupRestore(configuration);

        // Test serialization/deserialization for config store.
        serializeDeserializeWifiConfigurationForConfigStore(configuration);
    }

    private void serializeDeserializeIpConfiguration(IpConfiguration configuration)
            throws IOException, XmlPullParserException {
        // Serialize the configuration object.
        final XmlSerializer out = new FastXmlSerializer();
        final ByteArrayOutputStream outputStream = new ByteArrayOutputStream();
        out.setOutput(outputStream, StandardCharsets.UTF_8.name());
        XmlUtil.writeDocumentStart(out, mXmlDocHeader);
        IpConfigurationXmlUtil.writeToXml(out, configuration);
        XmlUtil.writeDocumentEnd(out, mXmlDocHeader);

        // Deserialize the configuration object.
        final XmlPullParser in = Xml.newPullParser();
        ByteArrayInputStream inputStream = new ByteArrayInputStream(outputStream.toByteArray());
        in.setInput(inputStream, StandardCharsets.UTF_8.name());
        XmlUtil.gotoDocumentStart(in, mXmlDocHeader);
        IpConfiguration retrievedConfiguration =
                IpConfigurationXmlUtil.parseFromXml(in, in.getDepth());
        assertEquals(configuration, retrievedConfiguration);
    }

    private byte[] serializeNetworkSelectionStatus(NetworkSelectionStatus status)
            throws IOException, XmlPullParserException {
        // Serialize the configuration object.
        final XmlSerializer out = new FastXmlSerializer();
        final ByteArrayOutputStream outputStream = new ByteArrayOutputStream();
        out.setOutput(outputStream, StandardCharsets.UTF_8.name());
        XmlUtil.writeDocumentStart(out, mXmlDocHeader);
        NetworkSelectionStatusXmlUtil.writeToXml(out, status);
        XmlUtil.writeDocumentEnd(out, mXmlDocHeader);
        return outputStream.toByteArray();
    }

    private NetworkSelectionStatus deserializeNetworkSelectionStatus(byte[] data)
            throws IOException, XmlPullParserException {
        final XmlPullParser in = Xml.newPullParser();
        ByteArrayInputStream inputStream = new ByteArrayInputStream(data);
        in.setInput(inputStream, StandardCharsets.UTF_8.name());
        XmlUtil.gotoDocumentStart(in, mXmlDocHeader);
        return NetworkSelectionStatusXmlUtil.parseFromXml(in, in.getDepth());
    }

    private void serializeDeserializeNetworkSelectionStatus(NetworkSelectionStatus status)
            throws IOException, XmlPullParserException {
        // Serialize the status object.
        byte[] data = serializeNetworkSelectionStatus(status);
        // Deserialize the status object.
        NetworkSelectionStatus retrievedStatus = deserializeNetworkSelectionStatus(data);

        WifiConfigurationTestUtil.assertNetworkSelectionStatusEqualForConfigStore(
                status, retrievedStatus);
    }

    private byte[] serializeWifiEnterpriseConfig(WifiEnterpriseConfig config)
            throws IOException, XmlPullParserException {
        final XmlSerializer out = new FastXmlSerializer();
        final ByteArrayOutputStream outputStream = new ByteArrayOutputStream();
        out.setOutput(outputStream, StandardCharsets.UTF_8.name());
        XmlUtil.writeDocumentStart(out, mXmlDocHeader);
        WifiEnterpriseConfigXmlUtil.writeToXml(
                out, config, mWifiConfigStoreEncryptionUtil);
        XmlUtil.writeDocumentEnd(out, mXmlDocHeader);
        return outputStream.toByteArray();
    }

    private WifiEnterpriseConfig deserializeWifiEnterpriseConfig(byte[] data)
            throws IOException, XmlPullParserException {
        final XmlPullParser in = Xml.newPullParser();
        ByteArrayInputStream inputStream = new ByteArrayInputStream(data);
        in.setInput(inputStream, StandardCharsets.UTF_8.name());
        XmlUtil.gotoDocumentStart(in, mXmlDocHeader);
        return WifiEnterpriseConfigXmlUtil.parseFromXml(
                in, in.getDepth(), mWifiConfigStoreEncryptionUtil != null,
                mWifiConfigStoreEncryptionUtil);
    }

    private void serializeDeserializeWifiEnterpriseConfig(WifiEnterpriseConfig config)
            throws IOException, XmlPullParserException {
        WifiEnterpriseConfig retrievedConfig =
                deserializeWifiEnterpriseConfig(serializeWifiEnterpriseConfig(config));
        WifiConfigurationTestUtil.assertWifiEnterpriseConfigEqualForConfigStore(
                config, retrievedConfig);
    }
}
