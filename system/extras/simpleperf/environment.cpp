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

#include "environment.h"

#include <inttypes.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/utsname.h>

#include <limits>
#include <set>
#include <unordered_map>
#include <vector>

#include <android-base/file.h>
#include <android-base/logging.h>
#include <android-base/parseint.h>
#include <android-base/strings.h>
#include <android-base/stringprintf.h>
#include <procinfo/process.h>
#include <procinfo/process_map.h>

#if defined(__ANDROID__)
#include <android-base/properties.h>
#endif

#include "event_type.h"
#include "IOEventLoop.h"
#include "read_elf.h"
#include "thread_tree.h"
#include "utils.h"
#include "workload.h"

class LineReader {
 public:
  explicit LineReader(FILE* fp) : fp_(fp), buf_(nullptr), bufsize_(0) {
  }

  ~LineReader() {
    free(buf_);
    fclose(fp_);
  }

  char* ReadLine() {
    if (getline(&buf_, &bufsize_, fp_) != -1) {
      return buf_;
    }
    return nullptr;
  }

  size_t MaxLineSize() {
    return bufsize_;
  }

 private:
  FILE* fp_;
  char* buf_;
  size_t bufsize_;
};

std::vector<int> GetOnlineCpus() {
  std::vector<int> result;
  FILE* fp = fopen("/sys/devices/system/cpu/online", "re");
  if (fp == nullptr) {
    PLOG(ERROR) << "can't open online cpu information";
    return result;
  }

  LineReader reader(fp);
  char* line;
  if ((line = reader.ReadLine()) != nullptr) {
    result = GetCpusFromString(line);
  }
  CHECK(!result.empty()) << "can't get online cpu information";
  return result;
}

std::vector<int> GetCpusFromString(const std::string& s) {
  std::set<int> cpu_set;
  bool have_dash = false;
  const char* p = s.c_str();
  char* endp;
  int last_cpu;
  int cpu;
  // Parse line like: 0,1-3, 5, 7-8
  while ((cpu = static_cast<int>(strtol(p, &endp, 10))) != 0 || endp != p) {
    if (have_dash && !cpu_set.empty()) {
      for (int t = last_cpu + 1; t < cpu; ++t) {
        cpu_set.insert(t);
      }
    }
    have_dash = false;
    cpu_set.insert(cpu);
    last_cpu = cpu;
    p = endp;
    while (!isdigit(*p) && *p != '\0') {
      if (*p == '-') {
        have_dash = true;
      }
      ++p;
    }
  }
  return std::vector<int>(cpu_set.begin(), cpu_set.end());
}

static std::vector<KernelMmap> GetLoadedModules() {
  std::vector<KernelMmap> result;
  FILE* fp = fopen("/proc/modules", "re");
  if (fp == nullptr) {
    // There is no /proc/modules on Android devices, so we don't print error if failed to open it.
    PLOG(DEBUG) << "failed to open file /proc/modules";
    return result;
  }
  LineReader reader(fp);
  char* line;
  while ((line = reader.ReadLine()) != nullptr) {
    // Parse line like: nf_defrag_ipv6 34768 1 nf_conntrack_ipv6, Live 0xffffffffa0fe5000
    char name[reader.MaxLineSize()];
    uint64_t addr;
    uint64_t len;
    if (sscanf(line, "%s%" PRIu64 "%*u%*s%*s 0x%" PRIx64, name, &len, &addr) == 3) {
      KernelMmap map;
      map.name = name;
      map.start_addr = addr;
      map.len = len;
      result.push_back(map);
    }
  }
  bool all_zero = true;
  for (const auto& map : result) {
    if (map.start_addr != 0) {
      all_zero = false;
    }
  }
  if (all_zero) {
    LOG(DEBUG) << "addresses in /proc/modules are all zero, so ignore kernel modules";
    return std::vector<KernelMmap>();
  }
  return result;
}

