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

import static com.android.server.wifi.WifiMetricsTestUtil.assertHistogramBucketsEqual;
import static com.android.server.wifi.WifiMetricsTestUtil.buildHistogramBucketInt32;

import static org.hamcrest.core.IsEqual.equalTo;

import androidx.test.filters.SmallTest;

import com.android.server.wifi.nano.WifiMetricsProto.HistogramBucketInt32;

import org.junit.Before;
import org.junit.Rule;
import org.junit.Test;
import org.junit.rules.ErrorCollector;
import org.mockito.MockitoAnnotations;


/**
 * Unit tests for IntHistogram.
 */
@SmallTest
public class IntHistogramTest {

    @Rule
    public ErrorCollector collector = new ErrorCollector();

    private static final int[] TEST_BUCKET_BOUNDARIES = {10, 30, 60, 100};
    private static final int[] TEST_NEGATIVE_BUCKET_BOUNDARIES =
            {-100, -20, -1, 0, 1, 5, 60, 1000};

    private IntHistogram mHistogram;

    @Before
    public void setUp() throws Exception {
        MockitoAnnotations.initMocks(this);
        mHistogram = null;
    }

    private void incrementValueAndVerify(int value, int expectedBucketIndex, int expectedCount) {
        mHistogram.increment(value);
        collector.checkThat(String.format("Unexpected bucket[%d] count!", expectedBucketIndex),
                mHistogram.getBucketByIndex(expectedBucketIndex).count, equalTo(expectedCount));
    }

    /**
     * Tests adding values to the histogram.
     */
    @Test
    public void testAddToHistogram() {
        mHistogram = new IntHistogram(TEST_BUCKET_BOUNDARIES);

        incrementValueAndVerify(-5, 0, 1);
        incrementValueAndVerify(0, 0, 2);
        incrementValueAndVerify(1, 0, 3);
        incrementValueAndVerify(9, 0, 4);
        incrementValueAndVerify(10, 1, 1);
        incrementValueAndVerify(20, 1, 2);
        incrementValueAndVerify(30, 2, 1);
        incrementValueAndVerify(40, 2, 2);
        incrementValueAndVerify(50, 2, 3);
        incrementValueAndVerify(60, 3, 1);
        incrementValueAndVerify(70, 3, 2);
        incrementValueAndVerify(80, 3, 3);
        incrementValueAndVerify(90, 3, 4);
        incrementValueAndVerify(100, 4, 1);
        incrementValueAndVerify(110, 4, 2);
        incrementValueAndVerify(98999, 4, 3);

        collector.checkThat("Unexpected number of non-empty buckets!",
                mHistogram.numNonEmptyBuckets(), equalTo(TEST_BUCKET_BOUNDARIES.length + 1));

        String expectedStr = "{[Integer.MIN_VALUE,10)=4, [10,30)=2, [30,60)=3, [60,100)=4, "
                + "[100,Integer.MAX_VALUE]=3}";
        collector.checkThat("Unexpected toString() result",
                mHistogram.toString(), equalTo(expectedStr));

        HistogramBucketInt32[] actualProto = mHistogram.toProto();
        HistogramBucketInt32[] expectedProto = {
                buildHistogramBucketInt32(Integer.MIN_VALUE, 10, 4),
                buildHistogramBucketInt32(10, 30, 2),
                buildHistogramBucketInt32(30, 60, 3),
                buildHistogramBucketInt32(60, 100, 4),
                buildHistogramBucketInt32(100, Integer.MAX_VALUE, 3)
        };
        assertHistogramBucketsEqual(expectedProto, actualProto);
    }

    /**
     * Tests when a bucket was never incremented. Expect that bucket should not exist in
     * toString() and toProto() results.
     */
    @Test
    public void testAddToHistogramWithGap() {
        mHistogram = new IntHistogram(TEST_BUCKET_BOUNDARIES);

        incrementValueAndVerify(-5, 0, 1);
        incrementValueAndVerify(0, 0, 2);
        incrementValueAndVerify(1, 0, 3);
        incrementValueAndVerify(9, 0, 4);
        incrementValueAndVerify(30, 1, 1);
        incrementValueAndVerify(40, 1, 2);
        incrementValueAndVerify(50, 1, 3);
        incrementValueAndVerify(60, 2, 1);
        incrementValueAndVerify(70, 2, 2);
        incrementValueAndVerify(80, 2, 3);
        incrementValueAndVerify(90, 2, 4);
        incrementValueAndVerify(100, 3, 1);
        incrementValueAndVerify(110, 3, 2);
        incrementValueAndVerify(98999, 3, 3);

        collector.checkThat("Unexpected number of non-empty buckets!",
                mHistogram.numNonEmptyBuckets(), equalTo(4));

        String expectedStr = "{[Integer.MIN_VALUE,10)=4, [30,60)=3, [60,100)=4, "
                + "[100,Integer.MAX_VALUE]=3}";
        collector.checkThat("Unexpected toString() result",
                mHistogram.toString(), equalTo(expectedStr));

        HistogramBucketInt32[] actualProto = mHistogram.toProto();
        HistogramBucketInt32[] expectedProto = {
                buildHistogramBucketInt32(Integer.MIN_VALUE, 10, 4),
                buildHistogramBucketInt32(30, 60, 3),
                buildHistogramBucketInt32(60, 100, 4),
                buildHistogramBucketInt32(100, Integer.MAX_VALUE, 3)
        };
        assertHistogramBucketsEqual(expectedProto, actualProto);
    }

