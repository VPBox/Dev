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

package com.android.server.wifi.util;

import androidx.test.filters.SmallTest;

import org.junit.Assert;
import org.junit.Test;

/**
 * Unit tests for {@link com.android.server.wifi.util.BitMask}.
 */
@SmallTest
public class BitMaskTest {
    /**
     * Test that checkoff.testAndClear works as advertised
     */
    @Test
    public void testBitMask() throws Exception {

        BitMask checkoff = new BitMask(0x53);

        Assert.assertTrue(checkoff.testAndClear(0x10)); // First time, bit should be there
        Assert.assertFalse(checkoff.testAndClear(0x10)); // Second time, should be gone
        Assert.assertFalse(checkoff.testAndClear(0x100)); // This bit was not set
        Assert.assertEquals(0x43, checkoff.value); // These should be left
    }
}
