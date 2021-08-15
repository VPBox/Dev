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

package com.android.server.wifi;

import static org.mockito.Mockito.any;
import static org.mockito.Mockito.anyInt;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;

import androidx.test.filters.SmallTest;

import com.android.internal.app.IBatteryStats;

import org.junit.Before;
import org.junit.Test;
import org.mockito.Mock;
import org.mockito.MockitoAnnotations;

/*
 * Unit tests for {@link com.android.server.wifi.WifiStateTracker}.
 */
@SmallTest
public class WifiStateTrackerTest {

    private static final String TAG = "WifiStateTrackerTest";
    @Mock IBatteryStats mBatteryStats;
    private WifiStateTracker mWifiStateTracker;

    /**
     * Setup test.
     */
    @Before
    public void setUp() {
        MockitoAnnotations.initMocks(this);
        mWifiStateTracker = new WifiStateTracker(mBatteryStats);
    }

    /**
     * Ensure BatteryStats's noteWifiState() is called when the method
     * updateState() is invoked on WifiStateTracker for relevant states.
     */
    @Test
    public void testBatteryStatsUpdated() throws Exception {
        int[] relevantStates = new int[] { WifiStateTracker.SCAN_MODE,
                WifiStateTracker.CONNECTED, WifiStateTracker.DISCONNECTED,
                WifiStateTracker.SOFT_AP};
        for (int i = 0; i < relevantStates.length; i++) {
            mWifiStateTracker.updateState(relevantStates[i]);
        }
        verify(mBatteryStats, times(relevantStates.length)).noteWifiState(anyInt(), any());
    }

    /**
     * Ensure BatteryStats's noteWifiState() is not called when the method
     * updateState() is invoked on WifiStateTracker for irrelevant states.
     */
    @Test
    public void testBatteryStatsNotUpdated() throws Exception {
        int[] irrelevantStates = new int[] { WifiStateTracker.SCAN_MODE - 1,
                WifiStateTracker.SOFT_AP + 1};
        for (int i = 0; i < irrelevantStates.length; i++) {
            mWifiStateTracker.updateState(irrelevantStates[i]);
        }
        verify(mBatteryStats, times(0)).noteWifiState(anyInt(), any());
    }
}
