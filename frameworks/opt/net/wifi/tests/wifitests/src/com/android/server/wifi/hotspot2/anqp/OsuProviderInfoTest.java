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

package com.android.server.wifi.hotspot2.anqp;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertNotNull;

import androidx.test.filters.SmallTest;

import org.junit.Test;

import java.net.ProtocolException;
import java.nio.BufferUnderflowException;
import java.nio.ByteBuffer;
import java.util.ArrayList;
import java.util.List;
import java.util.Locale;

/**
 * Unit tests for {@link com.android.server.wifi.hotspot2.anqp.OsuProviderInfo}.
 */
@SmallTest
public class OsuProviderInfoTest {
    /**
     * Verify that BufferUnderflowException will be thrown when parsing an empty buffer.
     * @throws Exception
     */
    @Test(expected = BufferUnderflowException.class)
    public void parseEmptyBuffer() throws Exception {
        OsuProviderInfo.parse(ByteBuffer.allocate(0));
    }

    /**
     * Verify that BufferUnderflowException will be thrown when parsing a truncated buffer
     * (missing a byte at the end).
     *
     * @throws Exception
     */
    @Test(expected = BufferUnderflowException.class)
    public void parseTruncatedBuffer() throws Exception {
        ByteBuffer buffer = ByteBuffer.wrap(
                OsuProviderInfoTestUtil.TEST_OSU_PROVIDER_INFO_RAW_BYTES);
        buffer.limit(buffer.remaining() - 1);
        OsuProviderInfo.parse(buffer);
    }

    /**
     * Verify that ProtocolException will be thrown when parsing a buffer containing an
     * invalid length value.
     *
     * @throws Exception
     */
    @Test(expected = ProtocolException.class)
    public void parseBufferWithInvalidLength() throws Exception {
        ByteBuffer buffer = ByteBuffer.wrap(
                OsuProviderInfoTestUtil.TEST_OSU_PROVIDER_INFO_RAW_BYTES_WITH_INVALID_LENGTH);
        OsuProviderInfo.parse(buffer);
    }

    /**
     * Verify that an expected {@link OsuProviderInfo} will be returned when parsing a buffer
     * containing pre-defined test data.
     *
     * @throws Exception
     */
    @Test
    public void parseBufferWithTestData() throws Exception {
        ByteBuffer buffer = ByteBuffer.wrap(
                OsuProviderInfoTestUtil.TEST_OSU_PROVIDER_INFO_RAW_BYTES);
        assertEquals(OsuProviderInfoTestUtil.TEST_OSU_PROVIDER_INFO,
                OsuProviderInfo.parse(buffer));
    }

    /**
     * Verify that when a provider contained multiple friendly names in different languages, the
     * friendly name that's in default language is returned.
     *
     * @throws Exception
     */
    @Test
    public void getFriendlyNameMatchingDefaultLocale() throws Exception {
        List<I18Name> friendlyNames = new ArrayList<>();
        Locale defaultLocale = Locale.getDefault();
        Locale nonDefaultLocale = Locale.FRENCH;
        if (defaultLocale.equals(nonDefaultLocale)) {
            nonDefaultLocale = Locale.ENGLISH;
        }
        String nonDefaultString = "Non-default";
        String defaultString = "Default";
        friendlyNames.add(
                new I18Name(nonDefaultLocale.getLanguage(), nonDefaultLocale, nonDefaultString));
        friendlyNames.add(new I18Name(defaultLocale.getLanguage(), defaultLocale, defaultString));
        OsuProviderInfo providerInfo =
                new OsuProviderInfo(friendlyNames, null, null, null, null, null);
        assertEquals(defaultString, providerInfo.getFriendlyName());
    }

    /**
     * Verify that when a provider contained multiple friendly names where no friendly name
     * is in default language, the first name in the list is returned.
     *
     * @throws Exception
     */
    @Test
    public void getFriendlyNameNotMatchingDefaultLocale() throws Exception {
        List<I18Name> friendlyNames = new ArrayList<>();
        Locale nonDefaultLocale = Locale.FRENCH;
        if (nonDefaultLocale.equals(Locale.getDefault())) {
            nonDefaultLocale = Locale.ENGLISH;
        }
        String firstString = "First name";
        String secondString = "Second name";
        friendlyNames.add(
                new I18Name(nonDefaultLocale.getLanguage(), nonDefaultLocale, firstString));
        friendlyNames.add(
                new I18Name(nonDefaultLocale.getLanguage(), nonDefaultLocale, secondString));
        OsuProviderInfo providerInfo =
                new OsuProviderInfo(friendlyNames, null, null, null, null, null);
        assertNotNull(providerInfo.getFriendlyName());
    }

    /**
     * Verify that null will be returned for a provider containing empty friendly name list.
     *
     * @throws Exception
     */
    @Test
    public void getFriendlyNameWithEmptyList() throws Exception {
        OsuProviderInfo providerInfo =
                new OsuProviderInfo(new ArrayList<I18Name>(), null, null, null, null, null);
        assertEquals(null, providerInfo.getFriendlyName());
    }

    /**
     * Verify that when a provider contained multiple service descriptions in different languages,
     * the service description that's in default language is returned.
     *
     * @throws Exception
     */
    @Test
    public void getServiceDescriptionMatchingDefaultLocale() throws Exception {
        List<I18Name> serviceDescriptions = new ArrayList<>();
        Locale defaultLocale = Locale.getDefault();
        Locale nonDefaultLocale = Locale.FRENCH;
        if (defaultLocale.equals(nonDefaultLocale)) {
            nonDefaultLocale = Locale.ENGLISH;
        }
        String nonDefaultString = "Non-default";
        String defaultString = "Default";
        serviceDescriptions.add(
                new I18Name(nonDefaultLocale.getLanguage(), nonDefaultLocale, nonDefaultString));
        serviceDescriptions.add(
                new I18Name(defaultLocale.getLanguage(), defaultLocale, defaultString));
        OsuProviderInfo providerInfo =
                new OsuProviderInfo(null, null, null, null, null, serviceDescriptions);
        assertEquals(defaultString, providerInfo.getServiceDescription());
    }

    /**
     * Verify that when a provider contained multiple service descriptions where none of them
     * is in default language, the first element in the list is returned.
     *
     * @throws Exception
     */
    @Test
    public void getServiceDescriptionNotMatchingDefaultLocale() throws Exception {
        List<I18Name> serviceDescriptions = new ArrayList<>();
        Locale nonDefaultLocale = Locale.FRENCH;
        if (nonDefaultLocale.equals(Locale.getDefault())) {
            nonDefaultLocale = Locale.ENGLISH;
        }
        String firstString = "First name";
        String secondString = "Second name";
        serviceDescriptions.add(
                new I18Name(nonDefaultLocale.getLanguage(), nonDefaultLocale, firstString));
        serviceDescriptions.add(
                new I18Name(nonDefaultLocale.getLanguage(), nonDefaultLocale, secondString));
        OsuProviderInfo providerInfo =
                new OsuProviderInfo(null, null, null, null, null, serviceDescriptions);
        assertEquals(firstString, providerInfo.getServiceDescription());
    }

    /**
     * Verify that null will be returned for a provider containing empty friendly name list.
     *
     * @throws Exception
     */
    @Test
    public void getServiceDescriptionWithEmptyList() throws Exception {
        OsuProviderInfo providerInfo =
                new OsuProviderInfo(null, null, null, null, null, new ArrayList<I18Name>());
        assertEquals(null, providerInfo.getServiceDescription());
    }
}
