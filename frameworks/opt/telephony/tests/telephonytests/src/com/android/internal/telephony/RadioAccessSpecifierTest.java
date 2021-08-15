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
import android.telephony.AccessNetworkConstants.AccessNetworkType;
import android.telephony.AccessNetworkConstants.GeranBand;
import android.telephony.RadioAccessSpecifier;

import androidx.test.filters.SmallTest;

import org.junit.Test;

/** Unit tests for {@link RadioAccessSpecifier}. */
public class RadioAccessSpecifierTest {

    @Test
    @SmallTest
    public void testParcel() {
        int ranGsm = AccessNetworkType.GERAN;
        int[] gsmBands = {GeranBand.BAND_T380, GeranBand.BAND_T410};
        int[] gsmChannels = {1, 2, 3, 4};
        RadioAccessSpecifier ras = new RadioAccessSpecifier(ranGsm, gsmBands, gsmChannels);

        Parcel p = Parcel.obtain();
        ras.writeToParcel(p, 0);
        p.setDataPosition(0);

        RadioAccessSpecifier newRas = RadioAccessSpecifier.CREATOR.createFromParcel(p);
        assertEquals(ras, newRas);
    }
}
