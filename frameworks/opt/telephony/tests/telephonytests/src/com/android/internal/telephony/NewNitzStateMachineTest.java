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
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertNull;
import static org.junit.Assert.assertTrue;
import static org.mockito.ArgumentMatchers.any;
import static org.mockito.ArgumentMatchers.eq;
import static org.mockito.Mockito.atLeast;
import static org.mockito.Mockito.clearInvocations;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.verifyNoMoreInteractions;
import static org.mockito.Mockito.when;

import android.icu.util.Calendar;
import android.icu.util.GregorianCalendar;
import android.icu.util.TimeZone;
import android.util.TimestampedValue;

import com.android.internal.telephony.TimeZoneLookupHelper.CountryResult;
import com.android.internal.telephony.TimeZoneLookupHelper.OffsetResult;

import org.junit.After;
import org.junit.Before;
import org.junit.Test;
import org.mockito.Mock;

public class NewNitzStateMachineTest extends TelephonyTest {

    // A country with a single zone : the zone can be guessed from the country.
    // The UK uses UTC for part of the year so it is not good for detecting bogus NITZ signals.
    private static final Scenario UNITED_KINGDOM_SCENARIO = new Scenario.Builder()
            .setInitialDeviceSystemClockUtc(1977, 1, 1, 12, 0, 0)
            .setInitialDeviceRealtimeMillis(123456789L)
            .setTimeZone("Europe/London")
            .setActualTimeUtc(2018, 1, 1, 12, 0, 0)
            .setCountryIso("gb")
            .build();

    // A country that has multiple zones, but there is only one matching time zone at the time :
    // the zone cannot be guessed from the country alone, but can be guessed from the country +
    // NITZ. The US never uses UTC so it can be used for testing bogus NITZ signal handling.
    private static final Scenario UNIQUE_US_ZONE_SCENARIO = new Scenario.Builder()
            .setInitialDeviceSystemClockUtc(1977, 1, 1, 12, 0, 0)
            .setInitialDeviceRealtimeMillis(123456789L)
            .setTimeZone("America/Los_Angeles")
            .setActualTimeUtc(2018, 1, 1, 12, 0, 0)
            .setCountryIso("us")
            .build();

    // A country with a single zone: the zone can be guessed from the country alone. CZ never uses
    // UTC so it can be used for testing bogus NITZ signal handling.
    private static final Scenario CZECHIA_SCENARIO = new Scenario.Builder()
            .setInitialDeviceSystemClockUtc(1977, 1, 1, 12, 0, 0)
            .setInitialDeviceRealtimeMillis(123456789L)
            .setTimeZone("Europe/Prague")
            .setActualTimeUtc(2018, 1, 1, 12, 0, 0)
            .setCountryIso("cz")
            .build();

    @Mock
    private NewNitzStateMachine.DeviceState mDeviceState;

    @Mock
    private NewTimeServiceHelper mTimeServiceHelper;

    private TimeZoneLookupHelper mRealTimeZoneLookupHelper;

    private NewNitzStateMachine mNitzStateMachine;

    @Before
    public void setUp() throws Exception {
        logd("NitzStateMachineTest +Setup!");
        super.setUp("NitzStateMachineTest");

        // In tests we use the real TimeZoneLookupHelper.
        mRealTimeZoneLookupHelper = new TimeZoneLookupHelper();
        mNitzStateMachine = new NewNitzStateMachine(
                mPhone, mTimeServiceHelper, mDeviceState, mRealTimeZoneLookupHelper);

        logd("ServiceStateTrackerTest -Setup!");
    }

    @After
    public void tearDown() throws Exception {
        checkNoUnverifiedSetOperations(mTimeServiceHelper);

        super.tearDown();
    }

    @Test
    public void test_uniqueUsZone_Assumptions() {
        // Check we'll get the expected behavior from TimeZoneLookupHelper.

        // allZonesHaveSameOffset == false, so we shouldn't pick an arbitrary zone.
        CountryResult expectedCountryLookupResult = new CountryResult(
                "America/New_York", false /* allZonesHaveSameOffset */,
                UNIQUE_US_ZONE_SCENARIO.getInitialSystemClockMillis());
        CountryResult actualCountryLookupResult =
                mRealTimeZoneLookupHelper.lookupByCountry(
                        UNIQUE_US_ZONE_SCENARIO.getNetworkCountryIsoCode(),
                        UNIQUE_US_ZONE_SCENARIO.getInitialSystemClockMillis());
        assertEquals(expectedCountryLookupResult, actualCountryLookupResult);

        // isOnlyMatch == true, so the combination of country + NITZ should be enough.
        OffsetResult expectedLookupResult =
                new OffsetResult("America/Los_Angeles", true /* isOnlyMatch */);
        OffsetResult actualLookupResult = mRealTimeZoneLookupHelper.lookupByNitzCountry(
                UNIQUE_US_ZONE_SCENARIO.getNitzSignal().getValue(),
                UNIQUE_US_ZONE_SCENARIO.getNetworkCountryIsoCode());
        assertEquals(expectedLookupResult, actualLookupResult);
    }

