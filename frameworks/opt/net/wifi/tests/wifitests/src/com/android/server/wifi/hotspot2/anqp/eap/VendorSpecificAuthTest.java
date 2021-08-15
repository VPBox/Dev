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

package com.android.server.wifi.hotspot2.anqp.eap;

import static org.junit.Assert.assertEquals;

import androidx.test.filters.SmallTest;

import org.junit.Test;

import java.nio.BufferUnderflowException;
import java.nio.ByteBuffer;

/**
 * Unit tests for {@link com.android.server.wifi.hotspot2.anqp.eap.VendorSpecificAuth}.
 */
@SmallTest
public class VendorSpecificAuthTest {
    private static final byte[] TEST_DATA = new byte[] {0x12, 0x34, 0x45, 0x56};

    /**
     * Verify that BufferUnderflowException will be thrown when parsing from an empty buffer.
     *
     * @throws Exception
     */
    @Test(expected = BufferUnderflowException.class)
    public void parseEmptyBuffer() throws Exception {
        VendorSpecificAuth.parse(ByteBuffer.wrap(new byte[0]), 1);
    }

    /**
     * Verify that BufferUnderflowException will be thrown when parsing from a truncated buffer.
     *
     * @throws Exception
     */
    @Test(expected = BufferUnderflowException.class)
    public void parseTruncatedBuffer() throws Exception {
        VendorSpecificAuth.parse(
                ByteBuffer.wrap(TEST_DATA, 0, TEST_DATA.length - 1), TEST_DATA.length);
    }

    /**
     * Verify that a VendorSpecificAuth with a empty data array is returned when parsing
     * a zero byte from a buffer.
     *
     * @throws Exception
     */
    @Test
    public void parseBufferWithZeroLength() throws Exception {
        VendorSpecificAuth expected = new VendorSpecificAuth(new byte[0]);
        assertEquals(expected, VendorSpecificAuth.parse(ByteBuffer.wrap(TEST_DATA), 0));
    }

    /**
     * Verify that an expected VendorSpecificAuth is returned when parsing a buffer contained
     * the expected data.
     *
     * @throws Exception
     */
    @Test
    public void parseBuffer() throws Exception {
        VendorSpecificAuth expected = new VendorSpecificAuth(TEST_DATA);
        VendorSpecificAuth actual =
                VendorSpecificAuth.parse(ByteBuffer.wrap(TEST_DATA), TEST_DATA.length);
        assertEquals(expected, actual);
    }
}
