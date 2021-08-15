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

import android.util.Pair;

import androidx.test.filters.SmallTest;

import org.junit.Test;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.net.ProtocolException;
import java.nio.BufferUnderflowException;
import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.List;

/**
 * Unit tests for {@link com.android.server.wifi.hotspot2.anqp.RoamingConsortiumElement}.
 */
@SmallTest
public class RoamingConsortiumElementTest {
    // Default test data.  Each test data contained a pair indicating the number of bytes for the
    // OI and the value of the OI.
    private static final Pair<Integer, Long> TEST_OI1 = new Pair<Integer, Long>(1, 0x12L);
    private static final Pair<Integer, Long> TEST_OI2 = new Pair<Integer, Long>(2, 0x1234L);
    private static final Pair<Integer, Long> TEST_OI3 = new Pair<Integer, Long>(4, 0x12345678L);
    private static final Pair<Integer, Long> TEST_OI4 = new Pair<Integer, Long>(8, 0x1234567890L);

    /**
     * Helper function for appending an OI field to the given output stream.
     *
     * @param stream The output stream to write to
     * @param OI The OI to write to the output stream
     */
    private void appendOI(ByteArrayOutputStream stream, Pair<Integer, Long> oi) {
        stream.write(oi.first.byteValue());
        // Write the OI data in big-endian.
        for (int i = oi.first.intValue() - 1; i >= 0; i--) {
            stream.write((byte) ((oi.second.longValue() >> i * Byte.SIZE) & 0xFF));
        }
    }
    /**
     * Helper function for generating test data with the provided OIs.
     *
     * @param OIs The OIs to generate the data with
     * @return byte[] of data
     * @throws IOException
     */
    private byte[] getTestData(List<Pair<Integer, Long>> ois) throws IOException {
        ByteArrayOutputStream stream = new ByteArrayOutputStream();
        for (Pair<Integer, Long> oi : ois) {
            appendOI(stream, oi);
        }
        return stream.toByteArray();
    }

    /**
     * Helper function for generating test data using the predefined OIs.
     *
     * @return byte[] of data
     * @throws IOException
     */
    private byte[] getDefaultTestData() throws IOException {
        List<Pair<Integer, Long>> oiList = new ArrayList<>();
        oiList.add(TEST_OI1);
        oiList.add(TEST_OI2);
        oiList.add(TEST_OI3);
        oiList.add(TEST_OI4);
        return getTestData(oiList);
    }

    /**
     * Helper function for creating a RoamingConsortiumElement using the predefined OIs.
     *
     * @return {@link RoamingConsortiumElement}
     */
    private RoamingConsortiumElement getDefaultElement() {
        List<Long> oiList = new ArrayList<>();
        oiList.add(TEST_OI1.second);
        oiList.add(TEST_OI2.second);
        oiList.add(TEST_OI3.second);
        oiList.add(TEST_OI4.second);
        return new RoamingConsortiumElement(oiList);
    }

    /**
     * Verify that no exception will be thrown when parsing an empty buffer and the returned
     * RoamingConsortiumElement will contained an empty list of OIs.
     *
     * @throws Exception
     */
    @Test
    public void parseEmptyBuffer() throws Exception {
        RoamingConsortiumElement element = RoamingConsortiumElement.parse(ByteBuffer.allocate(0));
        assertTrue(element.getOIs().isEmpty());
    }

    /**
     * Verify that BufferUnderflowException will be thrown when parsing a truncated buffer
     * (missing a byte at the end).
     *
     * @throws Exception
     */
    @Test(expected = BufferUnderflowException.class)
    public void parseTruncatedBuffer() throws Exception {
        ByteBuffer buffer = ByteBuffer.wrap(getDefaultTestData());
        buffer.limit(buffer.remaining() - 1);
        RoamingConsortiumElement.parse(buffer);
    }

    /**
     * Verify that an expected RoamingConsortiumElement will be returned when parsing a buffer
     * containing valid data.
     *
     * @throws Exception
     */
    @Test
    public void parseBufferWithDefaultTestData() throws Exception {
        // Setup expected element.
        RoamingConsortiumElement expectedElement = getDefaultElement();

        ByteBuffer buffer = ByteBuffer.wrap(getDefaultTestData());
        assertEquals(expectedElement, RoamingConsortiumElement.parse(buffer));
    }

    /**
     * Verify that ProtocolException will be thrown when parsing a buffer contained an OI length
     * that's less than minimum allowed.
     *
     * @throws Exception
     */
    @Test(expected = ProtocolException.class)
    public void parseBufferWithOILengthLessThanMinimum() throws Exception {
        ByteBuffer buffer = ByteBuffer.allocate(1);
        buffer.put((byte) (RoamingConsortiumElement.MINIMUM_OI_LENGTH - 1));
        buffer.position(0);
        RoamingConsortiumElement.parse(buffer);
    }

    /**
     * Verify that ProtocolException will be thrown when parsing a buffer contained an OI length
     * that's more than maximum allowed.
     *
     * @throws Exception
     */
    @Test(expected = ProtocolException.class)
    public void parseBufferWithOILengthMoreThanMaximum() throws Exception {
        ByteBuffer buffer = ByteBuffer.allocate(1);
        buffer.put((byte) (RoamingConsortiumElement.MAXIMUM_OI_LENGTH + 1));
        buffer.position(0);
        RoamingConsortiumElement.parse(buffer);
    }
}