static void GetAllModuleFiles(const std::string& path,
                              std::unordered_map<std::string, std::string>* module_file_map) {
  for (const auto& name : GetEntriesInDir(path)) {
    std::string entry_path = path + "/" + name;
    if (IsRegularFile(entry_path) && android::base::EndsWith(name, ".ko")) {
      std::string module_name = name.substr(0, name.size() - 3);
      std::replace(module_name.begin(), module_name.end(), '-', '_');
      module_file_map->insert(std::make_pair(module_name, entry_path));
    } else if (IsDir(entry_path)) {
      GetAllModuleFiles(entry_path, module_file_map);
    }
  }
}

static std::vector<KernelMmap> GetModulesInUse() {
  std::vector<KernelMmap> module_mmaps = GetLoadedModules();
  if (module_mmaps.empty()) {
    return std::vector<KernelMmap>();
  }
  std::unordered_map<std::string, std::string> module_file_map;
#if defined(__ANDROID__)
  // Search directories listed in "File locations" section in
  // https://source.android.com/devices/architecture/kernel/modular-kernels.
  for (const auto& path : {"/vendor/lib/modules", "/odm/lib/modules", "/lib/modules"}) {
    GetAllModuleFiles(path, &module_file_map);
  }
#else
  utsname uname_buf;
  if (TEMP_FAILURE_RETRY(uname(&uname_buf)) != 0) {
    PLOG(ERROR) << "uname() failed";
    return std::vector<KernelMmap>();
  }
  std::string linux_version = uname_buf.release;
  std::string module_dirpath = "/lib/modules/" + linux_version + "/kernel";
  GetAllModuleFiles(module_dirpath, &module_file_map);
#endif
  for (auto& module : module_mmaps) {
    auto it = module_file_map.find(module.name);
    if (it != module_file_map.end()) {
      module.filepath = it->second;
    }
  }
  return module_mmaps;
}

void GetKernelAndModuleMmaps(KernelMmap* kernel_mmap, std::vector<KernelMmap>* module_mmaps) {
  kernel_mmap->name = DEFAULT_KERNEL_MMAP_NAME;
  kernel_mmap->start_addr = 0;
  kernel_mmap->len = std::numeric_limits<uint64_t>::max();
  kernel_mmap->filepath = kernel_mmap->name;
  *module_mmaps = GetModulesInUse();
  for (auto& map : *module_mmaps) {
    if (map.filepath.empty()) {
      map.filepath = "[" + map.name + "]";
    }
  }
}

static bool ReadThreadNameAndPid(pid_t tid, std::string* comm, pid_t* pid) {
  android::procinfo::ProcessInfo procinfo;
  if (!android::procinfo::GetProcessInfo(tid, &procinfo)) {
    return false;
  }
  if (comm != nullptr) {
    *comm = procinfo.name;
  }
  if (pid != nullptr) {
    *pid = procinfo.pid;
  }
  return true;
}

std::vector<pid_t> GetThreadsInProcess(pid_t pid) {
  std::vector<pid_t> result;
  android::procinfo::GetProcessTids(pid, &result);
  return result;
}

bool IsThreadAlive(pid_t tid) {
  return IsDir(android::base::StringPrintf("/proc/%d", tid));
}

bool GetProcessForThread(pid_t tid, pid_t* pid) {
  return ReadThreadNameAndPid(tid, nullptr, pid);
}

bool GetThreadName(pid_t tid, std::string* name) {
  return ReadThreadNameAndPid(tid, name, nullptr);
}

std::vector<pid_t> GetAllProcesses() {
  std::vector<pid_t> result;
  std::vector<std::string> entries = GetEntriesInDir("/proc");
  for (const auto& entry : entries) {
    pid_t pid;
    if (!android::base::ParseInt(entry.c_str(), &pid, 0)) {
      continue;
    }
    result.push_back(pid);
  }
  return result;
}

bool GetThreadMmapsInProcess(pid_t pid, std::vector<ThreadMmap>* thread_mmaps) {
  thread_mmaps->clear();
  return android::procinfo::ReadProcessMaps(
      pid, [&](uint64_t start, uint64_t end, uint16_t flags, uint64_t pgoff,
               ino_t, const char* name) {
        thread_mmaps->emplace_back(start, end - start, pgoff, name, flags);
      });
}

