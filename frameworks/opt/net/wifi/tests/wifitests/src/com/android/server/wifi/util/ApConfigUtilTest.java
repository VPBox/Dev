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

package com.android.server.wifi.util;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;
import static org.mockito.Mockito.when;

import android.net.wifi.WifiConfiguration;
import android.net.wifi.WifiScanner;

import androidx.test.filters.SmallTest;

import com.android.internal.util.ArrayUtils;
import com.android.server.wifi.WifiNative;

import org.junit.Before;
import org.junit.Test;
import org.mockito.Mock;
import org.mockito.MockitoAnnotations;

import java.util.ArrayList;
import java.util.Arrays;

/**
 * Unit tests for {@link com.android.server.wifi.util.ApConfigUtil}.
 */
@SmallTest
public class ApConfigUtilTest {

    private static final String TEST_COUNTRY_CODE = "TestCountry";

    /**
     * Frequency to channel map. This include some frequencies used outside the US.
     * Representing it using a vector (instead of map) for simplification.  Values at
     * even indices are frequencies and odd indices are channels.
     */
    private static final int[] FREQUENCY_TO_CHANNEL_MAP = {
            2412, 1,
            2417, 2,
            2422, 3,
            2427, 4,
            2432, 5,
            2437, 6,
            2442, 7,
            2447, 8,
            2452, 9,
            2457, 10,
            2462, 11,
            /* 12, 13 are only legitimate outside the US. */
            2467, 12,
            2472, 13,
            /* 14 is for Japan, DSSS and CCK only. */
            2484, 14,
            /* 34 valid in Japan. */
            5170, 34,
            5180, 36,
            5190, 38,
            5200, 40,
            5210, 42,
            5220, 44,
            5230, 46,
            5240, 48,
            5260, 52,
            5280, 56,
            5300, 60,
            5320, 64,
            5500, 100,
            5520, 104,
            5540, 108,
            5560, 112,
            5580, 116,
            /* 120, 124, 128 valid in Europe/Japan. */
            5600, 120,
            5620, 124,
            5640, 128,
            /* 132+ valid in US. */
            5660, 132,
            5680, 136,
            5700, 140,
            /* 144 is supported by a subset of WiFi chips. */
            5720, 144,
            5745, 149,
            5765, 153,
            5785, 157,
            5805, 161,
            5825, 165
    };

    private static final Integer[] ALLOWED_2G_CHANNELS = {1, 2, 3, 4};
    private static final int[] ALLOWED_5G_FREQS = {5180, 5190, 5200};
    private static final int[] ALLOWED_5G_CHANNELS = {36, 38, 40};

    @Mock WifiNative mWifiNative;
    private final ArrayList<Integer> mAllowed2GChannels =
            new ArrayList<Integer>(Arrays.asList(ALLOWED_2G_CHANNELS));

    /**
     * Setup test.
     */
    @Before
    public void setUp() throws Exception {
        MockitoAnnotations.initMocks(this);
    }

    /**
     * Verify frequency to channel conversion for all possible frequencies.
     */
    @Test
    public void convertFrequencyToChannel() throws Exception {
        for (int i = 0; i < FREQUENCY_TO_CHANNEL_MAP.length; i += 2) {
            assertEquals(FREQUENCY_TO_CHANNEL_MAP[i + 1],
                    ApConfigUtil.convertFrequencyToChannel(
                            FREQUENCY_TO_CHANNEL_MAP[i]));
        }
    }

    /**
     * Verify frequency to channel conversion failed for an invalid frequency.
     */
    @Test
    public void convertFrequencyToChannelWithInvalidFreq() throws Exception {
        assertEquals(-1, ApConfigUtil.convertFrequencyToChannel(6200));
    }

    /**
     * Verify default channel is used when picking a 2G channel without
     * any allowed 2G channels.
     */
    @Test
    public void chooseApChannel2GBandWithNoAllowedChannel() throws Exception {
        assertEquals(ApConfigUtil.DEFAULT_AP_CHANNEL, ApConfigUtil.chooseApChannel(
                WifiConfiguration.AP_BAND_2GHZ, null, ALLOWED_5G_FREQS));
    }