    @Test
    public void test_unitedKingdom_Assumptions() {
        // Check we'll get the expected behavior from TimeZoneLookupHelper.

        // allZonesHaveSameOffset == true (not only that, there is only one zone), so we can pick
        // the zone knowing only the country.
        CountryResult expectedCountryLookupResult = new CountryResult(
                "Europe/London", true /* allZonesHaveSameOffset */,
                UNITED_KINGDOM_SCENARIO.getInitialSystemClockMillis());
        CountryResult actualCountryLookupResult =
                mRealTimeZoneLookupHelper.lookupByCountry(
                        UNITED_KINGDOM_SCENARIO.getNetworkCountryIsoCode(),
                        UNITED_KINGDOM_SCENARIO.getInitialSystemClockMillis());
        assertEquals(expectedCountryLookupResult, actualCountryLookupResult);

        OffsetResult expectedLookupResult =
                new OffsetResult("Europe/London", true /* isOnlyMatch */);
        OffsetResult actualLookupResult = mRealTimeZoneLookupHelper.lookupByNitzCountry(
                UNITED_KINGDOM_SCENARIO.getNitzSignal().getValue(),
                UNITED_KINGDOM_SCENARIO.getNetworkCountryIsoCode());
        assertEquals(expectedLookupResult, actualLookupResult);
    }

    @Test
    public void test_uniqueUsZone_timeZoneEnabled_countryThenNitz() throws Exception {
        Scenario scenario = UNIQUE_US_ZONE_SCENARIO;
        Device device = new DeviceBuilder()
                .setClocksFromScenario(scenario)
                .setTimeZoneDetectionEnabled(true)
                .setTimeZoneSettingInitialized(false)
                .initialize();
        Script script = new Script(device);

        script.countryReceived(scenario.getNetworkCountryIsoCode())
                // Country won't be enough for time zone detection.
                .verifyNothingWasSetAndReset()
                .nitzReceived(scenario.getNitzSignal())
                // Country + NITZ is enough for both time + time zone detection.
                .verifyTimeSuggestedAndZoneSetAndReset(
                        scenario.getNitzSignal(), scenario.getTimeZoneId());

        // Check NitzStateMachine state.
        assertTrue(mNitzStateMachine.getNitzTimeZoneDetectionSuccessful());
        assertEquals(scenario.getNitzSignal().getValue(), mNitzStateMachine.getCachedNitzData());
        assertEquals(scenario.getTimeZoneId(), mNitzStateMachine.getSavedTimeZoneId());
    }

    @Test
    public void test_unitedKingdom_timeZoneEnabled_countryThenNitz() throws Exception {
        Scenario scenario = UNITED_KINGDOM_SCENARIO;
        Device device = new DeviceBuilder()
                .setClocksFromScenario(scenario)
                .setTimeZoneDetectionEnabled(true)
                .setTimeZoneSettingInitialized(false)
                .initialize();
        Script script = new Script(device);

        script.countryReceived(scenario.getNetworkCountryIsoCode())
                // Country alone is enough to guess the time zone.
                .verifyOnlyTimeZoneWasSetAndReset(scenario.getTimeZoneId())
                .nitzReceived(scenario.getNitzSignal())
                // Country + NITZ is enough for both time + time zone detection.
                .verifyTimeSuggestedAndZoneSetAndReset(
                        scenario.getNitzSignal(), scenario.getTimeZoneId());

        // Check NitzStateMachine state.
        assertTrue(mNitzStateMachine.getNitzTimeZoneDetectionSuccessful());
        assertEquals(scenario.getNitzSignal().getValue(), mNitzStateMachine.getCachedNitzData());
        assertEquals(scenario.getTimeZoneId(), mNitzStateMachine.getSavedTimeZoneId());
    }

    @Test
    public void test_uniqueUsZone_timeZoneDisabled_countryThenNitz() throws Exception {
        Scenario scenario = UNIQUE_US_ZONE_SCENARIO;
        Device device = new DeviceBuilder()
                .setClocksFromScenario(scenario)
                .setTimeZoneDetectionEnabled(false)
                .setTimeZoneSettingInitialized(false)
                .initialize();
        Script script = new Script(device);

        script.countryReceived(scenario.getNetworkCountryIsoCode())
                // Country is not enough to guess the time zone and time zone detection is disabled.
                .verifyNothingWasSetAndReset()
                .nitzReceived(scenario.getNitzSignal())
                // Time zone detection is disabled, but time should be suggested from NITZ.
                .verifyOnlyTimeWasSuggestedAndReset(scenario.getNitzSignal());

        // Check NitzStateMachine state.
        assertTrue(mNitzStateMachine.getNitzTimeZoneDetectionSuccessful());
        assertEquals(scenario.getNitzSignal().getValue(), mNitzStateMachine.getCachedNitzData());
        assertEquals(scenario.getTimeZoneId(), mNitzStateMachine.getSavedTimeZoneId());
    }

    @Test
    public void test_unitedKingdom_timeZoneDisabled_countryThenNitz() throws Exception {
        Scenario scenario = UNITED_KINGDOM_SCENARIO;
        Device device = new DeviceBuilder()
                .setClocksFromScenario(scenario)
                .setTimeZoneDetectionEnabled(false)
                .setTimeZoneSettingInitialized(false)
                .initialize();
        Script script = new Script(device);

        script.countryReceived(scenario.getNetworkCountryIsoCode())
                // Country alone would be enough for time zone detection, but it's disabled.
                .verifyNothingWasSetAndReset()
                .nitzReceived(scenario.getNitzSignal())
                // Time zone detection is disabled, but time should be suggested from NITZ.
                .verifyOnlyTimeWasSuggestedAndReset(scenario.getNitzSignal());

        // Check NitzStateMachine state.
        assertTrue(mNitzStateMachine.getNitzTimeZoneDetectionSuccessful());
        assertEquals(scenario.getNitzSignal().getValue(), mNitzStateMachine.getCachedNitzData());
        assertEquals(scenario.getTimeZoneId(), mNitzStateMachine.getSavedTimeZoneId());
    }

