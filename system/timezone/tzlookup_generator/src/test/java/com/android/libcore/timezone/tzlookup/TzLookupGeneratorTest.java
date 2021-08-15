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

import com.android.libcore.timezone.tzlookup.proto.CountryZonesFile;
import com.google.protobuf.TextFormat;
import com.ibm.icu.util.TimeZone;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;

import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.stream.Collectors;

import static com.android.libcore.timezone.tzlookup.TestUtils.assertAbsent;
import static com.android.libcore.timezone.tzlookup.TestUtils.assertContains;
import static com.android.libcore.timezone.tzlookup.TestUtils.createFile;
import static com.android.libcore.timezone.tzlookup.proto.CountryZonesFile.Country;
import static junit.framework.TestCase.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

public class TzLookupGeneratorTest {

    public static final String INVALID_TIME_ZONE_ID = "NOT_A_VALID_ID";

    private Path tempDir;

    @Before
    public void setUp() throws Exception {
        tempDir = Files.createTempDirectory("TzLookupGeneratorTest");
    }

    @After
    public void tearDown() throws Exception {
        TestUtils.deleteDir(tempDir);
    }

    @Test
    public void invalidCountryZonesFile() throws Exception {
        String countryZonesFile = createFile(tempDir, "THIS IS NOT A VALID FILE");
        List<ZoneTabFile.CountryEntry> gbZoneTabEntries = createValidZoneTabEntriesGb();
        String zoneTabFile = createZoneTabFile(gbZoneTabEntries);
        String outputFile = Files.createTempFile(tempDir, "out", null /* suffix */).toString();

        TzLookupGenerator tzLookupGenerator =
                new TzLookupGenerator(countryZonesFile, zoneTabFile, outputFile);
        assertFalse(tzLookupGenerator.execute());
    }

    @Test
    public void invalidRulesVersion() throws Exception {
        CountryZonesFile.Country validGb = createValidCountryGb();

        // The IANA version won't match ICU's IANA version so we should see a failure.
        CountryZonesFile.CountryZones badIanaVersionCountryZones =
                createValidCountryZones(validGb).toBuilder().setIanaVersion("2001a").build();
        String countryZonesFile = createCountryZonesFile(badIanaVersionCountryZones);

        List<ZoneTabFile.CountryEntry> gbZoneTabEntries = createValidZoneTabEntriesGb();
        String zoneTabFile = createZoneTabFile(gbZoneTabEntries);

        String outputFile = Files.createTempFile(tempDir, "out", null /* suffix */).toString();

        TzLookupGenerator tzLookupGenerator =
                new TzLookupGenerator(countryZonesFile, zoneTabFile, outputFile);
        assertFalse(tzLookupGenerator.execute());

        Path outputFilePath = Paths.get(outputFile);
        assertEquals(0, Files.size(outputFilePath));
    }

    @Test
    public void countryWithNoTimeZoneMappings() throws Exception {
        // No zones found!
        CountryZonesFile.Country gbWithoutZones =
                createValidCountryGb().toBuilder().clearTimeZoneMappings().build();
        CountryZonesFile.CountryZones countryZones = createValidCountryZones(gbWithoutZones);
        String countryZonesFile = createCountryZonesFile(countryZones);

        String zoneTabFile = createZoneTabFile(createValidZoneTabEntriesGb());

        String outputFile = Files.createTempFile(tempDir, "out", null /* suffix */).toString();

        TzLookupGenerator tzLookupGenerator =
                new TzLookupGenerator(countryZonesFile, zoneTabFile, outputFile);
        assertFalse(tzLookupGenerator.execute());

        Path outputFilePath = Paths.get(outputFile);
        assertEquals(0, Files.size(outputFilePath));
    }

