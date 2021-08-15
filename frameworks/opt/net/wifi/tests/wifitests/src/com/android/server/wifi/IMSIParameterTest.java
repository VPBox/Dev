/*
 * Copyright (C) 2016 The Android Open Source Project
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

package com.android.server.wifi;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import androidx.test.filters.SmallTest;

import org.junit.Test;

import java.util.HashMap;
import java.util.Map;

/**
 * Unit tests for {@link com.android.server.wifi.IMSIParameter}.
 */
@SmallTest
public class IMSIParameterTest {
    /**
     * Data points for testing function {@link IMSIParameter#build}.
     */
    private static final Map<String, IMSIParameter> BUILD_PARAM_TEST_MAP = new HashMap<>();
    static {
        BUILD_PARAM_TEST_MAP.put(null, null);
        BUILD_PARAM_TEST_MAP.put("", null);
        BUILD_PARAM_TEST_MAP.put("1234a123", null);    // IMSI contained invalid character.
        BUILD_PARAM_TEST_MAP.put("1234567890123451", null);   // IMSI exceeding max length.
        BUILD_PARAM_TEST_MAP.put("123456789012345", new IMSIParameter("123456789012345", false));
        BUILD_PARAM_TEST_MAP.put("1234*", new IMSIParameter("1234", true));
    }

    /**
     * Verify the expectations of {@link IMSIParameter#build} function using the predefined
     * test data {@link #BUILD_PARAM_TEST_MAP}.
     *
     * @throws Exception
     */
    @Test
    public void verifyBuildIMSIParameter() throws Exception {
        for (Map.Entry<String, IMSIParameter> entry : BUILD_PARAM_TEST_MAP.entrySet()) {
            assertEquals(entry.getValue(), IMSIParameter.build(entry.getKey()));
        }
    }

    /**
     * Verify that attempt to match a null IMSI will not cause any crash and should return false.
     *
     * @throws Exception
     */
    @Test
    public void matchesNullImsi() throws Exception {
        IMSIParameter param = new IMSIParameter("1234", false);
        assertFalse(param.matchesImsi(null));
    }

    /**
     * Verify that an IMSIParameter containing a full IMSI will only match against an IMSI of the
     * same value.
     *
     * @throws Exception
     */
    @Test
    public void matchesImsiWithFullImsi() throws Exception {
        IMSIParameter param = new IMSIParameter("1234", false);

        // Full IMSI requires exact matching.
        assertFalse(param.matchesImsi("123"));
        assertFalse(param.matchesImsi("12345"));
        assertTrue(param.matchesImsi("1234"));
    }

    /**
     * Verify that an IMSIParameter containing a prefix IMSI will match against any IMSI that
     * starts with the same prefix.
     *
     * @throws Exception
     */
    @Test
    public void matchesImsiWithPrefixImsi() throws Exception {
        IMSIParameter param = new IMSIParameter("1234", true);

        // Prefix IMSI will match any IMSI that starts with the same prefix.
        assertFalse(param.matchesImsi("123"));
        assertTrue(param.matchesImsi("12345"));
        assertTrue(param.matchesImsi("1234"));
    }

    /**
     * Verify that attempt to match a null MCC-MNC will not cause any crash and should return
     * false.
     *
     * @throws Exception
     */
    @Test
    public void matchesNullMccMnc() throws Exception {
        IMSIParameter param = new IMSIParameter("1234", false);
        assertFalse(param.matchesMccMnc(null));
    }

    /**
     * Verify that an IMSIParameter containing a full IMSI will only match against a 6 digit
     * MCC-MNC that is a prefix of the IMSI.
     *
     * @throws Exception
     */
    @Test
    public void matchesMccMncFullImsi() throws Exception {
        IMSIParameter param = new IMSIParameter("1234567890", false);

        assertFalse(param.matchesMccMnc("1234567"));    // Invalid length for MCC-MNC
        assertFalse(param.matchesMccMnc("12345"));      // Invalid length for MCC-MNC
        assertTrue(param.matchesMccMnc("123456"));
    }

    /**
     * Verify that an IMSIParameter containing an IMSI prefix that's less than 6 digits
     * will match against any 6-digit MCC-MNC that starts with the same prefix.
     *
     * @throws Exception
     */
    @Test
    public void matchesMccMncWithPrefixImsiLessThanMccMncLength() throws Exception {
        IMSIParameter param = new IMSIParameter("12345", true);

        assertFalse(param.matchesMccMnc("123448"));     // Prefix mismatch
        assertFalse(param.matchesMccMnc("12345"));      // Invalid length for MCC-MNC
        assertFalse(param.matchesMccMnc("1234567"));    // Invalid length for MCC-MNC
        assertTrue(param.matchesMccMnc("123457"));
        assertTrue(param.matchesMccMnc("123456"));
    }

    /**
     * Verify that an IMSIParameter containing an IMSI prefix that's more than 6 digits
     * will only match against a 6-digit MCC-MNC that matches the first 6-digit of the prefix.
     *
     * @throws Exception
     */
    @Test
    public void matchesMccMncWithPrefixImsiMoreThanMccMncLength() throws Exception {
        IMSIParameter param = new IMSIParameter("1234567890", true);
        assertFalse(param.matchesMccMnc("12345"));      // Invalid length for MCC-MNC
        assertFalse(param.matchesMccMnc("1234567"));    // Invalid length for MCC-MNC
        assertTrue(param.matchesMccMnc("123456"));
    }
}
