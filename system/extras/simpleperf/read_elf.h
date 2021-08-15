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

#ifndef SIMPLE_PERF_READ_ELF_H_
#define SIMPLE_PERF_READ_ELF_H_

#include <functional>
#include <ostream>
#include <string>
#include "build_id.h"

// Read ELF functions are called in different situations, so it is hard to
// decide whether to report error or not. So read ELF functions don't report
// error when something wrong happens, instead they return ElfStatus, which
// identifies different errors met while reading elf file.
enum ElfStatus {
  NO_ERROR,
  FILE_NOT_FOUND,
  READ_FAILED,
  FILE_MALFORMED,
  NO_SYMBOL_TABLE,
  NO_BUILD_ID,
  BUILD_ID_MISMATCH,
  SECTION_NOT_FOUND,
};

std::ostream& operator<<(std::ostream& os, const ElfStatus& status);

ElfStatus GetBuildIdFromNoteFile(const std::string& filename, BuildId* build_id);
ElfStatus GetBuildIdFromElfFile(const std::string& filename, BuildId* build_id);
ElfStatus GetBuildIdFromEmbeddedElfFile(const std::string& filename, uint64_t file_offset,
                                        uint32_t file_size, BuildId* build_id);

// The symbol prefix used to indicate that the symbol belongs to android linker.
static const std::string linker_prefix = "__dl_";

struct ElfFileSymbol {
  uint64_t vaddr;
  uint64_t len;
  bool is_func;
  bool is_label;
  bool is_in_text_section;
  std::string name;

  ElfFileSymbol() : vaddr(0), len(0), is_func(false), is_label(false), is_in_text_section(false) {
  }
};

ElfStatus ParseSymbolsFromElfFile(const std::string& filename,
                                  const BuildId& expected_build_id,
                                  const std::function<void(const ElfFileSymbol&)>& callback);
ElfStatus ParseSymbolsFromEmbeddedElfFile(const std::string& filename, uint64_t file_offset,
                                          uint32_t file_size, const BuildId& expected_build_id,
                                          const std::function<void(const ElfFileSymbol&)>& callback);
ElfStatus ParseSymbolsFromElfFileInMemory(const char* data, size_t size,
                                          const std::function<void(const ElfFileSymbol&)>& callback);
ElfStatus ParseDynamicSymbolsFromElfFile(const std::string& filename,
                                         const std::function<void(const ElfFileSymbol&)>& callback);

ElfStatus ReadMinExecutableVirtualAddressFromElfFile(const std::string& filename,
                                                     const BuildId& expected_build_id,
                                                     uint64_t* min_addr,
                                                     uint64_t* file_offset_of_min_vaddr);
ElfStatus ReadMinExecutableVirtualAddressFromEmbeddedElfFile(const std::string& filename,
                                                             uint64_t file_offset,
                                                             uint32_t file_size,
                                                             const BuildId& expected_build_id,
                                                             uint64_t* min_vaddr,
                                                             uint64_t* file_offset_of_min_vaddr);

ElfStatus ReadSectionFromElfFile(const std::string& filename, const std::string& section_name,
                                 std::string* content);

// Expose the following functions for unit tests.
bool IsArmMappingSymbol(const char* name);
ElfStatus IsValidElfFile(int fd);
bool IsValidElfFileMagic(const char* buf, size_t buf_size);
ElfStatus IsValidElfPath(const std::string& filename);
bool GetBuildIdFromNoteSection(const char* section, size_t section_size, BuildId* build_id);

#endif  // SIMPLE_PERF_READ_ELF_H_
