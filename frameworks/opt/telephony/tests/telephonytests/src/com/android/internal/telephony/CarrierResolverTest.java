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

import static com.android.internal.telephony.TelephonyTestUtils.waitForMs;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNull;
import static org.mockito.Matchers.eq;
import static org.mockito.Mockito.doReturn;

import android.database.Cursor;
import android.database.MatrixCursor;
import android.net.Uri;
import android.os.HandlerThread;
import android.provider.Telephony.CarrierId;
import android.provider.Telephony.Carriers;
import android.service.carrier.CarrierIdentifier;
import android.telephony.TelephonyManager;
import android.test.mock.MockContentProvider;
import android.test.mock.MockContentResolver;
import android.test.suitebuilder.annotation.SmallTest;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import java.util.Arrays;

public class CarrierResolverTest extends TelephonyTest {
    private static final String MCCMNC = "311480";
    private static final String NAME = "VZW";
    private static final int CID_VZW = 1;

    private static final String MCCMNC_VODAFONE = "20205";
    private static final String NAME_VODAFONE = "VODAFONE";
    private static final String SPN_VODAFONE = "vodafone GR";
    private static final int CID_VODAFONE = 5;

    private static final String SPN_FI = "PROJECT FI";
    private static final String NAME_FI = "FI";
    private static final int CID_FI = 2;

    private static final String NAME_DOCOMO = "DOCOMO";
    private static final String APN_DOCOMO = "mopera.net";
    private static final int CID_DOCOMO = 3;

    private static final String NAME_TMO = "TMO";
    private static final String MCCMNC_TMO = "310260";
    private static final int CID_TMO = 4;

    private static final String MCCMNC_ATT = "310410";
    private static final int CID_ATT = 5;

    private static final int CID_TRACFONE = 6;
    private static final int CID_TRACFONE_ATT = 7;
    private static final int CID_TRACFONE_TMO = 8;
    private static final String MCCMNC_TRACFONE_ATT = "310410";
    private static final String MCCMNC_TRACFONE_TMO = "310260";
    private static final String GID_TRACFONE = "DDFF";

    private static final int CID_O2 = 9;
    private static final int CID_O2_PREPAID = 10;
    private static final String MCCMNC_O2 = "23410";
    private static final String GID_O2_PREPAID = "61";


    private static final int CID_UNKNOWN = -1;

    // events to trigger carrier identification
    private static final int SIM_LOAD_EVENT       = 1;
    private static final int ICC_CHANGED_EVENT    = 2;
    private static final int PREFER_APN_SET_EVENT = 3;

    private CarrierResolver mCarrierResolver;
    private CarrierResolverHandler mCarrierCarrierResolverHandler;

    private class CarrierResolverHandler extends HandlerThread {
        private CarrierResolverHandler(String name) {
            super(name);
        }

        @Override
        public void onLooperPrepared() {
            mCarrierResolver = new CarrierResolver(mPhone);
            setReady(true);
        }
    }

    @Before
    public void setUp() throws Exception {
        logd("CarrierResolverTest +Setup!");
        super.setUp(getClass().getSimpleName());
        ((MockContentResolver) mContext.getContentResolver()).addProvider(
                CarrierId.AUTHORITY, new CarrierIdContentProvider());
        // start handler thread
        mCarrierCarrierResolverHandler = new CarrierResolverHandler(getClass().getSimpleName());
        mCarrierCarrierResolverHandler.start();
        waitUntilReady();
        mCarrierResolver.sendEmptyMessage(ICC_CHANGED_EVENT);
        logd("CarrierResolverTest -Setup!");
    }

    @After
    public void tearDown() throws Exception {
        logd("CarrierResolver -tearDown");
        mCarrierResolver.removeCallbacksAndMessages(null);
        mCarrierResolver = null;
        mCarrierCarrierResolverHandler.quit();
        mCarrierCarrierResolverHandler.join();
        super.tearDown();
    }

