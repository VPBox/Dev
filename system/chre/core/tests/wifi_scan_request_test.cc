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

#include "gtest/gtest.h"

#include "chre/core/wifi_scan_request.h"

using chre::WifiScanRequest;
using chre::WifiScanType;

TEST(WifiScanRequest, DefaultWithMinimalPriority) {
  WifiScanRequest request;
  EXPECT_EQ(request.getScanType(), WifiScanType::Invalid);
  EXPECT_EQ(request.getFrequencies().size(), 0);
  EXPECT_EQ(request.getSsids().size(), 0);
}
