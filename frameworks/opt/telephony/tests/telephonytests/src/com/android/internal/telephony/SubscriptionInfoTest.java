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
package com.android.internal.telephony;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotEquals;

import android.os.Parcel;
import android.telephony.SubscriptionInfo;
import android.test.suitebuilder.annotation.SmallTest;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

public class SubscriptionInfoTest {
    private SubscriptionInfo mSubscriptionInfoUT;
    private static final String[] EHPLMNS = new String[] {"310999", "310998"};
    private static final String[] HPLMNS = new String[] {"310001"};

    @After
    public void tearDown() throws Exception {
        mSubscriptionInfoUT = null;
    }

    @Before
    public void setUp() throws Exception {
        mSubscriptionInfoUT = new SubscriptionInfo(1, "890126042XXXXXXXXXXX", 0, "T-mobile",
                "T-mobile", 0, 255, "12345", 0, null, "310", "260", "156", false, null, null);
        mSubscriptionInfoUT.setAssociatedPlmns(EHPLMNS, HPLMNS);
    }

    @Test
    @SmallTest
    public void testSubProperties() {
        assertEquals(260, mSubscriptionInfoUT.getMnc());
        assertEquals(310, mSubscriptionInfoUT.getMcc());
        assertEquals("12345", mSubscriptionInfoUT.getNumber());
        assertEquals(0, mSubscriptionInfoUT.getDataRoaming());
        assertEquals("T-mobile", mSubscriptionInfoUT.getDisplayName());
        assertEquals("T-mobile", mSubscriptionInfoUT.getCarrierName());
        assertEquals("156", mSubscriptionInfoUT.getCountryIso());
        assertEquals(255, mSubscriptionInfoUT.getIconTint());
        assertEquals(0, mSubscriptionInfoUT.getNameSource());
        assertEquals(1, mSubscriptionInfoUT.getSubscriptionId());
        assertEquals(0, mSubscriptionInfoUT.getSimSlotIndex());
        assertEquals("890126042XXXXXXXXXXX", mSubscriptionInfoUT.getIccId());
    }

    @Test
    @SmallTest
    public void testSetGetCarrierName() {
        assertEquals("T-mobile", mSubscriptionInfoUT.getCarrierName());
        mSubscriptionInfoUT.setCarrierName("Verizon");
        assertEquals("Verizon", mSubscriptionInfoUT.getCarrierName());
    }

    @Test
    @SmallTest
    public void testSetGetDisplayName() {
        assertEquals("T-mobile", mSubscriptionInfoUT.getDisplayName());
        mSubscriptionInfoUT.setDisplayName("Verizon");
        assertEquals("Verizon", mSubscriptionInfoUT.getDisplayName());
    }

    @Test
    @SmallTest
    public void testSetGetIconTint() {
        assertEquals(255, mSubscriptionInfoUT.getIconTint());
        mSubscriptionInfoUT.setIconTint(0);
        assertEquals(0, mSubscriptionInfoUT.getIconTint());
    }

    @Test
    @SmallTest
    public void testParcelUnparcel() {
        Parcel p = Parcel.obtain();
        mSubscriptionInfoUT.writeToParcel(p, 0);
        p.setDataPosition(0);
        SubscriptionInfo copy = SubscriptionInfo.CREATOR.createFromParcel(p);
        assertEquals(mSubscriptionInfoUT, copy);
    }

    @Test
    @SmallTest
    public void testEquals() {
        SubscriptionInfo copiedInfo = new SubscriptionInfo(1, "890126042XXXXXXXXXXX", 0,
                "T-mobile", "T-mobile", 0, 255, "12345", 0, null,
                "310", "260", "156", false, null, null);
        copiedInfo.setAssociatedPlmns(EHPLMNS, HPLMNS);
        SubscriptionInfo differentDisplayName = new SubscriptionInfo(1, "890126042XXXXXXXXXXX", 0,
                "AT&T", "T-mobile", 0, 255, "12345", 0, null,
                "310", "260", "156", false, null, null);
        SubscriptionInfo differentSubId = new SubscriptionInfo(2, "890126042XXXXXXXXXXX", 0,
                "AT&T", "T-mobile", 0, 255, "12345", 0, null,
                "310", "260", "156", false, null, null);

        assertEquals(mSubscriptionInfoUT, copiedInfo);
        assertNotEquals(mSubscriptionInfoUT, differentDisplayName);
        assertNotEquals(mSubscriptionInfoUT, differentSubId);
    }
}
