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

package com.android.internal.telephony;

import android.os.Parcel;
import android.service.carrier.CarrierIdentifier;
import android.telephony.CarrierRestrictionRules;
import android.test.AndroidTestCase;
import android.test.suitebuilder.annotation.SmallTest;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

/** Unit tests for {@link CarrierRestrictionRules}. */

public class CarrierRestrictionRulesTest extends AndroidTestCase {

    private static final String MCC1 = "110";
    private static final String MNC1 = "210";
    private static final String MCC2 = "120";
    private static final String MNC2 = "220";
    private static final String MCC3 = "130";
    private static final String MNC3 = "230";
    private static final String MCC4 = "140";
    private static final String MNC4 = "240";
    private static final String MCC5 = "150";
    private static final String MNC5 = "250";
    private static final String MCC6 = "360";
    private static final String MNC6 = "380";
    private static final String MCC1_WILDCHAR = "3??";
    private static final String MNC1_WILDCHAR = "???";
    private static final String MCC2_WILDCHAR = "31?";
    private static final String MNC2_WILDCHAR = "?0";
    private static final String SPN = "spn";
    private static final String IMSI = "123456";
    private static final String GID1 = "80";
    private static final String GID2 = "506090";

    @SmallTest
    public void testBuilderAllowedAndExcludedCarriers() {
        ArrayList<CarrierIdentifier> allowedCarriers = new ArrayList<>();
        allowedCarriers.add(new CarrierIdentifier(MCC1, MNC1, null, null, null, null));
        allowedCarriers.add(new CarrierIdentifier(MCC2, MNC2, null, null, null, null));

        ArrayList<CarrierIdentifier> excludedCarriers = new ArrayList<>();
        excludedCarriers.add(new CarrierIdentifier(MCC2, MNC2, null, null, GID1, null));

        CarrierRestrictionRules rules = CarrierRestrictionRules.newBuilder()
                .setAllowedCarriers(allowedCarriers)
                .setExcludedCarriers(excludedCarriers)
                .build();

        assertEquals(false, rules.isAllCarriersAllowed());
        assertTrue(rules.getAllowedCarriers().equals(allowedCarriers));
        assertTrue(rules.getExcludedCarriers().equals(excludedCarriers));
        assertEquals(CarrierRestrictionRules.CARRIER_RESTRICTION_DEFAULT_NOT_ALLOWED,
                rules.getDefaultCarrierRestriction());
    }

    @SmallTest
    public void testBuilderEmptyLists() {
        ArrayList<CarrierIdentifier> emptyCarriers = new ArrayList<>();

        CarrierRestrictionRules rules = CarrierRestrictionRules.newBuilder().build();

        assertEquals(false, rules.isAllCarriersAllowed());
        assertTrue(rules.getAllowedCarriers().equals(emptyCarriers));
        assertTrue(rules.getExcludedCarriers().equals(emptyCarriers));
        assertEquals(CarrierRestrictionRules.CARRIER_RESTRICTION_DEFAULT_NOT_ALLOWED,
                rules.getDefaultCarrierRestriction());
    }

    @SmallTest
    public void testBuilderWildCharacter() {
        ArrayList<CarrierIdentifier> allowedCarriers = new ArrayList<>();
        allowedCarriers.add(new CarrierIdentifier(MCC1_WILDCHAR, MNC1_WILDCHAR, null, null,
                null, null));

        ArrayList<CarrierIdentifier> excludedCarriers = new ArrayList<>();
        excludedCarriers.add(new CarrierIdentifier(MCC2_WILDCHAR, MNC2_WILDCHAR, null, null,
                GID1, null));

        CarrierRestrictionRules rules = CarrierRestrictionRules.newBuilder()
                .setAllowedCarriers(allowedCarriers)
                .setExcludedCarriers(excludedCarriers)
                .build();

        assertEquals(false, rules.isAllCarriersAllowed());
        assertTrue(rules.getAllowedCarriers().equals(allowedCarriers));
        assertTrue(rules.getExcludedCarriers().equals(excludedCarriers));
        assertEquals(CarrierRestrictionRules.CARRIER_RESTRICTION_DEFAULT_NOT_ALLOWED,
                rules.getDefaultCarrierRestriction());
    }

