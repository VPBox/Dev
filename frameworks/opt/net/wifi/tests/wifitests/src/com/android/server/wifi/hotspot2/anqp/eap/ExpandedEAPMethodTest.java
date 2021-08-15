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

package com.android.server.wifi.hotspot2.anqp.eap;

import static org.junit.Assert.assertEquals;

import androidx.test.filters.SmallTest;

import org.junit.Test;

import java.net.ProtocolException;
import java.nio.BufferUnderflowException;
import java.nio.ByteBuffer;

/**
 * Unit tests for {@link com.android.server.wifi.hotspot2.anqp.eap.ExpandedEAPMethod}.
 */
@SmallTest
public class ExpandedEAPMethodTest {
    private static final int TEST_VENDOR_ID = 0x123456;
    private static final long TEST_VENDOR_TYPE = 0x23456523;
    private static final byte[] TEST_DATA_BYTES =
            new byte[] {0x12, 0x34, 0x56, 0x23, 0x45, 0x65, 0x23};

    /**
     * Verify that BufferUnderflowException will be thrown when parsing from an empty buffer.
     *
     * @throws Exception
     */
    @Test(expected = BufferUnderflowException.class)
    public void parseEmptyBuffer() throws Exception {
        ExpandedEAPMethod.parse(
                ByteBuffer.wrap(new byte[0]), ExpandedEAPMethod.EXPECTED_LENGTH_VALUE, false);
    }

    /**
     * Verify that ProtocolException will be thrown when the data length is not the expected
     * length.
     *
     * @throws Exception
     */
    @Test(expected = ProtocolException.class)
    public void parseBufferWithInvalidLength() throws Exception {
        ExpandedEAPMethod.parse(ByteBuffer.wrap(TEST_DATA_BYTES),
                ExpandedEAPMethod.EXPECTED_LENGTH_VALUE - 1, false);
    }

    /**
     * Verify that BufferUnderflowException will be thrown when parsing a truncated buffer.
     *
     * @throws Exception
     */
    @Test(expected = BufferUnderflowException.class)
    public void parseBufferWithTruncatedBuffer() throws Exception {
        ExpandedEAPMethod.parse(ByteBuffer.wrap(TEST_DATA_BYTES, 0, TEST_DATA_BYTES.length - 1),
                ExpandedEAPMethod.EXPECTED_LENGTH_VALUE, false);
    }

    /**
     * Verify that an expected ExpandedEAPMethod is returned when parsing the buffer for a
     * non-inner EAP method.
     *
     * @throws Exception
     */
    @Test
    public void parseBufferForNonInnerEAPMethod() throws Exception {
        ExpandedEAPMethod expected = new ExpandedEAPMethod(
                AuthParam.PARAM_TYPE_EXPANDED_EAP_METHOD, TEST_VENDOR_ID, TEST_VENDOR_TYPE);
        ExpandedEAPMethod actual = ExpandedEAPMethod.parse(
                ByteBuffer.wrap(TEST_DATA_BYTES), ExpandedEAPMethod.EXPECTED_LENGTH_VALUE, false);
        assertEquals(expected, actual);
    }

    /**
     * Verify that an expected CredentialType is returned when parsing the buffer for a
     * inner EAP method.
     *
     * @throws Exception
     */
    @Test
    public void parseBufferForTunneledEAPMethod() throws Exception {
        ExpandedEAPMethod expected = new ExpandedEAPMethod(
                AuthParam.PARAM_TYPE_EXPANDED_INNER_EAP_METHOD, TEST_VENDOR_ID, TEST_VENDOR_TYPE);
        ExpandedEAPMethod actual = ExpandedEAPMethod.parse(
                ByteBuffer.wrap(TEST_DATA_BYTES), ExpandedEAPMethod.EXPECTED_LENGTH_VALUE, true);
        assertEquals(expected, actual);
    }
}
