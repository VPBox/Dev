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

import android.database.Cursor;
import android.database.MatrixCursor;
import android.net.Uri;
import android.telephony.CarrierConfigManager;
import android.telephony.TelephonyManager;
import android.test.mock.MockContentProvider;
import android.test.mock.MockContentResolver;
import android.test.suitebuilder.annotation.SmallTest;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import java.util.Arrays;

import static org.junit.Assert.assertEquals;
import static org.mockito.Matchers.anyInt;
import static org.mockito.Mockito.doReturn;

public class SmsNumberUtilsTest extends TelephonyTest {

    private static final String TMO_MCC_MNC = "310260";
    private static final String TAIWAN_FET_MCC_MNC = "46602";
    private static final String INDIA_AIRTEL_MCC_MNC = "40553";
    private static final String JAPAN_NTTDOCOMO_MCC_MNC = "44020";


    // Simulate partial packages/providers/TelephonyProvider/res/xml/hbpcd_lookup_tables.xml
    private class HbpcdContentProvider extends MockContentProvider {

        public HbpcdContentProvider() {}

        @Override
        public Cursor query(Uri uri, String[] projection, String selection, String[] selectionArgs,
                            String sortOrder) {
            logd("HbpcdContentProvider: query");
            logd("   uri = " + uri);
            logd("   projection = " + Arrays.toString(projection));
            logd("   selection = " + selection);
            logd("   selectionArgs = " + Arrays.toString(selectionArgs));
            logd("   sortOrder = " + sortOrder);

            if (uri.compareTo(HbpcdLookup.MccIdd.CONTENT_URI) == 0) {
                if (projection.length == 2 && projection[0].equals(HbpcdLookup.MccIdd.IDD) &&
                        projection[1].equals(HbpcdLookup.MccIdd.MCC) && selectionArgs.length == 1) {
                    MatrixCursor mc = new MatrixCursor(
                            new String[]{HbpcdLookup.MccIdd.IDD});

                    switch (Integer.parseInt(selectionArgs[0])) {
                        case 310:
                            mc.addRow(new Object[]{"011"}); // US IDD code
                            break;
                        case 466:
                            mc.addRow(new Object[]{"002"}); // Taiwan IDD code
                            break;
                        case 440:
                            mc.addRow(new Object[]{"010"}); // Japan IDD code
                            break;
                        case 405:
                            mc.addRow(new Object[]{"010"}); // India IDD code
                            break;
                        default:
                            logd("Unhandled MCC" + Integer.parseInt(selectionArgs[0]));
                    }

                    return mc;
                } else {
                    logd("Unhandled IDD look up request.");
                }
            } else if (uri.compareTo(HbpcdLookup.MccLookup.CONTENT_URI) == 0) {
                if (projection.length == 1 && projection[0].equals(
                        HbpcdLookup.MccLookup.COUNTRY_CODE) &&
                        selection == null && selectionArgs == null) {
                    MatrixCursor mccCursor = new MatrixCursor(
                            new String[]{HbpcdLookup.MccLookup.COUNTRY_CODE});

                    mccCursor.addRow(new Object[]{"1"});
                    mccCursor.addRow(new Object[]{"886"});
                    mccCursor.addRow(new Object[]{"81"});
                    mccCursor.addRow(new Object[]{"91"});

                    return mccCursor;
                } else {
                    logd("Unhandled mcc look up request");
                }

            } else {
                logd("Unknown URI: " + uri);
            }

            return null;
        }
    }

    HbpcdContentProvider mHbpcdContentProvider;

    @Before
    public void setUp() throws Exception {
        logd("SmsNumberUtilsTest +Setup!");
        super.setUp(getClass().getSimpleName());

        mHbpcdContentProvider = new HbpcdContentProvider();

        doReturn(TMO_MCC_MNC).when(mTelephonyManager).getNetworkOperator(anyInt());

        ((MockContentResolver) mContextFixture.getTestDouble().getContentResolver())
                .addProvider(HbpcdLookup.MccIdd.CONTENT_URI.getAuthority(), mHbpcdContentProvider);
        mContextFixture.getCarrierConfigBundle().
                putBoolean(CarrierConfigManager.KEY_SMS_REQUIRES_DESTINATION_NUMBER_CONVERSION_BOOL,
                        true);

        logd("SmsNumberUtilsTest -Setup!");
    }

    @After
    public void tearDown() throws Exception {
        super.tearDown();
    }

    @Test
    @SmallTest
    public void testInvalidNumberConversion() {
        assertEquals("123", SmsNumberUtils.filterDestAddr(mPhone, "123"));
    }

