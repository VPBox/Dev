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

package com.android.server.wifi.util;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertSame;
import static org.junit.Assert.assertTrue;

import androidx.test.filters.SmallTest;

import org.junit.Test;

/**
 * Unit tests for {@link com.android.server.wifi.util.ByteArrayRingBuffer}.
 */
@SmallTest
public class ByteArrayRingBufferTest {
    private static final int MAX_BYTES = 10;

    @Test
    public void ctorCreatesEmptyRingBuffer() {
        final ByteArrayRingBuffer rb = new ByteArrayRingBuffer(MAX_BYTES);
        assertEquals(0, rb.getNumBuffers());
    }

    @Test
    public void canAddAndRetrieveSingleElement() {
        final ByteArrayRingBuffer rb = new ByteArrayRingBuffer(MAX_BYTES);
        final byte[] data = {0};
        assertTrue(rb.appendBuffer(data));
        assertEquals(1, rb.getNumBuffers());
        assertSame(data, rb.getBuffer(0));
    }

    @Test
    public void canAddAndRetrieveMultipleElements() {
        final ByteArrayRingBuffer rb = new ByteArrayRingBuffer(MAX_BYTES);
        final byte[] data1 = {0};
        final byte[] data2 = {1};
        assertTrue(rb.appendBuffer(data1));
        assertTrue(rb.appendBuffer(data2));
        assertEquals(2, rb.getNumBuffers());
        assertSame(data1, rb.getBuffer(0));
        assertSame(data2, rb.getBuffer(1));
    }

    @Test
    public void canFillToCapacityWithMultipleElements() {
        final ByteArrayRingBuffer rb = new ByteArrayRingBuffer(MAX_BYTES);
        final byte[] data1 = {1, 2, 3, 4, 5};
        final byte[] data2 = {6, 7, 8, 9, 10};
        assertEquals(MAX_BYTES, data1.length + data2.length);
        assertTrue(rb.appendBuffer(data1));
        assertTrue(rb.appendBuffer(data2));
        assertEquals(2, rb.getNumBuffers());
    }

    @Test
    public void canFillToCapacityWithSingleElement() {
        final ByteArrayRingBuffer rb = new ByteArrayRingBuffer(MAX_BYTES);
        final byte[] data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        assertEquals(MAX_BYTES, data.length);
        assertTrue(rb.appendBuffer(data));
    }

    @Test
    public void canNotOverfillWithSingleElement() {
        final ByteArrayRingBuffer rb = new ByteArrayRingBuffer(MAX_BYTES);
        final byte[] data = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
        assertTrue(data.length > MAX_BYTES);
        assertFalse(rb.appendBuffer(data));
        assertEquals(0, rb.getNumBuffers());
    }

    @Test
    public void appendPrunesBufferIfNecessary() {
        final ByteArrayRingBuffer rb = new ByteArrayRingBuffer(MAX_BYTES);
        final byte[] data1 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        assertTrue(rb.appendBuffer(data1));

        final byte[] data2 = {11};
        assertTrue(rb.appendBuffer(data2));
        assertEquals(1, rb.getNumBuffers());
        assertSame(data2, rb.getBuffer(0));
    }

    @Test
    public void appendPrunesMultipleBuffersIfNecessary() {
        final ByteArrayRingBuffer rb = new ByteArrayRingBuffer(MAX_BYTES);
        final byte[] data1 = {1, 2, 3, 4, 5};
        final byte[] data2 = {6, 7, 8, 9, 10};
        assertTrue(rb.appendBuffer(data1));
        assertTrue(rb.appendBuffer(data2));

        final byte[] data3 = {11, 12, 13, 14, 15, 16};
        assertTrue(rb.appendBuffer(data3));
        assertEquals(1, rb.getNumBuffers());
        assertSame(data3, rb.getBuffer(0));
    }

    @Test
    public void appendPrunesOnlyWhenNecessary() {
        final ByteArrayRingBuffer rb = new ByteArrayRingBuffer(MAX_BYTES);
        final byte[] data1 = {1, 2, 3, 4, 5};
        assertTrue(rb.appendBuffer(data1));

        final byte[] data2 = {6};
        assertTrue(rb.appendBuffer(data2));
        assertEquals(2, rb.getNumBuffers());
    }

    @Test
    public void appendPrunesInFifoOrder() {
        final ByteArrayRingBuffer rb = new ByteArrayRingBuffer(MAX_BYTES);
        final byte[] data1 = {1, 2, 3, 4, 5};
        final byte[] data2 = {6, 7, 8, 9, 10};
        assertTrue(rb.appendBuffer(data1));
        assertTrue(rb.appendBuffer(data2));

        final byte[] data3 = {11};
        assertTrue(rb.appendBuffer(data3));
        assertEquals(2, rb.getNumBuffers());
        assertSame(data2, rb.getBuffer(0));
        assertSame(data3, rb.getBuffer(1));
    }

    @Test
    public void appendPrunesEvenWhenNewDataCanNotFit() {
        final ByteArrayRingBuffer rb = new ByteArrayRingBuffer(MAX_BYTES);
        final byte[] data1 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        assertTrue(rb.appendBuffer(data1));

        final byte[] data2 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
        assertFalse(rb.appendBuffer(data2));
        assertEquals(0, rb.getNumBuffers());
    }

    /** Verifies resizes()'s behavior when shrinking the buffer:
     *  1) Existing data is pruned.
     *  2) We really do decrease the size limit.
     */
    @Test
    public void resizePrunesDataAndUpdatesSizeLimitOnShrink() {
        final ByteArrayRingBuffer rb = new ByteArrayRingBuffer(MAX_BYTES);
        assertTrue(rb.appendBuffer(new byte[MAX_BYTES]));

        final byte newSize = 1;
        rb.resize(newSize);
        assertEquals(0, rb.getNumBuffers());
        assertFalse(rb.appendBuffer(new byte[newSize + 1]));
    }

    /** Verifies resize()'s behavior when growing the buffer:
     *  1) Existing data is retained.
     *  2) We really do increase the size limit.
     */
    @Test
    public void resizeRetainsExistingDataAndUpdatesSizeLimitOnGrow() {
        final ByteArrayRingBuffer rb = new ByteArrayRingBuffer(MAX_BYTES);
        assertTrue(rb.appendBuffer(new byte[MAX_BYTES]));
        rb.resize(MAX_BYTES * 2);
        assertTrue(rb.appendBuffer(new byte[MAX_BYTES]));
        assertEquals(2, rb.getNumBuffers());
    }

    /** Verifies that we don't crash when shrinking an empty buffer. */
    @Test
    public void shrinkingEmptyBufferSucceeds() {
        final ByteArrayRingBuffer rb = new ByteArrayRingBuffer(MAX_BYTES * 2);
        rb.resize(MAX_BYTES);
    }

    /** Verifies that we don't crash when growing an empty buffer. */
    @Test
    public void growingEmptyBufferSucceeds() {
        final ByteArrayRingBuffer rb = new ByteArrayRingBuffer(MAX_BYTES);
        rb.resize(MAX_BYTES * 2);
    }

}
