/*
 * Copyright (C) 2017 The Android Open Source Project
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

import static android.os.Process.SYSTEM_UID;

import static org.junit.Assert.*;
import static org.mockito.Mockito.*;

import android.content.Context;
import android.content.pm.PackageManager;
import android.net.MacAddress;
import android.net.wifi.WifiConfiguration;
import android.net.wifi.WifiEnterpriseConfig;
import android.util.Xml;

import androidx.test.filters.SmallTest;

import com.android.internal.util.FastXmlSerializer;
import com.android.server.wifi.util.WifiConfigStoreEncryptionUtil;
import com.android.server.wifi.util.XmlUtilTest;

import org.junit.Before;
import org.junit.Test;
import org.mockito.Mock;
import org.mockito.MockitoAnnotations;
import org.xmlpull.v1.XmlPullParser;
import org.xmlpull.v1.XmlPullParserException;
import org.xmlpull.v1.XmlSerializer;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

/**
 * Unit tests for {@link com.android.server.wifi.NetworkListStoreData}.
 */
@SmallTest
public class NetworkListStoreDataTest {

    private static final String TEST_SSID = "WifiConfigStoreDataSSID_";
    private static final String TEST_CONNECT_CHOICE = "XmlUtilConnectChoice";
    private static final long TEST_CONNECT_CHOICE_TIMESTAMP = 0x4566;
    private static final String TEST_CREATOR_NAME = "CreatorName";
    private static final MacAddress TEST_RANDOMIZED_MAC =
            MacAddress.fromString("da:a1:19:c4:26:fa");

    private static final String SINGLE_OPEN_NETWORK_DATA_XML_STRING_FORMAT =
            "<Network>\n"
                    + "<WifiConfiguration>\n"
                    + "<string name=\"ConfigKey\">%s</string>\n"
                    + "<string name=\"SSID\">%s</string>\n"
                    + "<null name=\"BSSID\" />\n"
                    + "<null name=\"PreSharedKey\" />\n"
                    + "<null name=\"WEPKeys\" />\n"
                    + "<int name=\"WEPTxKeyIndex\" value=\"0\" />\n"
                    + "<boolean name=\"HiddenSSID\" value=\"false\" />\n"
                    + "<boolean name=\"RequirePMF\" value=\"false\" />\n"
                    + "<byte-array name=\"AllowedKeyMgmt\" num=\"1\">01</byte-array>\n"
                    + "<byte-array name=\"AllowedProtocols\" num=\"0\"></byte-array>\n"
                    + "<byte-array name=\"AllowedAuthAlgos\" num=\"0\"></byte-array>\n"
                    + "<byte-array name=\"AllowedGroupCiphers\" num=\"0\"></byte-array>\n"
                    + "<byte-array name=\"AllowedPairwiseCiphers\" num=\"0\"></byte-array>\n"
                    + "<byte-array name=\"AllowedGroupMgmtCiphers\" num=\"0\"></byte-array>\n"
                    + "<byte-array name=\"AllowedSuiteBCiphers\" num=\"0\"></byte-array>\n"
                    + "<boolean name=\"Shared\" value=\"%s\" />\n"
                    + "<int name=\"Status\" value=\"2\" />\n"
                    + "<null name=\"FQDN\" />\n"
                    + "<null name=\"ProviderFriendlyName\" />\n"
                    + "<null name=\"LinkedNetworksList\" />\n"
                    + "<null name=\"DefaultGwMacAddress\" />\n"
                    + "<boolean name=\"ValidatedInternetAccess\" value=\"false\" />\n"
                    + "<boolean name=\"NoInternetAccessExpected\" value=\"false\" />\n"
                    + "<int name=\"UserApproved\" value=\"0\" />\n"
                    + "<boolean name=\"MeteredHint\" value=\"false\" />\n"
                    + "<int name=\"MeteredOverride\" value=\"2\" />\n"
                    + "<boolean name=\"UseExternalScores\" value=\"false\" />\n"
                    + "<int name=\"NumAssociation\" value=\"0\" />\n"
                    + "<int name=\"CreatorUid\" value=\"%d\" />\n"
                    + "<string name=\"CreatorName\">%s</string>\n"
                    + "<null name=\"CreationTime\" />\n"
                    + "<int name=\"LastUpdateUid\" value=\"-1\" />\n"
                    + "<null name=\"LastUpdateName\" />\n"
                    + "<int name=\"LastConnectUid\" value=\"0\" />\n"
                    + "<boolean name=\"IsLegacyPasspointConfig\" value=\"false\" />\n"
                    + "<long-array name=\"RoamingConsortiumOIs\" num=\"0\" />\n"
                    + "<string name=\"RandomizedMacAddress\">%s</string>\n"
                    + "<int name=\"MacRandomizationSetting\" value=\"1\" />\n"
                    + "</WifiConfiguration>\n"
                    + "<NetworkStatus>\n"
                    + "<string name=\"SelectionStatus\">NETWORK_SELECTION_ENABLED</string>\n"
                    + "<string name=\"DisableReason\">NETWORK_SELECTION_ENABLE</string>\n"
                    + "<null name=\"ConnectChoice\" />\n"
                    + "<long name=\"ConnectChoiceTimeStamp\" value=\"-1\" />\n"
                    + "<boolean name=\"HasEverConnected\" value=\"false\" />\n"
                    + "</NetworkStatus>\n"
                    + "<IpConfiguration>\n"
                    + "<string name=\"IpAssignment\">DHCP</string>\n"
                    + "<string name=\"ProxySettings\">NONE</string>\n"
                    + "</IpConfiguration>\n"
                    + "</Network>\n";

