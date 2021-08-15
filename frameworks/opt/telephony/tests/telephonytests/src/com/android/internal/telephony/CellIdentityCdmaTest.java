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

package com.android.internal.telephony;

import static org.junit.Assert.assertEquals;

import android.os.Parcel;
import android.telephony.CellIdentityCdma;
import android.test.AndroidTestCase;
import android.test.suitebuilder.annotation.SmallTest;

/** Unit tests for {@link CellIdentityCdma}. */

public class CellIdentityCdmaTest extends AndroidTestCase {

    // Network Id ranges from 0 to 65535.
    private static final int NETWORK_ID  = 65535;
    // CDMA System Id ranges from 0 to 32767
    private static final int SYSTEM_ID = 32767;
    // Base Station Id ranges from 0 to 65535
    private static final int BASESTATION_ID = 65535;
    // Longitude ranges from -2592000 to 2592000.
    private static final int LONGITUDE = 2592000;
    // Latitude ranges from -1296000 to 1296000.
    private static final int LATITUDE = 1296000;
    private static final String ALPHA_LONG = "long";
    private static final String ALPHA_SHORT = "short";

    @SmallTest
    public void testConstructor() {
        CellIdentityCdma  ci =
                new CellIdentityCdma(NETWORK_ID, SYSTEM_ID, BASESTATION_ID, LONGITUDE, LATITUDE,
                        ALPHA_LONG, ALPHA_SHORT);

        assertEquals(NETWORK_ID, ci.getNetworkId());
        assertEquals(LATITUDE, ci.getLatitude());
        assertEquals(LONGITUDE, ci.getLongitude());
        assertEquals(ALPHA_LONG, ci.getOperatorAlphaLong());
        assertEquals(ALPHA_SHORT, ci.getOperatorAlphaShort());
    }

    @SmallTest
    public void testNullIsland() {
        CellIdentityCdma  ci =
                new CellIdentityCdma(NETWORK_ID, SYSTEM_ID, BASESTATION_ID, -1, 0,
                        ALPHA_LONG, ALPHA_SHORT);

        assertEquals(Integer.MAX_VALUE, ci.getLatitude());
        assertEquals(Integer.MAX_VALUE, ci.getLongitude());
    }

    @SmallTest
    public void testEquals() {
        CellIdentityCdma  ciA =
                new CellIdentityCdma(NETWORK_ID, SYSTEM_ID, BASESTATION_ID, LONGITUDE, LATITUDE,
                        ALPHA_LONG, ALPHA_SHORT);
        CellIdentityCdma  ciB =
                new CellIdentityCdma(NETWORK_ID, SYSTEM_ID, BASESTATION_ID, LONGITUDE, LATITUDE,
                        ALPHA_LONG, ALPHA_SHORT);

        assertTrue(ciA.equals(ciB));

        ciA = new CellIdentityCdma(NETWORK_ID, SYSTEM_ID, BASESTATION_ID, LONGITUDE, LATITUDE,
                null, null);
        ciB = new CellIdentityCdma(NETWORK_ID, SYSTEM_ID, BASESTATION_ID, LONGITUDE, LATITUDE,
                null, null);

        assertTrue(ciA.equals(ciB));

        ciA = new CellIdentityCdma(NETWORK_ID, SYSTEM_ID, BASESTATION_ID, LONGITUDE, LATITUDE,
                ALPHA_LONG, ALPHA_SHORT);
        ciB = new CellIdentityCdma(NETWORK_ID, SYSTEM_ID, BASESTATION_ID, LONGITUDE, LATITUDE,
                null, null);

        assertFalse(ciA.equals(ciB));
    }

    @SmallTest
    public void testParcel() {
        CellIdentityCdma  ci =
                new CellIdentityCdma(NETWORK_ID, SYSTEM_ID, BASESTATION_ID, LONGITUDE, LATITUDE,
                        ALPHA_LONG, ALPHA_SHORT);

        Parcel p = Parcel.obtain();
        ci.writeToParcel(p, 0);
        p.setDataPosition(0);

        CellIdentityCdma newCi = CellIdentityCdma.CREATOR.createFromParcel(p);
        assertEquals(ci, newCi);
    }
}
