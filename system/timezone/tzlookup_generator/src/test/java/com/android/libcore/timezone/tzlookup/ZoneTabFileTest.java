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

package com.android.libcore.timezone.tzlookup;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import static junit.framework.TestCase.assertEquals;
import static org.junit.Assert.assertTrue;
import static org.junit.Assert.fail;

public class ZoneTabFileTest {

    private Path tempDir;

    @Before
    public void setUp() throws Exception {
        tempDir = Files.createTempDirectory("ZoneTabFileTest");
    }

    @After
    public void tearDown() throws Exception {
        TestUtils.deleteDir(tempDir);
    }

    @Test
    public void parseEmpty() throws Exception {
        String file = createFile("");
        ZoneTabFile zoneTab = ZoneTabFile.parse(file);
        assertTrue(zoneTab.getCountryEntries().isEmpty());
    }

    @Test
    public void parseIgnoresCommentsAndEmptyLines() throws Exception {
        String file = createFile(
                "# This is a comment",
                "",
                "# And another",
                "GB\tStuff\tZoneId\tStuff"
        );
        ZoneTabFile zoneTab = ZoneTabFile.parse(file);
        assertEquals(
                Arrays.asList(new ZoneTabFile.CountryEntry("GB", "ZoneId")),
                zoneTab.getCountryEntries());
    }

    @Test
    public void parse() throws Exception {
        String file = createFile(
                "# This is a comment",
                "GB\tStuff\tEurope/London\tStuff",
                "# This is a comment",
                "US\tStuff\tAmerica/New_York\tStuff",
                "# This is a comment",
                "US\tStuff\tAmerica/Los_Angeles",
                "# This is a comment"
        );
        ZoneTabFile zoneTab = ZoneTabFile.parse(file);
        assertEquals(
                Arrays.asList(
                        new ZoneTabFile.CountryEntry("GB", "Europe/London"),
                        new ZoneTabFile.CountryEntry("US", "America/New_York"),
                        new ZoneTabFile.CountryEntry("US", "America/Los_Angeles")),
                zoneTab.getCountryEntries());
    }

    @Test
    public void parseMalformedFile() throws Exception {
        // Mapping lines are expected to have at least three tab-separated columns.
        String file = createFile("GB\tStuff");
        try {
            ZoneTabFile.parse(file);
            fail();
        } catch (IOException expected) {}
    }

    @Test
    public void createCountryToOlsonIdsMap() throws Exception {
        String file = createFile(
                "# This is a comment",
                "GB\tStuff\tEurope/London\tStuff",
                "US\tStuff\tAmerica/New_York\tStuff",
                "US\tStuff\tAmerica/Los_Angeles"
        );
        Map<String, List<String>> expected = new HashMap<>();
        addIds(expected, "GB", "Europe/London");
        addIds(expected, "US", "America/New_York", "America/Los_Angeles");

        ZoneTabFile zoneTab = ZoneTabFile.parse(file);
        Map<String, List<String>> actual = ZoneTabFile.createCountryToOlsonIdsMap(zoneTab);
        assertEquals(expected, actual);
    }

    private void addIds(Map<String, List<String>> map, String iso, String... ids) {
        map.put(iso, Arrays.asList(ids));
    }

    private String createFile(String... lines) throws IOException {
        return TestUtils.createFile(tempDir, lines);
    }
}
