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

import com.android.libcore.timezone.tzlookup.CountryZonesFileSupport;
import com.android.libcore.timezone.tzlookup.TzLookupGenerator;
import com.android.libcore.timezone.tzlookup.proto.CountryZonesFile.Country;
import com.android.libcore.timezone.tzlookup.proto.CountryZonesFile.CountryZones;

import java.io.IOException;
import java.time.Instant;
import java.util.Collections;
import java.util.Map;
import java.util.Set;
import java.util.function.Function;
import java.util.stream.Collectors;

import static java.util.stream.Collectors.toSet;

/**
 * A tool for building and visualizing one or more {@link CountryZoneTree} instances.
 */
public final class UniqueZonesVisualizer {

    public static void main(String[] args) throws Exception {
        if (args.length < 3) {
            System.err.println("Usage:");
            System.err.println(UniqueZonesVisualizer.class
                    + " <countryzones.txt file> [country code|_] <output dir> [nocompress]");
            System.err.println("_ means \"all countries\"");
            System.exit(1);
        }
        String countryZonesTextFile = args[0];
        String countryCodeArg = args[1];
        String outputDir = args[2];
        boolean compressTree = args.length == 3 || !"nocompress".equals(args[3]);

        CountryZones countryZones =
                CountryZonesFileSupport.parseCountryZonesTextFile(countryZonesTextFile);
        UniqueZonesVisualizer zonesCalculator = new UniqueZonesVisualizer(countryZones);

        Set<String> countryCodes;
        if (countryCodeArg.equals("_")) {
            countryCodes = countryZones.getCountriesList().stream()
                    .map(Country::getIsoCode)
                    .collect(toSet());
        } else {
            countryCodes = Collections.singleton(countryCodeArg);
        }

        Instant startInclusive = TzLookupGenerator.ZONE_USAGE_CALCS_START;
        Instant endExclusive = TzLookupGenerator.ZONE_USAGE_CALCS_END;
        for (String countryCode : countryCodes) {
            zonesCalculator.createGraphvizFile(countryCode, startInclusive,
                    endExclusive, compressTree, outputDir + "/" + countryCode + ".gv");
        }
    }

    private final Map<String, Country> countryMap;

    private UniqueZonesVisualizer(CountryZones countryZones) {
        this.countryMap = countryZones.getCountriesList().stream()
                .collect(Collectors.toMap(Country::getIsoCode, Function.identity()));
    }

    private void createGraphvizFile(String countryIso, Instant startInclusive, Instant endExclusive,
            boolean compress, String outputFile) throws IOException {
        Country country = countryMap.get(countryIso);
        CountryZoneTree tree =
                CountryZoneTree.create(country, startInclusive, endExclusive, compress);
        tree.createGraphvizFile(outputFile);
    }
}

