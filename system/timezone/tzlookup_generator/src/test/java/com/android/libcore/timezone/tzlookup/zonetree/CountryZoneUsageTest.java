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

import org.junit.Test;

import java.time.Instant;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;
import static org.junit.Assert.fail;

public class CountryZoneUsageTest {

    @Test
    public void testGetIsCode() {
        CountryZoneUsage countryZoneUsage = new CountryZoneUsage("us");
        assertEquals("us", countryZoneUsage.getIsoCode());
    }

    @Test
    public void testNoEntry() {
        CountryZoneUsage countryZoneUsage = new CountryZoneUsage("us");
        String missingZoneId = "Europe/London";
        assertFalse(countryZoneUsage.hasEntry(missingZoneId));
        try {
            countryZoneUsage.getNotUsedAfterInstant(missingZoneId);
            fail();
        } catch (IllegalArgumentException expected) {
        }
    }

    @Test
    public void testWithEntry() {
        CountryZoneUsage countryZoneUsage = new CountryZoneUsage("us");
        String usZoneId = "America/Boise";
        Instant instant = Instant.ofEpochSecond(1234);
        countryZoneUsage.addEntry(usZoneId, instant);

        assertTrue(countryZoneUsage.hasEntry(usZoneId));
        assertEquals(instant, countryZoneUsage.getNotUsedAfterInstant(usZoneId));

        try {
            countryZoneUsage.addEntry(usZoneId, instant);
            fail();
        } catch (IllegalArgumentException expected) {
        }
    }
}