bool GetKernelBuildId(BuildId* build_id) {
  ElfStatus result = GetBuildIdFromNoteFile("/sys/kernel/notes", build_id);
  if (result != ElfStatus::NO_ERROR) {
    LOG(DEBUG) << "failed to read /sys/kernel/notes: " << result;
  }
  return result == ElfStatus::NO_ERROR;
}

bool GetModuleBuildId(const std::string& module_name, BuildId* build_id) {
  std::string notefile = "/sys/module/" + module_name + "/notes/.note.gnu.build-id";
  return GetBuildIdFromNoteFile(notefile, build_id);
}

bool GetValidThreadsFromThreadString(const std::string& tid_str, std::set<pid_t>* tid_set) {
  std::vector<std::string> strs = android::base::Split(tid_str, ",");
  for (const auto& s : strs) {
    int tid;
    if (!android::base::ParseInt(s.c_str(), &tid, 0)) {
      LOG(ERROR) << "Invalid tid '" << s << "'";
      return false;
    }
    if (!IsDir(android::base::StringPrintf("/proc/%d", tid))) {
      LOG(ERROR) << "Non existing thread '" << tid << "'";
      return false;
    }
    tid_set->insert(tid);
  }
  return true;
}

/*
 * perf event paranoia level:
 *  -1 - not paranoid at all
 *   0 - disallow raw tracepoint access for unpriv
 *   1 - disallow cpu events for unpriv
 *   2 - disallow kernel profiling for unpriv
 *   3 - disallow user profiling for unpriv
 */
static bool ReadPerfEventParanoid(int* value) {
  std::string s;
  if (!android::base::ReadFileToString("/proc/sys/kernel/perf_event_paranoid", &s)) {
    PLOG(DEBUG) << "failed to read /proc/sys/kernel/perf_event_paranoid";
    return false;
  }
  s = android::base::Trim(s);
  if (!android::base::ParseInt(s.c_str(), value)) {
    PLOG(ERROR) << "failed to parse /proc/sys/kernel/perf_event_paranoid: " << s;
    return false;
  }
  return true;
}

bool CanRecordRawData() {
  int value;
  return ReadPerfEventParanoid(&value) && value == -1;
}

static const char* GetLimitLevelDescription(int limit_level) {
  switch (limit_level) {
    case -1: return "unlimited";
    case 0: return "disallowing raw tracepoint access for unpriv";
    case 1: return "disallowing cpu events for unpriv";
    case 2: return "disallowing kernel profiling for unpriv";
    case 3: return "disallowing user profiling for unpriv";
    default: return "unknown level";
  }
}

bool CheckPerfEventLimit() {
  // Root is not limited by /proc/sys/kernel/perf_event_paranoid. However, the monitored threads
  // may create child processes not running as root. To make sure the child processes have
  // enough permission to create inherited tracepoint events, write -1 to perf_event_paranoid.
  // See http://b/62230699.
  if (IsRoot()) {
    char* env = getenv("PERFPROFD_DISABLE_PERF_EVENT_PARANOID_CHANGE");
    if (env != nullptr && strcmp(env, "1") == 0) {
      return true;
    }
    return android::base::WriteStringToFile("-1", "/proc/sys/kernel/perf_event_paranoid");
  }
  int limit_level;
  bool can_read_paranoid = ReadPerfEventParanoid(&limit_level);
  if (can_read_paranoid && limit_level <= 1) {
    return true;
  }
#if defined(__ANDROID__)
  const std::string prop_name = "security.perf_harden";
  std::string prop_value = android::base::GetProperty(prop_name, "");
  if (prop_value.empty()) {
    // can't do anything if there is no such property.
    return true;
  }
  if (prop_value == "0") {
    return true;
  }
  // Try to enable perf_event_paranoid by setprop security.perf_harden=0.
  if (android::base::SetProperty(prop_name, "0")) {
    sleep(1);
    if (can_read_paranoid && ReadPerfEventParanoid(&limit_level) && limit_level <= 1) {
      return true;
    }
    if (android::base::GetProperty(prop_name, "") == "0") {
      return true;
    }
  }
  if (can_read_paranoid) {
    LOG(WARNING) << "/proc/sys/kernel/perf_event_paranoid is " << limit_level
        << ", " << GetLimitLevelDescription(limit_level) << ".";
  }
  LOG(WARNING) << "Try using `adb shell setprop security.perf_harden 0` to allow profiling.";
  return false;
#else
  if (can_read_paranoid) {
    LOG(WARNING) << "/proc/sys/kernel/perf_event_paranoid is " << limit_level
        << ", " << GetLimitLevelDescription(limit_level) << ".";
    return false;
  }
#endif
  return true;
}

