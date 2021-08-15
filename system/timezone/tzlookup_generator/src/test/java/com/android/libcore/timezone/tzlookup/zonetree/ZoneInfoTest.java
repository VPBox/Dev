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

package com.android.libcore.timezone.tzlookup.zonetree;

import com.ibm.icu.text.TimeZoneNames;
import com.ibm.icu.util.BasicTimeZone;
import com.ibm.icu.util.TimeZone;
import com.ibm.icu.util.ULocale;

import org.junit.Test;

import java.time.Instant;
import java.time.temporal.ChronoUnit;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.fail;

public class ZoneInfoTest{

    // 19700101 00:00:00 UTC
    private static final Instant START_INSTANT = Instant.EPOCH;
    // 19900101 00:00:00 UTC - in the past so the data shouldn't change.
    private static final Instant END_INSTANT = Instant.ofEpochSecond(631152000L);

    @Test
    public void testCreate() {
        TimeZoneNames timeZoneNames = TimeZoneNames.getInstance(ULocale.ENGLISH);
        BasicTimeZone londonTz = (BasicTimeZone) TimeZone.getTimeZone("Europe/London");

        // Use a known historical period to determine correctness so it shouldn't change.
        ZoneInfo zoneInfo = ZoneInfo.create(timeZoneNames, londonTz, 5, START_INSTANT, END_INSTANT);

        assertEquals(londonTz.getID(), zoneInfo.getZoneId());
        assertEquals(5, zoneInfo.getPriority());
        int expectedPeriodCount = 38; // This is just the answer.
        assertEquals(expectedPeriodCount, zoneInfo.getZoneOffsetPeriodCount());

        // Sample some of the periods and check they match expectations.
        ZoneOffsetPeriod expectedPeriod0 =
                ZoneOffsetPeriod.create(timeZoneNames, londonTz, START_INSTANT, END_INSTANT);

        assertEquals(Instant.ofEpochSecond(57722400L) /* 1971-10-31T02:00:00Z */,
                expectedPeriod0.getEndInstant());
        assertEquals(expectedPeriod0, zoneInfo.getZoneOffsetPeriod(0));

        ZoneOffsetPeriod expectedPeriod1 =
                ZoneOffsetPeriod.create(timeZoneNames, londonTz,
                        Instant.ofEpochSecond(57722400L) /* 1971-10-31T02:00:00Z */, END_INSTANT);
        assertEquals(Instant.ofEpochSecond(69818400L) /* 1972-03-19T02:00:00Z */,
                expectedPeriod1.getEndInstant());
        assertEquals(expectedPeriod1, zoneInfo.getZoneOffsetPeriod(1));

        ZoneOffsetPeriod expectedPeriod37 =
                ZoneOffsetPeriod.create(timeZoneNames, londonTz,
                        Instant.ofEpochSecond(625626000L) /* 1989-10-29T01:00:00Z */, END_INSTANT);
        assertEquals(END_INSTANT, expectedPeriod37.getEndInstant());
        assertEquals(expectedPeriod37, zoneInfo.getZoneOffsetPeriod(37));
    }

    @Test
    public void testCreateKey() {
        TimeZoneNames timeZoneNames = TimeZoneNames.getInstance(ULocale.ENGLISH);

        // Use a known historical period to determine correctness so it shouldn't change.
        // These two zones are known to be the same for some periods.
        BasicTimeZone denverTz = (BasicTimeZone) TimeZone.getTimeZone("America/Denver");
        ZoneInfo denverZoneInfo =
                ZoneInfo.create(timeZoneNames, denverTz, 5, START_INSTANT, END_INSTANT);
        BasicTimeZone boiseTz = (BasicTimeZone) TimeZone.getTimeZone("America/Boise");
        ZoneInfo boiseZoneInfo =
                ZoneInfo.create(timeZoneNames, boiseTz, 5, START_INSTANT, END_INSTANT);

        // These zones differed initially (1970-01 to 1974-10).
        ZoneOffsetPeriod.ZonePeriodsKey denverPeriodsKey =
                denverZoneInfo.createZonePeriodsKey(0, 10);
        ZoneOffsetPeriod.ZonePeriodsKey boisePeriodsKey =
                boiseZoneInfo.createZonePeriodsKey(0, 10);
        assertFalse(denverPeriodsKey.equals(boisePeriodsKey));

        // But after a while they became the same (1974-10 to 1979 (and beyond)).
        denverPeriodsKey = denverZoneInfo.createZonePeriodsKey(10, 20);
        boisePeriodsKey = boiseZoneInfo.createZonePeriodsKey(10, 20);
        assertEquals(denverPeriodsKey, boisePeriodsKey);
    }

