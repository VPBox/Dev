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

package com.android.server.wifi.util;

import static org.hamcrest.core.IsEqual.equalTo;

import android.util.SparseIntArray;

import androidx.test.filters.SmallTest;

import org.junit.Before;
import org.junit.Rule;
import org.junit.Test;
import org.junit.rules.ErrorCollector;
import org.mockito.MockitoAnnotations;

/**
 * Unit test harness for MetricsUtils.
 */
@SmallTest
public class MetricsUtilsTest {
    @Rule
    public ErrorCollector collector = new ErrorCollector();

    /**
     * Pre-test configuration. Initialize and install mocks.
     */
    @Before
    public void setUp() throws Exception {
        MockitoAnnotations.initMocks(this);
    }

    /**
     * Histogram of following buckets, start[i] = 0 + 1 * 10^i with 9 sub-buckets, i=0,...,5
     * 1 - 10: 9 sub-buckets each of width 1
     * 10 - 100: 10
     * 100 - 10e3: 10^2
     * 10e3 - 10e4: 10^3
     * 10e4 - 10e5: 10^4
     * 10e5 - 10e6: 10^5
     */
    private static final MetricsUtils.LogHistParms HIST1 = new MetricsUtils.LogHistParms(0, 1, 10,
            9, 6);

    /**
     * Histogram of following buckets, start[i] = -20 + 2 * 5^i with 40 sub-buckets, i=0,...,2
     * -18 - -10: 40 sub-bucket each of width 0.2
     * -10 - 30: 1
     * 30 - 230: 5
     */
    private static final MetricsUtils.LogHistParms HIST2 = new MetricsUtils.LogHistParms(-20, 2, 5,
            40, 3);

    // Linear histogram of following buckets:
    //   <10
    //   [10, 30)
    //   [30, 60)
    //   [60, 100)
    //   >100
    private static final int[] HIST_LINEAR = {10, 30, 60, 100};