    @Test
    public void test_uniqueUsZone_timeZoneEnabled_nitzThenCountry() throws Exception {
        Scenario scenario = UNIQUE_US_ZONE_SCENARIO;
        Device device = new DeviceBuilder()
                .setClocksFromScenario(scenario)
                .setTimeZoneDetectionEnabled(true)
                .setTimeZoneSettingInitialized(false)
                .initialize();
        Script script = new Script(device);

        // Simulate receiving an NITZ signal.
        script.nitzReceived(scenario.getNitzSignal())
                // The NITZ alone isn't enough to detect a time zone.
                .verifyOnlyTimeWasSuggestedAndReset(scenario.getNitzSignal());

        // Check NitzStateMachine state.
        assertFalse(mNitzStateMachine.getNitzTimeZoneDetectionSuccessful());
        assertEquals(scenario.getNitzSignal().getValue(), mNitzStateMachine.getCachedNitzData());
        assertNull(mNitzStateMachine.getSavedTimeZoneId());

        // Simulate the country code becoming known.
        script.countryReceived(scenario.getNetworkCountryIsoCode())
                // The NITZ + country is enough to detect the time zone.
                .verifyOnlyTimeZoneWasSetAndReset(scenario.getTimeZoneId());

        // Check NitzStateMachine state.
        assertTrue(mNitzStateMachine.getNitzTimeZoneDetectionSuccessful());
        assertEquals(scenario.getNitzSignal().getValue(), mNitzStateMachine.getCachedNitzData());
        assertEquals(scenario.getTimeZoneId(), mNitzStateMachine.getSavedTimeZoneId());
    }

    @Test
    public void test_unitedKingdom_timeZoneEnabled_nitzThenCountry() throws Exception {
        Scenario scenario = UNITED_KINGDOM_SCENARIO;
        Device device = new DeviceBuilder()
                .setClocksFromScenario(scenario)
                .setTimeZoneDetectionEnabled(true)
                .setTimeZoneSettingInitialized(false)
                .initialize();
        Script script = new Script(device);

        // Simulate receiving an NITZ signal.
        script.nitzReceived(scenario.getNitzSignal())
                // The NITZ alone isn't enough to detect a time zone.
                .verifyOnlyTimeWasSuggestedAndReset(scenario.getNitzSignal());

        // Check NitzStateMachine state.
        assertFalse(mNitzStateMachine.getNitzTimeZoneDetectionSuccessful());
        assertEquals(scenario.getNitzSignal().getValue(), mNitzStateMachine.getCachedNitzData());
        assertNull(mNitzStateMachine.getSavedTimeZoneId());

        // Simulate the country code becoming known.
        script.countryReceived(scenario.getNetworkCountryIsoCode());

        // The NITZ + country is enough to detect the time zone.
        // NOTE: setting the time zone happens twice because of a quirk in NitzStateMachine: it
        // handles the country lookup / set, then combines the country with the NITZ state and does
        // another lookup / set. We shouldn't require it is set twice but we do for simplicity.
        script.verifyOnlyTimeZoneWasSetAndReset(scenario.getTimeZoneId(), 2 /* times */);

        // Check NitzStateMachine state.
        assertTrue(mNitzStateMachine.getNitzTimeZoneDetectionSuccessful());
        assertEquals(scenario.getNitzSignal().getValue(), mNitzStateMachine.getCachedNitzData());
        assertEquals(scenario.getTimeZoneId(), mNitzStateMachine.getSavedTimeZoneId());
    }

    @Test
    public void test_validCzNitzSignal_nitzReceivedFirst() throws Exception {
        Scenario scenario = CZECHIA_SCENARIO;
        Device device = new DeviceBuilder()
                .setClocksFromScenario(scenario)
                .setTimeZoneDetectionEnabled(true)
                .setTimeZoneSettingInitialized(true)
                .initialize();
        Script script = new Script(device);

        TimestampedValue<NitzData> goodNitzSignal = scenario.getNitzSignal();

        // Simulate receiving an NITZ signal.
        script.nitzReceived(goodNitzSignal)
                // The NITZ alone isn't enough to detect a time zone.
                .verifyOnlyTimeWasSuggestedAndReset(scenario.getNitzSignal());

        // Check NitzStateMachine state.
        assertFalse(mNitzStateMachine.getNitzTimeZoneDetectionSuccessful());
        assertEquals(goodNitzSignal.getValue(), mNitzStateMachine.getCachedNitzData());
        assertNull(mNitzStateMachine.getSavedTimeZoneId());

        // Simulate the country code becoming known.
        script.countryReceived(scenario.getNetworkCountryIsoCode())
                // The NITZ country is enough to detect the time zone, but the NITZ + country is
                // also sufficient so we expect the time zone to be set twice.
                .verifyOnlyTimeZoneWasSetAndReset(scenario.getTimeZoneId(), 2);

        // Check NitzStateMachine state.
        assertTrue(mNitzStateMachine.getNitzTimeZoneDetectionSuccessful());
        assertEquals(goodNitzSignal.getValue(), mNitzStateMachine.getCachedNitzData());
        assertEquals(scenario.getTimeZoneId(), mNitzStateMachine.getSavedTimeZoneId());
    }

