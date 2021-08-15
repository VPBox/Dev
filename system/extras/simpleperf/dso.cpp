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

#include "dso.h"

#include <stdlib.h>
#include <string.h>

#include <algorithm>
#include <limits>
#include <memory>
#include <vector>

#include <android-base/file.h>
#include <android-base/logging.h>
#include <android-base/strings.h>

#include "environment.h"
#include "read_apk.h"
#include "read_dex_file.h"
#include "read_elf.h"
#include "utils.h"

namespace simpleperf_dso_impl {

std::string RemovePathSeparatorSuffix(const std::string& path) {
  // Don't remove path separator suffix for '/'.
  if (android::base::EndsWith(path, OS_PATH_SEPARATOR) && path.size() > 1u) {
    return path.substr(0, path.size() - 1);
  }
  return path;
}

void DebugElfFileFinder::Reset() {
  vdso_64bit_.clear();
  vdso_32bit_.clear();
  symfs_dir_.clear();
  build_id_to_file_map_.clear();
}

bool DebugElfFileFinder::SetSymFsDir(const std::string& symfs_dir) {
  symfs_dir_ = RemovePathSeparatorSuffix(symfs_dir);
  if (!IsDir(symfs_dir_)) {
    LOG(ERROR) << "Invalid symfs_dir '" << symfs_dir_ << "'";
    return false;
  }
  std::string build_id_list_file = symfs_dir_ + OS_PATH_SEPARATOR + "build_id_list";
  std::string build_id_list;
  if (android::base::ReadFileToString(build_id_list_file, &build_id_list)) {
    for (auto& line : android::base::Split(build_id_list, "\n")) {
      std::vector<std::string> items = android::base::Split(line, "=");
      if (items.size() == 2u) {
        build_id_to_file_map_[items[0]] = symfs_dir_ + OS_PATH_SEPARATOR + items[1];
      }
    }
  }
  return true;
}

bool DebugElfFileFinder::AddSymbolDir(const std::string& symbol_dir) {
  if (!IsDir(symbol_dir)) {
    LOG(ERROR) << "Invalid symbol dir " << symbol_dir;
    return false;
  }
  std::string dir = RemovePathSeparatorSuffix(symbol_dir);
  CollectBuildIdInDir(dir);
  return true;
}

void DebugElfFileFinder::CollectBuildIdInDir(const std::string& dir) {
  for (const std::string& entry : GetEntriesInDir(dir)) {
    std::string path = dir + OS_PATH_SEPARATOR + entry;
    if (IsDir(path)) {
      CollectBuildIdInDir(path);
    } else {
      BuildId build_id;
      if (GetBuildIdFromElfFile(path, &build_id) == ElfStatus::NO_ERROR) {
        build_id_to_file_map_[build_id.ToString()] = path;
      }
    }
  }
}

void DebugElfFileFinder::SetVdsoFile(const std::string& vdso_file, bool is_64bit) {
  if (is_64bit) {
    vdso_64bit_ = vdso_file;
  } else {
    vdso_32bit_ = vdso_file;
  }
}

std::string DebugElfFileFinder::FindDebugFile(const std::string& dso_path, bool force_64bit,
                                              BuildId& build_id) {
  if (dso_path == "[vdso]") {
    if (force_64bit && !vdso_64bit_.empty()) {
      return vdso_64bit_;
    } else if (!force_64bit && !vdso_32bit_.empty()) {
      return vdso_32bit_;
    }
  }
  // 1. Try build_id_to_file_map.
  if (!build_id_to_file_map_.empty()) {
    if (!build_id.IsEmpty() || GetBuildIdFromDsoPath(dso_path, &build_id)) {
      auto it = build_id_to_file_map_.find(build_id.ToString());
      if (it != build_id_to_file_map_.end()) {
        return it->second;
      }
    }
  }
  auto check_path = [&](const std::string& path) {
    BuildId debug_build_id;
    if (GetBuildIdFromDsoPath(path, &debug_build_id)) {
      if (!build_id.IsEmpty() || GetBuildIdFromDsoPath(dso_path, &build_id)) {
        if (build_id == debug_build_id) {
          return true;
        }
      }
    }
    return false;
  };

  // 2. Try concatenating symfs_dir and dso_path.
  if (!symfs_dir_.empty()) {
    std::string path = GetPathInSymFsDir(dso_path);
    if (check_path(path)) {
      return path;
    }
  }
  // 3. Try concatenating /usr/lib/debug and dso_path.
  // Linux host can store debug shared libraries in /usr/lib/debug.
  if (check_path("/usr/lib/debug" + dso_path)) {
    return "/usr/lib/debug" + dso_path;
  }
  return dso_path;
}

std::string DebugElfFileFinder::GetPathInSymFsDir(const std::string& path) {
  auto add_symfs_prefix = [&](const std::string& path) {
    if (android::base::StartsWith(path, OS_PATH_SEPARATOR)) {
      return symfs_dir_ + path;
    }
    return symfs_dir_ + OS_PATH_SEPARATOR + path;
  };
  if (OS_PATH_SEPARATOR == '/') {
    return add_symfs_prefix(path);
  }
  // Paths in recorded perf.data uses '/' as path separator. When reporting on Windows, it needs
  // to be converted to '\\'.
  auto tuple = SplitUrlInApk(path);
  if (std::get<0>(tuple)) {
    std::string apk_path = std::get<1>(tuple);
    std::string entry_path = std::get<2>(tuple);
    std::replace(apk_path.begin(), apk_path.end(), '/', OS_PATH_SEPARATOR);
    return GetUrlInApk(add_symfs_prefix(apk_path), entry_path);
  }
  std::string elf_path = path;
  std::replace(elf_path.begin(), elf_path.end(), '/', OS_PATH_SEPARATOR);
  return add_symfs_prefix(elf_path);
}
}  // namespace simpleperf_dso_imp

