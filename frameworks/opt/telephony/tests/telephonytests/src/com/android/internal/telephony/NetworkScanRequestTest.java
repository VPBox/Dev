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
import android.telephony.AccessNetworkConstants.EutranBand;
import android.telephony.AccessNetworkConstants.GeranBand;
import android.telephony.NetworkScanRequest;
import android.telephony.RadioAccessSpecifier;

import androidx.test.filters.SmallTest;

import org.junit.Test;

import java.util.ArrayList;

/** Unit tests for {@link NetworkScanRequest}. */
public class NetworkScanRequestTest {

    @Test
    @SmallTest
    public void testParcel() {
        int ranGsm = AccessNetworkType.GERAN;
        int[] gsmBands = {GeranBand.BAND_T380, GeranBand.BAND_T410};
        int[] gsmChannels = {1, 2, 3, 4};
        RadioAccessSpecifier gsm = new RadioAccessSpecifier(ranGsm, gsmBands, gsmChannels);
        int ranLte = AccessNetworkType.EUTRAN;
        int[] lteBands = {EutranBand.BAND_10, EutranBand.BAND_11};
        int[] lteChannels = {5, 6, 7, 8};
        RadioAccessSpecifier lte = new RadioAccessSpecifier(ranLte, lteBands, lteChannels);
        RadioAccessSpecifier[] ras = {gsm, lte};
        int searchPeriodicity = 70;
        int maxSearchTime = 200;
        boolean incrementalResults = true;
        int incrementalResultsPeriodicity = 7;
        ArrayList<String> mccmncs = new ArrayList<String>();
        mccmncs.add("310480");
        mccmncs.add("21002");
        NetworkScanRequest nsq = new NetworkScanRequest(NetworkScanRequest.SCAN_TYPE_ONE_SHOT, ras,
                  searchPeriodicity, maxSearchTime, incrementalResults,
                  incrementalResultsPeriodicity, mccmncs);

        Parcel p = Parcel.obtain();
        nsq.writeToParcel(p, 0);
        p.setDataPosition(0);

        NetworkScanRequest newNsq = NetworkScanRequest.CREATOR.createFromParcel(p);
        assertEquals(nsq, newNsq);
    }
}
