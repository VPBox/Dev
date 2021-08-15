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

import android.app.test.TestAlarmManager;
import android.content.Context;
import android.content.pm.PackageManager;
import android.net.MacAddress;
import android.net.wifi.WifiConfiguration;
import android.os.test.TestLooper;

import androidx.test.filters.SmallTest;

import com.android.internal.util.ArrayUtils;
import com.android.server.wifi.WifiConfigStore.StoreData;
import com.android.server.wifi.WifiConfigStore.StoreFile;
import com.android.server.wifi.util.EncryptedData;
import com.android.server.wifi.util.WifiConfigStoreEncryptionUtil;
import com.android.server.wifi.util.XmlUtil;

import libcore.util.HexEncoding;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.mockito.Mock;
import org.mockito.MockitoAnnotations;
import org.xmlpull.v1.XmlPullParser;
import org.xmlpull.v1.XmlPullParserException;
import org.xmlpull.v1.XmlSerializer;

import java.io.File;
import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Random;

/**
 * Unit tests for {@link com.android.server.wifi.WifiConfigStore}.
 */
@SmallTest
public class WifiConfigStoreTest {
    private static final String TEST_USER_DATA = "UserData";
    private static final String TEST_SHARE_DATA = "ShareData";
    private static final String TEST_CREATOR_NAME = "CreatorName";
    private static final MacAddress TEST_RANDOMIZED_MAC =
            MacAddress.fromString("da:a1:19:c4:26:fa");

    private static final String TEST_DATA_XML_STRING_FORMAT =
            "<?xml version='1.0' encoding='utf-8' standalone='yes' ?>\n"
                    + "<WifiConfigStoreData>\n"
                    + "<int name=\"Version\" value=\"3\" />\n"
                    + "<NetworkList>\n"
                    + "<Network>\n"
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
                    + "</Network>\n"
                    + "</NetworkList>\n"
                    + "<DeletedEphemeralSSIDList>\n"
                    + "<map name=\"SSIDList\">\n"
                    + "<long name=\"%s\" value=\"0\" />\n"
                    + "</map>\n"
                    + "</DeletedEphemeralSSIDList>\n"
                    + "</WifiConfigStoreData>\n";

    private static final String TEST_DATA_XML_STRING_FORMAT_V1_WITH_ONE_DATA_SOURCE =
            "<?xml version='1.0' encoding='utf-8' standalone='yes' ?>\n"
                    + "<WifiConfigStoreData>\n"
                    + "<int name=\"Version\" value=\"1\" />\n"
                    + "<%s/>n"
                    + "</WifiConfigStoreData>\n";
    private static final String TEST_DATA_XML_STRING_FORMAT_V1_WITH_TWO_DATA_SOURCE =
            "<?xml version='1.0' encoding='utf-8' standalone='yes' ?>\n"
                    + "<WifiConfigStoreData>\n"
                    + "<int name=\"Version\" value=\"1\" />\n"
                    + "<%s/>n"
                    + "<%s/>n"
                    + "</WifiConfigStoreData>\n";
    private static final String TEST_DATA_XML_STRING_FORMAT_V2_WITH_ONE_DATA_SOURCE =
            "<?xml version='1.0' encoding='utf-8' standalone='yes' ?>\n"
                    + "<WifiConfigStoreData>\n"
                    + "<int name=\"Version\" value=\"2\" />\n"
                    + "<Integrity>\n"
                    + "<byte-array name=\"EncryptedData\" num=\"48\">%s</byte-array>\n"
                    + "<byte-array name=\"IV\" num=\"12\">%s</byte-array>\n"
                    + "</Integrity>\n"
                    + "<%s />\n"
                    + "</WifiConfigStoreData>\n";
    // Test mocks
    @Mock private Context mContext;
    @Mock private PackageManager mPackageManager;
    private TestAlarmManager mAlarmManager;
    private TestLooper mLooper;
    @Mock private Clock mClock;
    @Mock private WifiMetrics mWifiMetrics;
    @Mock private WifiConfigStoreEncryptionUtil mEncryptionUtil;
    private MockStoreFile mSharedStore;
    private MockStoreFile mUserStore;
    private MockStoreFile mUserNetworkSuggestionsStore;
    private List<StoreFile> mUserStores = new ArrayList<StoreFile>();
    private MockStoreData mSharedStoreData;
    private MockStoreData mUserStoreData;

    /**
     * Test instance of WifiConfigStore.
     */
    private WifiConfigStore mWifiConfigStore;

