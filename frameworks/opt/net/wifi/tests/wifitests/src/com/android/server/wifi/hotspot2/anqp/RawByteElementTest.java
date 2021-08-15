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

import java.nio.ByteBuffer;

/**
 * Unit tests for {@link com.android.server.wifi.hotspot2.anqp.RawByteElement}.
 */
@SmallTest
public class RawByteElementTest {
    private static final Constants.ANQPElementType TEST_ELEMENT_ID =
            Constants.ANQPElementType.HSOSUProviders;

    /**
     * Verify that a RawByteElement with an empty payload will be returned when parsing
     * an empty buffer.
     *
     * @throws Exception
     */
    @Test
    public void parseEmptyBuffer() throws Exception {
        byte[] data = new byte[0];
        RawByteElement actual = RawByteElement.parse(TEST_ELEMENT_ID, ByteBuffer.wrap(data));
        RawByteElement expected = new RawByteElement(TEST_ELEMENT_ID, data);
        assertEquals(expected, actual);
    }

    /**
     * Verify that the expected RawByteElement will be returned when parsing a non-empty
     * buffer.
     *
     * @throws Exception
     */
    @Test
    public void parseNonEmptyBuffer() throws Exception {
        byte[] data = new byte[10];
        RawByteElement actual = RawByteElement.parse(TEST_ELEMENT_ID, ByteBuffer.wrap(data));
        RawByteElement expected = new RawByteElement(TEST_ELEMENT_ID, data);
        assertEquals(expected, actual);
    }
}