    @SmallTest
    public void testBuilderDefaultAllowed() {
        ArrayList<CarrierIdentifier> allowedCarriers = new ArrayList<>();
        allowedCarriers.add(new CarrierIdentifier(MCC1, MNC1, null, null, null, null));
        allowedCarriers.add(new CarrierIdentifier(MCC2, MNC2, null, null, null, null));

        ArrayList<CarrierIdentifier> excludedCarriers = new ArrayList<>();
        excludedCarriers.add(new CarrierIdentifier(MCC2, MNC2, null, null, GID1, null));

        CarrierRestrictionRules rules = CarrierRestrictionRules.newBuilder()
                .setAllowedCarriers(allowedCarriers)
                .setExcludedCarriers(excludedCarriers)
                .setDefaultCarrierRestriction(
                    CarrierRestrictionRules.CARRIER_RESTRICTION_DEFAULT_ALLOWED)
                .build();

        assertEquals(false, rules.isAllCarriersAllowed());
        assertTrue(rules.getAllowedCarriers().equals(allowedCarriers));
        assertTrue(rules.getExcludedCarriers().equals(excludedCarriers));
        assertEquals(CarrierRestrictionRules.CARRIER_RESTRICTION_DEFAULT_ALLOWED,
                rules.getDefaultCarrierRestriction());
    }

    @SmallTest
    public void testBuilderAllCarriersAllowed() {
        ArrayList<CarrierIdentifier> allowedCarriers = new ArrayList<>();
        ArrayList<CarrierIdentifier> excludedCarriers = new ArrayList<>();

        CarrierRestrictionRules rules = CarrierRestrictionRules.newBuilder()
                .setAllCarriersAllowed()
                .build();

        assertEquals(true, rules.isAllCarriersAllowed());
        assertTrue(rules.getAllowedCarriers().equals(allowedCarriers));
        assertTrue(rules.getExcludedCarriers().equals(excludedCarriers));
        assertEquals(CarrierRestrictionRules.CARRIER_RESTRICTION_DEFAULT_ALLOWED,
                rules.getDefaultCarrierRestriction());
    }

    @SmallTest
    public void testParcel() {
        ArrayList<CarrierIdentifier> allowedCarriers = new ArrayList<>();
        allowedCarriers.add(new CarrierIdentifier(MCC1, MNC1, null, null, null, null));
        allowedCarriers.add(new CarrierIdentifier(MCC2, MNC2, null, null, null, null));

        ArrayList<CarrierIdentifier> excludedCarriers = new ArrayList<>();
        excludedCarriers.add(new CarrierIdentifier(MCC2, MNC2, null, null, GID1, null));

        CarrierRestrictionRules rules = CarrierRestrictionRules.newBuilder()
                .setAllowedCarriers(allowedCarriers)
                .setExcludedCarriers(excludedCarriers)
                .setDefaultCarrierRestriction(
                    CarrierRestrictionRules.CARRIER_RESTRICTION_DEFAULT_NOT_ALLOWED)
                .setMultiSimPolicy(
                    CarrierRestrictionRules.MULTISIM_POLICY_ONE_VALID_SIM_MUST_BE_PRESENT)
                .build();

        Parcel p = Parcel.obtain();
        rules.writeToParcel(p, 0);
        p.setDataPosition(0);

        CarrierRestrictionRules newRules = CarrierRestrictionRules.CREATOR.createFromParcel(p);

        assertEquals(false, rules.isAllCarriersAllowed());
        assertTrue(allowedCarriers.equals(newRules.getAllowedCarriers()));
        assertTrue(excludedCarriers.equals(newRules.getExcludedCarriers()));
        assertEquals(rules.getDefaultCarrierRestriction(),
                newRules.getDefaultCarrierRestriction());
        assertEquals(rules.getMultiSimPolicy(),
                CarrierRestrictionRules.MULTISIM_POLICY_ONE_VALID_SIM_MUST_BE_PRESENT);
    }

    @SmallTest
    public void testDefaultMultiSimPolicy() {
        CarrierRestrictionRules rules = CarrierRestrictionRules.newBuilder().build();

        assertEquals(CarrierRestrictionRules.MULTISIM_POLICY_NONE, rules.getMultiSimPolicy());
    }

