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

package com.android.server.wifi;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;
import static org.mockito.ArgumentMatchers.any;
import static org.mockito.ArgumentMatchers.anyBoolean;
import static org.mockito.ArgumentMatchers.eq;
import static org.mockito.Mockito.doReturn;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;

import android.content.Context;
import android.content.res.Resources;
import android.database.ContentObserver;
import android.net.wifi.WifiInfo;
import android.os.Handler;
import android.provider.Settings;

import androidx.test.filters.SmallTest;

import com.android.internal.R;

import org.junit.Before;
import org.junit.Test;
import org.mockito.ArgumentCaptor;
import org.mockito.Mock;
import org.mockito.MockitoAnnotations;
import org.mockito.Spy;

/**
 * Unit tests for {@link com.android.server.wifi.ScoringParams}.
 */
@SmallTest
public class ScoringParamsTest {

    private static final String EXPECTED_DEFAULTS =
            "rssi2=-83:-80:-73:-60,rssi5=-80:-77:-70:-57,pps=0:1:100,horizon=15,nud=8,expid=0";

    private ScoringParams mScoringParams;

    /**
     * Sets up for unit test
     */
    @Before
    public void setUp() throws Exception {
        MockitoAnnotations.initMocks(this);
        setUpResources(mResources);
        when(mContext.getResources()).thenReturn(mResources);
        mScoringParams = new ScoringParams();
    }

    /**
     * Check that thresholds are properly ordered, and in range.
     */
    private void checkThresholds(int frequency) {
        assertTrue(-127 < mScoringParams.getExitRssi(frequency));
        assertTrue(mScoringParams.getExitRssi(frequency)
                <= mScoringParams.getEntryRssi(frequency));
        assertTrue(mScoringParams.getEntryRssi(frequency)
                <= mScoringParams.getSufficientRssi(frequency));
        assertTrue(mScoringParams.getSufficientRssi(frequency)
                <= mScoringParams.getGoodRssi(frequency));
        assertTrue(mScoringParams.getGoodRssi(frequency) < 0);
    }

    /**
     * Test basic constructor
     */
    @Test
    public void testBasicConstructor() throws Exception {
        mScoringParams = new ScoringParams();
        checkThresholds(2412);
        checkThresholds(5020);
        assertEquals(15, mScoringParams.getHorizonSeconds());
    }

    /**
     * Test toString
     */
    @Test
    public void testToString() throws Exception {
        mScoringParams = new ScoringParams();
        String expect = EXPECTED_DEFAULTS;
        String actual = mScoringParams.toString();
        assertEquals(expect, actual);
    }

    /**
     * Test complete update
     */
    @Test
    public void testUpdateEverything() throws Exception {
        mScoringParams = new ScoringParams();
        String params = EXPECTED_DEFAULTS.replaceAll("8", "9");
        assertTrue(mScoringParams.update(params));
        assertEquals(params, mScoringParams.toString());
    }

    /**
     * Test partial update
     */
    @Test
    public void testPartialUpdate() throws Exception {
        mScoringParams = new ScoringParams();
        String before = mScoringParams.toString();
        String partial = "rssi5=-88:-77:-66:-55";
        assertFalse(before.contains(partial));
        assertTrue(mScoringParams.update(partial));
        String after = mScoringParams.toString();
        assertTrue(after + " should contain " + partial, after.contains(partial));
        assertEquals(before.replaceAll("rssi5=[^,]*,", partial + ","), after);
    }

    /**
     * Test some failed updates
     */
    @Test
    public void testUpdateFail() throws Exception {
        mScoringParams = new ScoringParams();
        String before = mScoringParams.toString();
        assertFalse(mScoringParams.update("word"));
        assertFalse(mScoringParams.update("42"));
        assertFalse(mScoringParams.update(" "));
        assertFalse(mScoringParams.update("horizon=flat"));
        assertFalse(mScoringParams.update("horizon=+"));
        assertFalse(mScoringParams.update(",,,,,,,,,,,,,,,,,,"));
        assertFalse(mScoringParams.update("rssi2=-86"));
        assertFalse(mScoringParams.update("rssi2=-99:-88:-77:-66:-55"));
        assertFalse(mScoringParams.update("rssi5=one:two:three:four"));
        assertFalse(mScoringParams.update("nud=-1"));
        assertFalse(mScoringParams.update("nud=11"));
        assertFalse(mScoringParams.update("pps=1:2:3:4"));
        assertFalse(mScoringParams.update("pps=1:0:9"));
        assertFalse(mScoringParams.update("pps=packets-per-second"));
        assertEquals(before, mScoringParams.toString());
    }