static OneTimeFreeAllocator symbol_name_allocator;

Symbol::Symbol(std::string_view name, uint64_t addr, uint64_t len)
    : addr(addr),
      len(len),
      name_(symbol_name_allocator.AllocateString(name)),
      demangled_name_(nullptr),
      dump_id_(UINT_MAX) {
}

const char* Symbol::DemangledName() const {
  if (demangled_name_ == nullptr) {
    const std::string s = Dso::Demangle(name_);
    if (s == name_) {
      demangled_name_ = name_;
    } else {
      demangled_name_ = symbol_name_allocator.AllocateString(s);
    }
  }
  return demangled_name_;
}

bool Dso::demangle_ = true;
std::string Dso::vmlinux_;
std::string Dso::kallsyms_;
bool Dso::read_kernel_symbols_from_proc_;
std::unordered_map<std::string, BuildId> Dso::build_id_map_;
size_t Dso::dso_count_;
uint32_t Dso::g_dump_id_;
simpleperf_dso_impl::DebugElfFileFinder Dso::debug_elf_file_finder_;

void Dso::SetDemangle(bool demangle) { demangle_ = demangle; }

extern "C" char* __cxa_demangle(const char* mangled_name, char* buf, size_t* n,
                                int* status);

std::string Dso::Demangle(const std::string& name) {
  if (!demangle_) {
    return name;
  }
  int status;
  bool is_linker_symbol = (name.find(linker_prefix) == 0);
  const char* mangled_str = name.c_str();
  if (is_linker_symbol) {
    mangled_str += linker_prefix.size();
  }
  std::string result = name;
  char* demangled_name = __cxa_demangle(mangled_str, nullptr, nullptr, &status);
  if (status == 0) {
    if (is_linker_symbol) {
      result = std::string("[linker]") + demangled_name;
    } else {
      result = demangled_name;
    }
    free(demangled_name);
  } else if (is_linker_symbol) {
    result = std::string("[linker]") + mangled_str;
  }
  return result;
}

bool Dso::SetSymFsDir(const std::string& symfs_dir) {
  return debug_elf_file_finder_.SetSymFsDir(symfs_dir);
}

