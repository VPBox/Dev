/*
 * Copyright (C) 2012 The Android Open Source Project
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

import static android.telephony.SmsManager.SMS_CATEGORY_FREE_SHORT_CODE;
import static android.telephony.SmsManager.SMS_CATEGORY_NOT_SHORT_CODE;
import static android.telephony.SmsManager.SMS_CATEGORY_POSSIBLE_PREMIUM_SHORT_CODE;
import static android.telephony.SmsManager.SMS_CATEGORY_PREMIUM_SHORT_CODE;
import static android.telephony.SmsManager.SMS_CATEGORY_STANDARD_SHORT_CODE;

import static org.junit.Assert.assertEquals;

import android.os.Looper;

import org.junit.Ignore;

/**
 * Test cases for SMS short code pattern matching in SmsUsageMonitor.
 */
public class SmsUsageMonitorShortCodeTest {

    private static final class ShortCodeTest {
        final String countryIso;
        final String address;
        final int category;

        ShortCodeTest(String countryIso, String destAddress, int category) {
            this.countryIso = countryIso;
            this.address = destAddress;
            this.category = category;
        }
    }

    /**
     * List of short code test cases.
     */
    private static final ShortCodeTest[] sShortCodeTests = new ShortCodeTest[] {
            new ShortCodeTest("al", "112", SMS_CATEGORY_NOT_SHORT_CODE),
            new ShortCodeTest("al", "4321", SMS_CATEGORY_NOT_SHORT_CODE),
            new ShortCodeTest("al", "54321", SMS_CATEGORY_POSSIBLE_PREMIUM_SHORT_CODE),
            new ShortCodeTest("al", "15191", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("al", "55500", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("al", "55600", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("al", "654321", SMS_CATEGORY_NOT_SHORT_CODE),

            new ShortCodeTest("am", "112", SMS_CATEGORY_NOT_SHORT_CODE),
            new ShortCodeTest("am", "101", SMS_CATEGORY_FREE_SHORT_CODE),
            new ShortCodeTest("am", "102", SMS_CATEGORY_FREE_SHORT_CODE),
            new ShortCodeTest("am", "103", SMS_CATEGORY_FREE_SHORT_CODE),
            new ShortCodeTest("am", "222", SMS_CATEGORY_POSSIBLE_PREMIUM_SHORT_CODE),
            new ShortCodeTest("am", "1111", SMS_CATEGORY_POSSIBLE_PREMIUM_SHORT_CODE),
            new ShortCodeTest("am", "9999", SMS_CATEGORY_POSSIBLE_PREMIUM_SHORT_CODE),
            new ShortCodeTest("am", "1121", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("am", "1141", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("am", "1161", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("am", "3024", SMS_CATEGORY_PREMIUM_SHORT_CODE),

            new ShortCodeTest("at", "112", SMS_CATEGORY_NOT_SHORT_CODE),
            new ShortCodeTest("at", "116117", SMS_CATEGORY_FREE_SHORT_CODE),
            new ShortCodeTest("at", "0901234", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("at", "0900666266", SMS_CATEGORY_PREMIUM_SHORT_CODE),

            new ShortCodeTest("au", "112", SMS_CATEGORY_NOT_SHORT_CODE),
            new ShortCodeTest("au", "180000", SMS_CATEGORY_NOT_SHORT_CODE),
            new ShortCodeTest("au", "190000", SMS_CATEGORY_POSSIBLE_PREMIUM_SHORT_CODE),
            new ShortCodeTest("au", "1900000", SMS_CATEGORY_NOT_SHORT_CODE),
            new ShortCodeTest("au", "19000000", SMS_CATEGORY_POSSIBLE_PREMIUM_SHORT_CODE),
            new ShortCodeTest("au", "19998882", SMS_CATEGORY_PREMIUM_SHORT_CODE),

            new ShortCodeTest("az", "112", SMS_CATEGORY_NOT_SHORT_CODE),
            new ShortCodeTest("az", "1234", SMS_CATEGORY_POSSIBLE_PREMIUM_SHORT_CODE),
            new ShortCodeTest("az", "12345", SMS_CATEGORY_POSSIBLE_PREMIUM_SHORT_CODE),
            new ShortCodeTest("az", "87744", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("az", "3301", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("az", "3302", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("az", "9012", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("az", "9014", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("az", "9394", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("az", "87744", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("az", "93101", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("az", "123456", SMS_CATEGORY_NOT_SHORT_CODE),

            new ShortCodeTest("be", "112", SMS_CATEGORY_NOT_SHORT_CODE),
            new ShortCodeTest("be", "116117", SMS_CATEGORY_FREE_SHORT_CODE),
            new ShortCodeTest("be", "567890", SMS_CATEGORY_NOT_SHORT_CODE),
            new ShortCodeTest("be", "8000", SMS_CATEGORY_FREE_SHORT_CODE),
            new ShortCodeTest("be", "6566", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("be", "7777", SMS_CATEGORY_PREMIUM_SHORT_CODE),

            new ShortCodeTest("bg", "112", SMS_CATEGORY_NOT_SHORT_CODE),
            new ShortCodeTest("bg", "116117", SMS_CATEGORY_FREE_SHORT_CODE),
            new ShortCodeTest("bg", "1234", SMS_CATEGORY_POSSIBLE_PREMIUM_SHORT_CODE),
            new ShortCodeTest("bg", "12345", SMS_CATEGORY_POSSIBLE_PREMIUM_SHORT_CODE),
            new ShortCodeTest("bg", "1816", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("bg", "1915", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("bg", "1916", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("bg", "1935", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("bg", "18423", SMS_CATEGORY_PREMIUM_SHORT_CODE),

            new ShortCodeTest("by", "112", SMS_CATEGORY_NOT_SHORT_CODE),
            new ShortCodeTest("by", "1234", SMS_CATEGORY_POSSIBLE_PREMIUM_SHORT_CODE),
            new ShortCodeTest("by", "3336", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("by", "5013", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("by", "5014", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("by", "7781", SMS_CATEGORY_PREMIUM_SHORT_CODE),

            new ShortCodeTest("ca", "911", SMS_CATEGORY_NOT_SHORT_CODE),
            new ShortCodeTest("ca", "+18005551234", SMS_CATEGORY_NOT_SHORT_CODE),
            new ShortCodeTest("ca", "8005551234", SMS_CATEGORY_NOT_SHORT_CODE),
            new ShortCodeTest("ca", "20000", SMS_CATEGORY_POSSIBLE_PREMIUM_SHORT_CODE),
            new ShortCodeTest("ca", "200000", SMS_CATEGORY_POSSIBLE_PREMIUM_SHORT_CODE),
            new ShortCodeTest("ca", "2000000", SMS_CATEGORY_NOT_SHORT_CODE),
            new ShortCodeTest("ca", "60999", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("ca", "88188", SMS_CATEGORY_PREMIUM_SHORT_CODE),

            new ShortCodeTest("ch", "112", SMS_CATEGORY_NOT_SHORT_CODE),
            new ShortCodeTest("ch", "123", SMS_CATEGORY_NOT_SHORT_CODE),
            new ShortCodeTest("ch", "234", SMS_CATEGORY_POSSIBLE_PREMIUM_SHORT_CODE),
            new ShortCodeTest("ch", "3456", SMS_CATEGORY_POSSIBLE_PREMIUM_SHORT_CODE),
            new ShortCodeTest("ch", "98765", SMS_CATEGORY_POSSIBLE_PREMIUM_SHORT_CODE),
            new ShortCodeTest("ch", "543", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("ch", "83111", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("ch", "234567", SMS_CATEGORY_NOT_SHORT_CODE),
            new ShortCodeTest("ch", "87654321", SMS_CATEGORY_NOT_SHORT_CODE),

            new ShortCodeTest("cn", "120", SMS_CATEGORY_NOT_SHORT_CODE),
            new ShortCodeTest("cn", "1062503000", SMS_CATEGORY_NOT_SHORT_CODE),
            new ShortCodeTest("cn", "1065123456", SMS_CATEGORY_FREE_SHORT_CODE),
            new ShortCodeTest("cn", "1066335588", SMS_CATEGORY_PREMIUM_SHORT_CODE),

            new ShortCodeTest("cy", "112", SMS_CATEGORY_NOT_SHORT_CODE),
            new ShortCodeTest("cy", "116117", SMS_CATEGORY_FREE_SHORT_CODE),
            new ShortCodeTest("cy", "4321", SMS_CATEGORY_POSSIBLE_PREMIUM_SHORT_CODE),
            new ShortCodeTest("cy", "54321", SMS_CATEGORY_POSSIBLE_PREMIUM_SHORT_CODE),
            new ShortCodeTest("cy", "654321", SMS_CATEGORY_POSSIBLE_PREMIUM_SHORT_CODE),
            new ShortCodeTest("cy", "7510", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("cy", "987654321", SMS_CATEGORY_NOT_SHORT_CODE),

            new ShortCodeTest("cz", "112", SMS_CATEGORY_NOT_SHORT_CODE),
            new ShortCodeTest("cz", "116117", SMS_CATEGORY_FREE_SHORT_CODE),
            new ShortCodeTest("cz", "9090150", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("cz", "90901599", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("cz", "987654321", SMS_CATEGORY_NOT_SHORT_CODE),

            new ShortCodeTest("de", "112", SMS_CATEGORY_NOT_SHORT_CODE),
            new ShortCodeTest("de", "116117", SMS_CATEGORY_FREE_SHORT_CODE),
            new ShortCodeTest("de", "1234", SMS_CATEGORY_POSSIBLE_PREMIUM_SHORT_CODE),
            new ShortCodeTest("de", "12345", SMS_CATEGORY_POSSIBLE_PREMIUM_SHORT_CODE),
            new ShortCodeTest("de", "8888", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("de", "11111", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("de", "11886", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("de", "22022", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("de", "23300", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("de", "3434", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("de", "34567", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("de", "41414", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("de", "55655", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("de", "66766", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("de", "66777", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("de", "77677", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("de", "80888", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("de", "1232286", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("de", "987654321", SMS_CATEGORY_NOT_SHORT_CODE),

            new ShortCodeTest("dk", "112", SMS_CATEGORY_NOT_SHORT_CODE),
            new ShortCodeTest("dk", "116117", SMS_CATEGORY_FREE_SHORT_CODE),
            new ShortCodeTest("dk", "1259", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("dk", "16123", SMS_CATEGORY_POSSIBLE_PREMIUM_SHORT_CODE),
            new ShortCodeTest("dk", "987654321", SMS_CATEGORY_NOT_SHORT_CODE),

            new ShortCodeTest("ee", "112", SMS_CATEGORY_NOT_SHORT_CODE),
            new ShortCodeTest("ee", "116117", SMS_CATEGORY_FREE_SHORT_CODE),
            new ShortCodeTest("ee", "123", SMS_CATEGORY_POSSIBLE_PREMIUM_SHORT_CODE),
            new ShortCodeTest("ee", "1259", SMS_CATEGORY_POSSIBLE_PREMIUM_SHORT_CODE),
            new ShortCodeTest("ee", "15330", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("ee", "17999", SMS_CATEGORY_POSSIBLE_PREMIUM_SHORT_CODE),
            new ShortCodeTest("ee", "17010", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("ee", "17013", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("ee", "9034567", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("ee", "34567890", SMS_CATEGORY_NOT_SHORT_CODE),

            new ShortCodeTest("es", "112", SMS_CATEGORY_NOT_SHORT_CODE),
            new ShortCodeTest("es", "116117", SMS_CATEGORY_FREE_SHORT_CODE),
            new ShortCodeTest("es", "25165", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("es", "27333", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("es", "995399", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("es", "87654321", SMS_CATEGORY_NOT_SHORT_CODE),

            new ShortCodeTest("fi", "112", SMS_CATEGORY_NOT_SHORT_CODE),
            new ShortCodeTest("fi", "116117", SMS_CATEGORY_FREE_SHORT_CODE),
            new ShortCodeTest("fi", "12345", SMS_CATEGORY_POSSIBLE_PREMIUM_SHORT_CODE),
            new ShortCodeTest("fi", "123456", SMS_CATEGORY_POSSIBLE_PREMIUM_SHORT_CODE),
            new ShortCodeTest("fi", "17159", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("fi", "17163", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("fi", "0600123", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("fi", "070012345", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("fi", "987654321", SMS_CATEGORY_NOT_SHORT_CODE),

            new ShortCodeTest("fr", "112", SMS_CATEGORY_NOT_SHORT_CODE),
            new ShortCodeTest("fr", "116117", SMS_CATEGORY_FREE_SHORT_CODE),
            new ShortCodeTest("fr", "34567", SMS_CATEGORY_FREE_SHORT_CODE),
            new ShortCodeTest("fr", "45678", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("fr", "81185", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("fr", "87654321", SMS_CATEGORY_NOT_SHORT_CODE),

            new ShortCodeTest("gb", "112", SMS_CATEGORY_NOT_SHORT_CODE),
            new ShortCodeTest("gb", "999", SMS_CATEGORY_NOT_SHORT_CODE),
            new ShortCodeTest("gb", "116117", SMS_CATEGORY_FREE_SHORT_CODE),
            new ShortCodeTest("gb", "4567", SMS_CATEGORY_POSSIBLE_PREMIUM_SHORT_CODE),
            new ShortCodeTest("gb", "45678", SMS_CATEGORY_POSSIBLE_PREMIUM_SHORT_CODE),
            new ShortCodeTest("gb", "56789", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("gb", "79067", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("gb", "80079", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("gb", "654321", SMS_CATEGORY_POSSIBLE_PREMIUM_SHORT_CODE),
            new ShortCodeTest("gb", "7654321", SMS_CATEGORY_NOT_SHORT_CODE),

            new ShortCodeTest("ge", "112", SMS_CATEGORY_NOT_SHORT_CODE),
            new ShortCodeTest("ge", "8765", SMS_CATEGORY_POSSIBLE_PREMIUM_SHORT_CODE),
            new ShortCodeTest("ge", "2345", SMS_CATEGORY_POSSIBLE_PREMIUM_SHORT_CODE),
            new ShortCodeTest("ge", "8012", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("ge", "8013", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("ge", "8014", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("ge", "8889", SMS_CATEGORY_PREMIUM_SHORT_CODE),

            new ShortCodeTest("gr", "112", SMS_CATEGORY_NOT_SHORT_CODE),
            new ShortCodeTest("gr", "116117", SMS_CATEGORY_FREE_SHORT_CODE),
            new ShortCodeTest("gr", "54321", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("gr", "19567", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("gr", "19678", SMS_CATEGORY_POSSIBLE_PREMIUM_SHORT_CODE),
            new ShortCodeTest("gr", "87654321", SMS_CATEGORY_NOT_SHORT_CODE),

            new ShortCodeTest("hu", "112", SMS_CATEGORY_NOT_SHORT_CODE),
            new ShortCodeTest("hu", "116117", SMS_CATEGORY_FREE_SHORT_CODE),
            new ShortCodeTest("hu", "012", SMS_CATEGORY_NOT_SHORT_CODE),
            new ShortCodeTest("hu", "0123", SMS_CATEGORY_POSSIBLE_PREMIUM_SHORT_CODE),
            new ShortCodeTest("hu", "1234", SMS_CATEGORY_POSSIBLE_PREMIUM_SHORT_CODE),
            new ShortCodeTest("hu", "1784", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("hu", "2345", SMS_CATEGORY_NOT_SHORT_CODE),
            new ShortCodeTest("hu", "01234", SMS_CATEGORY_NOT_SHORT_CODE),
            new ShortCodeTest("hu", "012345678", SMS_CATEGORY_NOT_SHORT_CODE),
            new ShortCodeTest("hu", "0123456789", SMS_CATEGORY_POSSIBLE_PREMIUM_SHORT_CODE),
            new ShortCodeTest("hu", "1234567890", SMS_CATEGORY_POSSIBLE_PREMIUM_SHORT_CODE),
            new ShortCodeTest("hu", "0691227910", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("hu", "2345678901", SMS_CATEGORY_NOT_SHORT_CODE),
            new ShortCodeTest("hu", "01234567890", SMS_CATEGORY_NOT_SHORT_CODE),

            new ShortCodeTest("ie", "112", SMS_CATEGORY_NOT_SHORT_CODE),
            new ShortCodeTest("ie", "116117", SMS_CATEGORY_FREE_SHORT_CODE),
            new ShortCodeTest("ie", "50123", SMS_CATEGORY_FREE_SHORT_CODE),
            new ShortCodeTest("ie", "51234", SMS_CATEGORY_STANDARD_SHORT_CODE),
            new ShortCodeTest("ie", "52345", SMS_CATEGORY_STANDARD_SHORT_CODE),
            new ShortCodeTest("ie", "57890", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("ie", "67890", SMS_CATEGORY_POSSIBLE_PREMIUM_SHORT_CODE),
            new ShortCodeTest("ie", "87654321", SMS_CATEGORY_NOT_SHORT_CODE),

            new ShortCodeTest("il", "112", SMS_CATEGORY_NOT_SHORT_CODE),
            new ShortCodeTest("il", "5432", SMS_CATEGORY_POSSIBLE_PREMIUM_SHORT_CODE),
            new ShortCodeTest("il", "4422", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("il", "4545", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("il", "98765", SMS_CATEGORY_NOT_SHORT_CODE),

            new ShortCodeTest("it", "112", SMS_CATEGORY_NOT_SHORT_CODE),
            new ShortCodeTest("it", "116117", SMS_CATEGORY_FREE_SHORT_CODE),
            new ShortCodeTest("it", "4567", SMS_CATEGORY_NOT_SHORT_CODE),
            new ShortCodeTest("it", "48000", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("it", "45678", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("it", "56789", SMS_CATEGORY_POSSIBLE_PREMIUM_SHORT_CODE),
            new ShortCodeTest("it", "456789", SMS_CATEGORY_NOT_SHORT_CODE),

            new ShortCodeTest("kg", "112", SMS_CATEGORY_NOT_SHORT_CODE),
            new ShortCodeTest("kg", "5432", SMS_CATEGORY_POSSIBLE_PREMIUM_SHORT_CODE),
            new ShortCodeTest("kg", "4152", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("kg", "4157", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("kg", "4449", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("kg", "98765", SMS_CATEGORY_NOT_SHORT_CODE),

            new ShortCodeTest("kz", "112", SMS_CATEGORY_NOT_SHORT_CODE),
            new ShortCodeTest("kz", "5432", SMS_CATEGORY_POSSIBLE_PREMIUM_SHORT_CODE),
            new ShortCodeTest("kz", "9194", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("kz", "7790", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("kz", "98765", SMS_CATEGORY_NOT_SHORT_CODE),

            new ShortCodeTest("lt", "112", SMS_CATEGORY_NOT_SHORT_CODE),
            new ShortCodeTest("lt", "116117", SMS_CATEGORY_FREE_SHORT_CODE),
            new ShortCodeTest("lt", "123", SMS_CATEGORY_POSSIBLE_PREMIUM_SHORT_CODE),
            new ShortCodeTest("lt", "1234", SMS_CATEGORY_POSSIBLE_PREMIUM_SHORT_CODE),
            new ShortCodeTest("lt", "1381", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("lt", "1394", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("lt", "1645", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("lt", "12345", SMS_CATEGORY_POSSIBLE_PREMIUM_SHORT_CODE),
            new ShortCodeTest("lt", "123456", SMS_CATEGORY_NOT_SHORT_CODE),

            new ShortCodeTest("lu", "112", SMS_CATEGORY_NOT_SHORT_CODE),
            new ShortCodeTest("lu", "116117", SMS_CATEGORY_FREE_SHORT_CODE),
            new ShortCodeTest("lu", "1234", SMS_CATEGORY_NOT_SHORT_CODE),
            new ShortCodeTest("lu", "12345", SMS_CATEGORY_NOT_SHORT_CODE),
            new ShortCodeTest("lu", "64747", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("lu", "678901", SMS_CATEGORY_NOT_SHORT_CODE),

            new ShortCodeTest("lv", "112", SMS_CATEGORY_NOT_SHORT_CODE),
            new ShortCodeTest("lv", "116117", SMS_CATEGORY_FREE_SHORT_CODE),
            new ShortCodeTest("lv", "5432", SMS_CATEGORY_POSSIBLE_PREMIUM_SHORT_CODE),
            new ShortCodeTest("lv", "1819", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("lv", "1863", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("lv", "1874", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("lv", "98765", SMS_CATEGORY_NOT_SHORT_CODE),

            new ShortCodeTest("mx", "112", SMS_CATEGORY_NOT_SHORT_CODE),
            new ShortCodeTest("mx", "2345", SMS_CATEGORY_POSSIBLE_PREMIUM_SHORT_CODE),
            new ShortCodeTest("mx", "7766", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("mx", "23456", SMS_CATEGORY_POSSIBLE_PREMIUM_SHORT_CODE),
            new ShortCodeTest("mx", "53035", SMS_CATEGORY_PREMIUM_SHORT_CODE),

            new ShortCodeTest("my", "112", SMS_CATEGORY_NOT_SHORT_CODE),
            new ShortCodeTest("my", "1234", SMS_CATEGORY_NOT_SHORT_CODE),
            new ShortCodeTest("my", "23456", SMS_CATEGORY_POSSIBLE_PREMIUM_SHORT_CODE),
            new ShortCodeTest("my", "32298", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("my", "33776", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("my", "345678", SMS_CATEGORY_NOT_SHORT_CODE),

            new ShortCodeTest("nl", "112", SMS_CATEGORY_NOT_SHORT_CODE),
            new ShortCodeTest("nl", "116117", SMS_CATEGORY_FREE_SHORT_CODE),
            new ShortCodeTest("nl", "1234", SMS_CATEGORY_POSSIBLE_PREMIUM_SHORT_CODE),
            new ShortCodeTest("nl", "4466", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("nl", "5040", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("nl", "23456", SMS_CATEGORY_NOT_SHORT_CODE),

            new ShortCodeTest("no", "112", SMS_CATEGORY_NOT_SHORT_CODE),
            new ShortCodeTest("no", "1234", SMS_CATEGORY_POSSIBLE_PREMIUM_SHORT_CODE),
            new ShortCodeTest("no", "2201", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("no", "2226", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("no", "2227", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("no", "23456", SMS_CATEGORY_POSSIBLE_PREMIUM_SHORT_CODE),
            new ShortCodeTest("no", "234567", SMS_CATEGORY_NOT_SHORT_CODE),

            new ShortCodeTest("nz", "112", SMS_CATEGORY_NOT_SHORT_CODE),
            new ShortCodeTest("nz", "123", SMS_CATEGORY_POSSIBLE_PREMIUM_SHORT_CODE),
            new ShortCodeTest("nz", "2345", SMS_CATEGORY_POSSIBLE_PREMIUM_SHORT_CODE),
            new ShortCodeTest("nz", "3903", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("nz", "8995", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("nz", "23456", SMS_CATEGORY_NOT_SHORT_CODE),

            new ShortCodeTest("pl", "112", SMS_CATEGORY_NOT_SHORT_CODE),
            new ShortCodeTest("pl", "116117", SMS_CATEGORY_FREE_SHORT_CODE),
            new ShortCodeTest("pl", "7890", SMS_CATEGORY_POSSIBLE_PREMIUM_SHORT_CODE),
            new ShortCodeTest("pl", "34567", SMS_CATEGORY_POSSIBLE_PREMIUM_SHORT_CODE),
            new ShortCodeTest("pl", "7910", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("pl", "74240", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("pl", "79866", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("pl", "92525", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("pl", "87654321", SMS_CATEGORY_NOT_SHORT_CODE),

            new ShortCodeTest("pt", "112", SMS_CATEGORY_NOT_SHORT_CODE),
            new ShortCodeTest("pt", "116117", SMS_CATEGORY_FREE_SHORT_CODE),
            new ShortCodeTest("pt", "61000", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("pt", "62345", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("pt", "68304", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("pt", "69876", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("pt", "87654321", SMS_CATEGORY_NOT_SHORT_CODE),

            new ShortCodeTest("ro", "112", SMS_CATEGORY_NOT_SHORT_CODE),
            new ShortCodeTest("ro", "116117", SMS_CATEGORY_FREE_SHORT_CODE),
            new ShortCodeTest("ro", "1234", SMS_CATEGORY_POSSIBLE_PREMIUM_SHORT_CODE),
            new ShortCodeTest("ro", "1263", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("ro", "1288", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("ro", "1314", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("ro", "1380", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("ro", "7890", SMS_CATEGORY_POSSIBLE_PREMIUM_SHORT_CODE),
            new ShortCodeTest("ro", "12345", SMS_CATEGORY_NOT_SHORT_CODE),

            new ShortCodeTest("ru", "112", SMS_CATEGORY_NOT_SHORT_CODE),
            new ShortCodeTest("ru", "5432", SMS_CATEGORY_POSSIBLE_PREMIUM_SHORT_CODE),
            new ShortCodeTest("ru", "1161", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("ru", "2097", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("ru", "3933", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("ru", "7781", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("ru", "98765", SMS_CATEGORY_NOT_SHORT_CODE),

            new ShortCodeTest("se", "112", SMS_CATEGORY_NOT_SHORT_CODE),
            new ShortCodeTest("se", "116117", SMS_CATEGORY_FREE_SHORT_CODE),
            new ShortCodeTest("se", "1234", SMS_CATEGORY_NOT_SHORT_CODE),
            new ShortCodeTest("se", "72345", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("se", "72999", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("se", "123456", SMS_CATEGORY_NOT_SHORT_CODE),
            new ShortCodeTest("se", "87654321", SMS_CATEGORY_NOT_SHORT_CODE),

            new ShortCodeTest("sg", "112", SMS_CATEGORY_NOT_SHORT_CODE),
            new ShortCodeTest("sg", "1234", SMS_CATEGORY_NOT_SHORT_CODE),
            new ShortCodeTest("sg", "70000", SMS_CATEGORY_POSSIBLE_PREMIUM_SHORT_CODE),
            new ShortCodeTest("sg", "79999", SMS_CATEGORY_POSSIBLE_PREMIUM_SHORT_CODE),
            new ShortCodeTest("sg", "73800", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("sg", "74688", SMS_CATEGORY_STANDARD_SHORT_CODE),
            new ShortCodeTest("sg", "987654", SMS_CATEGORY_NOT_SHORT_CODE),

            new ShortCodeTest("si", "112", SMS_CATEGORY_NOT_SHORT_CODE),
            new ShortCodeTest("si", "116117", SMS_CATEGORY_FREE_SHORT_CODE),
            new ShortCodeTest("si", "1234", SMS_CATEGORY_POSSIBLE_PREMIUM_SHORT_CODE),
            new ShortCodeTest("si", "3838", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("si", "72999", SMS_CATEGORY_NOT_SHORT_CODE),

            new ShortCodeTest("sk", "112", SMS_CATEGORY_NOT_SHORT_CODE),
            new ShortCodeTest("sk", "116117", SMS_CATEGORY_FREE_SHORT_CODE),
            new ShortCodeTest("sk", "1234", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("sk", "6674", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("sk", "7604", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("sk", "72999", SMS_CATEGORY_NOT_SHORT_CODE),

            new ShortCodeTest("tj", "112", SMS_CATEGORY_NOT_SHORT_CODE),
            new ShortCodeTest("tj", "5432", SMS_CATEGORY_POSSIBLE_PREMIUM_SHORT_CODE),
            new ShortCodeTest("tj", "1161", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("tj", "1171", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("tj", "4161", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("tj", "4449", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("tj", "98765", SMS_CATEGORY_NOT_SHORT_CODE),

            new ShortCodeTest("ua", "112", SMS_CATEGORY_NOT_SHORT_CODE),
            new ShortCodeTest("ua", "5432", SMS_CATEGORY_POSSIBLE_PREMIUM_SHORT_CODE),
            new ShortCodeTest("ua", "4448", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("ua", "7094", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("ua", "7540", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("ua", "98765", SMS_CATEGORY_NOT_SHORT_CODE),

            new ShortCodeTest("us", "911", SMS_CATEGORY_NOT_SHORT_CODE),
            new ShortCodeTest("us", "+18005551234", SMS_CATEGORY_NOT_SHORT_CODE),
            new ShortCodeTest("us", "8005551234", SMS_CATEGORY_NOT_SHORT_CODE),
            new ShortCodeTest("us", "20000", SMS_CATEGORY_POSSIBLE_PREMIUM_SHORT_CODE),
            new ShortCodeTest("us", "200000", SMS_CATEGORY_POSSIBLE_PREMIUM_SHORT_CODE),
            new ShortCodeTest("us", "2000000", SMS_CATEGORY_NOT_SHORT_CODE),
            new ShortCodeTest("us", "20433", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("us", "21472", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("us", "23333", SMS_CATEGORY_PREMIUM_SHORT_CODE),
            new ShortCodeTest("us", "99807", SMS_CATEGORY_PREMIUM_SHORT_CODE),

            // generic rules for other countries: 5 digits or less considered potential short code
            new ShortCodeTest("zz", "2000000", SMS_CATEGORY_NOT_SHORT_CODE),
            new ShortCodeTest("zz", "54321", SMS_CATEGORY_POSSIBLE_PREMIUM_SHORT_CODE),
            new ShortCodeTest("zz", "4321", SMS_CATEGORY_POSSIBLE_PREMIUM_SHORT_CODE),
            new ShortCodeTest("zz", "321", SMS_CATEGORY_POSSIBLE_PREMIUM_SHORT_CODE),
            new ShortCodeTest("zz", "112", SMS_CATEGORY_NOT_SHORT_CODE),
            new ShortCodeTest(null, "2000000", SMS_CATEGORY_NOT_SHORT_CODE),
            new ShortCodeTest(null, "54321", SMS_CATEGORY_POSSIBLE_PREMIUM_SHORT_CODE),
            new ShortCodeTest(null, "4321", SMS_CATEGORY_POSSIBLE_PREMIUM_SHORT_CODE),
            new ShortCodeTest(null, "321", SMS_CATEGORY_POSSIBLE_PREMIUM_SHORT_CODE),
            new ShortCodeTest(null, "112", SMS_CATEGORY_NOT_SHORT_CODE),
    };

    @Ignore
    public void testSmsUsageMonitor() {
        // InstrumentationTestRunner prepares a looper, but AndroidJUnitRunner does not.
        // http://b/25897652 .
        if (Looper.myLooper() == null) {
            Looper.prepare();
        }
        SmsUsageMonitor monitor = new SmsUsageMonitor(TestApplication.getAppContext());
        for (ShortCodeTest test : sShortCodeTests) {
            assertEquals("country: " + test.countryIso + " number: " + test.address,
                    test.category, monitor.checkDestination(test.address, test.countryIso));
        }
    }
}