    /**
     * Tests adding negative values to the histogram.
     */
    @Test
    public void testAddNegativeToHistogram() {
        mHistogram = new IntHistogram(TEST_NEGATIVE_BUCKET_BOUNDARIES);

        incrementValueAndVerify(-99999, 0, 1);
        incrementValueAndVerify(-101, 0, 2);
        incrementValueAndVerify(-100, 1, 1);
        incrementValueAndVerify(-99, 1, 2);
        incrementValueAndVerify(-21, 1, 3);
        incrementValueAndVerify(-20, 2, 1);
        incrementValueAndVerify(-19, 2, 2);
        incrementValueAndVerify(-2, 2, 3);
        incrementValueAndVerify(-1, 3, 1);
        incrementValueAndVerify(0, 4, 1);
        incrementValueAndVerify(0, 4, 2);
        incrementValueAndVerify(1, 5, 1);
        incrementValueAndVerify(2, 5, 2);
        incrementValueAndVerify(4, 5, 3);
        incrementValueAndVerify(5, 6, 1);
        incrementValueAndVerify(6, 6, 2);
        incrementValueAndVerify(59, 6, 3);
        incrementValueAndVerify(60, 7, 1);
        incrementValueAndVerify(61, 7, 2);
        incrementValueAndVerify(999, 7, 3);
        incrementValueAndVerify(1000, 8, 1);
        incrementValueAndVerify(1001, 8, 2);
        incrementValueAndVerify(99999, 8, 3);

        collector.checkThat("Unexpected number of non-empty buckets!",
                mHistogram.numNonEmptyBuckets(),
                equalTo(TEST_NEGATIVE_BUCKET_BOUNDARIES.length + 1));

        String expectedStr = "{[Integer.MIN_VALUE,-100)=2, [-100,-20)=3, [-20,-1)=3, [-1,0)=1, "
                + "[0,1)=2, [1,5)=3, [5,60)=3, [60,1000)=3, [1000,Integer.MAX_VALUE]=3}";
        collector.checkThat("Unexpected toString() result",
                mHistogram.toString(), equalTo(expectedStr));

        HistogramBucketInt32[] actualProto = mHistogram.toProto();
        HistogramBucketInt32[] expectedProto = {
                buildHistogramBucketInt32(Integer.MIN_VALUE, -100, 2),
                buildHistogramBucketInt32(-100, -20, 3),
                buildHistogramBucketInt32(-20,  -1, 3),
                buildHistogramBucketInt32(-1, 0, 1),
                buildHistogramBucketInt32(0, 1, 2),
                buildHistogramBucketInt32(1, 5, 3),
                buildHistogramBucketInt32(5, 60, 3),
                buildHistogramBucketInt32(60, 1000, 3),
                buildHistogramBucketInt32(1000, Integer.MAX_VALUE, 3)
        };
        assertHistogramBucketsEqual(expectedProto, actualProto);
    }

    /**
     * Tests when the histogram is empty.
     */
    @Test
    public void testEmptyHistogram() {
        mHistogram = new IntHistogram(TEST_BUCKET_BOUNDARIES);

        collector.checkThat("Unexpected number of non-empty buckets!",
                mHistogram.numNonEmptyBuckets(), equalTo(0));

        String expectedStr = "{}";
        collector.checkThat("Unexpected toString() result",
                mHistogram.toString(), equalTo(expectedStr));

        HistogramBucketInt32[] expectedProto = {};
        assertHistogramBucketsEqual(expectedProto, mHistogram.toProto());
    }

    /**
     * Tests when the histogram has length 1.
     */
    @Test
    public void testLength1Histogram() {
        mHistogram = new IntHistogram(TEST_BUCKET_BOUNDARIES);

        incrementValueAndVerify(10, 0, 1);
        incrementValueAndVerify(20, 0, 2);

        collector.checkThat("Unexpected number of non-empty buckets!",
                mHistogram.numNonEmptyBuckets(), equalTo(1));

        String expectedStr = "{[10,30)=2}";
        collector.checkThat("Unexpected toString() result",
                mHistogram.toString(), equalTo(expectedStr));

        HistogramBucketInt32[] expectedProto = {
                buildHistogramBucketInt32(10, 30, 2)
        };
        assertHistogramBucketsEqual(expectedProto, mHistogram.toProto());
    }

    /**
     * Should throw exception for null boundary array.
     */
    @Test(expected = IllegalArgumentException.class)
    public void testNullBucketBoundary() {
        new IntHistogram(null);
    }

    /**
     * Should throw exception for empty array of boundaries.
     */
    @Test(expected = IllegalArgumentException.class)
    public void testEmptyBucketBoundary() {
        new IntHistogram(new int[] {});
    }

    /**
     * Should throw exception for boundary values that are not monotonically increasing.
     */
    @Test(expected = IllegalArgumentException.class)
    public void testNonMonotonicBucketBoundaries() {
        new IntHistogram(new int[] {1, 2, 3, 3});
    }
}
