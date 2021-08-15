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

#include "FontUtils.h"

#include <gtest/gtest.h>

namespace minikin {
namespace {

constexpr uint32_t MakeTag(char c1, char c2, char c3, char c4) {
    return ((uint32_t)c1 << 24) | ((uint32_t)c2 << 16) | ((uint32_t)c3 << 8) | (uint32_t)c4;
}

static size_t writeU16(uint16_t x, uint8_t* out, size_t offset) {
    out[offset] = x >> 8;
    out[offset + 1] = x;
    return offset + 2;
}

static size_t writeU32(uint32_t x, uint8_t* out, size_t offset) {
    out[offset] = x >> 24;
    out[offset + 1] = x >> 16;
    out[offset + 2] = x >> 8;
    out[offset + 3] = x;
    return offset + 4;
}

static uint32_t floatToFixed(float x) {
    return (uint32_t)(x * 65536);
}

struct Fvar {
    Fvar(uint32_t tag, float minValue, float defaultValue, float maxValue)
            : tag(tag), minValue(minValue), defaultValue(defaultValue), maxValue(maxValue) {}

    uint32_t tag;
    float minValue;
    float defaultValue;
    float maxValue;
};

// Returns valid fvar table contents. No InstanceRecord are filled.
static std::vector<uint8_t> buildFvarTable(const std::vector<Fvar>& fvars) {
    const uint32_t HEADER_SIZE = 0x10;
    const uint32_t AXIS_RECORD_SIZE = 0x14;
    std::vector<uint8_t> out(HEADER_SIZE + fvars.size() * AXIS_RECORD_SIZE);
    size_t head = writeU16(1, out.data(), 0);             // major version
    head = writeU16(0, out.data(), head);                 // minor version
    head = writeU16(HEADER_SIZE, out.data(), head);       // axes array offset
    head = writeU16(2, out.data(), head);                 // reserved
    head = writeU16(fvars.size(), out.data(), head);      // count of axes
    head = writeU16(AXIS_RECORD_SIZE, out.data(), head);  // size of variaiton axis record
    head = writeU16(0, out.data(), head);                 // number of instance record count
    head = writeU16(0, out.data(), head);                 // instance record size

    for (const Fvar& fvar : fvars) {
        head = writeU32(fvar.tag, out.data(), head);
        head = writeU32(floatToFixed(fvar.minValue), out.data(), head);
        head = writeU32(floatToFixed(fvar.defaultValue), out.data(), head);
        head = writeU32(floatToFixed(fvar.maxValue), out.data(), head);
        head = writeU16(0, out.data(), head);  // flags
        head = writeU16(0, out.data(), head);  // axis name ID
    }

    return out;
}

TEST(FontUtilsTest, analyzeAxes_tagCount) {
    std::vector<uint8_t> fvarTable = buildFvarTable({
            Fvar(MakeTag('w', 'd', 't', 'h'), 0.0f, 1.0f, 2.0f),
            Fvar(MakeTag('w', 'g', 'h', 't'), 0.0f, 1.0f, 2.0f),
    });

    std::unordered_set<uint32_t> axes;
    ASSERT_TRUE(analyzeAxes(fvarTable.data(), fvarTable.size(), &axes));
    ASSERT_EQ(2u, axes.size());
    EXPECT_EQ(1u, axes.count(MakeTag('w', 'd', 't', 'h')));
    EXPECT_EQ(1u, axes.count(MakeTag('w', 'g', 'h', 't')));
    EXPECT_EQ(0u, axes.count(MakeTag('s', 'l', 'n', 't')));
}

TEST(FontUtilsTest, analyzeAxes_emptyBuffer) {
    std::vector<uint8_t> fvarTable;
    std::unordered_set<uint32_t> axes;
    ASSERT_FALSE(analyzeAxes(fvarTable.data(), fvarTable.size(), &axes));
}

TEST(FontUtilsTest, analyzeAxes_invalidTableSize) {
    std::vector<uint8_t> fvarTable = buildFvarTable({
            Fvar(MakeTag('w', 'd', 't', 'h'), 0.0f, 1.0f, 2.0f),
            Fvar(MakeTag('w', 'g', 'h', 't'), 0.0f, 1.0f, 2.0f),
    });

    fvarTable.resize(1000);
    writeU16(50, fvarTable.data(), 8);  // Set axisCount = 50

    std::unordered_set<uint32_t> axes;
    ASSERT_FALSE(analyzeAxes(fvarTable.data(), fvarTable.size(), &axes));
}

}  // namespace
}  // namespace minikin