    private static final String SINGLE_EAP_NETWORK_DATA_XML_STRING_FORMAT =
            "<Network>\n"
                    + "<WifiConfiguration>\n"
                    + "<string name=\"ConfigKey\">%s</string>\n"
                    + "<string name=\"SSID\">%s</string>\n"
                    + "<null name=\"BSSID\" />\n"
                    + "<null name=\"PreSharedKey\" />\n"
                    + "<null name=\"WEPKeys\" />\n"
                    + "<int name=\"WEPTxKeyIndex\" value=\"0\" />\n"
                    + "<boolean name=\"HiddenSSID\" value=\"false\" />\n"
                    + "<boolean name=\"RequirePMF\" value=\"false\" />\n"
                    + "<byte-array name=\"AllowedKeyMgmt\" num=\"1\">0c</byte-array>\n"
                    + "<byte-array name=\"AllowedProtocols\" num=\"0\"></byte-array>\n"
                    + "<byte-array name=\"AllowedAuthAlgos\" num=\"0\"></byte-array>\n"
                    + "<byte-array name=\"AllowedGroupCiphers\" num=\"0\"></byte-array>\n"
                    + "<byte-array name=\"AllowedPairwiseCiphers\" num=\"0\"></byte-array>\n"
                    + "<byte-array name=\"AllowedGroupMgmtCiphers\" num=\"0\"></byte-array>\n"
                    + "<byte-array name=\"AllowedSuiteBCiphers\" num=\"0\"></byte-array>\n"
                    + "<boolean name=\"Shared\" value=\"%s\" />\n"
                    + "<int name=\"Status\" value=\"2\" />\n"
                    + "<null name=\"FQDN\" />\n"
                    + "<null name=\"ProviderFriendlyName\" />\n"
                    + "<null name=\"LinkedNetworksList\" />\n"
                    + "<null name=\"DefaultGwMacAddress\" />\n"
                    + "<boolean name=\"ValidatedInternetAccess\" value=\"false\" />\n"
                    + "<boolean name=\"NoInternetAccessExpected\" value=\"false\" />\n"
                    + "<int name=\"UserApproved\" value=\"0\" />\n"
                    + "<boolean name=\"MeteredHint\" value=\"false\" />\n"
                    + "<int name=\"MeteredOverride\" value=\"0\" />\n"
                    + "<boolean name=\"UseExternalScores\" value=\"false\" />\n"
                    + "<int name=\"NumAssociation\" value=\"0\" />\n"
                    + "<int name=\"CreatorUid\" value=\"%d\" />\n"
                    + "<string name=\"CreatorName\">%s</string>\n"
                    + "<null name=\"CreationTime\" />\n"
                    + "<int name=\"LastUpdateUid\" value=\"-1\" />\n"
                    + "<null name=\"LastUpdateName\" />\n"
                    + "<int name=\"LastConnectUid\" value=\"0\" />\n"
                    + "<boolean name=\"IsLegacyPasspointConfig\" value=\"false\" />\n"
                    + "<long-array name=\"RoamingConsortiumOIs\" num=\"0\" />\n"
                    + "<string name=\"RandomizedMacAddress\">%s</string>\n"
                    + "<int name=\"MacRandomizationSetting\" value=\"1\" />\n"
                    + "</WifiConfiguration>\n"
                    + "<NetworkStatus>\n"
                    + "<string name=\"SelectionStatus\">NETWORK_SELECTION_ENABLED</string>\n"
                    + "<string name=\"DisableReason\">NETWORK_SELECTION_ENABLE</string>\n"
                    + "<null name=\"ConnectChoice\" />\n"
                    + "<long name=\"ConnectChoiceTimeStamp\" value=\"-1\" />\n"
                    + "<boolean name=\"HasEverConnected\" value=\"false\" />\n"
                    + "</NetworkStatus>\n"
                    + "<IpConfiguration>\n"
                    + "<string name=\"IpAssignment\">DHCP</string>\n"
                    + "<string name=\"ProxySettings\">NONE</string>\n"
                    + "</IpConfiguration>\n"
                    + "<WifiEnterpriseConfiguration>\n"
                    + "<string name=\"Identity\"></string>\n"
                    + "<string name=\"AnonIdentity\"></string>\n"
                    + "<string name=\"Password\"></string>\n"
                    + "<string name=\"ClientCert\"></string>\n"
                    + "<string name=\"CaCert\"></string>\n"
                    + "<string name=\"SubjectMatch\"></string>\n"
                    + "<string name=\"Engine\"></string>\n"
                    + "<string name=\"EngineId\"></string>\n"
                    + "<string name=\"PrivateKeyId\"></string>\n"
                    + "<string name=\"AltSubjectMatch\"></string>\n"
                    + "<string name=\"DomSuffixMatch\"></string>\n"
                    + "<string name=\"CaPath\"></string>\n"
                    + "<int name=\"EapMethod\" value=\"2\" />\n"
                    + "<int name=\"Phase2Method\" value=\"0\" />\n"
                    + "<string name=\"PLMN\"></string>\n"
                    + "<string name=\"Realm\"></string>\n"
                    + "</WifiEnterpriseConfiguration>\n"
                    + "</Network>\n";

