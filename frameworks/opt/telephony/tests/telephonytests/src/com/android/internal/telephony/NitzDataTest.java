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

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertNull;

import org.junit.Test;

import java.util.GregorianCalendar;
import java.util.TimeZone;
import java.util.concurrent.TimeUnit;

public class NitzDataTest {

    @Test
    public void testParse_dateOutsideAllowedRange() {
        assertNull(NitzData.parse("38/06/20,00:00:00+0"));
    }

    @Test
    public void testParse_missingRequiredFields() {
        // "yy/mm/dd,hh:mm:ss(+/-)tz[,dt[,tzid]]"

        // No tz field.
        assertNull(NitzData.parse("38/06/20,00:00:00"));
    }

    @Test
    public void testParse_withDst() {
        // "yy/mm/dd,hh:mm:ss(+/-)tz[,dt[,tzid]]"
        // tz, dt are in number of quarter-hours
        {
            NitzData nitz = NitzData.parse("15/06/20,01:02:03-1,0");
            assertEquals(createUtcTime(2015, 6, 20, 1, 2, 3), nitz.getCurrentTimeInMillis());
            assertEquals(TimeUnit.MINUTES.toMillis(-1 * 15), nitz.getLocalOffsetMillis());
            assertEquals(0, nitz.getDstAdjustmentMillis().longValue());
            assertNull(nitz.getEmulatorHostTimeZone());
        }
        {
            NitzData nitz = NitzData.parse("15/06/20,01:02:03+8,1");
            assertEquals(createUtcTime(2015, 6, 20, 1, 2, 3), nitz.getCurrentTimeInMillis());
            assertEquals(TimeUnit.MINUTES.toMillis(8 * 15), nitz.getLocalOffsetMillis());
            assertEquals(TimeUnit.HOURS.toMillis(1), nitz.getDstAdjustmentMillis().longValue());
            assertNull(nitz.getEmulatorHostTimeZone());
        }
        {
            NitzData nitz = NitzData.parse("15/06/20,01:02:03-8,1");
            assertEquals(createUtcTime(2015, 6, 20, 1, 2, 3), nitz.getCurrentTimeInMillis());
            assertEquals(TimeUnit.MINUTES.toMillis(-8 * 15), nitz.getLocalOffsetMillis());
            assertEquals(TimeUnit.HOURS.toMillis(1), nitz.getDstAdjustmentMillis().longValue());
            assertNull(nitz.getEmulatorHostTimeZone());
        }
    }

    @Test
    public void testParse_noDstField() {
        {
            NitzData nitz = NitzData.parse("15/06/20,01:02:03+4");
            assertEquals(createUtcTime(2015, 6, 20, 1, 2, 3), nitz.getCurrentTimeInMillis());
            assertEquals(TimeUnit.MINUTES.toMillis(4 * 15), nitz.getLocalOffsetMillis());
            assertNull(nitz.getDstAdjustmentMillis());
            assertNull(nitz.getEmulatorHostTimeZone());
        }
        {
            NitzData nitz = NitzData.parse("15/06/20,01:02:03-4");
            assertEquals(createUtcTime(2015, 6, 20, 1, 2, 3), nitz.getCurrentTimeInMillis());
            assertEquals(TimeUnit.MINUTES.toMillis(-4 * 15), nitz.getLocalOffsetMillis());
            assertNull(nitz.getDstAdjustmentMillis());
            assertNull(nitz.getEmulatorHostTimeZone());
        }
    }

    @Test
    public void testParse_androidEmulatorTimeZoneExtension() {
        NitzData nitz = NitzData.parse("15/06/20,01:02:03-32,1,America!Los_Angeles");
        assertEquals(createUtcTime(2015, 6, 20, 1, 2, 3), nitz.getCurrentTimeInMillis());
        assertEquals(TimeUnit.MINUTES.toMillis(-32 * 15), nitz.getLocalOffsetMillis());
        assertEquals(TimeUnit.HOURS.toMillis(1), nitz.getDstAdjustmentMillis().longValue());
        assertEquals("America/Los_Angeles", nitz.getEmulatorHostTimeZone().getID());
    }

    @Test
    public void testToString() {
        assertNotNull(NitzData.parse("15/06/20,01:02:03-32").toString());
        assertNotNull(NitzData.parse("15/06/20,01:02:03-32,4").toString());
        assertNotNull(NitzData.parse("15/06/20,01:02:03-32,4,America!Los_Angeles")
                .toString());
    }

    private static long createUtcTime(
            int year, int monthOfYear, int dayOfMonth, int hourOfDay, int minute, int second) {
        GregorianCalendar calendar = new GregorianCalendar(TimeZone.getTimeZone("UTC"));
        calendar.clear(); // Clear millis, etc.
        calendar.set(year, monthOfYear - 1, dayOfMonth, hourOfDay, minute, second);
        return calendar.getTimeInMillis();
    }
}