    /**
     * Setup mocks before the test starts.
     */
    private void setupMocks() throws Exception {
        MockitoAnnotations.initMocks(this);
        mAlarmManager = new TestAlarmManager();
        mLooper = new TestLooper();
        when(mContext.getSystemService(Context.ALARM_SERVICE))
                .thenReturn(mAlarmManager.getAlarmManager());
        when(mContext.getPackageManager()).thenReturn(mPackageManager);
        when(mPackageManager.getNameForUid(anyInt())).thenReturn(TEST_CREATOR_NAME);
        when(mEncryptionUtil.encrypt(any(byte[].class)))
                .thenReturn(new EncryptedData(new byte[0], new byte[0]));
        when(mEncryptionUtil.decrypt(any(EncryptedData.class)))
                .thenReturn(new byte[0]);
        mSharedStore = new MockStoreFile(WifiConfigStore.STORE_FILE_SHARED_GENERAL);
        mUserStore = new MockStoreFile(WifiConfigStore.STORE_FILE_USER_GENERAL);
        mUserNetworkSuggestionsStore =
                new MockStoreFile(WifiConfigStore.STORE_FILE_USER_NETWORK_SUGGESTIONS);
        mUserStores.add(mUserStore);
        mUserStores.add(mUserNetworkSuggestionsStore);

        mSharedStoreData = new MockStoreData(WifiConfigStore.STORE_FILE_SHARED_GENERAL);
        mUserStoreData = new MockStoreData(WifiConfigStore.STORE_FILE_USER_GENERAL);
    }

    /**
     * Setup the test environment.
     */
    @Before
    public void setUp() throws Exception {
        setupMocks();

        mWifiConfigStore = new WifiConfigStore(mContext, mLooper.getLooper(), mClock, mWifiMetrics,
                mSharedStore);
        // Enable verbose logging before tests.
        mWifiConfigStore.enableVerboseLogging(true);
    }

    /**
     * Called after each test
     */
    @After
    public void cleanup() {
        validateMockitoUsage();
    }

    /**
     * Verify that no write occurs if there is {@link StoreData} registered for any
     * {@link StoreFile}.
     *
     * @throws Exception
     */
    @Test
    public void testWriteWithNoStoreData() throws Exception {
        // Perform force write to both share and user store file.
        mWifiConfigStore.setUserStores(mUserStores);
        mWifiConfigStore.write(true);

        assertFalse(mSharedStore.isStoreWritten());
        assertFalse(mUserStore.isStoreWritten());
        assertFalse(mUserNetworkSuggestionsStore.isStoreWritten());

        verify(mWifiMetrics, never()).noteWifiConfigStoreWriteDuration(anyInt());
    }

    /**
     * Tests the write API with the force flag set to true.
     * Expected behavior: This should trigger an immediate write to the store files and no alarms
     * should be started.
     */
    @Test
    public void testForceWrite() throws Exception {
        // Register data container.
        mWifiConfigStore.registerStoreData(mSharedStoreData);
        mWifiConfigStore.registerStoreData(mUserStoreData);

        mWifiConfigStore.switchUserStoresAndRead(mUserStores);
        mWifiConfigStore.write(true);

        assertFalse(mAlarmManager.isPending(WifiConfigStore.BUFFERED_WRITE_ALARM_TAG));
        assertTrue(mSharedStore.isStoreWritten());
        assertTrue(mUserStore.isStoreWritten());
        assertFalse(mUserNetworkSuggestionsStore.isStoreWritten());

        verify(mWifiMetrics).noteWifiConfigStoreWriteDuration(anyInt());
    }

    /**
     * Tests the write API with the force flag set to false.
     * Expected behavior: This should set an alarm to write to the store files.
     */
    @Test
    public void testBufferedWrite() throws Exception {
        // Register data container.
        mWifiConfigStore.registerStoreData(mSharedStoreData);
        mWifiConfigStore.registerStoreData(mUserStoreData);

        mWifiConfigStore.switchUserStoresAndRead(mUserStores);
        mWifiConfigStore.write(false);

        assertTrue(mAlarmManager.isPending(WifiConfigStore.BUFFERED_WRITE_ALARM_TAG));
        assertFalse(mSharedStore.isStoreWritten());
        assertFalse(mUserStore.isStoreWritten());
        assertFalse(mUserNetworkSuggestionsStore.isStoreWritten());

        // Now send the alarm and ensure that the writes happen.
        mAlarmManager.dispatch(WifiConfigStore.BUFFERED_WRITE_ALARM_TAG);
        mLooper.dispatchAll();
        assertTrue(mSharedStore.isStoreWritten());
        assertTrue(mUserStore.isStoreWritten());
        assertFalse(mUserNetworkSuggestionsStore.isStoreWritten());

        verify(mWifiMetrics).noteWifiConfigStoreWriteDuration(anyInt());
    }

