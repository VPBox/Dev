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

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "DisplayHardware/DisplayIdentification.h"

namespace android {
namespace {

const unsigned char kInternalEdid[] =
        "\x00\xff\xff\xff\xff\xff\xff\x00\x4c\xa3\x42\x31\x00\x00\x00\x00"
        "\x00\x15\x01\x03\x80\x1a\x10\x78\x0a\xd3\xe5\x95\x5c\x60\x90\x27"
        "\x19\x50\x54\x00\x00\x00\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01"
        "\x01\x01\x01\x01\x01\x01\x9e\x1b\x00\xa0\x50\x20\x12\x30\x10\x30"
        "\x13\x00\x05\xa3\x10\x00\x00\x19\x00\x00\x00\x0f\x00\x00\x00\x00"
        "\x00\x00\x00\x00\x00\x23\x87\x02\x64\x00\x00\x00\x00\xfe\x00\x53"
        "\x41\x4d\x53\x55\x4e\x47\x0a\x20\x20\x20\x20\x20\x00\x00\x00\xfe"
        "\x00\x31\x32\x31\x41\x54\x31\x31\x2d\x38\x30\x31\x0a\x20\x00\x45";

const unsigned char kExternalEdid[] =
        "\x00\xff\xff\xff\xff\xff\xff\x00\x22\xf0\x6c\x28\x01\x01\x01\x01"
        "\x02\x16\x01\x04\xb5\x40\x28\x78\xe2\x8d\x85\xad\x4f\x35\xb1\x25"
        "\x0e\x50\x54\x00\x00\x00\x01\x01\x01\x01\x01\x01\x01\x01\x01\x01"
        "\x01\x01\x01\x01\x01\x01\xe2\x68\x00\xa0\xa0\x40\x2e\x60\x30\x20"
        "\x36\x00\x81\x90\x21\x00\x00\x1a\xbc\x1b\x00\xa0\x50\x20\x17\x30"
        "\x30\x20\x36\x00\x81\x90\x21\x00\x00\x1a\x00\x00\x00\xfc\x00\x48"
        "\x50\x20\x5a\x52\x33\x30\x77\x0a\x20\x20\x20\x20\x00\x00\x00\xff"
        "\x00\x43\x4e\x34\x32\x30\x32\x31\x33\x37\x51\x0a\x20\x20\x00\x71";

// Extended EDID with timing extension.
const unsigned char kExternalEedid[] =
        "\x00\xff\xff\xff\xff\xff\xff\x00\x4c\x2d\xfe\x08\x00\x00\x00\x00"
        "\x29\x15\x01\x03\x80\x10\x09\x78\x0a\xee\x91\xa3\x54\x4c\x99\x26"
        "\x0f\x50\x54\xbd\xef\x80\x71\x4f\x81\xc0\x81\x00\x81\x80\x95\x00"
        "\xa9\xc0\xb3\x00\x01\x01\x02\x3a\x80\x18\x71\x38\x2d\x40\x58\x2c"
        "\x45\x00\xa0\x5a\x00\x00\x00\x1e\x66\x21\x56\xaa\x51\x00\x1e\x30"
        "\x46\x8f\x33\x00\xa0\x5a\x00\x00\x00\x1e\x00\x00\x00\xfd\x00\x18"
        "\x4b\x0f\x51\x17\x00\x0a\x20\x20\x20\x20\x20\x20\x00\x00\x00\xfc"
        "\x00\x53\x41\x4d\x53\x55\x4e\x47\x0a\x20\x20\x20\x20\x20\x01\x1d"
        "\x02\x03\x1f\xf1\x47\x90\x04\x05\x03\x20\x22\x07\x23\x09\x07\x07"
        "\x83\x01\x00\x00\xe2\x00\x0f\x67\x03\x0c\x00\x20\x00\xb8\x2d\x01"
        "\x1d\x80\x18\x71\x1c\x16\x20\x58\x2c\x25\x00\xa0\x5a\x00\x00\x00"
        "\x9e\x01\x1d\x00\x72\x51\xd0\x1e\x20\x6e\x28\x55\x00\xa0\x5a\x00"
        "\x00\x00\x1e\x8c\x0a\xd0\x8a\x20\xe0\x2d\x10\x10\x3e\x96\x00\xa0"
        "\x5a\x00\x00\x00\x18\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
        "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\xc6";

template <size_t N>
DisplayIdentificationData asDisplayIdentificationData(const unsigned char (&bytes)[N]) {
    return DisplayIdentificationData(bytes, bytes + N - 1);
}

} // namespace

const DisplayIdentificationData& getInternalEdid() {
    static const DisplayIdentificationData data = asDisplayIdentificationData(kInternalEdid);
    return data;
}

const DisplayIdentificationData& getExternalEdid() {
    static const DisplayIdentificationData data = asDisplayIdentificationData(kExternalEdid);
    return data;
}

const DisplayIdentificationData& getExternalEedid() {
    static const DisplayIdentificationData data = asDisplayIdentificationData(kExternalEedid);
    return data;
}

TEST(DisplayIdentificationTest, isEdid) {
    EXPECT_FALSE(isEdid({}));

    EXPECT_TRUE(isEdid(getInternalEdid()));
    EXPECT_TRUE(isEdid(getExternalEdid()));
    EXPECT_TRUE(isEdid(getExternalEedid()));
}

TEST(DisplayIdentificationTest, parseEdid) {
    auto edid = parseEdid(getInternalEdid());
    ASSERT_TRUE(edid);
    EXPECT_EQ(0x4ca3u, edid->manufacturerId);
    EXPECT_STREQ("SEC", edid->pnpId.data());
    // ASCII text should be used as fallback if display name and serial number are missing.
    EXPECT_EQ("121AT11-801", edid->displayName);

    edid = parseEdid(getExternalEdid());
    ASSERT_TRUE(edid);
    EXPECT_EQ(0x22f0u, edid->manufacturerId);
    EXPECT_STREQ("HWP", edid->pnpId.data());
    EXPECT_EQ("HP ZR30w", edid->displayName);

    edid = parseEdid(getExternalEedid());
    ASSERT_TRUE(edid);
    EXPECT_EQ(0x4c2du, edid->manufacturerId);
    EXPECT_STREQ("SAM", edid->pnpId.data());
    EXPECT_EQ("SAMSUNG", edid->displayName);
}

TEST(DisplayIdentificationTest, parseInvalidEdid) {
    EXPECT_FALSE(isEdid({}));
    EXPECT_FALSE(parseEdid({}));

    // Display name must be printable.
    auto data = getExternalEdid();
    data[97] = '\x1b';
    auto edid = parseEdid(data);
    ASSERT_TRUE(edid);
    // Serial number should be used as fallback if display name is invalid.
    EXPECT_EQ("CN4202137Q", edid->displayName);

    // Parsing should succeed even if EDID is truncated.
    data.pop_back();
    edid = parseEdid(data);
    ASSERT_TRUE(edid);
    EXPECT_EQ("CN4202137Q", edid->displayName);
}

TEST(DisplayIdentificationTest, getPnpId) {
    EXPECT_FALSE(getPnpId(0));
    EXPECT_FALSE(getPnpId(static_cast<uint16_t>(-1)));

    EXPECT_STREQ("SEC", getPnpId(0x4ca3u).value_or(PnpId{}).data());
    EXPECT_STREQ("HWP", getPnpId(0x22f0u).value_or(PnpId{}).data());
    EXPECT_STREQ("SAM", getPnpId(0x4c2du).value_or(PnpId{}).data());
}

TEST(DisplayIdentificationTest, parseDisplayIdentificationData) {
    const auto primaryInfo = parseDisplayIdentificationData(0, getInternalEdid());
    ASSERT_TRUE(primaryInfo);

    const auto secondaryInfo = parseDisplayIdentificationData(1, getExternalEdid());
    ASSERT_TRUE(secondaryInfo);

    const auto tertiaryInfo = parseDisplayIdentificationData(2, getExternalEedid());
    ASSERT_TRUE(tertiaryInfo);

    // Display IDs should be unique.
    EXPECT_NE(primaryInfo->id, secondaryInfo->id);
    EXPECT_NE(primaryInfo->id, tertiaryInfo->id);
    EXPECT_NE(secondaryInfo->id, tertiaryInfo->id);
}

TEST(DisplayIdentificationTest, getFallbackDisplayId) {
    // Manufacturer ID should be invalid.
    ASSERT_FALSE(getPnpId(getFallbackDisplayId(0)));
    ASSERT_FALSE(getPnpId(getFallbackDisplayId(0xffu)));
}

TEST(DisplayIdentificationTest, getVirtualDisplayId) {
    // Manufacturer ID should be invalid.
    ASSERT_FALSE(getPnpId(getVirtualDisplayId(0)));
    ASSERT_FALSE(getPnpId(getVirtualDisplayId(0xffff'ffffu)));
}

} // namespace android
