/*
 * Copyright (C) 2015 The Android Open Source Project
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

#include "read_elf.h"

#include <gtest/gtest.h>

#include <map>

#include <android-base/file.h>

#include "get_test_data.h"
#include "test_util.h"
#include "utils.h"

#define ELF_NOTE_GNU "GNU"
#define NT_GNU_BUILD_ID 3

TEST(read_elf, GetBuildIdFromNoteSection) {
  BuildId build_id;
  std::vector<char> data;
  // Fail to read build id for no data.
  ASSERT_FALSE(GetBuildIdFromNoteSection(data.data(), 0, &build_id));

  // Read build id from data starting from different alignment addresses.
  char build_id_data[20];
  for (int i = 0; i < 20; ++i) {
    build_id_data[i] = i;
  }
  BuildId expected_build_id(build_id_data, 20);
  data.resize(100, '\0');

  for (size_t alignment = 0; alignment <= 3; ++alignment) {
    char* start = data.data() + alignment;
    char* p = start;
    uint32_t type = NT_GNU_BUILD_ID;
    uint32_t namesz = 4;
    uint32_t descsz = 20;
    MoveToBinaryFormat(namesz, p);
    MoveToBinaryFormat(descsz, p);
    MoveToBinaryFormat(type, p);
    MoveToBinaryFormat(ELF_NOTE_GNU, 4, p);
    MoveToBinaryFormat(build_id_data, 20, p);
    ASSERT_TRUE(GetBuildIdFromNoteSection(start, p - start, &build_id));
    ASSERT_TRUE(build_id == expected_build_id);
  }
}

TEST(read_elf, GetBuildIdFromElfFile) {
  BuildId build_id;
  ASSERT_EQ(ElfStatus::NO_ERROR, GetBuildIdFromElfFile(GetTestData(ELF_FILE), &build_id));
  ASSERT_EQ(build_id, BuildId(elf_file_build_id));
}

TEST(read_elf, GetBuildIdFromEmbeddedElfFile) {
  BuildId build_id;
  ASSERT_EQ(ElfStatus::NO_ERROR, GetBuildIdFromEmbeddedElfFile(GetTestData(APK_FILE), NATIVELIB_OFFSET_IN_APK,
                                            NATIVELIB_SIZE_IN_APK, &build_id));
  ASSERT_EQ(build_id, native_lib_build_id);
}

void ParseSymbol(const ElfFileSymbol& symbol, std::map<std::string, ElfFileSymbol>* symbols) {
  (*symbols)[symbol.name] = symbol;
}

static void CheckGlobalVariableSymbols(const std::map<std::string, ElfFileSymbol>& symbols) {
  auto pos = symbols.find("GlobalVar");
  ASSERT_NE(pos, symbols.end());
  ASSERT_FALSE(pos->second.is_func);
}

static void CheckFunctionSymbols(const std::map<std::string, ElfFileSymbol>& symbols) {
  auto pos = symbols.find("GlobalFunc");
  ASSERT_NE(pos, symbols.end());
  ASSERT_TRUE(pos->second.is_func);
  ASSERT_TRUE(pos->second.is_in_text_section);
}

void CheckElfFileSymbols(const std::map<std::string, ElfFileSymbol>& symbols) {
  CheckGlobalVariableSymbols(symbols);
  CheckFunctionSymbols(symbols);
}

TEST(read_elf, parse_symbols_from_elf_file_with_correct_build_id) {
  std::map<std::string, ElfFileSymbol> symbols;
  ASSERT_EQ(ElfStatus::NO_ERROR, ParseSymbolsFromElfFile(GetTestData(ELF_FILE), elf_file_build_id,
                                      std::bind(ParseSymbol, std::placeholders::_1, &symbols)));
  CheckElfFileSymbols(symbols);
}

TEST(read_elf, parse_symbols_from_elf_file_without_build_id) {
  std::map<std::string, ElfFileSymbol> symbols;
  ASSERT_EQ(ElfStatus::NO_ERROR, ParseSymbolsFromElfFile(GetTestData(ELF_FILE), BuildId(),
                                      std::bind(ParseSymbol, std::placeholders::_1, &symbols)));
  CheckElfFileSymbols(symbols);
}

TEST(read_elf, parse_symbols_from_elf_file_with_wrong_build_id) {
  BuildId build_id("01010101010101010101");
  std::map<std::string, ElfFileSymbol> symbols;
  ASSERT_EQ(ElfStatus::BUILD_ID_MISMATCH, ParseSymbolsFromElfFile(GetTestData(ELF_FILE), build_id,
                                       std::bind(ParseSymbol, std::placeholders::_1, &symbols)));
}

TEST(read_elf, ParseSymbolsFromEmbeddedElfFile) {
  std::map<std::string, ElfFileSymbol> symbols;
  ASSERT_EQ(ElfStatus::NO_SYMBOL_TABLE, ParseSymbolsFromEmbeddedElfFile(GetTestData(APK_FILE), NATIVELIB_OFFSET_IN_APK,
                                              NATIVELIB_SIZE_IN_APK, native_lib_build_id,
                                              std::bind(ParseSymbol, std::placeholders::_1, &symbols)));
  CheckElfFileSymbols(symbols);
}

TEST(read_elf, ParseSymbolFromMiniDebugInfoElfFile) {
  std::map<std::string, ElfFileSymbol> symbols;
  ASSERT_EQ(ElfStatus::NO_ERROR, ParseSymbolsFromElfFile(GetTestData(ELF_FILE_WITH_MINI_DEBUG_INFO), BuildId(),
                                      std::bind(ParseSymbol, std::placeholders::_1, &symbols)));
  CheckFunctionSymbols(symbols);
}

TEST(read_elf, arm_mapping_symbol) {
  ASSERT_TRUE(IsArmMappingSymbol("$a"));
  ASSERT_FALSE(IsArmMappingSymbol("$b"));
  ASSERT_TRUE(IsArmMappingSymbol("$a.anything"));
  ASSERT_FALSE(IsArmMappingSymbol("$a_no_dot"));
}

TEST(read_elf, IsValidElfPath) {
  ASSERT_NE(ElfStatus::NO_ERROR, IsValidElfPath("/dev/zero"));
  TemporaryFile tmp_file;
  ASSERT_EQ(ElfStatus::READ_FAILED, IsValidElfPath(tmp_file.path));
  ASSERT_TRUE(android::base::WriteStringToFile("wrong format for elf", tmp_file.path));
  ASSERT_EQ(ElfStatus::FILE_MALFORMED, IsValidElfPath(tmp_file.path));
  ASSERT_EQ(ElfStatus::NO_ERROR, IsValidElfPath(GetTestData(ELF_FILE)));
}

TEST(read_elf, check_symbol_for_plt_section) {
  std::map<std::string, ElfFileSymbol> symbols;
  ASSERT_EQ(ElfStatus::NO_ERROR, ParseSymbolsFromElfFile(GetTestData(ELF_FILE), BuildId(),
                                      std::bind(ParseSymbol, std::placeholders::_1, &symbols)));
  ASSERT_NE(symbols.find("@plt"), symbols.end());
}

TEST(read_elf, read_elf_with_broken_section_table) {
  std::string elf_path = GetTestData("libsgmainso-6.4.36.so");
  std::map<std::string, ElfFileSymbol> symbols;
  ASSERT_EQ(ElfStatus::NO_SYMBOL_TABLE,
            ParseSymbolsFromElfFile(elf_path, BuildId(),
                                    std::bind(ParseSymbol, std::placeholders::_1, &symbols)));
  BuildId build_id;
  ASSERT_EQ(ElfStatus::NO_BUILD_ID, GetBuildIdFromElfFile(elf_path, &build_id));
  uint64_t min_vaddr;
  uint64_t file_offset_of_min_vaddr;
  ASSERT_EQ(ElfStatus::NO_ERROR, ReadMinExecutableVirtualAddressFromElfFile(
      elf_path, BuildId(), &min_vaddr, &file_offset_of_min_vaddr));
  ASSERT_EQ(min_vaddr, 0u);
  ASSERT_EQ(file_offset_of_min_vaddr, 0u);
}

TEST(read_elf, ReadMinExecutableVirtualAddressFromElfFile) {
  uint64_t min_vaddr;
  uint64_t file_offset_of_min_vaddr;
  ASSERT_EQ(ElfStatus::NO_ERROR, ReadMinExecutableVirtualAddressFromElfFile(
      GetTestData("libc.so"), BuildId(), &min_vaddr, &file_offset_of_min_vaddr));
  ASSERT_EQ(min_vaddr, 0x29000u);
  ASSERT_EQ(file_offset_of_min_vaddr, 0x29000u);
}