    // We use {@link NetworkListSharedStoreData} instance because {@link NetworkListStoreData} is
    // abstract.
    private NetworkListSharedStoreData mNetworkListSharedStoreData;
    @Mock private Context mContext;
    @Mock private PackageManager mPackageManager;

    @Before
    public void setUp() throws Exception {
        MockitoAnnotations.initMocks(this);
        when(mContext.getPackageManager()).thenReturn(mPackageManager);
        when(mPackageManager.getNameForUid(anyInt())).thenReturn(TEST_CREATOR_NAME);
        mNetworkListSharedStoreData = new NetworkListSharedStoreData(mContext);
    }

    /**
     * Helper function for serializing configuration data to a XML block.
     *
     * @return byte[] of the XML data
     * @throws Exception
     */
    private byte[] serializeData() throws Exception {
        final XmlSerializer out = new FastXmlSerializer();
        final ByteArrayOutputStream outputStream = new ByteArrayOutputStream();
        out.setOutput(outputStream, StandardCharsets.UTF_8.name());
        mNetworkListSharedStoreData.serializeData(out, mock(WifiConfigStoreEncryptionUtil.class));
        out.flush();
        return outputStream.toByteArray();
    }

    /**
     * Helper function for parsing configuration data from a XML block.
     *
     * @param data XML data to parse from
     * @return List of WifiConfiguration parsed
     * @throws Exception
     */
    private List<WifiConfiguration> deserializeData(byte[] data) throws Exception {
        final XmlPullParser in = Xml.newPullParser();
        final ByteArrayInputStream inputStream = new ByteArrayInputStream(data);
        in.setInput(inputStream, StandardCharsets.UTF_8.name());
        mNetworkListSharedStoreData.deserializeData(in, in.getDepth(),
                WifiConfigStore.ENCRYPT_CREDENTIALS_CONFIG_STORE_DATA_VERSION,
                mock(WifiConfigStoreEncryptionUtil.class));
        return mNetworkListSharedStoreData.getConfigurations();
    }

