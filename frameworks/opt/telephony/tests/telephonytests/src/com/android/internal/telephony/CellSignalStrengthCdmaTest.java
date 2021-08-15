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

package com.android.internal.telephony;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import android.os.Parcel;
import android.telephony.CellSignalStrengthCdma;
import android.test.AndroidTestCase;
import android.test.suitebuilder.annotation.SmallTest;

/** Unit tests for {@link CellSignalStrengthCdma}. */

public class CellSignalStrengthCdmaTest extends AndroidTestCase {

    private static final int CDMA_DBM = -74;
    private static final int CDMA_ECIO = -124;
    private static final int EVDO_DBM = -23;
    private static final int EVDO_ECIO = -108;
    private static final int EVDO_SNR = 7;

    @SmallTest
    public void testConstructor() {
        CellSignalStrengthCdma css = new CellSignalStrengthCdma(
                CDMA_DBM, CDMA_ECIO, EVDO_DBM, EVDO_ECIO, EVDO_SNR);
        assertEquals(CDMA_DBM, css.getCdmaDbm());
        assertEquals(CDMA_ECIO, css.getCdmaEcio());
        assertEquals(EVDO_DBM, css.getEvdoDbm());
        assertEquals(EVDO_ECIO, css.getEvdoEcio());
        assertEquals(EVDO_SNR, css.getEvdoSnr());
    }

    @SmallTest
    public void testInvalidConstructor() {
        CellSignalStrengthCdma css = new CellSignalStrengthCdma(200, 2000, 20, 400, 200);
        assertEquals(Integer.MAX_VALUE, css.getCdmaDbm());
        assertEquals(Integer.MAX_VALUE, css.getCdmaEcio());
        assertEquals(Integer.MAX_VALUE, css.getEvdoDbm());
        assertEquals(Integer.MAX_VALUE, css.getEvdoEcio());
        assertEquals(Integer.MAX_VALUE, css.getEvdoSnr());
    }

    @SmallTest
    public void testDefaultConstructor() {
        CellSignalStrengthCdma css = new CellSignalStrengthCdma();
        assertEquals(Integer.MAX_VALUE, css.getCdmaDbm());
        assertEquals(Integer.MAX_VALUE, css.getCdmaEcio());
        assertEquals(Integer.MAX_VALUE, css.getEvdoDbm());
        assertEquals(Integer.MAX_VALUE, css.getEvdoEcio());
        assertEquals(Integer.MAX_VALUE, css.getEvdoSnr());
    }

    @SmallTest
    public void testEquals() {
        assertTrue(new CellSignalStrengthCdma(
                CDMA_DBM, CDMA_ECIO, EVDO_DBM, EVDO_ECIO, EVDO_SNR).equals(
                        new CellSignalStrengthCdma(
                                CDMA_DBM, CDMA_ECIO, EVDO_DBM, EVDO_ECIO, EVDO_SNR)));
        assertFalse(new CellSignalStrengthCdma(
                CDMA_DBM, CDMA_ECIO, EVDO_DBM, EVDO_ECIO, EVDO_SNR).equals(
                    new CellSignalStrengthCdma(CDMA_DBM, CDMA_ECIO, -24, EVDO_ECIO, EVDO_SNR)));
    }

    @SmallTest
    public void testParcel() {
        CellSignalStrengthCdma css = new CellSignalStrengthCdma(
                CDMA_DBM, CDMA_ECIO, EVDO_DBM, EVDO_ECIO, EVDO_SNR);

        Parcel p = Parcel.obtain();
        css.writeToParcel(p, 0);
        p.setDataPosition(0);

        CellSignalStrengthCdma newCss = CellSignalStrengthCdma.CREATOR.createFromParcel(p);
        assertEquals(css, newCss);
    }
}
