/*
 * Copyright (C) 2014 The Android Open Source Project
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

#define LOG_NDEBUG 0
#define LOG_TAG "VendorTagDescriptorTests"

#include <binder/Parcel.h>
#include <camera/VendorTagDescriptor.h>
#include <camera_metadata_tests_fake_vendor.h>
#include <camera_metadata_hidden.h>
#include <system/camera_vendor_tags.h>
#include <utils/Errors.h>
#include <utils/Log.h>
#include <utils/RefBase.h>

#include <gtest/gtest.h>
#include <stdint.h>

using namespace android;

enum {
    BAD_TAG_ARRAY = 0xDEADBEEFu,
    BAD_TAG = 0x8DEADBADu,
};

#define ARRAY_SIZE(a)      (sizeof(a) / sizeof((a)[0]))

static bool ContainsTag(uint32_t* tagArray, size_t size, uint32_t tag) {
    for (size_t i = 0; i < size; ++i) {
        if (tag == tagArray[i]) return true;
    }
    return false;
}

#define EXPECT_CONTAINS_TAG(t, a) \
    EXPECT_TRUE(ContainsTag(a, ARRAY_SIZE(a), t))

#define ASSERT_NOT_NULL(x) \
    ASSERT_TRUE((x) != NULL)

extern "C" {

static int zero_get_tag_count(const vendor_tag_ops_t*) {
    return 0;
}

static int default_get_tag_count(const vendor_tag_ops_t*) {
    return VENDOR_TAG_COUNT_ERR;
}

static void default_get_all_tags(const vendor_tag_ops_t*, uint32_t*) {
    //Noop
}

static const char* default_get_section_name(const vendor_tag_ops_t*, uint32_t) {
    return VENDOR_SECTION_NAME_ERR;
}

static const char* default_get_tag_name(const vendor_tag_ops_t*, uint32_t) {
    return VENDOR_TAG_NAME_ERR;
}

static int default_get_tag_type(const vendor_tag_ops_t*, uint32_t) {
    return VENDOR_TAG_TYPE_ERR;
}

} /*extern "C"*/

// Set default vendor operations for a vendor_tag_ops struct
static void FillWithDefaults(vendor_tag_ops_t* vOps) {
    ASSERT_NOT_NULL(vOps);
    vOps->get_tag_count = default_get_tag_count;
    vOps->get_all_tags = default_get_all_tags;
    vOps->get_section_name = default_get_section_name;
    vOps->get_tag_name = default_get_tag_name;
    vOps->get_tag_type = default_get_tag_type;
}

/**
 * Test if values from VendorTagDescriptor methods match corresponding values
 * from vendor_tag_ops functions.
 */
TEST(VendorTagDescriptorTest, ConsistentWithVendorTags) {
    sp<VendorTagDescriptor> vDesc;
    const vendor_tag_ops_t *vOps = &fakevendor_ops;
    EXPECT_EQ(OK, VendorTagDescriptor::createDescriptorFromOps(vOps, /*out*/vDesc));

    ASSERT_NOT_NULL(vDesc);

    // Ensure reasonable tag count
    int tagCount = vDesc->getTagCount();
    EXPECT_EQ(tagCount, vOps->get_tag_count(vOps));

    uint32_t descTagArray[tagCount];
    uint32_t opsTagArray[tagCount];

    // Get all tag ids
    vDesc->getTagArray(descTagArray);
    vOps->get_all_tags(vOps, opsTagArray);

    ASSERT_NOT_NULL(descTagArray);
    ASSERT_NOT_NULL(opsTagArray);

    uint32_t tag;
    for (int i = 0; i < tagCount; ++i) {
        // For each tag id, check whether type, section name, tag name match
        tag = descTagArray[i];
        EXPECT_CONTAINS_TAG(tag, opsTagArray);
        EXPECT_EQ(vDesc->getTagType(tag), vOps->get_tag_type(vOps, tag));
        EXPECT_STREQ(vDesc->getSectionName(tag), vOps->get_section_name(vOps, tag));
        EXPECT_STREQ(vDesc->getTagName(tag), vOps->get_tag_name(vOps, tag));
    }
}

/**
 * Test if values from VendorTagDescriptor methods stay consistent after being
 * parcelled/unparcelled.
 */
