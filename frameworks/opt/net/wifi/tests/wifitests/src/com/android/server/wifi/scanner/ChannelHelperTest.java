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

package com.android.server.wifi.scanner;

import static com.android.server.wifi.ScanTestUtil.channelsToNativeSettings;
import static com.android.server.wifi.ScanTestUtil.channelsToSpec;
import static com.android.server.wifi.ScanTestUtil.createRequest;

import static org.junit.Assert.assertEquals;
import static org.mockito.Mockito.any;
import static org.mockito.Mockito.doCallRealMethod;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.verify;

import android.net.wifi.WifiScanner;

import androidx.test.filters.SmallTest;

import com.android.server.wifi.WifiNative;

import org.junit.Before;
import org.junit.Test;
import org.junit.experimental.runners.Enclosed;
import org.junit.runner.RunWith;

/**
 * Unit tests for {@link com.android.server.wifi.scanner.ChannelHelper}.
 */
@RunWith(Enclosed.class) // WARNING: tests cannot be declared in the outer class
public class ChannelHelperTest {

    /**
     * Unit tests for
     * {@link com.android.server.wifi.scanner.ChannelHelper#toString}.
     */
    @SmallTest
    public static class ToStringTest {
        /**
         * Compute a string representing the channels in a ScanSettings with a band set.
         */
        @Test
        public void scanSettings_band() {
            WifiScanner.ScanSettings scanSettings = createRequest(WifiScanner.WIFI_BAND_BOTH,
                    10000, 0, 20, WifiScanner.REPORT_EVENT_AFTER_EACH_SCAN);
            assertEquals("24Ghz & 5Ghz (no DFS)", ChannelHelper.toString(scanSettings));
        }

        /**
         * Compute a string representing the channels in a ScanSettings with a list of channels set.
         */
        @Test
        public void scanSettings_channels() {
            WifiScanner.ScanSettings scanSettings = createRequest(channelsToSpec(5150, 2400),
                    10000, 0, 20, WifiScanner.REPORT_EVENT_AFTER_EACH_SCAN);
            assertEquals("[5150,2400]", ChannelHelper.toString(scanSettings));
        }

        /**
         * Compute a string representing the channels in a BucketSettings with a band set.
         */
        @Test
        public void bucketSettings_band() {
            WifiScanner.ScanSettings scanSettings = createRequest(WifiScanner.WIFI_BAND_5_GHZ,
                    10000, 0, 20, WifiScanner.REPORT_EVENT_AFTER_EACH_SCAN);
            assertEquals("5Ghz (no DFS)", ChannelHelper.toString(scanSettings));
        }

        /**
         * Compute a string representing the channels in a BucketSettings with a list of channels
         * set.
         */
        @Test
        public void bucketSettings_channels() {
            WifiScanner.ScanSettings scanSettings = createRequest(channelsToSpec(2400, 5100),
                    10000, 0, 20, WifiScanner.REPORT_EVENT_AFTER_EACH_SCAN);
            assertEquals("[2400,5100]", ChannelHelper.toString(scanSettings));
        }
    }

    /**
     * Unit tests for
     * {@link com.android.server.wifi.scanner.ChannelHelper#bandToString}.
     */
    @Test
    public void bandToString_ShouldReturnApproapriateString() {
        assertEquals("unspecified", ChannelHelper.bandToString(
                WifiScanner.WIFI_BAND_UNSPECIFIED));
        assertEquals("24Ghz", ChannelHelper.bandToString(WifiScanner.WIFI_BAND_24_GHZ));
        assertEquals("5Ghz (no DFS)", ChannelHelper.bandToString(WifiScanner.WIFI_BAND_5_GHZ));
        assertEquals("5Ghz (DFS only)", ChannelHelper.bandToString(
                WifiScanner.WIFI_BAND_5_GHZ_DFS_ONLY));
        assertEquals("5Ghz (DFS incl)", ChannelHelper.bandToString(
                WifiScanner.WIFI_BAND_5_GHZ_WITH_DFS));
        assertEquals("24Ghz & 5Ghz (no DFS)", ChannelHelper.bandToString(
                WifiScanner.WIFI_BAND_BOTH));
        assertEquals("24Ghz & 5Ghz (DFS incl)", ChannelHelper.bandToString(
                WifiScanner.WIFI_BAND_BOTH_WITH_DFS));
        assertEquals("invalid band", ChannelHelper.bandToString(-235342));
    }

    /**
     * Unit tests for
     * {@link com.android.server.wifi.scanner.ChannelHelper.ChannelCollection}.
     */
    @SmallTest
    public static class ChannelCollectionTest {
        ChannelHelper.ChannelCollection mChannelCollection;

        /**
         * Called before each test
         * Create an instance of ChannelCollection that calls all real methods, but allows mocking
         * of abstract methods.
         */
        @Before
        public void setUp() throws Exception {
            mChannelCollection = mock(ChannelHelper.ChannelCollection.class);
            doCallRealMethod().when(mChannelCollection)
                    .addChannels(any(WifiScanner.ScanSettings.class));
            doCallRealMethod().when(mChannelCollection)
                    .addChannels(any(WifiNative.BucketSettings.class));
        }

        /**
         * Verify adding channels from a WifiScanner.ScanSettings with channels
         */
        @Test
        public void addChannelsWifiScanner_channels() {
            WifiScanner.ScanSettings testSettings = createRequest(channelsToSpec(5150, 2400),
                    10000, 0, 20, WifiScanner.REPORT_EVENT_AFTER_EACH_SCAN);

            mChannelCollection.addChannels(testSettings);

            verify(mChannelCollection).addChannel(2400);
            verify(mChannelCollection).addChannel(5150);
        }

        /**
         * Verify adding channels from a WifiScanner.ScanSettings with bands
         */
        @Test
        public void addChannelsWifiScanner_band() {
            WifiScanner.ScanSettings testSettings = createRequest(WifiScanner.WIFI_BAND_BOTH,
                    10000, 0, 20, WifiScanner.REPORT_EVENT_AFTER_EACH_SCAN);

            mChannelCollection.addChannels(testSettings);

            verify(mChannelCollection).addBand(WifiScanner.WIFI_BAND_BOTH);
        }

        /**
         * Verify adding channels from a WifiNative.BucketSetting with channels
         */
        @Test
        public void addChannelsWifiNativeBucket_channels() {
            WifiNative.BucketSettings testBucket = new WifiNative.BucketSettings();
            testBucket.band = WifiScanner.WIFI_BAND_UNSPECIFIED;
            testBucket.num_channels = 2;
            testBucket.channels = channelsToNativeSettings(2450, 5100);

            mChannelCollection.addChannels(testBucket);

            verify(mChannelCollection).addChannel(2450);
            verify(mChannelCollection).addChannel(5100);
        }

        /**
         * Verify adding channels from a WifiNative.BucketSetting with bands
         */
        @Test
        public void addChannelsWifiNativeBucket_band() {
            WifiNative.BucketSettings testBucket = new WifiNative.BucketSettings();
            testBucket.band = WifiScanner.WIFI_BAND_5_GHZ;

            mChannelCollection.addChannels(testBucket);

            verify(mChannelCollection).addBand(WifiScanner.WIFI_BAND_5_GHZ);
        }
    }
}