    @Test
    public void countryWithDuplicateTimeZoneMappings() throws Exception {
        // Duplicate zones found!
        CountryZonesFile.Country validCountryGb = createValidCountryGb();
        CountryZonesFile.Country gbWithDuplicateZones =
                validCountryGb.toBuilder()
                        .setDefaultTimeZoneId(validCountryGb.getTimeZoneMappings(0).getId())
                        .addAllTimeZoneMappings(validCountryGb.getTimeZoneMappingsList())
                        .build();
        CountryZonesFile.CountryZones countryZones =
                createValidCountryZones(gbWithDuplicateZones);
        String countryZonesFile = createCountryZonesFile(countryZones);

        String zoneTabFile = createZoneTabFile(createValidZoneTabEntriesGb());

        String outputFile = Files.createTempFile(tempDir, "out", null /* suffix */).toString();

        TzLookupGenerator tzLookupGenerator =
                new TzLookupGenerator(countryZonesFile, zoneTabFile, outputFile);
        assertFalse(tzLookupGenerator.execute());

        Path outputFilePath = Paths.get(outputFile);
        assertEquals(0, Files.size(outputFilePath));
    }

    @Test
    public void badDefaultId() throws Exception {
        // Set an invalid default.
        CountryZonesFile.Country validGb =
                createValidCountryGb().toBuilder()
                        .setDefaultTimeZoneId("NOT_A_TIMEZONE_ID")
                        .build();
        CountryZonesFile.CountryZones gbCountryZones = createValidCountryZones(validGb);
        String countryZonesFile = createCountryZonesFile(gbCountryZones);

        List<ZoneTabFile.CountryEntry> gbZoneTabEntries = createValidZoneTabEntriesGb();
        String zoneTabFile = createZoneTabFile(gbZoneTabEntries);

        String outputFile = Files.createTempFile(tempDir, "out", null /* suffix */).toString();

        TzLookupGenerator tzLookupGenerator =
                new TzLookupGenerator(countryZonesFile, zoneTabFile, outputFile);
        assertFalse(tzLookupGenerator.execute());

        Path outputFilePath = Paths.get(outputFile);
        assertEquals(0, Files.size(outputFilePath));
    }

    @Test
    public void explicitDefaultIdInvalid() throws Exception {
        // Set a valid default, but to one that isn't referenced by "gb".
        CountryZonesFile.Country validGb = createValidCountryGb().toBuilder()
                .setDefaultTimeZoneId(createValidCountryFr().getTimeZoneMappings(0).getId())
                .build();
        CountryZonesFile.CountryZones gbCountryZones = createValidCountryZones(validGb);
        String countryZonesFile = createCountryZonesFile(gbCountryZones);

        List<ZoneTabFile.CountryEntry> gbZoneTabEntries = createValidZoneTabEntriesGb();
        String zoneTabFile = createZoneTabFile(gbZoneTabEntries);

        String outputFile = Files.createTempFile(tempDir, "out", null /* suffix */).toString();

        TzLookupGenerator tzLookupGenerator =
                new TzLookupGenerator(countryZonesFile, zoneTabFile, outputFile);
        assertFalse(tzLookupGenerator.execute());

        Path outputFilePath = Paths.get(outputFile);
        assertEquals(0, Files.size(outputFilePath));
    }

    @Test
    public void calculatedDefaultZone() throws Exception {
        // Ensure there's no explicit default for "gb" and there's one zone.
        CountryZonesFile.Country validCountryGb = createValidCountryGb();
        assertEquals(1, validCountryGb.getTimeZoneMappingsCount());

        String gbTimeZoneId = validCountryGb.getTimeZoneMappings(0).getId();
        CountryZonesFile.Country gbWithoutDefault = validCountryGb.toBuilder()
                .clearDefaultTimeZoneId().build();
        List<ZoneTabFile.CountryEntry> gbZoneTabEntries = createValidZoneTabEntriesGb();

        String tzLookupXml = generateTzLookupXml(gbWithoutDefault, gbZoneTabEntries);

        // Check gb's time zone was defaulted.
        assertContains(tzLookupXml, "code=\"gb\" default=\"" + gbTimeZoneId + "\"");
    }

