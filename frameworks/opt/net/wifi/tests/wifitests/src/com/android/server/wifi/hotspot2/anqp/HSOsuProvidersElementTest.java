/*
 * Copyright (C) 2017 The Android Open Source Project
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

import android.net.wifi.WifiSsid;

import androidx.test.filters.SmallTest;

import org.junit.Test;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.net.ProtocolException;
import java.nio.BufferUnderflowException;
import java.nio.ByteBuffer;
import java.nio.charset.StandardCharsets;
import java.util.Arrays;
import java.util.List;

/**
 * Unit tests for {@link com.android.server.wifi.hotspot2.anqp.HSOsuProvidersElement}.
 */
@SmallTest
public class HSOsuProvidersElementTest {
    private static final byte[] TEST_OSU_SSID_BYTES = "Test SSID".getBytes(StandardCharsets.UTF_8);
    private static final WifiSsid TEST_OSU_SSID =
            WifiSsid.createFromByteArray(TEST_OSU_SSID_BYTES);
    private static final List<OsuProviderInfo> TEST_PROVIDER_LIST =
            Arrays.asList(OsuProviderInfoTestUtil.TEST_OSU_PROVIDER_INFO);

    private static final HSOsuProvidersElement TEST_OSU_PROVIDERS_ELEMENT =
            new HSOsuProvidersElement(TEST_OSU_SSID, TEST_PROVIDER_LIST);

    /**
     * Utility function for generating test data.
     *
     * @param osuSsidBytes The OSU SSID bytes
     * @return byte[]
     */
    private static byte[] getTestData(byte[] osuSsidBytes) {
        try {
            ByteArrayOutputStream out = new ByteArrayOutputStream();
            out.write((byte) osuSsidBytes.length);
            out.write(osuSsidBytes);
            out.write((byte) TEST_PROVIDER_LIST.size());
            out.write(OsuProviderInfoTestUtil.TEST_OSU_PROVIDER_INFO_RAW_BYTES);
            return out.toByteArray();
        } catch (IOException e) {
            return null;
        }
    }

    /**
     * Verify that BufferUnderflowException will be thrown when parsing an empty buffer.
     * @throws Exception
     */
    @Test(expected = BufferUnderflowException.class)
    public void parseEmptyBuffer() throws Exception {
        HSOsuProvidersElement.parse(ByteBuffer.allocate(0));
    }

    /**
     * Verify that BufferUnderflowException will be thrown when parsing a truncated buffer
     * (missing a byte at the end).
     *
     * @throws Exception
     */
    @Test(expected = BufferUnderflowException.class)
    public void parseTruncatedBuffer() throws Exception {
        ByteBuffer buffer = ByteBuffer.wrap(getTestData(TEST_OSU_SSID_BYTES));
        buffer.limit(buffer.remaining() - 1);
        HSOsuProvidersElement.parse(buffer);
    }

    /**
     * Verify that ProtocolException will be thrown when parsing a buffer containing an
     * invalid OSU SSID.
     *
     * @throws Exception
     */
    @Test(expected = ProtocolException.class)
    public void parseBufferWithInvalidLength() throws Exception {
        byte[] invalidSsidBytes = new byte[HSOsuProvidersElement.MAXIMUM_OSU_SSID_LENGTH + 1];
        ByteBuffer buffer = ByteBuffer.wrap(getTestData(invalidSsidBytes));
        HSOsuProvidersElement.parse(buffer);
    }

    /**
     * Verify that an expected {@link HSOsuProvidersElement} will be returned when parsing a buffer
     * containing pre-defined test data.
     *
     * @throws Exception
     */
    @Test
    public void parseBufferWithTestData() throws Exception {
        ByteBuffer buffer = ByteBuffer.wrap(getTestData(TEST_OSU_SSID_BYTES));
        assertEquals(TEST_OSU_PROVIDERS_ELEMENT, HSOsuProvidersElement.parse(buffer));
    }
}
