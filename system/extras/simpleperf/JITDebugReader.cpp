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

#include "JITDebugReader.h"

#include <inttypes.h>
#include <sys/mman.h>
#include <sys/uio.h>

#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <android-base/file.h>
#include <android-base/logging.h>
#include <android-base/strings.h>

#include "dso.h"
#include "environment.h"
#include "read_apk.h"
#include "read_elf.h"
#include "utils.h"

namespace simpleperf {

// If the size of a symfile is larger than EXPECTED_MAX_SYMFILE_SIZE, we don't want to read it
// remotely.
static constexpr size_t MAX_JIT_SYMFILE_SIZE = 1024 * 1024u;

// It takes about 30us-130us on Pixel (depending on the cpu frequency) to check if the descriptors
// have been updated (most time spent in process_vm_preadv). We want to know if the JIT debug info
// changed as soon as possible, while not wasting too much time checking for updates. So use a
// period of 100 ms.
// In system wide profiling, we may need to check JIT debug info changes for many processes, to
// avoid spending all time checking, wait 100 ms between any two checks.
static constexpr size_t kUpdateJITDebugInfoIntervalInMs = 100;

// Match the format of JITDescriptor in art/runtime/jit/debugger_itnerface.cc.
template <typename ADDRT>
struct JITDescriptor {
  uint32_t version;
  uint32_t action_flag;
  ADDRT relevant_entry_addr;
  ADDRT first_entry_addr;
  uint8_t magic[8];
  uint32_t flags;
  uint32_t sizeof_descriptor;
  uint32_t sizeof_entry;
  uint32_t action_seqlock;  // incremented before and after any modification
  uint64_t action_timestamp;  // CLOCK_MONOTONIC time of last action

  bool Valid() const {
    return version == 1 && strncmp(reinterpret_cast<const char*>(magic), "Android1", 8) == 0;
  }
};

// Match the format of JITCodeEntry in art/runtime/jit/debugger_itnerface.cc.
template <typename ADDRT>
struct JITCodeEntry {
  ADDRT next_addr;
  ADDRT prev_addr;
  ADDRT symfile_addr;
  uint64_t symfile_size;
  uint64_t register_timestamp;  // CLOCK_MONOTONIC time of entry registration

  bool Valid() const {
    return symfile_addr > 0u && symfile_size > 0u;
  }
};

// Match the format of JITCodeEntry in art/runtime/jit/debugger_interface.cc.
template <typename ADDRT>
struct __attribute__((packed)) PackedJITCodeEntry {
  ADDRT next_addr;
  ADDRT prev_addr;
  ADDRT symfile_addr;
  uint64_t symfile_size;
  uint64_t register_timestamp;

