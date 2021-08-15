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

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.net.ProtocolException;
import java.nio.BufferUnderflowException;
import java.nio.ByteBuffer;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Locale;

/**
 * Unit tests for {@link com.android.server.wifi.hotspot2.anqp.HSFriendlyNameElement}.
 */
@SmallTest
public class HSFriendlyNameElementTest {
    private static final String TEST_LANGUAGE = "en";
    private static final Locale TEST_LOCALE = Locale.forLanguageTag(TEST_LANGUAGE);
    private static final String TEST_OPERATOR_NAME1 = "Operator1";
    private static final String TEST_OPERATOR_NAME2 = "Operator2";

    /**
     * Helper function for appending a Operator Name to an output stream.
     *
     * @param stream Stream to write to
     * @param operator The name of the operator
     * @throws IOException
     */
    private void appendOperatorName(ByteArrayOutputStream stream, String operator)
            throws IOException {
        byte[] nameBytes = operator.getBytes(StandardCharsets.UTF_8);
        int length = I18Name.LANGUAGE_CODE_LENGTH + operator.length();
        stream.write((byte) length);
        stream.write(TEST_LANGUAGE.getBytes(StandardCharsets.US_ASCII));
        stream.write(new byte[]{(byte) 0x0});  // Padding for language code.
        stream.write(nameBytes);
    }

    /**
     * Helper function for generating test data.
     *
     * @return byte[] of data
     * @throws IOException
     */
    private byte[] getTestData(String[] names) throws IOException {
        ByteArrayOutputStream stream = new ByteArrayOutputStream();
        for (String name : names) {
            appendOperatorName(stream, name);
        }
        return stream.toByteArray();
    }

    /**
     * Verify that HSFriendlyNameElement with a empty operator name list will be returned when
     * parsing an empty buffer.
     *
     * @throws Exception
     */
    @Test
    public void parseBufferWithEmptyBuffer() throws Exception {
        assertTrue(HSFriendlyNameElement.parse(ByteBuffer.allocate(0)).getNames().isEmpty());
    }

    /**
     * Verify that BufferUnderflowException will be thrown when parsing a truncated buffer
     * (missing a byte at the end).
     *
     * @throws Exception
     */
    @Test(expected = BufferUnderflowException.class)
    public void parseBufferWithTruncatedByte() throws Exception {
        byte[] testData = getTestData(new String[] {TEST_OPERATOR_NAME1});
        // Truncate a byte at the end.
        ByteBuffer buffer = ByteBuffer.allocate(testData.length - 1);
        buffer.put(testData, 0, testData.length - 1);
        buffer.position(0);
        HSFriendlyNameElement.parse(buffer);
    }

    /**
     * Verify that an expected HSFriendlyNameElement will be returned when parsing a buffer
     * containing the default test data.
     *
     * @throws Exception
     */
    @Test
    public void parseBufferWithDefaultTestData() throws Exception {
        byte[] testData = getTestData(new String[] {TEST_OPERATOR_NAME1, TEST_OPERATOR_NAME2});
        ByteBuffer buffer = ByteBuffer.allocate(testData.length);
        buffer.put(testData);
        buffer.position(0);

        // Setup expected element.
        List<I18Name> nameList = new ArrayList<>();
        nameList.add(new I18Name(TEST_LANGUAGE, TEST_LOCALE, TEST_OPERATOR_NAME1));
        nameList.add(new I18Name(TEST_LANGUAGE, TEST_LOCALE, TEST_OPERATOR_NAME2));
        HSFriendlyNameElement expectedElement = new HSFriendlyNameElement(nameList);

        assertEquals(expectedElement, HSFriendlyNameElement.parse(buffer));
    }

    /**
     * Verify that an expected HSFriendlyNameElement will be returned when parsing a buffer
     * containing a operator name with the maximum length.
     *
     * @throws Exception
     */
    @Test
    public void parseBufferWithMaxLengthOperatoreName() throws Exception {
        // Operator name with the maximum length.
        byte[] textData = new byte[HSFriendlyNameElement.MAXIMUM_OPERATOR_NAME_LENGTH];
        Arrays.fill(textData, (byte) 'a');
        String text = new String(textData);
        byte[] testData = getTestData(new String[] {text});
        ByteBuffer buffer = ByteBuffer.allocate(testData.length);
        buffer.put(testData);
        buffer.position(0);

        // Setup expected element.
        List<I18Name> nameList = new ArrayList<>();
        nameList.add(new I18Name(TEST_LANGUAGE, TEST_LOCALE, text));
        HSFriendlyNameElement expectedElement = new HSFriendlyNameElement(nameList);

        assertEquals(expectedElement, HSFriendlyNameElement.parse(buffer));
    }

    /**
     * Verify that ProtocolException will be thrown when parsing a buffer containing a
     * operator name that exceeds the maximum length.
     *
     * @throws Exception
     */
    @Test(expected = ProtocolException.class)
    public void parseBufferWithOperatorNameLengthExceedMax() throws Exception {
        byte[] textData = new byte[HSFriendlyNameElement.MAXIMUM_OPERATOR_NAME_LENGTH + 1];
        Arrays.fill(textData, (byte) 'a');
        String text = new String(textData);
        byte[] testData = getTestData(new String[] {text});
        ByteBuffer buffer = ByteBuffer.allocate(testData.length);
        buffer.put(testData);
        buffer.position(0);
        HSFriendlyNameElement.parse(buffer);
    }

}
