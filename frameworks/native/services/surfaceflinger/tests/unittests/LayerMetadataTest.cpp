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

#undef LOG_TAG
#define LOG_TAG "LibSurfaceFlingerUnittests"

#include <binder/Parcel.h>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <gui/LayerMetadata.h>
#include <log/log.h>

namespace android {
namespace {

class LayerMetadataTest : public testing::Test {
public:
    LayerMetadataTest();
    ~LayerMetadataTest() override;
};

LayerMetadataTest::LayerMetadataTest() {
    const ::testing::TestInfo* const test_info =
            ::testing::UnitTest::GetInstance()->current_test_info();
    ALOGD("**** Setting up for %s.%s\n", test_info->test_case_name(), test_info->name());
}

LayerMetadataTest::~LayerMetadataTest() {
    const ::testing::TestInfo* const test_info =
            ::testing::UnitTest::GetInstance()->current_test_info();
    ALOGD("**** Tearing down after %s.%s\n", test_info->test_case_name(), test_info->name());
}

TEST_F(LayerMetadataTest, testLayerMetadata) {
    LayerMetadata metadata;

    ASSERT_EQ(0, metadata.mMap.size());

    // Test non-set
    ASSERT_EQ(3, metadata.getInt32(4, 3));

    // Make sure it's still unset
    ASSERT_EQ(5, metadata.getInt32(4, 5));

    metadata.setInt32(4, 2);
    ASSERT_EQ(2, metadata.getInt32(4, 0));

    // data is too small
    metadata.mMap[2] = std::vector<uint8_t>{'a', 'b'};
    ASSERT_EQ(0, metadata.getInt32(2, 0));

    Parcel p;
    metadata.writeToParcel(&p);
    LayerMetadata reconstructed;
    reconstructed.setInt32(3, 1); // to make sure it gets replaced
    p.setDataPosition(0);
    reconstructed.readFromParcel(&p);
    ASSERT_EQ(metadata.mMap, reconstructed.mMap);
}

TEST_F(LayerMetadataTest, merge) {
    LayerMetadata metadata;
    metadata.setInt32(4, 2);
    metadata.mMap[2] = std::vector<uint8_t>{'a', 'b'};

    LayerMetadata second;
    std::vector<uint8_t> someData{'c', 'd', '\0'};
    second.mMap[2] = someData;
    second.setInt32(6, 5);
    second.mMap[4].clear(); // will not delete if eraseEmpty is false
    bool changed = metadata.merge(second);

    ASSERT_TRUE(changed);
    ASSERT_EQ(3, metadata.mMap.size());
    ASSERT_EQ(someData, second.mMap[2]);
    ASSERT_EQ(5, metadata.getInt32(6, 0));
    ASSERT_TRUE(metadata.mMap.at(4).empty());

    LayerMetadata withErase;
    withErase.mMap[6].clear();
    changed = metadata.merge(withErase, true /* eraseEmpty */);
    ASSERT_TRUE(changed);
    ASSERT_EQ(2, metadata.mMap.size());
    ASSERT_EQ(someData, second.mMap[2]);
    ASSERT_EQ(true, metadata.has(4));

    // test for change detection
    LayerMetadata third;
    third.mMap[2] = someData;
    third.mMap[5].clear();
    changed = metadata.merge(third);
    ASSERT_FALSE(changed);
}

} // namespace
} // namespace android