    /**
     * Validate that a set of values are bucketed correctly into the histogram, and that they are
     * converted to a primitive proto-buffer array correctly.
     */
    @Test
    public void testHistBucketing() {
        SparseIntArray hist1 = new SparseIntArray();
        SparseIntArray hist2 = new SparseIntArray();

        bucketValueAndVerify("HIST1: x=", -5, hist1, HIST1, 0, 1);
        bucketValueAndVerify("HIST1: x=", 0, hist1, HIST1, 0, 2);
        bucketValueAndVerify("HIST1: x=", 1, hist1, HIST1, 0, 3);
        bucketValueAndVerify("HIST1: x=", 9, hist1, HIST1, 8, 1);
        bucketValueAndVerify("HIST1: x=", 10, hist1, HIST1, 9, 1);
        bucketValueAndVerify("HIST1: x=", 99, hist1, HIST1, 17, 1);
        bucketValueAndVerify("HIST1: x=", 100, hist1, HIST1, 18, 1);
        bucketValueAndVerify("HIST1: x=", 989, hist1, HIST1, 26, 1);
        bucketValueAndVerify("HIST1: x=", 990, hist1, HIST1, 26, 2);
        bucketValueAndVerify("HIST1: x=", 999, hist1, HIST1, 26, 3);
        bucketValueAndVerify("HIST1: x=", 1000, hist1, HIST1, 27, 1);
        bucketValueAndVerify("HIST1: x=", 9899, hist1, HIST1, 35, 1);
        bucketValueAndVerify("HIST1: x=", 9900, hist1, HIST1, 35, 2);
        bucketValueAndVerify("HIST1: x=", 9999, hist1, HIST1, 35, 3);
        bucketValueAndVerify("HIST1: x=", 10000, hist1, HIST1, 36, 1);
        bucketValueAndVerify("HIST1: x=", 98999, hist1, HIST1, 44, 1);
        bucketValueAndVerify("HIST1: x=", 99000, hist1, HIST1, 44, 2);
        bucketValueAndVerify("HIST1: x=", 99999, hist1, HIST1, 44, 3);
        bucketValueAndVerify("HIST1: x=", 100000, hist1, HIST1, 45, 1);
        bucketValueAndVerify("HIST1: x=", 989999, hist1, HIST1, 53, 1);
        bucketValueAndVerify("HIST1: x=", 990000, hist1, HIST1, 53, 2);
        bucketValueAndVerify("HIST1: x=", 999999, hist1, HIST1, 53, 3);
        bucketValueAndVerify("HIST1: x=", 1000000, hist1, HIST1, 53, 4);
        bucketValueAndVerify("HIST1: x=", 1000001, hist1, HIST1, 53, 5);
        bucketValueAndVerify("HIST1: x=", 5000000, hist1, HIST1, 53, 6);
        bucketValueAndVerify("HIST1: x=", 10000000, hist1, HIST1, 53, 7);

        MetricsUtils.GenericBucket[] phb1 = MetricsUtils.logHistogramToGenericBuckets(hist1, HIST1);
        collector.checkThat("Number of buckets #1", phb1.length, equalTo(hist1.size()));
        validateProtoHistBucket("Bucket1[0]", phb1[0], 1, 2, 3);
        validateProtoHistBucket("Bucket1[1]", phb1[1], 9, 10, 1);
        validateProtoHistBucket("Bucket1[2]", phb1[2], 10, 20, 1);
        validateProtoHistBucket("Bucket1[3]", phb1[3], 90, 100, 1);
        validateProtoHistBucket("Bucket1[4]", phb1[4], 100, 200, 1);
        validateProtoHistBucket("Bucket1[5]", phb1[5], 900, 1000, 3);
        validateProtoHistBucket("Bucket1[6]", phb1[6], 1000, 2000, 1);
        validateProtoHistBucket("Bucket1[7]", phb1[7], 9000, 10000, 3);
        validateProtoHistBucket("Bucket1[8]", phb1[8], 10000, 20000, 1);
        validateProtoHistBucket("Bucket1[9]", phb1[9], 90000, 100000, 3);
        validateProtoHistBucket("Bucket1[10]", phb1[10], 100000, 200000, 1);
        validateProtoHistBucket("Bucket1[11]", phb1[11], 900000, 1000000, 7);

        bucketValueAndVerify("HIST2: x=", -20, hist2, HIST2, 0, 1);
        bucketValueAndVerify("HIST2: x=", -18, hist2, HIST2, 0, 2);
        bucketValueAndVerify("HIST2: x=", -17, hist2, HIST2, 5, 1);
        bucketValueAndVerify("HIST2: x=", -11, hist2, HIST2, 35, 1);
        bucketValueAndVerify("HIST2: x=", -10, hist2, HIST2, 40, 1);
        bucketValueAndVerify("HIST2: x=", 29, hist2, HIST2, 79, 1);
        bucketValueAndVerify("HIST2: x=", 30, hist2, HIST2, 80, 1);
        bucketValueAndVerify("HIST2: x=", 229, hist2, HIST2, 119, 1);
        bucketValueAndVerify("HIST2: x=", 230, hist2, HIST2, 119, 2);
        bucketValueAndVerify("HIST2: x=", 300, hist2, HIST2, 119, 3);
        bucketValueAndVerify("HIST2: x=", 1000000, hist2, HIST2, 119, 4);

        MetricsUtils.GenericBucket[] phb2 = MetricsUtils.logHistogramToGenericBuckets(hist2, HIST2);
        collector.checkThat("Number of buckets #2", phb2.length, equalTo(hist2.size()));
        validateProtoHistBucket("Bucket2[0]", phb2[0], -18, -17, 2);
        validateProtoHistBucket("Bucket2[1]", phb2[1], -17, -16, 1);
        validateProtoHistBucket("Bucket2[2]", phb2[2], -11, -10, 1);
        validateProtoHistBucket("Bucket2[3]", phb2[3], -10, -9, 1);
        validateProtoHistBucket("Bucket2[4]", phb2[4], 29, 30, 1);
        validateProtoHistBucket("Bucket2[5]", phb2[5], 30, 35, 1);
        validateProtoHistBucket("Bucket2[6]", phb2[6], 225, 230, 4);
    }

