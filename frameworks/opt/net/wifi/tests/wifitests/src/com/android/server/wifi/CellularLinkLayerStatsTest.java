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

import android.telephony.TelephonyManager;

import androidx.test.filters.SmallTest;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

/**
 * Unit tests for {@link com.android.server.wifi.CellularLinkLayerStats}.
 */
@SmallTest
public class CellularLinkLayerStatsTest {
    private static final String TAG = "CellularStatsTest";

    CellularLinkLayerStats mStats;

    /**
     * Sets up for unit test
     */
    @Before
    public void setUp() throws Exception {
        mStats = new CellularLinkLayerStats();
    }

    @After
    public void cleanUp() throws Exception {
    }

    /**
     * Test all set and get methods by checking if the inputs of set() match the output of get()
     */
    @Test
    public void testAllSetGetMethods() throws Exception {
        int dataNetworkType = TelephonyManager.NETWORK_TYPE_GSM;
        mStats.setDataNetworkType(dataNetworkType);
        assertEquals(dataNetworkType, mStats.getDataNetworkType());
        int dbmVal = -100;
        mStats.setSignalStrengthDbm(dbmVal);
        assertEquals(dbmVal, mStats.getSignalStrengthDbm());
        int dbVal = -20;
        mStats.setSignalStrengthDb(dbVal);
        assertEquals(dbVal, mStats.getSignalStrengthDb());
        boolean isSameCell = true;
        mStats.setIsSameRegisteredCell(isSameCell);
        assertEquals(isSameCell, mStats.getIsSameRegisteredCell());
    }
}
