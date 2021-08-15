/*
 * Copyright (C) 2019 The Android Open Source Project
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

import static org.junit.Assert.*;

import org.junit.Test;

import java.util.ArrayList;

/**
 * Unit tests for {@link com.android.server.wifi.hotspot2.Utils}.
 */
public class UtilsTest {
    @Test
    public void testRoamingConsortiumsToStringLong() {
        assertEquals("null", Utils.roamingConsortiumsToString((long[]) null));

        long[] ois = new long[]{1L, 2L, 1L << (63 - 40)};
        String expected = "000001, 000002, 0000800000";
        String result = Utils.roamingConsortiumsToString(ois);
        assertEquals(expected, result);
    }

    @Test
    public void testRoamingConsortiumsToStringCollection() {
        ArrayList<Long> ois = new ArrayList<>();
        assertEquals("", Utils.roamingConsortiumsToString(ois));

        ois.add(1L);
        ois.add(2L);
        ois.add((1L << (63 - 40)));
        String expected = "000001, 000002, 0000800000";
        String result = Utils.roamingConsortiumsToString(ois);
        assertEquals(expected, result);
    }

    @Test
    public void testToUnicodeEscapedString() {
        assertEquals("", Utils.toUnicodeEscapedString(""));

        StringBuilder unescapedStringBuilder = new StringBuilder();
        StringBuilder escapedStringBuilder = new StringBuilder();
        for (int c = 0; c < 128; c++) {
            unescapedStringBuilder.append((char) c);
            if (c >= ' ' && c < 127) {
                escapedStringBuilder.append((char) c);
            } else {
                escapedStringBuilder.append("\\u").append(String.format("%04x", c));
            }
        }
        assertEquals(escapedStringBuilder.toString(),
                Utils.toUnicodeEscapedString(unescapedStringBuilder.toString()));
    }

    @Test
    public void testToHexString() {
        assertEquals("null", Utils.toHexString(null));

        byte[] bytes = {(byte) 0xab, (byte) 0xcd, (byte) 0xef};
        String expected = "ab cd ef";
        String result = Utils.toHexString(bytes);
        assertEquals(expected, result.toLowerCase());
    }

    @Test
    public void testToHex() {
        assertEquals("", Utils.toHex(new byte[0]));

        byte[] bytes = {(byte) 0xab, (byte) 0xcd, (byte) 0xef};
        String expected = "abcdef";
        String result = Utils.toHex(bytes);
        assertEquals(expected, result.toLowerCase());

    }

    @Test
    public void testHexToBytes() {
        assertArrayEquals(new byte[0], Utils.hexToBytes(""));

        String hexString = "abcd";
        byte[] expected = {(byte) 0xab, (byte) 0xcd};
        byte[] result = Utils.hexToBytes(hexString);
        assertArrayEquals(expected, result);
    }

    @Test
    public void testFromHex() {
        int i = 0;
        for (char c : "0123456789abcdef".toCharArray()) {
            assertEquals(i, Utils.fromHex(c, true));
            assertEquals(i, Utils.fromHex(c, false));
            assertEquals(i, Utils.fromHex(Character.toUpperCase(c), true));
            assertEquals(i, Utils.fromHex(Character.toUpperCase(c), false));
            i++;
        }

        assertEquals(-1, Utils.fromHex('q', true));

        try {
            Utils.fromHex('q', false);
            fail("Exception should be thrown!");
        } catch (NumberFormatException e) {
            // expected
        }
    }

    @Test
    public void testCompare() {
        assertEquals(-1, Utils.compare(-1, 1));
        assertEquals(0, Utils.compare(0, 0));
        assertEquals(1, Utils.compare(1, -1));

        assertEquals(-1, Utils.compare(null, 0));
        assertEquals(0, Utils.compare(null, null));
        assertEquals(1, Utils.compare(0, null));
    }

    @Test
    public void testToHMS() {
        long hours = 12;
        long minutes = 34;
        long millis = 56789;

        long time = (((hours * 60) + minutes) * 60) * 1000 + millis;

        String expected = "12:34:56.789";
        String result = Utils.toHMS(time);
        assertEquals(expected, result);

        expected = "-12:34:56.789";
        result = Utils.toHMS(-time);
        assertEquals(expected, result);
    }

    @Test
    public void testToUTCString() {
        long millis = 832077296000L;

        String expected = "1996/05/14 12:34:56Z";
        String result = Utils.toUTCString(millis);
        assertEquals(expected, result);
    }

    @Test
    public void testUnquote() {
        assertEquals(null, Utils.unquote(null));

        String unquoted = "This is a wug.";
        String quoted = "\"This is a wug.\"";
        String twiceQuoted = "\"\"This is a wug.\"\"";
        String unclosedQuoted = "\"This is a wug.";
        String quotedUnclosedQuoted = "\"\"This is a wug.\"";

        assertEquals(unquoted, Utils.unquote(quoted));
        assertEquals(quoted, Utils.unquote(twiceQuoted));
        assertEquals(unclosedQuoted, Utils.unquote(unclosedQuoted));
        assertEquals(unclosedQuoted, Utils.unquote(quotedUnclosedQuoted));
    }
}


