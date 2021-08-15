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

import static junit.framework.Assert.assertTrue;

import android.net.wifi.RttManager;
import android.net.wifi.RttManager.ParcelableRttParams;
import android.net.wifi.RttManager.RttParams;
import android.os.Parcel;

import androidx.test.filters.SmallTest;

import org.junit.Test;

/**
 * Unit test for {@link RttManager}
 */
@SmallTest
public class RttManagerTest {

    // Verify ParcelableRttParams are the same after writing and reading from parcel.
    private void verifyReadWriteParcelForRttParams(ParcelableRttParams params) {
        Parcel parcel = Parcel.obtain();
        params.writeToParcel(parcel, 0);
        parcel.setDataPosition(0);
        ParcelableRttParams paramsFromParcel = ParcelableRttParams.CREATOR.createFromParcel(parcel);
        assertTrue(verifyEquals(params, paramsFromParcel));
    }

    // Check if two ParcelableRttParams equals.
    private boolean verifyEquals(ParcelableRttParams params, ParcelableRttParams params2) {
        if (params.mParams == params2.mParams) {
            return true;
        }
        if (params == null || params2.mParams == null) {
            return false;
        }
        RttParams[] paramsArray = params.mParams;
        RttParams[] paramsArray2 = params2.mParams;
        if (paramsArray.length != paramsArray2.length) {
            return false;
        }
        for (int i = 0; i < paramsArray.length; i++) {
            if (!rttParamsEquals(paramsArray[i], paramsArray2[i])) {
                return false;
            }
        }
        return true;
    }

    // Check if two RttParams equals. Note only a subset of fields are checked.
    private boolean rttParamsEquals(RttParams params1, RttParams params2) {
        return params1.bssid.equals(params2.bssid)
                && params1.secure == params2.secure
                && params1.frequency == params2.frequency;
    }

    /**
     * Test writing and reading {@link RttParams} from Parcel.
     */
    @Test
    public void testRttParamsReadWriteParcel() throws Exception {
        RttParams params = new RttParams();
        params.bssid = "12-34-56-78-9A-BC";
        params.secure = true;
        params.frequency = 5240;

        RttParams params2 = new RttParams();
        params2.bssid = "12-34-56-78-9B-CD";
        params2.secure = false;
        params2.frequency = 5220;

        ParcelableRttParams parcelableParams = new ParcelableRttParams(new RttParams[] {
                params, params2
        });
        verifyReadWriteParcelForRttParams(parcelableParams);
        // Make sure writing/reading parcel doesn't change value for empty RttParams.
        verifyReadWriteParcelForRttParams(new ParcelableRttParams(new RttParams[0]));
    }

}
