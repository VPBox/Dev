/*
 * Copyright (C) 2018 The Android Open Source Project
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
package com.android.libcore.timezone.tzlookup.zonetree;

import com.ibm.icu.text.TimeZoneNames;
import com.ibm.icu.util.BasicTimeZone;
import com.ibm.icu.util.TimeZone;
import com.ibm.icu.util.TimeZoneTransition;

import java.time.Instant;
import java.util.List;
import java.util.Objects;

/**
 * A period of time when all time-zone related properties are expected to remain the same.
 */
final class ZoneOffsetPeriod {
    /** The start of the period (inclusive) */
    private final Instant start;
    /** The end of the period (exclusive) */
    private final Instant end;
    /** The offset from UTC in milliseconds. */
    private final int rawOffsetMillis;
    /** The additional offset to apply due to DST */
    private final int dstOffsetMillis;
    /** A name for the time. */
    private final String name;

    private ZoneOffsetPeriod(Instant start, Instant end, int rawOffsetMillis, int dstOffsetMillis,
            String name) {
        this.start = start;
        this.end = end;
        this.rawOffsetMillis = rawOffsetMillis;
        this.dstOffsetMillis = dstOffsetMillis;
        this.name = name;
    }

    /**
     * Constructs an instance using ICU data.
     */
    public static ZoneOffsetPeriod create(TimeZoneNames timeZoneNames, BasicTimeZone timeZone,
            Instant minTime, Instant maxTime) {

        long startMillis = minTime.toEpochMilli();
        TimeZoneTransition transition =
                timeZone.getNextTransition(startMillis, true /* inclusive */);
        Instant end;
        if (transition == null) {
            // The zone has no transitions from start, so we create a ZoneOffsetPeriod
            // from minTime to maxTime.
            end = maxTime;
        } else {
            TimeZoneTransition nextTransition =
                    timeZone.getNextTransition(startMillis, false /* inclusive */);
            if (nextTransition != null) {
                long endTimeMillis = Math.min(nextTransition.getTime(), maxTime.toEpochMilli());
                end = Instant.ofEpochMilli(endTimeMillis);
            } else {
                // The zone has no next transition after minTime, so we create a ZoneOffsetPeriod
                // from minTime to maxTime.
                end = maxTime;
            }
        }

        String longName = getNameAtTime(timeZoneNames, timeZone, startMillis);
        int[] offsets = new int[2];
        timeZone.getOffset(startMillis, false /* local */, offsets);
        return new ZoneOffsetPeriod(minTime, end, offsets[0], offsets[1], longName);
    }


    /** Splits a period in two at the specified instant, returning the generated periods. */
    public static ZoneOffsetPeriod[] splitAtTime(
            ZoneOffsetPeriod toSplit, TimeZoneNames timeZoneNames, BasicTimeZone timeZone,
            Instant partitionInstant) {
        if (!partitionInstant.isAfter(toSplit.start)
                || !partitionInstant.isBefore(toSplit.end)) {
            throw new IllegalArgumentException(partitionInstant + " is not between "
                    + toSplit.start + " and " + toSplit.end);
        }
        // Work out the name at the split so the name is always the name at the beginning of the
        // zone offset period.
        String nameAtSplit =
                getNameAtTime(timeZoneNames, timeZone, partitionInstant.toEpochMilli());
        int rawOffsetMillis = toSplit.rawOffsetMillis;
        int dstOffsetMillis = toSplit.dstOffsetMillis;
        return new ZoneOffsetPeriod[] {
                new ZoneOffsetPeriod(toSplit.start, partitionInstant, rawOffsetMillis,
                        dstOffsetMillis, toSplit.name),
                new ZoneOffsetPeriod(partitionInstant, toSplit.end, rawOffsetMillis,
                        dstOffsetMillis, nameAtSplit)
        };
    }

    public Instant getStartInstant() {
        return start;
    }

    public Instant getEndInstant() {
        return end;
    }

    public long getStartMillis() {
        return start.toEpochMilli();
    }

    public long getEndMillis() {
        return end.toEpochMilli();
    }

    public String getName() {
        return name;
    }

    public int getRawOffsetMillis() {
        return rawOffsetMillis;
    }

    public int getDstOffsetMillis() {
        return dstOffsetMillis;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) {
            return true;
        }
        if (o == null || getClass() != o.getClass()) {
            return false;
        }
        ZoneOffsetPeriod that = (ZoneOffsetPeriod) o;
        return rawOffsetMillis == that.rawOffsetMillis &&
                dstOffsetMillis == that.dstOffsetMillis &&
                Objects.equals(start, that.start) &&
                Objects.equals(end, that.end) &&
                Objects.equals(name, that.name);
    }

    @Override
    public int hashCode() {
        return Objects.hash(start, end, rawOffsetMillis, dstOffsetMillis, name);
    }

    @Override
    public String toString() {
        return "ZoneOffsetPeriod{" +
                "start=" + start +
                ", end=" + end +
                ", rawOffsetMillis=" + rawOffsetMillis +
                ", dstOffsetMillis=" + dstOffsetMillis +
                ", name='" + name + '\'' +
                '}';
    }

    /**
     * A class for establishing when multiple periods are identical.
     */
    static final class ZonePeriodsKey {

        private final List<ZoneOffsetPeriod> periods;

        public ZonePeriodsKey(List<ZoneOffsetPeriod> periods) {
            this.periods = periods;
        }

        @Override
        public boolean equals(Object o) {
            if (this == o) {
                return true;
            }
            if (o == null || getClass() != o.getClass()) {
                return false;
            }
            ZonePeriodsKey zoneKey = (ZonePeriodsKey) o;
            return Objects.equals(periods, zoneKey.periods);
        }

        @Override
        public int hashCode() {
            return Objects.hash(periods);
        }

        @Override
        public String toString() {
            return "ZonePeriodsKey{" +
                    "periods=" + periods +
                    '}';
        }
    }

    private static String getNameAtTime(
            TimeZoneNames timeZoneNames, BasicTimeZone timeZone, long startMillis) {
        int[] offsets = new int[2];
        timeZone.getOffset(startMillis, false /* local */, offsets);
        String canonicalID = TimeZone.getCanonicalID(timeZone.getID());
        TimeZoneNames.NameType longNameType = offsets[1] == 0
                ? TimeZoneNames.NameType.LONG_STANDARD : TimeZoneNames.NameType.LONG_DAYLIGHT;
        return timeZoneNames.getDisplayName(canonicalID, longNameType, startMillis);
    }
}
