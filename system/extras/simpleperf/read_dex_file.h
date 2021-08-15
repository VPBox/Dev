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

#ifndef SIMPLE_PERF_READ_DEX_FILE_H_
#define SIMPLE_PERF_READ_DEX_FILE_H_

#include <inttypes.h>

#include <string>
#include <vector>

#ifndef NO_LIBDEXFILE_SUPPORT
#include <art_api/dex_file_support.h>
#endif

#ifndef NO_LIBDEXFILE_SUPPORT
typedef art_api::dex::MethodInfo DexFileSymbol;
#else
struct DexFileSymbol {
  uint64_t offset;
  uint64_t len;
  std::string name;
};
#endif

bool ReadSymbolsFromDexFileInMemory(void* addr, uint64_t size,
                                    const std::vector<uint64_t>& dex_file_offsets,
                                    std::vector<DexFileSymbol>* symbols);
bool ReadSymbolsFromDexFile(const std::string& file_path,
                            const std::vector<uint64_t>& dex_file_offsets,
                            std::vector<DexFileSymbol>* symbols);

#endif  // SIMPLE_PERF_READ_DEX_FILE_H_