    /**
     * Helper function for generating a network list for testing purpose.  The network list
     * will contained an open and an EAP network.
     *
     * @param shared Flag indicating shared network
     * @return List of WifiConfiguration
     */
    private List<WifiConfiguration> getTestNetworksConfig(boolean shared) {
        WifiConfiguration openNetwork = WifiConfigurationTestUtil.createOpenNetwork();
        openNetwork.creatorName = TEST_CREATOR_NAME;
        openNetwork.shared = shared;
        openNetwork.setIpConfiguration(
                WifiConfigurationTestUtil.createDHCPIpConfigurationWithNoProxy());
        openNetwork.setRandomizedMacAddress(TEST_RANDOMIZED_MAC);
        openNetwork.meteredOverride = WifiConfiguration.METERED_OVERRIDE_NOT_METERED;
        WifiConfiguration eapNetwork = WifiConfigurationTestUtil.createEapNetwork();
        eapNetwork.shared = shared;
        eapNetwork.creatorName = TEST_CREATOR_NAME;
        eapNetwork.setIpConfiguration(
                WifiConfigurationTestUtil.createDHCPIpConfigurationWithNoProxy());
        eapNetwork.setRandomizedMacAddress(TEST_RANDOMIZED_MAC);
        List<WifiConfiguration> networkList = new ArrayList<>();
        networkList.add(openNetwork);
        networkList.add(eapNetwork);
        return networkList;
    }

    /**
     * Helper function for generating XML block containing two networks, an open and an EAP
     * network.
     *
     * @param openNetwork The WifiConfiguration for an open network
     * @param eapNetwork The WifiConfiguration for an EAP network
     * @return byte[] of the XML data
     */
    private byte[] getTestNetworksXmlBytes(WifiConfiguration openNetwork,
            WifiConfiguration eapNetwork) {
        String openNetworkXml = String.format(SINGLE_OPEN_NETWORK_DATA_XML_STRING_FORMAT,
                openNetwork.configKey().replaceAll("\"", "&quot;"),
                openNetwork.SSID.replaceAll("\"", "&quot;"),
                openNetwork.shared, openNetwork.creatorUid,
                openNetwork.creatorName, openNetwork.getRandomizedMacAddress());
        String eapNetworkXml = String.format(SINGLE_EAP_NETWORK_DATA_XML_STRING_FORMAT,
                eapNetwork.configKey().replaceAll("\"", "&quot;"),
                eapNetwork.SSID.replaceAll("\"", "&quot;"),
                eapNetwork.shared, eapNetwork.creatorUid,
                eapNetwork.creatorName, eapNetwork.getRandomizedMacAddress());
        return (openNetworkXml + eapNetworkXml).getBytes(StandardCharsets.UTF_8);
    }

    /**
     * Verify that serializing the store data without any configuration doesn't cause any crash
     * and no data should be serialized.
     *
     * @throws Exception
     */
    @Test
    public void serializeEmptyConfigs() throws Exception {
        assertEquals(0, serializeData().length);
    }

    /**
     * Verify that parsing an empty data doesn't cause any crash and no configuration should
     * be parsed.
     *
     * @throws Exception
     */
    @Test
    public void deserializeEmptyData() throws Exception {
        assertTrue(deserializeData(new byte[0]).isEmpty());
    }

    /**
     * Verify that {@link NetworkListSharedStoreData} is written to
     * {@link WifiConfigStore#STORE_FILE_NAME_SHARED_GENERAL}.
     * Verify that {@link NetworkListUserStoreData} is written to
     * {@link WifiConfigStore#STORE_FILE_NAME_USER_GENERAL}.
     *
     * @throws Exception
     */
    @Test
    public void getUserStoreFileId() throws Exception {
        assertEquals(WifiConfigStore.STORE_FILE_SHARED_GENERAL,
                mNetworkListSharedStoreData.getStoreFileId());
        assertEquals(WifiConfigStore.STORE_FILE_USER_GENERAL,
                new NetworkListUserStoreData(mContext).getStoreFileId());
    }

    /**
     * Verify that the shared configurations (containing an open and an EAP network) are serialized
     * correctly, matching the expected XML string.
     *
     * @throws Exception
     */
    @Test
    public void serializeSharedConfigurations() throws Exception {
        List<WifiConfiguration> networkList = getTestNetworksConfig(true /* shared */);
        mNetworkListSharedStoreData.setConfigurations(networkList);
        byte[] expectedData = getTestNetworksXmlBytes(networkList.get(0), networkList.get(1));
        assertTrue(Arrays.equals(expectedData, serializeData()));
    }