bool Dso::AddSymbolDir(const std::string& symbol_dir) {
  return debug_elf_file_finder_.AddSymbolDir(symbol_dir);
}

void Dso::SetVmlinux(const std::string& vmlinux) { vmlinux_ = vmlinux; }

void Dso::SetBuildIds(
    const std::vector<std::pair<std::string, BuildId>>& build_ids) {
  std::unordered_map<std::string, BuildId> map;
  for (auto& pair : build_ids) {
    LOG(DEBUG) << "build_id_map: " << pair.first << ", "
               << pair.second.ToString();
    map.insert(pair);
  }
  build_id_map_ = std::move(map);
}

void Dso::SetVdsoFile(const std::string& vdso_file, bool is_64bit) {
  debug_elf_file_finder_.SetVdsoFile(vdso_file, is_64bit);
}

BuildId Dso::FindExpectedBuildIdForPath(const std::string& path) {
  auto it = build_id_map_.find(path);
  if (it != build_id_map_.end()) {
    return it->second;
  }
  return BuildId();
}

BuildId Dso::GetExpectedBuildId() {
  return FindExpectedBuildIdForPath(path_);
}

Dso::Dso(DsoType type, const std::string& path, const std::string& debug_file_path)
    : type_(type),
      path_(path),
      debug_file_path_(debug_file_path),
      is_loaded_(false),
      dump_id_(UINT_MAX),
      symbol_dump_id_(0),
      symbol_warning_loglevel_(android::base::WARNING) {
  size_t pos = path.find_last_of("/\\");
  if (pos != std::string::npos) {
    file_name_ = path.substr(pos + 1);
  } else {
    file_name_ = path;
  }
  dso_count_++;
}

Dso::~Dso() {
  if (--dso_count_ == 0) {
    // Clean up global variables when no longer used.
    symbol_name_allocator.Clear();
    demangle_ = true;
    vmlinux_.clear();
    kallsyms_.clear();
    read_kernel_symbols_from_proc_ = false;
    build_id_map_.clear();
    g_dump_id_ = 0;
    debug_elf_file_finder_.Reset();
  }
}

uint32_t Dso::CreateDumpId() {
  CHECK(!HasDumpId());
  return dump_id_ = g_dump_id_++;
}

uint32_t Dso::CreateSymbolDumpId(const Symbol* symbol) {
  CHECK(!symbol->HasDumpId());
  symbol->dump_id_ = symbol_dump_id_++;
  return symbol->dump_id_;
}

const Symbol* Dso::FindSymbol(uint64_t vaddr_in_dso) {
  if (!is_loaded_) {
    Load();
  }
  auto it = std::upper_bound(symbols_.begin(), symbols_.end(),
                             Symbol("", vaddr_in_dso, 0),
                             Symbol::CompareValueByAddr);
  if (it != symbols_.begin()) {
    --it;
    if (it->addr <= vaddr_in_dso && (it->addr + it->len > vaddr_in_dso)) {
      return &*it;
    }
  }
  if (!unknown_symbols_.empty()) {
    auto it = unknown_symbols_.find(vaddr_in_dso);
    if (it != unknown_symbols_.end()) {
      return &it->second;
    }
  }
  return nullptr;
}

void Dso::SetSymbols(std::vector<Symbol>* symbols) {
  symbols_ = std::move(*symbols);
  symbols->clear();
}

void Dso::AddUnknownSymbol(uint64_t vaddr_in_dso, const std::string& name) {
  unknown_symbols_.insert(std::make_pair(vaddr_in_dso, Symbol(name, vaddr_in_dso, 1)));
}

bool Dso::IsForJavaMethod() {
  if (type_ == DSO_DEX_FILE) {
    return true;
  }
  if (type_ == DSO_ELF_FILE) {
    // JIT symfiles for JITed Java methods are dumped as temporary files, whose name are in format
    // "TemporaryFile-XXXXXX".
    size_t pos = path_.rfind('/');
    pos = (pos == std::string::npos) ? 0 : pos + 1;
    return strncmp(&path_[pos], "TemporaryFile", strlen("TemporaryFile")) == 0;
  }
  return false;
}

