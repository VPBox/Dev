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

package com.android.server.wifi;

import static org.junit.Assert.assertEquals;

import android.net.wifi.WifiManager;

import com.android.server.wifi.nano.WifiMetricsProto.DeviceMobilityStatePnoScanStats;
import com.android.server.wifi.nano.WifiMetricsProto.HistogramBucketInt32;
import com.android.server.wifi.nano.WifiMetricsProto.Int32Count;
import com.android.server.wifi.nano.WifiMetricsProto.LinkProbeStats.ExperimentProbeCounts;
import com.android.server.wifi.nano.WifiMetricsProto.LinkProbeStats.LinkProbeFailureReasonCount;
import com.android.server.wifi.nano.WifiMetricsProto.StaEvent;

import java.util.Arrays;
import java.util.Comparator;

/**
 * Utility functions for {@link WifiMetricsTest}.
 */
public class WifiMetricsTestUtil {

    /**
     * Asserts that the two arrays are equal, reporting any difference between them.
     * Note: The order of buckets in each array must match!
     */
    public static void assertHistogramBucketsEqual(HistogramBucketInt32[] expected,
            HistogramBucketInt32[] actual) {
        assertEquals("Number of buckets do not match!", expected.length, actual.length);

        for (int i = 0; i < expected.length; i++) {
            HistogramBucketInt32 expectedBucket = expected[i];
            HistogramBucketInt32 actualBucket = actual[i];

            assertEquals(String.format("Bucket[%d].start does not match!", i),
                    expectedBucket.start, actualBucket.start);
            assertEquals(String.format("Bucket[%d].end does not match!", i),
                    expectedBucket.end, actualBucket.end);
            assertEquals(String.format("Bucket[%d].count does not match!", i),
                    expectedBucket.count, actualBucket.count);
        }
    }

    /**
     * The constructor we wish HistogramBucketInt32 had.
     */
    public static HistogramBucketInt32 buildHistogramBucketInt32(int start, int end, int count) {
        HistogramBucketInt32 bucket = new HistogramBucketInt32();
        bucket.start = start;
        bucket.end = end;
        bucket.count = count;
        return bucket;
    }

    /**
     * Asserts that the two arrays are equal, reporting any difference between them.
     * Note: The order of key counts in each array must match!
     */
    public static void assertKeyCountsEqual(Int32Count[] expected, Int32Count[] actual) {
        assertEquals("Number of key counts do not match!", expected.length, actual.length);

        for (int i = 0; i < expected.length; i++) {
            Int32Count expectedKeyCount = expected[i];
            Int32Count actualKeyCount = actual[i];

            assertEquals(String.format("KeyCount[%d].key does not match!", i),
                    expectedKeyCount.key, actualKeyCount.key);
            assertEquals(String.format("KeyCount[%d].count does not match!", i),
                    expectedKeyCount.count, actualKeyCount.count);
        }
    }

    /**
     * The constructor we wish Int32Count had.
     */
    public static Int32Count buildInt32Count(int key, int count) {
        Int32Count keyCount = new Int32Count();
        keyCount.key = key;
        keyCount.count = count;
        return keyCount;
    }

    /**
     * Asserts that the two arrays are equal (ignoring order),
     * reporting any difference between them.
     */
    public static void assertLinkProbeFailureReasonCountsEqual(
            LinkProbeFailureReasonCount[] expected, LinkProbeFailureReasonCount[] actual) {
        assertEquals("Number of LinkProbeFailureReasonCounts do not match!",
                expected.length, actual.length);

        Arrays.sort(expected, Comparator.comparingInt(x -> x.failureReason));
        Arrays.sort(actual, Comparator.comparingInt(x -> x.failureReason));

        for (int i = 0; i < expected.length; i++) {
            LinkProbeFailureReasonCount expectedFailureReasonCount = expected[i];
            LinkProbeFailureReasonCount actualFailureReasonCount = actual[i];

            assertEquals(String.format(
                    "LinkProbeFailureReasonCount[%d].failureReason does not match!", i),
                    expectedFailureReasonCount.failureReason,
                    actualFailureReasonCount.failureReason);
            assertEquals(String.format("LinkProbeFailureReasonCount[%d].count does not match!", i),
                    expectedFailureReasonCount.count, actualFailureReasonCount.count);
        }
    }

    /**
     * The constructor we wish LinkProbeFailureReasonCount had.
     */
    public static LinkProbeFailureReasonCount buildLinkProbeFailureReasonCount(int failureReason,
            int count) {
        LinkProbeFailureReasonCount failureReasonCount = new LinkProbeFailureReasonCount();
        failureReasonCount.failureReason = failureReason;
        failureReasonCount.count = count;
        return failureReasonCount;
    }

    /**
     * The constructor we wish DeviceMobilityStatePnoScanStats had.
     */
    public static DeviceMobilityStatePnoScanStats buildDeviceMobilityStatePnoScanStats(
            @WifiManager.DeviceMobilityState int deviceMobilityState, int numTimesEnteredState,
            long totalDurationMs, long pnoDurationMs) {
        DeviceMobilityStatePnoScanStats stats = new DeviceMobilityStatePnoScanStats();
        stats.deviceMobilityState = deviceMobilityState;
        stats.numTimesEnteredState = numTimesEnteredState;
        stats.totalDurationMs = totalDurationMs;
        stats.pnoDurationMs = pnoDurationMs;
        return stats;
    }

