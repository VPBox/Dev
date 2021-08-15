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
import java.util.ArrayList;
import java.util.List;

/**
 * Unit tests for {@link com.android.server.wifi.hotspot2.anqp.ThreeGPPNetworkElement}.
 */
@SmallTest
public class ThreeGPPNetworkElementTest {
    private static final byte[][] TEST_NETWORK1_PLMN_BYTES =
            new byte[][] { new byte[] {0x21, 0x63, 0x54},
                           new byte[] {0x43, (byte) 0x85, 0x76} };
    private static final List<String> TEST_NETWORK1_PLMN_LIST = new ArrayList<>();
    static {
        TEST_NETWORK1_PLMN_LIST.add("123456");
        TEST_NETWORK1_PLMN_LIST.add("345678");
    }
    private static final CellularNetwork TEST_NETWORK1 =
            new CellularNetwork(TEST_NETWORK1_PLMN_LIST);

    private static final byte[][] TEST_NETWORK2_PLMN_BYTES =
            new byte[][] { new byte[] {(byte) 0x87, 0x29, 0x10},
                           new byte[] {0x62, (byte) 0xF5, 0x73} };
    private static final List<String> TEST_NETWORK2_PLMN_LIST = new ArrayList<>();
    static {
        TEST_NETWORK2_PLMN_LIST.add("789012");
        TEST_NETWORK2_PLMN_LIST.add("26537");
    }
    private static final CellularNetwork TEST_NETWORK2 =
            new CellularNetwork(TEST_NETWORK2_PLMN_LIST);

    /**
     * Helper function for generating test data.
     *
     * @param version The GUD version number
     * @param ieiList The array containing IEI data
     * @return byte[]
     * @throws IOException
     */
    private static byte[] getTestData(int version, byte[][] ieiList)
            throws IOException {
        ByteArrayOutputStream stream = new ByteArrayOutputStream();
        int totalIeiSize = CellularNetworkTestUtil.getDataSize(ieiList);
        stream.write((byte) version);
        stream.write((byte) totalIeiSize);
        for (byte[] iei : ieiList) {
            stream.write(iei);
        }
        return stream.toByteArray();
    }

    /**
     * Verify that BufferUnderflowException will be thrown when parsing an empty buffer.
     *
     * @throws Exception
     */
    @Test(expected = BufferUnderflowException.class)
    public void parseBufferWithEmptyBuffer() throws Exception {
        ThreeGPPNetworkElement.parse(ByteBuffer.allocate(0));
    }

    /**
     * Verify that ProtocolException will be thrown when parsing an buffer contained
     * an unsupported version number.
     *
     * @throws Exception
     */
    @Test(expected = ProtocolException.class)
    public void parseBufferWithUnsupportedVersionNumber() throws Exception {
        byte[][] testIeiList = new byte[][] {
            CellularNetworkTestUtil.formatPLMNListIEI(TEST_NETWORK1_PLMN_BYTES) };
        byte[] testData = getTestData(1, testIeiList);
        ThreeGPPNetworkElement.parse(ByteBuffer.wrap(testData));
    }

    /**
     * Verify that Protocol will be thrown when parsing a truncated buffer (missing a
     * byte at the end), which will cause a inconsistency between the length value and
     * the buffer size.
     *
     * @throws Exception
     */
    @Test(expected = ProtocolException.class)
    public void parseBufferWithIncompleteData() throws Exception {
        byte[][] testIeiList = new byte[][] {
            CellularNetworkTestUtil.formatPLMNListIEI(TEST_NETWORK1_PLMN_BYTES) };
        byte[] testData = getTestData(ThreeGPPNetworkElement.GUD_VERSION_1, testIeiList);
        ThreeGPPNetworkElement.parse(ByteBuffer.wrap(testData, 0, testData.length - 1));
    }

    /**
     * Verify that the expected ThreeGPPNetworkElement is returned when parsing a buffer contained
     * the test data.
     *
     * @throws Exception
     */
    @Test
    public void parseBufferWithTestData() throws Exception {
        byte[][] testIeiList = new byte[][] {
            CellularNetworkTestUtil.formatPLMNListIEI(TEST_NETWORK1_PLMN_BYTES),
            CellularNetworkTestUtil.formatPLMNListIEI(TEST_NETWORK2_PLMN_BYTES) };
        byte[] testData = getTestData(ThreeGPPNetworkElement.GUD_VERSION_1, testIeiList);

        // Setup the expected ThreeGPPNetworkElement.
        List<CellularNetwork> networkList = new ArrayList<>();
        networkList.add(TEST_NETWORK1);
        networkList.add(TEST_NETWORK2);
        ThreeGPPNetworkElement expected = new ThreeGPPNetworkElement(networkList);

        assertEquals(expected, ThreeGPPNetworkElement.parse(ByteBuffer.wrap(testData)));
    }
}