    /**
     * Tests the force write after a buffered write.
     * Expected behaviour: The force write should override the previous buffered write and stop the
     * buffer write alarms.
     */
    @Test
    public void testForceWriteAfterBufferedWrite() throws Exception {
        // Register a test data container with bogus data.
        mWifiConfigStore.registerStoreData(mSharedStoreData);
        mWifiConfigStore.registerStoreData(mUserStoreData);

        mSharedStoreData.setData("abcds");
        mUserStoreData.setData("asdfa");

        // Perform buffered write for both user and share store file.
        mWifiConfigStore.switchUserStoresAndRead(mUserStores);
        mWifiConfigStore.write(false);

        assertTrue(mAlarmManager.isPending(WifiConfigStore.BUFFERED_WRITE_ALARM_TAG));
        assertFalse(mSharedStore.isStoreWritten());
        assertFalse(mUserStore.isStoreWritten());

        // Update the container with new set of data. The send a force write and ensure that the
        // writes have been performed and alarms have been stopped and updated data are written.
        mUserStoreData.setData(TEST_USER_DATA);
        mSharedStoreData.setData(TEST_SHARE_DATA);
        mWifiConfigStore.write(true);

        assertFalse(mAlarmManager.isPending(WifiConfigStore.BUFFERED_WRITE_ALARM_TAG));
        assertTrue(mSharedStore.isStoreWritten());
        assertTrue(mUserStore.isStoreWritten());

        // Verify correct data are loaded to the data container after a read.
        mWifiConfigStore.read();
        assertEquals(TEST_USER_DATA, mUserStoreData.getData());
        assertEquals(TEST_SHARE_DATA, mSharedStoreData.getData());
    }

    /**
     * Tests the force write with no new data after a buffered write.
     * Expected behaviour: The force write should flush the previous buffered write and stop the
     * buffer write alarms.
     */
    @Test
    public void testForceWriteWithNoNewDataAfterBufferedWrite() throws Exception {
        // Register a test data container with bogus data.
        mWifiConfigStore.registerStoreData(mSharedStoreData);
        mWifiConfigStore.registerStoreData(mUserStoreData);

        mSharedStoreData.setData("abcds");
        mUserStoreData.setData("asdfa");

        // Perform buffered write for both user and share store file.
        mWifiConfigStore.switchUserStoresAndRead(mUserStores);
        mWifiConfigStore.write(false);

        assertTrue(mAlarmManager.isPending(WifiConfigStore.BUFFERED_WRITE_ALARM_TAG));
        assertFalse(mSharedStore.isStoreWritten());
        assertFalse(mUserStore.isStoreWritten());

        // Containers have no new data.
        mUserStoreData.setHasAnyNewData(false);
        mSharedStoreData.setHasAnyNewData(false);
        mWifiConfigStore.write(true);

        assertFalse(mAlarmManager.isPending(WifiConfigStore.BUFFERED_WRITE_ALARM_TAG));
        assertTrue(mSharedStore.isStoreWritten());
        assertTrue(mUserStore.isStoreWritten());

        // Verify correct data are loaded to the data container after a read.
        mWifiConfigStore.read();
        assertEquals("abcds", mSharedStoreData.getData());
        assertEquals("asdfa", mUserStoreData.getData());
    }


    /**
     * Tests the read API behaviour after a write to the store files.
     * Expected behaviour: The read should return the same data that was last written.
     */
    @Test
    public void testReadAfterWrite() throws Exception {
        // Register data container.
        mWifiConfigStore.registerStoreData(mSharedStoreData);
        mWifiConfigStore.registerStoreData(mUserStoreData);

        // Read both share and user config store.
        mWifiConfigStore.switchUserStoresAndRead(mUserStores);

        // Verify no data is read.
        assertNull(mUserStoreData.getData());
        assertNull(mSharedStoreData.getData());

        // Write share and user data.
        mUserStoreData.setData(TEST_USER_DATA);
        mSharedStoreData.setData(TEST_SHARE_DATA);
        mWifiConfigStore.write(true);

        // Read and verify the data content in the data container.
        mWifiConfigStore.read();
        assertEquals(TEST_USER_DATA, mUserStoreData.getData());
        assertEquals(TEST_SHARE_DATA, mSharedStoreData.getData());

        verify(mWifiMetrics, times(2)).noteWifiConfigStoreReadDuration(anyInt());
        verify(mWifiMetrics).noteWifiConfigStoreWriteDuration(anyInt());
    }

    /**
     * Tests the read API behaviour when the shared store file is empty and the user store
     * is not yet visible (user not yet unlocked).
     * Expected behaviour: The read should return an empty store data instance when the file not
     * found exception is raised.
     */
    @Test
    public void testReadWithNoSharedStoreFileAndUserStoreNotVisible() throws Exception {
        StoreData sharedStoreData = mock(StoreData.class);
        when(sharedStoreData.getStoreFileId())
                .thenReturn(WifiConfigStore.STORE_FILE_SHARED_GENERAL);
        StoreData userStoreData = mock(StoreData.class);
        when(userStoreData.getStoreFileId())
                .thenReturn(WifiConfigStore.STORE_FILE_USER_GENERAL);

        // Reading the mock store without a write should simulate the file not found case because
        // |readRawData| would return null.
        mWifiConfigStore.registerStoreData(sharedStoreData);
        mWifiConfigStore.registerStoreData(userStoreData);
        assertFalse(mWifiConfigStore.areStoresPresent());
        mWifiConfigStore.read();

        // Ensure that we got the call to deserialize empty shared data, but no user data.
        verify(sharedStoreData).resetData();
        verify(sharedStoreData).deserializeData(eq(null), anyInt(), anyInt(), any());
        verify(userStoreData, never()).resetData();
        verify(userStoreData, never()).deserializeData(any(), anyInt(), anyInt(), any());
    }

