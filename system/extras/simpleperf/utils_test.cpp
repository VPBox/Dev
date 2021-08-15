/*
 * Copyright (C) 2016 The Android Open Source Project
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

#include "get_test_data.h"
#include "utils.h"

static bool ModulesMatch(const char* p, const char* q) {
  if (p == nullptr && q == nullptr) {
    return true;
  }
  if (p != nullptr && q != nullptr) {
    return strcmp(p, q) == 0;
  }
  return false;
}

static bool KernelSymbolsMatch(const KernelSymbol& sym1,
                               const KernelSymbol& sym2) {
  return sym1.addr == sym2.addr && sym1.type == sym2.type &&
         strcmp(sym1.name, sym2.name) == 0 &&
         ModulesMatch(sym1.module, sym2.module);
}

TEST(utils, ProcessKernelSymbols) {
  std::string data =
      "ffffffffa005c4e4 d __warned.41698   [libsas]\n"
      "aaaaaaaaaaaaaaaa T _text\n"
      "cccccccccccccccc c ccccc\n";
  KernelSymbol expected_symbol;
  expected_symbol.addr = 0xffffffffa005c4e4ULL;
  expected_symbol.type = 'd';
  expected_symbol.name = "__warned.41698";
  expected_symbol.module = "libsas";
  ASSERT_TRUE(ProcessKernelSymbols(
      data,
      std::bind(&KernelSymbolsMatch, std::placeholders::_1, expected_symbol)));

  expected_symbol.addr = 0xaaaaaaaaaaaaaaaaULL;
  expected_symbol.type = 'T';
  expected_symbol.name = "_text";
  expected_symbol.module = nullptr;
  ASSERT_TRUE(ProcessKernelSymbols(
      data,
      std::bind(&KernelSymbolsMatch, std::placeholders::_1, expected_symbol)));

  expected_symbol.name = "non_existent_symbol";
  ASSERT_FALSE(ProcessKernelSymbols(
      data,
      std::bind(&KernelSymbolsMatch, std::placeholders::_1, expected_symbol)));
}

TEST(utils, ConvertBytesToValue) {
  char buf[8];
  for (int i = 0; i < 8; ++i) {
    buf[i] = i;
  }
  ASSERT_EQ(0x1ULL, ConvertBytesToValue(buf + 1, 1));
  ASSERT_EQ(0x201ULL, ConvertBytesToValue(buf + 1, 2));
  ASSERT_EQ(0x05040302ULL, ConvertBytesToValue(buf + 2, 4));
  ASSERT_EQ(0x0706050403020100ULL, ConvertBytesToValue(buf, 8));
}

TEST(utils, ArchiveHelper) {
  std::unique_ptr<ArchiveHelper> ahelper = ArchiveHelper::CreateInstance(GetTestData(APK_FILE));
  ASSERT_TRUE(ahelper);
  bool found = false;
  ZipEntry lib_entry;
  ASSERT_TRUE(ahelper->IterateEntries([&](ZipEntry& entry, const std::string& name) {
    if (name == NATIVELIB_IN_APK) {
      found = true;
      lib_entry = entry;
      return false;
    }
    return true;
  }));
  ASSERT_TRUE(found);
  ZipEntry entry;
  ASSERT_TRUE(ahelper->FindEntry(NATIVELIB_IN_APK, &entry));
  ASSERT_EQ(entry.offset, lib_entry.offset);
  std::vector<uint8_t> data;
  ASSERT_TRUE(ahelper->GetEntryData(entry, &data));

  // Check reading wrong file formats.
  ASSERT_FALSE(ArchiveHelper::CreateInstance(GetTestData(ELF_FILE)));
  ASSERT_FALSE(ArchiveHelper::CreateInstance("/dev/zero"));
}
