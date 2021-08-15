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

import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.function.Function;
import java.util.stream.Collectors;

/**
 * A class that knows about the structure of the zone.tab file.
 */
final class ZoneTabFile {

    private final List<CountryEntry> countryEntries = new ArrayList<>();

    private ZoneTabFile() {}

    static ZoneTabFile parse(String zoneTabFile) throws IOException {
        ZoneTabFile zoneTab = new ZoneTabFile();

        List<String> lines = Files
                .readAllLines(Paths.get(zoneTabFile), StandardCharsets.US_ASCII);

        // Remove comments
        List<String> mappingLines =
                lines.stream()
                        .filter(s -> !(s.startsWith("#") || s.isEmpty()))
                        .collect(Collectors.toList());

        for (String mappingLine : mappingLines) {
            String[] fields = mappingLine.split("\t");
            if (fields.length < 3) {
                throw new IOException("Line is malformed: " + mappingLine);
            }
            CountryEntry countryEntry = new CountryEntry(fields[0], fields[2]);
            zoneTab.addCountryEntry(countryEntry);
        }
        return zoneTab;
    }

    private void addCountryEntry(CountryEntry countryEntry) {
        countryEntries.add(countryEntry);
    }

    List<CountryEntry> getCountryEntries() {
        return Collections.unmodifiableList(countryEntries);
    }

    static Map<String, List<String>> createCountryToOlsonIdsMap(ZoneTabFile zoneTab) {
        Map<String, List<String>> countryIsoToOlsonIdsMap = new HashMap<>();
        for (CountryEntry countryEntry : zoneTab.countryEntries) {
            List<String> olsonIds = countryIsoToOlsonIdsMap.computeIfAbsent(
                    countryEntry.isoCode, k -> new ArrayList<>());
            olsonIds.add(countryEntry.olsonId);
        }
        return countryIsoToOlsonIdsMap;
    }

    static class CountryEntry {
        final String isoCode;
        final String olsonId;

        CountryEntry(String isoCode, String olsonId) {
            this.isoCode = isoCode;
            this.olsonId = olsonId;
        }

        @Override
        public boolean equals(Object o) {
            if (this == o) {
                return true;
            }
            if (o == null || getClass() != o.getClass()) {
                return false;
            }

            CountryEntry that = (CountryEntry) o;

            if (!isoCode.equals(that.isoCode)) {
                return false;
            }
            return olsonId.equals(that.olsonId);
        }

        @Override
        public int hashCode() {
            int result = isoCode.hashCode();
            result = 31 * result + olsonId.hashCode();
            return result;
        }
    }
}
