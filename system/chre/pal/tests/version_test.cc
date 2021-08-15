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

#include "gtest/gtest.h"
#include "chre/pal/version.h"

TEST(PalVersionTest, CreateApiVersion) {
  constexpr uint32_t version = CHRE_PAL_CREATE_API_VERSION(3, 6);
  EXPECT_EQ(version, 0x03060000);
  EXPECT_EQ(CHRE_PAL_GET_API_MAJOR_VERSION(version), 3);
}

TEST(PalVersionTest, CompatibilityCheck) {
  constexpr uint32_t pal_version = CHRE_PAL_CREATE_API_VERSION(2, 8);

  uint32_t requested_version = CHRE_PAL_CREATE_API_VERSION(3, 6);
  EXPECT_FALSE(CHRE_PAL_VERSIONS_ARE_COMPATIBLE(pal_version,
                                                requested_version));

  requested_version = CHRE_PAL_CREATE_API_VERSION(1, 5);
  EXPECT_FALSE(CHRE_PAL_VERSIONS_ARE_COMPATIBLE(pal_version,
                                                requested_version));

  requested_version = CHRE_PAL_CREATE_API_VERSION(2, 7);
  EXPECT_TRUE(CHRE_PAL_VERSIONS_ARE_COMPATIBLE(pal_version,
                                               requested_version));

  requested_version = CHRE_PAL_CREATE_API_VERSION(2, 4);
  EXPECT_TRUE(CHRE_PAL_VERSIONS_ARE_COMPATIBLE(pal_version,
                                               requested_version));

  requested_version = CHRE_PAL_CREATE_API_VERSION(2, 9);
  EXPECT_TRUE(CHRE_PAL_VERSIONS_ARE_COMPATIBLE(pal_version,
                                               requested_version));
}