    /**
     * Verify a 2G channel is selected from the list of allowed channels.
     */
    @Test
    public void chooseApChannel2GBandWithAllowedChannels() throws Exception {
        int channel = ApConfigUtil.chooseApChannel(
                WifiConfiguration.AP_BAND_2GHZ, mAllowed2GChannels, ALLOWED_5G_FREQS);
        assertTrue(mAllowed2GChannels.contains(channel));
    }

    /**
     * Verify a 5G channel is selected from the list of allowed channels.
     */
    @Test
    public void chooseApChannel5GBandWithAllowedChannels() throws Exception {
        int channel = ApConfigUtil.chooseApChannel(
                WifiConfiguration.AP_BAND_5GHZ, mAllowed2GChannels, ALLOWED_5G_FREQS);
        assertTrue(ArrayUtils.contains(ALLOWED_5G_CHANNELS, channel));
    }

    /**
     * Verify chooseApChannel failed when selecting a channel in 5GHz band
     * with no channels allowed.
     */
    @Test
    public void chooseApChannel5GBandWithNoAllowedChannels() throws Exception {
        assertEquals(-1, ApConfigUtil.chooseApChannel(
                WifiConfiguration.AP_BAND_5GHZ, mAllowed2GChannels, null));
    }

    /**
     * Verify default band and channel is used when HAL support is
     * not available.
     */
    @Test
    public void updateApChannelConfigWithoutHal() throws Exception {
        WifiConfiguration config = new WifiConfiguration();
        config.apChannel = 36;
        config.apBand = WifiConfiguration.AP_BAND_5GHZ;
        when(mWifiNative.isHalStarted()).thenReturn(false);
        assertEquals(ApConfigUtil.SUCCESS,
                ApConfigUtil.updateApChannelConfig(
                        mWifiNative, TEST_COUNTRY_CODE, mAllowed2GChannels, config));
        /* Verify default band and channel is used. */
        assertEquals(ApConfigUtil.DEFAULT_AP_BAND, config.apBand);
        assertEquals(ApConfigUtil.DEFAULT_AP_CHANNEL, config.apChannel);
    }

    /**
     * Verify updateApChannelConfig will return an error when selecting channel
     * for 5GHz band without country code.
     */
    @Test
    public void updateApChannelConfig5GBandNoCountryCode() throws Exception {
        WifiConfiguration config = new WifiConfiguration();
        config.apBand = WifiConfiguration.AP_BAND_5GHZ;
        when(mWifiNative.isHalStarted()).thenReturn(true);
        assertEquals(ApConfigUtil.ERROR_GENERIC,
                ApConfigUtil.updateApChannelConfig(
                        mWifiNative, null, mAllowed2GChannels, config));
    }

    /**
     * Verify the AP band and channel is not updated if specified.
     */
    @Test
    public void updateApChannelConfigWithChannelSpecified() throws Exception {
        WifiConfiguration config = new WifiConfiguration();
        config.apBand = WifiConfiguration.AP_BAND_5GHZ;
        config.apChannel = 36;
        when(mWifiNative.isHalStarted()).thenReturn(true);
        assertEquals(ApConfigUtil.SUCCESS,
                ApConfigUtil.updateApChannelConfig(
                        mWifiNative, TEST_COUNTRY_CODE, mAllowed2GChannels, config));
        assertEquals(WifiConfiguration.AP_BAND_5GHZ, config.apBand);
        assertEquals(36, config.apChannel);
    }

    /**
     * Verify updateApChannelConfig will return an error when selecting 5GHz channel
     * without any allowed channels.
     */
    @Test
    public void updateApChannelConfigWith5GBandNoChannelAllowed() throws Exception {
        WifiConfiguration config = new WifiConfiguration();
        config.apBand = WifiConfiguration.AP_BAND_5GHZ;
        when(mWifiNative.isHalStarted()).thenReturn(true);
        when(mWifiNative.getChannelsForBand(WifiScanner.WIFI_BAND_5_GHZ))
                .thenReturn(null);
        assertEquals(ApConfigUtil.ERROR_NO_CHANNEL,
                ApConfigUtil.updateApChannelConfig(
                        mWifiNative, TEST_COUNTRY_CODE, mAllowed2GChannels, config));
    }
}
