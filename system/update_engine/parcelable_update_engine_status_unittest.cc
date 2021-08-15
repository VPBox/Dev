//
// Copyright (C) 2017 The Android Open Source Project
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

#include "update_engine/parcelable_update_engine_status.h"
#include "update_engine/update_status_utils.h"

#include <binder/Parcel.h>
#include <gtest/gtest.h>

using android::Parcel;
using android::status_t;
using android::String16;
using android::brillo::ParcelableUpdateEngineStatus;
using update_engine::UpdateEngineStatus;
using update_engine::UpdateStatus;

TEST(ParcelableUpdateEngineStatusTest, TestCreationFromUpdateEngineStatus) {
  // This test creates an object and verifies that all the UpdateEngineStatus
  // values are properly reflected in the Parcelable version of the class.

  UpdateEngineStatus ue_status = {123456789,
                                  UpdateStatus::DOWNLOADING,
                                  "0.1.2.3",
                                  "1.2.3.4",
                                  0.5f,
                                  34567,
                                  "2.3.4.5",
                                  "3.4.5.6"};
  ParcelableUpdateEngineStatus parcelable_status(ue_status);
  EXPECT_EQ(ue_status.last_checked_time, parcelable_status.last_checked_time_);
  EXPECT_EQ(
      String16{chromeos_update_engine::UpdateStatusToString(ue_status.status)},
      parcelable_status.current_operation_);
  EXPECT_EQ(String16{ue_status.current_version.c_str()},
            parcelable_status.current_version_);
  EXPECT_EQ(String16{ue_status.current_system_version.c_str()},
            parcelable_status.current_system_version_);
  EXPECT_EQ(ue_status.progress, parcelable_status.progress_);
  EXPECT_EQ(static_cast<int64_t>(ue_status.new_size_bytes),
            parcelable_status.new_size_);
  EXPECT_EQ(String16{ue_status.new_version.c_str()},
            parcelable_status.new_version_);
  EXPECT_EQ(String16{ue_status.new_system_version.c_str()},
            parcelable_status.new_system_version_);
}

TEST(ParcelableUpdateEngineStatusTest, TestParceling) {
  // This tests the writeToParcel and readFromParcel methods for being correctly
  // matched.
  UpdateEngineStatus ue_status = {123456789,
                                  UpdateStatus::DOWNLOADING,
                                  "0.1.2.3",
                                  "1.2.3.4",
                                  0.5f,
                                  34567,
                                  "2.3.4.5",
                                  "3.4.5.6"};
  ParcelableUpdateEngineStatus source_status(ue_status);
  Parcel parcel_source, parcel_target;
  status_t status = source_status.writeToParcel(&parcel_source);
  EXPECT_EQ(::android::OK, status);
  size_t parcel_len = parcel_source.dataSize();
  status = parcel_target.setData(parcel_source.data(), parcel_len);
  EXPECT_EQ(::android::OK, status);
  ParcelableUpdateEngineStatus target_status;
  status = target_status.readFromParcel(&parcel_target);
  EXPECT_EQ(::android::OK, status);

  EXPECT_EQ(source_status.last_checked_time_, target_status.last_checked_time_);
  EXPECT_EQ(source_status.current_operation_, target_status.current_operation_);
  EXPECT_EQ(source_status.current_version_, target_status.current_version_);
  EXPECT_EQ(source_status.current_system_version_,
            target_status.current_system_version_);
  EXPECT_EQ(source_status.progress_, target_status.progress_);
  EXPECT_EQ(source_status.new_size_, target_status.new_size_);
  EXPECT_EQ(source_status.new_version_, target_status.new_version_);
  EXPECT_EQ(source_status.new_system_version_,
            target_status.new_system_version_);
}