    @Test
    public void explicitDefaultZone() throws Exception {
        // Ensure there's an explicit default for "gb" and there's one zone.
        CountryZonesFile.Country validCountryGb = createValidCountryGb();
        String gbTimeZoneId = validCountryGb.getTimeZoneMappings(0).getId();
        CountryZonesFile.Country gbWithExplicitDefaultTimeZone =
                validCountryGb.toBuilder()
                        .setDefaultTimeZoneId(gbTimeZoneId)
                        .build();
        List<ZoneTabFile.CountryEntry> gbZoneTabEntries = createValidZoneTabEntriesGb();

        String tzLookupXml = generateTzLookupXml(gbWithExplicitDefaultTimeZone, gbZoneTabEntries);

        // Check gb's time zone was defaulted.
        assertContains(tzLookupXml, "code=\"gb\" default=\"" + gbTimeZoneId + "\"");
    }

    @Test
    public void countryZonesContainsNonLowercaseIsoCode() throws Exception {
        CountryZonesFile.Country validCountry = createValidCountryGb();
        CountryZonesFile.Country invalidCountry =
                createValidCountryGb().toBuilder().setIsoCode("Gb").build();

        CountryZonesFile.CountryZones countryZones =
                createValidCountryZones(validCountry, invalidCountry);
        String countryZonesFile = createCountryZonesFile(countryZones);

        List<ZoneTabFile.CountryEntry> gbZoneTabEntries = createValidZoneTabEntriesGb();
        String zoneTabFile = createZoneTabFile(gbZoneTabEntries);

        String outputFile = Files.createTempFile(tempDir, "out", null /* suffix */).toString();

        TzLookupGenerator tzLookupGenerator =
                new TzLookupGenerator(countryZonesFile, zoneTabFile, outputFile);
        assertFalse(tzLookupGenerator.execute());

        Path outputFilePath = Paths.get(outputFile);
        assertEquals(0, Files.size(outputFilePath));
    }

    @Test
    public void countryZonesContainsDuplicate() throws Exception {
        CountryZonesFile.Country validGb = createValidCountryGb();

        // The file contains "gb" twice.
        CountryZonesFile.CountryZones duplicateGbData =
                createValidCountryZones(validGb, validGb);
        String countryZonesFile = createCountryZonesFile(duplicateGbData);

        List<ZoneTabFile.CountryEntry> gbZoneTabEntries = createValidZoneTabEntriesGb();
        String zoneTabFile = createZoneTabFile(gbZoneTabEntries);

        String outputFile = Files.createTempFile(tempDir, "out", null /* suffix */).toString();

        TzLookupGenerator tzLookupGenerator =
                new TzLookupGenerator(countryZonesFile, zoneTabFile, outputFile);
        assertFalse(tzLookupGenerator.execute());

        Path outputFilePath = Paths.get(outputFile);
        assertEquals(0, Files.size(outputFilePath));
    }

    @Test
    public void countryZonesAndZoneTabCountryMismatch() throws Exception {
        // The two input files contain non-identical country ISO codes.
        CountryZonesFile.CountryZones countryZones =
                createValidCountryZones(createValidCountryGb(), createValidCountryFr());
        String countryZonesFile = createCountryZonesFile(countryZones);

        String zoneTabFile =
                createZoneTabFile(createValidZoneTabEntriesFr(), createValidZoneTabEntriesUs());

        String outputFile = Files.createTempFile(tempDir, "out", null /* suffix */).toString();

        TzLookupGenerator tzLookupGenerator =
                new TzLookupGenerator(countryZonesFile, zoneTabFile, outputFile);
        assertFalse(tzLookupGenerator.execute());

        Path outputFilePath = Paths.get(outputFile);
        assertEquals(0, Files.size(outputFilePath));
    }

    @Test
    public void countryZonesAndZoneTabDisagreeOnZones() throws Exception {
        CountryZonesFile.Country gbWithWrongZones =
                createValidCountryGb().toBuilder()
                        .clearTimeZoneMappings()
                        .addAllTimeZoneMappings(createValidCountryFr().getTimeZoneMappingsList())
                        .build();
        CountryZonesFile.CountryZones countryZones = createValidCountryZones(gbWithWrongZones);
        String countryZonesFile = createCountryZonesFile(countryZones);

        String zoneTabFile = createZoneTabFile(createValidZoneTabEntriesGb());

        String outputFile = Files.createTempFile(tempDir, "out", null /* suffix */).toString();

        TzLookupGenerator tzLookupGenerator =
                new TzLookupGenerator(countryZonesFile, zoneTabFile, outputFile);
        assertFalse(tzLookupGenerator.execute());

        Path outputFilePath = Paths.get(outputFile);
        assertEquals(0, Files.size(outputFilePath));
    }