    /**
     * Validate that a set of values are bucketed correctly into the linear histogram, and that
     * they are converted to a primitive proto-buffer array correctly.
     */
    @Test
    public void testLinearHistBucketing() {
        SparseIntArray hist = new SparseIntArray();

        bucketValueAndVerify("HIST_LINEAR: x=", -5, hist, HIST_LINEAR, 0, 1);
        bucketValueAndVerify("HIST_LINEAR: x=", 0, hist, HIST_LINEAR, 0, 2);
        bucketValueAndVerify("HIST_LINEAR: x=", 1, hist, HIST_LINEAR, 0, 3);
        bucketValueAndVerify("HIST_LINEAR: x=", 9, hist, HIST_LINEAR, 0, 4);
        bucketValueAndVerify("HIST_LINEAR: x=", 10, hist, HIST_LINEAR, 1, 1);
        bucketValueAndVerify("HIST_LINEAR: x=", 20, hist, HIST_LINEAR, 1, 2);
        bucketValueAndVerify("HIST_LINEAR: x=", 30, hist, HIST_LINEAR, 2, 1);
        bucketValueAndVerify("HIST_LINEAR: x=", 40, hist, HIST_LINEAR, 2, 2);
        bucketValueAndVerify("HIST_LINEAR: x=", 50, hist, HIST_LINEAR, 2, 3);
        bucketValueAndVerify("HIST_LINEAR: x=", 60, hist, HIST_LINEAR, 3, 1);
        bucketValueAndVerify("HIST_LINEAR: x=", 70, hist, HIST_LINEAR, 3, 2);
        bucketValueAndVerify("HIST_LINEAR: x=", 80, hist, HIST_LINEAR, 3, 3);
        bucketValueAndVerify("HIST_LINEAR: x=", 90, hist, HIST_LINEAR, 3, 4);
        bucketValueAndVerify("HIST_LINEAR: x=", 100, hist, HIST_LINEAR, 4, 1);
        bucketValueAndVerify("HIST_LINEAR: x=", 110, hist, HIST_LINEAR, 4, 2);
        bucketValueAndVerify("HIST_LINEAR: x=", 98999, hist, HIST_LINEAR, 4, 3);

        MetricsUtils.GenericBucket[] phb = MetricsUtils.linearHistogramToGenericBuckets(hist,
                HIST_LINEAR);
        collector.checkThat("Number of buckets", phb.length, equalTo(hist.size()));
        validateProtoHistBucket("Bucket[0]", phb[0], Integer.MIN_VALUE, 10, 4);
        validateProtoHistBucket("Bucket[1]", phb[1], 10, 30, 2);
        validateProtoHistBucket("Bucket[2]", phb[2], 30, 60, 3);
        validateProtoHistBucket("Bucket[3]", phb[3], 60, 100, 4);
        validateProtoHistBucket("Bucket[4]", phb[4], 100, Integer.MAX_VALUE, 3);
    }

    // utilities

    private void bucketValueAndVerify(String prefix, long value, SparseIntArray h,
            MetricsUtils.LogHistParms hp, int expectedKey, int expectedValue) {
        MetricsUtils.addValueToLogHistogram(value, h, hp);
        collector.checkThat(prefix + value, h.get(expectedKey), equalTo(expectedValue));
    }

    private void bucketValueAndVerify(String prefix, int value, SparseIntArray h, int[] hp,
            int expectedKey, int expectedValue) {
        MetricsUtils.addValueToLinearHistogram(value, h, hp);
        collector.checkThat(prefix + value, h.get(expectedKey), equalTo(expectedValue));
    }

    private void validateProtoHistBucket(String prefix, MetricsUtils.GenericBucket bucket,
            long start, long end, int count) {
        collector.checkThat(prefix + ": start", bucket.start, equalTo(start));
        collector.checkThat(prefix + ": end", bucket.end, equalTo(end));
        collector.checkThat(prefix + ": count", bucket.count, equalTo(count));
    }
}
