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
#include "read_apk.h"

#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <algorithm>
#include <limits>

#include <android-base/file.h>
#include <android-base/logging.h>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunused-parameter"

#include <llvm/ADT/StringRef.h>
#include <llvm/Object/Binary.h>
#include <llvm/Object/ELFObjectFile.h>
#include <llvm/Object/ObjectFile.h>

#pragma clang diagnostic pop

#include "utils.h"

#define ELF_NOTE_GNU "GNU"
#define NT_GNU_BUILD_ID 3

std::ostream& operator<<(std::ostream& os, const ElfStatus& status) {
  switch (status) {
    case ElfStatus::NO_ERROR:
      os << "No error";
      break;
    case ElfStatus::FILE_NOT_FOUND:
      os << "File not found";
      break;
    case ElfStatus::READ_FAILED:
      os << "Read failed";
      break;
    case ElfStatus::FILE_MALFORMED:
      os << "Malformed file";
      break;
    case ElfStatus::NO_SYMBOL_TABLE:
      os << "No symbol table";
      break;
    case ElfStatus::NO_BUILD_ID:
      os << "No build id";
      break;
    case ElfStatus::BUILD_ID_MISMATCH:
      os << "Build id mismatch";
      break;
    case ElfStatus::SECTION_NOT_FOUND:
      os << "Section not found";
      break;
  }
  return os;
}

bool IsValidElfFileMagic(const char* buf, size_t buf_size) {
  static const char elf_magic[] = {0x7f, 'E', 'L', 'F'};
  return (buf_size >= 4u && memcmp(buf, elf_magic, 4) == 0);
}

ElfStatus IsValidElfFile(int fd) {
  char buf[4];
  if (!android::base::ReadFully(fd, buf, 4)) {
    return ElfStatus::READ_FAILED;
  }
  return IsValidElfFileMagic(buf, 4) ? ElfStatus::NO_ERROR : ElfStatus::FILE_MALFORMED;
}

ElfStatus IsValidElfPath(const std::string& filename) {
  if (!IsRegularFile(filename)) {
    return ElfStatus::FILE_NOT_FOUND;
  }
  std::string mode = std::string("rb") + CLOSE_ON_EXEC_MODE;
  FILE* fp = fopen(filename.c_str(), mode.c_str());
  if (fp == nullptr) {
    return ElfStatus::READ_FAILED;
  }
  ElfStatus result = IsValidElfFile(fileno(fp));
  fclose(fp);
  return result;
}

bool GetBuildIdFromNoteSection(const char* section, size_t section_size, BuildId* build_id) {
  const char* p = section;
  const char* end = p + section_size;
  while (p < end) {
    if (p + 12 >= end) {
      return false;
    }
    uint32_t namesz;
    uint32_t descsz;
    uint32_t type;
    MoveFromBinaryFormat(namesz, p);
    MoveFromBinaryFormat(descsz, p);
    MoveFromBinaryFormat(type, p);
    namesz = Align(namesz, 4);
    descsz = Align(descsz, 4);
    if ((type == NT_GNU_BUILD_ID) && (p < end) && (strcmp(p, ELF_NOTE_GNU) == 0)) {
      const char* desc_start = p + namesz;
      const char* desc_end = desc_start + descsz;
      if (desc_start > p && desc_start < desc_end && desc_end <= end) {
        *build_id = BuildId(p + namesz, descsz);
        return true;
      } else {
        return false;
      }
    }
    p += namesz + descsz;
  }
  return false;
}

ElfStatus GetBuildIdFromNoteFile(const std::string& filename, BuildId* build_id) {
  std::string content;
  if (!android::base::ReadFileToString(filename, &content)) {
    return ElfStatus::READ_FAILED;
  }
  if (!GetBuildIdFromNoteSection(content.c_str(), content.size(), build_id)) {
    return ElfStatus::NO_BUILD_ID;
  }
  return ElfStatus::NO_ERROR;
}