    @Test
    public void duplicateEntriesInZoneTab() throws Exception {
        CountryZonesFile.Country validGbCountry = createValidCountryGb();
        CountryZonesFile.CountryZones countryZones = createValidCountryZones(validGbCountry);
        String countryZonesFile = createCountryZonesFile(countryZones);

        String zoneTabFileWithDupes = createZoneTabFile(
                createValidZoneTabEntriesGb(), createValidZoneTabEntriesGb());

        String outputFile = Files.createTempFile(tempDir, "out", null /* suffix */).toString();

        TzLookupGenerator tzLookupGenerator =
                new TzLookupGenerator(countryZonesFile, zoneTabFileWithDupes, outputFile);
        assertFalse(tzLookupGenerator.execute());

        Path outputFilePath = Paths.get(outputFile);
        assertEquals(0, Files.size(outputFilePath));
    }

    @Test
    public void incorrectOffset() throws Exception {
        CountryZonesFile.Country validGbCountry = createValidCountryGb();
        CountryZonesFile.Country.Builder gbWithWrongOffsetBuilder = validGbCountry.toBuilder();
        gbWithWrongOffsetBuilder.getTimeZoneMappingsBuilder(0).setUtcOffset("20:00").build();
        CountryZonesFile.Country gbWithWrongOffset = gbWithWrongOffsetBuilder.build();

        CountryZonesFile.CountryZones countryZones = createValidCountryZones(gbWithWrongOffset);
        String countryZonesFile = createCountryZonesFile(countryZones);

        String zoneTabFile = createZoneTabFile(createValidZoneTabEntriesGb());

        String outputFile = Files.createTempFile(tempDir, "out", null /* suffix */).toString();

        TzLookupGenerator tzLookupGenerator =
                new TzLookupGenerator(countryZonesFile, zoneTabFile, outputFile);
        assertFalse(tzLookupGenerator.execute());

        Path outputFilePath = Paths.get(outputFile);
        assertEquals(0, Files.size(outputFilePath));
    }

    @Test
    public void badTimeZoneMappingId() throws Exception {
        CountryZonesFile.Country validGbCountry = createValidCountryGb();
        CountryZonesFile.Country.Builder gbWithBadIdBuilder = validGbCountry.toBuilder();
        gbWithBadIdBuilder.setDefaultTimeZoneId(validGbCountry.getTimeZoneMappings(0).getId())
                .addTimeZoneMappingsBuilder().setId(INVALID_TIME_ZONE_ID).setUtcOffset("00:00");
        CountryZonesFile.Country gbWithBadId = gbWithBadIdBuilder.build();

        CountryZonesFile.CountryZones countryZones = createValidCountryZones(gbWithBadId);
        String countryZonesFile = createCountryZonesFile(countryZones);

        List<ZoneTabFile.CountryEntry> zoneTabEntriesWithBadId =
                new ArrayList<>(createValidZoneTabEntriesGb());
        zoneTabEntriesWithBadId.add(new ZoneTabFile.CountryEntry("GB", INVALID_TIME_ZONE_ID));
        String zoneTabFile = createZoneTabFile(zoneTabEntriesWithBadId);

        String outputFile = Files.createTempFile(tempDir, "out", null /* suffix */).toString();

        TzLookupGenerator tzLookupGenerator =
                new TzLookupGenerator(countryZonesFile, zoneTabFile, outputFile);
        assertFalse(tzLookupGenerator.execute());

        Path outputFilePath = Paths.get(outputFile);
        assertEquals(0, Files.size(outputFilePath));
    }

    @Test
    public void everUtc_true() throws Exception {
        CountryZonesFile.Country validCountryGb = createValidCountryGb();
        String tzLookupXml = generateTzLookupXml(validCountryGb, createValidZoneTabEntriesGb());

        // Check gb's entry contains everutc="y".
        assertContains(tzLookupXml, "everutc=\"y\"");
    }

