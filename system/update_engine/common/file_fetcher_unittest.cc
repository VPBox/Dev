//
// Copyright (C) 2016 The Android Open Source Project
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

#include "update_engine/common/file_fetcher.h"

#include <string>

#include <gtest/gtest.h>

#include "update_engine/common/test_utils.h"

namespace chromeos_update_engine {

class FileFetcherUnitTest : public ::testing::Test {};

TEST_F(FileFetcherUnitTest, SupporterUrlsTest) {
  EXPECT_TRUE(FileFetcher::SupportedUrl("file:///path/to/somewhere.bin"));
  EXPECT_TRUE(FileFetcher::SupportedUrl("FILE:///I/LIKE/TO/SHOUT"));

  EXPECT_FALSE(FileFetcher::SupportedUrl("file://relative"));
  EXPECT_FALSE(FileFetcher::SupportedUrl("http:///no_http_here"));
}

}  // namespace chromeos_update_engine
