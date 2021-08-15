/*
 * Copyright (C) 2019 The Android Open Source Project
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

import static com.android.server.wifi.WifiMetricsTestUtil.assertKeyCountsEqual;
import static com.android.server.wifi.WifiMetricsTestUtil.buildInt32Count;

import androidx.test.filters.SmallTest;

import com.android.server.wifi.nano.WifiMetricsProto.Int32Count;

import org.junit.Test;


/**
 * Unit tests for IntCounter.
 */
@SmallTest
public class IntCounterTest {

    private static final int[] TEST_KEYS = {
            100, 20, 34, 5656, 3535, 6456, -1231, -4235, 20, 3535, -5, 100, 6456, 34, -4235, -4235
    };

    /**
     * Tests when the counter is empty.
     */
    @Test
    public void testEmpty() {
        IntCounter counter = new IntCounter();
        assertKeyCountsEqual(new Int32Count[0], counter.toProto());
    }

    /**
     * Tests adding to the counter.
     */
    @Test
    public void testAddToCounter() {
        IntCounter counter = new IntCounter();

        for (int k : TEST_KEYS) {
            counter.increment(k);
        }

        Int32Count[] expected = {
                buildInt32Count(-4235, 3),
                buildInt32Count(-1231, 1),
                buildInt32Count(-5, 1),
                buildInt32Count(20, 2),
                buildInt32Count(34, 2),
                buildInt32Count(100, 2),
                buildInt32Count(3535, 2),
                buildInt32Count(5656, 1),
                buildInt32Count(6456, 2),
        };
        assertKeyCountsEqual(expected, counter.toProto());
    }

    /**
     * Tests adding to clamped counter.
     */
    @Test
    public void testAddToClampedCounter() {
        IntCounter counter = new IntCounter(-5, 100);

        for (int k : TEST_KEYS) {
            counter.increment(k);
        }

        Int32Count[] expected = {
                buildInt32Count(-5, 5),
                buildInt32Count(20, 2),
                buildInt32Count(34, 2),
                buildInt32Count(100, 7),
        };
        assertKeyCountsEqual(expected, counter.toProto());
    }
}
