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
import static org.junit.Assert.assertNotEquals;

import android.os.Parcel;
import android.telephony.ModemInfo;
import android.test.suitebuilder.annotation.SmallTest;

import org.junit.Test;

public class ModemInfoTest {
    @Test
    @SmallTest
    public void basicTests() throws Exception {
        int modemId = 1;
        int rat = 2;
        boolean isVoiceSupported = true;
        boolean isDataSupported = false;
        ModemInfo modemInfo = new ModemInfo(modemId, rat, isVoiceSupported, isDataSupported);

        assertEquals(modemId, modemInfo.modemId);
        assertEquals(rat, modemInfo.rat);
        assertEquals(isVoiceSupported, modemInfo.isVoiceSupported);
        assertEquals(isDataSupported, modemInfo.isDataSupported);
        assertNotEquals(modemInfo, new ModemInfo(
                modemId + 1, rat, isVoiceSupported, isDataSupported));
        assertNotEquals(modemInfo, new ModemInfo(
                modemId, rat + 1, isVoiceSupported, isDataSupported));
        assertNotEquals(modemInfo, new ModemInfo(modemId, rat, !isVoiceSupported, isDataSupported));
        assertNotEquals(modemInfo, new ModemInfo(modemId, rat, isVoiceSupported, !isDataSupported));
    }

    @Test
    @SmallTest
    public void parcelReadWrite() throws Exception {
        int modemId = 1;
        int rat = 2;
        boolean isVoiceSupported = true;
        boolean isDataSupported = false;
        ModemInfo modemInfo = new ModemInfo(modemId, rat, isVoiceSupported, isDataSupported);

        Parcel parcel = Parcel.obtain();
        modemInfo.writeToParcel(parcel, 0);
        parcel.setDataPosition(0);
        ModemInfo toCompare = ModemInfo.CREATOR.createFromParcel(parcel);

        assertEquals(modemId, toCompare.modemId);
        assertEquals(rat, toCompare.rat);
        assertEquals(isVoiceSupported, toCompare.isVoiceSupported);
        assertEquals(isDataSupported, toCompare.isDataSupported);
        assertEquals(modemInfo, toCompare);
    }
}