    /**
     * Tests the read API behaviour when there are no user/shared store files on the device.
     * Expected behaviour: The read should return an empty store data instance when the file not
     * found exception is raised.
     */
    @Test
    public void testReadWithNoStoreFiles() throws Exception {
        StoreData sharedStoreData = mock(StoreData.class);
        when(sharedStoreData.getStoreFileId())
                .thenReturn(WifiConfigStore.STORE_FILE_SHARED_GENERAL);
        StoreData userStoreData = mock(StoreData.class);
        when(userStoreData.getStoreFileId())
                .thenReturn(WifiConfigStore.STORE_FILE_USER_GENERAL);

        // Reading the mock store without a write should simulate the file not found case because
        // |readRawData| would return null.
        mWifiConfigStore.registerStoreData(sharedStoreData);
        mWifiConfigStore.registerStoreData(userStoreData);
        // Read both share and user config store.
        mWifiConfigStore.setUserStores(mUserStores);
        assertFalse(mWifiConfigStore.areStoresPresent());
        mWifiConfigStore.read();

        // Ensure that we got the call to deserialize empty shared & user data.
        verify(userStoreData).resetData();
        verify(userStoreData).deserializeData(eq(null), anyInt(), anyInt(), any());
        verify(sharedStoreData).resetData();
        verify(sharedStoreData).deserializeData(eq(null), anyInt(), anyInt(), any());
    }

    /**
     * Tests the read API behaviour after a write to the shared store file when the user
     * store file is null.
     * Expected behaviour: The read should return the same data that was last written.
     */
    @Test
    public void testReadAfterWriteWithNoUserStore() throws Exception {
        // Setup data container.
        mWifiConfigStore.registerStoreData(mSharedStoreData);
        mSharedStoreData.setData(TEST_SHARE_DATA);

        // Perform write for the share store file.
        mWifiConfigStore.write(true);
        mWifiConfigStore.read();
        // Verify data content for both user and share data.
        assertEquals(TEST_SHARE_DATA, mSharedStoreData.getData());
    }

    /**
     * Verifies that a read operation will reset the data in the data container, to avoid
     * any stale data from previous read.
     *
     * @throws Exception
     */
    @Test
    public void testReadWillResetStoreData() throws Exception {
        // Register and setup store data.
        mWifiConfigStore.registerStoreData(mSharedStoreData);
        mWifiConfigStore.registerStoreData(mUserStoreData);

        // Perform force write with empty data content to both user and share store file.
        mWifiConfigStore.switchUserStoresAndRead(mUserStores);
        mWifiConfigStore.write(true);

        // Setup data container with some value.
        mUserStoreData.setData(TEST_USER_DATA);
        mSharedStoreData.setData(TEST_SHARE_DATA);

        // Perform read of both user and share store file and verify data in the data container
        // is in sync (empty) with what is in the file.
        mWifiConfigStore.read();
        assertNull(mSharedStoreData.getData());
        assertNull(mUserStoreData.getData());
    }

    /**
     * Verify that a store file contained WiFi configuration store data (network list and
     * deleted ephemeral SSID list) using the predefined test XML data is read and parsed
     * correctly.
     *
     * @throws Exception
     */
    @Test
    public void testReadWifiConfigStoreData() throws Exception {
        // Setup network list.
        NetworkListStoreData networkList = new NetworkListUserStoreData(mContext);
        mWifiConfigStore.registerStoreData(networkList);
        WifiConfiguration openNetwork = WifiConfigurationTestUtil.createOpenNetwork();
        openNetwork.creatorName = TEST_CREATOR_NAME;
        openNetwork.setIpConfiguration(
                WifiConfigurationTestUtil.createDHCPIpConfigurationWithNoProxy());
        openNetwork.setRandomizedMacAddress(TEST_RANDOMIZED_MAC);
        List<WifiConfiguration> userConfigs = new ArrayList<>();
        userConfigs.add(openNetwork);

        // Setup deleted ephemeral SSID list.
        DeletedEphemeralSsidsStoreData deletedEphemeralSsids =
                new DeletedEphemeralSsidsStoreData(mClock);
        mWifiConfigStore.registerStoreData(deletedEphemeralSsids);
        String testSsid = "\"Test SSID\"";
        Map<String, Long> ssidMap = new HashMap<>();
        ssidMap.put(testSsid, 0L);

        // Setup user store XML bytes.
        String xmlString = String.format(TEST_DATA_XML_STRING_FORMAT,
                openNetwork.configKey().replaceAll("\"", "&quot;"),
                openNetwork.SSID.replaceAll("\"", "&quot;"),
                openNetwork.shared, openNetwork.creatorUid, openNetwork.creatorName,
                openNetwork.getRandomizedMacAddress(), testSsid.replaceAll("\"", "&quot;"));
        byte[] xmlBytes = xmlString.getBytes(StandardCharsets.UTF_8);
        mUserStore.storeRawDataToWrite(xmlBytes);

        mWifiConfigStore.switchUserStoresAndRead(mUserStores);
        WifiConfigurationTestUtil.assertConfigurationsEqualForConfigStore(
                userConfigs, networkList.getConfigurations());
        assertEquals(ssidMap, deletedEphemeralSsids.getSsidToTimeMap());
    }

