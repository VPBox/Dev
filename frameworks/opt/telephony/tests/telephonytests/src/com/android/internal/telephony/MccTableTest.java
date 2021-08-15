/*
 * Copyright (C) 2006 The Android Open Source Project
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

import android.test.AndroidTestCase;
import android.test.suitebuilder.annotation.SmallTest;

import org.junit.Ignore;

import java.util.Locale;

// TODO try using InstrumentationRegistry.getContext() instead of the default
// AndroidTestCase context
public class MccTableTest extends AndroidTestCase {
    private final static String LOG_TAG = "GSM";

    @SmallTest
    @Ignore
    public void testTimeZone() throws Exception {
        assertEquals("Europe/Paris", MccTable.defaultTimeZoneForMcc(208));
        assertEquals("Europe/Vienna", MccTable.defaultTimeZoneForMcc(232));
        assertEquals("Africa/Johannesburg", MccTable.defaultTimeZoneForMcc(655));
        assertEquals("Asia/Tokyo", MccTable.defaultTimeZoneForMcc(440));
        assertEquals("Asia/Tokyo", MccTable.defaultTimeZoneForMcc(441));
        assertEquals("Asia/Singapore", MccTable.defaultTimeZoneForMcc(525));
        assertEquals("Europe/Stockholm", MccTable.defaultTimeZoneForMcc(240));

        /* A test for the special handling for MCC 505. http://b/33228250. */
        assertEquals("Australia/Sydney", MccTable.defaultTimeZoneForMcc(505));
        assertEquals(null, MccTable.defaultTimeZoneForMcc(0));    // mcc not defined, hence default
        assertEquals(null, MccTable.defaultTimeZoneForMcc(2000)); // mcc not defined, hence default
    }

    @SmallTest
    @Ignore
    public void testCountryCode() throws Exception {
        assertEquals("lu", MccTable.countryCodeForMcc(270));
        assertEquals("gr", MccTable.countryCodeForMcc(202));
        assertEquals("fk", MccTable.countryCodeForMcc(750));
        assertEquals("mg", MccTable.countryCodeForMcc(646));
        assertEquals("us", MccTable.countryCodeForMcc(314));
        assertEquals("", MccTable.countryCodeForMcc(300));  // mcc not defined, hence default
        assertEquals("", MccTable.countryCodeForMcc(0));    // mcc not defined, hence default
        assertEquals("", MccTable.countryCodeForMcc(2000)); // mcc not defined, hence default
    }

    @SmallTest
    @Ignore
    public void testLang() throws Exception {
        assertEquals("en", MccTable.defaultLanguageForMcc(311));
        assertEquals("de", MccTable.defaultLanguageForMcc(232));
        assertEquals("cs", MccTable.defaultLanguageForMcc(230));
        assertEquals("nl", MccTable.defaultLanguageForMcc(204));
        assertEquals("is", MccTable.defaultLanguageForMcc(274));
        assertEquals(null, MccTable.defaultLanguageForMcc(0));    // mcc not defined, hence default
        assertEquals(null, MccTable.defaultLanguageForMcc(2000)); // mcc not defined, hence default
    }

    @SmallTest
    @Ignore
    public void testLang_India() throws Exception {
        assertEquals("en", MccTable.defaultLanguageForMcc(404));
        assertEquals("en", MccTable.defaultLanguageForMcc(405));
        assertEquals("en", MccTable.defaultLanguageForMcc(406));
    }

    @SmallTest
    @Ignore
    public void testLocale() throws Exception {
        assertEquals(Locale.forLanguageTag("en-CA"),
                MccTable.getLocaleFromMcc(getContext(), 302, null));
        assertEquals(Locale.forLanguageTag("en-GB"),
                MccTable.getLocaleFromMcc(getContext(), 234, null));
        assertEquals(Locale.forLanguageTag("en-US"),
                MccTable.getLocaleFromMcc(getContext(), 0, "en"));
        assertEquals(Locale.forLanguageTag("zh-HK"),
                MccTable.getLocaleFromMcc(getContext(), 454, null));
        assertEquals(Locale.forLanguageTag("en-HK"),
                MccTable.getLocaleFromMcc(getContext(), 454, "en"));
        assertEquals(Locale.forLanguageTag("zh-TW"),
                MccTable.getLocaleFromMcc(getContext(), 466, null));
    }

    @SmallTest
    @Ignore
    public void testSmDigits() throws Exception {
        assertEquals(3, MccTable.smallestDigitsMccForMnc(312));
        assertEquals(2, MccTable.smallestDigitsMccForMnc(430));
        assertEquals(3, MccTable.smallestDigitsMccForMnc(365));
        assertEquals(2, MccTable.smallestDigitsMccForMnc(536));
        assertEquals(2, MccTable.smallestDigitsMccForMnc(352));  // sd not defined, hence default
        assertEquals(2, MccTable.smallestDigitsMccForMnc(0));    // mcc not defined, hence default
        assertEquals(2, MccTable.smallestDigitsMccForMnc(2000)); // mcc not defined, hence default
    }
}
