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

package com.android.server.wifi.hotspot2.anqp;

import static org.junit.Assert.assertEquals;

import androidx.test.filters.SmallTest;

import org.junit.Test;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.nio.BufferUnderflowException;
import java.nio.ByteBuffer;
import java.nio.charset.StandardCharsets;

/**
 * Unit tests for {@link com.android.server.wifi.hotspot2.anqp.HSIconFileElement}.
 */
@SmallTest
public class HSIconFileElementTest {
    private static final String TEST_ICON_TYPE = "png";
    private static final byte[] TEST_ICON_DATA = new byte[8];

    /**
     * Utility function for generating test data.
     *
     * @param statusCode Status code of the icon file download
     * @return byte[]
     */
    private static byte[] getTestData(int statusCode) throws IOException {
        ByteArrayOutputStream out = new ByteArrayOutputStream();
        out.write((byte) statusCode);
        if (statusCode != HSIconFileElement.STATUS_CODE_SUCCESS) {
            // No need to write other data if status code is not success.
            return out.toByteArray();
        }

        byte[] iconTypeBytes = TEST_ICON_TYPE.getBytes(StandardCharsets.US_ASCII);
        out.write(iconTypeBytes.length);
        out.write(iconTypeBytes);
        out.write(TEST_ICON_DATA.length & 0xFF);
        out.write((TEST_ICON_DATA.length >> 8) & 0xFF);
        out.write(TEST_ICON_DATA);
        return out.toByteArray();
    }

    /**
     * Verify that BufferUnderflowException will be thrown when parsing an empty buffer.
     * @throws Exception
     */
    @Test(expected = BufferUnderflowException.class)
    public void parseEmptyBuffer() throws Exception {
        HSIconFileElement.parse(ByteBuffer.allocate(0));
    }

    /**
     * Verify that BufferUnderflowException will be thrown when parsing a truncated buffer
     * (missing a byte at the end).
     *
     * @throws Exception
     */
    @Test(expected = BufferUnderflowException.class)
    public void parseTruncatedBuffer() throws Exception {
        ByteBuffer buffer = ByteBuffer.wrap(getTestData(HSIconFileElement.STATUS_CODE_SUCCESS));
        buffer.limit(buffer.remaining() - 1);
        HSIconFileElement.parse(buffer);
    }

    /**
     * Verify that an expected {@link HSIconFileElement} is returned when parsing a buffer
     * containing icon data.
     *
     * @throws Exception
     */
    @Test
    public void parseBufferWithIconData() throws Exception {
        ByteBuffer buffer = ByteBuffer.wrap(getTestData(HSIconFileElement.STATUS_CODE_SUCCESS));
        HSIconFileElement expected = new HSIconFileElement(
                HSIconFileElement.STATUS_CODE_SUCCESS, TEST_ICON_TYPE, TEST_ICON_DATA);
        assertEquals(expected, HSIconFileElement.parse(buffer));
    }

    /**
     * Verify that an expected {@link HSIconFileElement} is returned when parsing a buffer
     * without icon data (icon file not found).
     *
     * @throws Exception
     */
    @Test
    public void parseBufferWithoutIconData() throws Exception {
        ByteBuffer buffer =
                ByteBuffer.wrap(getTestData(HSIconFileElement.STATUS_CODE_FILE_NOT_FOUND));
        HSIconFileElement expected =
                new HSIconFileElement(HSIconFileElement.STATUS_CODE_FILE_NOT_FOUND, null, null);
        assertEquals(expected, HSIconFileElement.parse(buffer));
    }
}
