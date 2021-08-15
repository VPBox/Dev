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

#include <binder/request_id.h>

#include <gtest/gtest.h>

using namespace iorap::binder;  // NOLINT

using android::Parcel;

TEST(AutoParcelable, RequestIdRoundTrip) {
  RequestId typical;
  typical.request_id = 0xC0FFEE;

  static constexpr auto NO_ERROR = android::NO_ERROR;
  static constexpr int COMPARED_SAME = 0;

  {
    Parcel p1, p2;

    p1.writeInt64(0x1234);
    p2.writeInt64(0x1234);

    EXPECT_EQ(COMPARED_SAME, p1.compareData(p2));
  }

  {
    Parcel parcel_typical;

    EXPECT_EQ(NO_ERROR, typical.writeToParcel(&parcel_typical));

    Parcel parcel_manual;

    parcel_manual.writeInt64(typical.request_id);

    EXPECT_EQ(COMPARED_SAME, parcel_manual.compareData(parcel_typical));

    parcel_typical.setDataPosition(0);
    EXPECT_EQ(typical.request_id, parcel_typical.readInt64());
    // Test that the 'writeToParcel' works correctly.
  }

  {
    // Test that the 'readFromParcel' works correctly.
    RequestId copy;
    copy.request_id = 0;

    Parcel parcel_typical;
    parcel_typical.writeInt64(typical.request_id);
    parcel_typical.setDataPosition(0);

    EXPECT_EQ(NO_ERROR, copy.readFromParcel(&parcel_typical));
    EXPECT_EQ(typical.request_id, copy.request_id);
  }

  {
    // Test that write and then read from parcel works correctly.
    RequestId copy;
    copy.request_id = 0;

    Parcel parcel{};

    EXPECT_EQ(NO_ERROR, typical.writeToParcel(&parcel));
    parcel.setDataPosition(0);  // rewind after write before read.
    EXPECT_EQ(NO_ERROR, copy.readFromParcel(&parcel));

    // for each field, they should be equal.
    EXPECT_EQ(typical.request_id, copy.request_id);
  }
}