    @Test
    @SmallTest
    public void testCarrierMatch() {
        int phoneId = mPhone.getPhoneId();
        doReturn(MCCMNC).when(mTelephonyManager).getSimOperatorNumericForPhone(eq(phoneId));
        // trigger sim loading event
        mCarrierResolver.sendEmptyMessage(SIM_LOAD_EVENT);
        waitForMs(200);
        assertEquals(CID_VZW, mCarrierResolver.getCarrierId());
        assertEquals(NAME, mCarrierResolver.getCarrierName());

        doReturn(SPN_FI).when(mSimRecords).getServiceProviderName();
        mCarrierResolver.sendEmptyMessage(SIM_LOAD_EVENT);
        waitForMs(200);
        assertEquals(CID_FI, mCarrierResolver.getCarrierId());
        assertEquals(NAME_FI, mCarrierResolver.getCarrierName());
    }

    @Test
    @SmallTest
    public void testMnoCarrierId() {
        int phoneId = mPhone.getPhoneId();
        doReturn(MCCMNC).when(mTelephonyManager).getSimOperatorNumericForPhone(eq(phoneId));
        doReturn(SPN_FI).when(mSimRecords).getServiceProviderName();

        mCarrierResolver.sendEmptyMessage(SIM_LOAD_EVENT);
        waitForMs(200);

        assertEquals(CID_FI, mCarrierResolver.getCarrierId());
        assertEquals(NAME_FI, mCarrierResolver.getCarrierName());
        assertEquals(CID_VZW, mCarrierResolver.getMnoCarrierId());

        doReturn(MCCMNC_VODAFONE).when(mTelephonyManager)
                .getSimOperatorNumericForPhone(eq(phoneId));
        doReturn(SPN_VODAFONE).when(mSimRecords).getServiceProviderName();
        mCarrierResolver.sendEmptyMessage(SIM_LOAD_EVENT);
        waitForMs(200);
        assertEquals(CID_VODAFONE, mCarrierResolver.getCarrierId());
        assertEquals(NAME_VODAFONE, mCarrierResolver.getCarrierName());
        assertEquals(CID_VODAFONE, mCarrierResolver.getMnoCarrierId());
    }

    @Test
    @SmallTest
    public void testPreciseCarrierId() {
        int phoneId = mPhone.getPhoneId();
        doReturn(MCCMNC_TRACFONE_ATT).when(mTelephonyManager)
                .getSimOperatorNumericForPhone(eq(phoneId));

        mCarrierResolver.sendEmptyMessage(SIM_LOAD_EVENT);
        waitForMs(200);
        assertEquals(CID_ATT, mCarrierResolver.getCarrierId());
        assertEquals(CID_ATT, mCarrierResolver.getSpecificCarrierId());

        doReturn(GID_TRACFONE).when(mPhone).getGroupIdLevel1();
        mCarrierResolver.sendEmptyMessage(SIM_LOAD_EVENT);
        waitForMs(200);
        assertEquals(CID_TRACFONE, mCarrierResolver.getCarrierId());
        assertEquals(CID_TRACFONE_ATT, mCarrierResolver.getSpecificCarrierId());

        doReturn(MCCMNC_TRACFONE_TMO).when(mTelephonyManager)
                .getSimOperatorNumericForPhone(eq(phoneId));
        mCarrierResolver.sendEmptyMessage(SIM_LOAD_EVENT);
        waitForMs(200);
        assertEquals(CID_TRACFONE, mCarrierResolver.getCarrierId());
        assertEquals(CID_TRACFONE_TMO, mCarrierResolver.getSpecificCarrierId());

        doReturn(MCCMNC_O2).when(mTelephonyManager)
                .getSimOperatorNumericForPhone(eq(phoneId));
        mCarrierResolver.sendEmptyMessage(SIM_LOAD_EVENT);
        waitForMs(200);
        assertEquals(CID_O2, mCarrierResolver.getCarrierId());
        assertEquals(CID_O2, mCarrierResolver.getSpecificCarrierId());

        doReturn(MCCMNC_O2).when(mTelephonyManager)
                .getSimOperatorNumericForPhone(eq(phoneId));
        doReturn(GID_O2_PREPAID).when(mPhone).getGroupIdLevel1();
        mCarrierResolver.sendEmptyMessage(SIM_LOAD_EVENT);
        waitForMs(200);
        assertEquals(CID_O2, mCarrierResolver.getCarrierId());
        assertEquals(CID_O2_PREPAID, mCarrierResolver.getSpecificCarrierId());
    }

