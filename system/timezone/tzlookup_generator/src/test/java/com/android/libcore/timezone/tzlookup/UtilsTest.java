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

package com.android.libcore.timezone.tzlookup;

import org.junit.Test;

import java.text.ParseException;
import java.util.Arrays;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

import static com.android.libcore.timezone.tzlookup.Utils.parseUtcOffsetToMillis;
import static com.android.libcore.timezone.tzlookup.Utils.toUtcOffsetString;
import static java.util.concurrent.TimeUnit.HOURS;
import static java.util.concurrent.TimeUnit.MINUTES;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertTrue;
import static org.junit.Assert.fail;

public class UtilsTest {

    @Test
    public void subtract() {
        Set<String> a = set("Foo", "Bar");
        Set<String> b = set("Bar", "Baz");
        Set<String> empty = set();
        assertEquals(set("Foo"), Utils.subtract(a, b));
        assertEquals(set("Baz"), Utils.subtract(b, a));
        assertEquals(a, Utils.subtract(a, empty));
        assertEquals(empty, Utils.subtract(empty, a));
    }

    @Test
    public void toUpperCase() {
        assertEquals(list("ONE", "TWO", "THREE"), Utils.toUpperCase(list("one", "Two", "THREE")));
        assertEquals(list(), Utils.toUpperCase(list()));
    }

    @Test
    public void allLowerCaseAscii() {
        assertTrue(Utils.allLowerCaseAscii(list("one", "a", "z")));
        assertFalse(Utils.allLowerCaseAscii(list("one", "a", "Z")));
        assertFalse(Utils.allLowerCaseAscii(list("one", "A", "z")));
        assertFalse(Utils.allLowerCaseAscii(list("onE", "a", "z")));
    }

    @Test
    public void utcOffsets() throws Exception {
        assertParseUtcFails("");
        assertParseUtcFails("0");
        assertParseUtcFails("0:0");

        assertEquals(0, parseUtcOffsetToMillis("0:00"));
        assertEquals(0, parseUtcOffsetToMillis("00:00"));
        assertEquals(0, parseUtcOffsetToMillis("-0:00"));
        assertParseUtcFails("+00:00");

        assertEquals("00:00", toUtcOffsetString(0));

        long hourInMillis = HOURS.toMillis(1);
        assertEquals(hourInMillis, parseUtcOffsetToMillis("1:00"));
        assertEquals(hourInMillis, parseUtcOffsetToMillis("01:00"));
        assertEquals(-hourInMillis, parseUtcOffsetToMillis("-1:00"));
        assertEquals(-hourInMillis, parseUtcOffsetToMillis("-01:00"));

        assertEquals("01:00", toUtcOffsetString(hourInMillis));
        assertEquals("-01:00", toUtcOffsetString(-hourInMillis));

        long hourAndAHalfInMillis = hourInMillis + MINUTES.toMillis(30);
        assertEquals(hourAndAHalfInMillis, parseUtcOffsetToMillis("1:30"));
        assertEquals(hourAndAHalfInMillis, parseUtcOffsetToMillis("01:30"));
        assertEquals(-hourAndAHalfInMillis, parseUtcOffsetToMillis("-1:30"));
        assertEquals(-hourAndAHalfInMillis, parseUtcOffsetToMillis("-01:30"));

        assertEquals("01:30", toUtcOffsetString(hourAndAHalfInMillis));
        assertEquals("-01:30", toUtcOffsetString(-hourAndAHalfInMillis));

        long hourAndTenInMillis = hourInMillis + MINUTES.toMillis(10);
        assertEquals(hourAndTenInMillis, parseUtcOffsetToMillis("01:10"));
        assertEquals(-hourAndTenInMillis, parseUtcOffsetToMillis("-01:10"));

        assertEquals("01:10", toUtcOffsetString(hourAndTenInMillis));
        assertEquals("-01:10", toUtcOffsetString(-hourAndTenInMillis));

        assertParseUtcFails("01:60");
        assertParseUtcFails("01:90");
        assertParseUtcFails("01:-1");
    }

    private void assertParseUtcFails(String s) {
        try {
            parseUtcOffsetToMillis(s);
            fail();
        } catch (ParseException expected) {

        }
    }

    @Test
    public void allUnique() throws Exception {
        assertTrue(Utils.allUnique(list()));
        assertTrue(Utils.allUnique(list("One")));
        assertTrue(Utils.allUnique(list("One", "Two")));
        assertFalse(Utils.allUnique(list("One", "Two", "One")));
    }

    @Test
    public void setEquals() throws Exception {
        assertTrue(Utils.setEquals(list("One", "Two"), list("Two", "One")));
        // Dupes allowed.
        assertTrue(Utils.setEquals(list("One", "One", "Two"), list("Two", "One")));

        assertFalse(Utils.setEquals(list("One", "Two", "Three"), list("Two", "One")));
    }

    private static <X> List<X> list(X... values) {
        return Arrays.asList(values);
    }

    private static <X> Set<X> set(X... values) {
        return new HashSet<>(Arrays.asList(values));
    }
}
