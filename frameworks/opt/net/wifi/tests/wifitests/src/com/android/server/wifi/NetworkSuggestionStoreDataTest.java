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

import android.net.wifi.WifiConfiguration;
import android.net.wifi.WifiNetworkSuggestion;
import android.util.Xml;

import androidx.test.filters.SmallTest;

import com.android.internal.util.FastXmlSerializer;
import com.android.server.wifi.WifiNetworkSuggestionsManager.ExtendedWifiNetworkSuggestion;
import com.android.server.wifi.WifiNetworkSuggestionsManager.PerAppInfo;
import com.android.server.wifi.util.WifiConfigStoreEncryptionUtil;

import org.junit.Before;
import org.junit.Test;
import org.mockito.ArgumentCaptor;
import org.mockito.Mock;
import org.mockito.MockitoAnnotations;
import org.xmlpull.v1.XmlPullParser;
import org.xmlpull.v1.XmlPullParserException;
import org.xmlpull.v1.XmlSerializer;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.nio.charset.StandardCharsets;
import java.util.HashMap;
import java.util.Map;

/**
 * Unit tests for {@link com.android.server.wifi.NetworkSuggestionStoreData}.
 */
@SmallTest
public class NetworkSuggestionStoreDataTest {
    private static final int TEST_UID_1 = 14556;
    private static final int TEST_UID_2 = 14536;
    private static final String TEST_PACKAGE_NAME_1 = "com.android.test.1";
    private static final String TEST_PACKAGE_NAME_2 = "com.android.test.2";
    private static final String TEST_CORRUPT_DATA_INVALID_SSID =
            "<NetworkSuggestionPerApp>\n"
            + "<string name=\"SuggestorPackageName\">com.android.test.1</string>\n"
            + "<boolean name=\"SuggestorHasUserApproved\" value=\"false\" />\n"
            + "<NetworkSuggestion>\n"
            + "<WifiConfiguration>\n"
            + "<string name=\"ConfigKey\">&quot;WifiConfigurationTestUtilSSID7&quot;NONE</string>\n"
            + "<blah blah=\"SSID\">&quot;WifiConfigurationTestUtilSSID7&quot;</blah>\n"
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
            + "<boolean name=\"Shared\" value=\"true\" />\n"
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
            + "<int name=\"CreatorUid\" value=\"5\" />\n"
            + "<null name=\"CreatorName\" />\n"
            + "<null name=\"CreationTime\" />\n"
            + "<int name=\"LastUpdateUid\" value=\"-1\" />\n"
            + "<null name=\"LastUpdateName\" />\n"
            + "<int name=\"LastConnectUid\" value=\"0\" />\n"
            + "<boolean name=\"IsLegacyPasspointConfig\" value=\"false\" />\n"
            + "<long-array name=\"RoamingConsortiumOIs\" num=\"0\" />\n"
            + "<string name=\"RandomizedMacAddress\">02:00:00:00:00:00</string>\n"
            + "</WifiConfiguration>\n"
            + "<boolean name=\"IsAppInteractionRequired\" value=\"false\" />\n"
            + "<boolean name=\"IsUserInteractionRequired\" value=\"false\" />\n"
            + "<int name=\"SuggestorUid\" value=\"14556\" />\n"
            + "</NetworkSuggestion>\n"
            + "</NetworkSuggestionPerApp>";

    private @Mock NetworkSuggestionStoreData.DataSource mDataSource;
    private NetworkSuggestionStoreData mNetworkSuggestionStoreData;

    @Before
    public void setUp() throws Exception {
        MockitoAnnotations.initMocks(this);
        mNetworkSuggestionStoreData = new NetworkSuggestionStoreData(mDataSource);
    }

    /**
     * Helper function for serializing configuration data to a XML block.
     */
    private byte[] serializeData() throws Exception {
        final XmlSerializer out = new FastXmlSerializer();
        final ByteArrayOutputStream outputStream = new ByteArrayOutputStream();
        out.setOutput(outputStream, StandardCharsets.UTF_8.name());
        mNetworkSuggestionStoreData.serializeData(out, mock(WifiConfigStoreEncryptionUtil.class));
        out.flush();
        return outputStream.toByteArray();
    }

