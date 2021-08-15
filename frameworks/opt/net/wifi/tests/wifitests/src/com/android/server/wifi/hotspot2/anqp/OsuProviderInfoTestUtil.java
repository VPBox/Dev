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

import android.net.Uri;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.nio.charset.StandardCharsets;
import java.util.Arrays;
import java.util.List;
import java.util.Locale;

/**
 * Utility class containing test data and object for {@link OsuProviderInfo}.
 */
public class OsuProviderInfoTestUtil {
    // Test data
    private static final List<I18Name> TEST_FRIENDLY_NAMES =
            Arrays.asList(new I18Name("en", Locale.forLanguageTag("en"), "FriendlyName"));
    private static final String TEST_SERVER_URI = "https://test.server.com";
    private static final List<Integer> TEST_METHOD_LIST = Arrays.asList(0, 1);
    private static final List<IconInfo> TEST_ICON_INFO_LIST =
            Arrays.asList(IconInfoTestUtil.TEST_ICON_INFO);
    private static final String TEST_NAI = "network_access@test.com";
    private static final List<I18Name> TEST_SERVICE_DESCRIPTIONS =
            Arrays.asList(new I18Name("en", Locale.forLanguageTag("en"), "Test Service"));

    /**
     * {@link IconInfo} object with pre-defined test data.
     */
    public static final OsuProviderInfo TEST_OSU_PROVIDER_INFO =
            new OsuProviderInfo(TEST_FRIENDLY_NAMES, Uri.parse(TEST_SERVER_URI), TEST_METHOD_LIST,
                    TEST_ICON_INFO_LIST, TEST_NAI, TEST_SERVICE_DESCRIPTIONS);

    /**
     * Raw bytes of icon info with pre-defined test data.
     */
    public static final byte[] TEST_OSU_PROVIDER_INFO_RAW_BYTES = getTestData();

    public static final byte[] TEST_OSU_PROVIDER_INFO_RAW_BYTES_WITH_INVALID_LENGTH =
            getTestDataWithInvalidLength();

    /**
     * Generate and return the raw data based on pre-defined test data.
     *
     * @return byte[]
     */
    private static byte[] getTestData() {
        try {
            ByteArrayOutputStream out = new ByteArrayOutputStream();
            byte[] payload = getTestPayload();
            writeShortLE(out, payload.length);
            out.write(payload);
            return out.toByteArray();
        } catch (Exception e) {
            return null;
        }
    }

    /**
     * Generate and return the raw data based on pre-defined test data.
     *
     * @return byte[]
     */
    private static byte[] getTestDataWithInvalidLength() {
        try {
            ByteArrayOutputStream out = new ByteArrayOutputStream();
            byte[] payload = getTestPayload();
            // Set length to less than the minimum required.
            writeShortLE(out, OsuProviderInfo.MINIMUM_LENGTH - 1);
            out.write(payload);
            return out.toByteArray();
        } catch (Exception e) {
            return null;
        }
    }

    /**
     * Generate and return the payload containing OSU provider test data, excluding the length
     * field.
     *
     * @return byte[]
     * @throws IOException
     */
    private static byte[] getTestPayload() throws IOException {
        ByteArrayOutputStream out = new ByteArrayOutputStream();

        // Write friendly name list.
        byte[] friendlyNamesData = getI18NameListData(TEST_FRIENDLY_NAMES);
        writeShortLE(out, friendlyNamesData.length);
        out.write(friendlyNamesData);

        // Write server URI.
        writeByteArrayWithLength(out, TEST_SERVER_URI.getBytes(StandardCharsets.UTF_8));

        // Write method list.
        out.write((byte) TEST_METHOD_LIST.size());
        for (Integer method : TEST_METHOD_LIST) {
            out.write((byte) method.intValue());
        }

        // Write icon info list.
        writeShortLE(out, IconInfoTestUtil.TEST_ICON_INFO_RAW_BYTES.length);
        out.write(IconInfoTestUtil.TEST_ICON_INFO_RAW_BYTES);

        // Write NAI.
        writeByteArrayWithLength(out, TEST_NAI.getBytes(StandardCharsets.UTF_8));

        // Write service descriptions.
        byte[] serviceDescriptionsData = getI18NameListData(TEST_SERVICE_DESCRIPTIONS);
        writeShortLE(out, serviceDescriptionsData.length);
        out.write(serviceDescriptionsData);

        return out.toByteArray();
    }

    private static byte[] getI18NameListData(List<I18Name> nameList) throws IOException {
        ByteArrayOutputStream out = new ByteArrayOutputStream();
        for (I18Name name : nameList) {
            byte[] data = getI18NameData(name);
            out.write((byte) data.length);
            out.write(data);
        }
        return out.toByteArray();
    }

    /**
     * Format the raw bytes for the given {@link I18Name}.
     *
     * @param value The {@link I18Name} to serialize
     * @return byte[]
     * @throws IOException
     */
    private static byte[] getI18NameData(I18Name value) throws IOException {
        ByteArrayOutputStream out = new ByteArrayOutputStream();
        out.write(value.getLanguage().getBytes(StandardCharsets.US_ASCII));
        out.write((byte) 0);    // Padding for language code.
        out.write(value.getText().getBytes(StandardCharsets.UTF_8));
        return out.toByteArray();
    }

    /**
     * Write the lower 2-bytes of an integer to the given output stream in Little-Endian.
     *
     * @param out The output stream to write to
     * @param value The integer value to write
     */
    private static void writeShortLE(ByteArrayOutputStream out, int value) {
        out.write(value & 0xFF);
        out.write((value >> 8) & 0xFF);
    }

    /**
     * Write the given byte array to the given output stream, the array data is prefixed with a
     * byte specifying the length of the byte array.
     *
     * @param out The output stream to write to
     * @param data The byte array to write
     * @throws IOException
     */
    private static void writeByteArrayWithLength(ByteArrayOutputStream out, byte[] data)
            throws IOException {
        out.write((byte) data.length);
        out.write(data);
    }
}