    /**
     * Asserts that the two arrays are equal (ignoring order),
     * reporting any difference between them.
     */
    public static void assertDeviceMobilityStatePnoScanStatsEqual(
            DeviceMobilityStatePnoScanStats[] expected, DeviceMobilityStatePnoScanStats[] actual) {

        assertEquals("Number of DeviceMobilityStatePnoScanStats do not match!",
                expected.length, actual.length);

        Arrays.sort(expected, Comparator.comparingInt(x -> x.deviceMobilityState));
        Arrays.sort(actual, Comparator.comparingInt(x -> x.deviceMobilityState));

        for (int i = 0; i < expected.length; i++) {
            DeviceMobilityStatePnoScanStats expectedStats = expected[i];
            DeviceMobilityStatePnoScanStats actualStats = actual[i];

            assertEquals(String.format(
                    "DeviceMobilityStatePnoScanStats[%d].deviceMobilityState does not match!", i),
                    expectedStats.deviceMobilityState, actualStats.deviceMobilityState);
            assertEquals(String.format(
                    "DeviceMobilityStatePnoScanStats[%d].numTimesEnteredState does not match!", i),
                    expectedStats.numTimesEnteredState, actualStats.numTimesEnteredState);
            assertEquals(String.format(
                    "DeviceMobilityStatePnoScanStats[%d].totalDurationMs does not match!", i),
                    expectedStats.totalDurationMs, actualStats.totalDurationMs);
            assertEquals(String.format(
                    "DeviceMobilityStatePnoScanStats[%d].pnoDurationMs does not match!", i),
                    expectedStats.pnoDurationMs, actualStats.pnoDurationMs);
        }
    }

    /**
     * Creates a StaEvent of type TYPE_LINK_PROBE that was successful.
     */
    public static StaEvent buildLinkProbeSuccessStaEvent(int elapsedTimeMs) {
        StaEvent probe = new StaEvent();
        probe.type = StaEvent.TYPE_LINK_PROBE;
        probe.linkProbeWasSuccess = true;
        probe.linkProbeSuccessElapsedTimeMs = elapsedTimeMs;
        return probe;
    }

    /**
     * Creates a StaEvent of type TYPE_LINK_PROBE that failed.
     */
    public static StaEvent buildLinkProbeFailureStaEvent(int reason) {
        StaEvent probe = new StaEvent();
        probe.type = StaEvent.TYPE_LINK_PROBE;
        probe.linkProbeWasSuccess = false;
        probe.linkProbeFailureReason = reason;
        return probe;
    }

    /**
     * Asserts that the two arrays are equal, reporting any difference between them.
     * Note that the order must match.
     */
    public static void assertLinkProbeStaEventsEqual(StaEvent[] expected, StaEvent[] actual) {
        assertEquals("Number of StaEvents do not match!", expected.length, actual.length);

        for (int i = 0; i < expected.length; i++) {
            StaEvent expectedEvent = expected[i];
            StaEvent actualEvent = actual[i];

            assertEquals(String.format("expected StaEvent[%d].type != TYPE_LINK_PROBE", i),
                    StaEvent.TYPE_LINK_PROBE, expectedEvent.type);
            assertEquals(String.format("actual StaEvent[%d].type != TYPE_LINK_PROBE", i),
                    StaEvent.TYPE_LINK_PROBE, actualEvent.type);
            assertEquals(String.format("StaEvent[%d].linkProbeWasSuccess does not match!", i),
                    expectedEvent.linkProbeWasSuccess, actualEvent.linkProbeWasSuccess);
            if (expectedEvent.linkProbeWasSuccess) {
                assertEquals(String.format(
                        "StaEvent[%d].linkProbeSuccessElapsedTimeMs does not match!", i),
                        expectedEvent.linkProbeSuccessElapsedTimeMs,
                        actualEvent.linkProbeSuccessElapsedTimeMs);
            } else {
                assertEquals(String.format(
                        "StaEvent[%d].linkProbeFailureReason does not match!", i),
                        expectedEvent.linkProbeFailureReason, actualEvent.linkProbeFailureReason);
            }
        }
    }

    /**
     * The constructor we wish ExperimentProbeCounts had.
     */
    public static ExperimentProbeCounts buildExperimentProbeCounts(
            String experimentId, int probeCount) {
        ExperimentProbeCounts counts = new ExperimentProbeCounts();
        counts.experimentId = experimentId;
        counts.probeCount = probeCount;
        return counts;
    }

    /**
     * Asserts that the two arrays are equal (ignoring order),
     * reporting any difference between them.
     */
    public static void assertExperimentProbeCountsEqual(
            ExperimentProbeCounts[] expected, ExperimentProbeCounts[] actual) {

        assertEquals("Number of ExperimentProbeCounts do not match!",
                expected.length, actual.length);

        Arrays.sort(expected, Comparator.comparing(x -> x.experimentId));
        Arrays.sort(actual, Comparator.comparing(x -> x.experimentId));

        for (int i = 0; i < expected.length; i++) {
            ExperimentProbeCounts expectedCounts = expected[i];
            ExperimentProbeCounts actualCounts = actual[i];

            assertEquals(String.format(
                    "ExperimentProbeCounts[%d].experimentId does not match!", i),
                    expectedCounts.experimentId, actualCounts.experimentId);
            assertEquals(String.format(
                    "ExperimentProbeCounts[%d].probeCount does not match!", i),
                    expectedCounts.probeCount, actualCounts.probeCount);
        }
    }
}