    /**
     * Verify that the shared configurations are parsed correctly from a XML string containing
     * test networks (an open and an EAP network).
     * @throws Exception
     */
    @Test
    public void deserializeSharedConfigurations() throws Exception {
        List<WifiConfiguration> networkList = getTestNetworksConfig(true /* shared */);
        byte[] xmlData = getTestNetworksXmlBytes(networkList.get(0), networkList.get(1));
        WifiConfigurationTestUtil.assertConfigurationsEqualForConfigStore(
                networkList, deserializeData(xmlData));
    }

    /**
     * Verify that a XmlPullParserException will be thrown when parsing a <Network> block
     * containing an unknown tag.
     *
     * @throws Exception
     */
    @Test(expected = XmlPullParserException.class)
    public void parseNetworkWithUnknownTag() throws Exception {
        String configFormat =
                "<Network>\n"
                        + "<WifiConfiguration>\n"
                        + "<string name=\"ConfigKey\">%s</string>\n"
                        + "<string name=\"SSID\">%s</string>\n"
                        + "<null name=\"BSSID\" />\n"
                        + "<null name=\"PreSharedKey\" />\n"
                        + "<null name=\"WEPKeys\" />\n"
                        + "<int name=\"WEPTxKeyIndex\" value=\"0\" />\n"
                        + "<boolean name=\"HiddenSSID\" value=\"false\" />\n"
                        + "<boolean name=\"RequirePMF\" value=\"false\" />\n"
                        + "<byte-array name=\"AllowedKeyMgmt\" num=\"1\">01</byte-array>\n"
                        + "<byte-array name=\"AllowedProtocols\" num=\"0\"></byte-array>\n"
                        + "<byte-array name=\"AllowedAuthAlgos\" num=\"0\"></byte-array>\n"
                        + "<byte-array name=\"AllowedGroupCiphers\" num=\"0\"></byte-array>\n"
                        + "<byte-array name=\"AllowedPairwiseCiphers\" num=\"0\"></byte-array>\n"
                        + "<byte-array name=\"AllowedGroupMgmtCiphers\" num=\"0\"></byte-array>\n"
                        + "<byte-array name=\"AllowedSuiteBCiphers\" num=\"0\"></byte-array>\n"
                        + "<boolean name=\"Shared\" value=\"%s\" />\n"
                        + "<null name=\"FQDN\" />\n"
                        + "<null name=\"ProviderFriendlyName\" />\n"
                        + "<null name=\"LinkedNetworksList\" />\n"
                        + "<null name=\"DefaultGwMacAddress\" />\n"
                        + "<boolean name=\"ValidatedInternetAccess\" value=\"false\" />\n"
                        + "<boolean name=\"NoInternetAccessExpected\" value=\"false\" />\n"
                        + "<int name=\"UserApproved\" value=\"0\" />\n"
                        + "<boolean name=\"MeteredHint\" value=\"false\" />\n"
                        + "<boolean name=\"UseExternalScores\" value=\"false\" />\n"
                        + "<int name=\"NumAssociation\" value=\"0\" />\n"
                        + "<int name=\"CreatorUid\" value=\"%d\" />\n"
                        + "<null name=\"CreatorName\" />\n"
                        + "<null name=\"CreationTime\" />\n"
                        + "<int name=\"LastUpdateUid\" value=\"-1\" />\n"
                        + "<null name=\"LastUpdateName\" />\n"
                        + "<int name=\"LastConnectUid\" value=\"0\" />\n"
                        + "<string name=\"RandomizedMacAddress\">%s</string>\n"
                        + "<int name=\"MacRandomizationSetting\" value=\"1\" />\n"
                        + "</WifiConfiguration>\n"
                        + "<NetworkStatus>\n"
                        + "<string name=\"SelectionStatus\">NETWORK_SELECTION_ENABLED</string>\n"
                        + "<string name=\"DisableReason\">NETWORK_SELECTION_ENABLE</string>\n"
                        + "<null name=\"ConnectChoice\" />\n"
                        + "<long name=\"ConnectChoiceTimeStamp\" value=\"-1\" />\n"
                        + "<boolean name=\"HasEverConnected\" value=\"false\" />\n"
                        + "</NetworkStatus>\n"
                        + "<IpConfiguration>\n"
                        + "<string name=\"IpAssignment\">DHCP</string>\n"
                        + "<string name=\"ProxySettings\">NONE</string>\n"
                        + "</IpConfiguration>\n"
                        + "<Unknown>"       // Unknown tag.
                        + "<int name=\"test\" value=\"0\" />\n"
                        + "</Unknown>"
                        + "</Network>\n";
        WifiConfiguration openNetwork = WifiConfigurationTestUtil.createOpenNetwork();
        byte[] xmlData = String.format(configFormat,
                openNetwork.configKey().replaceAll("\"", "&quot;"),
                openNetwork.SSID.replaceAll("\"", "&quot;"),
                openNetwork.shared, openNetwork.creatorUid, openNetwork.getRandomizedMacAddress())
            .getBytes(StandardCharsets.UTF_8);
        deserializeData(xmlData);
    }

