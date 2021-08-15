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

#ifndef SIMPLE_PERF_ENVIRONMENT_H_
#define SIMPLE_PERF_ENVIRONMENT_H_

#include <sys/types.h>
#include <time.h>

#if defined(__linux__)
#include <sys/syscall.h>
#include <unistd.h>
#endif

#include <functional>
#include <set>
#include <string>
#include <vector>

#include <android-base/file.h>

#include "build_id.h"
#include "perf_regs.h"

std::vector<int> GetOnlineCpus();
std::vector<int> GetCpusFromString(const std::string& s);

struct KernelMmap {
  std::string name;
  uint64_t start_addr;
  uint64_t len;
  std::string filepath;
};

void GetKernelAndModuleMmaps(KernelMmap* kernel_mmap, std::vector<KernelMmap>* module_mmaps);

struct ThreadMmap {
  uint64_t start_addr;
  uint64_t len;
  uint64_t pgoff;
  std::string name;
  uint32_t prot;
  ThreadMmap() {}
  ThreadMmap(uint64_t start, uint64_t len, uint64_t pgoff, const char* name, uint32_t prot)
      : start_addr(start), len(len), pgoff(pgoff), name(name), prot(prot) {}
};

bool GetThreadMmapsInProcess(pid_t pid, std::vector<ThreadMmap>* thread_mmaps);

constexpr char DEFAULT_KERNEL_FILENAME_FOR_BUILD_ID[] = "[kernel.kallsyms]";

bool GetKernelBuildId(BuildId* build_id);
bool GetModuleBuildId(const std::string& module_name, BuildId* build_id);

bool IsThreadAlive(pid_t tid);
std::vector<pid_t> GetAllProcesses();
std::vector<pid_t> GetThreadsInProcess(pid_t pid);
bool GetProcessForThread(pid_t tid, pid_t* pid);
bool GetThreadName(pid_t tid, std::string* name);

bool GetValidThreadsFromThreadString(const std::string& tid_str, std::set<pid_t>* tid_set);

bool CheckPerfEventLimit();
bool SetPerfEventLimits(uint64_t sample_freq, size_t cpu_percent, uint64_t mlock_kb);
bool GetMaxSampleFrequency(uint64_t* max_sample_freq);
bool SetMaxSampleFrequency(uint64_t max_sample_freq);
bool GetCpuTimeMaxPercent(size_t* percent);
bool SetCpuTimeMaxPercent(size_t percent);
bool GetPerfEventMlockKb(uint64_t* mlock_kb);
bool SetPerfEventMlockKb(uint64_t mlock_kb);
bool CheckKernelSymbolAddresses();
bool CanRecordRawData();

#if defined(__linux__)
static inline uint64_t GetSystemClock() {
  timespec ts;
  // Assume clock_gettime() doesn't fail.
  clock_gettime(CLOCK_MONOTONIC, &ts);
  return ts.tv_sec * 1000000000ULL + ts.tv_nsec;
}

#if !defined(__ANDROID__)
static inline int gettid() {
  return syscall(__NR_gettid);
}
#endif
#endif

ArchType GetMachineArch();
void PrepareVdsoFile();

std::set<pid_t> WaitForAppProcesses(const std::string& package_name);
bool IsAppDebuggable(const std::string& package_name);
bool RunInAppContext(const std::string& app_package_name, const std::string& cmd,
                     const std::vector<std::string>& args, size_t workload_args_size,
                     const std::string& output_filepath, bool need_tracepoint_events);

void AllowMoreOpenedFiles();

class ScopedTempFiles {
 public:
  ScopedTempFiles(const std::string& tmp_dir);
  ~ScopedTempFiles();
  // If delete_in_destructor = true, the temp file will be deleted in the destructor of
  // ScopedTempFile. Otherwise, it should be deleted by the caller.
  static std::unique_ptr<TemporaryFile> CreateTempFile(bool delete_in_destructor = true);

 private:
  static std::string tmp_dir_;
  static std::vector<std::string> files_to_delete_;
};

bool SignalIsIgnored(int signo);
// Return 0 if no android version.
int GetAndroidVersion();

constexpr int kAndroidVersionP = 9;

std::string GetHardwareFromCpuInfo(const std::string& cpu_info);

bool MappedFileOnlyExistInMemory(const char* filename);

std::string GetCompleteProcessName(pid_t pid);

#endif  // SIMPLE_PERF_ENVIRONMENT_H_
