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

import androidx.test.filters.SmallTest;

import org.junit.Test;

import java.net.ProtocolException;
import java.nio.BufferUnderflowException;
import java.nio.ByteBuffer;

/**
 * Unit tests for {@link com.android.server.wifi.hotspot2.anqp.NAIRealmData}.
 */
@SmallTest
public class NAIRealmDataTest {
    /**
     * Verify that BufferUnderflowException will be thrown when parsing from an empty buffer.
     *
     * @throws Exception
     */
    @Test(expected = BufferUnderflowException.class)
    public void parseEmptyBuffer() throws Exception {
        NAIRealmData.parse(ByteBuffer.wrap(new byte[0]));
    }

    /**
     * Verify that ProtocolException will be thrown when parsing a truncated buffer
     * (missing a byte at the end).
     *
     * @throws Exception
     */
    @Test(expected = ProtocolException.class)
    public void parseTruncatedBuffer() throws Exception {
        ByteBuffer buffer = ByteBuffer.wrap(NAIRealmDataTestUtil.TEST_REAML_WITH_UTF8_DATA_BYTES);
        buffer.limit(buffer.remaining() - 1);
        NAIRealmData.parse(buffer);
    }

    /**
     * Verify that an expected NAIRealmData will be returned when parsing a buffer contained
     * the test data with realm string encoded using UTF8.
     *
     * @throws Exception
     */
    @Test
    public void parseBufferWithUTF8EncodedNAIRealmData() throws Exception {
        ByteBuffer buffer = ByteBuffer.wrap(NAIRealmDataTestUtil.TEST_REAML_WITH_UTF8_DATA_BYTES);
        assertEquals(NAIRealmDataTestUtil.TEST_REALM_DATA, NAIRealmData.parse(buffer));
    }

    /**
     * Verify that the expected NAIRealmData will be returned when parsing a buffer contained
     * the test data with realm string encoded using non-UTF8.
     *
     * @throws Exception
     */
    @Test
    public void parseBufferWithNonUTF8EncodedNAIRealmData() throws Exception {
        ByteBuffer buffer = ByteBuffer.wrap(
                NAIRealmDataTestUtil.TEST_REAML_WITH_NON_UTF8_DATA_BYTES);
        assertEquals(NAIRealmDataTestUtil.TEST_REALM_DATA, NAIRealmData.parse(buffer));
    }
}