    /**
     * Helper function for parsing configuration data from a XML block.
     */
    private void deserializeData(byte[] data) throws Exception {
        final XmlPullParser in = Xml.newPullParser();
        final ByteArrayInputStream inputStream = new ByteArrayInputStream(data);
        in.setInput(inputStream, StandardCharsets.UTF_8.name());
        mNetworkSuggestionStoreData.deserializeData(in, in.getDepth(),
                WifiConfigStore.ENCRYPT_CREDENTIALS_CONFIG_STORE_DATA_VERSION,
                mock(WifiConfigStoreEncryptionUtil.class));
    }

    /**
     * Verify store file Id.
     */
    @Test
    public void verifyStoreFileId() throws Exception {
        assertEquals(WifiConfigStore.STORE_FILE_USER_NETWORK_SUGGESTIONS,
                mNetworkSuggestionStoreData.getStoreFileId());
    }

    /**
     * Serialize/Deserialize a single network suggestion from a single app.
     */
    @Test
    public void serializeDeserializeSingleNetworkSuggestionFromSingleApp() throws Exception {
        Map<String, PerAppInfo> networkSuggestionsMap = new HashMap<>();

        PerAppInfo appInfo = new PerAppInfo(TEST_PACKAGE_NAME_1);

        WifiConfiguration configuration = WifiConfigurationTestUtil.createEapNetwork();
        configuration.enterpriseConfig =
                WifiConfigurationTestUtil.createPEAPWifiEnterpriseConfigWithGTCPhase2();
        WifiNetworkSuggestion networkSuggestion =
                new WifiNetworkSuggestion(configuration, false, false, TEST_UID_1,
                TEST_PACKAGE_NAME_1);
        appInfo.hasUserApproved = false;
        appInfo.extNetworkSuggestions.add(
                ExtendedWifiNetworkSuggestion.fromWns(networkSuggestion, appInfo));
        networkSuggestionsMap.put(TEST_PACKAGE_NAME_1, appInfo);

        Map<String, PerAppInfo> deserializedPerAppInfoMap =
                assertSerializeDeserialize(networkSuggestionsMap);
        ExtendedWifiNetworkSuggestion deserializedSuggestion =
                deserializedPerAppInfoMap.get(TEST_PACKAGE_NAME_1).extNetworkSuggestions.stream()
                        .findAny()
                        .orElse(null);

        WifiConfigurationTestUtil.assertConfigurationEqual(
                configuration, deserializedSuggestion.wns.wifiConfiguration);
        WifiConfigurationTestUtil.assertWifiEnterpriseConfigEqualForConfigStore(
                configuration.enterpriseConfig,
                deserializedSuggestion.wns.wifiConfiguration.enterpriseConfig);
    }

    /**
     * Serialize/Deserialize a single network suggestion from multiple apps.
     */
    @Test
    public void serializeDeserializeSingleNetworkSuggestionFromMultipleApps() throws Exception {
        Map<String, PerAppInfo> networkSuggestionsMap = new HashMap<>();

        PerAppInfo appInfo1 = new PerAppInfo(TEST_PACKAGE_NAME_1);
        WifiNetworkSuggestion networkSuggestion1 = new WifiNetworkSuggestion(
                WifiConfigurationTestUtil.createOpenNetwork(), false, false, TEST_UID_1,
                TEST_PACKAGE_NAME_1);
        appInfo1.hasUserApproved = false;
        appInfo1.extNetworkSuggestions.add(
                ExtendedWifiNetworkSuggestion.fromWns(networkSuggestion1, appInfo1));
        networkSuggestionsMap.put(TEST_PACKAGE_NAME_1, appInfo1);

        PerAppInfo appInfo2 = new PerAppInfo(TEST_PACKAGE_NAME_2);
        WifiNetworkSuggestion networkSuggestion2 = new WifiNetworkSuggestion(
                WifiConfigurationTestUtil.createOpenNetwork(), true, false, TEST_UID_2,
                TEST_PACKAGE_NAME_2);
        appInfo2.hasUserApproved = true;
        appInfo2.extNetworkSuggestions.add(
                ExtendedWifiNetworkSuggestion.fromWns(networkSuggestion2, appInfo2));
        networkSuggestionsMap.put(TEST_PACKAGE_NAME_2, appInfo2);

        assertSerializeDeserialize(networkSuggestionsMap);
    }

