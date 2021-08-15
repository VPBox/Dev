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

import java.text.DateFormat;
import java.text.DecimalFormat;
import java.text.ParseException;
import java.util.Collection;
import java.util.HashSet;
import java.util.List;
import java.util.Set;
import java.util.TimeZone;
import java.util.concurrent.TimeUnit;
import java.util.function.Predicate;
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import java.util.stream.Collectors;

/**
 * Arbitrary static utility methods.
 */
final class Utils {

    private static final DateFormat UTC_FORMAT;
    static {
        UTC_FORMAT = DateFormat.getDateTimeInstance(DateFormat.LONG, DateFormat.LONG);
        UTC_FORMAT.setTimeZone(TimeZone.getTimeZone("UTC"));
    }

    private Utils() {}

    static List<String> toUpperCase(List<String> strings) {
        return strings.stream().map(String::toUpperCase).collect(Collectors.toList());
    }

    static long parseUtcOffsetToMillis(String utcOffsetString) throws ParseException {
        Pattern offsetPattern = Pattern.compile("(-?\\d?\\d):(\\d\\d)");
        Matcher matcher = offsetPattern.matcher(utcOffsetString);
        if (!matcher.matches()) {
            throw new ParseException("Invalid offset string: " + utcOffsetString, 0);
        }
        int minutes = Integer.parseInt(matcher.group(2));
        if (minutes > 59) {
            throw new ParseException("Invalid offset string: " + utcOffsetString, 0);
        }

        long millis = TimeUnit.HOURS.toMillis(Integer.parseInt(matcher.group(1)));
        long minutesInMillis = TimeUnit.MINUTES.toMillis(minutes);
        if (millis < 0) {
            millis -= minutesInMillis;
        } else {
            millis += minutesInMillis;
        }
        return millis;
    }

    static String toUtcOffsetString(long offsetMillis) {
        boolean negative = false;
        if (offsetMillis < 0) {
            negative = true;
            offsetMillis = Math.abs(offsetMillis);
        }
        long hours = TimeUnit.MILLISECONDS.toHours(offsetMillis);
        long minutes = TimeUnit.MILLISECONDS.toMinutes(offsetMillis - TimeUnit.HOURS.toMillis(hours));
        DecimalFormat formatter = new DecimalFormat("00");
        return (negative ? "-" : "") + formatter.format(hours) + ":" + formatter.format(minutes);
    }

    /** Returns (a - b) as a new Set. */
    static <X> Set<X> subtract(Set<X> a, Set<X> b) {
        Set<X> result = set(a);
        result.removeAll(b);
        return result;
    }

    static <X> boolean allUnique(Collection<X> values) {
        Set<X> set = set(values);
        return values.size() == set.size();
    }

    static <X> boolean setEquals(Collection<X> a, Collection<X> b) {
        Set<X> aSet = set(a);
        Set<X> bSet = set(b);
        return aSet.equals(bSet);
    }

    static <X> Set<X> set(Collection<X> values) {
        return new HashSet<>(values);
    }

    static boolean allLowerCaseAscii(List<String> strings) {
        Predicate<String> onlyLowerCaseAscii = s -> {
            for (char c : s.toCharArray()) {
                if (c < 'a' || c > 'z') {
                    return false;
                }
            }
            return true;
        };
        Predicate<String> nonLowerCaseAscii = onlyLowerCaseAscii.negate();
        return !strings.stream().anyMatch(nonLowerCaseAscii);
    }

    static String formatUtc(long timeMillis) {
        return UTC_FORMAT.format(timeMillis);
    }
}