    /**
     * Duplicate keys not allowed
     */
    @Test
    public void testDuplicateKeysNotAllowed() throws Exception {
        mScoringParams = new ScoringParams();
        String before = mScoringParams.toString();
        String partial = "rssi5=-88:-77:-66:-55";
        assertFalse(mScoringParams.update(partial + "," + partial));
        assertEquals(before, mScoringParams.toString());
    }

    /**
     * Range and ordering checks for rssi values should work
     */
    @Test
    public void testRssiRangeChecks() throws Exception {
        mScoringParams = new ScoringParams();
        String before = mScoringParams.toString();
        assertFalse("Must be negative", mScoringParams.update("rssi2=0:1:2:3"));
        assertFalse("Must be ordered", mScoringParams.update("rssi5=-88:-89:-66:-55"));
        assertFalse("Must be not too negative", mScoringParams.update("rssi5=-128:-77:-66:-55"));
        String what = "rssi5=" + WifiInfo.INVALID_RSSI + ":-77:-66:-55";
        assertFalse(what, mScoringParams.update(what));
        assertEquals(before, mScoringParams.toString());
    }

    /**
     * Range checks should catch wild values
     */
    @Test
    public void testScalarRangeChecks() throws Exception {
        mScoringParams = new ScoringParams();
        assertTrue(mScoringParams.update("horizon=60"));
        assertFalse(mScoringParams.update("horizon=61"));
        assertTrue(mScoringParams.update("horizon=-9")); // Not recommended, but shouldn't break
        assertFalse(mScoringParams.update("horizon=-10"));
    }

    /**
     * Test that nud is hooked up
     */
    @Test
    public void testNudKnob() throws Exception {
        mScoringParams = new ScoringParams();
        assertTrue(mScoringParams.update("nud=6"));
        assertEquals(6, mScoringParams.getNudKnob());
    }

    /**
     * Test that expid is hooked up
     */
    @Test
    public void testExperimentIdentifier() throws Exception {
        mScoringParams = new ScoringParams();
        assertEquals(0, mScoringParams.getExperimentIdentifier());
        assertTrue(mScoringParams.update("expid=99"));
        assertEquals(99, mScoringParams.getExperimentIdentifier());
    }

    /**
     * Check character set
     */
    @Test
    public void testBadCharacterChecks() throws Exception {
        mScoringParams = new ScoringParams();
        String before = mScoringParams.toString();
        assertFalse(mScoringParams.update("\007ding\007=7"));
        assertTrue(mScoringParams.update("unknown_key=14,ThatThing=987654321"));
        assertEquals(before, mScoringParams.toString());
    }

    /**
     * Sanitize should hide garbage characters and truncate length
     */
    @Test
    public void testSanitize() throws Exception {
        mScoringParams = new ScoringParams();
        String longKey = "A123456789_123456789_123456789_123456789_123456789";
        String longVal = "9.99999999999999999999999999999999999999999999999";
        String param100 = longKey + "=" + longVal;
        assertEquals(param100, mScoringParams.sanitize(param100));
        assertEquals(101, mScoringParams.sanitize(param100 + "00000000000").length());
        assertEquals(param100.substring(0, 90),
                mScoringParams.sanitize(param100 + "00000000000").substring(0, 90));
        assertEquals("q?=???", mScoringParams.sanitize("q\b= ~["));
        assertEquals("", mScoringParams.sanitize(null));
    }

    /**
     * Test that empty updates are OK
     */
    @Test
    public void testEmptyUpdate() throws Exception {
        mScoringParams = new ScoringParams();
        String before = mScoringParams.toString();
        assertTrue(mScoringParams.update(""));
        assertTrue(mScoringParams.update(null));
        assertEquals(before, mScoringParams.toString());
    }