  bool Valid() const {
    return symfile_addr > 0u && symfile_size > 0u;
  }
};

using JITDescriptor32 = JITDescriptor<uint32_t>;
using JITDescriptor64 = JITDescriptor<uint64_t>;

#if defined(__x86_64__)
// Make sure simpleperf built for i386 and x86_64 see the correct JITCodeEntry layout of i386.
using JITCodeEntry32 = PackedJITCodeEntry<uint32_t>;
#else
using JITCodeEntry32 = JITCodeEntry<uint32_t>;
#endif
using JITCodeEntry64 = JITCodeEntry<uint64_t>;

// We want to support both 64-bit and 32-bit simpleperf when profiling either 64-bit or 32-bit
// apps. So using static_asserts to make sure that simpleperf on arm and aarch64 having the same
// view of structures, and simpleperf on i386 and x86_64 having the same view of structures.
static_assert(sizeof(JITDescriptor32) == 48, "");
static_assert(sizeof(JITDescriptor64) == 56, "");
#if defined(__i386__) or defined(__x86_64__)
static_assert(sizeof(JITCodeEntry32) == 28, "");
#else
static_assert(sizeof(JITCodeEntry32) == 32, "");
#endif
static_assert(sizeof(JITCodeEntry64) == 40, "");

bool JITDebugReader::RegisterDebugInfoCallback(IOEventLoop* loop,
                                             const debug_info_callback_t& callback) {
  debug_info_callback_ = callback;
  read_event_ = loop->AddPeriodicEvent(SecondToTimeval(kUpdateJITDebugInfoIntervalInMs / 1000.0),
                                       [this]() { return ReadAllProcesses(); });
  return (read_event_ != nullptr && IOEventLoop::DisableEvent(read_event_));
}

bool JITDebugReader::MonitorProcess(pid_t pid) {
  if (processes_.find(pid) == processes_.end()) {
    processes_[pid].pid = pid;
    LOG(DEBUG) << "Start monitoring process " << pid;
    if (processes_.size() == 1u) {
      if (!IOEventLoop::EnableEvent(read_event_)) {
        return false;
      }
    }
  }
  return true;
}

static bool IsArtLib(const std::string& filename) {
  return android::base::EndsWith(filename, "libart.so") ||
      android::base::EndsWith(filename, "libartd.so");
}

bool JITDebugReader::UpdateRecord(const Record* record) {
  if (record->type() == PERF_RECORD_MMAP) {
    auto r = static_cast<const MmapRecord*>(record);
    if (IsArtLib(r->filename)) {
      pids_with_art_lib_.emplace(r->data->pid, false);
    }
  } else if (record->type() == PERF_RECORD_MMAP2) {
    auto r = static_cast<const Mmap2Record*>(record);
    if (IsArtLib(r->filename)) {
      pids_with_art_lib_.emplace(r->data->pid, false);
    }
  } else if (record->type() == PERF_RECORD_FORK) {
    auto r = static_cast<const ForkRecord*>(record);
    if (r->data->pid != r->data->ppid &&
        pids_with_art_lib_.find(r->data->ppid) != pids_with_art_lib_.end()) {
      pids_with_art_lib_.emplace(r->data->pid, false);
    }
  } else if (record->type() == PERF_RECORD_SAMPLE) {
    auto r = static_cast<const SampleRecord*>(record);
    auto it = pids_with_art_lib_.find(r->tid_data.pid);
    if (it != pids_with_art_lib_.end() && !it->second) {
      it->second = true;
      if (!MonitorProcess(r->tid_data.pid)) {
        return false;
      }
      return ReadProcess(r->tid_data.pid);
    }
  }
  return FlushDebugInfo(record->Timestamp());
}

bool JITDebugReader::FlushDebugInfo(uint64_t timestamp) {
  if (sync_with_records_) {
    if (!debug_info_q_.empty() && debug_info_q_.top().timestamp < timestamp) {
      std::vector<JITDebugInfo> debug_info;
      while (!debug_info_q_.empty() && debug_info_q_.top().timestamp < timestamp) {
        debug_info.emplace_back(debug_info_q_.top());
        debug_info_q_.pop();
      }
      return debug_info_callback_(debug_info, false);
    }
  }
  return true;
}

bool JITDebugReader::ReadAllProcesses() {
  if (!IOEventLoop::DisableEvent(read_event_)) {
    return false;
  }
  std::vector<JITDebugInfo> debug_info;
  for (auto it = processes_.begin(); it != processes_.end();) {
    Process& process = it->second;
    ReadProcess(process, &debug_info);
    if (process.died) {
      LOG(DEBUG) << "Stop monitoring process " << process.pid;
      it = processes_.erase(it);
    } else {
      ++it;
    }
  }
  if (!AddDebugInfo(debug_info, true)) {
    return false;
  }
  if (!processes_.empty()) {
    return IOEventLoop::EnableEvent(read_event_);
  }
  return true;
}

bool JITDebugReader::ReadProcess(pid_t pid) {
  auto it = processes_.find(pid);
  if (it != processes_.end()) {
    std::vector<JITDebugInfo> debug_info;
    ReadProcess(it->second, &debug_info);
    return AddDebugInfo(debug_info, false);
  }
  return true;
}

void JITDebugReader::ReadProcess(Process& process, std::vector<JITDebugInfo>* debug_info) {
  if (process.died || (!process.initialized && !InitializeProcess(process))) {
    return;
  }
  // 1. Read descriptors.
  Descriptor jit_descriptor;
  Descriptor dex_descriptor;
  if (!ReadDescriptors(process, &jit_descriptor, &dex_descriptor)) {
    return;
  }
  // 2. Return if descriptors are not changed.
  if (jit_descriptor.action_seqlock == process.last_jit_descriptor.action_seqlock &&
      dex_descriptor.action_seqlock == process.last_dex_descriptor.action_seqlock) {
    return;
  }

  // 3. Read new symfiles.
  auto check_descriptor = [&](Descriptor& descriptor, bool is_jit) {
      Descriptor tmp_jit_descriptor;
      Descriptor tmp_dex_descriptor;
      if (!ReadDescriptors(process, &tmp_jit_descriptor, &tmp_dex_descriptor)) {
        return false;
      }
      if (is_jit) {
        return descriptor.action_seqlock == tmp_jit_descriptor.action_seqlock;
      }
      return descriptor.action_seqlock == tmp_dex_descriptor.action_seqlock;
  };

  auto read_debug_info = [&](Descriptor& new_descriptor, Descriptor& old_descriptor, bool is_jit) {
    bool has_update = new_descriptor.action_seqlock != old_descriptor.action_seqlock &&
                      (new_descriptor.action_seqlock & 1) == 0;
    LOG(DEBUG) << (is_jit ? "JIT" : "Dex") << " symfiles of pid " << process.pid
        << ": old seqlock " << old_descriptor.action_seqlock
        << ", new seqlock " << new_descriptor.action_seqlock;
    if (!has_update) {
      return false;
    }
    std::vector<CodeEntry> new_entries;
    // Adding or removing one code entry will make two increments of action_seqlock. So we should
    // not read more than (seqlock_diff / 2) new entries.
    uint32_t read_entry_limit = (new_descriptor.action_seqlock - old_descriptor.action_seqlock) / 2;
    if (!ReadNewCodeEntries(process, new_descriptor, old_descriptor.action_timestamp,
                            read_entry_limit, &new_entries)) {
      return false;
    }
    // Check if the descriptor was changed while we were reading new entries.
    if (!check_descriptor(new_descriptor, is_jit)) {
      return false;
    }
    LOG(DEBUG) << (is_jit ? "JIT" : "Dex") << " symfiles of pid " << process.pid
               << ": read " << new_entries.size() << " new entries";
    if (new_entries.empty()) {
      return true;
    }
    if (is_jit) {
      ReadJITCodeDebugInfo(process, new_entries, debug_info);
    } else {
      ReadDexFileDebugInfo(process, new_entries, debug_info);
    }
    return true;
  };
  if (read_debug_info(jit_descriptor, process.last_jit_descriptor, true)) {
    process.last_jit_descriptor = jit_descriptor;
  }
  if (read_debug_info(dex_descriptor, process.last_dex_descriptor, false)) {
    process.last_dex_descriptor = dex_descriptor;
  }
}

bool JITDebugReader::InitializeProcess(Process& process) {
  // 1. Read map file to find the location of libart.so.
  std::vector<ThreadMmap> thread_mmaps;
  if (!GetThreadMmapsInProcess(process.pid, &thread_mmaps)) {
    process.died = true;
    return false;
  }
  std::string art_lib_path;
  uint64_t min_vaddr_in_memory;
  for (auto& map : thread_mmaps) {
    if ((map.prot & PROT_EXEC) && IsArtLib(map.name)) {
      art_lib_path = map.name;
      min_vaddr_in_memory = map.start_addr;
      break;
    }
  }
  if (art_lib_path.empty()) {
    return false;
  }
  process.is_64bit = art_lib_path.find("lib64") != std::string::npos;

  // 2. Read libart.so to find the addresses of __jit_debug_descriptor and __dex_debug_descriptor.
  const DescriptorsLocation* location = GetDescriptorsLocation(art_lib_path, process.is_64bit);
  if (location == nullptr) {
    return false;
  }
  process.descriptors_addr = location->relative_addr + min_vaddr_in_memory;
  process.descriptors_size = location->size;
  process.jit_descriptor_offset = location->jit_descriptor_offset;
  process.dex_descriptor_offset = location->dex_descriptor_offset;
  process.initialized = true;
  return true;
}

const JITDebugReader::DescriptorsLocation* JITDebugReader::GetDescriptorsLocation(
    const std::string& art_lib_path, bool is_64bit) {
  auto it = descriptors_location_cache_.find(art_lib_path);
  if (it != descriptors_location_cache_.end()) {
    return it->second.relative_addr == 0u ? nullptr : &it->second;
  }
  DescriptorsLocation& location = descriptors_location_cache_[art_lib_path];

  // Read libart.so to find the addresses of __jit_debug_descriptor and __dex_debug_descriptor.
  uint64_t min_vaddr_in_file;
  uint64_t file_offset;
  ElfStatus status = ReadMinExecutableVirtualAddressFromElfFile(art_lib_path, BuildId(),
                                                                &min_vaddr_in_file,
                                                                &file_offset);
  if (status != ElfStatus::NO_ERROR) {
    LOG(ERROR) << "ReadMinExecutableVirtualAddress failed, status = " << status;
    return nullptr;
  }
  const char* jit_str = "__jit_debug_descriptor";
  const char* dex_str = "__dex_debug_descriptor";
  uint64_t jit_addr = 0u;
  uint64_t dex_addr = 0u;

  auto callback = [&](const ElfFileSymbol& symbol) {
    if (symbol.name == jit_str) {
      jit_addr = symbol.vaddr - min_vaddr_in_file;
    } else if (symbol.name == dex_str) {
      dex_addr = symbol.vaddr - min_vaddr_in_file;
    }
  };
  if (ParseDynamicSymbolsFromElfFile(art_lib_path, callback) != ElfStatus::NO_ERROR) {
    return nullptr;
  }
  if (jit_addr == 0u || dex_addr == 0u) {
    return nullptr;
  }
  location.relative_addr = std::min(jit_addr, dex_addr);
  location.size = std::max(jit_addr, dex_addr) +
      (is_64bit ? sizeof(JITDescriptor64) : sizeof(JITDescriptor32)) - location.relative_addr;
  if (location.size >= 4096u) {
    PLOG(WARNING) << "The descriptors_size is unexpected large: " << location.size;
  }
  if (descriptors_buf_.size() < location.size) {
    descriptors_buf_.resize(location.size);
  }
  location.jit_descriptor_offset = jit_addr - location.relative_addr;
  location.dex_descriptor_offset = dex_addr - location.relative_addr;
  return &location;
}

bool JITDebugReader::ReadRemoteMem(Process& process, uint64_t remote_addr, uint64_t size,
                                   void* data) {
  iovec local_iov;
  local_iov.iov_base = data;
  local_iov.iov_len = size;
  iovec remote_iov;
  remote_iov.iov_base = reinterpret_cast<void*>(static_cast<uintptr_t>(remote_addr));
  remote_iov.iov_len = size;
  ssize_t result = process_vm_readv(process.pid, &local_iov, 1, &remote_iov, 1, 0);
  if (static_cast<size_t>(result) != size) {
    PLOG(DEBUG) << "ReadRemoteMem(" << " pid " << process.pid << ", addr " << std::hex
                << remote_addr << ", size " << size << ") failed";
    process.died = true;
    return false;
  }
  return true;
}

bool JITDebugReader::ReadDescriptors(Process& process, Descriptor* jit_descriptor,
                                     Descriptor* dex_descriptor) {
  if (!ReadRemoteMem(process, process.descriptors_addr, process.descriptors_size,
                     descriptors_buf_.data())) {
    return false;
  }
  return LoadDescriptor(process.is_64bit, &descriptors_buf_[process.jit_descriptor_offset],
                        jit_descriptor) &&
      LoadDescriptor(process.is_64bit, &descriptors_buf_[process.dex_descriptor_offset],
                     dex_descriptor);
}

bool JITDebugReader::LoadDescriptor(bool is_64bit, const char* data, Descriptor* descriptor) {
  if (is_64bit) {
    return LoadDescriptorImpl<JITDescriptor64, JITCodeEntry64>(data, descriptor);
  }
  return LoadDescriptorImpl<JITDescriptor32, JITCodeEntry32>(data, descriptor);
}

template <typename DescriptorT, typename CodeEntryT>
bool JITDebugReader::LoadDescriptorImpl(const char* data, Descriptor* descriptor) {
  DescriptorT raw_descriptor;
  MoveFromBinaryFormat(raw_descriptor, data);
  if (!raw_descriptor.Valid() || sizeof(raw_descriptor) != raw_descriptor.sizeof_descriptor ||
      sizeof(CodeEntryT) != raw_descriptor.sizeof_entry) {
    return false;
  }
  descriptor->action_seqlock = raw_descriptor.action_seqlock;
  descriptor->action_timestamp = raw_descriptor.action_timestamp;
  descriptor->first_entry_addr = raw_descriptor.first_entry_addr;
  return true;
}

// Read new code entries with timestamp > last_action_timestamp.
// Since we don't stop the app process while reading code entries, it is possible we are reading
// broken data. So return false once we detect that the data is broken.
bool JITDebugReader::ReadNewCodeEntries(Process& process, const Descriptor& descriptor,
                                        uint64_t last_action_timestamp, uint32_t read_entry_limit,
                                        std::vector<CodeEntry>* new_code_entries) {
  if (process.is_64bit) {
    return ReadNewCodeEntriesImpl<JITDescriptor64, JITCodeEntry64>(
        process, descriptor, last_action_timestamp, read_entry_limit, new_code_entries);
  }
  return ReadNewCodeEntriesImpl<JITDescriptor32, JITCodeEntry32>(
      process, descriptor, last_action_timestamp, read_entry_limit, new_code_entries);
}

template <typename DescriptorT, typename CodeEntryT>
bool JITDebugReader::ReadNewCodeEntriesImpl(Process& process, const Descriptor& descriptor,
                                            uint64_t last_action_timestamp,
                                            uint32_t read_entry_limit,
                                            std::vector<CodeEntry>* new_code_entries) {
  uint64_t current_entry_addr = descriptor.first_entry_addr;
  uint64_t prev_entry_addr = 0u;
  std::unordered_set<uint64_t> entry_addr_set;
  for (size_t i = 0u; i < read_entry_limit && current_entry_addr != 0u; ++i) {
    if (entry_addr_set.find(current_entry_addr) != entry_addr_set.end()) {
      // We enter a loop, which means a broken linked list.
      return false;
    }
    CodeEntryT entry;
    if (!ReadRemoteMem(process, current_entry_addr, sizeof(entry), &entry)) {
      return false;
    }
    if (entry.prev_addr != prev_entry_addr || !entry.Valid()) {
      // A broken linked list
      return false;
    }
    if (entry.register_timestamp <= last_action_timestamp) {
      // The linked list has entries with timestamp in decreasing order. So stop searching
      // once we hit an entry with timestamp <= last_action_timestmap.
      break;
    }
    CodeEntry code_entry;
    code_entry.addr = current_entry_addr;
    code_entry.symfile_addr = entry.symfile_addr;
    code_entry.symfile_size = entry.symfile_size;
    code_entry.timestamp = entry.register_timestamp;
    new_code_entries->push_back(code_entry);
    entry_addr_set.insert(current_entry_addr);
    prev_entry_addr = current_entry_addr;
    current_entry_addr = entry.next_addr;
  }
  return true;
}

void JITDebugReader::ReadJITCodeDebugInfo(Process& process,
                                          const std::vector<CodeEntry>& jit_entries,
                                          std::vector<JITDebugInfo>* debug_info) {
  std::vector<char> data;
  for (auto& jit_entry : jit_entries) {
    if (jit_entry.symfile_size > MAX_JIT_SYMFILE_SIZE) {
      continue;
    }
    if (data.size() < jit_entry.symfile_size) {
      data.resize(jit_entry.symfile_size);
    }
    if (!ReadRemoteMem(process, jit_entry.symfile_addr, jit_entry.symfile_size, data.data())) {
      continue;
    }
    if (!IsValidElfFileMagic(data.data(), jit_entry.symfile_size)) {
      continue;
    }
    uint64_t min_addr = UINT64_MAX;
    uint64_t max_addr = 0;
    auto callback = [&](const ElfFileSymbol& symbol) {
      min_addr = std::min(min_addr, symbol.vaddr);
      max_addr = std::max(max_addr, symbol.vaddr + symbol.len);
      LOG(VERBOSE) << "JITSymbol " << symbol.name << " at [" << std::hex << symbol.vaddr
                   << " - " << (symbol.vaddr + symbol.len) << " with size " << symbol.len;
    };
    if (ParseSymbolsFromElfFileInMemory(data.data(), jit_entry.symfile_size, callback) !=
        ElfStatus::NO_ERROR || min_addr >= max_addr) {
      continue;
    }
    std::unique_ptr<TemporaryFile> tmp_file = ScopedTempFiles::CreateTempFile(!keep_symfiles_);
    if (tmp_file == nullptr || !android::base::WriteFully(tmp_file->fd, data.data(),
                                                          jit_entry.symfile_size)) {
      continue;
    }
    if (keep_symfiles_) {
      tmp_file->DoNotRemove();
    }
    debug_info->emplace_back(process.pid, jit_entry.timestamp, min_addr, max_addr - min_addr,
                             tmp_file->path);
  }
}

void JITDebugReader::ReadDexFileDebugInfo(Process& process,
                                          const std::vector<CodeEntry>& dex_entries,
                                          std::vector<JITDebugInfo>* debug_info) {
  std::vector<ThreadMmap> thread_mmaps;
  if (!GetThreadMmapsInProcess(process.pid, &thread_mmaps)) {
    process.died = true;
    return;
  }
  auto comp = [](const ThreadMmap& map, uint64_t addr) {
    return map.start_addr <= addr;
  };
  for (auto& dex_entry : dex_entries) {
    auto it = std::lower_bound(thread_mmaps.begin(), thread_mmaps.end(),
                               dex_entry.symfile_addr, comp);
    if (it == thread_mmaps.begin()) {
      continue;
    }
    --it;
    if (it->start_addr + it->len < dex_entry.symfile_addr + dex_entry.symfile_size) {
      continue;
    }
    std::string file_path;
    std::string zip_path;
    std::string entry_path;
    if (ParseExtractedInMemoryPath(it->name, &zip_path, &entry_path)) {
      file_path = GetUrlInApk(zip_path, entry_path);
    } else {
      if (!IsRegularFile(it->name)) {
        // TODO: read dex file only exist in memory?
        continue;
      }
      file_path = it->name;
    }
    // Offset of dex file in .vdex file or .apk file.
    uint64_t dex_file_offset = dex_entry.symfile_addr - it->start_addr + it->pgoff;
    debug_info->emplace_back(process.pid, dex_entry.timestamp, dex_file_offset, file_path);
    LOG(VERBOSE) << "DexFile " << file_path << "+" << std::hex << dex_file_offset
                 << " in map [" << it->start_addr << " - " << (it->start_addr + it->len)
                 << "] with size " << dex_entry.symfile_size;
  }
}

bool JITDebugReader::AddDebugInfo(const std::vector<JITDebugInfo>& debug_info,
                                    bool sync_kernel_records) {
  if (!debug_info.empty()) {
    if (sync_with_records_) {
      for (auto& info : debug_info) {
        debug_info_q_.push(std::move(info));
      }
    } else {
      return debug_info_callback_(debug_info, sync_kernel_records);
    }
  }
  return true;
}

}  // namespace simpleperf