#if defined(__ANDROID__)
static bool SetProperty(const char* prop_name, uint64_t value) {
  if (!android::base::SetProperty(prop_name, std::to_string(value))) {
    LOG(ERROR) << "Failed to SetProperty " << prop_name << " to " << value;
    return false;
  }
  return true;
}

bool SetPerfEventLimits(uint64_t sample_freq, size_t cpu_percent, uint64_t mlock_kb) {
  if (!SetProperty("debug.perf_event_max_sample_rate", sample_freq) ||
      !SetProperty("debug.perf_cpu_time_max_percent", cpu_percent) ||
      !SetProperty("debug.perf_event_mlock_kb", mlock_kb) ||
      !SetProperty("security.perf_harden", 0)) {
    return false;
  }
  // Wait for init process to change perf event limits based on properties.
  const size_t max_wait_us = 3 * 1000000;
  int finish_mask = 0;
  for (size_t i = 0; i < max_wait_us && finish_mask != 7; ++i) {
    usleep(1);  // Wait 1us to avoid busy loop.
    if ((finish_mask & 1) == 0) {
      uint64_t freq;
      if (!GetMaxSampleFrequency(&freq) || freq == sample_freq) {
        finish_mask |= 1;
      }
    }
    if ((finish_mask & 2) == 0) {
      size_t percent;
      if (!GetCpuTimeMaxPercent(&percent) || percent == cpu_percent) {
        finish_mask |= 2;
      }
    }
    if ((finish_mask & 4) == 0) {
      uint64_t kb;
      if (!GetPerfEventMlockKb(&kb) || kb == mlock_kb) {
        finish_mask |= 4;
      }
    }
  }
  if (finish_mask != 7) {
    LOG(WARNING) << "Wait setting perf event limits timeout";
  }
  return true;
}
#else  // !defined(__ANDROID__)
bool SetPerfEventLimits(uint64_t, size_t, uint64_t) {
  return true;
}
#endif

template <typename T>
static bool ReadUintFromProcFile(const std::string& path, T* value) {
  std::string s;
  if (!android::base::ReadFileToString(path, &s)) {
    PLOG(DEBUG) << "failed to read " << path;
    return false;
  }
  s = android::base::Trim(s);
  if (!android::base::ParseUint(s.c_str(), value)) {
    LOG(ERROR) << "failed to parse " << path << ": " << s;
    return false;
  }
  return true;
}

template <typename T>
static bool WriteUintToProcFile(const std::string& path, T value) {
  if (IsRoot()) {
    return android::base::WriteStringToFile(std::to_string(value), path);
  }
  return false;
}

bool GetMaxSampleFrequency(uint64_t* max_sample_freq) {
  return ReadUintFromProcFile("/proc/sys/kernel/perf_event_max_sample_rate", max_sample_freq);
}

bool SetMaxSampleFrequency(uint64_t max_sample_freq) {
  return WriteUintToProcFile("/proc/sys/kernel/perf_event_max_sample_rate", max_sample_freq);
}

bool GetCpuTimeMaxPercent(size_t* percent) {
  return ReadUintFromProcFile("/proc/sys/kernel/perf_cpu_time_max_percent", percent);
}

bool SetCpuTimeMaxPercent(size_t percent) {
  return WriteUintToProcFile("/proc/sys/kernel/perf_cpu_time_max_percent", percent);
}

bool GetPerfEventMlockKb(uint64_t* mlock_kb) {
  return ReadUintFromProcFile("/proc/sys/kernel/perf_event_mlock_kb", mlock_kb);
}

bool SetPerfEventMlockKb(uint64_t mlock_kb) {
  return WriteUintToProcFile("/proc/sys/kernel/perf_event_mlock_kb", mlock_kb);
}