    /**
     * Verify that the WiFi configuration store data containing network list and deleted
     * ephemeral SSID list are serialized correctly, matches the predefined test XML data.
     *
     * @throws Exception
     */
    @Test
    public void testWriteWifiConfigStoreData() throws Exception {
        // Setup user store.
        mWifiConfigStore.switchUserStoresAndRead(mUserStores);

        // Setup network list store data.
        NetworkListStoreData networkList = new NetworkListUserStoreData(mContext);
        mWifiConfigStore.registerStoreData(networkList);
        WifiConfiguration openNetwork = WifiConfigurationTestUtil.createOpenNetwork();
        openNetwork.creatorName = TEST_CREATOR_NAME;
        openNetwork.setIpConfiguration(
                WifiConfigurationTestUtil.createDHCPIpConfigurationWithNoProxy());
        openNetwork.setRandomizedMacAddress(TEST_RANDOMIZED_MAC);
        List<WifiConfiguration> userConfigs = new ArrayList<>();
        userConfigs.add(openNetwork);
        networkList.setConfigurations(userConfigs);

        // Setup deleted ephemeral SSID list store data.
        DeletedEphemeralSsidsStoreData deletedEphemeralSsids =
                new DeletedEphemeralSsidsStoreData(mClock);
        mWifiConfigStore.registerStoreData(deletedEphemeralSsids);
        String testSsid = "Test SSID";
        Map<String, Long> ssidMap = new HashMap<>();
        ssidMap.put(testSsid, 0L);
        deletedEphemeralSsids.setSsidToTimeMap(ssidMap);

        // Setup expected XML bytes.
        String xmlString = String.format(TEST_DATA_XML_STRING_FORMAT,
                openNetwork.configKey().replaceAll("\"", "&quot;"),
                openNetwork.SSID.replaceAll("\"", "&quot;"),
                openNetwork.shared, openNetwork.creatorUid, openNetwork.creatorName,
                openNetwork.getRandomizedMacAddress(), testSsid.replaceAll("\"", "&quot;"));

        mWifiConfigStore.write(true);
        // Verify the user store content.
        assertEquals(xmlString, new String(mUserStore.getStoreBytes()));
    }

