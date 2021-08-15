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
import java.nio.ByteBuffer;
import java.nio.ByteOrder;

/**
 * Unit tests for {@link com.android.server.wifi.hotspot2.anqp.HSWanMetricsElement}.
 */
@SmallTest
public class HSWanMetricsElementTest {
    private static final int TEST_LINK_STATUS = HSWanMetricsElement.LINK_STATUS_UP;
    private static final boolean TEST_SYMMETRIC_LINK = true;
    private static final boolean TEST_AT_CAPACITY = true;
    private static final long TEST_DOWNLINK_SPEED = 0x1234556L;
    private static final long TEST_UPLINK_SPEED = 0x342343L;
    private static final int TEST_DOWNLINK_LOAD = 0x23;
    private static final int TEST_UPLINK_LOAD = 0x45;
    private static final int TEST_LMD = 0x2132;

    private static final HSWanMetricsElement TEST_ELEMENT = new HSWanMetricsElement(
            TEST_LINK_STATUS, TEST_SYMMETRIC_LINK, TEST_AT_CAPACITY,
            TEST_DOWNLINK_SPEED, TEST_UPLINK_SPEED, TEST_DOWNLINK_LOAD,
            TEST_UPLINK_LOAD, TEST_LMD);

    /**
     * Helper function for generating a ByteBuffer with the test data.
     *
     * @return {@link ByteBuffer}
     */
    private ByteBuffer getTestBuffer() {
        ByteBuffer buffer = ByteBuffer.allocate(HSWanMetricsElement.EXPECTED_BUFFER_SIZE)
                .order(ByteOrder.LITTLE_ENDIAN);
        int wanInfo = TEST_LINK_STATUS & HSWanMetricsElement.LINK_STATUS_MASK;
        if (TEST_SYMMETRIC_LINK) wanInfo |= HSWanMetricsElement.SYMMETRIC_LINK_MASK;
        if (TEST_AT_CAPACITY) wanInfo |= HSWanMetricsElement.AT_CAPACITY_MASK;
        buffer.put((byte) wanInfo);
        buffer.putInt((int) (TEST_DOWNLINK_SPEED & 0xFFFFFFFFL));
        buffer.putInt((int) (TEST_UPLINK_SPEED & 0xFFFFFFFFL));
        buffer.put((byte) (TEST_DOWNLINK_LOAD & 0xFF));
        buffer.put((byte) (TEST_UPLINK_LOAD & 0xFF));
        buffer.putShort((short) (TEST_LMD & 0xFFFF));
        buffer.position(0);
        return buffer;
    }

    @Test
    public void testGetStatus() {
        assertEquals(TEST_LINK_STATUS, TEST_ELEMENT.getStatus());
    }

    @Test
    public void testIsSymmetric() {
        assertEquals(TEST_SYMMETRIC_LINK, TEST_ELEMENT.isSymmetric());
    }

    @Test
    public void testIsCapped() {
        assertEquals(TEST_AT_CAPACITY, TEST_ELEMENT.isCapped());
    }

    @Test
    public void testGetDownlinkSpeed() {
        assertEquals(TEST_DOWNLINK_SPEED, TEST_ELEMENT.getDownlinkSpeed());
    }

    @Test
    public void testGetUplinkSpeed() {
        assertEquals(TEST_UPLINK_SPEED, TEST_ELEMENT.getUplinkSpeed());
    }

    @Test
    public void testGetDownlinkLoad() {
        assertEquals(TEST_DOWNLINK_LOAD, TEST_ELEMENT.getDownlinkLoad());
    }

    @Test
    public void testGetUplinkLoad() {
        assertEquals(TEST_UPLINK_LOAD, TEST_ELEMENT.getUplinkLoad());
    }

    @Test
    public void testGetLMD() {
        assertEquals(TEST_LMD, TEST_ELEMENT.getLMD());
    }

    /**
     * Verify that ProtocolException will be thrown when parsing an empty buffer.
     *
     * @throws Exception
     */
    @Test(expected = ProtocolException.class)
    public void parseEmptyBuffer() throws Exception {
        HSWanMetricsElement.parse(ByteBuffer.allocate(0));
    }

    /**
     * Verify that ProtocolException will be thrown when a buffer with size less than the
     * expected.
     *
     * @throws Exception
     */
    @Test(expected = ProtocolException.class)
    public void parseBufferWithLessThanExpectedSize() throws Exception {
        ByteBuffer buffer = ByteBuffer.allocate(HSWanMetricsElement.EXPECTED_BUFFER_SIZE - 1);
        buffer.put(new byte[HSWanMetricsElement.EXPECTED_BUFFER_SIZE - 1]);
        buffer.position(0);
        HSWanMetricsElement.parse(buffer);
    }

    /**
     * Verify that ProtocolException will be thrown when a buffer with size more than the
     * expected.
     *
     * @throws Exception
     */
    @Test(expected = ProtocolException.class)
    public void parseBufferWithMoreThanExpectedSize() throws Exception {
        ByteBuffer buffer = ByteBuffer.allocate(HSWanMetricsElement.EXPECTED_BUFFER_SIZE + 1);
        buffer.put(new byte[HSWanMetricsElement.EXPECTED_BUFFER_SIZE + 1]);
        buffer.position(0);
        HSWanMetricsElement.parse(buffer);
    }

    /**
     * Verify that the expected HSWanMetricsElement is returned when parsing
     * a buffer containing the test data.
     *
     * @throws Exception
     */
    @Test
    public void parseBufferWithTestData() throws Exception {
        ByteBuffer buffer = getTestBuffer();
        HSWanMetricsElement expectedElement = new HSWanMetricsElement(
                TEST_LINK_STATUS, TEST_SYMMETRIC_LINK, TEST_AT_CAPACITY,
                TEST_DOWNLINK_SPEED, TEST_UPLINK_SPEED, TEST_DOWNLINK_LOAD,
                TEST_UPLINK_LOAD, TEST_LMD);
        assertEquals(expectedElement, HSWanMetricsElement.parse(buffer));
    }
}
