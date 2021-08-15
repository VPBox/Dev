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

import java.nio.BufferUnderflowException;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;

/**
 * Unit tests for {@link com.android.server.wifi.hotspot2.anqp.ProtocolPortTuple}.
 */
@SmallTest
public class ProtocolPortTupleTest {
    private static final int TEST_PROTOCOL = 1;
    private static final int TEST_PORT = 2;
    private static final int TEST_STATUS = ProtocolPortTuple.PROTO_STATUS_CLOSED;

    /**
     * Helper function for generating a buffer with test data.
     *
     * @param protocol Protocol value
     * @param port Port value
     * @param status Status value
     * @return {@link ByteBuffer}
     */
    private ByteBuffer getTestBuffer(int protocol, int port, int status) {
        ByteBuffer buffer = ByteBuffer.allocate(ProtocolPortTuple.RAW_BYTE_SIZE)
                .order(ByteOrder.LITTLE_ENDIAN);
        buffer.put((byte) protocol);
        buffer.putShort((short) port);
        buffer.put((byte) status);
        buffer.position(0);
        return buffer;
    }

    /**
     * Verify that BufferUnderflowException will be thrown when parsing an empty buffer.
     *
     * @throws Exception
     */
    @Test(expected = BufferUnderflowException.class)
    public void parseEmptyBuffer() throws Exception {
        ProtocolPortTuple.parse(ByteBuffer.allocate(0));
    }

    /**
     * Verify that BufferUnderflowException will be thrown when parsing a buffer without
     * the complete tuple data (missing status field).
     *
     * @throws Exception
     */
    @Test(expected = BufferUnderflowException.class)
    public void parseBufferWithIncompleteData() throws Exception {
        ByteBuffer buffer = ByteBuffer.allocate(ProtocolPortTuple.RAW_BYTE_SIZE - 1);
        buffer.put(new byte[ProtocolPortTuple.RAW_BYTE_SIZE - 1]);
        buffer.position(0);
        ProtocolPortTuple.parse(buffer);
    }

    /**
     * Verify that the expected ProtocolPortTuple is returned when parsing a buffer contained
     * the test data.
     *
     * @throws Exception
     */
    @Test
    public void parseBufferWithTestData() throws Exception {
        ByteBuffer buffer = getTestBuffer(TEST_PROTOCOL, TEST_PORT, TEST_STATUS);
        ProtocolPortTuple expected = new ProtocolPortTuple(TEST_PROTOCOL, TEST_PORT, TEST_STATUS);
        assertEquals(expected, ProtocolPortTuple.parse(buffer));
    }
}
