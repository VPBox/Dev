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
 * Unit tests for {@link com.android.server.wifi.hotspot2.anqp.VenueNameElement}.
 */
@SmallTest
public class VenueNameElementTest {
    private static final String TEST_LANGUAGE = "en";
    private static final Locale TEST_LOCALE = Locale.forLanguageTag(TEST_LANGUAGE);
    private static final String TEST_VENUE_NAME1 = "Venue1";
    private static final String TEST_VENUE_NAME2 = "Venue2";

    /**
     * Helper function for appending a Venue Name to an output stream.
     *
     * @param stream Stream to write to
     * @param venue The venue name string
     * @throws IOException
     */
    private void appendVenue(ByteArrayOutputStream stream, String venue) throws IOException {
        byte[] venueBytes = venue.getBytes(StandardCharsets.UTF_8);
        int length = I18Name.LANGUAGE_CODE_LENGTH + venue.length();
        stream.write((byte) length);
        stream.write(TEST_LANGUAGE.getBytes(StandardCharsets.US_ASCII));
        stream.write(new byte[]{(byte) 0x0});  // Padding for language code.
        stream.write(venueBytes);
    }

    /**
     * Helper function for generating test data.
     *
     * @return byte[] of data
     * @throws IOException
     */
    private byte[] getTestData(String[] names) throws IOException {
        ByteArrayOutputStream stream = new ByteArrayOutputStream();
        // Venue info data not currently used.
        stream.write(new byte[VenueNameElement.VENUE_INFO_LENGTH]);
        for (String name : names) {
            appendVenue(stream, name);
        }
        return stream.toByteArray();
    }

    /**
     * Verify that BufferUnderflowException will be thrown when parsing an empty buffer.
     *
     * @throws Exception
     */
    @Test(expected = BufferUnderflowException.class)
    public void parseEmptyBuffer() throws Exception {
        VenueNameElement.parse(ByteBuffer.allocate(0));
    }

    /**
     * Verify that BufferUnderflowException will be thrown when parsing a truncated buffer
     * (missing a byte at the end).
     *
     * @throws Exception
     */
    @Test(expected = BufferUnderflowException.class)
    public void parseTruncatedBuffer() throws Exception {
        ByteBuffer buffer = ByteBuffer.wrap(getTestData(new String[] {TEST_VENUE_NAME1}));
        // Truncate a byte at the end.
        buffer.limit(buffer.remaining() - 1);
        VenueNameElement.parse(buffer);
    }

    /**
     * Verify that a VenueNameElement with empty name list will be returned when parsing a buffer
     * contained no venue name (only contained the venue info data).
     *
     * @throws Exception
     */
    @Test
    public void parseBufferWithEmptyVenueName() throws Exception {
        ByteBuffer buffer = ByteBuffer.wrap(getTestData(new String[0]));
        assertTrue(VenueNameElement.parse(buffer).getNames().isEmpty());
    }
    /**
     * Verify that an expected VenueNameElement will be returned when parsing a buffer contained
     * valid Venue Name data.
     *
     * @throws Exception
     */
    @Test
    public void parseBufferWithValidVenueNames() throws Exception {
        // Setup expected element.
        List<I18Name> nameList = new ArrayList<>();
        nameList.add(new I18Name(TEST_LANGUAGE, TEST_LOCALE, TEST_VENUE_NAME1));
        nameList.add(new I18Name(TEST_LANGUAGE, TEST_LOCALE, TEST_VENUE_NAME2));
        VenueNameElement expectedElement = new VenueNameElement(nameList);

        ByteBuffer buffer = ByteBuffer.wrap(
                getTestData(new String[] {TEST_VENUE_NAME1, TEST_VENUE_NAME2}));
        assertEquals(expectedElement, VenueNameElement.parse(buffer));
    }

    /**
     * Verify that an expected VenueNameElement will be returned when parsing a buffer
     * contained a venue name with the maximum length.
     *
     * @throws Exception
     */
    @Test
    public void parseBufferWithMaxLengthVenueName() throws Exception {
        // Venue name with maximum length.
        byte[] textData = new byte[VenueNameElement.MAXIMUM_VENUE_NAME_LENGTH];
        Arrays.fill(textData, (byte) 'a');
        String text = new String(textData);
        ByteBuffer buffer = ByteBuffer.wrap(getTestData(new String[] {text}));

        // Setup expected element.
        List<I18Name> nameList = new ArrayList<>();
        nameList.add(new I18Name(TEST_LANGUAGE, TEST_LOCALE, text));
        VenueNameElement expectedElement = new VenueNameElement(nameList);

        assertEquals(expectedElement, VenueNameElement.parse(buffer));
    }

    /**
     * Verify that ProtocolException will be thrown when parsing a buffer contained a
     * venue name that exceeds the maximum length.
     *
     * @throws Exception
     */
    @Test(expected = ProtocolException.class)
    public void parseBufferWithVenueNameLengthExceedMax() throws Exception {
        byte[] textData = new byte[VenueNameElement.MAXIMUM_VENUE_NAME_LENGTH + 1];
        Arrays.fill(textData, (byte) 'a');
        String text = new String(textData);
        ByteBuffer buffer = ByteBuffer.wrap(getTestData(new String[] {text}));
        VenueNameElement.parse(buffer);
    }
}
