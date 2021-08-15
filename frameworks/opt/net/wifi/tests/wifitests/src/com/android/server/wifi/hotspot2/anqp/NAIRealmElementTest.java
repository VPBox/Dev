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

package com.android.server.wifi.hotspot2.anqp;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

import androidx.test.filters.SmallTest;

import org.junit.Test;

import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.ArrayList;
import java.util.List;

/**
 * Unit tests for {@link com.android.server.wifi.hotspot2.anqp.NAIRealmElement}.
 */
@SmallTest
public class NAIRealmElementTest {
    /**
     * Helper function for returning a ByteBuffer containing raw bytes for NAI Realm Element
     * with specified number of NAI Realm Data.
     *
     * @param dataCount The number of NAI Realm Data to be added to the buffer
     * @return {@link ByteBuffer}
     */
    private static ByteBuffer getTestBufferWithNAIRealmData(int dataCount) {
        int dataLength = NAIRealmDataTestUtil.TEST_REAML_WITH_UTF8_DATA_BYTES.length * dataCount;
       // 2-bytes for the NAI Realm Data count header.
        ByteBuffer buffer = ByteBuffer.allocate(dataLength + 2).order(ByteOrder.LITTLE_ENDIAN);
        buffer.putShort((short) dataCount);
        for (int i = 0; i < dataCount; i++) {
            buffer.put(NAIRealmDataTestUtil.TEST_REAML_WITH_UTF8_DATA_BYTES);
        }
        buffer.position(0);
        return buffer;
    }

    /**
     * Verify that a NAIRealmElement with an empty NAIRealmData list will be returned when parsing
     * from an empty buffer.
     *
     * @throws Exception
     */
    @Test
    public void parseEmptyBuffer() throws Exception {
        assertTrue(NAIRealmElement.parse(
                ByteBuffer.wrap(new byte[0])).getRealmDataList().isEmpty());
    }

    /**
     * Verify that an expected NAIRealmElement will be returned when parsing a buffer containing
     * a NAI Realm Element with single NAI Realm Data.
     *
     * @throws Exception
     */
    @Test
    public void parseBufferWithSingleNAIRealmData() throws Exception {
        // Setup expected NAIRealmElement.
        List<NAIRealmData> realmDataList = new ArrayList<>();
        realmDataList.add(NAIRealmDataTestUtil.TEST_REALM_DATA);
        NAIRealmElement expected = new NAIRealmElement(realmDataList);

        assertEquals(expected, NAIRealmElement.parse(getTestBufferWithNAIRealmData(1)));
    }

    /**
     * Verify that an expected NAIRealmElement will be returned when parsing a buffer containing
     * a NAI Realm Element with multiple NAI Realm Data.
     *
     * @throws Exception
     */
    @Test
    public void parseBufferWithMultipleNAIRealmData() throws Exception {
        // Setup expected NAIRealmElement.
        List<NAIRealmData> realmDataList = new ArrayList<>();
        realmDataList.add(NAIRealmDataTestUtil.TEST_REALM_DATA);
        realmDataList.add(NAIRealmDataTestUtil.TEST_REALM_DATA);
        NAIRealmElement expected = new NAIRealmElement(realmDataList);

        assertEquals(expected, NAIRealmElement.parse(getTestBufferWithNAIRealmData(2)));
    }
}