    /**
     * Verify that a XmlPullParseException will be thrown when parsing a network configuration
     * containing a mismatched config key.
     *
     * @throws Exception
     */
    @Test(expected = XmlPullParserException.class)
    public void parseNetworkWithMismatchConfigKey() throws Exception {
        WifiConfiguration openNetwork = WifiConfigurationTestUtil.createOpenNetwork();
        byte[] xmlData = String.format(SINGLE_OPEN_NETWORK_DATA_XML_STRING_FORMAT,
                "InvalidConfigKey",
                openNetwork.SSID.replaceAll("\"", "&quot;"),
                openNetwork.shared, openNetwork.creatorUid,
                openNetwork.creatorName, openNetwork.getRandomizedMacAddress())
            .getBytes(StandardCharsets.UTF_8);
        deserializeData(xmlData);
    }

    /**
     * Tests that an invalid data in one of the WifiConfiguration object parsing would be skipped
     * gracefully. The other networks in the XML should still be parsed out correctly.
     */
    @Test
    public void parseNetworkListWithOneNetworkIllegalArgException() throws Exception {
        WifiConfiguration openNetwork = WifiConfigurationTestUtil.createOpenNetwork();
        WifiConfiguration eapNetwork = WifiConfigurationTestUtil.createEapNetwork();
        String xmlString = new String(getTestNetworksXmlBytes(openNetwork, eapNetwork));
        // Manipulate the XML data to set the EAP method to None, this should raise an Illegal
        // argument exception in WifiEnterpriseConfig.setEapMethod().
        xmlString = xmlString.replaceAll(
                String.format(XmlUtilTest.XML_STRING_EAP_METHOD_REPLACE_FORMAT,
                        eapNetwork.enterpriseConfig.getEapMethod()),
                String.format(XmlUtilTest.XML_STRING_EAP_METHOD_REPLACE_FORMAT,
                        WifiEnterpriseConfig.Eap.NONE));
        List<WifiConfiguration> retrievedNetworkList =
                deserializeData(xmlString.getBytes(StandardCharsets.UTF_8));
        // Retrieved network should not contain the eap network.
        assertEquals(1, retrievedNetworkList.size());
        for (WifiConfiguration network : retrievedNetworkList) {
            assertNotEquals(eapNetwork.SSID, network.SSID);
        }
    }

    /**
     * Verify that a saved network config with invalid creatorUid resets it to
     * {@link android.os.Process#SYSTEM_UID}.
     */
    @Test
    public void parseNetworkWithInvalidCreatorUidResetsToSystem() throws Exception {
        WifiConfiguration openNetwork = WifiConfigurationTestUtil.createOpenNetwork();
        openNetwork.creatorUid = -1;
        // Return null for invalid uid.
        when(mPackageManager.getNameForUid(eq(openNetwork.creatorUid))).thenReturn(null);

        byte[] xmlData = String.format(SINGLE_OPEN_NETWORK_DATA_XML_STRING_FORMAT,
                openNetwork.configKey().replaceAll("\"", "&quot;"),
                openNetwork.SSID.replaceAll("\"", "&quot;"),
                openNetwork.shared, openNetwork.creatorUid,
                openNetwork.creatorName, openNetwork.getRandomizedMacAddress())
            .getBytes(StandardCharsets.UTF_8);
        List<WifiConfiguration> deserializedNetworks = deserializeData(xmlData);
        assertEquals(1, deserializedNetworks.size());
        assertEquals(openNetwork.configKey(), deserializedNetworks.get(0).configKey());
        assertEquals(SYSTEM_UID, deserializedNetworks.get(0).creatorUid);
        assertEquals(TEST_CREATOR_NAME, deserializedNetworks.get(0).creatorName);
    }

