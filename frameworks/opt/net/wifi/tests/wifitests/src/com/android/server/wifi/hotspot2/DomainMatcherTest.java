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

package com.android.server.wifi.hotspot2;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;

import android.util.Pair;

import androidx.test.filters.SmallTest;

import org.junit.Test;

import java.util.Arrays;
import java.util.HashMap;
import java.util.Map;

/**
 * Unit tests for {@link com.android.server.wifi.hotspot2.DomainMatcher}.
 */
@SmallTest
public class DomainMatcherTest {
    private static final String PRIMARY_DOMAIN = "google.com";
    private static final String SECONDARY_DOMAIN1 = "android.com";
    private static final String SECONDARY_DOMAIN2 = "testing.test.com";

    /**
     * Test data for isSubDomain function.
     */
    private static final Map<String, Integer> TEST_DOMAIN_MAP = new HashMap<>();
    static {
        TEST_DOMAIN_MAP.put("", DomainMatcher.MATCH_NONE);
        TEST_DOMAIN_MAP.put("com", DomainMatcher.MATCH_NONE);
        TEST_DOMAIN_MAP.put("test.com", DomainMatcher.MATCH_NONE);
        TEST_DOMAIN_MAP.put("google.com", DomainMatcher.MATCH_PRIMARY);
        TEST_DOMAIN_MAP.put("test.google.com", DomainMatcher.MATCH_PRIMARY);
        TEST_DOMAIN_MAP.put("android.com", DomainMatcher.MATCH_SECONDARY);
        TEST_DOMAIN_MAP.put("test.android.com", DomainMatcher.MATCH_SECONDARY);
        TEST_DOMAIN_MAP.put("testing.test.com", DomainMatcher.MATCH_SECONDARY);
        TEST_DOMAIN_MAP.put("adbcd.testing.test.com", DomainMatcher.MATCH_SECONDARY);
    }

    /**
     * Test data for arg2SubdomainOfArg1 function.
     */
    private static final Map<Pair<String, String>, Boolean> TEST_ARG_DOMAIN_MAP = new HashMap<>();
    static {
        TEST_ARG_DOMAIN_MAP.put(new Pair<String, String>("test.com", "abc.test.com"), true);
        TEST_ARG_DOMAIN_MAP.put(new Pair<String, String>("test.com", "ad.abc.test.com"), true);
        TEST_ARG_DOMAIN_MAP.put(new Pair<String, String>("com", "test.com"), true);
        TEST_ARG_DOMAIN_MAP.put(new Pair<String, String>("abc.test.com", "test.com"), false);
        TEST_ARG_DOMAIN_MAP.put(new Pair<String, String>("test1.com", "test.com"), false);
        TEST_ARG_DOMAIN_MAP.put(new Pair<String, String>("test.com", "com"), false);
    }

    /**
     * Verify that creating a matcher with empty domains doesn't cause any exceptions.
     *
     * @throws Exception
     */
    @Test
    public void createMatcherWithEmptyDomains() throws Exception {
        DomainMatcher domainMatcher = new DomainMatcher(null, null);
        assertEquals(DomainMatcher.MATCH_NONE, domainMatcher.isSubDomain("google.com"));
    }

    /**
     * Verify that matching a null domain doesn't cause any exceptions.
     *
     * @throws Exception
     */
    @Test
    public void matchNullDomain() throws Exception {
        DomainMatcher domainMatcher = new DomainMatcher(PRIMARY_DOMAIN,
                Arrays.asList(SECONDARY_DOMAIN1, SECONDARY_DOMAIN2));
        assertEquals(DomainMatcher.MATCH_NONE, domainMatcher.isSubDomain(null));
    }

    /**
     * Verify the domain matching expectations based on the predefined {@link #TEST_DOMAIN_MAP}.
     *
     * @throws Exception
     */
    @Test
    public void matchTestDomains() throws Exception {
        DomainMatcher domainMatcher = new DomainMatcher(PRIMARY_DOMAIN,
                Arrays.asList(SECONDARY_DOMAIN1, SECONDARY_DOMAIN2));
        for (Map.Entry<String, Integer> entry : TEST_DOMAIN_MAP.entrySet()) {
            assertEquals(entry.getValue().intValue(), domainMatcher.isSubDomain(entry.getKey()));
        }
    }