    @Test
    @SmallTest
    public void testCarrierMatchSimAbsent() {
        int phoneId = mPhone.getPhoneId();
        doReturn(MCCMNC).when(mTelephonyManager).getSimOperatorNumericForPhone(eq(phoneId));
        // trigger sim loading event
        mCarrierResolver.sendEmptyMessage(SIM_LOAD_EVENT);
        waitForMs(200);
        assertEquals(CID_VZW, mCarrierResolver.getCarrierId());
        assertEquals(NAME, mCarrierResolver.getCarrierName());
        // trigger sim absent event
        mCarrierResolver.resolveSubscriptionCarrierId(IccCardConstants.INTENT_VALUE_ICC_ABSENT);
        waitForMs(200);
        assertEquals(CID_UNKNOWN, mCarrierResolver.getCarrierId());
        assertNull(mCarrierResolver.getCarrierName());
        assertNull(mCarrierResolver.getCarrierName());
    }

    @Test
    @SmallTest
    public void testCarrierNoMatch() {
        // un-configured MCCMNC
        int phoneId = mPhone.getPhoneId();
        doReturn("12345").when(mTelephonyManager).getSimOperatorNumericForPhone(eq(phoneId));
        // trigger sim loading event
        mCarrierResolver.sendEmptyMessage(SIM_LOAD_EVENT);
        waitForMs(200);
        assertEquals(CID_UNKNOWN, mCarrierResolver.getCarrierId());
        assertNull(mCarrierResolver.getCarrierName());
    }

    @Test
    @SmallTest
    public void testGetCarrierIdFromIdentifier() {
        // trigger sim loading event
        mCarrierResolver.sendEmptyMessage(SIM_LOAD_EVENT);
        waitForMs(200);

        CarrierIdentifier identifier = new CarrierIdentifier(null, null, null, null, null, null);
        int carrierid = mCarrierResolver.getCarrierIdFromIdentifier(mContext, identifier);
        assertEquals(CID_UNKNOWN, carrierid);

        identifier = new CarrierIdentifier(MCCMNC.substring(0, 3), MCCMNC.substring(3), null, null,
                null, null);
        carrierid = mCarrierResolver.getCarrierIdFromIdentifier(mContext, identifier);
        assertEquals(CID_VZW, carrierid);

        identifier = new CarrierIdentifier(MCCMNC.substring(0, 3), MCCMNC.substring(3),  SPN_FI, null,
                null, null);
        carrierid = mCarrierResolver.getCarrierIdFromIdentifier(mContext, identifier);
        assertEquals(CID_FI, carrierid);
    }

    @Test
    @SmallTest
    public void testCarrierMatchPreferApnChange() {
        int phoneId = mPhone.getPhoneId();
        doReturn(MCCMNC).when(mTelephonyManager).getSimOperatorNumericForPhone(eq(phoneId));
        // trigger sim loading event
        mCarrierResolver.sendEmptyMessage(SIM_LOAD_EVENT);
        waitForMs(200);
        assertEquals(CID_VZW, mCarrierResolver.getCarrierId());
        assertEquals(NAME, mCarrierResolver.getCarrierName());
        // mock apn
        ((MockContentResolver) mContext.getContentResolver()).addProvider(
                Carriers.CONTENT_URI.getAuthority(), new CarrierIdContentProvider());
        mCarrierResolver.sendEmptyMessage(PREFER_APN_SET_EVENT);
        waitForMs(200);
        assertEquals(CID_DOCOMO, mCarrierResolver.getCarrierId());
        assertEquals(NAME_DOCOMO, mCarrierResolver.getCarrierName());
    }