    /**
     * Verify that a saved network config with invalid creatorName resets it to the package name
     * provided {@link PackageManager} for the creatorUid.
     */
    @Test
    public void parseNetworkWithInvalidCreatorNameResetsToPackageNameForCreatorUid()
            throws Exception {
        String badCreatorName = "bad";
        String correctCreatorName = "correct";
        WifiConfiguration openNetwork = WifiConfigurationTestUtil.createOpenNetwork();
        openNetwork.creatorUid = 1324422;
        openNetwork.creatorName = badCreatorName;
        when(mPackageManager.getNameForUid(eq(openNetwork.creatorUid)))
            .thenReturn(correctCreatorName);

        byte[] xmlData = String.format(SINGLE_OPEN_NETWORK_DATA_XML_STRING_FORMAT,
                openNetwork.configKey().replaceAll("\"", "&quot;"),
                openNetwork.SSID.replaceAll("\"", "&quot;"),
                openNetwork.shared, openNetwork.creatorUid,
                openNetwork.creatorName, openNetwork.getRandomizedMacAddress())
            .getBytes(StandardCharsets.UTF_8);
        List<WifiConfiguration> deserializedNetworks = deserializeData(xmlData);
        assertEquals(1, deserializedNetworks.size());
        assertEquals(openNetwork.configKey(), deserializedNetworks.get(0).configKey());
        assertEquals(openNetwork.creatorUid, deserializedNetworks.get(0).creatorUid);
        assertEquals(correctCreatorName, deserializedNetworks.get(0).creatorName);
    }

    /**
     * Verify that a saved network config with invalid creatorName resets it to the package name
     * provided {@link PackageManager} for the creatorUid.
     */
    @Test
    public void parseNetworkWithNullCreatorNameResetsToPackageNameForCreatorUid()
            throws Exception {
        String correctCreatorName = "correct";
        WifiConfiguration openNetwork = WifiConfigurationTestUtil.createOpenNetwork();
        openNetwork.creatorUid = 1324422;
        openNetwork.creatorName = null;
        when(mPackageManager.getNameForUid(eq(openNetwork.creatorUid)))
            .thenReturn(correctCreatorName);

        byte[] xmlData = String.format(SINGLE_OPEN_NETWORK_DATA_XML_STRING_FORMAT,
                openNetwork.configKey().replaceAll("\"", "&quot;"),
                openNetwork.SSID.replaceAll("\"", "&quot;"),
                openNetwork.shared, openNetwork.creatorUid,
                openNetwork.creatorName, openNetwork.getRandomizedMacAddress())
            .getBytes(StandardCharsets.UTF_8);
        List<WifiConfiguration> deserializedNetworks = deserializeData(xmlData);
        assertEquals(1, deserializedNetworks.size());
        assertEquals(openNetwork.configKey(), deserializedNetworks.get(0).configKey());
        assertEquals(openNetwork.creatorUid, deserializedNetworks.get(0).creatorUid);
        assertEquals(correctCreatorName, deserializedNetworks.get(0).creatorName);
    }

    /**
     * Verify that a saved network config with valid creatorUid is preserved.
     */
    @Test
    public void parseNetworkWithValidCreatorUid() throws Exception {
        WifiConfiguration openNetwork = WifiConfigurationTestUtil.createOpenNetwork();
        openNetwork.creatorUid = 1324422;

        byte[] xmlData = String.format(SINGLE_OPEN_NETWORK_DATA_XML_STRING_FORMAT,
                openNetwork.configKey().replaceAll("\"", "&quot;"),
                openNetwork.SSID.replaceAll("\"", "&quot;"),
                openNetwork.shared, openNetwork.creatorUid,
                openNetwork.creatorName, openNetwork.getRandomizedMacAddress())
            .getBytes(StandardCharsets.UTF_8);
        List<WifiConfiguration> deserializedNetworks = deserializeData(xmlData);
        assertEquals(1, deserializedNetworks.size());
        assertEquals(openNetwork.configKey(), deserializedNetworks.get(0).configKey());
        assertEquals(openNetwork.creatorUid, deserializedNetworks.get(0).creatorUid);
        assertEquals(TEST_CREATOR_NAME, deserializedNetworks.get(0).creatorName);
    }
}
