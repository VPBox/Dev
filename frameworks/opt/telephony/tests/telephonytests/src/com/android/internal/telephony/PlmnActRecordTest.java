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

package com.android.internal.telephony.uicc;

import static org.junit.Assert.assertEquals;

import android.os.Parcel;
import android.test.AndroidTestCase;
import android.test.suitebuilder.annotation.SmallTest;

import java.util.Arrays;

/** Unit tests for PlmnActRecord */

public class PlmnActRecordTest extends AndroidTestCase {

    private static final String TEST_PLMN_5DIGIT = "12345";
    private static final byte[] TEST_RECORD_5DIGIT = new byte[] {
            (byte) 0x21, (byte) 0xF3, (byte) 0x54, (byte) 0xC0, (byte) 0x80};
    private static final String TEST_PLMN_6DIGIT = "123456";
    private static final byte[] TEST_RECORD_6DIGIT = new byte[] {
            (byte) 0x21, (byte) 0x63, (byte) 0x54, (byte) 0xC0, (byte) 0x80};

    private static final int ACCESS_TECHS_3GPP = PlmnActRecord.ACCESS_TECH_EUTRAN
            | PlmnActRecord.ACCESS_TECH_UTRAN | PlmnActRecord.ACCESS_TECH_GSM;

    @SmallTest
    public void testConstructors() {
        PlmnActRecord rec = new PlmnActRecord(TEST_PLMN_5DIGIT, ACCESS_TECHS_3GPP);
        assertEquals(TEST_PLMN_5DIGIT, rec.plmn);
        assertEquals(ACCESS_TECHS_3GPP, rec.accessTechs);

        PlmnActRecord rec2 = new PlmnActRecord(TEST_RECORD_5DIGIT, 0);
        assertEquals(TEST_PLMN_5DIGIT, rec.plmn);
        assertEquals(ACCESS_TECHS_3GPP, rec.accessTechs);

        assertEquals(rec, rec2);

        rec = new PlmnActRecord(TEST_PLMN_6DIGIT, ACCESS_TECHS_3GPP);
        assertEquals(TEST_PLMN_6DIGIT, rec.plmn);
        assertEquals(ACCESS_TECHS_3GPP, rec.accessTechs);

        rec2 = new PlmnActRecord(TEST_RECORD_6DIGIT, 0);
        assertEquals(TEST_PLMN_6DIGIT, rec.plmn);
        assertEquals(ACCESS_TECHS_3GPP, rec.accessTechs);

        assertEquals(rec, rec2);
    }

    @SmallTest
    public void testParcel() {
        PlmnActRecord par = new PlmnActRecord(TEST_PLMN_5DIGIT, ACCESS_TECHS_3GPP);

        Parcel p = Parcel.obtain();
        par.writeToParcel(p, 0);
        p.setDataPosition(0);

        PlmnActRecord par2 = PlmnActRecord.CREATOR.createFromParcel(p);
        assertEquals(par, par2);
    }

    @SmallTest
    public void testEncoding() {
        PlmnActRecord rec = new PlmnActRecord(TEST_RECORD_5DIGIT, 0);
        assertTrue(Arrays.equals(rec.getBytes(), TEST_RECORD_5DIGIT));

        rec = new PlmnActRecord(TEST_RECORD_6DIGIT, 0);
        assertTrue(Arrays.equals(rec.getBytes(), TEST_RECORD_6DIGIT));
    }
}
