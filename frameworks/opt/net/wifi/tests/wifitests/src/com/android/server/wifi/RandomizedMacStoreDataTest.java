/*
 * Copyright (C) 2019 The Android Open Source Project
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
import org.xmlpull.v1.XmlPullParser;
import org.xmlpull.v1.XmlSerializer;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.nio.charset.StandardCharsets;
import java.util.HashMap;
import java.util.Map;

/**
 * Unit tests for {@link com.android.server.wifi.RandomizedMacStoreData}.
 */
@SmallTest
public class RandomizedMacStoreDataTest {
    private static final String TEST_MAC_ADDRESS_1 = "da:a1:19:0:0:0";
    private static final String TEST_MAC_ADDRESS_2 = "ff:ff:ff:0:0:0";
    private static final String TEST_CONFIG_KEY_1 = "TP-LINK_B6C1_5GWPA_PSK";
    private static final String TEST_CONFIG_KEY_2 = "GoogleGuest-LegacyNONE";
    private RandomizedMacStoreData mRandomizedMacStoreData;

    @Before
    public void setUp() throws Exception {
        mRandomizedMacStoreData = new RandomizedMacStoreData();
    }

    /**
     * Helper function for serializing data to a XML block.
     *
     * @return byte[] of the XML data
     * @throws Exception
     */
    private byte[] serializeData() throws Exception {
        final XmlSerializer out = new FastXmlSerializer();
        final ByteArrayOutputStream outputStream = new ByteArrayOutputStream();
        out.setOutput(outputStream, StandardCharsets.UTF_8.name());
        mRandomizedMacStoreData.serializeData(out, mock(WifiConfigStoreEncryptionUtil.class));
        out.flush();
        return outputStream.toByteArray();
    }

    /**
     * Helper function for parsing data from a XML block.
     *
     * @param data XML data to parse from
     * @return Map from configKey to MAC address
     * @throws Exception
     */
    private Map<String, String> deserializeData(byte[] data) throws Exception {
        final XmlPullParser in = Xml.newPullParser();
        final ByteArrayInputStream inputStream = new ByteArrayInputStream(data);
        in.setInput(inputStream, StandardCharsets.UTF_8.name());
        mRandomizedMacStoreData.deserializeData(in, in.getDepth(),
                WifiConfigStore.ENCRYPT_CREDENTIALS_CONFIG_STORE_DATA_VERSION,
                mock(WifiConfigStoreEncryptionUtil.class));
        return mRandomizedMacStoreData.getMacMapping();
    }

    /**
     * Verify that serializing empty Map causes no crash and no data should be serialized.
     *
     * @throws Exception
     */
    @Test
    public void serializeEmptyMap() throws Exception {
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
     * Verify that RandomizedMacStoreData is written to
     * {@link WifiConfigStore#STORE_FILE_SHARED_GENERAL}.
     *
     * @throws Exception
     */
    @Test
    public void getSharedStoreFileId() throws Exception {
        assertEquals(WifiConfigStore.STORE_FILE_SHARED_GENERAL,
                mRandomizedMacStoreData.getStoreFileId());
    }

    /**
     * Verify that MAC address mapping data is serialized and deserialized correctly.
     * @throws Exception
     */
    @Test
    public void testSerializeDeserialize() throws Exception {
        Map<String, String> macMap = new HashMap<>();
        macMap.put(TEST_CONFIG_KEY_1, TEST_MAC_ADDRESS_1);
        macMap.put(TEST_CONFIG_KEY_2, TEST_MAC_ADDRESS_2);
        mRandomizedMacStoreData.setMacMapping(macMap);
        byte[] data = serializeData();
        Map<String, String> deserializedMap = deserializeData(data);
        assertEquals(macMap, deserializedMap);
    }
}