template <class ELFT>
ElfStatus GetBuildIdFromELFFile(const llvm::object::ELFObjectFile<ELFT>* elf, BuildId* build_id) {
  llvm::StringRef data = elf->getData();
  const char* binary_start = data.data();
  const char* binary_end = data.data() + data.size();
  for (auto it = elf->section_begin(); it != elf->section_end(); ++it) {
    const llvm::object::ELFSectionRef& section_ref = *it;
    if (section_ref.getType() == llvm::ELF::SHT_NOTE) {
      if (it->getContents(data)) {
        return ElfStatus::READ_FAILED;
      }
      if (data.data() < binary_start || data.data() + data.size() > binary_end) {
        return ElfStatus::NO_BUILD_ID;
      }
      if (GetBuildIdFromNoteSection(data.data(), data.size(), build_id)) {
        return ElfStatus::NO_ERROR;
      }
    }
  }
  return ElfStatus::NO_BUILD_ID;
}

static ElfStatus GetBuildIdFromObjectFile(llvm::object::ObjectFile* obj, BuildId* build_id) {
  if (auto elf = llvm::dyn_cast<llvm::object::ELF32LEObjectFile>(obj)) {
    return GetBuildIdFromELFFile(elf, build_id);
  } else if (auto elf = llvm::dyn_cast<llvm::object::ELF64LEObjectFile>(obj)) {
    return GetBuildIdFromELFFile(elf, build_id);
  }
  return ElfStatus::FILE_MALFORMED;
}

struct BinaryWrapper {
  llvm::object::OwningBinary<llvm::object::Binary> binary;
  llvm::object::ObjectFile* obj;

  BinaryWrapper() : obj(nullptr) {
  }
};

static ElfStatus OpenObjectFile(const std::string& filename, uint64_t file_offset,
                                uint64_t file_size, BinaryWrapper* wrapper) {
  android::base::unique_fd fd = FileHelper::OpenReadOnly(filename);
  if (fd == -1) {
    return ElfStatus::READ_FAILED;
  }
  if (file_size == 0) {
    file_size = GetFileSize(filename);
    if (file_size == 0) {
      return ElfStatus::READ_FAILED;
    }
  }
  auto buffer_or_err = llvm::MemoryBuffer::getOpenFileSlice(fd, filename, file_size, file_offset);
  if (!buffer_or_err) {
    return ElfStatus::READ_FAILED;
  }
  auto binary_or_err = llvm::object::createBinary(buffer_or_err.get()->getMemBufferRef());
  if (!binary_or_err) {
    return ElfStatus::READ_FAILED;
  }
  wrapper->binary = llvm::object::OwningBinary<llvm::object::Binary>(std::move(binary_or_err.get()),
                                                                        std::move(buffer_or_err.get()));
  wrapper->obj = llvm::dyn_cast<llvm::object::ObjectFile>(wrapper->binary.getBinary());
  if (wrapper->obj == nullptr) {
    return ElfStatus::FILE_MALFORMED;
  }
  return ElfStatus::NO_ERROR;
}

static ElfStatus OpenObjectFileInMemory(const char* data, size_t size, BinaryWrapper* wrapper) {
  auto buffer = llvm::MemoryBuffer::getMemBuffer(llvm::StringRef(data, size));
  auto binary_or_err = llvm::object::createBinary(buffer->getMemBufferRef());
  if (!binary_or_err) {
    return ElfStatus::FILE_MALFORMED;
  }
  wrapper->binary = llvm::object::OwningBinary<llvm::object::Binary>(std::move(binary_or_err.get()),
                                                                std::move(buffer));
  wrapper->obj = llvm::dyn_cast<llvm::object::ObjectFile>(wrapper->binary.getBinary());
  if (wrapper->obj == nullptr) {
    return ElfStatus::FILE_MALFORMED;
  }
  return ElfStatus::NO_ERROR;
}

ElfStatus GetBuildIdFromElfFile(const std::string& filename, BuildId* build_id) {
  ElfStatus result = IsValidElfPath(filename);
  if (result != ElfStatus::NO_ERROR) {
    return result;
  }
  return GetBuildIdFromEmbeddedElfFile(filename, 0, 0, build_id);
}