    @Test
    public void test_validCzNitzSignal_countryReceivedFirst() throws Exception {
        Scenario scenario = CZECHIA_SCENARIO;
        Device device = new DeviceBuilder()
                .setClocksFromScenario(scenario)
                .setTimeZoneDetectionEnabled(true)
                .setTimeZoneSettingInitialized(true)
                .initialize();
        Script script = new Script(device);

        TimestampedValue<NitzData> goodNitzSignal = scenario.getNitzSignal();

        // Simulate the country code becoming known.
        script.countryReceived(scenario.getNetworkCountryIsoCode())
                // The NITZ country is enough to detect the time zone.
                .verifyOnlyTimeZoneWasSetAndReset(scenario.getTimeZoneId(), 1);

        // Check NitzStateMachine state.
        assertFalse(mNitzStateMachine.getNitzTimeZoneDetectionSuccessful());
        assertNull(mNitzStateMachine.getCachedNitzData());
        assertEquals(scenario.getTimeZoneId(), mNitzStateMachine.getSavedTimeZoneId());

        // Simulate receiving an NITZ signal.
        script.nitzReceived(goodNitzSignal)
                // The time will be suggested from the NITZ signal.
                // The combination of NITZ + country will cause the time zone to be set.
                .verifyTimeSuggestedAndZoneSetAndReset(
                        scenario.getNitzSignal(), scenario.getTimeZoneId());

        // Check NitzStateMachine state.
        assertTrue(mNitzStateMachine.getNitzTimeZoneDetectionSuccessful());
        assertEquals(goodNitzSignal.getValue(), mNitzStateMachine.getCachedNitzData());
        assertEquals(scenario.getTimeZoneId(), mNitzStateMachine.getSavedTimeZoneId());
    }

    @Test
    public void test_bogusCzNitzSignal_nitzReceivedFirst() throws Exception {
        Scenario scenario = CZECHIA_SCENARIO;
        Device device = new DeviceBuilder()
                .setClocksFromScenario(scenario)
                .setTimeZoneDetectionEnabled(true)
                .setTimeZoneSettingInitialized(true)
                .initialize();
        Script script = new Script(device);

        TimestampedValue<NitzData> goodNitzSignal = scenario.getNitzSignal();

        // Create a corrupted NITZ signal, where the offset information has been lost.
        NitzData bogusNitzData = NitzData.createForTests(
                0 /* UTC! */, null /* dstOffsetMillis */,
                goodNitzSignal.getValue().getCurrentTimeInMillis(),
                null /* emulatorHostTimeZone */);
        TimestampedValue<NitzData> badNitzSignal = new TimestampedValue<>(
                goodNitzSignal.getReferenceTimeMillis(), bogusNitzData);

        // Simulate receiving an NITZ signal.
        script.nitzReceived(badNitzSignal)
                // The NITZ alone isn't enough to detect a time zone, but there isn't enough
                // information to work out it is bogus.
                .verifyOnlyTimeWasSuggestedAndReset(scenario.getNitzSignal());

        // Check NitzStateMachine state.
        assertFalse(mNitzStateMachine.getNitzTimeZoneDetectionSuccessful());
        assertEquals(badNitzSignal.getValue(), mNitzStateMachine.getCachedNitzData());
        assertNull(mNitzStateMachine.getSavedTimeZoneId());

        // Simulate the country code becoming known.
        script.countryReceived(scenario.getNetworkCountryIsoCode())
                // The country is enough to detect the time zone for CZ. If the NITZ signal
                // wasn't obviously bogus we'd try to set it twice.
                .verifyOnlyTimeZoneWasSetAndReset(scenario.getTimeZoneId(), 1);

        // Check NitzStateMachine state.
        assertFalse(mNitzStateMachine.getNitzTimeZoneDetectionSuccessful());
        assertEquals(badNitzSignal.getValue(), mNitzStateMachine.getCachedNitzData());
        assertEquals(scenario.getTimeZoneId(), mNitzStateMachine.getSavedTimeZoneId());
    }

    @Test
    public void test_bogusCzNitzSignal_countryReceivedFirst() throws Exception {
        Scenario scenario = CZECHIA_SCENARIO;
        Device device = new DeviceBuilder()
                .setClocksFromScenario(scenario)
                .setTimeZoneDetectionEnabled(true)
                .setTimeZoneSettingInitialized(true)
                .initialize();
        Script script = new Script(device);

        TimestampedValue<NitzData> goodNitzSignal = scenario.getNitzSignal();

        // Create a corrupted NITZ signal, where the offset information has been lost.
        NitzData bogusNitzData = NitzData.createForTests(
                0 /* UTC! */, null /* dstOffsetMillis */,
                goodNitzSignal.getValue().getCurrentTimeInMillis(),
                null /* emulatorHostTimeZone */);
        TimestampedValue<NitzData> badNitzSignal = new TimestampedValue<>(
                goodNitzSignal.getReferenceTimeMillis(), bogusNitzData);

        // Simulate the country code becoming known.
        script.countryReceived(scenario.getNetworkCountryIsoCode())
                // The country is enough to detect the time zone for CZ.
                .verifyOnlyTimeZoneWasSetAndReset(scenario.getTimeZoneId());

        // Check NitzStateMachine state.
        assertFalse(mNitzStateMachine.getNitzTimeZoneDetectionSuccessful());
        assertNull(mNitzStateMachine.getCachedNitzData());
        assertEquals(scenario.getTimeZoneId(), mNitzStateMachine.getSavedTimeZoneId());

        // Simulate receiving an NITZ signal.
        script.nitzReceived(badNitzSignal)
                // The NITZ should be detected as bogus so only the time will be suggested.
                .verifyOnlyTimeWasSuggestedAndReset(scenario.getNitzSignal());

        // Check NitzStateMachine state.
        assertFalse(mNitzStateMachine.getNitzTimeZoneDetectionSuccessful());
        assertEquals(badNitzSignal.getValue(), mNitzStateMachine.getCachedNitzData());
        assertEquals(scenario.getTimeZoneId(), mNitzStateMachine.getSavedTimeZoneId());
    }