    /**
     * Tests for obtaining values from device configuration (config.xml)
     */
    int mBad2GHz, mEntry2GHz, mSufficient2GHz, mGood2GHz;
    int mBad5GHz, mEntry5GHz, mSufficient5GHz, mGood5GHz;

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
        mBad2GHz = setupIntegerResource(
                R.integer.config_wifi_framework_wifi_score_bad_rssi_threshold_24GHz, -88);
        mEntry2GHz = setupIntegerResource(
                R.integer.config_wifi_framework_wifi_score_entry_rssi_threshold_24GHz, -77);
        mSufficient2GHz = setupIntegerResource(
                R.integer.config_wifi_framework_wifi_score_low_rssi_threshold_24GHz, -66);
        mGood2GHz = setupIntegerResource(
                R.integer.config_wifi_framework_wifi_score_good_rssi_threshold_24GHz, -55);
        mBad5GHz = setupIntegerResource(
                R.integer.config_wifi_framework_wifi_score_bad_rssi_threshold_5GHz, -80);
        mEntry5GHz = setupIntegerResource(
                R.integer.config_wifi_framework_wifi_score_entry_rssi_threshold_5GHz, -70);
        mSufficient5GHz = setupIntegerResource(
                R.integer.config_wifi_framework_wifi_score_low_rssi_threshold_5GHz, -60);
        mGood5GHz = setupIntegerResource(
                R.integer.config_wifi_framework_wifi_score_good_rssi_threshold_5GHz, -50);
    }

    /**
     * Check that we get the config.xml values, if that's what we want
     */
    @Test
    public void testContextConstructor() throws Exception {
        mScoringParams = new ScoringParams(mContext);

        assertEquals(mBad2GHz, mScoringParams.getExitRssi(2412));
        assertEquals(mEntry2GHz, mScoringParams.getEntryRssi(2480));
        assertEquals(mSufficient2GHz, mScoringParams.getSufficientRssi(2400));
        assertEquals(mGood2GHz, mScoringParams.getGoodRssi(2499));
        assertEquals(mGood2GHz, mScoringParams.getGoodRssi(ScoringParams.BAND2));

        assertEquals(mBad5GHz, mScoringParams.getExitRssi(5000));
        assertEquals(mEntry5GHz, mScoringParams.getEntryRssi(5010));
        assertEquals(mSufficient5GHz, mScoringParams.getSufficientRssi(5100));
        assertEquals(mGood5GHz, mScoringParams.getGoodRssi(5678));
        assertEquals(mGood5GHz, mScoringParams.getGoodRssi(ScoringParams.BAND5));
    }

    /**
     * Additional mocks for handling Settings
     */
    @Mock FrameworkFacade mFrameworkFacade;
    @Mock Handler mHandler;

    /**
     * Test getting updates from Settings
     *
     * Exercises the ContentObserver notification path
     */
    @Test
    public void testFullConstructorWithUpdatesFromSettings() throws Exception {
        ArgumentCaptor<ContentObserver> captor = ArgumentCaptor.forClass(ContentObserver.class);
        when(mFrameworkFacade.getStringSetting(mContext, Settings.Global.WIFI_SCORE_PARAMS))
                .thenReturn(null);
        mScoringParams = new ScoringParams(mContext, mFrameworkFacade, mHandler);
        verify(mFrameworkFacade)
                .registerContentObserver(eq(mContext), any(), anyBoolean(), captor.capture());

        String before = mScoringParams.toString();
        String changed = before.replace('8', '9');
        assertFalse(changed.equals(before));

        when(mFrameworkFacade.getStringSetting(mContext, Settings.Global.WIFI_SCORE_PARAMS))
                .thenReturn(changed);
        captor.getValue().onChange(/*selfChange*/ false);
        assertEquals(changed, mScoringParams.toString());

        when(mFrameworkFacade.getStringSetting(mContext, Settings.Global.WIFI_SCORE_PARAMS))
                .thenReturn("");
        captor.getValue().onChange(/*selfChange*/ false);
        assertEquals(before, mScoringParams.toString());
    }

    @Test
    public void testBadSettings() throws Exception {
        ArgumentCaptor<ContentObserver> captor = ArgumentCaptor.forClass(ContentObserver.class);
        when(mFrameworkFacade.getStringSetting(mContext, Settings.Global.WIFI_SCORE_PARAMS))
                .thenReturn(null);
        mScoringParams = new ScoringParams(mContext, mFrameworkFacade, mHandler);
        verify(mFrameworkFacade)
                .registerContentObserver(eq(mContext), any(), anyBoolean(), captor.capture());

        String before = mScoringParams.toString();
        String garbage = "what??";

        when(mFrameworkFacade.getStringSetting(mContext, Settings.Global.WIFI_SCORE_PARAMS))
                .thenReturn(garbage);
        captor.getValue().onChange(/*selfChange*/ false);
        assertEquals(before, mScoringParams.toString());
    }
}