    @SmallTest
    public void testTestCarrierId_allCarriersAllowed() {
        // Carrier lock configuration
        CarrierRestrictionRules rules = CarrierRestrictionRules.newBuilder()
                .setAllCarriersAllowed()
                .build();

        // Carrier list for test
        ArrayList<CarrierIdentifier> list = new ArrayList<>();
        list.add(new CarrierIdentifier(MCC1, MNC1, null, null, null, null));
        list.add(new CarrierIdentifier(MCC1, MNC1, SPN, null, GID1, GID2));
        list.add(new CarrierIdentifier(MCC2, MNC2, SPN, null, null, null));
        list.add(new CarrierIdentifier(MCC3, MNC3, null, IMSI, null, null));
        list.add(new CarrierIdentifier(MCC4, MNC4, null, null, GID1, null));
        list.add(new CarrierIdentifier(MCC6, MNC1, null, null, null, null));
        list.add(new CarrierIdentifier(MCC1, MNC6, null, null, null, null));
        list.add(new CarrierIdentifier(MCC2, MNC2, null, null, null, null));
        list.add(new CarrierIdentifier(MCC3, MNC3, null, null, null, null));
        list.add(new CarrierIdentifier(MCC4, MNC4, null, null, null, null));
        list.add(new CarrierIdentifier(MCC5, MNC5, null, null, null, null));

        List<Boolean> result = rules.areCarrierIdentifiersAllowed(list);

        List<Boolean> expected =
                Arrays.asList(true, true, true, true, true, true, true, true, true, true, true);
        assertTrue(result.equals(expected));
    }

    @SmallTest
    public void testTestCarrierId_defaultNotAllowed() {
        // Carrier lock configuration
        ArrayList<CarrierIdentifier> allowedCarriers = new ArrayList<>();
        allowedCarriers.add(new CarrierIdentifier(MCC1, MNC1, null, null, null, null));
        allowedCarriers.add(new CarrierIdentifier(MCC2, MNC2, SPN, null, null, null));
        allowedCarriers.add(new CarrierIdentifier(MCC3, MNC3, null, IMSI, null, null));
        allowedCarriers.add(new CarrierIdentifier(MCC4, MNC4, null, null, GID1, null));
        allowedCarriers.add(new CarrierIdentifier(MCC1_WILDCHAR, MNC1, null, null, null, null));
        allowedCarriers.add(new CarrierIdentifier(MCC1, MNC1_WILDCHAR, null, null, null, null));

        CarrierRestrictionRules rules = CarrierRestrictionRules.newBuilder()
                .setAllowedCarriers(allowedCarriers)
                .setDefaultCarrierRestriction(
                    CarrierRestrictionRules.CARRIER_RESTRICTION_DEFAULT_NOT_ALLOWED)
                .build();

        // Carrier list for test
        ArrayList<CarrierIdentifier> list = new ArrayList<>();
        // Allowed carriers
        list.add(new CarrierIdentifier(MCC1, MNC1, null, null, null, null));
        list.add(new CarrierIdentifier(MCC1, MNC1, SPN, null, GID1, GID2));
        list.add(new CarrierIdentifier(MCC2, MNC2, SPN, null, null, null));
        list.add(new CarrierIdentifier(MCC3, MNC3, null, IMSI, null, null));
        list.add(new CarrierIdentifier(MCC4, MNC4, null, null, GID1, null));
        list.add(new CarrierIdentifier(MCC6, MNC1, null, null, null, null));
        list.add(new CarrierIdentifier(MCC1, MNC6, null, null, null, null));
        // Excluded carriers
        list.add(new CarrierIdentifier(MCC2, MNC2, null, null, null, null));
        list.add(new CarrierIdentifier(MCC3, MNC3, null, null, null, null));
        list.add(new CarrierIdentifier(MCC4, MNC4, null, null, null, null));
        list.add(new CarrierIdentifier(MCC5, MNC5, null, null, null, null));

        List<Boolean> result = rules.areCarrierIdentifiersAllowed(list);

        List<Boolean> expected =
                Arrays.asList(true, true, true, true, true, true, true,
                    false, false, false, false);
        assertTrue(result.equals(expected));
    }

