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

#include "read_dex_file.h"

#include <gtest/gtest.h>

#include <algorithm>

#include "get_test_data.h"
#include "test_util.h"
#include "utils.h"

TEST(read_dex_file, smoke) {
  std::vector<DexFileSymbol> symbols;
  ASSERT_TRUE(ReadSymbolsFromDexFile(GetTestData("base.vdex"), {0x28}, &symbols));
  ASSERT_EQ(12435u, symbols.size());
  DexFileSymbol target;
  target.offset = 0x6c77e;
  target.len = 0x16;
  target.name = art_api::dex::DexString(
      "com.example.simpleperf.simpleperfexamplewithnative.MixActivity$1.run");
  ASSERT_NE(std::find(symbols.begin(), symbols.end(), target), symbols.end());
}
