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
package libcore.timezone.testing;

import java.io.ByteArrayOutputStream;
import java.nio.ByteBuffer;
import java.nio.charset.StandardCharsets;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * Helps with creating valid and invalid test data.
 */
public class ZoneInfoTestHelper {

    private ZoneInfoTestHelper() {}

    /**
     * Constructs valid and invalid zic data for tests. Currently it only creates minimal TZif
     * version 1 data necessary to be compatible with Android's ZoneInfo class.
     */
    public static class ZicDataBuilder {

        private int magic = 0x545a6966; // Default, valid magic.
        private Integer transitionCountOverride; // Used to override the correct transition count.
        private int[] transitionTimes; // Time of each transition, one per transition.
        private byte[] transitionTypes; // Type of each transition, one per transition.
        private Integer typesCountOverride; // Used to override the correct type count.
        private int[] isDsts; // Whether a type uses DST, one per type.
        private int[] offsetsSeconds; // The UTC offset, one per type.

        public ZicDataBuilder() {}

        public ZicDataBuilder setMagic(int magic) {
            this.magic = magic;
            return this;
        }

        public ZicDataBuilder setTypeCountOverride(int typesCountOverride) {
            this.typesCountOverride = typesCountOverride;
            return this;
        }

        public ZicDataBuilder setTransitionCountOverride(int transitionCountOverride) {
            this.transitionCountOverride = transitionCountOverride;
            return this;
        }

        /**
         * See {@link #setTransitions(int[][])} and {@link #setTypes(int[][])}.
         */
        public ZicDataBuilder setTransitionsAndTypes(
                int[][] transitionPairs, int[][] typePairs) {
            setTransitions(transitionPairs);
            setTypes(typePairs);
            return this;
        }
        /**
         * Sets transition information using an array of pairs of ints. e.g.
         *
         * new int[][] {
         *   { transitionTimeSeconds1, typeIndex1 },
         *   { transitionTimeSeconds2, typeIndex1 },
         * }
         */
        public ZicDataBuilder setTransitions(int[][] transitionPairs) {
            int[] transitions = new int[transitionPairs.length];
            byte[] types = new byte[transitionPairs.length];
            for (int i = 0; i < transitionPairs.length; i++) {
                transitions[i] = transitionPairs[i][0];
                types[i] = (byte) transitionPairs[i][1];
            }
            this.transitionTimes = transitions;
            this.transitionTypes = types;
            return this;
        }

        /**
         * Sets transition information using an array of pairs of ints. e.g.
         *
         * new int[][] {
         *   { typeIsDst1, offsetSeconds1 },
         *   { typeIsDst2, offsetSeconds2 },
         * }
         */
        public ZicDataBuilder setTypes(int[][] typePairs) {
            int[] isDsts = new int[typePairs.length];
            int[] offsetSeconds = new int[typePairs.length];
            for (int i = 0; i < typePairs.length; i++) {
                offsetSeconds[i] = typePairs[i][0];
                isDsts[i] = typePairs[i][1];
            }
            this.isDsts = isDsts;
            this.offsetsSeconds = offsetSeconds;
            return this;
        }

        /** Initializes to a minimum viable ZoneInfo data. */
        public ZicDataBuilder initializeToValid() {
            setTransitions(new int[0][0]);
            setTypes(new int[][] {
                    { 3600, 0}
            });
            return this;
        }

        public byte[] build() {
            ByteArrayOutputStream baos = new ByteArrayOutputStream();

            // Magic number.
            writeInt(baos, magic);

            // Some useless stuff in the header.
            for (int i = 0; i < 28; ++i) {
                baos.write(i);
            }

            // Transition time count
            int transitionsCount = transitionCountOverride != null
                    ? transitionCountOverride : transitionTimes.length;
            writeInt(baos, transitionsCount);

            // Transition type count.
            int typesCount = typesCountOverride != null
                    ? typesCountOverride : offsetsSeconds.length;
            writeInt(baos, typesCount);
            // Useless stuff.
            writeInt(baos, 0xdeadbeef);

            // Transition time array, as ints.
            writeIntArray(baos, transitionTimes);

            // Transition type array.
            writeByteArray(baos, transitionTypes);

            // Offset / DST
            for (int i = 0; i < offsetsSeconds.length; i++) {
                writeInt(baos, offsetsSeconds[i]);
                writeByte(baos, isDsts[i]);
                // Useless stuff.
                writeByte(baos, i);
            }
            return baos.toByteArray();
        }
    }

    /**
     * Constructs valid and invalid tzdata files for tests. See also ZoneCompactor class in
     * system/timezone/zone_compactor which is the real thing.
     */
    public static class TzDataBuilder {

        private String headerMagic;
        // A list is used in preference to a Map to allow simulation of badly ordered / duplicate
        // IDs.
        private List<ZicDatum> zicData = new ArrayList<>();
        private String zoneTab;
        private Integer indexOffsetOverride;
        private Integer dataOffsetOverride;
        private Integer zoneTabOffsetOverride;

