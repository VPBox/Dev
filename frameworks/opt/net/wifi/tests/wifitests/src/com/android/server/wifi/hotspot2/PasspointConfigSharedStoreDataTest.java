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

package com.android.server.wifi.hotspot2;

import static org.junit.Assert.*;
import static org.mockito.Mockito.*;

import android.util.Xml;

import androidx.test.filters.SmallTest;

import com.android.internal.util.FastXmlSerializer;
import com.android.server.wifi.WifiConfigStore;
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

/**
 * Unit tests for {@link com.android.server.wifi.hotspot2.PasspointConfigSharedStoreData}.
 */
@SmallTest
public class PasspointConfigSharedStoreDataTest {
    @Mock PasspointConfigSharedStoreData.DataSource mDataSource;
    PasspointConfigSharedStoreData mConfigStoreData;

    /** Sets up test. */
    @Before
    public void setUp() throws Exception {
        MockitoAnnotations.initMocks(this);
        mConfigStoreData = new PasspointConfigSharedStoreData(mDataSource);
    }

    /**
     * Helper function for serializing store data to a XML block.
     *
     * @return byte[]
     * @throws Exception
     */
    private byte[] serializeData() throws Exception {
        final XmlSerializer out = new FastXmlSerializer();
        final ByteArrayOutputStream outputStream = new ByteArrayOutputStream();
        out.setOutput(outputStream, StandardCharsets.UTF_8.name());
        mConfigStoreData.serializeData(out, mock(WifiConfigStoreEncryptionUtil.class));
        out.flush();
        return outputStream.toByteArray();
    }

    /**
     * Helper function for deserializing store data from a XML block.
     *
     * @param data The XML block data bytes
     * @throws Exception
     */
    private void deserializeData(byte[] data) throws Exception {
        final XmlPullParser in = Xml.newPullParser();
        final ByteArrayInputStream inputStream = new ByteArrayInputStream(data);
        in.setInput(inputStream, StandardCharsets.UTF_8.name());
        mConfigStoreData.deserializeData(in, in.getDepth(),
                WifiConfigStore.ENCRYPT_CREDENTIALS_CONFIG_STORE_DATA_VERSION,
                mock(WifiConfigStoreEncryptionUtil.class));
    }

    /**
     * Verify that the serialization and deserialization of share store data works as expected.
     * The data used for serialization matches the result of the deserialization.
     *
     * @throws Exception
     */
    @Test
    public void serializeAndDeserializeShareStoreData() throws Exception {
        // Setup expected data.
        long providerIndex = 412;

        // Serialize data for share store.
        when(mDataSource.getProviderIndex()).thenReturn(providerIndex);
        byte[] data = serializeData();

        // Deserialize data for share store and verify the content.
        deserializeData(data);
        verify(mDataSource).setProviderIndex(providerIndex);
    }

    /**
     * Verify that deserialization of an empty share store data doesn't cause any exception
     * and the corresponding data source should not be updated.
     *
     * @throws Exception
     */
    @Test
    public void deserializeEmptyShareStoreData() throws Exception {
        deserializeData(new byte[0]);
        verify(mDataSource, never()).setProviderIndex(anyLong());
    }

    /**
     * Verify that PasspointConfigUserStoreData is written to
     * {@link WifiConfigStore#STORE_FILE_NAME_SHARED_GENERAL}.
     *
     * @throws Exception
     */
    @Test
    public void getUserStoreFileId() throws Exception {
        assertEquals(WifiConfigStore.STORE_FILE_SHARED_GENERAL,
                mConfigStoreData.getStoreFileId());
    }
}