    /**
     * Verify that a store file contained WiFi configuration store data (network list and
     * deleted ephemeral SSID list) using the predefined test XML data is read and parsed
     * correctly.
     *
     * @throws Exception
     */
    @Test
    public void testReadWifiConfigStoreDataIndicateClientsThatThereIsNoDataForThem()
            throws Exception {
        // Set both the user store & shared store files.
        mWifiConfigStore.switchUserStoresAndRead(mUserStores);

        String storeData1Name = "test1";
        String storeData2Name = "test2";
        StoreData storeData1 = mock(StoreData.class);
        StoreData storeData2 = mock(StoreData.class);

        assertTrue(mWifiConfigStore.registerStoreData(storeData1));
        assertTrue(mWifiConfigStore.registerStoreData(storeData2));

        String fileContentsXmlStringWithOnlyStoreData1 =
                String.format(TEST_DATA_XML_STRING_FORMAT_V1_WITH_ONE_DATA_SOURCE, storeData1Name);
        String fileContentsXmlStringWithOnlyStoreData2 =
                String.format(TEST_DATA_XML_STRING_FORMAT_V1_WITH_ONE_DATA_SOURCE, storeData2Name);
        String fileContentsXmlStringWithStoreData1AndStoreData2 =
                String.format(TEST_DATA_XML_STRING_FORMAT_V1_WITH_TWO_DATA_SOURCE,
                        storeData1Name, storeData2Name);

        // Scenario 1: StoreData1 in shared store file.
        when(storeData1.getName()).thenReturn(storeData1Name);
        when(storeData2.getName()).thenReturn(storeData2Name);
        when(storeData1.getStoreFileId())
                .thenReturn(WifiConfigStore.STORE_FILE_SHARED_GENERAL);
        when(storeData2.getStoreFileId())
                .thenReturn(WifiConfigStore.STORE_FILE_SHARED_GENERAL);
        mSharedStore.storeRawDataToWrite(fileContentsXmlStringWithOnlyStoreData1.getBytes());
        mUserStore.storeRawDataToWrite(null);

        mWifiConfigStore.read();
        verify(storeData1).deserializeData(notNull(), anyInt(), anyInt(), any());
        verify(storeData1, never()).deserializeData(eq(null), anyInt(), anyInt(), any());
        verify(storeData2).deserializeData(eq(null), anyInt(), anyInt(), any());
        reset(storeData1, storeData2);

        // Scenario 2: StoreData2 in user store file.
        when(storeData1.getName()).thenReturn(storeData1Name);
        when(storeData2.getName()).thenReturn(storeData2Name);
        when(storeData1.getStoreFileId())
                .thenReturn(WifiConfigStore.STORE_FILE_USER_GENERAL);
        when(storeData2.getStoreFileId())
                .thenReturn(WifiConfigStore.STORE_FILE_USER_GENERAL);
        mSharedStore.storeRawDataToWrite(null);
        mUserStore.storeRawDataToWrite(fileContentsXmlStringWithOnlyStoreData2.getBytes());

        mWifiConfigStore.read();
        verify(storeData1).deserializeData(eq(null), anyInt(), anyInt(), any());
        verify(storeData2).deserializeData(notNull(), anyInt(), anyInt(), any());
        verify(storeData2, never()).deserializeData(eq(null), anyInt(), anyInt(), any());
        reset(storeData1, storeData2);

        // Scenario 3: StoreData1 in shared store file & StoreData2 in user store file.
        when(storeData1.getName()).thenReturn(storeData1Name);
        when(storeData2.getName()).thenReturn(storeData2Name);
        when(storeData1.getStoreFileId())
                .thenReturn(WifiConfigStore.STORE_FILE_SHARED_GENERAL);
        when(storeData2.getStoreFileId())
                .thenReturn(WifiConfigStore.STORE_FILE_USER_GENERAL);
        mSharedStore.storeRawDataToWrite(fileContentsXmlStringWithOnlyStoreData1.getBytes());
        mUserStore.storeRawDataToWrite(fileContentsXmlStringWithOnlyStoreData2.getBytes());

        mWifiConfigStore.read();
        verify(storeData1).deserializeData(notNull(), anyInt(), anyInt(), any());
        verify(storeData1, never()).deserializeData(eq(null), anyInt(), anyInt(), any());
        verify(storeData2).deserializeData(notNull(), anyInt(), anyInt(), any());
        verify(storeData2, never()).deserializeData(eq(null), anyInt(), anyInt(), any());
        reset(storeData1, storeData2);

        // Scenario 4: StoreData1 & StoreData2 in shared store file.
        when(storeData1.getName()).thenReturn(storeData1Name);
        when(storeData2.getName()).thenReturn(storeData2Name);
        when(storeData1.getStoreFileId())
                .thenReturn(WifiConfigStore.STORE_FILE_SHARED_GENERAL);
        when(storeData2.getStoreFileId())
                .thenReturn(WifiConfigStore.STORE_FILE_SHARED_GENERAL);
        mSharedStore.storeRawDataToWrite(
                fileContentsXmlStringWithStoreData1AndStoreData2.getBytes());
        mUserStore.storeRawDataToWrite(null);

        mWifiConfigStore.read();
        verify(storeData1).deserializeData(notNull(), anyInt(), anyInt(), any());
        verify(storeData1, never()).deserializeData(eq(null), anyInt(), anyInt(), any());
        verify(storeData2).deserializeData(notNull(), anyInt(), anyInt(), any());
        verify(storeData2, never()).deserializeData(eq(null), anyInt(), anyInt(), any());
        reset(storeData1, storeData2);
    }

    /**
     * Tests the write API behavior when all the store data's registered for a given store file
     * has no new data to write.
     * Expected behaviour: The write should not trigger a new file write for that specific store
     * file.
     */
    @Test
    public void testWriteWithNoNewData() throws Exception {
        StoreData sharedStoreData = mock(StoreData.class);
        when(sharedStoreData.getStoreFileId())
                .thenReturn(WifiConfigStore.STORE_FILE_SHARED_GENERAL);
        when(sharedStoreData.hasNewDataToSerialize()).thenReturn(true);
        when(sharedStoreData.getName()).thenReturn("sharedStoreData");

        StoreData userStoreData = mock(StoreData.class);
        when(userStoreData.getStoreFileId())
                .thenReturn(WifiConfigStore.STORE_FILE_USER_GENERAL);
        when(userStoreData.hasNewDataToSerialize()).thenReturn(true);
        when(userStoreData.getName()).thenReturn("userStoreData");

        StoreData userStoreNetworkSuggestionsData =
                mock(StoreData.class);
        when(userStoreNetworkSuggestionsData.getStoreFileId())
                .thenReturn(WifiConfigStore.STORE_FILE_USER_NETWORK_SUGGESTIONS);
        when(userStoreNetworkSuggestionsData.hasNewDataToSerialize()).thenReturn(false);
        when(userStoreNetworkSuggestionsData.getName())
                .thenReturn("userStoreNetworkSuggestionsData");

        assertTrue(mWifiConfigStore.registerStoreData(sharedStoreData));
        assertTrue(mWifiConfigStore.registerStoreData(userStoreData));
        assertTrue(mWifiConfigStore.registerStoreData(userStoreNetworkSuggestionsData));

        // Write both share and user config store.
        mWifiConfigStore.setUserStores(mUserStores);

        // Now trigger a write.
        mWifiConfigStore.write(true);

        verify(sharedStoreData).hasNewDataToSerialize();
        verify(userStoreData).hasNewDataToSerialize();
        verify(userStoreNetworkSuggestionsData).hasNewDataToSerialize();

        // Verify that we serialized data from the first 2 data source, but not from the last one.
        verify(sharedStoreData).serializeData(any(), any());
        verify(userStoreData).serializeData(any(), any());
        verify(userStoreNetworkSuggestionsData, never()).serializeData(any(), any());
    }

