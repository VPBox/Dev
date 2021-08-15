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

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.nio.charset.StandardCharsets;

/**
 * Utility class containing test data and object for {@link IconInfo}.
 */
public class IconInfoTestUtil {
    // Test data
    private static final int TEST_WIDTH = 9811;
    private static final int TEST_HEIGHT = 4523;
    private static final String TEST_LANGUAGE = "en";
    private static final String TEST_TYPE = "png";
    private static final String TEST_FILENAME = "testicon.png";

    /**
     * {@link IconInfo} object with pre-defined test data.
     */
    public static final IconInfo TEST_ICON_INFO =
            new IconInfo(TEST_WIDTH, TEST_HEIGHT, TEST_LANGUAGE, TEST_TYPE, TEST_FILENAME);

    /**
     * Raw bytes of icon info with pre-defined test data.
     */
    public static final byte[] TEST_ICON_INFO_RAW_BYTES = getTestData();

    /**
     * Generate raw bytes based on the pre-defined test data.
     *
     * @return array of bytes
     */
    private static byte[] getTestData() {
        try {
            ByteArrayOutputStream out = new ByteArrayOutputStream();
            writeShortLE(out, TEST_WIDTH);
            writeShortLE(out, TEST_HEIGHT);
            out.write(TEST_LANGUAGE.getBytes(StandardCharsets.US_ASCII));
            out.write((byte) 0);    // Padding for language code.
            writeByteArrayWithLength(out, TEST_TYPE.getBytes(StandardCharsets.US_ASCII));
            writeByteArrayWithLength(out, TEST_FILENAME.getBytes(StandardCharsets.UTF_8));
            return out.toByteArray();
        } catch (IOException e) {
            return null;
        }
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
