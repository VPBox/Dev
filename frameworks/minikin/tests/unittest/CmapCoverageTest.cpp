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

#include "minikin/CmapCoverage.h"

#include <random>

#include <gtest/gtest.h>
#include <log/log.h>

#include "minikin/SparseBitSet.h"

#include "MinikinInternal.h"

namespace minikin {

static constexpr uint16_t VS_PLATFORM_ID = 0;
static constexpr uint16_t VS_ENCODING_ID = 5;

size_t writeU8(uint8_t x, uint8_t* out, size_t offset) {
    out[offset] = x;
    return offset + 1;
}

size_t writeU16(uint16_t x, uint8_t* out, size_t offset) {
    out[offset] = x >> 8;
    out[offset + 1] = x;
    return offset + 2;
}

size_t writeI16(int16_t sx, uint8_t* out, size_t offset) {
    return writeU16(static_cast<uint16_t>(sx), out, offset);
}

size_t writeU24(uint32_t x, uint8_t* out, size_t offset) {
    out[offset] = x >> 16;
    out[offset + 1] = x >> 8;
    out[offset + 2] = x;
    return offset + 3;
}

size_t writeU32(uint32_t x, uint8_t* out, size_t offset) {
    out[offset] = x >> 24;
    out[offset + 1] = x >> 16;
    out[offset + 2] = x >> 8;
    out[offset + 3] = x;
    return offset + 4;
}

// Returns valid cmap format 4 table contents. All glyph ID is same value as code point. (e.g.
// 'a' (U+0061) is mapped to Glyph ID = 0x0061).
// 'range' should be specified with inclusive-inclusive values.
static std::vector<uint8_t> buildCmapFormat4Table(const std::vector<uint16_t>& ranges) {
    uint16_t segmentCount = ranges.size() / 2 + 1 /* +1 for end marker */;

    const size_t numOfUint16 = 8 /* format, length, languages, segCountX2, searchRange,
                                    entrySelector, rangeShift, pad */
                               +
                               segmentCount * 4 /* endCount, startCount, idRange, idRangeOffset */;
    const size_t finalLength = sizeof(uint16_t) * numOfUint16;

    std::vector<uint8_t> out(finalLength);
    size_t head = 0;
    head = writeU16(4, out.data(), head);            // format
    head = writeU16(finalLength, out.data(), head);  // length
    head = writeU16(0, out.data(), head);            // langauge

    const uint16_t searchRange = 2 * (1 << static_cast<int>(floor(log2(segmentCount))));

    head = writeU16(segmentCount * 2, out.data(), head);                // segCountX2
    head = writeU16(searchRange, out.data(), head);                     // searchRange
    head = writeU16(__builtin_ctz(searchRange) - 1, out.data(), head);  // entrySelector
    head = writeU16(segmentCount * 2 - searchRange, out.data(), head);  // rangeShift

    size_t endCountHead = head;
    size_t startCountHead = head + segmentCount * sizeof(uint16_t) + 2 /* padding */;
    size_t idDeltaHead = startCountHead + segmentCount * sizeof(uint16_t);
    size_t idRangeOffsetHead = idDeltaHead + segmentCount * sizeof(uint16_t);

    for (size_t i = 0; i < ranges.size() / 2; ++i) {
        const uint16_t begin = ranges[i * 2];
        const uint16_t end = ranges[i * 2 + 1];
        startCountHead = writeU16(begin, out.data(), startCountHead);
        endCountHead = writeU16(end, out.data(), endCountHead);
        // map glyph ID as the same value of the code point.
        idDeltaHead = writeU16(0, out.data(), idDeltaHead);
        idRangeOffsetHead = writeU16(0 /* we don't use this */, out.data(), idRangeOffsetHead);
    }

    // fill end marker
    endCountHead = writeU16(0xFFFF, out.data(), endCountHead);
    startCountHead = writeU16(0xFFFF, out.data(), startCountHead);
    idDeltaHead = writeU16(1, out.data(), idDeltaHead);
    idRangeOffsetHead = writeU16(0, out.data(), idRangeOffsetHead);
    LOG_ALWAYS_FATAL_IF(endCountHead > finalLength);
    LOG_ALWAYS_FATAL_IF(startCountHead > finalLength);
    LOG_ALWAYS_FATAL_IF(idDeltaHead > finalLength);
    LOG_ALWAYS_FATAL_IF(idRangeOffsetHead != finalLength);
    return out;
}

// Returns valid cmap format 4 table contents. All glyph ID is same value as code point. (e.g.
// 'a' (U+0061) is mapped to Glyph ID = 0x0061).
// 'range' should be specified with inclusive-inclusive values.
static std::vector<uint8_t> buildCmapFormat12Table(const std::vector<uint32_t>& ranges) {
    uint32_t numGroups = ranges.size() / 2;

    const size_t finalLength = 2 /* format */ + 2 /* reserved */ + 4 /* length */ +
                               4 /* languages */ + 4 /* numGroups */ +
                               12 /* size of a group */ * numGroups;

    std::vector<uint8_t> out(finalLength);
    size_t head = 0;
    head = writeU16(12, out.data(), head);           // format
    head = writeU16(0, out.data(), head);            // reserved
    head = writeU32(finalLength, out.data(), head);  // length
    head = writeU32(0, out.data(), head);            // langauge
    head = writeU32(numGroups, out.data(), head);    // numGroups

    for (uint32_t i = 0; i < numGroups; ++i) {
        const uint32_t start = ranges[2 * i];
        const uint32_t end = ranges[2 * i + 1];
        head = writeU32(start, out.data(), head);
        head = writeU32(end, out.data(), head);
        // map glyph ID as the same value of the code point.
        // TODO: Use glyph IDs lower than 65535.
        // Cmap can store 32 bit glyph ID but due to the size of numGlyph, a font file can contain
        // up to 65535 glyphs in a file.
        head = writeU32(start, out.data(), head);
    }

    LOG_ALWAYS_FATAL_IF(head != finalLength);
    return out;
}

struct VariationSelectorRecord {
    uint32_t codePoint;
    std::vector<uint32_t> defaultUVSRanges;
    std::vector<uint32_t> nonDefaultUVS;

    std::vector<uint8_t> getDefaultUVSAsBinary() const {
        if (defaultUVSRanges.empty()) {
            return std::vector<uint8_t>();
        }
        const size_t numOfRanges = defaultUVSRanges.size() / 2;
        const size_t length = sizeof(uint32_t) /* numUnicodeValueRanges */ +
                              numOfRanges * 4 /* size of Unicode Range Table */;

        std::vector<uint8_t> out(length);
        size_t head = 0;
        head = writeU32(numOfRanges, out.data(), head);
        for (size_t i = 0; i < numOfRanges; ++i) {
            const uint32_t startUnicodeValue = defaultUVSRanges[i * 2];
            const uint32_t endUnicodeValue = defaultUVSRanges[i * 2 + 1];
            head = writeU24(startUnicodeValue, out.data(), head);
            head = writeU8(endUnicodeValue - startUnicodeValue, out.data(), head);
        }
        LOG_ALWAYS_FATAL_IF(head != length);
        return out;
    }