    @Test
    public void testSplit() {
        TimeZoneNames timeZoneNames = TimeZoneNames.getInstance(ULocale.ENGLISH);
        BasicTimeZone denverTz = (BasicTimeZone) TimeZone.getTimeZone("America/Denver");

        ZoneInfo denverZoneInfo1 =
                ZoneInfo.create(timeZoneNames, denverTz, 5, START_INSTANT, END_INSTANT);
        ZoneInfo denverZoneInfo2 =
                ZoneInfo.create(timeZoneNames, denverTz, 5, START_INSTANT, END_INSTANT);
        // Both zone infos start out identical.
        assertEquals(denverZoneInfo1.getZoneOffsetPeriodCount(),
                denverZoneInfo2.getZoneOffsetPeriodCount());

        // Assert that a split instant must be inside the period.
        ZoneOffsetPeriod toSplit = denverZoneInfo2.getZoneOffsetPeriod(5);
        try {
            Instant invalidSplitInstant = toSplit.getStartInstant().minus(1, ChronoUnit.DAYS);
            ZoneInfo.splitZoneOffsetPeriodAtTime(
                    timeZoneNames, denverZoneInfo1, 5, invalidSplitInstant);
            fail();
        } catch (IllegalArgumentException expected) {
        }

        // Try a valid split.
        Instant splitInstant = toSplit.getStartInstant().plus(5, ChronoUnit.DAYS);
        ZoneInfo.splitZoneOffsetPeriodAtTime(timeZoneNames, denverZoneInfo2, 5, splitInstant);

        // The ZoneInfo where the split was made should now have one more ZoneOffsetPeriod.
        assertEquals(denverZoneInfo1.getZoneOffsetPeriodCount() + 1,
                denverZoneInfo2.getZoneOffsetPeriodCount());

        // Extract the two new periods.
        ZoneOffsetPeriod lowerPeriod = denverZoneInfo2.getZoneOffsetPeriod(5);
        ZoneOffsetPeriod upperPeriod = denverZoneInfo2.getZoneOffsetPeriod(6);

        // Confirm the properties of the periods are what we expect.

        // One period was split in two, so check the period instants.
        assertEquals(toSplit.getStartInstant(), lowerPeriod.getStartInstant());
        assertEquals(toSplit.getEndInstant(), upperPeriod.getEndInstant());
        assertEquals(splitInstant, lowerPeriod.getEndInstant());
        assertEquals(splitInstant, upperPeriod.getStartInstant());

        // These properties should just be copied.
        assertEquals(toSplit.getDstOffsetMillis(), lowerPeriod.getDstOffsetMillis());
        assertEquals(toSplit.getDstOffsetMillis(), upperPeriod.getDstOffsetMillis());
        assertEquals(toSplit.getRawOffsetMillis(), lowerPeriod.getRawOffsetMillis());
        assertEquals(toSplit.getRawOffsetMillis(), upperPeriod.getRawOffsetMillis());
        assertEquals(toSplit.getName(), lowerPeriod.getName());

        // This may not be true because the name is recalculated using the start of the upper
        // period, but it happens to be the same for the period being tested.
        assertEquals(toSplit.getName(), upperPeriod.getName());
    }
}