    @Test
    @SmallTest
    public void testNaPcCountryCodeAreaLocalNumberConversion() {
        // NP_NANP_NBPCD_CC_AREA_LOCAL tests
        doReturn(PhoneConstants.PHONE_TYPE_CDMA).when(mPhone).getPhoneType();
        assertEquals("18583420022", SmsNumberUtils.filterDestAddr(mPhone, "+1-858-342-0022"));
    }

    @Test
    @SmallTest
    public void testPcCountryCodeAreaLocalNumberConversion() {
        // NP_NBPCD_CC_AREA_LOCAL tests
        assertEquals("01188671234567", SmsNumberUtils.filterDestAddr(mPhone, "+886-7-1234567"));
    }

    @Test
    @SmallTest
    public void testIndiaPcCountryCodeAreaLocalNumberConversion() {
        // NP_NBPCD_CC_AREA_LOCAL tests
        doReturn(INDIA_AIRTEL_MCC_MNC).when(mTelephonyManager).getNetworkOperator(anyInt());
        assertEquals("0119172345678", SmsNumberUtils.filterDestAddr(mPhone, "+91-7-234-5678"));
    }

    @Test
    @SmallTest
    public void testPcHomeIddCountryCodeAreaLocalNumberConversion() {
        // NP_NBPCD_HOMEIDD_CC_AREA_LOCAL tests
        assertEquals("01188671234567", SmsNumberUtils.filterDestAddr(mPhone, "+011886-7-1234567"));
    }

    @Test
    @SmallTest
    public void testHomeIddCountryCodeAreaLocalNumberConversion() {
        // NP_HOMEIDD_CC_AREA_LOCAL tests
        assertEquals("01188671234567", SmsNumberUtils.filterDestAddr(mPhone, "011886-7-1234567"));
    }

    @Test
    @SmallTest
    public void testLocalIddCountryCodeAreaLocalNumberConversion() {
        // NP_LOCALIDD_CC_AREA_LOCAL tests
        doReturn(TAIWAN_FET_MCC_MNC).when(mTelephonyManager).getNetworkOperator(anyInt());
        assertEquals("01118581234567", SmsNumberUtils.filterDestAddr(mPhone, "002-1-858-1234567"));
    }

    @Test
    @SmallTest
    public void testIndiaLocalIddCountryCodeAreaLocalNumberConversion() {
        // NP_LOCALIDD_CC_AREA_LOCAL tests
        doReturn(INDIA_AIRTEL_MCC_MNC).when(mTelephonyManager).getNetworkOperator(anyInt());
        assertEquals("01118581234567", SmsNumberUtils.filterDestAddr(mPhone, "010-1-858-1234567"));
    }

    @Test
    @SmallTest
    public void testJapanLocalIddCountryCodeAreaLocalNumberConversion() {
        // NP_LOCALIDD_CC_AREA_LOCAL tests
        doReturn(JAPAN_NTTDOCOMO_MCC_MNC).when(mTelephonyManager).getNetworkOperator(anyInt());
        assertEquals("01118581234567", SmsNumberUtils.filterDestAddr(mPhone, "010-1-858-1234567"));
    }

    @Test
    @SmallTest
    public void testCountryCodeAreaLocalNumberConversion() {
        // NP_CC_AREA_LOCAL tests
        assertEquals("011886286281234", SmsNumberUtils.filterDestAddr(mPhone, "886-2-86281234"));
    }

    @Test
    @SmallTest
    public void testNaLocalNumberConversion() {
        // NP_NANP_LOCAL
        assertEquals("2345678", SmsNumberUtils.filterDestAddr(mPhone, "234-5678"));
    }

    @Test
    @SmallTest
    public void testNaAreaLocalNumberConversion() {
        // NP_NANP_AREA_LOCAL
        assertEquals("8582345678", SmsNumberUtils.filterDestAddr(mPhone, "858-234-5678"));
    }

    @Test
    @SmallTest
    public void testNaNddAreaLocalNumberConversion() {
        // NP_NANP_NDD_AREA_LOCAL
        assertEquals("18582345678", SmsNumberUtils.filterDestAddr(mPhone, "1-858-234-5678"));
    }

    @Test
    @SmallTest
    public void testNaLocalIddCcAreaLocalNumberConversion() {
        // NP_NANP_LOCALIDD_CC_AREA_LOCAL
        assertEquals("+18582345678", SmsNumberUtils.filterDestAddr(mPhone, "011-1-858-234-5678"));
    }

    @Test
    @SmallTest
    public void testNaPcHomeIddCcAreaLocalNumberConversion() {
        // NP_NANP_NBPCD_HOMEIDD_CC_AREA_LOCAL
        assertEquals("01118582345678",
                SmsNumberUtils.filterDestAddr(mPhone, "+011-1-858-234-5678"));
    }
}