void Dso::Load() {
  is_loaded_ = true;
  std::vector<Symbol> symbols = LoadSymbols();
  if (symbols_.empty()) {
    symbols_ = std::move(symbols);
  } else {
    std::vector<Symbol> merged_symbols;
    std::set_union(symbols_.begin(), symbols_.end(), symbols.begin(), symbols.end(),
                   std::back_inserter(merged_symbols), Symbol::CompareValueByAddr);
    symbols_ = std::move(merged_symbols);
  }
}

static void ReportReadElfSymbolResult(ElfStatus result, const std::string& path,
    const std::string& debug_file_path,
    android::base::LogSeverity warning_loglevel = android::base::WARNING) {
  if (result == ElfStatus::NO_ERROR) {
    LOG(VERBOSE) << "Read symbols from " << debug_file_path << " successfully";
  } else if (result == ElfStatus::NO_SYMBOL_TABLE) {
    if (path == "[vdso]") {
      // Vdso only contains dynamic symbol table, and we can't change that.
      return;
    }
    // Lacking symbol table isn't considered as an error but worth reporting.
    LOG(warning_loglevel) << debug_file_path << " doesn't contain symbol table";
  } else {
    LOG(warning_loglevel) << "failed to read symbols from " << debug_file_path << ": " << result;
  }
}

static void SortAndFixSymbols(std::vector<Symbol>& symbols) {
  std::sort(symbols.begin(), symbols.end(), Symbol::CompareValueByAddr);
  Symbol* prev_symbol = nullptr;
  for (auto& symbol : symbols) {
    if (prev_symbol != nullptr && prev_symbol->len == 0) {
      prev_symbol->len = symbol.addr - prev_symbol->addr;
    }
    prev_symbol = &symbol;
  }
}

class DexFileDso : public Dso {
 public:
  DexFileDso(const std::string& path, const std::string& debug_file_path)
      : Dso(DSO_DEX_FILE, path, debug_file_path) {}

  void AddDexFileOffset(uint64_t dex_file_offset) override {
    auto it = std::lower_bound(dex_file_offsets_.begin(), dex_file_offsets_.end(),
                               dex_file_offset);
    if (it != dex_file_offsets_.end() && *it == dex_file_offset) {
      return;
    }
    dex_file_offsets_.insert(it, dex_file_offset);
  }

  const std::vector<uint64_t>* DexFileOffsets() override {
    return &dex_file_offsets_;
  }

  uint64_t IpToVaddrInFile(uint64_t ip, uint64_t map_start, uint64_t map_pgoff) override {
    return ip - map_start + map_pgoff;
  }

  std::vector<Symbol> LoadSymbols() override {
    std::vector<Symbol> symbols;
    std::vector<DexFileSymbol> dex_file_symbols;
    auto tuple = SplitUrlInApk(debug_file_path_);
    bool status = false;
    if (std::get<0>(tuple)) {
      std::unique_ptr<ArchiveHelper> ahelper = ArchiveHelper::CreateInstance(std::get<1>(tuple));
      ZipEntry entry;
      std::vector<uint8_t> data;
      if (ahelper &&
          ahelper->FindEntry(std::get<2>(tuple), &entry) && ahelper->GetEntryData(entry, &data)) {
        status = ReadSymbolsFromDexFileInMemory(data.data(), data.size(), dex_file_offsets_,
                                                &dex_file_symbols);
      }
    } else {
      status = ReadSymbolsFromDexFile(debug_file_path_, dex_file_offsets_, &dex_file_symbols);
    }
    if (!status) {
      android::base::LogSeverity level = symbols_.empty() ? android::base::WARNING
                                                          : android::base::DEBUG;
      LOG(level) << "Failed to read symbols from " << debug_file_path_;
      return symbols;
    }
    LOG(VERBOSE) << "Read symbols from " << debug_file_path_ << " successfully";
    for (auto& symbol : dex_file_symbols) {
      symbols.emplace_back(symbol.name, symbol.offset, symbol.len);
    }
    SortAndFixSymbols(symbols);
    return symbols;
  }