    std::vector<uint8_t> getNonDefaultUVSAsBinary() const {
        if (nonDefaultUVS.empty()) {
            return std::vector<uint8_t>();
        }
        const size_t length = sizeof(uint32_t) /* numUnicodeValueRanges */ +
                              nonDefaultUVS.size() * 5 /* size of UVS Mapping Record */;

        std::vector<uint8_t> out(length);
        size_t head = 0;
        head = writeU32(nonDefaultUVS.size(), out.data(), head);
        for (uint32_t codePoint : nonDefaultUVS) {
            head = writeU24(codePoint, out.data(), head);
            head = writeU16(4 /* fixed glyph id */, out.data(), head);
        }
        LOG_ALWAYS_FATAL_IF(head != length);
        return out;
    }
};

static std::vector<uint8_t> buildCmapFormat14Table(
        const std::vector<VariationSelectorRecord>& vsRecords) {
    const size_t headerLength = sizeof(uint16_t) /* format */ + sizeof(uint32_t) /* length */ +
                                sizeof(uint32_t) /* numVarSelectorRecords */ +
                                11 /* size of variation selector record */ * vsRecords.size();

    std::vector<uint8_t> out(headerLength);
    size_t head = 0;
    head = writeU16(14, out.data(), head);                // format
    head += sizeof(uint32_t);                             // length will be filled later
    head = writeU32(vsRecords.size(), out.data(), head);  // numVarSelectorRecords;

    for (const auto& record : vsRecords) {
        const uint32_t vsCodePoint = record.codePoint;
        head = writeU24(vsCodePoint, out.data(), head);

        std::vector<uint8_t> defaultUVS = record.getDefaultUVSAsBinary();
        if (defaultUVS.empty()) {
            head = writeU32(0, out.data(), head);
        } else {
            head = writeU32(out.size(), out.data(), head);
            out.insert(out.end(), defaultUVS.begin(), defaultUVS.end());
        }

        std::vector<uint8_t> nonDefaultUVS = record.getNonDefaultUVSAsBinary();
        if (nonDefaultUVS.empty()) {
            head = writeU32(0, out.data(), head);
        } else {
            head = writeU32(out.size(), out.data(), head);
            out.insert(out.end(), nonDefaultUVS.begin(), nonDefaultUVS.end());
        }
    }
    LOG_ALWAYS_FATAL_IF(head != headerLength);
    writeU32(out.size(), out.data(), 2);  // fill the length.
    return out;
}

class CmapBuilder {
public:
    static constexpr size_t kEncodingTableHead = 4;
    static constexpr size_t kEncodingTableSize = 8;

    CmapBuilder(int numTables) : mNumTables(numTables), mCurrentTableIndex(0) {
        const size_t headerSize =
                2 /* version */ + 2 /* numTables */ + kEncodingTableSize * numTables;
        out.resize(headerSize);
        writeU16(0, out.data(), 0);
        writeU16(numTables, out.data(), 2);
    }

    void appendTable(uint16_t platformId, uint16_t encodingId, const std::vector<uint8_t>& table) {
        appendEncodingTable(platformId, encodingId, out.size());
        out.insert(out.end(), table.begin(), table.end());
    }

    std::vector<uint8_t> build() {
        LOG_ALWAYS_FATAL_IF(mCurrentTableIndex != mNumTables);
        return out;
    }

    // Helper functions.
    static std::vector<uint8_t> buildSingleFormat4Cmap(uint16_t platformId, uint16_t encodingId,
                                                       const std::vector<uint16_t>& ranges) {
        CmapBuilder builder(1);
        builder.appendTable(platformId, encodingId, buildCmapFormat4Table(ranges));
        return builder.build();
    }

    static std::vector<uint8_t> buildSingleFormat12Cmap(uint16_t platformId, uint16_t encodingId,
                                                        const std::vector<uint32_t>& ranges) {
        CmapBuilder builder(1);
        builder.appendTable(platformId, encodingId, buildCmapFormat12Table(ranges));
        return builder.build();
    }

private:
    void appendEncodingTable(uint16_t platformId, uint16_t encodingId, uint32_t offset) {
        LOG_ALWAYS_FATAL_IF(mCurrentTableIndex == mNumTables);

        const size_t currentEncodingTableHead =
                kEncodingTableHead + mCurrentTableIndex * kEncodingTableSize;
        size_t head = writeU16(platformId, out.data(), currentEncodingTableHead);
        head = writeU16(encodingId, out.data(), head);
        head = writeU32(offset, out.data(), head);
        LOG_ALWAYS_FATAL_IF((head - currentEncodingTableHead) != kEncodingTableSize);
        mCurrentTableIndex++;
    }