bool CheckKernelSymbolAddresses() {
  const std::string kptr_restrict_file = "/proc/sys/kernel/kptr_restrict";
  std::string s;
  if (!android::base::ReadFileToString(kptr_restrict_file, &s)) {
    PLOG(DEBUG) << "failed to read " << kptr_restrict_file;
    return false;
  }
  s = android::base::Trim(s);
  int value;
  if (!android::base::ParseInt(s.c_str(), &value)) {
    LOG(ERROR) << "failed to parse " << kptr_restrict_file << ": " << s;
    return false;
  }
  // Accessible to everyone?
  if (value == 0) {
    return true;
  }
  // Accessible to root?
  if (value == 1 && IsRoot()) {
    return true;
  }
  // Can we make it accessible to us?
  if (IsRoot() && android::base::WriteStringToFile("1", kptr_restrict_file)) {
    return true;
  }
  LOG(WARNING) << "Access to kernel symbol addresses is restricted. If "
      << "possible, please do `echo 0 >/proc/sys/kernel/kptr_restrict` "
      << "to fix this.";
  return false;
}

ArchType GetMachineArch() {
  utsname uname_buf;
  if (TEMP_FAILURE_RETRY(uname(&uname_buf)) != 0) {
    PLOG(WARNING) << "uname() failed";
    return GetBuildArch();
  }
  ArchType arch = GetArchType(uname_buf.machine);
  if (arch != ARCH_UNSUPPORTED) {
    return arch;
  }
  return GetBuildArch();
}

void PrepareVdsoFile() {
  // vdso is an elf file in memory loaded in each process's user space by the kernel. To read
  // symbols from it and unwind through it, we need to dump it into a file in storage.
  // It doesn't affect much when failed to prepare vdso file, so there is no need to return values.
  std::vector<ThreadMmap> thread_mmaps;
  if (!GetThreadMmapsInProcess(getpid(), &thread_mmaps)) {
    return;
  }
  const ThreadMmap* vdso_map = nullptr;
  for (const auto& map : thread_mmaps) {
    if (map.name == "[vdso]") {
      vdso_map = &map;
      break;
    }
  }
  if (vdso_map == nullptr) {
    return;
  }
  std::string s(vdso_map->len, '\0');
  memcpy(&s[0], reinterpret_cast<void*>(static_cast<uintptr_t>(vdso_map->start_addr)),
         vdso_map->len);
  std::unique_ptr<TemporaryFile> tmpfile = ScopedTempFiles::CreateTempFile();
  if (!android::base::WriteStringToFd(s, tmpfile->fd)) {
    return;
  }
  Dso::SetVdsoFile(tmpfile->path, sizeof(size_t) == sizeof(uint64_t));
}

static bool HasOpenedAppApkFile(int pid) {
  std::string fd_path = "/proc/" + std::to_string(pid) + "/fd/";
  std::vector<std::string> files = GetEntriesInDir(fd_path);
  for (const auto& file : files) {
    std::string real_path;
    if (!android::base::Readlink(fd_path + file, &real_path)) {
      continue;
    }
    if (real_path.find("app") != std::string::npos && real_path.find(".apk") != std::string::npos) {
      return true;
    }
  }
  return false;
}

std::set<pid_t> WaitForAppProcesses(const std::string& package_name) {
  std::set<pid_t> result;
  size_t loop_count = 0;
  while (true) {
    std::vector<pid_t> pids = GetAllProcesses();
    for (pid_t pid : pids) {
      std::string cmdline;
      if (!android::base::ReadFileToString("/proc/" + std::to_string(pid) + "/cmdline", &cmdline)) {
        // Maybe we don't have permission to read it.
        continue;
      }
      std::string process_name = android::base::Basename(cmdline);
      // The app may have multiple processes, with process name like
      // com.google.android.googlequicksearchbox:search.
      size_t split_pos = process_name.find(':');
      if (split_pos != std::string::npos) {
        process_name = process_name.substr(0, split_pos);
      }
      if (process_name != package_name) {
        continue;
      }
      // If a debuggable app with wrap.sh runs on Android O, the app will be started with
      // logwrapper as below:
      // 1. Zygote forks a child process, rename it to package_name.
      // 2. The child process execute sh, which starts a child process running
      //    /system/bin/logwrapper.
      // 3. logwrapper starts a child process running sh, which interprets wrap.sh.
      // 4. wrap.sh starts a child process running the app.
      // The problem here is we want to profile the process started in step 4, but sometimes we
      // run into the process started in step 1. To solve it, we can check if the process has
      // opened an apk file in some app dirs.
      if (!HasOpenedAppApkFile(pid)) {
        continue;
      }
      if (loop_count > 0u) {
        LOG(INFO) << "Got process " << pid << " for package " << package_name;
      }
      result.insert(pid);
    }
    if (!result.empty()) {
      return result;
    }
    if (++loop_count == 1u) {
      LOG(INFO) << "Waiting for process of app " << package_name;
    }
    usleep(1000);
  }
}