ElfStatus GetBuildIdFromEmbeddedElfFile(const std::string& filename, uint64_t file_offset,
                                        uint32_t file_size, BuildId* build_id) {
  BinaryWrapper wrapper;
  ElfStatus result = OpenObjectFile(filename, file_offset, file_size, &wrapper);
  if (result != ElfStatus::NO_ERROR) {
    return result;
  }
  return GetBuildIdFromObjectFile(wrapper.obj, build_id);
}

template <class ELFT>
ElfStatus ReadSectionFromELFFile(const llvm::object::ELFObjectFile<ELFT>* elf, const std::string& section_name,
                                 std::string* content) {
  for (llvm::object::section_iterator it = elf->section_begin(); it != elf->section_end(); ++it) {
    llvm::StringRef name;
    if (it->getName(name) || name != section_name) {
      continue;
    }
    llvm::StringRef data;
    std::error_code err = it->getContents(data);
    if (err) {
      return ElfStatus::READ_FAILED;
    }
    *content = data;
    return ElfStatus::NO_ERROR;
  }
  return ElfStatus::SECTION_NOT_FOUND;
}

bool IsArmMappingSymbol(const char* name) {
  // Mapping symbols in arm, which are described in "ELF for ARM Architecture" and
  // "ELF for ARM 64-bit Architecture". The regular expression to match mapping symbol
  // is ^\$(a|d|t|x)(\..*)?$
  return name[0] == '$' && strchr("adtx", name[1]) != nullptr && (name[2] == '\0' || name[2] == '.');
}

void ReadSymbolTable(llvm::object::symbol_iterator sym_begin,
                     llvm::object::symbol_iterator sym_end,
                     const std::function<void(const ElfFileSymbol&)>& callback,
                     bool is_arm,
                     const llvm::object::section_iterator& section_end) {
  for (; sym_begin != sym_end; ++sym_begin) {
    ElfFileSymbol symbol;
    auto symbol_ref = static_cast<const llvm::object::ELFSymbolRef*>(&*sym_begin);
    llvm::Expected<llvm::object::section_iterator> section_it_or_err = symbol_ref->getSection();
    if (!section_it_or_err) {
      continue;
    }
    // Symbols in .dynsym section don't have associated section.
    if (section_it_or_err.get() != section_end) {
      llvm::StringRef section_name;
      if (section_it_or_err.get()->getName(section_name) || section_name.empty()) {
        continue;
      }
      if (section_name == ".text") {
        symbol.is_in_text_section = true;
      }
    }

    llvm::Expected<llvm::StringRef> symbol_name_or_err = symbol_ref->getName();
    if (!symbol_name_or_err || symbol_name_or_err.get().empty()) {
      continue;
    }

    symbol.name = symbol_name_or_err.get();
    symbol.vaddr = symbol_ref->getValue();
    if ((symbol.vaddr & 1) != 0 && is_arm) {
      // Arm sets bit 0 to mark it as thumb code, remove the flag.
      symbol.vaddr &= ~1;
    }
    symbol.len = symbol_ref->getSize();
    llvm::object::SymbolRef::Type symbol_type = *symbol_ref->getType();
    if (symbol_type == llvm::object::SymbolRef::ST_Function) {
      symbol.is_func = true;
    } else if (symbol_type == llvm::object::SymbolRef::ST_Unknown) {
      if (symbol.is_in_text_section) {
        symbol.is_label = true;
        if (is_arm) {
          // Remove mapping symbols in arm.
          const char* p = (symbol.name.compare(0, linker_prefix.size(), linker_prefix) == 0)
                              ? symbol.name.c_str() + linker_prefix.size()
                              : symbol.name.c_str();
          if (IsArmMappingSymbol(p)) {
            symbol.is_label = false;
          }
        }
      }
    }

    callback(symbol);
  }
}

