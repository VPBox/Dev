/*
 * Copyright (C) 2006 The Android Open Source Project
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

package com.android.internal.telephony.uicc.euicc.asn1;

import static org.junit.Assert.assertArrayEquals;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNull;

import android.test.suitebuilder.annotation.SmallTest;

import com.android.internal.telephony.uicc.IccUtils;

import org.junit.Test;

public class IccUtilsTest {

    @SmallTest
    @Test
    public void testBytesToInt() {
        assertEquals(0, IccUtils.bytesToInt(new byte[] {}, 0, 0));
        assertEquals(0, IccUtils.bytesToInt(new byte[] {0}, 0, 0));
        assertEquals(0x1234, IccUtils.bytesToInt(new byte[] {0x12, 0x34}, 0, 2));
        assertEquals(0x34, IccUtils.bytesToInt(new byte[] {0x12, 0x34}, 1, 1));
        assertEquals(0xFF, IccUtils.bytesToInt(new byte[] {-2, 0, -1, -3, -4}, 1, 2));
        assertEquals(0x7FFFFFFF, IccUtils.bytesToInt(new byte[] {0x7F, -1, -1, -1}, 0, 4));
        assertEquals(0x80, IccUtils.bytesToInt(new byte[] {0x00, -128}, 0, 2));
    }

    @SmallTest
    @Test(expected = IllegalArgumentException.class)
    public void testBytesToInt_IllegalLength() {
        IccUtils.bytesToInt(new byte[] {1, 2, 3, 4, 4}, 0, 5);
    }

    @SmallTest
    @Test(expected = IndexOutOfBoundsException.class)
    public void testBytesToInt_IndexOutOfBounds() {
        IccUtils.bytesToInt(new byte[] {1, 2}, 0, 3);
    }

    @SmallTest
    @Test(expected = IllegalArgumentException.class)
    public void testBytesToInt_NegativeResult() {
        IccUtils.bytesToInt(new byte[] {-1, 1, 2, 3}, 0, 4);
    }

    @SmallTest
    @Test
    public void testBytesToLong() {
        assertEquals(0, IccUtils.bytesToRawLong(new byte[] {}, 0, 0));
        assertEquals(0, IccUtils.bytesToRawLong(new byte[] {0}, 0, 0));
        assertEquals(
                0x12345678,
                IccUtils.bytesToRawLong(new byte[] {0x12, 0x34, 0x56, 0x78}, 0, 4));
        assertEquals(0x34, IccUtils.bytesToRawLong(new byte[] {0x12, 0x34, 0x56, 0x78}, 1, 1));
        assertEquals(0xFF, IccUtils.bytesToRawLong(new byte[] {-2, 0, -1, -3, -4}, 1, 2));
        assertEquals(
                -1,
                IccUtils.bytesToRawLong(new byte[] {-1, -1, -1, -1, -1, -1, -1, -1}, 0, 8));
        assertEquals(0x80, IccUtils.bytesToRawLong(new byte[] {0x00, -128}, 0, 2));
    }

    @SmallTest
    @Test(expected = IllegalArgumentException.class)
    public void testBytesToLong_IllegalLength() {
        IccUtils.bytesToRawLong(new byte[] {1, 2, 3, 4, 5, 6, 7, 8, 9}, 0, 9);
    }

    @SmallTest
    @Test(expected = IndexOutOfBoundsException.class)
    public void testBytesToLong_IndexOutOfBounds() {
        IccUtils.bytesToRawLong(new byte[] {1, 2}, 0, 3);
    }

    @SmallTest
    @Test
    public void testUnsignedToBytes() {
        assertArrayEquals(new byte[] {0}, IccUtils.unsignedIntToBytes(0));
        assertArrayEquals(new byte[] {0x12, 0x34}, IccUtils.unsignedIntToBytes(0x1234));
        assertArrayEquals(
                new byte[] {0x7F, 0x12, -2, 0x34}, IccUtils.unsignedIntToBytes(0x7F12FE34));
        assertArrayEquals(new byte[] {-128}, IccUtils.unsignedIntToBytes(0x80));
        assertArrayEquals(new byte[] {-128, 0x05}, IccUtils.unsignedIntToBytes(0x8005));

        byte[] bs = new byte[] {0, 0, 0, 0, 0, 0};
        assertEquals(4, IccUtils.unsignedIntToBytes(0x7F12FE34, bs, 2));
        assertArrayEquals(new byte[] {0, 0, 0x7F, 0x12, -2, 0x34}, bs);
    }

    @SmallTest
    @Test
    public void testSignedToBytes() {
        assertArrayEquals(new byte[] {0}, IccUtils.signedIntToBytes(0));
        assertArrayEquals(new byte[] {0x12, 0x34}, IccUtils.signedIntToBytes(0x1234));
        assertArrayEquals(
                new byte[] {0x7F, 0x12, -2, 0x34}, IccUtils.signedIntToBytes(0x7F12FE34));
        assertArrayEquals(new byte[] {0, -128}, IccUtils.signedIntToBytes(0x80));
        assertArrayEquals(new byte[] {0, -128, 0x05}, IccUtils.signedIntToBytes(0x8005));

        byte[] bs = new byte[] {0, 0, 0, 0, 0, 0};
        assertEquals(4, IccUtils.signedIntToBytes(0x801234, bs, 2));
        assertArrayEquals(new byte[] {0, 0, 0, -128, 0x12, 0x34}, bs);
    }

    @SmallTest
    @Test(expected = IndexOutOfBoundsException.class)
    public void testUnsignedIntToBytes_NoEnoughSpace() {
        byte[] bs = new byte[] {0, 0, 0, 0, 0, 0};
        IccUtils.unsignedIntToBytes(0x7F12FE34, bs, 3);
    }

    @SmallTest
    @Test(expected = IndexOutOfBoundsException.class)
    public void testSignedIntToBytes_NoEnoughSpace() {
        byte[] bs = new byte[] {0, 0, 0, 0, 0, 0};
        IccUtils.signedIntToBytes(0x801234, bs, 3);
    }

    @SmallTest
    @Test(expected = IllegalArgumentException.class)
    public void testUnsignedIntToBytes_NegativeNumber() {
        IccUtils.unsignedIntToBytes(-1);
    }

    @SmallTest
    @Test(expected = IllegalArgumentException.class)
    public void testSignedIntToBytes_NegativeNumber() {
        IccUtils.signedIntToBytes(-1);
    }

    @SmallTest
    @Test
    public void testByteNumForUnsignedInt() {
        assertEquals(1, IccUtils.byteNumForUnsignedInt(0));
        assertEquals(1, IccUtils.byteNumForUnsignedInt(0x12));
        assertEquals(1, IccUtils.byteNumForUnsignedInt(0x7F));
        assertEquals(1, IccUtils.byteNumForUnsignedInt(0x80));
        assertEquals(2, IccUtils.byteNumForUnsignedInt(0x7FFF));
        assertEquals(2, IccUtils.byteNumForUnsignedInt(0x8000));
        assertEquals(3, IccUtils.byteNumForUnsignedInt(0x7FFFFF));
        assertEquals(3, IccUtils.byteNumForUnsignedInt(0x800000));
        assertEquals(4, IccUtils.byteNumForUnsignedInt(0x12345678));
        assertEquals(4, IccUtils.byteNumForUnsignedInt(0x7FFFFFFF));
    }

    @SmallTest
    @Test
    public void testByteNumForSignedInt() {
        assertEquals(1, IccUtils.byteNumForSignedInt(0));
        assertEquals(1, IccUtils.byteNumForSignedInt(0x12));
        assertEquals(1, IccUtils.byteNumForSignedInt(0x7F));
        assertEquals(2, IccUtils.byteNumForSignedInt(0x80));
        assertEquals(2, IccUtils.byteNumForSignedInt(0x7FFF));
        assertEquals(3, IccUtils.byteNumForSignedInt(0x8000));
        assertEquals(3, IccUtils.byteNumForSignedInt(0x7FFFFF));
        assertEquals(4, IccUtils.byteNumForSignedInt(0x800000));
        assertEquals(4, IccUtils.byteNumForSignedInt(0x12345678));
        assertEquals(4, IccUtils.byteNumForSignedInt(0x7FFFFFFF));
    }

    @SmallTest
    @Test(expected = IllegalArgumentException.class)
    public void testByteNumForUnsignedInt_NegativeNumber() {
        IccUtils.byteNumForUnsignedInt(0xFFFFFFFF);
    }

    @SmallTest
    @Test(expected = IllegalArgumentException.class)
    public void testByteNumForSignedInt_NegativeNumber() {
        IccUtils.byteNumForSignedInt(0xFFFFFFFF);
    }

    @SmallTest
    @Test
    public void testCountTrailingZeros() {
        assertEquals(8, IccUtils.countTrailingZeros((byte) 0));
        assertEquals(7, IccUtils.countTrailingZeros((byte) 0x80));
        assertEquals(5, IccUtils.countTrailingZeros((byte) 0xA0));
        assertEquals(0, IccUtils.countTrailingZeros((byte) 1));
    }

    @SmallTest
    @Test
    public void testBytesToHexString() {
        assertEquals("", IccUtils.bytesToHexString(new byte[] {}));
        assertEquals("00", IccUtils.bytesToHexString(new byte[] {0}));
        assertEquals(
                "FF12FE34FD56FC78",
                IccUtils.bytesToHexString(new byte[] {-1, 0x12, -2, 0x34, -3, 0x56, -4, 0x78}));
    }

    @SmallTest
    @Test
    public void testHexStringToBytes() {
        assertArrayEquals(new byte[] {}, IccUtils.hexStringToBytes(""));
        assertArrayEquals(new byte[] {0}, IccUtils.hexStringToBytes("00"));
        assertArrayEquals(
                new byte[] {-1, 0x12, -2, 0x34, -3, 0x56, -4, 0x78},
                IccUtils.hexStringToBytes("FF12FE34FD56FC78"));
    }

    @SmallTest
    @Test
    public void testBcdToString() {
        assertEquals("", IccUtils.bcdToString(new byte[] {}));
        assertEquals("00", IccUtils.bcdToString(new byte[] {0}));
        assertEquals("214365", IccUtils.bcdToString(new byte[] {0x12, 0x34, 0x56}));
    }

    @SmallTest
    @Test
    public void testBcdToBytes() {
        assertArrayEquals(new byte[] {}, IccUtils.bcdToBytes(""));
        assertArrayEquals(new byte[] {0}, IccUtils.bcdToBytes("00"));
        assertArrayEquals(new byte[] {0x21, 0x43, 0x65}, IccUtils.bcdToBytes("123456"));
        assertArrayEquals(new byte[] {0x21, 0x43, 0x05}, IccUtils.bcdToBytes("12345"));

        byte[] output;
        output = new byte[3];
        IccUtils.bcdToBytes("123456", output);
        assertArrayEquals(new byte[] {0x21, 0x43, 0x65}, output);

        output = new byte[2];
        IccUtils.bcdToBytes("123456", output);
        assertArrayEquals(new byte[] {0x21, 0x43}, output);

        output = new byte[4];
        IccUtils.bcdToBytes("12345", output);
        assertArrayEquals(new byte[] {0x21, 0x43, 0x05, 0}, output);
    }

    @SmallTest
    @Test
    public void testStripTrailingFs() {
        assertNull(IccUtils.stripTrailingFs(null));
        assertEquals("", IccUtils.stripTrailingFs(""));
        assertEquals("1234", IccUtils.stripTrailingFs("1234"));
        assertEquals("1234", IccUtils.stripTrailingFs("1234ff"));
        assertEquals("1234", IccUtils.stripTrailingFs("1234FF"));
    }
}