bool IsAppDebuggable(const std::string& package_name) {
  return Workload::RunCmd({"run-as", package_name, "echo", ">/dev/null", "2>/dev/null"}, false);
}

namespace {

class InAppRunner {
 public:
  InAppRunner(const std::string& package_name) : package_name_(package_name) {}
  virtual ~InAppRunner() {
    if (!tracepoint_file_.empty()) {
      unlink(tracepoint_file_.c_str());
    }
  }
  virtual bool Prepare() = 0;
  bool RunCmdInApp(const std::string& cmd, const std::vector<std::string>& args,
                   size_t workload_args_size, const std::string& output_filepath,
                   bool need_tracepoint_events);
 protected:
  virtual std::vector<std::string> GetPrefixArgs(const std::string& cmd) = 0;

  const std::string package_name_;
  std::string tracepoint_file_;
};

bool InAppRunner::RunCmdInApp(const std::string& cmd, const std::vector<std::string>& cmd_args,
                              size_t workload_args_size, const std::string& output_filepath,
                              bool need_tracepoint_events) {
  // 1. Build cmd args running in app's context.
  std::vector<std::string> args = GetPrefixArgs(cmd);
  args.insert(args.end(), {"--in-app", "--log", GetLogSeverityName()});
  if (need_tracepoint_events) {
    // Since we can't read tracepoint events from tracefs in app's context, we need to prepare
    // them in tracepoint_file in shell's context, and pass the path of tracepoint_file to the
    // child process using --tracepoint-events option.
    const std::string tracepoint_file = "/data/local/tmp/tracepoint_events";
    if (!android::base::WriteStringToFile(GetTracepointEvents(), tracepoint_file)) {
      PLOG(ERROR) << "Failed to store tracepoint events";
      return false;
    }
    tracepoint_file_ = tracepoint_file;
    args.insert(args.end(), {"--tracepoint-events", tracepoint_file_});
  }

  android::base::unique_fd out_fd;
  if (!output_filepath.empty()) {
    // A process running in app's context can't open a file outside it's data directory to write.
    // So pass it a file descriptor to write.
    out_fd = FileHelper::OpenWriteOnly(output_filepath);
    if (out_fd == -1) {
      PLOG(ERROR) << "Failed to open " << output_filepath;
      return false;
    }
    args.insert(args.end(), {"--out-fd", std::to_string(int(out_fd))});
  }

  // We can't send signal to a process running in app's context. So use a pipe file to send stop
  // signal.
  android::base::unique_fd stop_signal_rfd;
  android::base::unique_fd stop_signal_wfd;
  if (!android::base::Pipe(&stop_signal_rfd, &stop_signal_wfd, 0)) {
    PLOG(ERROR) << "pipe";
    return false;
  }
  args.insert(args.end(), {"--stop-signal-fd", std::to_string(int(stop_signal_rfd))});

  for (size_t i = 0; i < cmd_args.size(); ++i) {
    if (i < cmd_args.size() - workload_args_size) {
      // Omit "-o output_file". It is replaced by "--out-fd fd".
      if (cmd_args[i] == "-o" || cmd_args[i] == "--app") {
        i++;
        continue;
      }
    }
    args.push_back(cmd_args[i]);
  }
  char* argv[args.size() + 1];
  for (size_t i = 0; i < args.size(); ++i) {
    argv[i] = &args[i][0];
  }
  argv[args.size()] = nullptr;

  // 2. Run child process in app's context.
  auto ChildProcFn = [&]() {
    stop_signal_wfd.reset();
    execvp(argv[0], argv);
    exit(1);
  };
  std::unique_ptr<Workload> workload = Workload::CreateWorkload(ChildProcFn);
  if (!workload) {
    return false;
  }
  stop_signal_rfd.reset();

  // Wait on signals.
  IOEventLoop loop;
  bool need_to_stop_child = false;
  std::vector<int> stop_signals = {SIGINT, SIGTERM};
  if (!SignalIsIgnored(SIGHUP)) {
    stop_signals.push_back(SIGHUP);
  }
  if (!loop.AddSignalEvents(stop_signals,
                            [&]() { need_to_stop_child = true; return loop.ExitLoop(); })) {
    return false;
  }
  if (!loop.AddSignalEvent(SIGCHLD, [&]() { return loop.ExitLoop(); })) {
    return false;
  }

  if (!workload->Start()) {
    return false;
  }
  if (!loop.RunLoop()) {
    return false;
  }
  if (need_to_stop_child) {
    stop_signal_wfd.reset();
  }
  int exit_code;
  if (!workload->WaitChildProcess(&exit_code) || exit_code != 0) {
    return false;
  }
  return true;
}

class RunAs : public InAppRunner {
 public:
  RunAs(const std::string& package_name) : InAppRunner(package_name) {}
  virtual ~RunAs() {
    if (simpleperf_copied_in_app_) {
      Workload::RunCmd({"run-as", package_name_, "rm", "-rf", "simpleperf"});
    }
  }
  bool Prepare() override;

