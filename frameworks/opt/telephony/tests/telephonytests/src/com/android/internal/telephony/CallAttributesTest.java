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

import android.os.Parcel;
import android.telephony.CallAttributes;
import android.telephony.CallQuality;
import android.telephony.PreciseCallState;
import android.test.AndroidTestCase;
import android.test.suitebuilder.annotation.SmallTest;

/**
 * Simple GTS test verifying the parceling and unparceling of CallAttributes.
 */
public class CallAttributesTest extends AndroidTestCase {

    @SmallTest
    public void testParcelUnparcelCallQuality() {
        CallQuality quality = new CallQuality();
        PreciseCallState pcs = new PreciseCallState();
        CallAttributes data = new CallAttributes(pcs, 10, quality);

        Parcel parcel = Parcel.obtain();
        data.writeToParcel(parcel, 0);
        parcel.setDataPosition(0);
        CallAttributes unparceledData = CallAttributes.CREATOR.createFromParcel(parcel);
        parcel.recycle();

        assertEquals("CallQuality is not equal after parceled/unparceled",
                data.getCallQuality(),
                unparceledData.getCallQuality());
    }

    @SmallTest
    public void testParcelUnparcelPreciseCallState() {
        CallQuality quality = new CallQuality();
        PreciseCallState pcs = new PreciseCallState();
        CallAttributes data = new CallAttributes(pcs, 10, quality);

        Parcel parcel = Parcel.obtain();
        data.writeToParcel(parcel, 0);
        parcel.setDataPosition(0);
        CallAttributes unparceledData = CallAttributes.CREATOR.createFromParcel(parcel);
        parcel.recycle();

        assertEquals("PreciseCallState is not equal after parceled/unparceled",
                data.getPreciseCallState(),
                unparceledData.getPreciseCallState());
    }

    @SmallTest
    public void testParcelUnparcelNetworkType() {
        CallQuality quality = new CallQuality();
        PreciseCallState pcs = new PreciseCallState();
        CallAttributes data = new CallAttributes(pcs, 10, quality);

        Parcel parcel = Parcel.obtain();
        data.writeToParcel(parcel, 0);
        parcel.setDataPosition(0);
        CallAttributes unparceledData = CallAttributes.CREATOR.createFromParcel(parcel);
        parcel.recycle();

        assertEquals("NetworkType is not equal after parceled/unparceled",
                data.getNetworkType(),
                unparceledData.getNetworkType());
    }
}
