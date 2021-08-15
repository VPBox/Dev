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
import static org.junit.Assert.assertNotEquals;

import androidx.test.filters.SmallTest;

import org.junit.Test;

import java.nio.BufferUnderflowException;
import java.nio.ByteBuffer;

/**
 * Unit tests for {@link com.android.server.wifi.hotspot2.anqp.IconInfo}.
 */
@SmallTest
public class IconInfoTest {
    private static final int TEST_WIDTH = 1111;
    private static final int TEST_HEIGHT = 2222;
    private static final String TEST_LANGUAGE = "language";
    private static final String TEST_ICON_TYPE = "iconType";
    private static final String TEST_FILE_NAME = "filename";

    private static final IconInfo TEST_ICON_INFO =
            new IconInfo(TEST_WIDTH, TEST_HEIGHT, TEST_LANGUAGE, TEST_ICON_TYPE, TEST_FILE_NAME);

    @Test
    public void testGetWidth() {
        assertEquals(TEST_WIDTH, TEST_ICON_INFO.getWidth());
    }

    @Test
    public void testGetHeight() {
        assertEquals(TEST_HEIGHT, TEST_ICON_INFO.getHeight());
    }

    @Test
    public void testGetLanguage() {
        assertEquals(TEST_LANGUAGE, TEST_ICON_INFO.getLanguage());
    }

    @Test
    public void testGetIconType() {
        assertEquals(TEST_ICON_TYPE, TEST_ICON_INFO.getIconType());
    }

    @Test
    public void testGetFileName() {
        assertEquals(TEST_FILE_NAME, TEST_ICON_INFO.getFileName());
    }

    /**
     * Verify that BufferUnderflowException will be thrown when parsing an empty buffer.
     * @throws Exception
     */
    @Test(expected = BufferUnderflowException.class)
    public void parseEmptyBuffer() throws Exception {
        IconInfo.parse(ByteBuffer.allocate(0));
    }

    /**
     * Verify that BufferUnderflowException will be thrown when parsing a truncated buffer
     * (missing a byte at the end).
     *
     * @throws Exception
     */
    @Test(expected = BufferUnderflowException.class)
    public void parseTruncatedBuffer() throws Exception {
        ByteBuffer buffer = ByteBuffer.wrap(IconInfoTestUtil.TEST_ICON_INFO_RAW_BYTES);
        buffer.limit(buffer.remaining() - 1);
        IconInfo.parse(buffer);
    }

    /**
     * Verify that an expected {@link IconInfo} will be returned when parsing a buffer containing
     * pre-defined test data.
     *
     * @throws Exception
     */
    @Test
    public void parseBufferWithTestData() throws Exception {
        ByteBuffer buffer = ByteBuffer.wrap(IconInfoTestUtil.TEST_ICON_INFO_RAW_BYTES);
        assertEquals(IconInfoTestUtil.TEST_ICON_INFO, IconInfo.parse(buffer));
    }

    /**
     * Tests the hash function.
     */
    @Test
    public void testHashCode() {
        int width1 = 1;
        int height1 = 1;
        String language1 = "language1";
        String iconType1 = "iconType1";
        String fileName1 = "filename1";

        int width2 = 2;
        int height2 = 2;
        String language2 = "language2";
        String iconType2 = "iconType2";
        String fileName2 = "filename2";

        IconInfo info1 = new IconInfo(width1, height1, language1, iconType1, fileName1);
        IconInfo info2 = new IconInfo(width2, height2, language2, iconType2, fileName2);
        IconInfo info1Dup = new IconInfo(width1, height1, language1, iconType1, fileName1);

        assertNotEquals(info1.hashCode(), info2.hashCode());
        assertEquals(info1.hashCode(), info1Dup.hashCode());
    }
}
