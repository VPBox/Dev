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

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.net.ProtocolException;
import java.nio.BufferUnderflowException;
import java.nio.ByteBuffer;
import java.nio.charset.StandardCharsets;
import java.util.Locale;

/**
 * Unit tests for {@link com.android.server.wifi.hotspot2.anqp.I18Name}.
 */
@SmallTest
public class I18NameTest {
    private static final String TEST_LANGUAGE = "en";
    private static final Locale TEST_LOCALE = Locale.forLanguageTag(TEST_LANGUAGE);
    private static final String TEST_TEXT = "Hello World";

    /**
     * Helper function for returning byte array containing test data.
     *
     * @param language The language code string
     * @param text The text string
     * @return byte[]
     * @throws IOException
     */
    private byte[] getTestData(String language, String text) throws IOException {
        ByteArrayOutputStream stream = new ByteArrayOutputStream();
        stream.write(language.getBytes(StandardCharsets.US_ASCII));
        stream.write(new byte[]{(byte) 0x0});  // Padding for language code.
        stream.write(text.getBytes(StandardCharsets.UTF_8));
        return stream.toByteArray();
    }

    /**
     * Helper function for generating default test data. The test data include the language code
     * and text field.
     *
     * @return byte[] of data
     * @throws IOException
     */
    private byte[] getDefaultTestData() throws IOException {
        return getTestData(TEST_LANGUAGE, TEST_TEXT);
    }

    /**
     * Helper function for returning a buffer containing a I18Name test data.
     *
     * @Param data The byte array of I18Name data
     * @param length The length value to set in the I18Name header
     * @return {@link ByteBuffer}
     * @throws IOException
     */
    private ByteBuffer getTestBuffer(byte[] data, int length) throws IOException {
        // Allocate extra byte for storing the length field.
        ByteBuffer buffer = ByteBuffer.allocate(data.length + 1);
        buffer.put((byte) length);
        buffer.put(data);
        buffer.position(0);
        return buffer;
    }

    /**
     * Verify that BufferUnderflowException will be thrown when parsing from an empty buffer.
     *
     * @throws Exception
     */
    @Test(expected = BufferUnderflowException.class)
    public void parseEmptyBuffer() throws Exception {
        I18Name.parse(ByteBuffer.allocate(0));
    }

    /**
     * Verify that BufferUnderflowException will be thrown when the length field is set to more
     * than the actual buffer size.
     *
     * @throws Exception
     */
    @Test(expected = BufferUnderflowException.class)
    public void parseTruncatedBuffer() throws Exception {
        byte[] data = getDefaultTestData();
        ByteBuffer buffer = getTestBuffer(data, data.length);
        buffer.limit(buffer.remaining() - 1);
        I18Name.parse(buffer);
    }

    /**
     * Verify that ProtocolException will be thrown when the length field is set to less than
     * the minimum.
     *
     * @throws Exception
     */
    @Test(expected = ProtocolException.class)
    public void parseBufferWithLengthLessThanMinimum() throws Exception {
        byte[] data = getDefaultTestData();
        I18Name.parse(getTestBuffer(data, I18Name.MINIMUM_LENGTH - 1));
    }

    /**
     * Verify that the expected I18Name will be returned when parsing a buffer contained the
     * predefined test data.
     *
     * @throws Exception
     */
    @Test
    public void parseBufferWithDefaultTestData() throws Exception {
        byte[] data = getDefaultTestData();
        I18Name actualName = I18Name.parse(getTestBuffer(data, data.length));
        I18Name expectedName = new I18Name(TEST_LANGUAGE, TEST_LOCALE, TEST_TEXT);
        assertEquals(expectedName, actualName);
    }

    /**
     * Verify that the expected I18Name will be returned when parsing a buffer contained
     * a non-English (French) language.
     *
     * @throws Exception
     */
    @Test
    public void parseBufferWithFrenchData() throws Exception {
        // Test data for French.
        String language = "fr";
        String text = "Hello World";
        byte[] data = getTestData(language, text);
        I18Name actualName = I18Name.parse(getTestBuffer(data, data.length));
        I18Name expectedName = new I18Name(language, Locale.forLanguageTag(language), text);
        assertEquals(expectedName, actualName);
    }

    /**
     * Verify that an I18Name with an empty text will be returned when parsing a buffer contained
     * an empty text field.
     *
     * @throws Exception
     */
    @Test
    public void parseBufferWithEmptyText() throws Exception {
        byte[] data = getTestData(TEST_LANGUAGE, "");
        I18Name actualName = I18Name.parse(getTestBuffer(data, data.length));
        I18Name expectedName = new I18Name(TEST_LANGUAGE, TEST_LOCALE, "");
        assertEquals(expectedName, actualName);
    }
}