    int mNumTables;
    int mCurrentTableIndex;
    std::vector<uint8_t> out;
};

TEST(CmapCoverageTest, SingleFormat4_brokenCmap) {
    std::vector<std::unique_ptr<SparseBitSet>> vsTables;
    {
        SCOPED_TRACE("Reading beyond buffer size - Too small cmap size");
        std::vector<uint8_t> cmap =
                CmapBuilder::buildSingleFormat4Cmap(0, 0, std::vector<uint16_t>({'a', 'a'}));

        SparseBitSet coverage =
                CmapCoverage::getCoverage(cmap.data(), 3 /* too small */, &vsTables);
        EXPECT_EQ(0U, coverage.length());
        EXPECT_TRUE(vsTables.empty());
    }
    {
        SCOPED_TRACE("Reading beyond buffer size - space needed for tables goes beyond cmap size");
        std::vector<uint8_t> cmap =
                CmapBuilder::buildSingleFormat4Cmap(0, 0, std::vector<uint16_t>({'a', 'a'}));

        writeU16(1000, cmap.data(), 2 /* offset of num tables in cmap header */);
        SparseBitSet coverage = CmapCoverage::getCoverage(cmap.data(), cmap.size(), &vsTables);
        EXPECT_EQ(0U, coverage.length());
        EXPECT_TRUE(vsTables.empty());
    }
    {
        SCOPED_TRACE("Reading beyond buffer size - Invalid offset in encoding table");
        std::vector<uint8_t> cmap =
                CmapBuilder::buildSingleFormat4Cmap(0, 0, std::vector<uint16_t>({'a', 'a'}));

        writeU16(1000, cmap.data(), 8 /* offset of the offset in the first encoding record */);
        SparseBitSet coverage = CmapCoverage::getCoverage(cmap.data(), cmap.size(), &vsTables);
        EXPECT_EQ(0U, coverage.length());
        EXPECT_TRUE(vsTables.empty());
    }
    {
        SCOPED_TRACE("Reversed range");
        std::vector<uint8_t> cmap = CmapBuilder::buildSingleFormat4Cmap(
                0, 0, std::vector<uint16_t>({'b', 'b', 'a', 'a'}));

        SparseBitSet coverage = CmapCoverage::getCoverage(cmap.data(), cmap.size(), &vsTables);
        EXPECT_EQ(0U, coverage.length());
        EXPECT_TRUE(vsTables.empty());
    }
    {
        SCOPED_TRACE("Reversed range - partially readable");
        std::vector<uint8_t> cmap = CmapBuilder::buildSingleFormat4Cmap(
                0, 0, std::vector<uint16_t>({'a', 'a', 'c', 'c', 'b', 'b'}));

        SparseBitSet coverage = CmapCoverage::getCoverage(cmap.data(), cmap.size(), &vsTables);
        EXPECT_EQ(0U, coverage.length());
        EXPECT_TRUE(vsTables.empty());
    }
}

TEST(CmapCoverageTest, SingleFormat4) {
    std::vector<std::unique_ptr<SparseBitSet>> vsTables;
    struct TestCast {
        std::string testTitle;
        uint16_t platformId;
        uint16_t encodingId;
    } TEST_CASES[] = {
            {"Platform 0, Encoding 0", 0, 0}, {"Platform 0, Encoding 1", 0, 1},
            {"Platform 0, Encoding 2", 0, 2}, {"Platform 0, Encoding 3", 0, 3},
            {"Platform 3, Encoding 1", 3, 1},
    };

    for (const auto& testCase : TEST_CASES) {
        SCOPED_TRACE(testCase.testTitle.c_str());
        std::vector<uint8_t> cmap = CmapBuilder::buildSingleFormat4Cmap(
                testCase.platformId, testCase.encodingId, std::vector<uint16_t>({'a', 'a'}));
        SparseBitSet coverage = CmapCoverage::getCoverage(cmap.data(), cmap.size(), &vsTables);
        EXPECT_TRUE(coverage.get('a'));
        EXPECT_FALSE(coverage.get('b'));
        EXPECT_TRUE(vsTables.empty());
    }
}

TEST(CmapCoverageTest, SingleFormat12) {
    std::vector<std::unique_ptr<SparseBitSet>> vsTables;

    struct TestCast {
        std::string testTitle;
        uint16_t platformId;
        uint16_t encodingId;
    } TEST_CASES[] = {
            {"Platform 0, Encoding 4", 0, 4},
            {"Platform 0, Encoding 6", 0, 6},
            {"Platform 3, Encoding 10", 3, 10},
    };

    for (const auto& testCase : TEST_CASES) {
        SCOPED_TRACE(testCase.testTitle.c_str());
        std::vector<uint8_t> cmap = CmapBuilder::buildSingleFormat12Cmap(
                testCase.platformId, testCase.encodingId, std::vector<uint32_t>({'a', 'a'}));
        SparseBitSet coverage = CmapCoverage::getCoverage(cmap.data(), cmap.size(), &vsTables);
        EXPECT_TRUE(coverage.get('a'));
        EXPECT_FALSE(coverage.get('b'));
        EXPECT_TRUE(vsTables.empty());
    }
}

TEST(CmapCoverageTest, Format12_beyondTheUnicodeLimit) {
    std::vector<std::unique_ptr<SparseBitSet>> vsTables;
    {
        SCOPED_TRACE("Starting range is out of Unicode code point. Should be ignored.");
        std::vector<uint8_t> cmap = CmapBuilder::buildSingleFormat12Cmap(
                0, 0, std::vector<uint32_t>({'a', 'a', 0x110000, 0x110000}));

        SparseBitSet coverage = CmapCoverage::getCoverage(cmap.data(), cmap.size(), &vsTables);
        EXPECT_TRUE(coverage.get('a'));
        EXPECT_FALSE(coverage.get(0x110000));
        EXPECT_TRUE(vsTables.empty());
    }
    {
        SCOPED_TRACE("Ending range is out of Unicode code point. Should be ignored.");
        std::vector<uint8_t> cmap = CmapBuilder::buildSingleFormat12Cmap(
                0, 0, std::vector<uint32_t>({'a', 'a', 0x10FF00, 0x110000}));

        SparseBitSet coverage = CmapCoverage::getCoverage(cmap.data(), cmap.size(), &vsTables);
        EXPECT_TRUE(coverage.get('a'));
        EXPECT_TRUE(coverage.get(0x10FF00));
        EXPECT_TRUE(coverage.get(0x10FFFF));
        EXPECT_FALSE(coverage.get(0x110000));
        EXPECT_TRUE(vsTables.empty());
    }
}

TEST(CmapCoverageTest, notSupportedEncodings) {
    std::vector<std::unique_ptr<SparseBitSet>> vsTables;

    struct TestCast {
        std::string testTitle;
        uint16_t platformId;
        uint16_t encodingId;
    } TEST_CASES[] = {
            // Any encodings with platform 2 is not supported.
            {"Platform 2, Encoding 0", 2, 0},
            {"Platform 2, Encoding 1", 2, 1},
            {"Platform 2, Encoding 2", 2, 2},
            {"Platform 2, Encoding 3", 2, 3},
            // UCS-2 or UCS-4 are supported on Platform == 3. Others are not supported.
            {"Platform 3, Encoding 0", 3, 0},  // Symbol
            {"Platform 3, Encoding 2", 3, 2},  // ShiftJIS
            {"Platform 3, Encoding 3", 3, 3},  // RPC
            {"Platform 3, Encoding 4", 3, 4},  // Big5
            {"Platform 3, Encoding 5", 3, 5},  // Wansung
            {"Platform 3, Encoding 6", 3, 6},  // Johab
            {"Platform 3, Encoding 7", 3, 7},  // Reserved
            {"Platform 3, Encoding 8", 3, 8},  // Reserved
            {"Platform 3, Encoding 9", 3, 9},  // Reserved
            // Uknown platforms
            {"Platform 4, Encoding 0", 4, 0},
            {"Platform 5, Encoding 1", 5, 1},
            {"Platform 6, Encoding 0", 6, 0},
            {"Platform 7, Encoding 1", 7, 1},
    };

    for (const auto& testCase : TEST_CASES) {
        SCOPED_TRACE(testCase.testTitle.c_str());
        CmapBuilder builder(1);
        std::vector<uint8_t> cmap = CmapBuilder::buildSingleFormat4Cmap(
                testCase.platformId, testCase.encodingId, std::vector<uint16_t>({'a', 'a'}));
        SparseBitSet coverage = CmapCoverage::getCoverage(cmap.data(), cmap.size(), &vsTables);
        EXPECT_EQ(0U, coverage.length());
        EXPECT_TRUE(vsTables.empty());
    }
}

TEST(CmapCoverageTest, brokenFormat4Table) {
    std::vector<std::unique_ptr<SparseBitSet>> vsTables;
    {
        SCOPED_TRACE("Too small table cmap size");
        std::vector<uint8_t> table = buildCmapFormat4Table(std::vector<uint16_t>({'a', 'a'}));
        table.resize(2);  // Remove trailing data.

        CmapBuilder builder(1);
        builder.appendTable(0, 0, table);
        std::vector<uint8_t> cmap = builder.build();

        SparseBitSet coverage = CmapCoverage::getCoverage(cmap.data(), cmap.size(), &vsTables);
        EXPECT_EQ(0U, coverage.length());
        EXPECT_TRUE(vsTables.empty());
    }
    {
        SCOPED_TRACE("Too many segments");
        std::vector<uint8_t> table = buildCmapFormat4Table(std::vector<uint16_t>({'a', 'a'}));
        writeU16(5000, table.data(), 6 /* segment count offset */);  // 5000 segments.
        CmapBuilder builder(1);
        builder.appendTable(0, 0, table);
        std::vector<uint8_t> cmap = builder.build();

        SparseBitSet coverage = CmapCoverage::getCoverage(cmap.data(), cmap.size(), &vsTables);
        EXPECT_EQ(0U, coverage.length());
        EXPECT_TRUE(vsTables.empty());
    }
    {
        SCOPED_TRACE("Inversed range");
        std::vector<uint8_t> table = buildCmapFormat4Table(std::vector<uint16_t>({'b', 'b'}));
        // Put smaller end code point to inverse the range.
        writeU16('a', table.data(), 14 /* the first element of endCount offset */);
        CmapBuilder builder(1);
        builder.appendTable(0, 0, table);
        std::vector<uint8_t> cmap = builder.build();

        SparseBitSet coverage = CmapCoverage::getCoverage(cmap.data(), cmap.size(), &vsTables);
        EXPECT_EQ(0U, coverage.length());
        EXPECT_TRUE(vsTables.empty());
    }
    {
        SCOPED_TRACE("Reversed end code points");
        std::vector<uint8_t> table =
                buildCmapFormat4Table(std::vector<uint16_t>({'b', 'b', 'a', 'a'}));
        CmapBuilder builder(1);
        builder.appendTable(0, 0, table);
        std::vector<uint8_t> cmap = builder.build();

        SparseBitSet coverage = CmapCoverage::getCoverage(cmap.data(), cmap.size(), &vsTables);
        EXPECT_EQ(0U, coverage.length());
        EXPECT_TRUE(vsTables.empty());
    }
}

TEST(CmapCoverageTest, duplicatedCmap4EntryTest) {
    std::vector<std::unique_ptr<SparseBitSet>> vsTables;
    std::vector<uint8_t> table = buildCmapFormat4Table(std::vector<uint16_t>({'a', 'b', 'b', 'b'}));
    CmapBuilder builder(1);
    builder.appendTable(0, 0, table);
    std::vector<uint8_t> cmap = builder.build();

    SparseBitSet coverage = CmapCoverage::getCoverage(cmap.data(), cmap.size(), &vsTables);
    EXPECT_TRUE(coverage.get('a'));
    EXPECT_TRUE(coverage.get('b'));
    EXPECT_TRUE(vsTables.empty());
}

TEST(CmapCoverageTest, brokenFormat12Table) {
    std::vector<std::unique_ptr<SparseBitSet>> vsTables;
    {
        SCOPED_TRACE("Too small cmap size");
        std::vector<uint8_t> table = buildCmapFormat12Table(std::vector<uint32_t>({'a', 'a'}));
        table.resize(2);  // Remove trailing data.

        CmapBuilder builder(1);
        builder.appendTable(0, 0, table);
        std::vector<uint8_t> cmap = builder.build();

        SparseBitSet coverage = CmapCoverage::getCoverage(cmap.data(), cmap.size(), &vsTables);
        EXPECT_EQ(0U, coverage.length());
        EXPECT_TRUE(vsTables.empty());
    }
    {
        SCOPED_TRACE("Too many groups");
        std::vector<uint8_t> table = buildCmapFormat12Table(std::vector<uint32_t>({'a', 'a'}));
        writeU32(5000, table.data(), 12 /* num group offset */);  // 5000 groups.

        CmapBuilder builder(1);
        builder.appendTable(0, 0, table);
        std::vector<uint8_t> cmap = builder.build();

        SparseBitSet coverage = CmapCoverage::getCoverage(cmap.data(), cmap.size(), &vsTables);
        EXPECT_EQ(0U, coverage.length());
        EXPECT_TRUE(vsTables.empty());
    }
    {
        SCOPED_TRACE("Inversed range.");
        std::vector<uint8_t> table = buildCmapFormat12Table(std::vector<uint32_t>({'a', 'a'}));
        // Put larger start code point to inverse the range.
        writeU32('b', table.data(), 16 /* start code point offset in the first  group */);

        CmapBuilder builder(1);
        builder.appendTable(0, 0, table);
        std::vector<uint8_t> cmap = builder.build();

        SparseBitSet coverage = CmapCoverage::getCoverage(cmap.data(), cmap.size(), &vsTables);
        EXPECT_EQ(0U, coverage.length());
        EXPECT_TRUE(vsTables.empty());
    }
    {
        SCOPED_TRACE("Too large code point");
        std::vector<uint8_t> cmap = CmapBuilder::buildSingleFormat12Cmap(
                0, 0, std::vector<uint32_t>({0x110000, 0x110000}));

        SparseBitSet coverage = CmapCoverage::getCoverage(cmap.data(), cmap.size(), &vsTables);
        EXPECT_EQ(0U, coverage.length());
        EXPECT_TRUE(vsTables.empty());
    }
    {
        SCOPED_TRACE("Reversed range");
        std::vector<uint8_t> cmap = CmapBuilder::buildSingleFormat12Cmap(
                0, 0, std::vector<uint32_t>({'b', 'b', 'a', 'a'}));
        SparseBitSet coverage = CmapCoverage::getCoverage(cmap.data(), cmap.size(), &vsTables);
        EXPECT_EQ(0U, coverage.length());
        EXPECT_TRUE(vsTables.empty());
    }
    {
        SCOPED_TRACE("Reversed range - partially readable");
        std::vector<uint8_t> cmap = CmapBuilder::buildSingleFormat12Cmap(
                0, 0, std::vector<uint32_t>({'a', 'a', 'c', 'c', 'b', 'b'}));
        SparseBitSet coverage = CmapCoverage::getCoverage(cmap.data(), cmap.size(), &vsTables);
        EXPECT_EQ(0U, coverage.length());
        EXPECT_TRUE(vsTables.empty());
    }
}

TEST(CmapCoverageTest, TableSelection_Priority) {
    std::vector<uint8_t> highestFormat12Table =
            buildCmapFormat12Table(std::vector<uint32_t>({'a', 'a'}));
    std::vector<uint8_t> highestFormat4Table =
            buildCmapFormat4Table(std::vector<uint16_t>({'a', 'a'}));
    std::vector<uint8_t> format4 = buildCmapFormat4Table(std::vector<uint16_t>({'b', 'b'}));
    std::vector<uint8_t> format12 = buildCmapFormat12Table(std::vector<uint32_t>({'b', 'b'}));

    std::vector<std::unique_ptr<SparseBitSet>> vsTables;
    {
        SCOPED_TRACE("(platform, encoding) = (3, 10) is the highest priority.");

        struct LowerPriorityTable {
            uint16_t platformId;
            uint16_t encodingId;
            const std::vector<uint8_t>& table;
        } LOWER_PRIORITY_TABLES[] = {
                {0, 0, format4},  {0, 1, format4},  {0, 2, format4}, {0, 3, format4},
                {0, 4, format12}, {0, 6, format12}, {3, 1, format4},
        };

        for (const auto& table : LOWER_PRIORITY_TABLES) {
            CmapBuilder builder(2);
            builder.appendTable(table.platformId, table.encodingId, table.table);
            builder.appendTable(3, 10, highestFormat12Table);
            std::vector<uint8_t> cmap = builder.build();

            SparseBitSet coverage = CmapCoverage::getCoverage(cmap.data(), cmap.size(), &vsTables);
            EXPECT_TRUE(coverage.get('a'));   // comes from highest table
            EXPECT_FALSE(coverage.get('b'));  // should not use other table.
            EXPECT_TRUE(vsTables.empty());
        }
    }
    {
        SCOPED_TRACE("(platform, encoding) = (3, 1) case");

        struct LowerPriorityTable {
            uint16_t platformId;
            uint16_t encodingId;
            const std::vector<uint8_t>& table;
        } LOWER_PRIORITY_TABLES[] = {
                {0, 0, format4}, {0, 1, format4}, {0, 2, format4}, {0, 3, format4},
        };

        for (const auto& table : LOWER_PRIORITY_TABLES) {
            CmapBuilder builder(2);
            builder.appendTable(table.platformId, table.encodingId, table.table);
            builder.appendTable(3, 1, highestFormat4Table);
            std::vector<uint8_t> cmap = builder.build();

            SparseBitSet coverage = CmapCoverage::getCoverage(cmap.data(), cmap.size(), &vsTables);
            EXPECT_TRUE(coverage.get('a'));   // comes from highest table
            EXPECT_FALSE(coverage.get('b'));  // should not use other table.
            EXPECT_TRUE(vsTables.empty());
        }
    }
}

TEST(CmapCoverageTest, TableSelection_SkipBrokenFormat4Table) {
    std::vector<uint8_t> validTable = buildCmapFormat4Table(std::vector<uint16_t>({'a', 'a'}));
    std::vector<std::unique_ptr<SparseBitSet>> vsTables;
    {
        SCOPED_TRACE("Unsupported format");
        CmapBuilder builder(2);
        std::vector<uint8_t> table = buildCmapFormat4Table(std::vector<uint16_t>({'b', 'b'}));
        writeU16(0, table.data(), 0 /* format offset */);
        builder.appendTable(3, 1, table);
        builder.appendTable(0, 0, validTable);
        std::vector<uint8_t> cmap = builder.build();

        SparseBitSet coverage = CmapCoverage::getCoverage(cmap.data(), cmap.size(), &vsTables);
        EXPECT_TRUE(coverage.get('a'));   // comes from valid table
        EXPECT_FALSE(coverage.get('b'));  // should not use invalid table.
        EXPECT_TRUE(vsTables.empty());
    }
    {
        SCOPED_TRACE("Invalid language");
        CmapBuilder builder(2);
        std::vector<uint8_t> table = buildCmapFormat4Table(std::vector<uint16_t>({'b', 'b'}));
        writeU16(1, table.data(), 4 /* language offset */);
        builder.appendTable(3, 1, table);
        builder.appendTable(0, 0, validTable);
        std::vector<uint8_t> cmap = builder.build();

        SparseBitSet coverage = CmapCoverage::getCoverage(cmap.data(), cmap.size(), &vsTables);
        EXPECT_TRUE(coverage.get('a'));   // comes from valid table
        EXPECT_FALSE(coverage.get('b'));  // should not use invalid table.
        EXPECT_TRUE(vsTables.empty());
    }
    {
        SCOPED_TRACE("Invalid length");
        CmapBuilder builder(2);
        std::vector<uint8_t> table = buildCmapFormat4Table(std::vector<uint16_t>({'b', 'b'}));
        writeU16(5000, table.data(), 2 /* length offset */);
        builder.appendTable(3, 1, table);
        builder.appendTable(0, 0, validTable);
        std::vector<uint8_t> cmap = builder.build();

        SparseBitSet coverage = CmapCoverage::getCoverage(cmap.data(), cmap.size(), &vsTables);
        EXPECT_TRUE(coverage.get('a'));   // comes from valid table
        EXPECT_FALSE(coverage.get('b'));  // should not use invalid table.
        EXPECT_TRUE(vsTables.empty());
    }
}

TEST(CmapCoverageTest, TableSelection_SkipBrokenFormat12Table) {
    std::vector<std::unique_ptr<SparseBitSet>> vsTables;
    std::vector<uint8_t> validTable = buildCmapFormat12Table(std::vector<uint32_t>({'a', 'a'}));
    {
        SCOPED_TRACE("Unsupported format");
        CmapBuilder builder(2);
        std::vector<uint8_t> table = buildCmapFormat12Table(std::vector<uint32_t>({'b', 'b'}));
        writeU16(0, table.data(), 0 /* format offset */);
        builder.appendTable(3, 1, table);
        builder.appendTable(0, 0, validTable);
        std::vector<uint8_t> cmap = builder.build();

        SparseBitSet coverage = CmapCoverage::getCoverage(cmap.data(), cmap.size(), &vsTables);
        EXPECT_TRUE(coverage.get('a'));   // comes from valid table
        EXPECT_FALSE(coverage.get('b'));  // should not use invalid table.
        EXPECT_TRUE(vsTables.empty());
    }
    {
        SCOPED_TRACE("Invalid language");
        CmapBuilder builder(2);
        std::vector<uint8_t> table = buildCmapFormat12Table(std::vector<uint32_t>({'b', 'b'}));
        writeU32(1, table.data(), 8 /* language offset */);
        builder.appendTable(3, 1, table);
        builder.appendTable(0, 0, validTable);
        std::vector<uint8_t> cmap = builder.build();

        SparseBitSet coverage = CmapCoverage::getCoverage(cmap.data(), cmap.size(), &vsTables);
        EXPECT_TRUE(coverage.get('a'));   // comes from valid table
        EXPECT_FALSE(coverage.get('b'));  // should not use invalid table.
        EXPECT_TRUE(vsTables.empty());
    }
    {
        SCOPED_TRACE("Invalid length");
        CmapBuilder builder(2);
        std::vector<uint8_t> table = buildCmapFormat12Table(std::vector<uint32_t>({'b', 'b'}));
        writeU32(5000, table.data(), 4 /* length offset */);
        builder.appendTable(3, 1, table);
        builder.appendTable(0, 0, validTable);
        std::vector<uint8_t> cmap = builder.build();

        SparseBitSet coverage = CmapCoverage::getCoverage(cmap.data(), cmap.size(), &vsTables);
        EXPECT_TRUE(coverage.get('a'));   // comes from valid table
        EXPECT_FALSE(coverage.get('b'));  // should not use invalid table.
        EXPECT_TRUE(vsTables.empty());
    }
}

TEST(CmapCoverageTest, TableSelection_VSTable) {
    std::vector<uint8_t> smallLetterTable =
            buildCmapFormat12Table(std::vector<uint32_t>({'a', 'z'}));
    std::vector<uint8_t> vsTable = buildCmapFormat14Table(std::vector<VariationSelectorRecord>({
            {0xFE0E, {'a', 'b'}, {} /* no non-default UVS table */},
            {0xFE0F, {} /* no default UVS table */, {'a', 'b'}},
            {0xE0100, {'a', 'a'}, {'b'}},
    }));
    CmapBuilder builder(2);
    builder.appendTable(3, 1, smallLetterTable);
    builder.appendTable(VS_PLATFORM_ID, VS_ENCODING_ID, vsTable);
    std::vector<uint8_t> cmap = builder.build();

    std::vector<std::unique_ptr<SparseBitSet>> vsTables;
    SparseBitSet coverage = CmapCoverage::getCoverage(cmap.data(), cmap.size(), &vsTables);
    EXPECT_TRUE(coverage.get('a'));
    ASSERT_FALSE(vsTables.empty());

    const uint16_t vs15Index = getVsIndex(0xFE0E);
    ASSERT_LT(vs15Index, vsTables.size());
    ASSERT_TRUE(vsTables[vs15Index]);
    EXPECT_TRUE(vsTables[vs15Index]->get('a'));
    EXPECT_TRUE(vsTables[vs15Index]->get('b'));

    const uint16_t vs16Index = getVsIndex(0xFE0F);
    ASSERT_LT(vs16Index, vsTables.size());
    ASSERT_TRUE(vsTables[vs16Index]);
    EXPECT_TRUE(vsTables[vs16Index]->get('a'));
    EXPECT_TRUE(vsTables[vs16Index]->get('b'));

    const uint16_t vs17Index = getVsIndex(0xE0100);
    ASSERT_LT(vs17Index, vsTables.size());
    ASSERT_TRUE(vsTables[vs17Index]);
    EXPECT_TRUE(vsTables[vs17Index]->get('a'));
    EXPECT_TRUE(vsTables[vs17Index]->get('b'));
}

TEST(CmapCoverageTest, TableSelection_InterSection) {
    std::vector<uint8_t> smallLetterTable =
            buildCmapFormat12Table(std::vector<uint32_t>({'a', 'z'}));
    std::vector<uint8_t> vsTable = buildCmapFormat14Table(std::vector<VariationSelectorRecord>({
            {0xFE0E,
             {'a', 'e'},
             {
                     'c', 'd',
             }},
            {0xFE0F, {'c', 'e'}, {'a', 'b', 'c', 'd', 'e'}},
            {0xE0100, {'a', 'c'}, {'b', 'c', 'd'}},
            {0xE0101, {'b', 'd'}, {'a', 'b', 'c', 'd'}},
            {0xE0102, {'a', 'c', 'd', 'g'}, {'b', 'c', 'd', 'e', 'f', 'g', 'h'}},
            {0xE0103,
             {'a', 'f'},
             {
                     'b', 'd',
             }},
    }));
    CmapBuilder builder(2);
    builder.appendTable(3, 1, smallLetterTable);
    builder.appendTable(VS_PLATFORM_ID, VS_ENCODING_ID, vsTable);
    std::vector<uint8_t> cmap = builder.build();

    std::vector<std::unique_ptr<SparseBitSet>> vsTables;
    SparseBitSet coverage = CmapCoverage::getCoverage(cmap.data(), cmap.size(), &vsTables);
    EXPECT_TRUE(coverage.get('a'));
    ASSERT_FALSE(vsTables.empty());

    const uint16_t vs15Index = getVsIndex(0xFE0E);
    ASSERT_LT(vs15Index, vsTables.size());
    ASSERT_TRUE(vsTables[vs15Index]);
    EXPECT_TRUE(vsTables[vs15Index]->get('a'));
    EXPECT_TRUE(vsTables[vs15Index]->get('b'));
    EXPECT_TRUE(vsTables[vs15Index]->get('c'));
    EXPECT_TRUE(vsTables[vs15Index]->get('d'));
    EXPECT_TRUE(vsTables[vs15Index]->get('e'));

    const uint16_t vs16Index = getVsIndex(0xFE0F);
    ASSERT_LT(vs16Index, vsTables.size());
    ASSERT_TRUE(vsTables[vs16Index]);
    EXPECT_TRUE(vsTables[vs16Index]->get('a'));
    EXPECT_TRUE(vsTables[vs16Index]->get('b'));
    EXPECT_TRUE(vsTables[vs16Index]->get('c'));
    EXPECT_TRUE(vsTables[vs16Index]->get('d'));
    EXPECT_TRUE(vsTables[vs16Index]->get('e'));

    const uint16_t vs17Index = getVsIndex(0xE0100);
    ASSERT_LT(vs17Index, vsTables.size());
    ASSERT_TRUE(vsTables[vs17Index]);
    EXPECT_TRUE(vsTables[vs17Index]->get('a'));
    EXPECT_TRUE(vsTables[vs17Index]->get('b'));
    EXPECT_TRUE(vsTables[vs17Index]->get('c'));
    EXPECT_TRUE(vsTables[vs17Index]->get('d'));

    const uint16_t vs18Index = getVsIndex(0xE0101);
    ASSERT_LT(vs18Index, vsTables.size());
    ASSERT_TRUE(vsTables[vs18Index]);
    EXPECT_TRUE(vsTables[vs18Index]->get('a'));
    EXPECT_TRUE(vsTables[vs18Index]->get('b'));
    EXPECT_TRUE(vsTables[vs18Index]->get('c'));
    EXPECT_TRUE(vsTables[vs18Index]->get('d'));

    const uint16_t vs19Index = getVsIndex(0xE0102);
    ASSERT_LT(vs19Index, vsTables.size());
    ASSERT_TRUE(vsTables[vs19Index]);
    EXPECT_TRUE(vsTables[vs19Index]->get('a'));
    EXPECT_TRUE(vsTables[vs19Index]->get('b'));
    EXPECT_TRUE(vsTables[vs19Index]->get('c'));
    EXPECT_TRUE(vsTables[vs19Index]->get('d'));
    EXPECT_TRUE(vsTables[vs19Index]->get('e'));
    EXPECT_TRUE(vsTables[vs19Index]->get('f'));
    EXPECT_TRUE(vsTables[vs19Index]->get('g'));
    EXPECT_TRUE(vsTables[vs19Index]->get('h'));

    const uint16_t vs20Index = getVsIndex(0xE0103);
    ASSERT_LT(vs20Index, vsTables.size());
    ASSERT_TRUE(vsTables[vs20Index]);
    EXPECT_TRUE(vsTables[vs20Index]->get('a'));
    EXPECT_TRUE(vsTables[vs20Index]->get('b'));
    EXPECT_TRUE(vsTables[vs20Index]->get('c'));
    EXPECT_TRUE(vsTables[vs20Index]->get('d'));
    EXPECT_TRUE(vsTables[vs20Index]->get('e'));
    EXPECT_TRUE(vsTables[vs20Index]->get('f'));
}

TEST(CmapCoverageTest, TableSelection_brokenVSTable) {
    std::vector<uint8_t> cmap12Table = buildCmapFormat12Table(std::vector<uint32_t>({'a', 'z'}));
    {
        SCOPED_TRACE("Too small cmap size");
        std::vector<uint8_t> vsTable = buildCmapFormat14Table(
                std::vector<VariationSelectorRecord>({{0xFE0E, {'a', 'a'}, {'b'}}}));
        CmapBuilder builder(2);
        builder.appendTable(3, 1, cmap12Table);
        builder.appendTable(VS_PLATFORM_ID, VS_ENCODING_ID, vsTable);
        std::vector<uint8_t> cmap = builder.build();

        std::vector<std::unique_ptr<SparseBitSet>> vsTables;
        SparseBitSet coverage =
                CmapCoverage::getCoverage(cmap.data(), 3 /* too small size */, &vsTables);
        EXPECT_FALSE(coverage.get('a'));
        ASSERT_TRUE(vsTables.empty());
    }
    {
        SCOPED_TRACE("Too many variation records");
        std::vector<uint8_t> vsTable = buildCmapFormat14Table(
                std::vector<VariationSelectorRecord>({{0xFE0F, {'a', 'a'}, {'b'}}}));
        writeU32(5000, vsTable.data(), 6 /* numVarSelectorRecord offset */);
        CmapBuilder builder(2);
        builder.appendTable(3, 1, cmap12Table);
        builder.appendTable(VS_PLATFORM_ID, VS_ENCODING_ID, vsTable);
        std::vector<uint8_t> cmap = builder.build();

        std::vector<std::unique_ptr<SparseBitSet>> vsTables;
        SparseBitSet coverage = CmapCoverage::getCoverage(cmap.data(), cmap.size(), &vsTables);
        ASSERT_TRUE(vsTables.empty());
    }
    {
        SCOPED_TRACE("Invalid default UVS offset in variation records");
        std::vector<uint8_t> vsTable = buildCmapFormat14Table(
                std::vector<VariationSelectorRecord>({{0xFE0F, {'a', 'a'}, {'b'}}}));
        writeU32(5000, vsTable.data(), 13 /* defaultUVSffset offset in the first record */);
        CmapBuilder builder(2);
        builder.appendTable(3, 1, cmap12Table);
        builder.appendTable(VS_PLATFORM_ID, VS_ENCODING_ID, vsTable);
        std::vector<uint8_t> cmap = builder.build();

        std::vector<std::unique_ptr<SparseBitSet>> vsTables;
        SparseBitSet coverage = CmapCoverage::getCoverage(cmap.data(), cmap.size(), &vsTables);
        ASSERT_TRUE(vsTables.empty());
    }
    {
        SCOPED_TRACE("Invalid non default UVS offset in variation records");
        std::vector<uint8_t> vsTable = buildCmapFormat14Table(
                std::vector<VariationSelectorRecord>({{0xFE0F, {'a', 'a'}, {'b'}}}));
        writeU32(5000, vsTable.data(), 17 /* nonDefaultUVSffset offset in the first record */);
        CmapBuilder builder(2);
        builder.appendTable(3, 1, cmap12Table);
        builder.appendTable(VS_PLATFORM_ID, VS_ENCODING_ID, vsTable);
        std::vector<uint8_t> cmap = builder.build();

        std::vector<std::unique_ptr<SparseBitSet>> vsTables;
        SparseBitSet coverage = CmapCoverage::getCoverage(cmap.data(), cmap.size(), &vsTables);
        ASSERT_TRUE(vsTables.empty());
    }
    {
        SCOPED_TRACE("Too many ranges entry in default UVS table");
        std::vector<uint8_t> vsTable = buildCmapFormat14Table(
                std::vector<VariationSelectorRecord>({{0xFE0F, {'a', 'a'}, {'b'}}}));
        // 21 is the offset of the numUnicodeValueRanges in the fist defulat UVS table.
        writeU32(5000, vsTable.data(), 21);
        CmapBuilder builder(2);
        builder.appendTable(3, 1, cmap12Table);
        builder.appendTable(VS_PLATFORM_ID, VS_ENCODING_ID, vsTable);
        std::vector<uint8_t> cmap = builder.build();

        std::vector<std::unique_ptr<SparseBitSet>> vsTables;
        SparseBitSet coverage = CmapCoverage::getCoverage(cmap.data(), cmap.size(), &vsTables);
        ASSERT_TRUE(vsTables.empty());
    }
    {
        SCOPED_TRACE("Too many ranges entry in non default UVS table");
        std::vector<uint8_t> vsTable = buildCmapFormat14Table(
                std::vector<VariationSelectorRecord>({{0xFE0F, {'a', 'a'}, {'b'}}}));
        // 29 is the offset of the numUnicodeValueRanges in the fist defulat UVS table.
        writeU32(5000, vsTable.data(), 29);
        CmapBuilder builder(2);
        builder.appendTable(3, 1, cmap12Table);
        builder.appendTable(VS_PLATFORM_ID, VS_ENCODING_ID, vsTable);
        std::vector<uint8_t> cmap = builder.build();

        std::vector<std::unique_ptr<SparseBitSet>> vsTables;
        SparseBitSet coverage = CmapCoverage::getCoverage(cmap.data(), cmap.size(), &vsTables);
        ASSERT_TRUE(vsTables.empty());
    }
    {
        SCOPED_TRACE("Reversed range in default UVS table");
        std::vector<uint8_t> vsTable = buildCmapFormat14Table(
                std::vector<VariationSelectorRecord>({{0xFE0F, {'b', 'b', 'a', 'a'}, {}}}));
        CmapBuilder builder(2);
        builder.appendTable(3, 1, cmap12Table);
        builder.appendTable(VS_PLATFORM_ID, VS_ENCODING_ID, vsTable);
        std::vector<uint8_t> cmap = builder.build();

        std::vector<std::unique_ptr<SparseBitSet>> vsTables;
        SparseBitSet coverage = CmapCoverage::getCoverage(cmap.data(), cmap.size(), &vsTables);
        ASSERT_TRUE(vsTables.empty());
    }
    {
        SCOPED_TRACE("Reversed range in default UVS table - partially readable");
        std::vector<uint8_t> vsTable = buildCmapFormat14Table(std::vector<VariationSelectorRecord>(
                {{0xFE0F, {'a', 'a', 'c', 'c', 'b', 'b'}, {}}}));
        CmapBuilder builder(2);
        builder.appendTable(3, 1, cmap12Table);
        builder.appendTable(VS_PLATFORM_ID, VS_ENCODING_ID, vsTable);
        std::vector<uint8_t> cmap = builder.build();

        std::vector<std::unique_ptr<SparseBitSet>> vsTables;
        SparseBitSet coverage = CmapCoverage::getCoverage(cmap.data(), cmap.size(), &vsTables);
        ASSERT_TRUE(vsTables.empty());
    }
    {
        SCOPED_TRACE("Reversed mapping entries in non default UVS table");
        std::vector<uint8_t> vsTable = buildCmapFormat14Table(
                std::vector<VariationSelectorRecord>({{0xFE0F, {}, {'b', 'a'}}}));
        CmapBuilder builder(2);
        builder.appendTable(3, 1, cmap12Table);
        builder.appendTable(VS_PLATFORM_ID, VS_ENCODING_ID, vsTable);
        std::vector<uint8_t> cmap = builder.build();

        std::vector<std::unique_ptr<SparseBitSet>> vsTables;
        SparseBitSet coverage = CmapCoverage::getCoverage(cmap.data(), cmap.size(), &vsTables);
        ASSERT_TRUE(vsTables.empty());
    }
    {
        SCOPED_TRACE("Reversed mapping entries in non default UVS table");
        std::vector<uint8_t> vsTable = buildCmapFormat14Table(
                std::vector<VariationSelectorRecord>({{0xFE0F, {}, {'a', 'c', 'b'}}}));
        CmapBuilder builder(2);
        builder.appendTable(3, 1, cmap12Table);
        builder.appendTable(VS_PLATFORM_ID, VS_ENCODING_ID, vsTable);
        std::vector<uint8_t> cmap = builder.build();

        std::vector<std::unique_ptr<SparseBitSet>> vsTables;
        SparseBitSet coverage = CmapCoverage::getCoverage(cmap.data(), cmap.size(), &vsTables);
        ASSERT_TRUE(vsTables.empty());
    }
    {
        // http://b/70808908
        SCOPED_TRACE("OOB access due to integer overflow in non default UVS table");
        std::vector<uint8_t> vsTable = buildCmapFormat14Table(
                std::vector<VariationSelectorRecord>({{0xFE0F, {'a', 'a'}, {'b'}}}));
        // 6 is the offset of the numRecords in the Cmap format14 subtable header.
        writeU32(0x1745d174 /* 2^32 / kRecordSize(=11) */, vsTable.data(), 6);
        CmapBuilder builder(2);
        builder.appendTable(3, 1, cmap12Table);
        builder.appendTable(VS_PLATFORM_ID, VS_ENCODING_ID, vsTable);
        std::vector<uint8_t> cmap = builder.build();

        std::vector<std::unique_ptr<SparseBitSet>> vsTables;
        SparseBitSet coverage = CmapCoverage::getCoverage(cmap.data(), cmap.size(), &vsTables);
        ASSERT_TRUE(vsTables.empty());
    }
    {
        // http://b/70808908
        SCOPED_TRACE("OOB access due to integer overflow in non default UVS table");
        std::vector<uint8_t> vsTable = buildCmapFormat14Table(
                std::vector<VariationSelectorRecord>({{0xFE0F, {'a', 'a'}, {'b'}}}));
        // 29 is the offset of the numUVSMappings in the fist non defulat UVS table.
        writeU32(0x33333333 /* 2^32 / kUVSMappingRecordSize(=5) */, vsTable.data(), 29);
        CmapBuilder builder(2);
        builder.appendTable(3, 1, cmap12Table);
        builder.appendTable(VS_PLATFORM_ID, VS_ENCODING_ID, vsTable);
        std::vector<uint8_t> cmap = builder.build();

        std::vector<std::unique_ptr<SparseBitSet>> vsTables;
        SparseBitSet coverage = CmapCoverage::getCoverage(cmap.data(), cmap.size(), &vsTables);
        ASSERT_TRUE(vsTables.empty());
    }
    {
        // http://b/70808908
        SCOPED_TRACE("OOB access due to integer overflow in default UVS table");
        std::vector<uint8_t> vsTable = buildCmapFormat14Table(
                std::vector<VariationSelectorRecord>({{0xFE0F, {'a', 'a'}, {'b'}}}));
        // 21 is the offset of the numUnicodeValueRanges in the fist defulat UVS table.
        writeU32(0x40000000 /* 2^32 / kUnicodeRangeRecordSize(=4) */, vsTable.data(), 21);
        CmapBuilder builder(2);
        builder.appendTable(3, 1, cmap12Table);
        builder.appendTable(VS_PLATFORM_ID, VS_ENCODING_ID, vsTable);
        std::vector<uint8_t> cmap = builder.build();

        std::vector<std::unique_ptr<SparseBitSet>> vsTables;
        SparseBitSet coverage = CmapCoverage::getCoverage(cmap.data(), cmap.size(), &vsTables);
        ASSERT_TRUE(vsTables.empty());
    }
    {
        // http://b/70808908
        SCOPED_TRACE("OOB access due to integer overflow in non default UVS table");
        std::vector<uint8_t> vsTable = buildCmapFormat14Table(
                std::vector<VariationSelectorRecord>({{0xFE0F, {'a', 'a'}, {'b'}}}));
        // 6 is the offset of the numRecords in the Cmap format14 subtable header.
        writeU32(0x1745d174 /* 2^32 / kRecordSize(=11) */, vsTable.data(), 6);
        CmapBuilder builder(2);
        builder.appendTable(3, 1, cmap12Table);
        builder.appendTable(VS_PLATFORM_ID, VS_ENCODING_ID, vsTable);
        std::vector<uint8_t> cmap = builder.build();

        std::vector<std::unique_ptr<SparseBitSet>> vsTables;
        SparseBitSet coverage = CmapCoverage::getCoverage(cmap.data(), cmap.size(), &vsTables);
        ASSERT_TRUE(vsTables.empty());
    }
    {
        // http://b/70808908
        SCOPED_TRACE("OOB access due to integer overflow in non default UVS table");
        std::vector<uint8_t> vsTable = buildCmapFormat14Table(
                std::vector<VariationSelectorRecord>({{0xFE0F, {'a', 'a'}, {'b'}}}));
        // 29 is the offset of the numUVSMappings in the fist non defulat UVS table.
        writeU32(0x33333333 /* 2^32 / kUVSMappingRecordSize(=5) */, vsTable.data(), 29);
        CmapBuilder builder(2);
        builder.appendTable(3, 1, cmap12Table);
        builder.appendTable(VS_PLATFORM_ID, VS_ENCODING_ID, vsTable);
        std::vector<uint8_t> cmap = builder.build();

        std::vector<std::unique_ptr<SparseBitSet>> vsTables;
        SparseBitSet coverage = CmapCoverage::getCoverage(cmap.data(), cmap.size(), &vsTables);
        ASSERT_TRUE(vsTables.empty());
    }
    {
        // http://b/70808908
        SCOPED_TRACE("OOB access due to integer overflow in default UVS table");
        std::vector<uint8_t> vsTable = buildCmapFormat14Table(
                std::vector<VariationSelectorRecord>({{0xFE0F, {'a', 'a'}, {'b'}}}));
        // 21 is the offset of the numUnicodeValueRanges in the fist defulat UVS table.
        writeU32(0x40000000 /* 2^32 / kUnicodeRangeRecordSize(=4) */, vsTable.data(), 21);
        CmapBuilder builder(2);
        builder.appendTable(3, 1, cmap12Table);
        builder.appendTable(VS_PLATFORM_ID, VS_ENCODING_ID, vsTable);
        std::vector<uint8_t> cmap = builder.build();

        std::vector<std::unique_ptr<SparseBitSet>> vsTables;
        SparseBitSet coverage = CmapCoverage::getCoverage(cmap.data(), cmap.size(), &vsTables);
        ASSERT_TRUE(vsTables.empty());
    }
}

TEST(CmapCoverageTest, TableSelection_brokenVSTable_bestEffort) {
    std::vector<uint8_t> cmap12Table = buildCmapFormat12Table(std::vector<uint32_t>({'a', 'a'}));
    {
        SCOPED_TRACE("Invalid default UVS offset in variation records");
        std::vector<uint8_t> vsTable = buildCmapFormat14Table(std::vector<VariationSelectorRecord>({
                {0xFE0E, {'a', 'a'}, {'b'}}, {0xFE0F, {'a', 'a'}, {'b'}},
        }));
        writeU32(5000, vsTable.data(), 13 /* defaultUVSffset offset in the record for 0xFE0E */);
        CmapBuilder builder(2);
        builder.appendTable(3, 1, cmap12Table);
        builder.appendTable(VS_PLATFORM_ID, VS_ENCODING_ID, vsTable);
        std::vector<uint8_t> cmap = builder.build();

        std::vector<std::unique_ptr<SparseBitSet>> vsTables;
        SparseBitSet coverage = CmapCoverage::getCoverage(cmap.data(), cmap.size(), &vsTables);

        const uint16_t vs16Index = getVsIndex(0xFE0F);
        ASSERT_LT(vs16Index, vsTables.size());
        ASSERT_TRUE(vsTables[vs16Index]);
        EXPECT_TRUE(vsTables[vs16Index]->get('a'));
        EXPECT_TRUE(vsTables[vs16Index]->get('b'));

        const uint16_t vs15Index = getVsIndex(0xFE0E);
        EXPECT_FALSE(vsTables[vs15Index]);
    }
    {
        SCOPED_TRACE("Invalid non default UVS offset in variation records");
        std::vector<uint8_t> vsTable = buildCmapFormat14Table(std::vector<VariationSelectorRecord>({
                {0xFE0E, {'a', 'a'}, {'b'}}, {0xFE0F, {'a', 'a'}, {'b'}},
        }));
        writeU32(5000, vsTable.data(), 17 /* nonDefaultUVSffset offset in the first record */);
        CmapBuilder builder(2);
        builder.appendTable(3, 1, cmap12Table);
        builder.appendTable(VS_PLATFORM_ID, VS_ENCODING_ID, vsTable);
        std::vector<uint8_t> cmap = builder.build();

        std::vector<std::unique_ptr<SparseBitSet>> vsTables;
        SparseBitSet coverage = CmapCoverage::getCoverage(cmap.data(), cmap.size(), &vsTables);

        const uint16_t vs16Index = getVsIndex(0xFE0F);
        ASSERT_LT(vs16Index, vsTables.size());
        ASSERT_TRUE(vsTables[vs16Index]);
        EXPECT_TRUE(vsTables[vs16Index]->get('a'));
        EXPECT_TRUE(vsTables[vs16Index]->get('b'));

        const uint16_t vs15Index = getVsIndex(0xFE0E);
        EXPECT_FALSE(vsTables[vs15Index]);
    }
    {
        SCOPED_TRACE("Unknown variation selectors.");
        std::vector<uint8_t> vsTable = buildCmapFormat14Table(std::vector<VariationSelectorRecord>({
                {0xFE0F, {'a', 'a'}, {'b'}}, {0xEFFFF, {'a', 'a'}, {'b'}},
        }));
        CmapBuilder builder(2);
        builder.appendTable(3, 1, cmap12Table);
        builder.appendTable(VS_PLATFORM_ID, VS_ENCODING_ID, vsTable);
        std::vector<uint8_t> cmap = builder.build();

        std::vector<std::unique_ptr<SparseBitSet>> vsTables;
        SparseBitSet coverage = CmapCoverage::getCoverage(cmap.data(), cmap.size(), &vsTables);

        const uint16_t vs16Index = getVsIndex(0xFE0F);
        ASSERT_LT(vs16Index, vsTables.size());
        ASSERT_TRUE(vsTables[vs16Index]);
        EXPECT_TRUE(vsTables[vs16Index]->get('a'));
        EXPECT_TRUE(vsTables[vs16Index]->get('b'));
    }
}

// Used only for better looking of range definition.
#define RANGE(x, y) x, y

TEST(CmapCoverageTest, TableSelection_defaultUVSPointMissingGlyph) {
    std::vector<uint8_t> baseTable = buildCmapFormat12Table(std::vector<uint32_t>(
            {RANGE('a', 'e'), RANGE('g', 'h'), RANGE('j', 'j'), RANGE('m', 'z')}));
    std::vector<uint8_t> vsTable = buildCmapFormat14Table(
            std::vector<VariationSelectorRecord>({{0xFE0F, {'a', 'z'}, {}}}));

    CmapBuilder builder(2);
    builder.appendTable(3, 1, baseTable);
    builder.appendTable(VS_PLATFORM_ID, VS_ENCODING_ID, vsTable);
    std::vector<uint8_t> cmap = builder.build();

    std::vector<std::unique_ptr<SparseBitSet>> vsTables;
    SparseBitSet coverage = CmapCoverage::getCoverage(cmap.data(), cmap.size(), &vsTables);
    const uint16_t vsIndex = getVsIndex(0xFE0F);
    ASSERT_LT(vsIndex, vsTables.size());
    ASSERT_TRUE(vsTables[vsIndex]);

    for (char c = 'a'; c <= 'z'; ++c) {
        // Default UVS table points the variation sequence to the glyph of the base code point.
        // Thus, if the base code point is not supported, we should exclude them.
        EXPECT_EQ(coverage.get(c), vsTables[vsIndex]->get(c)) << c;
    }
}

#undef RANGE

TEST(CmapCoverageTest, TableSelection_vsTableOnly) {
    std::vector<uint8_t> vsTable =
            buildCmapFormat14Table(std::vector<VariationSelectorRecord>({{0xFE0F, {}, {'a'}}}));

    CmapBuilder builder(1);
    builder.appendTable(VS_PLATFORM_ID, VS_ENCODING_ID, vsTable);
    std::vector<uint8_t> cmap = builder.build();

    std::vector<std::unique_ptr<SparseBitSet>> vsTables;
    SparseBitSet coverage = CmapCoverage::getCoverage(cmap.data(), cmap.size(), &vsTables);
    const uint16_t vsIndex = getVsIndex(0xFE0F);
    ASSERT_LT(vsIndex, vsTables.size());
    ASSERT_TRUE(vsTables[vsIndex]);
    EXPECT_TRUE(vsTables[vsIndex]->get('a'));
}
}  // namespace minikin
