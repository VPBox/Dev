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

#include <gtest/gtest.h>
#include <ui/BufferHubMetadata.h>

namespace android {
namespace dvr {

constexpr size_t kEmptyUserMetadataSize = 0;

class BufferHubMetadataTest : public ::testing::Test {};

TEST_F(BufferHubMetadataTest, Create_UserMetdataSizeTooBig) {
    BufferHubMetadata m1 = BufferHubMetadata::create(std::numeric_limits<uint32_t>::max());
    EXPECT_FALSE(m1.isValid());
}

TEST_F(BufferHubMetadataTest, Create_Success) {
    BufferHubMetadata m1 = BufferHubMetadata::create(kEmptyUserMetadataSize);
    EXPECT_TRUE(m1.isValid());
    EXPECT_NE(m1.metadataHeader(), nullptr);
}

TEST_F(BufferHubMetadataTest, Import_Success) {
    BufferHubMetadata m1 = BufferHubMetadata::create(kEmptyUserMetadataSize);
    EXPECT_TRUE(m1.isValid());
    EXPECT_NE(m1.metadataHeader(), nullptr);

    unique_fd h2 = unique_fd(dup(m1.ashmemFd().get()));
    EXPECT_NE(h2.get(), -1);

    BufferHubMetadata m2 = BufferHubMetadata::import(std::move(h2));
    EXPECT_EQ(h2.get(), -1);
    EXPECT_TRUE(m1.isValid());
    BufferHubDefs::MetadataHeader* mh1 = m1.metadataHeader();
    EXPECT_NE(mh1, nullptr);

    // Check if the newly allocated buffer is initialized in released state (i.e.
    // state equals to 0U).
    EXPECT_TRUE(mh1->bufferState.load() == 0U);

    EXPECT_TRUE(m2.isValid());
    BufferHubDefs::MetadataHeader* mh2 = m2.metadataHeader();
    EXPECT_NE(mh2, nullptr);

    // Check if the newly allocated buffer is initialized in released state (i.e.
    // state equals to 0U).
    EXPECT_TRUE(mh2->bufferState.load() == 0U);
}

TEST_F(BufferHubMetadataTest, MoveMetadataInvalidatesOldOne) {
    BufferHubMetadata m1 = BufferHubMetadata::create(sizeof(int));
    EXPECT_TRUE(m1.isValid());
    EXPECT_NE(m1.metadataHeader(), nullptr);
    EXPECT_NE(m1.ashmemFd().get(), -1);
    EXPECT_EQ(m1.userMetadataSize(), sizeof(int));

    BufferHubMetadata m2 = std::move(m1);

    // After the move, the metadata header (a raw pointer) should be reset in the older buffer.
    EXPECT_EQ(m1.metadataHeader(), nullptr);
    EXPECT_NE(m2.metadataHeader(), nullptr);

    EXPECT_EQ(m1.ashmemFd().get(), -1);
    EXPECT_NE(m2.ashmemFd().get(), -1);

    EXPECT_EQ(m1.userMetadataSize(), 0U);
    EXPECT_EQ(m2.userMetadataSize(), sizeof(int));

    BufferHubMetadata m3{std::move(m2)};

    // After the move, the metadata header (a raw pointer) should be reset in the older buffer.
    EXPECT_EQ(m2.metadataHeader(), nullptr);
    EXPECT_NE(m3.metadataHeader(), nullptr);

    EXPECT_EQ(m2.ashmemFd().get(), -1);
    EXPECT_NE(m3.ashmemFd().get(), -1);

    EXPECT_EQ(m2.userMetadataSize(), 0U);
    EXPECT_EQ(m3.userMetadataSize(), sizeof(int));
}

}  // namespace dvr
}  // namespace android