    /**
     * Verify that a XmlPullParserException will be thrown when reading an user store file
     * containing unknown data.
     *
     * @throws Exception
     */
    @Test(expected = XmlPullParserException.class)
    public void testReadUserStoreContainedUnknownData() throws Exception {
        String storeFileData =
                "<?xml version='1.0' encoding='utf-8' standalone='yes' ?>\n"
                        + "<WifiConfigStoreData>\n"
                        + "<int name=\"Version\" value=\"1\" />\n"
                        + "<UnknownTag>\n"    // No StoreData registered to handle this tag.
                        + "</UnknownTag>\n"
                        + "</WifiConfigStoreData>\n";
        mUserStore.storeRawDataToWrite(storeFileData.getBytes(StandardCharsets.UTF_8));
        mWifiConfigStore.switchUserStoresAndRead(mUserStores);
    }

    /**
     * Verify that a XmlPullParserException will be thrown when reading the share store file
     * containing unknown data.
     *
     * @throws Exception
     */
    @Test(expected = XmlPullParserException.class)
    public void testReadShareStoreContainedUnknownData() throws Exception {
        String storeFileData =
                "<?xml version='1.0' encoding='utf-8' standalone='yes' ?>\n"
                        + "<WifiConfigStoreData>\n"
                        + "<int name=\"Version\" value=\"1\" />\n"
                        + "<UnknownTag>\n"    // No StoreData registered to handle this tag.
                        + "</UnknownTag>\n"
                        + "</WifiConfigStoreData>\n";
        mSharedStore.storeRawDataToWrite(storeFileData.getBytes(StandardCharsets.UTF_8));
        mWifiConfigStore.read();
    }

    /**
     * Tests the read API behaviour when the config store file is version 1.
     * Expected behaviour: The read should be successful and send the data to the corresponding
     *                     {@link StoreData} instance.
     */
    @Test
    public void testReadVersion1StoreFile() throws Exception {
        // Register data container.
        StoreData sharedStoreData = mock(StoreData.class);
        when(sharedStoreData.getStoreFileId())
                .thenReturn(WifiConfigStore.STORE_FILE_SHARED_GENERAL);
        when(sharedStoreData.getName()).thenReturn(TEST_SHARE_DATA);
        StoreData userStoreData = mock(StoreData.class);
        when(userStoreData.getStoreFileId())
                .thenReturn(WifiConfigStore.STORE_FILE_USER_GENERAL);
        when(userStoreData.getName()).thenReturn(TEST_USER_DATA);
        mWifiConfigStore.registerStoreData(sharedStoreData);
        mWifiConfigStore.registerStoreData(userStoreData);

        // Read both share and user config store.
        mWifiConfigStore.setUserStores(mUserStores);

        // Now store some content in the shared and user data files.
        mUserStore.storeRawDataToWrite(
                String.format(TEST_DATA_XML_STRING_FORMAT_V1_WITH_ONE_DATA_SOURCE,
                        TEST_USER_DATA).getBytes());
        mSharedStore.storeRawDataToWrite(
                String.format(TEST_DATA_XML_STRING_FORMAT_V1_WITH_ONE_DATA_SOURCE,
                        TEST_SHARE_DATA).getBytes());

        // Read and verify the data content in the store file (metadata stripped out) has been sent
        // to the corresponding store data when integrity check passes.
        mWifiConfigStore.read();
        verify(sharedStoreData, times(1)).deserializeData(
                any(XmlPullParser.class), anyInt(),
                eq(WifiConfigStore.INITIAL_CONFIG_STORE_DATA_VERSION), any());
        verify(userStoreData, times(1)).deserializeData(
                any(XmlPullParser.class), anyInt(),
                eq(WifiConfigStore.INITIAL_CONFIG_STORE_DATA_VERSION), any());
    }

