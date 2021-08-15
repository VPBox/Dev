//
// Copyright (C) 2019 The Android Open Source Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#include <android-base/unique_fd.h>
#include <android/hardware/weaver/1.0/IWeaver.h>
#include <ext4_utils/ext4_utils.h>
#include <fstab/fstab.h>
#include <gtest/gtest.h>

using namespace android::fs_mgr;

using android::base::unique_fd;
using android::hardware::weaver::V1_0::IWeaver;
using android::hardware::weaver::V1_0::WeaverConfig;
using android::hardware::weaver::V1_0::WeaverStatus;

TEST(MetadataPartition, FirstStageMount) {
    Fstab fstab;
    if (ReadFstabFromDt(&fstab)) {
        auto entry = GetEntryForMountPoint(&fstab, "/metadata");
        ASSERT_NE(entry, nullptr);
    } else {
        ASSERT_TRUE(ReadDefaultFstab(&fstab));
        auto entry = GetEntryForMountPoint(&fstab, "/metadata");
        ASSERT_NE(entry, nullptr);
        EXPECT_TRUE(entry->fs_mgr_flags.first_stage_mount);
    }
}

TEST(MetadataPartition, MinimumSize) {
    Fstab fstab;
    ASSERT_TRUE(ReadDefaultFstab(&fstab));

    auto entry = GetEntryForMountPoint(&fstab, "/metadata");
    ASSERT_NE(entry, nullptr);

    unique_fd fd(open(entry->blk_device.c_str(), O_RDONLY | O_CLOEXEC));
    ASSERT_GE(fd, 0);

    uint64_t size = get_block_device_size(fd);
    EXPECT_GE(size, 16777216);
}

TEST(Weaver, MinimumSlots) {
    auto weaver = IWeaver::getService();
    if (!weaver) {
        return;
    }

    WeaverStatus hw_status;
    WeaverConfig hw_config;

    auto res = weaver->getConfig([&](WeaverStatus status, const WeaverConfig& config) {
            hw_status = status;
            hw_config = config;
    });
    ASSERT_TRUE(res.isOk());
    ASSERT_EQ(hw_status, WeaverStatus::OK);
    EXPECT_GE(hw_config.slots, 16);
}