template <class ELFT>
void AddSymbolForPltSection(const llvm::object::ELFObjectFile<ELFT>* elf,
                            const std::function<void(const ElfFileSymbol&)>& callback) {
  // We may sample instructions in .plt section if the program
  // calls functions from shared libraries. Different architectures use
  // different formats to store .plt section, so it needs a lot of work to match
  // instructions in .plt section to symbols. As samples in .plt section rarely
  // happen, and .plt section can hardly be a performance bottleneck, we can
  // just use a symbol @plt to represent instructions in .plt section.
  for (auto it = elf->section_begin(); it != elf->section_end(); ++it) {
    const llvm::object::ELFSectionRef& section_ref = *it;
    llvm::StringRef section_name;
    std::error_code err = section_ref.getName(section_name);
    if (err || section_name != ".plt") {
      continue;
    }
    const auto* shdr = elf->getSection(section_ref.getRawDataRefImpl());
    if (shdr == nullptr) {
      return;
    }
    ElfFileSymbol symbol;
    symbol.vaddr = shdr->sh_addr;
    symbol.len = shdr->sh_size;
    symbol.is_func = true;
    symbol.is_label = true;
    symbol.is_in_text_section = true;
    symbol.name = "@plt";
    callback(symbol);
    return;
  }
}

template <class ELFT>
void CheckSymbolSections(const llvm::object::ELFObjectFile<ELFT>* elf,
                         bool* has_symtab, bool* has_dynsym) {
  *has_symtab = false;
  *has_dynsym = false;
  for (auto it = elf->section_begin(); it != elf->section_end(); ++it) {
    const llvm::object::ELFSectionRef& section_ref = *it;
    llvm::StringRef section_name;
    std::error_code err = section_ref.getName(section_name);
    if (err) {
      continue;
    }
    if (section_name == ".dynsym") {
      *has_dynsym = true;
    } else if (section_name == ".symtab") {
      *has_symtab = true;
    }
  }
}

template <class ELFT>
ElfStatus ParseSymbolsFromELFFile(const llvm::object::ELFObjectFile<ELFT>* elf,
                                  const std::function<void(const ElfFileSymbol&)>& callback) {
  auto machine = elf->getELFFile()->getHeader()->e_machine;
  bool is_arm = (machine == llvm::ELF::EM_ARM || machine == llvm::ELF::EM_AARCH64);
  AddSymbolForPltSection(elf, callback);
  // Some applications deliberately ship elf files with broken section tables.
  // So check the existence of .symtab section and .dynsym section before reading symbols.
  bool has_symtab;
  bool has_dynsym;
  CheckSymbolSections(elf, &has_symtab, &has_dynsym);
  if (has_symtab && elf->symbol_begin() != elf->symbol_end()) {
    ReadSymbolTable(elf->symbol_begin(), elf->symbol_end(), callback, is_arm, elf->section_end());
    return ElfStatus::NO_ERROR;
  } else if (has_dynsym &&
      elf->dynamic_symbol_begin()->getRawDataRefImpl() != llvm::object::DataRefImpl()) {
    ReadSymbolTable(elf->dynamic_symbol_begin(), elf->dynamic_symbol_end(), callback, is_arm,
                    elf->section_end());
  }
  std::string debugdata;
  ElfStatus result = ReadSectionFromELFFile(elf, ".gnu_debugdata", &debugdata);
  if (result == ElfStatus::SECTION_NOT_FOUND) {
    return ElfStatus::NO_SYMBOL_TABLE;
  } else if (result == ElfStatus::NO_ERROR) {
    std::string decompressed_data;
    if (XzDecompress(debugdata, &decompressed_data)) {
      BinaryWrapper wrapper;
      result = OpenObjectFileInMemory(decompressed_data.data(), decompressed_data.size(),
                                      &wrapper);
      if (result == ElfStatus::NO_ERROR) {
        if (auto elf = llvm::dyn_cast<llvm::object::ELF32LEObjectFile>(wrapper.obj)) {
          return ParseSymbolsFromELFFile(elf, callback);
        } else if (auto elf = llvm::dyn_cast<llvm::object::ELF64LEObjectFile>(wrapper.obj)) {
          return ParseSymbolsFromELFFile(elf, callback);
        } else {
          return ElfStatus::FILE_MALFORMED;
        }
      }
    }
  }
  return result;
}

