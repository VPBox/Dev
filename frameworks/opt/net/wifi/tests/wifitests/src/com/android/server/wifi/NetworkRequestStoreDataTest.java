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

import android.net.MacAddress;
import android.net.wifi.WifiConfiguration;
import android.util.Xml;

import androidx.test.filters.SmallTest;

import com.android.internal.util.FastXmlSerializer;
import com.android.server.wifi.WifiNetworkFactory.AccessPoint;
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
import java.util.HashSet;
import java.util.Map;
import java.util.Set;

/**
 * Unit tests for {@link com.android.server.wifi.NetworkRequestStoreData}.
 */
@SmallTest
public class NetworkRequestStoreDataTest {
    private static final String TEST_PACKAGE_NAME_1 = "com.android.test.1";
    private static final String TEST_PACKAGE_NAME_2 = "com.android.test.2";
    private static final String TEST_CORRUPT_DATA_INVALID_SSID =
            "<NetworkRequestMap>\n"
            + "<ApprovedAccessPointsPerApp>\n"
            + "<string name=\"RequestorPackageName\">com.android.test.1</string>\n"
            + "<AccessPoint>\n"
            + "<blah blah=\"SSID\">&quot;WifiConfigurationTestUtilSSID7&quot;</blah>\n"
            + "<null name=\"BSSID\" />\n"
            + "<int name=\"NetworkType\" value=\"2\" />\n"
            + "</AccessPoint>\n"
            + "</ApprovedAccessPointsPerApp>"
            + "</NetworkRequestMap>";

    private @Mock NetworkRequestStoreData.DataSource mDataSource;
    private NetworkRequestStoreData mNetworkRequestStoreData;

    @Before
    public void setUp() throws Exception {
        MockitoAnnotations.initMocks(this);
        mNetworkRequestStoreData = new NetworkRequestStoreData(mDataSource);
    }

    /**
     * Helper function for serializing configuration data to a XML block.
     */
    private byte[] serializeData() throws Exception {
        final XmlSerializer out = new FastXmlSerializer();
        final ByteArrayOutputStream outputStream = new ByteArrayOutputStream();
        out.setOutput(outputStream, StandardCharsets.UTF_8.name());
        mNetworkRequestStoreData.serializeData(out, mock(WifiConfigStoreEncryptionUtil.class));
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
        mNetworkRequestStoreData.deserializeData(in, in.getDepth(),
                WifiConfigStore.ENCRYPT_CREDENTIALS_CONFIG_STORE_DATA_VERSION,
                mock(WifiConfigStoreEncryptionUtil.class));
    }

    /**
     * Verify store file Id.
     */
    @Test
    public void verifyStoreFileId() throws Exception {
        assertEquals(WifiConfigStore.STORE_FILE_USER_GENERAL,
                mNetworkRequestStoreData.getStoreFileId());
    }

    /**
     * Serialize/Deserialize a single approved access points from a single app.
     */
    @Test
    public void serializeDeserializeSingleNetworkRequestFromSingleApp() throws Exception {
        Map<String, Set<AccessPoint>> approvedAccessPointsMap = new HashMap<>();

        AccessPoint accessPoint = new AccessPoint(
                WifiConfigurationTestUtil.createPskNetwork().SSID,
                MacAddress.createRandomUnicastAddress(), WifiConfiguration.SECURITY_TYPE_PSK);
        Set<AccessPoint> accessPoints = new HashSet<AccessPoint>() {{
                        add(accessPoint);
                }};
        approvedAccessPointsMap.put(TEST_PACKAGE_NAME_1, accessPoints);

        assertSerializeDeserialize(approvedAccessPointsMap);
    }

    /**
     * Serialize/Deserialize a single approved access point from multiple apps.
     */
    @Test
    public void serializeDeserializeSingleNetworkRequestFromMultipleApps() throws Exception {
        Map<String, Set<AccessPoint>> approvedAccessPointsMap = new HashMap<>();

        AccessPoint accessPoint1 = new AccessPoint(
                WifiConfigurationTestUtil.createPskNetwork().SSID,
                MacAddress.createRandomUnicastAddress(), WifiConfiguration.SECURITY_TYPE_PSK);
        Set<AccessPoint> accessPoints1 = new HashSet<AccessPoint>() {{
                        add(accessPoint1);
                }};
        approvedAccessPointsMap.put(TEST_PACKAGE_NAME_1, accessPoints1);

        AccessPoint accessPoint2 = new AccessPoint(
                WifiConfigurationTestUtil.createPskNetwork().SSID,
                MacAddress.createRandomUnicastAddress(), WifiConfiguration.SECURITY_TYPE_PSK);
        Set<AccessPoint> accessPoints2 = new HashSet<AccessPoint>() {{
                        add(accessPoint2);
                }};
        approvedAccessPointsMap.put(TEST_PACKAGE_NAME_2, accessPoints2);

        assertSerializeDeserialize(approvedAccessPointsMap);
    }

    /**
     * Serialize/Deserialize multiple approved access points from multiple apps.
     */
    @Test
    public void serializeDeserializeMultipleNetworkRequestFromMultipleApps() throws Exception {
        Map<String, Set<AccessPoint>> approvedAccessPointsMap = new HashMap<>();

        AccessPoint accessPoint1 = new AccessPoint(
                WifiConfigurationTestUtil.createPskNetwork().SSID,
                MacAddress.createRandomUnicastAddress(), WifiConfiguration.SECURITY_TYPE_PSK);
        AccessPoint accessPoint2 = new AccessPoint(
                WifiConfigurationTestUtil.createOpenNetwork().SSID,
                MacAddress.createRandomUnicastAddress(), WifiConfiguration.SECURITY_TYPE_OPEN);
        Set<AccessPoint> accessPoints1 = new HashSet<AccessPoint>() {{
                        add(accessPoint1);
                        add(accessPoint2);
                }};
        approvedAccessPointsMap.put(TEST_PACKAGE_NAME_1, accessPoints1);

        AccessPoint accessPoint3 = new AccessPoint(
                WifiConfigurationTestUtil.createSaeNetwork().SSID,
                MacAddress.createRandomUnicastAddress(), WifiConfiguration.SECURITY_TYPE_SAE);
        AccessPoint accessPoint4 = new AccessPoint(
                WifiConfigurationTestUtil.createOpenNetwork().SSID,
                MacAddress.createRandomUnicastAddress(), WifiConfiguration.SECURITY_TYPE_OPEN);
        Set<AccessPoint> accessPoints2 = new HashSet<AccessPoint>() {{
                        add(accessPoint3);
                        add(accessPoint4);
                }};
        approvedAccessPointsMap.put(TEST_PACKAGE_NAME_2, accessPoints2);

        assertSerializeDeserialize(approvedAccessPointsMap);
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

    private void assertSerializeDeserialize(
            Map<String, Set<AccessPoint>> approvedAccessPointsMap)
            throws Exception {
        // Setup the data to serialize.
        when(mDataSource.toSerialize()).thenReturn(approvedAccessPointsMap);

        // Serialize/deserialize data.
        deserializeData(serializeData());

        // Verify the deserialized data.
        ArgumentCaptor<HashMap> deserializedApprovedAccessPointsMap =
                ArgumentCaptor.forClass(HashMap.class);
        verify(mDataSource).fromDeserialized(deserializedApprovedAccessPointsMap.capture());
        assertEquals(approvedAccessPointsMap, deserializedApprovedAccessPointsMap.getValue());
    }
}
