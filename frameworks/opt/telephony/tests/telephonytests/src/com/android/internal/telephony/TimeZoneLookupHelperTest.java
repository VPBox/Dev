/*
 * Copyright 2017 The Android Open Source Project
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

import com.android.internal.telephony.TimeZoneLookupHelper.CountryResult;
import com.android.internal.telephony.TimeZoneLookupHelper.OffsetResult;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertNull;
import static org.junit.Assert.assertTrue;

import libcore.timezone.TimeZoneFinder;

import org.junit.Before;
import org.junit.Test;

import java.util.Calendar;
import java.util.Date;
import java.util.GregorianCalendar;
import java.util.List;
import java.util.TimeZone;
import java.util.concurrent.TimeUnit;

public class TimeZoneLookupHelperTest {
    // Note: Historical dates are used to avoid the test breaking due to data changes.
    /* Arbitrary summer date in the Northern hemisphere. */
    private static final long NH_SUMMER_TIME_MILLIS = createUtcTime(2015, 6, 20, 1, 2, 3);
    /* Arbitrary winter date in the Northern hemisphere. */
    private static final long NH_WINTER_TIME_MILLIS = createUtcTime(2015, 1, 20, 1, 2, 3);

    private TimeZoneLookupHelper mTimeZoneLookupHelper;

    @Before
    public void setUp() {
        mTimeZoneLookupHelper = new TimeZoneLookupHelper();
    }

    @Test
    public void testLookupByNitzdByNitz() {
        // Historical dates are used to avoid the test breaking due to data changes.
        // However, algorithm updates may change the exact time zone returned, though it shouldn't
        // ever be a less exact match.
        long nhSummerTimeMillis = createUtcTime(2015, 6, 20, 1, 2, 3);
        long nhWinterTimeMillis = createUtcTime(2015, 1, 20, 1, 2, 3);

        String nhSummerTimeString = "15/06/20,01:02:03";
        String nhWinterTimeString = "15/01/20,01:02:03";

        // Tests for London, UK.
        {
            String lonSummerTimeString = nhSummerTimeString + "+4";
            int lonSummerOffsetMillis = (int) TimeUnit.HOURS.toMillis(1);
            int lonSummerDstOffsetMillis = (int) TimeUnit.HOURS.toMillis(1);

            String lonWinterTimeString = nhWinterTimeString + "+0";
            int lonWinterOffsetMillis = 0;
            int lonWinterDstOffsetMillis = 0;

            OffsetResult lookupResult;

            // Summer, known DST state (DST == true).
            NitzData lonSummerNitzDataWithOffset = NitzData.parse(lonSummerTimeString + ",4");
            lookupResult = mTimeZoneLookupHelper.lookupByNitz(lonSummerNitzDataWithOffset);
            assertOffsetResultZoneOffsets(nhSummerTimeMillis, lonSummerOffsetMillis,
                    lonSummerDstOffsetMillis, lookupResult);
            assertOffsetResultMetadata(false, lookupResult);

            // Winter, known DST state (DST == false).
            NitzData lonWinterNitzDataWithOffset = NitzData.parse(lonWinterTimeString + ",0");
            lookupResult = mTimeZoneLookupHelper.lookupByNitz(lonWinterNitzDataWithOffset);
            assertOffsetResultZoneOffsets(nhWinterTimeMillis, lonWinterOffsetMillis,
                    lonWinterDstOffsetMillis, lookupResult);
            assertOffsetResultMetadata(false, lookupResult);

            // Summer, unknown DST state
            NitzData lonSummerNitzDataWithoutOffset = NitzData.parse(lonSummerTimeString);
            lookupResult = mTimeZoneLookupHelper.lookupByNitz(lonSummerNitzDataWithoutOffset);
            assertOffsetResultZoneOffsets(nhSummerTimeMillis, lonSummerOffsetMillis, null,
                    lookupResult);
            assertOffsetResultMetadata(false, lookupResult);

            // Winter, unknown DST state
            NitzData lonWinterNitzDataWithoutOffset = NitzData.parse(lonWinterTimeString);
            lookupResult = mTimeZoneLookupHelper.lookupByNitz(lonWinterNitzDataWithoutOffset);
            assertOffsetResultZoneOffsets(nhWinterTimeMillis, lonWinterOffsetMillis, null,
                    lookupResult);
            assertOffsetResultMetadata(false, lookupResult);
        }

        // Tests for Mountain View, CA, US.
        {
            String mtvSummerTimeString = nhSummerTimeString + "-32";
            int mtvSummerOffsetMillis = (int) TimeUnit.HOURS.toMillis(-8);
            int mtvSummerDstOffsetMillis = (int) TimeUnit.HOURS.toMillis(1);

            String mtvWinterTimeString = nhWinterTimeString + "-28";
            int mtvWinterOffsetMillis = (int) TimeUnit.HOURS.toMillis(-7);
            int mtvWinterDstOffsetMillis = 0;

            OffsetResult lookupResult;

            // Summer, known DST state (DST == true).
            NitzData mtvSummerNitzDataWithOffset = NitzData.parse(mtvSummerTimeString + ",4");
            lookupResult = mTimeZoneLookupHelper.lookupByNitz(mtvSummerNitzDataWithOffset);
            assertOffsetResultZoneOffsets(nhSummerTimeMillis, mtvSummerOffsetMillis,
                    mtvSummerDstOffsetMillis, lookupResult);
            assertOffsetResultMetadata(false, lookupResult);

            // Winter, known DST state (DST == false).
            NitzData mtvWinterNitzDataWithOffset = NitzData.parse(mtvWinterTimeString + ",0");
            lookupResult = mTimeZoneLookupHelper.lookupByNitz(mtvWinterNitzDataWithOffset);
            assertOffsetResultZoneOffsets(nhWinterTimeMillis, mtvWinterOffsetMillis,
                    mtvWinterDstOffsetMillis, lookupResult);
            assertOffsetResultMetadata(false, lookupResult);

            // Summer, unknown DST state
            NitzData mtvSummerNitzDataWithoutOffset = NitzData.parse(mtvSummerTimeString);
            lookupResult = mTimeZoneLookupHelper.lookupByNitz(mtvSummerNitzDataWithoutOffset);
            assertOffsetResultZoneOffsets(nhSummerTimeMillis, mtvSummerOffsetMillis, null,
                    lookupResult);
            assertOffsetResultMetadata(false, lookupResult);

            // Winter, unknown DST state
            NitzData mtvWinterNitzDataWithoutOffset = NitzData.parse(mtvWinterTimeString);
            lookupResult = mTimeZoneLookupHelper.lookupByNitz(mtvWinterNitzDataWithoutOffset);
            assertOffsetResultZoneOffsets(nhWinterTimeMillis, mtvWinterOffsetMillis, null,
                    lookupResult);
            assertOffsetResultMetadata(false, lookupResult);
        }
    }

    @Test
    public void testLookupByNitzCountry() {
        // Historical dates are used to avoid the test breaking due to data changes.
        // However, algorithm updates may change the exact time zone returned, though it shouldn't
        // ever be a less exact match.
        long nhSummerTimeMillis = createUtcTime(2015, 6, 20, 1, 2, 3);
        long nhWinterTimeMillis = createUtcTime(2015, 1, 20, 1, 2, 3);

        // Two countries in the northern hemisphere that share the same Winter and Summer DST
        // offsets at the dates being used.
        String deIso = "DE"; // Germany
        String adIso = "AD"; // Andora
        String summerTimeNitzString = "15/06/20,01:02:03+8";
        String winterTimeNitzString = "15/01/20,01:02:03+4";

        // Summer, known DST state (DST == true).
        {
            String summerTimeNitzStringWithDst = summerTimeNitzString + ",1";
            NitzData nitzData = NitzData.parse(summerTimeNitzStringWithDst);
            int expectedUtcOffset = (int) TimeUnit.HOURS.toMillis(2);
            Integer expectedDstOffset = (int) TimeUnit.HOURS.toMillis(1);
            assertEquals(expectedUtcOffset, nitzData.getLocalOffsetMillis());
            assertEquals(expectedDstOffset, nitzData.getDstAdjustmentMillis());

            OffsetResult expectedResult;

            OffsetResult deSummerWithDstResult =
                    mTimeZoneLookupHelper.lookupByNitzCountry(nitzData, deIso);
            expectedResult = new OffsetResult("Europe/Berlin", false /* isOnlyMatch */);
            assertEquals(expectedResult, deSummerWithDstResult);
            assertOffsetResultZoneOffsets(nhSummerTimeMillis, expectedUtcOffset, expectedDstOffset,
                    deSummerWithDstResult);

            OffsetResult adSummerWithDstResult =
                    mTimeZoneLookupHelper.lookupByNitzCountry(nitzData, adIso);
            expectedResult = new OffsetResult("Europe/Andorra", true /* isOnlyMatch */);
            assertEquals(expectedResult, adSummerWithDstResult);
            assertOffsetResultZoneOffsets(nhSummerTimeMillis, expectedUtcOffset, expectedDstOffset,
                    adSummerWithDstResult);
            assertOffsetResultZoneCountry(adIso, adSummerWithDstResult);
        }

        // Winter, known DST state (DST == false)
        {
            String winterTimeNitzStringWithDst = winterTimeNitzString + ",0";
            NitzData nitzData = NitzData.parse(winterTimeNitzStringWithDst);
            int expectedUtcOffset = (int) TimeUnit.HOURS.toMillis(1);
            Integer expectedDstOffset = 0;
            assertEquals(expectedUtcOffset, nitzData.getLocalOffsetMillis());
            assertEquals(expectedDstOffset, nitzData.getDstAdjustmentMillis());

            OffsetResult expectedResult;

            OffsetResult deWinterWithDstResult =
                    mTimeZoneLookupHelper.lookupByNitzCountry(nitzData, deIso);
            expectedResult = new OffsetResult("Europe/Berlin", false /* isOnlyMatch */);
            assertEquals(expectedResult, deWinterWithDstResult);
            assertOffsetResultZoneOffsets(nhWinterTimeMillis, expectedUtcOffset, expectedDstOffset,
                    deWinterWithDstResult);

            OffsetResult adWinterWithDstResult =
                    mTimeZoneLookupHelper.lookupByNitzCountry(nitzData, adIso);
            expectedResult = new OffsetResult("Europe/Andorra", true /* isOnlyMatch */);
            assertEquals(expectedResult, adWinterWithDstResult);
            assertOffsetResultZoneOffsets(nhWinterTimeMillis, expectedUtcOffset, expectedDstOffset,
                    adWinterWithDstResult);
        }

        // Summer, unknown DST state
        // For historic reasons, GuessZoneIdByNitzCountry() does not handle unknown DST state - it
        // assumes that "unknown DST" means "no DST": This leads to no match when DST is actually in
        // force.
        {
            NitzData nitzData = NitzData.parse(summerTimeNitzString);
            int expectedUtcOffset = (int) TimeUnit.HOURS.toMillis(2);
            Integer expectedDstOffset = null; // Unknown
            assertEquals(expectedUtcOffset, nitzData.getLocalOffsetMillis());
            assertEquals(expectedDstOffset, nitzData.getDstAdjustmentMillis());

            OffsetResult deSummerUnknownDstResult =
                    mTimeZoneLookupHelper.lookupByNitzCountry(nitzData, deIso);
            assertNull(deSummerUnknownDstResult);

            OffsetResult adSummerUnknownDstResult =
                    mTimeZoneLookupHelper.lookupByNitzCountry(nitzData, adIso);
            assertNull(adSummerUnknownDstResult);
        }

        // Winter, unknown DST state
        {
            NitzData nitzData = NitzData.parse(winterTimeNitzString);
            int expectedUtcOffset = (int) TimeUnit.HOURS.toMillis(1);
            Integer expectedDstOffset = null; // Unknown
            assertEquals(expectedUtcOffset, nitzData.getLocalOffsetMillis());
            assertEquals(expectedDstOffset, nitzData.getDstAdjustmentMillis());

            OffsetResult expectedResult;

            OffsetResult deWinterUnknownDstResult =
                    mTimeZoneLookupHelper.lookupByNitzCountry(nitzData, deIso);
            expectedResult = new OffsetResult("Europe/Berlin", false /* isOnlyMatch */);
            assertEquals(expectedResult, deWinterUnknownDstResult);
            assertOffsetResultZoneOffsets(nhWinterTimeMillis, expectedUtcOffset, expectedDstOffset,
                    deWinterUnknownDstResult);

            OffsetResult adWinterUnknownDstResult =
                    mTimeZoneLookupHelper.lookupByNitzCountry(nitzData, adIso);
            expectedResult = new OffsetResult("Europe/Andorra", true /* isOnlyMatch */);
            assertEquals(expectedResult, adWinterUnknownDstResult);
            assertOffsetResultZoneOffsets(nhWinterTimeMillis, expectedUtcOffset, expectedDstOffset,
                    adWinterUnknownDstResult);
        }
    }

    @Test
    public void testLookupByCountry() {
        // Historical dates are used to avoid the test breaking due to data changes.
        long nhSummerTimeMillis = createUtcTime(2015, 6, 20, 1, 2, 3);
        long nhWinterTimeMillis = createUtcTime(2015, 1, 20, 1, 2, 3);

        CountryResult expectedResult;

        // GB has one time zone.
        expectedResult = new CountryResult("Europe/London", true /* allZonesHaveSameOffset */,
                nhSummerTimeMillis);
        assertEquals(expectedResult,
                mTimeZoneLookupHelper.lookupByCountry("gb", nhSummerTimeMillis));
        expectedResult = new CountryResult("Europe/London", true /* allZonesHaveSameOffset */,
                nhWinterTimeMillis);
        assertEquals(expectedResult,
                mTimeZoneLookupHelper.lookupByCountry("gb", nhWinterTimeMillis));

        // DE has two time zones according to data, but they agree on offset.
        expectedResult = new CountryResult("Europe/Berlin", true /* allZonesHaveSameOffset */,
                nhSummerTimeMillis);
        assertEquals(expectedResult,
                mTimeZoneLookupHelper.lookupByCountry("de", nhSummerTimeMillis));
        expectedResult = new CountryResult("Europe/Berlin", true /* allZonesHaveSameOffset */,
                nhWinterTimeMillis);
        assertEquals(expectedResult,
                mTimeZoneLookupHelper.lookupByCountry("de", nhWinterTimeMillis));

        // US has many time zones that have different offsets.
        expectedResult = new CountryResult("America/New_York", false /* allZonesHaveSameOffset */,
                nhSummerTimeMillis);
        assertEquals(expectedResult,
                mTimeZoneLookupHelper.lookupByCountry("us", nhSummerTimeMillis));
        expectedResult = new CountryResult("America/New_York", false /* allZonesHaveSameOffset */,
                nhWinterTimeMillis);
        assertEquals(expectedResult,
                mTimeZoneLookupHelper.lookupByCountry("us", nhWinterTimeMillis));
    }

    @Test
    public void testCountryUsesUtc() {
        assertFalse(mTimeZoneLookupHelper.countryUsesUtc("us", NH_SUMMER_TIME_MILLIS));
        assertFalse(mTimeZoneLookupHelper.countryUsesUtc("us", NH_WINTER_TIME_MILLIS));
        assertFalse(mTimeZoneLookupHelper.countryUsesUtc("gb", NH_SUMMER_TIME_MILLIS));
        assertTrue(mTimeZoneLookupHelper.countryUsesUtc("gb", NH_WINTER_TIME_MILLIS));
    }

    private static void assertOffsetResultZoneCountry(
            String isoCountryCode, OffsetResult lookupResult) {
        String timeZoneId = lookupResult.zoneId;
        List<String> zoneIdsByCountry =
                TimeZoneFinder.getInstance().lookupTimeZoneIdsByCountry(isoCountryCode);
        assertTrue(timeZoneId + " must be used in " + isoCountryCode,
                zoneIdsByCountry.contains(timeZoneId));
    }

    /**
     * Assert the time zone in the OffsetResult has the expected properties at the specified time.
     */
    private static void assertOffsetResultZoneOffsets(long time, int expectedOffsetAtTime,
            Integer expectedDstAtTime, OffsetResult lookupResult) {

        TimeZone timeZone = TimeZone.getTimeZone(lookupResult.zoneId);
        GregorianCalendar calendar = new GregorianCalendar(timeZone);
        calendar.setTimeInMillis(time);
        int actualOffsetAtTime =
                calendar.get(Calendar.ZONE_OFFSET) + calendar.get(Calendar.DST_OFFSET);
        assertEquals(expectedOffsetAtTime, actualOffsetAtTime);

        if (expectedDstAtTime != null) {
            Date date = new Date(time);
            assertEquals(expectedDstAtTime > 0, timeZone.inDaylightTime(date));

            // The code under test assumes DST means +1 in all cases,
            // This code makes fewer assumptions.
            assertEquals(expectedDstAtTime.intValue(), calendar.get(Calendar.DST_OFFSET));
        }
    }

    private static void assertOffsetResultMetadata(boolean isOnlyMatch, OffsetResult lookupResult) {
        assertEquals(isOnlyMatch, lookupResult.isOnlyMatch);
    }

    private static long createUtcTime(
            int year, int monthOfYear, int dayOfMonth, int hourOfDay, int minute, int second) {
        GregorianCalendar calendar = new GregorianCalendar(TimeZone.getTimeZone("UTC"));
        calendar.clear(); // Clear millis, etc.
        calendar.set(year, monthOfYear - 1, dayOfMonth, hourOfDay, minute, second);
        return calendar.getTimeInMillis();
    }
}
