/*
 * Copyright (C) 2015 The Android Open Source Project
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

package com.android.server.wifi.scanner;

import static com.android.server.wifi.ScanTestUtil.channelsToSpec;
import static com.android.server.wifi.ScanTestUtil.createRequest;
import static com.android.server.wifi.ScanTestUtil.createScanDatas;
import static com.android.server.wifi.ScanTestUtil.createScanResult;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertNull;
import static org.junit.Assert.assertTrue;

import android.net.wifi.WifiScanner;
import android.net.wifi.WifiScanner.ScanData;
import android.net.wifi.WifiScanner.ScanSettings;

import androidx.test.filters.SmallTest;

import org.junit.Before;
import org.junit.Test;

/**
 * Unit tests for filtering of scan results in
 * {@link com.android.server.wifi.scanner.ScanScheduleUtil}.
 */
@SmallTest
public class ScanScheduleUtilFilterTest {

    private ChannelHelper mChannelHelper;

    @Before
    public void setUp() throws Exception {
        mChannelHelper = new PresetKnownBandsChannelHelper(
                new int[]{2400, 2450},
                new int[]{5150, 5175},
                new int[]{5600, 5650});
    }

    @Test
    public void reportFullResultTrueForBandsWithNoBucketInfo() {
        ScanSettings settings = createRequest(
                WifiScanner.WIFI_BAND_24_GHZ, 30000, 0, 20,
                WifiScanner.REPORT_EVENT_FULL_SCAN_RESULT
        );

        assertTrue(ScanScheduleUtil.shouldReportFullScanResultForSettings(mChannelHelper,
                        createScanResult(2400), 0, settings, -1));
    }

    @Test
    public void reportFullResultFalseForBandsWithNoBucketInfo() {
        ScanSettings settings = createRequest(
                WifiScanner.WIFI_BAND_24_GHZ, 30000, 0, 20,
                WifiScanner.REPORT_EVENT_FULL_SCAN_RESULT
        );

        assertFalse(ScanScheduleUtil.shouldReportFullScanResultForSettings(mChannelHelper,
                        createScanResult(5150), 0, settings, 0));
    }

    @Test
    public void reportFullResultTrueForChannelsWithNoBucketInfo() {
        ScanSettings settings = createRequest(
                channelsToSpec(2400, 5150), 30000, 0, 20,
                WifiScanner.REPORT_EVENT_FULL_SCAN_RESULT
        );

        assertTrue(ScanScheduleUtil.shouldReportFullScanResultForSettings(mChannelHelper,
                        createScanResult(2400), 0, settings, 0));
    }

    @Test
    public void reportFullResultFalseForChannelsWithNoBucketInfo() {
        ScanSettings settings = createRequest(
                channelsToSpec(2400, 5150), 30000, 0, 20,
                WifiScanner.REPORT_EVENT_FULL_SCAN_RESULT
        );

        assertFalse(ScanScheduleUtil.shouldReportFullScanResultForSettings(mChannelHelper,
                        createScanResult(5175), 0, settings, 0));
    }

    @Test
    public void reportFullResultTrueForChannelsWithBucketDefinitlyScanned() {
        ScanSettings settings = createRequest(
                channelsToSpec(2400, 5150), 30000, 0, 20,
                WifiScanner.REPORT_EVENT_FULL_SCAN_RESULT
        );

        assertTrue(ScanScheduleUtil.shouldReportFullScanResultForSettings(mChannelHelper,
                        createScanResult(2400), 5, settings, 2));
    }

    @Test
    public void reportFullResultFalseForChannelsWithBucketDefinitlyNotScanned() {
        ScanSettings settings = createRequest(
                channelsToSpec(2400, 5150), 30000, 0, 20,
                WifiScanner.REPORT_EVENT_FULL_SCAN_RESULT
        );

        assertFalse(ScanScheduleUtil.shouldReportFullScanResultForSettings(mChannelHelper,
                        createScanResult(2400), 1, settings, 1));
    }

    @Test
    public void filterScanDataEmptyWithNoBucketInfo() {
        ScanSettings settings = createRequest(
                channelsToSpec(2400, 5150), 30000, 0, 20,
                WifiScanner.REPORT_EVENT_FULL_SCAN_RESULT
        );

        ScanData[] results = ScanScheduleUtil.filterResultsForSettings(mChannelHelper,
                new ScanData[0], settings, -1);
        assertScanDataFreqsEquals(null, results);
    }