    @Test
    public void test_bogusUniqueUsNitzSignal_nitzReceivedFirst() throws Exception {
        Scenario scenario = UNIQUE_US_ZONE_SCENARIO;
        Device device = new DeviceBuilder()
                .setClocksFromScenario(scenario)
                .setTimeZoneDetectionEnabled(true)
                .setTimeZoneSettingInitialized(true)
                .initialize();
        Script script = new Script(device);

        TimestampedValue<NitzData> goodNitzSignal = scenario.getNitzSignal();

        // Create a corrupted NITZ signal, where the offset information has been lost.
        NitzData bogusNitzData = NitzData.createForTests(
                0 /* UTC! */, null /* dstOffsetMillis */,
                goodNitzSignal.getValue().getCurrentTimeInMillis(),
                null /* emulatorHostTimeZone */);
        TimestampedValue<NitzData> badNitzSignal = new TimestampedValue<>(
                goodNitzSignal.getReferenceTimeMillis(), bogusNitzData);

        // Simulate receiving an NITZ signal.
        script.nitzReceived(badNitzSignal)
                // The NITZ alone isn't enough to detect a time zone, but there isn't enough
                // information to work out its bogus.
                .verifyOnlyTimeWasSuggestedAndReset(scenario.getNitzSignal());

        // Check NitzStateMachine state.
        assertFalse(mNitzStateMachine.getNitzTimeZoneDetectionSuccessful());
        assertEquals(badNitzSignal.getValue(), mNitzStateMachine.getCachedNitzData());
        assertNull(mNitzStateMachine.getSavedTimeZoneId());

        // Simulate the country code becoming known.
        script.countryReceived(scenario.getNetworkCountryIsoCode())
                // The country isn't enough to detect the time zone for US so we will leave the time
                // zone unset.
                .verifyNothingWasSetAndReset();

        // Check NitzStateMachine state.
        assertFalse(mNitzStateMachine.getNitzTimeZoneDetectionSuccessful());
        assertEquals(badNitzSignal.getValue(), mNitzStateMachine.getCachedNitzData());
        assertNull(mNitzStateMachine.getSavedTimeZoneId());
    }

    @Test
    public void test_bogusUsUniqueNitzSignal_countryReceivedFirst() throws Exception {
        Scenario scenario = UNIQUE_US_ZONE_SCENARIO;
        Device device = new DeviceBuilder()
                .setClocksFromScenario(scenario)
                .setTimeZoneDetectionEnabled(true)
                .setTimeZoneSettingInitialized(true)
                .initialize();
        Script script = new Script(device);

        TimestampedValue<NitzData> goodNitzSignal = scenario.getNitzSignal();

        // Create a corrupted NITZ signal, where the offset information has been lost.
        NitzData bogusNitzData = NitzData.createForTests(
                0 /* UTC! */, null /* dstOffsetMillis */,
                goodNitzSignal.getValue().getCurrentTimeInMillis(),
                null /* emulatorHostTimeZone */);
        TimestampedValue<NitzData> badNitzSignal = new TimestampedValue<>(
                goodNitzSignal.getReferenceTimeMillis(), bogusNitzData);

        // Simulate the country code becoming known.
        script.countryReceived(scenario.getNetworkCountryIsoCode())
                // The country isn't enough to detect the time zone for US so we will leave the time
                // zone unset.
                .verifyNothingWasSetAndReset();

        // Check NitzStateMachine state.
        assertFalse(mNitzStateMachine.getNitzTimeZoneDetectionSuccessful());
        assertNull(mNitzStateMachine.getCachedNitzData());
        assertNull(mNitzStateMachine.getSavedTimeZoneId());

        // Simulate receiving an NITZ signal.
        script.nitzReceived(badNitzSignal)
                // The NITZ should be detected as bogus so only the time will be suggested.
                .verifyOnlyTimeWasSuggestedAndReset(scenario.getNitzSignal());

        // Check NitzStateMachine state.
        assertFalse(mNitzStateMachine.getNitzTimeZoneDetectionSuccessful());
        assertEquals(badNitzSignal.getValue(), mNitzStateMachine.getCachedNitzData());
        assertNull(mNitzStateMachine.getSavedTimeZoneId());
    }

