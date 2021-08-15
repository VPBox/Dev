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

#ifndef SIMPLE_PERF_JIT_DEBUG_READER_H_
#define SIMPLE_PERF_JIT_DEBUG_READER_H_

#include <unistd.h>

#include <functional>
#include <memory>
#include <queue>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <android-base/file.h>
#include <android-base/logging.h>

#include "IOEventLoop.h"
#include "record.h"

namespace simpleperf {

// JITDebugInfo represents the debug info of a JITed Java method or a dex file.
struct JITDebugInfo {
  enum {
    JIT_DEBUG_JIT_CODE,
    JIT_DEBUG_DEX_FILE,
  } type;
  pid_t pid;           // Process of the debug info
  uint64_t timestamp;  // Monotonic timestamp for the creation of the debug info
  union {
    struct {
      uint64_t jit_code_addr;  // The start addr of the JITed code
      uint64_t jit_code_len;   // The end addr of the JITed code
    };
    uint64_t dex_file_offset;  // The offset of the dex file in the file containing it
  };
  // For JITed code, it is the path of a temporary ELF file storing its debug info.
  // For dex file, it is the path of the file containing the dex file.
  std::string file_path;

  JITDebugInfo(pid_t pid, uint64_t timestamp, uint64_t jit_code_addr, uint64_t jit_code_len,
               const std::string& file_path)
      : type(JIT_DEBUG_JIT_CODE), pid(pid), timestamp(timestamp), jit_code_addr(jit_code_addr),
        jit_code_len(jit_code_len), file_path(file_path) {}

  JITDebugInfo(pid_t pid, uint64_t timestamp, uint64_t dex_file_offset,
               const std::string& file_path)
      : type(JIT_DEBUG_DEX_FILE), pid(pid), timestamp(timestamp), dex_file_offset(dex_file_offset),
        file_path(file_path) {}

  bool operator>(const JITDebugInfo& other) const {
    return timestamp > other.timestamp;
  }
};

// JITDebugReader reads debug info of JIT code and dex files of processes using ART. The
// corresponding debug interface in ART is at art/runtime/jit/debugger_interface.cc.
class JITDebugReader {
 public:
  // keep_symfiles: whether to keep dumped JIT debug info files after recording. Usually they
  //                are only kept for debug unwinding.
  // sync_with_records: If true, sync debug info with records based on monotonic timestamp.
  //                    Otherwise, save debug info whenever they are added.
  JITDebugReader(bool keep_symfiles, bool sync_with_records)
      : keep_symfiles_(keep_symfiles), sync_with_records_(sync_with_records) {}

  bool SyncWithRecords() const {
    return sync_with_records_;
  }

  typedef std::function<bool(const std::vector<JITDebugInfo>&, bool)> debug_info_callback_t;
  bool RegisterDebugInfoCallback(IOEventLoop* loop, const debug_info_callback_t& callback);

  // There are two ways to select which processes to monitor. One is using MonitorProcess(), the
  // other is finding all processes having libart.so using records.
  bool MonitorProcess(pid_t pid);
  bool UpdateRecord(const Record* record);

  // Read new debug info from all monitored processes.
  bool ReadAllProcesses();
  // Read new debug info from one process.
  bool ReadProcess(pid_t pid);

  // Flush all debug info registered before timestamp.
  bool FlushDebugInfo(uint64_t timestamp);

 private:

  // An arch-independent representation of JIT/dex debug descriptor.
  struct Descriptor {
    uint32_t action_seqlock = 0;  // incremented before and after any modification
    uint64_t action_timestamp = 0;  // CLOCK_MONOTONIC time of last action
    uint64_t first_entry_addr = 0;
  };

  // An arch-independent representation of JIT/dex code entry.
  struct CodeEntry {
    uint64_t addr;
    uint64_t symfile_addr;
    uint64_t symfile_size;
    uint64_t timestamp;  // CLOCK_MONOTONIC time of last action
  };

  struct Process {
    pid_t pid = -1;
    bool initialized = false;
    bool died = false;
    bool is_64bit = false;
    // The jit descriptor and dex descriptor can be read in one process_vm_readv() call.
    uint64_t descriptors_addr = 0;
    uint64_t descriptors_size = 0;
    // offset relative to descriptors_addr
    uint64_t jit_descriptor_offset = 0;
    // offset relative to descriptors_addr
    uint64_t dex_descriptor_offset = 0;

    // The state we know about the remote jit debug descriptor.
    Descriptor last_jit_descriptor;
    // The state we know about the remote dex debug descriptor.
    Descriptor last_dex_descriptor;
  };

  // The location of descriptors in libart.so.
  struct DescriptorsLocation {
    uint64_t relative_addr = 0;
    uint64_t size = 0;
    uint64_t jit_descriptor_offset = 0;
    uint64_t dex_descriptor_offset = 0;
  };

  void ReadProcess(Process& process, std::vector<JITDebugInfo>* debug_info);
  bool InitializeProcess(Process& process);
  const DescriptorsLocation* GetDescriptorsLocation(const std::string& art_lib_path,
                                                    bool is_64bit);
  bool ReadRemoteMem(Process& process, uint64_t remote_addr, uint64_t size, void* data);
  bool ReadDescriptors(Process& process, Descriptor* jit_descriptor, Descriptor* dex_descriptor);
  bool LoadDescriptor(bool is_64bit, const char* data, Descriptor* descriptor);
  template <typename DescriptorT, typename CodeEntryT>
  bool LoadDescriptorImpl(const char* data, Descriptor* descriptor);

  bool ReadNewCodeEntries(Process& process, const Descriptor& descriptor,
                          uint64_t last_action_timestamp, uint32_t read_entry_limit,
                          std::vector<CodeEntry>* new_code_entries);
  template <typename DescriptorT, typename CodeEntryT>
  bool ReadNewCodeEntriesImpl(Process& process, const Descriptor& descriptor,
                              uint64_t last_action_timestamp, uint32_t read_entry_limit,
                              std::vector<CodeEntry>* new_code_entries);

  void ReadJITCodeDebugInfo(Process& process, const std::vector<CodeEntry>& jit_entries,
                       std::vector<JITDebugInfo>* debug_info);
  void ReadDexFileDebugInfo(Process& process, const std::vector<CodeEntry>& dex_entries,
                       std::vector<JITDebugInfo>* debug_info);
  bool AddDebugInfo(const std::vector<JITDebugInfo>& jit_symfiles, bool sync_kernel_records);

  bool keep_symfiles_ = false;
  bool sync_with_records_ = false;
  IOEventRef read_event_ = nullptr;
  debug_info_callback_t debug_info_callback_;

  // Keys are pids of processes having libart.so, values show whether a process has been monitored.
  std::unordered_map<pid_t, bool> pids_with_art_lib_;

  // All monitored processes
  std::unordered_map<pid_t, Process> processes_;
  std::unordered_map<std::string, DescriptorsLocation> descriptors_location_cache_;
  std::vector<char> descriptors_buf_;

  std::priority_queue<JITDebugInfo, std::vector<JITDebugInfo>, std::greater<JITDebugInfo>>
      debug_info_q_;
};

}  //namespace simpleperf

#endif   // SIMPLE_PERF_JIT_DEBUG_READER_H_