    /**
     * Serialize/Deserialize multiple network suggestion from multiple apps.
     */
    @Test
    public void serializeDeserializeMultipleNetworkSuggestionFromMultipleApps() throws Exception {
        Map<String, PerAppInfo> networkSuggestionsMap = new HashMap<>();

        PerAppInfo appInfo1 = new PerAppInfo(TEST_PACKAGE_NAME_1);
        WifiNetworkSuggestion networkSuggestion1 = new WifiNetworkSuggestion(
                WifiConfigurationTestUtil.createOpenNetwork(), false, true, TEST_UID_1,
                TEST_PACKAGE_NAME_1);
        WifiNetworkSuggestion networkSuggestion2 = new WifiNetworkSuggestion(
                WifiConfigurationTestUtil.createOpenNetwork(), true, false, TEST_UID_1,
                TEST_PACKAGE_NAME_1);
        appInfo1.hasUserApproved = true;
        appInfo1.extNetworkSuggestions.add(
                ExtendedWifiNetworkSuggestion.fromWns(networkSuggestion1, appInfo1));
        appInfo1.extNetworkSuggestions.add(
                ExtendedWifiNetworkSuggestion.fromWns(networkSuggestion2, appInfo1));
        networkSuggestionsMap.put(TEST_PACKAGE_NAME_1, appInfo1);

        PerAppInfo appInfo2 = new PerAppInfo(TEST_PACKAGE_NAME_2);
        WifiNetworkSuggestion networkSuggestion3 = new WifiNetworkSuggestion(
                WifiConfigurationTestUtil.createOpenNetwork(), true, false, TEST_UID_2,
                TEST_PACKAGE_NAME_2);
        WifiNetworkSuggestion networkSuggestion4 = new WifiNetworkSuggestion(
                WifiConfigurationTestUtil.createOpenNetwork(), false, true, TEST_UID_2,
                TEST_PACKAGE_NAME_2);
        appInfo2.hasUserApproved = true;
        appInfo2.extNetworkSuggestions.add(
                ExtendedWifiNetworkSuggestion.fromWns(networkSuggestion3, appInfo2));
        appInfo2.extNetworkSuggestions.add(
                ExtendedWifiNetworkSuggestion.fromWns(networkSuggestion4, appInfo2));
        networkSuggestionsMap.put(TEST_PACKAGE_NAME_2, appInfo2);

        assertSerializeDeserialize(networkSuggestionsMap);
    }

    /**
     * Deserialize corrupt data and ensure that we gracefully handle any errors in the data.
     * graceful == throw XmlPullParserException (which is handled in
     * {@link WifiConfigManager#loadFromStore()}).
     */
    @Test(expected = XmlPullParserException.class)
    public void deserializeCorruptData() throws Exception {
        deserializeData(TEST_CORRUPT_DATA_INVALID_SSID.getBytes());
    }

    private Map<String, PerAppInfo> assertSerializeDeserialize(
            Map<String, PerAppInfo> networkSuggestionsMap) throws Exception {
        // Setup the data to serialize.
        when(mDataSource.toSerialize()).thenReturn(networkSuggestionsMap);

        // Serialize/deserialize data.
        deserializeData(serializeData());

        // Verify the deserialized data.
        ArgumentCaptor<HashMap> deserializedNetworkSuggestionsMap =
                ArgumentCaptor.forClass(HashMap.class);
        verify(mDataSource).fromDeserialized(deserializedNetworkSuggestionsMap.capture());
        assertEquals(networkSuggestionsMap, deserializedNetworkSuggestionsMap.getValue());
        return deserializedNetworkSuggestionsMap.getValue();
    }
}