    @Test
    public void everUtc_false() throws Exception {
        CountryZonesFile.Country validCountryFr = createValidCountryFr();
        String tzLookupXml = generateTzLookupXml(validCountryFr, createValidZoneTabEntriesFr());

        // Check fr's entry contains everutc="n".
        assertContains(tzLookupXml, "everutc=\"n\"");
    }

    @Test
    public void shownInPicker_false() throws Exception {
        CountryZonesFile.Country countryPrototype = createValidCountryFr();

        CountryZonesFile.TimeZoneMapping.Builder timeZoneMappingBuilder =
                countryPrototype.getTimeZoneMappings(0).toBuilder();
        timeZoneMappingBuilder.setShownInPicker(false);

        CountryZonesFile.Country.Builder countryBuilder = countryPrototype.toBuilder();
        countryBuilder.setTimeZoneMappings(0, timeZoneMappingBuilder);
        CountryZonesFile.Country country = countryBuilder.build();

        String tzLookupXml = generateTzLookupXml(country, createValidZoneTabEntriesFr());

        assertContains(tzLookupXml, "picker=\"n\"");
    }

    @Test
    public void shownInPicker_true() throws Exception {
        CountryZonesFile.Country countryPrototype = createValidCountryFr();

        CountryZonesFile.TimeZoneMapping.Builder timeZoneMappingBuilder =
                countryPrototype.getTimeZoneMappings(0).toBuilder();
        timeZoneMappingBuilder.setShownInPicker(true);

        CountryZonesFile.Country.Builder countryBuilder = countryPrototype.toBuilder();
        countryBuilder.setTimeZoneMappings(0, timeZoneMappingBuilder);
        CountryZonesFile.Country country = countryBuilder.build();

        String tzLookupXml = generateTzLookupXml(country, createValidZoneTabEntriesFr());

        // We should not see anything "picker="y" is the implicit default.
        assertAbsent(tzLookupXml, "picker=");
    }

    @Test
    public void notAfter() throws Exception {
        CountryZonesFile.Country country = createValidCountryUs();
        List<ZoneTabFile.CountryEntry> zoneTabEntries = createValidZoneTabEntriesUs();
        String tzLookupXml = generateTzLookupXml(country, zoneTabEntries);
        String expectedOutput =
                "<id>America/New_York</id>\n"
                + "<id notafter=\"167814000000\">America/Detroit</id>\n"
                + "<id notafter=\"152089200000\">America/Kentucky/Louisville</id>\n"
                + "<id notafter=\"972802800000\">America/Kentucky/Monticello</id>\n"
                + "<id notafter=\"1130652000000\">America/Indiana/Indianapolis</id>\n"
                + "<id notafter=\"1194159600000\">America/Indiana/Vincennes</id>\n"
                + "<id notafter=\"1173600000000\">America/Indiana/Winamac</id>\n"
                + "<id notafter=\"183535200000\">America/Indiana/Marengo</id>\n"
                + "<id notafter=\"247042800000\">America/Indiana/Petersburg</id>\n"
                + "<id notafter=\"89186400000\">America/Indiana/Vevay</id>\n"
                + "<id>America/Chicago</id>\n"
                + "<id notafter=\"688546800000\">America/Indiana/Knox</id>\n"
                + "<id notafter=\"104918400000\">America/Menominee</id>\n"
                + "<id notafter=\"720000000000\">America/North_Dakota/Center</id>\n"
                + "<id notafter=\"1067155200000\">America/North_Dakota/New_Salem</id>\n"
                + "<id notafter=\"1143964800000\">America/Indiana/Tell_City</id>\n"
                + "<id notafter=\"1289116800000\">America/North_Dakota/Beulah</id>\n"
                + "<id>America/Denver</id>\n"
                + "<id notafter=\"129114000000\">America/Boise</id>\n"
                + "<id>America/Phoenix</id>\n"
                + "<id>America/Los_Angeles</id>\n"
                + "<id>America/Anchorage</id>\n"
                + "<id notafter=\"436359600000\">America/Juneau</id>\n"
                + "<id notafter=\"436356000000\">America/Yakutat</id>\n"
                + "<id notafter=\"436363200000\">America/Nome</id>\n"
                + "<id notafter=\"1446372000000\">America/Metlakatla</id>\n"
                + "<id notafter=\"341402400000\">America/Sitka</id>\n"
                + "<id>Pacific/Honolulu</id>\n"
                + "<id>America/Adak</id>\n";
        String[] expectedLines = expectedOutput.split("\\n");
        for (String expectedLine : expectedLines) {
            assertContains(tzLookupXml, expectedLine);
        }
    }

