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

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;

public class ZoneOffsetPeriodTest {

    @Test
    public void testEqualPeriods_withDstTransitions() {
        TimeZoneNames timeZoneNames = TimeZoneNames.getInstance(ULocale.ENGLISH);

        // These two zones have DST and have had the same rules since 1974-10-27.
        BasicTimeZone denverTz = (BasicTimeZone) TimeZone.getTimeZone("America/Denver");
        BasicTimeZone boiseTz = (BasicTimeZone) TimeZone.getTimeZone("America/Boise");
        Instant startInstant = Instant.ofEpochSecond(152092800L); /* 1974-10-27T08:00:00Z */
        Instant expectedPeriod0End = Instant.ofEpochSecond(162378000L); /* 1975-02-23T09:00:00Z */
        // endInstant is an arbitrary "ceiling" time value for a period if there are no transitions.
        // It just needs to be after the period we are testing and does not influence the test.
        Instant endInstant = Instant.ofEpochSecond(631152000L); /* 1990-01-01T00:00:00Z */
        {
            // Request periods from both zones starting at startInstant. They should be the same.
            int expectedPeriod0RawOffset = -25200000;
            int expectedPeriod0DstOffset = 0;
            String expectedPeriod0Name = "Mountain Standard Time";

            ZoneOffsetPeriod denverPeriod0 =
                    ZoneOffsetPeriod.create(timeZoneNames, denverTz, startInstant, endInstant);
            assertEquals(startInstant, denverPeriod0.getStartInstant());
            assertEquals(expectedPeriod0End, denverPeriod0.getEndInstant());
            assertEquals(expectedPeriod0RawOffset, denverPeriod0.getRawOffsetMillis());
            assertEquals(expectedPeriod0DstOffset, denverPeriod0.getDstOffsetMillis());
            assertEquals(expectedPeriod0Name, denverPeriod0.getName());

            ZoneOffsetPeriod boisePeriod0 =
                    ZoneOffsetPeriod.create(timeZoneNames, boiseTz, startInstant, endInstant);
            assertEquals(startInstant, boisePeriod0.getStartInstant());
            assertEquals(expectedPeriod0End, boisePeriod0.getEndInstant());
            assertEquals(expectedPeriod0RawOffset, boisePeriod0.getRawOffsetMillis());
            assertEquals(expectedPeriod0DstOffset, boisePeriod0.getDstOffsetMillis());
            assertEquals(expectedPeriod0Name, boisePeriod0.getName());

            assertEquals(denverPeriod0, boisePeriod0);
        }

        {
            // Request two other periods from both zones starting at expectedPeriod0End.
            // They should be the same.
            Instant expectedPeriod1Start = expectedPeriod0End;
            Instant expectedPeriod1End =
                    Instant.ofEpochSecond(183542400L); /* 1975-10-26T08:00:00Z */
            int expectedPeriod1RawOffset = -25200000;
            int expectedPeriod1DstOffset = 3600000;
            String expectedPeriod1Name = "Mountain Daylight Time";

            ZoneOffsetPeriod denverPeriod1 = ZoneOffsetPeriod.create(
                    timeZoneNames, denverTz, expectedPeriod1Start, endInstant);
            assertEquals(expectedPeriod1Start, denverPeriod1.getStartInstant());
            assertEquals(expectedPeriod1End, denverPeriod1.getEndInstant());
            assertEquals(expectedPeriod1RawOffset, denverPeriod1.getRawOffsetMillis());
            assertEquals(expectedPeriod1DstOffset, denverPeriod1.getDstOffsetMillis());
            assertEquals(expectedPeriod1Name, denverPeriod1.getName());

            ZoneOffsetPeriod boisePeriod1 = ZoneOffsetPeriod.create(
                    timeZoneNames, boiseTz, expectedPeriod1Start, endInstant);
            assertEquals(expectedPeriod1Start, boisePeriod1.getStartInstant());
            assertEquals(expectedPeriod1End, boisePeriod1.getEndInstant());
            assertEquals(expectedPeriod1End, boisePeriod1.getEndInstant());
            assertEquals(expectedPeriod1RawOffset, boisePeriod1.getRawOffsetMillis());
            assertEquals(expectedPeriod1DstOffset, boisePeriod1.getDstOffsetMillis());
            assertEquals(expectedPeriod1Name, boisePeriod1.getName());

            assertEquals(denverPeriod1, boisePeriod1);
        }
    }