        public TzDataBuilder() {}

        /** Sets the header. A valid header is in the form "tzdata2016g". */
        public TzDataBuilder setHeaderMagic(String headerMagic) {
            this.headerMagic = headerMagic;
            return this;
        }

        public TzDataBuilder setIndexOffsetOverride(int indexOffset) {
            this.indexOffsetOverride = indexOffset;
            return this;
        }

        public TzDataBuilder setDataOffsetOverride(int dataOffset) {
            this.dataOffsetOverride = dataOffset;
            return this;
        }

        public TzDataBuilder setZoneTabOffsetOverride(int zoneTabOffset) {
            this.zoneTabOffsetOverride = zoneTabOffset;
            return this;
        }

        /**
         * Adds data for a new zone. These must be added in ID string order to generate
         * a valid file.
         */
        public TzDataBuilder addZicData(String id, byte[] data) {
            zicData.add(new ZicDatum(id, data));
            return this;
        }

        public TzDataBuilder setZoneTab(String zoneTab) {
            this.zoneTab = zoneTab;
            return this;
        }

        public TzDataBuilder initializeToValid() {
            setHeaderMagic("tzdata9999a");
            addZicData("Europe/Elbonia", new ZicDataBuilder().initializeToValid().build());
            setZoneTab("ZoneTab data");
            return this;
        }

        public TzDataBuilder clearZicData() {
            zicData.clear();
            return this;
        }

        public byte[] build() {
            ByteArrayOutputStream baos = new ByteArrayOutputStream();

            byte[] headerMagicBytes = headerMagic.getBytes(StandardCharsets.US_ASCII);
            baos.write(headerMagicBytes, 0, headerMagicBytes.length);
            baos.write(0);

            // Write out the offsets for later manipulation.
            int indexOffsetOffset = baos.size();
            writeInt(baos, 0);
            int dataOffsetOffset = baos.size();
            writeInt(baos, 0);
            int zoneTabOffsetOffset = baos.size();
            writeInt(baos, 0);

            // Construct the data section in advance, so we know the offsets.
            ByteArrayOutputStream dataBytes = new ByteArrayOutputStream();
            Map<String, Integer> offsets = new HashMap<>();
            for (ZicDatum datum : zicData) {
                int offset = dataBytes.size();
                offsets.put(datum.id, offset);
                writeByteArray(dataBytes, datum.data);
            }

            int indexOffset = baos.size();

            // Write the index section.
            for (ZicDatum zicDatum : zicData) {
                // Write the ID.
                String id = zicDatum.id;
                byte[] idBytes = id.getBytes(StandardCharsets.US_ASCII);
                byte[] paddedIdBytes = new byte[40];
                System.arraycopy(idBytes, 0, paddedIdBytes, 0, idBytes.length);
                writeByteArray(baos, paddedIdBytes);
                // Write offset of zic data in the data section.
                Integer offset = offsets.get(id);
                writeInt(baos, offset);
                // Write the length of the zic data.
                writeInt(baos, zicDatum.data.length);
                // Write a filler value (not used)
                writeInt(baos, 0);
            }

            // Write the data section.
            int dataOffset = baos.size();
            writeByteArray(baos, dataBytes.toByteArray());

            // Write the zoneTab section.
            int zoneTabOffset = baos.size();
            byte[] zoneTabBytes = zoneTab.getBytes(StandardCharsets.US_ASCII);
            writeByteArray(baos, zoneTabBytes);

            byte[] bytes = baos.toByteArray();
            setInt(bytes, indexOffsetOffset,
                    indexOffsetOverride != null ? indexOffsetOverride : indexOffset);
            setInt(bytes, dataOffsetOffset,
                    dataOffsetOverride != null ? dataOffsetOverride : dataOffset);
            setInt(bytes, zoneTabOffsetOffset,
                    zoneTabOffsetOverride != null ? zoneTabOffsetOverride : zoneTabOffset);
            return bytes;
        }

        private static class ZicDatum {
            public final String id;
            public final byte[] data;

            ZicDatum(String id, byte[] data) {
                this.id = id;
                this.data = data;
            }
        }
    }

    static void writeByteArray(ByteArrayOutputStream baos, byte[] array) {
        baos.write(array, 0, array.length);
    }

    static void writeByte(ByteArrayOutputStream baos, int value) {
        baos.write(value);
    }

    static void writeIntArray(ByteArrayOutputStream baos, int[] array) {
        for (int value : array) {
            writeInt(baos, value);
        }
    }

    static void writeInt(ByteArrayOutputStream os, int value) {
        byte[] bytes = ByteBuffer.allocate(4).putInt(value).array();
        writeByteArray(os, bytes);
    }

    static void setInt(byte[] bytes, int offset, int value) {
        bytes[offset] = (byte) (value >>> 24);
        bytes[offset + 1] = (byte) (value >>> 16);
        bytes[offset + 2] = (byte) (value >>> 8);
        bytes[offset + 3] = (byte) value;
    }
}