ElfStatus MatchBuildId(llvm::object::ObjectFile* obj, const BuildId& expected_build_id) {
  if (expected_build_id.IsEmpty()) {
    return ElfStatus::NO_ERROR;
  }
  BuildId real_build_id;
  ElfStatus result = GetBuildIdFromObjectFile(obj, &real_build_id);
  if (result != ElfStatus::NO_ERROR) {
    return result;
  }
  if (expected_build_id != real_build_id) {
    return ElfStatus::BUILD_ID_MISMATCH;
  }
  return ElfStatus::NO_ERROR;
}

ElfStatus ParseSymbolsFromElfFile(const std::string& filename,
                                  const BuildId& expected_build_id,
                                  const std::function<void(const ElfFileSymbol&)>& callback) {
  ElfStatus result = IsValidElfPath(filename);
  if (result != ElfStatus::NO_ERROR) {
    return result;
  }
  return ParseSymbolsFromEmbeddedElfFile(filename, 0, 0, expected_build_id, callback);
}

ElfStatus ParseSymbolsFromEmbeddedElfFile(const std::string& filename, uint64_t file_offset,
                                     uint32_t file_size, const BuildId& expected_build_id,
                                     const std::function<void(const ElfFileSymbol&)>& callback) {
  BinaryWrapper wrapper;
  ElfStatus result = OpenObjectFile(filename, file_offset, file_size, &wrapper);
  if (result != ElfStatus::NO_ERROR) {
    return result;
  }
  result = MatchBuildId(wrapper.obj, expected_build_id);
  if (result != ElfStatus::NO_ERROR) {
    return result;
  }
  if (auto elf = llvm::dyn_cast<llvm::object::ELF32LEObjectFile>(wrapper.obj)) {
    return ParseSymbolsFromELFFile(elf, callback);
  } else if (auto elf = llvm::dyn_cast<llvm::object::ELF64LEObjectFile>(wrapper.obj)) {
    return ParseSymbolsFromELFFile(elf, callback);
  }
  return ElfStatus::FILE_MALFORMED;
}

ElfStatus ParseSymbolsFromElfFileInMemory(const char* data, size_t size,
                                          const std::function<void(const ElfFileSymbol&)>& callback) {
  BinaryWrapper wrapper;
  ElfStatus result = OpenObjectFileInMemory(data, size, &wrapper);
  if (result != ElfStatus::NO_ERROR) {
    return result;
  }
  if (auto elf = llvm::dyn_cast<llvm::object::ELF32LEObjectFile>(wrapper.obj)) {
    return ParseSymbolsFromELFFile(elf, callback);
  } else if (auto elf = llvm::dyn_cast<llvm::object::ELF64LEObjectFile>(wrapper.obj)) {
    return ParseSymbolsFromELFFile(elf, callback);
  }
  return ElfStatus::FILE_MALFORMED;
}

template <class ELFT>
ElfStatus ParseDynamicSymbolsFromELFFile(const llvm::object::ELFObjectFile<ELFT>* elf,
                                         const std::function<void(const ElfFileSymbol&)>& callback) {
  auto machine = elf->getELFFile()->getHeader()->e_machine;
  bool is_arm = (machine == llvm::ELF::EM_ARM || machine == llvm::ELF::EM_AARCH64);
  ReadSymbolTable(elf->dynamic_symbol_begin(), elf->dynamic_symbol_end(), callback, is_arm,
                  elf->section_end());
  return ElfStatus::NO_ERROR;
}

ElfStatus ParseDynamicSymbolsFromElfFile(const std::string& filename,
                                         const std::function<void(const ElfFileSymbol&)>& callback) {
  BinaryWrapper wrapper;
  ElfStatus result = OpenObjectFile(filename, 0, 0, &wrapper);
  if (result != ElfStatus::NO_ERROR) {
    return result;
  }
  if (auto elf = llvm::dyn_cast<llvm::object::ELF32LEObjectFile>(wrapper.obj)) {
    return ParseDynamicSymbolsFromELFFile(elf, callback);
  } else if (auto elf = llvm::dyn_cast<llvm::object::ELF64LEObjectFile>(wrapper.obj)) {
    return ParseDynamicSymbolsFromELFFile(elf, callback);
  }
  return ElfStatus::FILE_MALFORMED;
}

