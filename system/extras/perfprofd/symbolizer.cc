/*
 *
 * Copyright 2018, The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "symbolizer.h"

#include <map>
#include <memory>
#include <unordered_map>

#include <android-base/logging.h>
#include <base/mem_map.h>
#include <build_id.h>
#include <read_dex_file.h>
#include <read_elf.h>
#include <vdex_file.h>

namespace perfprofd {

namespace {

struct SimpleperfSymbolizer : public Symbolizer {
  // For simplicity, we assume non-overlapping symbols.
  struct Symbol {
    Symbol(std::string_view n, uint64_t l) : name(n), length(l) {}

    std::string name;
    uint64_t length;
  };
  using SymbolMap = std::map<uint64_t, Symbol>;

  std::string Decode(const std::string& dso, uint64_t address) override {
    auto it = dsos.find(dso);
    if (it == dsos.end()) {
      LoadDso(dso);
      it = dsos.find(dso);
      DCHECK(it != dsos.end());
    }

    const SymbolMap& map = it->second;
    if (map.empty()) {
      return "";
    }
    auto upper_bound = map.upper_bound(address);
    if (upper_bound == map.begin()) {
      // Nope, not in the map.
      return "";
    }

    upper_bound--;
    if (upper_bound->first + upper_bound->second.length > address) {
      // This element covers the given address, return its name.
      return upper_bound->second.name;
    }

    return "";
  }

  void LoadDso(const std::string& dso) {
    // See whether it's an ELF file.
    {
      SymbolMap elf_data;
      auto callback = [&elf_data](const ElfFileSymbol& sym) {
        if (sym.is_func) {
          if (sym.len == 0) {
            LOG(ERROR) << "Symbol size is zero for " << sym.name;
          }
          elf_data.emplace(sym.vaddr, Symbol(sym.name, sym.len));
        }
      };
      ElfStatus status = ParseSymbolsFromElfFile(dso, BuildId(), callback);
      if (status == ElfStatus::NO_ERROR) {
        dsos.emplace(dso, std::move(elf_data));
        return;
      }
    }

    // See whether it's a vdex file.
    {
      ::art::MemMap::Init();

      using VdexFile = ::art::VdexFile;
      std::string error_msg;
      std::unique_ptr<VdexFile> vdex = VdexFile::Open(dso,
                                                      /* writable= */ false,
                                                      /* low_4gb= */ false,
                                                      /* unquicken= */ false,
                                                      &error_msg);
      if (vdex != nullptr) {
        const uint8_t* cur = nullptr;
        std::vector<uint64_t> dex_file_offsets;
        const uint8_t* base = vdex->Begin();
        for (;;) {
          cur = vdex->GetNextDexFileData(cur);
          if (cur == nullptr) {
            break;
          }
          dex_file_offsets.push_back(cur - base);
        }

        if (!dex_file_offsets.empty()) {
          std::vector<DexFileSymbol> symbols;
          if (ReadSymbolsFromDexFile(dso, dex_file_offsets, &symbols)) {
            SymbolMap vdex_data;
            for (const DexFileSymbol& symbol : symbols) {
              vdex_data.emplace(symbol.offset, Symbol(symbol.name, symbol.len));
            }
            dsos.emplace(dso, std::move(vdex_data));
            LOG(INFO) << "Found " << symbols.size() << " dex symbols in " << dso;
            return;
          } else {
            LOG(WARNING) << "Could not read symbols from dex files in " << dso;
          }
        } else {
          LOG(WARNING) << "Could not find dex files for vdex " << dso;
          dsos.emplace(dso, SymbolMap());
        }
      } else {
        LOG(WARNING) << dso << " is not a vdex: " << error_msg;
      }
    }

    // TODO: See whether it's a dex file.

    // OK, give up.
    LOG(WARNING) << "Could not symbolize " << dso;
    dsos.emplace(dso, SymbolMap());
  }

  bool GetMinExecutableVAddr(const std::string& dso, uint64_t* addr) override {
    uint64_t file_offset_of_min_vaddr;
    ElfStatus status = ReadMinExecutableVirtualAddressFromElfFile(dso, BuildId(), addr,
                                                                  &file_offset_of_min_vaddr);
    if (status != ElfStatus::NO_ERROR) {
      return true;
    }

    {
      ::art::MemMap::Init();

      using VdexFile = ::art::VdexFile;
      std::string error_msg;
      std::unique_ptr<VdexFile> vdex = VdexFile::Open(dso,
                                                      /* writable= */ false,
                                                      /* low_4gb= */ false,
                                                      /* unquicken= */ false,
                                                      &error_msg);
      if (vdex != nullptr) {
        *addr = 0u;
        return true;
      }
    }

    return false;
  }

  std::unordered_map<std::string, SymbolMap> dsos;
};

}  // namespace

std::unique_ptr<Symbolizer> CreateELFSymbolizer() {
  return std::unique_ptr<Symbolizer>(new SimpleperfSymbolizer());
}

}  // namespace perfprofd
