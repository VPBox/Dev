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

import com.google.protobuf.TextFormat;

import org.junit.Test;

import java.time.Instant;
import java.time.temporal.ChronoUnit;

import static com.android.libcore.timezone.tzlookup.proto.CountryZonesFile.Country;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertNull;
import static org.junit.Assert.assertTrue;
import static org.junit.Assert.fail;

public class CountryZoneTreeTest {

    // 19700101 00:00:00 UTC
    private static final Instant RULES_START_INSTANT = Instant.EPOCH;
    // 20000101 00:00:00 UTC - in the past so the data shouldn't change.
    private static final Instant NOT_USED_AFTER_CUT_OFF = Instant.ofEpochSecond(946684800L);
    // 20020101-ish - in the past so the data shouldn't change.
    private static final Instant RULES_END_INSTANT =
            NOT_USED_AFTER_CUT_OFF.plus(2 * 365, ChronoUnit.DAYS);

    @Test
    public void testSimpleCountry() throws Exception {
        String countryText = "  isoCode:\"ad\"\n"
                + "  timeZoneMappings:<\n"
                + "    utcOffset:\"1:00\"\n"
                + "    id:\"Europe/Andorra\"\n"
                + "  >\n";
        Country country = parseCountry(countryText);
        CountryZoneTree zoneTree =
                CountryZoneTree.create(country, RULES_START_INSTANT, RULES_END_INSTANT);
        assertTrue(zoneTree.validateNoPriorityClashes().isEmpty());
        CountryZoneUsage zoneUsage = zoneTree.calculateCountryZoneUsage(NOT_USED_AFTER_CUT_OFF);
        assertNull(zoneUsage.getNotUsedAfterInstant("Europe/Andorra"));
    }

    @Test
    public void testCountryRequiringPriority() throws Exception {
        // This is a country that has two zones which were distinct initially but then became the
        // same. The CountryZoneTree needs a priority on one to indicate which "merged into" the
        // other. In this test it is lacking that priority.
        String countryText = "  isoCode:\"de\"\n"
                + "  defaultTimeZoneId:\"Europe/Berlin\"\n"
                + "  timeZoneMappings:<\n"
                + "    utcOffset:\"1:00\"\n"
                + "    id:\"Europe/Berlin\"\n"
                + "  >\n"
                + "  timeZoneMappings:<\n"
                + "    utcOffset:\"1:00\"\n"
                + "    id:\"Europe/Busingen\"\n"
                + "  >\n";
        Country country = parseCountry(countryText);
        CountryZoneTree zoneTree = CountryZoneTree.create(country, RULES_START_INSTANT,
                RULES_END_INSTANT);
        assertFalse(zoneTree.validateNoPriorityClashes().isEmpty());
        try {
            zoneTree.calculateCountryZoneUsage(NOT_USED_AFTER_CUT_OFF);
            fail();
        } catch (IllegalStateException expected) {
        }
    }

    @Test
    public void testCountryWithPriority() throws Exception {
        // This is a country that has two zones which were distinct initially but then became the
        // same. The CountryZoneTree needs a priority on one to indicate which "merged into" the
        // other. In this test one zone has the priority.
        String countryText = "  isoCode:\"de\"\n"
                + "  defaultTimeZoneId:\"Europe/Berlin\"\n"
                + "  timeZoneMappings:<\n"
                + "    utcOffset:\"1:00\"\n"
                + "    id:\"Europe/Berlin\"\n"
                + "    priority: 10\n"
                + "  >\n"
                + "  timeZoneMappings:<\n"
                + "    utcOffset:\"1:00\"\n"
                + "    id:\"Europe/Busingen\"\n"
                + "  >\n";
        Country country = parseCountry(countryText);
        CountryZoneTree zoneTree = CountryZoneTree.create(country, RULES_START_INSTANT,
                RULES_END_INSTANT);
        assertTrue(zoneTree.validateNoPriorityClashes().isEmpty());
        CountryZoneUsage countryZoneUsage =
                zoneTree.calculateCountryZoneUsage(NOT_USED_AFTER_CUT_OFF);
        assertNull(countryZoneUsage.getNotUsedAfterInstant("Europe/Berlin"));
        Instant expectedNotUsedAfterInstant =
                Instant.ofEpochSecond(338950800); /* 1980-09-28T01:00:00Z */
        assertEquals(expectedNotUsedAfterInstant,
                countryZoneUsage.getNotUsedAfterInstant("Europe/Busingen"));
    }

    @Test
    public void testCountryLongDstPeriod() throws Exception {
        // This is a (simplified) Australia: it that has two zones which finish with no DST but
        // started the no-DST period at different times.
        String countryText = "  isoCode:\"au\"\n"
                + "  defaultTimeZoneId:\"Australia/Brisbane\"\n"
                + "  timeZoneMappings:<\n"
                + "    utcOffset:\"10:00\"\n"
                + "    id:\"Australia/Brisbane\"\n"
                + "    priority: 10\n"
                + "  >\n"
                + "  timeZoneMappings:<\n"
                + "    utcOffset:\"10:00\"\n"
                + "    id:\"Australia/Lindeman\"\n"
                + "  >\n";
        australiaTreeTest(countryText);

        // This is the same as the one above, except the zones are ordered differently. Order should
        // not affect the output.
        countryText = "  isoCode:\"au\"\n"
                + "  defaultTimeZoneId:\"Australia/Brisbane\"\n"
                + "  timeZoneMappings:<\n"
                + "    utcOffset:\"10:00\"\n"
                + "    id:\"Australia/Lindeman\"\n"
                + "  >\n"
                + "  timeZoneMappings:<\n"
                + "    utcOffset:\"10:00\"\n"
                + "    id:\"Australia/Brisbane\"\n"
                + "    priority: 10\n"
                + "  >\n";
        australiaTreeTest(countryText);
    }

    private static void australiaTreeTest(String countryText) throws Exception {
        Country country = parseCountry(countryText);
        CountryZoneTree zoneTree = CountryZoneTree.create(country, RULES_START_INSTANT,
                RULES_END_INSTANT);
        assertTrue(zoneTree.validateNoPriorityClashes().isEmpty());
        CountryZoneUsage countryZoneUsage =
                zoneTree.calculateCountryZoneUsage(NOT_USED_AFTER_CUT_OFF);
        Instant expectedNotUsedAfterInstant =
                Instant.ofEpochSecond(762883200); /* 1994-03-05T16:00:00Z */
        assertEquals(expectedNotUsedAfterInstant,
                countryZoneUsage.getNotUsedAfterInstant("Australia/Lindeman"));
        assertNull(countryZoneUsage.getNotUsedAfterInstant("Australia/Brisbane"));
    }

    private static Country parseCountry(String text) throws Exception {
        Country.Builder builder =
                Country.newBuilder();
        TextFormat.getParser().merge(text, builder);
        return builder.build();
    }
}