 private:
  std::vector<uint64_t> dex_file_offsets_;
};

class ElfDso : public Dso {
 public:
  ElfDso(const std::string& path, const std::string& debug_file_path)
      : Dso(DSO_ELF_FILE, path, debug_file_path) {}

  void SetMinExecutableVaddr(uint64_t min_vaddr, uint64_t file_offset) override {
    min_vaddr_ = min_vaddr;
    file_offset_of_min_vaddr_ = file_offset;
  }

  void GetMinExecutableVaddr(uint64_t* min_vaddr, uint64_t* file_offset) override {
    if (type_ == DSO_DEX_FILE) {
      return dex_file_dso_->GetMinExecutableVaddr(min_vaddr, file_offset);
    }
    if (min_vaddr_ == uninitialized_value) {
      min_vaddr_ = 0;
      BuildId build_id = GetExpectedBuildId();
      uint64_t addr;
      uint64_t offset;
      ElfStatus result;
      auto tuple = SplitUrlInApk(debug_file_path_);
      if (std::get<0>(tuple)) {
        EmbeddedElf* elf = ApkInspector::FindElfInApkByName(std::get<1>(tuple),
                                                            std::get<2>(tuple));
        if (elf == nullptr) {
          result = ElfStatus::FILE_NOT_FOUND;
        } else {
          result = ReadMinExecutableVirtualAddressFromEmbeddedElfFile(
              elf->filepath(), elf->entry_offset(), elf->entry_size(), build_id, &addr, &offset);
        }
      } else {
        result = ReadMinExecutableVirtualAddressFromElfFile(debug_file_path_, build_id, &addr,
                                                            &offset);
      }
      if (result != ElfStatus::NO_ERROR) {
        LOG(WARNING) << "failed to read min virtual address of "
                     << GetDebugFilePath() << ": " << result;
      } else {
        min_vaddr_ = addr;
        file_offset_of_min_vaddr_ = offset;
      }
    }
    *min_vaddr = min_vaddr_;
    *file_offset = file_offset_of_min_vaddr_;
  }

  uint64_t IpToVaddrInFile(uint64_t ip, uint64_t map_start, uint64_t map_pgoff) override {
    if (type_ == DSO_DEX_FILE) {
      return dex_file_dso_->IpToVaddrInFile(ip, map_start, map_pgoff);
    }
    uint64_t min_vaddr;
    uint64_t file_offset_of_min_vaddr;
    GetMinExecutableVaddr(&min_vaddr, &file_offset_of_min_vaddr);
    if (file_offset_of_min_vaddr == uninitialized_value) {
      return ip - map_start + min_vaddr;
    }
    // Apps may make part of the executable segment of a shared library writeable, which can
    // generate multiple executable segments at runtime. So use map_pgoff to calculate
    // vaddr_in_file.
    return ip - map_start + map_pgoff - file_offset_of_min_vaddr + min_vaddr;
  }

  void AddDexFileOffset(uint64_t dex_file_offset) override {
    if (type_ == DSO_ELF_FILE) {
      // When simpleperf does unwinding while recording, it processes mmap records before reading
      // dex file linked list (via JITDebugReader). To process mmap records, it creates Dso
      // objects of type ELF_FILE. Then after reading dex file linked list, it realizes some
      // ELF_FILE Dso objects should actually be DEX_FILE, because they have dex file offsets.
      // So here converts ELF_FILE Dso into DEX_FILE Dso.
      type_ = DSO_DEX_FILE;
      dex_file_dso_.reset(new DexFileDso(path_, path_));
    }
    dex_file_dso_->AddDexFileOffset(dex_file_offset);
  }

  const std::vector<uint64_t>* DexFileOffsets() override {
    return dex_file_dso_ ? dex_file_dso_->DexFileOffsets() : nullptr;
  }

