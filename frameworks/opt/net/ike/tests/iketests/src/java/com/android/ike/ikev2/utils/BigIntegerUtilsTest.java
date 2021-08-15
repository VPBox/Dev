/*
 * Copyright (C) 2018 The Android Open Source Project
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

package com.android.ike.ikev2.utils;

import static org.junit.Assert.assertArrayEquals;

import org.junit.Test;

import java.math.BigInteger;

public final class BigIntegerUtilsTest {
    private static final String TEST_BIGINTEGER_A = "3FFF";
    private static final String TEST_BIGINTEGER_B = "FFFF";
    private static final byte[] EXPECTED_BYTE_ARRAY_A_2 = {(byte) 0x3F, (byte) 0xFF};
    private static final byte[] EXPECTED_BYTE_ARRAY_B_2 = {(byte) 0xFF, (byte) 0xFF};
    private static final byte[] EXPECTED_BYTE_ARRAY_A_3 = {0, (byte) 0x3F, (byte) 0xFF};
    private static final byte[] EXPECTED_BYTE_ARRAY_B_3 = {0, (byte) 0xFF, (byte) 0xFF};

    @Test
    public void testBigIntegerToUnsignedByteArray() throws Exception {
        BigInteger bigIntA = new BigInteger(TEST_BIGINTEGER_A, 16);
        BigInteger bigIntB = new BigInteger(TEST_BIGINTEGER_B, 16);

        byte[] byteArrayA2 = BigIntegerUtils.bigIntegerToUnsignedByteArray(bigIntA, 2);
        assertArrayEquals(EXPECTED_BYTE_ARRAY_A_2, byteArrayA2);
        byte[] byteArrayB2 = BigIntegerUtils.bigIntegerToUnsignedByteArray(bigIntB, 2);
        assertArrayEquals(EXPECTED_BYTE_ARRAY_B_2, byteArrayB2);

        byte[] byteArrayA3 = BigIntegerUtils.bigIntegerToUnsignedByteArray(bigIntA, 3);

        assertArrayEquals(EXPECTED_BYTE_ARRAY_A_3, byteArrayA3);
        byte[] byteArrayB3 = BigIntegerUtils.bigIntegerToUnsignedByteArray(bigIntB, 3);
        assertArrayEquals(EXPECTED_BYTE_ARRAY_B_3, byteArrayB3);
    }
}
