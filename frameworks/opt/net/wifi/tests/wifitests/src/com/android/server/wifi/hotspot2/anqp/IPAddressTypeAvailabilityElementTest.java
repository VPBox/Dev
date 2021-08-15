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

/**
 * Unit tests for {@link com.android.server.wifi.hotspot2.anqp.IPAddressTypeAvailabilityElement}.
 */
@SmallTest
public class IPAddressTypeAvailabilityElementTest {
    private static final int TEST_IPV4_AVAILABILITY =
            IPAddressTypeAvailabilityElement.IPV4_PUBLIC;
    private static final int TEST_IPV6_AVAILABILITY =
            IPAddressTypeAvailabilityElement.IPV6_AVAILABLE;

    private static int getIPAvailability() {
        return (TEST_IPV4_AVAILABILITY << 2) | TEST_IPV6_AVAILABILITY;
    }

    /**
     * Verify that ProtocolException will be thrown when parsing an empty buffer.
     *
     * @throws Exception
     */
    @Test(expected = ProtocolException.class)
    public void parseBufferEmptyBuffer() throws Exception {
        IPAddressTypeAvailabilityElement.parse(ByteBuffer.allocate(0));
    }

    /**
     * Verify that ProtocolException will be thrown when parsing an buffer containing excess
     * data.
     *
     * @throws Exception
     */
    @Test(expected = ProtocolException.class)
    public void parseBufferWithExcessData() throws Exception {
        ByteBuffer buffer = ByteBuffer.allocate(
                IPAddressTypeAvailabilityElement.EXPECTED_BUFFER_LENGTH + 1);
        buffer.put((byte) getIPAvailability());
        buffer.put((byte) 0);    // Excess data.
        buffer.position(0);
        IPAddressTypeAvailabilityElement.parse(ByteBuffer.allocate(0));
    }

    /**
     * Verify that the expected IPAddressTypeAvailabilityElement is returned when parsing
     * a buffer containing the test data.
     *
     * @throws Exception
     */
    @Test
    public void parseBufferWithTestData() throws Exception {
        ByteBuffer buffer = ByteBuffer.allocate(
                IPAddressTypeAvailabilityElement.EXPECTED_BUFFER_LENGTH);
        buffer.put((byte) getIPAvailability());
        buffer.position(0);

        IPAddressTypeAvailabilityElement expectedElement = new IPAddressTypeAvailabilityElement(
                TEST_IPV4_AVAILABILITY, TEST_IPV6_AVAILABILITY);
        assertEquals(expectedElement, IPAddressTypeAvailabilityElement.parse(buffer));
    }
}
