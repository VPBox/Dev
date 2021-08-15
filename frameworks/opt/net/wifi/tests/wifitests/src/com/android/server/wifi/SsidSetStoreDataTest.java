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

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;
import static org.mockito.Mockito.any;
import static org.mockito.Mockito.eq;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.never;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;

import android.util.Xml;

import androidx.test.filters.SmallTest;

import com.android.internal.util.FastXmlSerializer;
import com.android.server.wifi.util.WifiConfigStoreEncryptionUtil;

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
import java.util.Arrays;
import java.util.HashSet;
import java.util.Set;

/**
 * Unit tests for {@link com.android.server.wifi.SsidSetStoreData}.
 */
@SmallTest
public class SsidSetStoreDataTest {
    private static final String TEST_NOTIFIER_NAME = "TestNetwork";
    private static final String TEST_SSID1 = "SSID 1";
    private static final String TEST_SSID2 = "SSID 2";
    private static final String TEST_SSID_SET_XML_STRING =
            "<set name=\"SSIDSet\">\n"
                    + "<string>" + TEST_SSID1 + "</string>\n"
                    + "<string>" + TEST_SSID2 + "</string>\n"
                    + "</set>\n";
    private static final byte[] TEST_SSID_SET_XML_BYTES =
            TEST_SSID_SET_XML_STRING.getBytes(StandardCharsets.UTF_8);

    @Mock SsidSetStoreData.DataSource mDataSource;
    SsidSetStoreData mSsidSetStoreData;

    @Before
    public void setUp() throws Exception {
        MockitoAnnotations.initMocks(this);
        mSsidSetStoreData = new SsidSetStoreData(TEST_NOTIFIER_NAME, mDataSource);
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
        mSsidSetStoreData.serializeData(out, mock(WifiConfigStoreEncryptionUtil.class));
        out.flush();
        return outputStream.toByteArray();
    }

    /**
     * Helper function for parsing configuration data from a XML block.
     *
     * @param data XML data to parse from
     * @throws Exception
     */
    private void deserializeData(byte[] data) throws Exception {
        final XmlPullParser in = Xml.newPullParser();
        final ByteArrayInputStream inputStream = new ByteArrayInputStream(data);
        in.setInput(inputStream, StandardCharsets.UTF_8.name());
        mSsidSetStoreData.deserializeData(in, in.getDepth(),
                WifiConfigStore.ENCRYPT_CREDENTIALS_CONFIG_STORE_DATA_VERSION,
                mock(WifiConfigStoreEncryptionUtil.class));
    }

    /**
     * Verify that serializing the user store data without any configuration doesn't cause any
     * crash and no data should be serialized.
     *
     * @throws Exception
     */
    @Test
    public void serializeEmptyConfigs() throws Exception {
        when(mDataSource.getSsids()).thenReturn(new HashSet<String>());
        assertEquals(0, serializeData().length);
    }

    /**
     * Verify that parsing an empty data doesn't cause any crash and no configuration should
     * be deserialized.
     *
     * @throws Exception
     */
    @Test
    public void deserializeEmptyStoreData() throws Exception {
        deserializeData(new byte[0]);
        verify(mDataSource, never()).setSsids(any(Set.class));
    }

    /**
     * Verify that SsidSetStoreData is written to
     * {@link WifiConfigStore#STORE_FILE_NAME_USER_GENERAL}.
     *
     * @throws Exception
     */
    @Test
    public void getUserStoreFileId() throws Exception {
        assertEquals(WifiConfigStore.STORE_FILE_USER_GENERAL,
                mSsidSetStoreData.getStoreFileId());
    }

    /**
     * Verify that the store data is serialized correctly, matches the predefined test XML data.
     *
     * @throws Exception
     */
    @Test
    public void serializeSsidSet() throws Exception {
        Set<String> ssidSet = new HashSet<>();
        ssidSet.add(TEST_SSID1);
        ssidSet.add(TEST_SSID2);
        when(mDataSource.getSsids()).thenReturn(ssidSet);
        byte[] actualData = serializeData();
        assertTrue(Arrays.equals(TEST_SSID_SET_XML_BYTES, actualData));
    }

    /**
     * Verify that the store data is deserialized correctly using the predefined test XML data.
     *
     * @throws Exception
     */
    @Test
    public void deserializeSsidSet() throws Exception {
        Set<String> ssidSet = new HashSet<>();
        ssidSet.add(TEST_SSID1);
        ssidSet.add(TEST_SSID2);
        deserializeData(TEST_SSID_SET_XML_BYTES);
        verify(mDataSource).setSsids(eq(ssidSet));
    }

    /**
     * Verify that a XmlPullParserException will be thrown when parsing a SSIDSet set with an
     * unknown tag.
     *
     * @throws Exception
     */
    @Test(expected = XmlPullParserException.class)
    public void parseSetWithUnknownTag() throws Exception {
        String ssidSet =
                "<set name=\"SSIDSet\">\n"
                        + "<string>" + TEST_SSID1 + "</string>\n"
                        + "<string>" + TEST_SSID2 + "</string>\n"
                        + "<Unknown>" + "badInput" + "</Unknown>" // Unknown tag.
                        + "</set>\n";
        deserializeData(ssidSet.getBytes(StandardCharsets.UTF_8));
    }
}