 protected:
  std::vector<std::string> GetPrefixArgs(const std::string& cmd) {
    return {"run-as", package_name_,
            simpleperf_copied_in_app_ ? "./simpleperf" : simpleperf_path_, cmd,
            "--app", package_name_};
  }

  bool simpleperf_copied_in_app_ = false;
  std::string simpleperf_path_;
};

bool RunAs::Prepare() {
  // Test if run-as can access the package.
  if (!IsAppDebuggable(package_name_)) {
    return false;
  }
  // run-as can't run /data/local/tmp/simpleperf directly. So copy simpleperf binary if needed.
  if (!android::base::Readlink("/proc/self/exe", &simpleperf_path_)) {
    PLOG(ERROR) << "ReadLink failed";
    return false;
  }
  if (simpleperf_path_.find("CtsSimpleperfTest") != std::string::npos) {
    simpleperf_path_ = "/system/bin/simpleperf";
    return true;
  }
  if (android::base::StartsWith(simpleperf_path_, "/system")) {
    return true;
  }
  if (!Workload::RunCmd({"run-as", package_name_, "cp", simpleperf_path_, "simpleperf"})) {
    return false;
  }
  simpleperf_copied_in_app_ = true;
  return true;
}

class SimpleperfAppRunner : public InAppRunner {
 public:
  SimpleperfAppRunner(const std::string& package_name) : InAppRunner(package_name) {}
  bool Prepare() override {
    return GetAndroidVersion() >= kAndroidVersionP + 1;
  }

 protected:
  std::vector<std::string> GetPrefixArgs(const std::string& cmd) {
    return {"simpleperf_app_runner", package_name_, cmd};
  }
};

}  // namespace

bool RunInAppContext(const std::string& app_package_name, const std::string& cmd,
                     const std::vector<std::string>& args, size_t workload_args_size,
                     const std::string& output_filepath, bool need_tracepoint_events) {
  std::unique_ptr<InAppRunner> in_app_runner(new RunAs(app_package_name));
  if (!in_app_runner->Prepare()) {
    in_app_runner.reset(new SimpleperfAppRunner(app_package_name));
    if (!in_app_runner->Prepare()) {
      LOG(ERROR) << "Package " << app_package_name
          << " doesn't exist or isn't debuggable/profileable.";
      return false;
    }
  }
  return in_app_runner->RunCmdInApp(cmd, args, workload_args_size, output_filepath,
                                    need_tracepoint_events);
}