TEST(VendorTagDescriptorTest, ConsistentAcrossParcel) {
    sp<VendorTagDescriptor> vDescOriginal, vDescParceled;
    const vendor_tag_ops_t *vOps = &fakevendor_ops;
    EXPECT_EQ(OK, VendorTagDescriptor::createDescriptorFromOps(vOps, /*out*/vDescOriginal));

    ASSERT_TRUE(vDescOriginal != NULL);

    Parcel p;

    // Check whether parcel read/write succeed
    EXPECT_EQ(OK, vDescOriginal->writeToParcel(&p));
    p.setDataPosition(0);

    vDescParceled = new VendorTagDescriptor();
    ASSERT_EQ(OK, vDescParceled->readFromParcel(&p));

    // Ensure consistent tag count
    int tagCount = vDescOriginal->getTagCount();
    ASSERT_EQ(tagCount, vDescParceled->getTagCount());

    uint32_t descTagArray[tagCount];
    uint32_t desc2TagArray[tagCount];

    // Get all tag ids
    vDescOriginal->getTagArray(descTagArray);
    vDescParceled->getTagArray(desc2TagArray);

    ASSERT_NOT_NULL(descTagArray);
    ASSERT_NOT_NULL(desc2TagArray);

    uint32_t tag;
    for (int i = 0; i < tagCount; ++i) {
        // For each tag id, check consistency between the two vendor tag
        // descriptors for each type, section name, tag name
        tag = descTagArray[i];
        EXPECT_CONTAINS_TAG(tag, desc2TagArray);
        EXPECT_EQ(vDescOriginal->getTagType(tag), vDescParceled->getTagType(tag));
        EXPECT_STREQ(vDescOriginal->getSectionName(tag), vDescParceled->getSectionName(tag));
        EXPECT_STREQ(vDescOriginal->getTagName(tag), vDescParceled->getTagName(tag));
    }
}

/**
 * Test defaults and error conditions.
 */
TEST(VendorTagDescriptorTest, ErrorConditions) {
    sp<VendorTagDescriptor> vDesc;
    vendor_tag_ops_t vOps;
    FillWithDefaults(&vOps);

    // Make empty tag count
    vOps.get_tag_count = zero_get_tag_count;

    // Ensure create fails when using null vOps
    EXPECT_EQ(BAD_VALUE, VendorTagDescriptor::createDescriptorFromOps(/*vOps*/NULL, vDesc));

    // Ensure creat succeeds for empty vendor tag ops
    ASSERT_EQ(OK, VendorTagDescriptor::createDescriptorFromOps(&vOps, vDesc));

    // Ensure defaults are returned when no vtags are defined, or tag is unknown
    EXPECT_EQ(VENDOR_TAG_COUNT_ERR, vDesc->getTagCount());
    uint32_t* tagArray = reinterpret_cast<uint32_t*>(BAD_TAG_ARRAY);
    uint32_t* testArray = tagArray;
    vDesc->getTagArray(tagArray);
    EXPECT_EQ(testArray, tagArray);
    EXPECT_EQ(VENDOR_SECTION_NAME_ERR, vDesc->getSectionName(BAD_TAG));
    EXPECT_EQ(VENDOR_TAG_NAME_ERR, vDesc->getTagName(BAD_TAG));
    EXPECT_EQ(VENDOR_TAG_TYPE_ERR, vDesc->getTagType(BAD_TAG));

    // Make sure global can be set/cleared
    sp<VendorTagDescriptor> prevGlobal = VendorTagDescriptor::getGlobalVendorTagDescriptor();
    VendorTagDescriptor::clearGlobalVendorTagDescriptor();

    EXPECT_TRUE(VendorTagDescriptor::getGlobalVendorTagDescriptor() == NULL);
    EXPECT_EQ(OK, VendorTagDescriptor::setAsGlobalVendorTagDescriptor(vDesc));
    EXPECT_TRUE(VendorTagDescriptor::getGlobalVendorTagDescriptor() != NULL);
    EXPECT_EQ(VENDOR_SECTION_NAME_ERR, vDesc->getSectionName(BAD_TAG));
    EXPECT_EQ(OK, VendorTagDescriptor::setAsGlobalVendorTagDescriptor(prevGlobal));
    EXPECT_EQ(prevGlobal, VendorTagDescriptor::getGlobalVendorTagDescriptor());
}
