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
import android.os.PersistableBundle;
import android.telephony.CarrierConfigManager;
import android.telephony.CellInfo;
import android.telephony.CellSignalStrengthWcdma;
import android.test.AndroidTestCase;
import android.test.suitebuilder.annotation.SmallTest;

/** Unit tests for {@link CellSignalStrengthCdma}. */

public class CellSignalStrengthWcdmaTest extends AndroidTestCase {

    private static final int RSSI = -95;
    private static final int BER = 3;
    private static final int RSCP = -106;
    private static final int ECNO = -5;

    @SmallTest
    public void testConstructor() {
        CellSignalStrengthWcdma css = new CellSignalStrengthWcdma(RSSI, BER, RSCP, ECNO);
        assertEquals(RSSI, css.getRssi());
        assertEquals(BER, css.getBitErrorRate());
        assertEquals(RSCP, css.getRscp());
        assertEquals(ECNO, css.getEcNo());
    }

    @SmallTest
    public void testInvalidConstructor() {
        CellSignalStrengthWcdma css = new CellSignalStrengthWcdma(-1, -1, -1, 10);
        assertEquals(CellInfo.UNAVAILABLE, css.getRssi());
        assertEquals(CellInfo.UNAVAILABLE, css.getBitErrorRate());
        assertEquals(CellInfo.UNAVAILABLE, css.getRscp());
        assertEquals(CellInfo.UNAVAILABLE, css.getEcNo());
    }

    @SmallTest
    public void testDefaultConstructor() {
        CellSignalStrengthWcdma css = new CellSignalStrengthWcdma();
        assertEquals(CellInfo.UNAVAILABLE, css.getRssi());
        assertEquals(CellInfo.UNAVAILABLE, css.getBitErrorRate());
        assertEquals(CellInfo.UNAVAILABLE, css.getRscp());
        assertEquals(CellInfo.UNAVAILABLE, css.getEcNo());
    }

    @SmallTest
    public void testEquals() {
        assertTrue(new CellSignalStrengthWcdma(RSSI, BER, RSCP, ECNO).equals(
                        new CellSignalStrengthWcdma(RSSI, BER, RSCP, ECNO)));
        assertFalse(new CellSignalStrengthWcdma(RSSI, BER, RSCP, ECNO).equals(
                    new CellSignalStrengthWcdma(RSSI, BER, RSCP + 1, ECNO)));
    }

    @SmallTest
    public void testParcel() {
        CellSignalStrengthWcdma css = new CellSignalStrengthWcdma(-1, -1, -1, 10);

        Parcel p = Parcel.obtain();
        css.writeToParcel(p, 0);
        p.setDataPosition(0);

        CellSignalStrengthWcdma newCss = CellSignalStrengthWcdma.CREATOR.createFromParcel(p);
        assertEquals(css, newCss);
    }

    @SmallTest
    public void testLevel() {
        CellSignalStrengthWcdma css = new CellSignalStrengthWcdma(RSSI, BER, RSCP, ECNO);
        PersistableBundle b = new PersistableBundle();

        // No keys in the bundle - should use RSSI and default levels.
        css.updateLevel(b, null); // ServiceState isn't used in WCDMA (yet)
        assertEquals(2 /* MODERATE */, css.getLevel());

        // Add RSCP levels but set the measurement as an invalid (empty string), should still use
        // RSSI.
        b.putIntArray(CarrierConfigManager.KEY_WCDMA_RSCP_THRESHOLDS_INT_ARRAY,
                new int[] {
                        -115, /* SIGNAL_STRENGTH_POOR */
                        -105, /* SIGNAL_STRENGTH_MODERATE */
                        -95, /* SIGNAL_STRENGTH_GOOD */
                        -85 /* SIGNAL_STRENGTH_GREAT */
                });
        b.putString(CarrierConfigManager.KEY_WCDMA_DEFAULT_SIGNAL_STRENGTH_MEASUREMENT_STRING, "");
        css.updateLevel(b, null); // ServiceState isn't used in WCDMA (yet)
        assertEquals(2 /* MODERATE */, css.getLevel());

        // Update the calculation to use RSCP, and expect the level to be calculated accordingly.
        b.putString(CarrierConfigManager.KEY_WCDMA_DEFAULT_SIGNAL_STRENGTH_MEASUREMENT_STRING,
                "rscp");
        css.updateLevel(b, null); // ServiceState isn't used in WCDMA (yet)
        assertEquals(1 /* POOR */, css.getLevel());
    }

}