    /**
     * Verify that the correct match status is returned when a domain matches both primary
     * and secondary domain (primary domain have precedence over secondary).
     *
     * @throws Exception
     */
    @Test
    public void matchDomainWithBothPrimaryAndSecondary() throws Exception {
        DomainMatcher domainMatcher = new DomainMatcher(PRIMARY_DOMAIN,
                Arrays.asList(PRIMARY_DOMAIN));
        assertEquals(DomainMatcher.MATCH_PRIMARY, domainMatcher.isSubDomain(PRIMARY_DOMAIN));
    }

    /**
     * Verify domain matching expectation when the secondary domain is a sub-domain of the
     * primary domain.
     *
     * @throws Exception
     */
    @Test
    public void matchDomainWhenSecondaryIsSubdomainOfPrimary() throws Exception {
        DomainMatcher domainMatcher = new DomainMatcher("google.com",
                Arrays.asList("test.google.com"));
        assertEquals(DomainMatcher.MATCH_PRIMARY, domainMatcher.isSubDomain("google.com"));
        assertEquals(DomainMatcher.MATCH_PRIMARY, domainMatcher.isSubDomain("test.google.com"));
        assertEquals(DomainMatcher.MATCH_PRIMARY,
                domainMatcher.isSubDomain("abcd.test.google.com"));
    }

    /**
     * Verify domain matching expectations when the secondary domain is a sub-domain of the
     * primary domain.
     *
     * @throws Exception
     */
    @Test
    public void matchDomainWhenPrimaryIsSubdomainOfSecondary() throws Exception {
        DomainMatcher domainMatcher = new DomainMatcher("test.google.com",
                Arrays.asList("google.com"));
        assertEquals(DomainMatcher.MATCH_SECONDARY, domainMatcher.isSubDomain("google.com"));
        assertEquals(DomainMatcher.MATCH_SECONDARY, domainMatcher.isSubDomain("test2.google.com"));
        assertEquals(DomainMatcher.MATCH_PRIMARY, domainMatcher.isSubDomain("test.google.com"));
        assertEquals(DomainMatcher.MATCH_PRIMARY,
                domainMatcher.isSubDomain("adcd.test.google.com"));
    }

    /**
     * Verify domain matching expectations when the domain names contained empty label (domain
     * name that contained "..").
     *
     * @throws Exception
     */
    @Test
    public void matchDomainWithEmptyLabel() throws Exception {
        DomainMatcher domainMatcher = new DomainMatcher("test.google..com",
                Arrays.asList("google..com"));
        assertEquals(DomainMatcher.MATCH_PRIMARY, domainMatcher.isSubDomain("test.google..com"));
        assertEquals(DomainMatcher.MATCH_SECONDARY, domainMatcher.isSubDomain("google..com"));
    }

    /**
     * Verify domain matching expectation for arg2SubdomainOfArg1 based on predefined
     * {@link #TEST_ARG_DOMAIN_MAP}.
     *
     * @throws Exception
     */
    @Test
    public void verifyArg2SubdomainOfArg1() throws Exception {
        for (Map.Entry<Pair<String, String>, Boolean> entry : TEST_ARG_DOMAIN_MAP.entrySet()) {
            assertEquals(entry.getValue().booleanValue(),
                    DomainMatcher.arg2SubdomainOfArg1(entry.getKey().first, entry.getKey().second));
        }
    }

    /**
     * Verify that arg2SubdomainOfArg1 works as expected when pass in null domains.
     *
     * @throws Exception
     */
    @Test
    public void arg2SubdomainOfArg1WithNullDomain() throws Exception {
        assertFalse(DomainMatcher.arg2SubdomainOfArg1(null, "test.com"));
        assertFalse(DomainMatcher.arg2SubdomainOfArg1("test.com", null));
        assertFalse(DomainMatcher.arg2SubdomainOfArg1(null, null));
    }

    /**
     * Verify that arg2SubdomainOfArg1 works as expected when domain contains empty label.
     *
     * @throws Exception
     */
    @Test
    public void arg2SubdomainOfArg1WithEmptyLabel() throws Exception {
        assertTrue(DomainMatcher.arg2SubdomainOfArg1("test..com", "adsf.test..com"));
    }

}