    private String generateTzLookupXml(CountryZonesFile.Country country,
            List<ZoneTabFile.CountryEntry> zoneTabEntries) throws Exception {

        CountryZonesFile.CountryZones countryZones = createValidCountryZones(country);
        String countryZonesFile = createCountryZonesFile(countryZones);

        String zoneTabFile = createZoneTabFile(zoneTabEntries);

        String outputFile = Files.createTempFile(tempDir, "out", null /* suffix */).toString();

        TzLookupGenerator tzLookupGenerator =
                new TzLookupGenerator(countryZonesFile, zoneTabFile, outputFile);
        assertTrue(tzLookupGenerator.execute());

        Path outputFilePath = Paths.get(outputFile);
        assertTrue(Files.exists(outputFilePath));

        return readFileToString(outputFilePath);
    }

    private static String readFileToString(Path file) throws IOException {
        return new String(Files.readAllBytes(file), StandardCharsets.UTF_8);
    }

    private String createZoneTabFile(List<ZoneTabFile.CountryEntry>... zoneTabEntriesLists)
            throws Exception {
        List<List<ZoneTabFile.CountryEntry>> entries = Arrays.asList(zoneTabEntriesLists);
        List<String> lines = entries.stream()
                .flatMap(List::stream)
                .map(country -> country.isoCode + "\tIgnored\t" + country.olsonId)
                .collect(Collectors.toList());
        return TestUtils.createFile(tempDir, lines.toArray(new String[0]));
    }

    private String createCountryZonesFile(CountryZonesFile.CountryZones countryZones) throws Exception {
        return TestUtils.createFile(tempDir, TextFormat.printToString(countryZones));
    }

    private static CountryZonesFile.CountryZones createValidCountryZones(
            CountryZonesFile.Country... countries) {
        CountryZonesFile.CountryZones.Builder builder =
                CountryZonesFile.CountryZones.newBuilder()
                        .setIanaVersion(TimeZone.getTZDataVersion());
        for (CountryZonesFile.Country country : countries) {
            builder.addCountries(country);
        }
        return builder.build();
    }

    private static CountryZonesFile.Country createValidCountryGb() {
        return CountryZonesFile.Country.newBuilder()
                .setIsoCode("gb")
                .addTimeZoneMappings(CountryZonesFile.TimeZoneMapping.newBuilder()
                        .setUtcOffset("00:00")
                        .setId("Europe/London"))
                .build();
    }