    /**
     * Tests the read API behaviour to ensure that the integrity data is parsed from the file.
     */
    @Test
    public void testReadVersion2StoreFile() throws Exception {
        byte[] encryptedData = new byte[0];
        byte[] iv = new byte[0];
        Random random = new Random();
        random.nextBytes(encryptedData);
        random.nextBytes(iv);

        // Register data container.
        StoreData sharedStoreData = mock(StoreData.class);
        when(sharedStoreData.getStoreFileId())
                .thenReturn(WifiConfigStore.STORE_FILE_SHARED_GENERAL);
        when(sharedStoreData.getName()).thenReturn(TEST_SHARE_DATA);
        when(sharedStoreData.hasNewDataToSerialize()).thenReturn(true);
        StoreData userStoreData = mock(StoreData.class);
        when(userStoreData.getStoreFileId())
                .thenReturn(WifiConfigStore.STORE_FILE_USER_GENERAL);
        when(userStoreData.getName()).thenReturn(TEST_USER_DATA);
        when(userStoreData.hasNewDataToSerialize()).thenReturn(true);
        mWifiConfigStore.registerStoreData(sharedStoreData);
        mWifiConfigStore.registerStoreData(userStoreData);

        // Read both share and user config store.
        mWifiConfigStore.setUserStores(mUserStores);

        // Now store some content in the shared and user data files with encrypted data from above.
        mUserStore.storeRawDataToWrite(
                String.format(TEST_DATA_XML_STRING_FORMAT_V2_WITH_ONE_DATA_SOURCE,
                        HexEncoding.encodeToString(encryptedData),
                        HexEncoding.encodeToString(iv),
                        TEST_USER_DATA).getBytes());
        mSharedStore.storeRawDataToWrite(
                String.format(TEST_DATA_XML_STRING_FORMAT_V2_WITH_ONE_DATA_SOURCE,
                        HexEncoding.encodeToString(encryptedData),
                        HexEncoding.encodeToString(iv),
                        TEST_SHARE_DATA).getBytes());

        // Read and verify the data content in the store file (metadata stripped out) has been sent
        // to the corresponding store data.
        mWifiConfigStore.read();
        verify(sharedStoreData, times(1))
                .deserializeData(any(XmlPullParser.class), anyInt(),
                        eq(WifiConfigStore.INTEGRITY_CONFIG_STORE_DATA_VERSION), any());
        verify(userStoreData, times(1))
                .deserializeData(any(XmlPullParser.class), anyInt(),
                        eq(WifiConfigStore.INTEGRITY_CONFIG_STORE_DATA_VERSION), any());
    }

    /**
     * Mock Store File to redirect all file writes from WifiConfigStore to local buffers.
     * This can be used to examine the data output by WifiConfigStore.
     */
    private class MockStoreFile extends StoreFile {
        private byte[] mStoreBytes;
        private boolean mStoreWritten;

        MockStoreFile(@WifiConfigStore.StoreFileId int fileId) {
            super(new File("MockStoreFile"), fileId, mEncryptionUtil);
        }

        @Override
        public byte[] readRawData() {
            return mStoreBytes;
        }

        @Override
        public void storeRawDataToWrite(byte[] data) {
            mStoreBytes = data;
            mStoreWritten = false;
        }

        @Override
        public boolean exists() {
            return (mStoreBytes != null);
        }

        @Override
        public void writeBufferedRawData() {
            if (!ArrayUtils.isEmpty(mStoreBytes)) {
                mStoreWritten = true;
            }
        }

        public byte[] getStoreBytes() {
            return mStoreBytes;
        }

        public boolean isStoreWritten() {
            return mStoreWritten;
        }
    }

    /**
     * Mock data container for providing test data for the store file.
     */
    private class MockStoreData implements StoreData {
        private static final String XML_TAG_TEST_HEADER = "TestHeader";
        private static final String XML_TAG_TEST_DATA = "TestData";

        private @WifiConfigStore.StoreFileId int mFileId;
        private String mData;
        private boolean mHasAnyNewData = true;

        MockStoreData(@WifiConfigStore.StoreFileId int fileId) {
            mFileId = fileId;
        }

        @Override
        public void serializeData(XmlSerializer out, WifiConfigStoreEncryptionUtil encryptionUtil)
                throws XmlPullParserException, IOException {
            XmlUtil.writeNextValue(out, XML_TAG_TEST_DATA, mData);
        }

        @Override
        public void deserializeData(XmlPullParser in, int outerTagDepth, int version,
                WifiConfigStoreEncryptionUtil encryptionUtil)
                throws XmlPullParserException, IOException {
            if (in == null) {
                return;
            }
            mData = (String) XmlUtil.readNextValueWithName(in, XML_TAG_TEST_DATA);
        }

        @Override
        public void resetData() {
            mData = null;
        }

        @Override
        public boolean hasNewDataToSerialize() {
            return mHasAnyNewData;
        }

        @Override
        public String getName() {
            return XML_TAG_TEST_HEADER;
        }

        @Override
        public @WifiConfigStore.StoreFileId int getStoreFileId() {
            return mFileId;
        }

        public String getData() {
            return mData;
        }

        public void setData(String data) {
            mData = data;
        }

        public void setHasAnyNewData(boolean hasAnyNewData) {
            mHasAnyNewData = hasAnyNewData;
        }
    }
}