void AllowMoreOpenedFiles() {
  // On Android <= O, the hard limit is 4096, and the soft limit is 1024.
  // On Android >= P, both the hard and soft limit are 32768.
  rlimit limit;
  if (getrlimit(RLIMIT_NOFILE, &limit) == 0) {
    limit.rlim_cur = limit.rlim_max;
    setrlimit(RLIMIT_NOFILE, &limit);
  }
}

std::string ScopedTempFiles::tmp_dir_;
std::vector<std::string> ScopedTempFiles::files_to_delete_;

ScopedTempFiles::ScopedTempFiles(const std::string& tmp_dir) {
  CHECK(tmp_dir_.empty());  // No other ScopedTempFiles.
  tmp_dir_ = tmp_dir;
}

ScopedTempFiles::~ScopedTempFiles() {
  tmp_dir_.clear();
  for (auto& file : files_to_delete_) {
    unlink(file.c_str());
  }
  files_to_delete_.clear();
}

std::unique_ptr<TemporaryFile> ScopedTempFiles::CreateTempFile(bool delete_in_destructor) {
  CHECK(!tmp_dir_.empty());
  std::unique_ptr<TemporaryFile> tmp_file(new TemporaryFile(tmp_dir_));
  CHECK_NE(tmp_file->fd, -1);
  if (delete_in_destructor) {
    tmp_file->DoNotRemove();
    files_to_delete_.push_back(tmp_file->path);
  }
  return tmp_file;
}

bool SignalIsIgnored(int signo) {
  struct sigaction act;
  if (sigaction(signo, nullptr, &act) != 0) {
    PLOG(FATAL) << "failed to query signal handler for signal " << signo;
  }

  if ((act.sa_flags & SA_SIGINFO)) {
    return false;
  }

  return act.sa_handler == SIG_IGN;
}

int GetAndroidVersion() {
#if defined(__ANDROID__)
  static int android_version = -1;
  if (android_version == -1) {
    android_version = 0;
    std::string s = android::base::GetProperty("ro.build.version.release", "");
    // The release string can be a list of numbers (like 8.1.0), a character (like Q)
    // or many characters (like OMR1).
    if (!s.empty()) {
      // Each Android version has a version number: L is 5, M is 6, N is 7, O is 8, etc.
      if (s[0] >= 'A' && s[0] <= 'Z') {
        android_version = s[0] - 'P' + kAndroidVersionP;
      } else if (isdigit(s[0])) {
        sscanf(s.c_str(), "%d", &android_version);
      }
    }
  }
  return android_version;
#else  // defined(__ANDROID__)
  return 0;
#endif
}

std::string GetHardwareFromCpuInfo(const std::string& cpu_info) {
  for (auto& line : android::base::Split(cpu_info, "\n")) {
    size_t pos = line.find(':');
    if (pos != std::string::npos) {
      std::string key = android::base::Trim(line.substr(0, pos));
      if (key == "Hardware") {
        return android::base::Trim(line.substr(pos + 1));
      }
    }
  }
  return "";
}

bool MappedFileOnlyExistInMemory(const char* filename) {
  // Mapped files only existing in memory:
  //   empty name
  //   [anon:???]
  //   [stack]
  //   /dev/*
  //   //anon: generated by kernel/events/core.c.
  //   /memfd: created by memfd_create.
  return filename[0] == '\0' ||
           (filename[0] == '[' && strcmp(filename, "[vdso]") != 0) ||
            strncmp(filename, "//", 2) == 0 ||
            strncmp(filename, "/dev/", 5) == 0 ||
            strncmp(filename, "/memfd:", 7) == 0;
}

std::string GetCompleteProcessName(pid_t pid) {
  std::string s;
  if (!android::base::ReadFileToString(android::base::StringPrintf("/proc/%d/cmdline", pid), &s)) {
    s.clear();
  }
  for (size_t i = 0; i < s.size(); ++i) {
    // /proc/pid/cmdline uses 0 to separate arguments.
    if (isspace(s[i]) || s[i] == 0) {
      s.resize(i);
      break;
    }
  }
  return s;
}