template <class ELFT>
ElfStatus ReadMinExecutableVirtualAddress(const llvm::object::ELFFile<ELFT>* elf,
                                          uint64_t* p_vaddr,
                                          uint64_t* file_offset) {
  bool has_vaddr = false;
  uint64_t min_addr = std::numeric_limits<uint64_t>::max();
  for (auto it = elf->program_header_begin(); it != elf->program_header_end(); ++it) {
    if ((it->p_type == llvm::ELF::PT_LOAD) && (it->p_flags & llvm::ELF::PF_X)) {
      if (it->p_vaddr < min_addr) {
        min_addr = it->p_vaddr;
        *file_offset = it->p_offset;
        has_vaddr = true;
      }
    }
  }
  if (!has_vaddr) {
    // JIT symfiles don't have program headers.
    min_addr = 0;
    *file_offset = 0;
  }
  *p_vaddr = min_addr;
  return ElfStatus::NO_ERROR;
}

ElfStatus ReadMinExecutableVirtualAddressFromElfFile(const std::string& filename,
                                                     const BuildId& expected_build_id,
                                                     uint64_t* min_vaddr,
                                                     uint64_t* file_offset_of_min_vaddr) {
  ElfStatus result = IsValidElfPath(filename);
  if (result != ElfStatus::NO_ERROR) {
    return result;
  }
  return ReadMinExecutableVirtualAddressFromEmbeddedElfFile(filename, 0, 0, expected_build_id,
                                                            min_vaddr, file_offset_of_min_vaddr);
}

ElfStatus ReadMinExecutableVirtualAddressFromEmbeddedElfFile(const std::string& filename,
                                                             uint64_t file_offset,
                                                             uint32_t file_size,
                                                             const BuildId& expected_build_id,
                                                             uint64_t* min_vaddr,
                                                             uint64_t* file_offset_of_min_vaddr) {
  BinaryWrapper wrapper;
  ElfStatus result = OpenObjectFile(filename, file_offset, file_size, &wrapper);
  if (result != ElfStatus::NO_ERROR) {
    return result;
  }
  result = MatchBuildId(wrapper.obj, expected_build_id);
  if (result != ElfStatus::NO_ERROR) {
    return result;
  }
  if (auto elf = llvm::dyn_cast<llvm::object::ELF32LEObjectFile>(wrapper.obj)) {
    return ReadMinExecutableVirtualAddress(elf->getELFFile(), min_vaddr, file_offset_of_min_vaddr);
  } else if (auto elf = llvm::dyn_cast<llvm::object::ELF64LEObjectFile>(wrapper.obj)) {
    return ReadMinExecutableVirtualAddress(elf->getELFFile(), min_vaddr, file_offset_of_min_vaddr);
  }
  return ElfStatus::FILE_MALFORMED;
}

ElfStatus ReadSectionFromElfFile(const std::string& filename, const std::string& section_name,
                                 std::string* content) {
  ElfStatus result = IsValidElfPath(filename);
  if (result != ElfStatus::NO_ERROR) {
    return result;
  }
  BinaryWrapper wrapper;
  result = OpenObjectFile(filename, 0, 0, &wrapper);
  if (result != ElfStatus::NO_ERROR) {
    return result;
  }
  if (auto elf = llvm::dyn_cast<llvm::object::ELF32LEObjectFile>(wrapper.obj)) {
    return ReadSectionFromELFFile(elf, section_name, content);
  } else if (auto elf = llvm::dyn_cast<llvm::object::ELF64LEObjectFile>(wrapper.obj)) {
    return ReadSectionFromELFFile(elf, section_name, content);
  } else {
    return ElfStatus::FILE_MALFORMED;
  }
}