    private static CountryZonesFile.Country createValidCountryUs() throws Exception {
        // This country demonstrates most interesting algorithm behavior. This is copied verbatim
        // from countryzones.txt.
        String usText =
                "  isoCode:\"us\"\n"
                + "  defaultTimeZoneId:\"America/New_York\"\n"
                + "  timeZoneMappings:<\n"
                + "    utcOffset:\"-5:00\"\n"
                + "    id:\"America/New_York\"\n"
                + "    priority:10\n"
                + "  >\n"
                + "  timeZoneMappings:<\n"
                + "    utcOffset:\"-5:00\"\n"
                + "    id:\"America/Detroit\"\n"
                + "  >\n"
                + "  timeZoneMappings:<\n"
                + "    utcOffset:\"-5:00\"\n"
                + "    id:\"America/Kentucky/Louisville\"\n"
                + "  >\n"
                + "  timeZoneMappings:<\n"
                + "    utcOffset:\"-5:00\"\n"
                + "    id:\"America/Kentucky/Monticello\"\n"
                + "  >\n"
                + "  timeZoneMappings:<\n"
                + "    utcOffset:\"-5:00\"\n"
                + "    id:\"America/Indiana/Indianapolis\"\n"
                + "    priority:9\n"
                + "  >\n"
                + "  timeZoneMappings:<\n"
                + "    utcOffset:\"-5:00\"\n"
                + "    id:\"America/Indiana/Vincennes\"\n"
                + "    priority:9\n"
                + "  >\n"
                + "  timeZoneMappings:<\n"
                + "    utcOffset:\"-5:00\"\n"
                + "    id:\"America/Indiana/Winamac\"\n"
                + "  >\n"
                + "  timeZoneMappings:<\n"
                + "    utcOffset:\"-5:00\"\n"
                + "    id:\"America/Indiana/Marengo\"\n"
                + "  >\n"
                + "  timeZoneMappings:<\n"
                + "    utcOffset:\"-5:00\"\n"
                + "    id:\"America/Indiana/Petersburg\"\n"
                + "  >\n"
                + "  timeZoneMappings:<\n"
                + "    utcOffset:\"-5:00\"\n"
                + "    id:\"America/Indiana/Vevay\"\n"
                + "  >\n"
                + "  timeZoneMappings:<\n"
                + "    utcOffset:\"-6:00\"\n"
                + "    id:\"America/Chicago\"\n"
                + "    priority:10\n"
                + "  >\n"
                + "  timeZoneMappings:<\n"
                + "    utcOffset:\"-6:00\"\n"
                + "    id:\"America/Indiana/Knox\"\n"
                + "  >\n"
                + "  timeZoneMappings:<\n"
                + "    utcOffset:\"-6:00\"\n"
                + "    id:\"America/Menominee\"\n"
                + "  >\n"
                + "  timeZoneMappings:<\n"
                + "    utcOffset:\"-6:00\"\n"
                + "    id:\"America/North_Dakota/Center\"\n"
                + "  >\n"
                + "  timeZoneMappings:<\n"
                + "    utcOffset:\"-6:00\"\n"
                + "    id:\"America/North_Dakota/New_Salem\"\n"
                + "  >\n"
                + "  timeZoneMappings:<\n"
                + "    utcOffset:\"-6:00\"\n"
                + "    id:\"America/Indiana/Tell_City\"\n"
                + "    priority:9\n"
                + "  >\n"
                + "  timeZoneMappings:<\n"
                + "    utcOffset:\"-6:00\"\n"
                + "    id:\"America/North_Dakota/Beulah\"\n"
                + "  >\n"
                + "  timeZoneMappings:<\n"
                + "    utcOffset:\"-7:00\"\n"
                + "    id:\"America/Denver\"\n"
                + "    priority:9\n"
                + "  >\n"
                + "  timeZoneMappings:<\n"
                + "    utcOffset:\"-7:00\"\n"
                + "    id:\"America/Boise\"\n"
                + "  >\n"
                + "  timeZoneMappings:<\n"
                + "    utcOffset:\"-7:00\"\n"
                + "    id:\"America/Phoenix\"\n"
                + "    priority:10\n"
                + "  >\n"
                + "  timeZoneMappings:<\n"
                + "    utcOffset:\"-8:00\"\n"
                + "    id:\"America/Los_Angeles\"\n"
                + "  >\n"
                + "  timeZoneMappings:<\n"
                + "    utcOffset:\"-9:00\"\n"
                + "    id:\"America/Anchorage\"\n"
                + "    priority:10\n"
                + "  >\n"
                + "  timeZoneMappings:<\n"
                + "    utcOffset:\"-9:00\"\n"
                + "    id:\"America/Juneau\"\n"
                + "    priority:9\n"
                + "  >\n"
                + "  timeZoneMappings:<\n"
                + "    utcOffset:\"-9:00\"\n"
                + "    id:\"America/Yakutat\"\n"
                + "  >\n"
                + "  timeZoneMappings:<\n"
                + "    utcOffset:\"-9:00\"\n"
                + "    id:\"America/Nome\"\n"
                + "  >\n"
                + "  timeZoneMappings:<\n"
                + "    utcOffset:\"-9:00\"\n"
                + "    id:\"America/Metlakatla\"\n"
                + "  >\n"
                + "  timeZoneMappings:<\n"
                + "    utcOffset:\"-9:00\"\n"
                + "    id:\"America/Sitka\"\n"
                + "  >\n"
                + "  timeZoneMappings:<\n"
                + "    utcOffset:\"-10:00\"\n"
                + "    id:\"Pacific/Honolulu\"\n"
                + "    priority:10\n"
                + "  >\n"
                + "  timeZoneMappings:<\n"
                + "    utcOffset:\"-10:00\"\n"
                + "    id:\"America/Adak\"\n"
                + "  >\n";
        return parseCountry(usText);
    }