    @Test
    public void testEqualPeriods_withoutDstTransitions() {
        TimeZoneNames timeZoneNames = TimeZoneNames.getInstance(ULocale.ENGLISH);

        Instant startInstant = Instant.EPOCH; /* 1970-01-01T00:00:00Z */
        // endInstant is an arbitrary "ceiling" time value for a period if there are no transitions.
        // Since there are no transitions this will be the end of the period.
        Instant endInstant = Instant.ofEpochSecond(631152000L); /* 1990-01-01T00:00:00Z */

        // These two zones have not observed DST but have different offsets and names for their
        // local time.
        BasicTimeZone honoluluTz = (BasicTimeZone) TimeZone.getTimeZone("Pacific/Honolulu");
        int honoluluRawOffset = -36000000;
        int honoluluDstOffset = 0;
        // ICU doesn't have a name for this zone for 1970.
        String honoluluName = null;
        ZoneOffsetPeriod honoluluPeriod =
                ZoneOffsetPeriod.create(timeZoneNames, honoluluTz, startInstant, endInstant);
        assertEquals(startInstant, honoluluPeriod.getStartInstant());
        assertEquals(endInstant, honoluluPeriod.getEndInstant());
        assertEquals(honoluluRawOffset, honoluluPeriod.getRawOffsetMillis());
        assertEquals(honoluluDstOffset, honoluluPeriod.getDstOffsetMillis());
        assertEquals(honoluluName, honoluluPeriod.getName());

        BasicTimeZone phoenixTz = (BasicTimeZone) TimeZone.getTimeZone("America/Phoenix");
        int phoenixRawOffset = -25200000;
        int phoenixDstOffset = 0;
        String phoenixName = "Mountain Standard Time";
        ZoneOffsetPeriod phoenixPeriod =
                ZoneOffsetPeriod.create(timeZoneNames, phoenixTz, startInstant, endInstant);
        assertEquals(startInstant, phoenixPeriod.getStartInstant());
        assertEquals(endInstant, phoenixPeriod.getEndInstant());
        assertEquals(phoenixRawOffset, phoenixPeriod.getRawOffsetMillis());
        assertEquals(phoenixDstOffset, phoenixPeriod.getDstOffsetMillis());
        assertEquals(phoenixName, phoenixPeriod.getName());

        assertFalse(honoluluPeriod.equals(phoenixPeriod));
    }

    @Test
    public void testSplitAtTime() {
        TimeZoneNames timeZoneNames = TimeZoneNames.getInstance(ULocale.ENGLISH);

        // This zone does not observe DST.
        BasicTimeZone honoluluTz = (BasicTimeZone) TimeZone.getTimeZone("Pacific/Honolulu");
        int honoluluRawOffset = -36000000;
        int honoluluDstOffset = 0;
        // ICU doesn't have a display name for the zone in 1970....
        String honoluluOldName = null;
        // ... but it does in 1990.
        String honoluluNewName = "Hawaii-Aleutian Standard Time";

        Instant startInstant = Instant.EPOCH; /* 1970-01-01T00:00:00Z */
        // endInstant is an arbitrary "ceiling" time value for a period if there are no transitions.
        // Since there are no transitions for the zone this will be the end of the period.
        Instant endInstant = Instant.ofEpochSecond(631152000L); /* 1990-01-01T00:00:00Z */

        ZoneOffsetPeriod honoluluPeriod =
                ZoneOffsetPeriod.create(timeZoneNames, honoluluTz, startInstant, endInstant);

        // Split at an arbitrary point.
        Instant partitionInstant = Instant.ofEpochSecond(500000000L);
        ZoneOffsetPeriod[] shards = ZoneOffsetPeriod.splitAtTime(
                honoluluPeriod, timeZoneNames, honoluluTz, partitionInstant);

        // Check the properties.
        assertEquals(startInstant, shards[0].getStartInstant());
        assertEquals(partitionInstant, shards[0].getEndInstant());
        assertEquals(honoluluRawOffset, shards[0].getRawOffsetMillis());
        assertEquals(honoluluDstOffset, shards[0].getDstOffsetMillis());
        assertEquals(honoluluOldName, shards[0].getName());

        assertEquals(partitionInstant, shards[1].getStartInstant());
        assertEquals(endInstant, shards[1].getEndInstant());
        assertEquals(honoluluRawOffset, shards[1].getRawOffsetMillis());
        assertEquals(honoluluDstOffset, shards[1].getDstOffsetMillis());
        assertEquals(honoluluNewName, shards[1].getName());
    }
}