    @Test
    public void test_emulatorNitzExtensionUsedForTimeZone() throws Exception {
        Scenario scenario = UNIQUE_US_ZONE_SCENARIO;
        Device device = new DeviceBuilder()
                .setClocksFromScenario(scenario)
                .setTimeZoneDetectionEnabled(true)
                .setTimeZoneSettingInitialized(true)
                .initialize();
        Script script = new Script(device);

        TimestampedValue<NitzData> originalNitzSignal = scenario.getNitzSignal();

        // Create an NITZ signal with an explicit time zone (as can happen on emulators)
        NitzData originalNitzData = originalNitzSignal.getValue();
        // A time zone that is obviously not in the US, but it should not be questioned.
        String emulatorTimeZoneId = "Europe/London";
        NitzData emulatorNitzData = NitzData.createForTests(
                originalNitzData.getLocalOffsetMillis(),
                originalNitzData.getDstAdjustmentMillis(),
                originalNitzData.getCurrentTimeInMillis(),
                java.util.TimeZone.getTimeZone(emulatorTimeZoneId) /* emulatorHostTimeZone */);
        TimestampedValue<NitzData> emulatorNitzSignal = new TimestampedValue<>(
                originalNitzSignal.getReferenceTimeMillis(), emulatorNitzData);

        // Simulate receiving the emulator NITZ signal.
        script.nitzReceived(emulatorNitzSignal)
                .verifyTimeSuggestedAndZoneSetAndReset(
                        scenario.getNitzSignal(), emulatorTimeZoneId);

        // Check NitzStateMachine state.
        assertTrue(mNitzStateMachine.getNitzTimeZoneDetectionSuccessful());
        assertEquals(emulatorNitzSignal.getValue(), mNitzStateMachine.getCachedNitzData());
        assertEquals(emulatorTimeZoneId, mNitzStateMachine.getSavedTimeZoneId());
    }

    @Test
    public void test_emptyCountryStringUsTime_countryReceivedFirst() throws Exception {
        Scenario scenario = UNIQUE_US_ZONE_SCENARIO;
        Device device = new DeviceBuilder()
                .setClocksFromScenario(scenario)
                .setTimeZoneDetectionEnabled(true)
                .setTimeZoneSettingInitialized(true)
                .initialize();
        Script script = new Script(device);

        String expectedZoneId = checkNitzOnlyLookupIsAmbiguousAndReturnZoneId(scenario);

        // Nothing should be set. The country is not valid.
        script.countryReceived("").verifyNothingWasSetAndReset();

        // Check NitzStateMachine state.
        assertFalse(mNitzStateMachine.getNitzTimeZoneDetectionSuccessful());
        assertNull(mNitzStateMachine.getCachedNitzData());
        assertNull(mNitzStateMachine.getSavedTimeZoneId());

        // Simulate receiving the NITZ signal.
        script.nitzReceived(scenario.getNitzSignal())
                .verifyTimeSuggestedAndZoneSetAndReset(scenario.getNitzSignal(), expectedZoneId);

        // Check NitzStateMachine state.
        assertTrue(mNitzStateMachine.getNitzTimeZoneDetectionSuccessful());
        assertEquals(scenario.getNitzSignal().getValue(), mNitzStateMachine.getCachedNitzData());
        assertEquals(expectedZoneId, mNitzStateMachine.getSavedTimeZoneId());
    }

    @Test
    public void test_emptyCountryStringUsTime_nitzReceivedFirst() throws Exception {
        Scenario scenario = UNIQUE_US_ZONE_SCENARIO;
        Device device = new DeviceBuilder()
                .setClocksFromScenario(scenario)
                .setTimeZoneDetectionEnabled(true)
                .setTimeZoneSettingInitialized(true)
                .initialize();
        Script script = new Script(device);

        String expectedZoneId = checkNitzOnlyLookupIsAmbiguousAndReturnZoneId(scenario);

        // Simulate receiving the NITZ signal.
        script.nitzReceived(scenario.getNitzSignal())
                .verifyOnlyTimeWasSuggestedAndReset(scenario.getNitzSignal());

        // Check NitzStateMachine state.
        assertFalse(mNitzStateMachine.getNitzTimeZoneDetectionSuccessful());
        assertEquals(scenario.getNitzSignal().getValue(), mNitzStateMachine.getCachedNitzData());
        assertNull(mNitzStateMachine.getSavedTimeZoneId());

        // The time zone should be set (but the country is not valid so it's unlikely to be
        // correct).
        script.countryReceived("").verifyOnlyTimeZoneWasSetAndReset(expectedZoneId);

        // Check NitzStateMachine state.
        assertTrue(mNitzStateMachine.getNitzTimeZoneDetectionSuccessful());
        assertEquals(scenario.getNitzSignal().getValue(), mNitzStateMachine.getCachedNitzData());
        assertEquals(expectedZoneId, mNitzStateMachine.getSavedTimeZoneId());
    }

    /**
     * Asserts a test scenario has the properties we expect for NITZ-only lookup. There are
     * usually multiple zones that will share the same UTC offset so we get a low quality / low
     * confidence answer, but the zone we find should at least have the correct offset.
     */
    private String checkNitzOnlyLookupIsAmbiguousAndReturnZoneId(Scenario scenario) {
        OffsetResult result =
                mRealTimeZoneLookupHelper.lookupByNitz(scenario.getNitzSignal().getValue());
        String expectedZoneId = result.zoneId;
        // All our scenarios should return multiple matches. The only cases where this wouldn't be
        // true are places that use offsets like XX:15, XX:30 and XX:45.
        assertFalse(result.isOnlyMatch);
        assertSameOffset(scenario.getActualTimeMillis(), expectedZoneId, scenario.getTimeZoneId());
        return expectedZoneId;
    }

    private static void assertSameOffset(long timeMillis, String zoneId1, String zoneId2) {
        assertEquals(TimeZone.getTimeZone(zoneId1).getOffset(timeMillis),
                TimeZone.getTimeZone(zoneId2).getOffset(timeMillis));
    }

    private static long createUtcTime(int year, int monthInYear, int day, int hourOfDay, int minute,
            int second) {
        Calendar cal = new GregorianCalendar(TimeZone.getTimeZone("Etc/UTC"));
        cal.clear();
        cal.set(year, monthInYear - 1, day, hourOfDay, minute, second);
        return cal.getTimeInMillis();
    }