 protected:
  std::vector<Symbol> LoadSymbols() override {
    if (dex_file_dso_) {
      return dex_file_dso_->LoadSymbols();
    }
    std::vector<Symbol> symbols;
    BuildId build_id = GetExpectedBuildId();
    auto symbol_callback = [&](const ElfFileSymbol& symbol) {
      if (symbol.is_func || (symbol.is_label && symbol.is_in_text_section)) {
        symbols.emplace_back(symbol.name, symbol.vaddr, symbol.len);
      }
    };
    ElfStatus status;
    std::tuple<bool, std::string, std::string> tuple = SplitUrlInApk(debug_file_path_);
    if (std::get<0>(tuple)) {
      EmbeddedElf* elf = ApkInspector::FindElfInApkByName(std::get<1>(tuple), std::get<2>(tuple));
      if (elf == nullptr) {
        status = ElfStatus::FILE_NOT_FOUND;
      } else {
        status = ParseSymbolsFromEmbeddedElfFile(elf->filepath(), elf->entry_offset(),
                                                 elf->entry_size(), build_id, symbol_callback);
      }
    } else {
      status = ParseSymbolsFromElfFile(debug_file_path_, build_id, symbol_callback);
    }
    ReportReadElfSymbolResult(status, path_, debug_file_path_,
                              symbols_.empty() ? android::base::WARNING : android::base::DEBUG);
    SortAndFixSymbols(symbols);
    return symbols;
  }

 private:
  static constexpr uint64_t uninitialized_value = std::numeric_limits<uint64_t>::max();

  uint64_t min_vaddr_ = uninitialized_value;
  uint64_t file_offset_of_min_vaddr_ = uninitialized_value;
  std::unique_ptr<DexFileDso> dex_file_dso_;
};

class KernelDso : public Dso {
 public:
  KernelDso(const std::string& path, const std::string& debug_file_path)
      : Dso(DSO_KERNEL, path, debug_file_path) {}

  uint64_t IpToVaddrInFile(uint64_t ip, uint64_t, uint64_t) override {
    return ip;
  }

 protected:
  std::vector<Symbol> LoadSymbols() override {
    std::vector<Symbol> symbols;
    BuildId build_id = GetExpectedBuildId();
    if (!vmlinux_.empty()) {
      auto symbol_callback = [&](const ElfFileSymbol& symbol) {
        if (symbol.is_func) {
          symbols.emplace_back(symbol.name, symbol.vaddr, symbol.len);
        }
      };
      ElfStatus status = ParseSymbolsFromElfFile(vmlinux_, build_id, symbol_callback);
      ReportReadElfSymbolResult(status, path_, vmlinux_);
    } else if (!kallsyms_.empty()) {
      symbols = ReadSymbolsFromKallsyms(kallsyms_);
    } else if (read_kernel_symbols_from_proc_ || !build_id.IsEmpty()) {
      // Try /proc/kallsyms only when asked to do so, or when build id matches.
      // Otherwise, it is likely to use /proc/kallsyms on host for perf.data recorded on device.
      bool can_read_kallsyms = true;
      if (!build_id.IsEmpty()) {
        BuildId real_build_id;
        if (!GetKernelBuildId(&real_build_id) || build_id != real_build_id) {
          LOG(DEBUG) << "failed to read symbols from /proc/kallsyms: Build id mismatch";
          can_read_kallsyms = false;
        }
      }
      if (can_read_kallsyms) {
        std::string kallsyms;
        if (!android::base::ReadFileToString("/proc/kallsyms", &kallsyms)) {
          LOG(DEBUG) << "failed to read /proc/kallsyms";
        } else {
          symbols = ReadSymbolsFromKallsyms(kallsyms);
        }
      }
    }
    SortAndFixSymbols(symbols);
    if (!symbols.empty()) {
      symbols.back().len = std::numeric_limits<uint64_t>::max() - symbols.back().addr;
    }
    return symbols;
  }

 private:
  std::vector<Symbol> ReadSymbolsFromKallsyms(std::string& kallsyms) {
    std::vector<Symbol> symbols;
    auto symbol_callback = [&](const KernelSymbol& symbol) {
      if (strchr("TtWw", symbol.type) && symbol.addr != 0u) {
        symbols.emplace_back(symbol.name, symbol.addr, 0);
      }
      return false;
    };
    ProcessKernelSymbols(kallsyms, symbol_callback);
    if (symbols.empty()) {
      LOG(WARNING) << "Symbol addresses in /proc/kallsyms on device are all zero. "
                      "`echo 0 >/proc/sys/kernel/kptr_restrict` if possible.";
    }
    return symbols;
  }
};

