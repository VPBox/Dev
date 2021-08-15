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
package com.android.internal.telephony;

import android.telephony.TelephonyHistogram;
import android.test.suitebuilder.annotation.SmallTest;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.junit.Assert;

import static org.junit.Assert.*;

public class TelephonyHistogramTest {
    private TelephonyHistogram mHistogram = null;

    @After
    public void tearDown() throws Exception {
        mHistogram = null;
    }

    @Before
    public void setUp() throws Exception {
        mHistogram = new TelephonyHistogram(TelephonyHistogram.TELEPHONY_CATEGORY_RIL, 1, 3);
    }

    @Test
    @SmallTest
    public void testTelephonyHistogramConstructor() {
        assertEquals(TelephonyHistogram.TELEPHONY_CATEGORY_RIL, mHistogram.getCategory());
        assertEquals(1, mHistogram.getId());
        assertEquals(Integer.MAX_VALUE, mHistogram.getMinTime());
        assertEquals(0, mHistogram.getMaxTime());
        assertEquals(0, mHistogram.getAverageTime());
        assertEquals(0, mHistogram.getSampleCount());
        assertEquals(3, mHistogram.getBucketCount());

        // Test to verify that an exception is thrown when bucketCount <= 1
        mHistogram = null;
        try {
            mHistogram = new TelephonyHistogram(TelephonyHistogram.TELEPHONY_CATEGORY_RIL, 1, 1);
            Assert.fail("TelephonyHistogram should throw exception for bucketCount <= 1");
        } catch (IllegalArgumentException ex) {
            mHistogram = new TelephonyHistogram(TelephonyHistogram.TELEPHONY_CATEGORY_RIL, 1, 3);
        }
    }

    @Test
    @SmallTest
    public void testAddTimeTaken() {
        // Test when addTimeTaken is called less than 10 times
        mHistogram.addTimeTaken(10);
        mHistogram.addTimeTaken(20);
        mHistogram.addTimeTaken(30);
        mHistogram.addTimeTaken(40);
        mHistogram.addTimeTaken(50);
        assertEquals(10,mHistogram.getMinTime());
        assertEquals(50, mHistogram.getMaxTime());
        assertEquals(5, mHistogram.getSampleCount());
        assertEquals(30, mHistogram.getAverageTime());
        assertArrayEquals(new int[]{23, 36}, mHistogram.getBucketEndPoints());
        assertArrayEquals(new int[]{2, 1, 2}, mHistogram.getBucketCounters());

        // Test when addTimeTaken is called more than 10 times
        mHistogram.addTimeTaken(60);
        mHistogram.addTimeTaken(70);
        mHistogram.addTimeTaken(80);
        mHistogram.addTimeTaken(90);
        mHistogram.addTimeTaken(100);
        mHistogram.addTimeTaken(10);
        mHistogram.addTimeTaken(100);
        assertEquals(10,mHistogram.getMinTime());
        assertEquals(100, mHistogram.getMaxTime());
        assertEquals(12, mHistogram.getSampleCount());
        assertEquals(54, mHistogram.getAverageTime());
        assertArrayEquals(new int[]{40, 70}, mHistogram.getBucketEndPoints());
        assertArrayEquals(new int[]{5, 3, 4}, mHistogram.getBucketCounters());
    }
}
