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

import static org.junit.Assert.*;
import static org.mockito.Mockito.*;

import android.util.Xml;

import androidx.test.filters.SmallTest;

import com.android.internal.util.FastXmlSerializer;
import com.android.server.wifi.util.WifiConfigStoreEncryptionUtil;

import org.junit.Before;
import org.junit.Test;
import org.mockito.Mock;
import org.mockito.MockitoAnnotations;
import org.xmlpull.v1.XmlPullParser;
import org.xmlpull.v1.XmlSerializer;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.nio.charset.StandardCharsets;
import java.util.HashMap;
import java.util.Map;

/**
 * Unit tests for {@link com.android.server.wifi.DeletedEphemeralSsidsStoreData}.
 */
@SmallTest
public class DeletedEphemeralSsidsStoreDataTest {
    private static final String TEST_SSID1 = "SSID 1";
    private static final String TEST_SSID2 = "SSID 2";
    private static final long TEST_SSID1_TSTAMP = 6837367L;
    private static final long TEST_SSID2_TSTAMP = 4567367L;

    private static final String TEST_SSID_LIST_XML_STRING_MAP =
            "<map name=\"SSIDList\">\n"
            + "<long name=\"" + TEST_SSID1 + "\" value=\"" + TEST_SSID1_TSTAMP + "\" />\n"
            + "<long name=\"" + TEST_SSID2 + "\" value=\"" + TEST_SSID2_TSTAMP + "\" />\n"
            + "</map>\n";
    private static final String TEST_SSID_LIST_XML_STRING_SET =
            "<set name=\"SSIDList\">\n"
            + "<string>" + TEST_SSID1 + "</string>\n"
            + "<string>" + TEST_SSID2 + "</string>\n"
            + "</set>\n";
    @Mock Clock mClock;
    private DeletedEphemeralSsidsStoreData mDeletedEphemeralSsidsStoreData;

    @Before
    public void setUp() throws Exception {
        MockitoAnnotations.initMocks(this);

        mDeletedEphemeralSsidsStoreData = new DeletedEphemeralSsidsStoreData(mClock);
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
        mDeletedEphemeralSsidsStoreData.serializeData(
                out, mock(WifiConfigStoreEncryptionUtil.class));
        out.flush();
        return outputStream.toByteArray();
    }

    /**
     * Helper function for parsing configuration data from a XML block.
     *
     * @param data XML data to parse from
     * @return SSID list
     * @throws Exception
     */
    private Map<String, Long> deserializeData(byte[] data) throws Exception {
        final XmlPullParser in = Xml.newPullParser();
        final ByteArrayInputStream inputStream = new ByteArrayInputStream(data);
        in.setInput(inputStream, StandardCharsets.UTF_8.name());
        mDeletedEphemeralSsidsStoreData.deserializeData(in, in.getDepth(),
                WifiConfigStore.ENCRYPT_CREDENTIALS_CONFIG_STORE_DATA_VERSION,
                mock(WifiConfigStoreEncryptionUtil.class));
        return mDeletedEphemeralSsidsStoreData.getSsidToTimeMap();
    }

    /**
     * Verify that serializing the user store data without any configuration doesn't cause any
     * crash and no data should be serialized.
     *
     * @throws Exception
     */
    @Test
    public void serializeEmptyConfigs() throws Exception {
        assertEquals(0, serializeData().length);
    }

    /**
     * Verify that parsing an empty data doesn't cause any crash and no configuration should
     * be deserialized.
     *
     * @throws Exception
     */
    @Test
    public void deserializeEmptyData() throws Exception {
        assertTrue(deserializeData(new byte[0]).isEmpty());
    }

    /**
     * Verify that DeletedEphemeralSsidsStoreData is written to
     * {@link WifiConfigStore#STORE_FILE_NAME_USER_GENERAL}.
     *
     * @throws Exception
     */
    @Test
    public void getUserStoreFileId() throws Exception {
        assertEquals(WifiConfigStore.STORE_FILE_USER_GENERAL,
                mDeletedEphemeralSsidsStoreData.getStoreFileId());
    }

    /**
     * Verify that user store SSID list is serialized correctly, matches the predefined test
     * XML data.
     *
     * @throws Exception
     */
    @Test
    public void serializeSsidToTimeMap() throws Exception {
        Map<String, Long> ssidToTimeMap = new HashMap<>();
        ssidToTimeMap.put(TEST_SSID1, TEST_SSID1_TSTAMP);
        ssidToTimeMap.put(TEST_SSID2, TEST_SSID2_TSTAMP);
        mDeletedEphemeralSsidsStoreData.setSsidToTimeMap(ssidToTimeMap);
        byte[] actualData = serializeData();
        assertEquals(TEST_SSID_LIST_XML_STRING_MAP, new String(actualData));
    }

    /**
     * Verify that user store SSID list is deserialized correctly using the predefined test XML
     * data.
     *
     * @throws Exception
     */
    @Test
    public void deserializeSsidToTimeMap() throws Exception {
        Map<String, Long> ssidToTimeMap = new HashMap<>();
        ssidToTimeMap.put(TEST_SSID1, TEST_SSID1_TSTAMP);
        ssidToTimeMap.put(TEST_SSID2, TEST_SSID2_TSTAMP);
        assertEquals(ssidToTimeMap, deserializeData(TEST_SSID_LIST_XML_STRING_MAP.getBytes()));
    }

    /**
     * Verify that user store SSID list is deserialized correctly using the predefined test XML
     * data.
     * This simulates the config store migration from the previous set representation to the new
     * map of ssid to timestamp representation.
     *
     * @throws Exception
     */
    @Test
    public void deserializeSsidToTimeMapFromSet() throws Exception {
        long currentTimeMs = 67934838L;
        when(mClock.getWallClockMillis()).thenReturn(currentTimeMs);
        Map<String, Long> ssidToTimeMap = new HashMap<>();
        ssidToTimeMap.put(TEST_SSID1, currentTimeMs);
        ssidToTimeMap.put(TEST_SSID2, currentTimeMs);
        assertEquals(ssidToTimeMap, deserializeData(TEST_SSID_LIST_XML_STRING_SET.getBytes()));
    }
}
