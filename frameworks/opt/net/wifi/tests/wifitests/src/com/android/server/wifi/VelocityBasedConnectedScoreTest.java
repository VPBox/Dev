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

package com.android.server.wifi;

import static org.junit.Assert.assertTrue;
import static org.mockito.Mockito.doReturn;
import static org.mockito.Mockito.when;

import android.content.Context;
import android.content.res.Resources;
import android.net.wifi.WifiInfo;

import androidx.test.filters.SmallTest;

import com.android.internal.R;

import org.junit.Before;
import org.junit.Test;
import org.mockito.Mock;
import org.mockito.MockitoAnnotations;
import org.mockito.Spy;

/**
 * Unit tests for {@link com.android.server.wifi.VelocityBasedConnectedScore}.
 */
@SmallTest
public class VelocityBasedConnectedScoreTest {


    class FakeClock extends Clock {
        long mWallClockMillis = 1500000000000L;
        int mStepMillis = 3001;

        @Override
        public long getWallClockMillis() {
            mWallClockMillis += mStepMillis;
            return mWallClockMillis;
        }
    }

    FakeClock mClock;
    VelocityBasedConnectedScore mVelocityBasedConnectedScore;
    ScanDetailCache mScanDetailCache;
    WifiInfo mWifiInfo;
    int mRssiExitThreshold2GHz;
    int mRssiExitThreshold5GHz;
    @Mock Context mContext;
    @Spy private MockResources mResources = new MockResources();

    private int setupIntegerResource(int resourceName, int value) {
        doReturn(value).when(mResources).getInteger(resourceName);
        return value;
    }

    /**
     * Sets up resource values for testing
     *
     * See frameworks/base/core/res/res/values/config.xml
     */
    private void setUpResources(Resources resources) {
        mRssiExitThreshold2GHz = setupIntegerResource(
                R.integer.config_wifi_framework_wifi_score_bad_rssi_threshold_24GHz, -83);
        mRssiExitThreshold5GHz = setupIntegerResource(
                R.integer.config_wifi_framework_wifi_score_bad_rssi_threshold_5GHz, -80);
    }

    /**
     * Sets up for unit test
     */
    @Before
    public void setUp() throws Exception {
        MockitoAnnotations.initMocks(this);
        setUpResources(mResources);
        mWifiInfo = new WifiInfo();
        mWifiInfo.setFrequency(2412);
        when(mContext.getResources()).thenReturn(mResources);
        mClock = new FakeClock();
        mVelocityBasedConnectedScore = new VelocityBasedConnectedScore(
            new ScoringParams(mContext), mClock);
    }

    /**
     * Generate a score with no updates
     *
     * Expect no crash, passing score
     */
    @Test
    public void noCrashWhenNoData() throws Exception {
        int score = mVelocityBasedConnectedScore.generateScore();
        assertTrue(score > ConnectedScore.WIFI_TRANSITION_SCORE);
    }

    /**
     *
     * Low RSSI, but some data is moving and error rate is low.
     *
     * Expect a score above threshold.
     */
    @Test
    public void allowLowRssiIfErrorRateIsLowAndSomeDataIsMoving() throws Exception {
        mWifiInfo.setRssi(mRssiExitThreshold2GHz - 2);
        mWifiInfo.setLinkSpeed(6); // Mbps
        mWifiInfo.txSuccessRate = 2.1; // proportional to pps
        mWifiInfo.txBadRate = .5;
        mWifiInfo.rxSuccessRate = 2.1;
        for (int i = 0; i < 10; i++) {
            mVelocityBasedConnectedScore.updateUsingWifiInfo(mWifiInfo,
                    mClock.getWallClockMillis());
        }
        int score = mVelocityBasedConnectedScore.generateScore();
        assertTrue(score > ConnectedScore.WIFI_TRANSITION_SCORE);
        // If we reset, should be below threshold after the first input
        mVelocityBasedConnectedScore.reset();
        mVelocityBasedConnectedScore.updateUsingWifiInfo(mWifiInfo, mClock.getWallClockMillis());
        score = mVelocityBasedConnectedScore.generateScore();
        assertTrue(score < ConnectedScore.WIFI_TRANSITION_SCORE);
    }

    /**
     *
     * Low RSSI, and almost no data is moving.
     *
     * Expect a score below threshold.
     */
    @Test
    public void disallowLowRssiIfDataIsNotMoving() throws Exception {
        mWifiInfo.setRssi(mRssiExitThreshold2GHz - 1);
        mWifiInfo.setLinkSpeed(6); // Mbps
        mWifiInfo.txSuccessRate = .1; // proportional to pps
        mWifiInfo.txBadRate = 0;
        mWifiInfo.rxSuccessRate = .1;
        for (int i = 0; i < 10; i++) {
            mVelocityBasedConnectedScore.updateUsingWifiInfo(mWifiInfo,
                    mClock.getWallClockMillis());
        }
        int score = mVelocityBasedConnectedScore.generateScore();
        assertTrue(score < ConnectedScore.WIFI_TRANSITION_SCORE);
    }
}