    private static CountryZonesFile.Country createValidCountryFr() {
        return CountryZonesFile.Country.newBuilder()
                .setIsoCode("fr")
                .addTimeZoneMappings(CountryZonesFile.TimeZoneMapping.newBuilder()
                        .setUtcOffset("01:00")
                        .setId("Europe/Paris"))
                .build();
    }

    private static List<ZoneTabFile.CountryEntry> createValidZoneTabEntriesGb() {
        return Arrays.asList(new ZoneTabFile.CountryEntry("GB", "Europe/London"));
    }

    private static List<ZoneTabFile.CountryEntry> createValidZoneTabEntriesUs() {
        return Arrays.asList(
                new ZoneTabFile.CountryEntry("US", "America/New_York"),
                new ZoneTabFile.CountryEntry("US", "America/Detroit"),
                new ZoneTabFile.CountryEntry("US", "America/Kentucky/Louisville"),
                new ZoneTabFile.CountryEntry("US", "America/Kentucky/Monticello"),
                new ZoneTabFile.CountryEntry("US", "America/Indiana/Indianapolis"),
                new ZoneTabFile.CountryEntry("US", "America/Indiana/Vincennes"),
                new ZoneTabFile.CountryEntry("US", "America/Indiana/Winamac"),
                new ZoneTabFile.CountryEntry("US", "America/Indiana/Marengo"),
                new ZoneTabFile.CountryEntry("US", "America/Indiana/Petersburg"),
                new ZoneTabFile.CountryEntry("US", "America/Indiana/Vevay"),
                new ZoneTabFile.CountryEntry("US", "America/Chicago"),
                new ZoneTabFile.CountryEntry("US", "America/Indiana/Tell_City"),
                new ZoneTabFile.CountryEntry("US", "America/Indiana/Knox"),
                new ZoneTabFile.CountryEntry("US", "America/Menominee"),
                new ZoneTabFile.CountryEntry("US", "America/North_Dakota/Center"),
                new ZoneTabFile.CountryEntry("US", "America/North_Dakota/New_Salem"),
                new ZoneTabFile.CountryEntry("US", "America/North_Dakota/Beulah"),
                new ZoneTabFile.CountryEntry("US", "America/Denver"),
                new ZoneTabFile.CountryEntry("US", "America/Boise"),
                new ZoneTabFile.CountryEntry("US", "America/Phoenix"),
                new ZoneTabFile.CountryEntry("US", "America/Los_Angeles"),
                new ZoneTabFile.CountryEntry("US", "America/Anchorage"),
                new ZoneTabFile.CountryEntry("US", "America/Juneau"),
                new ZoneTabFile.CountryEntry("US", "America/Sitka"),
                new ZoneTabFile.CountryEntry("US", "America/Metlakatla"),
                new ZoneTabFile.CountryEntry("US", "America/Yakutat"),
                new ZoneTabFile.CountryEntry("US", "America/Nome"),
                new ZoneTabFile.CountryEntry("US", "America/Adak"),
                new ZoneTabFile.CountryEntry("US", "Pacific/Honolulu"));
    }

    private static List<ZoneTabFile.CountryEntry> createValidZoneTabEntriesFr() {
        return Arrays.asList(
                new ZoneTabFile.CountryEntry("FR", "Europe/Paris"));
    }

    private static Country parseCountry(String text) throws Exception {
        Country.Builder builder = Country.newBuilder();
        TextFormat.getParser().merge(text, builder);
        return builder.build();
    }

}
