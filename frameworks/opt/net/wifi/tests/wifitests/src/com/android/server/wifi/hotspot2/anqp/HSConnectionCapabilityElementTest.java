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

import java.nio.BufferUnderflowException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;
import java.util.ArrayList;
import java.util.List;

/**
 * Unit tests for {@link com.android.server.wifi.hotspot2.anqp.HSConnectionCapabilityElement}.
 */
@SmallTest
public class HSConnectionCapabilityElementTest {
    private static final ProtocolPortTuple TEST_TUPLE1 =
            new ProtocolPortTuple(1, 2, ProtocolPortTuple.PROTO_STATUS_CLOSED);
    private static final ProtocolPortTuple TEST_TUPLE2 =
            new ProtocolPortTuple(3, 4, ProtocolPortTuple.PROTO_STATUS_OPEN);

    /**
     * Helper function for writing a ProtocolPortTuple into a buffer.
     *
     * @param buffer The buffer to write to
     * @param tuple The tuple to write
     */
    private void appendProtocolPortTuple(ByteBuffer buffer, ProtocolPortTuple tuple) {
        buffer.put((byte) tuple.getProtocol());
        buffer.putShort((short) tuple.getPort());
        buffer.put((byte) tuple.getStatus());
    }

    /**
     * Helper function for generating a buffer with test data.
     *
     * @param tuples Tuples to put in the buffer
     * @return {@link ByteBuffer}
     */
    private ByteBuffer getTestBuffer(ProtocolPortTuple[] tuples) {
        ByteBuffer buffer = ByteBuffer.allocate(tuples.length * ProtocolPortTuple.RAW_BYTE_SIZE)
                .order(ByteOrder.LITTLE_ENDIAN);
        for (ProtocolPortTuple tuple : tuples) {
            appendProtocolPortTuple(buffer, tuple);
        }
        buffer.position(0);
        return buffer;
    }

    /**
     * Verify that a HSConnectionCapabilityElement with an empty status list will be returned
     * when parsing an empty buffer.
     *
     * @throws Exception
     */
    @Test
    public void parseEmptyBuffer() throws Exception {
        HSConnectionCapabilityElement element =
                HSConnectionCapabilityElement.parse(ByteBuffer.allocate(0));
        assertTrue(element.getStatusList().isEmpty());
    }

    /**
     * Verify that BufferUnderflowException will be thrown when parsing a buffer without
     * the complete tuple data (missing status field).
     *
     * @throws Exception
     */
    @Test(expected = BufferUnderflowException.class)
    public void parseBufferWithLessThanMinimumSize() throws Exception {
        ByteBuffer buffer = ByteBuffer.allocate(ProtocolPortTuple.RAW_BYTE_SIZE - 1);
        buffer.put(new byte[ProtocolPortTuple.RAW_BYTE_SIZE - 1]);
        buffer.position(0);
        HSConnectionCapabilityElement.parse(buffer);
    }

    /**
     * Verify that BufferUnderflowException will be thrown when parsing a buffer that contained
     * incomplete bytes for a tuple.
     *
     * @throws Exception
     */
    @Test(expected = BufferUnderflowException.class)
    public void parseBufferWithIncompleteTupleBytes() throws Exception {
        // Construct a buffer which will contained a tuple and an extra byte at the end.
        ByteBuffer buffer = ByteBuffer.allocate(ProtocolPortTuple.RAW_BYTE_SIZE + 1);
        appendProtocolPortTuple(buffer, TEST_TUPLE1);
        buffer.put((byte) 0);
        buffer.position(0);
        HSConnectionCapabilityElement.parse(buffer);
    }

    /**
     * Verify that the expected HSConnectionCapabilityElement is returned when parsing
     * a buffer containing the test data.
     *
     * @throws Exception
     */
    @Test
    public void parseBufferWithTestData() throws Exception {
        ByteBuffer buffer = getTestBuffer(new ProtocolPortTuple[] {TEST_TUPLE1, TEST_TUPLE2});

        // Setup expected element.
        List<ProtocolPortTuple> tupleList = new ArrayList<>();
        tupleList.add(TEST_TUPLE1);
        tupleList.add(TEST_TUPLE2);
        HSConnectionCapabilityElement expected = new HSConnectionCapabilityElement(tupleList);

        assertEquals(expected, HSConnectionCapabilityElement.parse(buffer));
    }
}