    private class CarrierIdContentProvider extends MockContentProvider {
        @Override
        public Cursor query(Uri uri, String[] projection, String selection, String[] selectionArgs,
                String sortOrder) {
            logd("CarrierIdContentProvider: query");
            logd("   uri = " + uri);
            logd("   projection = " + Arrays.toString(projection));
            logd("   selection = " + selection);
            logd("   selectionArgs = " + Arrays.toString(selectionArgs));
            logd("   sortOrder = " + sortOrder);

            if (CarrierId.All.CONTENT_URI.getAuthority().equals(
                    uri.getAuthority())) {
                MatrixCursor mc = new MatrixCursor(
                        new String[]{CarrierId._ID,
                                CarrierId.All.MCCMNC,
                                CarrierId.All.GID1,
                                CarrierId.All.GID2,
                                CarrierId.All.PLMN,
                                CarrierId.All.IMSI_PREFIX_XPATTERN,
                                CarrierId.All.ICCID_PREFIX,
                                CarrierId.All.PRIVILEGE_ACCESS_RULE,
                                CarrierId.All.SPN,
                                CarrierId.All.APN,
                                CarrierId.CARRIER_NAME,
                                CarrierId.CARRIER_ID,
                                CarrierId.PARENT_CARRIER_ID});

                mc.addRow(new Object[] {
                        1,                      // id
                        MCCMNC,                 // mccmnc
                        null,                   // gid1
                        null,                   // gid2
                        null,                   // plmn
                        null,                   // imsi_prefix
                        null,                   // iccid_prefix
                        null,                   // access rule
                        null,                   // spn
                        null,                   // apn
                        NAME,                   // carrier name
                        CID_VZW,                // cid
                        TelephonyManager.UNKNOWN_CARRIER_ID, // parent cid
                });
                mc.addRow(new Object[] {
                        2,                      // id
                        MCCMNC_TMO,             // mccmnc
                        null,                   // gid1
                        null,                   // gid2
                        null,                   // plmn
                        null,                   // imsi_prefix
                        null,                   // iccid_prefix
                        null,                   // access_rule
                        null,                   // spn
                        null,                   // apn
                        NAME_TMO,               // carrier name
                        CID_TMO,                // cid
                        TelephonyManager.UNKNOWN_CARRIER_ID, // parent cid
                });
                mc.addRow(new Object[] {
                        3,                      // id
                        MCCMNC,                 // mccmnc
                        null,                   // gid1
                        null,                   // gid2
                        null,                   // plmn
                        null,                   // imsi_prefix
                        null,                   // iccid_prefix
                        null,                   // access_rule
                        SPN_FI,                 // spn
                        null,                   // apn
                        NAME_FI,                // carrier name
                        CID_FI,                 // cid
                        TelephonyManager.UNKNOWN_CARRIER_ID, // parent cid
                });
                mc.addRow(new Object[] {
                        4,                      // id
                        MCCMNC,                 // mccmnc
                        null,                   // gid1
                        null,                   // gid2
                        null,                   // plmn
                        null,                   // imsi_prefix
                        null,                   // iccid_prefix
                        null,                   // access_rule
                        SPN_FI,                 // spn
                        null,                   // apn
                        null,                   // carrier name
                        CID_FI,                 // cid
                        TelephonyManager.UNKNOWN_CARRIER_ID, // parent cid
                });
                mc.addRow(new Object[] {
                        5,                      // id
                        MCCMNC,                 // mccmnc
                        null,                   // gid1
                        null,                   // gid2
                        null,                   // plmn
                        null,                   // imsi_prefix
                        null,                   // iccid_prefix
                        null,                   // access_rule
                        null,                   // spn
                        APN_DOCOMO,             // apn
                        NAME_DOCOMO,            // carrier name
                        CID_DOCOMO,             // cid
                        TelephonyManager.UNKNOWN_CARRIER_ID, // parent cid
                });
                mc.addRow(new Object[] {
                        6,                      // id
                        MCCMNC_VODAFONE,        // mccmnc
                        null,                   // gid1
                        null,                   // gid2
                        null,                   // plmn
                        null,                   // imsi_prefix
                        null,                   // iccid_prefix
                        null,                   // access_rule
                        SPN_VODAFONE,           // spn
                        null,                   // apn
                        NAME_VODAFONE,          // carrier name
                        CID_VODAFONE,           // cid
                        TelephonyManager.UNKNOWN_CARRIER_ID, // parent cid
                });
                mc.addRow(new Object[] {
                        7,                      // id
                        MCCMNC_ATT,             // mccmnc
                        null,                   // gid1
                        null,                   // gid2
                        null,                   // plmn
                        null,                   // imsi_prefix
                        null,                   // iccid_prefix
                        null,                   // access_rule
                        null,                   // spn
                        null,                   // apn
                        null,                   // carrier name
                        CID_ATT,                // cid
                        TelephonyManager.UNKNOWN_CARRIER_ID, // parent cid
                });
                mc.addRow(new Object[] {
                        8,                      // id
                        MCCMNC_TRACFONE_ATT,    // mccmnc
                        GID_TRACFONE,           // gid1
                        null,                   // gid2
                        null,                   // plmn
                        null,                   // imsi_prefix
                        null,                   // iccid_prefix
                        null,                   // access_rule
                        null,                   // spn
                        null,                   // apn
                        null,                   // carrier name
                        CID_TRACFONE,           // cid
                        TelephonyManager.UNKNOWN_CARRIER_ID, // parent cid
                });
                mc.addRow(new Object[] {
                        9,                      // id
                        MCCMNC_TRACFONE_TMO,    // mccmnc
                        GID_TRACFONE,           // gid1
                        null,                   // gid2
                        null,                   // plmn
                        null,                   // imsi_prefix
                        null,                   // iccid_prefix
                        null,                   // access_rule
                        null,                   // spn
                        null,                   // apn
                        null,                   // carrier name
                        CID_TRACFONE,           // cid
                        TelephonyManager.UNKNOWN_CARRIER_ID, // parent cid
                });
                mc.addRow(new Object[] {
                        10,                     // id
                        MCCMNC_TRACFONE_ATT,    // mccmnc
                        GID_TRACFONE,           // gid1
                        null,                   // gid2
                        null,                   // plmn
                        null,                   // imsi_prefix
                        null,                   // iccid_prefix
                        null,                   // access_rule
                        null,                   // spn
                        null,                   // apn
                        null,                   // carrier name
                        CID_TRACFONE_ATT,       // cid
                        CID_TRACFONE,           // parent cid
                });
                mc.addRow(new Object[] {
                        11,                     // id
                        MCCMNC_TRACFONE_TMO,    // mccmnc
                        GID_TRACFONE,           // gid1
                        null,                   // gid2
                        null,                   // plmn
                        null,                   // imsi_prefix
                        null,                   // iccid_prefix
                        null,                   // access_rule
                        null,                   // spn
                        null,                   // apn
                        null,                   // carrier name
                        CID_TRACFONE_TMO,       // cid
                        CID_TRACFONE,           // parent cid
                });
                mc.addRow(new Object[] {
                        12,                     // id
                        MCCMNC_O2,              // mccmnc
                        null,                   // gid1
                        null,                   // gid2
                        null,                   // plmn
                        null,                   // imsi_prefix
                        null,                   // iccid_prefix
                        null,                   // access_rule
                        null,                   // spn
                        null,                   // apn
                        null,                   // carrier name
                        CID_O2,                 // cid
                        TelephonyManager.UNKNOWN_CARRIER_ID,  // parent cid
                });
                mc.addRow(new Object[] {
                        13,                     // id
                        MCCMNC_O2,              // mccmnc
                        GID_O2_PREPAID,         // gid1
                        null,                   // gid2
                        null,                   // plmn
                        null,                   // imsi_prefix
                        null,                   // iccid_prefix
                        null,                   // access_rule
                        null,                   // spn
                        null,                   // apn
                        null,                   // carrier name
                        CID_O2_PREPAID,         // cid
                        CID_O2,                 // parent cid
                });
                return mc;
            } else if (Carriers.CONTENT_URI.getAuthority().equals(uri.getAuthority())) {
                MatrixCursor mc = new MatrixCursor(new String[]{Carriers._ID, Carriers.APN});
                mc.addRow(new Object[] {
                        1,                      // id
                        APN_DOCOMO              // apn
                });
                return mc;
            }
            return null;
        }
        @Override
        public int update(android.net.Uri uri, android.content.ContentValues values,
                java.lang.String selection, java.lang.String[] selectionArgs) {
            return 0;
        }
    }
}