    @Test
    public void filterScanDataSingleNotMatchingWithNoBucketInfo() {
        ScanSettings settings = createRequest(
                channelsToSpec(2400, 5150), 30000, 0, 20,
                WifiScanner.REPORT_EVENT_FULL_SCAN_RESULT
        );

        ScanData[] results = ScanScheduleUtil.filterResultsForSettings(mChannelHelper,
                createScanDatas(new int[][]{ { 2450 } }), settings, 0);
        assertScanDataFreqsEquals(null, results);
    }

    @Test
    public void filterScanDataSingleNotMatchingWithDefinitlyScannedBucketIndex() {
        ScanSettings settings = createRequest(
                channelsToSpec(2400, 5150), 30000, 0, 20,
                WifiScanner.REPORT_EVENT_FULL_SCAN_RESULT
        );

        ScanData[] results = ScanScheduleUtil.filterResultsForSettings(mChannelHelper,
                createScanDatas(new int[][]{ { 2450 } }), settings, -1);
        assertScanDataFreqsEquals(new int[][]{ { } }, results);
    }

    @Test
    public void filterScanDataSingleNotMatchingWithBucketDefinitlyScanned() {
        ScanSettings settings = createRequest(
                channelsToSpec(2400, 5150), 30000, 0, 20,
                WifiScanner.REPORT_EVENT_FULL_SCAN_RESULT
        );

        ScanData[] results = ScanScheduleUtil.filterResultsForSettings(mChannelHelper,
                createScanDatas(new int[][]{ { 2450 } }, new int[]{ 2 }), settings, 1);
        assertScanDataFreqsEquals(new int[][]{ { } }, results);
    }

    @Test
    public void filterScanDataSingleMatchingWithNoBucketInfo() {
        ScanSettings settings = createRequest(
                channelsToSpec(2400, 5150), 30000, 0, 20,
                WifiScanner.REPORT_EVENT_FULL_SCAN_RESULT
        );

        ScanData[] results = ScanScheduleUtil.filterResultsForSettings(mChannelHelper,
                createScanDatas(new int[][]{ { 2400 } }), settings, 0);

        assertScanDataFreqsEquals(new int[][]{ { 2400 } }, results);
    }

    @Test
    public void filterScanDataSingleMatchingWithBucketDefinitlyNotScanned() {
        ScanSettings settings = createRequest(
                channelsToSpec(2400, 5150), 30000, 0, 20,
                WifiScanner.REPORT_EVENT_FULL_SCAN_RESULT
        );

        ScanData[] results = ScanScheduleUtil.filterResultsForSettings(mChannelHelper,
                createScanDatas(new int[][]{ { 2400 } }, new int[] { 5 }), settings, 1);

        assertScanDataFreqsEquals(null, results);
    }

    @Test
    public void filterScanDataSinglePartialMatchingWithNoBucketInfo() {
        ScanSettings settings = createRequest(
                channelsToSpec(2400, 5150), 30000, 0, 20,
                WifiScanner.REPORT_EVENT_FULL_SCAN_RESULT
        );

        ScanData[] results = ScanScheduleUtil.filterResultsForSettings(mChannelHelper,
                createScanDatas(new int[][]{ { 2400, 2450, 5150, 5175 } }), settings, 0);

        assertScanDataFreqsEquals(new int[][]{ { 2400, 5150 } }, results);
    }

    @Test
    public void filterScanDataMultipleNotMatchingWithNoBucketInfo() {
        ScanSettings settings = createRequest(
                channelsToSpec(2400, 5150), 30000, 0, 20,
                WifiScanner.REPORT_EVENT_FULL_SCAN_RESULT
        );

        ScanData[] results = ScanScheduleUtil.filterResultsForSettings(mChannelHelper,
                createScanDatas(new int[][]{ { 2450 }, { 2450, 5175 } }), settings, 0);
        assertScanDataFreqsEquals(null, results);
    }

    @Test
    public void filterScanDataMultipleNotMatchingWithOneDefinitlyScanned() {
        ScanSettings settings = createRequest(
                channelsToSpec(2400, 5150), 30000, 0, 20,
                WifiScanner.REPORT_EVENT_FULL_SCAN_RESULT
        );

        ScanData[] results = ScanScheduleUtil.filterResultsForSettings(mChannelHelper,
                createScanDatas(new int[][]{ { 2450 }, { 2450, 5175 } }, new int[]{ 1, 8 }),
                settings, 3);
        assertScanDataFreqsEquals(new int[][]{ {} }, results);
    }

