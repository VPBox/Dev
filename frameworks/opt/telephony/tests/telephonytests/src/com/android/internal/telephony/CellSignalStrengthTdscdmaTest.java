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

package com.android.internal.telephony;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import android.os.Parcel;
import android.telephony.CellInfo;
import android.telephony.CellSignalStrengthTdscdma;
import android.test.AndroidTestCase;
import android.test.suitebuilder.annotation.SmallTest;

/** Unit tests for {@link CellSignalStrengthCdma}. */

public class CellSignalStrengthTdscdmaTest extends AndroidTestCase {

    private static final int RSSI = -95;
    private static final int BER = 3;
    private static final int RSCP = -106;

    @SmallTest
    public void testConstructor() {
        CellSignalStrengthTdscdma css = new CellSignalStrengthTdscdma(RSSI, BER, RSCP);
        assertEquals(RSSI, css.getRssi());
        assertEquals(BER, css.getBitErrorRate());
        assertEquals(RSCP, css.getRscp());
    }

    @SmallTest
    public void testInvalidConstructor() {
        CellSignalStrengthTdscdma css = new CellSignalStrengthTdscdma(-1, -1, -1);
        assertEquals(CellInfo.UNAVAILABLE, css.getRssi());
        assertEquals(CellInfo.UNAVAILABLE, css.getBitErrorRate());
        assertEquals(CellInfo.UNAVAILABLE, css.getRscp());
    }

    @SmallTest
    public void testDefaultConstructor() {
        CellSignalStrengthTdscdma css = new CellSignalStrengthTdscdma();
        assertEquals(CellInfo.UNAVAILABLE, css.getRssi());
        assertEquals(CellInfo.UNAVAILABLE, css.getBitErrorRate());
        assertEquals(CellInfo.UNAVAILABLE, css.getRscp());
    }

    @SmallTest
    public void testEquals() {
        assertTrue(new CellSignalStrengthTdscdma(RSSI, BER, RSCP).equals(
                        new CellSignalStrengthTdscdma(RSSI, BER, RSCP)));
        assertFalse(new CellSignalStrengthTdscdma(RSSI, BER, RSCP).equals(
                    new CellSignalStrengthTdscdma(RSSI, BER, RSCP + 1)));
    }

    @SmallTest
    public void testParcel() {
        CellSignalStrengthTdscdma css = new CellSignalStrengthTdscdma(-1, -1, -1);

        Parcel p = Parcel.obtain();
        css.writeToParcel(p, 0);
        p.setDataPosition(0);

        CellSignalStrengthTdscdma newCss = CellSignalStrengthTdscdma.CREATOR.createFromParcel(p);
        assertEquals(css, newCss);
    }

    @SmallTest
    public void testLevel() {
        CellSignalStrengthTdscdma css = new CellSignalStrengthTdscdma(RSSI, BER, RSCP);
        assertEquals(1 /* POOR */, css.getLevel());
    }

}