class KernelModuleDso : public Dso {
 public:
  KernelModuleDso(const std::string& path, const std::string& debug_file_path)
      : Dso(DSO_KERNEL_MODULE, path, debug_file_path) {}

  uint64_t IpToVaddrInFile(uint64_t ip, uint64_t map_start, uint64_t) override {
    return ip - map_start;
  }

 protected:
  std::vector<Symbol> LoadSymbols() override {
    std::vector<Symbol> symbols;
    BuildId build_id = GetExpectedBuildId();
    auto symbol_callback = [&](const ElfFileSymbol& symbol) {
      if (symbol.is_func || symbol.is_in_text_section) {
        symbols.emplace_back(symbol.name, symbol.vaddr, symbol.len);
      }
    };
    ElfStatus status = ParseSymbolsFromElfFile(debug_file_path_, build_id, symbol_callback);
    ReportReadElfSymbolResult(status, path_, debug_file_path_,
                              symbols_.empty() ? android::base::WARNING : android::base::DEBUG);
    SortAndFixSymbols(symbols);
    return symbols;
  }
};

class UnknownDso : public Dso {
 public:
  UnknownDso(const std::string& path) : Dso(DSO_UNKNOWN_FILE, path, path) {}

  uint64_t IpToVaddrInFile(uint64_t ip, uint64_t, uint64_t) override {
    return ip;
  }

 protected:
  std::vector<Symbol> LoadSymbols() override {
    return std::vector<Symbol>();
  }
};

std::unique_ptr<Dso> Dso::CreateDso(DsoType dso_type, const std::string& dso_path,
                                    bool force_64bit) {
  switch (dso_type) {
    case DSO_ELF_FILE: {
      BuildId build_id = FindExpectedBuildIdForPath(dso_path);
      return std::unique_ptr<Dso>(new ElfDso(dso_path,
          debug_elf_file_finder_.FindDebugFile(dso_path, force_64bit, build_id)));
    }
    case DSO_KERNEL:
      return std::unique_ptr<Dso>(new KernelDso(dso_path, dso_path));
    case DSO_KERNEL_MODULE:
      return std::unique_ptr<Dso>(new KernelModuleDso(dso_path, dso_path));
    case DSO_DEX_FILE:
      return std::unique_ptr<Dso>(new DexFileDso(dso_path, dso_path));
    case DSO_UNKNOWN_FILE:
      return std::unique_ptr<Dso>(new UnknownDso(dso_path));
    default:
      LOG(FATAL) << "Unexpected dso_type " << static_cast<int>(dso_type);
  }
  return nullptr;
}

const char* DsoTypeToString(DsoType dso_type) {
  switch (dso_type) {
    case DSO_KERNEL:
      return "dso_kernel";
    case DSO_KERNEL_MODULE:
      return "dso_kernel_module";
    case DSO_ELF_FILE:
      return "dso_elf_file";
    case DSO_DEX_FILE:
      return "dso_dex_file";
    default:
      return "unknown";
  }
}

bool GetBuildIdFromDsoPath(const std::string& dso_path, BuildId* build_id) {
  auto tuple = SplitUrlInApk(dso_path);
  ElfStatus result;
  if (std::get<0>(tuple)) {
    EmbeddedElf* elf = ApkInspector::FindElfInApkByName(std::get<1>(tuple), std::get<2>(tuple));
    if (elf == nullptr) {
      result = ElfStatus::FILE_NOT_FOUND;
    } else {
      result = GetBuildIdFromEmbeddedElfFile(elf->filepath(), elf->entry_offset(),
                                             elf->entry_size(), build_id);
    }
  } else {
    result = GetBuildIdFromElfFile(dso_path, build_id);
  }
  return result == ElfStatus::NO_ERROR;
}