    /**
     * A helper class for common test operations involving a device.
     */
    class Script {
        private final Device mDevice;

        Script(Device device) {
            this.mDevice = device;
        }

        Script countryReceived(String countryIsoCode) {
            mDevice.networkCountryKnown(countryIsoCode);
            return this;
        }

        Script nitzReceived(TimestampedValue<NitzData> nitzSignal) {
            mDevice.nitzSignalReceived(nitzSignal);
            return this;
        }

        Script verifyNothingWasSetAndReset() {
            mDevice.verifyTimeZoneWasNotSet();
            mDevice.verifyTimeWasNotSuggested();
            mDevice.checkNoUnverifiedSetOperations();
            mDevice.resetInvocations();
            return this;
        }

        Script verifyOnlyTimeZoneWasSetAndReset(String timeZoneId, int times) {
            mDevice.verifyTimeZoneWasSet(timeZoneId, times);
            mDevice.verifyTimeWasNotSuggested();
            mDevice.checkNoUnverifiedSetOperations();
            mDevice.resetInvocations();
            return this;
        }

        Script verifyOnlyTimeZoneWasSetAndReset(String timeZoneId) {
            return verifyOnlyTimeZoneWasSetAndReset(timeZoneId, 1);
        }

        Script verifyOnlyTimeWasSuggestedAndReset(TimestampedValue<NitzData> nitzSignal) {
            mDevice.verifyTimeZoneWasNotSet();

            TimestampedValue<Long> time = new TimestampedValue<>(
                    nitzSignal.getReferenceTimeMillis(),
                    nitzSignal.getValue().getCurrentTimeInMillis());
            mDevice.verifyTimeWasSuggested(time);
            mDevice.checkNoUnverifiedSetOperations();
            mDevice.resetInvocations();
            return this;
        }

        Script verifyTimeSuggestedAndZoneSetAndReset(
                TimestampedValue<NitzData> nitzSignal, String timeZoneId) {
            mDevice.verifyTimeZoneWasSet(timeZoneId);

            TimestampedValue<Long> time = new TimestampedValue<>(
                    nitzSignal.getReferenceTimeMillis(),
                    nitzSignal.getValue().getCurrentTimeInMillis());
            mDevice.verifyTimeWasSuggested(time);
            mDevice.checkNoUnverifiedSetOperations();
            mDevice.resetInvocations();
            return this;
        }

        Script reset() {
            mDevice.checkNoUnverifiedSetOperations();
            mDevice.resetInvocations();
            return this;
        }
    }

    /**
     * An abstraction of a device for use in telephony time zone detection tests. It can be used to
     * retrieve device state, modify device state and verify changes.
     */
    class Device {

        private final long mInitialSystemClockMillis;
        private final long mInitialRealtimeMillis;
        private final boolean mTimeZoneDetectionEnabled;
        private final boolean mTimeZoneSettingInitialized;

        Device(long initialSystemClockMillis, long initialRealtimeMillis,
                boolean timeZoneDetectionEnabled, boolean timeZoneSettingInitialized) {
            mInitialSystemClockMillis = initialSystemClockMillis;
            mInitialRealtimeMillis = initialRealtimeMillis;
            mTimeZoneDetectionEnabled = timeZoneDetectionEnabled;
            mTimeZoneSettingInitialized = timeZoneSettingInitialized;
        }

        void initialize() {
            // Set initial configuration.
            when(mDeviceState.getIgnoreNitz()).thenReturn(false);
            when(mDeviceState.getNitzUpdateDiffMillis()).thenReturn(2000);
            when(mDeviceState.getNitzUpdateSpacingMillis()).thenReturn(1000 * 60 * 10);

            // Simulate the country not being known.
            when(mDeviceState.getNetworkCountryIsoForPhone()).thenReturn("");

            when(mTimeServiceHelper.elapsedRealtime()).thenReturn(mInitialRealtimeMillis);
            when(mTimeServiceHelper.currentTimeMillis()).thenReturn(mInitialSystemClockMillis);
            when(mTimeServiceHelper.isTimeZoneDetectionEnabled())
                    .thenReturn(mTimeZoneDetectionEnabled);
            when(mTimeServiceHelper.isTimeZoneSettingInitialized())
                    .thenReturn(mTimeZoneSettingInitialized);
        }

        void networkCountryKnown(String countryIsoCode) {
            when(mDeviceState.getNetworkCountryIsoForPhone()).thenReturn(countryIsoCode);
            mNitzStateMachine.handleNetworkCountryCodeSet(true);
        }

        void nitzSignalReceived(TimestampedValue<NitzData> nitzSignal) {
            mNitzStateMachine.handleNitzReceived(nitzSignal);
        }

        void verifyTimeZoneWasNotSet() {
            verify(mTimeServiceHelper, times(0)).setDeviceTimeZone(any(String.class));
        }

        void verifyTimeZoneWasSet(String timeZoneId) {
            verifyTimeZoneWasSet(timeZoneId, 1 /* times */);
        }

        void verifyTimeZoneWasSet(String timeZoneId, int times) {
            verify(mTimeServiceHelper, times(times)).setDeviceTimeZone(timeZoneId);
        }

        void verifyTimeWasNotSuggested() {
            verify(mTimeServiceHelper, times(0)).suggestDeviceTime(any());
        }

        void verifyTimeWasSuggested(TimestampedValue<Long> expectedTime) {
            verify(mTimeServiceHelper, times(1)).suggestDeviceTime(eq(expectedTime));
        }