    @Test
    public void filterScanDataMultipleMatchingWithNoBucketInfo() {
        ScanSettings settings = createRequest(
                channelsToSpec(2400, 5150), 30000, 0, 20,
                WifiScanner.REPORT_EVENT_FULL_SCAN_RESULT
        );

        ScanData[] results = ScanScheduleUtil.filterResultsForSettings(mChannelHelper,
                createScanDatas(new int[][]{ { 2400 }, {2400, 5150} }), settings, 0);

        assertScanDataFreqsEquals(new int[][]{ { 2400 }, {2400, 5150} }, results);
    }

    @Test
    public void filterScanDataMultiplePartialMatchingWithNoBucketInfo() {
        ScanSettings settings = createRequest(
                channelsToSpec(2400, 5150), 30000, 0, 20,
                WifiScanner.REPORT_EVENT_FULL_SCAN_RESULT
        );

        ScanData[] results = ScanScheduleUtil.filterResultsForSettings(mChannelHelper,
                createScanDatas(new int[][]{ { 2400, 2450, 5150, 5175 }, { 2400, 2450, 5175 } }),
                settings, -1);

        assertScanDataFreqsEquals(new int[][]{ { 2400, 5150 }, { 2400 } }, results);
    }

    @Test
    public void filterScanDataMultipleDuplicateFrequenciesWithNoBucketInfo() {
        ScanSettings settings = createRequest(
                channelsToSpec(2400, 5150), 30000, 0, 20,
                WifiScanner.REPORT_EVENT_FULL_SCAN_RESULT
        );

        ScanData[] results = ScanScheduleUtil.filterResultsForSettings(mChannelHelper,
                createScanDatas(new int[][]{
                        { 2400, 2450, 5150, 5175, 2400 },
                        { 2400, 2450, 5175 },
                        { 5175, 5175, 5150 } }), settings, 0);

        assertScanDataFreqsEquals(new int[][]{ { 2400, 5150, 2400 }, { 2400 }, { 5150 } }, results);
    }

    @Test
    public void filterScanDataMultipleSomeNotMatchingWithNoBucketInfo() {
        ScanSettings settings = createRequest(
                channelsToSpec(2400, 5150), 30000, 0, 20,
                WifiScanner.REPORT_EVENT_FULL_SCAN_RESULT
        );

        ScanData[] results = ScanScheduleUtil.filterResultsForSettings(mChannelHelper,
                createScanDatas(new int[][]{
                        { 2400, 2450, 5150, 5175, 2400 },
                        { 5175 },
                        { 5175, 5175, 5150 } }), settings, 0);

        assertScanDataFreqsEquals(new int[][]{ { 2400, 5150, 2400 }, { 5150 } }, results);
    }

    @Test
    public void filterScanDataExceedMaxBssidsPerScanWithNoBucketInfo() {
        ScanSettings settings = createRequest(
                channelsToSpec(2400, 5150), 30000, 0, 3,
                WifiScanner.REPORT_EVENT_FULL_SCAN_RESULT
        );

        ScanData[] results = ScanScheduleUtil.filterResultsForSettings(mChannelHelper,
                createScanDatas(new int[][]{
                        { 2400, 2450, 5150, 5175, 2400, 2400},
                        { 5175 },
                        { 5175, 5175, 5150, 2400, 2400, 5150 } }), settings, 0);

        assertScanDataFreqsEquals(new int[][]{ { 2400, 5150, 2400 }, { 5150, 2400, 2400 } },
                results);
    }


    private static void assertScanDataFreqsEquals(int[][] expected, ScanData[] results) {
        if (expected == null) {
            assertNull("Expected no results", results);
        } else {
            assertNotNull("Expected some results", results);
            assertEquals("num scans", expected.length, results.length);
            for (int i = 0; i < expected.length; ++i) {
                assertNotNull("scan[" + i + "] was null", results[i]);
                assertEquals("num aps in scan[" + i + "]", expected[i].length,
                        results[i].getResults().length);
                for (int j = 0; j < expected[i].length; ++j) {
                    assertNotNull("ap result[" + i + "][" + j + "] was null",
                            results[i].getResults()[j]);
                    assertEquals("ap freq in result[" + i + "][" + j + "]", expected[i][j],
                            results[i].getResults()[j].frequency);
                }
            }
        }
    }
}
