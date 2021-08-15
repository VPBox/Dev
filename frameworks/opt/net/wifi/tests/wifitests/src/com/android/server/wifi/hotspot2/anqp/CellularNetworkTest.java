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
import static org.junit.Assert.assertNull;

import androidx.test.filters.SmallTest;

import org.junit.Test;

import java.net.ProtocolException;
import java.nio.BufferUnderflowException;
import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.List;

/**
 * Unit tests for {@link com.android.server.wifi.hotspot2.anqp.CellularNetwork}.
 */
@SmallTest
public class CellularNetworkTest {
    private static final byte[] TEST_PLMN_BYTES_1 = new byte[] {0x12, 0x34, 0x56};
    private static final String TEST_PLMN_STRING_1 = "214653";
    private static final byte[] TEST_PLMN_BYTES_2 = new byte[] {0x13, (byte) 0xF9, 0x32};
    private static final String TEST_PLMN_STRING_2 = "31923";

    /**
     * Verify that BufferUnderflowException will be thrown when parsing an empty buffer.
     *
     * @throws Exception
     */
    @Test(expected = BufferUnderflowException.class)
    public void parseBufferWithEmptyBuffer() throws Exception {
        CellularNetwork.parse(ByteBuffer.allocate(0));
    }

    /**
     * Verify that a null will be returned when parsing a buffer contained an unsupported IEI type.
     *
     * @throws Exception
     */
    @Test
    public void parseBufferWithInvalidIEIType() throws Exception {
        byte[][] plmnsData = new byte[][] {TEST_PLMN_BYTES_1, TEST_PLMN_BYTES_2};
        byte[] testData = CellularNetworkTestUtil.formatPLMNListIEI(1, plmnsData);
        assertNull(CellularNetwork.parse(ByteBuffer.wrap(testData)));
    }

    /**
     * Verify that BufferUnderflowException will be thrown when parsing a truncated buffer
     * (missing a byte at the end).
     *
     * @throws Exception
     */
    @Test(expected = BufferUnderflowException.class)
    public void parseBufferWithIncompleteData() throws Exception {
        byte[][] plmnsData = new byte[][] {TEST_PLMN_BYTES_1, TEST_PLMN_BYTES_2};
        byte[] testData = CellularNetworkTestUtil.formatPLMNListIEI(plmnsData);
        CellularNetwork.parse(ByteBuffer.wrap(testData, 0, testData.length - 1));
    }

    /**
     * Verify that ProtocolException will be thrown when IEI size and the PLMN count doesn't
     * match.
     *
     * @throws Exception
     */
    @Test(expected = ProtocolException.class)
    public void parseBufferWithMismatchIEISizeAndPLMNCount() throws Exception {
        byte[][] plmnsData = new byte[][] {TEST_PLMN_BYTES_1, TEST_PLMN_BYTES_2};
        // Get test data with IEI size set to incorrect value.
        byte[] testData = CellularNetworkTestUtil.formatPLMNListIEI(
                CellularNetwork.IEI_TYPE_PLMN_LIST, plmnsData, true);
        CellularNetwork.parse(ByteBuffer.wrap(testData));
    }

    /**
     * Verify that the expected ProtocolPortTuple is returned when parsing a buffer contained
     * the test data.
     *
     * @throws Exception
     */
    @Test
    public void parseBufferWithTestData() throws Exception {
        byte[][] plmnsData = new byte[][] {TEST_PLMN_BYTES_1, TEST_PLMN_BYTES_2};
        byte[] testData = CellularNetworkTestUtil.formatPLMNListIEI(plmnsData);

        // Setup the expected CellularNetwork.
        List<String> plmnList = new ArrayList<>();
        plmnList.add(TEST_PLMN_STRING_1);
        plmnList.add(TEST_PLMN_STRING_2);
        CellularNetwork expected = new CellularNetwork(plmnList);

        assertEquals(expected, CellularNetwork.parse(ByteBuffer.wrap(testData)));
    }
}