        /**
         * Used after calling verify... methods to reset expectations.
         */
        void resetInvocations() {
            clearInvocations(mTimeServiceHelper);
        }

        void checkNoUnverifiedSetOperations() {
            NewNitzStateMachineTest.checkNoUnverifiedSetOperations(mTimeServiceHelper);
        }
    }

    /** A class used to construct a Device. */
    class DeviceBuilder {

        private long mInitialSystemClock;
        private long mInitialRealtimeMillis;
        private boolean mTimeZoneDetectionEnabled;
        private boolean mTimeZoneSettingInitialized;

        Device initialize() {
            Device device = new Device(mInitialSystemClock, mInitialRealtimeMillis,
                    mTimeZoneDetectionEnabled, mTimeZoneSettingInitialized);
            device.initialize();
            return device;
        }

        DeviceBuilder setTimeZoneDetectionEnabled(boolean enabled) {
            mTimeZoneDetectionEnabled = enabled;
            return this;
        }

        DeviceBuilder setTimeZoneSettingInitialized(boolean initialized) {
            mTimeZoneSettingInitialized = initialized;
            return this;
        }

        DeviceBuilder setClocksFromScenario(Scenario scenario) {
            mInitialRealtimeMillis = scenario.getInitialRealTimeMillis();
            mInitialSystemClock = scenario.getInitialSystemClockMillis();
            return this;
        }
    }

    /**
     * A scenario used during tests. Describes a fictional reality.
     */
    static class Scenario {

        private final long mInitialDeviceSystemClockMillis;
        private final long mInitialDeviceRealtimeMillis;
        private final long mActualTimeMillis;
        private final TimeZone mZone;
        private final String mNetworkCountryIsoCode;

        private TimestampedValue<NitzData> mNitzSignal;

        Scenario(long initialDeviceSystemClock, long elapsedRealtime, long timeMillis,
                String zoneId, String countryIsoCode) {
            mInitialDeviceSystemClockMillis = initialDeviceSystemClock;
            mActualTimeMillis = timeMillis;
            mInitialDeviceRealtimeMillis = elapsedRealtime;
            mZone = TimeZone.getTimeZone(zoneId);
            mNetworkCountryIsoCode = countryIsoCode;
        }

        TimestampedValue<NitzData> getNitzSignal() {
            if (mNitzSignal == null) {
                int[] offsets = new int[2];
                mZone.getOffset(mActualTimeMillis, false /* local */, offsets);
                int zoneOffsetMillis = offsets[0] + offsets[1];
                NitzData nitzData = NitzData.createForTests(
                        zoneOffsetMillis, offsets[1], mActualTimeMillis,
                        null /* emulatorHostTimeZone */);
                mNitzSignal = new TimestampedValue<>(mInitialDeviceRealtimeMillis, nitzData);
            }
            return mNitzSignal;
        }

        long getInitialRealTimeMillis() {
            return mInitialDeviceRealtimeMillis;
        }

        long getInitialSystemClockMillis() {
            return mInitialDeviceSystemClockMillis;
        }

        String getNetworkCountryIsoCode() {
            return mNetworkCountryIsoCode;
        }

        String getTimeZoneId() {
            return mZone.getID();
        }

        long getActualTimeMillis() {
            return mActualTimeMillis;
        }

        static class Builder {

            private long mInitialDeviceSystemClockMillis;
            private long mInitialDeviceRealtimeMillis;
            private long mActualTimeMillis;
            private String mZoneId;
            private String mCountryIsoCode;

            Builder setInitialDeviceSystemClockUtc(int year, int monthInYear, int day,
                    int hourOfDay, int minute, int second) {
                mInitialDeviceSystemClockMillis = createUtcTime(year, monthInYear, day, hourOfDay,
                        minute, second);
                return this;
            }

            Builder setInitialDeviceRealtimeMillis(long realtimeMillis) {
                mInitialDeviceRealtimeMillis = realtimeMillis;
                return this;
            }

            Builder setActualTimeUtc(int year, int monthInYear, int day, int hourOfDay,
                    int minute, int second) {
                mActualTimeMillis = createUtcTime(year, monthInYear, day, hourOfDay, minute,
                        second);
                return this;
            }

            Builder setTimeZone(String zoneId) {
                mZoneId = zoneId;
                return this;
            }

            Builder setCountryIso(String isoCode) {
                mCountryIsoCode = isoCode;
                return this;
            }

            Scenario build() {
                return new Scenario(mInitialDeviceSystemClockMillis, mInitialDeviceRealtimeMillis,
                        mActualTimeMillis, mZoneId, mCountryIsoCode);
            }
        }
    }

    /**
     * Confirms all mTimeServiceHelper side effects were verified.
     */
    private static void checkNoUnverifiedSetOperations(NewTimeServiceHelper mTimeServiceHelper) {
        // We don't care about current auto time / time zone state retrievals / listening so we can
        // use "at least 0" times to indicate they don't matter.
        verify(mTimeServiceHelper, atLeast(0)).setListener(any());
        verify(mTimeServiceHelper, atLeast(0)).isTimeZoneDetectionEnabled();
        verify(mTimeServiceHelper, atLeast(0)).isTimeZoneSettingInitialized();
        verify(mTimeServiceHelper, atLeast(0)).elapsedRealtime();
        verify(mTimeServiceHelper, atLeast(0)).currentTimeMillis();
        verifyNoMoreInteractions(mTimeServiceHelper);
    }
}