    @SmallTest
    public void testTestCarrierId_defaultAllowed() {
        // Carrier lock configuration
        ArrayList<CarrierIdentifier> allowedCarriers = new ArrayList<>();
        ArrayList<CarrierIdentifier> excludedCarriers = new ArrayList<>();
        excludedCarriers.add(new CarrierIdentifier(MCC1, MNC1_WILDCHAR, null, null, null, null));
        allowedCarriers.add(new CarrierIdentifier(MCC1, MNC1, null, null, null, null));
        allowedCarriers.add(new CarrierIdentifier(MCC1, MNC2, null, null, null, null));

        CarrierRestrictionRules rules = CarrierRestrictionRules.newBuilder()
                .setAllowedCarriers(allowedCarriers)
                .setExcludedCarriers(excludedCarriers)
                .setDefaultCarrierRestriction(
                    CarrierRestrictionRules.CARRIER_RESTRICTION_DEFAULT_ALLOWED)
                .build();

        // Carrier list for test
        ArrayList<CarrierIdentifier> list = new ArrayList<>();
        // Allowed carriers
        list.add(new CarrierIdentifier(MCC1, MNC1, null, null, null, null));
        list.add(new CarrierIdentifier(MCC1, MNC2, null, null, null, null));
        list.add(new CarrierIdentifier(MCC2, MNC2, null, null, null, null));
        list.add(new CarrierIdentifier(MCC3, MNC3, null, null, null, null));
        // Excluded carriers
        list.add(new CarrierIdentifier(MCC1, MNC3, null, null, null, null));
        list.add(new CarrierIdentifier(MCC1, MNC4, null, null, null, null));

        List<Boolean> result = rules.areCarrierIdentifiersAllowed(list);

        List<Boolean> expected = Arrays.asList(true, true, true, true, false, false);
        assertTrue(result.equals(expected));
    }

    @SmallTest
    public void testTestCarrierId_OneValidCardMustBePresentPolicy_CardPresent() {
        // Carrier lock configuration
        ArrayList<CarrierIdentifier> allowedCarriers = new ArrayList<>();
        allowedCarriers.add(new CarrierIdentifier(MCC1, MNC1, null, null, null, null));
        allowedCarriers.add(new CarrierIdentifier(MCC2, MNC2, null, null, null, null));

        CarrierRestrictionRules rules = CarrierRestrictionRules.newBuilder()
                .setAllowedCarriers(allowedCarriers)
                .setMultiSimPolicy(
                    CarrierRestrictionRules.MULTISIM_POLICY_ONE_VALID_SIM_MUST_BE_PRESENT)
                .build();

        // Carrier list for test: this contains at least one valid carrier and one non valid
        // carrier to verify that multisim policy is applied correctly.
        ArrayList<CarrierIdentifier> list = new ArrayList<>();
        list.add(new CarrierIdentifier(MCC1, MNC1, SPN, null, GID1, GID2));  // allowed carrier
        list.add(new CarrierIdentifier(MCC3, MNC3, null, null, null, null));
        list.add(new CarrierIdentifier(MCC4, MNC4, null, null, null, null));
        list.add(new CarrierIdentifier(MCC5, MNC5, null, null, null, null));

        List<Boolean> result = rules.areCarrierIdentifiersAllowed(list);

        List<Boolean> expected = Arrays.asList(true, true, true, true);
        assertTrue(result.equals(expected));
    }

    @SmallTest
    public void testTestCarrierId_OneValidCardMustBePresentPolicy_CardNotPresent() {
        // Carrier lock configuration
        ArrayList<CarrierIdentifier> allowedCarriers = new ArrayList<>();
        allowedCarriers.add(new CarrierIdentifier(MCC1, MNC1, null, null, null, null));
        allowedCarriers.add(new CarrierIdentifier(MCC2, MNC2, null, null, null, null));

        CarrierRestrictionRules rules = CarrierRestrictionRules.newBuilder()
                .setAllowedCarriers(allowedCarriers)
                .setMultiSimPolicy(
                    CarrierRestrictionRules.MULTISIM_POLICY_ONE_VALID_SIM_MUST_BE_PRESENT)
                .build();

        // Carrier list for test. This does not contain any valid carrier to verify that multisim
        // policy is applied correctly.
        ArrayList<CarrierIdentifier> list = new ArrayList<>();
        list.add(new CarrierIdentifier(MCC3, MNC3, null, null, null, null));
        list.add(new CarrierIdentifier(MCC4, MNC4, null, null, null, null));
        list.add(new CarrierIdentifier(MCC5, MNC5, null, null, null, null));

        List<Boolean> result = rules.areCarrierIdentifiersAllowed(list);

        List<Boolean> expected = Arrays.asList(false, false, false);
        assertTrue(result.equals(expected));
    }
}
